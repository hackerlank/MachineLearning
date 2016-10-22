function [ A, B, C ] = tensor3_decomp_jennrich( T, ethres )
% Implements Jennrich's algorithm for tensor decomposition
% T is a matlab three-dimensional array
% ethres [optional] is the eigenvalue threshold separating factors and
% noise.
%
% Author: Zilong Tan (ztan@cs.duke.edu)
%
% Example Usage:
%%
% A = randn(5,3);
% B = randn(4,3);
% C = randn(8,3);
% T = tensor3_create(A,B,C);  % Create a three-way tensor T
% [A1,B1,C1] = tensor3_decomp_jennrich(T); % Recover the factor matrices A,
% B, and C. NOTE: A1, B1, C1 will have respectively permuted/scaled columns
% of A, B, and C.
%%


% eigen threshold
if nargin == 1
    ethres = 1e-6;
end

na = size(T,1);
nb = size(T,2);
nc = size(T,3);

x = rand(nc,1);
y = rand(nc,1);

P = tensor3_collapse(T,x);
Q = tensor3_collapse(T,y);

[A,Dp] = eig(P*pinv(Q));
Dp = diag(Dp);
[B,Dq] = eig(Q'*pinv(P)');
Dq = diag(Dq);

A  = A(:, abs(Dp)>ethres);
Dp = Dp(abs(Dp)>ethres);
B  = B(:, abs(Dq)>ethres);
Dq = Dq(abs(Dq)>ethres);

% match up A's columns with B's
A1 = A(:,Dp>0);
A2 = A(:,Dp<=0);
B1 = B(:,Dq>0);
B2 = B(:,Dq<=0);

[~,AI] = sort(Dp(Dp>0),'ascend');
[~,BI] = sort(Dq(Dq>0),'descend');
A = A1(:,AI);
B = B1(:,BI);

[~,AI] = sort(Dp(Dp<=0),'ascend');
[~,BI] = sort(Dq(Dq<=0),'descend');
A = [A A2(:,AI)];
B = [B B2(:,BI)];

% Solve for C
r = min([size(A,2), na, nb, nc]);
G = zeros(na*nb,r);
M = zeros(na*nb,nc);

for i = 1:r
    G(:,i) = reshape(A(:,i)*B(:,i)',na*nb,1);
    M(:,i) = reshape(T(:,:,i),na*nb,1);
end

for i = r+1:nc
    M(:,i) = reshape(T(:,:,i),na*nb,1);
end

C = (G\M)';

end
