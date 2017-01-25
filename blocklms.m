function [e,w,w_hist]=blocklms(mu,M,u,d);
%           Call:
%           [e,w]=blocklms(mu,M,u,Dalton);       
%
%           Input arguments:
%           mu      = step size, dim 1x1
%           M       = filter length, dim 1x1
%           u       = input signal, dim Nx1
%           d       = desired signal, dim Nx1    
%
%           Output arguments:
%           e       = estimation error, dim Nx1
%           w       = final filter coefficients, dim Mx1
%
%           The length N is adjusted such that N/M is integer!
%           

%initialization
w=zeros(M,1);
N=length(u);
d=d(:);
e=d;
u=u(:);
%no. of blocks
Blocks=N/M;

w_hist = [];

%Loop, BlockLMS
for k=1:Blocks-1
       
   %Set up input signal matrix, dim. MxM (cf. example 1, Haykin p. 448) 
   umat=toeplitz(u(k*M:1:(k+1)*M-1),u(k*M:-1:(k-1)*M+1));
   
   %Set up vector with desired signal 
   dvec=d(k*M:1:(k+1)*M-1);
  
   %calculate output signal (Eq.10.5)
   yvec=umat*w;
   
   %calculate error vector (Eq.10.8)
   evec=dvec-yvec;
   
   %log error
   e(k*M:1:(k+1)*M-1)=evec;
   
   %calculate gradient estimate (Eq.10.11)
   phi=umat.'*evec;
   
   %update filter coefficients (Eq.10.10)
   w=w+mu*phi;
   
   w_hist = [w_hist,w];

end


