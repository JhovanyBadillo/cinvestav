# Hamming distances use case with Intel intrinsics

The assignment was to play with Python `ctypes` module implementing and using a shared library with C and Intel intrinsics.

The problem that the shared library try to solve is the computation of a vector of Hamming distances, i.e., computes the Hamming distance between some $256$-bit vector $Q$ and each $B_i$ of $N$ other $256$-bit vectors.

A bit of context.

Hamming distance is defined for two bit vectors $A$ and $B$ of size $K$ as

$$d_H(A, B) = \sum_{i=0}^{K-1} (a_i \oplus b_i)$$

where

$$A = (a_0, a_1, \dots, a_{K-1}), \quad B = (b_0, b_1, \dots, b_{K-1})$$

and $\oplus$ is the XOR operation.

$\text{Ex}^1$: $K = 4$, $A = (0, 1, 1, 1)$, $B = (1, 1, 1, 1)$,
$$d_H(A, B) = 1$$

$\text{Ex}^2$: $K = 4$, $A = (0, 1, 0, 1)$, $B = (1, 0, 1, 0)$,
$$d_H(A, B) = 4$$

Now suppose we want to compare one bit vector $Q$ to a set of $N \gg 1$ vectors of the same size, $K$.

Comparing $Q$ to each of the $B_i$, $0 \le i < N$, will give us a vector of Hamming distances:

$$D_H(Q, B) = \begin{pmatrix} d_H(Q, B_0) \\ d_H(Q, B_1) \\ \vdots \\ d_H(Q, B_{N-1}) \end{pmatrix}$$.
