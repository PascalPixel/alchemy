@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c4bc
	.thumb_func
Func_0800c4bc:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	ldr	r1, [pc, #36]
	adds	r5, r0, #0
	bl	Func_0800c2d8
	cmp	r6, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	adds	r2, r5, #0
	str	r3, [r5, #48]
	adds	r2, #100
	movs	r3, #0
	str	r6, [r5, #104]
	strh	r3, [r2, #0]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x080135f0
