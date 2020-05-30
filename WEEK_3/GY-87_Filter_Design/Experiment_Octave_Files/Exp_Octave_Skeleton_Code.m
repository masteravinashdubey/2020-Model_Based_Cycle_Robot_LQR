clc;
close all;
clear all;

global A = csvread('data.csv');  #do not change this line

################################################
#######Declare your global variables here#######
################################################


function accel_data(axl,axh,ayl,ayh,azl,azh)  
  
####Code to combine the HIGH and LOW values from ACCELEROMETER #####
  
  
####Call function lowpass(ax,ay,az,f_cut) here####

endfunction

function gyro_data(gxl,gxh,gyl,gyh,gzl,gzh)
  
######Code to combine the HIGH and LOW values from GYROSCOPE #######

####Call function highpass(ax,ay,az,f_cut) here####

endfunction



function lowpass(ax,ay,az,f_cut)
  dT = ;  #time in seconds
  Tau= ;
  alpha = Tau/(Tau+dT);                #do not change this line
  
  ##############Write your code here##############

  
endfunction



function highpass(gx,gy,gz,f_cut)
  dT = ;  #time in seconds
  Tau= ;
  alpha = Tau/(Tau+dT);                #do not change this line
  
  ##############Write your code here##############
  
endfunction

function comp_filter_pitch(ax,ay,az,gx,gy,gz)

#######Calculate PITCH using complementry filter ######  
 
endfunction 

function comp_filter_roll(ax,ay,az,gx,gy,gz)

#######Calculate ROLL using complementry filter ####### 
 
endfunction 

function run
  

  for n = 1:rows(A)                    #do not change this line
    
####Call comp_filter_pitch and comp_filter_roll functions for the entire data.csv here####
    
  endfor
  csvwrite('output.csv',B);        #do not change this line
endfunction


run                           #do not change this line
