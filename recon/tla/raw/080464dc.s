.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080138a8, 0x080138a8
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080143f8, 0x080143f8
	.set sub_08014d78, 0x08014d78
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08039418, 0x08039418
	.set sub_08039430, 0x08039430
	.set sub_080396bc, 0x080396bc
	.set sub_0803a2b0, 0x0803a2b0
	.set sub_0803aae4, 0x0803aae4
	.set sub_0803acd4, 0x0803acd4
	.set sub_0803c274, 0x0803c274
	.set sub_0803c378, 0x0803c378
	.set sub_0803c9bc, 0x0803c9bc
	.set sub_0803cca8, 0x0803cca8
	.set sub_0803ccd0, 0x0803ccd0
	.set sub_0803d98c, 0x0803d98c
	.set sub_08041b68, 0x08041b68
	.set sub_08041c0c, 0x08041c0c
	.set sub_08041c54, 0x08041c54
	.set sub_08041f70, 0x08041f70
	.set sub_08042010, 0x08042010
	.set sub_080420fc, 0x080420fc
	.set sub_08042214, 0x08042214
	.set sub_08042244, 0x08042244
	.set sub_0804297c, 0x0804297c
	.set sub_0804524c, 0x0804524c
	.set sub_0804537c, 0x0804537c
	.set sub_08045464, 0x08045464
	.set sub_080454a0, 0x080454a0
	.set sub_080454dc, 0x080454dc
	.set sub_08045528, 0x08045528
	.set sub_08046134, 0x08046134
	.set sub_08046260, 0x08046260
	.set sub_08046284, 0x08046284
	.set sub_080463d4, 0x080463d4
	.set sub_08046414, 0x08046414
	.set sub_080ad000, 0x080ad000
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.set sub_080ad158, 0x080ad158
	.set sub_080ad160, 0x080ad160
	.set sub_080ad180, 0x080ad180
	.set sub_080ad188, 0x080ad188
	.set sub_080ad1a0, 0x080ad1a0
	.set sub_080ad1b0, 0x080ad1b0
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080ad200, 0x080ad200
	.set sub_080ad250, 0x080ad250
	.set sub_08118010, 0x08118010
	.set sub_08118098, 0x08118098
	.set sub_08118118, 0x08118118
	.set sub_08118120, 0x08118120
	.set sub_08118148, 0x08118148
	.set sub_081c0010, 0x081c0010
	.global Func_080464dc
	.thumb_func
