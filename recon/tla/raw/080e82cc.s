.syntax unified
	.thumb
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014d00, 0x08014d00
	.set sub_0801587c, 0x0801587c
	.set sub_08020138, 0x08020138
	.set sub_08020228, 0x08020228
	.set sub_08020328, 0x08020328
	.set sub_080c8978, 0x080c8978
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d4384, 0x080d4384
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080e7f94, 0x080e7f94
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb298, 0x080eb298
	.set sub_080eb824, 0x080eb824
	.set sub_080eb930, 0x080eb930
	.set sub_081c0010, 0x081c0010
	.global Func_080e82cc
	.thumb_func
Func_080e82cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #164
	lsls	r1, r1, #6
	adds	r1, #140
	movs	r0, #92
	sub	sp, #64
	bl	sub_08014d00
	movs	r2, #192
	str	r0, [sp, #48]
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #0
	str	r3, [sp, #44]
	ldr	r2, [r2, #108]
	str	r2, [sp, #40]
	ldr	r4, [sp, #40]
	ldr	r0, [r3, #16]
	movs	r2, #1
	str	r0, [sp, #36]
	movs	r0, #192
	str	r1, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r1, [sp, #8]
	lsls	r0, r0, #4
	adds	r0, #172
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e8320
	b.n	.L_080e8768
.L_080e8320:
	bl	sub_080dc294
	add	r1, sp, #16
	ldr	r3, [pc, #136]
	ldrb	r1, [r1, #0]
	strb	r1, [r3, #0]
	bl	sub_080eb824
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #240
	ldr	r3, [r3, #0]
	add	r2, sp, #16
	str	r3, [sp, #32]
	adds	r3, #192
	ldrb	r2, [r2, #0]
	movs	r1, #128
	strb	r2, [r3, #0]
	mov	r3, sp
	adds	r3, #52
	str	r3, [sp, #4]
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #4]
	ldr	r2, [r4, #16]
	lsls	r1, r1, #13
	ldr	r3, [r2, #8]
	str	r3, [r0, #0]
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r2, #16]
	str	r3, [r0, #8]
	ldr	r0, [sp, #4]
	bl	sub_080dc390
	ldr	r4, [sp, #4]
	ldr	r1, [pc, #72]
	movs	r2, #2
	ldrsh	r3, [r4, r2]
	ldr	r2, [pc, #60]
	subs	r3, r2, r3
	strh	r3, [r1, #0]
	movs	r0, #10
	ldrsh	r3, [r4, r0]
	subs	r2, r2, r3
	strh	r2, [r1, #2]
	ldr	r6, [pc, #56]
	movs	r1, #128
	lsls	r1, r1, #4
	movs	r2, #0
	ldr	r0, [pc, #52]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2060
	movs	r5, #128
	lsls	r5, r5, #6
	bl	sub_0801314c
	adds	r1, r5, #0
	movs	r0, #96
	bl	sub_08014d00
	mov	r9, r0
	adds	r1, r5, #0
	movs	r2, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2001
	b.n	.L_080e83c0
	movs	r0, r0
	.4byte 0x00000040
	.4byte 0x0300123c
	.4byte 0x03001120
	.4byte 0x03000260
	.2byte 0x2000
	.2byte 0x0600
.L_080e83c0:
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	mov	r0, r9
	lsls	r1, r1, #19
	ldr	r2, [pc, #256]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #144
	lsls	r2, r2, #3
	subs	r3, #204
	strh	r2, [r3, #0]
	ldr	r0, [pc, #248]
	bl	sub_08013300
	ldr	r1, [sp, #48]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [sp, #48]
	str	r0, [sp, #28]
	bl	sub_080142d4
	ldr	r2, [sp, #48]
	movs	r3, #208
	movs	r4, #128
	str	r0, [sp, #24]
	movs	r1, #0
	lsls	r3, r3, #5
	lsls	r4, r4, #5
	mov	sl, r1
	adds	r6, r2, r3
	adds	r5, r2, r4
.L_080e840e:
	ldr	r0, [sp, #24]
	movs	r1, #4
	str	r0, [sp, #0]
	movs	r2, #4
	movs	r3, #0
	adds	r0, r5, #0
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	mov	r2, sl
	movs	r3, #240
	strh	r3, [r5, #30]
	negs	r3, r2
	cmp	r3, #0
	bge.n	.L_080e8444
	adds	r3, #3
.L_080e8444:
	asrs	r3, r3, #2
	str	r3, [r6, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #40
	adds	r6, #28
	cmp	r4, #63
	ble.n	.L_080e840e
	ldr	r0, [sp, #48]
	movs	r1, #164
	lsls	r1, r1, #6
	movs	r2, #164
	adds	r1, #132
	lsls	r2, r2, #6
	adds	r3, r0, r1
	adds	r2, #133
	movs	r1, #0
	strb	r1, [r3, #0]
	adds	r3, r0, r2
	strb	r1, [r3, #0]
	movs	r4, #164
	movs	r3, #164
	lsls	r3, r3, #6
	lsls	r4, r4, #6
	adds	r3, #134
	adds	r4, #136
	adds	r5, r0, r3
	movs	r2, #1
	adds	r3, r0, r4
	strb	r2, [r5, #0]
	strh	r1, [r3, #0]
	ldr	r0, [sp, #40]
	movs	r1, #192
	lsls	r1, r1, #4
	movs	r4, #192
	adds	r1, #172
	lsls	r4, r4, #4
	adds	r3, r0, r1
	adds	r4, #173
	strb	r2, [r3, #0]
	adds	r3, r0, r4
	strb	r2, [r3, #0]
	movs	r3, #186
	adds	r1, #2
	lsls	r3, r3, #2
	adds	r2, r0, r1
	adds	r3, #255
	ldr	r6, [pc, #40]
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl	sub_080145a8
	movs	r0, #1
	bl	sub_08013560
	strb	r6, [r5, #0]
	movs	r2, #0
	mov	fp, r2
	mov	r8, r2
.L_080e84c0:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080e852e
	ldr	r5, [sp, #20]
	movs	r4, #7
	mov	sl, r4
	b.n	.L_080e84e0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x84000800
	.4byte 0x000001eb
	.2byte 0x807d
	.2byte 0x080e
.L_080e84e0:
	movs	r1, #208
	adds	r2, r5, #0
	mov	r0, r9
	lsls	r1, r1, #14
	movs	r3, #243
	bl	sub_080e7f94
	movs	r1, #212
	adds	r2, r5, #0
	mov	r0, r9
	lsls	r1, r1, #14
	movs	r3, #247
	bl	sub_080e7f94
	movs	r1, #216
	adds	r2, r5, #0
	mov	r0, r9
	lsls	r1, r1, #14
	movs	r3, #251
	bl	sub_080e7f94
	movs	r1, #220
	mov	r0, r9
	lsls	r1, r1, #14
	adds	r2, r5, #0
	movs	r3, #255
	bl	sub_080e7f94
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r1, sl
	adds	r5, #128
	cmp	r1, #0
	bge.n	.L_080e84e0
	ldr	r2, [sp, #32]
	movs	r3, #11
	adds	r2, #193
	strb	r3, [r2, #0]
.L_080e852e:
	ldr	r3, [sp, #48]
	movs	r4, #208
	movs	r0, #128
	movs	r2, #0
	lsls	r4, r4, #5
	lsls	r0, r0, #5
	mov	sl, r2
	adds	r6, r3, r4
	adds	r7, r3, r0
.L_080e8540:
	ldr	r1, [r6, #24]
	cmp	r1, #0
	bne.n	.L_080e858a
	ldr	r4, [sp, #36]
	ldr	r3, [sp, #16]
	mov	r2, sl
	movs	r1, #1
	ands	r1, r2
	lsls	r1, r1, #15
	adds	r1, r3, r1
	ldr	r3, [r4, #8]
	movs	r0, #128
	str	r3, [r6, #0]
	lsls	r0, r0, #13
	ldr	r3, [r4, #12]
	adds	r2, r6, #0
	adds	r3, r3, r0
	str	r3, [r6, #4]
	movs	r0, #220
	ldr	r3, [r4, #16]
	lsls	r0, r0, #14
	str	r3, [r6, #8]
	bl	sub_0801489c
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	sub_08014878
	lsls	r5, r5, #1
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	ldr	r1, [r6, #24]
.L_080e858a:
	cmp	r1, #15
	bhi.n	.L_080e85c8
	ldr	r2, [sp, #24]
	movs	r3, #3
	ands	r1, r3
	ldr	r3, [pc, #36]
	lsls	r1, r1, #1
	adds	r1, r2, r1
	ands	r1, r3
	ldr	r2, [pc, #32]
	ldrh	r3, [r7, #8]
	adds	r0, r7, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	adds	r1, r6, #0
	bl	sub_080eb298
	ldr	r3, [r6, #4]
	ldr	r4, [pc, #16]
	ldr	r1, [r6, #24]
	adds	r3, r3, r4
	str	r3, [r6, #4]
	b.n	.L_080e85c8
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x0000
	.2byte 0xffff
.L_080e85c8:
	.2byte 0x3101
	str	r1, [r6, #24]
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq.n	.L_080e85da
	cmp	r1, #16
	bne.n	.L_080e85da
	movs	r3, #0
	str	r3, [r6, #24]
.L_080e85da:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r7, #40
	adds	r6, #28
	cmp	r2, #63
	ble.n	.L_080e8540
	mov	r3, fp
	cmp	r3, #1
	beq.n	.L_080e8618
	cmp	r3, #1
	bgt.n	.L_080e85f8
	cmp	r3, #0
	beq.n	.L_080e8600
	b.n	.L_080e8646
.L_080e85f8:
	mov	r4, fp
	cmp	r4, #2
	beq.n	.L_080e8634
	b.n	.L_080e8646
.L_080e8600:
	mov	r0, r8
	lsls	r0, r0, #10
	mov	r1, r8
	str	r0, [sp, #16]
	cmp	r1, #32
	bne.n	.L_080e8646
	movs	r3, #1
	movs	r2, #1
	negs	r3, r3
	mov	fp, r2
	mov	r8, r3
	b.n	.L_080e8646
.L_080e8618:
	mov	r0, r8
	lsls	r3, r0, #10
	movs	r4, #1
	str	r4, [sp, #8]
	str	r3, [sp, #20]
	str	r3, [sp, #16]
	cmp	r0, #32
	bne.n	.L_080e8646
	movs	r2, #1
	movs	r1, #2
	negs	r2, r2
	mov	fp, r1
	mov	r8, r2
	b.n	.L_080e8646
.L_080e8634:
	movs	r3, #0
	mov	r4, r8
	str	r3, [sp, #12]
	cmp	r4, #16
	bne.n	.L_080e8646
	movs	r0, #186
	lsls	r0, r0, #2
	adds	r0, #255
	mov	fp, r0
.L_080e8646:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #186
	lsls	r2, r2, #2
	movs	r1, #1
	adds	r2, #255
	add	r8, r1
	cmp	fp, r2
	beq.n	.L_080e865c
	b.n	.L_080e84c0
.L_080e865c:
	ldr	r0, [pc, #340]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	mov	r0, r9
	lsls	r1, r1, #19
	ldr	r2, [pc, #320]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #320]
	bl	sub_08013300
	ldr	r1, [sp, #48]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #2
	ldr	r2, [sp, #48]
	adds	r5, r0, #0
	bl	sub_080142d4
	ldr	r4, [sp, #48]
	mov	r9, r0
	movs	r1, #164
	movs	r0, #166
	lsls	r0, r0, #6
	lsls	r1, r1, #6
	adds	r3, r4, r0
	adds	r1, #130
	strh	r5, [r3, #0]
	mov	r2, r9
	adds	r3, r4, r1
	adds	r0, #4
	strh	r2, [r3, #0]
	adds	r1, #3
	adds	r3, r4, r0
	movs	r2, #1
	strb	r2, [r3, #0]
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #4]
	movs	r2, #0
	movs	r3, #152
	ldr	r1, [sp, #48]
	mov	sl, r2
	lsls	r3, r3, #6
	movs	r2, #132
	adds	r6, r4, r3
	lsls	r2, r2, #6
	movs	r4, #240
	mov	fp, r4
	mov	r8, r0
	adds	r7, r1, r2
.L_080e86d6:
	bl	sub_08014878
	mov	r3, r9
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r5, r0, #0
	movs	r1, #8
	adds	r0, r7, #0
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_080eaf98
	ldrb	r1, [r7, #9]
	movs	r0, #13
	negs	r0, r0
	adds	r3, r0, #0
	mov	r4, fp
	strh	r4, [r7, #30]
	ands	r1, r3
	mov	r0, fp
	ldrb	r3, [r7, #5]
	movs	r4, #33
	orrs	r1, r0
	negs	r4, r4
	strb	r1, [r7, #9]
	adds	r2, r4, #0
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #7
	strb	r3, [r7, #5]
	adds	r3, r5, r1
	strh	r3, [r7, #28]
	ldr	r3, [sp, #44]
	mov	r4, r8
	ldr	r2, [r3, #16]
	movs	r0, #128
	ldr	r3, [r2, #8]
	lsls	r0, r0, #13
	str	r3, [r4, #0]
	adds	r7, #40
	ldr	r3, [r2, #12]
	adds	r3, r3, r0
	str	r3, [r4, #4]
	mov	r0, r8
	ldr	r3, [r2, #16]
	str	r3, [r4, #8]
	bl	sub_080dc390
	movs	r0, #128
	adds	r1, r5, #0
	mov	r2, r8
	lsls	r0, r0, #15
	bl	sub_0801489c
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r2, sl
	str	r3, [r6, #0]
	ldr	r3, [r1, #8]
	str	r3, [r6, #4]
	negs	r3, r2
	str	r3, [r6, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r6, #28
	cmp	r4, #31
	ble.n	.L_080e86d6
	ldr	r0, [sp, #28]
	bl	sub_08014274
	bl	sub_080dc384
.L_080e8768:
	ldr	r0, [sp, #48]
	movs	r1, #164
	lsls	r1, r1, #6
	adds	r1, #134
	adds	r3, r0, r1
	movs	r2, #1
	strb	r2, [r3, #0]
	ldr	r4, [sp, #40]
	movs	r0, #192
	lsls	r0, r0, #4
	movs	r1, #192
	adds	r0, #172
	lsls	r1, r1, #4
	adds	r3, r4, r0
	adds	r1, #173
	strb	r2, [r3, #0]
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #174
	adds	r2, r4, r3
	movs	r3, #150
	lsls	r3, r3, #2
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000001db
	.4byte 0x84000800
	.4byte 0x000001ed
	.2byte 0x807d
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	ldr	r6, [r3, #108]
	ldr	r3, [pc, #408]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	mov	sl, r1
	bl	sub_080d2d84
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #172
	adds	r2, r6, r3
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_080e8800
	b.n	.L_080e89d6
.L_080e8800:
	movs	r5, #0
	movs	r1, #192
	strb	r5, [r2, #0]
	lsls	r1, r1, #4
	movs	r2, #192
	adds	r1, #173
	lsls	r2, r2, #4
	adds	r3, r6, r1
	adds	r2, #174
	strb	r5, [r3, #0]
	adds	r3, r6, r2
	strh	r5, [r3, #0]
	ldr	r0, [pc, #348]
	bl	sub_08014644
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #167
	bl	sub_081c0010
	ldr	r3, [pc, #336]
	movs	r1, #160
	lsls	r1, r1, #3
	ldr	r2, [pc, #332]
	ldr	r0, [pc, #336]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf72c
	.2byte 0xf9b8
	.2byte 0x2280
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #3
	adds	r3, #8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #316]
	ldr	r2, [pc, #320]
	strh	r5, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #308]
	bl	sub_08013300
	mov	r1, sl
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #1
	mov	r2, sl
	str	r0, [sp, #4]
	bl	sub_080142d4
	movs	r6, #208
	movs	r7, #128
	lsls	r6, r6, #5
	lsls	r7, r7, #5
	mov	fp, r0
	mov	r8, r5
	add	r6, sl
	add	r7, sl
.L_080e8886:
	bl	sub_08014878
	mov	r1, r9
	ldr	r3, [r1, #8]
	movs	r2, #128
	str	r3, [r6, #0]
	lsls	r2, r2, #13
	ldr	r3, [r1, #12]
	adds	r5, r0, #0
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	movs	r3, #208
	lsls	r3, r3, #14
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_0801489c
	ldr	r3, [pc, #220]
	movs	r2, #0
	str	r2, [r6, #12]
	str	r3, [r6, #16]
	str	r2, [r6, #20]
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #1
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #10
	adds	r2, r6, #0
	adds	r0, r0, r1
	adds	r2, #12
	adds	r1, r5, #0
	bl	sub_0801489c
	mov	r2, r8
	negs	r3, r2
	cmp	r3, #0
	bge.n	.L_080e88e2
	adds	r3, #3
.L_080e88e2:
	asrs	r3, r3, #2
	str	r3, [r6, #24]
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r7, #9]
	movs	r1, #13
	strb	r3, [r7, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #1
	strb	r3, [r7, #9]
	add	r8, r2
	movs	r3, #240
	strh	r3, [r7, #30]
	mov	r3, r8
	adds	r7, #40
	adds	r6, #28
	cmp	r3, #63
	ble.n	.L_080e8886
	movs	r0, #1
	bl	sub_08013560
	movs	r7, #0
.L_080e8928:
	movs	r6, #208
	movs	r5, #128
	lsls	r6, r6, #5
	lsls	r5, r5, #5
	movs	r1, #63
	add	r6, sl
	add	r5, sl
	mov	r8, r1
.L_080e8938:
	ldr	r1, [r6, #24]
	cmp	r1, #0
	blt.n	.L_080e8998
	movs	r3, #3
	ands	r1, r3
	ldr	r3, [pc, #40]
	lsls	r1, r1, #1
	add	r1, fp
	ands	r1, r3
	ldr	r2, [pc, #36]
	ldrh	r3, [r5, #8]
	adds	r0, r5, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	adds	r1, r6, #0
	bl	sub_080eb298
	movs	r2, #128
	movs	r1, #62
	adds	r0, r6, #0
	lsls	r2, r2, #8
	bl	sub_080c8978
	ldr	r1, [r6, #24]
	b.n	.L_080e8998
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x02000240
	.4byte 0x080e807d
	.4byte 0x03000260
	.4byte 0xf000f000
	.4byte 0x06002000
	.4byte 0x03001120
	.4byte 0x0300123c
	.4byte 0x000001ea
	.2byte 0x0000
	.2byte 0xfffc
.L_080e8998:
	.2byte 0x2201
	negs	r2, r2
	adds	r3, r1, #1
	add	r8, r2
	str	r3, [r6, #24]
	mov	r3, r8
	adds	r5, #40
	adds	r6, #28
	cmp	r3, #0
	bge.n	.L_080e8938
	movs	r0, #1
	adds	r7, #1
	bl	sub_08013560
	cmp	r7, #39
	ble.n	.L_080e8928
	ldr	r0, [sp, #4]
	bl	sub_08014274
	movs	r3, #166
	lsls	r3, r3, #6
	add	r3, sl
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08014274
	bl	sub_080eb930
	movs	r0, #92
	bl	sub_0801314c
.L_080e89d6:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r1, [r2, #108]
	movs	r4, #230
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r3, [r3, #0]
	ldr	r7, [r2, #32]
	mov	r8, r3
	adds	r3, r7, #0
	adds	r3, #228
	movs	r4, #2
	ldrsh	r5, [r3, r4]
	adds	r3, #4
	mov	fp, r5
	movs	r4, #2
	ldrsh	r5, [r3, r4]
	sub	sp, #20
	str	r5, [sp, #0]
	adds	r3, #20
	ldr	r3, [r3, #0]
	ldr	r6, [r2, #92]
	cmp	r3, #0
	beq.n	.L_080e8a32
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080e8a4a
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080e8a62
.L_080e8a32:
	movs	r5, #197
	lsls	r5, r5, #1
	adds	r3, r1, r5
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e8a72
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080e8a5a
.L_080e8a4a:
	movs	r0, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r2, r2, #9
	movs	r1, #0
	bl	sub_08020228
	b.n	.L_080e8b30
.L_080e8a5a:
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080e8a72
.L_080e8a62:
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #0
	bl	sub_08020228
	b.n	.L_080e8b30
.L_080e8a72:
	ldr	r3, [pc, #204]
	movs	r0, #253
	lsls	r0, r0, #1
	add	r5, sp, #4
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	adds	r1, r5, #0
	bl	sub_08020328
	ldr	r3, [r5, #4]
	mov	r1, r8
	mov	lr, r3
	lsls	r0, r3, #3
	ldr	r3, [r1, #8]
	ldr	r2, [r5, #0]
	ldr	r4, [r5, #8]
	ldr	r5, [r5, #12]
	str	r3, [r6, #0]
	mov	ip, r2
	ldr	r3, [r1, #12]
	lsls	r2, r2, #3
	str	r3, [r6, #4]
	adds	r2, #12
	ldr	r3, [r1, #16]
	movs	r1, #0
	mov	sl, r4
	mov	r9, r5
	lsls	r4, r4, #3
	lsls	r5, r5, #3
	str	r3, [r6, #8]
	strh	r1, [r6, #28]
	cmp	fp, r2
	bge.n	.L_080e8abe
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r6, #28]
.L_080e8abe:
	mov	r2, fp
	adds	r3, r0, #0
	adds	r2, #240
	subs	r3, #12
	cmp	r2, r3
	blt.n	.L_080e8ace
	movs	r3, #1
	strh	r3, [r6, #28]
.L_080e8ace:
	strh	r1, [r6, #30]
	ldr	r2, [sp, #0]
	adds	r3, r4, #0
	adds	r3, #12
	cmp	r2, r3
	bge.n	.L_080e8ae2
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r6, #30]
.L_080e8ae2:
	ldr	r2, [sp, #0]
	adds	r3, r5, #0
	adds	r2, #160
	subs	r3, #12
	cmp	r2, r3
	blt.n	.L_080e8af2
	movs	r3, #1
	strh	r3, [r6, #30]
.L_080e8af2:
	adds	r4, r7, #0
	adds	r4, #236
	ldr	r3, [r4, #0]
	adds	r0, r7, #0
	str	r3, [r6, #12]
	adds	r0, #244
	ldr	r3, [r0, #0]
	adds	r1, r7, #0
	str	r3, [r6, #16]
	adds	r1, #240
	ldr	r3, [r1, #0]
	adds	r2, r7, #0
	str	r3, [r6, #20]
	adds	r2, #248
	ldr	r3, [r2, #0]
	mov	r5, ip
	str	r3, [r6, #24]
	lsls	r3, r5, #19
	str	r3, [r4, #0]
	mov	r4, lr
	lsls	r3, r4, #19
	mov	r5, sl
	str	r3, [r0, #0]
	lsls	r3, r5, #19
	mov	r0, r9
	str	r3, [r1, #0]
	lsls	r3, r0, #19
	str	r3, [r2, #0]
	mov	r0, r8
	bl	sub_08020138
.L_080e8b30:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r2
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r7, r1, #0
	ldr	r1, [r2, #108]
	mov	fp, r0
	movs	r0, #230
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r6, [r2, #92]
	mov	r9, r3
	ldr	r3, [r2, #32]
	adds	r3, #252
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080e8b76
	b.n	.L_080e8c8a
.L_080e8b76:
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e8b88
	b.n	.L_080e8c8a
.L_080e8b88:
	movs	r3, #1
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080e8c20
	movs	r1, #28
	ldrsh	r3, [r6, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080e8bc8
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	sub_08002096
	cmp	r0, #0
	blt.n	.L_080e8bb2
	adds	r0, r5, #0
	bl	sub_08002096
	b.n	.L_080e8bba
.L_080e8bb2:
	adds	r0, r5, #0
	bl	sub_08002096
	negs	r0, r0
.L_080e8bba:
	ldr	r3, [pc, #220]
	mov	r1, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6833
	adds	r3, r3, r0
	b.n	.L_080e8c22
.L_080e8bc8:
	cmp	r3, #1
	bne.n	.L_080e8bf6
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	sub_08002096
	cmp	r0, #0
	blt.n	.L_080e8be2
	adds	r0, r5, #0
	bl	sub_08002096
	negs	r0, r0
	b.n	.L_080e8be8
.L_080e8be2:
	adds	r0, r5, #0
	bl	sub_08002096
.L_080e8be8:
	ldr	r3, [pc, #172]
	mov	r1, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6833
	adds	r3, r3, r0
	b.n	.L_080e8c22
.L_080e8bf6:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	sub_08002096
	cmp	r0, #0
	blt.n	.L_080e8c0a
	adds	r0, r5, #0
	bl	sub_08002096
	b.n	.L_080e8c12
.L_080e8c0a:
	adds	r0, r5, #0
	bl	sub_08002096
	negs	r0, r0
.L_080e8c12:
	ldr	r3, [pc, #132]
	mov	r1, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6833
	adds	r3, r3, r0
	b.n	.L_080e8c22
.L_080e8c20:
	ldr	r3, [r6, #0]
.L_080e8c22:
	mov	r8, r3
	movs	r3, #2
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080e8c7c
	movs	r1, #30
	ldrsh	r3, [r6, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080e8c52
	cmp	r3, #1
	bne.n	.L_080e8c52
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_080e8c66
	adds	r0, r5, #0
	bl	sub_08002096
	b.n	.L_080e8c6e
.L_080e8c52:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	sub_08002096
	cmp	r0, #0
	blt.n	.L_080e8c66
	adds	r0, r5, #0
	bl	sub_08002096
	b.n	.L_080e8c6e
.L_080e8c66:
	adds	r0, r5, #0
	bl	sub_08002096
	negs	r0, r0
.L_080e8c6e:
	ldr	r3, [pc, #40]
	mov	r1, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68b3
	adds	r2, r3, r0
	b.n	.L_080e8c7e
.L_080e8c7c:
	ldr	r2, [r6, #8]
.L_080e8c7e:
	mov	r0, r9
	mov	r3, r8
	str	r3, [r0, #8]
	ldr	r3, [r6, #4]
	str	r2, [r0, #16]
	str	r3, [r0, #12]
.L_080e8c8a:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	adds	r2, r3, #0
	adds	r2, #224
	ldr	r0, [r3, #92]
	ldr	r4, [r2, #0]
	ldr	r2, [r3, #108]
	adds	r3, r1, #0
	adds	r3, #252
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080e8cc8
	movs	r5, #197
	lsls	r5, r5, #1
	adds	r3, r2, r5
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e8cd4
.L_080e8cc8:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_08020228
	b.n	.L_080e8cf8
.L_080e8cd4:
	ldr	r3, [r0, #12]
	adds	r2, r1, #0
	adds	r2, #236
	str	r3, [r2, #0]
	adds	r2, #8
	ldr	r3, [r0, #16]
	movs	r1, #1
	str	r3, [r2, #0]
	subs	r2, #4
	ldr	r3, [r0, #20]
	str	r3, [r2, #0]
	adds	r2, #8
	ldr	r3, [r0, #24]
	str	r3, [r2, #0]
	movs	r3, #24
	ldrsh	r0, [r4, r3]
	bl	sub_080d4384
.L_080e8cf8:
	pop	{r5, pc}
	.align 2, 0
