function [e,w,w_hist]=blocklms(mu,M,u,d);

w=zeros(M,1);
N=length(u);

d=d(:);
e=d;
u=u(:);

Blocks=N/M;

w_hist = [];

for k=1:Blocks-1
       
   umat=toeplitz(u(k*M:1:(k+1)*M-1),u(k*M:-1:(k-1)*M+1));   

   dvec=d(k*M:1:(k+1)*M-1);
  
   yvec=umat*w;
   
   evec=dvec-yvec;
   
   e(k*M:1:(k+1)*M-1)=evec;
   
   phi=umat.'*evec;
   
   w=w+mu*phi;
   
   w_hist = [w_hist,w];

end


