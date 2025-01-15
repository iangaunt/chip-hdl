// i = 1.
@i.
M = 1.

// Initialize sum at address 1;
@1.
@sum.

// sum = 0.
@sum.
M = 0.

// sum = sum + i.
@i.
D = M.

@sum.
M = M + D.

// i = i + 1.
@i.
M = M + 1.