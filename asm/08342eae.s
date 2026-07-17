@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08342eae
	.thumb_func
Func_08342eae:
	push	{r2, r3, r4, r7, lr}
	asrs	r6, r0, #3
	adds	r5, #212
	ldrb	r5, [r0, #28]
	pop	{r0, r1, r2, r3, r4, pc}
