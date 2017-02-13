function [e,w,w_hist]=blocklms(mu,M,u,d);

w=zeros(M,1);
N=length(u);

d=d(:);
e=d;
u=u(:);

Blocks=N/M;

w_hist = [];

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


