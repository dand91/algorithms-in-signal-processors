function [Jmin,R,p,wo]=wiener(W,sigmav2);
%WIENER Returns R and p, together with the Wiener filter
%       solution and Jmin for computer exercise 2.4.
%
%           Call:
%           [Jmin,R,p,wo]=wiener(W,sigmav2);
%       
%           Input arguments:
%           W           =eigenvalue spread, dim 1x1
%           sigmav2     =variance of the additive noise
%                        source, dim 1x1
%
%           Output arguments:
%           Jmin        =minimum MSE obtained by the Wiener
%                        filter, dim 1x1
%           R           =autocorrelation matrix, dim 11x11          
%           p           =cross-correlation vector, dim 11x1
%           wo          =optimal filter, dim 11x1
%
%           Choose the remaining parameters according to
%           Haykin chapter 9.7.

%filter coefficients h1,h2,h3
h1=1/2*(1+cos(2*pi/W*(1-2)));
h2=1/2*(1+cos(2*pi/W*(2-2)));
h3=1/2*(1+cos(2*pi/W*(3-2)));

%variance of driving noise
sigmax2=1;

%theoretical autocorrelation matrix R 11x11
R=sigmax2*toeplitz([h1^2+h2^2+h3^2,...
  h1*h2+h2*h3,h1*h3,zeros(1,8)])+sigmav2*eye(11);

%theoretical cross-correlation vector p 11x1
p=sigmax2*[zeros(4,1);h3;h2;h1;zeros(4,1)];

%Wiener filter  
wo=R\p;

%Jmin
Jmin=sigmax2-p'*wo;

