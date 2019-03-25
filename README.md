# -pythagorean-numbers
An introduction into SSE instructions for xmm registers in assembly. The program checks what pairs of numers (under 1000) are pythagorean numbers (c^2 = a^2 + b^2). The assembly function squares each number in the register, adds it to the square of the other register number's square,
creates a copy of that sum, rounds it and then substarcts the rounded value from the original sum of squares.
If the result after the substraction is equal to 0, then the pair are pythagorean numbers.

