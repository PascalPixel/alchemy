.syntax unified
	.thumb
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080da2ac
	.thumb_func
Func_080da2ac:
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
	.4byte 0x00007824
