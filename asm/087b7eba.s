@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087b7eba
	.thumb_func
Func_087b7eba:
	push	{r2, r3, r6, r7, lr}
	ldr	r1, [r3, #76]
	ldrb	r4, [r2, r6]
	subs	r3, #122
	subs	r3, r5, r3
	pop	{r1, r3, r5, r6, r7, pc}
