.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_0800307c, 0x0800307c
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08009008, 0x08009008
	.set sub_080b5040, 0x080b5040
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd594, 0x080cd594
	.set sub_080d6750, 0x080d6750
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080ec728, 0x080ec728
	.set sub_080ec72a, 0x080ec72a
	.set sub_080ec752, 0x080ec752
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080ec100
Overlay_080ec100:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #104]
	ldr	r1, [r6, #0]
	sub	sp, #116
	str	r1, [sp, #68]
	subs	r3, r6, #4
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [pc, #92]
	ldr	r2, [r6, #4]
	add	r3, fp
	str	r2, [sp, #64]
	str	r0, [r3, #0]
	movs	r0, #0
	mov	r8, r3
	bl	sub_080cd594
	bl	sub_080c9048
	ldr	r2, [pc, #60]
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r5, #144
	strh	r2, [r3, #0]
	lsls	r5, r5, #3
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #56]
	bl	sub_080041d8
	ldr	r2, [pc, #56]
	movs	r0, #2
	movs	r1, #0
	bl	sub_0800307c
	movs	r4, #239
	lsls	r4, r4, #7
	add	r4, fp
	movs	r3, #0
	str	r3, [r4, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #36]
	mov	sl, r4
	bl	sub_080041d8
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #8]
	b.n	.L_080ec190
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00002137
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x080ec0e1
	.4byte 0x080ec0f1
	.4byte 0x080cd261
	.2byte 0x0048
	.2byte 0x0400
.L_080ec190:
	movs	r1, #0
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080cd104
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #56]
	mov	r7, r8
	strh	r3, [r2, #0]
	ldr	r0, [r7, #0]
	bl	sub_080d6750
	movs	r1, #189
	lsls	r1, r1, #1
	movs	r0, #8
	movs	r2, #1
	bl	sub_080dbb24
	ldr	r2, [pc, #36]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #28]
	movs	r0, #1
	movs	r2, #0
	bl	sub_080b5040
	ldr	r0, [pc, #24]
	ldr	r1, [sp, #64]
	movs	r2, #0
	movs	r3, #0
	b.n	.L_080ec1e8
	.4byte 0x0000f0f0
	.4byte 0x04000040
	.4byte 0x03001ce0
	.4byte 0x0000003d
	.2byte 0x0073
	.2byte 0x0000
.L_080ec1e8:
	bl	sub_080e0524
	ldr	r0, [pc, #92]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r1, [pc, #84]
	ldr	r0, [pc, #88]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [pc, #52]
	ldr	r3, [pc, #56]
	movs	r1, #140
	movs	r2, #200
	movs	r0, #1
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	str	r0, [sp, #60]
	str	r1, [sp, #56]
	str	r2, [sp, #52]
	str	r3, [sp, #48]
	str	r4, [sp, #44]
	ldr	r3, [pc, #36]
	b.n	.L_080ec264
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x0000006e
	.4byte 0x00004e20
	.4byte 0x00000076
	.4byte 0xfff80000
	.4byte 0xfffc0000
	.2byte 0x1ad0
	.2byte 0x0300
.L_080ec264:
	ldrh	r3, [r3, #4]
	str	r3, [sp, #40]
	ldr	r2, [pc, #124]
	ldr	r7, [r6, #16]
	movs	r5, #2
	mov	r0, sl
	add	r2, fp
	movs	r3, #75
	str	r7, [sp, #36]
	str	r5, [r0, #0]
	str	r3, [r2, #0]
	ldr	r1, [sp, #60]
	movs	r3, #3
	str	r1, [r7, #16]
	movs	r2, #7
	movs	r1, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #24]
	movs	r1, #7
	str	r3, [sp, #80]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	mov	r2, sp
	adds	r2, #80
	str	r2, [sp, #24]
	str	r3, [r2, #4]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	movs	r3, #0
	mov	r8, r3
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #56]
	negs	r1, r1
	lsls	r2, r2, #3
.L_080ec2bc:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080ec2bc
	movs	r7, #225
	movs	r6, #0
	lsls	r7, r7, #7
	mov	r8, r6
	add	r7, fp
.L_080ec2d2:
	bl	sub_08004458
	ldr	r6, [pc, #28]
	ands	r6, r0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r6, r6, r0
	b.n	.L_080ec2f8
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00007784
	.4byte 0x0400000c
	.4byte 0x02010018
	.2byte 0x3fff
	.2byte 0x0000
.L_080ec2f8:
	bl	sub_08004458
	movs	r5, #127
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #255
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #2
	str	r3, [r7, #0]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r2, r3, #2
	ldr	r3, [r7, #0]
	negs	r3, r3
	str	r2, [r7, #4]
	cmp	r3, #0
	bge.n	.L_080ec328
	adds	r3, #31
.L_080ec328:
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	negs	r3, r2
	cmp	r3, #0
	bge.n	.L_080ec334
	adds	r3, #31
.L_080ec334:
	movs	r1, #1
	asrs	r3, r3, #5
	add	r8, r1
	str	r3, [r7, #16]
	mov	r2, r8
	movs	r3, #0
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #32
	bne.n	.L_080ec2d2
	ldr	r7, [pc, #260]
	mov	r8, r3
.L_080ec34c:
	bl	sub_08004458
	movs	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	ldr	r3, [pc, #248]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	movs	r4, #128
	lsls	r4, r4, #16
	asrs	r3, r3, #3
	adds	r3, r3, r4
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r6, #192
	asrs	r3, r3, #4
	lsls	r6, r6, #15
	adds	r3, r3, r6
	str	r3, [r7, #4]
	bl	sub_08004458
	ldr	r3, [pc, #200]
	ands	r3, r0
	ldr	r0, [pc, #200]
	negs	r3, r3
	adds	r3, r3, r0
	lsls	r3, r3, #5
	str	r3, [r7, #12]
	bl	sub_08004458
	ldr	r3, [pc, #192]
	ldr	r1, [pc, #192]
	ands	r3, r0
	negs	r3, r3
	adds	r3, r3, r1
	lsls	r3, r3, #5
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #178
	add	r8, r2
	lsls	r3, r3, #2
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_080ec34c
	movs	r4, #0
	mov	sl, r4
	mov	r6, sl
	cmp	r6, #24
	bne.n	.L_080ec3ce
	movs	r0, #162
	bl	sub_080f9010
.L_080ec3ce:
	mov	r7, sl
	cmp	r7, #76
	bne.n	.L_080ec3da
	movs	r0, #164
	bl	sub_080f9010
.L_080ec3da:
	mov	r0, sl
	cmp	r0, #154
	bne.n	.L_080ec3e6
	movs	r0, #142
	bl	sub_080f9010
.L_080ec3e6:
	mov	r1, sl
	cmp	r1, #222
	bne.n	.L_080ec3f2
	movs	r0, #145
	bl	sub_080f9010
.L_080ec3f2:
	mov	r2, sl
	cmp	r2, #15
	bhi.n	.L_080ec43e
	ldr	r7, [pc, #84]
	cmp	r2, #1
	bne.n	.L_080ec426
	ldr	r5, [pc, #104]
	adds	r6, r5, #0
	movs	r3, #63
	adds	r6, #128
.L_080ec406:
	str	r3, [sp, #12]
	bl	sub_08004458
	ldr	r3, [sp, #12]
	ands	r0, r3
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_080ec406
	ldr	r3, [pc, #48]
	movs	r1, #144
	strh	r3, [r7, #0]
	ldr	r0, [pc, #76]
	lsls	r1, r1, #3
	bl	sub_080041d8
.L_080ec426:
	ldr	r4, [sp, #60]
	ldrh	r3, [r7, #0]
	mov	r6, sl
	adds	r3, r3, r4
	adds	r4, #3
	strh	r3, [r7, #0]
	str	r4, [sp, #60]
	cmp	r6, #15
	bne.n	.L_080ec43e
	ldr	r0, [pc, #48]
	bl	sub_08004278
.L_080ec43e:
	mov	r7, sl
	cmp	r7, #21
	bgt.n	.L_080ec446
	b.n	.L_080ec596
.L_080ec446:
	ldr	r3, [pc, #40]
	b.n	.L_080ec474
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00001fff
	.4byte 0xfffff800
	.4byte 0x00000fff
	.4byte 0xffffe800
	.4byte 0x02010002
	.4byte 0x080d66cd
	.2byte 0xdae8
	.2byte 0x080e
.L_080ec474:
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #72]
	str	r4, [sp, #76]
	cmp	r7, #83
	bgt.n	.L_080ec4d8
	cmp	r7, #48
	ble.n	.L_080ec496
	ldr	r0, [sp, #48]
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080ec492
	adds	r3, #63
.L_080ec492:
	asrs	r3, r3, #6
	str	r3, [sp, #48]
.L_080ec496:
	ldr	r1, [sp, #44]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080ec4a4
	adds	r3, #63
.L_080ec4a4:
	asrs	r3, r3, #6
	str	r3, [sp, #44]
	ldr	r2, [sp, #52]
	movs	r3, #132
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_080ec4bc
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #572]
	adds	r4, r4, r6
	str	r4, [sp, #44]
	b.n	.L_080ec4c6
.L_080ec4bc:
	ldr	r7, [sp, #44]
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r7, r7, r0
	str	r7, [sp, #44]
.L_080ec4c6:
	ldr	r1, [sp, #56]
	ldr	r3, [sp, #52]
	ldr	r2, [sp, #48]
	ldr	r4, [sp, #44]
	adds	r1, r1, r2
	adds	r3, r3, r4
	str	r1, [sp, #56]
	str	r3, [sp, #52]
	b.n	.L_080ec540
.L_080ec4d8:
	mov	r6, sl
	cmp	r6, #170
	bgt.n	.L_080ec540
	cmp	r6, #107
	bgt.n	.L_080ec4ec
	ldr	r7, [sp, #48]
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r7, r7, r0
	str	r7, [sp, #48]
.L_080ec4ec:
	ldr	r1, [sp, #48]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080ec4fa
	adds	r3, #63
.L_080ec4fa:
	ldr	r2, [sp, #56]
	asrs	r3, r3, #6
	adds	r2, r2, r3
	str	r3, [sp, #48]
	mov	r3, sl
	str	r2, [sp, #56]
	cmp	r3, #117
	bgt.n	.L_080ec540
	movs	r6, #138
	ldr	r4, [sp, #52]
	lsls	r6, r6, #16
	cmp	r4, r6
	ble.n	.L_080ec51e
	ldr	r7, [sp, #44]
	ldr	r0, [pc, #476]
	adds	r7, r7, r0
	str	r7, [sp, #44]
	b.n	.L_080ec528
.L_080ec51e:
	ldr	r1, [sp, #44]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r1, r1, r2
	str	r1, [sp, #44]
.L_080ec528:
	ldr	r4, [sp, #44]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080ec536
	adds	r3, #63
.L_080ec536:
	ldr	r6, [sp, #52]
	asrs	r3, r3, #6
	adds	r6, r6, r3
	str	r3, [sp, #44]
	str	r6, [sp, #52]
.L_080ec540:
	add	r2, sp, #100
	movs	r3, #0
	ldr	r5, [pc, #432]
	str	r3, [r2, #12]
	str	r3, [r2, #4]
	mov	r8, r3
	add	r7, sp, #72
	adds	r6, r2, #0
	add	r5, fp
.L_080ec552:
	ldr	r3, [pc, #424]
	mov	r0, r8
	ldrb	r2, [r3, r0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [sp, #56]
	asrs	r3, r3, #1
	ldr	r2, [pc, #412]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #408]
	ldrb	r2, [r3, r0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r4, [sp, #52]
	asrs	r3, r3, #1
	ldr	r0, [pc, #400]
	lsls	r3, r3, #16
	adds	r3, r3, r4
	adds	r3, r3, r0
	str	r3, [r6, #8]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldmia	r5!, {r0}
	movs	r3, #0
	bl	sub_08009008
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #8
	bne.n	.L_080ec552
.L_080ec596:
	mov	r3, sl
	cmp	r3, #76
	bne.n	.L_080ec61a
	movs	r4, #0
	ldr	r7, [pc, #364]
	mov	r8, r4
.L_080ec5a2:
	movs	r6, #1
	ldr	r3, [r7, #24]
	negs	r6, r6
	cmp	r3, r6
	bne.n	.L_080ec60c
	bl	sub_08004458
	movs	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	ldr	r3, [pc, #340]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #128
	lsls	r0, r0, #15
	asrs	r3, r3, #3
	adds	r3, r3, r0
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #192
	lsls	r1, r1, #15
	asrs	r3, r3, #4
	adds	r3, r3, r1
	str	r3, [r7, #4]
	bl	sub_08004458
	movs	r2, #63
	ands	r2, r0
	movs	r3, #32
	subs	r3, r3, r2
	lsls	r3, r3, #12
	str	r3, [r7, #12]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	negs	r3, r3
	subs	r3, #16
	lsls	r3, r3, #13
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #24]
.L_080ec60c:
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #3
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_080ec5a2
.L_080ec61a:
	mov	r4, sl
	cmp	r4, #152
	bne.n	.L_080ec63a
	ldr	r2, [pc, #240]
	movs	r0, #2
	movs	r1, #96
	bl	sub_0800307c
	ldr	r2, [pc, #236]
	movs	r3, #24
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r2, [pc, #232]
	movs	r3, #0
	add	r2, fp
	str	r3, [r2, #0]
.L_080ec63a:
	movs	r6, #152
	negs	r6, r6
	add	r6, sl
	mov	r9, r6
	cmp	r6, #87
	bhi.n	.L_080ec6b6
	movs	r6, #225
	movs	r7, #0
	lsls	r6, r6, #7
	mov	r8, r7
	add	r6, fp
.L_080ec650:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080ec658
	adds	r3, #3
.L_080ec658:
	asrs	r2, r3, #2
	adds	r3, r2, #0
	adds	r3, #152
	cmp	sl, r3
	blt.n	.L_080ec6aa
	adds	r3, #32
	cmp	sl, r3
	bge.n	.L_080ec6aa
	movs	r0, #2
	ldrsh	r2, [r6, r0]
	mov	r4, r8
	movs	r0, #3
	ands	r0, r4
	adds	r0, #5
	lsls	r5, r0, #1
	ldr	r4, [pc, #168]
	movs	r1, #6
	ldrsh	r3, [r6, r1]
	subs	r1, r5, #2
	ldrh	r1, [r4, r1]
	ldr	r7, [pc, #160]
	adds	r2, #112
	adds	r3, #62
	add	r1, fp
	subs	r2, r2, r0
	subs	r3, r3, r0
	adds	r1, r1, r7
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #4]
	adds	r2, r2, r3
	str	r2, [r6, #4]
.L_080ec6aa:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #32
	bne.n	.L_080ec650
.L_080ec6b6:
	mov	r2, sl
	cmp	r2, #222
	bne.n	sub_080ec752
	movs	r5, #225
	movs	r3, #0
	lsls	r5, r5, #7
	mov	r8, r3
	movs	r6, #15
	add	r5, fp
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #8
	str	r0, [r5, #0]
	bl	sub_08004458
	ldr	r3, [r5, #0]
	ands	r0, r6
	subs	r0, #8
	str	r0, [r5, #4]
	cmp	r3, #0
	bge.n	.L_080ec6e6
	subs	r3, #4
	b.n	.L_080ec6e8
.L_080ec6e6:
	adds	r3, #4
.L_080ec6e8:
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bge.n	sub_080ec728
	subs	r3, #4
	b.n	sub_080ec72a
