@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087fb4cc
	.thumb_func
Func_087fb4cc:
	push	{r4, r5, r7, lr}
	adds	r6, #11
	ldrh	r5, [r6, #10]
	movs	r5, #184
	strh	r0, [r4, #6]
	strh	r0, [r6, r7]
	strb	r7, [r5, #15]
	pop	{r2, r3, r5, pc}
