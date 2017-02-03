%% MP3 File as input 

y = dsp.AudioFileReader('handel.ogg','SamplesPerFrame',44100);

y_t = [];

for i = 1:5
    y_t = [y_t;y.step()];
end

y = y_t(1:44100*5);

%% Listen to audio

soundsc(y, 44100);


%% Generate sinusoid

sine = dsp.SineWave('Amplitude',0.5,'Frequency',1000,...
    'SampleRate',44100,...
    'SamplesPerFrame',44100);


sine2 = dsp.SineWave('Amplitude',0.5,'Frequency',500,...
    'SampleRate',44100,...
    'SamplesPerFrame',44100);

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

soundsc(eta1, 44100);

%% Listen to mixed signal

soundsc(y + eta1, 44100);

%% Listen to mixed signal

soundsc(y + eta1 + eta2, 44100);

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

delay = dsp.Delay(block_size);

% Algorithm

s = eta1 + y;
%s = eta1 + eta2 + y;

d = delay(s);

disp(d(1:1:100))

%d = s;

%[e,y_out,w,w_hist] = lms(0.01,filter_size,s,d);
%[e,w,w_hist] = blocklms(0.00001,filter_size,s,d);
[e,y_out,w,w_hist] = nlms(0.5,filter_size,s,d,100);

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

D = block_size;

delay = dsp.Delay(D);

% Algorithm

s = fm_eta + y;

d = delay(s);
    
%[e,w,w_hist] = lms(0.1,filter_size,s,d);
[e,y_out] = blocklms(0.01,filter_size,s,d);
%[e,y_out] = nlms(0.1,filter_size,s,d,100);
   
plot(e)
plot(w_hist)
soundsc(e, 44100);


%% test

clc

block_size = 4;

filter_size = block_size;

% Delay 

D = block_size;

mu = 0.9

delayy = dsp.Delay(2);

u = [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]

d = [0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]

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
   
   e(k*M:1:(k+1)*M-1)=evec
   
   phi=umat.'*evec
   
   w=w+mu*phi
   
end

