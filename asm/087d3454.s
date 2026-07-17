@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087d3454
	.thumb_func
Func_087d3454:
	push	{r1, r2, r4, lr}
	movs	r7, #145
	pop	{r1, r2, r6, r7, pc}
