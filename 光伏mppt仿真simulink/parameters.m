kHz = 1e3;

fsw = 145*kHz;

fmax = 185*kHz;
conmin=0.85;

fmin = 105*kHz;
conmax=1.15;

fgain=(fmin-fmax)/(conmin-conmax);

f0=fmax+fgain*conmin;



deadtime= 0.2e-6;

fp=10*kHz;
ffc=15*kHz;
xi=0.707;


tsw2= 1/fsw/2;
tsw = tsw2*2;
wp=2*pi*fp;
wfc=2*pi*ffc;