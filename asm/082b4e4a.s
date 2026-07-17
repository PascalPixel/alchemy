@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_082b4e4a
	.thumb_func
Func_082b4e4a:
	push	{r0, r3, r4, r6, r7, lr}
	b.n	Func_082b4d2e
	pop	{r1, r2, r3, r5, r6, r7, pc}
