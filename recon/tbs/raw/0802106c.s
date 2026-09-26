.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_080165d8, 0x080165d8
	.set sub_08019908, 0x08019908
	.set sub_08019ba0, 0x08019ba0
	.set sub_08019d2c, 0x08019d2c
	.set sub_0801a4fc, 0x0801a4fc
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801eadc, 0x0801eadc
	.set sub_08020a60, 0x08020a60
	.set sub_080b0020, 0x080b0020
	.set sub_080b0030, 0x080b0030
	.set sub_080b0038, 0x080b0038
	.set sub_080f9010, 0x080f9010
	.set sub_080f9048, 0x080f9048
	.global PartyTalkMenu_Choose
	.global Func_0802106c
	.thumb_func
PartyTalkMenu_Choose:
Func_0802106c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r3, #7
	mov	sl, r2
	movs	r0, #7
	movs	r2, #18
	bl	sub_080162d4
	ldr	r5, [pc, #112]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #0
	bl	sub_0801e7c0
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #16
	adds	r5, #2
	bl	sub_0801e7c0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #32
	bl	sub_0801e7c0
	bl	sub_08004080
	movs	r7, #0
	str	r0, [sp, #8]
	cmp	r0, #95
	bgt.n	.L_0802110c
	ldr	r2, [pc, #64]
	movs	r1, #128
	bl	sub_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r2, r6, #0
	movs	r3, #0
	ldr	r0, [sp, #8]
	str	r7, [sp, #0]
	bl	sub_0801eadc
	add	r3, sp, #12
	str	r0, [r3, #0]
	ldrh	r1, [r6, #12]
	ldrh	r2, [r6, #14]
	mov	r8, r3
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #3
	adds	r2, #9
	mov	r0, r8
	bl	sub_080b0038
	b.n	.L_08021110
.L_080210f8:
	movs	r0, #113
	movs	r7, #1
	bl	sub_080f9010
	negs	r7, r7
	b.n	.L_080211fa
	.4byte 0x00002080
	.2byte 0x10a4
	.2byte 0x0803
.L_0802110c:
	add	r2, sp, #12
	mov	r8, r2
.L_08021110:
	ldr	r4, [pc, #28]
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	adds	r1, r4, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #28]
	strh	r3, [r4, #8]
	movs	r3, #1
	mov	fp, r3
	mov	r9, r2
	b.n	.L_08021144
	.4byte 0x00006318
	.4byte 0x050001c0
	.4byte 0x040000d4
	.4byte 0x050001e0
	.4byte 0x84000008
	.2byte 0x1b04
	.2byte 0x0300
.L_08021144:
	lsls	r5, r7, #1
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r1, #1
	movs	r3, #14
	adds	r2, r5, #0
	adds	r0, r6, #0
	str	r3, [sp, #4]
	bl	sub_08020a60
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #14
	movs	r1, #1
	adds	r2, r5, #0
	bl	sub_08020a60
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_08021192
	movs	r2, #0
	mov	sl, r2
	ldrh	r2, [r6, #14]
	ldrh	r1, [r6, #12]
	adds	r2, r2, r5
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #3
	adds	r2, #9
	mov	r0, r8
	movs	r3, #3
	bl	sub_080b0030
.L_08021192:
	mov	r0, r8
	bl	sub_080b0020
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080211bc
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	movs	r3, #1
	subs	r7, #1
	negs	r2, r2
	mov	sl, r3
	cmp	r7, r2
	bne.n	.L_080211ba
	movs	r7, #2
.L_080211ba:
	ldr	r1, [pc, #104]
.L_080211bc:
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080211dc
	movs	r0, #111
	bl	sub_080f9010
	adds	r7, #1
	movs	r3, #1
	mov	sl, r3
	cmp	r7, #3
	bne.n	.L_080211da
	movs	r7, #0
.L_080211da:
	ldr	r1, [pc, #72]
.L_080211dc:
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080211ea
	b.n	.L_080210f8
.L_080211ea:
	ldr	r3, [r1, #0]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021144
	movs	r0, #112
	bl	sub_080f9010
.L_080211fa:
	movs	r1, #2
	adds	r0, r6, #0
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #8]
	bl	sub_08003f3c
	adds	r0, r7, #0
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.2byte 0x1b04
	.2byte 0x0300
