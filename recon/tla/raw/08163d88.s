.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08118098, 0x08118098
	.global Func_08163d88
	.thumb_func
Func_08163d88:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	bl	sub_08118098
	adds	r7, r0, #0
	adds	r0, r5, #0
	ldr	r6, [r7, #0]
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	movs	r3, #102
	adds	r3, #255
	cmp	r0, r3
	beq.n	.L_08163df6
	adds	r2, #39
	cmp	r0, r2
	beq.n	.L_08163df6
	adds	r3, #16
	cmp	r0, r3
	beq.n	.L_08163df6
	adds	r2, #7
	cmp	r0, r2
	beq.n	.L_08163df6
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r6, #52]
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r6, #48]
	movs	r3, #192
	lsls	r3, r3, #9
	movs	r5, #63
	ands	r5, r0
	str	r3, [r6, #40]
	adds	r0, r6, #0
	bl	sub_08020138
	ldr	r3, [r7, #12]
	adds	r5, #130
	adds	r0, r5, #0
	muls	r0, r3
	movs	r1, #100
	bl	sub_08002054
	ldr	r3, [r7, #16]
	adds	r1, r0, #0
	movs	r2, #0
	adds	r0, r6, #0
	bl	sub_08020148
.L_08163df6:
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, r6, #0
	str	r3, [r6, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r6, #68]
	adds	r0, r6, #0
	strb	r3, [r2, #0]
	movs	r1, #5
	bl	sub_08020090
	pop	{r5, r6, r7, pc}
	.align 2, 0
