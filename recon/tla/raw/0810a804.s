.syntax unified
	.thumb
	.set sub_080ad100, 0x080ad100
	.set sub_0810a7dc, 0x0810a7dc
	.global Func_0810a804
	.thumb_func
Func_0810a804:
	push	{r5, r6, r7, lr}
	sub	sp, #32
	mov	r6, sp
	adds	r0, r6, #0
	bl	sub_080ad100
	movs	r5, #0
	adds	r7, r0, #0
	cmp	r5, r7
	bge.n	.L_0810a82e
.L_0810a818:
	ldrh	r0, [r6, #0]
	adds	r6, #2
	bl	sub_0810a7dc
	cmp	r0, #0
	beq.n	.L_0810a828
	movs	r0, #1
	b.n	.L_0810a830
.L_0810a828:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L_0810a818
.L_0810a82e:
	movs	r0, #0
.L_0810a830:
	add	sp, #32
	pop	{r5, r6, r7, pc}
	ldr	r2, [pc, #12]
	ldr	r3, [pc, #16]
	ands	r0, r2
	movs	r2, #175
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	bx	lr
	.4byte 0x000001ff
	.2byte 0x0240
	.2byte 0x0200
	ldr	r3, [pc, #16]
	movs	r2, #175
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bx	lr
	.4byte 0x02000240
