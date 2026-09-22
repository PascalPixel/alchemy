.syntax unified
	.thumb
	.set sub_08013438, 0x08013438
	.set sub_08014644, 0x08014644
	.global Overlay_080d0788
Overlay_080d0788:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #124]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #60
	adds	r4, r6, r1
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	ldr	r7, [r3, #32]
	cmp	r2, #0
	beq.n	.L_080d0814
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #61
	adds	r1, r6, r3
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L_080d07d4
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r0, [pc, #152]
	bl	sub_08014644
	movs	r1, #0
	movs	r2, #0
	movs	r0, #1
	bl	sub_08013438
	movs	r1, #151
	lsls	r1, r1, #4
	adds	r2, r7, r1
	movs	r3, #3
	strb	r3, [r2, #0]
	b.n	.L_080d084c
.L_080d07d4:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #59
	adds	r3, r6, r2
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #58
	adds	r5, r6, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #88]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	ldrsb	r3, [r5, r3]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #42
	adds	r3, r3, r0
	adds	r2, r6, r1
	strh	r3, [r2, #0]
.L_080d0814:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #42
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	cmp	r2, #79
	bls.n	.L_080d0838
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #200
	strh	r2, [r3, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L_080d084c
.L_080d0838:
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	ldr	r3, [pc, #12]
	subs	r3, r3, r2
	movs	r2, #131
	lsls	r2, r2, #1
	adds	r1, r7, r2
	strh	r3, [r1, #0]
.L_080d084c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000009f
	.4byte 0x080d0789
	.2byte 0x0528
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #124]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #60
	adds	r4, r6, r1
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	ldr	r7, [r3, #32]
	cmp	r2, #0
	beq.n	.L_080d08de
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #61
	adds	r1, r6, r3
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L_080d089e
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r0, [pc, #160]
	bl	sub_08014644
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	b.n	.L_080d0950
.L_080d089e:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #59
	adds	r3, r6, r2
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #58
	adds	r5, r6, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #104]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	ldrsb	r3, [r5, r3]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #42
	adds	r3, r3, r0
	adds	r2, r6, r1
	strh	r3, [r2, #0]
.L_080d08de:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #42
	adds	r3, r6, r2
	ldrh	r5, [r3, #0]
	cmp	r5, #79
	bls.n	.L_080d0900
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #200
	strh	r2, [r3, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #250
	b.n	.L_080d094e
.L_080d0900:
	cmp	r5, #0
	beq.n	.L_080d093c
	ldr	r3, [pc, #48]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d093c
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r2, r5, #0
	adds	r3, r7, r1
	adds	r2, #80
	strh	r2, [r3, #0]
	ldr	r3, [pc, #12]
	movs	r2, #131
	lsls	r2, r2, #1
	subs	r3, r3, r5
	adds	r1, r7, r2
	strh	r3, [r1, #0]
	b.n	.L_080d0950
	movs	r0, r0
	.4byte 0x00000050
	.4byte 0x080d085d
	.4byte 0x03000528
	.2byte 0x122c
	.2byte 0x0300
.L_080d093c:
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #159
.L_080d094e:
	strh	r3, [r2, #0]
.L_080d0950:
	pop	{r5, r6, r7, pc}
