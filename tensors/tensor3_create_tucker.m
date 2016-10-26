function [ T ] = tensor3_create_tucker( G, A, B, C )

T = double(ttm(ttm(ttm(tensor(G),A,1),B,2),C,3));

end

