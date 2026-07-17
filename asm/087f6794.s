@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087f6794
	.thumb_func
Func_087f6794:
	push	{r4, r5, r6, r7, lr}
	strh	r5, [r1, #16]
	ldr	r2, [r1, #52]
	ldrb	r3, [r3, #24]
	bx	r2
