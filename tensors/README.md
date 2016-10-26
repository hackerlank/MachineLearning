Tensor Decomposition using Jennrich's Algorithm

* Example Usage:

 A = randn(5,3);
 
 B = randn(4,3);
 
 C = randn(8,3);
 
 T = tensor3_create(A,B,C);  % Create a three-way tensor T
 
 [A1,B1,C1] = tensor3_decomp_jennrich(T); % Recover the factor matrices A,
 B, and C. NOTE: A1, B1, C1 will have respectively permuted/scaled columns
 of A, B, and C.
 
