.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0811ff08, 0x0811ff08
	.set sub_081201c4, 0x081201c4
	.set sub_08120454, 0x08120454
	.set sub_08126cfc, 0x08126cfc
	.global Func_0811e36c
	.thumb_func
Func_0811e36c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r1, #0
	movs	r0, #0
	bl	sub_08126cfc
	movs	r3, #1
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_0811e386
	bl	sub_0811ff08
	b.n	.L_0811e3a2
.L_0811e386:
	movs	r5, #0
	cmp	r5, r3
	bge.n	.L_0811e3a2
.L_0811e38c:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08120454
	bl	sub_081201c4
	movs	r3, #1
	ldrsb	r3, [r6, r3]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_0811e38c
.L_0811e3a2:
	movs	r0, #1
	bl	sub_08013560
	pop	{r5, r6, pc}
	.2byte 0x0000
