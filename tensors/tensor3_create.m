function [ T ] = tensor3_create( A, B, C )
% Returns the tensor: T = \sum_i a_i x b_i x c_i, as a MATLAB three-dimensional
% array.
%
% Author: Zilong Tan (ztan@cs.duke.edu)
%

na = size(A,1);
nb = size(B,1);
nc = size(C,1);
T  = zeros(na,nb,nc);

for i = 1:nc
    D = diag(C(i,:));
    T(:,:,i) = A*D*B';
end

end
