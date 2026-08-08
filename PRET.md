# Working like pret contributors

The fastest path to byte-exact C is a small, evidence-driven loop. For
`resource_373:5b48`, we started with the smallest useful target and looked for
related owners before changing code. Six owners produced identical machine
code:

`resource_373:5b48`, `resource_38e:0974`, `resource_392:0cb4`,
`resource_39f:2c0c`, `resource_3a8:3a3c`, and `resource_3b2:2ed8`.

We compiled every existing semantic formulation through the fixed compiler
route and compared the results. `resource_392:0cb4` was the closest sibling,
with 9 differing halfwords; `resource_373:5b48` had 17, and the other siblings
had 88, 106, 88, and 88. That made 392 the useful lead rather than assuming
the target was the easiest draft.

We inspected the residual as clusters, tested one ordinary C hypothesis at a
time, and reverted regressions. An ordinary `u8` temporary for the object flag
store corrected one cluster without an escape hatch. We then exhausted the
bounded source and compiler-mode searches. Only after that did we add the
narrow compiler correction: all six independent witnesses required the same
instruction sequence, so the evidence pointed to a compiler scheduling
choice, not a game-specific C trick.

After proving `resource_392:0cb4` exact, we first transposed its meaning to
`resource_373:5b48` through equivalent structs. That reasonable-looking rewrite
regressed to 88 differing halfwords because it changed alias information and
register allocation. A literal, address-adjusted transcription of the proven
392 C shape matched 373 immediately. This is the useful rule: transpose the
known machine-producing source structure first; humanize it only while each
change remains exact.

We then verified the source under its eventual `exact/` path and adopted it
from `semantic/`. Compiler flags are routed by source path, so a candidate that
matches under `semantic/` is not the final proof. Adoption is the step that
counts toward byte-exact progress; a near-match is never promoted.

`decomp-permuter` is useful as a bounded rescue tool when the deterministic
source and compiler searches have been exhausted. It is not the first choice
for this case: the sibling comparison isolated a repeatable instruction
sequence across six owners, making a readable source change and a narrow,
evidence-backed compiler correction more direct and reproducible than a broad
stochastic permutation search.

Finally, run the complete verification gate. It rebuilds the ROM and related
outputs and is the authority for whether the reconstruction is real. If an
adoption unexpectedly changes the audited executable denominator, stop and
explain it. Here that check exposed a backwards `ldr`/`bx` veneer classifier;
fixing and testing the measurement was part of proving the result, not an
optional cleanup after it.
