.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_080156e8, 0x080156e8
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813e002, 0x0813e002
	.set sub_0813e338, 0x0813e338
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0813d098
	.thumb_func
Func_0813d098:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #164
	str	r1, [sp, #80]
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #76]
	ldr	r1, [r3, #96]
	str	r1, [sp, #72]
	ldr	r2, [r3, #100]
	str	r2, [sp, #60]
	ldr	r3, [r3, #48]
	str	r3, [sp, #56]
	ldr	r3, [sp, #80]
	cmp	r3, #8
	bne.n	.L_0813d0ce
	movs	r0, #0
	bl	sub_081435e0
	b.n	.L_0813d0d4
.L_0813d0ce:
	movs	r0, #1
	bl	sub_081435e0
.L_0813d0d4:
	movs	r2, #128
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r5, [sp, #80]
	cmp	r5, #8
	bne.n	.L_0813d0fe
	ldr	r6, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	movs	r1, #238
	adds	r0, #180
	lsls	r1, r1, #7
	adds	r2, r6, r0
	movs	r3, #24
	adds	r1, #184
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #0
	str	r3, [r2, #0]
.L_0813d0fe:
	ldr	r1, [sp, #60]
	ldr	r0, [pc, #16]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #76]
	movs	r3, #224
	b.n	.L_0813d118
	.4byte 0x00001010
	.2byte 0x0134
	.2byte 0x0000
.L_0813d118:
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #748]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [sp, #80]
	ldr	r6, [sp, #80]
	lsls	r5, r5, #3
	ldr	r2, [pc, #736]
	str	r5, [sp, #28]
	str	r5, [sp, #52]
	subs	r3, r5, r6
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_0813d14e
	ldr	r2, [sp, #76]
	movs	r3, #156
	lsls	r3, r3, #5
	adds	r1, r2, r3
	ldr	r0, [pc, #720]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0813d160
.L_0813d14e:
	ldr	r5, [sp, #76]
	movs	r6, #156
	lsls	r6, r6, #5
	ldr	r0, [pc, #704]
	adds	r1, r5, r6
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0813d160:
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #80]
	ldr	r2, [pc, #680]
	subs	r3, r0, r1
	adds	r3, #3
	ldrb	r3, [r2, r3]
	cmp	r3, #6
	bhi.n	.L_0813d1a8
	ldr	r2, [pc, #680]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0813d194
	.4byte 0x0813d198
	.4byte 0x0813d19c
	.4byte 0x0813d1a0
	.4byte 0x0813d1a4
	.4byte 0x0813d1a8
	.2byte 0xd1ac
	.2byte 0x0813
	ldr	r0, [pc, #648]
	b.n	.L_0813d1ae
	ldr	r0, [pc, #648]
	b.n	.L_0813d1ae
	ldr	r0, [pc, #648]
	b.n	.L_0813d1ae
	ldr	r0, [pc, #648]
	b.n	.L_0813d1ae
	ldr	r0, [pc, #648]
	b.n	.L_0813d1ae
.L_0813d1a8:
	ldr	r0, [pc, #648]
	b.n	.L_0813d1ae
	ldr	r0, [pc, #648]
.L_0813d1ae:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #644]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d0d
	ldr	r6, [sp, #80]
	ldr	r2, [pc, #584]
	subs	r3, r5, r6
	adds	r3, #2
	ldrb	r2, [r2, r3]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d1e0
	ldr	r2, [sp, #76]
	movs	r3, #228
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #608]
	b.n	.L_0813d1f2
.L_0813d1e0:
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d1fc
	ldr	r5, [sp, #76]
	movs	r6, #228
	lsls	r6, r6, #6
	ldr	r0, [pc, #596]
	adds	r1, r5, r6
.L_0813d1f2:
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0813d20e
.L_0813d1fc:
	ldr	r2, [sp, #76]
	movs	r3, #228
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #576]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
.L_0813d20e:
	mov	r6, sp
	movs	r5, #0
	adds	r6, #152
	movs	r0, #36
	str	r5, [sp, #64]
	str	r6, [sp, #32]
	str	r0, [sp, #20]
	str	r5, [sp, #16]
.L_0813d21e:
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #84]
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [sp, #20]
	ldr	r1, [sp, #84]
	mov	fp, r0
	ldrsh	r0, [r5, r1]
	ldr	r1, [sp, #32]
	bl	sub_0815e21c
	ldr	r5, [sp, #76]
	movs	r2, #0
	mov	sl, r2
.L_0813d23e:
	ldr	r6, [sp, #32]
	movs	r0, #0
	ldr	r3, [r6, #0]
	str	r0, [r5, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r1, #255
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r2, #255
	ands	r0, r2
	subs	r0, #128
	movs	r6, #1
	lsls	r0, r0, #9
	movs	r3, #0
	add	sl, r6
	str	r0, [r5, #16]
	str	r3, [r5, #20]
	mov	r0, sl
	subs	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_0813d23e
	ldr	r2, [sp, #16]
	ldr	r3, [pc, #448]
	movs	r1, #0
	mov	sl, r1
	movs	r6, #255
	adds	r5, r2, r3
.L_0813d292:
	mov	r0, fp
	ldr	r3, [r0, #8]
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	subs	r0, #128
	movs	r3, #1
	add	sl, r1
	lsls	r0, r0, #11
	negs	r3, r3
	mov	r2, sl
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_0813d292
	ldr	r6, [sp, #80]
	ldr	r5, [sp, #52]
	movs	r3, #0
	subs	r5, r5, r6
	ldr	r6, [pc, #364]
	movs	r0, #255
	mov	sl, r3
	mov	r9, r5
	mov	r8, r0
.L_0813d2ea:
	mov	r1, fp
	ldr	r3, [r1, #8]
	ldr	r5, [pc, #288]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r6, #4]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	mov	r3, r9
	adds	r3, #2
	ldrb	r2, [r5, r3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d354
	bl	sub_08014878
	adds	r7, r0, #0
	bl	sub_08014878
	adds	r5, r0, #0
	mov	r0, r8
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	lsls	r2, r3, #11
	str	r2, [r6, #12]
	ldr	r1, [sp, #84]
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_0813d336
	negs	r3, r2
	str	r3, [r6, #12]
.L_0813d336:
	adds	r0, r7, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r6, #16]
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r6, #20]
	b.n	.L_0813d3b4
.L_0813d354:
	ldr	r2, [pc, #184]
	mov	r5, r9
	ldrb	r3, [r2, r5]
	cmp	r3, #1
	bne.n	.L_0813d384
	bl	sub_08014878
	mov	r1, r8
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08014878
	mov	r3, r8
	ands	r0, r3
	b.n	.L_0813d3a2
.L_0813d384:
	bl	sub_08014878
	mov	r5, r8
	ands	r0, r5
	lsls	r0, r0, #11
	str	r0, [r6, #12]
	bl	sub_08014878
	ands	r0, r5
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #16]
	bl	sub_08014878
	ands	r0, r5
.L_0813d3a2:
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r6, #20]
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L_0813d3b4
	ldr	r3, [r6, #12]
	negs	r3, r3
	str	r3, [r6, #12]
.L_0813d3b4:
	movs	r3, #1
	movs	r0, #1
	movs	r1, #128
	negs	r3, r3
	add	sl, r0
	lsls	r1, r1, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	sl, r1
	bne.n	.L_0813d2ea
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldr	r6, [sp, #64]
	movs	r5, #224
	lsls	r5, r5, #4
	adds	r2, #2
	adds	r3, r3, r5
	adds	r6, #1
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r6, [sp, #64]
	cmp	r6, #1
	beq.n	.L_0813d3e4
	b.n	.L_0813d21e
.L_0813d3e4:
	ldr	r0, [sp, #76]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r5, [sp, #52]
	ldr	r6, [sp, #80]
	ldr	r2, [pc, #24]
	subs	r3, r5, r6
	adds	r3, #2
	ldrb	r2, [r2, r3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d454
	adds	r1, #4
	adds	r2, r0, r1
	movs	r3, #50
	b.n	.L_0813d460
	.4byte 0x00000192
	.4byte 0x0819754f
	.4byte 0x00000189
	.4byte 0x0000018a
	.4byte 0x0813d178
	.4byte 0x00000190
	.4byte 0x00000163
	.4byte 0x00000164
	.4byte 0x00000178
	.4byte 0x00000150
	.4byte 0x00000188
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x0000013e
	.4byte 0x000000d5
	.4byte 0x00000161
	.4byte 0x02010000
	.2byte 0x3800
	.2byte 0x0201
.L_0813d454:
	ldr	r3, [sp, #76]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #132
	adds	r2, r3, r5
	movs	r3, #75
.L_0813d460:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #840]
	bl	sub_080145a8
	ldr	r1, [sp, #84]
	mov	r2, sp
	adds	r2, #140
	movs	r6, #36
	ldrsh	r0, [r1, r6]
	adds	r1, r2, #0
	str	r2, [sp, #36]
	bl	sub_0815e20c
	ldr	r5, [sp, #84]
	mov	r6, sp
	adds	r6, #128
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	adds	r1, r6, #0
	str	r6, [sp, #48]
	bl	sub_0815e21c
	ldr	r0, [sp, #36]
	ldr	r3, [r6, #4]
	ldr	r1, [r0, #4]
	ldr	r4, [pc, #796]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #4]
	ldr	r2, [sp, #52]
	ldr	r5, [sp, #80]
	movs	r1, #0
	str	r1, [sp, #68]
	subs	r3, r2, r5
	adds	r3, #6
	adds	r6, r4, #0
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_0813d4bc
	bl	.L_0813e0d4
.L_0813d4bc:
	ldr	r0, [sp, #56]
	mov	r1, sp
	adds	r0, #12
	adds	r1, #96
	str	r0, [sp, #24]
	str	r1, [sp, #40]
.L_0813d4c8:
	ldr	r3, [sp, #80]
	ldr	r5, [pc, #744]
	subs	r2, r2, r3
	adds	r3, r2, #1
	ldrb	r1, [r5, r3]
	movs	r6, #0
	adds	r3, r2, #4
	ldrb	r5, [r5, r3]
	str	r6, [sp, #44]
	ldr	r6, [pc, #728]
	muls	r1, r5
	ldrb	r3, [r6, r2]
	lsls	r0, r1, #2
	cmp	r3, #0
	beq.n	.L_0813d4e8
	b.n	.L_0813d8b6
.L_0813d4e8:
	lsls	r3, r1, #1
	ldr	r0, [sp, #68]
	adds	r3, r3, r1
	lsls	r3, r3, #1
	cmp	r0, r3
	blt.n	.L_0813d4f6
	b.n	.L_0813d8a6
.L_0813d4f6:
	adds	r1, r5, #0
	bl	sub_08002054
	movs	r1, #6
	bl	sub_08002064
	lsls	r1, r5, #1
	adds	r1, r1, r5
	adds	r6, r0, #0
	lsls	r1, r1, #1
	ldr	r0, [sp, #68]
	bl	sub_08002064
	cmp	r0, #0
	bne.n	.L_0813d55c
	ldr	r3, [sp, #80]
	subs	r3, #12
	cmp	r3, #1
	bhi.n	.L_0813d55c
	ldr	r2, [sp, #84]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	ldr	r1, [sp, #36]
	bl	sub_0815e20c
	ldr	r5, [sp, #84]
	ldr	r1, [sp, #48]
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_0815e21c
	ldr	r2, [sp, #36]
	ldr	r0, [sp, #48]
	ldr	r1, [r2, #4]
	ldr	r3, [r0, #4]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #36]
	str	r1, [r3, #4]
	bl	sub_08014878
	ldr	r5, [sp, #36]
	movs	r2, #15
	ldr	r3, [r5, #4]
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #8
	str	r3, [r5, #4]
.L_0813d55c:
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_0813d5b4
	movs	r1, #23
	movs	r0, #104
	bl	sub_081963ec
	ldr	r2, [pc, #584]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r5, [sp, #36]
	ldr	r2, [sp, #76]
	movs	r3, #224
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #564]
	asrs	r2, r2, #1
	ldrb	r3, [r3, r6]
	lsrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #560]
	ldrb	r5, [r3, r6]
	ldr	r3, [pc, #560]
	subs	r2, r2, r5
	ldrb	r4, [r3, r6]
	ldr	r6, [sp, #36]
	lsrs	r0, r4, #1
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	subs	r3, r3, r0
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r4, [r0, #104]
	adds	r2, #8
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe024
.L_0813d5b4:
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r2, [pc, #504]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r5, [sp, #36]
	ldr	r2, [sp, #76]
	movs	r3, #224
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [pc, #484]
	asrs	r2, r2, #1
	ldrb	r3, [r3, r6]
	lsrs	r3, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #484]
	subs	r2, #8
	ldrb	r4, [r3, r6]
	ldr	r3, [r5, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	ldr	r0, [pc, #468]
	ldrb	r0, [r0, r6]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r4, [r6, #104]
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #80]
	ldr	r4, [pc, #424]
	subs	r6, r0, r1
	adds	r3, r6, #4
	ldrb	r5, [r4, r3]
	ldr	r0, [sp, #68]
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #1
	str	r4, [sp, #8]
	bl	sub_08002064
	lsls	r5, r5, #2
	subs	r5, #4
	ldr	r4, [sp, #8]
	cmp	r0, r5
	bne.n	.L_0813d63c
	adds	r3, r6, #2
	ldrb	r2, [r4, r3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d63c
	movs	r0, #212
	bl	sub_081c0010
	ldr	r4, [sp, #8]
.L_0813d63c:
	ldr	r3, [sp, #80]
	ldr	r2, [sp, #28]
	ldr	r0, [sp, #68]
	subs	r6, r2, r3
	adds	r3, r6, #4
	ldrb	r5, [r4, r3]
	str	r4, [sp, #8]
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #1
	bl	sub_08002064
	lsls	r5, r5, #2
	ldr	r4, [sp, #8]
	cmp	r0, r5
	beq.n	.L_0813d65e
	b.n	.L_0813d8a6
.L_0813d65e:
	ldr	r5, [sp, #80]
	cmp	r5, #12
	bne.n	.L_0813d67a
	movs	r3, #88
	ldrb	r1, [r4, r3]
	ldr	r0, [sp, #68]
	bl	sub_08002054
	movs	r1, #6
	bl	sub_08002054
	cmp	r0, #2
	bgt.n	.L_0813d68e
	b.n	.L_0813d686
.L_0813d67a:
	ldr	r0, [sp, #80]
	cmp	r0, #7
	bne.n	.L_0813d696
	ldr	r1, [sp, #68]
	cmp	r1, #8
	bgt.n	.L_0813d68e
.L_0813d686:
	movs	r0, #133
	bl	sub_081c0010
	b.n	.L_0813d6e8
.L_0813d68e:
	movs	r0, #133
	bl	sub_081180e8
	b.n	.L_0813d6e8
.L_0813d696:
	ldr	r2, [sp, #80]
	cmp	r2, #13
	bne.n	.L_0813d6b8
	movs	r3, #95
	ldrb	r1, [r4, r3]
	ldr	r0, [sp, #68]
	bl	sub_08002054
	movs	r1, #6
	bl	sub_08002054
	cmp	r0, #6
	bgt.n	.L_0813d6da
	movs	r0, #134
	bl	sub_081c0010
	b.n	.L_0813d6e8
.L_0813d6b8:
	adds	r3, r6, #2
	ldrb	r2, [r4, r3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0813d6cc
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d6d4
.L_0813d6cc:
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_0813d6e8
.L_0813d6d4:
	ldr	r3, [sp, #80]
	cmp	r3, #8
	bne.n	.L_0813d6e2
.L_0813d6da:
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0813d6e8
.L_0813d6e2:
	movs	r0, #133
	bl	sub_081180e8
.L_0813d6e8:
	ldr	r5, [sp, #28]
	ldr	r6, [sp, #80]
	ldr	r4, [pc, #196]
	subs	r3, r5, r6
	adds	r3, #2
	adds	r1, r4, #0
	ldrb	r2, [r1, r3]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d74c
	ldr	r0, [sp, #76]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r3, [sp, #84]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #84]
	movs	r1, #4
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	ldr	r5, [pc, #156]
	bl	sub_08118088
	movs	r0, #0
	mov	sl, r0
	movs	r6, #15
.L_0813d734:
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	add	sl, r1
	adds	r0, #15
	mov	r2, sl
	str	r0, [r5, #0]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0813d734
	b.n	.L_0813d82e
.L_0813d74c:
	ldr	r5, [sp, #28]
	ldr	r6, [sp, #80]
	subs	r3, r5, r6
	adds	r3, #2
	ldrb	r2, [r1, r3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d7cc
	ldr	r0, [sp, #76]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r3, [sp, #84]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #84]
	movs	r1, #4
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	ldr	r5, [pc, #60]
	bl	sub_08118088
	movs	r0, #0
	mov	sl, r0
	movs	r6, #15
.L_0813d794:
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	movs	r2, #128
	adds	r0, #15
	add	sl, r1
	lsls	r2, r2, #1
	str	r0, [r5, #0]
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_0813d794
	b.n	.L_0813d82e
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x0819754f
	.4byte 0x081975ee
	.4byte 0x081975fa
	.4byte 0x081975e2
	.4byte 0x081975e8
	.2byte 0x3818
	.2byte 0x0201
.L_0813d7cc:
	ldr	r3, [sp, #76]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r2, r3, r5
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r1, [sp, #84]
	movs	r3, #8
	movs	r6, #36
	ldrsh	r0, [r1, r6]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #80]
	ldr	r5, [sp, #28]
	ldr	r7, [pc, #736]
	movs	r2, #0
	subs	r3, r5, r6
	mov	sl, r2
	adds	r6, r3, #4
.L_0813d7fc:
	ldrb	r3, [r7, r6]
	ldr	r0, [sp, #68]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #1
	bl	sub_08002054
	lsls	r0, r0, #5
	add	r0, sl
	lsls	r5, r0, #3
	subs	r5, r5, r0
	ldr	r0, [pc, #708]
	lsls	r5, r5, #2
	adds	r5, r5, r0
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	sl, r1
	adds	r3, #7
	mov	r2, sl
	str	r3, [r5, #24]
	cmp	r2, #32
	bne.n	.L_0813d7fc
.L_0813d82e:
	ldr	r3, [sp, #80]
	cmp	r3, #16
	bne.n	.L_0813d86a
	ldr	r2, [pc, #668]
	movs	r3, #116
	ldrb	r2, [r2, r3]
	movs	r5, #0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	mov	sl, r5
	lsls	r5, r3, #1
.L_0813d844:
	ldr	r0, [sp, #68]
	adds	r1, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	add	r0, sl
	lsls	r3, r0, #3
	ldr	r6, [sp, #76]
	subs	r3, r3, r0
	movs	r0, #1
	lsls	r3, r3, #2
	add	sl, r0
	adds	r3, r6, r3
	movs	r2, #0
	mov	r1, sl
	str	r2, [r3, #24]
	cmp	r1, #4
	bne.n	.L_0813d844
	b.n	.L_0813d8a4
.L_0813d86a:
	ldr	r5, [sp, #28]
	ldr	r6, [sp, #80]
	movs	r2, #0
	mov	sl, r2
	ldr	r2, [pc, #608]
	subs	r3, r5, r6
	adds	r3, #4
	ldrb	r2, [r2, r3]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r5, r3, #1
.L_0813d880:
	ldr	r0, [sp, #68]
	adds	r1, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	add	r0, sl
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r0, [sp, #76]
	lsls	r3, r3, #2
	movs	r1, #1
	movs	r2, #0
	adds	r3, r0, r3
	add	sl, r1
	str	r2, [r3, #24]
	mov	r2, sl
	cmp	r2, #8
	bne.n	.L_0813d880
.L_0813d8a4:
	ldr	r4, [pc, #556]
.L_0813d8a6:
	ldr	r3, [sp, #68]
	subs	r3, #12
	cmp	r3, #19
	bls.n	.L_0813d8b0
	b.n	.L_0813db96
.L_0813d8b0:
	movs	r3, #1
	str	r3, [sp, #44]
	b.n	.L_0813db96
.L_0813d8b6:
	ldr	r6, [sp, #68]
	adds	r3, r0, #4
	cmp	r6, r3
	blt.n	.L_0813d8c0
	b.n	.L_0813db96
.L_0813d8c0:
	cmp	r6, r0
	bge.n	.L_0813d8e2
	adds	r0, r6, #0
	adds	r1, r5, #0
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r4, [sp, #8]
	cmp	r0, #4
	ble.n	.L_0813d8da
.L_0813d8d4:
	subs	r0, #4
	cmp	r0, #4
	bgt.n	.L_0813d8d4
.L_0813d8da:
	ldr	r3, [pc, #512]
	ldrb	r3, [r3, r0]
	mov	fp, r3
	b.n	.L_0813d8e6
.L_0813d8e2:
	movs	r0, #3
	mov	fp, r0
.L_0813d8e6:
	ldr	r2, [sp, #80]
	ldr	r1, [sp, #28]
	movs	r3, #5
	subs	r1, r1, r2
	mov	r9, r1
	add	r3, r9
	mov	sl, r3
	ldrb	r3, [r4, r3]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #104
	bl	sub_08196404
	mov	r6, fp
	ldr	r3, [sp, #36]
	lsls	r5, r6, #3
	subs	r5, r5, r6
	ldr	r2, [r3, #0]
	lsls	r5, r5, #2
	ldr	r0, [sp, #76]
	subs	r5, r5, r6
	movs	r6, #18
	str	r6, [sp, #0]
	mov	r8, r6
	movs	r6, #48
	str	r6, [sp, #4]
	lsrs	r3, r2, #31
	lsls	r5, r5, #5
	movs	r1, #156
	adds	r5, r0, r5
	lsls	r1, r1, #5
	adds	r2, r2, r3
	movs	r0, #192
	adds	r5, r5, r1
	lsls	r0, r0, #18
	asrs	r2, r2, #1
	ldr	r4, [r0, #104]
	adds	r1, r5, #0
	ldr	r0, [sp, #72]
	subs	r2, #18
	movs	r3, #56
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	ldr	r1, [pc, #396]
	mov	r2, sl
	ldrb	r3, [r1, r2]
	movs	r0, #104
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_08196404
	mov	r3, sp
	adds	r3, #140
	str	r3, [sp, #36]
	mov	r0, r8
	ldr	r2, [r3, #0]
	str	r0, [sp, #0]
	str	r6, [sp, #4]
	lsrs	r3, r2, #31
	movs	r1, #192
	adds	r2, r2, r3
	lsls	r1, r1, #18
	ldr	r4, [r1, #104]
	movs	r3, #56
	adds	r1, r5, #0
	asrs	r2, r2, #1
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	ldr	r4, [pc, #332]
	mov	r6, r9
	adds	r7, r6, #4
	ldrb	r5, [r4, r7]
	ldr	r2, [sp, #28]
	lsls	r1, r5, #2
	ldr	r0, [sp, #68]
	str	r4, [sp, #8]
	mov	r8, r2
	bl	sub_08002064
	lsls	r3, r5, #1
	adds	r3, r3, r5
	ldr	r4, [sp, #8]
	cmp	r0, r3
	beq.n	.L_0813d9a6
	b.n	.L_0813db8c
.L_0813d9a6:
	ldr	r5, [sp, #84]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #238
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #168
	ldr	r4, [sp, #8]
	adds	r2, r0, r1
	movs	r3, #4
	str	r3, [r2, #0]
	adds	r3, r6, #2
	ldrb	r2, [r4, r3]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0813d9de
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813d9e6
.L_0813d9de:
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_0813da08
.L_0813d9e6:
	mov	r3, r9
	adds	r3, #1
	ldrb	r3, [r4, r3]
	ldrb	r2, [r4, r7]
	lsls	r3, r3, #2
	subs	r3, #4
	muls	r3, r2
	ldr	r2, [sp, #68]
	cmp	r2, r3
	ble.n	.L_0813da02
	movs	r0, #133
	bl	sub_081180e8
	b.n	.L_0813da08
.L_0813da02:
	movs	r0, #133
	bl	sub_081c0010
.L_0813da08:
	ldr	r6, [sp, #80]
	ldr	r4, [pc, #200]
	mov	r5, r8
	subs	r3, r5, r6
	adds	r3, #2
	ldrb	r2, [r4, r3]
	movs	r3, #32
	ands	r3, r2
	adds	r1, r4, #0
	cmp	r3, #0
	beq.n	.L_0813da6c
	ldr	r0, [sp, #76]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r3, [sp, #84]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #84]
	movs	r1, #4
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	ldr	r5, [pc, #148]
	bl	sub_08118088
	movs	r0, #0
	mov	sl, r0
	movs	r6, #15
.L_0813da54:
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	add	sl, r1
	adds	r0, #15
	mov	r2, sl
	str	r0, [r5, #0]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0813da54
	b.n	.L_0813dacc
.L_0813da6c:
	ldr	r6, [sp, #80]
	mov	r5, r8
	subs	r3, r5, r6
	adds	r3, #2
	ldrb	r2, [r1, r3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813dace
	ldr	r0, [sp, #76]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r3, [sp, #84]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #84]
	movs	r1, #4
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	ldr	r5, [pc, #52]
	bl	sub_08118088
	movs	r0, #0
	mov	sl, r0
	movs	r6, #15
.L_0813dab4:
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r6
	movs	r2, #128
	adds	r0, #15
	add	sl, r1
	lsls	r2, r2, #1
	str	r0, [r5, #0]
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L_0813dab4
.L_0813dacc:
	ldr	r4, [pc, #4]
.L_0813dace:
	movs	r3, #0
	mov	sl, r3
	b.n	.L_0813dae6
	.4byte 0x0819754f
	.4byte 0x02013800
	.4byte 0x08197600
	.2byte 0x3818
	.2byte 0x0201
.L_0813dae4:
	ldr	r4, [pc, #916]
.L_0813dae6:
	ldr	r6, [sp, #80]
	mov	r5, r8
	subs	r3, r5, r6
	adds	r6, r3, #4
	ldrb	r3, [r4, r6]
	ldr	r0, [sp, #68]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #1
	bl	sub_08002054
	lsls	r0, r0, #6
	add	r0, sl
	lsls	r5, r0, #3
	subs	r5, r5, r0
	ldr	r0, [pc, #888]
	lsls	r5, r5, #2
	adds	r5, r5, r0
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	sl, r1
	adds	r3, #7
	mov	r2, sl
	str	r3, [r5, #24]
	cmp	r2, #64
	bne.n	.L_0813dae4
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [pc, #852]
	ldrb	r2, [r3, r6]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r5, r3, #1
.L_0813db2e:
	ldr	r0, [sp, #68]
	adds	r1, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	add	r0, sl
	lsls	r3, r0, #3
	ldr	r6, [sp, #76]
	subs	r3, r3, r0
	movs	r0, #1
	lsls	r3, r3, #2
	add	sl, r0
	adds	r3, r6, r3
	movs	r2, #0
	mov	r1, sl
	str	r2, [r3, #24]
	cmp	r1, #8
	bne.n	.L_0813db2e
	ldr	r6, [sp, #80]
	mov	r5, r8
	mov	sl, r2
	ldr	r2, [pc, #800]
	subs	r3, r5, r6
	adds	r3, #4
	ldrb	r2, [r2, r3]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r5, r3, #1
.L_0813db66:
	ldr	r0, [sp, #68]
	adds	r1, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	add	r0, sl
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r0, [pc, #780]
	lsls	r3, r3, #2
	movs	r1, #1
	movs	r2, #0
	adds	r3, r3, r0
	add	sl, r1
	str	r2, [r3, #24]
	mov	r2, sl
	cmp	r2, #16
	bne.n	.L_0813db66
	ldr	r4, [pc, #752]
.L_0813db8c:
	mov	r3, fp
	cmp	r3, #3
	bne.n	.L_0813db96
	movs	r5, #1
	str	r5, [sp, #44]
.L_0813db96:
	ldr	r6, [sp, #28]
	ldr	r0, [sp, #80]
	subs	r3, r6, r0
	adds	r3, #2
	ldrb	r2, [r4, r3]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813dca0
	ldr	r1, [sp, #44]
	cmp	r1, #0
	beq.n	.L_0813dca0
	ldr	r3, [sp, #84]
	add	r5, sp, #116
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r1, [sp, #84]
	mov	fp, r5
	movs	r6, #36
	ldrsh	r0, [r1, r6]
	ldr	r1, [sp, #48]
	bl	sub_0815e21c
	ldr	r2, [sp, #48]
	ldr	r1, [r5, #4]
	ldr	r3, [r2, #4]
	movs	r6, #3
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r5, #4]
	ldr	r7, [sp, #68]
	movs	r3, #0
	mov	sl, r3
	mov	r9, r6
	ands	r7, r6
.L_0813dbe8:
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
	ldr	r3, [pc, #616]
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
	ldr	r4, [pc, #596]
	ldr	r5, [r0, #4]
	asrs	r3, r3, #17
	subs	r5, r5, r3
	ldrb	r3, [r4, r7]
	str	r4, [sp, #8]
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08014878
	ldr	r3, [pc, #580]
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
	ldr	r2, [pc, #560]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
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
	ldr	r0, [sp, #72]
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	cmp	r5, #3
	bne.n	.L_0813dbe8
.L_0813dca0:
	bl	sub_08014de4
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #24]
	bl	sub_080156e8
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r3, [r6, #104]
	movs	r1, #3
	movs	r0, #188
	str	r3, [sp, #96]
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #40]
	movs	r1, #0
	str	r3, [r0, #4]
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #80]
	str	r1, [sp, #64]
	subs	r3, r2, r5
	adds	r3, #2
	mov	fp, r3
.L_0813dcde:
	ldr	r6, [sp, #84]
	ldr	r0, [r6, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r4, [pc, #400]
	mov	r9, r0
	mov	r0, fp
	ldrb	r2, [r4, r0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813dd60
	ldr	r5, [sp, #76]
	movs	r1, #0
	mov	sl, r1
.L_0813dcfe:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0813dd52
	cmp	r3, #0
	bge.n	.L_0813dd0a
	adds	r3, #3
.L_0813dd0a:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	adds	r1, r1, r3
	ldr	r3, [sp, #76]
	mov	r2, sl
	movs	r4, #1
	lsls	r1, r1, #7
	movs	r6, #228
	ands	r4, r2
	adds	r1, r3, r1
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	lsls	r6, r6, #6
	movs	r0, #24
	adds	r1, r1, r6
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r0, [sp, #40]
	lsls	r4, r4, #2
	subs	r3, #24
	ldr	r4, [r4, r0]
	subs	r2, #12
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #336]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0813dd52:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0813dcfe
	ldr	r4, [pc, #284]
.L_0813dd60:
	mov	r3, fp
	ldrb	r2, [r4, r3]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813ddd2
	movs	r5, #0
	mov	sl, r5
	ldr	r6, [pc, #296]
	ldr	r5, [sp, #76]
.L_0813dd74:
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bhi.n	.L_0813ddc4
	cmp	r1, #0
	bge.n	.L_0813dd80
	adds	r1, #3
.L_0813dd80:
	ldr	r0, [sp, #76]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r2, #228
	adds	r1, r0, r1
	lsls	r2, r2, #6
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	ldr	r4, [sp, #96]
	ldr	r0, [sp, #72]
	subs	r2, #16
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	adds	r2, r6, #0
	bl	sub_08138086
	adds	r0, r5, #0
	movs	r1, #62
	adds	r2, r6, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0813ddc4:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0813dd74
	ldr	r4, [pc, #168]
.L_0813ddd2:
	mov	r3, fp
	ldrb	r2, [r4, r3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0813dde0
	b.n	.L_0813df0a
.L_0813dde0:
	ldr	r6, [sp, #80]
	movs	r5, #3
	mov	r8, r5
	cmp	r6, #11
	bne.n	.L_0813ddee
	movs	r0, #8
	mov	r8, r0
.L_0813ddee:
	ldr	r1, [sp, #68]
	cmp	r1, #55
	bne.n	.L_0813de08
	ldr	r2, [sp, #84]
	movs	r3, #0
	ldr	r0, [r2, #8]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0813de08:
	ldr	r3, [sp, #68]
	cmp	r3, #90
	bne.n	.L_0813de22
	ldr	r5, [sp, #84]
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r5, #8]
	movs	r1, #0
	str	r3, [sp, #0]
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0813de22:
	ldr	r5, [pc, #96]
	movs	r6, #0
	mov	r0, r8
	mov	sl, r6
	lsls	r7, r0, #1
	add	r6, sp, #104
.L_0813de2e:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0813defc
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_0815e1ec
	ldr	r2, [r6, #0]
	ldr	r1, [pc, #96]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	subs	r3, r7, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #60]
	mov	r0, r8
	adds	r1, r3, r1
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r7, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #10
	ble.n	.L_0813defc
	b.n	.L_0813dea4
	movs	r0, r0
	.4byte 0x0819754f
	.4byte 0x02013800
	.4byte 0x02010000
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08197605
	.4byte 0x08197486
	.4byte 0xfffffc00
	.4byte 0xfffff800
	.2byte 0x7410
	.2byte 0x0819
.L_0813dea4:
	mov	r1, r9
	ldr	r0, [r1, #8]
	ldr	r3, [r5, #0]
	ldr	r1, [r1, #12]
	subs	r0, r0, r3
	ldr	r3, [r5, #4]
	movs	r2, #160
	subs	r1, r1, r3
	lsls	r2, r2, #13
	mov	r3, r9
	adds	r1, r1, r2
	ldr	r2, [r3, #16]
	ldr	r3, [r5, #8]
	asrs	r0, r0, #8
	subs	r2, r2, r3
	ldr	r3, [r5, #12]
	asrs	r1, r1, #8
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #8
	adds	r3, r3, r1
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r1, #255
	str	r3, [r5, #16]
	ldr	r3, [r5, #20]
	adds	r0, r0, r1
	movs	r1, #248
	lsls	r1, r1, #5
	adds	r3, r3, r2
	adds	r1, #254
	str	r3, [r5, #20]
	cmp	r0, r1
	bhi.n	.L_0813defc
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #255
	adds	r3, r2, r0
	cmp	r3, r1
	bhi.n	.L_0813defc
	movs	r1, #1
	negs	r1, r1
	str	r1, [r5, #24]
.L_0813defc:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_0813de2e
	ldr	r4, [pc, #480]
.L_0813df0a:
	mov	r5, fp
	ldrb	r2, [r4, r5]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813df80
	ldr	r0, [pc, #472]
	ldr	r7, [pc, #472]
	movs	r6, #0
	mov	sl, r6
	mov	r8, r0
	add	r6, sp, #104
.L_0813df22:
	ldr	r5, [r7, #24]
	cmp	r5, #0
	blt.n	.L_0813df72
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	adds	r0, r7, #0
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
	asrs	r5, r5, #3
	adds	r5, #1
	lsls	r0, r5, #1
	subs	r3, r0, #2
	mov	r2, r8
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #60]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	subs	r3, r3, r5
	str	r0, [sp, #4]
	ldr	r5, [sp, #40]
	ldr	r0, [sp, #72]
	ldr	r4, [r5, #4]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
	subs	r3, #1
	str	r3, [r7, #24]
.L_0813df72:
	movs	r0, #1
	movs	r1, #128
	add	sl, r0
	lsls	r1, r1, #1
	adds	r7, #28
	cmp	sl, r1
	bne.n	.L_0813df22
.L_0813df80:
	ldr	r2, [sp, #64]
	adds	r2, #1
	str	r2, [sp, #64]
	cmp	r2, #1
	beq.n	.L_0813df8c
	b.n	.L_0813dcde
.L_0813df8c:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r5, [sp, #28]
	ldr	r6, [sp, #80]
	ldr	r4, [pc, #332]
	subs	r3, r5, r6
	adds	r3, #2
	ldrb	r2, [r4, r3]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813e07e
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #0
	adds	r7, r0, #0
	str	r3, [r7, #20]
	ldr	r2, [pc, #308]
	ldr	r3, [sp, #88]
	movs	r1, #6
	ands	r3, r2
	ldr	r2, [pc, #304]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #192
	ldr	r0, [sp, #76]
	lsls	r2, r2, #3
	orrs	r3, r2
	movs	r2, #228
	lsls	r2, r2, #6
	str	r3, [sp, #88]
	adds	r3, r0, r2
	add	r2, sp, #88
	str	r3, [r2, #4]
	ldr	r3, [pc, #284]
	str	r1, [r7, #0]
	str	r3, [r7, #8]
	mov	r3, r8
	str	r2, [r7, #16]
	str	r3, [r7, #12]
	ldr	r5, [sp, #68]
	cmp	r5, #8
	bne.n	sub_0813e002
	movs	r1, #128
	ldr	r3, [pc, #268]
	ldr	r0, [sp, #72]
	lsls	r1, r1, #7
	ldr	r2, [pc, #264]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9811
	subs	r0, #8
	cmp	r0, #9
	bhi.n	.L_0813e070
	movs	r6, #192
	lsls	r6, r6, #6
	lsls	r0, r0, #11
	adds	r0, r0, r6
	bl	sub_08002096
	lsls	r5, r0, #3
	adds	r5, r5, r0
	lsls	r3, r5, #4
	adds	r5, r5, r3
	bl	sub_08014de4
	ldr	r1, [sp, #36]
	movs	r2, #0
	ldr	r0, [r1, #0]
	ldr	r1, [r1, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #64
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	bl	sub_08015160
	lsls	r5, r5, #2
	movs	r0, #128
	asrs	r6, r5, #8
	lsls	r0, r0, #7
	lsrs	r5, r5, #31
	bl	sub_08015024
	adds	r5, r6, r5
	movs	r0, #128
	lsls	r0, r0, #6
	asrs	r5, r5, #1
	bl	sub_08015068
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #168]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_0813e070:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	ldr	r4, [pc, #108]
.L_0813e07e:
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #80]
	subs	r3, r2, r5
	adds	r3, #2
	ldrb	r2, [r4, r3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0813e09a
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	.L_0813e0a2
.L_0813e09a:
	movs	r0, #2
	movs	r1, #4
	bl	sub_08158ce0
.L_0813e0a2:
	bl	sub_081434f8
	movs	r0, #240
	ldr	r6, [sp, #76]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r6, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #80]
	adds	r1, #1
	str	r1, [sp, #68]
	ldr	r4, [pc, #36]
	subs	r3, r2, r5
	adds	r3, #6
	ldrb	r3, [r4, r3]
	cmp	r1, r3
	beq.n	.L_0813e0d4
	bl	.L_0813d4c8
.L_0813e0d4:
	ldr	r0, [pc, #56]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #164
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0819754f
	.4byte 0x08197410
	.4byte 0x02013800
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x08199210
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #12]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r2, [r5, #92]
	ldr	r3, [r5, #96]
	movs	r0, #1
	str	r3, [sp, #8]
	mov	r9, r2
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #50
	strh	r3, [r2, #0]
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #16]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	movs	r4, #16
	movs	r1, #224
	add	r4, sp
	lsls	r1, r1, #3
	str	r3, [r4, #4]
	ldr	r0, [pc, #16]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	b.n	.L_0813e180
	.4byte 0x00000100
	.4byte 0x00001000
	.2byte 0x0139
	.2byte 0x0000
.L_0813e180:
	mov	fp, r4
	bl	sub_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #0
	add	r2, r9
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #120]
	bl	sub_080145a8
	ldr	r2, [sp, #12]
	add	r5, sp, #24
	adds	r1, r5, #0
	movs	r6, #36
	ldrsh	r0, [r2, r6]
	bl	sub_0815e20c
	ldr	r2, [r5, #0]
	movs	r1, #128
	movs	r3, #64
	lsls	r1, r1, #19
	subs	r3, r3, r2
	lsls	r3, r3, #8
	adds	r1, #40
	str	r3, [r1, #0]
	movs	r3, #0
	mov	r8, r3
	mov	r5, r9
.L_0813e1ca:
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	mov	r2, r8
	adds	r0, #16
	str	r0, [r5, #0]
	cmp	r2, #0
	bge.n	.L_0813e1e0
	adds	r2, #3
.L_0813e1e0:
	asrs	r2, r2, #2
	movs	r3, #24
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	cmp	r0, #43
	bgt.n	.L_0813e1f2
	movs	r3, #3
	b.n	.L_0813e224
.L_0813e1f2:
	cmp	r0, #51
	bgt.n	.L_0813e1fa
	movs	r3, #2
	b.n	.L_0813e224
.L_0813e1fa:
	cmp	r0, #59
	bgt.n	.L_0813e202
	movs	r3, #1
	b.n	.L_0813e224
.L_0813e202:
	cmp	r0, #67
	bgt.n	.L_0813e20a
	movs	r3, #0
	b.n	.L_0813e224
.L_0813e20a:
	cmp	r0, #75
	bgt.n	.L_0813e212
	movs	r3, #1
	b.n	.L_0813e222
.L_0813e212:
	cmp	r0, #83
	bgt.n	.L_0813e220
	movs	r3, #2
	b.n	.L_0813e222
	movs	r0, r0
	.2byte 0x3001
	.2byte 0x0814
.L_0813e220:
	movs	r3, #3
.L_0813e222:
	negs	r3, r3
.L_0813e224:
	str	r3, [r5, #12]
	ldr	r3, [r5, #12]
	movs	r4, #1
	lsls	r3, r3, #17
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	ldr	r3, [r5, #0]
	add	r8, r4
	lsls	r3, r3, #16
	mov	r6, r8
	str	r3, [r5, #0]
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L_0813e1ca
	movs	r0, #212
	bl	sub_081c0010
	movs	r7, #0
.L_0813e24c:
	cmp	r7, #16
	bgt.n	.L_0813e268
	ldr	r2, [pc, #44]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	cmp	r7, #16
	bne.n	.L_0813e268
	ldr	r3, [pc, #32]
	subs	r2, #2
	strh	r3, [r2, #0]
.L_0813e268:
	cmp	r7, #103
	ble.n	.L_0813e29a
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #16]
	movs	r4, #128
	subs	r3, r3, r7
	lsls	r4, r4, #19
	orrs	r3, r2
	adds	r4, #82
	strh	r3, [r4, #0]
	b.n	.L_0813e28c
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00000000
	.2byte 0x0078
	.2byte 0x0000
.L_0813e28c:
	cmp	r7, #104
	bne.n	.L_0813e29a
	movs	r2, #128
	ldr	r3, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
.L_0813e29a:
	movs	r5, #210
	movs	r6, #15
	movs	r2, #32
	lsls	r5, r5, #1
	mov	r8, r6
	mov	sl, r2
	add	r5, r9
.L_0813e2a8:
	ldr	r0, [r5, #12]
	adds	r1, r0, #0
	cmp	r0, #0
	bge.n	.L_0813e2b8
	negs	r1, r0
	b.n	.L_0813e2b8
	.2byte 0x3f44
	.2byte 0x0000
.L_0813e2b8:
	mov	r3, r8
	lsls	r6, r3, #2
	adds	r3, r6, #0
	adds	r3, #25
	asrs	r1, r1, #17
	cmp	r7, r3
	bge.n	.L_0813e30e
	lsls	r1, r1, #10
	movs	r4, #224
	lsls	r4, r4, #3
	add	r1, r9
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r1, r1, r4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	lsrs	r0, r0, #31
	subs	r3, #16
	mov	ip, r3
	mov	r3, sl
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	lsls	r0, r0, #2
	mov	r3, fp
	ldr	r4, [r0, r3]
	subs	r2, #16
	mov	r3, ip
	ldr	r0, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c33
	adds	r3, #16
	cmp	r7, r3
	blt.n	sub_0813e338
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	b.n	sub_0813e338
.L_0813e30e:
	lsls	r1, r1, #10
	movs	r4, #224
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	lsls	r4, r4, #3
	mov	r6, sl
	add	r1, r9
	adds	r1, r1, r4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	lsrs	r0, r0, #31
	lsls	r0, r0, #2
	mov	r6, fp
	ldr	r4, [r0, r6]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2201
	negs	r2, r2
	add	r8, r2
	subs	r5, #28
	cmp	r8, r2
	bne.n	.L_0813e2a8
	adds	r3, r7, #0
	subs	r3, #23
	cmp	r3, #64
	bhi.n	.L_0813e382
	movs	r3, #3
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_0813e382
	ldr	r4, [sp, #12]
	movs	r2, #5
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r3, #7
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_0813e382
	movs	r0, #133
	bl	sub_081c0010
.L_0813e382:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r7, #1
	bl	sub_08013560
	cmp	r7, #120
	beq.n	.L_0813e3a8
	b.n	.L_0813e24c
.L_0813e3a8:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08143001
