@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083670e0
	.thumb_func
Func_083670e0:
	push	{r1, r2, r4, r5, r6, lr}
	adcs	r6, r2
	bls.n	Func_08367154
	stmia	r4!, {r0, r3, r4, r6, r7}
	lsrs	r4, r7, #24
	pop	{r2, r6, pc}
