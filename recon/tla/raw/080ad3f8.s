.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_080aebd0, 0x080aebd0
	.set sub_080aec04, 0x080aec04
	.set sub_080af6f4, 0x080af6f4
	.set sub_080af7ac, 0x080af7ac
	.set sub_080afdbc, 0x080afdbc
	.set sub_080afdd8, 0x080afdd8
	.set sub_080b0288, 0x080b0288
	.set sub_080b0a94, 0x080b0a94
	.global Func_080ad3f8
	.thumb_func
Func_080ad3f8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #96
	sub	sp, #4
	bl	sub_08014dac
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08016ca4
	adds	r7, r0, #0
	movs	r0, #16
	ldrsh	r3, [r7, r0]
	movs	r5, #3
	str	r3, [r6, #0]
	movs	r1, #18
	ldrsh	r3, [r7, r1]
	adds	r1, r7, #0
	str	r3, [r6, #4]
	adds	r1, #36
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
	adds	r2, r6, #0
	str	r3, [r6, #32]
	adds	r3, r7, #0
	adds	r3, #33
	ldrb	r3, [r3, #0]
	adds	r2, #40
	str	r3, [r6, #36]
.L_080ad452:
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	subs	r5, #1
	str	r3, [r2, #0]
	movs	r0, #2
	ldrsh	r3, [r1, r0]
	adds	r1, #4
	str	r3, [r2, #4]
	adds	r2, #8
	cmp	r5, #0
	bge.n	.L_080ad452
	movs	r1, #52
	ldrsh	r2, [r7, r1]
	movs	r0, #20
	ldrsh	r3, [r7, r0]
	muls	r3, r2
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_080ad480
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	adds	r2, r3, r1
.L_080ad480:
	asrs	r0, r2, #14
	movs	r2, #56
	ldrsh	r1, [r7, r2]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_080ad492
	cmp	r2, #1
	bgt.n	.L_080ad4c8
	b.n	.L_080ad498
.L_080ad492:
	subs	r3, r1, r0
	cmp	r3, #1
	bgt.n	.L_080ad4c8
.L_080ad498:
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	movs	r0, #22
	ldrsh	r3, [r7, r0]
	muls	r3, r2
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_080ad4b0
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	adds	r2, r3, r1
.L_080ad4b0:
	asrs	r0, r2, #14
	movs	r2, #58
	ldrsh	r1, [r7, r2]
	subs	r2, r0, r1
	cmp	r2, #0
	blt.n	.L_080ad4c2
	cmp	r2, #1
	bgt.n	.L_080ad4c8
	b.n	.L_080ad4d8
.L_080ad4c2:
	subs	r3, r1, r0
	cmp	r3, #1
	ble.n	.L_080ad4d8
.L_080ad4c8:
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #20]
	strh	r3, [r7, #22]
	ldrh	r3, [r7, #52]
	strh	r3, [r7, #56]
	ldrh	r3, [r7, #54]
	strh	r3, [r7, #58]
.L_080ad4d8:
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r3, [r1, #0]
	movs	r0, #4
	negs	r0, r0
	ands	r0, r3
	movs	r3, #4
	ands	r3, r0
	strb	r0, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080ad4f8
	movs	r2, #1
	adds	r3, r0, #0
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_080ad4f8:
	movs	r0, #162
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080ad50a
	ldr	r3, [r6, #28]
	adds	r3, #1
	str	r3, [r6, #28]
.L_080ad50a:
	movs	r1, #161
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #42
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080ad522
	b.n	.L_080ad8d2
.L_080ad522:
	movs	r5, #0
.L_080ad524:
	lsls	r3, r5, #1
	adds	r1, r3, #0
	adds	r1, #216
	ldrh	r2, [r7, r1]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ad538
	b.n	.L_080ad6c0
.L_080ad538:
	ldrh	r0, [r7, r1]
	bl	sub_080aec04
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	str	r0, [r6, #88]
	cmp	r3, #0
	beq.n	.L_080ad558
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r2, [r1, #0]
	movs	r3, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_080ad558:
	ldr	r1, [r6, #88]
	ldr	r3, [r6, #8]
	movs	r0, #8
	ldrsh	r2, [r1, r0]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r2, #10
	ldrsb	r2, [r1, r2]
	ldr	r3, [r6, #12]
	movs	r1, #0
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r8, r1
.L_080ad572:
	ldr	r2, [r6, #88]
	mov	r0, r8
	lsls	r3, r0, #2
	adds	r3, #24
	ldrb	r1, [r2, r3]
	adds	r2, r2, r3
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	str	r1, [r6, #72]
	str	r3, [r6, #84]
	cmp	r1, #26
	bls.n	.L_080ad58c
	b.n	.L_080ad6b4
.L_080ad58c:
	ldr	r2, [pc, #792]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080ad6b4
	.4byte 0x080ad600
	.4byte 0x080ad60a
	.4byte 0x080ad614
	.4byte 0x080ad61e
	.4byte 0x080ad628
	.4byte 0x080ad632
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad6b4
	.4byte 0x080ad63c
	.4byte 0x080ad646
	.4byte 0x080ad650
	.4byte 0x080ad65a
	.4byte 0x080ad664
	.4byte 0x080ad66e
	.4byte 0x080ad678
	.4byte 0x080ad682
	.4byte 0x080ad68c
	.4byte 0x080ad6b4
	.4byte 0x080ad69c
	.2byte 0xd6ac
	.2byte 0x080a
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #32]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #32]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #36]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #36]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #16]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #40]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #40]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #48]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #48]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #56]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #56]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #64]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #64]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #44]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #44]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #52]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #52]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #60]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #60]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #68]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #68]
	b.n	.L_080ad6b4
	movs	r2, #161
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldrb	r3, [r1, #0]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	strb	r3, [r1, #0]
	b.n	.L_080ad6b4
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r1, [r3, #0]
	movs	r2, #8
	orrs	r2, r1
	strb	r2, [r3, #0]
	b.n	.L_080ad6b4
	ldr	r3, [r6, #28]
	ldr	r2, [r6, #84]
	adds	r3, r3, r2
	str	r3, [r6, #28]
.L_080ad6b4:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #3
	bgt.n	.L_080ad6c0
	b.n	.L_080ad572
.L_080ad6c0:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L_080ad6c8
	b.n	.L_080ad524
.L_080ad6c8:
	movs	r3, #152
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ad6e0
	movs	r3, #10
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #0]
.L_080ad6e0:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, r0, r7
	movs	r4, #0
	mov	r8, r0
.L_080ad6ea:
	mov	r1, r8
	ldr	r1, [r1, #0]
	movs	r5, #0
	mov	sl, r1
.L_080ad6f2:
	movs	r3, #1
	lsls	r3, r5
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ad746
	adds	r0, r4, #0
	adds	r1, r5, #0
	str	r4, [sp, #0]
	bl	sub_080b0a94
	ldr	r3, [r6, #0]
	movs	r2, #4
	ldrsb	r2, [r0, r2]
	ldr	r4, [sp, #0]
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
.L_080ad746:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_080ad6f2
	movs	r3, #4
	adds	r4, #1
	add	r8, r3
	cmp	r4, #3
	ble.n	.L_080ad6ea
	movs	r0, #42
	adds	r0, #255
	adds	r3, r7, r0
	ldrb	r0, [r3, #0]
	bl	sub_080b0288
	adds	r5, r0, #0
	ldrb	r2, [r5, #8]
	ldr	r3, [r6, #0]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	ldrb	r2, [r5, #9]
	ldr	r3, [r6, #4]
	str	r0, [r6, #0]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	ldrb	r2, [r5, #10]
	ldr	r3, [r6, #8]
	str	r0, [r6, #4]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	ldrb	r2, [r5, #11]
	ldr	r3, [r6, #12]
	str	r0, [r6, #8]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	ldrb	r2, [r5, #12]
	ldr	r3, [r6, #16]
	str	r0, [r6, #12]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	ldrb	r2, [r5, #13]
	ldr	r3, [r6, #24]
	str	r0, [r6, #16]
	movs	r1, #10
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002054
	str	r0, [r6, #24]
	movs	r5, #0
.L_080ad7c6:
	lsls	r3, r5, #1
	adds	r1, r3, #0
	adds	r1, #216
	ldrh	r2, [r7, r1]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ad8ca
	ldrh	r0, [r7, r1]
	bl	sub_080aec04
	str	r0, [r6, #88]
	movs	r1, #0
	mov	r8, r1
.L_080ad7e4:
	ldr	r2, [r6, #88]
	mov	r0, r8
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
	bhi.n	.L_080ad8c0
	ldr	r2, [pc, #172]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080ad828
	.4byte 0x080ad83a
	.4byte 0x080ad84c
	.4byte 0x080ad85e
	.4byte 0x080ad870
	.4byte 0x080ad882
	.4byte 0x080ad894
	.2byte 0xd8b0
	.2byte 0x080a
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #0]
	b.n	.L_080ad8c0
	ldr	r2, [r6, #32]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #32]
	b.n	.L_080ad8c0
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #4]
	b.n	.L_080ad8c0
	ldr	r2, [r6, #36]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #36]
	b.n	.L_080ad8c0
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #8]
	b.n	.L_080ad8c0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #12]
	b.n	.L_080ad8c0
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #16]
	b.n	.L_080ad8c0
	movs	r0, r0
	.4byte 0x080ad594
	.2byte 0xd808
	.2byte 0x080a
	ldr	r2, [r6, #24]
	ldr	r3, [r6, #84]
	movs	r1, #10
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	str	r0, [r6, #24]
.L_080ad8c0:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #3
	ble.n	.L_080ad7e4
.L_080ad8ca:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L_080ad8d2
	b.n	.L_080ad7c6
.L_080ad8d2:
	movs	r0, #52
	adds	r0, #255
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #8]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_080ad8ea
	adds	r3, #7
.L_080ad8ea:
	asrs	r3, r3, #3
	str	r3, [r6, #8]
	movs	r1, #54
	adds	r1, #255
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #12]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_080ad906
	adds	r3, #7
.L_080ad906:
	asrs	r3, r3, #3
	str	r3, [r6, #12]
	movs	r2, #72
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [r6, #16]
	adds	r3, #8
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_080ad922
	adds	r3, #7
.L_080ad922:
	asrs	r3, r3, #3
	str	r3, [r6, #16]
	movs	r3, #150
	lsls	r3, r3, #1
	movs	r4, #40
	movs	r5, #3
	adds	r0, r7, r3
.L_080ad930:
	ldrb	r3, [r0, #0]
	subs	r5, #1
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r2, r3, #0
	muls	r2, r3
	adds	r2, r2, r3
	ldr	r3, [r4, r6]
	lsls	r1, r2, #2
	adds	r1, r1, r2
	adds	r3, r3, r1
	str	r3, [r4, r6]
	adds	r0, #1
	adds	r4, #8
	cmp	r5, #0
	bge.n	.L_080ad930
	movs	r1, #56
	adds	r1, #255
	adds	r0, r7, r1
	movs	r5, #3
	movs	r1, #44
.L_080ad95a:
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	subs	r5, #1
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r1, r6]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r1, r6]
	adds	r1, #8
	cmp	r5, #0
	bge.n	.L_080ad95a
	movs	r2, #42
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080ad9e6
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_080ad9dc
	ldr	r2, [pc, #600]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080ad9c0
	.4byte 0x080ad9b4
	.4byte 0x080ad9ca
	.4byte 0x080ad9b8
	.4byte 0x080ad9c0
	.4byte 0x080ad9bc
	.4byte 0x080ad9ca
	.2byte 0xd9d4
	.2byte 0x080a
	movs	r0, #137
	b.n	.L_080ad9c2
	movs	r0, #18
	b.n	.L_080ad9cc
	movs	r0, #137
	b.n	.L_080ad9c2
	movs	r0, #136
.L_080ad9c2:
	lsls	r0, r0, #1
	bl	sub_08016ce4
	b.n	.L_080ad9dc
	movs	r0, #20
.L_080ad9cc:
	adds	r0, #255
	bl	sub_08016ce4
	b.n	.L_080ad9dc
	movs	r0, #18
	adds	r0, #255
	bl	sub_08016ce4
.L_080ad9dc:
	cmp	r0, #0
	beq.n	.L_080ad9e6
	ldr	r3, [r6, #36]
	adds	r3, #4
	str	r3, [r6, #36]
.L_080ad9e6:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bge.n	.L_080ad9f0
	movs	r3, #0
	str	r3, [r6, #8]
.L_080ad9f0:
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_080ad9fc
	str	r2, [r6, #8]
.L_080ad9fc:
	ldr	r3, [r6, #12]
	cmp	r3, #0
	bge.n	.L_080ada06
	movs	r3, #0
	str	r3, [r6, #12]
.L_080ada06:
	cmp	r3, r2
	ble.n	.L_080ada0c
	str	r2, [r6, #12]
.L_080ada0c:
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L_080ada16
	movs	r3, #0
	str	r3, [r6, #16]
.L_080ada16:
	cmp	r3, r2
	ble.n	.L_080ada1c
	str	r2, [r6, #16]
.L_080ada1c:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	bge.n	.L_080ada26
	movs	r3, #0
	str	r3, [r6, #24]
.L_080ada26:
	cmp	r3, #99
	ble.n	.L_080ada2e
	movs	r3, #99
	str	r3, [r6, #24]
.L_080ada2e:
	movs	r2, #42
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	cmp	r2, #0
	bne.n	.L_080ada4c
	ldr	r3, [r6, #28]
	cmp	r3, #0
	bge.n	.L_080ada44
	str	r2, [r6, #28]
	movs	r3, #0
.L_080ada44:
	cmp	r3, #4
	ble.n	.L_080ada5e
	movs	r3, #4
	b.n	.L_080ada5c
.L_080ada4c:
	ldr	r3, [r6, #28]
	cmp	r3, #0
	bge.n	.L_080ada56
	movs	r3, #0
	str	r3, [r6, #28]
.L_080ada56:
	cmp	r3, #2
	ble.n	.L_080ada5e
	movs	r3, #2
.L_080ada5c:
	str	r3, [r6, #28]
.L_080ada5e:
	ldr	r3, [r6, #32]
	cmp	r3, #0
	bge.n	.L_080ada68
	movs	r3, #0
	str	r3, [r6, #32]
.L_080ada68:
	movs	r2, #156
	lsls	r2, r2, #6
	adds	r2, #16
	cmp	r3, r2
	ble.n	.L_080ada74
	str	r2, [r6, #32]
.L_080ada74:
	ldr	r3, [r6, #36]
	cmp	r3, #0
	bge.n	.L_080ada7e
	movs	r3, #0
	str	r3, [r6, #36]
.L_080ada7e:
	cmp	r3, #200
	ble.n	.L_080ada86
	movs	r3, #200
	str	r3, [r6, #36]
.L_080ada86:
	movs	r3, #200
	adds	r2, r6, #0
	adds	r1, r6, #0
	movs	r5, #0
	movs	r0, #0
	mov	ip, r3
	movs	r4, #44
	adds	r2, #40
	adds	r1, #44
.L_080ada98:
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_080adaa2
	str	r0, [r2, #0]
	adds	r3, r0, #0
.L_080adaa2:
	cmp	r3, #200
	ble.n	.L_080adaaa
	mov	r3, ip
	str	r3, [r2, #0]
.L_080adaaa:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_080adab4
	str	r0, [r1, #0]
	adds	r3, r0, #0
.L_080adab4:
	cmp	r3, #200
	ble.n	.L_080adabc
	mov	r3, ip
	str	r3, [r6, r4]
.L_080adabc:
	adds	r5, #1
	adds	r1, #8
	adds	r4, #8
	adds	r2, #8
	cmp	r5, #3
	ble.n	.L_080ada98
	ldr	r3, [r6, #8]
	adds	r1, r7, #0
	strh	r3, [r7, #60]
	adds	r1, #72
	ldr	r3, [r6, #12]
	movs	r5, #3
	strh	r3, [r7, #62]
	adds	r3, r7, #0
	ldr	r2, [r6, #16]
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r2, r7, #0
	ldr	r3, [r6, #24]
	adds	r2, #66
	strb	r3, [r2, #0]
	adds	r2, #1
	ldr	r3, [r6, #28]
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	ldr	r2, [r6, #32]
	adds	r3, #68
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	ldr	r3, [r6, #36]
	adds	r2, #69
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #40
.L_080adb00:
	ldr	r3, [r2, #0]
	subs	r5, #1
	strh	r3, [r1, #0]
	ldr	r3, [r2, #4]
	adds	r2, #8
	strh	r3, [r1, #2]
	adds	r1, #4
	cmp	r5, #0
	bge.n	.L_080adb00
	movs	r0, #42
	adds	r0, #255
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080adb2c
	movs	r1, #252
	movs	r0, #156
	lsls	r1, r1, #6
	lsls	r0, r0, #6
	adds	r1, #255
	adds	r0, #15
	b.n	.L_080adb34
.L_080adb2c:
	movs	r0, #218
	lsls	r0, r0, #3
	adds	r0, #255
	adds	r1, r0, #0
.L_080adb34:
	movs	r3, #52
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_080adb42
	movs	r3, #0
	str	r3, [r6, #0]
.L_080adb42:
	cmp	r3, r1
	ble.n	.L_080adb4a
	str	r1, [r6, #0]
	adds	r3, r1, #0
.L_080adb4a:
	strh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	beq.n	.L_080adb86
	movs	r3, #20
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #0]
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_080adb68
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r2, r2, r3
.L_080adb68:
	asrs	r2, r2, #14
	cmp	r2, #0
	bge.n	.L_080adb70
	movs	r2, #0
.L_080adb70:
	cmp	r2, r1
	ble.n	.L_080adb76
	adds	r2, r1, #0
.L_080adb76:
	movs	r1, #56
	ldrsh	r3, [r7, r1]
	cmp	r3, #0
	beq.n	.L_080adb84
	cmp	r2, #0
	bne.n	.L_080adb84
	movs	r2, #1
.L_080adb84:
	strh	r2, [r7, #56]
.L_080adb86:
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080adb94
	movs	r3, #0
	str	r3, [r6, #4]
.L_080adb94:
	cmp	r3, r0
	ble.n	.L_080adb9c
	str	r0, [r6, #4]
	adds	r3, r0, #0
.L_080adb9c:
	strh	r3, [r7, #54]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, r3
	beq.n	.L_080adbd8
	movs	r1, #22
	ldrsh	r2, [r7, r1]
	ldr	r3, [r6, #4]
	muls	r2, r3
	cmp	r2, #0
	bge.n	.L_080adbba
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r2, r2, r3
.L_080adbba:
	asrs	r2, r2, #14
	cmp	r2, #0
	bge.n	.L_080adbc2
	movs	r2, #0
.L_080adbc2:
	cmp	r2, r0
	ble.n	.L_080adbc8
	adds	r2, r0, #0
.L_080adbc8:
	movs	r0, #58
	ldrsh	r3, [r7, r0]
	cmp	r3, #0
	beq.n	.L_080adbd6
	cmp	r2, #0
	bne.n	.L_080adbd6
	movs	r2, #1
.L_080adbd6:
	strh	r2, [r7, #58]
.L_080adbd8:
	adds	r0, r6, #0
	bl	sub_08013164
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0xd994
	.2byte 0x080a
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #104
	adds	r0, #255
	sub	sp, #8
	bl	sub_08016d18
	bl	sub_080afdbc
	mov	sl, r0
	movs	r0, #0
	mov	r8, r0
	cmp	r8, sl
	bge.n	.L_080adc7e
	ldr	r3, [pc, #120]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #2
	mov	r9, r2
	mov	fp, r3
.L_080adc22:
	mov	r2, r9
	ldrb	r0, [r2, #0]
	bl	sub_08016ca4
	movs	r2, #216
	adds	r7, r0, #0
	movs	r1, #14
.L_080adc30:
	ldrh	r3, [r2, r7]
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080adc6c
	ldrh	r0, [r2, r7]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	sub_080aec04
	ldr	r2, [sp, #0]
	ldr	r1, [sp, #4]
	adds	r5, r0, #0
	adds	r5, #24
	movs	r6, #3
.L_080adc4e:
	ldrb	r3, [r5, #0]
	adds	r5, #4
	cmp	r3, #27
	bne.n	.L_080adc66
	movs	r0, #104
	adds	r0, #255
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	sub_08016cfc
	ldr	r2, [sp, #0]
	ldr	r1, [sp, #4]
.L_080adc66:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080adc4e
.L_080adc6c:
	subs	r1, #1
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L_080adc30
	movs	r2, #1
	add	r8, r2
	add	r9, r2
	cmp	r8, sl
	blt.n	.L_080adc22
.L_080adc7e:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
.L_080adc90:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	r8, r2
	movs	r6, #0
	bl	sub_080afdbc
	cmp	r0, #0
	bne.n	.L_080adca8
	movs	r0, #0
	b.n	.L_080adce0
.L_080adca8:
	cmp	r6, r0
	bge.n	.L_080adcd0
	ldr	r3, [pc, #56]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r7, r3, r2
	adds	r5, r0, #0
.L_080adcb6:
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	movs	r2, #58
	ldrsh	r3, [r0, r2]
	subs	r5, #1
	add	r8, r3
	movs	r2, #54
	ldrsh	r3, [r0, r2]
	adds	r7, #1
	adds	r6, r6, r3
	cmp	r5, #0
	bne.n	.L_080adcb6
.L_080adcd0:
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_080adce0
	mov	r3, r8
	lsls	r0, r3, #16
	adds	r1, r6, #0
	bl	sub_08002054
.L_080adce0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
.L_080adcec:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #1
	negs	r2, r2
	movs	r7, #0
	mov	r8, r2
	bl	sub_080afdbc
	cmp	r0, #0
	bne.n	.L_080add06
	movs	r0, #0
	b.n	.L_080add52
.L_080add06:
	cmp	r0, #0
	ble.n	.L_080add42
	ldr	r3, [pc, #76]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r6, r3, r2
	adds	r5, r0, #0
.L_080add14:
	ldrb	r0, [r6, #0]
	bl	sub_08016ca4
	adds	r3, r0, #0
	movs	r2, #54
	ldrsh	r1, [r3, r2]
	movs	r0, #0
	cmp	r1, #0
	ble.n	.L_080add30
	movs	r2, #58
	ldrsh	r0, [r3, r2]
	lsls	r0, r0, #16
	bl	sub_08002054
.L_080add30:
	cmp	r0, r7
	ble.n	.L_080add3a
	ldrb	r3, [r6, #0]
	adds	r7, r0, #0
	mov	r8, r3
.L_080add3a:
	subs	r5, #1
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L_080add14
.L_080add42:
	movs	r1, #1
	negs	r1, r1
	movs	r0, #0
	cmp	r8, r1
	beq.n	.L_080add52
	mov	r0, r8
	bl	sub_080aebd0
.L_080add52:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	cmp	r0, #0
	ble.n	.L_080add6e
	adds	r5, r0, #0
.L_080add64:
	subs	r5, #1
	bl	.L_080adcec
	cmp	r5, #0
	bne.n	.L_080add64
.L_080add6e:
	bl	.L_080adc90
	pop	{r5, pc}
.L_080add74:
	push	{r5, lr}
	ldr	r0, [pc, #104]
	bl	sub_08013300
	ldrb	r2, [r0, #0]
	ldr	r1, [pc, #100]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r5, r3, r1
	ldrb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r5, r3
	adds	r5, r3, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r4, r3, r1
	ldrb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r4, r3
	adds	r4, r3, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r2, r3, r1
	ldrb	r3, [r0, #1]
	subs	r5, #48
	adds	r3, r2, r3
	adds	r2, r3, #0
	subs	r4, #48
	lsls	r3, r5, #4
	adds	r3, r3, r4
	subs	r2, #48
	lsls	r3, r3, #6
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #21
	orrs	r2, r3
	ldr	r3, [pc, #24]
	asrs	r0, r2, #16
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080addda
	ldr	r3, [pc, #20]
	orrs	r0, r3
.L_080addda:
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r5, pc}
	.4byte 0x00000002
	.4byte 0xfffffe20
	.4byte 0x03001238
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r3, #128
	movs	r2, #133
	mov	r5, sp
	movs	r4, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r4, [r5, #0]
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #164]
	adds	r2, #184
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r4, [r5, #0]
	adds	r0, r5, #0
	ldr	r1, [pc, #156]
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r3, #128
	adds	r2, #212
	lsls	r3, r3, #24
.L_080ade30:
	ldr	r1, [r2, #8]
	ands	r1, r3
	mov	fp, r1
	cmp	r1, #0
	bne.n	.L_080ade30
	movs	r3, #128
	movs	r2, #133
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r1, [r5, #0]
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #120]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #104]
	movs	r3, #255
	strb	r3, [r2, #4]
	mov	r3, fp
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	mov	r9, r2
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #96]
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_080af7ac
	ldr	r7, [pc, #68]
	ldr	r3, [pc, #60]
	movs	r1, #140
	lsls	r1, r1, #2
	mov	r8, r3
	adds	r2, r7, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	movs	r2, #141
	adds	r3, r7, r1
	lsls	r2, r2, #2
	movs	r1, #2
	strh	r1, [r3, #0]
	movs	r0, #4
	adds	r3, r7, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #32]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #54
	mov	sl, r2
	adds	r2, r7, r3
	movs	r3, #8
	strh	r3, [r2, #0]
	movs	r3, #142
	lsls	r3, r3, #2
	adds	r2, r7, r3
	subs	r3, #56
	strh	r3, [r2, #0]
	b.n	.L_080aded0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x02000240
	.4byte 0x02001000
	.4byte 0x850007c8
	.4byte 0x02000040
	.4byte 0x02000520
	.2byte 0x0298
	.2byte 0x8500
.L_080aded0:
	adds	r3, #58
	adds	r2, r7, r3
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r2, #0]
	movs	r2, #143
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strh	r1, [r3, #0]
	movs	r1, #144
	lsls	r1, r1, #2
	mov	r2, fp
	adds	r3, r7, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r7, r1
	subs	r1, #46
	strh	r2, [r3, #0]
	adds	r3, r7, r1
	str	r0, [r3, #0]
	bl	sub_080afdd8
	movs	r0, #5
	bl	sub_080afdd8
	movs	r1, #149
	movs	r0, #4
	bl	sub_080af6f4
	movs	r1, #140
	movs	r0, #4
	bl	sub_080af6f4
	movs	r1, #141
	movs	r0, #6
	bl	sub_080af6f4
	movs	r1, #33
	movs	r0, #7
	bl	sub_080af6f4
	movs	r1, #149
	movs	r0, #0
	bl	sub_080af6f4
	movs	r1, #140
	movs	r0, #0
	bl	sub_080af6f4
	movs	r1, #140
	movs	r0, #1
	bl	sub_080af6f4
	movs	r1, #141
	movs	r0, #2
	bl	sub_080af6f4
	movs	r1, #144
	movs	r0, #2
	bl	sub_080af6f4
	movs	r2, #163
	movs	r3, #200
	lsls	r2, r2, #2
	str	r3, [r7, #16]
	mov	r1, fp
	adds	r3, r7, r2
	adds	r2, #48
	str	r1, [r3, #0]
	adds	r3, r7, r2
	mov	r1, fp
	subs	r2, #72
	strh	r1, [r3, #0]
	adds	r3, r7, r2
	mov	r1, r8
	strb	r1, [r3, #0]
	ldr	r5, [pc, #52]
	movs	r1, #128
	subs	r2, #72
	lsls	r1, r1, #2
	adds	r3, r7, r2
	adds	r1, #42
	strb	r5, [r3, #0]
	subs	r2, #1
	adds	r3, r7, r1
	strb	r5, [r3, #0]
	ldr	r6, [pc, #36]
	adds	r3, r7, r2
	subs	r1, #5
	strb	r5, [r3, #0]
	mov	r2, r8
	adds	r3, r7, r1
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	mov	r2, fp
	strb	r6, [r3, #0]
	str	r2, [r7, #0]
	bl	.L_080add74
	movs	r1, #182
	lsls	r1, r1, #2
	adds	r3, r7, r1
	b.n	.L_080adfa8
	.4byte 0x00000001
	.2byte 0x0008
	.2byte 0x0000
.L_080adfa8:
	str	r0, [r3, #0]
	ldr	r3, [pc, #96]
	mov	r2, fp
	str	r2, [r3, #0]
	ldr	r3, [pc, #92]
	mov	r1, r8
	strb	r1, [r3, #0]
	movs	r1, #128
	str	r2, [r7, #4]
	lsls	r1, r1, #2
	ldrb	r2, [r3, #0]
	adds	r1, #74
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #76]
	mov	r2, fp
	strh	r2, [r3, #0]
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	adds	r1, #60
	strh	r3, [r2, #0]
	ldr	r3, [pc, #72]
	mov	r8, r9
	ldrh	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #62
	adds	r2, #255
	adds	r3, r7, r2
	mov	r1, sl
	adds	r2, #1
	strb	r1, [r3, #0]
	adds	r3, r7, r2
	strb	r1, [r3, #0]
	movs	r1, #64
	adds	r1, #255
	adds	r3, r7, r1
	mov	r2, sl
	strb	r2, [r3, #0]
	adds	r1, #1
	movs	r2, #66
	adds	r3, r7, r1
	adds	r2, #255
	strb	r6, [r3, #0]
	adds	r1, #2
	adds	r3, r7, r2
	strb	r6, [r3, #0]
	b.n	.L_080ae020
	.4byte 0xffffffff
	.4byte 0x0300117c
	.4byte 0x03001200
	.4byte 0x03001218
	.4byte 0x020036d0
	.2byte 0x3860
	.2byte 0x0200
.L_080ae020:
	adds	r2, #2
	adds	r3, r7, r1
	strb	r6, [r3, #0]
	adds	r1, #2
	adds	r3, r7, r2
	movs	r2, #16
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #163
	lsls	r2, r2, #1
	adds	r3, r7, r2
	adds	r1, #2
	movs	r2, #32
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r3, #74
	adds	r3, #255
	adds	r2, r7, r3
	adds	r1, #2
	movs	r3, #64
	strb	r3, [r2, #0]
	adds	r2, r7, r1
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r1
	strb	r3, [r2, #0]
	movs	r7, #0
.L_080ae066:
	lsls	r3, r7, #1
	adds	r3, r3, r7
	movs	r2, #136
	lsls	r3, r3, #2
	lsls	r2, r2, #5
	add	r3, r8
	adds	r2, #184
	adds	r5, r3, r2
	movs	r6, #7
.L_080ae078:
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r6, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	cmp	r6, #0
	bge.n	.L_080ae078
	adds	r7, #1
	cmp	r7, #31
	ble.n	.L_080ae066
	ldr	r3, [pc, #64]
	movs	r4, #152
	ldr	r5, [pc, #64]
	lsls	r4, r4, #5
	movs	r7, #0
	mov	ip, r3
	movs	r0, #0
	adds	r4, #56
.L_080ae0a2:
	adds	r2, r0, r7
	mov	r1, ip
	adds	r3, r2, r1
	adds	r1, r3, r4
	adds	r2, r2, r5
	movs	r6, #4
.L_080ae0ae:
	ldrb	r3, [r2, #0]
	subs	r6, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r6, #0
	bge.n	.L_080ae0ae
	adds	r7, #1
	adds	r0, #4
	cmp	r7, #3
	ble.n	.L_080ae0a2
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02001000
	.2byte 0x1f2c
	.2byte 0x080b
	ldr	r3, [pc, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #5
	str	r2, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02000240
