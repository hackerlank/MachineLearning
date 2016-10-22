function [ M ] = tensor3_collapse( T, x )
% Compute M = \sum_i T(:,:,i) * x_i
%
% Author: Zilong Tan (ztan@cs.duke.edu)
%

sz = size(T);
M  = zeros(sz(1:2));

for i = 1:sz(3)
    M = M + T(:,:,i)*x(i);
end

end

