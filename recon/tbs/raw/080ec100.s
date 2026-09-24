.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_0800307c, 0x0800307c
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08007308, 0x08007308
	.set sub_08009008, 0x08009008
	.set sub_08009038, 0x08009038
	.set sub_080b5040, 0x080b5040
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cde90, 0x080cde90
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global FunctionHead_080ec100
	.global Func_080ec100
	.thumb_func
FunctionHead_080ec100:
Func_080ec100:
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
.L_080ec3c2:
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
	bne.n	.L_080ec752
	movs	r5, #225
	movs	r3, #0
	lsls	r5, r5, #7
	mov	r8, r3
	movs	r6, #15
	add	r5, fp
.L_080ec6c8:
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
	bge.n	.L_080ec728
	subs	r3, #4
	b.n	.L_080ec72a
	.4byte 0xffffe000
	.4byte 0x000077d8
	.4byte 0x080eef68
	.4byte 0xffe00000
	.4byte 0x080eef70
	.4byte 0xffe80000
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x080ec0f1
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x080ede5c
	.2byte 0x4e20
	.2byte 0x0000
.L_080ec728:
	adds	r3, #4
.L_080ec72a:
	str	r3, [r5, #4]
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	movs	r3, #6
	movs	r4, #1
	negs	r3, r3
	add	r8, r4
	adds	r1, #100
	adds	r2, #52
	str	r3, [r5, #12]
	mov	r7, r8
	movs	r3, #0
	str	r1, [r5, #0]
	str	r2, [r5, #4]
	str	r1, [r5, #8]
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_080ec6c8
.L_080ec752:
	mov	r0, sl
	cmp	r0, #221
	ble.n	.L_080ec7b6
	movs	r5, #225
	movs	r1, #0
	lsls	r5, r5, #7
	ldr	r7, [pc, #860]
	mov	r8, r1
	movs	r6, #2
	add	r5, fp
.L_080ec766:
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #222
	cmp	sl, r3
	blt.n	.L_080ec7aa
	ldrh	r1, [r7, #0]
	ldr	r3, [pc, #840]
	add	r1, fp
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	ldr	r3, [r5, #4]
	subs	r2, #1
	subs	r3, #1
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #12]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	cmp	r3, #0
	bge.n	.L_080ec7aa
	ldr	r3, [r5, #8]
	str	r2, [r5, #4]
	str	r3, [r5, #0]
.L_080ec7aa:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_080ec766
.L_080ec7b6:
	mov	r1, sl
	cmp	r1, #175
	ble.n	.L_080ec82a
	movs	r6, #5
	cmp	r1, #221
	ble.n	.L_080ec7ce
	mov	r3, sl
	subs	r3, #223
	movs	r6, #60
	cmp	r3, #1
	bls.n	.L_080ec7ce
	movs	r6, #40
.L_080ec7ce:
	cmp	r6, #60
	ble.n	.L_080ec7d4
	movs	r6, #60
.L_080ec7d4:
	movs	r2, #0
	mov	r8, r2
	cmp	r6, #0
	beq.n	.L_080ec82a
	lsls	r7, r6, #1
.L_080ec7de:
	mov	r5, r8
	adds	r5, #1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r1, r3, #1
	lsls	r3, r1, #2
	subs	r4, r7, r3
	cmp	r4, #11
	bgt.n	.L_080ec7f2
	movs	r4, #12
.L_080ec7f2:
	cmp	r4, #63
	ble.n	.L_080ec7f8
	movs	r4, #63
.L_080ec7f8:
	movs	r3, #1
	mov	r0, r8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ec804
	negs	r1, r1
.L_080ec804:
	lsrs	r2, r1, #31
	adds	r0, r1, #0
	adds	r2, r1, r2
	adds	r1, #1
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	asrs	r1, r1, #1
	asrs	r2, r2, #1
	movs	r3, #63
	subs	r3, r3, r1
	adds	r0, #48
	adds	r2, #111
	movs	r1, #0
	mov	r8, r5
	str	r4, [sp, #0]
	bl	sub_080cde90
	cmp	r8, r6
	bne.n	.L_080ec7de
.L_080ec82a:
	mov	r1, r9
	cmp	r1, #39
	bhi.n	.L_080ec862
	lsrs	r3, r1, #31
	add	r3, r9
	asrs	r3, r3, #1
	adds	r4, r3, #1
	cmp	r4, #4
	ble.n	.L_080ec83e
	movs	r4, #4
.L_080ec83e:
	lsls	r0, r4, #1
	ldr	r2, [pc, #632]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [pc, #632]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, fp
	ldr	r6, [sp, #24]
	adds	r1, r1, r2
	movs	r3, #60
	movs	r2, #108
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r0, [sp, #68]
	ldr	r4, [r6, #4]
	bl	sub_080072f4
.L_080ec862:
	mov	r2, sl
	subs	r2, #192
	cmp	r2, #7
	bhi.n	.L_080ec89c
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	adds	r4, r3, #3
	cmp	r4, #6
	ble.n	.L_080ec878
	movs	r4, #6
.L_080ec878:
	lsls	r0, r4, #1
	ldr	r2, [pc, #576]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r7, [pc, #572]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	add	r1, fp
	movs	r2, #108
	movs	r3, #60
	subs	r2, r2, r4
	subs	r3, r3, r4
	adds	r1, r1, r7
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
.L_080ec89c:
	mov	r1, sl
	cmp	r1, #199
	ble.n	.L_080ec90e
	mov	r5, sl
	subs	r5, #200
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080ec8b0
	mov	r3, sl
	subs	r3, #197
.L_080ec8b0:
	asrs	r6, r3, #2
	adds	r4, r6, #5
	cmp	r4, #8
	ble.n	.L_080ec8ba
	movs	r4, #8
.L_080ec8ba:
	lsls	r0, r4, #1
	ldr	r7, [pc, #508]
	subs	r3, r0, #2
	ldrh	r1, [r7, r3]
	ldr	r2, [pc, #508]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, fp
	ldr	r0, [sp, #24]
	adds	r1, r1, r2
	movs	r3, #60
	movs	r2, #108
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
	mov	r1, sl
	cmp	r1, #213
	bgt.n	.L_080ec90e
	adds	r4, r6, #1
	cmp	r4, #4
	ble.n	.L_080ec8ec
	movs	r4, #4
.L_080ec8ec:
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r7, r3]
	ldr	r2, [pc, #460]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, fp
	ldr	r6, [sp, #24]
	adds	r1, r1, r2
	movs	r3, #52
	movs	r2, #100
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r0, [sp, #68]
	ldr	r4, [r6, #4]
	bl	sub_080072f4
.L_080ec90e:
	mov	r3, sl
	subs	r3, #214
	cmp	r3, #7
	bhi.n	.L_080ec94a
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L_080ec920
	mov	r1, sl
	subs	r1, #211
.L_080ec920:
	asrs	r3, r1, #2
	cmp	r3, #2
	ble.n	.L_080ec928
	movs	r3, #2
.L_080ec928:
	lsls	r1, r3, #3
	adds	r1, r1, r3
	movs	r3, #48
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r1, r1, #8
	movs	r7, #128
	add	r1, fp
	lsls	r7, r7, #3
	ldr	r4, [r0, #4]
	adds	r1, r1, r7
	ldr	r0, [sp, #68]
	movs	r2, #63
	movs	r3, #18
	bl	sub_080072f4
.L_080ec94a:
	mov	r1, sl
	cmp	r1, #221
	ble.n	.L_080ec9a4
	mov	r3, sl
	subs	r3, #222
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r5, r3, #0
	cmp	r3, #0
	bge.n	.L_080ec962
	adds	r5, r3, #3
.L_080ec962:
	asrs	r5, r5, #2
	lsls	r5, r5, #2
	subs	r5, r3, r5
	adds	r5, #3
	lsls	r1, r5, #3
	adds	r1, r1, r5
	lsls	r1, r1, #8
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r6, #48
	add	r1, fp
	adds	r1, r1, r2
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	movs	r2, #72
	movs	r3, #30
	bl	sub_080072f4
	cmp	r5, #5
	bne.n	.L_080ec9a4
	movs	r1, #176
	lsls	r1, r1, #5
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	add	r1, fp
	movs	r2, #66
	movs	r3, #22
	bl	sub_080072f4
.L_080ec9a4:
	mov	r3, sl
	cmp	r3, #27
	ble.n	.L_080eca2a
	ldr	r4, [sp, #24]
	ldr	r4, [r4, #4]
	movs	r6, #0
	ldr	r5, [pc, #276]
	mov	r9, r4
	mov	r8, r6
	movs	r7, #3
.L_080ec9b8:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080eca1c
	movs	r1, #3
	mov	r0, r8
	bl	sub_080022fc
	adds	r0, #2
	lsls	r4, r0, #1
	ldr	r2, [pc, #256]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #64]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #68]
	bl	sub_08007308
	mov	r3, r8
	ldr	r2, [pc, #220]
	ands	r3, r7
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	ble.n	.L_080eca1c
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	cmp	r3, #104
	ble.n	.L_080eca1c
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080eca1c:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_080ec9b8
.L_080eca2a:
	movs	r3, #0
	mov	r8, r3
	movs	r5, #32
.L_080eca30:
	mov	r4, r8
	mov	r2, sl
	lsls	r1, r4, #5
	cmp	r2, #0
	bge.n	.L_080eca3c
	adds	r2, #3
.L_080eca3c:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r6, [sp, #24]
	adds	r2, r1, r2
	movs	r7, #1
	subs	r2, #32
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #68]
	mov	r1, fp
	movs	r3, #88
	add	r8, r7
	bl	sub_080072f4
	mov	r0, r8
	cmp	r0, #5
	bne.n	.L_080eca30
	ldr	r2, [pc, #112]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #244
	beq.n	.L_080eca8e
	cmp	r2, #16
	bgt.n	.L_080eca80
	b.n	.L_080ec3c2
.L_080eca80:
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080eca8e
	b.n	.L_080ec3c2
.L_080eca8e:
	add	r4, sp, #40
	ldr	r3, [pc, #72]
	ldrh	r4, [r4, #0]
	strh	r4, [r3, #4]
	ldr	r6, [sp, #36]
	movs	r2, #0
	str	r2, [r6, #16]
	movs	r1, #0
	movs	r0, #2
	bl	sub_0800307c
	bl	sub_080d67dc
	ldr	r0, [pc, #52]
	bl	sub_08004278
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	b.n	.L_080ecae8
	movs	r0, r0
	.4byte 0x080ede5c
	.4byte 0x00004e20
	.4byte 0x000000f0
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0x080eef78
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x03001ad0
	.4byte 0x080ec0e1
	.2byte 0x0040
	.2byte 0x0400
.L_080ecae8:
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #208]
	movs	r3, #75
	add	r2, fp
	str	r3, [r2, #0]
	movs	r7, #0
	movs	r2, #128
	ldr	r3, [pc, #200]
	mov	r8, r7
	movs	r1, #0
	lsls	r2, r2, #3
.L_080ecb02:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080ecb02
	movs	r5, #225
	movs	r1, #0
	lsls	r5, r5, #7
	mov	r8, r1
	movs	r6, #0
	add	r5, fp
	movs	r7, #31
.L_080ecb1c:
	bl	sub_08004458
	movs	r2, #1
	ands	r0, r7
	add	r8, r2
	adds	r0, #32
	mov	r3, r8
	str	r0, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r3, #16
	bne.n	.L_080ecb1c
	ldr	r2, [pc, #144]
	add	r2, fp
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_080ecb7a
	movs	r5, #225
	lsls	r5, r5, #7
	adds	r7, r2, #0
	add	r6, sp, #88
	add	r5, fp
	movs	r4, #36
.L_080ecb52:
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r4]
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_080e396c
	ldr	r3, [r6, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r7, #0]
	ldr	r4, [sp, #8]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r5, #28
	adds	r4, #2
	cmp	r8, r3
	bne.n	.L_080ecb52
.L_080ecb7a:
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #76]
	mov	r1, fp
	bl	sub_080e0524
	ldr	r0, [pc, #72]
	bl	sub_080f9010
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #50
	strh	r3, [r2, #0]
	movs	r3, #0
	mov	sl, r3
.L_080ecb9c:
	mov	r4, sl
	cmp	r4, #96
	bne.n	.L_080ecba8
	movs	r0, #134
	bl	sub_080b50e8
.L_080ecba8:
	ldr	r7, [sp, #24]
	str	r7, [sp, #20]
	movs	r7, #225
	movs	r6, #0
	lsls	r7, r7, #7
	mov	r8, r6
	add	r7, fp
	b.n	.L_080ecbd8
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00007784
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x0000006f
	.4byte 0x00000121
	.2byte 0x0020
	.2byte 0x0400
.L_080ecbd8:
	mov	r0, r8
	lsls	r5, r0, #4
	adds	r3, r5, #7
	cmp	sl, r3
	bne.n	.L_080ecbe8
	movs	r0, #154
	bl	sub_080f9010
.L_080ecbe8:
	cmp	sl, r5
	bge.n	.L_080ecbee
	b.n	.L_080ecd06
.L_080ecbee:
	ldr	r3, [r7, #24]
	cmp	r3, #31
	ble.n	.L_080ecbf6
	b.n	.L_080ecd06
.L_080ecbf6:
	mov	r1, sl
	lsls	r0, r1, #4
	movs	r1, #104
	bl	sub_080022fc
	movs	r6, #104
	ldr	r3, [r7, #4]
	movs	r4, #17
	adds	r5, r0, #0
	ldr	r2, [r7, #0]
	ldr	r0, [sp, #20]
	str	r6, [sp, #4]
	str	r4, [sp, #0]
	adds	r3, r3, r5
	ldr	r4, [r0, #4]
	subs	r2, #8
	ldr	r0, [sp, #68]
	subs	r3, #216
	mov	r1, fp
	bl	sub_080072f4
	ldr	r3, [r7, #4]
	movs	r1, #17
	adds	r3, r3, r5
	subs	r5, r6, r5
	ldr	r2, [r7, #0]
	ldr	r6, [sp, #20]
	str	r1, [sp, #0]
	str	r5, [sp, #4]
	subs	r2, #8
	subs	r3, #112
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #68]
	mov	r1, fp
	bl	sub_080072f4
	movs	r1, #34
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #65
	str	r1, [sp, #4]
	movs	r1, #221
	lsls	r1, r1, #3
	subs	r3, #65
	subs	r2, #17
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #68]
	add	r1, fp
	bl	sub_080072f4
	ldr	r3, [r7, #4]
	cmp	r3, #111
	bgt.n	.L_080ecc68
	adds	r3, #16
	str	r3, [r7, #4]
	b.n	.L_080ecc6e
.L_080ecc68:
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_080ecc6e:
	ldr	r3, [r7, #4]
	cmp	r3, #111
	ble.n	.L_080ecd06
	movs	r0, #0
	str	r0, [sp, #32]
	ldr	r1, [r7, #0]
	str	r1, [sp, #28]
	ldr	r3, [r7, #24]
	cmp	r3, #7
	bgt.n	.L_080ecc8a
	ldr	r2, [pc, #464]
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
.L_080ecc8a:
	movs	r2, #0
	mov	r9, r2
	ldr	r2, [pc, #456]
.L_080ecc90:
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne.n	.L_080eccf8
	str	r2, [sp, #16]
	bl	sub_08004458
	ldr	r6, [pc, #444]
	ands	r6, r0
	bl	sub_08004458
	ldr	r5, [pc, #440]
	ldr	r4, [sp, #28]
	ldr	r3, [pc, #440]
	ldr	r2, [sp, #16]
	ands	r5, r0
	adds	r5, r5, r3
	movs	r0, #112
	lsls	r3, r4, #16
	str	r3, [r2, #0]
	lsls	r3, r0, #16
	str	r3, [r2, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r2, [sp, #16]
	asrs	r3, r3, #7
	str	r3, [r2, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	ldr	r2, [sp, #16]
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r2, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	ldr	r2, [sp, #16]
	adds	r3, #32
	str	r3, [r2, #24]
	ldr	r1, [sp, #32]
	adds	r1, #1
	str	r1, [sp, #32]
	cmp	r1, #4
	beq.n	.L_080ecd06
.L_080eccf8:
	movs	r3, #1
	movs	r4, #128
	add	r9, r3
	lsls	r4, r4, #3
	adds	r2, #28
	cmp	r9, r4
	bne.n	.L_080ecc90
.L_080ecd06:
	movs	r6, #1
	add	r8, r6
	mov	r0, r8
	adds	r7, #28
	cmp	r0, #5
	beq.n	.L_080ecd14
	b.n	.L_080ecbd8
.L_080ecd14:
	ldr	r2, [pc, #336]
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_080ecd60
	movs	r6, #36
	movs	r7, #8
.L_080ecd28:
	cmp	sl, r7
	bne.n	.L_080ecd4c
	mov	r0, fp
	adds	r5, r0, r2
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #2
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
.L_080ecd4c:
	ldr	r2, [pc, #280]
	movs	r3, #1
	mov	r4, fp
	add	r8, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r7, #16
	cmp	r8, r3
	bne.n	.L_080ecd28
.L_080ecd60:
	movs	r6, #0
	mov	r8, r6
	ldr	r6, [pc, #240]
.L_080ecd66:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080ecdde
	subs	r3, #1
	movs	r2, #128
	str	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r1, #60
	lsls	r2, r2, #7
	bl	sub_080e3908
	ldr	r3, [r6, #4]
	asrs	r7, r3, #16
	mov	ip, r7
	cmp	r7, #120
	ble.n	.L_080ecd94
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080ecdde
.L_080ecd94:
	ldr	r2, [r6, #0]
	cmp	r2, #0
	blt.n	.L_080ecdde
	asrs	r7, r2, #16
	cmp	r7, #126
	bgt.n	.L_080ecdde
	cmp	r3, #0
	blt.n	.L_080ecdde
	ldr	r0, [r6, #24]
	cmp	r0, #0
	bge.n	.L_080ecdac
	adds	r0, #7
.L_080ecdac:
	asrs	r0, r0, #3
	adds	r0, #3
	lsls	r5, r0, #1
	ldr	r2, [pc, #184]
	mov	r1, r8
	subs	r3, r5, #2
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #64]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r2, r7, r2
	mov	r7, ip
	str	r0, [sp, #0]
	subs	r3, r7, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
.L_080ecdde:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_080ecd66
	movs	r0, #4
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #144
	beq.n	.L_080ece12
	b.n	.L_080ecb9c
.L_080ece12:
	ldr	r5, [pc, #96]
	movs	r6, #0
	mov	r8, r6
	add	r5, fp
.L_080ece1a:
	movs	r7, #1
	ldmia	r5!, {r0}
	add	r8, r7
	bl	sub_08009038
	mov	r0, r8
	cmp	r0, #8
	bne.n	.L_080ece1a
	ldr	r0, [pc, #76]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
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
	movs	r0, r0
	.4byte 0x000077a8
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007fff
	.4byte 0xffffc000
	.4byte 0x00007828
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x000077d8
	.4byte 0x080cd261
