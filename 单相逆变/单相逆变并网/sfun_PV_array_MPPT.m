function [sys,x0,str,ts] = sfun_PV_array_MPPT(t,x,u,flag)
switch flag,
  case 0,
    [sys,x0,str,ts] = mdlInitializeSizes;
  case 2,                                                
    sys = mdlUpdate(t,x,u); 
  case 3,                                                
    sys = mdlOutputs(t,x,u);
  case {1,4,9},                                                
    sys =[];
  otherwise
    error(['unhandled flag = ',num2str(flag)]);
end

function [sys,x0,str,ts] = mdlInitializeSizes
sizes = simsizes;
sizes.NumContStates  = 0;
sizes.NumDiscStates  = 3;
sizes.NumOutputs     = 1;
sizes.NumInputs      = 2;
sizes.DirFeedthrough = 1;
sizes.NumSampleTimes = 1;
sys = simsizes(sizes);
x0  =[0,0,0];
str =[];
ts  =[1e-6 0]; 

function sys = mdlUpdate(t,x,u)
k1=0.01;
dV=u(1)-x(1);
dI=u(2)-x(2); 
if dV==0
    if dI==0
    Vref = x(3);
    elseif dI > 0
    Vref = x(3)+k1;            
    elseif  dI < 0
    Vref = x(3)-k1;  
    end
else
    if  (dI/dV) ==(-u(2)/u(1))
        Vref = x(3);    
    elseif (dI/dV) >(-u(2)/u(1))
        Vref = x(3)+k1;   
    elseif  (dI/dV) <(-u(2)/u(1))
        Vref = x(3)-k1; 
    end
end
x(1)=u(1);
x(2)=u(2);
x(3)=Vref;
sys=[x(1),x(2),x(3)];

function sys = mdlOutputs(t,x,u)

sys=x(3);