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
     0 50 0 0;                          % Q matrix of system
     0 0 0.1 0;
     0 0 0 1];
R = 1;                                 % R parameter
%K = lqr(A,B,Q,R)                        % lqr function 

Ts = 1/200;
sys_s = ss(A,B,C,D);
sys_d = c2d(sys_s,Ts,'zoh');
 
A_d = sys_d.A;
B_d = sys_d.B;
Cc = [sys_d.C];                % Cc is the discrete C matrix
Dc = [sys_d.D];                % Dc is the discrete D matrix
K = dlqr(A_d,B_d,Q,R)
        % dlqr function

 x_initial = [0.087,0,0,0]; % initial point
 x_set = [0;0;0;0];             % end point
 sys_cl = ss((A_d-B_d*K),B_d,Cc,Dc,Ts);   
 t = 0:0.01:20;
 [y,t,x] = initial(sys_cl,x_initial,t);
display(y);
for i= 1:size(t)(1)
  u(i) = -K*x(i,:)';
endfor


U=0;
y1=[0;0;0;0];
y0=[31.4;0;0;0];
cost = 0;
for i = 1:100
   cost = cost +  (y1'*Q*y1 +U'*R*U);
   y_new = (A-B*K)*y1;
   y1 = y_new;
   U = -K*(y1-y0);
endfor
display(K);
figure;
subplot(3,2,1);
plot(t,y(:,1));
title('Theta');
 
subplot(3,2,2);
plot(t,y(:,2));
title('theta dot');
 
 
subplot(3,2,3);
plot(t,y(:,3));
title('phi');
 
 
subplot(3,2,4);
plot(t,y(:,4));
title('phi dot');
 
subplot(3,2,5);
hold on;
plot(t,u');
plot(t,300*ones(size(t)));
plot(t,-300*ones(size(t)));
hold off;
title('effort');      
          
