@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_086b0b98
	.thumb_func
Func_086b0b98:
	push	{r3, r7, lr}
	sbcs	r6, r1
	cmp	r5, r9
	pop	{r1, r3, r7, pc}
