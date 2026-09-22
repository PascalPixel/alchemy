.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_080044d0, 0x080044d0
	.set sub_08009088, 0x08009088
	.set sub_08009260, 0x08009260
	.set sub_08015130, 0x08015130
	.set sub_08077008, 0x08077008
	.set sub_080b6cb0, 0x080b6cb0
	.set sub_080b7b6c, 0x080b7b6c
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b7f70, 0x080b7f70
	.set sub_080b8000, 0x080b8000
	.set sub_080b8178, 0x080b8178
	.set sub_080b82c4, 0x080b82c4
	.set sub_080b9d34, 0x080b9d34
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080be02c, 0x080be02c
	.set sub_080c0cec, 0x080c0cec
	.set sub_080c0df4, 0x080c0df4
	.set sub_080c10e8, 0x080c10e8
	.set sub_080c2410, 0x080c2410
	.set sub_080c9008, 0x080c9008
	.set sub_080c9018, 0x080c9018
	.global Overlay_080ba2c0
Overlay_080ba2c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r0, [sp, #12]
	ldr	r3, [pc, #268]
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #12]
	str	r3, [sp, #4]
	ldrb	r0, [r1, #0]
	bl	sub_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	bl	sub_080044d0
	ldr	r3, [pc, #248]
	lsls	r0, r0, #16
	ldr	r1, [sp, #12]
	lsrs	r0, r0, #16
	adds	r2, r0, r3
	ldrb	r3, [r1, #0]
	cmp	r3, #7
	bls.n	.L_080ba300
	movs	r3, #192
	lsls	r3, r3, #7
	adds	r2, r0, r3
.L_080ba300:
	ldr	r3, [pc, #228]
	ldr	r1, [pc, #224]
	ands	r2, r3
	adds	r3, r2, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #128
	asrs	r3, r3, #1
	lsls	r1, r1, #6
	adds	r2, r3, r1
	ldr	r1, [sp, #4]
	ldr	r3, [r1, #0]
	cmp	r3, r2
	bne.n	.L_080ba326
	str	r2, [r1, #0]
	movs	r0, #5
	bl	sub_080030f8
	b.n	.L_080ba330
.L_080ba326:
	ldr	r3, [sp, #4]
	movs	r0, #10
	str	r2, [r3, #0]
	bl	sub_080030f8
.L_080ba330:
	movs	r0, #0
	movs	r1, #0
	bl	sub_080c10e8
	add	r7, sp, #24
	ldr	r0, [sp, #12]
	adds	r1, r7, #0
	bl	sub_080b9d34
	ldr	r3, [r7, #0]
	cmp	r3, #135
	bne.n	.L_080ba35a
	ldr	r3, [pc, #160]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08015130
.L_080ba35a:
	ldr	r0, [r7, #8]
	bl	sub_08077008
	adds	r5, r0, #0
	movs	r1, #36
	ldrsh	r0, [r7, r1]
	bl	sub_08077008
	ldr	r3, [sp, #12]
	adds	r3, #44
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [sp, #12]
	str	r3, [sp, #8]
	movs	r3, #30
	ldrsb	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_080ba384
	movs	r6, #1
.L_080ba384:
	ldr	r3, [sp, #12]
	ldrb	r0, [r3, #0]
	bl	sub_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_080b7f70
	ldr	r3, [r0, #40]
	movs	r2, #1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #2
	bl	sub_08009260
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r3, r5, r2
	mov	r9, r0
	ldrb	r0, [r3, #0]
	bl	sub_080c2410
	adds	r3, r0, #0
	lsls	r3, r3, #16
	movs	r2, #36
	ldrsh	r1, [r7, r2]
	ldr	r0, [r7, #8]
	mov	r2, r9
	bl	sub_080b82c4
	ldr	r0, [r7, #8]
	bl	sub_080b7dd0
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	sub_08009088
	movs	r3, #36
	ldrsh	r0, [r7, r3]
	bl	sub_080b7dd0
	ldrh	r3, [r7, #36]
	cmp	r3, #7
	bhi.n	.L_080ba3f0
	movs	r3, #1
	b.n	.L_080ba3f2
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x00007fff
	.2byte 0x1e74
	.2byte 0x0300
.L_080ba3f0:
	movs	r3, #0
.L_080ba3f2:
	str	r3, [r7, #4]
	ldr	r1, [pc, #44]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #44]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #24]
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_080ba440
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x00006000
	.4byte 0x04000040
	.2byte 0x0048
	.2byte 0x0400
.L_080ba440:
	cmp	r6, #0
	beq.n	.L_080ba484
	movs	r0, #10
	bl	sub_080030f8
	movs	r1, #36
	ldrsh	r0, [r7, r1]
	bl	sub_080b8178
	movs	r0, #2
	bl	sub_080030f8
	movs	r0, #4
	bl	sub_080030f8
	movs	r0, #10
	bl	sub_080030f8
	ldr	r2, [sp, #12]
	movs	r0, #0
	ldrb	r1, [r2, #2]
	bl	sub_080bbabc
	ldr	r1, [pc, #268]
	movs	r0, #4
	bl	sub_080bbabc
	bl	sub_080bb938
	movs	r3, #36
	ldrsh	r0, [r7, r3]
	bl	sub_080b8000
	b.n	.L_080ba562
.L_080ba484:
	movs	r1, #0
	str	r1, [sp, #0]
	str	r1, [r7, #28]
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #88]
	cmp	r3, #0
	beq.n	.L_080ba496
	movs	r3, #1
	str	r3, [r7, #28]
.L_080ba496:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080ba4be
	ldr	r3, [r7, #0]
	adds	r3, #200
	str	r3, [r7, #0]
	ldr	r2, [sp, #4]
	movs	r1, #1
	str	r1, [sp, #0]
	str	r1, [r2, #20]
	ldr	r3, [r7, #8]
	add	r0, sp, #16
	strh	r3, [r0, #0]
	ldr	r3, [r7, #12]
	strh	r3, [r0, #2]
	movs	r3, #255
	strh	r3, [r0, #4]
	movs	r1, #0
	bl	sub_080b7b6c
.L_080ba4be:
	movs	r3, #8
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	bgt.n	.L_080ba4ce
	movs	r2, #1
	mov	r9, r2
.L_080ba4ce:
	movs	r3, #0
	mov	r1, r9
	mov	fp, r3
	cmp	r1, #0
	beq.n	.L_080ba50e
	mov	r8, r7
	mov	sl, r3
.L_080ba4dc:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_080ba4fc
	mov	r3, r8
	mov	r1, r9
	mov	r0, sl
	ldr	r5, [r3, #8]
	ldr	r6, [r3, #12]
	bl	sub_080022ec
	adds	r2, r0, #0
	adds	r2, #100
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080c0df4
.L_080ba4fc:
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	movs	r1, #30
	add	fp, r2
	add	sl, r1
	cmp	fp, r9
	bne.n	.L_080ba4dc
.L_080ba50e:
	movs	r1, #200
	ldr	r0, [pc, #108]
	lsls	r1, r1, #4
	bl	sub_080041d8
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_080ba53a
	ldr	r1, [sp, #12]
	movs	r2, #128
	ldr	r3, [r1, #88]
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ba534
	adds	r0, r7, #0
	bl	sub_080c9008
	b.n	.L_080ba53a
.L_080ba534:
	adds	r0, r7, #0
	bl	sub_080c9018
.L_080ba53a:
	bl	sub_080be02c
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L_080ba55a
	ldr	r1, [sp, #4]
	movs	r3, #0
	str	r3, [r1, #20]
	bl	sub_080b6cb0
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_080c0cec
.L_080ba55a:
	movs	r2, #36
	ldrsh	r0, [r7, r2]
	bl	sub_080b8000
.L_080ba562:
	ldr	r0, [r7, #8]
	bl	sub_080b8000
	movs	r0, #0
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000853
	.4byte 0x080bd899
