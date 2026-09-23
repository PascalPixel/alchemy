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
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_0801587c, 0x0801587c
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_0814f5d0, 0x0814f5d0
	.set sub_0814fab8, 0x0814fab8
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_081c0010, 0x081c0010
	.global Func_0814ef44
	.thumb_func
Func_0814ef44:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #148
	str	r0, [sp, #76]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	adds	r7, r1, #0
	str	r0, [sp, #72]
	movs	r2, #0
	ldr	r1, [r3, #96]
	movs	r0, #0
	str	r1, [sp, #68]
	ldr	r3, [r3, #100]
	str	r2, [sp, #48]
	str	r3, [sp, #56]
	bl	sub_081435e0
	cmp	r7, #13
	bne.n	.L_0814ef84
	ldr	r3, [sp, #72]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #180
	adds	r2, r3, r5
	movs	r3, #24
	str	r3, [r2, #0]
.L_0814ef84:
	ldr	r0, [sp, #76]
	ldr	r3, [r0, #28]
	cmp	r3, #1
	bne.n	.L_0814efee
	movs	r1, #2
	cmp	r7, #6
	beq.n	.L_0814efaa
	cmp	r7, #2
	beq.n	.L_0814ef9a
	cmp	r7, #7
	bne.n	.L_0814ef9e
.L_0814ef9a:
	movs	r1, #0
	b.n	.L_0814efaa
.L_0814ef9e:
	adds	r3, r7, #0
	subs	r3, #9
	movs	r1, #3
	cmp	r3, #1
	bls.n	.L_0814efaa
	movs	r1, #1
.L_0814efaa:
	cmp	r7, #6
	beq.n	.L_0814efbe
	cmp	r7, #0
	beq.n	.L_0814efbe
	cmp	r7, #8
	beq.n	.L_0814efbe
	cmp	r7, #9
	beq.n	.L_0814efbe
	cmp	r7, #10
	bne.n	.L_0814efd2
.L_0814efbe:
	ldr	r2, [sp, #76]
	ldr	r0, [sp, #76]
	ldr	r3, [r2, #4]
	add	r2, sp, #136
	lsls	r3, r3, #4
	orrs	r1, r3
	add	r3, sp, #124
	bl	sub_0815585c
	b.n	.L_0814efe8
.L_0814efd2:
	ldr	r5, [sp, #76]
	add	r2, sp, #136
	ldr	r3, [r5, #4]
	adds	r0, r5, #0
	lsls	r3, r3, #4
	orrs	r1, r3
	movs	r3, #32
	orrs	r1, r3
	add	r3, sp, #124
	bl	sub_0815585c
.L_0814efe8:
	ldr	r0, [sp, #76]
	movs	r3, #0
	str	r3, [r0, #24]
.L_0814efee:
	cmp	r7, #0
	beq.n	.L_0814effa
	cmp	r7, #8
	beq.n	.L_0814effa
	cmp	r7, #9
	bne.n	.L_0814f02c
.L_0814effa:
	ldr	r2, [sp, #76]
	add	r5, sp, #112
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r2, [r5, #0]
	movs	r3, #64
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	str	r3, [sp, #48]
	adds	r2, #40
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #44]
	b.n	.L_0814f030
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
.L_0814f02c:
	movs	r5, #1
	str	r5, [sp, #44]
.L_0814f030:
	ldr	r1, [sp, #56]
	ldr	r0, [pc, #728]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #72]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #712]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #72]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r5, r1, r2
	ldr	r0, [pc, #700]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [sp, #72]
	mov	sl, r3
	movs	r3, #156
	movs	r1, #144
	lsls	r3, r3, #6
	lsls	r1, r1, #1
	adds	r2, r0, r3
.L_0814f072:
	ldrb	r3, [r5, #0]
	movs	r0, #1
	add	sl, r0
	strb	r3, [r2, #0]
	adds	r5, #2
	adds	r2, #1
	cmp	sl, r1
	bne.n	.L_0814f072
	cmp	r7, #1
	bls.n	.L_0814f09a
	cmp	r7, #3
	beq.n	.L_0814f09a
	cmp	r7, #4
	beq.n	.L_0814f09a
	cmp	r7, #5
	beq.n	.L_0814f09a
	cmp	r7, #8
	beq.n	.L_0814f09a
	cmp	r7, #12
	bne.n	.L_0814f0aa
.L_0814f09a:
	ldr	r1, [sp, #76]
	ldr	r3, [r1, #24]
	cmp	r3, #0
	bne.n	.L_0814f0a6
	ldr	r0, [pc, #628]
	b.n	.L_0814f0c8
.L_0814f0a6:
	ldr	r0, [pc, #628]
	b.n	.L_0814f0c8
.L_0814f0aa:
	adds	r3, r7, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_0814f0b6
	ldr	r0, [pc, #620]
	b.n	.L_0814f0c8
.L_0814f0b6:
	cmp	r7, #6
	beq.n	.L_0814f0c2
	cmp	r7, #11
	beq.n	.L_0814f0c2
	cmp	r7, #13
	bne.n	.L_0814f0c6
.L_0814f0c2:
	ldr	r0, [pc, #608]
	b.n	.L_0814f0c8
.L_0814f0c6:
	ldr	r0, [pc, #608]
.L_0814f0c8:
	bl	sub_08013300
	adds	r2, r0, #0
	movs	r0, #160
	adds	r1, r2, #0
	ldr	r3, [pc, #600]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0b
	cmp	r2, #0
	bne.n	.L_0814f106
	cmp	r7, #6
	bne.n	.L_0814f0ea
	ldr	r0, [pc, #584]
	b.n	.L_0814f130
.L_0814f0ea:
	cmp	r7, #2
	beq.n	.L_0814f0f2
	cmp	r7, #7
	bne.n	.L_0814f0f6
.L_0814f0f2:
	ldr	r0, [pc, #576]
	b.n	.L_0814f130
.L_0814f0f6:
	adds	r3, r7, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_0814f102
	ldr	r0, [pc, #544]
	b.n	.L_0814f130
.L_0814f102:
	ldr	r0, [pc, #564]
	b.n	.L_0814f130
.L_0814f106:
	cmp	r7, #6
	beq.n	.L_0814f112
	cmp	r7, #11
	beq.n	.L_0814f112
	cmp	r7, #13
	bne.n	.L_0814f116
.L_0814f112:
	ldr	r0, [pc, #552]
	b.n	.L_0814f130
.L_0814f116:
	cmp	r7, #2
	beq.n	.L_0814f11e
	cmp	r7, #7
	bne.n	.L_0814f122
.L_0814f11e:
	ldr	r0, [pc, #544]
	b.n	.L_0814f130
.L_0814f122:
	adds	r3, r7, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_0814f12e
	ldr	r0, [pc, #536]
	b.n	.L_0814f130
.L_0814f12e:
	ldr	r0, [pc, #536]
.L_0814f130:
	bl	sub_08013300
	adds	r2, r0, #0
	ldr	r3, [sp, #72]
	movs	r5, #184
	adds	r2, #128
	lsls	r5, r5, #5
	adds	r1, r3, r5
	adds	r0, r2, #0
	bl	sub_0801587c
	ldr	r1, [sp, #76]
	mov	r2, sp
	adds	r2, #80
	ldr	r0, [r1, #4]
	adds	r1, r2, #0
	str	r2, [sp, #40]
	bl	sub_08144aac
	cmp	r7, #0
	beq.n	.L_0814f16a
	cmp	r7, #6
	beq.n	.L_0814f16a
	cmp	r7, #8
	beq.n	.L_0814f16a
	cmp	r7, #9
	beq.n	.L_0814f16a
	cmp	r7, #10
	bne.n	.L_0814f1c4
.L_0814f16a:
	ldr	r5, [pc, #480]
	movs	r3, #0
	mov	sl, r3
	movs	r6, #255
.L_0814f172:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #8]
	movs	r1, #128
	movs	r0, #1
	movs	r3, #0
	add	sl, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L_0814f172
	movs	r2, #54
	str	r2, [sp, #64]
	cmp	r7, #9
	bne.n	.L_0814f1b4
	b.n	.L_0814f446
.L_0814f1b4:
	movs	r3, #76
	str	r3, [sp, #64]
	cmp	r7, #8
	bne.n	.L_0814f1be
	b.n	.L_0814f446
.L_0814f1be:
	ldr	r5, [sp, #76]
	ldr	r3, [r5, #20]
	b.n	.L_0814f440
.L_0814f1c4:
	cmp	r7, #7
	bne.n	.L_0814f220
	ldr	r5, [pc, #384]
	movs	r0, #0
	movs	r1, #255
	mov	sl, r0
	movs	r6, #0
	mov	r8, r1
.L_0814f1d4:
	movs	r3, #192
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	str	r6, [r5, #0]
	str	r6, [r5, #8]
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #8
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	negs	r3, r3
	lsls	r3, r3, #9
	str	r3, [r5, #16]
	bl	sub_08014878
	mov	r3, r8
	ands	r0, r3
	subs	r0, #127
	lsls	r0, r0, #8
	str	r0, [r5, #20]
	movs	r1, #128
	movs	r0, #1
	add	sl, r0
	lsls	r1, r1, #2
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L_0814f1d4
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #20]
	b.n	.L_0814f440
.L_0814f220:
	cmp	r7, #12
	bne.n	.L_0814f26a
	ldr	r5, [pc, #292]
	movs	r3, #0
	mov	sl, r3
	movs	r6, #255
.L_0814f22c:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #8]
	movs	r1, #128
	movs	r0, #1
	movs	r3, #0
	add	sl, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L_0814f22c
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #20]
	b.n	.L_0814f440
.L_0814f26a:
	cmp	r7, #1
	beq.n	.L_0814f272
	cmp	r7, #13
	bne.n	.L_0814f2b6
.L_0814f272:
	ldr	r5, [pc, #216]
	movs	r3, #0
	mov	sl, r3
	movs	r6, #255
.L_0814f27a:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #8]
	movs	r1, #128
	movs	r0, #1
	movs	r3, #0
	add	sl, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L_0814f27a
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #20]
	b.n	.L_0814f440
.L_0814f2b6:
	cmp	r7, #11
	bne.n	.L_0814f350
	ldr	r5, [pc, #144]
	movs	r3, #0
	mov	sl, r3
.L_0814f2c0:
	bl	sub_08014878
	movs	r1, #200
	bl	sub_0800206c
	subs	r0, #100
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #200
	bl	sub_0800206c
	subs	r0, #100
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r1, #200
	bl	sub_0800206c
	subs	r0, #100
	lsls	r0, r0, #15
	str	r0, [r5, #8]
	movs	r1, #128
	movs	r0, #1
	movs	r3, #0
	add	sl, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L_0814f2c0
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #20]
	lsls	r3, r3, #3
	adds	r3, #46
	b.n	.L_0814f444
	.4byte 0x00000134
	.4byte 0x0000017e
	.4byte 0x00000190
	.4byte 0x00000177
	.4byte 0x0000017d
	.4byte 0x00000152
	.4byte 0x00000148
	.4byte 0x00000184
	.4byte 0x03000730
	.4byte 0x00000150
	.4byte 0x00000156
	.4byte 0x00000154
	.4byte 0x00000151
	.4byte 0x00000157
	.4byte 0x00000153
	.4byte 0x00000155
	.2byte 0x0000
	.2byte 0x0201
.L_0814f350:
	cmp	r7, #2
	bne.n	.L_0814f3ba
	ldr	r5, [pc, #800]
	movs	r3, #0
	mov	sl, r3
	mov	r8, r5
.L_0814f35c:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #63
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [pc, #760]
	str	r3, [r0, #4]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	mov	r1, r8
	str	r3, [r1, #8]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	mov	r2, r8
	lsls	r3, r3, #13
	str	r3, [r2, #16]
	movs	r5, #1
	movs	r3, #0
	movs	r0, #128
	str	r3, [r2, #24]
	add	sl, r5
	movs	r3, #28
	lsls	r0, r0, #2
	add	r8, r3
	cmp	sl, r0
	bne.n	.L_0814f35c
	b.n	.L_0814f43c
.L_0814f3ba:
	cmp	r7, #3
	bne.n	.L_0814f3fe
	ldr	r5, [pc, #696]
	movs	r2, #0
	mov	sl, r2
	movs	r6, #255
.L_0814f3c6:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #14
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	movs	r3, #0
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	movs	r0, #128
	movs	r3, #1
	add	sl, r3
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	sl, r0
	bne.n	.L_0814f3c6
	b.n	.L_0814f43c
.L_0814f3fe:
	ldr	r5, [pc, #632]
	movs	r2, #0
	mov	sl, r2
	movs	r6, #255
.L_0814f406:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	movs	r3, #0
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	movs	r0, #128
	movs	r3, #1
	add	sl, r3
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	sl, r0
	bne.n	.L_0814f406
.L_0814f43c:
	ldr	r1, [sp, #76]
	ldr	r3, [r1, #20]
.L_0814f440:
	lsls	r3, r3, #3
	adds	r3, #58
.L_0814f444:
	str	r3, [sp, #64]
.L_0814f446:
	movs	r2, #64
	str	r2, [sp, #52]
	ldr	r5, [sp, #76]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_0814f458
	movs	r0, #32
	str	r0, [sp, #52]
	b.n	.L_0814f460
.L_0814f458:
	cmp	r3, #2
	bne.n	.L_0814f460
	movs	r1, #128
	str	r1, [sp, #52]
.L_0814f460:
	cmp	r7, #8
	bne.n	.L_0814f468
	movs	r2, #128
	str	r2, [sp, #52]
.L_0814f468:
	cmp	r7, #12
	bne.n	.L_0814f470
	movs	r3, #16
	str	r3, [sp, #52]
.L_0814f470:
	ldr	r5, [sp, #72]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r5, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r5, r1
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #500]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r3, [sp, #64]
	movs	r2, #0
	mov	fp, r2
	cmp	r3, #0
	bne.n	.L_0814f49e
	b.n	.L_0814f9c4
.L_0814f49e:
	mov	r5, sp
	adds	r5, #136
	str	r5, [sp, #32]
	str	r2, [sp, #8]
.L_0814f4a6:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	mov	r0, fp
	str	r3, [sp, #36]
	cmp	r0, #40
	bne.n	.L_0814f4ba
	movs	r0, #0
	bl	sub_081180e8
.L_0814f4ba:
	cmp	r7, #7
	bne.n	.L_0814f4ce
	movs	r3, #3
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0814f4ce
	ldr	r0, [pc, #440]
	bl	sub_0815f0a0
.L_0814f4ce:
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #28]
	cmp	r3, #1
	bne.n	sub_0814f5d0
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0814f552
	ldr	r0, [sp, #8]
	bl	sub_08002096
	ldr	r5, [sp, #32]
	lsls	r3, r0, #2
	ldr	r2, [r5, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #2
	ldr	r0, [sp, #48]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r0
	subs	r3, #20
	ldr	r0, [sp, #8]
	mov	sl, r3
	bl	sub_08002090
	ldr	r3, [r5, #4]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r1, fp
	subs	r5, #24
	cmp	r1, #32
	ble.n	.L_0814f518
	lsls	r3, r1, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #64
.L_0814f518:
	ldr	r2, [sp, #72]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r2, r2, r3
	mov	r8, r2
	movs	r6, #40
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	mov	r1, r8
	mov	r2, sl
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4658
	cmp	r0, #3
	bgt.n	sub_0814f5d0
	ldr	r1, [sp, #40]
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #68]
	ldr	r4, [r1, #4]
	mov	r2, sl
	mov	r1, r8
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe03e
.L_0814f552:
	ldr	r0, [sp, #8]
	bl	sub_08002096
	ldr	r5, [sp, #32]
	lsls	r2, r0, #2
	ldr	r3, [r5, #0]
	adds	r2, r2, r0
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	lsls	r2, r2, #1
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	adds	r2, r2, r3
	subs	r2, #10
	ldr	r0, [sp, #8]
	mov	r9, r2
	bl	sub_08002090
	ldr	r3, [r5, #4]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r0, fp
	subs	r5, #24
	cmp	r0, #32
	ble.n	.L_0814f590
	lsls	r3, r0, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #64
.L_0814f590:
	ldr	r1, [sp, #72]
	movs	r2, #184
	lsls	r2, r2, #5
	movs	r3, #20
	movs	r0, #40
	adds	r6, r1, r2
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r6, #0
	mov	sl, r3
	mov	r8, r0
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	mov	r2, r9
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4659
	cmp	r1, #3
	bgt.n	sub_0814f5d0
	mov	r2, sl
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	ldr	r4, [r0, #4]
	mov	r2, r9
	ldr	r0, [sp, #68]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2100
	str	r1, [sp, #60]
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #20]
	cmp	r3, #0
	bne.n	.L_0814f5de
	b.n	.L_0814f996
.L_0814f5de:
	ldr	r3, [sp, #36]
	mov	r5, sp
	mov	r0, fp
	adds	r3, #12
	adds	r5, #88
	lsls	r0, r0, #9
	movs	r1, #36
	str	r3, [sp, #20]
	str	r5, [sp, #28]
	str	r0, [sp, #16]
	str	r1, [sp, #12]
.L_0814f5f4:
	ldr	r5, [sp, #76]
	ldr	r2, [sp, #12]
	ldrsh	r0, [r2, r5]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	ldr	r0, [sp, #60]
	lsls	r0, r0, #3
	mov	r9, r0
	bl	sub_08014de4
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #20]
	bl	sub_080156e8
	ldr	r3, [r5, #8]
	ldr	r1, [sp, #28]
	str	r3, [r1, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r1, #4]
	ldr	r3, [r5, #16]
	str	r3, [r1, #8]
	ldr	r0, [sp, #28]
	bl	sub_08015128
	mov	r3, r9
	adds	r3, #20
	cmp	fp, r3
	bne.n	.L_0814f636
	movs	r0, #126
	bl	sub_081c0010
.L_0814f636:
	cmp	r7, #8
	bne.n	.L_0814f652
	mov	r3, r9
	adds	r3, #41
	cmp	fp, r3
	bne.n	.L_0814f6a6
	ldr	r2, [sp, #12]
	ldr	r5, [sp, #76]
	movs	r1, #7
	ldrsh	r0, [r2, r5]
	movs	r3, #32
	movs	r2, #1
	str	r3, [sp, #0]
	b.n	.L_0814f66c
.L_0814f652:
	cmp	r7, #11
	bne.n	.L_0814f688
	mov	r3, r9
	adds	r3, #24
	cmp	fp, r3
	bne.n	.L_0814f6a6
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #76]
	ldrsh	r0, [r1, r3]
	movs	r3, #28
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
.L_0814f66c:
	negs	r2, r2
	ldr	r3, [sp, #60]
	bl	sub_0814cd48
	b.n	.L_0814f6a6
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffce0000
	.4byte 0x08143001
	.2byte 0x0166
	.2byte 0x0000
.L_0814f688:
	mov	r3, r9
	adds	r3, #36
	cmp	fp, r3
	bne.n	.L_0814f6a6
	ldr	r2, [sp, #76]
	ldr	r5, [sp, #12]
	movs	r3, #28
	ldrsh	r0, [r5, r2]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #60]
	bl	sub_0814cd48
.L_0814f6a6:
	cmp	fp, r9
	bgt.n	.L_0814f6ac
	b.n	.L_0814f978
.L_0814f6ac:
	cmp	r7, #0
	beq.n	.L_0814f71e
	cmp	r7, #6
	beq.n	.L_0814f71e
	cmp	r7, #7
	beq.n	.L_0814f71e
	cmp	r7, #8
	beq.n	.L_0814f71e
	cmp	r7, #10
	beq.n	.L_0814f71e
	cmp	r7, #12
	beq.n	.L_0814f71e
	cmp	r7, #9
	bne.n	.L_0814f6d6
	mov	r3, fp
	mov	r5, r9
	subs	r0, r3, r5
	lsls	r0, r0, #11
	bl	sub_08015068
	b.n	.L_0814f732
.L_0814f6d6:
	cmp	r7, #11
	beq.n	.L_0814f732
	cmp	r7, #13
	bne.n	.L_0814f6e8
	mov	r1, fp
	lsls	r0, r1, #9
	bl	sub_080150e4
	b.n	.L_0814f732
.L_0814f6e8:
	cmp	r7, #1
	bne.n	.L_0814f6fe
	mov	r2, fp
	lsls	r5, r2, #9
	adds	r0, r5, #0
	bl	sub_08015024
	adds	r0, r5, #0
	bl	sub_080150e4
	b.n	.L_0814f732
.L_0814f6fe:
	cmp	r7, #2
	beq.n	.L_0814f706
	cmp	r7, #12
	bne.n	.L_0814f71a
.L_0814f706:
	ldr	r3, [sp, #60]
	mov	r5, fp
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #3
	subs	r0, r5, r0
	lsls	r0, r0, #9
	bl	sub_08015068
	b.n	.L_0814f732
.L_0814f71a:
	cmp	r7, #3
	bne.n	.L_0814f726
.L_0814f71e:
	ldr	r0, [sp, #16]
	bl	sub_08015068
	b.n	.L_0814f732
.L_0814f726:
	ldr	r0, [sp, #16]
	bl	sub_08015068
	ldr	r0, [sp, #16]
	bl	sub_08015024
.L_0814f732:
	ldr	r1, [sp, #52]
	movs	r0, #0
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L_0814f73e
	b.n	.L_0814f978
.L_0814f73e:
	ldr	r3, [sp, #60]
	subs	r5, r7, #3
	lsls	r2, r3, #6
	ldr	r0, [pc, #672]
	lsls	r3, r3, #9
	subs	r3, r3, r2
	str	r5, [sp, #24]
	lsls	r3, r3, #2
	adds	r6, r3, r0
.L_0814f750:
	ldr	r1, [sp, #24]
	cmp	r1, #2
	bhi.n	.L_0814f766
	mov	r2, sl
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r3, r3, #1
	add	r3, r9
	adds	r2, r3, #0
	adds	r2, #32
	b.n	.L_0814f77e
.L_0814f766:
	cmp	r7, #7
	bne.n	.L_0814f77a
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	add	r3, r9
	adds	r2, r3, #0
	adds	r2, #64
	b.n	.L_0814f77e
.L_0814f77a:
	movs	r2, #128
	lsls	r2, r2, #9
.L_0814f77e:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0814f786
	adds	r3, #3
.L_0814f786:
	asrs	r3, r3, #2
	add	r3, r9
	cmp	fp, r3
	bgt.n	.L_0814f790
	b.n	.L_0814f96a
.L_0814f790:
	cmp	fp, r2
	blt.n	.L_0814f796
	b.n	.L_0814f96a
.L_0814f796:
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	adds	r0, r0, r2
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #564]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1240
	mov	r8, r0
	lsls	r3, r0, #1
	add	r3, r8
	lsls	r0, r3, #4
	cmp	r0, #0
	bge.n	.L_0814f7ca
	adds	r0, #63
.L_0814f7ca:
	asrs	r0, r0, #6
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_0814f7d4
	b.n	.L_0814f96a
.L_0814f7d4:
	add	r5, sp, #100
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	cmp	r7, #0
	beq.n	.L_0814f7ea
	cmp	r7, #8
	beq.n	.L_0814f7ea
	cmp	r7, #9
	bne.n	.L_0814f7f2
.L_0814f7ea:
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #48]
	adds	r3, r3, r2
	b.n	.L_0814f7f6
.L_0814f7f2:
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
.L_0814f7f6:
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r0, #58
	adds	r3, #16
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r0, #255
	cmp	r3, r0
	bgt.n	.L_0814f80e
	movs	r3, #157
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L_0814f80e:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #122
	cmp	r3, r2
	ble.n	.L_0814f81c
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_0814f81c:
	ldr	r2, [pc, #464]
	adds	r1, r3, r2
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_0814f82a
	adds	r2, r3, #0
	subs	r2, #187
.L_0814f82a:
	asrs	r2, r2, #7
	movs	r3, #3
	subs	r4, r3, r2
	cmp	r7, #0
	beq.n	.L_0814f83c
	cmp	r7, #8
	beq.n	.L_0814f83c
	cmp	r7, #9
	bne.n	.L_0814f842
.L_0814f83c:
	mov	r3, sl
	lsls	r0, r3, #2
	b.n	.L_0814f858
.L_0814f842:
	cmp	r7, #11
	beq.n	.L_0814f84a
	cmp	r7, #13
	bne.n	.L_0814f850
.L_0814f84a:
	movs	r3, #4
	subs	r4, r3, r2
	b.n	.L_0814f862
.L_0814f850:
	cmp	r7, #7
	bne.n	.L_0814f862
	mov	r1, sl
	lsls	r0, r1, #2
.L_0814f858:
	add	r0, fp
	movs	r1, #9
	bl	sub_08002064
	adds	r4, r0, #0
.L_0814f862:
	cmp	r7, #0
	beq.n	.L_0814f886
	cmp	r7, #3
	beq.n	.L_0814f886
	cmp	r7, #4
	beq.n	.L_0814f886
	cmp	r7, #5
	beq.n	.L_0814f886
	cmp	r7, #7
	beq.n	.L_0814f886
	cmp	r7, #8
	beq.n	.L_0814f886
	cmp	r7, #9
	beq.n	.L_0814f886
	cmp	r7, #11
	beq.n	.L_0814f886
	cmp	r7, #13
	bne.n	.L_0814f8b4
.L_0814f886:
	ldr	r2, [pc, #364]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #72]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #352]
	ldr	r2, [r5, #0]
	ldrb	r0, [r3, r4]
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r5, [sp, #40]
	lsrs	r4, r0, #1
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r0, [sp, #68]
	ldr	r4, [r5, #4]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe027
.L_0814f8b4:
	cmp	r7, #12
	bne.n	.L_0814f8dc
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #0]
	subs	r3, #12
	mov	ip, r3
	ldr	r3, [sp, #72]
	movs	r5, #156
	movs	r1, #24
	lsls	r5, r5, #6
	str	r1, [sp, #4]
	subs	r2, #6
	adds	r1, r3, r5
	str	r7, [sp, #0]
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #68]
	mov	r3, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe013
.L_0814f8dc:
	ldr	r2, [pc, #284]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #56]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r5, [sp, #40]
	subs	r3, r3, r4
	ldr	r0, [sp, #68]
	ldr	r4, [r5, #4]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f02
	bls.n	.L_0814f924
	cmp	r7, #6
	beq.n	.L_0814f924
	cmp	r7, #8
	beq.n	.L_0814f924
	cmp	r7, #9
	beq.n	.L_0814f924
	cmp	r7, #10
	beq.n	.L_0814f924
	cmp	r7, #11
	beq.n	.L_0814f924
	cmp	r7, #13
	beq.n	.L_0814f924
	cmp	r7, #12
	bne.n	.L_0814f94e
.L_0814f924:
	ldr	r5, [r6, #0]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #8]
.L_0814f94e:
	cmp	r7, #7
	bne.n	.L_0814f96a
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #4]
	adds	r2, r2, r3
	str	r2, [r6, #4]
	ldr	r3, [r6, #20]
	ldr	r2, [r6, #8]
	adds	r2, r2, r3
	str	r2, [r6, #8]
.L_0814f96a:
	ldr	r1, [sp, #52]
	movs	r0, #1
	add	sl, r0
	adds	r6, #28
	cmp	sl, r1
	beq.n	.L_0814f978
	b.n	.L_0814f750
.L_0814f978:
	ldr	r2, [sp, #16]
	ldr	r3, [pc, #132]
	ldr	r5, [sp, #12]
	ldr	r0, [sp, #60]
	adds	r2, r2, r3
	adds	r5, #2
	adds	r0, #1
	str	r2, [sp, #16]
	str	r5, [sp, #12]
	str	r0, [sp, #60]
	ldr	r1, [sp, #76]
	ldr	r3, [r1, #20]
	cmp	r0, r3
	beq.n	.L_0814f996
	b.n	.L_0814f5f4
.L_0814f996:
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #72]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #8]
	movs	r1, #128
	ldr	r3, [sp, #64]
	lsls	r1, r1, #4
	movs	r2, #1
	adds	r0, r0, r1
	add	fp, r2
	str	r0, [sp, #8]
	cmp	fp, r3
	beq.n	.L_0814f9c4
	b.n	.L_0814f4a6
.L_0814f9c4:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #148
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x030002d4
	.4byte 0xfffffec6
	.4byte 0x0819744c
	.4byte 0x0819745e
	.4byte 0x08197410
	.4byte 0xfffff000
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0814fab8
	pop	{pc}
	.2byte 0x0000
