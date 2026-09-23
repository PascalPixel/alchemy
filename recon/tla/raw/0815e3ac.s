.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_080156e8, 0x080156e8
	.set sub_08015778, 0x08015778
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08118030, 0x08118030
	.set sub_08118048, 0x08118048
	.set sub_08118058, 0x08118058
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_0813ba50, 0x0813ba50
	.set sub_08143d04, 0x08143d04
	.set sub_08143d80, 0x08143d80
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e1fc, 0x0815e1fc
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e8ca, 0x0815e8ca
	.set sub_0815e8fc, 0x0815e8fc
	.set sub_0815eede, 0x0815eede
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_0815e3ac
	.thumb_func
Func_0815e3ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	ldr	r5, [sp, #52]
	str	r0, [sp, #48]
	ldr	r1, [r3, #96]
	str	r1, [sp, #44]
	ldr	r2, [r3, #100]
	str	r2, [sp, #36]
	ldr	r3, [r3, #48]
	str	r3, [sp, #32]
	ldr	r3, [sp, #52]
	ldr	r3, [r3, #0]
	str	r3, [sp, #28]
	ldr	r0, [r5, #8]
	bl	sub_08016ca4
	str	r0, [sp, #24]
	movs	r0, #1
	bl	sub_08013560
	bl	sub_0813ba50
	bl	sub_08143d80
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #28]
	subs	r6, #5
	str	r6, [sp, #20]
	cmp	r6, #1
	bhi.n	.L_0815e43a
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0815e428
	movs	r1, #11
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #27
	bl	sub_081963ec
	b.n	.L_0815e464
	movs	r0, r0
	.2byte 0x1f80
	.2byte 0x0000
.L_0815e428:
	movs	r1, #15
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #31
	bl	sub_081963ec
	b.n	.L_0815e464
.L_0815e43a:
	ldr	r0, [sp, #52]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0815e454
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0815e464
.L_0815e454:
	movs	r1, #7
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #23
	bl	sub_081963ec
.L_0815e464:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #104]
	adds	r3, #188
	str	r2, [sp, #56]
	mov	r1, sp
	ldr	r3, [r3, #0]
	adds	r1, #56
	str	r1, [sp, #12]
	movs	r0, #1
	str	r3, [r1, #4]
	bl	sub_08013560
	ldr	r2, [sp, #28]
	cmp	r2, #4
	bne.n	.L_0815e490
	ldr	r3, [sp, #48]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r3, r5
	ldr	r0, [pc, #252]
	b.n	.L_0815e4de
.L_0815e490:
	ldr	r6, [sp, #28]
	cmp	r6, #3
	bne.n	.L_0815e4aa
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #240]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0815e4fa
.L_0815e4aa:
	ldr	r5, [sp, #28]
	cmp	r5, #6
	bhi.n	.L_0815e4fa
	ldr	r2, [pc, #224]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0815e4d4
	.4byte 0x0815e4d4
	.4byte 0x0815e4e8
	.4byte 0x0815e4fa
	.4byte 0x0815e4fa
	.4byte 0x0815e4d4
	.2byte 0xe4d4
	.2byte 0x0815
	ldr	r6, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r6, r2
	ldr	r0, [pc, #184]
.L_0815e4de:
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_0815e4fa
	ldr	r3, [sp, #48]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r3, r5
	ldr	r0, [pc, #168]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_0815e4fa:
	ldr	r6, [sp, #52]
	ldr	r3, [r6, #8]
	cmp	r3, #7
	ble.n	.L_0815e506
	ldr	r0, [pc, #156]
	b.n	.L_0815e508
.L_0815e506:
	ldr	r0, [pc, #156]
.L_0815e508:
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #128]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #120]
	ldr	r1, [pc, #120]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [sp, #48]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r1, #200
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #88]
	bl	sub_080145a8
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r6, [sp, #52]
	mov	r1, sp
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	adds	r1, #76
	str	r1, [sp, #16]
	bl	sub_0815e1fc
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne.n	.L_0815e5b8
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #0]
	movs	r3, #96
	b.n	.L_0815e5be
	.4byte 0x00001f81
	.4byte 0x0000012c
	.4byte 0x00000189
	.4byte 0x0815e4b8
	.4byte 0x00000179
	.4byte 0x0000017a
	.4byte 0x00000151
	.4byte 0x0000010c
	.4byte 0x00000137
	.4byte 0x0000015c
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
.L_0815e5b8:
	ldr	r5, [sp, #16]
	movs	r3, #32
	ldr	r2, [r5, #0]
.L_0815e5be:
	subs	r3, r3, r2
	str	r3, [sp, #40]
	ldr	r6, [sp, #40]
	cmp	r6, #0
	ble.n	.L_0815e5cc
	movs	r0, #0
	str	r0, [sp, #40]
.L_0815e5cc:
	ldr	r1, [sp, #40]
	movs	r3, #128
	negs	r3, r3
	cmp	r1, r3
	bge.n	.L_0815e5d8
	str	r3, [sp, #40]
.L_0815e5d8:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #40]
	ldr	r3, [r2, #0]
	add	r6, sp, #40
	adds	r3, r3, r5
	str	r3, [r2, #0]
	ldrh	r6, [r6, #0]
	ldr	r2, [pc, #656]
	movs	r3, #80
	strh	r3, [r2, #6]
	strh	r6, [r2, #4]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #52]
	movs	r7, #255
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r5, [sp, #52]
	ldr	r6, [r0, #0]
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	ldr	r5, [sp, #48]
	mov	r8, r0
	movs	r0, #0
	mov	sl, r0
.L_0815e61a:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, r8
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_0815e656
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_0815e656:
	ldr	r3, [r5, #12]
	movs	r1, #1
	negs	r3, r3
	str	r3, [r5, #12]
	mov	r3, sl
	add	sl, r1
	adds	r3, #16
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0815e61a
	ldr	r6, [sp, #32]
	movs	r5, #88
	adds	r6, #12
	str	r6, [sp, #8]
	movs	r3, #0
	add	r5, sp
	mov	r9, r3
	mov	fp, r5
.L_0815e67e:
	mov	r0, r9
	cmp	r0, #5
	bne.n	.L_0815e6a4
	ldr	r1, [sp, #24]
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r0, [r3, #0]
	bl	sub_08118058
	cmp	r0, #0
	beq.n	.L_0815e69e
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0815e6a4
.L_0815e69e:
	movs	r0, #133
	bl	sub_081180e8
.L_0815e6a4:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L_0815e704
	ldr	r5, [sp, #28]
	cmp	r5, #7
	bne.n	.L_0815e6be
	ldr	r1, [sp, #52]
	movs	r6, #36
	ldrsh	r0, [r1, r6]
	movs	r1, #4
	bl	sub_08118088
	b.n	.L_0815e704
.L_0815e6be:
	ldr	r2, [sp, #20]
	cmp	r2, #1
	bhi.n	.L_0815e6d2
	ldr	r5, [sp, #52]
	movs	r1, #1
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
	b.n	.L_0815e704
.L_0815e6d2:
	ldr	r6, [sp, #28]
	cmp	r6, #8
	bne.n	.L_0815e6f8
	ldr	r2, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #10
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r3, #150
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	movs	r1, #1
	lsls	r2, r2, #9
	lsls	r3, r3, #11
	bl	.L_0815f000
	b.n	.L_0815e704
.L_0815e6f8:
	ldr	r5, [sp, #52]
	movs	r1, #0
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118088
.L_0815e704:
	ldr	r6, [sp, #52]
	add	r2, sp, #88
	ldr	r0, [r6, #8]
	adds	r1, r2, #0
	bl	sub_0815e20c
	ldr	r3, [sp, #92]
	ldr	r5, [sp, #28]
	adds	r3, #16
	str	r3, [sp, #92]
	cmp	r5, #6
	bne.n	.L_0815e722
	adds	r3, #16
	mov	r6, fp
	str	r3, [r6, #4]
.L_0815e722:
	ldr	r0, [sp, #28]
	cmp	r0, #4
	bne.n	.L_0815e79c
	mov	r1, r9
	cmp	r1, #11
	ble.n	.L_0815e730
	b.n	sub_0815e8ca
.L_0815e730:
	ldr	r2, [sp, #52]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0815e76a
	lsrs	r2, r1, #31
	add	r2, r9
	asrs	r2, r2, #1
	movs	r3, #5
	subs	r3, r3, r2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #48]
	mov	r6, fp
	ldr	r2, [r6, #0]
	ldr	r0, [sp, #40]
	lsls	r1, r1, #8
	adds	r1, r3, r1
	movs	r5, #224
	ldr	r3, [r6, #4]
	lsls	r5, r5, #3
	adds	r2, r2, r0
	adds	r1, r1, r5
	movs	r0, #16
	movs	r5, #48
	str	r0, [sp, #4]
	subs	r2, #48
	subs	r3, #8
	str	r5, [sp, #0]
	b.n	.L_0815e86c
.L_0815e76a:
	mov	r6, r9
	lsrs	r2, r6, #31
	add	r2, r9
	asrs	r2, r2, #1
	movs	r3, #5
	subs	r3, r3, r2
	lsls	r1, r3, #1
	ldr	r0, [sp, #48]
	adds	r1, r1, r3
	lsls	r1, r1, #8
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #3
	mov	r3, fp
	adds	r1, r1, r2
	ldr	r5, [sp, #40]
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r0, #16
	movs	r6, #48
	str	r0, [sp, #4]
	adds	r2, r2, r5
	subs	r3, #8
	str	r6, [sp, #0]
	b.n	.L_0815e86c
.L_0815e79c:
	ldr	r0, [sp, #28]
	cmp	r0, #2
	bls.n	.L_0815e7aa
	cmp	r0, #5
	beq.n	.L_0815e7aa
	cmp	r0, #6
	bne.n	.L_0815e81a
.L_0815e7aa:
	mov	r1, r9
	cmp	r1, #11
	ble.n	.L_0815e7b2
	b.n	sub_0815e8ca
.L_0815e7b2:
	ldr	r2, [sp, #52]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0815e7ea
	lsrs	r3, r1, #31
	add	r3, r9
	asrs	r3, r3, #1
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	subs	r1, r1, r3
	ldr	r3, [sp, #48]
	lsls	r1, r1, #7
	ldr	r2, [sp, #88]
	ldr	r6, [sp, #40]
	adds	r1, r3, r1
	ldr	r3, [sp, #92]
	movs	r0, #48
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r2, r2, r6
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	adds	r1, r1, r5
	subs	r2, #48
	subs	r3, #40
	b.n	.L_0815e86c
.L_0815e7ea:
	mov	r1, r9
	lsrs	r3, r1, #31
	add	r3, r9
	asrs	r3, r3, #1
	lsls	r1, r3, #3
	subs	r1, r1, r3
	ldr	r2, [sp, #48]
	lsls	r1, r1, #2
	subs	r1, r1, r3
	lsls	r1, r1, #7
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r2, [sp, #88]
	ldr	r5, [sp, #40]
	ldr	r3, [sp, #92]
	movs	r0, #72
	movs	r6, #48
	str	r0, [sp, #4]
	adds	r2, r2, r5
	subs	r3, #40
	str	r6, [sp, #0]
	b.n	.L_0815e86c
.L_0815e81a:
	ldr	r0, [sp, #28]
	cmp	r0, #3
	bne.n	sub_0815e8ca
	mov	r1, r9
	cmp	r1, #17
	bgt.n	sub_0815e8ca
	mov	r0, r9
	movs	r1, #3
	bl	sub_08002054
	ldr	r2, [sp, #52]
	adds	r6, r0, #0
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0815e88c
	ldr	r2, [pc, #64]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #48]
	mov	r0, fp
	adds	r1, r3, r1
	ldr	r3, [pc, #56]
	ldr	r2, [r0, #0]
	ldrb	r3, [r3, r6]
	movs	r5, #224
	adds	r2, r2, r3
	ldr	r3, [sp, #40]
	lsls	r5, r5, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #44]
	adds	r1, r1, r5
	ldrb	r4, [r3, r6]
	mov	r5, fp
	ldr	r3, [r5, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	ldr	r0, [pc, #36]
	subs	r2, #58
	ldrb	r0, [r0, r6]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
.L_0815e86c:
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe029
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0x0819886c
	.4byte 0x08198878
	.4byte 0x08198866
	.2byte 0x8860
	.2byte 0x0819
.L_0815e88c:
	ldr	r2, [pc, #320]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r0, [sp, #48]
	ldr	r3, [pc, #316]
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #3
	mov	r5, fp
	ldrb	r3, [r3, r6]
	adds	r1, r1, r2
	ldr	r2, [r5, #0]
	ldr	r0, [sp, #40]
	subs	r2, r2, r3
	ldr	r3, [pc, #300]
	adds	r2, r2, r0
	ldrb	r5, [r3, r6]
	ldr	r3, [pc, #300]
	subs	r2, r2, r5
	ldrb	r4, [r3, r6]
	mov	r6, fp
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	adds	r2, #58
	str	r5, [sp, #0]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x464d
	subs	r5, #4
	cmp	r5, #11
	bhi.n	sub_0815e8fc
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r1, r3, #4
	subs	r1, r1, r3
	ldr	r3, [sp, #16]
	ldr	r0, [pc, #256]
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	lsls	r1, r1, #7
	adds	r1, r1, r0
	movs	r6, #48
	movs	r0, #40
	str	r0, [sp, #0]
	subs	r2, #16
	subs	r3, #24
	str	r6, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xf6b6
	.2byte 0xfa72
	.2byte 0x9808
	ldr	r1, [sp, #8]
	bl	sub_080156e8
	cmp	r5, #27
	bhi.n	.L_0815e986
	movs	r1, #64
	movs	r0, #0
	add	r1, sp
	mov	sl, r0
	mov	r8, r1
.L_0815e916:
	mov	r2, sl
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r6, r3, #1
	ldr	r5, [sp, #48]
	lsls	r3, r6, #3
	subs	r3, r3, r6
	lsls	r3, r3, #2
	adds	r7, r5, r3
	ldr	r5, [r7, #24]
	cmp	r5, #0
	ble.n	.L_0815e97c
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_08015778
	mov	r0, r8
	ldr	r2, [r0, #0]
	ldr	r1, [sp, #40]
	asrs	r5, r5, #3
	adds	r5, #2
	adds	r2, r2, r1
	ldr	r1, [pc, #160]
	lsls	r4, r5, #1
	str	r2, [r0, #0]
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #36]
	movs	r0, #1
	ands	r0, r6
	mov	r6, r8
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	subs	r2, r2, r5
	subs	r3, r3, r5
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r5, [sp, #12]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #60
	ldr	r2, [pc, #116]
	bl	sub_08138058
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_0815e97c:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	cmp	r0, #64
	bne.n	.L_0815e916
.L_0815e986:
	ldr	r1, [sp, #48]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	movs	r5, #1
	adds	r2, r1, r3
	add	r9, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	mov	r6, r9
	bl	sub_08013560
	cmp	r6, #32
	beq.n	.L_0815e9a6
	b.n	.L_0815e67e
.L_0815e9a6:
	ldr	r0, [pc, #68]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r3, [pc, #52]
	mov	r0, r9
	strh	r0, [r3, #6]
	bl	sub_08143d04
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0819886c
	.4byte 0x08198878
	.4byte 0x08198860
	.4byte 0x08198866
	.4byte 0x02010000
	.4byte 0x08197424
	.4byte 0xfffffc00
	.4byte 0x08143001
	.2byte 0x1120
	.2byte 0x0300
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
	sub	sp, #116
	str	r1, [sp, #56]
	mov	fp, r0
	ldr	r2, [r3, #96]
	str	r2, [sp, #52]
	ldr	r5, [r3, #100]
	str	r5, [sp, #44]
	movs	r5, #1
	ldr	r3, [r3, #48]
	str	r3, [sp, #40]
	ldr	r3, [r0, #0]
	cmp	r3, #199
	bgt.n	.L_0815ea24
	movs	r5, #0
.L_0815ea24:
	mov	r1, fp
	movs	r2, #130
	ldr	r0, [r1, #8]
	ldr	r1, [r1, #12]
	bl	sub_08118030
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08143d80
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	mov	r2, fp
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0815ea64
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0815ea74
	movs	r0, r0
	.2byte 0x1f80
	.2byte 0x0000
.L_0815ea64:
	movs	r1, #7
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #23
	bl	sub_081963ec
.L_0815ea74:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #104]
	mov	r1, sp
	str	r2, [sp, #60]
	adds	r1, #60
	adds	r3, #188
	ldr	r3, [r3, #0]
	str	r1, [sp, #16]
	mov	r2, fp
	str	r3, [r1, #4]
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #12]
	movs	r2, #130
	bl	sub_08118030
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #56]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [pc, #704]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	mov	r3, fp
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	movs	r2, #130
	bl	sub_08118030
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #680]
	ldr	r1, [pc, #680]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	mov	r1, fp
	ldr	r0, [r1, #8]
	cmp	r0, #7
	ble.n	.L_0815eaf0
	ldr	r0, [pc, #668]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, fp
	ldr	r0, [r2, #8]
.L_0815eaf0:
	mov	r3, fp
	ldr	r1, [r3, #12]
	movs	r2, #130
	bl	sub_08118030
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #0
	ldr	r1, [sp, #44]
	ldr	r0, [pc, #620]
	movs	r2, #0
	bl	sub_08157cf4
	mov	r1, fp
	ldr	r0, [r1, #8]
	movs	r2, #130
	ldr	r1, [r1, #12]
	bl	sub_08118030
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #56]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [sp, #56]
	adds	r1, #4
	adds	r2, r3, r1
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #576]
	lsls	r1, r1, #4
	bl	sub_080145a8
	cmp	r5, #1
	beq.n	.L_0815eb44
	b.n	.L_0815ed04
.L_0815eb44:
	mov	r2, fp
	ldr	r0, [r2, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r6, #255
	ldr	r5, [sp, #56]
	movs	r3, #0
	lsls	r6, r6, #8
	mov	r8, r0
	mov	sl, r3
	movs	r7, #0
	adds	r6, #255
.L_0815eb5e:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r7, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r1, #1
	add	sl, r1
	ands	r0, r6
	mov	r2, sl
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0815eb5e
	mov	r0, r8
	movs	r1, #0
	bl	sub_08020098
	mov	r3, r8
	ldr	r3, [r3, #36]
	mov	r5, r8
	str	r3, [sp, #36]
	mov	r1, r8
	ldr	r5, [r5, #40]
	mov	r2, r8
	str	r5, [sp, #32]
	mov	r3, r8
	ldr	r1, [r1, #44]
	movs	r5, #0
	str	r1, [sp, #28]
	mov	r1, r8
	ldr	r2, [r2, #72]
	mov	r9, r5
	str	r2, [sp, #20]
	mov	r2, fp
	ldr	r3, [r3, #52]
	str	r3, [sp, #24]
	str	r5, [r1, #36]
	str	r5, [r1, #40]
	str	r5, [r1, #44]
	str	r5, [r1, #52]
	str	r5, [r1, #72]
	mov	r3, sp
	adds	r3, #104
	ldr	r0, [r2, #8]
	adds	r1, r3, #0
	str	r3, [sp, #12]
	bl	sub_0815e20c
	ldr	r1, [sp, #12]
	mov	r2, sl
	ldr	r3, [r1, #0]
	ldr	r0, [pc, #412]
	subs	r2, r2, r3
	str	r2, [sp, #48]
	add	r3, sp, #48
	ldr	r2, [pc, #408]
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #4]
	movs	r3, #80
	strh	r3, [r2, #6]
	ldr	r1, [sp, #56]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #180
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	adds	r3, r1, r2
	movs	r1, #144
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r0, #212
	bl	sub_081c0010
.L_0815ec16:
	mov	r3, fp
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	movs	r2, #130
	bl	sub_08118030
	ldr	r6, [sp, #56]
	movs	r5, #0
	mov	sl, r5
.L_0815ec28:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	blt.n	.L_0815ecb4
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0815ec36
	adds	r3, #3
.L_0815ec36:
	asrs	r3, r3, #2
	cmp	r9, r3
	blt.n	.L_0815ecb4
	mov	r1, sl
	movs	r3, #1
	ands	r3, r1
	adds	r7, r3, #5
	bl	sub_08014de4
	ldr	r0, [r6, #20]
	bl	sub_080150e4
	ldr	r0, [r6, #12]
	bl	sub_08015024
	ldr	r0, [r6, #16]
	bl	sub_08015068
	add	r5, sp, #80
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	adds	r3, #64
	str	r3, [r5, #0]
	ldr	r2, [sp, #108]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	adds	r3, #24
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	movs	r2, #60
	negs	r2, r2
	cmp	r3, r2
	bge.n	.L_0815ec82
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_0815ec82:
	cmp	r3, #60
	ble.n	.L_0815ec8a
	movs	r3, #60
	str	r3, [r5, #8]
.L_0815ec8a:
	ldr	r2, [pc, #248]
	lsls	r0, r7, #1
	adds	r3, #60
	str	r3, [r5, #8]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	ldr	r3, [r5, #4]
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	subs	r3, r3, r7
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, r2, r7
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6833
	subs	r3, #4
	str	r3, [r6, #0]
.L_0815ecb4:
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	adds	r6, #28
	cmp	r5, #64
	bne.n	.L_0815ec28
	ldr	r1, [sp, #56]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r5, #1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	add	r9, r5
	bl	sub_08013560
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_0815ec16
	ldr	r0, [pc, #156]
	bl	sub_08014644
	movs	r1, #16
	mov	r0, r8
	bl	sub_08020098
	ldr	r2, [sp, #36]
	mov	r3, r8
	str	r2, [r3, #36]
	ldr	r5, [sp, #32]
	str	r5, [r3, #40]
	ldr	r1, [sp, #28]
	str	r1, [r3, #44]
	ldr	r2, [sp, #24]
	str	r2, [r3, #52]
	ldr	r5, [sp, #20]
	str	r5, [r3, #72]
	b.n	.L_0815ed0a
.L_0815ed04:
	mov	r1, sp
	adds	r1, #104
	str	r1, [sp, #12]
.L_0815ed0a:
	movs	r1, #240
	ldr	r5, [pc, #120]
	ldr	r0, [sp, #52]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	lsls	r1, r1, #6
	ldr	r0, [pc, #112]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b0e
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #56]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	mov	r1, fp
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	add	r5, sp, #92
	adds	r1, r5, #0
	bl	sub_0815e20c
	mov	r2, fp
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0815ed90
	ldr	r2, [r5, #0]
	movs	r3, #32
	b.n	.L_0815ed94
	.4byte 0x00001f81
	.4byte 0x0000010c
	.4byte 0x0000010b
	.4byte 0x02010000
	.4byte 0x00000151
	.4byte 0x00000137
	.4byte 0x08143001
	.4byte 0x08143489
	.4byte 0x03001120
	.4byte 0x08197424
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
.L_0815ed90:
	ldr	r2, [r5, #0]
	movs	r3, #96
.L_0815ed94:
	subs	r3, r3, r2
	str	r3, [sp, #48]
	ldr	r3, [sp, #48]
	cmp	r3, #0
	ble.n	.L_0815eda2
	movs	r1, #0
	str	r1, [sp, #48]
.L_0815eda2:
	ldr	r2, [sp, #48]
	movs	r3, #128
	negs	r3, r3
	cmp	r2, r3
	bge.n	.L_0815edae
	str	r3, [sp, #48]
.L_0815edae:
	ldr	r1, [sp, #48]
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #564]
	adds	r3, r3, r1
	str	r3, [r5, #0]
	add	r3, sp, #48
	ldrh	r3, [r3, #0]
	mov	r1, fp
	strh	r3, [r2, #4]
	movs	r3, #80
	strh	r3, [r2, #6]
	movs	r5, #36
	ldrsh	r0, [r1, r5]
	bl	sub_08118098
	mov	r3, fp
	ldr	r6, [r0, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118070
	movs	r5, #0
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	mov	sl, r5
	ldr	r5, [sp, #56]
	asrs	r0, r0, #1
	mov	r8, r0
	movs	r7, #255
.L_0815ede8:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, r8
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_0815ee22
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_0815ee22:
	movs	r1, #1
	mov	r3, sl
	add	sl, r1
	adds	r3, #16
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0815ede8
	ldr	r5, [sp, #40]
	movs	r3, #0
	adds	r5, #12
	str	r5, [sp, #8]
	mov	r9, r3
.L_0815ee3e:
	mov	r1, r9
	cmp	r1, #5
	bne.n	.L_0815ee4a
	movs	r0, #134
	bl	sub_081180e8
.L_0815ee4a:
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L_0815ee5c
	mov	r5, fp
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	bl	sub_08118088
.L_0815ee5c:
	mov	r1, fp
	ldr	r0, [r1, #8]
	ldr	r1, [sp, #12]
	bl	sub_0815e20c
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #4]
	adds	r3, #16
	str	r3, [r2, #4]
	mov	r3, r9
	cmp	r3, #1
	bgt.n	.L_0815ee8a
	ldr	r5, [sp, #56]
	movs	r2, #224
	movs	r3, #120
	lsls	r2, r2, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r5, r2
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	movs	r2, #0
	b.n	.L_0815eeb6
.L_0815ee8a:
	mov	r3, r9
	cmp	r3, #3
	bgt.n	.L_0815eea0
	movs	r3, #128
	movs	r2, #4
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #332]
	b.n	.L_0815eeb4
.L_0815eea0:
	mov	r5, r9
	cmp	r5, #5
	bgt.n	.L_0815eebe
	movs	r3, #128
	movs	r2, #4
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #316]
.L_0815eeb4:
	negs	r2, r2
.L_0815eeb6:
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00f
.L_0815eebe:
	mov	r1, r9
	cmp	r1, #7
	bgt.n	sub_0815eede
	movs	r3, #120
	ldr	r2, [sp, #56]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #253
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	movs	r2, #0
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xf6b5
	.2byte 0xff81
	.2byte 0x980a
	ldr	r1, [sp, #8]
	bl	sub_080156e8
	mov	r3, r9
	subs	r3, #4
	cmp	r3, #27
	bhi.n	.L_0815ef70
	movs	r1, #68
	movs	r5, #0
	add	r1, sp
	mov	sl, r5
	mov	r8, r1
.L_0815eefc:
	mov	r2, sl
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r5, r3, #1
	lsls	r3, r5, #3
	ldr	r1, [sp, #56]
	subs	r3, r3, r5
	lsls	r3, r3, #2
	adds	r7, r1, r3
	ldr	r6, [r7, #24]
	cmp	r6, #0
	ble.n	.L_0815ef66
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_0815e1ec
	mov	r3, r8
	ldr	r2, [r3, #0]
	ldr	r1, [sp, #48]
	asrs	r6, r6, #3
	adds	r2, r2, r1
	str	r2, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r0, #1
	adds	r6, #2
	ands	r0, r5
	ldr	r5, [pc, #192]
	lsls	r4, r6, #1
	adds	r3, #16
	mov	r1, r8
	str	r3, [r1, #4]
	subs	r1, r4, #2
	ldrh	r1, [r5, r1]
	ldr	r5, [sp, #44]
	str	r4, [sp, #0]
	adds	r1, r5, r1
	str	r4, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	subs	r3, r3, r6
	ldr	r4, [r0, r5]
	subs	r2, r2, r6
	ldr	r0, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c38
	movs	r1, #60
	ldr	r2, [pc, #156]
	bl	sub_08138058
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_0815ef66:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #64
	bne.n	.L_0815eefc
.L_0815ef70:
	ldr	r3, [sp, #56]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #32
	beq.n	.L_0815ef90
	b.n	.L_0815ee3e
.L_0815ef90:
	ldr	r0, [pc, #104]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r3, [pc, #68]
	mov	r5, r9
	strh	r5, [r3, #6]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r2, #206
	movs	r1, #0
	lsls	r2, r2, #3
	mov	r9, r1
	adds	r5, r3, r2
	movs	r6, #6
.L_0815efba:
	mov	r3, r9
	subs	r1, r6, r3
	ldrh	r0, [r5, #0]
	bl	sub_08118048
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #7
	bne.n	.L_0815efba
	bl	sub_08143d04
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0x02010000
	.4byte 0x02014000
	.4byte 0x08197424
	.4byte 0xfffffc00
	.2byte 0x3001
	.2byte 0x0814
.L_0815f000:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r2
	mov	sl, r3
	adds	r5, r0, #0
	mov	r9, r1
	bl	sub_08118098
	adds	r7, r0, #0
	adds	r0, r5, #0
	ldr	r6, [r7, #0]
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	movs	r3, #102
	adds	r3, #255
	cmp	r0, r3
	beq.n	.L_0815f07a
	adds	r2, #39
	cmp	r0, r2
	beq.n	.L_0815f07a
	adds	r3, #16
	cmp	r0, r3
	beq.n	.L_0815f07a
	adds	r2, #7
	cmp	r0, r2
	beq.n	.L_0815f07a
	mov	r3, r8
	str	r3, [r6, #52]
	mov	r2, sl
	mov	r3, r9
	str	r2, [r6, #48]
	cmp	r3, #0
	bne.n	.L_0815f056
	ldr	r3, [r6, #40]
	cmp	r3, #0
	beq.n	.L_0815f05a
.L_0815f056:
	ldr	r3, [sp, #28]
	str	r3, [r6, #40]
.L_0815f05a:
	adds	r0, r6, #0
	bl	sub_08020138
	ldr	r2, [r7, #12]
	ldr	r3, [sp, #32]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002054
	ldr	r3, [r7, #16]
	adds	r1, r0, #0
	movs	r2, #0
	adds	r0, r6, #0
	bl	sub_08020148
.L_0815f07a:
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, r6, #0
	str	r3, [r6, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r6, #68]
	adds	r0, r6, #0
	strb	r3, [r2, #0]
	movs	r1, #5
	bl	sub_08020090
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