Func_080464dc:
.L_080464dc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #76
	add	r4, sp, #72
	adds	r7, r1, #0
	str	r0, [r4, #0]
	adds	r0, r7, #0
	mov	sl, r4
	mov	r8, r2
	str	r3, [sp, #60]
	bl	sub_08016ca4
	str	r0, [sp, #56]
	mov	r1, sl
	ldr	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L_0804650e
	movs	r1, #1
	bl	sub_0803939c
.L_0804650e:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L_0804652a
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #11
	movs	r1, #8
	movs	r2, #20
	bl	sub_08039260
	mov	r3, sl
	str	r0, [r3, #0]
	b.n	.L_0804653e
.L_0804652a:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #5
	movs	r2, #21
	movs	r3, #14
	bl	sub_08039260
	mov	r4, sl
	str	r0, [r4, #0]
.L_0804653e:
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_0804654a
	b.n	.L_08046ae2
.L_0804654a:
	movs	r0, #128
	bl	sub_08014d78
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #48]
	adds	r0, r5, #0
	bl	sub_08014d78
	str	r0, [sp, #52]
	movs	r0, #96
	bl	sub_08014dac
	adds	r2, r5, #0
	str	r0, [sp, #24]
	ldr	r3, [pc, #648]
	ldr	r1, [sp, #56]
	ldr	r0, [sp, #52]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4642
	asrs	r5, r2, #8
	movs	r3, #15
	movs	r6, #255
	ands	r5, r3
	ands	r6, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080ad1b0
	cmp	r0, #0
	beq.n	.L_08046598
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080ad160
	b.n	.L_080465a2
.L_08046598:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080ad158
.L_080465a2:
	adds	r0, r7, #0
	bl	sub_080ad008
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #56]
	add	r2, sp, #64
	add	r3, sp, #68
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #24]
	adds	r0, #88
	bl	sub_08046284
	movs	r1, #5
	str	r0, [sp, #20]
	subs	r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #108]
	adds	r0, #1
	str	r0, [r3, #0]
	ldr	r4, [sp, #60]
	ldr	r1, [sp, #20]
	lsls	r3, r4, #2
	adds	r3, r3, r4
	subs	r3, #5
	cmp	r3, r1
	blt.n	.L_080465dc
	str	r0, [sp, #60]
.L_080465dc:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq.n	.L_080465e4
	b.n	.L_08046724
.L_080465e4:
	ldr	r5, [pc, #528]
	mov	r3, sl
	ldr	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08042010
	mov	r4, sl
	ldr	r1, [r4, #0]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #16
	bl	sub_08042010
	mov	r2, sl
	adds	r0, r5, #2
	ldr	r1, [r2, #0]
	movs	r3, #24
	movs	r2, #0
	bl	sub_08042010
	mov	r3, sl
	adds	r0, r5, #3
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r3, #32
	bl	sub_08042010
	mov	r4, sl
	ldr	r1, [r4, #0]
	adds	r0, r5, #4
	movs	r2, #0
	movs	r3, #40
	bl	sub_08042010
	adds	r5, #5
	mov	r2, sl
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08042010
	ldr	r3, [sp, #52]
	ldr	r0, [sp, #48]
	movs	r4, #56
	ldrsh	r3, [r3, r4]
	adds	r1, r3, #0
	str	r3, [sp, #44]
	bl	sub_08046260
	ldr	r5, [sp, #48]
	mov	r4, sl
	adds	r5, #14
	ldr	r1, [r4, #0]
	movs	r2, #5
	movs	r3, #1
	adds	r0, r5, #0
	bl	sub_0803acd4
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #48]
	movs	r2, #58
	ldrsh	r1, [r1, r2]
	str	r1, [sp, #40]
	bl	sub_08046260
	mov	r2, sl
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #5
	movs	r3, #2
	bl	sub_0803acd4
	ldr	r3, [sp, #52]
	ldr	r0, [sp, #48]
	ldrh	r3, [r3, #60]
	adds	r1, r3, #0
	str	r3, [sp, #36]
	bl	sub_08046260
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #5
	movs	r3, #3
	adds	r0, r5, #0
	bl	sub_0803acd4
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #48]
	ldrh	r1, [r1, #62]
	str	r1, [sp, #32]
	bl	sub_08046260
	ldr	r5, [sp, #48]
	mov	r2, sl
	adds	r5, #16
	ldr	r1, [r2, #0]
	movs	r3, #4
	movs	r2, #6
	adds	r0, r5, #0
	bl	sub_0803acd4
	ldr	r3, [sp, #52]
	ldr	r0, [sp, #48]
	adds	r3, #64
	ldrh	r3, [r3, #0]
	adds	r1, r3, #0
	str	r3, [sp, #28]
	bl	sub_08046260
	mov	r3, sl
	ldr	r1, [r3, #0]
	movs	r2, #6
	movs	r3, #5
	adds	r0, r5, #0
	bl	sub_0803acd4
	ldr	r3, [sp, #52]
	ldr	r0, [sp, #48]
	adds	r3, #66
	ldrb	r1, [r3, #0]
	bl	sub_08046260
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #6
	movs	r3, #6
	adds	r0, r5, #0
	bl	sub_0803acd4
	mov	r1, sl
	movs	r3, #8
	ldr	r0, [r1, #0]
	movs	r2, #8
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #19
	bl	sub_08041c54
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L_0804670a
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L_08046710
.L_0804670a:
	movs	r0, #2
	bl	sub_08041f70
.L_08046710:
	mov	r2, sl
	ldr	r0, [pc, #232]
	ldr	r1, [r2, #0]
	movs	r3, #64
	movs	r2, #24
	bl	sub_08042010
	movs	r0, #15
	bl	sub_08041f70
.L_08046724:
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bgt.n	.L_0804672c
	b.n	.L_080468f8
.L_0804672c:
	ldr	r1, [sp, #20]
	movs	r4, #0
	str	r4, [sp, #12]
	str	r1, [sp, #8]
	cmp	r1, #4
	ble.n	.L_0804673c
	movs	r2, #5
	str	r2, [sp, #8]
.L_0804673c:
	ldr	r4, [sp, #60]
	ldr	r2, [sp, #12]
	lsls	r3, r4, #2
	adds	r3, r3, r4
	subs	r3, #5
	mov	r8, r3
	ldr	r3, [sp, #8]
	movs	r1, #0
	str	r1, [sp, #16]
	cmp	r2, r3
	blt.n	.L_08046754
	b.n	.L_08046884
.L_08046754:
	ldr	r4, [sp, #20]
	cmp	r8, r4
	blt.n	.L_0804675c
	b.n	.L_08046884
.L_0804675c:
	ldr	r2, [sp, #24]
	movs	r4, #4
	mov	r1, r8
	negs	r4, r4
	lsls	r3, r1, #1
	str	r4, [sp, #4]
	adds	r6, r3, r2
	movs	r3, #0
	mov	r7, sl
	mov	r9, r3
	mov	fp, r3
.L_08046772:
	ldrh	r0, [r6, #0]
	bl	sub_080ad078
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L_0804679a
	movs	r2, #160
	mov	r1, sl
	lsls	r2, r2, #7
	ldr	r0, [r1, #0]
	adds	r2, #1
	adds	r1, r3, #0
	movs	r3, #0
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #15
	mov	r3, r9
	bl	sub_0803c378
.L_0804679a:
	ldrb	r3, [r5, #8]
	cmp	r3, #255
	bne.n	.L_080467a4
	movs	r3, #11
	b.n	.L_080467a6
.L_080467a4:
	subs	r3, #1
.L_080467a6:
	movs	r4, #0
	ldr	r0, [r7, #0]
	movs	r1, #16
	mov	r2, r9
	str	r4, [sp, #0]
	bl	sub_0804524c
	ldrh	r3, [r6, #0]
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	ands	r3, r1
	ldr	r2, [sp, #4]
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl	sub_080463d4
	ldrh	r2, [r6, #0]
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080467da
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_08046806
.L_080467da:
	ldr	r3, [pc, #20]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08046800
	movs	r0, #2
	bl	sub_08041f70
	b.n	.L_08046806
	movs	r0, r0
	.4byte 0x00008000
	.4byte 0x00004000
	.4byte 0x03000730
	.4byte 0x00000d0e
	.2byte 0x0d0d
	.2byte 0x0000
.L_08046800:
	movs	r0, #15
	bl	sub_08041f70
.L_08046806:
	ldrh	r3, [r6, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	ldr	r3, [pc, #732]
	ldr	r1, [r7, #0]
	adds	r0, r0, r3
	movs	r2, #16
	mov	r3, fp
	bl	sub_08042010
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	ldr	r0, [r7, #0]
	mov	r3, r9
	str	r2, [sp, #0]
	adds	r1, #31
	movs	r2, #11
	bl	sub_0803c378
	movs	r1, #240
	movs	r3, #0
	lsls	r1, r1, #8
	ldr	r0, [r7, #0]
	adds	r1, #30
	str	r3, [sp, #0]
	movs	r2, #12
	mov	r3, r9
	bl	sub_0803c378
	ldrh	r0, [r6, #0]
	bl	sub_080ad078
	mov	r4, fp
	ldr	r2, [r7, #0]
	ldrb	r0, [r0, #9]
	movs	r1, #2
	movs	r3, #104
	str	r4, [sp, #0]
	bl	sub_08042244
	movs	r2, #16
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #16]
	add	fp, r2
	ldr	r2, [sp, #8]
	movs	r1, #2
	add	r9, r1
	adds	r3, #16
	adds	r4, #1
	movs	r1, #1
	str	r3, [sp, #4]
	str	r4, [sp, #16]
	adds	r6, #2
	add	r8, r1
	cmp	r4, r2
	bge.n	.L_08046884
	ldr	r3, [sp, #20]
	cmp	r8, r3
	bge.n	.L_08046884
	b.n	.L_08046772
.L_08046884:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_080468a2
	movs	r0, #4
	bl	sub_08041f70
	mov	r4, sl
	ldr	r1, [r4, #0]
	ldr	r0, [pc, #604]
	movs	r2, #32
	movs	r3, #80
	bl	sub_08042010
	movs	r1, #1
	str	r1, [sp, #12]
.L_080468a2:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L_080468c6
	movs	r0, #2
	bl	sub_08041f70
	ldr	r4, [sp, #12]
	mov	r2, sl
	lsls	r3, r4, #3
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #576]
	adds	r3, #80
	movs	r2, #32
	bl	sub_08042010
	ldr	r1, [sp, #12]
	adds	r1, #1
	str	r1, [sp, #12]
.L_080468c6:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_080468da
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #552]
	movs	r2, #32
	movs	r3, #80
	bl	sub_08042010
.L_080468da:
	movs	r0, #15
	bl	sub_08041f70
	movs	r0, #15
	bl	sub_08041f70
	movs	r3, #10
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #10
	movs	r3, #19
	bl	sub_08041c54
.L_080468f8:
	ldr	r1, [sp, #60]
	cmp	r1, #0
	beq.n	.L_08046900
	b.n	.L_08046abe
.L_08046900:
	ldr	r2, [sp, #52]
	movs	r5, #42
	adds	r5, #255
	adds	r2, r2, r5
	ldrb	r0, [r2, #0]
	ldr	r6, [pc, #500]
	mov	r3, sl
	ldr	r1, [r3, #0]
	mov	r8, r2
	adds	r0, r0, r6
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042010
	ldr	r4, [sp, #56]
	mov	r2, sl
	adds	r5, r4, r5
	ldrb	r0, [r5, #0]
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #80
	adds	r0, r0, r6
	bl	sub_08042010
	mov	r3, r8
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #0]
	cmp	r2, r3
	beq.n	.L_08046952
	ldr	r2, [sp, #60]
	movs	r1, #247
	mov	r4, sl
	lsls	r1, r1, #8
	ldr	r0, [r4, #0]
	adds	r1, #40
	str	r2, [sp, #0]
	movs	r3, #0
	movs	r2, #9
	bl	sub_0803c378
	b.n	.L_08046968
.L_08046952:
	movs	r1, #247
	ldr	r4, [sp, #60]
	mov	r3, sl
	lsls	r1, r1, #8
	ldr	r0, [r3, #0]
	adds	r1, #41
	movs	r2, #9
	movs	r3, #0
	str	r4, [sp, #0]
	bl	sub_0803c378
.L_08046968:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r2, #56
	ldrsh	r1, [r3, r2]
	bl	sub_08046260
	ldr	r6, [sp, #48]
	mov	r4, sl
	adds	r6, #14
	ldr	r1, [r4, #0]
	movs	r2, #11
	movs	r3, #1
	adds	r0, r6, #0
	bl	sub_0803acd4
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #44]
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq.n	.L_080469a6
	movs	r2, #0
	cmp	r3, r4
	ble.n	.L_0804699a
	movs	r2, #1
.L_0804699a:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #14
	bl	sub_08046414
.L_080469a6:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r2, #58
	ldrsh	r1, [r3, r2]
	bl	sub_08046260
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #11
	movs	r3, #2
	adds	r0, r6, #0
	bl	sub_0803acd4
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #40]
	movs	r1, #58
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq.n	.L_080469e0
	movs	r2, #0
	cmp	r3, r4
	ble.n	.L_080469d4
	movs	r2, #1
.L_080469d4:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #22
	bl	sub_08046414
.L_080469e0:
	ldr	r2, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r2, #60]
	bl	sub_08046260
	mov	r3, sl
	ldr	r1, [r3, #0]
	adds	r0, r6, #0
	movs	r3, #3
	movs	r2, #11
	bl	sub_0803acd4
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #36]
	ldrh	r3, [r4, #60]
	cmp	r3, r1
	beq.n	.L_08046a16
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L_08046a0a
	movs	r2, #1
.L_08046a0a:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #30
	bl	sub_08046414
.L_08046a16:
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r4, #62]
	bl	sub_08046260
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #4
	adds	r0, r6, #0
	movs	r2, #11
	bl	sub_0803acd4
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #32]
	ldrh	r3, [r4, #62]
	cmp	r3, r1
	beq.n	.L_08046a4c
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L_08046a40
	movs	r2, #1
.L_08046a40:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #38
	bl	sub_08046414
.L_08046a4c:
	ldr	r5, [sp, #56]
	ldr	r0, [sp, #48]
	adds	r5, #64
	ldrh	r1, [r5, #0]
	bl	sub_08046260
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r3, #5
	adds	r0, r6, #0
	movs	r2, #11
	bl	sub_0803acd4
	ldrh	r3, [r5, #0]
	ldr	r1, [sp, #28]
	cmp	r3, r1
	beq.n	.L_08046a82
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L_08046a76
	movs	r2, #1
.L_08046a76:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #46
	bl	sub_08046414
.L_08046a82:
	ldr	r5, [sp, #56]
	ldr	r0, [sp, #48]
	adds	r5, #66
	ldrb	r1, [r5, #0]
	bl	sub_08046260
	ldr	r0, [sp, #48]
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r3, #6
	adds	r0, #16
	movs	r2, #12
	bl	sub_0803acd4
	ldr	r3, [sp, #52]
	ldrb	r1, [r5, #0]
	adds	r3, #66
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	beq.n	.L_08046abe
	movs	r2, #0
	cmp	r1, r3
	bls.n	.L_08046ab2
	movs	r2, #1
.L_08046ab2:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #54
	bl	sub_08046414
.L_08046abe:
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #64]
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #52]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9806
	bl	sub_08013164
	ldr	r0, [sp, #52]
	bl	sub_08013164
	ldr	r0, [sp, #48]
	bl	sub_08013164
	mov	r2, sl
	ldr	r0, [r2, #0]
.L_08046ae2:
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000005a7
	.4byte 0x000010d3
	.4byte 0x000010d4
	.4byte 0x000010d9
	.4byte 0x00000b63
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #280]
	movs	r3, #0
	add	sp, r5
	str	r1, [sp, #148]
	str	r2, [sp, #144]
	str	r0, [sp, #152]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #60]
	movs	r2, #1
	negs	r2, r2
	str	r0, [sp, #140]
	movs	r1, #1
	adds	r0, r2, #0
	str	r1, [sp, #132]
	str	r2, [sp, #124]
	str	r3, [sp, #104]
	bl	sub_080ad250
	str	r0, [sp, #100]
	movs	r0, #168
	movs	r4, #0
	lsls	r0, r0, #1
	str	r4, [sp, #92]
	str	r4, [sp, #88]
	bl	sub_08014dac
	str	r0, [sp, #84]
	movs	r0, #0
	str	r0, [sp, #80]
	str	r0, [sp, #76]
	str	r0, [sp, #72]
	ldr	r1, [sp, #152]
	add	r0, sp, #416
	ldrh	r3, [r1, #0]
	movs	r1, #1
	strh	r3, [r0, #0]
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [sp, #96]
	movs	r3, #255
	strh	r3, [r0, #2]
	bl	sub_08118010
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_080143e0
	str	r0, [sp, #108]
	adds	r5, #228
	ldr	r5, [r5, #0]
	ldr	r2, [sp, #132]
	ldr	r0, [r5, #68]
	str	r2, [r5, #72]
	cmp	r0, #0
	beq.n	.L_08046b90
	movs	r1, #1
	bl	sub_0803939c
	ldr	r3, [sp, #92]
	str	r3, [r5, #68]
.L_08046b90:
	movs	r0, #112
	bl	sub_081c0010
	movs	r6, #4
	movs	r2, #0
	add	r3, sp, #440
.L_08046b9c:
	subs	r6, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r6, #0
	bge.n	.L_08046b9c
	add	r2, sp, #436
	movs	r3, #0
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	movs	r0, #128
	bl	sub_080143e0
	movs	r4, #168
	str	r0, [sp, #120]
	movs	r0, #165
	lsls	r4, r4, #2
	lsls	r0, r0, #2
	add	r4, sp
	add	r0, sp
	movs	r1, #1
	str	r4, [sp, #36]
	str	r0, [sp, #40]
	negs	r1, r1
	mov	r8, r1
	adds	r5, r0, #0
	adds	r7, r4, #0
	movs	r6, #10
.L_08046bd4:
	movs	r0, #128
	bl	sub_080143e0
	subs	r6, #1
	mov	r2, r8
	strb	r2, [r5, #0]
	stmia	r7!, {r0}
	adds	r5, #1
	cmp	r6, #0
	bge.n	.L_08046bd4
	ldr	r3, [pc, #72]
	add	r5, sp, #488
	add	r7, sp, #420
	mov	r8, r3
	movs	r6, #3
.L_08046bf2:
	movs	r0, #128
	bl	sub_080143e0
	movs	r1, #1
	stmia	r7!, {r0}
	negs	r1, r1
	bl	sub_080143f8
	ldr	r3, [pc, #40]
	mov	r4, r8
	ands	r0, r3
	ldrh	r3, [r5, #0]
	subs	r6, #1
	ands	r3, r4
	orrs	r3, r0
	strh	r3, [r5, #0]
	adds	r5, #12
	cmp	r6, #0
	bge.n	.L_08046bf2
	ldr	r0, [sp, #148]
	cmp	r0, #0
	beq.n	.L_08046c5e
	ldr	r1, [sp, #152]
	movs	r6, #0
	ldrh	r3, [r1, #0]
	cmp	r3, #255
	beq.n	.L_08046c5e
	b.n	.L_08046c38
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffd1c
	.2byte 0xfc00
	.2byte 0xffff
.L_08046c38:
	cmp	r3, #254
	beq.n	.L_08046c42
	ldr	r2, [sp, #144]
	cmp	r3, r2
	beq.n	.L_08046c5c
.L_08046c42:
	adds	r6, #1
	cmp	r6, #5
	bgt.n	.L_08046c5e
	ldr	r4, [sp, #152]
	lsls	r3, r6, #1
	ldrh	r3, [r3, r4]
	cmp	r3, #255
	beq.n	.L_08046c5e
	cmp	r3, #254
	beq.n	.L_08046c42
	ldr	r0, [sp, #144]
	cmp	r3, r0
	bne.n	.L_08046c42
.L_08046c5c:
	str	r6, [sp, #124]
.L_08046c5e:
	movs	r3, #6
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	str	r1, [sp, #112]
	str	r1, [sp, #116]
	bl	sub_08039260
	movs	r3, #10
	str	r0, [sp, #136]
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #6
	movs	r1, #14
	movs	r0, #0
	bl	sub_08039260
	str	r0, [sp, #128]
	bl	sub_08041b68
	ldr	r2, [sp, #108]
	movs	r3, #182
	movs	r4, #179
	movs	r0, #132
	lsls	r3, r3, #2
	lsls	r4, r4, #2
	lsls	r0, r0, #2
	lsls	r2, r2, #2
	add	r3, sp
	add	r4, sp
	add	r0, sp
	str	r2, [sp, #24]
	str	r3, [sp, #28]
	str	r4, [sp, #32]
	str	r0, [sp, #44]
.L_08046ca8:
	ldr	r3, [pc, #108]
	ldr	r0, [pc, #112]
	ldr	r3, [r3, #12]
	add	r5, sp, #468
	str	r3, [sp, #68]
	bl	sub_0804537c
	ldr	r1, [sp, #132]
	cmp	r1, #0
	beq.n	.L_08046cc4
	ldr	r0, [sp, #144]
	ldr	r1, [sp, #108]
	bl	sub_08045528
.L_08046cc4:
	ldr	r3, [pc, #88]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #84]
	ldrh	r1, [r5, #8]
	adds	r3, r2, r3
	ldrh	r2, [r3, #2]
	ldr	r3, [pc, #80]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldrh	r2, [r5, #6]
	ldr	r3, [pc, #68]
	adds	r0, r5, #0
	ands	r3, r2
	ldr	r2, [pc, #40]
	movs	r1, #240
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strh	r3, [r5, #6]
	movs	r3, #24
	strb	r3, [r5, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r5, #9]
	bl	sub_08014128
	ldr	r3, [sp, #444]
	cmp	r3, #24
	bhi.n	.L_08046d9c
	ldr	r2, [pc, #32]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	b.n	.L_08046d34
	.4byte 0x00000008
	.4byte 0x03001150
	.4byte 0x060066c0
	.4byte 0x80000400
	.4byte 0x020036e0
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.2byte 0x6d38
	.2byte 0x0804
.L_08046d34:
	mov	pc, r3
	movs	r0, r0
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r4, [r3, #88]
	lsrs	r4, r0, #32
	ldr	r0, [r6, #88]
	lsrs	r4, r0, #32
	ldr	r0, [r6, #88]
	lsrs	r4, r0, #32
	ldr	r0, [r6, #88]
	lsrs	r4, r0, #32
	ldr	r0, [r6, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r2, [r7, #88]
	lsrs	r4, r0, #32
	ldr	r6, [r0, #92]
	lsrs	r4, r0, #32
	ldr	r2, [r2, #92]
	lsrs	r4, r0, #32
	ldr	r6, [r3, #92]
	lsrs	r4, r0, #32
.L_08046d9c:
	movs	r3, #0
	add	r2, sp, #436
	str	r3, [r2, #16]
	ldr	r3, [sp, #100]
	cmp	r3, #0
	beq.n	.L_08046dac
	movs	r3, #9
	b.n	.L_08046de6
.L_08046dac:
	movs	r3, #7
	b.n	.L_08046de6
	add	r2, sp, #436
	movs	r3, #1
	str	r3, [r2, #16]
	movs	r3, #4
	b.n	.L_08046de6
	add	r2, sp, #436
	movs	r3, #2
	str	r3, [r2, #16]
	ldr	r4, [sp, #104]
	str	r4, [r2, #20]
	b.n	.L_08046de8
	add	r2, sp, #436
	movs	r3, #3
	str	r3, [r2, #16]
	ldr	r0, [sp, #76]
	str	r0, [r2, #20]
	b.n	.L_08046de8
	add	r2, sp, #436
	movs	r3, #4
	str	r3, [r2, #16]
	ldr	r1, [sp, #80]
	str	r1, [r2, #20]
	b.n	.L_08046de8
	add	r2, sp, #436
	movs	r3, #5
	str	r3, [r2, #16]
	ldr	r3, [sp, #72]
.L_08046de6:
	str	r3, [r2, #20]
.L_08046de8:
	ldr	r1, [sp, #136]
	ldr	r3, [sp, #136]
	movs	r4, #12
	ldrsh	r0, [r1, r4]
	movs	r2, #14
	ldrsh	r1, [r1, r2]
	movs	r4, #15
	ldrh	r2, [r3, #8]
	ldrh	r3, [r3, #10]
	str	r4, [sp, #0]
	bl	sub_08046134
	ldr	r3, [pc, #784]
	movs	r0, #1
	ldr	r3, [r3, #4]
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08046e4c
	ldr	r2, [sp, #92]
	adds	r2, #1
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08046e1a
	ldr	r3, [sp, #92]
	adds	r3, #4
.L_08046e1a:
	asrs	r3, r3, #2
	str	r3, [sp, #92]
	lsls	r3, r3, #2
	subs	r2, r2, r3
	str	r2, [sp, #92]
	add	r1, sp, #436
	adds	r2, #3
	movs	r3, #0
	str	r2, [r1, #16]
	strb	r3, [r1, r2]
	str	r0, [r1, #20]
	ldr	r4, [sp, #92]
	cmp	r4, #0
	beq.n	.L_08046e3e
	adds	r3, r4, #0
	adds	r3, #21
	str	r3, [r1, #8]
	b.n	.L_08046e42
.L_08046e3e:
	ldr	r0, [sp, #92]
	str	r0, [r1, #8]
.L_08046e42:
	movs	r1, #1
	movs	r0, #112
	str	r1, [sp, #132]
	bl	sub_081c0010
.L_08046e4c:
	ldr	r2, [sp, #104]
	cmp	r2, #0
	bne.n	.L_08046e54
	b.n	.L_08047166
.L_08046e54:
	movs	r3, #218
	lsls	r3, r3, #1
	add	r3, sp
	ldr	r0, [r3, #16]
	mov	r8, r3
	cmp	r0, #1
	bhi.n	.L_08046efe
	ldr	r4, [sp, #68]
	ldrsb	r5, [r3, r0]
	movs	r3, #128
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08046e96
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	mov	r1, r8
	ldr	r3, [r1, #20]
	adds	r5, #1
	cmp	r5, r3
	bge.n	.L_08046e84
	b.n	.L_0804707c
.L_08046e84:
	ldr	r3, [r1, #16]
	movs	r5, #0
	cmp	r3, #1
	beq.n	.L_08046e8e
	b.n	.L_0804707c
.L_08046e8e:
	movs	r3, #2
	str	r3, [r1, #16]
	ldrsb	r5, [r1, r3]
	b.n	.L_0804707c
.L_08046e96:
	ldr	r2, [sp, #68]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08046ec8
	movs	r3, #0
	movs	r0, #111
	subs	r5, #1
	str	r3, [sp, #68]
	bl	sub_081c0010
	cmp	r5, #0
	blt.n	.L_08046eb2
	b.n	.L_0804707c
.L_08046eb2:
	mov	r4, r8
	ldr	r3, [r4, #20]
	subs	r5, r3, #1
	ldr	r3, [r4, #16]
	cmp	r3, #1
	beq.n	.L_08046ec0
	b.n	.L_0804707c
.L_08046ec0:
	movs	r3, #2
	str	r3, [r4, #16]
	ldrsb	r5, [r4, r3]
	b.n	.L_0804707c
.L_08046ec8:
	ldr	r0, [sp, #68]
	movs	r3, #48
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08046ed4
	b.n	.L_0804707c
.L_08046ed4:
	movs	r1, #0
	movs	r0, #111
	str	r1, [sp, #68]
	bl	sub_081c0010
	mov	r2, r8
	ldr	r3, [r2, #16]
	movs	r2, #2
	eors	r3, r2
	mov	r4, r8
	str	r3, [r4, #16]
	ldr	r3, [pc, #552]
	ldr	r5, [sp, #104]
	ldr	r2, [r3, #12]
	movs	r3, #32
	ands	r2, r3
	subs	r5, #1
	cmp	r2, #0
	beq.n	.L_08046efc
	b.n	.L_0804707c
.L_08046efc:
	b.n	.L_0804707a
.L_08046efe:
	cmp	r0, #2
	bne.n	.L_08046f80
	mov	r1, r8
	ldrsb	r5, [r1, r0]
	ldr	r2, [r1, #20]
	cmp	r5, r2
	blt.n	.L_08046f0e
	subs	r5, r2, #1
.L_08046f0e:
	cmp	r5, #0
	bge.n	.L_08046f1e
	mov	r2, r8
	movs	r3, #0
	str	r3, [r2, #16]
	movs	r5, #0
	ldrsb	r5, [r2, r5]
	b.n	.L_0804707c
.L_08046f1e:
	ldr	r4, [sp, #68]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08046f3c
	movs	r0, #0
	adds	r5, #1
	str	r0, [sp, #68]
	cmp	r5, r2
	blt.n	.L_08046f58
	mov	r1, r8
	str	r0, [r1, #16]
	movs	r5, #0
	ldrsb	r5, [r1, r5]
	b.n	.L_08046f58
.L_08046f3c:
	ldr	r2, [sp, #68]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08046f60
	movs	r3, #0
	subs	r5, #1
	str	r3, [sp, #68]
	cmp	r5, #0
	bge.n	.L_08046f58
	mov	r4, r8
	str	r3, [r4, #16]
	movs	r5, #0
	ldrsb	r5, [r4, r5]
.L_08046f58:
	movs	r0, #111
	bl	sub_081c0010
	b.n	.L_0804707c
.L_08046f60:
	ldr	r0, [sp, #68]
	movs	r3, #192
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08046f6c
	b.n	.L_0804707c
.L_08046f6c:
	movs	r1, #0
	str	r1, [sp, #68]
	mov	r2, r8
	str	r1, [r2, #16]
	movs	r0, #111
	movs	r5, #0
	ldrsb	r5, [r2, r5]
	bl	sub_081c0010
	b.n	.L_0804707c
.L_08046f80:
	cmp	r0, #3
	beq.n	.L_08046f88
	cmp	r0, #5
	bne.n	.L_08047084
.L_08046f88:
	ldr	r4, [sp, #68]
	mov	r3, r8
	ldrsb	r5, [r3, r0]
	movs	r3, #128
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08046fb6
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #132]
	mov	r2, r8
	ldr	r3, [r2, #20]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_08047076
	movs	r5, #0
	b.n	.L_0804706e
.L_08046fb6:
	ldr	r4, [sp, #68]
	movs	r3, #64
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08046fe0
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	subs	r5, #1
	str	r1, [sp, #132]
	cmp	r5, #0
	bge.n	.L_0804706e
	mov	r2, r8
	ldr	r3, [r2, #20]
	subs	r5, r3, #1
	b.n	.L_0804706e
.L_08046fe0:
	ldr	r4, [sp, #68]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0804702c
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #132]
	mov	r2, r8
	ldr	r1, [r2, #20]
	adds	r5, #4
	cmp	r5, r1
	blt.n	.L_08047076
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0804700e
	adds	r3, r5, #3
.L_0804700e:
	asrs	r0, r3, #2
	subs	r2, r1, #1
	subs	r4, r0, #1
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0804701c
	adds	r3, r1, #2
.L_0804701c:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08047028
	lsls	r3, r0, #2
	subs	r5, r5, r3
	b.n	.L_0804706e
.L_08047028:
	adds	r5, r2, #0
	b.n	.L_0804706e
.L_0804702c:
	ldr	r4, [sp, #68]
	movs	r3, #32
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0804706e
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	subs	r5, #4
	str	r1, [sp, #132]
	cmp	r5, #0
	bge.n	.L_0804706e
	mov	r2, r8
	ldr	r3, [r2, #20]
	cmp	r3, #0
	bge.n	.L_08047058
	adds	r3, #3
.L_08047058:
	asrs	r3, r3, #2
	adds	r2, r5, #4
	lsls	r1, r3, #2
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08047066
	adds	r3, r5, #7
.L_08047066:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	adds	r5, r1, r3
.L_0804706e:
	ldr	r3, [sp, #456]
	cmp	r5, r3
	blt.n	.L_08047076
	subs	r5, r3, #1
.L_08047076:
	cmp	r5, #0
	bge.n	.L_0804707c
.L_0804707a:
	movs	r5, #0
.L_0804707c:
	add	r2, sp, #436
	ldr	r3, [r2, #16]
	strb	r5, [r2, r3]
	b.n	.L_08047166
.L_08047084:
	cmp	r0, #4
	bne.n	.L_08047166
	ldr	r4, [sp, #68]
	mov	r3, r8
	ldrsb	r6, [r3, r0]
	movs	r3, #128
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080470b6
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #132]
	mov	r2, r8
	ldr	r3, [r2, #20]
	adds	r6, #1
	cmp	r6, r3
	blt.n	.L_0804715a
	movs	r6, #0
	b.n	.L_08047152
.L_080470b6:
	ldr	r4, [sp, #68]
	movs	r3, #64
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080470e0
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	subs	r6, #1
	str	r1, [sp, #132]
	cmp	r6, #0
	bge.n	.L_08047152
	mov	r2, r8
	ldr	r3, [r2, #20]
	subs	r6, r3, #1
	b.n	.L_08047152
.L_080470e0:
	ldr	r4, [sp, #68]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08047118
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	str	r1, [sp, #132]
	mov	r2, r8
	ldr	r3, [r2, #20]
	adds	r6, #3
	cmp	r6, r3
	blt.n	.L_0804715a
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_08002064
	adds	r6, r0, #0
	b.n	.L_08047152
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
.L_08047118:
	ldr	r4, [sp, #68]
	movs	r3, #32
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08047152
	movs	r0, #0
	str	r0, [sp, #68]
	movs	r0, #111
	bl	sub_081c0010
	ldr	r1, [sp, #132]
	movs	r3, #2
	orrs	r1, r3
	subs	r6, #3
	str	r1, [sp, #132]
	cmp	r6, #0
	bge.n	.L_08047152
	mov	r2, r8
	ldr	r0, [r2, #20]
	movs	r1, #3
	bl	sub_08002054
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #3
	adds	r0, r6, #3
	bl	sub_08002064
	adds	r6, r5, r0
.L_08047152:
	ldr	r3, [sp, #456]
	cmp	r6, r3
	blt.n	.L_0804715a
	subs	r6, r3, #1
.L_0804715a:
	cmp	r6, #0
	bge.n	.L_08047160
	movs	r6, #0
.L_08047160:
	add	r2, sp, #436
	ldr	r3, [r2, #16]
	strb	r6, [r2, r3]
.L_08047166:
	movs	r3, #218
	lsls	r3, r3, #1
	add	r3, sp
	ldr	r2, [r3, #16]
	mov	r8, r3
	cmp	r2, #0
	bne.n	.L_080471b2
	ldr	r4, [sp, #100]
	ldrsb	r2, [r3, r2]
	cmp	r4, #0
	bne.n	.L_0804717e
	adds	r2, #9
.L_0804717e:
	ldr	r3, [pc, #256]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	mov	r0, r8
	str	r3, [r0, #8]
	ldrb	r3, [r2, #1]
	str	r3, [r0, #24]
	ldrb	r3, [r2, #2]
	str	r3, [r0, #28]
	ldr	r3, [sp, #136]
	movs	r1, #12
	ldrsh	r0, [r3, r1]
	ldrb	r3, [r2, #3]
	adds	r0, r0, r3
	ldr	r3, [sp, #136]
	adds	r0, #1
	movs	r4, #14
	ldrsh	r1, [r3, r4]
	ldrb	r3, [r2, #4]
	ldrb	r2, [r2, #5]
	adds	r1, r1, r3
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r1, #1
	b.n	.L_08047278
.L_080471b2:
	cmp	r2, #1
	bne.n	.L_080471ce
	mov	r4, r8
	ldrsb	r3, [r4, r2]
	ldr	r1, [pc, #200]
	lsls	r3, r3, #2
	ldrb	r2, [r1, r3]
	adds	r3, r3, r1
	str	r2, [r4, #8]
	ldrb	r2, [r3, #1]
	ldrb	r3, [r3, #2]
	str	r2, [r4, #24]
	str	r3, [r4, #28]
	b.n	.L_080472da
.L_080471ce:
	cmp	r2, #2
	bne.n	.L_08047228
	mov	r0, r8
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #176]
	lsls	r3, r3, #2
	ldrb	r2, [r1, r3]
	adds	r3, r3, r1
	str	r2, [r0, #8]
	ldrb	r1, [r3, #1]
	ldrb	r2, [r3, #2]
	str	r1, [r0, #24]
	str	r2, [r0, #28]
	ldr	r4, [sp, #40]
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L_08047208
	ldr	r2, [sp, #136]
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r0, #15
	adds	r1, #1
	movs	r2, #10
	b.n	.L_08047278
.L_08047208:
	ldr	r3, [sp, #136]
	movs	r4, #12
	ldrsh	r0, [r3, r4]
	adds	r0, r0, r1
	movs	r4, #14
	ldrsh	r1, [r3, r4]
	movs	r3, #14
	adds	r1, r1, r2
	str	r3, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #2
	movs	r3, #2
	bl	sub_08046134
	b.n	.L_080472da
.L_08047228:
	cmp	r2, #3
	beq.n	.L_08047230
	cmp	r2, #5
	bne.n	.L_0804728c
.L_08047230:
	mov	r0, r8
	ldrsb	r1, [r0, r2]
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_0804723c
	adds	r3, r1, #3
.L_0804723c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r1, r3
	lsls	r3, r3, #24
	asrs	r3, r3, #23
	adds	r3, #8
	mov	r1, r8
	str	r3, [r1, #28]
	cmp	r2, #3
	bne.n	.L_08047258
	movs	r3, #2
	negs	r3, r3
	str	r3, [r1, #24]
	b.n	.L_08047260
.L_08047258:
	movs	r3, #1
	negs	r3, r3
	mov	r2, r8
	str	r3, [r2, #24]
.L_08047260:
	ldr	r4, [sp, #136]
	movs	r3, #12
	ldrsh	r0, [r4, r3]
	movs	r2, #14
	ldrsh	r1, [r4, r2]
	ldr	r3, [sp, #464]
	adds	r0, #1
	adds	r1, r1, r3
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r1, #1
	movs	r2, #20
.L_08047278:
	movs	r3, #1
	bl	sub_08046134
	b.n	.L_080472da
	.4byte 0x0805f7d8
	.4byte 0x0805f858
	.2byte 0xf868
	.2byte 0x0805
.L_0804728c:
	mov	r3, r8
	movs	r6, #4
	ldrsb	r6, [r3, r6]
	movs	r1, #3
	adds	r0, r6, #0
	bl	sub_08002064
	adds	r5, r0, #0
	lsls	r5, r5, #24
	asrs	r5, r5, #23
	adds	r5, #9
	mov	r4, r8
	movs	r1, #3
	str	r5, [r4, #28]
	adds	r0, r6, #0
	bl	sub_08002054
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	lsls	r3, r0, #3
	subs	r3, r3, r0
	subs	r3, #2
	mov	r0, r8
	str	r3, [r0, #24]
	ldr	r2, [sp, #136]
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	adds	r0, r0, r3
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	movs	r3, #14
	adds	r1, r1, r5
	str	r3, [sp, #0]
	adds	r0, #3
	adds	r1, #1
	movs	r2, #7
	movs	r3, #1
	bl	sub_08046134
.L_080472da:
	add	r1, sp, #436
	ldr	r3, [r1, #12]
	ldr	r2, [r1, #8]
	cmp	r3, r2
	beq.n	.L_080472ee
	str	r2, [r1, #12]
	ldr	r4, [sp, #132]
	movs	r3, #2
	orrs	r4, r3
	str	r4, [sp, #132]
.L_080472ee:
	ldr	r3, [r1, #24]
	ldr	r0, [sp, #96]
	lsls	r3, r3, #3
	str	r3, [sp, #112]
	ldr	r3, [r1, #28]
	ldr	r1, [sp, #144]
	lsls	r3, r3, #3
	str	r3, [sp, #116]
	cmp	r0, r1
	beq.n	.L_08047326
	add	r2, sp, #144
	ldrh	r2, [r2, #0]
	add	r5, sp, #412
	movs	r3, #255
	strh	r2, [r5, #0]
	strh	r3, [r5, #2]
	ldr	r0, [sp, #96]
	bl	sub_08118120
	ldr	r3, [sp, #144]
	adds	r0, r5, #0
	movs	r1, #1
	movs	r2, #0
	str	r3, [sp, #96]
	bl	sub_08118148
	movs	r4, #0
	str	r4, [sp, #88]
.L_08047326:
	ldr	r0, [sp, #144]
	bl	sub_08118098
	ldr	r3, [r0, #0]
	ldr	r0, [sp, #28]
	ldr	r2, [r3, #80]
	ldr	r3, [pc, #96]
	str	r3, [r0, #4]
	movs	r3, #0
	str	r3, [r0, #8]
	cmp	r2, #0
	beq.n	.L_08047374
	ldrh	r1, [r0, #8]
	ldrh	r2, [r2, #8]
	ldr	r3, [pc, #84]
	lsls	r2, r2, #22
	ands	r3, r1
	ldr	r1, [sp, #28]
	lsrs	r2, r2, #22
	orrs	r3, r2
	strh	r3, [r1, #8]
	ldrh	r2, [r1, #6]
	ldr	r3, [pc, #72]
	ands	r3, r2
	ldr	r2, [pc, #52]
	orrs	r3, r2
	ldr	r2, [sp, #28]
	strh	r3, [r2, #6]
	movs	r3, #56
	strb	r3, [r2, #4]
	ldr	r3, [sp, #88]
	cmp	r3, #0
	beq.n	.L_08047370
	ldr	r0, [sp, #28]
	movs	r1, #240
	bl	sub_08014128
.L_08047370:
	movs	r4, #1
	str	r4, [sp, #88]
.L_08047374:
	ldr	r0, [sp, #32]
	ldr	r3, [pc, #40]
	str	r3, [r0, #4]
	movs	r3, #0
	str	r3, [r0, #8]
	ldr	r0, [sp, #120]
	ldr	r1, [pc, #32]
	bl	sub_080143f8
	ldr	r3, [pc, #8]
	b.n	.L_080473a8
	movs	r0, r0
	.4byte 0x000000ac
	.4byte 0x000003ff
	.4byte 0xc0002400
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.4byte 0x40000400
	.2byte 0x97f8
	.2byte 0x0805
.L_080473a8:
	ldr	r1, [sp, #32]
	ands	r0, r3
	ldrh	r2, [r1, #8]
	ldr	r3, [pc, #140]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #132]
	ldr	r0, [sp, #136]
	ldr	r2, [r3, #0]
	movs	r3, #4
	ands	r2, r3
	movs	r4, #12
	ldrsh	r3, [r0, r4]
	ldr	r1, [sp, #112]
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r1, r3, #0
	ldr	r3, [sp, #92]
	adds	r1, #16
	cmp	r3, #0
	bne.n	.L_080473e4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080473de
	adds	r3, r2, #3
.L_080473de:
	asrs	r3, r3, #2
	subs	r3, r1, r3
	b.n	.L_080473f0
.L_080473e4:
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_080473ec
	adds	r3, #3
.L_080473ec:
	asrs	r3, r3, #2
	adds	r3, r1, r3
.L_080473f0:
	ldr	r4, [sp, #32]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ldrh	r1, [r4, #6]
	ands	r2, r3
	ldr	r3, [pc, #68]
	adds	r0, r4, #0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r2, [sp, #136]
	ldr	r4, [sp, #116]
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	lsls	r3, r3, #3
	adds	r1, r4, r3
	ldr	r3, [pc, #44]
	ldr	r2, [r3, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	bge.n	.L_08047420
	adds	r2, #3
.L_08047420:
	asrs	r3, r2, #2
	ldr	r0, [sp, #32]
	subs	r3, r1, r3
	adds	r3, #16
	strb	r3, [r0, #4]
	ldr	r1, [sp, #92]
	cmp	r1, #0
	beq.n	.L_08047448
	ldrb	r2, [r0, #7]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #7]
	b.n	.L_0804745a
	.4byte 0xfffffc00
	.4byte 0x0300122c
	.2byte 0xfe00
	.2byte 0xffff
.L_08047448:
	ldr	r2, [sp, #32]
	ldrb	r3, [r2, #7]
	movs	r2, #63
	negs	r2, r2
	ands	r2, r3
	movs	r3, #16
	orrs	r2, r3
	ldr	r3, [sp, #32]
	strb	r2, [r3, #7]
.L_0804745a:
	ldr	r0, [sp, #32]
	movs	r1, #241
	bl	sub_08014128
	ldr	r4, [sp, #132]
	cmp	r4, #0
	bne.n	.L_0804746c
	bl	.L_0804846e
.L_0804746c:
	ldr	r0, [sp, #144]
	bl	sub_08016ca4
	str	r0, [sp, #64]
	ldr	r0, [sp, #92]
	cmp	r0, #0
	bne.n	.L_0804747e
	bl	sub_080396bc
.L_0804747e:
	ldr	r1, [sp, #92]
	cmp	r1, #3
	beq.n	.L_08047486
	b.n	.L_0804777c
.L_08047486:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #64]
	adds	r3, #150
	mov	fp, r3
	movs	r3, #1
	negs	r3, r3
	mov	r8, r3
	movs	r3, #216
	ldrh	r5, [r4, r3]
	movs	r2, #0
	mov	r9, r2
	cmp	r5, #0
	beq.n	.L_080474d8
	adds	r7, r4, #0
	adds	r7, #216
	mov	r6, fp
.L_080474ac:
	adds	r0, r5, #0
	str	r2, [sp, #8]
	bl	sub_080ad010
	adds	r1, r5, #0
	ldr	r0, [sp, #144]
	bl	.L_080499b4
	ldr	r2, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080474ca
	movs	r0, #1
	strh	r5, [r6, #0]
	add	r9, r0
	adds	r6, #2
.L_080474ca:
	adds	r2, #1
	cmp	r2, #15
	beq.n	.L_080474d8
	adds	r7, #2
	ldrh	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L_080474ac
.L_080474d8:
	ldr	r1, [sp, #64]
	movs	r3, #216
	ldrh	r5, [r1, r3]
	movs	r2, #0
	cmp	r5, #0
	beq.n	.L_0804751c
	mov	r4, r9
	adds	r7, r1, #0
	lsls	r3, r4, #1
	mov	r0, fp
	adds	r7, #216
	adds	r6, r3, r0
.L_080474f0:
	adds	r0, r5, #0
	str	r2, [sp, #8]
	bl	sub_080ad010
	adds	r1, r5, #0
	ldr	r0, [sp, #144]
	bl	.L_080499b4
	ldr	r2, [sp, #8]
	cmp	r0, #0
	beq.n	.L_0804750e
	movs	r1, #1
	strh	r5, [r6, #0]
	add	r9, r1
	adds	r6, #2
.L_0804750e:
	adds	r2, #1
	cmp	r2, #15
	beq.n	.L_0804751c
	adds	r7, #2
	ldrh	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L_080474f0
.L_0804751c:
	mov	r2, r9
	lsls	r3, r2, #1
	ldr	r2, [pc, #40]
	mov	r4, fp
	mov	r0, r9
	strh	r2, [r3, r4]
	str	r0, [sp, #72]
	add	r1, sp, #436
	movs	r3, #5
	ldrsb	r3, [r1, r3]
	adds	r2, r3, #0
	cmp	r9, r3
	bgt.n	.L_0804753e
	mov	r3, r9
	subs	r3, #1
	strb	r3, [r1, #5]
	adds	r2, r3, #0
.L_0804753e:
	lsls	r3, r2, #24
	asrs	r2, r3, #24
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08047550
	adds	r3, r2, #3
	b.n	.L_08047550
	.2byte 0x0000
	.2byte 0x0000
.L_08047550:
	asrs	r3, r3, #2
	lsls	r1, r3, #2
	adds	r3, r1, #0
	subs	r3, r2, r3
	lsls	r3, r3, #24
	mov	r2, r9
	mov	sl, r1
	asrs	r3, r3, #24
	cmp	r2, #0
	beq.n	.L_08047596
	add	r3, sl
	lsls	r3, r3, #1
	mov	r4, fp
	adds	r5, r3, r4
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #144]
	bl	.L_080499b4
	cmp	r0, #2
	bne.n	.L_0804757e
	ldr	r0, [pc, #160]
	add	r1, sp, #284
	b.n	.L_0804758e
.L_0804757e:
	ldrh	r3, [r5, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	ldr	r3, [pc, #148]
	add	r1, sp, #284
	adds	r0, r0, r3
.L_0804758e:
	movs	r2, #32
	bl	sub_0803c9bc
	b.n	.L_080475a0
.L_08047596:
	ldr	r0, [pc, #140]
	add	r1, sp, #284
	movs	r2, #32
	bl	sub_0803c9bc
.L_080475a0:
	ldr	r0, [sp, #136]
	bl	sub_08039418
	ldr	r2, [sp, #128]
	ldr	r4, [sp, #128]
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	ldrh	r3, [r4, #10]
	ldrh	r2, [r2, #8]
	movs	r4, #0
	str	r4, [sp, #0]
	bl	sub_0803a2b0
	bl	sub_080396bc
	movs	r2, #0
	add	r0, sp, #284
	ldr	r1, [sp, #128]
	movs	r3, #20
	bl	sub_0803aae4
	cmp	sl, r8
	bne.n	.L_080475d4
	b.n	.L_080476f0
.L_080475d4:
	movs	r3, #16
	str	r3, [sp, #0]
	ldr	r0, [sp, #136]
	movs	r1, #0
	movs	r3, #29
	movs	r2, #16
	bl	sub_08041c54
	mov	r1, sl
	lsls	r3, r1, #1
	add	r3, fp
	ldrh	r5, [r3, #0]
	movs	r0, #0
	mov	r8, r0
	cmp	r5, #0
	beq.n	.L_08047698
	movs	r2, #64
	adds	r7, r3, #0
	add	r6, sp, #488
	mov	fp, r2
.L_080475fc:
	adds	r0, r5, #0
	bl	sub_080ad010
	movs	r0, #15
	bl	sub_08041f70
	adds	r1, r5, #0
	ldr	r0, [sp, #144]
	bl	.L_080499b4
	cmp	r0, #0
	beq.n	.L_08047628
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_08047638
	.4byte 0x00000d4f
	.4byte 0x00000092
	.2byte 0x0d46
	.2byte 0x0000
.L_08047628:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08047638
	movs	r0, #2
	bl	sub_08041f70
.L_08047638:
	movs	r0, #128
	ldr	r3, [pc, #88]
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r5
	ldr	r1, [sp, #136]
	adds	r0, r0, r3
	movs	r2, #32
	mov	r3, fp
	bl	sub_08042010
	movs	r0, #15
	bl	sub_08041f70
	mov	r4, r8
	add	r2, sp, #420
	lsls	r3, r4, #2
	ldr	r1, [r2, r3]
	adds	r0, r5, #0
	bl	sub_080454a0
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	ands	r0, r3
	ldrh	r3, [r6, #0]
	movs	r1, #1
	ands	r3, r2
	add	r8, r1
	orrs	r3, r0
	mov	r2, r8
	movs	r0, #16
	strh	r3, [r6, #0]
	add	fp, r0
	adds	r6, #12
	cmp	r2, #3
	bgt.n	.L_08047698
	adds	r7, #2
	ldrh	r5, [r7, #0]
	cmp	r5, #0
	bne.n	.L_080475fc
	b.n	.L_08047698
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x025f
	.2byte 0x0000
.L_08047698:
	mov	r3, r8
	cmp	r3, #3
	bgt.n	.L_080476f0
	lsls	r3, r3, #1
	add	r3, r8
	add	r4, sp, #740
	lsls	r3, r3, #2
	mov	r0, r8
	adds	r3, r3, r4
	ldr	r1, [pc, #64]
	adds	r5, r3, #0
	lsls	r3, r0, #2
	adds	r3, r3, r4
	adds	r6, r3, r1
	ldr	r7, [pc, #44]
	movs	r3, #4
	subs	r0, r3, r0
	subs	r5, #252
	mov	r8, r0
.L_080476be:
	movs	r1, #1
	ldmia	r6!, {r0}
	negs	r1, r1
	bl	sub_080143f8
	ldr	r3, [pc, #28]
	movs	r2, #1
	ands	r0, r3
	ldrh	r3, [r5, #0]
	negs	r2, r2
	ands	r3, r7
	orrs	r3, r0
	add	r8, r2
	strh	r3, [r5, #0]
	mov	r3, r8
	adds	r5, #12
	cmp	r3, #0
	bne.n	.L_080476be
	b.n	.L_080476f0
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0xfec0
	.2byte 0xffff
.L_080476f0:
	mov	r4, r9
	cmp	r4, #4
	bgt.n	.L_080476f8
	b.n	.L_08047d78
.L_080476f8:
	movs	r0, #0
	mov	r8, r0
	mov	r5, r9
	adds	r5, #3
	b.n	.L_0804773a
.L_08047702:
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r1, #1
	mov	r3, sl
	add	r1, r8
	cmp	r3, #0
	bge.n	.L_08047712
	adds	r3, #3
.L_08047712:
	asrs	r3, r3, #2
	cmp	r8, r3
	bne.n	.L_08047720
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r1, #11
	add	r1, r8
.L_08047720:
	ldr	r3, [sp, #136]
	ldrh	r2, [r3, #8]
	movs	r3, #0
	subs	r2, r2, r0
	add	r2, r8
	str	r3, [sp, #0]
	subs	r2, #10
	ldr	r0, [sp, #136]
	movs	r3, #7
	bl	sub_0803c378
	movs	r4, #1
	add	r8, r4
.L_0804773a:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08047744
	mov	r3, r9
	adds	r3, #6
.L_08047744:
	asrs	r0, r3, #2
	cmp	r8, r0
	blt.n	.L_08047702
	ldr	r1, [sp, #136]
	movs	r5, #0
	ldrh	r2, [r1, #8]
	movs	r1, #243
	subs	r2, r2, r0
	lsls	r1, r1, #8
	subs	r2, #11
	ldr	r0, [sp, #136]
	adds	r1, #54
	movs	r3, #7
	str	r5, [sp, #0]
	bl	sub_0803c378
	ldr	r3, [sp, #136]
	movs	r1, #243
	ldrh	r2, [r3, #8]
	lsls	r1, r1, #8
	adds	r0, r3, #0
	adds	r1, #55
	subs	r2, #10
	movs	r3, #7
	str	r5, [sp, #0]
	bl	sub_0803c378
	b.n	.L_08047d78
.L_0804777c:
	ldr	r4, [sp, #92]
	cmp	r4, #1
	beq.n	.L_08047784
	b.n	.L_08047ae4
.L_08047784:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	movs	r0, #0
	movs	r1, #1
	adds	r3, #150
	negs	r1, r1
	str	r0, [sp, #56]
	ldr	r0, [sp, #136]
	mov	r8, r1
	str	r3, [sp, #60]
	bl	sub_08039418
	ldr	r2, [sp, #64]
	movs	r3, #88
	ldrh	r3, [r2, r3]
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	adds	r5, r2, #0
	ands	r5, r3
	mov	r9, r8
	movs	r1, #0
	cmp	r5, #0
	beq.n	.L_080477f4
	ldr	r7, [sp, #64]
	ldr	r6, [sp, #60]
	adds	r7, #88
	adds	r4, r2, #0
.L_080477c0:
	adds	r0, r5, #0
	str	r1, [sp, #12]
	str	r4, [sp, #4]
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L_080477e2
	strh	r5, [r6, #0]
	ldr	r3, [sp, #56]
	adds	r6, #2
	adds	r3, #1
	str	r3, [sp, #56]
.L_080477e2:
	adds	r1, #1
	cmp	r1, #32
	beq.n	.L_080477f4
	adds	r7, #4
	ldrh	r3, [r7, #0]
	adds	r5, r4, #0
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_080477c0
.L_080477f4:
	ldr	r4, [sp, #56]
	ldr	r2, [pc, #44]
	ldr	r0, [sp, #60]
	lsls	r3, r4, #1
	strh	r2, [r3, r0]
	str	r4, [sp, #76]
	add	r1, sp, #436
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	ldr	r4, [sp, #56]
	adds	r2, r3, #0
	cmp	r4, r3
	bgt.n	.L_08047816
	adds	r3, r4, #0
	subs	r3, #1
	strb	r3, [r1, #3]
	adds	r2, r3, #0
.L_08047816:
	lsls	r3, r2, #24
	asrs	r2, r3, #24
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08047828
	adds	r3, r2, #3
	b.n	.L_08047828
	.2byte 0x0000
	.2byte 0x0000
.L_08047828:
	asrs	r3, r3, #2
	lsls	r0, r3, #2
	adds	r3, r0, #0
	subs	r3, r2, r3
	lsls	r3, r3, #24
	mov	sl, r0
	asrs	r5, r3, #24
	cmp	sl, r8
	bne.n	.L_08047840
	cmp	r5, r9
	bne.n	.L_08047840
	b.n	.L_08047d78
.L_08047840:
	ldr	r1, [sp, #140]
	movs	r3, #1
	strb	r3, [r1, #6]
	bl	sub_080396bc
	ldr	r2, [sp, #56]
	cmp	r2, #0
	beq.n	.L_08047870
	ldr	r0, [sp, #60]
	mov	r4, sl
	adds	r3, r4, r5
	lsls	r3, r3, #1
	ldrh	r3, [r3, r0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	ldr	r3, [pc, #216]
	add	r1, sp, #156
	adds	r0, r0, r3
	movs	r2, #52
	bl	sub_0803c9bc
	b.n	.L_0804787a
.L_08047870:
	ldr	r0, [pc, #204]
	add	r1, sp, #156
	movs	r2, #52
	bl	sub_0803c9bc
.L_0804787a:
	ldr	r0, [sp, #136]
	bl	sub_08039418
	ldr	r2, [sp, #128]
	ldr	r4, [sp, #128]
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	ldrh	r3, [r4, #10]
	ldrh	r2, [r2, #8]
	movs	r4, #0
	str	r4, [sp, #0]
	bl	sub_0803a2b0
	movs	r2, #0
	add	r0, sp, #156
	ldr	r1, [sp, #128]
	movs	r3, #20
	bl	sub_0803aae4
	cmp	sl, r8
	bne.n	.L_080478aa
	b.n	.L_08047a58
.L_080478aa:
	movs	r3, #16
	str	r3, [sp, #0]
	ldr	r0, [sp, #136]
	movs	r1, #0
	movs	r2, #16
	movs	r3, #29
	bl	sub_08041c54
	ldr	r2, [sp, #60]
	mov	r1, sl
	lsls	r3, r1, #1
	ldrh	r5, [r3, r2]
	movs	r0, #0
	mov	r8, r0
	cmp	r5, #0
	bne.n	.L_080478cc
	b.n	.L_08047a02
.L_080478cc:
	movs	r3, #64
	str	r3, [sp, #16]
	movs	r4, #8
	add	r7, sp, #488
	mov	r9, r4
.L_080478d6:
	adds	r0, r5, #0
	bl	sub_080ad078
	movs	r1, #240
	adds	r6, r0, #0
	lsls	r1, r1, #8
	movs	r0, #0
	str	r0, [sp, #0]
	adds	r1, #31
	ldr	r0, [sp, #136]
	movs	r2, #11
	mov	r3, r9
	bl	sub_0803c378
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r1, #240
	lsls	r1, r1, #8
	ldr	r0, [sp, #136]
	adds	r1, #30
	movs	r2, #12
	mov	r3, r9
	bl	sub_0803c378
	mov	r4, r8
	add	r2, sp, #420
	lsls	r3, r4, #2
	ldr	r1, [r2, r3]
	adds	r0, r5, #0
	bl	sub_080454dc
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #32]
	ands	r0, r3
	ldrh	r3, [r7, #0]
	ands	r3, r2
	ldrb	r2, [r6, #1]
	orrs	r3, r0
	strh	r3, [r7, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08047944
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_0804796c
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x00000885
	.2byte 0x0d48
	.2byte 0x0000
.L_08047944:
	ldr	r1, [sp, #64]
	ldrb	r2, [r6, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	ble.n	.L_08047958
	movs	r0, #2
	bl	sub_08041f70
	b.n	.L_0804796c
.L_08047958:
	ldr	r2, [sp, #64]
	movs	r4, #62
	adds	r4, #255
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804796c
	movs	r0, #9
	bl	sub_08041f70
.L_0804796c:
	ldr	r1, [sp, #140]
	movs	r0, #0
	mov	fp, r0
	ldr	r0, [pc, #84]
	movs	r3, #5
	strb	r3, [r1, #7]
	ldr	r1, [sp, #136]
	ldr	r3, [sp, #16]
	adds	r0, r5, r0
	movs	r2, #16
	bl	sub_08042010
	ldr	r2, [sp, #16]
	ldrb	r0, [r6, #9]
	movs	r3, #104
	str	r2, [sp, #0]
	movs	r1, #2
	ldr	r2, [sp, #136]
	bl	sub_08042214
	movs	r0, #15
	bl	sub_08041f70
	ldr	r4, [sp, #140]
	movs	r3, #15
	strb	r3, [r4, #7]
	ldrb	r3, [r6, #2]
	cmp	r3, #4
	beq.n	.L_080479be
	movs	r0, #160
	lsls	r0, r0, #7
	adds	r1, r3, #0
	adds	r0, #1
	mov	r2, fp
	adds	r1, r1, r0
	str	r2, [sp, #0]
	ldr	r0, [sp, #136]
	movs	r2, #15
	mov	r3, r9
	bl	sub_0803c378
.L_080479be:
	ldrb	r3, [r6, #8]
	cmp	r3, #255
	bne.n	.L_080479cc
	movs	r3, #11
	b.n	.L_080479ce
	.2byte 0x05a7
	.2byte 0x0000
.L_080479cc:
	subs	r3, #1
.L_080479ce:
	mov	r2, r9
	movs	r4, #0
	movs	r1, #16
	ldr	r0, [sp, #136]
	str	r4, [sp, #0]
	bl	sub_0804524c
	ldr	r0, [sp, #16]
	movs	r2, #1
	add	r8, r2
	adds	r0, #16
	movs	r1, #2
	mov	r3, r8
	str	r0, [sp, #16]
	adds	r7, #12
	add	r9, r1
	cmp	r3, #3
	bgt.n	.L_08047a02
	mov	r3, sl
	ldr	r4, [sp, #60]
	add	r3, r8
	lsls	r3, r3, #1
	ldrh	r5, [r3, r4]
	cmp	r5, #0
	beq.n	.L_08047a02
	b.n	.L_080478d6
.L_08047a02:
	mov	r0, r8
	cmp	r0, #3
	bgt.n	.L_08047a58
	lsls	r3, r0, #1
	add	r3, r8
	add	r1, sp, #740
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldr	r2, [pc, #64]
	adds	r5, r3, #0
	lsls	r3, r0, #2
	adds	r3, r3, r1
	adds	r6, r3, r2
	ldr	r7, [pc, #44]
	movs	r3, #4
	subs	r0, r3, r0
	subs	r5, #252
	mov	r8, r0
.L_08047a26:
	movs	r1, #1
	ldmia	r6!, {r0}
	negs	r1, r1
	bl	sub_080143f8
	ldr	r3, [pc, #28]
	ands	r0, r3
	ldrh	r3, [r5, #0]
	ands	r3, r7
	orrs	r3, r0
	strh	r3, [r5, #0]
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r4, r8
	adds	r5, #12
	cmp	r4, #0
	bne.n	.L_08047a26
	b.n	.L_08047a58
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0xfec0
	.2byte 0xffff
.L_08047a58:
	ldr	r0, [sp, #56]
	cmp	r0, #4
	bgt.n	.L_08047a60
	b.n	.L_08047d6e
.L_08047a60:
	movs	r1, #0
	mov	r8, r1
	adds	r5, r0, #0
	adds	r5, #3
	b.n	.L_08047aa2
.L_08047a6a:
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r1, #1
	mov	r3, sl
	add	r1, r8
	cmp	r3, #0
	bge.n	.L_08047a7a
	adds	r3, #3
.L_08047a7a:
	asrs	r3, r3, #2
	cmp	r8, r3
	bne.n	.L_08047a88
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r1, #11
	add	r1, r8
.L_08047a88:
	ldr	r3, [sp, #136]
	ldrh	r2, [r3, #8]
	movs	r3, #0
	subs	r2, r2, r0
	add	r2, r8
	str	r3, [sp, #0]
	subs	r2, #10
	ldr	r0, [sp, #136]
	movs	r3, #7
	bl	sub_0803c378
	movs	r4, #1
	add	r8, r4
.L_08047aa2:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08047aac
	ldr	r3, [sp, #56]
	adds	r3, #6
.L_08047aac:
	asrs	r0, r3, #2
	cmp	r8, r0
	blt.n	.L_08047a6a
	ldr	r1, [sp, #136]
	movs	r5, #0
	ldrh	r2, [r1, #8]
	movs	r1, #243
	subs	r2, r2, r0
	lsls	r1, r1, #8
	subs	r2, #11
	ldr	r0, [sp, #136]
	adds	r1, #54
	movs	r3, #7
	str	r5, [sp, #0]
	bl	sub_0803c378
	ldr	r3, [sp, #136]
	movs	r1, #243
	ldrh	r2, [r3, #8]
	lsls	r1, r1, #8
	adds	r0, r3, #0
	adds	r1, #55
	subs	r2, #10
	movs	r3, #7
	str	r5, [sp, #0]
	bl	sub_0803c378
	b.n	.L_08047d6e
.L_08047ae4:
	ldr	r0, [sp, #92]
	cmp	r0, #2
	beq.n	.L_08047aec
	b.n	.L_08047d78
.L_08047aec:
	movs	r2, #1
	movs	r1, #0
	negs	r2, r2
	ldr	r0, [sp, #136]
	mov	sl, r1
	mov	fp, r2
	mov	r9, r1
	bl	sub_08039418
	ldr	r4, [sp, #128]
	movs	r2, #14
	ldrsh	r1, [r4, r2]
	movs	r3, #12
	ldrsh	r0, [r4, r3]
	ldrh	r2, [r4, #8]
	ldrh	r3, [r4, #10]
	mov	r4, sl
	str	r4, [sp, #0]
	bl	sub_0803a2b0
	movs	r3, #16
	str	r3, [sp, #0]
	ldr	r0, [sp, #136]
	movs	r1, #0
	movs	r2, #16
	movs	r3, #29
	bl	sub_08041c54
	movs	r0, #0
	ldr	r7, [sp, #64]
	str	r0, [sp, #80]
	mov	r8, r0
	adds	r7, #248
.L_08047b2e:
	ldr	r1, [sp, #80]
	ldr	r2, [sp, #84]
	lsls	r3, r1, #2
	movs	r6, #0
	adds	r5, r3, r2
.L_08047b38:
	ldr	r3, [r7, #16]
	movs	r2, #1
	lsls	r2, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08047b54
	mov	r4, r8
	lsls	r3, r4, #8
	orrs	r3, r6
	stmia	r5!, {r3}
	ldr	r0, [sp, #80]
	adds	r0, #1
	str	r0, [sp, #80]
	b.n	.L_08047bec
.L_08047b54:
	ldr	r3, [r7, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08047bec
	ldr	r1, [sp, #144]
	movs	r0, #0
	cmp	r1, #7
	bls.n	.L_08047b66
	movs	r0, #1
.L_08047b66:
	bl	sub_080ad000
	movs	r2, #148
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r1, r3, #0
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r0, #0
	adds	r1, #8
	movs	r4, #0
	cmp	r0, r3
	bge.n	.L_08047bc4
	ldrb	r3, [r1, #2]
	ldr	r4, [sp, #144]
	cmp	r3, r4
	bne.n	.L_08047b94
	ldrb	r3, [r1, #0]
	cmp	r3, r8
	bne.n	.L_08047b94
	ldrb	r3, [r1, #1]
	cmp	r3, r6
	beq.n	.L_08047bbe
.L_08047b94:
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	adds	r0, #1
	cmp	r0, r3
	bge.n	.L_08047bc2
	lsls	r4, r0, #2
	adds	r2, r1, r4
	ldrb	r3, [r2, #2]
	mov	ip, r3
	ldr	r3, [sp, #144]
	cmp	ip, r3
	bne.n	.L_08047b94
	ldrb	r3, [r2, #0]
	cmp	r3, r8
	bne.n	.L_08047b94
	ldrb	r3, [r2, #1]
	cmp	r3, r6
	bne.n	.L_08047b94
	b.n	.L_08047bc4
.L_08047bbe:
	movs	r4, #0
	b.n	.L_08047bc4
.L_08047bc2:
	lsls	r4, r0, #2
.L_08047bc4:
	mov	r0, r8
	lsls	r2, r0, #8
	movs	r3, #128
	lsls	r3, r3, #9
	orrs	r2, r6
	orrs	r2, r3
	str	r2, [r5, #0]
	adds	r3, r1, r4
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08047be4
	lsls	r3, r3, #17
	orrs	r2, r3
	str	r2, [r5, #0]
.L_08047be4:
	ldr	r1, [sp, #80]
	adds	r5, #4
	adds	r1, #1
	str	r1, [sp, #80]
.L_08047bec:
	adds	r6, #1
	cmp	r6, #19
	ble.n	.L_08047b38
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #4
	cmp	r3, #3
	ble.n	.L_08047b2e
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #84]
	movs	r2, #128
	lsls	r3, r4, #2
	lsls	r2, r2, #24
	str	r2, [r3, r0]
	add	r1, sp, #436
	movs	r3, #4
	ldrsb	r3, [r1, r3]
	adds	r2, r3, #0
	cmp	r3, r4
	blt.n	.L_08047c1e
	adds	r3, r4, #0
	subs	r3, #1
	strb	r3, [r1, #4]
	adds	r2, r3, #0
.L_08047c1e:
	ldr	r1, [sp, #84]
	lsls	r3, r2, #24
	ldr	r2, [sp, #140]
	asrs	r3, r3, #22
	ldr	r5, [r3, r1]
	movs	r3, #1
	strb	r3, [r2, #6]
	ldr	r3, [sp, #80]
	cmp	r3, #0
	beq.n	.L_08047c7a
	bl	sub_0803cca8
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08047c58
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r5
	cmp	r0, #0
	beq.n	.L_08047c54
	lsrs	r0, r0, #17
	movs	r1, #5
	bl	sub_0803ccd0
	b.n	.L_08047c58
.L_08047c54:
	ldr	r0, [pc, #128]
	b.n	.L_08047c70
.L_08047c58:
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	ldr	r3, [pc, #112]
	lsls	r0, r0, #2
	adds	r0, r0, r5
	adds	r0, r0, r3
.L_08047c70:
	add	r1, sp, #156
	movs	r2, #32
	bl	sub_0803c9bc
	b.n	.L_08047c84
.L_08047c7a:
	ldr	r0, [pc, #100]
	add	r1, sp, #156
	movs	r2, #32
	bl	sub_0803c9bc
.L_08047c84:
	ldr	r0, [sp, #140]
	mov	r4, r9
	strb	r4, [r0, #6]
	bl	sub_080396bc
	movs	r2, #0
	add	r0, sp, #156
	ldr	r1, [sp, #128]
	movs	r3, #20
	bl	sub_0803aae4
	cmp	sl, fp
	beq.n	.L_08047d6e
	ldr	r2, [sp, #84]
	movs	r1, #0
	mov	r8, r1
	str	r2, [sp, #20]
	b.n	.L_08047d40
.L_08047ca8:
	movs	r1, #240
	lsls	r1, r1, #4
	movs	r3, #160
	ands	r1, r7
	lsls	r3, r3, #7
	adds	r3, #1
	lsrs	r1, r1, #8
	adds	r1, r1, r3
	movs	r3, #0
	str	r3, [sp, #0]
	ldr	r0, [sp, #136]
	adds	r3, r5, #0
	adds	r2, r6, #0
	bl	sub_0803c378
	movs	r3, #248
	lsls	r3, r3, #14
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08047ce4
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_08047cf4
	.4byte 0x00000cf9
	.4byte 0x000009b1
	.2byte 0x0d4e
	.2byte 0x0000
.L_08047ce4:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08047cf4
	movs	r0, #2
	bl	sub_08041f70
.L_08047cf4:
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r7
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r3, r7
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #832]
	lsls	r6, r6, #3
	adds	r2, r6, #0
	lsls	r5, r5, #3
	adds	r0, r0, r3
	adds	r2, #8
	ldr	r1, [sp, #136]
	adds	r3, r5, #0
	bl	sub_08042010
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r7
	cmp	r0, #0
	beq.n	.L_08047d36
	adds	r3, r6, #0
	lsrs	r0, r0, #17
	adds	r3, #40
	movs	r1, #2
	ldr	r2, [sp, #136]
	str	r5, [sp, #0]
	bl	sub_08042214
.L_08047d36:
	movs	r0, #15
	bl	sub_08041f70
	movs	r4, #1
	add	r8, r4
.L_08047d40:
	mov	r0, r8
	cmp	r0, #8
	bgt.n	.L_08047d6e
	ldr	r2, [sp, #20]
	ldmia	r2!, {r7}
	adds	r1, r2, #0
	str	r1, [sp, #20]
	movs	r1, #3
	bl	sub_08002054
	lsls	r3, r0, #3
	subs	r6, r3, r0
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	movs	r3, #128
	lsls	r0, r0, #1
	adds	r5, r0, #0
	lsls	r3, r3, #24
	adds	r5, #9
	cmp	r7, r3
	bne.n	.L_08047ca8
.L_08047d6e:
	ldr	r4, [sp, #140]
	movs	r2, #0
	movs	r3, #1
	strb	r3, [r4, #3]
	strb	r2, [r4, #6]
.L_08047d78:
	ldr	r0, [sp, #132]
	cmp	r0, #0
	bne.n	.L_08047d80
	b.n	.L_080482b2
.L_08047d80:
	ldr	r1, [sp, #92]
	cmp	r1, #0
	bne.n	.L_08047db2
	ldr	r0, [sp, #136]
	bl	sub_08039418
	ldr	r3, [sp, #128]
	movs	r4, #14
	ldrsh	r1, [r3, r4]
	ldr	r4, [sp, #92]
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	ldrh	r2, [r3, #8]
	ldrh	r3, [r3, #10]
	str	r4, [sp, #0]
	bl	sub_0803a2b0
	movs	r3, #14
	str	r3, [sp, #0]
	ldr	r0, [sp, #136]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #29
	bl	sub_08041c54
.L_08047db2:
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #136]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080420fc
	ldr	r0, [sp, #124]
	cmp	r0, #3
	ble.n	.L_08047dd0
	ldr	r0, [pc, #648]
	ldr	r1, [sp, #136]
	movs	r2, #40
	movs	r3, #16
	bl	sub_08042010
.L_08047dd0:
	ldr	r1, [sp, #136]
	ldr	r0, [pc, #640]
	movs	r2, #56
	movs	r3, #0
	bl	sub_080420fc
	ldr	r1, [sp, #64]
	movs	r3, #0
	ldr	r2, [sp, #136]
	ldrb	r0, [r1, #15]
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r3, #72
	bl	sub_08042244
	ldr	r7, [pc, #616]
	ldr	r1, [sp, #136]
	adds	r0, r7, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08042010
	ldr	r2, [sp, #64]
	movs	r4, #146
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	movs	r3, #8
	str	r3, [sp, #0]
	ldr	r2, [sp, #136]
	movs	r1, #8
	movs	r3, #40
	bl	sub_08042244
	ldr	r0, [pc, #580]
	ldr	r1, [sp, #136]
	movs	r2, #40
	movs	r3, #24
	bl	sub_080420fc
	ldr	r2, [sp, #64]
	movs	r3, #24
	movs	r1, #56
	ldrsh	r0, [r2, r1]
	mov	r8, r3
	str	r3, [sp, #0]
	ldr	r2, [sp, #136]
	movs	r1, #4
	movs	r3, #56
	bl	sub_08042244
	ldr	r5, [pc, #552]
	ldr	r1, [sp, #136]
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #24
	bl	sub_080420fc
	ldr	r1, [sp, #64]
	mov	r2, r8
	movs	r4, #52
	ldrsh	r0, [r1, r4]
	movs	r3, #96
	str	r2, [sp, #0]
	movs	r1, #4
	ldr	r2, [sp, #136]
	bl	sub_08042244
	movs	r3, #32
	ldr	r1, [sp, #136]
	ldr	r0, [pc, #516]
	movs	r2, #40
	bl	sub_080420fc
	ldr	r4, [sp, #64]
	ldr	r2, [sp, #136]
	movs	r3, #58
	ldrsh	r0, [r4, r3]
	movs	r6, #32
	movs	r1, #4
	movs	r3, #56
	str	r6, [sp, #0]
	bl	sub_08042244
	adds	r0, r5, #0
	ldr	r1, [sp, #136]
	movs	r2, #88
	movs	r3, #32
	bl	sub_080420fc
	ldr	r2, [sp, #64]
	movs	r3, #96
	movs	r1, #54
	ldrsh	r0, [r2, r1]
	ldr	r2, [sp, #136]
	movs	r1, #4
	str	r6, [sp, #0]
	bl	sub_08042244
	adds	r0, r7, #0
	ldr	r1, [sp, #136]
	subs	r0, #10
	movs	r2, #136
	movs	r3, #16
	bl	sub_08042010
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #136]
	ldrh	r0, [r3, #60]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r3, #184
	bl	sub_08042244
	adds	r0, r7, #0
	ldr	r1, [sp, #136]
	subs	r0, #9
	movs	r2, #136
	movs	r3, #24
	bl	sub_08042010
	ldr	r4, [sp, #64]
	mov	r1, r8
	ldrh	r0, [r4, #62]
	ldr	r2, [sp, #136]
	str	r1, [sp, #0]
	movs	r3, #184
	movs	r1, #3
	bl	sub_08042244
	adds	r0, r7, #0
	ldr	r1, [sp, #136]
	subs	r0, #8
	movs	r2, #136
	movs	r3, #32
	bl	sub_08042010
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #136]
	adds	r3, #64
	ldrh	r0, [r3, #0]
	movs	r1, #3
	movs	r3, #184
	str	r6, [sp, #0]
	bl	sub_08042244
	subs	r0, r7, #7
	ldr	r1, [sp, #136]
	movs	r2, #136
	movs	r3, #40
	bl	sub_08042010
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #136]
	adds	r3, #66
	ldrb	r0, [r3, #0]
	movs	r3, #40
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r3, #184
	bl	sub_08042244
	ldr	r2, [sp, #64]
	movs	r4, #42
	adds	r4, #255
	adds	r3, r2, r4
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #324]
	ldr	r1, [sp, #136]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #48
	bl	sub_08042010
	ldr	r0, [sp, #92]
	cmp	r0, #0
	bne.n	.L_08048032
	ldr	r1, [sp, #100]
	cmp	r1, #0
	beq.n	.L_08047f46
	subs	r0, r7, #1
	ldr	r1, [sp, #136]
	movs	r2, #0
	movs	r3, #72
	bl	sub_08042010
.L_08047f46:
	ldr	r1, [sp, #136]
	subs	r0, r7, #5
	movs	r2, #0
	movs	r3, #80
	bl	sub_08042010
	subs	r0, r7, #4
	ldr	r1, [sp, #136]
	movs	r2, #0
	movs	r3, #88
	bl	sub_08042010
	subs	r0, r7, #3
	ldr	r1, [sp, #136]
	movs	r2, #0
	movs	r3, #96
	bl	sub_08042010
	ldr	r0, [sp, #64]
	movs	r2, #0
	movs	r1, #140
	mov	r8, r2
	movs	r3, #72
	movs	r4, #40
	lsls	r1, r1, #1
	movs	r2, #7
	mov	fp, r3
	mov	sl, r4
	adds	r6, r0, r1
	movs	r7, #48
	mov	r9, r2
.L_08047f84:
	ldr	r4, [sp, #100]
	movs	r3, #1
	cmp	r4, #0
	beq.n	.L_08047f8e
	movs	r3, #0
.L_08047f8e:
	movs	r1, #160
	lsls	r1, r1, #7
	mov	r0, r8
	movs	r2, #0
	adds	r1, #1
	add	r1, r8
	str	r2, [sp, #0]
	lsls	r5, r0, #2
	adds	r3, #8
	ldr	r0, [sp, #136]
	mov	r2, r9
	bl	sub_0803c378
	ldr	r1, [sp, #100]
	cmp	r1, #0
	beq.n	.L_08047fdc
	mov	r2, fp
	ldrb	r0, [r6, #4]
	movs	r1, #1
	str	r2, [sp, #0]
	mov	r3, sl
	ldr	r2, [sp, #136]
	bl	sub_08042244
	ldr	r0, [pc, #160]
	ldr	r1, [sp, #136]
	adds	r2, r7, #0
	movs	r3, #72
	bl	sub_080420fc
	adds	r3, r7, #0
	ldrb	r0, [r6, #0]
	mov	r4, fp
	adds	r3, #8
	movs	r1, #1
	ldr	r2, [sp, #136]
	str	r4, [sp, #0]
	bl	sub_08042244
.L_08047fdc:
	ldr	r0, [sp, #144]
	mov	r1, r8
	bl	sub_080ad1a0
	movs	r3, #80
	ldr	r2, [sp, #136]
	str	r3, [sp, #0]
	movs	r1, #2
	adds	r3, r7, #0
	bl	sub_08042244
	ldr	r1, [sp, #64]
	adds	r5, #72
	movs	r3, #88
	ldrsh	r0, [r1, r5]
	str	r3, [sp, #0]
	ldr	r2, [sp, #136]
	mov	r3, sl
	movs	r1, #3
	bl	sub_08042244
	ldr	r3, [sp, #64]
	movs	r1, #3
	adds	r5, r3, r5
	movs	r3, #96
	movs	r4, #2
	ldrsh	r0, [r5, r4]
	ldr	r2, [sp, #136]
	str	r3, [sp, #0]
	mov	r3, sl
	bl	sub_08042244
	movs	r2, #1
	add	r8, r2
	movs	r0, #32
	movs	r1, #4
	mov	r3, r8
	add	sl, r0
	adds	r6, #1
	adds	r7, #32
	add	r9, r1
	cmp	r3, #3
	ble.n	.L_08047f84
.L_08048032:
	ldr	r0, [sp, #64]
	movs	r6, #0
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	bne.n	.L_08048046
	ldr	r1, [sp, #40]
	movs	r3, #16
	strb	r3, [r1, #0]
	movs	r6, #1
.L_08048046:
	ldr	r3, [sp, #40]
	adds	r2, r6, r3
	b.n	.L_08048200
	.4byte 0x000006d3
	.4byte 0x00000d1f
	.4byte 0x0805f888
	.4byte 0x00000d1a
	.4byte 0x0805f88c
	.4byte 0x0805f890
	.4byte 0x0805f894
	.2byte 0x0b63
	.2byte 0x0000
.L_0804806c:
	ldr	r4, [sp, #64]
	movs	r0, #50
	adds	r0, #255
	adds	r1, r4, r0
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_08048082
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048082:
	cmp	r6, #7
	ble.n	.L_08048088
	b.n	.L_0804823a
.L_08048088:
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #2
	bne.n	.L_08048096
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048096:
	cmp	r6, #7
	ble.n	.L_0804809c
	b.n	.L_0804823a
.L_0804809c:
	ldr	r1, [sp, #64]
	movs	r4, #62
	adds	r4, #255
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080480b2
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080480b2:
	cmp	r6, #7
	ble.n	.L_080480b8
	b.n	.L_0804823a
.L_080480b8:
	ldr	r0, [sp, #64]
	movs	r1, #60
	adds	r1, #255
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080480ce
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080480ce:
	cmp	r6, #7
	ble.n	.L_080480d4
	b.n	.L_0804823a
.L_080480d4:
	ldr	r4, [sp, #64]
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080480ea
	movs	r3, #5
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080480ea:
	cmp	r6, #7
	ble.n	.L_080480f0
	b.n	.L_0804823a
.L_080480f0:
	ldr	r1, [sp, #64]
	movs	r4, #160
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08048106
	movs	r3, #7
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048106:
	cmp	r6, #7
	ble.n	.L_0804810c
	b.n	.L_0804823a
.L_0804810c:
	ldr	r0, [sp, #64]
	movs	r1, #156
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08048122
	movs	r3, #6
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048122:
	cmp	r6, #7
	ble.n	.L_08048128
	b.n	.L_0804823a
.L_08048128:
	ldr	r4, [sp, #64]
	movs	r0, #153
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804815e
	movs	r1, #52
	adds	r1, #255
	adds	r3, r4, r1
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08048150
	movs	r3, #9
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048150:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_0804815e
	movs	r3, #10
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_0804815e:
	cmp	r6, #7
	bgt.n	.L_0804823a
	ldr	r4, [sp, #64]
	movs	r0, #154
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08048198
	movs	r1, #54
	adds	r1, #255
	adds	r3, r4, r1
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_0804818a
	movs	r3, #11
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_0804818a:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08048198
	movs	r3, #12
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048198:
	cmp	r6, #7
	bgt.n	.L_0804823a
	ldr	r4, [sp, #64]
	movs	r0, #155
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080481d2
	movs	r1, #56
	adds	r1, #255
	adds	r3, r4, r1
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080481c4
	movs	r3, #13
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080481c4:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_080481d2
	movs	r3, #14
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080481d2:
	cmp	r6, #7
	bgt.n	.L_0804823a
	ldr	r4, [sp, #64]
	movs	r0, #72
	adds	r0, #255
	adds	r3, r4, r0
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080481f2
	movs	r3, #17
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080481f2:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_0804823a
	movs	r3, #18
	strb	r3, [r2, #0]
	adds	r6, #1
	b.n	.L_0804823a
.L_08048200:
	ldr	r1, [sp, #64]
	movs	r4, #152
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0804821a
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_0804821a:
	cmp	r6, #7
	bgt.n	.L_0804823a
	ldr	r0, [sp, #64]
	movs	r1, #66
	adds	r1, #255
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08048234
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08048234:
	cmp	r6, #7
	bgt.n	.L_0804823a
	b.n	.L_0804806c
.L_0804823a:
	cmp	r6, #0
	ble.n	.L_0804825e
	ldr	r7, [sp, #36]
	ldr	r5, [sp, #40]
	mov	r8, r6
.L_08048244:
	ldrb	r0, [r5, #0]
	ldmia	r7!, {r1}
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	bl	sub_08045464
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	adds	r5, #1
	cmp	r3, #0
	bne.n	.L_08048244
.L_0804825e:
	cmp	r6, #0
	bne.n	.L_08048268
	ldr	r4, [sp, #40]
	strb	r6, [r4, #0]
	movs	r6, #1
.L_08048268:
	cmp	r6, #10
	bgt.n	.L_0804828c
	ldr	r3, [sp, #40]
	movs	r0, #1
	adds	r2, r6, r3
	movs	r3, #11
	negs	r0, r0
	subs	r3, r3, r6
	adds	r1, r0, #0
	mov	r8, r3
.L_0804827c:
	movs	r4, #1
	negs	r4, r4
	add	r8, r4
	mov	r0, r8
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r0, #0
	bne.n	.L_0804827c
.L_0804828c:
	str	r6, [sp, #104]
	ldr	r1, [sp, #40]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080482b2
	ldr	r4, [sp, #64]
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	cmp	r3, #0
	beq.n	.L_080482a6
	ldr	r0, [pc, #392]
	b.n	.L_080482a8
.L_080482a6:
	ldr	r0, [pc, #392]
.L_080482a8:
	ldr	r1, [sp, #136]
	movs	r2, #112
	movs	r3, #0
	bl	sub_08042010
.L_080482b2:
	ldr	r0, [sp, #92]
	cmp	r0, #0
	beq.n	.L_080482ba
	b.n	.L_08048464
.L_080482ba:
	movs	r0, #128
	lsls	r0, r0, #1
	bl	sub_08014d78
	ldr	r3, [sp, #444]
	str	r0, [sp, #52]
	cmp	r3, #13
	bhi.n	.L_080482cc
	b.n	.L_080483f6
.L_080482cc:
	ldr	r1, [sp, #40]
	subs	r3, #14
	ldrsb	r3, [r1, r3]
	str	r3, [sp, #48]
	cmp	r3, #0
	bne.n	.L_080482e6
	ldr	r4, [sp, #64]
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	cmp	r3, #0
	bne.n	.L_080482e6
	movs	r0, #16
	str	r0, [sp, #48]
.L_080482e6:
	movs	r1, #166
	lsls	r1, r1, #1
	mov	r8, r1
	mov	r0, r8
	bl	sub_08014d78
	mov	r2, r8
	ldr	r1, [sp, #64]
	ldr	r3, [pc, #316]
	adds	r6, r0, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c10
	ldr	r0, [sp, #64]
	ldr	r5, [sp, #64]
	ldrh	r4, [r4, #60]
	ldrh	r0, [r0, #62]
	adds	r5, #64
	ldr	r2, [sp, #64]
	ldrh	r1, [r5, #0]
	mov	sl, r4
	movs	r4, #52
	mov	fp, r0
	adds	r4, #255
	movs	r0, #54
	adds	r3, r2, r4
	movs	r7, #0
	mov	r9, r1
	adds	r0, #255
	movs	r1, #72
	strb	r7, [r3, #0]
	adds	r1, #255
	adds	r3, r2, r0
	strb	r7, [r3, #0]
	adds	r3, r2, r1
	strb	r7, [r3, #0]
	ldr	r0, [sp, #144]
	bl	sub_080ad008
	ldr	r2, [sp, #64]
	mov	r4, sl
	ldrh	r3, [r2, #60]
	mov	r0, fp
	subs	r4, r4, r3
	ldrh	r3, [r2, #62]
	mov	r1, r9
	subs	r0, r0, r3
	ldrh	r3, [r5, #0]
	mov	fp, r0
	subs	r1, r1, r3
	adds	r0, r2, #0
	ldr	r3, [pc, #228]
	mov	r2, r8
	mov	r9, r1
	adds	r1, r6, #0
	mov	sl, r4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	ldr	r3, [sp, #48]
	subs	r3, #8
	cmp	r3, #10
	bhi.n	.L_080483de
	ldr	r2, [pc, #204]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0804839c
	.4byte 0x080483a8
	.4byte 0x080483ac
	.4byte 0x080483b2
	.4byte 0x080483b6
	.4byte 0x080483c6
	.4byte 0x080483c6
	.4byte 0x080483de
	.4byte 0x080483de
	.4byte 0x080483bc
	.2byte 0x83c0
	.2byte 0x0804
	ldr	r4, [sp, #64]
	movs	r0, #66
	adds	r0, #255
	adds	r3, r4, r0
	ldrb	r7, [r3, #0]
	b.n	.L_080483de
	mov	r7, sl
	b.n	.L_080483de
	mov	r1, sl
	negs	r7, r1
	b.n	.L_080483de
	mov	r7, fp
	b.n	.L_080483de
	mov	r2, fp
	negs	r7, r2
	b.n	.L_080483de
	mov	r7, r9
	b.n	.L_080483de
	mov	r3, r9
	negs	r7, r3
	b.n	.L_080483de
	ldr	r4, [sp, #64]
	movs	r0, #56
	adds	r0, #255
	adds	r3, r4, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r7, r3, #2
	cmp	r7, #0
	bge.n	.L_080483de
	negs	r7, r7
.L_080483de:
	adds	r0, r7, #0
	movs	r1, #5
	bl	sub_0803ccd0
	ldr	r0, [pc, #84]
	movs	r2, #128
	ldr	r1, [sp, #48]
	adds	r0, r1, r0
	ldr	r1, [sp, #52]
	bl	sub_0803c9bc
	b.n	.L_08048452
.L_080483f6:
	cmp	r3, #2
	bne.n	.L_08048444
	ldr	r2, [sp, #64]
	ldrb	r3, [r2, #15]
	cmp	r3, #98
	bhi.n	.L_08048444
	adds	r1, r3, #0
	adds	r1, #1
	ldr	r0, [sp, #144]
	bl	sub_080ad200
	ldr	r4, [sp, #64]
	movs	r1, #146
	lsls	r1, r1, #1
	adds	r3, r4, r1
	ldr	r3, [r3, #0]
	movs	r1, #5
	subs	r0, r0, r3
	bl	sub_0803ccd0
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #52]
	movs	r2, #128
	bl	sub_0803c9bc
	b.n	.L_08048452
	movs	r0, r0
	.4byte 0x00000d1d
	.4byte 0x00000d1e
	.4byte 0x03000730
	.4byte 0x08048370
	.4byte 0x00000d33
	.2byte 0x0d20
	.2byte 0x0000
.L_08048444:
	ldr	r0, [sp, #444]
	ldr	r3, [pc, #128]
	ldr	r1, [sp, #52]
	adds	r0, r0, r3
	movs	r2, #128
	bl	sub_0803c9bc
.L_08048452:
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #128]
	movs	r2, #0
	movs	r3, #4
	bl	sub_0803aae4
	ldr	r0, [sp, #52]
	bl	sub_08013164
.L_08048464:
	ldr	r4, [sp, #140]
	movs	r2, #0
	movs	r3, #1
	str	r2, [sp, #132]
	strb	r3, [r4, #3]
.L_0804846e:
	ldr	r0, [pc, #92]
	ldr	r5, [sp, #44]
	ldr	r7, [sp, #36]
	movs	r1, #112
	movs	r6, #0
	mov	sl, r0
	mov	r8, r1
.L_0804847c:
	ldr	r3, [pc, #80]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	ldmia	r7!, {r3}
	ldr	r1, [pc, #52]
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r2, [r3, #2]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r3, [pc, #36]
	mov	r2, r8
	ands	r2, r3
	ldr	r4, [pc, #32]
	ldrh	r3, [r5, #6]
	ands	r3, r4
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r0, [sp, #136]
	movs	r2, #14
	ldrsh	r3, [r0, r2]
	lsls	r3, r3, #3
	adds	r3, #8
	strb	r3, [r5, #4]
	ldr	r1, [sp, #40]
	b.n	.L_080484d4
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x00000d21
	.4byte 0x020036e0
	.2byte 0x0400
	.2byte 0x4000
.L_080484d4:
	ldrsb	r3, [r1, r6]
	cmp	r3, #0
	ble.n	.L_080484e6
	adds	r0, r5, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	sub_08014128
	ldr	r4, [sp, #4]
.L_080484e6:
	movs	r2, #15
	adds	r6, #1
	add	r8, r2
	adds	r5, #12
	cmp	r6, #10
	ble.n	.L_0804847c
	ldr	r3, [sp, #92]
	cmp	r3, #1
	beq.n	.L_080484fc
	cmp	r3, #3
	bne.n	.L_08048578
.L_080484fc:
	ldr	r0, [pc, #60]
	movs	r6, #0
	mov	r8, r0
	add	r5, sp, #480
	movs	r7, #0
.L_08048506:
	ldr	r3, [pc, #56]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	add	r2, sp, #420
	lsls	r3, r6, #2
	ldr	r3, [r2, r3]
	ldr	r1, [pc, #28]
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r2, [r3, #2]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #17
	ands	r3, r1
	lsrs	r2, r2, #22
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r1, [sp, #92]
	cmp	r1, #3
	bne.n	.L_08048544
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #4]
	b.n	.L_08048548
	.4byte 0xfffffc00
	.4byte 0x00000018
	.4byte 0x020036e0
	.2byte 0x0400
	.2byte 0x4000
.L_08048544:
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #44]
.L_08048548:
	ands	r3, r4
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r0, [sp, #136]
	movs	r1, #240
	movs	r2, #14
	ldrsh	r3, [r0, r2]
	adds	r0, r5, #0
	adds	r3, r7, r3
	lsls	r3, r3, #3
	adds	r3, #68
	strb	r3, [r5, #4]
	str	r4, [sp, #4]
	bl	sub_08014128
	adds	r6, #1
	adds	r5, #12
	adds	r7, #2
	ldr	r4, [sp, #4]
	cmp	r6, #3
	ble.n	.L_08048506
	b.n	.L_08048578
	.2byte 0x0008
	.2byte 0x0000
.L_08048578:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_08048602
	ldr	r3, [pc, #256]
	movs	r2, #2
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08048602
	ldr	r1, [sp, #148]
	cmp	r1, #0
	beq.n	.L_080485f8
	ldr	r2, [sp, #68]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080485c8
	ldr	r3, [sp, #124]
	adds	r3, #1
	str	r3, [sp, #124]
	cmp	r3, r1
	blt.n	.L_080485b2
	movs	r4, #0
	str	r4, [sp, #124]
.L_080485b2:
	ldr	r0, [sp, #124]
	ldr	r1, [sp, #152]
	lsls	r3, r0, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #144]
	str	r2, [sp, #132]
	bl	sub_081c0010
	b.n	.L_080485f8
.L_080485c8:
	ldr	r4, [sp, #68]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080485f8
	ldr	r0, [sp, #124]
	subs	r0, #1
	str	r0, [sp, #124]
	cmp	r0, #0
	bge.n	.L_080485e4
	ldr	r1, [sp, #148]
	subs	r1, #1
	str	r1, [sp, #124]
.L_080485e4:
	ldr	r2, [sp, #124]
	ldr	r4, [sp, #152]
	lsls	r3, r2, #1
	ldrh	r3, [r3, r4]
	movs	r0, #1
	str	r0, [sp, #132]
	movs	r0, #111
	str	r3, [sp, #144]
	bl	sub_081c0010
.L_080485f8:
	movs	r0, #1
	bl	sub_08013560
	bl	.L_08046ca8
.L_08048602:
	add	r5, sp, #420
	movs	r6, #3
.L_08048606:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	sub_08014274
	cmp	r6, #0
	bge.n	.L_08048606
	ldr	r5, [sp, #36]
	movs	r6, #10
.L_08048616:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	sub_08014274
	cmp	r6, #0
	bge.n	.L_08048616
	ldr	r0, [sp, #108]
	bl	sub_08014274
	ldr	r0, [sp, #120]
	bl	sub_08014274
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08041b68
	movs	r1, #1
	ldr	r0, [sp, #136]
	bl	sub_0803939c
	movs	r5, #192
	ldr	r0, [sp, #128]
	movs	r1, #1
	lsls	r5, r5, #18
	bl	sub_0803939c
	ldr	r3, [r5, #36]
	adds	r5, #228
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_0804297c
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r3, [r2, #72]
	ldr	r0, [sp, #96]
	bl	sub_08118120
	bl	sub_08118118
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #84]
	bl	sub_08013164
	movs	r3, #185
	lsls	r3, r3, #2
	movs	r0, #0
	add	sp, r3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #224
	str	r0, [sp, #76]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #60]
	movs	r2, #1
	str	r0, [sp, #72]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #128
	str	r1, [sp, #68]
	str	r2, [sp, #64]
	bl	sub_080143e0
	str	r0, [sp, #60]
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08014dac
	str	r0, [sp, #56]
	ldr	r0, [sp, #64]
	movs	r3, #0
	str	r3, [sp, #52]
	movs	r3, #42
	movs	r6, #0
	str	r0, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	str	r6, [sp, #40]
	str	r6, [sp, #32]
	str	r6, [sp, #80]
	str	r6, [sp, #28]
	str	r6, [sp, #24]
	str	r6, [sp, #20]
	bl	sub_08039260
	str	r0, [sp, #44]
	movs	r0, #1
	bl	sub_08041c0c
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #10
	movs	r3, #11
	movs	r0, #20
	bl	sub_08039260
	mov	r9, r0
	adds	r5, #228
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #52]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r1
	mov	sl, r2
	str	r3, [sp, #36]
	ldr	r0, [sp, #76]
	bl	sub_08016ca4
	adds	r0, #248
	movs	r7, #0
	mov	r8, r0
.L_0804871e:
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #56]
	lsls	r3, r0, #2
	movs	r6, #0
	adds	r5, r3, r1
.L_08048728:
	mov	r0, r8
	ldr	r3, [r0, #16]
	movs	r2, #1
	lsls	r2, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08048744
	lsls	r3, r7, #8
	orrs	r3, r6
	stmia	r5!, {r3}
	ldr	r1, [sp, #52]
	adds	r1, #1
	str	r1, [sp, #52]
	b.n	.L_080487da
.L_08048744:
	mov	r0, r8
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080487da
	ldr	r1, [sp, #76]
	movs	r0, #0
	cmp	r1, #7
	bls.n	.L_08048758
	movs	r0, #1
.L_08048758:
	bl	sub_080ad000
	movs	r2, #148
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	adds	r0, #8
	movs	r4, #0
	cmp	r1, r3
	bge.n	.L_080487b4
	ldrb	r3, [r0, #2]
	ldr	r2, [sp, #76]
	cmp	r3, r2
	bne.n	.L_08048784
	ldrb	r3, [r0, #0]
	cmp	r3, r7
	bne.n	.L_08048784
	ldrb	r3, [r0, #1]
	cmp	r3, r6
	beq.n	.L_080487ae
.L_08048784:
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	adds	r1, #1
	cmp	r1, r3
	bge.n	.L_080487b2
	lsls	r4, r1, #2
	adds	r2, r0, r4
	ldrb	r3, [r2, #2]
	mov	ip, r3
	ldr	r3, [sp, #76]
	cmp	ip, r3
	bne.n	.L_08048784
	ldrb	r3, [r2, #0]
	cmp	r3, r7
	bne.n	.L_08048784
	ldrb	r3, [r2, #1]
	cmp	r3, r6
	bne.n	.L_08048784
	b.n	.L_080487b4
.L_080487ae:
	movs	r4, #0
	b.n	.L_080487b4
.L_080487b2:
	lsls	r4, r1, #2
.L_080487b4:
	lsls	r2, r7, #8
	movs	r3, #128
	lsls	r3, r3, #9
	orrs	r2, r6
	orrs	r2, r3
	str	r2, [r5, #0]
	adds	r3, r0, r4
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080487d2
	lsls	r3, r3, #17
	orrs	r2, r3
	str	r2, [r5, #0]
.L_080487d2:
	ldr	r0, [sp, #52]
	adds	r5, #4
	adds	r0, #1
	str	r0, [sp, #52]
.L_080487da:
	adds	r6, #1
	cmp	r6, #19
	ble.n	.L_08048728
	movs	r1, #4
	adds	r7, #1
	add	r8, r1
	cmp	r7, #3
	ble.n	.L_0804871e
	ldr	r2, [sp, #52]
	ldr	r0, [sp, #56]
	lsls	r3, r2, #2
	movs	r2, #128
	lsls	r2, r2, #24
	str	r2, [r3, r0]
	ldr	r1, [sp, #72]
	movs	r3, #1
	strb	r3, [r1, #3]
	mov	r2, sp
	mov	r3, sp
	adds	r2, #212
	adds	r3, #84
	str	r2, [sp, #4]
	str	r3, [sp, #8]
.L_08048808:
	ldr	r0, [sp, #48]
	cmp	fp, r0
	bne.n	.L_0804881c
	ldr	r1, [sp, #64]
	cmp	sl, r1
	bne.n	.L_0804881c
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_0804881c
	b.n	.L_08048b3a
.L_0804881c:
	ldr	r0, [sp, #56]
	mov	r3, fp
	ldr	r2, [sp, #72]
	add	r3, sl
	lsls	r3, r3, #2
	ldr	r5, [r3, r0]
	movs	r1, #0
	movs	r3, #1
	str	r1, [sp, #40]
	strb	r3, [r2, #6]
	mov	r1, r9
	movs	r3, #12
	ldrsh	r0, [r1, r3]
	movs	r2, #14
	ldrsh	r1, [r1, r2]
	ldr	r2, [sp, #64]
	adds	r0, #1
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r1, #1
	subs	r2, #2
	movs	r3, #1
	bl	sub_08046134
	ldr	r0, [sp, #32]
	cmp	r0, #0
	beq.n	.L_08048878
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_0803939c
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	bl	sub_08039260
	str	r0, [sp, #44]
	bl	sub_080396bc
.L_08048878:
	ldr	r2, [sp, #52]
	movs	r1, #0
	str	r1, [sp, #28]
	cmp	r2, #0
	bne.n	.L_08048884
	b.n	.L_080489a0
.L_08048884:
	bl	sub_0803cca8
	movs	r3, #0
	str	r3, [sp, #24]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #228
	ldr	r3, [r2, r3]
	ldr	r1, [sp, #28]
	cmp	r3, r5
	bne.n	.L_080488a4
	movs	r0, #1
	str	r0, [sp, #24]
	b.n	.L_080488c0
.L_080488a4:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_080488c0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	lsls	r3, r1, #2
	adds	r3, #228
	ldr	r3, [r2, r3]
	cmp	r3, r5
	bne.n	.L_080488a4
	movs	r1, #1
	str	r1, [sp, #24]
.L_080488c0:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	beq.n	.L_080488e8
	ldr	r6, [sp, #8]
	ldr	r0, [pc, #808]
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0803c9bc
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_080489ac
	adds	r0, r3, #0
	movs	r1, #1
	bl	sub_0803939c
	movs	r0, #0
	str	r0, [sp, #68]
	str	r0, [sp, #32]
	b.n	.L_080489ac
.L_080488e8:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0804896a
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r5
	cmp	r0, #0
	beq.n	.L_0804893c
	lsrs	r0, r0, #17
	movs	r1, #5
	bl	sub_0803ccd0
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	ldr	r6, [sp, #8]
	ldr	r3, [pc, #736]
	lsls	r0, r0, #2
	adds	r0, r0, r5
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #52
	bl	sub_0803c9bc
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L_080489ac
	adds	r0, r1, #0
	movs	r1, #1
	bl	sub_0803939c
	movs	r2, #0
	str	r2, [sp, #68]
	str	r2, [sp, #32]
	b.n	.L_080489ac
.L_0804893c:
	add	r3, sp, #80
	str	r3, [sp, #0]
	ldr	r1, [sp, #76]
	adds	r2, r5, #0
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #68]
	bl	.L_080464dc
	ldr	r6, [sp, #8]
	str	r0, [sp, #68]
	adds	r1, r6, #0
	ldr	r0, [pc, #680]
	movs	r2, #52
	bl	sub_0803c9bc
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r5, r3
	lsrs	r3, r5, #8
	movs	r0, #1
	lsls	r0, r3
	str	r0, [sp, #40]
	b.n	.L_080489ac
.L_0804896a:
	add	r3, sp, #80
	adds	r2, r5, #0
	str	r3, [sp, #0]
	ldr	r1, [sp, #76]
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #68]
	bl	.L_080464dc
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	str	r0, [sp, #68]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	ldr	r6, [sp, #8]
	ldr	r3, [pc, #616]
	lsls	r0, r0, #2
	adds	r0, r0, r5
	adds	r0, r0, r3
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0803c9bc
	b.n	.L_080489ac
.L_080489a0:
	ldr	r6, [sp, #8]
	ldr	r0, [pc, #604]
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0803c9bc
.L_080489ac:
	ldr	r2, [sp, #72]
	movs	r1, #0
	strb	r1, [r2, #6]
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_080489de
	movs	r3, #1
	strb	r3, [r2, #6]
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_0803939c
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r0, #0
	movs	r2, #30
	movs	r3, #4
	bl	sub_08039260
	str	r0, [sp, #44]
	add	r0, sp, #32
	ldrb	r0, [r0, #0]
	ldr	r1, [sp, #72]
	strb	r0, [r1, #6]
.L_080489de:
	ldr	r1, [sp, #44]
	movs	r2, #0
	adds	r0, r6, #0
	movs	r3, #4
	bl	sub_0803aae4
	ldr	r2, [sp, #48]
	mov	r1, sl
	str	r1, [sp, #64]
	cmp	fp, r2
	bne.n	.L_080489fa
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bne.n	.L_08048aba
.L_080489fa:
	mov	r0, r9
	bl	sub_08039418
	ldr	r1, [sp, #56]
	mov	r0, fp
	lsls	r3, r0, #2
	adds	r3, r3, r1
	ldr	r6, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #24
	movs	r7, #0
	cmp	r6, r2
	beq.n	.L_08048ab0
	mov	r8, r3
.L_08048a16:
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r1, r6, #0
	movs	r0, #160
	ands	r1, r3
	lsls	r0, r0, #7
	adds	r0, #1
	lsrs	r1, r1, #8
	adds	r1, r1, r0
	lsls	r3, r7, #1
	movs	r2, #0
	mov	r0, r9
	str	r2, [sp, #0]
	bl	sub_0803c378
	movs	r3, #248
	lsls	r3, r3, #14
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08048a46
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_08048a56
.L_08048a46:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08048a56
	movs	r0, #2
	bl	sub_08041f70
.L_08048a56:
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r3, r6, #0
	ands	r3, r0
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r3, r6
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #404]
	lsls	r5, r7, #4
	adds	r0, r0, r3
	mov	r1, r9
	movs	r2, #8
	adds	r3, r5, #0
	bl	sub_08042010
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r6
	cmp	r0, #0
	beq.n	.L_08048a94
	lsrs	r0, r0, #17
	movs	r1, #2
	mov	r2, r9
	movs	r3, #48
	str	r5, [sp, #0]
	bl	sub_08042214
.L_08048a94:
	movs	r0, #15
	adds	r7, #1
	bl	sub_08041f70
	cmp	r7, #4
	bgt.n	.L_08048ab0
	movs	r1, #4
	add	r8, r1
	mov	r2, r8
	ldr	r6, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #24
	cmp	r6, r3
	bne.n	.L_08048a16
.L_08048ab0:
	mov	ip, fp
	mov	r0, ip
	str	r0, [sp, #48]
	movs	r0, #0
	str	r0, [sp, #20]
.L_08048aba:
	ldr	r1, [sp, #52]
	cmp	r1, #5
	ble.n	.L_08048b0c
	movs	r7, #0
	adds	r1, #4
	mov	r8, r1
	b.n	.L_08048afe
.L_08048ac8:
	movs	r2, #243
	lsls	r2, r2, #8
	adds	r2, #1
	mov	r0, fp
	movs	r1, #5
	adds	r6, r7, r2
	bl	sub_08002054
	cmp	r7, r0
	bne.n	.L_08048ae4
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #11
	adds	r6, r7, r3
.L_08048ae4:
	mov	r0, r9
	ldrh	r2, [r0, #8]
	movs	r1, #0
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #1
	str	r1, [sp, #0]
	subs	r2, #2
	adds	r1, r6, #0
	negs	r3, r3
	bl	sub_0803c378
	adds	r7, #1
.L_08048afe:
	mov	r0, r8
	movs	r1, #5
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_08048ac8
.L_08048b0c:
	mov	r3, r9
	movs	r2, #14
	ldrsh	r1, [r3, r2]
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	mov	r2, sl
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	str	r3, [sp, #0]
	subs	r2, #2
	movs	r3, #1
	bl	sub_08046134
	ldr	r0, [sp, #72]
	movs	r3, #1
	movs	r1, #0
	strb	r3, [r0, #3]
	strb	r1, [r0, #6]
.L_08048b3a:
	ldr	r2, [sp, #52]
	cmp	r2, #5
	bgt.n	.L_08048b42
	b.n	.L_08048c58
.L_08048b42:
	movs	r7, #0
	adds	r2, #4
	mov	r8, r2
	b.n	.L_08048ba6
.L_08048b4a:
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #1
	adds	r6, r7, r3
	ldr	r3, [pc, #180]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08048b6c
	ldr	r3, [pc, #168]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08048b80
.L_08048b6c:
	mov	r0, fp
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	bne.n	.L_08048b80
	movs	r0, #243
	lsls	r0, r0, #8
	adds	r0, #11
	adds	r6, r7, r0
.L_08048b80:
	mov	r1, r9
	ldrh	r5, [r1, #8]
	mov	r0, r8
	movs	r1, #5
	bl	sub_08002054
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r2, #0
	subs	r5, #2
	movs	r3, #1
	str	r2, [sp, #0]
	mov	r0, r9
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	sub_0803c378
	adds	r7, #1
.L_08048ba6:
	mov	r0, r8
	movs	r1, #5
	bl	sub_08002054
	cmp	r7, r0
	blt.n	.L_08048b4a
	ldr	r3, [pc, #84]
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_08048c10
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r1, #243
	subs	r2, r2, r0
	lsls	r1, r1, #8
	movs	r3, #1
	subs	r2, #3
	mov	r0, r9
	adds	r1, #52
	negs	r3, r3
	str	r5, [sp, #0]
	bl	sub_0803c378
	mov	r0, r9
	ldrh	r2, [r0, #8]
	movs	r1, #243
	lsls	r1, r1, #8
	movs	r3, #1
	subs	r2, #2
	adds	r1, #53
	negs	r3, r3
	str	r5, [sp, #0]
	bl	sub_0803c378
	b.n	.L_08048c42
	movs	r0, r0
	.4byte 0x00000d50
	.4byte 0x000009b1
	.4byte 0x00000cf9
	.4byte 0x00000d4e
	.4byte 0x000006d3
	.4byte 0x03001150
	.2byte 0x122c
	.2byte 0x0300
.L_08048c10:
	mov	r1, r9
	ldrh	r2, [r1, #8]
	movs	r1, #240
	subs	r2, r2, r0
	movs	r3, #0
	lsls	r1, r1, #8
	subs	r2, #3
	str	r3, [sp, #0]
	mov	r0, r9
	adds	r1, #17
	subs	r3, #1
	bl	sub_0803c378
	mov	r0, r9
	ldrh	r2, [r0, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r1, #240
	lsls	r1, r1, #8
	movs	r3, #1
	subs	r2, #2
	adds	r1, #18
	negs	r3, r3
	bl	sub_0803c378
.L_08048c42:
	mov	r0, r9
	movs	r2, #14
	ldrsh	r3, [r0, r2]
	ldr	r1, [sp, #72]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #3]
	orrs	r2, r3
	strb	r2, [r1, #3]
.L_08048c58:
	ldr	r3, [pc, #756]
	ldr	r1, [r3, #4]
	ldr	r7, [r3, #12]
	ldr	r3, [r3, #0]
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08048c90
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r1, #0
	movs	r7, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08048c8c
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r7, #1
	movs	r1, #1
	b.n	.L_08048c90
.L_08048c8c:
	subs	r3, #1
	str	r3, [r2, #0]
.L_08048c90:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #76]
	cmp	r3, #0
	beq.n	.L_08048ca6
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08048cb2
.L_08048ca6:
	movs	r0, #113
	movs	r6, #1
	bl	sub_081c0010
	negs	r6, r6
	b.n	.L_080490c2
.L_08048cb2:
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08048d28
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_08048d22
	mov	r3, fp
	ldr	r1, [sp, #56]
	add	r3, sl
	lsls	r3, r3, #2
	ldr	r0, [r3, r1]
	movs	r6, #248
	lsls	r6, r6, #14
	adds	r5, r0, #0
	ands	r5, r6
	cmp	r5, #0
	bne.n	.L_08048cec
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L_08048d1a
	adds	r6, r0, #0
	mov	r1, sl
	mov	r0, fp
	str	r0, [r2, #52]
	str	r1, [r2, #48]
	ldr	r3, [sp, #36]
	str	r3, [r2, #56]
	b.n	.L_080490c2
.L_08048cec:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	bne.n	.L_08048d1a
	ands	r5, r6
	bl	sub_080396bc
	bl	sub_0803cca8
	lsrs	r0, r5, #17
	movs	r1, #5
	bl	sub_0803ccd0
	movs	r2, #52
	ldr	r1, [sp, #8]
	ldr	r0, [pc, #584]
	bl	sub_0803c9bc
	movs	r2, #0
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #44]
	movs	r3, #4
	bl	sub_0803aae4
.L_08048d1a:
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_08048d28
.L_08048d22:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_080490c2
.L_08048d28:
	ldr	r1, [sp, #52]
	cmp	r1, #0
	bne.n	.L_08048d30
	b.n	.L_08048f82
.L_08048d30:
	movs	r3, #128
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08048d5c
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #5
	beq.n	.L_08048d52
	ldr	r0, [sp, #52]
	mov	r3, fp
	add	r3, sl
	cmp	r3, r0
	bne.n	.L_08048d56
.L_08048d52:
	movs	r1, #0
	mov	sl, r1
.L_08048d56:
	mov	r2, sl
	str	r2, [sp, #36]
	b.n	.L_08048f82
.L_08048d5c:
	movs	r3, #64
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08048d9c
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r0, sl
	cmp	r0, #0
	bge.n	.L_08048d96
	ldr	r0, [sp, #52]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_08048d92
	ldr	r1, [sp, #52]
	mov	r2, fp
	subs	r3, r1, r2
	subs	r3, #1
	b.n	.L_08048d94
.L_08048d92:
	movs	r3, #4
.L_08048d94:
	mov	sl, r3
.L_08048d96:
	mov	r0, sl
	str	r0, [sp, #36]
	b.n	.L_08048f82
.L_08048d9c:
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08048daa
	b.n	.L_08048ebe
.L_08048daa:
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_08048e44
	ldr	r0, [sp, #80]
	movs	r5, #0
	cmp	r5, r0
	bge.n	.L_08048dfe
.L_08048db8:
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #1
	adds	r1, r5, r3
	ldr	r3, [pc, #404]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08048dde
	ldr	r3, [sp, #32]
	subs	r3, #1
	cmp	r5, r3
	bne.n	.L_08048dde
	ldr	r2, [sp, #32]
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #10
	adds	r1, r2, r3
.L_08048dde:
	ldr	r3, [sp, #68]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	movs	r0, #0
	adds	r2, r2, r5
	str	r0, [sp, #0]
	adds	r0, r3, #0
	movs	r3, #1
	subs	r2, #2
	negs	r3, r3
	bl	sub_0803c378
	ldr	r0, [sp, #80]
	adds	r5, #1
	cmp	r5, r0
	blt.n	.L_08048db8
.L_08048dfe:
	ldr	r1, [sp, #68]
	movs	r3, #0
	ldrh	r2, [r1, #8]
	str	r3, [sp, #0]
	subs	r2, r2, r0
	adds	r0, r1, #0
	movs	r1, #243
	lsls	r1, r1, #8
	subs	r2, #3
	adds	r1, #52
	subs	r3, #1
	bl	sub_0803c378
	ldr	r0, [sp, #68]
	movs	r1, #0
	ldrh	r2, [r0, #8]
	str	r1, [sp, #0]
	movs	r1, #243
	lsls	r1, r1, #8
	movs	r3, #1
	subs	r2, #2
	adds	r1, #53
	negs	r3, r3
	bl	sub_0803c378
	ldr	r2, [sp, #68]
	ldrh	r3, [r2, #14]
	ldr	r0, [sp, #72]
	lsls	r3, r3, #16
	asrs	r3, r3, #18
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r0, #3]
	orrs	r2, r3
	strb	r2, [r0, #3]
.L_08048e44:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L_08048e64
	ldr	r0, [sp, #80]
	cmp	r0, #0
	beq.n	.L_08048e66
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_08048e5c
	adds	r0, r2, #0
	bl	sub_08039430
.L_08048e5c:
	movs	r3, #1
	str	r3, [sp, #32]
	str	r3, [sp, #28]
	b.n	.L_08048f82
.L_08048e64:
	ldr	r0, [sp, #80]
.L_08048e66:
	ldr	r1, [sp, #32]
	cmp	r1, r0
	ble.n	.L_08048e6e
	str	r0, [sp, #32]
.L_08048e6e:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08048e76
	b.n	.L_08048f82
.L_08048e76:
	movs	r3, #16
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08048e9c
	movs	r0, #111
	bl	sub_081c0010
	ldr	r3, [sp, #32]
	adds	r3, #1
	str	r3, [sp, #32]
	ldr	r0, [sp, #32]
	ldr	r3, [sp, #80]
	cmp	r0, r3
	ble.n	.L_08048e96
	movs	r1, #1
	str	r1, [sp, #32]
.L_08048e96:
	movs	r2, #1
	str	r2, [sp, #28]
	b.n	.L_08048f82
.L_08048e9c:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08048f82
	movs	r0, #111
	bl	sub_081c0010
	ldr	r3, [sp, #32]
	subs	r3, #1
	str	r3, [sp, #32]
	cmp	r3, #0
	bgt.n	.L_08048eb8
	ldr	r0, [sp, #80]
	str	r0, [sp, #32]
.L_08048eb8:
	movs	r1, #1
	str	r1, [sp, #28]
	b.n	.L_08048f82
.L_08048ebe:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	beq.n	.L_08048edc
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_08048ed0
	adds	r0, r3, #0
	bl	sub_08039430
.L_08048ed0:
	movs	r1, #1
	movs	r0, #0
	str	r0, [sp, #32]
	str	r1, [sp, #28]
	str	r1, [sp, #20]
	b.n	.L_08048f82
.L_08048edc:
	movs	r3, #16
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08048f2a
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	ldr	r2, [sp, #52]
	mov	r3, fp
	adds	r3, #5
	cmp	r3, r2
	blt.n	.L_08048f08
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_08048f82
	ldr	r1, [sp, #36]
	movs	r0, #0
	mov	fp, r0
	mov	sl, r1
	b.n	.L_08048f82
.L_08048f08:
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #36]
	subs	r0, #1
	movs	r1, #5
	mov	fp, r3
	mov	sl, r2
	bl	sub_08002054
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_08048f82
	ldr	r0, [sp, #52]
	mov	r1, fp
	subs	r3, r0, r1
	ldr	r2, [sp, #36]
	b.n	.L_08048f78
.L_08048f2a:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08048f82
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_08048f5c
	ldr	r1, [sp, #36]
	movs	r0, #5
	negs	r0, r0
	add	fp, r0
	mov	sl, r1
	b.n	.L_08048f82
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x00000cf8
	.2byte 0x122c
	.2byte 0x0300
.L_08048f5c:
	ldr	r0, [sp, #52]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	ldr	r2, [sp, #36]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	mov	fp, r3
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_08048f82
	ldr	r0, [sp, #52]
	subs	r3, r0, r3
.L_08048f78:
	subs	r3, #1
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L_08048f82
	mov	sl, r2
.L_08048f82:
	mov	r2, r9
	movs	r1, #12
	ldrsh	r3, [r2, r1]
	movs	r1, #14
	ldrsh	r2, [r2, r1]
	lsls	r3, r3, #3
	subs	r3, #2
	mov	r0, sl
	str	r3, [sp, #12]
	lsls	r3, r0, #1
	adds	r3, r3, r2
	lsls	r3, r3, #3
	ldr	r2, [sp, #4]
	adds	r3, #20
	str	r3, [sp, #16]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #72]
	bl	sub_080143f8
	ldr	r3, [pc, #52]
	ldr	r1, [sp, #4]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ldr	r6, [pc, #60]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [r6, #0]
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	movs	r5, #4
	ldr	r3, [sp, #12]
	ands	r0, r5
	movs	r1, #255
	lsrs	r2, r0, #1
	lsls	r1, r1, #8
	adds	r2, r3, r2
	adds	r1, #250
	adds	r2, r2, r1
	ldr	r3, [pc, #20]
	ldr	r1, [sp, #4]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	b.n	.L_08049000
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x080597f8
	.2byte 0x122c
	.2byte 0x0300
.L_08049000:
	lsrs	r0, r0, #2
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #4]
	strh	r3, [r2, #6]
	ldr	r3, [sp, #16]
	subs	r0, r3, r0
	adds	r0, #248
	strb	r0, [r2, #4]
	ldr	r0, [sp, #52]
	cmp	r0, #0
	beq.n	.L_08049020
	ldr	r0, [sp, #4]
	movs	r1, #242
	bl	sub_08014128
.L_08049020:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #64]
	ldr	r6, [r6, #0]
	ldrh	r2, [r3, #12]
	ldr	r7, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	ands	r6, r5
	cmp	r3, #0
	beq.n	.L_0804907a
	movs	r5, #0
.L_08049038:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	ldr	r1, [sp, #40]
	movs	r3, #15
	subs	r2, r3, r2
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08049052
	movs	r2, #15
.L_08049052:
	movs	r3, #12
	ldrsh	r0, [r7, r3]
	ldr	r3, [pc, #216]
	ldrb	r3, [r3, r5]
	adds	r0, r0, r3
	movs	r3, #14
	ldrsh	r1, [r7, r3]
	ldr	r3, [pc, #208]
	adds	r0, #1
	ldrb	r3, [r3, r5]
	str	r2, [sp, #0]
	adds	r1, r1, r3
	adds	r1, #1
	movs	r2, #2
	movs	r3, #1
	adds	r5, #1
	bl	sub_08046134
	cmp	r5, #3
	ble.n	.L_08049038
.L_0804907a:
	ldr	r3, [pc, #188]
	movs	r2, #4
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080490a0
	ldr	r5, [pc, #180]
	movs	r2, #32
	adds	r1, r5, #0
	ldr	r6, [pc, #176]
	ldr	r0, [pc, #180]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x482c
	adds	r1, r5, #0
	movs	r2, #32
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe00b
.L_080490a0:
	ldr	r3, [pc, #168]
	movs	r1, #32
	ldr	r2, [pc, #168]
	ldr	r0, [pc, #156]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b24
	ldr	r0, [pc, #152]
	ldr	r1, [pc, #136]
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2001
	bl	sub_08013560
	bl	.L_08048808
.L_080490c2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #64]
	movs	r3, #2
	ldrh	r2, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080490ec
	ldr	r3, [r1, #0]
	movs	r1, #12
	ldrsh	r0, [r3, r1]
	movs	r2, #14
	ldrsh	r1, [r3, r2]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #4
	movs	r3, #4
	bl	sub_08046134
.L_080490ec:
	ldr	r0, [sp, #60]
	bl	sub_08014274
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	sub_0803939c
	movs	r1, #1
	ldr	r0, [sp, #68]
	bl	sub_0803939c
	movs	r1, #1
	mov	r0, r9
	bl	sub_0803939c
	bl	sub_08041b68
	movs	r0, #0
	bl	sub_08041c0c
	ldr	r0, [sp, #56]
	bl	sub_08013164
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r6, #0
	add	sp, #224
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0805f897
	.4byte 0x0805f89b
	.4byte 0x0300122c
	.4byte 0x0805f7b8
	.4byte 0x03000730
	.4byte 0x06006500
	.4byte 0x06006520
	.4byte 0x03000260
	.2byte 0x4444
	.2byte 0x4444
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #372
	str	r2, [sp, #84]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #60]
	movs	r1, #1
	str	r0, [sp, #72]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #68]
	mov	r9, r1
	bl	sub_080143e0
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #42
	str	r0, [sp, #64]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_08039260
	movs	r6, #6
	str	r0, [sp, #60]
	movs	r1, #8
	movs	r2, #10
	movs	r3, #3
	movs	r0, #20
	str	r6, [sp, #0]
	bl	sub_08039260
	movs	r2, #0
	str	r0, [sp, #56]
	str	r2, [sp, #52]
	adds	r5, #228
	ldr	r3, [r5, #0]
	ldr	r4, [r3, #52]
	ldr	r0, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r4
	mov	sl, r0
	str	r3, [sp, #48]
	str	r6, [sp, #0]
	movs	r1, #11
	movs	r0, #13
	movs	r2, #17
	movs	r3, #9
	bl	sub_08039260
	movs	r1, #156
	lsls	r1, r1, #1
	add	r1, sp
	str	r0, [sp, #76]
	str	r1, [sp, #28]
	ldr	r7, [pc, #80]
	movs	r6, #128
	movs	r4, #0
	adds	r5, r1, #0
	lsls	r6, r6, #23
.L_080491dc:
	movs	r3, #0
	lsls	r0, r4, #1
	str	r6, [r5, #4]
	str	r3, [r5, #8]
	ldr	r1, [sp, #76]
	adds	r4, #1
	movs	r3, #12
	ldrsh	r2, [r1, r3]
	ldr	r3, [pc, #48]
	ldrh	r1, [r5, #6]
	lsls	r2, r2, #3
	adds	r2, #8
	ands	r2, r3
	adds	r3, r7, #0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r1, [sp, #76]
	movs	r2, #14
	ldrsh	r3, [r1, r2]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	strb	r0, [r5, #4]
	adds	r5, #12
	cmp	r4, #3
	ble.n	.L_080491dc
	ldr	r2, [pc, #20]
	ldr	r7, [sp, #28]
	movs	r5, #8
	add	r6, sp, #96
	mov	r8, r2
	movs	r4, #3
	b.n	.L_0804922c
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_0804922c:
	movs	r0, #128
	str	r4, [sp, #4]
	bl	sub_080143e0
	movs	r1, #1
	negs	r1, r1
	stmia	r6!, {r0}
	bl	sub_080143f8
	ldr	r3, [pc, #24]
	ldr	r4, [sp, #4]
	ands	r0, r3
	ldrh	r3, [r5, r7]
	mov	r1, r8
	ands	r3, r1
	orrs	r3, r0
	subs	r4, #1
	strh	r3, [r5, r7]
	adds	r5, #12
	cmp	r4, #0
	bge.n	.L_0804922c
	b.n	.L_0804925c
	.2byte 0x03ff
	.2byte 0x0000
.L_0804925c:
	movs	r2, #138
	lsls	r2, r2, #1
	add	r2, sp
	mov	r8, r2
	mov	r0, r8
	bl	sub_080ad180
	str	r0, [sp, #80]
	movs	r7, #0
	adds	r3, r0, #0
	subs	r3, #1
	str	r3, [sp, #20]
	cmp	r3, #0
	blt.n	.L_080492c0
	mov	r4, sp
	adds	r4, #240
	str	r4, [sp, #32]
	adds	r5, r3, #0
	add	r5, r8
.L_08049282:
	ldrb	r6, [r5, #0]
	adds	r0, r6, #0
	bl	sub_080ad188
	ldr	r1, [sp, #84]
	adds	r0, #4
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	movs	r4, #0
	cmp	r2, r3
	bhi.n	.L_080492aa
.L_08049298:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_080492aa
	adds	r0, #1
	adds	r1, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	cmp	r2, r3
	bls.n	.L_08049298
.L_080492aa:
	cmp	r4, #4
	bne.n	.L_080492b8
	ldr	r2, [sp, #32]
	movs	r3, #32
	strb	r6, [r2, r7]
	strb	r3, [r5, #0]
	adds	r7, #1
.L_080492b8:
	subs	r5, #1
	cmp	r5, r8
	bge.n	.L_08049282
	b.n	.L_080492c6
.L_080492c0:
	mov	r3, sp
	adds	r3, #240
	str	r3, [sp, #32]
.L_080492c6:
	ldr	r4, [sp, #80]
	cmp	r4, #0
	ble.n	.L_080492e8
	ldr	r2, [sp, #32]
	mov	r0, r8
	adds	r1, r7, r2
	adds	r2, r4, #0
.L_080492d4:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #32
	beq.n	.L_080492e2
	strb	r3, [r1, #0]
	adds	r7, #1
	adds	r1, #1
.L_080492e2:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_080492d4
.L_080492e8:
	ldr	r4, [sp, #32]
	movs	r3, #32
	strb	r3, [r4, r7]
	ldr	r2, [sp, #64]
	movs	r0, #180
	lsls	r0, r0, #1
	mov	r1, sl
	add	r0, sp
	lsls	r1, r1, #1
	lsls	r2, r2, #16
	str	r0, [sp, #24]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
.L_08049302:
	cmp	fp, r9
	bne.n	.L_0804930e
	ldr	r3, [sp, #68]
	cmp	sl, r3
	bne.n	.L_0804930e
	b.n	.L_080495ce
.L_0804930e:
	ldr	r0, [sp, #72]
	movs	r4, #1
	strb	r4, [r0, #6]
	ldr	r2, [sp, #76]
	ldr	r4, [sp, #68]
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	ldrh	r2, [r2, #8]
	lsls	r3, r4, #1
	adds	r1, r1, r3
	movs	r3, #15
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r1, #1
	movs	r3, #1
	adds	r0, #1
	bl	sub_08046134
	bl	sub_080396bc
	ldr	r1, [sp, #32]
	mov	r3, fp
	add	r3, sl
	ldrb	r0, [r1, r3]
	bl	sub_080ad188
	adds	r6, r0, #0
	ldrh	r0, [r6, #0]
	ldr	r3, [pc, #124]
	add	r5, sp, #112
	adds	r0, r0, r3
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0803c9bc
	movs	r2, #0
	ldr	r1, [sp, #60]
	movs	r3, #4
	adds	r0, r5, #0
	bl	sub_0803aae4
	movs	r3, #0
	str	r3, [sp, #52]
	mov	r2, sl
	str	r2, [sp, #68]
	movs	r1, #1
	movs	r2, #0
	adds	r6, #4
.L_08049372:
	ldrb	r3, [r6, #0]
	adds	r6, #1
	cmp	r3, #0
	beq.n	.L_08049384
	ldr	r4, [sp, #52]
	adds	r3, r1, #0
	lsls	r3, r2
	orrs	r4, r3
	str	r4, [sp, #52]
.L_08049384:
	adds	r2, #1
	cmp	r2, #3
	ble.n	.L_08049372
	cmp	fp, r9
	bne.n	.L_08049390
	b.n	.L_08049546
.L_08049390:
	ldr	r0, [sp, #76]
	bl	sub_08039418
	movs	r7, #0
	ldr	r5, [sp, #84]
	movs	r0, #0
	mov	r8, r0
	movs	r6, #0
.L_080493a0:
	movs	r2, #160
	lsls	r2, r2, #7
	adds	r2, #1
	mov	r3, r8
	adds	r1, r7, r2
	str	r3, [sp, #0]
	ldr	r0, [sp, #56]
	movs	r3, #0
	adds	r2, r6, #0
	bl	sub_0803c378
	ldrb	r3, [r5, #0]
	cmp	r3, #9
	bls.n	.L_080493cc
	movs	r4, #241
	lsls	r4, r4, #8
	adds	r4, #150
	adds	r1, r3, r4
	b.n	.L_080493d8
	movs	r0, r0
	.2byte 0x0885
	.2byte 0x0000
.L_080493cc:
	ldrb	r3, [r5, #0]
	adds	r1, r3, #0
	movs	r3, #240
	adds	r1, #48
	lsls	r3, r3, #8
	orrs	r1, r3
.L_080493d8:
	mov	r0, r8
	adds	r2, r6, #1
	str	r0, [sp, #0]
	movs	r3, #0
	ldr	r0, [sp, #56]
	adds	r7, #1
	bl	sub_0803c378
	adds	r5, #1
	adds	r6, #2
	cmp	r7, #3
	ble.n	.L_080493a0
	ldr	r1, [sp, #32]
	mov	r2, fp
	ldrb	r6, [r1, r2]
	movs	r4, #0
	cmp	r6, #32
	bne.n	.L_080493fe
	b.n	.L_08049528
.L_080493fe:
	mov	r3, sp
	adds	r3, #88
	str	r3, [sp, #8]
.L_08049404:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080ad188
	str	r0, [sp, #36]
	adds	r1, r0, #0
	ldr	r0, [sp, #84]
	adds	r1, #4
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	movs	r7, #0
	ldr	r4, [sp, #4]
	cmp	r2, r3
	bhi.n	.L_08049432
.L_08049420:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L_08049432
	adds	r1, #1
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bls.n	.L_08049420
.L_08049432:
	ldr	r2, [sp, #36]
	movs	r3, #4
	eors	r3, r7
	negs	r5, r3
	orrs	r5, r3
	ldr	r0, [pc, #48]
	ldrh	r3, [r2, #0]
	movs	r1, #1
	ands	r0, r3
	add	r2, sp, #96
	lsls	r3, r4, #2
	lsrs	r5, r5, #31
	adds	r2, r2, r3
	str	r1, [sp, #0]
	ldr	r3, [sp, #8]
	subs	r5, r1, r5
	movs	r1, #0
	str	r4, [sp, #4]
	bl	sub_0803d98c
	ldr	r4, [sp, #4]
	ldr	r2, [sp, #28]
	lsls	r3, r4, #1
	adds	r1, r3, r4
	mov	r8, r3
	ldr	r0, [sp, #88]
	ldr	r3, [pc, #12]
	lsls	r1, r1, #2
	adds	r1, #8
	ands	r0, r3
	b.n	.L_08049478
	.4byte 0x00003fff
	.2byte 0x03ff
	.2byte 0x0000
.L_08049478:
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #56]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #28]
	strh	r3, [r0, r1]
	cmp	r5, #0
	bne.n	.L_08049490
	movs	r0, #2
	bl	sub_08041f70
	ldr	r4, [sp, #4]
.L_08049490:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080ad188
	ldr	r3, [pc, #28]
	ldr	r4, [sp, #4]
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	adds	r0, r0, r3
	movs	r2, #16
	lsls	r3, r4, #4
	bl	sub_08042010
	movs	r1, #0
	ldr	r6, [sp, #36]
	lsls	r3, r1, #1
	b.n	.L_080494bc
	movs	r0, r0
	.4byte 0xfffffc00
	.2byte 0x05a7
	.2byte 0x0000
.L_080494bc:
	ldr	r4, [sp, #4]
	adds	r5, r3, #0
	movs	r7, #0
	mov	r9, r1
	adds	r6, #4
	adds	r5, #11
.L_080494c8:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_080494fc
	movs	r2, #160
	lsls	r2, r2, #7
	adds	r2, #1
	mov	r3, r9
	adds	r1, r7, r2
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	mov	r3, r8
	str	r4, [sp, #4]
	bl	sub_0803c378
	ldrb	r1, [r6, #0]
	mov	r0, r9
	adds	r2, r5, #1
	str	r0, [sp, #0]
	adds	r1, #48
	ldr	r0, [sp, #76]
	mov	r3, r8
	bl	sub_0803c274
	ldr	r4, [sp, #4]
	adds	r5, #2
.L_080494fc:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #3
	ble.n	.L_080494c8
	movs	r0, #15
	str	r4, [sp, #4]
	bl	sub_08041f70
	ldr	r4, [sp, #4]
	add	r3, sp, #92
	movs	r1, #1
	strb	r1, [r3, r4]
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08049544
	ldr	r0, [sp, #32]
	mov	r2, fp
	adds	r3, r2, r4
	ldrb	r6, [r0, r3]
	cmp	r6, #32
	beq.n	.L_08049528
	b.n	.L_08049404
.L_08049528:
	cmp	r4, #3
	bgt.n	.L_08049544
	ldr	r0, [pc, #36]
	add	r2, sp, #372
	adds	r3, r4, r2
	adds	r2, r3, r0
	movs	r3, #4
	movs	r1, #0
	subs	r4, r3, r4
.L_0804953a:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_0804953a
.L_08049544:
	mov	r9, fp
.L_08049546:
	ldr	r1, [sp, #80]
	cmp	r1, #4
	ble.n	.L_080495a4
	movs	r4, #0
	adds	r5, r1, #0
	adds	r5, #3
	b.n	.L_08049594
	.2byte 0xfee8
	.2byte 0xffff
.L_08049558:
	movs	r2, #243
	lsls	r2, r2, #8
	adds	r2, #1
	mov	r3, fp
	adds	r1, r4, r2
	cmp	r3, #0
	bge.n	.L_08049568
	adds	r3, #3
.L_08049568:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08049576
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #11
	adds	r1, r4, r3
.L_08049576:
	ldr	r3, [sp, #76]
	str	r4, [sp, #4]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	movs	r0, #0
	adds	r2, r2, r4
	str	r0, [sp, #0]
	adds	r0, r3, #0
	movs	r3, #1
	subs	r2, #2
	negs	r3, r3
	bl	sub_0803c378
	ldr	r4, [sp, #4]
	adds	r4, #1
.L_08049594:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0804959e
	ldr	r3, [sp, #80]
	adds	r3, #6
.L_0804959e:
	asrs	r0, r3, #2
	cmp	r4, r0
	blt.n	.L_08049558
.L_080495a4:
	ldr	r2, [sp, #76]
	ldr	r4, [sp, #16]
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	ldrh	r2, [r2, #8]
	adds	r1, r1, r4
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	movs	r3, #1
	bl	sub_08046134
	ldr	r1, [sp, #72]
	movs	r0, #1
	movs	r2, #0
	strb	r0, [r1, #3]
	strb	r2, [r1, #6]
.L_080495ce:
	ldr	r6, [sp, #28]
	movs	r4, #0
	add	r5, sp, #92
.L_080495d4:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	cmp	r3, #0
	beq.n	.L_080495e8
	adds	r0, r6, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	sub_08014128
	ldr	r4, [sp, #4]
.L_080495e8:
	adds	r4, #1
	adds	r6, #12
	cmp	r4, #3
	ble.n	.L_080495d4
	ldr	r0, [sp, #76]
	ldr	r2, [sp, #16]
	movs	r4, #12
	ldrsh	r3, [r0, r4]
	ldr	r4, [sp, #24]
	lsls	r3, r3, #3
	subs	r3, #2
	str	r3, [sp, #40]
	movs	r1, #14
	ldrsh	r3, [r0, r1]
	movs	r0, #0
	adds	r3, r2, r3
	lsls	r3, r3, #3
	adds	r3, #20
	str	r3, [sp, #44]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r4, #4]
	str	r0, [r4, #8]
	ldr	r1, [sp, #12]
	movs	r5, #0
	lsrs	r0, r1, #16
	ldr	r1, [pc, #72]
	bl	sub_080143f8
	ldr	r3, [pc, #52]
	ldr	r2, [sp, #24]
	ands	r0, r3
	ldrh	r3, [r2, #8]
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #60]
	ldr	r4, [sp, #24]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [r1, #0]
	strh	r3, [r4, #8]
	movs	r3, #4
	ands	r0, r3
	ldr	r3, [sp, #40]
	lsrs	r2, r0, #1
	movs	r4, #255
	ldr	r1, [sp, #24]
	adds	r2, r3, r2
	lsls	r4, r4, #8
	ldr	r3, [pc, #20]
	adds	r4, #252
	adds	r2, r2, r4
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	b.n	.L_08049670
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x080597f8
	.2byte 0x122c
	.2byte 0x0300
.L_08049670:
	lsrs	r0, r0, #2
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #24]
	movs	r1, #242
	strh	r3, [r2, #6]
	ldr	r3, [sp, #44]
	subs	r0, r3, r0
	adds	r0, #248
	strb	r0, [r2, #4]
	ldr	r0, [sp, #24]
	bl	sub_08014128
	ldr	r4, [pc, #800]
	movs	r3, #8
	ldr	r6, [r4, #0]
	ands	r6, r3
.L_08049692:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	ldr	r0, [sp, #52]
	movs	r3, #15
	subs	r2, r3, r2
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080496ac
	movs	r2, #15
.L_080496ac:
	ldr	r3, [sp, #56]
	movs	r1, #12
	ldrsh	r0, [r3, r1]
	lsls	r3, r5, #1
	adds	r0, r0, r3
	ldr	r3, [sp, #56]
	adds	r0, #1
	movs	r4, #14
	ldrsh	r1, [r3, r4]
	adds	r5, #1
	str	r2, [sp, #0]
	adds	r1, #1
	movs	r2, #2
	movs	r3, #1
	bl	sub_08046134
	cmp	r5, #3
	ble.n	.L_08049692
	ldr	r4, [sp, #80]
	cmp	r4, #4
	ble.n	.L_0804978c
	movs	r4, #0
	ldr	r5, [sp, #80]
	adds	r5, #3
	b.n	.L_08049730
.L_080496de:
	ldr	r2, [pc, #716]
	movs	r0, #243
	ldr	r3, [r2, #0]
	lsls	r0, r0, #8
	movs	r2, #15
	adds	r0, #1
	ands	r3, r2
	adds	r1, r4, r0
	cmp	r3, #11
	bhi.n	.L_08049708
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_080496fa
	adds	r3, #3
.L_080496fa:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08049708
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #11
	adds	r1, r4, r3
.L_08049708:
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	ldrh	r3, [r0, #8]
	cmp	r5, #0
	bge.n	.L_08049716
	ldr	r2, [sp, #80]
	adds	r2, #6
.L_08049716:
	asrs	r2, r2, #2
	subs	r2, r3, r2
	adds	r2, r2, r4
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	ldr	r0, [sp, #76]
	subs	r3, #1
	str	r4, [sp, #4]
	bl	sub_0803c378
	ldr	r4, [sp, #4]
	adds	r4, #1
.L_08049730:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_0804973a
	ldr	r3, [sp, #80]
	adds	r3, #6
.L_0804973a:
	asrs	r2, r3, #2
	cmp	r4, r2
	blt.n	.L_080496de
	ldr	r4, [sp, #76]
	movs	r5, #1
	ldrh	r3, [r4, #8]
	movs	r1, #243
	negs	r5, r5
	subs	r2, r3, r2
	movs	r0, #0
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	adds	r3, r5, #0
	ldr	r0, [sp, #76]
	subs	r2, #3
	adds	r1, #52
	bl	sub_0803c378
	ldr	r1, [sp, #76]
	movs	r3, #0
	ldrh	r2, [r1, #8]
	adds	r0, r1, #0
	movs	r1, #243
	lsls	r1, r1, #8
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r1, #53
	adds	r3, r5, #0
	bl	sub_0803c378
	ldr	r0, [sp, #76]
	movs	r2, #2
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	ldr	r1, [sp, #72]
	subs	r3, #1
	lsrs	r3, r3, #2
	lsls	r2, r3
	ldrb	r3, [r1, #3]
	orrs	r2, r3
	strb	r2, [r1, #3]
.L_0804978c:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	mov	r4, sl
	mov	r3, fp
	str	r3, [r2, #52]
	str	r4, [r2, #48]
	ldr	r0, [sp, #48]
	str	r0, [r2, #56]
	ldr	r3, [pc, #524]
	ldr	r1, [r3, #4]
	ldr	r0, [r3, #12]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080497ca
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_080497c6
	movs	r3, #120
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	b.n	.L_080497ca
.L_080497c6:
	subs	r3, #1
	str	r3, [r2, #0]
.L_080497ca:
	adds	r3, r1, #0
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080497de
	ldr	r4, [sp, #32]
	mov	r3, fp
	add	r3, sl
	ldrb	r6, [r4, r3]
	b.n	.L_0804995a
.L_080497de:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_080497f4
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08049800
.L_080497f4:
	movs	r0, #113
	movs	r6, #1
	bl	sub_081c0010
	negs	r6, r6
	b.n	.L_0804995a
.L_08049800:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08049832
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #4
	beq.n	.L_08049822
	ldr	r2, [sp, #80]
	mov	r3, fp
	add	r3, sl
	cmp	r3, r2
	bne.n	.L_08049826
.L_08049822:
	movs	r3, #0
	mov	sl, r3
.L_08049826:
	mov	r0, sl
	mov	r4, sl
	lsls	r0, r0, #1
	str	r4, [sp, #48]
	str	r0, [sp, #16]
	b.n	.L_08049952
.L_08049832:
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0804987a
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #1
	negs	r1, r1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_0804986e
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_08049856
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_08049856:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_0804986a
	ldr	r4, [sp, #80]
	mov	r0, fp
	subs	r3, r4, r0
	subs	r3, #1
	mov	sl, r3
	b.n	.L_0804986e
.L_0804986a:
	movs	r1, #3
	mov	sl, r1
.L_0804986e:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	str	r3, [sp, #16]
	b.n	.L_08049952
.L_0804987a:
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080498d8
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	ldr	r4, [sp, #80]
	mov	r3, fp
	adds	r3, #4
	cmp	r3, r4
	blt.n	.L_080498ac
	mov	r0, fp
	cmp	r0, #0
	beq.n	.L_08049952
	ldr	r2, [sp, #48]
	movs	r1, #0
	mov	sl, r2
	mov	r3, sl
	lsls	r3, r3, #1
	mov	fp, r1
	str	r3, [sp, #16]
	b.n	.L_08049952
.L_080498ac:
	mov	fp, r3
	ldr	r4, [sp, #48]
	ldr	r3, [sp, #20]
	mov	sl, r4
	cmp	r3, #0
	bge.n	.L_080498bc
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_080498bc:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_08049934
	ldr	r0, [sp, #80]
	mov	r1, fp
	subs	r3, r0, r1
	ldr	r2, [sp, #48]
	subs	r3, #1
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L_0804993c
	mov	sl, r2
	b.n	.L_08049934
.L_080498d8:
	movs	r3, #32
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08049952
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	mov	r4, fp
	cmp	r4, #0
	beq.n	.L_08049902
	ldr	r1, [sp, #48]
	movs	r0, #4
	mov	sl, r1
	mov	r2, sl
	negs	r0, r0
	lsls	r2, r2, #1
	add	fp, r0
	str	r2, [sp, #16]
	b.n	.L_08049952
.L_08049902:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_0804990c
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_0804990c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	fp, r3
	ldr	r3, [sp, #48]
	mov	r4, fp
	mov	sl, r3
	cmp	r4, #0
	beq.n	.L_08049944
	ldr	r0, [sp, #80]
	ldr	r1, [sp, #48]
	subs	r3, r0, r4
	subs	r3, #1
	mov	sl, r3
	cmp	sl, r1
	ble.n	.L_0804994c
	mov	sl, r1
	mov	r2, sl
	lsls	r2, r2, #1
	str	r2, [sp, #16]
	b.n	.L_08049952
.L_08049934:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #16]
	b.n	.L_08049952
.L_0804993c:
	mov	r4, sl
	lsls	r4, r4, #1
	str	r4, [sp, #16]
	b.n	.L_08049952
.L_08049944:
	mov	r0, sl
	lsls	r0, r0, #1
	str	r0, [sp, #16]
	b.n	.L_08049952
.L_0804994c:
	mov	r1, sl
	lsls	r1, r1, #1
	str	r1, [sp, #16]
.L_08049952:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08049302
.L_0804995a:
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #3
	add	r5, sp, #96
.L_08049964:
	ldmia	r5!, {r0}
	str	r4, [sp, #4]
	bl	sub_08014274
	ldr	r4, [sp, #4]
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_08049964
	ldr	r2, [sp, #12]
	lsrs	r0, r2, #16
	bl	sub_08014274
	movs	r1, #1
	ldr	r0, [sp, #56]
	bl	sub_0803939c
	movs	r1, #1
	ldr	r0, [sp, #60]
	bl	sub_0803939c
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r6, #0
	add	sp, #372
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300122c
	.2byte 0x1150
	.2byte 0x0300
.L_080499b4:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r0, #0
	movs	r0, #1
	cmp	r6, #0
	beq.n	.L_08049a00
	adds	r0, r6, #0
	bl	sub_080ad010
	adds	r5, r0, #0
	ldrb	r3, [r5, #12]
	movs	r0, #1
	cmp	r3, #3
	beq.n	.L_08049a00
	ldrh	r3, [r5, #40]
	cmp	r3, #0
	beq.n	.L_08049a00
	ldrb	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L_080499ec
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080ad1c0
	cmp	r0, #0
	bne.n	.L_080499ec
	movs	r0, #1
	b.n	.L_08049a00
.L_080499ec:
	ldrh	r0, [r5, #40]
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	movs	r0, #2
	cmp	r3, #0
	beq.n	.L_08049a00
	movs	r0, #0
.L_08049a00:
	pop	{r5, r6, r7, pc}
	.align 2, 0
