@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0817b764
	.thumb_func
Func_0817b764:
	push	{r1, r3, r4, r6, r7, lr}
	lsrs	r7, r2, #32
	pop	{r0, r4, r5, r7}
	pop	{pc}
