%% MP3 File as input 

y = dsp.AudioFileReader('handel.ogg','SamplesPerFrame',48000);

y_t = [];

for i = 1:5
    y_t = [y_t;y.step()];
end

y = y_t(1:48000*5);

%% Listen to audio

soundsc(y, 48000);

%% Generate sinusoid

sine = dsp.SineWave('Amplitude',0.5,'Frequency',1000,...
    'SampleRate',48000,...
    'SamplesPerFrame',48000);


sine2 = dsp.SineWave('Amplitude',0.5,'Frequency',500,...
    'SampleRate',48000,...
    'SamplesPerFrame',48000);

eta1 = [];
eta1_1 = [];
eta1_2 = [];


for i = 1:5
    
    eta1_1 = [eta1_1; sine()];
    eta1_2 = [eta1_2; sine2()];
    
end

eta1 = eta1_1;

%% Generate colored noise

filter_length = 5;

%   filter coefficients
h=0.5.^[0:filter_length-1];
%   input signal

u=randn(44100*5,1);

eta2=conv(h,u);

eta2=eta2(filter_length:end)/20;

%% Listen to sinusoid 

soundsc(y*2, 48000);

%% Listen to mixed signal
    
soundsc(y + eta1, 48000);

%% Listen to mixed signalz
while true
    soundsc(y + eta1/10, 48000);
end
%% Print pre signals 

plot(y(1:500))
figure()
plot(eta1(1:500))
figure()
plot(eta2(1:500))
figure()
plot(y(1:500) + eta(1:500))

%% Monotone sinusoid - Run filter 

% Value 

block_size = 5;

filter_size = block_size;

% Delay 

D = block_size;

delay = dsp.Delay(2);

% Algorithm

s = eta1 + y;
%s = eta1 + eta2 + y;

d = delay(s);

%d = s;

[e,y_out,w,w_hist] = lms(0.01,filter_size,s,d);
%[e,w,w_hist] = blocklms(0.005,filter_size,s,d);
%[e,y_out,w,w_hist] = nlms(0.5,filter_size,s,d,100);

figure()
plot(e(1:1000))
figure()
plot(w_hist')
hold on
%plot(repmat(w_hist,length(w_hist'),1))
soundsc(e, 44100);

%% FM sinusoid 

Fs = 44100;
fm_eta = 0.5 * sin(2*pi*1000/Fs*(0:396899)' + 10*sin(2*pi/Fs*(0:396899)'));
fm_eta = fm_eta(1:44100*5);

soundsc(fm_eta,44100)

%% Run filter

% Value 

block_size = 5;

filter_size = block_size;

% Delay 

D = 50;

delay = dsp.Delay(D);

% Algorithm

s = fm_eta + y;

d = delay(s);
    
[e,w,w_hist] = lms(0.1,filter_size,s,d);
%[e,y_out] = blocklms(0.01,filter_size,s,d);
%[e,y_out] = nlms(0.1,filter_size,s,d,100);
   
plot(e)
plot(w_hist)
soundsc(e, 44100);


%% test

clc

block_size = 4;

filter_size = block_size;

% Delay 

D = 2;

mu = 0.9

delayy = dsp.Delay(2);

u = [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16];

d = [0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14];

M = filter_size;

w=zeros(M,1);
N=length(u);

d=d(:);
e=d;
u=u(:);

Blocks=N/M;


for k = 1:3
       
   u(k*M:1:(k+1)*M-1),u(k*M:-1:(k-1)*M+1)
   umat=toeplitz(u(k*M:1:(k+1)*M-1),u(k*M:-1:(k-1)*M+1))
   dvec=d(k*M:1:(k+1)*M-1)
  
   yvec=umat*w
   
   evec=dvec-yvec;
   
   e(k*M:1:(k+1)*M-1)=evec;
   
   evec
   
   phi=umat.'*evec
   
   w=w+mu*phi
   
end

%% test2 

clc



DSP_BLOCK_SIZE = int32(5);
DELAY_SIZE = int32(2);
mu = 0.01;

u = y;

u_1_s = int32(0);
u_2_s = int32(DSP_BLOCK_SIZE - 1);
d_s = int32(u_2_s - DELAY_SIZE);

w = zeros(1,DSP_BLOCK_SIZE);
w_h = [];

buffer = zeros(1,DSP_BLOCK_SIZE*2 -1);

N=length(u);

blocks= N/DSP_BLOCK_SIZE;

counter = 1;
e_v = [];

for k=1:blocks
       
    for l = 0:DSP_BLOCK_SIZE-1
        
        index = mod( (u_2_s  + l),(DSP_BLOCK_SIZE*2 -1));
                    
        buffer(index+1) = u(counter);
        counter = counter + 1;


        
    end
    
    [e,w,u_1_s,u_2_s,d_s] = block_m(buffer,w,int32(u_1_s),int32(u_2_s),int32(d_s));
  

    
    e_v = [e_v;e];   
    w_h = [w_h,w];

end
  
% Plot 
close all
%figure(1)
%plot(w_h')
%figure(2)
%plot(e_v)

%% test3 

clc

block_size =100;
delay = dsp.Delay(100);

u = eta1 + y;

d = delay(u);

w = zeros(1,block_size);
w_h = [];

N=length(u);

e_v = [];

for k=block_size:block_size+40000
    
    uvec=u(k:-1:k-block_size+1);
    
    d_val = d(k);

    [e,w] = block_m(w,uvec,d_val);
      
    e_v = [e_v;e];   
    w_h = [w_h,w];
    
end
  
% Plot 
close all
figure(1)
plot(w_h')
figure(2)
plot(e_v)


%%
close all
plot(w_h')
%%
soundsc(e_v, 44100);
