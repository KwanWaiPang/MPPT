function [sys,x0,str,ts] = afd(t,x,u,flag)
switch flag,
  case 0,
    [sys,x0,str,ts]=mdlInitializeSizes;
  case 1,
    sys=[];
  case 2,
    sys=[];
  case 3,
    sys=mdlOutputs(t,x,u);
  case 4,
    sys=[];
  case 9,
    sys=[];
   otherwise
    error(['Unhandled flag = ',num2str(flag)]);
end

%=============================================================================
% mdlInitializeSizes
function [sys,x0,str,ts]=mdlInitializeSizes
global f_i f_vo theta_i theta_vo isIslanding
f_i=50;
f_vo=50;
theta_i=0;
theta_vo=0;
isIslanding=0;
    
sizes = simsizes;
sizes.NumContStates  = 0;
sizes.NumDiscStates  = 0;
sizes.NumOutputs     = 3;
sizes.NumInputs      = 2;
sizes.DirFeedthrough = 1;
sizes.NumSampleTimes = 1;   % at least one sample time is needed
sys = simsizes(sizes);
x0  = [];
str = [];
ts  = [1e-4 0];
%=====================================================================
% mdlOutputs
function sys=mdlOutputs(t,x,u)
global f_i f_vo theta_i theta_vo isIslanding
theta_vo=u(2);

if abs(theta_vo)<0.04	% 电压相位为0时更新频率0.04
        f_vo=u(1);
end

if(isIslanding==0)	
    if abs(theta_vo)<0.04	
        if (f_vo>50.5) || (f_vo<49.5)	%判断频率是否越界
            sys=[0 0];
            isIslanding=1;
        else
            f_i=f_vo+2.5;%2.5
            theta_i=theta_vo;
        end
    else
        if(pi-theta_i<0.03) && (pi-theta_vo>0.03) 
            theta_i=pi;
        elseif(2*pi-theta_i<0.03) && (2*pi-theta_vo>0.03) 
            theta_i=2*pi;                    
        elseif (pi-theta_vo<0.03) && (pi-theta_i>0.03)
            theta_i=pi;
        elseif (2*pi-theta_vo<0.03) && (2*pi-theta_i>0.03)
            theta_i=2*pi;
        else
            theta_i=theta_i+2*pi*f_i*1e-4;
        end
    end
    sys(1)=sin(theta_i);
    sys(3)=1; 
else
    sys(1)=0;
    sys(3)=0;
end
sys(2)=f_vo;
