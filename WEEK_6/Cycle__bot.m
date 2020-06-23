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
a = (m1*(L1**2)) + (m2*(L2**2)) + I1;
b = (m1*L1 + m2*L2)*g;

A = [0 1 0 0;                           % state matrix
     b/a 0 0 0;
     0 0 0 1;
     -b/a 0 0 0];
B = [0;-1/a;0;(a+I2)/(a*I2)];            % input matrix
C = eye(4);                             % output matrix
D = [0;0;0;0];                          % feed-forward matrix
Q = [1000 0 0 0;
     0 10 0 0;                          % Q matrix of system
     0 0 0.1 0;
     0 0 0 1];
R = 0.001;                                 % R parameter
K = lqr(A,B,Q,R)                        % lqr function 

Ts = 1/100;
sys_s = ss(A,B,C,D);
sys_d = c2d(sys_s,Ts,'zoh');
 
A_d = sys_d.A;
B_d = sys_d.B;

K = dlqr(A_d,B_d,Q,R)        % dlqr function
 

