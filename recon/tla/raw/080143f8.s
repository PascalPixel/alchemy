.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_080142d4, 0x080142d4
	.global Func_080143f8
	.thumb_func
Func_080143f8:
	push	{lr}
	adds	r2, r1, #0
	ldr	r1, [pc, #8]
	lsls	r3, r0, #2
	ldrh	r1, [r1, r3]
	bl	sub_080142d4
	pop	{pc}
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	ldr	r1, [pc, #152]
	mov	sl, r2
	ldrb	r2, [r1, #0]
	adds	r6, r0, #0
	mov	r9, r2
	movs	r0, #31
	cmp	r2, #31
	bhi.n	.L_080144a8
	lsls	r3, r2, #3
	ldr	r2, [pc, #140]
	adds	r7, r3, r2
	mov	r3, r9
	adds	r3, #1
	strb	r3, [r1, #0]
	cmp	r8, sl
	beq.n	.L_08014440
	mov	r3, r8
	mov	r1, sl
	cmn	r3, r1
	bne.n	.L_08014468
.L_08014440:
	cmp	r6, #0
	bne.n	.L_08014468
	movs	r0, #128
	ldr	r3, [pc, #116]
	mov	r1, sl
	lsls	r0, r0, #9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4642
	mov	r1, sl
	adds	r3, r0, #0
	cmn	r2, r1
	bne.n	.L_0801445c
	negs	r3, r0
.L_0801445c:
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r3, r0, #16
	str	r3, [r7, #4]
	b.n	.L_080144a6
.L_08014468:
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r1, r8
	adds	r6, r0, #0
	bl	sub_08002054
	mov	r1, r8
	strh	r0, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r7, #2
	negs	r5, r5
	strh	r0, [r7, #0]
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r7, #2
	strh	r0, [r7, #0]
	mov	r1, sl
	adds	r0, r6, #0
	bl	sub_08002054
	adds	r7, #2
	strh	r0, [r7, #0]
.L_080144a6:
	mov	r0, r9
.L_080144a8:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x030011fc
	.4byte 0x020054d0
	.2byte 0x0528
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #48]
	ldr	r4, [pc, #32]
	strb	r2, [r3, #0]
	movs	r0, #0
	movs	r2, #23
.L_080144d4:
	ldrh	r3, [r1, #4]
	subs	r2, #1
	orrs	r3, r4
	str	r0, [r1, #0]
	strh	r3, [r1, #4]
	strb	r0, [r1, #6]
	adds	r1, #8
	cmp	r2, #0
	bge.n	.L_080144d4
	ldr	r2, [pc, #12]
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_08014500
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x03001228
	.4byte 0x03001108
	.2byte 0x3610
	.2byte 0x0200
.L_08014500:
	pop	{pc}
	.align 2, 0
