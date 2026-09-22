.syntax unified
	.thumb
	.set sub_08077394, 0x08077394
	.set sub_08077428, 0x08077428
	.global Overlay_080788c4
Overlay_080788c4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	sub_08077394
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r3, [r0, r5]
	movs	r6, #1
	negs	r6, r6
	cmp	r3, #0
	beq.n	.L_0807893a
	movs	r2, #248
	lsls	r2, r2, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080788f4
	ldr	r2, [pc, #8]
	adds	r3, r3, r2
	strh	r3, [r0, r5]
	movs	r6, #1
	b.n	.L_0807893a
	.2byte 0xf800
	.2byte 0xffff
.L_080788f4:
	adds	r6, r0, #0
	adds	r6, #216
	strh	r2, [r0, r5]
	adds	r4, r6, #0
	movs	r5, #0
	adds	r1, r6, #0
	movs	r0, #14
.L_08078902:
	ldrh	r2, [r4, #0]
	lsls	r3, r2, #16
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L_08078912
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
.L_08078912:
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L_08078902
	cmp	r5, #14
	bgt.n	.L_08078938
	lsls	r3, r5, #1
	adds	r0, r3, r6
	ldr	r2, [pc, #16]
	movs	r3, #15
	subs	r5, r3, r5
.L_08078926:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L_08078926
	b.n	.L_08078938
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_08078938:
	movs	r6, #2
.L_0807893a:
	adds	r0, r7, #0
	bl	sub_08077428
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
