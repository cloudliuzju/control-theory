
%     This file is part of CasADi.
%
%     CasADi -- A symbolic framework for dynamic optimization.
%     Copyright (C) 2010-2014 Joel Andersson, Joris Gillis, Moritz Diehl,
%                             K.U. Leuven. All rights reserved.
%     Copyright (C) 2011-2014 Greg Horn
%
%     CasADi is free software; you can redistribute it and/or
%     modify it under the terms of the GNU Lesser General Public
%     License as published by the Free Software Foundation; either
%     version 3 of the License, or (at your option) any later version.
%
%     CasADi is distributed in the hope that it will be useful,
%     but WITHOUT ANY WARRANTY; without even the implied warranty of
%     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%     Lesser General Public License for more details.
%
%     You should have received a copy of the GNU Lesser General Public
%     License along with CasADi; if not, write to the Free Software
%     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
%

% An implementation of direct collocation
% Joel Andersson, 2016

import casadi.*

Coll          = struct;
% Degree of interpolating polynomial
Coll.d = 3;

% Get collocation points
Coll.tau_root = [0 collocation_points(Coll.d, 'legendre')];

% Coefficients of the collocation equation
Coll.C = zeros(Coll.d+1,Coll.d+1);

% Coefficients of the continuity equation
Coll.D = zeros(Coll.d+1, 1);

% Coefficients of the quadrature function
Coll.B = zeros(Coll.d+1, 1);

% Construct polynomial basis
for j=1:Coll.d+1
  % Construct Lagrange polynomials to get the polynomial basis at the collocation point
  coeff = 1;
  for r=1:Coll.d+1
    if r ~= j
      coeff = conv(coeff, [1, -Coll.tau_root(r)]);
      coeff = coeff / (Coll.tau_root(j)-Coll.tau_root(r));
    end
  end
  % Evaluate the polynomial at the final time to get the coefficients of the continuity equation
  Coll.D(j) = polyval(coeff, 1.0);

  % Evaluate the time derivative of the polynomial at all collocation points to get the coefficients of the continuity equation
  pder = polyder(coeff);
  for r=1:Coll.d+1
    Coll.C(j,r) = polyval(pder, Coll.tau_root(r));
  end

  % Evaluate the integral of the polynomial to get the coefficients of the quadrature function
  pint = polyint(coeff);
  Coll.B(j) = polyval(pint, 1.0);
end

clear j r
%% Define Model and Cost ==================================================
nx   = 2;
nu   = 1;
x    = MX.sym('x',nx);
u    = MX.sym('u',nu);
xdot = [(1-x(2)^2)*x(1) - x(2) + u; x(1)];       % Model equations
L    = x(1)^2 + x(2)^2 + u^2;                    % Objective term
f    = Function('f', {x, u}, {xdot, L}); 

%% Control discretization =================================================
info.T   = 10;                     % Time horizon
info.N   = 20;                     % number of control intervals
info.h   = info.T/info.N;          % Step Size
x0       = [-0.1;0.5];             % inital condition

%% Build NLP ==============================================================
% Start with an empty NLP
nlp     = struct;
nlp.w   = {};
nlp.w0  = [];
nlp.lbw = [];
nlp.ubw = [];
nlp.J   = 0;
nlp.g   = {};
nlp.lbg = [];
nlp.ubg = [];

% define bounds on state and input
nlp.lb_states = [-0.25; -inf];
nlp.ub_states = [ 0.25;  inf];

nlp.lb_input  = [-1];
nlp.ub_input  = [ 1];

% "Lift" initial conditions
nlp.X0  = MX.sym('X0', nx);
nlp.w   = {nlp.w{:}, nlp.X0};
nlp.lbw = [nlp.lbw ; x0];
nlp.ubw = [nlp.ubw ; x0];
nlp.w0  = [nlp.w0  ; x0];

% Formulate the NLP
nlp.Xk = nlp.X0;

