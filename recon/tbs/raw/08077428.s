.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_08004970, 0x08004970
	.set sub_08077394, 0x08077394
	.set sub_08078414, 0x08078414
	.set sub_08079338, 0x08079338
	.set sub_08079ad8, 0x08079ad8
	.set sub_0807a0cc, 0x0807a0cc
	.global Owner_RecalculateStats
	.global Func_08077428
	.thumb_func
Owner_RecalculateStats:
Func_08077428:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #96
	sub	sp, #4
	bl	sub_08004970
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08077394
	adds	r7, r0, #0
	movs	r0, #16
	ldrsh	r3, [r7, r0]
	str	r3, [r6, #0]
	movs	r1, #18
	ldrsh	r3, [r7, r1]
	str	r3, [r6, #4]
	ldrh	r3, [r7, #24]
	str	r3, [r6, #8]
	ldrh	r3, [r7, #26]
	str	r3, [r6, #12]
	ldrh	r3, [r7, #28]
	str	r3, [r6, #16]
	ldrb	r3, [r7, #30]
	str	r3, [r6, #24]
	ldrb	r2, [r7, #31]
	movs	r3, #15
	ands	r3, r2
	str	r3, [r6, #28]
	adds	r3, r7, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	str	r3, [r6, #32]
	adds	r3, r7, #0
	adds	r3, #33
	ldrb	r3, [r3, #0]
	adds	r1, r7, #0
	adds	r2, r6, #0
	str	r3, [r6, #36]
	adds	r1, #36
	adds	r2, #40
	movs	r5, #3
.L_08077482:
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	str	r3, [r2, #0]
	movs	r0, #2
	ldrsh	r3, [r1, r0]
	subs	r5, #1
	str	r3, [r2, #4]
	adds	r1, #4
	adds	r2, #8
	cmp	r5, #0
	bge.n	.L_08077482
	movs	r1, #52
	ldrsh	r2, [r7, r1]
	movs	r0, #20
	ldrsh	r3, [r7, r0]
	muls	r3, r2
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_080774ac
	ldr	r1, [pc, #568]
	adds	r2, r3, r1
.L_080774ac:
	asrs	r0, r2, #14
	movs	r2, #56
	ldrsh	r1, [r7, r2]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_080774be
	cmp	r2, #1
	bgt.n	.L_080774f0
	b.n	.L_080774c4
.L_080774be:
	subs	r3, r1, r0
	cmp	r3, #1
	bgt.n	.L_080774f0
.L_080774c4:
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	movs	r0, #22
	ldrsh	r3, [r7, r0]
	muls	r3, r2
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_080774d8
	ldr	r1, [pc, #524]
	adds	r2, r3, r1
.L_080774d8:
	asrs	r0, r2, #14
	movs	r2, #58
	ldrsh	r1, [r7, r2]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_080774ea
	cmp	r2, #1
	bgt.n	.L_080774f0
	b.n	.L_08077500
.L_080774ea:
	subs	r3, r1, r0
	cmp	r3, #1
	ble.n	.L_08077500
.L_080774f0:
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #20]
	strh	r3, [r7, #22]
	ldrh	r3, [r7, #52]
	strh	r3, [r7, #56]
	ldrh	r3, [r7, #54]
	strh	r3, [r7, #58]
.L_08077500:
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r3, [r1, #0]
	movs	r0, #4
	negs	r0, r0
	ands	r0, r3
	movs	r3, #4
	ands	r3, r0
	movs	r4, #0
	strb	r0, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08077522
	movs	r2, #1
	adds	r3, r0, #0
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_08077522:
	movs	r0, #162
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08077534
	ldr	r3, [r6, #28]
	adds	r3, #1
	str	r3, [r6, #28]
.L_08077534:
	movs	r1, #161
	lsls	r1, r1, #1
	ldr	r2, [pc, #428]
	adds	r3, r7, r1
	strb	r4, [r3, #0]
	ldr	r0, [pc, #428]
	adds	r3, r7, r2
	strb	r4, [r3, #0]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0807754e
	b.n	.L_0807790c
.L_0807754e:
	movs	r5, #0
.L_08077550:
	lsls	r3, r5, #1
	adds	r1, r3, #0
	adds	r1, #216
	ldrh	r2, [r7, r1]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08077564
	b.n	.L_08077708
.L_08077564:
	ldrh	r0, [r7, r1]
	bl	sub_08078414
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	str	r0, [r6, #88]
	cmp	r3, #0
	beq.n	.L_08077584
	movs	r2, #152
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldrb	r2, [r1, #0]
	movs	r3, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_08077584:
	ldr	r1, [r6, #88]
	movs	r3, #8
	ldrsh	r2, [r1, r3]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r2, #10
	ldrsb	r2, [r1, r2]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	movs	r0, #0
	str	r3, [r6, #12]
	mov	r8, r0
.L_0807759e:
	mov	r1, r8
	ldr	r2, [r6, #88]
	lsls	r3, r1, #2
	adds	r3, #24
	ldrb	r1, [r2, r3]
	adds	r2, r2, r3
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	str	r1, [r6, #72]
	str	r3, [r6, #84]
	cmp	r1, #26
	bls.n	.L_080775b8
	b.n	.L_080776fc
.L_080775b8:
	ldr	r2, [pc, #308]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080776fc
	.4byte 0x0807762c
	.4byte 0x08077636
	.4byte 0x08077640
	.4byte 0x0807764a
	.4byte 0x08077654
	.4byte 0x0807765e
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x080776fc
	.4byte 0x08077668
	.4byte 0x08077672
	.4byte 0x0807767c
	.4byte 0x08077686
	.4byte 0x08077690
	.4byte 0x0807769a
	.4byte 0x080776a4
	.4byte 0x080776ae
	.4byte 0x080776b8
	.4byte 0x080776c4
	.4byte 0x080776d2
	.2byte 0x76f4
	.2byte 0x0807
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	b.n	.L_080776fc
	ldr	r3, [r6, #32]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #32]
	b.n	.L_080776fc
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	b.n	.L_080776fc
	ldr	r3, [r6, #36]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #36]
	b.n	.L_080776fc
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	b.n	.L_080776fc
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	b.n	.L_080776fc
	ldr	r3, [r6, #40]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #40]
	b.n	.L_080776fc
	ldr	r3, [r6, #48]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #48]
	b.n	.L_080776fc
	ldr	r3, [r6, #56]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #56]
	b.n	.L_080776fc
	ldr	r3, [r6, #64]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #64]
	b.n	.L_080776fc
	ldr	r3, [r6, #44]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #44]
	b.n	.L_080776fc
	ldr	r3, [r6, #52]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #52]
	b.n	.L_080776fc
	ldr	r3, [r6, #60]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #60]
	b.n	.L_080776fc
	ldr	r3, [r6, #68]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #68]
	b.n	.L_080776fc
	movs	r2, #161
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldrb	r3, [r1, #0]
	ldr	r2, [r6, #84]
	b.n	.L_080776cc
	ldr	r3, [pc, #32]
	adds	r1, r7, r3
	ldr	r2, [r6, #84]
	ldrb	r3, [r1, #0]
.L_080776cc:
	adds	r3, r3, r2
	strb	r3, [r1, #0]
	b.n	.L_080776fc
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r1, [r3, #0]
	movs	r2, #8
	orrs	r2, r1
	strb	r2, [r3, #0]
	b.n	.L_080776fc
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000143
	.4byte 0x00000129
	.2byte 0x75c0
	.2byte 0x0807
	ldr	r3, [r6, #28]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #28]
.L_080776fc:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #3
	bgt.n	.L_08077708
	b.n	.L_0807759e
.L_08077708:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L_08077710
	b.n	.L_08077550
.L_08077710:
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08077728
	movs	r3, #10
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #0]
.L_08077728:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, r0, r7
	movs	r4, #0
	mov	r8, r0
.L_08077732:
	mov	r1, r8
	ldr	r1, [r1, #0]
	movs	r5, #0
	mov	sl, r1
.L_0807773a:
	movs	r3, #1
	lsls	r3, r5
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0807778e
	adds	r0, r4, #0
	adds	r1, r5, #0
	str	r4, [sp, #0]
	bl	sub_0807a0cc
	ldr	r3, [r6, #0]
	movs	r2, #4
	ldrsb	r2, [r0, r2]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r2, #5
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	movs	r2, #6
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r2, #7
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	movs	r2, #9
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #24]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r4, [sp, #0]
.L_0807778e:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_0807773a
	movs	r3, #4
	adds	r4, #1
	add	r8, r3
	cmp	r4, #3
	ble.n	.L_08077732
	ldr	r0, [pc, #604]
	adds	r3, r7, r0
	ldrb	r0, [r3, #0]
	bl	sub_08079ad8
	adds	r5, r0, #0
	ldrb	r2, [r5, #8]
	ldr	r3, [r6, #0]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_080022ec
	ldrb	r2, [r5, #9]
	ldr	r3, [r6, #4]
	str	r0, [r6, #0]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_080022ec
	ldrb	r2, [r5, #10]
	ldr	r3, [r6, #8]
	str	r0, [r6, #4]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_080022ec
	ldrb	r2, [r5, #11]
	ldr	r3, [r6, #12]
	str	r0, [r6, #8]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_080022ec
	ldrb	r2, [r5, #12]
	ldr	r3, [r6, #16]
	str	r0, [r6, #12]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_080022ec
	ldrb	r2, [r5, #13]
	ldr	r3, [r6, #24]
	str	r0, [r6, #16]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_080022ec
	str	r0, [r6, #24]
	movs	r5, #0
.L_0807780c:
	lsls	r3, r5, #1
	adds	r1, r3, #0
	adds	r1, #216
	ldrh	r2, [r7, r1]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08077904
	ldrh	r0, [r7, r1]
	bl	sub_08078414
	movs	r1, #0
	str	r0, [r6, #88]
	mov	r8, r1
.L_0807782a:
	mov	r0, r8
	ldr	r2, [r6, #88]
	lsls	r3, r0, #2
	adds	r3, #24
	ldrb	r1, [r2, r3]
	adds	r2, r2, r3
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	str	r1, [r6, #72]
	subs	r1, #7
	str	r3, [r6, #84]
	cmp	r1, #7
	bhi.n	.L_080778fa
	ldr	r2, [pc, #440]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0807786c
	.4byte 0x0807787e
	.4byte 0x08077890
	.4byte 0x080778a2
	.4byte 0x080778b4
	.4byte 0x080778c6
	.4byte 0x080778d8
	.2byte 0x78ea
	.2byte 0x0807
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #0]
	b.n	.L_080778fa
	ldr	r2, [r6, #32]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #32]
	b.n	.L_080778fa
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #4]
	b.n	.L_080778fa
	ldr	r2, [r6, #36]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #36]
	b.n	.L_080778fa
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #8]
	b.n	.L_080778fa
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #12]
	b.n	.L_080778fa
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #16]
	b.n	.L_080778fa
	ldr	r2, [r6, #24]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_080022ec
	str	r0, [r6, #24]
.L_080778fa:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #3
	ble.n	.L_0807782a
.L_08077904:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L_0807790c
	b.n	.L_0807780c
.L_0807790c:
	ldr	r0, [pc, #244]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #8]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_08077922
	adds	r3, #7
.L_08077922:
	asrs	r3, r3, #3
	ldr	r1, [pc, #224]
	str	r3, [r6, #8]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #12]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0807793c
	adds	r3, #7
.L_0807793c:
	asrs	r3, r3, #3
	ldr	r2, [pc, #204]
	str	r3, [r6, #12]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #16]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_08077956
	adds	r3, #7
.L_08077956:
	asrs	r3, r3, #3
	str	r3, [r6, #16]
	movs	r3, #150
	lsls	r3, r3, #1
	movs	r4, #40
	movs	r5, #3
	adds	r0, r7, r3
.L_08077964:
	ldrb	r3, [r0, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r2, r3, #0
	muls	r2, r3
	adds	r2, r2, r3
	lsls	r1, r2, #2
	ldr	r3, [r4, r6]
	adds	r1, r1, r2
	adds	r3, r3, r1
	subs	r5, #1
	str	r3, [r4, r6]
	adds	r0, #1
	adds	r4, #8
	cmp	r5, #0
	bge.n	.L_08077964
	ldr	r1, [pc, #136]
	movs	r5, #3
	adds	r0, r7, r1
	movs	r1, #44
.L_0807798c:
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r1, r6]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	subs	r5, #1
	str	r3, [r1, r6]
	adds	r1, #8
	cmp	r5, #0
	bge.n	.L_0807798c
	ldr	r2, [pc, #84]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08077a32
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #5
	bhi.n	.L_08077a28
	ldr	r2, [pc, #84]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080779dc
	.4byte 0x080779e0
	.4byte 0x080779ea
	.4byte 0x080779f2
	.4byte 0x08077a28
	.2byte 0x7a20
	.2byte 0x0807
	movs	r0, #136
	b.n	.L_080779e2
	movs	r0, #137
.L_080779e2:
	lsls	r0, r0, #1
	bl	sub_08079338
	b.n	.L_08077a28
	ldr	r0, [pc, #44]
	bl	sub_08079338
	b.n	.L_08077a28
	ldr	r0, [pc, #40]
	bl	sub_08079338
	b.n	.L_08077a28
	movs	r0, r0
	.4byte 0x00000129
	.4byte 0x0807784c
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000147
	.4byte 0x00000137
	.4byte 0x080779c4
	.4byte 0x00000113
	.2byte 0x0111
	.2byte 0x0000
	movs	r0, #137
	lsls	r0, r0, #1
	bl	sub_08079338
.L_08077a28:
	cmp	r0, #0
	beq.n	.L_08077a32
	ldr	r3, [r6, #36]
	adds	r3, #4
	str	r3, [r6, #36]
.L_08077a32:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_08077a3c
	movs	r3, #0
	str	r3, [r6, #8]
.L_08077a3c:
	ldr	r2, [pc, #440]
	cmp	r3, r2
	ble.n	.L_08077a44
	str	r2, [r6, #8]
.L_08077a44:
	ldr	r3, [r6, #12]
	cmp	r3, #0
	bge.n	.L_08077a4e
	movs	r3, #0
	str	r3, [r6, #12]
.L_08077a4e:
	cmp	r3, r2
	ble.n	.L_08077a54
	str	r2, [r6, #12]
.L_08077a54:
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L_08077a5e
	movs	r3, #0
	str	r3, [r6, #16]
.L_08077a5e:
	cmp	r3, r2
	ble.n	.L_08077a64
	str	r2, [r6, #16]
.L_08077a64:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	bge.n	.L_08077a6e
	movs	r3, #0
	str	r3, [r6, #24]
.L_08077a6e:
	cmp	r3, #99
	ble.n	.L_08077a76
	movs	r3, #99
	str	r3, [r6, #24]
.L_08077a76:
	ldr	r3, [r6, #28]
	cmp	r3, #0
	bge.n	.L_08077a80
	movs	r3, #0
	str	r3, [r6, #28]
.L_08077a80:
	cmp	r3, #2
	ble.n	.L_08077a88
	movs	r3, #2
	str	r3, [r6, #28]
.L_08077a88:
	ldr	r3, [r6, #32]
	cmp	r3, #0
	bge.n	.L_08077a92
	movs	r3, #0
	str	r3, [r6, #32]
.L_08077a92:
	ldr	r2, [pc, #360]
	cmp	r3, r2
	ble.n	.L_08077a9a
	str	r2, [r6, #32]
.L_08077a9a:
	ldr	r3, [r6, #36]
	cmp	r3, #0
	bge.n	.L_08077aa4
	movs	r3, #0
	str	r3, [r6, #36]
.L_08077aa4:
	cmp	r3, #200
	ble.n	.L_08077aac
	movs	r3, #200
	str	r3, [r6, #36]
.L_08077aac:
	movs	r2, #200
	mov	ip, r2
	adds	r1, r6, #0
	adds	r2, r6, #0
	movs	r5, #0
	movs	r0, #0
	movs	r4, #44
	adds	r2, #40
	adds	r1, #44
.L_08077abe:
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_08077ac8
	str	r0, [r2, #0]
	adds	r3, r0, #0
.L_08077ac8:
	cmp	r3, #200
	ble.n	.L_08077ad0
	mov	r3, ip
	str	r3, [r2, #0]
.L_08077ad0:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_08077ada
	str	r0, [r1, #0]
	adds	r3, r0, #0
.L_08077ada:
	cmp	r3, #200
	ble.n	.L_08077ae2
	mov	r3, ip
	str	r3, [r6, r4]
.L_08077ae2:
	adds	r5, #1
	adds	r1, #8
	adds	r4, #8
	adds	r2, #8
	cmp	r5, #3
	ble.n	.L_08077abe
	ldr	r3, [r6, #8]
	strh	r3, [r7, #60]
	ldr	r3, [r6, #12]
	strh	r3, [r7, #62]
	adds	r3, r7, #0
	ldr	r2, [r6, #16]
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r2, r7, #0
	ldr	r3, [r6, #24]
	adds	r2, #66
	strb	r3, [r2, #0]
	ldr	r3, [r6, #28]
	adds	r2, #1
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	ldr	r2, [r6, #32]
	adds	r3, #68
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	ldr	r3, [r6, #36]
	adds	r2, #69
	strb	r3, [r2, #0]
	adds	r1, r7, #0
	adds	r2, r6, #0
	adds	r1, #72
	adds	r2, #40
	movs	r5, #3
.L_08077b26:
	ldr	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r3, [r2, #4]
	subs	r5, #1
	strh	r3, [r1, #2]
	adds	r2, #8
	adds	r1, #4
	cmp	r5, #0
	bge.n	.L_08077b26
	ldr	r0, [pc, #196]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	ldr	r1, [pc, #196]
	cmp	r3, #0
	beq.n	.L_08077b46
	ldr	r1, [pc, #192]
.L_08077b46:
	movs	r3, #52
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_08077b54
	movs	r3, #0
	str	r3, [r6, #0]
.L_08077b54:
	cmp	r3, r1
	ble.n	.L_08077b5c
	str	r1, [r6, #0]
	adds	r3, r1, #0
.L_08077b5c:
	strh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	beq.n	.L_08077b94
	movs	r0, #20
	ldrsh	r2, [r7, r0]
	ldr	r3, [r6, #0]
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_08077b76
	ldr	r3, [pc, #152]
	adds	r2, r2, r3
.L_08077b76:
	asrs	r2, r2, #14
	cmp	r2, #0
	bge.n	.L_08077b7e
	movs	r2, #0
.L_08077b7e:
	cmp	r2, r1
	ble.n	.L_08077b84
	adds	r2, r1, #0
.L_08077b84:
	movs	r0, #56
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	beq.n	.L_08077b92
	cmp	r2, #0
	bne.n	.L_08077b92
	movs	r2, #1
.L_08077b92:
	strh	r2, [r7, #56]
.L_08077b94:
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_08077ba2
	movs	r3, #0
	str	r3, [r6, #4]
.L_08077ba2:
	cmp	r3, r1
	ble.n	.L_08077baa
	str	r1, [r6, #4]
	adds	r3, r1, #0
.L_08077baa:
	strh	r3, [r7, #54]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	beq.n	.L_08077be2
	movs	r0, #22
	ldrsh	r2, [r7, r0]
	ldr	r3, [r6, #4]
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_08077bc4
	ldr	r3, [pc, #72]
	adds	r2, r2, r3
.L_08077bc4:
	asrs	r2, r2, #14
	cmp	r2, #0
	bge.n	.L_08077bcc
	movs	r2, #0
.L_08077bcc:
	cmp	r2, r1
	ble.n	.L_08077bd2
	adds	r2, r1, #0
.L_08077bd2:
	movs	r0, #58
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	beq.n	.L_08077be0
	cmp	r2, #0
	bne.n	.L_08077be0
	movs	r2, #1
.L_08077be0:
	strh	r2, [r7, #58]
.L_08077be2:
	adds	r0, r6, #0
	bl	sub_08002df0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000003e7
	.4byte 0x00002710
	.4byte 0x00000129
	.4byte 0x0000270f
	.4byte 0x000007cf
	.4byte 0x00003fff
