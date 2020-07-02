1;
pkg load control;
clear all;
close all;

m1 = 0.763;
m2 = 0.357;                    
L1 = 0.07683;
L2 = 0.14853;
I1 = 0.0021;
I2 = 0.00082;
g = 9.81;
Ng = 30;
Kt = 0.0277;
Ke = 0.012732;
Rm = 13.3;

a = (m1*(L1**2)) + (m2*(L2**2)) + I1;
b = (m1*L1 + m2*L2)*g;
a21 = b/a;
a24 = (Kt*Ke*(Ng**2))/(a*Rm);
a41 = -b/a;
a44 = -((a+I2)/a*I2)*((Kt*Ke*(Ng**2))/Rm);
b2 = -(Kt*Ng)/(a*Rm);
b4 = ((a+I2)*Kt*Ng)/(a*I2*Rm);

A = [0 1 0 0;                           % state matrix
     b/a 0 0 0;
     0 0 0 1;
     -b/a 0 0 0];
     
B = [0;-1/a;0;(a+I2)/(a*I2)];            % input matrix
C = eye(4);                             % output matrix
D = [0;0;0;0];                          % feed-forward matrix
Q = [100 0 0 0;
     0 80 0 0;                          % Q matrix of system
     0 0 1 0;
     0 0 0 1];
R = 1;                                   % R parameter
%K = lqr(A,B,Q,R)                        % lqr function 

Ts = 1/200;
sys_s = ss(A,B,C,D);
sys_d = c2d(sys_s,Ts,'zoh');
 
A_d = sys_d.A;
B_d = sys_d.B;
Cc = [sys_d.C];                % Cc is the discrete C matrix
Dc = [sys_d.D];                % Dc is the discrete D matrix
K = dlqr(A_d,B_d,Q,R)