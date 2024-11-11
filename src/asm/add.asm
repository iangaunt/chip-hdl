@1022.
D = A.
D = D + A.

// move the register of M.
@28623.
M = D.

@25000.
M = D.
M = M + 1.

@30000.
M = D.
M = M + 1.

// jumps based on the calculation between two values.
D ; JLE > @4.
D + 1; JLT > @4.
D + M; JGE > @4.

M = A.