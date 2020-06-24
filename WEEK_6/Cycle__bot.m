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
     a21 0 0 a24;
     0 0 0 1;
     a41 0 0 a44];
     
B = [0;b2;0;b4];            % input matrix
C = eye(4);                             % output matrix
D = [0;0;0;0];                          % feed-forward matrix
Q = [100 0 0 0;
     0 10 0 0;                          % Q matrix of system
     0 0 0.1 0;
     0 0 0 0.05];
R = 0.1;                                 % R parameter
%K = lqr(A,B,Q,R)                        % lqr function 

Ts = 1/100;
sys_s = ss(A,B,C,D);
sys_d = c2d(sys_s,Ts,'zoh');
 
A_d = sys_d.A;
B_d = sys_d.B;

K = dlqr(A_d,B_d,Q,R)        % dlqr function
 

