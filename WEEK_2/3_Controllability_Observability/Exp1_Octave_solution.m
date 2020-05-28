1;
pkg load control

##**************************************************************************
##*                            E-YSIP(2019-2020)
##*                            =================
##* This experiment is designed for the better understanding of concepts of
##* control systems.
##* Project ID: 17
##* Project Name: Model based robot design
##* Date: May 27, 2020
##* Author: Shreya Rastogi, Project Intern
##*
##**************************************************************************


## Function : definition()
## ----------------------------------------------------
## Input:   A - State Matrix
##          B - Input Matrix
##          C - output matrix
## Purpose: defines the A, B and C matrices.

function [A,B,C] = definition()
   A = [0	1	0	0;
       0	0	-1.96	0;
       0	0	0	1;
       0	0	5.88	0];
       
  B = [0; 0.2; 0; -0.1];
  
  C = [1 0 0 0;
       0 1 0 0;
       0 0 1 0;
       0 0 0 1];
endfunction


## Function : Controllability()
## ----------------------------------------------------
## Input:   A - State Matrix
##          B - Input Matrix
##
## Output:  r - rank of controllability matrix.
##          CO - controllability matrix
## Purpose: Checks whether a system is controllable or not.


function [CO,r] = Controllability(A,B)
  
  CO = ctrb(A,B)         #calculate the controllability matrix using the relevant function
  r  =  rank(CO)        #find out the rank of contrallability matrix
  
  if((length(A)-r)== 0)          #write the necessary condition to check whether the system is controllable or not
   disp(["The system is controllable"])
  endif
  
endfunction


## Function : Observability()
## ----------------------------------------------------
## Input:   A - State Matrix
##          C - Output Matrix
##
## Output:  r - rank of controllability matrix.
##          Ob - observability matrix
## Purpose: Checks whether a system is observable or not.


function [Ob,r] = Observability(A,C)
  
  Ob = obsv(A,C)         #calculate the observability matrix using the relevant function
  r  = rank(Ob)         #find out the rank of observability matrix
  
  if((length(A)-r)== 0)          #write the necessary condition to check whether the system is observable or not
   disp(["The system is observable"])
  endif
  
endfunction


## Function : control_systems_main()
## ----------------------------------------------------
## Purpose: All the functions are called in the main function.


function control_systems_main()
  
   [A,B,C] = definition();        #function call for A,B and C matrices
   [CO,r]  = Controllability(A,B);                    #call the relevant function to check controllability
   [Ob,r]  = Observability(A,C);                    #call the relevant function to check controllability
endfunction

control_systems_main()            #to automatically call the main function after run
