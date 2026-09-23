.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08156272, 0x08156272
	.set sub_0815629e, 0x0815629e
	.set sub_08156aa0, 0x08156aa0
	.set sub_08157058, 0x08157058
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Func_08156140
	.thumb_func
Func_08156140:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #200
	str	r0, [sp, #148]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	mov	r9, r1
	str	r0, [sp, #144]
	ldr	r4, [sp, #148]
	ldr	r1, [r3, #96]
	movs	r0, #1
	str	r1, [sp, #140]
	ldr	r2, [r3, #48]
	str	r2, [sp, #116]
	ldr	r3, [r3, #100]
	str	r3, [sp, #112]
	movs	r3, #0
	str	r3, [sp, #108]
	ldr	r4, [r4, #24]
	str	r4, [sp, #100]
	bl	sub_081435e0
	ldr	r5, [sp, #148]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L_08156192
	ldr	r1, [r5, #4]
	movs	r3, #65
	lsls	r1, r1, #4
	orrs	r1, r3
	add	r2, sp, #188
	add	r3, sp, #176
	adds	r0, r5, #0
	bl	sub_0815585c
.L_08156192:
	movs	r2, #128
	ldr	r3, [pc, #36]
	lsls	r2, r2, #19
	adds	r2, #82
	mov	r6, r9
	strh	r3, [r2, #0]
	cmp	r6, #5
	beq.n	.L_081561aa
	cmp	r6, #12
	beq.n	.L_081561aa
	cmp	r6, #13
	bne.n	.L_081561ca
.L_081561aa:
	ldr	r7, [sp, #148]
	ldr	r3, [r7, #4]
	cmp	r3, #1
	bne.n	.L_081561c0
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
	b.n	.L_081561e4
	.2byte 0x1010
	.2byte 0x0000
.L_081561c0:
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_081561e4
.L_081561ca:
	ldr	r0, [sp, #148]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_081561dc
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_081561e4
.L_081561dc:
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
.L_081561e4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r1, [sp, #112]
	str	r3, [sp, #120]
	ldr	r0, [pc, #436]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_08156206
	cmp	r1, #5
	beq.n	.L_0815620c
	cmp	r1, #8
	bne.n	.L_081562b6
.L_08156206:
	mov	r2, r9
	cmp	r2, #5
	bne.n	.L_08156210
.L_0815620c:
	movs	r3, #2
	str	r3, [sp, #100]
.L_08156210:
	mov	r4, r9
	cmp	r4, #8
	bne.n	.L_0815621a
	movs	r5, #0
	str	r5, [sp, #100]
.L_0815621a:
	ldr	r6, [sp, #100]
	cmp	r6, #0
	bne.n	.L_0815622c
	ldr	r7, [sp, #144]
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r0, [pc, #380]
	b.n	.L_0815623c
.L_0815622c:
	ldr	r3, [sp, #100]
	cmp	r3, #1
	bne.n	.L_08156246
	ldr	r4, [sp, #144]
	movs	r5, #184
	lsls	r5, r5, #5
	ldr	r0, [pc, #368]
	adds	r1, r4, r5
.L_0815623c:
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_08156258
.L_08156246:
	ldr	r6, [sp, #144]
	movs	r7, #184
	lsls	r7, r7, #5
	ldr	r0, [pc, #352]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_08156258:
	mov	r0, r9
	cmp	r0, #5
	bne.n	sub_08156272
	ldr	r0, [pc, #340]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #332]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a24
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #320]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	mov	r4, r9
	cmp	r4, #5
	bne.n	sub_0815629e
	ldr	r0, [pc, #296]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #288]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d24
	movs	r6, #239
	movs	r7, #238
	lsls	r6, r6, #7
	lsls	r7, r7, #7
	adds	r2, r5, r6
	movs	r3, #2
	adds	r7, #132
	str	r3, [r2, #0]
	adds	r2, r5, r7
	movs	r3, #75
	b.n	.L_08156540
.L_081562b6:
	mov	r3, r9
	subs	r3, #15
	cmp	r3, #2
	bhi.n	.L_08156364
	movs	r0, #0
	ldr	r1, [sp, #144]
	movs	r4, #224
	str	r0, [sp, #136]
	lsls	r4, r4, #3
	movs	r2, #60
	adds	r3, r1, r4
.L_081562cc:
	strb	r2, [r3, #0]
	ldr	r5, [sp, #136]
	adds	r3, #1
	adds	r5, #1
	str	r5, [sp, #136]
	cmp	r5, #8
	bne.n	.L_081562cc
	ldr	r6, [sp, #144]
	movs	r7, #224
	lsls	r7, r7, #3
	ldr	r0, [pc, #220]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r6, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r3, r6, r1
	movs	r2, #0
	str	r2, [r3, #0]
	mov	r3, r9
	cmp	r3, #15
	bne.n	.L_08156312
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	b.n	.L_08156542
.L_08156312:
	mov	r4, r9
	cmp	r4, #16
	bne.n	.L_0815633a
	ldr	r0, [pc, #168]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #148]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x26e0
	ldr	r5, [sp, #144]
	lsls	r6, r6, #3
	adds	r6, #34
	ldr	r0, [pc, #144]
	adds	r1, r5, r6
	b.n	.L_0815635a
.L_0815633a:
	ldr	r0, [pc, #144]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22e0
	ldr	r7, [sp, #144]
	lsls	r2, r2, #3
	adds	r2, #34
	adds	r1, r7, r2
	ldr	r0, [pc, #116]
.L_0815635a:
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_08156542
.L_08156364:
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_081563d8
	ldr	r4, [sp, #144]
	movs	r5, #224
	lsls	r5, r5, #3
	ldr	r0, [pc, #96]
	adds	r1, r4, r5
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r6, [sp, #144]
	movs	r7, #239
	movs	r1, #238
	lsls	r7, r7, #7
	lsls	r1, r1, #7
	adds	r3, r6, r7
	mov	r0, r9
	adds	r1, #132
	str	r0, [r3, #0]
	adds	r2, r6, r1
	movs	r3, #0
	b.n	.L_08156540
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000134
	.4byte 0x00000140
	.4byte 0x00000141
	.4byte 0x00000142
	.4byte 0x0000017d
	.4byte 0x03000730
	.4byte 0x0000018b
	.4byte 0x000000ff
	.4byte 0x00000163
	.4byte 0x00000161
	.4byte 0x0000012f
	.4byte 0x00000192
	.2byte 0x011f
	.2byte 0x0000
.L_081563d8:
	mov	r2, r9
	cmp	r2, #2
	bne.n	.L_0815641a
	ldr	r0, [pc, #884]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #876]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b24
	movs	r4, #224
	lsls	r4, r4, #3
	ldr	r0, [pc, #864]
	adds	r1, r3, r4
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #144]
	movs	r6, #239
	movs	r0, #238
	lsls	r6, r6, #7
	lsls	r0, r0, #7
	adds	r3, r5, r6
	mov	r7, r9
	adds	r0, #132
	str	r7, [r3, #0]
	adds	r2, r5, r0
	b.n	.L_0815653e
.L_0815641a:
	mov	r3, r9
	subs	r3, #3
	cmp	r3, #1
	bls.n	.L_08156428
	mov	r1, r9
	cmp	r1, #11
	bne.n	.L_08156450
.L_08156428:
	ldr	r2, [sp, #144]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #812]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #144]
	movs	r5, #239
	movs	r6, #238
	lsls	r5, r5, #7
	lsls	r6, r6, #7
	adds	r2, r4, r5
	movs	r3, #2
	adds	r6, #132
	str	r3, [r2, #0]
	adds	r2, r4, r6
	b.n	.L_0815653e
.L_08156450:
	mov	r7, r9
	cmp	r7, #6
	beq.n	.L_0815645a
	cmp	r7, #12
	bne.n	.L_08156482
.L_0815645a:
	ldr	r2, [sp, #144]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #768]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #144]
	movs	r5, #239
	movs	r6, #238
	lsls	r5, r5, #7
	lsls	r6, r6, #7
	adds	r2, r4, r5
	movs	r3, #2
	adds	r6, #132
	str	r3, [r2, #0]
	adds	r2, r4, r6
	b.n	.L_0815653e
.L_08156482:
	mov	r7, r9
	cmp	r7, #14
	bne.n	.L_081564b2
	ldr	r2, [sp, #144]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #724]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #144]
	movs	r5, #239
	movs	r6, #238
	lsls	r5, r5, #7
	lsls	r6, r6, #7
	adds	r2, r4, r5
	movs	r3, #1
	adds	r6, #132
	str	r3, [r2, #0]
	adds	r2, r4, r6
	movs	r3, #0
	b.n	.L_08156540
.L_081564b2:
	mov	r7, r9
	cmp	r7, #13
	bne.n	.L_08156506
	ldr	r1, [sp, #144]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r5, r1, r2
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #676]
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r4, [sp, #144]
	movs	r1, #160
	movs	r6, #184
	str	r3, [sp, #136]
	lsls	r1, r1, #3
	lsls	r6, r6, #5
	adds	r1, #178
	adds	r2, r4, r6
.L_081564de:
	ldrb	r3, [r5, #0]
	adds	r5, #2
	strb	r3, [r2, #0]
	ldr	r7, [sp, #136]
	adds	r2, #1
	adds	r7, #1
	str	r7, [sp, #136]
	cmp	r7, r1
	bne.n	.L_081564de
	ldr	r0, [sp, #144]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	b.n	.L_0815653e
.L_08156506:
	ldr	r4, [sp, #144]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r4, r5
	ldr	r0, [pc, #608]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r7, #216
	ldr	r6, [sp, #144]
	lsls	r7, r7, #7
	adds	r7, #192
	ldr	r0, [pc, #592]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r6, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r6, r1
.L_0815653e:
	movs	r3, #50
.L_08156540:
	str	r3, [r2, #0]
.L_08156542:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #560]
	bl	sub_080145a8
	ldr	r2, [sp, #148]
	ldr	r0, [r2, #8]
	bl	sub_08118098
	mov	r3, r9
	lsls	r3, r3, #2
	ldr	r7, [r0, #0]
	ldr	r1, [pc, #544]
	str	r3, [sp, #92]
	add	r3, r9
	ldrb	r4, [r1, r3]
	adds	r2, r3, #1
	str	r4, [sp, #96]
	ldr	r5, [sp, #148]
	ldrb	r2, [r1, r2]
	mov	sl, r2
	adds	r2, r3, #2
	ldrb	r2, [r1, r2]
	adds	r3, #3
	str	r2, [sp, #88]
	ldrb	r1, [r1, r3]
	str	r1, [sp, #84]
	ldr	r0, [r5, #8]
	bl	sub_08118070
	str	r0, [sp, #80]
	ldr	r6, [sp, #96]
	ldr	r2, [r5, #20]
	adds	r3, r2, #0
	muls	r3, r6
	cmp	r3, #63
	ble.n	.L_08156592
	movs	r3, #1
	str	r3, [r5, #20]
	movs	r2, #1
.L_08156592:
	movs	r0, #0
	str	r0, [sp, #136]
	cmp	r2, #0
	bne.n	.L_0815659c
	b.n	.L_081568b8
.L_0815659c:
	movs	r1, #36
	str	r0, [sp, #24]
	str	r1, [sp, #20]
.L_081565a2:
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #148]
	ldrsh	r0, [r2, r4]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [sp, #20]
	ldr	r1, [sp, #148]
	mov	r8, r0
	ldrsh	r0, [r5, r1]
	bl	sub_08118070
	ldr	r3, [sp, #96]
	movs	r2, #0
	str	r0, [sp, #76]
	str	r2, [sp, #132]
	cmp	r3, #0
	bne.n	.L_081565c8
	b.n	.L_0815689a
.L_081565c8:
	ldr	r5, [sp, #80]
	ldr	r6, [sp, #24]
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	mov	r4, r9
	subs	r4, #12
	mov	fp, r3
	ldr	r0, [sp, #144]
	lsls	r3, r6, #3
	subs	r3, r3, r6
	str	r4, [sp, #52]
	lsls	r3, r3, #2
	adds	r6, r3, r0
.L_081565e4:
	ldr	r1, [sp, #52]
	cmp	r1, #2
	bhi.n	.L_081565f0
	ldr	r3, [r7, #8]
	lsls	r3, r3, #1
	b.n	.L_081565f2
.L_081565f0:
	ldr	r3, [r7, #8]
.L_081565f2:
	str	r3, [r6, #0]
	mov	r2, r9
	cmp	r2, #7
	bne.n	.L_08156610
	bl	sub_08014878
	ldr	r3, [r7, #12]
	movs	r2, #15
	ands	r2, r0
	lsls	r2, r2, #16
	movs	r4, #232
	adds	r3, r3, r2
	lsls	r4, r4, #14
	adds	r3, r3, r4
	b.n	.L_081566c8
.L_08156610:
	mov	r5, r9
	cmp	r5, #10
	bne.n	.L_0815661c
	ldr	r3, [r7, #12]
	add	r3, fp
	b.n	.L_081566c8
.L_0815661c:
	mov	r0, r9
	cmp	r0, #18
	bne.n	.L_0815662a
	ldr	r3, [r7, #12]
	movs	r1, #200
	lsls	r1, r1, #14
	b.n	.L_081566c6
.L_0815662a:
	mov	r2, r9
	cmp	r2, #6
	bne.n	.L_08156636
	ldr	r3, [r7, #12]
	add	r3, fp
	b.n	.L_081566c8
.L_08156636:
	ldr	r3, [sp, #52]
	cmp	r3, #2
	bhi.n	.L_08156654
	bl	sub_08014878
	movs	r1, #80
	bl	sub_0800206c
	ldr	r5, [r7, #12]
	subs	r0, #16
	add	r5, fp
	lsls	r0, r0, #16
	adds	r5, r5, r0
	str	r5, [r6, #4]
	b.n	.L_081566ca
.L_08156654:
	mov	r4, r9
	cmp	r4, #9
	bne.n	.L_08156668
	bl	sub_08014878
	ldr	r1, [r7, #12]
	movs	r2, #31
	ands	r2, r0
	movs	r3, #16
	b.n	.L_08156690
.L_08156668:
	mov	r3, r9
	subs	r3, #3
	cmp	r3, #1
	bhi.n	.L_0815667e
	bl	sub_08014878
	ldr	r1, [r7, #12]
	movs	r2, #31
	ands	r2, r0
	movs	r3, #16
	b.n	.L_08156690
.L_0815667e:
	mov	r5, r9
	cmp	r5, #11
	bne.n	.L_0815669c
	bl	sub_08014878
	ldr	r1, [r7, #12]
	movs	r2, #63
	ands	r2, r0
	movs	r3, #32
.L_08156690:
	subs	r3, r3, r2
	add	r1, fp
	lsls	r3, r3, #16
	adds	r1, r1, r3
	str	r1, [r6, #4]
	b.n	.L_081566ca
.L_0815669c:
	mov	r0, r9
	cmp	r0, #5
	bne.n	.L_081566a8
	ldr	r3, [r7, #12]
	add	r3, fp
	b.n	.L_081566c8
.L_081566a8:
	mov	r3, r9
	subs	r3, #15
	cmp	r3, #2
	bhi.n	.L_081566c2
	bl	sub_08014878
	movs	r1, #24
	bl	sub_0800206c
	ldr	r3, [r7, #12]
	lsls	r0, r0, #16
	adds	r3, r3, r0
	b.n	.L_081566c8
.L_081566c2:
	ldr	r3, [r7, #12]
	ldr	r1, [sp, #80]
.L_081566c6:
	adds	r3, r3, r1
.L_081566c8:
	str	r3, [r6, #4]
.L_081566ca:
	ldr	r5, [r7, #16]
	mov	r2, r8
	str	r5, [r6, #8]
	ldr	r3, [r6, #0]
	ldr	r0, [r2, #8]
	mov	r1, sl
	subs	r0, r0, r3
	bl	sub_08002054
	str	r0, [r6, #12]
	mov	r3, r8
	ldr	r0, [r3, #16]
	mov	r1, sl
	subs	r0, r0, r5
	bl	sub_08002054
	mov	r4, r9
	str	r0, [r6, #20]
	cmp	r4, #7
	bne.n	.L_0815670e
	bl	sub_08014878
	mov	r5, r8
	movs	r3, #63
	ands	r3, r0
	ldr	r0, [r5, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #4]
	ldr	r1, [pc, #120]
	subs	r0, r0, r3
	adds	r0, r0, r1
	mov	r1, sl
	b.n	.L_08156880
.L_0815670e:
	mov	r2, r9
	cmp	r2, #8
	bne.n	.L_08156732
	bl	sub_08014878
	mov	r4, r8
	movs	r3, #7
	ands	r3, r0
	ldr	r0, [r4, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #4]
	movs	r5, #176
	subs	r0, r0, r3
	lsls	r5, r5, #13
	adds	r0, r0, r5
	mov	r1, sl
	b.n	.L_08156880
.L_08156732:
	mov	r0, r9
	cmp	r0, #14
	bne.n	.L_08156784
	bl	sub_08014878
	mov	r1, r8
	movs	r2, #63
	ands	r2, r0
	movs	r3, #64
	ldr	r0, [r1, #12]
	subs	r3, r3, r2
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #4]
	mov	r1, sl
	b.n	.L_0815687e
	movs	r0, r0
	.4byte 0x00000140
	.4byte 0x03000730
	.4byte 0x0000011e
	.4byte 0x0000011d
	.4byte 0x00000129
	.4byte 0x00000193
	.4byte 0x00000190
	.4byte 0x0000017c
	.4byte 0x00000155
	.4byte 0x08143001
	.4byte 0x08198478
	.2byte 0x0000
	.2byte 0xfff4
.L_08156784:
	.2byte 0x464a
	cmp	r2, #9
	bne.n	.L_081567a4
	bl	sub_08014878
	mov	r4, r8
	movs	r2, #63
	ands	r2, r0
	movs	r3, #64
	ldr	r0, [r4, #12]
	subs	r3, r3, r2
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #4]
	mov	r1, sl
	b.n	.L_0815687e
.L_081567a4:
	mov	r3, r9
	subs	r3, #15
	cmp	r3, #2
	bhi.n	.L_081567ba
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_081567b4
	adds	r3, #31
.L_081567b4:
	asrs	r3, r3, #5
	str	r3, [r6, #16]
	b.n	.L_08156886
.L_081567ba:
	mov	r5, r9
	cmp	r5, #10
	bne.n	.L_081567dc
	bl	sub_08014878
	mov	r1, r8
	movs	r3, #31
	ands	r3, r0
	ldr	r0, [r1, #12]
	ldr	r5, [r6, #4]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	movs	r2, #128
	lsls	r2, r2, #11
	subs	r0, r0, r5
	adds	r0, r0, r2
	b.n	.L_081567fc
.L_081567dc:
	mov	r3, r9
	cmp	r3, #18
	bne.n	.L_08156820
	bl	sub_08014878
	mov	r4, r8
	movs	r3, #31
	ands	r3, r0
	ldr	r0, [r4, #12]
	ldr	r5, [r6, #4]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	movs	r1, #128
	subs	r0, r0, r5
	lsls	r1, r1, #11
	adds	r0, r0, r1
.L_081567fc:
	mov	r1, sl
	bl	sub_08002054
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r6, #20]
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	str	r0, [r6, #16]
	lsls	r2, r2, #1
	lsls	r0, r0, #1
	adds	r5, r5, r0
	adds	r3, r3, r2
	str	r5, [r6, #4]
	str	r3, [r6, #8]
	b.n	.L_08156886
.L_08156820:
	mov	r3, r9
	subs	r3, #3
	cmp	r3, #1
	bls.n	.L_08156836
	mov	r2, r9
	cmp	r2, #11
	beq.n	.L_08156836
	cmp	r2, #5
	beq.n	.L_08156836
	cmp	r2, #12
	bne.n	.L_0815683c
.L_08156836:
	movs	r3, #0
	str	r3, [r6, #16]
	b.n	.L_08156886
.L_0815683c:
	mov	r3, r9
	cmp	r3, #6
	bne.n	.L_08156864
	bl	sub_08014878
	ldr	r4, [sp, #76]
	mov	r5, r8
	adds	r2, r0, #0
	lsrs	r3, r4, #31
	ldr	r0, [r5, #12]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	adds	r0, r0, r3
	movs	r3, #15
	ands	r3, r2
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #4]
	mov	r1, sl
	b.n	.L_0815687e
.L_08156864:
	bl	sub_08014878
	mov	r1, r8
	ldr	r3, [sp, #76]
	adds	r2, r0, #0
	ldr	r0, [r1, #12]
	mov	r1, sl
	adds	r0, r0, r3
	movs	r3, #15
	ands	r3, r2
	lsls	r3, r3, #16
	subs	r0, r0, r3
	ldr	r3, [r6, #4]
.L_0815687e:
	subs	r0, r0, r3
.L_08156880:
	bl	sub_08002054
	str	r0, [r6, #16]
.L_08156886:
	movs	r3, #0
	str	r3, [r6, #24]
	ldr	r4, [sp, #132]
	ldr	r5, [sp, #96]
	adds	r4, #1
	adds	r6, #28
	str	r4, [sp, #132]
	cmp	r4, r5
	beq.n	.L_0815689a
	b.n	.L_081565e4
.L_0815689a:
	ldr	r6, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #136]
	ldr	r0, [sp, #96]
	adds	r1, #2
	adds	r6, r6, r0
	adds	r2, #1
	str	r6, [sp, #24]
	str	r1, [sp, #20]
	str	r2, [sp, #136]
	ldr	r4, [sp, #148]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	beq.n	.L_081568b8
	b.n	.L_081565a2
.L_081568b8:
	movs	r5, #0
	str	r5, [sp, #136]
	ldr	r3, [pc, #828]
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #3
.L_081568c4:
	str	r1, [r3, #0]
	ldr	r6, [sp, #136]
	adds	r3, #28
	adds	r6, #1
	str	r6, [sp, #136]
	cmp	r6, r2
	bne.n	.L_081568c4
	ldr	r1, [sp, #148]
	add	r6, sp, #164
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	adds	r1, r6, #0
	bl	sub_0815e21c
	movs	r2, #0
	str	r2, [sp, #136]
	ldr	r5, [pc, #792]
	mov	r8, r2
	movs	r7, #255
.L_081568ea:
	ldr	r3, [r6, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	mov	r3, r8
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	movs	r3, #1
	lsls	r0, r0, #9
	mov	r4, r8
	negs	r3, r3
	str	r0, [r5, #16]
	str	r4, [r5, #20]
	str	r3, [r5, #24]
	ldr	r0, [sp, #136]
	adds	r5, #28
	adds	r0, #1
	str	r0, [sp, #136]
	cmp	r0, #64
	bne.n	.L_081568ea
	mov	r3, r9
	subs	r3, #6
	cmp	r3, #1
	bhi.n	.L_0815694e
	ldr	r1, [sp, #148]
	ldr	r2, [sp, #84]
	ldr	r3, [r1, #20]
	ldr	r4, [sp, #88]
	ldr	r5, [sp, #96]
	subs	r3, #1
	muls	r3, r2
	adds	r2, r4, #0
	muls	r2, r5
	adds	r3, r3, r2
	adds	r3, #52
	b.n	.L_08156992
.L_0815694e:
	mov	r6, r9
	cmp	r6, #2
	beq.n	.L_08156964
	cmp	r6, #12
	beq.n	.L_08156964
	cmp	r6, #13
	beq.n	.L_08156964
	cmp	r6, #14
	beq.n	.L_08156964
	cmp	r6, #16
	bne.n	.L_0815697c
.L_08156964:
	ldr	r7, [sp, #148]
	ldr	r0, [sp, #84]
	ldr	r3, [r7, #20]
	ldr	r1, [sp, #88]
	ldr	r4, [sp, #96]
	subs	r3, #1
	muls	r3, r0
	adds	r2, r1, #0
	muls	r2, r4
	adds	r3, r3, r2
	adds	r3, #48
	b.n	.L_08156992
.L_0815697c:
	ldr	r5, [sp, #148]
	ldr	r6, [sp, #84]
	ldr	r3, [r5, #20]
	ldr	r7, [sp, #88]
	ldr	r0, [sp, #96]
	subs	r3, #1
	muls	r3, r6
	adds	r2, r7, #0
	muls	r2, r0
	adds	r3, r3, r2
	adds	r3, #32
.L_08156992:
	str	r3, [sp, #104]
	ldr	r2, [sp, #104]
	movs	r1, #0
	str	r1, [sp, #128]
	cmp	r2, #0
	bne.n	.L_081569a2
	bl	.L_081574a8
.L_081569a2:
	ldr	r3, [sp, #116]
	adds	r3, #12
	str	r3, [sp, #48]
.L_081569a8:
	ldr	r4, [sp, #108]
	cmp	r4, #0
	ble.n	.L_081569b2
	subs	r4, #1
	str	r4, [sp, #108]
.L_081569b2:
	mov	r5, r9
	cmp	r5, #6
	beq.n	.L_081569c4
	cmp	r5, #12
	beq.n	.L_081569c4
	cmp	r5, #13
	beq.n	.L_081569c4
	cmp	r5, #14
	bne.n	.L_081569de
.L_081569c4:
	ldr	r6, [sp, #128]
	cmp	r6, #4
	bne.n	.L_081569d0
	movs	r0, #136
	bl	sub_081c0010
.L_081569d0:
	ldr	r7, [sp, #128]
	cmp	r7, #32
	bne.n	.L_08156a12
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_08156a12
.L_081569de:
	mov	r0, r9
	cmp	r0, #2
	bne.n	.L_081569ec
	ldr	r1, [sp, #128]
	cmp	r1, #32
	bne.n	.L_08156a12
	b.n	.L_081569f8
.L_081569ec:
	mov	r2, r9
	cmp	r2, #7
	bne.n	.L_08156a00
	ldr	r3, [sp, #128]
	cmp	r3, #48
	bne.n	.L_08156a12
.L_081569f8:
	movs	r0, #133
	bl	sub_081180e8
	b.n	.L_08156a12
.L_08156a00:
	mov	r4, r9
	cmp	r4, #5
	beq.n	.L_08156a12
	ldr	r5, [sp, #128]
	cmp	r5, #16
	bne.n	.L_08156a12
	movs	r0, #133
	bl	sub_081180e8
.L_08156a12:
	ldr	r6, [sp, #148]
	ldr	r3, [r6, #28]
	cmp	r3, #1
	bne.n	.L_08156ac2
	ldr	r7, [sp, #128]
	lsls	r6, r7, #11
	adds	r0, r6, #0
	bl	sub_08002096
	add	r5, sp, #188
	ldr	r3, [r5, #0]
	negs	r0, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r0, r0, #2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_08002090
	ldr	r3, [r5, #4]
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r6, r0, #0
	ldr	r0, [sp, #128]
	subs	r7, #10
	subs	r6, #24
	cmp	r0, #69
	ble.n	.L_08156a5a
	lsls	r3, r0, #1
	subs	r3, r6, r3
	adds	r6, r3, #0
	adds	r6, #138
.L_08156a5a:
	ldr	r1, [sp, #148]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_08156a6c
	movs	r0, #188
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_08156a74
.L_08156a6c:
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
.L_08156a74:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r5, [r3, #0]
	ldr	r2, [sp, #128]
	str	r5, [sp, #124]
	cmp	r2, #3
	bgt.n	sub_08156aa0
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	movs	r4, #216
	ldr	r3, [sp, #144]
	lsls	r4, r4, #7
	adds	r4, #192
	adds	r1, r3, r4
	ldr	r0, [sp, #140]
	adds	r2, r7, #0
	adds	r3, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2314
	str	r3, [sp, #0]
	movs	r3, #40
	ldr	r2, [sp, #144]
	str	r3, [sp, #4]
	movs	r3, #216
	lsls	r3, r3, #7
	adds	r3, #192
	adds	r1, r2, r3
	ldr	r0, [sp, #140]
	adds	r2, r7, #0
	adds	r3, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_08156ac2:
	bl	sub_08014de4
	ldr	r0, [sp, #116]
	ldr	r1, [sp, #48]
	bl	sub_080156e8
	movs	r4, #0
	str	r4, [sp, #72]
	str	r4, [sp, #136]
	ldr	r5, [sp, #148]
	ldr	r2, [r5, #20]
	cmp	r2, #0
	bne.n	.L_08156ade
	b.n	.L_08157098
.L_08156ade:
	movs	r6, #36
	str	r6, [sp, #36]
	str	r4, [sp, #32]
	str	r4, [sp, #28]
.L_08156ae6:
	ldr	r7, [sp, #36]
	ldr	r2, [sp, #148]
	ldrsh	r0, [r7, r2]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r4, [sp, #96]
	movs	r3, #0
	str	r0, [sp, #68]
	str	r3, [sp, #132]
	cmp	r4, #0
	bne.n	.L_08156b00
	b.n	.L_08157072
.L_08156b00:
	ldr	r6, [sp, #72]
	ldr	r7, [pc, #256]
	lsls	r3, r6, #3
	ldr	r5, [sp, #28]
	subs	r3, r3, r6
	lsls	r3, r3, #2
	adds	r7, r3, r7
	str	r5, [sp, #44]
	str	r7, [sp, #40]
.L_08156b12:
	ldr	r0, [sp, #88]
	ldr	r1, [sp, #132]
	ldr	r2, [sp, #44]
	adds	r3, r0, #0
	muls	r3, r1
	ldr	r4, [sp, #128]
	adds	r3, r3, r2
	cmp	r3, r4
	blt.n	.L_08156b26
	b.n	sub_08157058
.L_08156b26:
	ldr	r5, [sp, #32]
	ldr	r6, [sp, #144]
	adds	r2, r5, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	mov	r0, r9
	lsls	r3, r3, #2
	subs	r0, #15
	str	r5, [sp, #64]
	adds	r7, r6, r3
	str	r0, [sp, #60]
	cmp	r0, #2
	bhi.n	.L_08156b48
	ldr	r3, [r7, #24]
	cmp	r3, #1
	bne.n	.L_08156b48
	b.n	sub_08157058
.L_08156b48:
	add	r6, sp, #152
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	mov	r1, r9
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #12]
	adds	r2, r2, r3
	str	r2, [r7, #0]
	ldr	r3, [r7, #16]
	ldr	r2, [r7, #4]
	adds	r2, r2, r3
	str	r2, [r7, #4]
	ldr	r3, [r7, #20]
	ldr	r2, [r7, #8]
	adds	r2, r2, r3
	str	r2, [r7, #8]
	cmp	r1, #6
	beq.n	.L_08156b7a
	cmp	r1, #12
	bne.n	.L_08156be0
.L_08156b7a:
	ldr	r5, [pc, #140]
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #2
	mov	r8, r6
	movs	r4, #255
.L_08156b86:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_08156bd4
	mov	r0, r8
	ldr	r3, [r0, #0]
	str	r1, [sp, #16]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	str	r2, [sp, #12]
	ldr	r3, [r0, #4]
	str	r4, [sp, #8]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ldr	r4, [sp, #8]
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ldr	r4, [sp, #8]
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r2, [sp, #12]
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	adds	r2, #1
	str	r3, [r5, #24]
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r2, #2
	beq.n	.L_08156be0
.L_08156bd4:
	movs	r3, #240
	adds	r1, #1
	lsls	r3, r3, #2
	adds	r5, #28
	cmp	r1, r3
	bne.n	.L_08156b86
.L_08156be0:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	beq.n	.L_08156be8
	b.n	.L_08156dd0
.L_08156be8:
	ldr	r4, [sp, #68]
	ldr	r2, [r7, #0]
	ldr	r1, [r4, #8]
	cmp	r1, #0
	bge.n	.L_08156c0c
	lsrs	r3, r2, #31
	cmp	r3, #0
	bne.n	.L_08156c16
	b.n	.L_08156dd0
	movs	r0, r0
	.4byte 0x02010018
	.4byte 0x02016900
	.4byte 0x02010000
	.2byte 0x3800
	.2byte 0x0201
.L_08156c0c:
	mvns	r3, r2
	lsrs	r3, r3, #31
	cmp	r3, #0
	bne.n	.L_08156c16
	b.n	.L_08156dd0
.L_08156c16:
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_08156c1e
	negs	r3, r3
.L_08156c1e:
	adds	r2, r1, #0
	cmp	r2, #0
	bge.n	.L_08156c26
	negs	r2, r2
.L_08156c26:
	cmp	r3, r2
	bge.n	.L_08156c2c
	b.n	.L_08156dd0
.L_08156c2c:
	movs	r5, #0
	movs	r3, #1
	mov	r0, r9
	str	r5, [sp, #56]
	ldr	r2, [sp, #40]
	str	r3, [r7, #24]
	cmp	r0, #5
	bne.n	.L_08156c46
	movs	r0, #134
	str	r2, [sp, #12]
	bl	sub_081180e8
	b.n	.L_08156c6a
.L_08156c46:
	mov	r1, r9
	cmp	r1, #6
	beq.n	.L_08156c6c
	cmp	r1, #12
	beq.n	.L_08156c6c
	cmp	r1, #13
	beq.n	.L_08156c6c
	cmp	r1, #14
	beq.n	.L_08156c6c
	ldr	r3, [sp, #108]
	cmp	r3, #0
	bne.n	.L_08156c6c
	movs	r4, #8
	movs	r0, #132
	str	r2, [sp, #12]
	str	r4, [sp, #108]
	bl	sub_081c0010
.L_08156c6a:
	ldr	r2, [sp, #12]
.L_08156c6c:
	mov	r5, r9
	cmp	r5, #2
	bne.n	.L_08156ca0
	str	r2, [sp, #12]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r5, r3, #12
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	ldr	r2, [sp, #12]
	cmp	r0, #0
	beq.n	.L_08156c94
	ldr	r3, [r7, #16]
	adds	r3, r3, r5
	b.n	.L_08156c98
.L_08156c94:
	ldr	r3, [r7, #16]
	subs	r3, r3, r5
.L_08156c98:
	str	r3, [r7, #16]
	ldr	r3, [r7, #12]
	negs	r3, r3
	str	r3, [r7, #12]
.L_08156ca0:
	movs	r3, #1
	str	r3, [r2, #24]
	mov	r0, r9
	ldr	r3, [r6, #0]
	str	r3, [r2, #0]
	ldr	r3, [r6, #4]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	cmp	r0, #7
	beq.n	.L_08156cc4
	ldr	r1, [sp, #144]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
.L_08156cc4:
	ldr	r1, [sp, #148]
	ldr	r4, [sp, #36]
	movs	r3, #8
	ldrsh	r0, [r4, r1]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	ldr	r3, [sp, #136]
	bl	sub_0814cd48
	mov	r2, r9
	cmp	r2, #7
	beq.n	.L_08156d18
	cmp	r2, #9
	beq.n	.L_08156d18
	cmp	r2, #10
	beq.n	.L_08156d18
	cmp	r2, #18
	beq.n	.L_08156d18
	cmp	r2, #14
	beq.n	.L_08156d18
	cmp	r2, #15
	beq.n	.L_08156d18
	cmp	r2, #16
	beq.n	.L_08156d18
	cmp	r2, #17
	beq.n	.L_08156d18
	cmp	r2, #5
	bne.n	.L_08156d0c
	ldr	r3, [sp, #36]
	ldr	r5, [sp, #148]
	movs	r1, #4
	ldrsh	r0, [r3, r5]
	bl	sub_08118088
	b.n	.L_08156d18
.L_08156d0c:
	ldr	r1, [sp, #148]
	ldr	r3, [sp, #36]
	ldrsh	r0, [r1, r3]
	movs	r1, #5
	bl	sub_08118088
.L_08156d18:
	mov	r3, r9
	subs	r3, #16
	cmp	r3, #1
	bhi.n	.L_08156d3c
	ldr	r4, [sp, #132]
	ldr	r5, [pc, #756]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	lsls	r3, r3, #2
	movs	r1, #0
	movs	r2, #0
	adds	r3, r3, r5
.L_08156d30:
	adds	r1, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #1
	bne.n	.L_08156d30
	b.n	.L_08156dd0
.L_08156d3c:
	mov	r0, r9
	cmp	r0, #15
	beq.n	.L_08156dd0
	movs	r2, #5
	eors	r2, r0
	negs	r3, r2
	orrs	r3, r2
	lsrs	r2, r3, #31
	movs	r3, #12
	subs	r2, r3, r2
	movs	r3, #255
	mov	sl, r3
	ldr	r3, [sp, #92]
	ldr	r0, [pc, #708]
	add	r3, r9
	ldr	r5, [pc, #708]
	adds	r3, #4
	movs	r1, #100
	adds	r4, r6, #0
	mov	fp, r0
	mov	r8, r3
.L_08156d66:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_08156dc4
	ldr	r3, [r4, #0]
	str	r1, [sp, #16]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	str	r2, [sp, #12]
	ldr	r3, [r4, #4]
	str	r4, [sp, #8]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	mov	r3, sl
	ldr	r2, [sp, #12]
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r2
	str	r0, [r5, #12]
	bl	sub_08014878
	mov	r3, sl
	ldr	r2, [sp, #12]
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r2
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	ldr	r0, [sp, #56]
	mov	r3, fp
	adds	r0, #1
	str	r0, [sp, #56]
	mov	r0, r8
	ldrb	r3, [r3, r0]
	ldr	r1, [sp, #16]
	mov	ip, r3
	ldr	r3, [sp, #56]
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r3, ip
	beq.n	.L_08156dd0
.L_08156dc4:
	movs	r0, #128
	adds	r1, #1
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r1, r0
	bne.n	.L_08156d66
.L_08156dd0:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_08156dde
	cmp	r1, #5
	beq.n	.L_08156dde
	cmp	r1, #8
	bne.n	.L_08156e72
.L_08156dde:
	ldr	r2, [sp, #148]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08156df0
	movs	r0, #188
	movs	r1, #23
	bl	sub_081963ec
	b.n	.L_08156df8
.L_08156df0:
	movs	r0, #188
	movs	r1, #19
	bl	sub_081963ec
.L_08156df8:
	ldr	r4, [sp, #100]
	ldr	r3, [pc, #552]
	movs	r1, #32
	ldrb	r0, [r3, r4]
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	movs	r5, #192
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	lsls	r5, r5, #18
	ldr	r7, [sp, #144]
	adds	r5, #188
	ldr	r4, [r5, #0]
	movs	r5, #184
	lsls	r5, r5, #5
	subs	r3, r3, r0
	adds	r1, r7, r5
	subs	r2, #16
	ldr	r0, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	ldr	r7, [sp, #148]
	ldr	r3, [r7, #4]
	cmp	r3, #1
	bne.n	.L_08156e3a
	movs	r0, #188
	movs	r1, #31
	bl	sub_081963ec
	b.n	.L_08156e42
.L_08156e3a:
	movs	r0, #188
	movs	r1, #27
	bl	sub_081963ec
.L_08156e42:
	movs	r1, #32
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r0, [sp, #100]
	str	r1, [sp, #0]
	ldr	r1, [pc, #468]
	ldr	r5, [sp, #144]
	ldrb	r1, [r1, r0]
	movs	r6, #184
	str	r1, [sp, #4]
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #188
	lsls	r6, r6, #5
	ldr	r4, [r1, #0]
	subs	r2, #16
	ldr	r0, [sp, #140]
	adds	r1, r5, r6
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	b.n	sub_08157058
.L_08156e72:
	mov	r0, r9
	cmp	r0, #1
	bne.n	.L_08156ebc
	ldr	r0, [r7, #0]
	cmp	r0, #0
	bge.n	.L_08156e80
	negs	r0, r0
.L_08156e80:
	ldr	r1, [sp, #68]
	ldr	r3, [r1, #8]
	cmp	r3, #0
	bge.n	.L_08156e8a
	negs	r3, r3
.L_08156e8a:
	cmp	r0, r3
	ble.n	.L_08156e90
	b.n	sub_08157058
.L_08156e90:
	movs	r1, #6
	ldr	r0, [sp, #128]
	bl	sub_08002064
	lsls	r1, r0, #1
	ldr	r2, [sp, #144]
	adds	r1, r1, r0
	lsls	r1, r1, #8
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	ldr	r2, [r6, #0]
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #12
	ldr	r0, [sp, #140]
	b.n	.L_08157010
.L_08156ebc:
	mov	r5, r9
	cmp	r5, #7
	beq.n	.L_08156ece
	cmp	r5, #9
	beq.n	.L_08156ece
	cmp	r5, #10
	beq.n	.L_08156ece
	cmp	r5, #18
	bne.n	.L_08156ef6
.L_08156ece:
	ldr	r7, [sp, #132]
	ldr	r2, [pc, #340]
	movs	r3, #3
	ands	r3, r7
	lsls	r3, r3, #1
	ldrh	r1, [r2, r3]
	ldr	r0, [sp, #144]
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #3
	ldr	r3, [r6, #4]
	adds	r1, r1, r2
	ldr	r2, [r6, #0]
	movs	r0, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #4
	subs	r3, #4
	ldr	r0, [sp, #140]
	b.n	.L_08157010
.L_08156ef6:
	mov	r5, r9
	cmp	r5, #2
	bne.n	.L_08156f26
	movs	r1, #6
	ldr	r0, [sp, #132]
	bl	sub_08002064
	ldr	r7, [sp, #144]
	adds	r1, r0, #0
	lsls	r1, r1, #7
	movs	r0, #224
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	lsls	r0, r0, #3
	adds	r1, r7, r1
	adds	r1, r1, r0
	movs	r0, #8
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	subs	r2, #4
	subs	r3, #8
	ldr	r0, [sp, #140]
	b.n	.L_08157010
.L_08156f26:
	mov	r5, r9
	cmp	r5, #3
	bne.n	.L_08156f44
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r6, [sp, #144]
	movs	r1, #18
	movs	r7, #236
	str	r1, [sp, #0]
	lsls	r7, r7, #3
	movs	r1, #13
	str	r1, [sp, #4]
	subs	r2, #9
	subs	r3, #7
	b.n	.L_0815700c
.L_08156f44:
	mov	r5, r9
	cmp	r5, #4
	bne.n	.L_08156f62
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r6, [sp, #144]
	movs	r1, #12
	movs	r7, #224
	str	r1, [sp, #0]
	lsls	r7, r7, #3
	movs	r1, #8
	str	r1, [sp, #4]
	subs	r2, #6
	subs	r3, #4
	b.n	.L_0815700c
.L_08156f62:
	ldr	r5, [sp, #60]
	cmp	r5, #2
	bhi.n	.L_08156f80
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r6, [sp, #144]
	movs	r1, #17
	movs	r7, #224
	str	r1, [sp, #0]
	lsls	r7, r7, #3
	movs	r1, #2
	str	r1, [sp, #4]
	subs	r2, #8
	subs	r3, #1
	b.n	.L_0815700c
.L_08156f80:
	mov	r5, r9
	cmp	r5, #13
	bne.n	.L_08156f9a
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r6, [sp, #144]
	movs	r7, #224
	movs	r1, #24
	lsls	r7, r7, #3
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #12
	b.n	.L_0815700a
.L_08156f9a:
	mov	r5, r9
	cmp	r5, #14
	bne.n	.L_08156fee
	ldr	r1, [sp, #132]
	ldr	r7, [sp, #64]
	adds	r0, r7, r1
	movs	r1, #5
	bl	sub_08002064
	lsls	r3, r0, #1
	adds	r5, r3, r0
	ldr	r0, [sp, #128]
	cmp	r0, #0
	bge.n	.L_08156fb8
	adds	r0, #3
.L_08156fb8:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r2, [pc, #104]
	adds	r0, r5, r0
	lsls	r3, r0, #2
	ldr	r1, [r2, r3]
	ldr	r2, [sp, #144]
	movs	r3, #240
	adds	r1, r2, r1
	lsls	r3, r3, #4
	adds	r1, r1, r3
	ldr	r3, [pc, #92]
	ldr	r2, [r6, #0]
	ldrb	r5, [r3, r0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #84]
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #140]
	b.n	.L_08157010
.L_08156fee:
	mov	r5, r9
	cmp	r5, #11
	bne.n	.L_08157038
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	movs	r7, #128
	ldr	r6, [sp, #144]
	movs	r1, #29
	lsls	r7, r7, #4
	str	r1, [sp, #0]
	adds	r7, #74
	movs	r1, #23
	str	r1, [sp, #4]
	subs	r2, #15
.L_0815700a:
	subs	r3, #12
.L_0815700c:
	ldr	r0, [sp, #140]
	adds	r1, r6, r7
.L_08157010:
	ldr	r4, [sp, #120]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01f
	.4byte 0x02016918
	.4byte 0x08198478
	.4byte 0x02010af0
	.4byte 0x08198475
	.4byte 0x08198502
	.4byte 0x08197834
	.4byte 0x0819781a
	.2byte 0x7826
	.2byte 0x0819
.L_08157038:
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r5, [sp, #144]
	movs	r1, #40
	movs	r6, #224
	str	r1, [sp, #0]
	lsls	r6, r6, #3
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #20
	subs	r3, #32
	ldr	r0, [sp, #140]
	adds	r1, r5, r6
	ldr	r7, [sp, #120]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9821
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #72]
	ldr	r3, [sp, #96]
	adds	r0, #1
	adds	r1, #28
	adds	r2, #1
	str	r0, [sp, #132]
	str	r1, [sp, #40]
	str	r2, [sp, #72]
	cmp	r0, r3
	beq.n	.L_08157072
	b.n	.L_08156b12
.L_08157072:
	ldr	r4, [sp, #36]
	ldr	r5, [sp, #32]
	ldr	r7, [sp, #28]
	ldr	r1, [sp, #136]
	ldr	r6, [sp, #96]
	ldr	r0, [sp, #84]
	adds	r4, #2
	adds	r5, r5, r6
	adds	r7, r7, r0
	adds	r1, #1
	str	r4, [sp, #36]
	str	r5, [sp, #32]
	str	r7, [sp, #28]
	str	r1, [sp, #136]
	ldr	r3, [sp, #148]
	ldr	r2, [r3, #20]
	cmp	r1, r2
	beq.n	.L_08157098
	b.n	.L_08156ae6
.L_08157098:
	mov	r4, r9
	cmp	r4, #0
	beq.n	.L_081570a8
	cmp	r4, #5
	beq.n	.L_081570a8
	cmp	r4, #8
	beq.n	.L_081570a8
	b.n	.L_0815722a
.L_081570a8:
	ldr	r6, [sp, #96]
	movs	r5, #0
	adds	r3, r6, #0
	muls	r3, r2
	str	r5, [sp, #136]
	cmp	r3, #0
	bne.n	.L_081570b8
	b.n	.L_0815722a
.L_081570b8:
	ldr	r0, [pc, #688]
	ldr	r1, [pc, #692]
	ldr	r3, [pc, #692]
	ldr	r7, [pc, #696]
	mov	fp, r0
	mov	sl, r1
	mov	r8, r3
.L_081570c6:
	ldr	r1, [r7, #24]
	cmp	r1, #1
	beq.n	.L_081570ce
	b.n	.L_08157214
.L_081570ce:
	ldr	r3, [r7, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r6, r3, #1
	ldr	r3, [sp, #136]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0815716a
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	ldr	r4, [pc, #660]
	lsls	r5, r6, #1
	ldrh	r1, [r4, r5]
	ldr	r0, [sp, #144]
	mov	r3, fp
	ldrb	r4, [r3, r6]
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #3
	adds	r1, r1, r2
	mov	r3, sl
	ldr	r2, [r7, #0]
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r2, r2, r4
	mov	r4, r8
	subs	r3, r3, r0
	ldrb	r0, [r4, r6]
	str	r0, [sp, #4]
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r4, [r0, #0]
	ldr	r0, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	ldr	r2, [pc, #592]
	ldr	r3, [sp, #144]
	ldrh	r1, [r2, r5]
	mov	r5, sl
	adds	r1, r3, r1
	ldr	r3, [pc, #584]
	movs	r4, #224
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	lsls	r4, r4, #3
	adds	r3, r3, r0
	ldrb	r0, [r5, r6]
	adds	r1, r1, r4
	mov	r4, fp
	subs	r3, r3, r0
	ldrb	r0, [r4, r6]
	ldr	r2, [r7, #0]
	str	r0, [sp, #0]
	mov	r5, r8
	ldrb	r0, [r5, r6]
	movs	r6, #192
	str	r0, [sp, #4]
	lsls	r6, r6, #18
	adds	r6, #188
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	b.n	.L_081571fc
.L_0815716a:
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	ldr	r0, [pc, #520]
	lsls	r5, r6, #1
	ldrh	r1, [r0, r5]
	ldr	r2, [sp, #144]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r1, r2, r1
	adds	r3, #138
	mov	r4, sl
	adds	r1, r1, r3
	ldr	r2, [r7, #0]
	ldrb	r3, [r4, r6]
	mov	r0, fp
	ldrb	r4, [r0, r6]
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	subs	r3, r3, r4
	mov	ip, r3
	mov	r3, r8
	ldrb	r0, [r3, r6]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r4, [r0, #0]
	mov	r3, ip
	ldr	r0, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	ldr	r2, [pc, #444]
	ldr	r3, [sp, #144]
	ldrh	r1, [r2, r5]
	movs	r4, #200
	adds	r1, r3, r1
	ldr	r3, [pc, #440]
	ldr	r2, [r7, #0]
	ldrb	r3, [r3, r6]
	lsls	r4, r4, #5
	adds	r4, #138
	adds	r1, r1, r4
	mov	r5, sl
	mov	r4, r8
	ldrb	r0, [r4, r6]
	adds	r2, r2, r3
	ldrb	r3, [r5, r6]
	mov	r5, fp
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r0, [sp, #0]
	ldrb	r0, [r5, r6]
	movs	r6, #192
	str	r0, [sp, #4]
	lsls	r6, r6, #18
	adds	r6, #188
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_081571fc:
	ldr	r3, [r7, #8]
	adds	r3, #1
	str	r3, [r7, #8]
	cmp	r3, #12
	bne.n	.L_08157210
	movs	r3, #0
	str	r3, [r7, #24]
	ldr	r0, [sp, #148]
	ldr	r2, [r0, #20]
	b.n	.L_08157214
.L_08157210:
	ldr	r1, [sp, #148]
	ldr	r2, [r1, #20]
.L_08157214:
	ldr	r3, [sp, #136]
	ldr	r4, [sp, #96]
	adds	r3, #1
	str	r3, [sp, #136]
	ldr	r5, [sp, #136]
	adds	r3, r4, #0
	muls	r3, r2
	adds	r7, #28
	cmp	r5, r3
	beq.n	.L_0815722a
	b.n	.L_081570c6
.L_0815722a:
	movs	r6, #100
	str	r6, [sp, #136]
	ldr	r5, [pc, #340]
.L_08157230:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	ble.n	.L_081572a0
	asrs	r0, r0, #3
	adds	r0, #1
	ldr	r2, [pc, #332]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	ldr	r7, [sp, #112]
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	adds	r1, r7, r1
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #140]
	ldr	r7, [sp, #120]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4648
	cmp	r0, #13
	bne.n	.L_08157278
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #7
	bl	sub_08138086
	b.n	.L_08157284
.L_08157278:
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_08138086
.L_08157284:
	ldr	r3, [r5, #4]
	movs	r1, #224
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_0815729a
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
.L_0815729a:
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_081572a0:
	ldr	r2, [sp, #136]
	movs	r3, #128
	adds	r2, #1
	lsls	r3, r3, #2
	adds	r5, #28
	str	r2, [sp, #136]
	cmp	r2, r3
	bne.n	.L_08157230
	ldr	r6, [pc, #212]
	ldr	r5, [pc, #216]
.L_081572b4:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	ble.n	.L_081572f6
	asrs	r0, r0, #4
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #112]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r7, r1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #140]
	ldr	r4, [sp, #120]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #164]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_081572f6:
	ldr	r7, [sp, #136]
	movs	r0, #240
	adds	r7, #1
	lsls	r0, r0, #2
	adds	r5, #28
	str	r7, [sp, #136]
	cmp	r7, r0
	bne.n	.L_081572b4
	mov	r1, r9
	cmp	r1, #16
	bne.n	.L_0815739c
	movs	r2, #0
	str	r2, [sp, #136]
	ldr	r5, [pc, #128]
.L_08157312:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0815735e
	cmp	r3, #0
	bge.n	.L_0815731e
	adds	r3, #3
.L_0815731e:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	adds	r1, r1, r3
	ldr	r3, [sp, #144]
	lsls	r1, r1, #7
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r1, r3, r1
	movs	r4, #224
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	movs	r0, #24
	lsls	r4, r4, #3
	str	r0, [sp, #0]
	adds	r4, #34
	movs	r0, #48
	adds	r1, r1, r4
	subs	r3, #24
	str	r0, [sp, #4]
	subs	r2, #12
	ldr	r0, [sp, #140]
	ldr	r4, [sp, #120]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #68]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0815735e:
	ldr	r6, [sp, #136]
	adds	r5, #28
	adds	r6, #1
	str	r6, [sp, #136]
	cmp	r6, #64
	bne.n	.L_08157312
	b.n	.L_08157478
	.4byte 0x081984d7
	.4byte 0x081984ec
	.4byte 0x081984de
	.4byte 0x02010000
	.4byte 0x081984f4
	.4byte 0x081984e5
	.4byte 0x02010af0
	.4byte 0x08197410
	.4byte 0x02013800
	.4byte 0xffffc000
	.4byte 0x02016900
	.2byte 0xfc00
	.2byte 0xffff
.L_0815739c:
	mov	r7, r9
	cmp	r7, #17
	bne.n	.L_08157478
	movs	r0, #0
	ldr	r7, [sp, #128]
	ldr	r2, [pc, #288]
	str	r0, [sp, #136]
	movs	r1, #3
	ands	r7, r1
	mov	fp, r2
.L_081573b0:
	mov	r4, fp
	ldr	r3, [r4, #24]
	cmp	r3, #7
	bhi.n	.L_0815746a
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #31
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #4
	mov	r1, fp
	movs	r2, #2
	ldrsh	r1, [r1, r2]
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [pc, #232]
	mov	r8, r1
	asrs	r3, r3, #17
	add	r8, r3
	ldrb	r3, [r2, r7]
	mov	r4, r8
	lsrs	r3, r3, #1
	subs	r4, r4, r3
	adds	r0, r6, #0
	mov	r8, r4
	mov	sl, r2
	bl	sub_08002090
	mov	r2, fp
	adds	r3, r5, #0
	muls	r3, r0
	movs	r1, #6
	ldrsh	r6, [r2, r1]
	ldr	r5, [pc, #200]
	asrs	r3, r3, #17
	subs	r6, r6, r3
	ldrb	r3, [r5, r7]
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	bl	sub_08014878
	ldr	r3, [pc, #188]
	movs	r4, #3
	ands	r0, r4
	ldrb	r2, [r3, r0]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #0
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r2, [pc, #168]
	lsls	r3, r7, #1
	mov	r4, sl
	ldrh	r1, [r2, r3]
	ldrb	r3, [r4, r7]
	ldr	r0, [sp, #144]
	str	r3, [sp, #0]
	movs	r2, #224
	ldrb	r3, [r5, r7]
	movs	r5, #192
	str	r3, [sp, #4]
	lsls	r2, r2, #3
	lsls	r5, r5, #18
	adds	r1, r0, r1
	adds	r2, #34
	adds	r5, #188
	adds	r3, r6, #0
	adds	r1, r1, r2
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #140]
	mov	r2, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465e
	movs	r0, #188
	bl	sub_0801314c
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_0815746a:
	ldr	r1, [sp, #136]
	movs	r0, #28
	adds	r1, #1
	add	fp, r0
	str	r1, [sp, #136]
	cmp	r1, #64
	bne.n	.L_081573b0
.L_08157478:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #144]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #128]
	ldr	r6, [sp, #104]
	adds	r5, #1
	str	r5, [sp, #128]
	cmp	r5, r6
	beq.n	.L_081574a8
	bl	.L_081569a8
.L_081574a8:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #200
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02016900
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x0819850a
	.4byte 0x08197486
	.4byte 0x08143001
