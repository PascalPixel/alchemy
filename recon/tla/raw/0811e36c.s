.syntax unified
	.thumb
	.global Func_0811e36c
	.thumb_func
Func_0811e36c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r1, #0
	movs	r0, #0
	bl	0x08126cfc
	movs	r3, #1
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_0811e386
	bl	Func_0811ff08
	b.n	.L_0811e3a2
.L_0811e386:
	movs	r5, #0
	cmp	r5, r3
	bge.n	.L_0811e3a2
.L_0811e38c:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	0x08120454
	bl	Func_081201c4
	movs	r3, #1
	ldrsb	r3, [r6, r3]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_0811e38c
.L_0811e3a2:
	movs	r0, #1
	bl	WaitFrames
	pop	{r5, r6, pc}
	.2byte 0x0000
