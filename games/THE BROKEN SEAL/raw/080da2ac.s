.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5028, 0x080b5028
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdb24, 0x080cdb24
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080da2ac
Overlay_080da2ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #116
	str	r3, [sp, #72]
	subs	r2, #108
	ldr	r3, [pc, #84]
	mov	r9, r1
	ldr	r2, [r2, #0]
	add	r3, r9
	str	r2, [sp, #56]
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #52]
	movs	r1, #240
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #48
	lsls	r1, r1, #5
	strh	r3, [r2, #0]
	ldr	r0, [pc, #60]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #128
	ldr	r3, [pc, #48]
	lsls	r2, r2, #8
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #52]
	bl	sub_080072f0
	ldr	r3, [pc, #40]
	movs	r2, #7
	movs	r5, #0
	mov	ip, r2
	movs	r7, #0
	mov	lr, r3
	b.n	.L_080da338
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000008b
	.4byte 0x03001388
	.4byte 0x02010000
	.2byte 0x8000
	.2byte 0x0600
.L_080da338:
	adds	r4, r5, #0
	adds	r4, #96
	adds	r3, r4, #0
	mov	r6, ip
	ands	r3, r6
	lsls	r3, r3, #3
	mov	r8, r3
	adds	r3, r7, r5
	lsls	r3, r3, #3
	mov	r2, r9
	movs	r0, #0
	adds	r1, r3, r2
.L_080da350:
	adds	r3, r0, #0
	adds	r3, #32
	adds	r2, r3, #0
	mov	r6, ip
	ands	r2, r6
	cmp	r3, #0
	bge.n	.L_080da360
	adds	r3, #7
.L_080da360:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r3, r2, r3
	mov	r6, r8
	adds	r2, r3, r6
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L_080da372
	adds	r3, #7
.L_080da372:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	adds	r3, r2, r3
	mov	r2, lr
	ldrb	r3, [r3, r2]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r0, #40
	bne.n	.L_080da350
	adds	r5, #1
	adds	r7, #4
	cmp	r5, #16
	bne.n	.L_080da338
	ldr	r3, [pc, #780]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080da3a8
	ldr	r3, [pc, #772]
	ldr	r2, [pc, #772]
	str	r3, [r2, #0]
	movs	r3, #112
	negs	r3, r3
	str	r3, [sp, #52]
	b.n	.L_080da3ac
.L_080da3a8:
	movs	r4, #0
	str	r4, [sp, #52]
.L_080da3ac:
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	bl	sub_080ed408
	ldr	r3, [pc, #744]
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #60]
	ldr	r3, [pc, #724]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080da3ea
	ldr	r3, [pc, #712]
	ldr	r2, [pc, #724]
	add	r3, r9
	ldr	r1, [r3, #0]
	movs	r0, #0
	add	r2, r9
.L_080da3de:
	str	r0, [r2, #0]
	ldr	r3, [r1, #20]
	adds	r4, #1
	adds	r2, #28
	cmp	r4, r3
	bne.n	.L_080da3de
.L_080da3ea:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #696]
	movs	r3, #0
	add	r2, r9
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #688]
	bl	sub_080041d8
	mov	r6, sp
	adds	r6, #100
	str	r6, [sp, #48]
	ldr	r3, [pc, #680]
	adds	r2, r6, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	ldr	r2, [pc, #672]
	movs	r3, #128
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #141
	bl	sub_080f9010
	movs	r6, #0
	ldr	r3, [pc, #624]
	str	r6, [sp, #68]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #652]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080da440
	b.n	.L_080da67a
.L_080da440:
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #600]
	adds	r0, #12
	add	r1, r9
	str	r0, [sp, #28]
	str	r1, [sp, #44]
.L_080da44c:
	bl	sub_080049ac
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #28]
	bl	sub_080051d8
	ldr	r2, [sp, #44]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	ldr	r4, [pc, #608]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r4, r3]
	ldr	r6, [sp, #68]
	subs	r3, #16
	cmp	r6, r3
	bne.n	.L_080da474
	movs	r0, #133
	bl	sub_080b50e8
.L_080da474:
	movs	r0, #0
	movs	r1, #8
	movs	r2, #16
	str	r0, [sp, #64]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r0, [sp, #12]
.L_080da484:
	ldr	r4, [sp, #68]
	movs	r3, #31
	ands	r3, r4
	ldr	r6, [sp, #16]
	adds	r3, #32
	cmp	r3, r6
	bne.n	.L_080da49c
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #12]
	ldr	r3, [r0, r1]
	adds	r3, #32
	str	r3, [r0, r1]
.L_080da49c:
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bge.n	.L_080da4a6
	b.n	.L_080da5ae
.L_080da4a6:
	ldr	r6, [pc, #500]
	add	r6, r9
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r4, [pc, #524]
	adds	r3, #1
	ldrb	r3, [r4, r3]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #68]
	adds	r3, r0, r3
	cmp	r1, r3
	bge.n	.L_080da5ae
	subs	r0, r1, r0
	lsls	r0, r0, #10
	bl	sub_08002322
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #48]
	ldr	r3, [r2, r4]
	muls	r3, r0
	asrs	r5, r3, #16
	cmp	r5, #0
	bge.n	.L_080da4da
	negs	r5, r5
.L_080da4da:
	movs	r3, #112
	movs	r1, #240
	subs	r3, r3, r5
	movs	r0, #40
	lsls	r1, r1, #5
	str	r3, [sp, #40]
	str	r0, [sp, #0]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #60]
	str	r5, [sp, #4]
	ldr	r0, [sp, #72]
	add	r1, r9
	bl	sub_080072f4
	movs	r0, #40
	movs	r2, #16
	movs	r3, #96
	subs	r3, r3, r5
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #72]
	mov	r1, r9
	ldr	r2, [sp, #24]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080da5ae
	ldr	r2, [sp, #24]
	movs	r0, #76
	ldr	r1, [sp, #20]
	add	r0, sp
	adds	r2, #40
	mov	sl, r0
	movs	r3, #36
	str	r1, [sp, #36]
	str	r2, [sp, #32]
	adds	r7, r6, #0
	mov	r8, r3
	add	r6, sp, #88
	mov	fp, sl
.L_080da532:
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	str	r4, [sp, #8]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	mov	r1, fp
	str	r3, [r6, #8]
	adds	r0, r6, #0
	bl	sub_080e3944
	mov	r0, fp
	ldr	r3, [r0, #0]
	ldr	r1, [sp, #52]
	adds	r2, r3, r1
	str	r2, [r0, #0]
	ldr	r3, [sp, #36]
	adds	r3, #8
	ldr	r4, [sp, #8]
	cmp	r2, r3
	blt.n	.L_080da582
	ldr	r3, [sp, #32]
	cmp	r2, r3
	bgt.n	.L_080da582
	mov	r0, sl
	ldr	r3, [r0, #4]
	ldr	r1, [sp, #40]
	cmp	r3, r1
	blt.n	.L_080da582
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #40]
	ldr	r3, [pc, #324]
	str	r3, [r5, #72]
.L_080da582:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L_080da5a0
	ldr	r3, [r7, #0]
	mov	r2, r8
	ldrsh	r0, [r3, r2]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r4, [sp, #8]
.L_080da5a0:
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #20]
	movs	r2, #2
	adds	r4, #1
	add	r8, r2
	cmp	r4, r3
	bne.n	.L_080da532
.L_080da5ae:
	ldr	r3, [sp, #20]
	ldr	r4, [sp, #24]
	ldr	r6, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #64]
	adds	r3, #40
	adds	r4, #40
	adds	r6, #4
	adds	r0, #4
	adds	r1, #1
	str	r3, [sp, #20]
	str	r4, [sp, #24]
	str	r6, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #64]
	cmp	r1, #3
	beq.n	.L_080da5d2
	b.n	.L_080da484
.L_080da5d2:
	ldr	r2, [pc, #200]
	mov	r6, r9
	ldr	r3, [r6, r2]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080da636
	movs	r6, #225
	lsls	r6, r6, #7
	add	r6, r9
	movs	r7, #36
.L_080da5e8:
	mov	r0, r9
	adds	r5, r0, r2
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	str	r4, [sp, #8]
	bl	sub_080b5098
	ldr	r3, [r6, #24]
	ldr	r0, [r0, #0]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080da624
	ldr	r3, [r0, #12]
	cmp	r3, #0
	bgt.n	.L_080da624
	ldr	r3, [r0, #40]
	cmp	r3, #0
	bge.n	.L_080da624
	movs	r3, #1
	str	r3, [r6, #24]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	bl	sub_080d6888
	ldr	r4, [sp, #8]
.L_080da624:
	ldr	r2, [pc, #116]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #28
	adds	r7, #2
	cmp	r4, r3
	bne.n	.L_080da5e8
.L_080da636:
	ldr	r1, [sp, #44]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #124]
	adds	r3, #2
	ldrb	r1, [r2, r3]
	adds	r0, r1, #0
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [sp, #68]
	adds	r3, #1
	str	r3, [sp, #68]
	ldr	r4, [sp, #44]
	ldr	r3, [r4, #0]
	ldr	r2, [r3, #24]
	ldr	r6, [pc, #84]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #68]
	cmp	r0, r3
	beq.n	.L_080da67a
	b.n	.L_080da44c
.L_080da67a:
	ldr	r0, [pc, #56]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007828
	.4byte 0xffff9000
	.4byte 0x04000028
	.4byte 0x03001e50
	.4byte 0x00007098
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080eda88
	.4byte 0x000077a8
	.4byte 0x080eea38
	.4byte 0x0000ab85
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #44]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #56
	str	r3, [sp, #32]
	adds	r3, r2, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #24]
	ldr	r2, [r2, #8]
	ldr	r3, [pc, #24]
	str	r2, [sp, #20]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_080da710
	movs	r0, #1
	bl	sub_080cd594
	b.n	.L_080da716
	.4byte 0x03001eec
	.2byte 0x7828
	.2byte 0x0000
.L_080da710:
	movs	r0, #0
	bl	sub_080cd594
.L_080da716:
	ldr	r0, [pc, #128]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r0, [pc, #120]
	ldr	r1, [sp, #20]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #36
	ldr	r5, [pc, #68]
	str	r2, [sp, #12]
	str	r3, [r2, #4]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	add	r5, r9
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	r8, r0
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	mov	r5, r8
	ldr	r3, [r5, #12]
	adds	r3, r3, r0
	movs	r1, #127
	movs	r0, #0
	str	r3, [sp, #16]
	ldr	r7, [pc, #28]
	mov	sl, r0
	mov	fp, r1
	b.n	.L_080da7b0
	.4byte 0x00001010
	.4byte 0x000000b4
	.4byte 0x00000073
	.4byte 0x03001e50
	.4byte 0x00007828
	.4byte 0x04000052
	.2byte 0x0000
	.2byte 0x0201
.L_080da7b0:
	bl	sub_08004458
	adds	r6, r0, #0
	bl	sub_08004458
	mov	r2, fp
	adds	r5, r0, #0
	adds	r0, r6, #0
	ands	r5, r2
	bl	sub_08002322
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	bl	sub_08004458
	mov	r3, fp
	ands	r0, r3
	subs	r0, #16
	lsls	r0, r0, #16
	asrs	r0, r0, #6
	str	r0, [r7, #16]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #20]
	mov	r5, r8
	ldr	r3, [r5, #8]
	str	r3, [r7, #0]
	ldr	r0, [sp, #16]
	str	r0, [r7, #4]
	ldr	r3, [r5, #16]
	movs	r1, #1
	str	r3, [r7, #8]
	add	sl, r1
	movs	r3, #1
	negs	r3, r3
	mov	r2, sl
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_080da7b0
	ldr	r3, [pc, #804]
	movs	r2, #0
	add	r3, r9
	str	r2, [r3, #0]
	ldr	r3, [pc, #800]
	movs	r5, #144
	add	r3, r9
	lsls	r5, r5, #3
	str	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #792]
	bl	sub_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #780]
	movs	r3, #75
	add	r2, r9
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #772]
	bl	sub_080041d8
	movs	r3, #0
	str	r3, [sp, #28]
	ldr	r3, [pc, #768]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #764]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	movs	r5, #132
	lsrs	r3, r3, #1
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_080da85e
	b.n	.L_080dab06
.L_080da85e:
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #740]
	adds	r0, #12
	add	r1, r9
	str	r0, [sp, #8]
	mov	fp, r1
.L_080da86a:
	ldr	r3, [sp, #28]
	subs	r3, #17
	cmp	r3, #62
	bhi.n	.L_080da87c
	ldr	r2, [pc, #704]
	movs	r3, #128
	add	r2, r9
	lsls	r3, r3, #1
	b.n	.L_080da882
.L_080da87c:
	ldr	r2, [pc, #692]
	movs	r3, #0
	add	r2, r9
.L_080da882:
	str	r3, [r2, #0]
	ldr	r5, [pc, #704]
	add	r5, r9
	ldr	r3, [r5, #0]
	ldr	r6, [pc, #704]
	ldr	r3, [r3, #24]
	ldrb	r3, [r6, r3]
	ldr	r2, [sp, #28]
	lsrs	r3, r3, #1
	adds	r3, #108
	cmp	r2, r3
	bne.n	.L_080da8a0
	movs	r0, #133
	bl	sub_080b50e8
.L_080da8a0:
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	bl	sub_080b5028
	bl	sub_080049ac
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldrb	r3, [r6, r3]
	adds	r2, r6, #0
	cmp	r3, #0
	bne.n	.L_080da8ca
	b.n	.L_080daa5c
.L_080da8ca:
	ldr	r6, [pc, #644]
.L_080da8cc:
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	r8, r3
	ldr	r0, [sp, #28]
	mov	r7, r8
	adds	r7, #48
	cmp	r0, r8
	ble.n	.L_080da978
	movs	r1, #1
	ldr	r3, [r6, #24]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080da974
	add	r5, sp, #44
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_080da904
	movs	r3, #160
	str	r3, [r5, #8]
.L_080da904:
	ldr	r2, [pc, #588]
	cmp	r3, r2
	ble.n	.L_080da90e
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080da90e:
	adds	r2, r3, #0
	subs	r2, #160
	cmp	r2, #0
	bge.n	.L_080da918
	adds	r2, #63
.L_080da918:
	asrs	r2, r2, #6
	movs	r3, #10
	subs	r4, r3, r2
	mov	r7, r8
	ldr	r3, [sp, #28]
	movs	r2, #4
	adds	r7, #48
	mov	ip, r2
	cmp	r3, r7
	blt.n	.L_080da930
	movs	r0, #0
	mov	ip, r0
.L_080da930:
	lsls	r0, r4, #1
	ldr	r2, [pc, #548]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #12]
	mov	r5, ip
	subs	r3, r3, r4
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_080da978
.L_080da974:
	mov	r7, r8
	adds	r7, #48
.L_080da978:
	ldr	r1, [sp, #28]
	cmp	r1, r7
	ble.n	.L_080daa46
	movs	r2, #1
	ldr	r3, [r6, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080daa46
	mov	r3, fp
	ldr	r5, [r3, #0]
	mov	r0, sl
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	lsls	r0, r0, #1
	adds	r0, #36
	ldrsh	r0, [r5, r0]
	bl	sub_080b5098
	ldr	r1, [r0, #0]
	ldr	r2, [r6, #0]
	ldr	r3, [r1, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #9
	adds	r0, r2, r3
	str	r0, [r6, #12]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #12]
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #9
	adds	r4, r2, r3
	str	r4, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #16]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #9
	adds	r1, r2, r3
	str	r1, [r6, #20]
	mov	r3, r8
	ldr	r2, [sp, #28]
	adds	r3, #85
	cmp	r2, r3
	bge.n	.L_080daa04
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080da9e0
	adds	r2, #63
.L_080da9e0:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080da9f0
	adds	r2, #63
.L_080da9f0:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080daa00
	adds	r2, #63
.L_080daa00:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_080daa04:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080daa46
	movs	r3, #0
	str	r3, [r6, #24]
	add	r2, sp, #44
	ldr	r3, [r2, #0]
	str	r3, [r6, #0]
	ldr	r3, [r2, #4]
	movs	r0, #136
	str	r3, [r6, #4]
	bl	sub_080f9010
	mov	r3, fp
	ldr	r5, [r3, #0]
	mov	r0, sl
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	adds	r3, r0, #0
	lsls	r2, r3, #1
	adds	r2, #36
	ldrsh	r0, [r5, r2]
	movs	r2, #4
	str	r2, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	bl	sub_080d6888
	ldr	r2, [pc, #284]
	movs	r3, #2
	add	r2, r9
	str	r3, [r2, #0]
.L_080daa46:
	mov	r5, fp
	ldr	r3, [r5, #0]
	movs	r2, #1
	add	sl, r2
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #248]
	ldrb	r3, [r2, r3]
	adds	r6, #28
	cmp	sl, r3
	beq.n	.L_080daa5c
	b.n	.L_080da8cc
.L_080daa5c:
	mov	r1, fp
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080daac2
	ldr	r6, [pc, #224]
.L_080daa6e:
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bhi.n	.L_080daab0
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	asrs	r4, r4, #1
	ldr	r2, [pc, #228]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #224]
	ldrb	r5, [r3, r4]
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #220]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #212]
	ldrb	r0, [r0, r4]
	ldr	r5, [sp, #12]
	str	r0, [sp, #4]
	subs	r3, #56
	add	r1, r9
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	ldr	r2, [pc, #156]
.L_080daab0:
	mov	r1, fp
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	movs	r0, #1
	ldrb	r3, [r2, r3]
	add	sl, r0
	adds	r6, #28
	cmp	sl, r3
	bne.n	.L_080daa6e
.L_080daac2:
	ldr	r2, [pc, #116]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080daad0
	movs	r3, #1
	str	r3, [r2, #0]
.L_080daad0:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #144]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	mov	r5, fp
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	ldr	r0, [sp, #28]
	lsrs	r3, r3, #1
	adds	r3, #132
	cmp	r0, r3
	beq.n	.L_080dab06
	b.n	.L_080da86a
.L_080dab06:
	ldr	r0, [pc, #60]
	bl	sub_08004278
	ldr	r0, [pc, #44]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000077ac
	.4byte 0x000077b0
	.4byte 0x080d6505
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x080eea41
	.4byte 0x02010000
	.4byte 0x0000031f
	.4byte 0x080ede48
	.4byte 0x000077a8
	.4byte 0x080eea56
	.4byte 0x080eea44
	.4byte 0x080eea50
	.4byte 0x080eea4a
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #92]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	sub	sp, #96
	str	r1, [sp, #48]
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	ldr	r2, [r2, #8]
	str	r2, [sp, #36]
	ldr	r2, [pc, #76]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #48]
	ldr	r0, [pc, #64]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	mov	r3, sp
	adds	r3, #52
	movs	r0, #0
	adds	r1, r3, #0
	str	r3, [sp, #32]
	bl	sub_080cef64
	movs	r1, #225
	ldr	r0, [sp, #48]
	movs	r6, #0
	lsls	r1, r1, #7
	mov	sl, r6
	adds	r5, r0, r1
	b.n	.L_080dabf4
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000b8
	.2byte 0x00ba
	.2byte 0x0000
.L_080dabf4:
	bl	sub_08004458
	ldr	r3, [pc, #808]
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #8]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	subs	r3, #64
	movs	r2, #1
	lsls	r3, r3, #16
	add	sl, r2
	str	r3, [r5, #4]
	mov	r3, sl
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_080dabf4
	ldr	r6, [sp, #48]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #764]
	adds	r2, r6, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #50
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #748]
	bl	sub_080041d8
	ldr	r2, [pc, #748]
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_080dac54
	ldr	r2, [pc, #740]
	ldr	r3, [pc, #740]
	str	r3, [r2, #0]
.L_080dac54:
	movs	r3, #0
	str	r3, [sp, #40]
	ldr	r3, [r1, #24]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r0, [pc, #732]
	adds	r2, #2
	movs	r6, #75
	ldrb	r3, [r0, r2]
	negs	r6, r6
	cmp	r3, r6
	bne.n	.L_080dac6e
	b.n	.L_080db214
.L_080dac6e:
	ldr	r3, [pc, #704]
	ldr	r2, [sp, #48]
	mov	r1, sp
	adds	r1, #60
	adds	r3, r2, r3
	str	r1, [sp, #8]
	str	r3, [sp, #16]
.L_080dac7c:
	movs	r6, #240
	lsls	r6, r6, #15
	movs	r1, #0
	str	r6, [sp, #24]
	str	r1, [sp, #20]
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r0, r3]
	ldr	r6, [sp, #40]
	adds	r3, #11
	cmp	r6, r3
	bne.n	.L_080daca2
	movs	r0, #132
	bl	sub_080b50e8
.L_080daca2:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r0, [r1, #0]
	str	r0, [r1, #4]
	str	r3, [r1, #8]
	bl	sub_080049ac
	ldr	r0, [sp, #8]
	bl	sub_08004cb4
	ldr	r2, [sp, #40]
	subs	r2, #36
	str	r2, [sp, #12]
	cmp	r2, #27
	bhi.n	.L_080dacd4
	ldr	r6, [sp, #40]
	movs	r3, #3
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080dacd4
	movs	r0, #115
	bl	sub_080f9010
.L_080dacd4:
	ldr	r0, [sp, #40]
	cmp	r0, #85
	bne.n	.L_080dace0
	movs	r0, #136
	bl	sub_080f9010
.L_080dace0:
	ldr	r2, [pc, #588]
	ldr	r6, [sp, #48]
	ldr	r3, [r6, r2]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_080dad22
	movs	r6, #36
	movs	r5, #40
.L_080dacf4:
	ldr	r0, [sp, #40]
	cmp	r0, r5
	bne.n	.L_080dad0e
	ldr	r1, [sp, #48]
	ldr	r3, [r1, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #5
	subs	r3, #1
	bl	sub_080d6888
.L_080dad0e:
	ldr	r2, [pc, #544]
	ldr	r0, [sp, #48]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #4
	cmp	sl, r3
	bne.n	.L_080dacf4
.L_080dad22:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	ldr	r1, [pc, #528]
	adds	r2, r3, r2
	adds	r3, r2, #2
	ldrb	r3, [r1, r3]
	ldr	r0, [sp, #40]
	movs	r6, #16
	mov	r9, r1
	str	r6, [sp, #28]
	cmp	r0, r3
	bge.n	.L_080dad42
	ldrb	r2, [r1, r2]
	str	r2, [sp, #28]
.L_080dad42:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	ldr	r6, [sp, #40]
	adds	r3, #35
	cmp	r6, r3
	blt.n	.L_080dad5a
	b.n	.L_080dae90
.L_080dad5a:
	ldr	r1, [sp, #28]
	movs	r0, #0
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L_080dad66
	b.n	.L_080dae90
.L_080dad66:
	ldr	r6, [sp, #48]
	movs	r0, #225
	movs	r2, #84
	movs	r3, #72
	lsls	r0, r0, #7
	add	r2, sp
	add	r3, sp
	adds	r6, r6, r0
	mov	fp, r2
	mov	r9, r3
	mov	r8, r6
.L_080dad7c:
	ldr	r1, [sp, #40]
	cmp	r1, sl
	ble.n	.L_080dae7c
	mov	r1, sl
	cmp	r1, #0
	bge.n	.L_080dad8a
	adds	r1, #7
.L_080dad8a:
	asrs	r7, r1, #3
	mov	r2, sl
	lsls	r3, r7, #3
	subs	r7, r2, r3
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	r6, r8
	movs	r2, #48
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L_080dae10
	ldr	r0, [pc, #408]
	cmp	r3, r0
	ble.n	.L_080dae10
	ldr	r0, [r6, #0]
	bl	sub_08002322
	ldr	r3, [r6, #8]
	muls	r3, r0
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r6, #4]
	str	r3, [r1, #4]
	ldr	r0, [r6, #0]
	bl	sub_0800231c
	ldr	r3, [r6, #8]
	muls	r3, r0
	mov	r2, fp
	str	r3, [r2, #8]
	mov	r1, r9
	mov	r0, fp
	bl	sub_080e3944
	mov	r3, r9
	ldr	r2, [r3, #0]
	asrs	r2, r2, #17
	adds	r2, #64
	str	r2, [r3, #0]
	movs	r0, #6
	ldrsh	r3, [r3, r0]
	mov	r1, r9
	adds	r3, #60
	ldr	r0, [pc, #348]
	str	r3, [r1, #4]
	lsls	r1, r7, #1
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #48]
	adds	r1, r0, r1
	ldr	r0, [pc, #340]
	ldrb	r5, [r0, r7]
	lsrs	r0, r5, #1
	subs	r2, r2, r0
	ldr	r0, [pc, #336]
	ldrb	r4, [r0, r7]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #32]
	str	r4, [sp, #4]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
.L_080dae10:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #288]
	adds	r3, #2
	ldrb	r3, [r2, r3]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	bge.n	.L_080dae54
	mov	r3, sl
	adds	r3, #16
	cmp	r0, r3
	ble.n	.L_080dae7c
	ldr	r3, [r6, #8]
	cmp	r3, #4
	ble.n	.L_080dae38
	subs	r3, #2
	str	r3, [r6, #8]
.L_080dae38:
	ldr	r3, [r6, #4]
	ldr	r1, [pc, #276]
	cmp	r3, r1
	bgt.n	.L_080dae48
	movs	r2, #160
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #4]
.L_080dae48:
	ldr	r3, [r6, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r3, r3, r0
	str	r3, [r6, #0]
	b.n	.L_080dae7c
.L_080dae54:
	ldr	r3, [r6, #8]
	adds	r3, #8
	str	r3, [r6, #8]
	movs	r1, #5
	mov	r0, sl
	bl	sub_080022fc
	ldr	r3, [r6, #4]
	adds	r0, #2
	lsls	r0, r0, #16
	subs	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r1, [sp, #24]
	cmp	r1, r3
	ble.n	.L_080dae74
	str	r3, [sp, #24]
.L_080dae74:
	ldr	r2, [sp, #20]
	cmp	r2, r3
	bge.n	.L_080dae7c
	str	r3, [sp, #20]
.L_080dae7c:
	movs	r6, #1
	ldr	r0, [sp, #28]
	movs	r3, #28
	add	sl, r6
	add	r8, r3
	cmp	sl, r0
	beq.n	.L_080dae8c
	b.n	.L_080dad7c
.L_080dae8c:
	ldr	r1, [pc, #172]
	mov	r9, r1
.L_080dae90:
	ldr	r2, [sp, #24]
	ldr	r6, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #15
	adds	r2, r2, r3
	adds	r6, r6, r3
	str	r6, [sp, #20]
	str	r2, [sp, #24]
	ldr	r0, [sp, #16]
	ldr	r3, [r0, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r2, r3, r2
	adds	r3, r2, #2
	mov	r1, r9
	ldrb	r3, [r1, r3]
	ldr	r6, [sp, #40]
	cmp	r6, r3
	bge.n	.L_080dafa0
	adds	r3, r2, #1
	ldrb	r3, [r1, r3]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080dafa0
	ldr	r5, [pc, #144]
	ldr	r7, [pc, #144]
	mov	r8, r5
	movs	r6, #0
.L_080daeca:
	ldr	r0, [sp, #12]
	movs	r1, #3
	bl	sub_080022ec
	cmp	sl, r0
	bge.n	.L_080daf84
	movs	r1, #3
	mov	r0, sl
	bl	sub_080022fc
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r2, r9
	ldrb	r3, [r2, r3]
	adds	r4, r0, #0
	ldr	r0, [sp, #40]
	subs	r3, #7
	cmp	r0, r3
	blt.n	.L_080daf60
	ldr	r0, [pc, #96]
	lsls	r3, r4, #1
	ldrh	r1, [r7, r3]
	ldrb	r4, [r0, r4]
	ldr	r2, [sp, #48]
	mov	r3, r8
	movs	r0, #32
	adds	r1, r2, r1
	ldrb	r2, [r6, r3]
	ldrb	r3, [r5, #1]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	ldr	r1, [pc, #28]
	mov	r9, r1
	b.n	.L_080daf84
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x080eea88
	.4byte 0xffd00000
	.4byte 0x080eeaa2
	.4byte 0x080eea91
	.4byte 0x080eea99
	.4byte 0x002fffff
	.4byte 0x080eea62
	.4byte 0x080eeab2
	.2byte 0xeab8
	.2byte 0x080e
.L_080daf60:
	ldr	r0, [pc, #728]
	lsls	r3, r4, #1
	ldrh	r1, [r7, r3]
	ldrb	r4, [r0, r4]
	ldrb	r3, [r5, #1]
	ldr	r2, [sp, #48]
	movs	r0, #32
	subs	r3, r3, r4
	adds	r1, r2, r1
	ldrb	r2, [r5, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #52]
	bl	sub_080072f4
	ldr	r3, [pc, #700]
	mov	r9, r3
.L_080daf84:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #1
	mov	r2, r9
	movs	r0, #1
	ldrb	r3, [r2, r3]
	add	sl, r0
	adds	r5, #2
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_080daeca
.L_080dafa0:
	ldr	r6, [sp, #16]
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r0, r9
	ldrb	r3, [r0, r3]
	ldr	r1, [sp, #40]
	cmp	r1, r3
	bne.n	.L_080db00c
	movs	r2, #0
	ldr	r5, [pc, #648]
	mov	sl, r2
	movs	r6, #15
.L_080dafbe:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #80
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #12
	str	r3, [r5, #8]
	bl	sub_08004458
	negs	r0, r0
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #1
	ands	r0, r6
	adds	r0, #16
	add	sl, r3
	str	r0, [r5, #24]
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_080dafbe
	ldr	r1, [pc, #564]
	mov	r9, r1
.L_080db00c:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r6, r9
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	blt.n	.L_080db09e
	movs	r1, #0
	ldr	r5, [pc, #540]
	mov	sl, r1
.L_080db028:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080db07c
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080db036
	adds	r3, #7
.L_080db036:
	asrs	r4, r3, #3
	lsls	r3, r4, #3
	mov	r2, sl
	subs	r4, r2, r3
	ldr	r2, [pc, #520]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #48]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	ldr	r0, [pc, #508]
	ldrb	r0, [r0, r4]
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	str	r0, [sp, #0]
	ldr	r0, [pc, #504]
	ldrb	r0, [r0, r4]
	ldr	r6, [sp, #32]
	str	r0, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [r6, #4]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080db07c:
	ldr	r3, [r5, #4]
	ldr	r0, [sp, #24]
	cmp	r0, r3
	ble.n	.L_080db086
	str	r3, [sp, #24]
.L_080db086:
	ldr	r1, [sp, #20]
	cmp	r1, r3
	bge.n	.L_080db08e
	str	r3, [sp, #20]
.L_080db08e:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r5, #28
	cmp	r3, #24
	bne.n	.L_080db028
	ldr	r6, [pc, #420]
	mov	r9, r6
.L_080db09e:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	cmp	r1, r0
	bgt.n	.L_080db0b2
	adds	r0, #1
	str	r0, [sp, #20]
.L_080db0b2:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r6, r9
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	bne.n	.L_080db120
	ldr	r2, [sp, #48]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	sl, r1
	adds	r5, r2, r3
.L_080db0d4:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	ldr	r6, [sp, #20]
	ldr	r0, [sp, #24]
	cmp	r6, r0
	bne.n	.L_080db0ee
	lsls	r3, r0, #16
	str	r3, [r5, #16]
	b.n	.L_080db104
.L_080db0ee:
	bl	sub_08004458
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #24]
	subs	r1, r2, r3
	bl	sub_08002304
	ldr	r6, [sp, #24]
	adds	r0, r0, r6
	lsls	r0, r0, #16
	str	r0, [r5, #16]
.L_080db104:
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	sl, r0
	adds	r3, #20
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_080db0d4
	ldr	r2, [pc, #288]
	mov	r9, r2
.L_080db120:
	ldr	r6, [sp, #16]
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	mov	r0, r9
	ldrb	r3, [r0, r3]
	ldr	r1, [sp, #40]
	cmp	r1, r3
	blt.n	.L_080db1e4
	subs	r3, r1, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r8, r3
	movs	r6, #225
	ldr	r3, [sp, #48]
	movs	r2, #0
	lsls	r6, r6, #7
	ldr	r7, [pc, #244]
	mov	sl, r2
	adds	r5, r3, r6
.L_080db14e:
	ldr	r3, [r5, #24]
	cmp	r3, #17
	bhi.n	.L_080db18e
	movs	r0, #17
	subs	r0, r0, r3
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	ldr	r2, [pc, #244]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #36]
	adds	r1, r2, r1
	movs	r3, #14
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #232]
	ldrb	r4, [r3, r0]
	movs	r6, #18
	ldrsh	r3, [r5, r6]
	lsrs	r0, r4, #1
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r6, [sp, #32]
	subs	r2, r2, r0
	subs	r3, r3, r0
	mov	r0, r8
	subs	r3, r3, r0
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
.L_080db18e:
	movs	r0, #1
	subs	r3, #1
	negs	r0, r0
	str	r3, [r5, #24]
	cmp	r3, r0
	beq.n	.L_080db19e
	cmp	r3, #17
	bne.n	.L_080db1d8
.L_080db19e:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r7, r3]
	ldr	r2, [sp, #40]
	adds	r3, #35
	cmp	r2, r3
	bge.n	.L_080db1d8
	movs	r3, #17
	str	r3, [r5, #24]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	bl	sub_08004458
	ldr	r6, [sp, #24]
	ldr	r3, [sp, #20]
	subs	r1, r3, r6
	bl	sub_08002304
	adds	r0, r0, r6
	lsls	r0, r0, #16
	str	r0, [r5, #16]
.L_080db1d8:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_080db14e
.L_080db1e4:
	ldr	r3, [sp, #48]
	ldr	r6, [pc, #116]
	adds	r2, r3, r6
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r0, [pc, #56]
	adds	r3, #2
	ldrb	r3, [r0, r3]
	ldr	r2, [sp, #40]
	adds	r3, #75
	cmp	r2, r3
	beq.n	.L_080db214
	b.n	.L_080dac7c
.L_080db214:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #60]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080eeab8
	.4byte 0x080eea88
	.4byte 0x02010000
	.4byte 0x080eeacc
	.4byte 0x080eeabb
	.4byte 0x080eeac3
	.4byte 0x080ede84
	.4byte 0x080ede96
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #48]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #44
	str	r3, [sp, #32]
	ldr	r5, [pc, #40]
	mov	sl, r1
	ldr	r2, [r2, #8]
	add	r5, sl
	str	r2, [sp, #24]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	sub_080cdb24
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L_080db2b0
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #4]
	b.n	.L_080db2b4
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080db2b0:
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #56]
.L_080db2b4:
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r5, [pc, #44]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	ldr	r1, [pc, #16]
	adds	r2, #36
	b.n	.L_080db2fc
	.4byte 0x00000100
	.4byte 0x04000020
	.4byte 0x03001e50
	.2byte 0x060e
	.2byte 0x0000
.L_080db2fc:
	str	r2, [sp, #12]
	ldr	r0, [pc, #880]
	str	r3, [r2, #4]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #864]
	ldr	r1, [sp, #24]
	bl	sub_080e0524
	ldr	r3, [pc, #860]
	add	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	.L_080db338
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080db330
	ldr	r2, [pc, #848]
	ldr	r3, [pc, #848]
	b.n	.L_080db342
.L_080db330:
	ldr	r2, [pc, #840]
	movs	r3, #128
	lsls	r3, r3, #5
	b.n	.L_080db342
.L_080db338:
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080db344
	ldr	r2, [pc, #828]
	ldr	r3, [pc, #832]
.L_080db342:
	str	r3, [r2, #0]
.L_080db344:
	movs	r3, #0
	ldr	r7, [pc, #832]
	mov	r8, r3
.L_080db34a:
	bl	sub_08004458
	ldr	r6, [pc, #828]
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08004458
	ldr	r5, [pc, #816]
	movs	r3, #128
	ldr	r2, [pc, #816]
	lsls	r3, r3, #7
	str	r3, [r7, #0]
	ands	r5, r0
	movs	r3, #224
	adds	r5, r5, r2
	lsls	r3, r3, #7
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #16
	str	r3, [r7, #20]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r1, #128
	movs	r3, #1
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_080db34a
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #744]
	movs	r3, #75
	add	r2, sl
	str	r3, [r2, #0]
	adds	r1, #128
	ldr	r0, [pc, #740]
	bl	sub_080041d8
	movs	r0, #138
	bl	sub_080f9010
	movs	r2, #0
	str	r2, [sp, #28]
.L_080db3c6:
	ldr	r3, [sp, #28]
	cmp	r3, #20
	bne.n	.L_080db3d2
	movs	r0, #133
	bl	sub_080b50e8
.L_080db3d2:
	ldr	r1, [sp, #28]
	cmp	r1, #15
	ble.n	.L_080db3da
	b.n	.L_080db55a
.L_080db3da:
	adds	r0, r1, #0
	movs	r1, #5
	bl	sub_080022fc
	cmp	r0, #2
	bne.n	.L_080db3f4
	movs	r1, #128
	ldr	r3, [pc, #692]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #692]
	bl	sub_080072f0
.L_080db3f4:
	ldr	r3, [sp, #28]
	movs	r2, #0
	lsls	r3, r3, #11
	str	r2, [sp, #20]
	str	r3, [sp, #8]
.L_080db3fe:
	movs	r1, #0
	ldr	r2, [sp, #8]
	str	r1, [sp, #16]
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r2, r1
	ldr	r2, [sp, #20]
	adds	r5, r2, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	ldr	r1, [sp, #28]
	movs	r3, #32
	subs	r3, r3, r1
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #64
	adds	r0, r5, #0
	mov	r9, r3
	bl	sub_0800231c
	ldr	r3, [pc, #588]
	add	r3, sl
	lsls	r0, r0, #3
	ldr	r3, [r3, #0]
	asrs	r0, r0, #16
	negs	r0, r0
	ldr	r3, [r3, #24]
	adds	r6, r0, #0
	subs	r6, #8
	cmp	r3, #0
	bne.n	.L_080db47a
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	ldr	r2, [pc, #596]
	lsls	r5, r5, #6
	add	r5, sl
	adds	r5, r5, r2
	bl	sub_08004458
	movs	r2, #7
	ands	r2, r0
	movs	r3, #24
	add	r2, r9
	str	r3, [sp, #0]
	movs	r3, #120
	str	r3, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	sub_080072f4
	b.n	.L_080db4bc
.L_080db47a:
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	lsls	r5, r5, #6
	ldr	r3, [pc, #536]
	add	r5, sl
	adds	r5, r5, r3
	bl	sub_08004458
	ldr	r3, [sp, #20]
	movs	r1, #1
	ands	r1, r3
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #120
	ands	r2, r0
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	lsls	r1, r1, #2
	add	r2, r9
	ldr	r4, [r1, r3]
	subs	r2, #16
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	sub_080072f4
.L_080db4bc:
	adds	r3, r6, #0
	adds	r3, #112
	movs	r1, #0
	lsls	r3, r3, #16
	ldr	r7, [pc, #448]
	mov	r8, r1
	mov	fp, r3
.L_080db4ca:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_080db538
	bl	sub_08004458
	ldr	r6, [pc, #468]
	ands	r6, r0
	bl	sub_08004458
	ldr	r5, [pc, #432]
	ldr	r2, [pc, #436]
	mov	r1, r9
	ands	r5, r0
	lsls	r3, r1, #16
	adds	r5, r5, r2
	mov	r2, fp
	str	r2, [r7, #4]
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	ldr	r3, [pc, #340]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #388]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r1, r3]
	ldr	r2, [sp, #16]
	cmp	r2, r3
	beq.n	.L_080db546
.L_080db538:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_080db4ca
.L_080db546:
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #4
	beq.n	.L_080db552
	b.n	.L_080db3fe
.L_080db552:
	ldr	r2, [pc, #352]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
.L_080db55a:
	movs	r3, #0
	ldr	r5, [pc, #296]
	mov	r8, r3
.L_080db560:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_080db5ca
	subs	r3, #1
	str	r3, [r5, #24]
	movs	r1, #60
	adds	r0, r5, #0
	ldr	r2, [pc, #328]
	bl	sub_080e3908
	movs	r1, #240
	ldr	r3, [r5, #4]
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_080db58c
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	.L_080db5ca
.L_080db58c:
	ldr	r2, [r5, #0]
	ldr	r1, [pc, #300]
	cmp	r2, r1
	bhi.n	.L_080db5ca
	cmp	r3, #0
	blt.n	.L_080db5ca
	ldr	r0, [r5, #24]
	asrs	r6, r2, #16
	asrs	r7, r3, #16
	cmp	r0, #0
	bge.n	.L_080db5a4
	adds	r0, #7
.L_080db5a4:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #276]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r3, r7, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, r6, r2
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
.L_080db5ca:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_080db560
	ldr	r3, [sp, #28]
	subs	r3, #4
	cmp	r3, #91
	bhi.n	.L_080db622
	movs	r2, #0
	mov	r8, r2
	ldr	r2, [pc, #144]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080db622
	movs	r6, #36
	movs	r5, #4
.L_080db5f4:
	ldr	r3, [sp, #28]
	cmp	r3, r5
	bne.n	.L_080db60e
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	sub_080d6888
.L_080db60e:
	ldr	r2, [pc, #104]
	movs	r3, #1
	mov	r1, sl
	add	r8, r3
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #4
	cmp	r8, r3
	bne.n	.L_080db5f4
.L_080db622:
	movs	r0, #2
	movs	r1, #4
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #148]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	cmp	r2, #64
	beq.n	.L_080db648
	b.n	.L_080db3c6
.L_080db648:
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000000c4
	.4byte 0x00000073
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xfffff000
	.4byte 0xffff8000
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007fff
	.4byte 0xffffc000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x0000060e
	.4byte 0x000001ff
	.4byte 0x080eeadc
	.4byte 0x000077a8
	.4byte 0xfffff800
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
