function [ G, A, B, C ] = hooi( T, Dims )

[G,A,B,C] = hosvd(T,Dims);

maxiter = 100;
oldnorm = norm(G);

T1 = double(tenmat(tensor(T),1));
T2 = double(tenmat(tensor(T),2));
T3 = double(tenmat(tensor(T),3));

for i = 1:maxiter
    [A,~,~] = svd(T1*double(kron(B,C)));
    [B,~,~] = svd(T2*double(kron(A,C)));
    [C,~,~] = svd(T3*double(kron(A,B)));
    newnorm = norm(tensor(tensor3_create_tucker(T,A',B',C')));
    if newnorm > oldnorm - 1e-15
        break;
    end
    oldnorm = newnorm;
end

G = tensor3_create_tucker(T,A',B',C');

end
