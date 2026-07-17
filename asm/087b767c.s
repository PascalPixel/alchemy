@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087b767c
	.thumb_func
Func_087b767c:
	push	{r1, r6, r7, lr}
	subs	r4, #218
	strb	r0, [r5, #14]
	ldrb	r1, [r2, r1]
	pop	{r0, r1, r2, r4, r6, r7, pc}