for k = 0:info.N-1
    % New NLP variable for the control
    nlp.Uk  = MX.sym(['U_' num2str(k)],nu);
    nlp.w   = {nlp.w{:}, nlp.Uk};
    
    %% enforce bounds on control |u|<1 ------------------------------------
    nlp.lbw = [nlp.lbw; nlp.lb_input];
    nlp.ubw = [nlp.ubw; nlp.ub_input];
    
    % define some initial guess for control -------------------------------
    nlp.w0  = [nlp.w0;  zeros(nu,1)];

    % State at collocation points
    nlp.Xkj = {};
    for j=1:Coll.d
        nlp.Xkj{j} = MX.sym(['X_' num2str(k) '_' num2str(j)], nx);
        nlp.w      = {nlp.w{:}, nlp.Xkj{j}};
        % enforce state constraints   
        nlp.lbw    = [nlp.lbw; nlp.lb_states];
        nlp.ubw    = [nlp.ubw; nlp.ub_states];
        nlp.w0     = [nlp.w0 ; zeros(nx,1)];
    end
        
    % Loop over collocation points
    nlp.Xk_endj = Coll.D(1)*nlp.Xk;
    for j=1:Coll.d
       % Expression for the state derivative at the collocation point
       xp = Coll.C(1,j+1)*nlp.Xk;
       for r=1:Coll.d
           xp = xp + Coll.C(r+1,j+1)*nlp.Xkj{r};
       end
      
       % Append collocation equations
       [fj, qj] = f(nlp.Xkj{j},nlp.Uk);
       nlp.g    = {nlp.g{:}, info.h*fj - xp};
       nlp.lbg  = [nlp.lbg; zeros(nx,1)];
       nlp.ubg  = [nlp.ubg; zeros(nx,1)];
       
       % Add contribution to the end state
       nlp.Xk_endj = nlp.Xk_endj + Coll.D(j+1)*nlp.Xkj{j};
  
       % Add contribution to quadrature function
       nlp.J = nlp.J + Coll.B(j+1)*qj*info.h;
    end    
   
    % New NLP variable for state at end of interval
    nlp.Xk  = MX.sym(['X_' num2str(k+1)], nx);
    nlp.w   = {nlp.w{:}, nlp.Xk};
    nlp.lbw = [nlp.lbw; nlp.lb_states];
    nlp.ubw = [nlp.ubw; nlp.ub_states];
    nlp.w0  = [nlp.w0 ; zeros(nx,1)  ];

    % Add equality constraint
    nlp.g   = {nlp.g{:}, nlp.Xk_endj-nlp.Xk};
    nlp.lbg = [nlp.lbg; zeros(nx,1)];
    nlp.ubg = [nlp.ubg; zeros(nx,1)];            
end

% Create an NLP solver
prob   = struct('f', nlp.J, 'x', vertcat(nlp.w{:}), 'g', vertcat(nlp.g{:}));
% option IPOPT
% see: http://www.coin-or.org/Ipopt/documentation/node39.html
opts                = struct;
opts.ipopt.max_iter = 50;
%% Solve the NLP
solver   = nlpsol('solver', 'ipopt', prob,opts);
solution = solver('x0', nlp.w0, 'lbx', nlp.lbw, 'ubx', nlp.ubw,'lbg', nlp.lbg, 'ubg', nlp.ubg);
w_opt    = full(solution.x);

% Plot the solution
x1_opt = w_opt(1:(nx+nu)+2*Coll.d:end);
x2_opt = w_opt(2:(nx+nu)+2*Coll.d:end);
u_opt  = w_opt(3:(nx+nu)+2*Coll.d:end);
tgrid  = linspace(0, info.T, info.N+1);
clf;
figure(1);
hold on
plot(tgrid, x1_opt, '--')
plot(tgrid, x2_opt, '-')
stairs(tgrid, [u_opt; nan], '-.')
xlabel('t')
legend('x1','x2','u')

% Inspect Jacobian sparsity
nlp.g        = vertcat(nlp.g{:});
nlp.w        = vertcat(nlp.w{:});
nlp.Jacobian = jacobian(nlp.g, nlp.w);
figure(2)
spy(sparse(DM.ones(nlp.Jacobian.sparsity())),'r')

% Inspect Hessian of the Lagrangian sparsity
nlp.Lambda     = MX.sym('lam', nlp.g.sparsity());
nlp.Lagrancian = nlp.J + dot(nlp.Lambda, nlp.g);
nlp.Hessian    = hessian(nlp.Lagrancian, nlp.w);
figure(3)
spy(sparse(DM.ones(nlp.Hessian.sparsity())),'r')