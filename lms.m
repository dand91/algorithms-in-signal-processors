function [e,y,w,w_hist]=lms(mu,M,u,d);
%           Call:
%           [e,w]=lms(mu,M,u,d);
%
%           Input arguments:
%           mu      = step size, dim 1x1
%           M       = filter length, dim 1x1
%           u       = input signal, dim Nx1
%           d       = desired signal, dim Nx1    
%
%           Output arguments:w
%           e       = estimation error, dim Nx1
%           w       = final filter coefficients, dim Mx1

%initial weights
w=zeros(M,1);

%length of input signal
N=length(u);

%make sure that u and d are column vectors
u=u(:);
d=d(:);

w_hist = [];

%LMS
for n=M:N
    
    uvec=u(n:-1:n-M+1);
    y(n) = w'*uvec;
    e(n)=d(n)-y(n);  
    w=w+mu*uvec*conj(e(n));
    w_hist = [w_hist,w];

end
e=e(:);
y=y(:);
