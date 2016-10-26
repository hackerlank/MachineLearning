function [ G, A, B, C ] = hosvd( T, Dims )

T1 = tenmat(tensor(T),1);
[U,~,~] = svd(double(T1));
A = U(:,1:Dims(1));
T2 = tenmat(tensor(T),2);
[U,~,~] = svd(double(T2));
B = U(:,1:Dims(2));
T3 = tenmat(tensor(T),3);
[U,~,~] = svd(double(T3));
C = U(:,1:Dims(3));

G = ttm(ttm(ttm(tensor(T),A',1),B',2),C',3);

end
