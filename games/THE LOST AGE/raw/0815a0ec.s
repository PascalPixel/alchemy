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
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815a5e6, 0x0815a5e6
	.set sub_0815a60c, 0x0815a60c
	.set sub_0815a6fa, 0x0815a6fa
	.set sub_0815ae18, 0x0815ae18
	.set sub_0815afac, 0x0815afac
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Overlay_0815a0ec
Overlay_0815a0ec:
	push	{lr}
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0815a0fc
	movs	r1, #6
	bl	.L_0815a110
	b.n	.L_0815a10e
.L_0815a0fc:
	cmp	r3, #1
	bne.n	.L_0815a108
	movs	r1, #7
	bl	.L_0815a110
	b.n	.L_0815a10e
.L_0815a108:
	movs	r1, #8
	bl	.L_0815a110
.L_0815a10e:
	pop	{pc}
.L_0815a110:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #152
	str	r0, [sp, #68]
	str	r1, [sp, #64]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #60]
	movs	r0, #0
	ldr	r1, [r3, #96]
	str	r1, [sp, #56]
	ldr	r2, [r3, #100]
	str	r2, [sp, #48]
	ldr	r3, [r3, #48]
	str	r3, [sp, #44]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r5, [sp, #68]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L_0815a162
	ldr	r1, [r5, #4]
	movs	r3, #71
	lsls	r1, r1, #4
	orrs	r1, r3
	add	r2, sp, #116
	add	r3, sp, #104
	adds	r0, r5, #0
	bl	sub_0815585c
.L_0815a162:
	ldr	r1, [sp, #48]
	ldr	r0, [pc, #20]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #60]
	movs	r5, #224
	lsls	r5, r5, #3
	b.n	.L_0815a180
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x0134
	.2byte 0x0000
.L_0815a180:
	adds	r1, r2, r5
	ldr	r0, [pc, #884]
	movs	r3, #0
	movs	r2, #1
	bl	sub_08157cf4
	movs	r0, #184
	lsls	r0, r0, #6
	movs	r1, #144
	movs	r3, #0
	adds	r0, #136
	lsls	r1, r1, #1
	mov	ip, r3
	mov	lr, r0
	mov	r8, r5
	mov	sl, r1
	movs	r7, #0
	movs	r6, #0
.L_0815a1a4:
	ldr	r5, [sp, #60]
	mov	r2, r8
	adds	r3, r7, r2
	movs	r0, #0
	adds	r4, r6, #0
	adds	r1, r3, r5
.L_0815a1b0:
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	ldr	r2, [pc, #836]
	asrs	r3, r3, #1
	adds	r3, r4, r3
	adds	r3, r3, r2
	ldrb	r2, [r1, #0]
	mov	r5, lr
	adds	r0, #1
	adds	r1, #1
	strb	r2, [r3, r5]
	cmp	r0, #40
	bne.n	.L_0815a1b0
	movs	r0, #1
	add	ip, r0
	adds	r7, #40
	adds	r6, #20
	cmp	ip, sl
	bne.n	.L_0815a1a4
	ldr	r2, [sp, #60]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #800]
	movs	r3, #0
	movs	r2, #1
	bl	sub_08157cf4
	ldr	r5, [sp, #64]
	ldr	r3, [pc, #792]
	lsls	r5, r5, #3
	str	r5, [sp, #40]
	ldrb	r3, [r3, r5]
	cmp	r3, #0
	bne.n	.L_0815a20a
	ldr	r2, [sp, #60]
	movs	r3, #156
	lsls	r3, r3, #5
	adds	r1, r2, r3
	ldr	r0, [pc, #776]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_0815a21c
.L_0815a20a:
	ldr	r5, [sp, #60]
	movs	r2, #156
	lsls	r2, r2, #5
	adds	r1, r5, r2
	ldr	r0, [pc, #760]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_0815a21c:
	ldr	r0, [pc, #752]
	ldr	r1, [pc, #732]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #740]
	ldr	r1, [pc, #744]
	bl	sub_08157cf4
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #716]
	adds	r3, #1
	ldrb	r3, [r2, r3]
	cmp	r3, #1
	beq.n	.L_0815a254
	cmp	r3, #1
	bgt.n	.L_0815a24a
	cmp	r3, #0
	beq.n	.L_0815a250
	b.n	.L_0815a25c
.L_0815a24a:
	cmp	r3, #2
	beq.n	.L_0815a258
	b.n	.L_0815a25c
.L_0815a250:
	ldr	r0, [pc, #712]
	b.n	.L_0815a25e
.L_0815a254:
	ldr	r0, [pc, #712]
	b.n	.L_0815a25e
.L_0815a258:
	ldr	r0, [pc, #688]
	b.n	.L_0815a25e
.L_0815a25c:
	ldr	r0, [pc, #708]
.L_0815a25e:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #704]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b10
	cmp	r3, #12
	bne.n	.L_0815a290
	ldr	r5, [sp, #68]
	movs	r3, #192
	ldr	r0, [r5, #8]
	movs	r2, #36
	ldrsh	r1, [r5, r2]
	lsls	r3, r3, #11
	movs	r2, #8
	bl	sub_08118078
	movs	r0, #7
	bl	sub_08013560
	b.n	.L_0815a2a0
.L_0815a290:
	ldr	r3, [sp, #68]
	movs	r2, #4
	ldr	r0, [r3, #8]
	movs	r5, #36
	ldrsh	r1, [r3, r5]
	movs	r3, #0
	bl	sub_08118078
.L_0815a2a0:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #68]
	mov	r3, sp
	adds	r3, #128
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r3, #0
	str	r3, [sp, #36]
	bl	sub_0815e20c
	ldr	r5, [sp, #60]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r5, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r5, r1
	movs	r5, #200
	movs	r3, #75
	lsls	r5, r5, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #596]
	adds	r1, r5, #0
	bl	sub_080145a8
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #548]
	adds	r3, #7
	ldrb	r3, [r2, r3]
	cmp	r3, #2
	bne.n	.L_0815a2f0
	ldr	r0, [pc, #580]
	adds	r1, r5, #0
	bl	sub_080145a8
.L_0815a2f0:
	ldr	r3, [sp, #68]
	movs	r5, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118098
	mov	sl, r5
	ldr	r6, [r0, #0]
	ldr	r5, [pc, #560]
	movs	r7, #255
.L_0815a304:
	ldr	r3, [r6, #8]
	movs	r0, #200
	str	r3, [r5, #0]
	lsls	r0, r0, #13
	ldr	r3, [r6, #12]
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_0815a344
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_0815a344:
	movs	r3, #1
	movs	r1, #1
	movs	r2, #128
	negs	r3, r3
	add	sl, r1
	lsls	r2, r2, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_0815a304
	ldr	r5, [sp, #60]
	movs	r3, #0
	mov	sl, r3
	movs	r7, #0
	movs	r6, #255
.L_0815a362:
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	str	r7, [r5, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08014878
	mov	r1, sl
	ands	r0, r6
	movs	r2, #1
	negs	r3, r1
	subs	r0, #128
	add	sl, r2
	str	r3, [r5, #24]
	lsls	r0, r0, #9
	mov	r3, sl
	str	r0, [r5, #16]
	str	r7, [r5, #20]
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_0815a362
	movs	r5, #0
	ldr	r3, [sp, #40]
	str	r5, [sp, #52]
	ldr	r2, [pc, #344]
	adds	r3, #5
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_0815a3b6
	b.n	.L_0815a97e
.L_0815a3b6:
	ldr	r2, [sp, #44]
	mov	r0, sp
	mov	r1, sp
	adds	r0, #140
	adds	r1, #72
	adds	r2, #12
	str	r0, [sp, #24]
	str	r1, [sp, #28]
	str	r2, [sp, #20]
.L_0815a3c8:
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #52]
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r0, #0
	movs	r7, #192
	adds	r6, r3, r5
	mov	sl, r0
	lsls	r7, r7, #11
	lsls	r5, r1, #11
.L_0815a3dc:
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	subs	r3, r7, r3
	asrs	r3, r3, #10
	stmia	r6!, {r3}
	movs	r3, #1
	movs	r2, #128
	add	sl, r3
	lsls	r2, r2, #4
	mov	r0, sl
	adds	r5, r5, r2
	cmp	r0, #160
	bne.n	.L_0815a3dc
	ldr	r1, [sp, #64]
	cmp	r1, #10
	bne.n	.L_0815a410
	ldr	r2, [sp, #52]
	cmp	r2, #8
	bne.n	.L_0815a410
	movs	r0, #221
	bl	sub_081c0010
.L_0815a410:
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #240]
	adds	r3, #2
	ldrb	r2, [r2, r3]
	ldr	r3, [sp, #68]
	str	r2, [sp, #32]
	ldr	r1, [sp, #24]
	ldr	r0, [r3, #8]
	bl	sub_0815e20c
	ldr	r5, [sp, #24]
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0815a44a
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #27
	bl	sub_081963ec
	b.n	.L_0815a45a
.L_0815a44a:
	movs	r1, #23
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #31
	bl	sub_081963ec
.L_0815a45a:
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r3, [r1, #104]
	movs	r2, #192
	str	r3, [sp, #72]
	lsls	r2, r2, #18
	adds	r2, #188
	ldr	r3, [r2, #0]
	ldr	r5, [sp, #28]
	movs	r0, #0
	str	r3, [r5, #4]
	ldr	r1, [sp, #32]
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L_0815a47a
	b.n	.L_0815a710
.L_0815a47a:
	ldr	r2, [sp, #36]
	str	r0, [sp, #16]
	mov	fp, r2
.L_0815a480:
	ldr	r3, [sp, #40]
	ldr	r5, [pc, #128]
	adds	r3, #4
	ldrb	r3, [r5, r3]
	ldr	r1, [sp, #52]
	mov	r0, sl
	muls	r0, r3
	mov	r8, r0
	cmp	r1, r8
	bge.n	.L_0815a496
	b.n	sub_0815a5e6
.L_0815a496:
	mov	r3, r8
	adds	r3, #6
	cmp	r1, r3
	blt.n	.L_0815a4a0
	b.n	sub_0815a5e6
.L_0815a4a0:
	movs	r3, #3
	mov	r5, sl
	ands	r3, r5
	subs	r2, r1, r0
	cmp	r3, #1
	ble.n	.L_0815a4b6
	ldr	r0, [pc, #84]
	ldr	r1, [sp, #40]
	ldrb	r3, [r0, r1]
	cmp	r3, #1
	bne.n	.L_0815a570
.L_0815a4b6:
	ldr	r5, [sp, #68]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0815a538
	lsls	r1, r2, #3
	subs	r1, r1, r2
	lsls	r1, r1, #2
	subs	r1, r1, r2
	ldr	r2, [sp, #60]
	mov	r5, fp
	lsls	r1, r1, #7
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	movs	r3, #156
	movs	r0, #1
	lsls	r3, r3, #5
	mov	r4, sl
	ands	r4, r0
	adds	r1, r1, r3
	movs	r0, #48
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #16
	subs	r3, #40
	b.n	.L_0815a5a6
	movs	r0, r0
	.4byte 0x0000015c
	.4byte 0x02012400
	.4byte 0x00000192
	.4byte 0x081985b3
	.4byte 0x00000179
	.4byte 0x0000017a
	.4byte 0x0000012c
	.4byte 0x00000161
	.4byte 0x02013788
	.4byte 0x00000150
	.4byte 0x00000163
	.4byte 0x00000178
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x08152475
	.2byte 0x0000
	.2byte 0x0201
.L_0815a538:
	lsls	r1, r2, #3
	subs	r1, r1, r2
	lsls	r1, r1, #2
	subs	r1, r1, r2
	ldr	r2, [sp, #60]
	mov	r5, fp
	lsls	r1, r1, #7
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	movs	r3, #156
	movs	r0, #1
	lsls	r3, r3, #5
	mov	r4, sl
	ands	r4, r0
	adds	r1, r1, r3
	movs	r0, #48
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #32
	subs	r3, #40
	b.n	.L_0815a5a6
.L_0815a570:
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0815a5b0
	movs	r1, #1
	mov	r4, sl
	ands	r4, r1
	lsls	r1, r2, #1
	adds	r1, r1, r2
	ldr	r3, [sp, #36]
	ldr	r2, [pc, #196]
	lsls	r1, r1, #8
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r5, [sp, #24]
	lsrs	r3, r2, #31
	movs	r0, #48
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #16
	subs	r3, #8
.L_0815a5a6:
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01a
.L_0815a5b0:
	lsls	r1, r2, #1
	adds	r1, r1, r2
	ldr	r3, [sp, #36]
	ldr	r2, [pc, #148]
	lsls	r1, r1, #8
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r5, [sp, #24]
	movs	r0, #1
	mov	r4, sl
	ands	r4, r0
	lsrs	r3, r2, #31
	movs	r0, #48
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #32
	subs	r3, #8
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldr	r1, [sp, #52]
	add	r0, r8
	mov	r9, r0
	cmp	r1, r9
	bne.n	.L_0815a6b6
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #88]
	adds	r3, #6
	ldrb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	sub_0815a60c
	movs	r1, #128
	ldr	r3, [pc, #80]
	ldr	r0, [sp, #56]
	lsls	r1, r1, #7
	ldr	r2, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d11
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	movs	r2, #5
	movs	r3, #0
	movs	r5, #4
	str	r5, [sp, #0]
	bl	sub_0814cd48
	ldr	r3, [sp, #32]
	subs	r3, #1
	cmp	sl, r3
	bne.n	.L_0815a65c
	ldr	r2, [sp, #68]
	movs	r5, #238
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r3, [sp, #60]
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0815a684
	movs	r0, r0
	.4byte 0x02012400
	.4byte 0x081985b3
	.4byte 0x03000260
	.2byte 0x2f2f
	.2byte 0x2f2f
.L_0815a65c:
	mov	r3, sl
	movs	r0, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0815a672
	ldr	r2, [sp, #68]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #7
	bl	sub_08118088
.L_0815a672:
	ldr	r0, [sp, #60]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	str	r5, [r3, #0]
	movs	r0, #134
	bl	sub_081c0010
.L_0815a684:
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #800]
	adds	r3, #3
	ldrb	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_0815a6b6
	ldr	r3, [sp, #16]
	ldr	r0, [pc, #788]
	movs	r7, #7
	adds	r5, r3, r0
.L_0815a69a:
	str	r2, [sp, #12]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #15
	str	r0, [r5, #0]
	ldr	r3, [sp, #40]
	ldr	r2, [sp, #12]
	adds	r3, #3
	ldrb	r3, [r2, r3]
	adds	r6, #1
	adds	r5, #28
	cmp	r6, r3
	bne.n	.L_0815a69a
.L_0815a6b6:
	ldr	r1, [sp, #52]
	cmp	r1, r9
	blt.n	sub_0815a6fa
	mov	r3, r8
	adds	r3, #14
	cmp	r1, r3
	bge.n	sub_0815a6fa
	mov	r2, r8
	subs	r3, r1, r2
	subs	r3, #2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r1, r3, #4
	mov	r5, fp
	ldr	r2, [r5, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #724]
	lsls	r1, r1, #6
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	movs	r0, #20
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	subs	r2, #10
	subs	r3, #24
	ldr	r4, [sp, #72]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9904
	movs	r2, #224
	ldr	r5, [sp, #32]
	lsls	r2, r2, #2
	movs	r3, #1
	adds	r1, r1, r2
	add	sl, r3
	str	r1, [sp, #16]
	cmp	sl, r5
	beq.n	.L_0815a710
	b.n	.L_0815a480
.L_0815a710:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08014de4
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #20]
	bl	sub_080156e8
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r3, [r0, #104]
	movs	r1, #192
	str	r3, [sp, #72]
	lsls	r1, r1, #18
	adds	r1, #188
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #28]
	ldr	r6, [pc, #616]
	str	r3, [r2, #4]
	movs	r3, #0
	mov	sl, r3
	add	r7, sp, #92
.L_0815a754:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	ble.n	.L_0815a7c8
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r2, [r7, #0]
	mov	r1, sl
	lsrs	r3, r2, #31
	lsrs	r0, r1, #31
	asrs	r5, r5, #3
	adds	r5, #1
	adds	r2, r2, r3
	ldr	r1, [pc, #580]
	add	r0, sl
	lsls	r4, r5, #1
	asrs	r2, r2, #1
	movs	r3, #1
	asrs	r0, r0, #1
	str	r2, [r7, #0]
	ands	r0, r3
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #48]
	lsls	r0, r0, #2
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r5, [sp, #0]
	subs	r3, r3, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #28]
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	ldr	r2, [pc, #528]
	bl	sub_08138058
	ldr	r3, [r6, #4]
	ldr	r0, [pc, #524]
	cmp	r3, r0
	bgt.n	.L_0815a7c2
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
.L_0815a7c2:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_0815a7c8:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	adds	r6, #28
	cmp	sl, r2
	bne.n	.L_0815a754
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r3, [sp, #52]
	cmp	r3, #3
	bgt.n	.L_0815a7ea
	b.n	.L_0815a94a
.L_0815a7ea:
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #440]
	adds	r3, #7
	ldrb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_0815a8d2
	ldr	r5, [sp, #52]
	cmp	r5, #31
	bgt.n	.L_0815a8d2
	ldr	r2, [sp, #68]
	add	r5, sp, #80
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r7, [sp, #52]
	movs	r0, #3
	movs	r3, #0
	mov	sl, r3
	mov	r9, r0
	ands	r7, r0
	mov	fp, r5
.L_0815a818:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r0, #0
	ands	r2, r3
	str	r2, [sp, #12]
	bl	sub_08014878
	ldr	r2, [sp, #12]
	movs	r5, #31
	ands	r5, r0
	adds	r0, r2, #0
	bl	sub_08002096
	mov	r1, fp
	ldr	r6, [r1, #0]
	adds	r5, #4
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r6, r6, #1
	asrs	r3, r3, #17
	adds	r6, r6, r3
	ldr	r3, [pc, #372]
	ldr	r2, [sp, #12]
	mov	r8, r3
	ldrb	r3, [r3, r7]
	adds	r0, r2, #0
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, fp
	ldr	r4, [pc, #352]
	ldr	r5, [r0, #4]
	asrs	r3, r3, #17
	subs	r5, r5, r3
	ldrb	r3, [r4, r7]
	str	r4, [sp, #8]
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08014878
	ldr	r3, [pc, #336]
	mov	r1, r9
	ands	r0, r1
	ldrb	r2, [r3, r0]
	mov	r3, r9
	orrs	r3, r2
	movs	r2, #0
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r2, [pc, #316]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #60]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	mov	r0, r8
	adds	r1, r1, r3
	ldrb	r3, [r0, r7]
	ldr	r4, [sp, #8]
	str	r3, [sp, #0]
	movs	r2, #192
	ldrb	r3, [r4, r7]
	lsls	r2, r2, #18
	str	r3, [sp, #4]
	adds	r2, #188
	adds	r3, r5, #0
	ldr	r4, [r2, #0]
	ldr	r0, [sp, #56]
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	cmp	r5, #3
	bne.n	.L_0815a818
	ldr	r2, [pc, #212]
.L_0815a8d2:
	ldr	r3, [sp, #40]
	adds	r3, #7
	ldrb	r3, [r2, r3]
	cmp	r3, #2
	bne.n	.L_0815a94a
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r3, [r0, #104]
	ldr	r5, [sp, #60]
	str	r3, [sp, #72]
	movs	r1, #0
	mov	sl, r1
.L_0815a8f2:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0815a934
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_0815a900
	adds	r2, r3, #3
.L_0815a900:
	asrs	r3, r2, #2
	lsls	r1, r3, #3
	ldr	r2, [pc, #204]
	adds	r1, r1, r3
	lsls	r1, r1, #7
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	subs	r3, #24
	str	r0, [sp, #4]
	subs	r2, #12
	ldr	r4, [sp, #72]
	ldr	r0, [sp, #56]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #168]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0815a934:
	movs	r1, #1
	add	sl, r1
	adds	r3, #1
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_0815a8f2
	movs	r0, #104
	bl	sub_0801314c
.L_0815a94a:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #60]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #52]
	ldr	r3, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #52]
	ldr	r2, [pc, #52]
	adds	r3, #5
	ldrb	r3, [r2, r3]
	cmp	r0, r3
	beq.n	.L_0815a97e
	b.n	.L_0815a3c8
.L_0815a97e:
	ldr	r3, [sp, #40]
	adds	r3, #7
	ldrb	r3, [r2, r3]
	cmp	r3, #2
	bne.n	.L_0815a98e
	ldr	r0, [pc, #80]
	bl	sub_08014644
.L_0815a98e:
	ldr	r0, [pc, #80]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #152
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x081985b3
	.4byte 0x02010018
	.4byte 0x02015288
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0xfffffc00
	.4byte 0x0007ffff
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x0819861b
	.4byte 0x08197486
	.4byte 0x02013788
	.4byte 0xfffff000
	.4byte 0x08152475
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #80
	str	r0, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #48]
	movs	r0, #0
	ldr	r1, [r3, #96]
	str	r1, [sp, #44]
	ldr	r3, [r3, #100]
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r2, [sp, #52]
	ldr	r0, [r2, #24]
	cmp	r0, #0
	bne.n	.L_0815aa20
	movs	r2, #128
	ldr	r3, [pc, #4]
	b.n	.L_0815aa28
	movs	r0, r0
	.2byte 0x00cc
	.2byte 0x0000
.L_0815aa20:
	cmp	r0, #1
	bne.n	.L_0815aa34
	movs	r2, #128
	ldr	r3, [pc, #8]
.L_0815aa28:
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	b.n	.L_0815aa34
	.2byte 0x00aa
	.2byte 0x0000
.L_0815aa34:
	ldr	r3, [sp, #52]
	movs	r7, #16
	ldr	r4, [r3, #4]
	movs	r1, #12
	negs	r7, r7
	negs	r1, r1
	str	r7, [sp, #24]
	str	r1, [sp, #20]
	cmp	r4, #1
	bne.n	.L_0815aa60
	movs	r2, #8
	movs	r3, #40
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	cmp	r0, #0
	beq.n	.L_0815aa60
	movs	r7, #36
	str	r7, [sp, #20]
	cmp	r0, #1
	beq.n	.L_0815aa60
	movs	r1, #40
	str	r1, [sp, #20]
.L_0815aa60:
	lsls	r3, r4, #1
	ldr	r2, [pc, #100]
	adds	r3, r3, r4
	adds	r3, r0, r3
	ldrsb	r3, [r2, r3]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r2, #1
	ldr	r1, [pc, #84]
	movs	r3, #0
	ldr	r0, [pc, #84]
	bl	sub_08157cf4
	ldr	r0, [pc, #80]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #76]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x25a0
	ldr	r7, [pc, #44]
	lsls	r5, r5, #19
	adds	r5, #2
	movs	r6, #0
.L_0815aa9e:
	ldrh	r2, [r5, #0]
	movs	r4, #31
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r7
	lsrs	r1, r3, #21
	ands	r1, r7
	ands	r4, r2
	subs	r0, #8
	subs	r1, #8
	subs	r4, #8
	cmp	r0, #0
	bge.n	.L_0815aaba
	movs	r0, #0
.L_0815aaba:
	cmp	r1, #0
	bge.n	.L_0815aadc
	movs	r1, #0
	b.n	.L_0815aadc
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x08198648
	.4byte 0x02010000
	.4byte 0x0000013c
	.4byte 0x0000013d
	.2byte 0x0730
	.2byte 0x0300
.L_0815aadc:
	cmp	r4, #0
	bge.n	.L_0815aae2
	movs	r4, #0
.L_0815aae2:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #63
	bne.n	.L_0815aa9e
	ldr	r2, [pc, #404]
	movs	r3, #224
	ldr	r5, [pc, #404]
	mov	lr, r2
	lsls	r3, r3, #3
	movs	r4, #0
	movs	r7, #0
	mov	r9, lr
	mov	sl, r3
.L_0815ab06:
	mov	r0, lr
	ldrb	r0, [r0, #0]
	movs	r6, #0
	mov	ip, r0
.L_0815ab0e:
	movs	r1, #0
	mov	r3, ip
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_0815ab38
	ldr	r1, [sp, #48]
	ldrb	r0, [r2, r7]
	adds	r3, r4, r1
	mov	r2, sl
	adds	r1, r3, r2
	adds	r2, r5, #0
.L_0815ab24:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r1, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	adds	r1, #1
	cmp	r8, r0
	bne.n	.L_0815ab24
	mov	r2, r9
.L_0815ab38:
	adds	r6, #1
	cmp	r6, #32
	bne.n	.L_0815ab0e
	mov	r0, lr
	ldrb	r3, [r0, #0]
	movs	r1, #1
	adds	r7, #1
	adds	r5, r5, r3
	add	lr, r1
	cmp	r7, #9
	bne.n	.L_0815ab06
	movs	r2, #224
	lsls	r2, r2, #3
	movs	r7, #0
	mov	ip, r2
.L_0815ab56:
	adds	r0, r5, #0
	movs	r6, #0
	adds	r0, #48
.L_0815ab5c:
	ldr	r1, [sp, #48]
	mov	r2, ip
	adds	r3, r4, r1
	adds	r1, r3, r2
	adds	r2, r5, #0
.L_0815ab66:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r4, #1
	adds	r1, #1
	cmp	r2, r0
	bne.n	.L_0815ab66
	adds	r6, #1
	cmp	r6, #3
	bne.n	.L_0815ab5c
	adds	r7, #1
	adds	r5, #48
	cmp	r7, #32
	bne.n	.L_0815ab56
	ldr	r7, [sp, #48]
	movs	r0, #224
	movs	r1, #252
	adds	r3, r4, r7
	lsls	r0, r0, #3
	movs	r6, #0
	lsls	r1, r1, #2
	adds	r2, r3, r0
.L_0815ab92:
	ldrb	r3, [r5, #0]
	adds	r6, #1
	strb	r3, [r2, #0]
	adds	r5, #1
	adds	r2, #1
	adds	r4, #1
	cmp	r6, r1
	bne.n	.L_0815ab92
	ldr	r1, [pc, #240]
	ldr	r0, [pc, #240]
	movs	r6, #224
	adds	r7, r1, #3
	lsls	r6, r6, #3
.L_0815abac:
	movs	r2, #0
	mov	r8, r2
	ldrb	r3, [r0, #0]
	ldrb	r2, [r1, #0]
	muls	r3, r2
	cmp	r3, #0
	beq.n	.L_0815abd4
	ldr	r2, [sp, #48]
	mov	ip, r3
	adds	r3, r4, r2
	adds	r2, r3, r6
.L_0815abc2:
	ldrb	r3, [r5, #0]
	adds	r4, #1
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	adds	r5, #1
	cmp	r8, ip
	bne.n	.L_0815abc2
.L_0815abd4:
	adds	r1, #1
	adds	r0, #1
	cmp	r1, r7
	bne.n	.L_0815abac
	ldr	r0, [pc, #188]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r5, #238
	movs	r4, #13
	lsls	r5, r5, #7
	negs	r4, r4
	movs	r6, #0
	adds	r5, #220
	adds	r7, r4, #0
.L_0815abf6:
	movs	r0, #199
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_08020040
	ldr	r1, [sp, #48]
	str	r0, [r5, r1]
	cmp	r0, #0
	beq.n	.L_0815ac28
	movs	r3, #0
	strb	r3, [r0, #26]
	adds	r1, r6, #0
	cmp	r6, #0
	bge.n	.L_0815ac14
	adds	r1, r6, #3
.L_0815ac14:
	asrs	r1, r1, #2
	bl	sub_08020030
	ldr	r2, [sp, #48]
	ldr	r1, [r5, r2]
	movs	r2, #4
	ldrb	r3, [r1, #9]
	ands	r3, r7
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_0815ac28:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #11
	bne.n	.L_0815abf6
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #23
	movs	r0, #188
	str	r3, [sp, #32]
	bl	sub_081963ec
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #48]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	adds	r5, #188
	lsls	r0, r0, #7
	adds	r2, r4, r7
	ldr	r5, [r5, #0]
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r4, r0
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #36]
	str	r5, [sp, #36]
	bl	sub_080145a8
	ldr	r1, [sp, #48]
	b.n	.L_0815aca4
	.4byte 0x00003f46
	.4byte 0x00001010
	.4byte 0x08198632
	.4byte 0x02010000
	.4byte 0x08198642
	.4byte 0x08198645
	.4byte 0x00000134
	.2byte 0x3001
	.2byte 0x0814
.L_0815aca4:
	movs	r2, #140
	lsls	r2, r2, #1
	movs	r6, #0
	movs	r7, #15
	adds	r5, r1, r2
.L_0815acae:
	bl	sub_08014878
	ands	r0, r7
	movs	r3, #128
	adds	r0, #88
	str	r3, [r5, #4]
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #1
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #20]
	lsls	r2, r6, #2
	ands	r0, r7
	movs	r3, #44
	str	r6, [r5, #12]
	adds	r0, #2
	subs	r3, r3, r2
	adds	r6, #1
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #11
	bne.n	.L_0815acae
	ldr	r3, [sp, #48]
	movs	r4, #224
	ldr	r1, [pc, #144]
	lsls	r4, r4, #1
	movs	r6, #0
	adds	r2, r3, r4
.L_0815acee:
	ldrb	r3, [r1, #0]
	adds	r6, #1
	str	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #28
	cmp	r6, #6
	bne.n	.L_0815acee
	ldr	r5, [pc, #124]
	movs	r6, #0
	movs	r7, #63
.L_0815ad02:
	bl	sub_08014878
	ldr	r1, [sp, #24]
	ands	r0, r7
	adds	r0, r0, r1
	adds	r0, #32
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #31
	movs	r2, #128
	ands	r3, r0
	adds	r6, #1
	lsls	r2, r2, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, r2
	bne.n	.L_0815ad02
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [sp, #48]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #250
	str	r3, [r2, #0]
	ldr	r0, [sp, #24]
	movs	r7, #0
	adds	r0, #64
	str	r7, [sp, #40]
	str	r0, [sp, #16]
.L_0815ad64:
	ldr	r1, [sp, #40]
	cmp	r1, #0
	bne.n	.L_0815ad80
	movs	r0, #212
	bl	sub_081c0010
	b.n	.L_0815ad80
	movs	r0, r0
	.4byte 0x00000785
	.4byte 0x0819864e
	.2byte 0x0000
	.2byte 0x0201
.L_0815ad80:
	ldr	r2, [sp, #40]
	cmp	r2, #40
	bne.n	.L_0815ad8c
	movs	r0, #141
	bl	sub_081c0010
.L_0815ad8c:
	ldr	r3, [sp, #40]
	cmp	r3, #96
	bne.n	.L_0815ad98
	movs	r0, #145
	bl	sub_081c0010
.L_0815ad98:
	ldr	r4, [sp, #40]
	cmp	r4, #120
	bne.n	.L_0815ada4
	movs	r0, #134
	bl	sub_081180e8
.L_0815ada4:
	ldr	r7, [sp, #40]
	cmp	r7, #81
	bgt.n	sub_0815ae18
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_0815adb2
	adds	r3, #3
.L_0815adb2:
	asrs	r7, r3, #2
	cmp	r7, #2
	ble.n	.L_0815adbe
	movs	r3, #1
	ands	r3, r7
	adds	r7, r3, #1
.L_0815adbe:
	ldr	r0, [pc, #876]
	ldr	r4, [pc, #876]
	lsls	r1, r7, #1
	ldr	r2, [sp, #48]
	ldr	r6, [pc, #876]
	mov	fp, r0
	mov	sl, r1
	mov	r9, r4
	ldrh	r1, [r0, r1]
	ldrb	r4, [r4, r7]
	ldr	r0, [sp, #24]
	adds	r1, r2, r1
	movs	r3, #224
	subs	r2, r0, r4
	ldrb	r0, [r6, r7]
	lsls	r3, r3, #3
	mov	r8, r3
	movs	r5, #116
	subs	r3, r5, r0
	add	r1, r8
	adds	r2, #64
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4658
	mov	r2, sl
	ldrh	r1, [r0, r2]
	ldr	r3, [sp, #48]
	ldrb	r2, [r6, r7]
	mov	r4, r9
	adds	r1, r3, r1
	ldrb	r3, [r4, r7]
	subs	r5, r5, r2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	add	r1, r8
	ldr	r0, [sp, #44]
	ldr	r2, [sp, #16]
	adds	r3, r5, #0
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9b0a
	subs	r3, #12
	cmp	r3, #75
	bhi.n	.L_0815aeaa
	ldr	r0, [sp, #40]
	movs	r1, #3
	subs	r0, #64
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #0
	bge.n	.L_0815ae32
	movs	r5, #0
.L_0815ae32:
	cmp	r5, #7
	ble.n	.L_0815ae38
	movs	r5, #7
.L_0815ae38:
	ldr	r0, [sp, #24]
	lsls	r1, r5, #1
	ldr	r3, [pc, #760]
	movs	r2, #224
	str	r1, [sp, #8]
	movs	r4, #32
	adds	r0, #64
	lsls	r2, r2, #3
	mov	sl, r4
	movs	r6, #0
	mov	r8, r0
	mov	fp, r2
	mov	r9, r3
	subs	r4, #44
.L_0815ae54:
	ldr	r0, [sp, #8]
	ldr	r7, [pc, #740]
	ldr	r2, [sp, #48]
	ldrh	r1, [r7, r0]
	mov	r7, r9
	ldrb	r3, [r7, r5]
	ldr	r0, [sp, #24]
	str	r3, [sp, #0]
	adds	r1, r2, r1
	subs	r2, r0, r3
	mov	r3, sl
	str	r3, [sp, #4]
	str	r4, [sp, #12]
	adds	r3, r4, #0
	ldr	r7, [sp, #32]
	add	r1, fp
	adds	r2, #64
	ldr	r0, [sp, #44]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x48af
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #48]
	ldrh	r1, [r0, r2]
	mov	r7, r9
	ldr	r4, [sp, #12]
	adds	r1, r3, r1
	ldrb	r3, [r7, r5]
	mov	r0, sl
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, r4, #0
	add	r1, fp
	ldr	r0, [sp, #44]
	mov	r2, r8
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9c03
	adds	r6, #1
	adds	r4, #32
	cmp	r6, #4
	bne.n	.L_0815ae54
.L_0815aeaa:
	ldr	r0, [sp, #40]
	subs	r0, #160
	cmp	r0, #23
	bhi.n	.L_0815af3a
	movs	r1, #3
	bl	sub_08002054
	movs	r3, #7
	subs	r5, r3, r0
	cmp	r5, #0
	bge.n	.L_0815aec2
	movs	r5, #0
.L_0815aec2:
	cmp	r5, #7
	ble.n	.L_0815aec8
	movs	r5, #7
.L_0815aec8:
	ldr	r0, [sp, #24]
	lsls	r1, r5, #1
	ldr	r3, [pc, #616]
	movs	r2, #224
	str	r1, [sp, #8]
	movs	r4, #32
	adds	r0, #64
	lsls	r2, r2, #3
	mov	sl, r4
	movs	r6, #0
	mov	r8, r0
	mov	fp, r2
	mov	r9, r3
	subs	r4, #44
.L_0815aee4:
	ldr	r0, [sp, #8]
	ldr	r7, [pc, #596]
	ldr	r2, [sp, #48]
	ldrh	r1, [r7, r0]
	mov	r7, r9
	ldrb	r3, [r7, r5]
	ldr	r0, [sp, #24]
	str	r3, [sp, #0]
	adds	r1, r2, r1
	subs	r2, r0, r3
	mov	r3, sl
	str	r3, [sp, #4]
	str	r4, [sp, #12]
	adds	r3, r4, #0
	ldr	r7, [sp, #32]
	add	r1, fp
	adds	r2, #64
	ldr	r0, [sp, #44]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x488b
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #48]
	ldrh	r1, [r0, r2]
	mov	r7, r9
	ldr	r4, [sp, #12]
	adds	r1, r3, r1
	ldrb	r3, [r7, r5]
	mov	r0, sl
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, r4, #0
	add	r1, fp
	ldr	r0, [sp, #44]
	mov	r2, r8
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9c03
	adds	r6, #1
	adds	r4, #32
	cmp	r6, #4
	bne.n	.L_0815aee4
.L_0815af3a:
	ldr	r3, [sp, #40]
	subs	r3, #88
	cmp	r3, #71
	bhi.n	sub_0815afac
	ldr	r0, [sp, #48]
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #24]
	movs	r1, #214
	lsls	r1, r1, #5
	adds	r4, r0, r1
	movs	r6, #48
	movs	r5, #96
	str	r5, [sp, #4]
	str	r4, [sp, #12]
	adds	r2, #16
	str	r6, [sp, #0]
	mov	sl, r3
	ldr	r0, [sp, #44]
	adds	r1, r4, #0
	movs	r3, #0
	mov	r9, r2
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9c03
	ldr	r7, [sp, #36]
	str	r5, [sp, #4]
	adds	r1, r4, #0
	ldr	r2, [sp, #16]
	str	r6, [sp, #0]
	ldr	r0, [sp, #44]
	movs	r3, #0
	mov	r8, r7
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x980c
	movs	r1, #179
	lsls	r1, r1, #6
	adds	r4, r0, r1
	movs	r5, #21
	adds	r1, r4, #0
	mov	r2, r9
	movs	r3, #96
	str	r4, [sp, #12]
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #44]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9c03
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #44]
	adds	r1, r4, #0
	ldr	r2, [sp, #16]
	movs	r3, #96
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9a0a
	cmp	r2, #87
	ble.n	.L_0815b012
	ldr	r3, [pc, #396]
	ldr	r5, [pc, #396]
	movs	r6, #0
	mov	r8, r3
.L_0815afba:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_0815b006
	movs	r0, #3
	ands	r0, r6
	adds	r0, #5
	lsls	r4, r0, #1
	subs	r3, r4, #2
	mov	r7, r8
	ldrh	r1, [r7, r3]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x686a
	ldr	r3, [r5, #16]
	subs	r2, r2, r3
	str	r2, [r5, #4]
	cmp	r2, #0
	bge.n	.L_0815b00a
	ldr	r7, [sp, #40]
	cmp	r7, #159
	bgt.n	.L_0815b00a
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	b.n	.L_0815b00a
.L_0815b006:
	subs	r3, #1
	str	r3, [r5, #24]
.L_0815b00a:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L_0815afba
.L_0815b012:
	ldr	r0, [sp, #40]
	cmp	r0, #4
	bgt.n	.L_0815b01a
	b.n	.L_0815b172
.L_0815b01a:
	movs	r1, #6
	mov	sl, r1
	cmp	r0, #71
	ble.n	.L_0815b026
	movs	r2, #11
	mov	sl, r2
.L_0815b026:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	cmp	r4, #0
	bne.n	.L_0815b032
	b.n	.L_0815b172
.L_0815b032:
	ldr	r7, [sp, #48]
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r5, r7, r0
.L_0815b03a:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_0815b042
	b.n	.L_0815b162
.L_0815b042:
	ldr	r3, [pc, #260]
	ldr	r1, [sp, #40]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #56]
	str	r4, [sp, #60]
	cmp	r1, #71
	ble.n	.L_0815b066
	ldr	r7, [sp, #52]
	mov	r3, r8
	lsls	r2, r3, #12
	ldr	r3, [r7, #24]
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r2, r2, r4
	lsls	r3, r3, #14
	adds	r3, r3, r2
	b.n	.L_0815b06a
.L_0815b066:
	movs	r3, #128
	lsls	r3, r3, #8
.L_0815b06a:
	str	r3, [sp, #56]
	ldr	r3, [sp, #56]
	add	r7, sp, #56
	str	r3, [r7, #4]
	add	r6, sp, #64
	movs	r3, #0
	str	r3, [r6, #12]
	ldr	r0, [sp, #20]
	ldr	r3, [r5, #0]
	lsls	r2, r0, #1
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r2, #128
	ldr	r3, [r5, #4]
	lsls	r2, r2, #18
	lsls	r3, r3, #16
	subs	r3, r2, r3
	str	r3, [r6, #4]
	str	r2, [r6, #8]
	ldr	r1, [sp, #40]
	lsrs	r0, r1, #31
	adds	r0, r1, r0
	asrs	r0, r0, #1
	add	r0, r8
	movs	r1, #11
	bl	sub_08002064
	movs	r4, #1
	negs	r4, r4
	cmp	r0, r4
	beq.n	.L_0815b0c6
	movs	r2, #238
	ldr	r1, [sp, #48]
	lsls	r2, r2, #7
	lsls	r3, r0, #2
	adds	r2, #220
	adds	r3, r3, r2
	ldr	r0, [r1, r3]
	adds	r2, r7, #0
	adds	r1, r6, #0
	movs	r3, #0
	str	r4, [sp, #12]
	bl	sub_08020010
	ldr	r4, [sp, #12]
.L_0815b0c6:
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #8]
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r2, r2, r3
	str	r2, [r5, #12]
	cmp	r2, #12
	ble.n	.L_0815b0e0
	adds	r3, r2, #0
	subs	r3, #12
	str	r3, [r5, #12]
.L_0815b0e0:
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bge.n	.L_0815b166
	ldr	r2, [sp, #40]
	cmp	r2, #159
	ble.n	.L_0815b0f0
	str	r4, [r5, #24]
	b.n	.L_0815b166
.L_0815b0f0:
	ldr	r3, [sp, #40]
	cmp	r3, #87
	ble.n	.L_0815b15a
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #8
	str	r3, [r5, #8]
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #24]
	cmp	r3, #0
	bne.n	.L_0815b118
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	adds	r0, #42
	b.n	.L_0815b158
.L_0815b118:
	cmp	r3, #1
	bne.n	.L_0815b14c
	bl	sub_08014878
	movs	r1, #112
	bl	sub_0800206c
	adds	r0, #34
	b.n	.L_0815b158
	movs	r0, r0
	.4byte 0x0819863c
	.4byte 0x08198642
	.4byte 0x08198645
	.4byte 0x08198632
	.4byte 0x08198620
	.4byte 0x08197410
	.4byte 0x02010000
	.2byte 0x6e44
	.2byte 0x0819
.L_0815b14c:
	bl	sub_08014878
	movs	r1, #160
	bl	sub_0800206c
	adds	r0, #10
.L_0815b158:
	str	r0, [r5, #0]
.L_0815b15a:
	movs	r3, #128
	str	r3, [r5, #4]
	movs	r3, #8
	b.n	.L_0815b164
.L_0815b162:
	subs	r3, #1
.L_0815b164:
	str	r3, [r5, #24]
.L_0815b166:
	movs	r7, #1
	add	r8, r7
	adds	r5, #28
	cmp	r8, sl
	beq.n	.L_0815b172
	b.n	.L_0815b03a
.L_0815b172:
	ldr	r0, [sp, #40]
	cmp	r0, #158
	bgt.n	.L_0815b1cc
	ldr	r1, [sp, #52]
	movs	r6, #0
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_0815b1cc
	movs	r7, #3
	ands	r7, r0
	movs	r5, #36
.L_0815b188:
	ldr	r2, [sp, #40]
	cmp	r2, #85
	ble.n	.L_0815b1c4
	adds	r0, r2, #0
	movs	r1, #12
	bl	sub_08002064
	cmp	r0, #0
	bne.n	.L_0815b1ac
	ldr	r3, [sp, #52]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r6, #0
	bl	sub_0814cd48
.L_0815b1ac:
	cmp	r7, #0
	bne.n	.L_0815b1c0
	ldr	r1, [sp, #52]
	ldrsh	r0, [r5, r1]
	movs	r1, #5
	bl	sub_08118088
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #20]
	b.n	.L_0815b1c4
.L_0815b1c0:
	ldr	r0, [sp, #52]
	ldr	r3, [r0, #20]
.L_0815b1c4:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	bne.n	.L_0815b188
.L_0815b1cc:
	ldr	r3, [sp, #40]
	subs	r3, #90
	cmp	r3, #70
	bls.n	.L_0815b1de
	movs	r0, #2
	movs	r1, #2
	bl	sub_08158ce0
	b.n	.L_0815b1e6
.L_0815b1de:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
.L_0815b1e6:
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #48]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #40]
	adds	r4, #1
	str	r4, [sp, #40]
	cmp	r4, #192
	beq.n	.L_0815b20a
	b.n	.L_0815ad64
.L_0815b20a:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #238
	ldr	r7, [sp, #48]
	lsls	r0, r0, #7
	adds	r0, #220
	movs	r6, #0
	adds	r5, r7, r0
.L_0815b228:
	ldmia	r5!, {r0}
	adds	r6, #1
	bl	sub_08020048
	cmp	r6, #11
	bne.n	.L_0815b228
	bl	sub_08143bb8
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #12]
	movs	r0, #0
	ldrb	r3, [r2, #4]
	movs	r1, #0
	b.n	.L_0815b266
.L_0815b25c:
	adds	r1, #1
	adds	r2, #24
	cmp	r1, #63
	bgt.n	.L_0815b26c
	ldrb	r3, [r2, #4]
.L_0815b266:
	cmp	r3, #0
	bne.n	.L_0815b25c
	adds	r0, r2, #0
.L_0815b26c:
	ldr	r2, [pc, #24]
	movs	r3, #0
	strh	r3, [r0, #0]
	str	r3, [r0, #12]
	str	r3, [r0, #8]
	str	r3, [r0, #16]
	movs	r3, #1
	strb	r2, [r0, #7]
	strb	r2, [r0, #22]
	strb	r2, [r0, #20]
	strb	r3, [r0, #4]
	strb	r2, [r0, #5]
	b.n	.L_0815b28c
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0815b28c:
	pop	{pc}
