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
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_0801521c, 0x0801521c
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.set sub_08118038, 0x08118038
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b290, 0x0815b290
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08163c2c, 0x08163c2c
	.set sub_08166fb4, 0x08166fb4
	.set sub_0816711a, 0x0816711a
	.set sub_081671b2, 0x081671b2
	.set sub_08167224, 0x08167224
	.set sub_08167238, 0x08167238
	.set sub_0816767c, 0x0816767c
	.set sub_08167e76, 0x08167e76
	.set sub_08168188, 0x08168188
	.set sub_08168272, 0x08168272
	.set sub_08168298, 0x08168298
	.set sub_08168320, 0x08168320
	.set sub_08168344, 0x08168344
	.set sub_0816836a, 0x0816836a
	.set sub_081683fa, 0x081683fa
	.set sub_08168750, 0x08168750
	.set sub_08168774, 0x08168774
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08166b10
	.thumb_func
Func_08166b10:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	str	r0, [sp, #48]
	str	r1, [sp, #44]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	str	r0, [sp, #40]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #36]
	ldr	r2, [r5, #100]
	str	r2, [sp, #20]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #48]
	add	r6, sp, #52
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	adds	r1, r6, #0
	bl	sub_0815e21c
	ldr	r3, [r6, #0]
	movs	r1, #19
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r0, #104
	str	r3, [sp, #16]
	bl	sub_081963ec
	ldr	r6, [r5, #104]
	movs	r0, #188
	movs	r1, #35
	str	r6, [sp, #24]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r0, [sp, #44]
	b.n	.L_08166b7c
	.2byte 0x1010
	.2byte 0x0000
.L_08166b7c:
	str	r5, [sp, #28]
	cmp	r0, #2
	beq.n	.L_08166b96
	ldr	r2, [sp, #40]
	movs	r3, #170
	lsls	r3, r3, #7
	adds	r3, #32
	adds	r1, r2, r3
	ldr	r0, [pc, #120]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_08166b96:
	ldr	r4, [sp, #40]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r4, r5
	ldr	r0, [pc, #108]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #160
	ldr	r6, [sp, #40]
	lsls	r2, r2, #5
	adds	r2, #208
	adds	r1, r6, r2
	ldr	r0, [pc, #92]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #84]
	ldr	r1, [sp, #20]
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #44]
	cmp	r3, #1
	bne.n	.L_08166c18
	movs	r4, #0
	mov	r8, r4
	ldr	r4, [pc, #48]
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r5, #31
.L_08166bda:
	ldrh	r3, [r0, #0]
	adds	r2, r5, #0
	ands	r2, r3
	lsls	r3, r3, #16
	lsrs	r1, r3, #21
	lsrs	r3, r3, #26
	ands	r3, r4
	lsls	r2, r2, #10
	lsls	r3, r3, #5
	movs	r6, #1
	ands	r1, r4
	orrs	r2, r3
	add	r8, r6
	orrs	r2, r1
	mov	r1, r8
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r1, #64
	bne.n	.L_08166bda
	b.n	.L_08166c18
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x00000118
	.4byte 0x00000146
	.4byte 0x0000013e
	.2byte 0x0134
	.2byte 0x0000
.L_08166c18:
	ldr	r3, [sp, #40]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r5, [sp, #40]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #132
	adds	r2, r5, r6
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #472]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r7, [sp, #40]
	movs	r0, #0
	mov	r8, r0
.L_08166c42:
	mov	r1, r8
	lsls	r6, r1, #1
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	mov	r2, r8
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #25
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L_08166c42
	ldr	r3, [pc, #396]
	movs	r5, #0
	movs	r1, #1
	movs	r2, #171
	mov	r8, r5
	negs	r1, r1
	lsls	r2, r2, #2
.L_08166c94:
	movs	r6, #1
	add	r8, r6
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08166c94
	ldr	r1, [sp, #16]
	ldr	r7, [pc, #372]
	lsls	r1, r1, #16
	str	r1, [sp, #8]
	movs	r0, #0
	mov	r8, r0
.L_08166cac:
	bl	sub_08014878
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	ldr	r2, [sp, #8]
	adds	r5, r0, #0
	adds	r3, #255
	ands	r5, r3
	movs	r3, #176
	lsls	r3, r3, #15
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r4, #170
	movs	r3, #1
	add	r8, r3
	lsls	r4, r4, #1
	adds	r7, #28
	cmp	r8, r4
	bne.n	.L_08166cac
	movs	r5, #0
	str	r5, [sp, #32]
.L_08166d0e:
	ldr	r6, [sp, #44]
	cmp	r6, #0
	bne.n	.L_08166d24
	ldr	r3, [sp, #32]
	subs	r3, #41
	cmp	r3, #22
	bhi.n	.L_08166d48
	ldr	r0, [pc, #252]
	bl	sub_0815f0a0
	b.n	.L_08166d48
.L_08166d24:
	ldr	r0, [sp, #44]
	cmp	r0, #1
	bne.n	.L_08166d48
	ldr	r1, [sp, #32]
	cmp	r1, #61
	bne.n	.L_08166d48
	ldr	r0, [pc, #236]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #232]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7f4
	.2byte 0xfc76
.L_08166d48:
	.2byte 0x9a08
	cmp	r2, #8
	bne.n	.L_08166d5a
	ldr	r4, [sp, #40]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #168
	adds	r3, r4, r5
	str	r2, [r3, #0]
.L_08166d5a:
	ldr	r6, [sp, #32]
	cmp	r6, #48
	bne.n	.L_08166d6e
	ldr	r0, [sp, #40]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
.L_08166d6e:
	ldr	r2, [sp, #32]
	cmp	r2, #60
	bne.n	.L_08166d82
	ldr	r3, [sp, #40]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #16
	str	r3, [r2, #0]
.L_08166d82:
	ldr	r5, [sp, #32]
	cmp	r5, #4
	bne.n	.L_08166d8e
	movs	r0, #212
	bl	sub_081c0010
.L_08166d8e:
	ldr	r6, [sp, #32]
	cmp	r6, #32
	bne.n	.L_08166d9a
	movs	r0, #164
	bl	sub_081c0010
.L_08166d9a:
	ldr	r0, [sp, #32]
	cmp	r0, #60
	bne.n	.L_08166db4
	ldr	r1, [sp, #44]
	cmp	r1, #1
	bne.n	.L_08166dae
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_08166db4
.L_08166dae:
	movs	r0, #145
	bl	sub_081180e8
.L_08166db4:
	ldr	r2, [sp, #32]
	cmp	r2, #55
	ble.n	.L_08166e3e
	ldr	r4, [pc, #108]
	ldr	r5, [sp, #40]
	movs	r3, #0
	mov	r8, r3
	mov	sl, r4
.L_08166dc4:
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	ldr	r1, [sp, #16]
	ldr	r0, [r5, #24]
	movs	r6, #6
	ldrsh	r7, [r5, r6]
	adds	r6, r3, r1
	cmp	r0, #17
	bhi.n	.L_08166e08
	movs	r1, #3
	bl	sub_08002054
	mov	r2, sl
	ldrb	r1, [r2, r0]
	ldr	r3, [sp, #40]
	movs	r4, #160
	movs	r0, #32
	lsls	r1, r1, #11
	lsls	r4, r4, #5
	adds	r1, r3, r1
	adds	r4, #208
	adds	r2, r6, #0
	str	r0, [sp, #0]
	adds	r3, r7, #0
	movs	r0, #64
	str	r0, [sp, #4]
	adds	r1, r1, r4
	subs	r2, #16
	adds	r3, #48
	ldr	r0, [sp, #36]
	ldr	r6, [sp, #24]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x69a8
.L_08166e08:
	cmp	r0, #0
	ble.n	.L_08166e2c
	subs	r3, r0, #1
	b.n	.L_08166e30
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x02014ad0
	.4byte 0x00000184
	.4byte 0x00000188
	.4byte 0x03000730
	.2byte 0x8a9e
	.2byte 0x0819
.L_08166e2c:
	movs	r3, #1
	negs	r3, r3
.L_08166e30:
	str	r3, [r5, #24]
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_08166dc4
.L_08166e3e:
	ldr	r2, [sp, #32]
	cmp	r2, #28
	bne.n	.L_08166ec6
	ldr	r7, [pc, #784]
	movs	r3, #0
	movs	r4, #63
	mov	r8, r3
	mov	sl, r4
.L_08166e4e:
	ldr	r3, [r7, #24]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_08166eb8
	bl	sub_08014878
	adds	r6, r0, #0
	mov	r0, sl
	ands	r6, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r1, [sp, #8]
	asrs	r3, r3, #3
	adds	r3, r3, r1
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #192
	lsls	r2, r2, #15
	asrs	r3, r3, #2
	adds	r3, r3, r2
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r4, sl
	ands	r0, r4
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L_08166eb8:
	movs	r5, #1
	movs	r6, #128
	add	r8, r5
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L_08166e4e
.L_08166ec6:
	ldr	r0, [sp, #32]
	subs	r0, #32
	str	r0, [sp, #12]
	cmp	r0, #31
	bhi.n	sub_08166fb4
	ldr	r7, [pc, #644]
	movs	r1, #0
	movs	r2, #63
	mov	r9, r1
	mov	r8, r1
	mov	sl, r2
.L_08166edc:
	ldr	r3, [r7, #24]
	movs	r4, #1
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_08166f50
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	movs	r3, #255
	mov	r5, sl
	lsls	r3, r3, #8
	ands	r6, r5
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r0, [sp, #8]
	asrs	r3, r3, #3
	adds	r3, r3, r0
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #192
	lsls	r1, r1, #15
	asrs	r3, r3, #2
	adds	r3, r3, r1
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r3, sl
	ands	r0, r3
	negs	r0, r0
	movs	r4, #1
	subs	r0, #8
	add	r9, r4
	lsls	r0, r0, #13
	movs	r3, #0
	mov	r5, r9
	str	r0, [r7, #16]
	str	r3, [r7, #24]
	cmp	r5, #16
	beq.n	.L_08166f5e
.L_08166f50:
	movs	r6, #1
	movs	r0, #171
	add	r8, r6
	lsls	r0, r0, #2
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_08166edc
.L_08166f5e:
	ldr	r1, [sp, #12]
	cmp	r1, #31
	bhi.n	sub_08166fb4
	ldr	r2, [sp, #32]
	ldr	r1, [sp, #24]
	ldr	r4, [sp, #40]
	lsls	r0, r2, #4
	ldr	r2, [pc, #492]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r4, r4, r5
	movs	r3, #104
	mov	fp, r1
	adds	r0, r0, r2
	movs	r1, #104
	mov	sl, r4
	mov	r8, r3
	ldr	r6, [sp, #16]
	bl	sub_08002064
	subs	r6, #17
	adds	r5, r0, #0
	mov	r9, r6
	mov	r4, r8
	movs	r6, #34
	movs	r3, #4
	subs	r3, r3, r5
	mov	r1, sl
	mov	r2, r9
	str	r6, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x236c
	subs	r3, r3, r5
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #36]
	mov	r1, sl
	mov	r2, r9
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9d08
	cmp	r5, #71
	bgt.n	.L_081670b0
	ldr	r5, [pc, #412]
	movs	r6, #0
	mov	r8, r6
.L_08166fc0:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_081670a2
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L_08166fd8
	adds	r4, #2
.L_08166fd8:
	ldr	r0, [sp, #32]
	cmp	r0, #68
	ble.n	.L_08166fe4
	cmp	r4, #5
	bgt.n	.L_08166fe4
	movs	r4, #6
.L_08166fe4:
	ldr	r1, [sp, #32]
	cmp	r1, #70
	ble.n	.L_08166ff0
	cmp	r4, #6
	bgt.n	.L_08166ff0
	movs	r4, #7
.L_08166ff0:
	ldr	r2, [sp, #32]
	cmp	r2, #72
	ble.n	.L_08166ffc
	cmp	r4, #7
	bgt.n	.L_08166ffc
	movs	r4, #8
.L_08166ffc:
	ldr	r3, [sp, #32]
	cmp	r3, #74
	ble.n	.L_08167008
	cmp	r4, #8
	bgt.n	.L_08167008
	movs	r4, #9
.L_08167008:
	ldr	r6, [sp, #32]
	cmp	r6, #76
	ble.n	.L_08167010
	movs	r4, #10
.L_08167010:
	ldr	r2, [pc, #332]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r6, [sp, #32]
	cmp	r6, #80
	ble.n	.L_08167056
	ldr	r0, [pc, #272]
	adds	r3, r1, r0
	b.n	.L_08167064
.L_08167056:
	ldr	r3, [pc, #272]
	movs	r2, #3
	mov	r4, r8
	ands	r2, r4
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L_08167064:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_08167074
	adds	r3, #63
.L_08167074:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L_08167086
	adds	r2, #63
.L_08167086:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L_081670a2
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	cmp	r3, #108
	ble.n	.L_081670a2
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_081670a2:
	movs	r0, #1
	movs	r1, #171
	add	r8, r0
	lsls	r1, r1, #1
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_08166fc0
.L_081670b0:
	ldr	r2, [sp, #32]
	cmp	r2, #95
	bgt.n	sub_0816711a
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	subs	r2, #18
	movs	r1, #120
	cmp	r3, #60
	ble.n	.L_081670ca
	ldr	r5, [pc, #168]
	lsls	r3, r3, #3
	adds	r4, r3, r5
	b.n	.L_081670ee
.L_081670ca:
	ldr	r6, [sp, #32]
	cmp	r6, #32
	ble.n	.L_081670de
	ldr	r0, [sp, #12]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r4, r3, #0
	adds	r4, #16
	b.n	.L_081670ee
.L_081670de:
	ldr	r3, [sp, #32]
	cmp	r3, #9
	bgt.n	.L_081670ec
	lsls	r3, r3, #4
	adds	r4, r3, #0
	subs	r4, #128
	b.n	.L_081670ee
.L_081670ec:
	movs	r4, #16
.L_081670ee:
	adds	r3, r4, #0
	adds	r3, #120
	cmp	r3, #108
	ble.n	.L_081670fc
	subs	r3, r1, r4
	adds	r1, r3, #0
	subs	r1, #12
.L_081670fc:
	cmp	r1, #0
	ble.n	sub_0816711a
	ldr	r5, [sp, #40]
	movs	r6, #170
	lsls	r6, r6, #7
	movs	r3, #36
	adds	r6, #32
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #36]
	adds	r1, r5, r6
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d08
	cmp	r5, #59
	ble.n	.L_081671c0
	ldr	r7, [pc, #76]
	movs	r6, #0
	mov	r8, r6
.L_08167126:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	ble.n	sub_081671b2
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #64
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r7, #24]
	ldr	r6, [r7, #4]
	movs	r1, #216
	subs	r0, r3, #1
	lsls	r1, r1, #15
	str	r0, [r7, #24]
	cmp	r6, r1
	ble.n	.L_08167174
	ldr	r3, [r7, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #16]
	b.n	sub_081671b2
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffffff00
	.4byte 0x08197410
	.4byte 0xffff8000
	.4byte 0x08198aa4
	.4byte 0xfffffe3e
	.2byte 0x4ad0
	.2byte 0x0201
.L_08167174:
	ldr	r5, [r7, #0]
	ldr	r2, [pc, #272]
	cmp	r5, r2
	bhi.n	sub_081671b2
	cmp	r6, #0
	blt.n	sub_081671b2
	movs	r1, #5
	bl	sub_08002054
	ldr	r2, [pc, #260]
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #20]
	asrs	r5, r5, #16
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	asrs	r6, r6, #16
	subs	r5, r5, r3
	subs	r6, r6, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r4, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2501
	movs	r6, #170
	add	r8, r5
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L_08167126
.L_081671c0:
	ldr	r0, [sp, #32]
	cmp	r0, #68
	bne.n	.L_081671fc
	ldr	r2, [sp, #48]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_081671fc
	movs	r5, #36
.L_081671d4:
	ldr	r3, [sp, #48]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r6, [sp, #48]
	ldrsh	r0, [r5, r6]
	movs	r1, #7
	bl	sub_08118088
	ldr	r3, [r6, #20]
	movs	r2, #1
	add	r8, r2
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_081671d4
.L_081671fc:
	ldr	r3, [sp, #32]
	cmp	r3, #9
	bne.n	sub_08167224
	ldr	r5, [sp, #48]
	movs	r3, #8
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #128
	ldr	r3, [pc, #116]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #116]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9e08
	cmp	r6, #60
	bne.n	sub_08167238
	movs	r1, #128
	ldr	r3, [pc, #96]
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2010
	movs	r1, #16
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #40]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #32]
	adds	r2, #1
	str	r2, [sp, #32]
	cmp	r2, #102
	beq.n	.L_08167264
	b.n	.L_08166d0e
.L_08167264:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x007effff
	.4byte 0x08197410
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #188
	str	r0, [sp, #96]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	adds	r3, r5, #0
	str	r0, [sp, #92]
	adds	r3, #176
	ldr	r1, [r5, #92]
	movs	r0, #128
	str	r1, [sp, #88]
	lsls	r0, r0, #6
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #76]
	str	r3, [sp, #84]
	ldr	r2, [r5, #100]
	str	r2, [sp, #68]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	bl	sub_0813ba50
	ldr	r2, [pc, #48]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [sp, #88]
	movs	r4, #239
	lsls	r4, r4, #7
	ldr	r5, [r5, #104]
	adds	r2, r3, r4
	movs	r1, #200
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #16]
	str	r5, [sp, #76]
	b.n	.L_0816731c
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
.L_0816731c:
	bl	sub_080145a8
	movs	r1, #0
	movs	r0, #0
	bl	sub_08163c2c
	movs	r0, #240
	ldr	r5, [sp, #88]
	lsls	r0, r0, #7
	adds	r0, #240
	adds	r3, r5, r0
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r1, [pc, #232]
	movs	r2, #0
	movs	r0, #1
	bl	sub_08118040
	ldr	r0, [pc, #228]
	bl	sub_08013300
	movs	r2, #128
	adds	r7, r0, #0
	ldr	r5, [pc, #220]
	adds	r1, r7, #0
	lsls	r2, r2, #2
	ldr	r0, [pc, #220]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #2
	adds	r7, r7, r1
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_0801587c
	ldr	r2, [pc, #204]
	ldr	r3, [sp, #88]
	movs	r4, #238
	lsls	r4, r4, #7
	movs	r0, #13
	adds	r4, #220
	negs	r0, r0
	mov	sl, r5
	movs	r7, #0
	mov	r9, r2
	adds	r5, r3, r4
	mov	r8, r0
.L_0816737e:
	movs	r1, #32
	ldr	r2, [pc, #180]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	mov	r1, r8
	ands	r3, r1
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r0, [r3, #2]
	ldr	r2, [pc, #152]
	adds	r1, r6, #0
	adds	r0, r0, r2
	movs	r2, #128
	lsls	r2, r2, #3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #3
	adds	r7, #1
	adds	r6, r6, r3
	cmp	r7, #16
	bne.n	.L_0816737e
	ldr	r2, [pc, #132]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #84]
	ldr	r2, [pc, #120]
	movs	r3, #1
	str	r3, [r4, #16]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r1, #1
	bl	sub_08163c2c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [sp, #88]
	ldr	r5, [pc, #80]
	movs	r3, #224
	lsls	r3, r3, #3
	movs	r6, #128
	adds	r1, r2, r3
	lsls	r6, r6, #12
	ldr	r0, [pc, #72]
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #64]
	str	r6, [sp, #60]
	bl	sub_08157cf4
	b.n	.L_08167450
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x00000045
	.4byte 0x000000a8
	.4byte 0x03000730
	.4byte 0x05000200
	.4byte 0x020036e0
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x030011e0
	.4byte 0x03001120
	.4byte 0xffc00000
	.2byte 0x017f
	.2byte 0x0000
.L_08167450:
	ldr	r4, [sp, #88]
	movs	r5, #208
	lsls	r5, r5, #4
	adds	r1, r4, r5
	ldr	r0, [pc, #124]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #120]
	ldr	r1, [pc, #120]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #68]
	movs	r3, #0
	ldr	r0, [pc, #108]
	bl	sub_08157cf4
	ldr	r0, [pc, #108]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #100]
	adds	r1, r7, #0
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9e16
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r6, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	mov	r3, sp
	movs	r2, #0
	adds	r3, #176
	movs	r4, #148
	str	r2, [sp, #72]
	str	r3, [sp, #12]
	add	r4, sp
	mov	fp, r4
.L_081674c2:
	ldr	r5, [sp, #72]
	cmp	r5, #0
	bne.n	.L_08167586
	movs	r6, #255
	ldr	r5, [sp, #88]
	lsls	r6, r6, #8
	movs	r7, #0
	adds	r6, #255
	b.n	.L_081674f0
	.4byte 0x00000784
	.4byte 0x00000192
	.4byte 0x000000c2
	.4byte 0x02014000
	.4byte 0x00000134
	.4byte 0x00000126
	.2byte 0x0730
	.2byte 0x0300
.L_081674f0:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #72
	str	r3, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #16]
	bl	sub_08014878
	adds	r7, #1
	ands	r0, r6
	str	r0, [r5, #16]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_081674f0
	ldr	r0, [pc, #820]
	bl	sub_08013300
	movs	r6, #128
	adds	r7, r0, #0
	lsls	r6, r6, #2
	movs	r2, #128
	ldr	r5, [pc, #812]
	adds	r1, r7, #0
	lsls	r2, r2, #2
	adds	r7, r7, r6
	ldr	r0, [pc, #808]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c38
	ldr	r1, [pc, #804]
	bl	sub_0801587c
	ldr	r0, [pc, #800]
	ldr	r1, [sp, #88]
	movs	r2, #238
	adds	r4, r5, #0
	lsls	r2, r2, #7
	ldr	r5, [pc, #788]
	adds	r2, #220
	movs	r7, #0
	mov	r8, r0
	adds	r6, r1, r2
.L_08167554:
	ldmia	r6!, {r3}
	movs	r2, #128
	ldrb	r3, [r3, #16]
	adds	r1, r5, #0
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #772]
	str	r4, [sp, #8]
	adds	r0, r0, r3
	lsls	r2, r2, #3
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #3
	adds	r7, #1
	adds	r5, r5, r0
	ldr	r4, [sp, #8]
	cmp	r7, #16
	bne.n	.L_08167554
	ldr	r1, [pc, #748]
	movs	r2, #128
	lsls	r2, r2, #12
	str	r1, [sp, #64]
	str	r2, [sp, #60]
.L_08167586:
	ldr	r3, [sp, #72]
	cmp	r3, #143
	bne.n	.L_081675a0
	movs	r1, #128
	ldr	r3, [pc, #736]
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	ldr	r2, [pc, #732]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2091
	bl	sub_081c0010
.L_081675a0:
	ldr	r4, [sp, #72]
	cmp	r4, #80
	bne.n	.L_081675ac
	movs	r0, #142
	bl	sub_081c0010
.L_081675ac:
	ldr	r5, [sp, #72]
	cmp	r5, #72
	bne.n	.L_081675cc
	ldr	r6, [sp, #88]
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
.L_081675cc:
	ldr	r2, [sp, #12]
	movs	r3, #0
	str	r3, [r2, #8]
	str	r3, [r2, #4]
	ldr	r6, [sp, #88]
	movs	r7, #0
.L_081675d8:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_081675e0
	adds	r3, r7, #3
.L_081675e0:
	ldr	r4, [sp, #72]
	asrs	r3, r3, #2
	adds	r3, #80
	cmp	r4, r3
	ble.n	sub_0816767c
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	sub_0816767c
	bl	sub_08014de4
	ldr	r0, [r6, #16]
	bl	sub_080150e4
	ldr	r0, [r6, #12]
	bl	sub_08015024
	ldr	r5, [sp, #72]
	ldr	r0, [r6, #16]
	lsls	r3, r5, #9
	adds	r0, r0, r3
	bl	sub_08015068
	ldr	r3, [r6, #0]
	ldr	r0, [sp, #12]
	str	r3, [r0, #0]
	ldr	r3, [r6, #0]
	subs	r3, #2
	str	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_08167620
	movs	r3, #0
	str	r3, [r6, #0]
.L_08167620:
	add	r5, sp, #164
	ldr	r0, [sp, #12]
	adds	r1, r5, #0
	bl	sub_0815e1ec
	ldr	r0, [r5, #8]
	movs	r3, #60
	negs	r3, r3
	cmp	r0, r3
	bge.n	.L_08167638
	str	r3, [r5, #8]
	adds	r0, r3, #0
.L_08167638:
	cmp	r0, #60
	ble.n	.L_08167642
	movs	r3, #60
	str	r3, [r5, #8]
	movs	r0, #60
.L_08167642:
	adds	r0, #60
	str	r0, [r5, #8]
	movs	r1, #20
	bl	sub_08002054
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r0, #2
	adds	r2, #56
	adds	r3, #96
	ldr	r4, [pc, #544]
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	lsls	r5, r0, #1
	subs	r1, r5, #2
	ldrh	r1, [r4, r1]
	ldr	r4, [sp, #68]
	subs	r3, r3, r0
	adds	r1, r4, r1
	lsrs	r4, r0, #31
	adds	r4, r0, r4
	asrs	r4, r4, #1
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	subs	r2, r2, r4
	ldr	r0, [sp, #92]
	ldr	r5, [sp, #76]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x3701
	adds	r6, #28
	cmp	r7, #64
	bne.n	.L_081675d8
	ldr	r3, [pc, #500]
	mov	r6, fp
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r1, #238
	str	r3, [sp, #124]
	str	r4, [sp, #128]
	movs	r3, #0
	str	r3, [r6, #12]
	str	r3, [r6, #4]
	ldr	r0, [sp, #88]
	lsls	r1, r1, #7
	adds	r1, #220
	movs	r7, #0
	adds	r5, r0, r1
.L_081676a2:
	ldr	r2, [sp, #72]
	cmp	r2, #75
	ble.n	.L_081676b4
	cmp	r7, #8
	beq.n	.L_081676e2
	cmp	r7, #12
	beq.n	.L_081676e2
	cmp	r7, #15
	beq.n	.L_081676e2
.L_081676b4:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_081676bc
	adds	r3, r7, #3
.L_081676bc:
	asrs	r3, r3, #2
	lsls	r2, r3, #2
	subs	r2, r7, r2
	movs	r4, #152
	lsls	r2, r2, #21
	lsls	r4, r4, #15
	adds	r2, r2, r4
	mov	r6, fp
	str	r2, [r6, #0]
	ldr	r0, [sp, #64]
	lsls	r3, r3, #21
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldr	r0, [r5, #0]
	mov	r1, fp
	add	r2, sp, #124
	movs	r3, #0
	bl	sub_08020010
.L_081676e2:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #16
	bne.n	.L_081676a2
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #60]
	ldr	r3, [sp, #72]
	adds	r1, r1, r2
	str	r1, [sp, #64]
	cmp	r3, #47
	bgt.n	.L_081676fe
	ldr	r4, [pc, #388]
	adds	r2, r2, r4
	str	r2, [sp, #60]
.L_081676fe:
	ldr	r5, [sp, #72]
	cmp	r5, #32
	ble.n	.L_08167716
	ldr	r6, [sp, #60]
	lsls	r3, r6, #4
	subs	r3, r3, r6
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_08167712
	adds	r3, #63
.L_08167712:
	asrs	r3, r3, #6
	str	r3, [sp, #60]
.L_08167716:
	ldr	r0, [sp, #72]
	cmp	r0, #144
	bne.n	.L_08167720
	ldr	r1, [pc, #356]
	str	r1, [sp, #60]
.L_08167720:
	ldr	r2, [sp, #72]
	cmp	r2, #146
	bne.n	.L_0816772c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #60]
.L_0816772c:
	ldr	r4, [sp, #72]
	cmp	r4, #72
	bne.n	.L_081677ae
	ldr	r0, [pc, #340]
	bl	sub_08013300
	movs	r5, #128
	adds	r7, r0, #0
	lsls	r5, r5, #2
	movs	r2, #128
	ldr	r3, [pc, #276]
	adds	r1, r7, #0
	lsls	r2, r2, #2
	adds	r7, r7, r5
	ldr	r0, [pc, #272]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c38
	ldr	r1, [pc, #268]
	bl	sub_0801587c
	ldr	r1, [pc, #268]
	ldr	r2, [sp, #88]
	movs	r3, #238
	lsls	r3, r3, #7
	ldr	r6, [pc, #300]
	ldr	r0, [pc, #300]
	ldr	r5, [pc, #252]
	adds	r3, #220
	mov	ip, r1
	movs	r7, #0
	adds	r1, r2, r3
.L_0816776c:
	ldrh	r3, [r0, #0]
	mov	lr, r3
	strh	r0, [r0, #0]
	ldrh	r3, [r6, #0]
	cmp	r3, #31
	bgt.n	.L_0816779c
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r2, r6, r2
	adds	r2, #4
	adds	r3, #1
	strh	r3, [r6, #0]
	stmia	r2!, {r5}
	ldr	r3, [r1, #0]
	ldr	r4, [pc, #220]
	ldrb	r3, [r3, #16]
	lsls	r3, r3, #2
	add	r3, ip
	ldrh	r3, [r3, #2]
	adds	r3, r3, r4
	stmia	r2!, {r3}
	ldr	r3, [pc, #248]
	str	r3, [r2, #0]
.L_0816779c:
	mov	r2, lr
	strh	r2, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r7, #1
	adds	r5, r5, r3
	adds	r1, #4
	cmp	r7, #16
	bne.n	.L_0816776c
.L_081677ae:
	ldr	r4, [sp, #72]
	cmp	r4, #76
	bne.n	.L_08167836
	ldr	r0, [pc, #224]
	bl	sub_08013300
	movs	r5, #128
	adds	r7, r0, #0
	lsls	r5, r5, #2
	movs	r2, #128
	adds	r1, r7, #0
	ldr	r3, [pc, #144]
	adds	r7, r7, r5
	lsls	r2, r2, #2
	ldr	r0, [pc, #144]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c38
	ldr	r1, [pc, #140]
	bl	sub_0801587c
	ldr	r0, [pc, #136]
	ldr	r6, [pc, #176]
	ldr	r1, [pc, #176]
	ldr	r5, [pc, #128]
	ldr	r4, [pc, #184]
	movs	r7, #0
	mov	ip, r0
.L_081677e6:
	ldrh	r3, [r1, #0]
	mov	lr, r3
	strh	r1, [r1, #0]
	ldrh	r3, [r6, #0]
	cmp	r3, #31
	bgt.n	.L_08167824
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r6, #0]
	lsls	r2, r2, #2
	ldrb	r3, [r4, #0]
	adds	r2, r6, r2
	movs	r0, #238
	adds	r2, #4
	lsls	r0, r0, #7
	stmia	r2!, {r5}
	adds	r0, #220
	lsls	r3, r3, #2
	adds	r3, r3, r0
	ldr	r0, [sp, #88]
	ldr	r3, [r0, r3]
	ldr	r0, [pc, #84]
	ldrb	r3, [r3, #16]
	lsls	r3, r3, #2
	add	r3, ip
	ldrh	r3, [r3, #2]
	adds	r3, r3, r0
	stmia	r2!, {r3}
	ldr	r3, [pc, #112]
	str	r3, [r2, #0]
.L_08167824:
	mov	r2, lr
	strh	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r7, #1
	adds	r5, r5, r3
	adds	r4, #1
	cmp	r7, #13
	bne.n	.L_081677e6
.L_08167836:
	ldr	r6, [sp, #72]
	subs	r6, #116
	cmp	r6, #27
	bls.n	.L_08167840
	b.n	.L_08167956
.L_08167840:
	ldr	r4, [sp, #72]
	ldr	r0, [pc, #92]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	adds	r5, r3, r0
	cmp	r5, #80
	ble.n	.L_081678a4
	movs	r5, #80
	b.n	.L_081678b8
	movs	r0, r0
	.4byte 0x000000a8
	.4byte 0x03000730
	.4byte 0x05000200
	.4byte 0x02010000
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0xffc00000
	.4byte 0x03000260
	.4byte 0x2a2a2a2a
	.4byte 0x08197410
	.4byte 0x08196e74
	.4byte 0xffffc000
	.4byte 0xfff80000
	.4byte 0x000000a9
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x84000100
	.4byte 0x000000aa
	.4byte 0x08198ab4
	.2byte 0xfea6
	.2byte 0xffff
.L_081678a4:
	ldr	r1, [sp, #88]
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #86
	adds	r0, r1, r2
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
.L_081678b8:
	ldr	r3, [sp, #88]
	movs	r4, #200
	lsls	r4, r4, #5
	adds	r4, #86
	adds	r0, r3, r4
	movs	r1, #60
	movs	r2, #80
	adds	r3, r5, #0
	bl	sub_0818caa8
	cmp	r6, #27
	bhi.n	.L_08167956
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_08002054
	adds	r0, #16
	mov	r8, r0
	cmp	r0, #32
	ble.n	.L_081678e4
	movs	r5, #32
	mov	r8, r5
.L_081678e4:
	ldr	r6, [pc, #256]
	movs	r7, #0
	mov	sl, r6
.L_081678ea:
	movs	r4, #3
	ands	r4, r7
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r4, [sp, #8]
	mov	r6, r8
	muls	r6, r0
	mov	r0, sl
	ldrb	r3, [r0, r4]
	asrs	r6, r6, #16
	lsrs	r3, r3, #1
	adds	r0, r5, #0
	subs	r6, r6, r3
	bl	sub_08002090
	ldr	r2, [pc, #208]
	ldr	r4, [sp, #8]
	mov	r3, r8
	muls	r3, r0
	ldrb	r0, [r2, r4]
	ldr	r1, [pc, #200]
	lsrs	r2, r0, #1
	asrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r2, r4, #1
	ldrh	r1, [r1, r2]
	ldr	r2, [sp, #88]
	movs	r5, #224
	adds	r1, r2, r1
	lsls	r5, r5, #3
	adds	r1, r1, r5
	mov	r5, sl
	ldrb	r2, [r5, r4]
	adds	r6, #60
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	adds	r2, r6, #0
	adds	r3, #80
	ldr	r0, [sp, #92]
	ldr	r6, [sp, #76]
	adds	r7, #1
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2f06
	bne.n	.L_081678ea
.L_08167956:
	ldr	r2, [sp, #72]
	movs	r3, #0
	subs	r2, #108
	cmp	r2, #34
	bhi.n	.L_0816797e
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r2, r3, #1
	adds	r1, r2, #0
	cmp	r2, #0
	bge.n	.L_0816796e
	adds	r1, r2, #3
.L_0816796e:
	asrs	r1, r1, #2
	lsrs	r3, r3, #31
	adds	r3, r2, r3
	str	r1, [sp, #56]
	str	r2, [sp, #48]
	asrs	r3, r3, #1
	str	r3, [sp, #52]
	movs	r3, #1
.L_0816797e:
	ldr	r0, [sp, #72]
	cmp	r0, #143
	bne.n	.L_0816798e
	movs	r1, #31
	str	r1, [sp, #56]
	str	r1, [sp, #52]
	str	r1, [sp, #48]
	movs	r3, #1
.L_0816798e:
	ldr	r2, [sp, #72]
	subs	r2, #144
	mov	r9, r2
	cmp	r2, #8
	bhi.n	.L_081679aa
	ldr	r3, [sp, #72]
	lsls	r2, r3, #1
	movs	r3, #152
	lsls	r3, r3, #1
	subs	r2, r3, r2
	str	r2, [sp, #52]
	str	r2, [sp, #56]
	str	r2, [sp, #48]
	movs	r3, #1
.L_081679aa:
	cmp	r3, #1
	bne.n	.L_08167a2c
	ldr	r0, [pc, #68]
	bl	sub_08013300
	ldr	r5, [pc, #44]
	ldr	r4, [pc, #64]
	movs	r7, #0
.L_081679ba:
	ldrh	r3, [r0, #0]
	ldr	r6, [sp, #56]
	movs	r2, #31
	ands	r2, r3
	adds	r1, r2, r6
	lsls	r3, r3, #16
	ldr	r6, [sp, #52]
	lsrs	r2, r3, #21
	ands	r2, r5
	adds	r2, r2, r6
	ldr	r6, [sp, #48]
	lsrs	r3, r3, #26
	ands	r3, r5
	adds	r3, r3, r6
	cmp	r1, #31
	ble.n	.L_081679dc
	movs	r1, #31
.L_081679dc:
	cmp	r2, #31
	ble.n	.L_081679fc
	movs	r2, #31
	b.n	.L_081679fc
	.4byte 0x0000001f
	.4byte 0x08198ace
	.4byte 0x08198ad4
	.4byte 0x08198ac2
	.4byte 0x000000aa
	.2byte 0x0200
	.2byte 0x0500
.L_081679fc:
	cmp	r3, #31
	ble.n	.L_08167a02
	movs	r3, #31
.L_08167a02:
	cmp	r1, #0
	bge.n	.L_08167a08
	movs	r1, #0
.L_08167a08:
	cmp	r2, #0
	bge.n	.L_08167a0e
	movs	r2, #0
.L_08167a0e:
	cmp	r3, #0
	bge.n	.L_08167a14
	movs	r3, #0
.L_08167a14:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	movs	r1, #128
	adds	r7, #1
	lsls	r1, r1, #1
	strh	r3, [r4, #0]
	adds	r0, #2
	adds	r4, #2
	cmp	r7, r1
	bne.n	.L_081679ba
.L_08167a2c:
	ldr	r2, [sp, #72]
	cmp	r2, #143
	ble.n	.L_08167a94
	ldr	r3, [pc, #516]
	lsls	r2, r2, #4
	mov	r8, r2
	movs	r1, #3
	movs	r0, #188
	add	r8, r3
	bl	sub_081963ec
	movs	r6, #24
	movs	r5, #64
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r2, [sp, #88]
	movs	r4, #192
	lsls	r4, r4, #18
	movs	r3, #224
	adds	r4, #188
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [sp, #92]
	movs	r2, #36
	mov	r3, r8
	mov	sl, r4
	ldr	r4, [r4, #0]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #7
	movs	r0, #188
	bl	sub_081963ec
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r6, [sp, #88]
	movs	r2, #224
	lsls	r2, r2, #3
	mov	r5, sl
	adds	r1, r6, r2
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #92]
	movs	r2, #60
	mov	r3, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_08167a94:
	mov	r3, r9
	cmp	r3, #15
	bhi.n	.L_08167b26
	movs	r0, #32
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r4, [sp, #72]
	ldr	r1, [pc, #400]
	lsls	r3, r4, #13
	adds	r7, r3, r1
	ldr	r2, [pc, #396]
	ldr	r3, [sp, #116]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #392]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [pc, #384]
	adds	r5, r0, #0
	str	r3, [sp, #116]
	add	r3, sp, #116
	str	r2, [r3, #4]
	str	r3, [r5, #16]
	ldr	r3, [pc, #376]
	movs	r0, #0
	str	r1, [r5, #0]
	str	r3, [r5, #8]
	str	r6, [r5, #12]
	cmp	r4, #151
	ble.n	.L_08167ae8
	lsls	r3, r4, #3
	movs	r2, #152
	negs	r3, r3
	lsls	r2, r2, #3
	adds	r0, r3, r2
.L_08167ae8:
	str	r0, [r5, #20]
	bl	sub_08014de4
	movs	r1, #192
	lsls	r1, r1, #13
	movs	r2, #0
	ldr	r0, [pc, #344]
	bl	sub_08015160
	adds	r0, r7, #0
	bl	sub_0801521c
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #184
	bl	sub_08015024
	adds	r1, r6, #0
	movs	r2, #4
	ldr	r0, [pc, #324]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r6, #0
	bl	sub_08013164
.L_08167b26:
	ldr	r3, [sp, #88]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #72]
	adds	r5, #1
	str	r5, [sp, #72]
	cmp	r5, #160
	beq.n	.L_08167b58
	cmp	r5, #4
	bgt.n	.L_08167b4a
	b.n	.L_081674c2
.L_08167b4a:
	ldr	r3, [pc, #268]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08167b58
	b.n	.L_081674c2
.L_08167b58:
	movs	r1, #128
	ldr	r3, [pc, #256]
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20ee
	ldr	r6, [sp, #88]
	lsls	r0, r0, #7
	adds	r0, #220
	movs	r7, #0
	adds	r5, r6, r0
.L_08167b72:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08020048
	cmp	r7, #16
	bne.n	.L_08167b72
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08014c4c
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #200]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #192]
	movs	r3, #160
	ldrh	r2, [r2, #0]
	lsls	r3, r3, #19
	adds	r3, #188
	movs	r6, #238
	movs	r1, #13
	strh	r2, [r3, #0]
	lsls	r6, r6, #7
	negs	r1, r1
	movs	r7, #0
	adds	r6, #220
	mov	r8, r1
.L_08167bba:
	movs	r0, #199
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_08020040
	ldr	r2, [sp, #88]
	adds	r5, r0, #0
	str	r5, [r6, r2]
	cmp	r5, #0
	beq.n	.L_08167bee
	movs	r3, #0
	strb	r3, [r5, #26]
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_08002064
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_08020030
	ldr	r3, [sp, #88]
	mov	r4, r8
	ldr	r2, [r6, r3]
	ldrb	r3, [r2, #9]
	ands	r3, r4
	strb	r3, [r2, #9]
.L_08167bee:
	adds	r7, #1
	adds	r6, #4
	cmp	r7, #16
	bne.n	.L_08167bba
	movs	r0, #104
	bl	sub_0801314c
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r5, [sp, #88]
	movs	r6, #142
	lsls	r6, r6, #7
	str	r3, [sp, #76]
	ldr	r0, [pc, #84]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r0, #192
	ldr	r1, [sp, #68]
	lsls	r0, r0, #2
	movs	r7, #0
	movs	r4, #1
	adds	r0, #2
.L_08167c2a:
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	cmp	r2, #32
	bls.n	.L_08167c6c
	adds	r3, #224
	strb	r3, [r1, #0]
	b.n	.L_08167c72
	.4byte 0xfffff720
	.4byte 0xffee2000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02014000
	.4byte 0x08199364
	.4byte 0xfffc0000
	.4byte 0x08199210
	.4byte 0x03001150
	.4byte 0x03000260
	.4byte 0x05000200
	.4byte 0x050001e8
	.2byte 0x0126
	.2byte 0x0000
.L_08167c6c:
	cmp	r2, #0
	beq.n	.L_08167c72
	strb	r4, [r1, #0]
.L_08167c72:
	adds	r7, #1
	adds	r1, #1
	cmp	r7, r0
	bne.n	.L_08167c2a
	ldr	r0, [sp, #88]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r4, #238
	adds	r1, #180
	lsls	r4, r4, #7
	adds	r3, r0, r1
	movs	r2, #0
	adds	r4, #184
	str	r2, [r3, #0]
	adds	r3, r0, r4
	str	r2, [r3, #0]
	ldr	r1, [pc, #76]
	movs	r0, #1
	bl	sub_08118040
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	adds	r2, #8
	str	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #28
	strh	r3, [r2, #0]
	ldr	r5, [pc, #52]
	movs	r1, #128
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x263c
	ldr	r4, [sp, #88]
	movs	r1, #6
	str	r6, [sp, #44]
	str	r1, [sp, #36]
	movs	r6, #239
	movs	r1, #238
	movs	r0, #44
	movs	r3, #2
	lsls	r6, r6, #7
	lsls	r1, r1, #7
	b.n	.L_08167cec
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000784
	.4byte 0x00000075
	.4byte 0xffffc400
	.2byte 0x0260
	.2byte 0x0300
.L_08167cec:
	str	r0, [sp, #40]
	str	r3, [sp, #28]
	movs	r2, #0
	adds	r3, r4, r6
	movs	r0, #2
	adds	r1, #132
	str	r2, [sp, #32]
	str	r2, [sp, #24]
	str	r0, [r3, #0]
	adds	r2, r4, r1
	movs	r3, #75
	str	r3, [r2, #0]
	movs	r2, #224
	lsls	r2, r2, #3
	movs	r1, #225
	adds	r0, r4, r2
	lsls	r1, r1, #6
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [sp, #72]
.L_08167d18:
	ldr	r4, [sp, #72]
	cmp	r4, #66
	bne.n	.L_08167d24
	movs	r0, #208
	bl	sub_081c0010
.L_08167d24:
	ldr	r5, [sp, #72]
	cmp	r5, #88
	bne.n	.L_08167d30
	movs	r0, #230
	bl	sub_081c0010
.L_08167d30:
	ldr	r6, [sp, #72]
	cmp	r6, #155
	bne.n	.L_08167d3c
	movs	r0, #162
	bl	sub_081c0010
.L_08167d3c:
	ldr	r0, [sp, #72]
	cmp	r0, #217
	bne.n	.L_08167d48
	movs	r0, #156
	bl	sub_081c0010
.L_08167d48:
	ldr	r1, [sp, #72]
	movs	r2, #140
	lsls	r2, r2, #1
	cmp	r1, r2
	bne.n	.L_08167d58
	movs	r0, #157
	bl	sub_081c0010
.L_08167d58:
	ldr	r3, [sp, #72]
	movs	r4, #150
	lsls	r4, r4, #1
	cmp	r3, r4
	bne.n	.L_08167d98
	ldr	r5, [sp, #96]
	movs	r7, #0
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_08167d92
	movs	r5, #36
.L_08167d6e:
	ldr	r6, [sp, #96]
	movs	r3, #8
	ldrsh	r0, [r5, r6]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	bl	sub_0814cd48
	ldrsh	r0, [r5, r6]
	movs	r1, #1
	bl	sub_08118088
	ldr	r3, [r6, #20]
	adds	r7, #1
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_08167d6e
.L_08167d92:
	movs	r0, #145
	bl	sub_081180e8
.L_08167d98:
	ldr	r3, [pc, #164]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08167e5e
	ldr	r3, [sp, #72]
	subs	r3, #5
	cmp	r3, #144
	bhi.n	.L_08167e32
	ldr	r1, [pc, #148]
	movs	r3, #150
	str	r3, [sp, #72]
	ldr	r4, [pc, #148]
	ldrh	r3, [r4, #0]
	adds	r0, r3, #0
	movs	r5, #130
	ldr	r6, [pc, #140]
	lsls	r5, r5, #2
	strh	r5, [r6, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08167de4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	lsls	r2, r2, #19
	adds	r2, #32
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08167de4:
	ldr	r2, [pc, #96]
	strh	r0, [r2, #0]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	ldr	r4, [pc, #88]
	lsls	r3, r3, #2
	strh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08167e1a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	movs	r5, #0
	stmia	r3!, {r5}
	strh	r2, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08167e1a:
	ldr	r6, [pc, #44]
	strh	r0, [r6, #0]
	ldr	r0, [pc, #44]
	movs	r3, #142
	ldr	r2, [sp, #88]
	lsls	r3, r3, #7
	adds	r1, r2, r3
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_08167e5e
.L_08167e32:
	ldr	r3, [sp, #72]
	subs	r3, #155
	cmp	r3, #58
	bhi.n	.L_08167e50
	movs	r4, #214
	str	r4, [sp, #72]
	b.n	.L_08167e5e
	.4byte 0x03001150
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x0131
	.2byte 0x0000
.L_08167e50:
	ldr	r3, [sp, #72]
	subs	r3, #219
	cmp	r3, #60
	bhi.n	.L_08167e5e
	movs	r5, #140
	lsls	r5, r5, #1
	str	r5, [sp, #72]
.L_08167e5e:
	ldr	r6, [sp, #72]
	movs	r0, #159
	lsls	r0, r0, #1
	cmp	r6, r0
	bne.n	sub_08167e76
	movs	r1, #128
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	ldr	r2, [pc, #268]
	ldr	r3, [pc, #268]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c12
	movs	r5, #64
	adds	r5, #255
	cmp	r4, r5
	bne.n	.L_08167e9c
	bl	sub_0815b410
	ldr	r6, [sp, #88]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r6, r0
	movs	r3, #3
	movs	r1, #238
	str	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #236]
	adds	r1, #132
	adds	r2, r6, r1
	str	r3, [r2, #0]
.L_08167e9c:
	ldr	r2, [sp, #72]
	cmp	r2, #64
	bne.n	.L_08167f42
	ldr	r1, [pc, #228]
	ldr	r4, [pc, #228]
	ldrh	r3, [r4, #0]
	adds	r0, r3, #0
	movs	r5, #130
	ldr	r6, [pc, #220]
	lsls	r5, r5, #2
	strh	r5, [r6, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08167ed6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	lsls	r2, r2, #19
	adds	r2, #32
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08167ed6:
	ldr	r2, [pc, #180]
	strh	r0, [r2, #0]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	ldr	r4, [pc, #168]
	lsls	r3, r3, #2
	strh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08167f0c
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	movs	r5, #0
	stmia	r3!, {r5}
	strh	r2, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08167f0c:
	ldr	r6, [pc, #124]
	strh	r0, [r6, #0]
	ldr	r0, [sp, #92]
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	ldr	r3, [pc, #100]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x481c
	movs	r5, #142
	ldr	r4, [sp, #88]
	lsls	r5, r5, #7
	adds	r1, r4, r5
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #88]
	movs	r2, #206
	lsls	r2, r2, #7
	adds	r1, r6, r2
	ldr	r0, [pc, #88]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_08167f42:
	ldr	r3, [sp, #72]
	cmp	r3, #66
	bne.n	.L_08167f60
	movs	r2, #160
	ldr	r1, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #192
	movs	r7, #0
.L_08167f52:
	ldrh	r3, [r2, #0]
	adds	r7, #1
	eors	r3, r1
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r7, #128
	bne.n	.L_08167f52
.L_08167f60:
	ldr	r4, [sp, #72]
	cmp	r4, #69
	bne.n	.L_08167f98
	movs	r1, #128
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	ldr	r2, [pc, #12]
	ldr	r5, [pc, #16]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xe010
	movs	r0, r0
	.4byte 0x00007fff
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x04040404
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x00000131
	.2byte 0x0127
	.2byte 0x0000
.L_08167f98:
	ldr	r6, [sp, #72]
	cmp	r6, #70
	bne.n	.L_08167fa8
	movs	r0, #1
	ldr	r1, [pc, #268]
	movs	r2, #7
	bl	sub_08118038
.L_08167fa8:
	ldr	r0, [sp, #72]
	cmp	r0, #150
	bne.n	.L_08167fee
	movs	r1, #112
	movs	r2, #32
	movs	r3, #0
	movs	r4, #4
	movs	r5, #8
	str	r1, [sp, #44]
	str	r2, [sp, #40]
	str	r3, [sp, #32]
	str	r4, [sp, #28]
	str	r5, [sp, #36]
	ldr	r6, [sp, #88]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r0, r6, r1
	movs	r1, #225
	lsls	r1, r1, #6
	movs	r2, #0
	ldr	r3, [pc, #224]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9817
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #216]
	ldr	r4, [pc, #212]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	ldr	r1, [pc, #212]
	movs	r2, #0
	bl	sub_08118040
.L_08167fee:
	ldr	r5, [sp, #72]
	cmp	r5, #214
	bne.n	.L_08168066
	ldr	r6, [sp, #88]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r0, r6, r1
	movs	r1, #225
	lsls	r1, r1, #6
	movs	r2, #0
	ldr	r3, [pc, #176]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9817
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #168]
	ldr	r4, [pc, #160]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	ldr	r1, [pc, #164]
	movs	r2, #0
	bl	sub_08118040
	ldr	r5, [sp, #88]
	movs	r7, #0
.L_08168024:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	ldr	r2, [r5, #0]
	adds	r3, #16
	lsls	r3, r3, #15
	str	r3, [r5, #16]
	movs	r3, #128
	asrs	r2, r2, #7
	lsls	r3, r3, #8
	subs	r3, r3, r2
	str	r3, [r5, #8]
	adds	r7, #1
	movs	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_08168024
.L_08168066:
	ldr	r5, [sp, #72]
	movs	r6, #140
	lsls	r6, r6, #1
	cmp	r5, r6
	bne.n	.L_0816811c
	bl	sub_0814cca8
	ldr	r1, [sp, #88]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, r2
	movs	r1, #225
	lsls	r1, r1, #6
	movs	r2, #0
	ldr	r3, [pc, #48]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9817
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #52]
	ldr	r4, [pc, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2500
	ldr	r6, [sp, #84]
	movs	r2, #128
	ldr	r3, [pc, #12]
	lsls	r2, r2, #19
	adds	r2, #82
	str	r5, [r6, #16]
	strh	r3, [r2, #0]
	ldr	r5, [sp, #88]
	movs	r7, #0
	b.n	.L_081680c8
	.4byte 0x00001010
	.4byte 0x00000075
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x00000040
	.4byte 0x00000044
	.2byte 0x0101
	.2byte 0x0101
.L_081680c8:
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	lsls	r3, r3, #15
	movs	r1, #0
	adds	r7, #1
	str	r3, [r5, #16]
	str	r1, [r5, #8]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_081680c8
	ldr	r4, [sp, #88]
	movs	r5, #239
	movs	r6, #238
	movs	r2, #136
	lsls	r5, r5, #7
	lsls	r6, r6, #7
	adds	r3, r4, r5
	lsls	r2, r2, #1
	adds	r6, #132
	str	r2, [sp, #24]
	str	r1, [r3, #0]
	adds	r3, r4, r6
	str	r1, [r3, #0]
.L_0816811c:
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #56]
	adds	r3, r0, r1
	cmp	r3, #38
	bhi.n	.L_08168174
	movs	r5, #160
	ldr	r6, [pc, #40]
	lsls	r5, r5, #19
	adds	r5, #2
	movs	r7, #0
.L_08168130:
	ldrh	r2, [r5, #0]
	movs	r4, #31
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r6
	lsrs	r1, r3, #21
	ands	r1, r6
	ands	r4, r2
	adds	r0, #1
	adds	r1, #1
	adds	r4, #1
	cmp	r0, #31
	ble.n	.L_0816814c
	movs	r0, #31
.L_0816814c:
	cmp	r1, #31
	ble.n	.L_0816815c
	movs	r1, #31
	b.n	.L_0816815c
	.4byte 0x0000001f
	.2byte 0xfee8
	.2byte 0xffff
.L_0816815c:
	cmp	r4, #31
	ble.n	.L_08168162
	movs	r4, #31
.L_08168162:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r7, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r7, #63
	bne.n	.L_08168130
.L_08168174:
	ldr	r2, [sp, #72]
	cmp	r2, #182
	bne.n	sub_08168188
	movs	r1, #128
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	ldr	r2, [pc, #808]
	ldr	r3, [pc, #812]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c12
	cmp	r4, #63
	bgt.n	.L_0816824e
	ldr	r5, [sp, #72]
	ldr	r6, [sp, #88]
	movs	r3, #7
	movs	r0, #142
	subs	r4, #4
	lsls	r0, r0, #7
	ands	r3, r5
	mov	r8, r4
	adds	r7, r6, r0
	cmp	r3, #3
	ble.n	.L_081681ae
	ldr	r1, [sp, #88]
	movs	r2, #146
	lsls	r2, r2, #7
	adds	r2, #64
	adds	r7, r1, r2
.L_081681ae:
	movs	r1, #19
	movs	r0, #188
	bl	sub_081963ec
	movs	r5, #24
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	movs	r3, #192
	mov	r6, r8
	lsls	r3, r3, #18
	subs	r6, #24
	adds	r3, #188
	ldr	r0, [sp, #92]
	ldr	r4, [r3, #0]
	movs	r2, #36
	adds	r3, r6, #0
	adds	r1, r7, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	movs	r0, #192
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r4, [r0, #0]
	adds	r3, r6, #0
	ldr	r0, [sp, #92]
	movs	r2, #59
	adds	r1, r7, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #27
	movs	r0, #188
	bl	sub_081963ec
	movs	r1, #192
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	lsls	r1, r1, #18
	adds	r6, #23
	adds	r1, #188
	ldr	r4, [r1, #0]
	ldr	r0, [sp, #92]
	movs	r2, #36
	adds	r3, r6, #0
	adds	r1, r7, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	movs	r2, #192
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	lsls	r2, r2, #18
	adds	r2, #188
	ldr	r4, [r2, #0]
	ldr	r0, [sp, #92]
	adds	r1, r7, #0
	movs	r2, #59
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_0816824e:
	ldr	r3, [sp, #72]
	subs	r3, #64
	cmp	r3, #1
	bhi.n	sub_08168272
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r3, #17
	str	r3, [sp, #4]
	ldr	r3, [sp, #88]
	movs	r4, #206
	lsls	r4, r4, #7
	adds	r1, r3, r4
	ldr	r0, [sp, #92]
	movs	r2, #52
	movs	r3, #51
	ldr	r5, [sp, #76]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #66
	cmp	r3, #1
	bhi.n	sub_08168298
	ldr	r6, [sp, #88]
	movs	r2, #208
	movs	r3, #24
	lsls	r2, r2, #7
	adds	r2, #16
	str	r3, [sp, #0]
	movs	r3, #41
	str	r3, [sp, #4]
	adds	r1, r6, r2
	ldr	r0, [sp, #92]
	movs	r2, #48
	movs	r3, #40
	ldr	r4, [sp, #76]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #68
	cmp	r3, #7
	bhi.n	sub_08168320
	ldr	r5, [sp, #72]
	movs	r6, #76
	subs	r6, r6, r5
	ldr	r0, [sp, #88]
	lsrs	r5, r6, #31
	movs	r2, #38
	movs	r1, #214
	mov	sl, r2
	adds	r5, r6, r5
	ldr	r2, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #232
	asrs	r5, r5, #1
	movs	r4, #49
	adds	r7, r0, r1
	subs	r4, r4, r5
	movs	r1, #44
	mov	r3, sl
	movs	r0, #22
	subs	r3, r3, r6
	str	r0, [sp, #0]
	mov	r8, r1
	str	r1, [sp, #4]
	mov	fp, r2
	str	r4, [sp, #8]
	ldr	r0, [sp, #92]
	adds	r2, r4, #0
	adds	r1, r7, #0
	mov	sl, r3
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9c02
	adds	r6, #38
	movs	r3, #22
	mov	r0, r8
	adds	r2, r4, #0
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r7, #0
	ldr	r0, [sp, #92]
	adds	r3, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x3531
	movs	r1, #22
	mov	r2, r8
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	adds	r1, r7, #0
	adds	r2, r5, #0
	mov	r3, sl
	ldr	r0, [sp, #92]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x2316
	mov	r4, r8
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #92]
	adds	r1, r7, #0
	adds	r2, r5, #0
	adds	r3, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #78
	cmp	r3, #1
	bhi.n	sub_08168344
	ldr	r5, [sp, #88]
	movs	r3, #16
	movs	r6, #206
	str	r3, [sp, #0]
	lsls	r6, r6, #7
	movs	r3, #17
	str	r3, [sp, #4]
	ldr	r0, [sp, #92]
	adds	r1, r5, r6
	movs	r2, #52
	movs	r3, #51
	ldr	r4, [sp, #76]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #80
	cmp	r3, #1
	bhi.n	sub_0816836a
	ldr	r5, [sp, #88]
	movs	r6, #208
	movs	r3, #24
	lsls	r6, r6, #7
	str	r3, [sp, #0]
	adds	r6, #16
	movs	r3, #41
	str	r3, [sp, #4]
	ldr	r0, [sp, #92]
	adds	r1, r5, r6
	movs	r2, #48
	movs	r3, #40
	ldr	r4, [sp, #76]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #82
	cmp	r3, #3
	bhi.n	sub_081683fa
	ldr	r5, [sp, #72]
	ldr	r6, [sp, #88]
	movs	r0, #214
	lsls	r7, r5, #1
	lsls	r0, r0, #7
	adds	r2, r7, #0
	adds	r0, #232
	subs	r2, #164
	adds	r6, r6, r0
	mov	r9, r6
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08168390
	adds	r3, r7, #0
	subs	r3, #161
.L_08168390:
	movs	r4, #38
	subs	r4, r4, r2
	ldr	r2, [sp, #76]
	asrs	r3, r3, #2
	movs	r5, #49
	subs	r5, r5, r3
	movs	r6, #44
	movs	r1, #22
	str	r3, [sp, #20]
	mov	r8, r1
	str	r1, [sp, #0]
	mov	sl, r2
	adds	r3, r4, #0
	str	r4, [sp, #8]
	str	r6, [sp, #4]
	ldr	r0, [sp, #92]
	adds	r2, r5, #0
	mov	r1, r9
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x3f7e
	mov	fp, r7
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r2, r5, #0
	str	r6, [sp, #4]
	ldr	r0, [sp, #92]
	mov	r1, r9
	mov	r3, fp
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9d05
	ldr	r4, [sp, #8]
	adds	r5, #49
	mov	r0, r8
	str	r0, [sp, #0]
	mov	r1, r9
	adds	r2, r5, #0
	adds	r3, r4, #0
	str	r5, [sp, #16]
	str	r6, [sp, #4]
	ldr	r0, [sp, #92]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4641
	str	r1, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #92]
	mov	r1, r9
	ldr	r2, [sp, #16]
	mov	r3, fp
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #72
	cmp	r3, #15
	bhi.n	.L_08168490
	movs	r0, #32
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [sp, #72]
	ldr	r4, [pc, #160]
	lsls	r3, r2, #13
	adds	r7, r3, r4
	ldr	r2, [pc, #156]
	ldr	r3, [sp, #108]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #152]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #108]
	ldr	r3, [pc, #144]
	add	r2, sp, #108
	str	r3, [r2, #4]
	ldr	r3, [pc, #140]
	adds	r5, r0, #0
	str	r1, [r5, #0]
	str	r2, [r5, #16]
	str	r3, [r5, #8]
	str	r6, [r5, #12]
	ldr	r1, [sp, #72]
	movs	r0, #0
	cmp	r1, #79
	ble.n	.L_08168452
	lsls	r3, r1, #3
	movs	r2, #160
	negs	r3, r3
	lsls	r2, r2, #2
	adds	r0, r3, r2
.L_08168452:
	str	r0, [r5, #20]
	bl	sub_08014de4
	movs	r1, #128
	lsls	r1, r1, #12
	movs	r2, #0
	ldr	r0, [pc, #104]
	bl	sub_08015160
	adds	r0, r7, #0
	bl	sub_0801521c
	movs	r0, #152
	lsls	r0, r0, #5
	adds	r0, #136
	bl	sub_08015024
	adds	r1, r6, #0
	movs	r2, #4
	ldr	r0, [pc, #80]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r6, #0
	bl	sub_08013164
.L_08168490:
	ldr	r3, [sp, #72]
	cmp	r3, #85
	bgt.n	.L_08168498
	b.n	.L_081686a0
.L_08168498:
	cmp	r3, #213
	ble.n	.L_0816849e
	b.n	.L_081686a0
.L_0816849e:
	ldr	r4, [sp, #28]
	movs	r7, #0
	cmp	r4, #0
	bne.n	.L_081684a8
	b.n	.L_081686a0
.L_081684a8:
	b.n	.L_081684d0
	movs	r0, r0
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0xfff70800
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02014000
	.4byte 0x08199364
	.4byte 0xfffc0000
	.2byte 0x9210
	.2byte 0x0819
.L_081684d0:
	ldr	r5, [sp, #32]
	movs	r6, #0
	mov	lr, r5
	mov	sl, r6
	mov	r8, r5
	cmp	r5, #0
	bge.n	.L_081684e0
	b.n	.L_08168690
.L_081684e0:
	movs	r0, #224
	lsls	r0, r0, #3
	mov	r9, r0
.L_081684e6:
	mov	r1, lr
	lsrs	r3, r1, #31
	add	r3, lr
	asrs	r3, r3, #1
	mov	fp, r3
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081684f8
	adds	r3, #7
.L_081684f8:
	ldr	r4, [sp, #44]
	ldr	r5, [sp, #44]
	ldr	r6, [sp, #36]
	asrs	r3, r3, #3
	adds	r0, r4, r3
	subs	r4, r5, r3
	mov	r3, sl
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_0816850e
	adds	r3, #7
.L_0816850e:
	ldr	r1, [sp, #40]
	asrs	r3, r3, #3
	mov	r5, sl
	adds	r2, r1, r3
	lsls	r3, r5, #3
	cmp	r3, #0
	bge.n	.L_0816851e
	adds	r3, #7
.L_0816851e:
	ldr	r6, [sp, #40]
	asrs	r3, r3, #3
	subs	r1, r6, r3
	cmp	r1, #0
	bge.n	.L_0816852a
	movs	r1, #0
.L_0816852a:
	cmp	r2, #119
	ble.n	.L_08168530
	movs	r2, #119
.L_08168530:
	cmp	r4, #0
	bge.n	.L_08168536
	movs	r4, #0
.L_08168536:
	cmp	r0, #119
	ble.n	.L_0816853c
	movs	r0, #119
.L_0816853c:
	lsls	r3, r2, #4
	subs	r3, r3, r2
	ldr	r6, [sp, #88]
	lsls	r5, r3, #3
	adds	r3, r5, r0
	movs	r2, #20
	add	r3, r9
	strb	r2, [r6, r3]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r1, r3, #3
	adds	r3, r1, r0
	add	r3, r9
	strb	r2, [r6, r3]
	adds	r3, r5, r4
	add	r3, r9
	strb	r2, [r6, r3]
	adds	r3, r1, r4
	add	r3, r9
	strb	r2, [r6, r3]
	ldr	r3, [sp, #44]
	ldr	r2, [sp, #44]
	add	r3, fp
	mov	r4, fp
	adds	r0, r3, #1
	subs	r3, r2, r4
	adds	r4, r3, #1
	cmp	r4, #0
	bge.n	.L_08168578
	movs	r4, #0
.L_08168578:
	cmp	r0, #119
	ble.n	.L_0816857e
	movs	r0, #119
.L_0816857e:
	movs	r6, #224
	adds	r3, r5, r0
	lsls	r6, r6, #3
	adds	r3, r3, r6
	ldr	r6, [sp, #88]
	movs	r2, #20
	strb	r2, [r6, r3]
	adds	r3, r1, r0
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r3, r0
	strb	r2, [r6, r3]
	adds	r3, r5, r4
	adds	r3, r3, r0
	strb	r2, [r6, r3]
	adds	r3, r1, r4
	adds	r3, r3, r0
	mov	r1, sl
	strb	r2, [r6, r3]
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	ip, r3
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081685b4
	adds	r3, #7
.L_081685b4:
	ldr	r4, [sp, #44]
	ldr	r5, [sp, #44]
	ldr	r6, [sp, #36]
	asrs	r3, r3, #3
	adds	r0, r4, r3
	subs	r4, r5, r3
	mov	r3, lr
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_081685ca
	adds	r3, #7
.L_081685ca:
	ldr	r1, [sp, #40]
	asrs	r3, r3, #3
	mov	r5, lr
	adds	r2, r1, r3
	lsls	r3, r5, #3
	cmp	r3, #0
	bge.n	.L_081685da
	adds	r3, #7
.L_081685da:
	ldr	r6, [sp, #40]
	asrs	r3, r3, #3
	subs	r1, r6, r3
	cmp	r4, #0
	bge.n	.L_081685e6
	movs	r4, #0
.L_081685e6:
	cmp	r0, #119
	ble.n	.L_081685ec
	movs	r0, #119
.L_081685ec:
	cmp	r1, #0
	bge.n	.L_081685f2
	movs	r1, #0
.L_081685f2:
	cmp	r2, #119
	ble.n	.L_081685f8
	movs	r2, #119
.L_081685f8:
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r6, r3, #3
	movs	r2, #224
	adds	r3, r6, r0
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #88]
	movs	r5, #20
	strb	r5, [r2, r3]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #3
	ldr	r1, [sp, #88]
	adds	r3, r2, r0
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r3, r0
	strb	r5, [r1, r3]
	adds	r3, r6, r4
	adds	r3, r3, r0
	strb	r5, [r1, r3]
	adds	r3, r2, r4
	adds	r3, r3, r0
	strb	r5, [r1, r3]
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #44]
	add	r3, ip
	mov	r1, ip
	adds	r0, r3, #1
	subs	r3, r4, r1
	adds	r4, r3, #1
	cmp	r4, #0
	bge.n	.L_0816863e
	movs	r4, #0
.L_0816863e:
	cmp	r0, #119
	ble.n	.L_08168644
	movs	r0, #119
.L_08168644:
	movs	r1, #224
	adds	r3, r6, r0
	lsls	r1, r1, #3
	adds	r3, r3, r1
	ldr	r1, [sp, #88]
	strb	r5, [r1, r3]
	adds	r3, r2, r0
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r3, r0
	strb	r5, [r1, r3]
	adds	r3, r6, r4
	adds	r3, r3, r0
	strb	r5, [r1, r3]
	adds	r3, r2, r4
	adds	r3, r3, r0
	strb	r5, [r1, r3]
	mov	r1, sl
	mov	r2, r8
	lsls	r3, r1, #1
	subs	r3, r2, r3
	subs	r3, #1
	mov	r8, r3
	cmp	r3, #0
	bge.n	.L_08168686
	mov	r4, lr
	lsls	r3, r4, #1
	add	r3, r8
	movs	r5, #1
	subs	r3, #2
	negs	r5, r5
	mov	r8, r3
	add	lr, r5
.L_08168686:
	movs	r6, #1
	add	sl, r6
	cmp	lr, sl
	blt.n	.L_08168690
	b.n	.L_081684e6
.L_08168690:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	adds	r0, #1
	adds	r7, #1
	str	r0, [sp, #32]
	cmp	r7, r1
	beq.n	.L_081686a0
	b.n	.L_081684d0
.L_081686a0:
	ldr	r2, [sp, #72]
	subs	r2, #86
	mov	r8, r2
	cmp	r2, #63
	bhi.n	.L_08168720
	ldr	r3, [sp, #72]
	ldr	r4, [sp, #72]
	subs	r3, #70
	mov	r9, r3
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	ldr	r6, [pc, #880]
	asrs	r3, r3, #1
	movs	r7, #0
	mov	sl, r3
.L_081686be:
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	adds	r0, r6, #0
	asrs	r5, r3, #16
	bl	sub_08002090
	mov	r3, r9
	muls	r3, r0
	mov	r1, sl
	adds	r2, r1, r7
	asrs	r0, r3, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_081686e2
	adds	r3, r2, #3
.L_081686e2:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	ldr	r2, [sp, #88]
	lsls	r1, r1, #2
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #142
	adds	r1, r2, r1
	lsls	r3, r3, #7
	adds	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #32
	adds	r2, r5, #0
	str	r0, [sp, #0]
	movs	r5, #128
	movs	r0, #54
	str	r0, [sp, #4]
	adds	r2, #44
	adds	r3, #17
	ldr	r0, [sp, #92]
	ldr	r4, [sp, #76]
	lsls	r5, r5, #5
	adds	r7, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1976
	cmp	r7, #9
	bne.n	.L_081686be
.L_08168720:
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
	movs	r6, #192
	lsls	r6, r6, #18
	adds	r6, #188
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #72]
	str	r4, [sp, #80]
	cmp	r0, #85
	ble.n	sub_08168750
	movs	r3, #120
	ldr	r2, [sp, #88]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [sp, #92]
	movs	r2, #0
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9c12
	movs	r5, #24
	adds	r5, #255
	cmp	r4, r5
	ble.n	.L_08168760
	ldr	r6, [sp, #24]
	subs	r6, #8
	str	r6, [sp, #24]
.L_08168760:
	ldr	r0, [sp, #72]
	cmp	r0, #238
	bne.n	sub_08168774
	movs	r1, #128
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	ldr	r2, [pc, #700]
	ldr	r3, [pc, #704]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b12
	subs	r3, #214
	cmp	r3, #65
	bhi.n	.L_08168784
	ldr	r2, [pc, #692]
	ldrh	r3, [r2, #4]
	adds	r3, #8
	strh	r3, [r2, #4]
.L_08168784:
	ldr	r3, [sp, #72]
	subs	r3, #246
	cmp	r3, #33
	bhi.n	.L_081687aa
	ldr	r4, [sp, #24]
	ldr	r5, [sp, #88]
	movs	r6, #239
	movs	r1, #238
	lsls	r6, r6, #7
	lsls	r1, r1, #7
	adds	r3, r5, r6
	adds	r4, #8
	movs	r0, #0
	adds	r1, #132
	str	r4, [sp, #24]
	adds	r2, r5, r1
	str	r0, [r3, #0]
	movs	r3, #75
	str	r3, [r2, #0]
.L_081687aa:
	ldr	r2, [sp, #72]
	cmp	r2, #213
	bgt.n	.L_081687b2
	b.n	.L_081689ae
.L_081687b2:
	add	r3, sp, #132
	movs	r4, #0
	str	r4, [r3, #12]
	str	r4, [r3, #4]
	ldr	r0, [sp, #88]
	movs	r1, #238
	movs	r5, #100
	lsls	r1, r1, #7
	add	r5, sp
	adds	r1, #220
	mov	r8, r5
	movs	r7, #0
	adds	r4, r3, #0
	adds	r6, r0, r1
	adds	r5, r0, #0
.L_081687d0:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_0816887e
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #255
	str	r3, [sp, #100]
	cmp	r3, r2
	bgt.n	.L_081687f0
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [sp, #100]
.L_081687f0:
	mov	r0, r8
	str	r3, [r0, #4]
	movs	r2, #255
	ldr	r3, [r5, #0]
	lsls	r2, r2, #16
	str	r2, [r4, #4]
	str	r3, [r4, #0]
	adds	r1, r4, #0
	ldr	r3, [r5, #4]
	ldr	r0, [r6, #0]
	adds	r3, r3, r2
	str	r3, [r4, #8]
	mov	r2, r8
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	ldr	r1, [pc, #544]
	subs	r2, r2, r3
	str	r2, [r5, #4]
	ldr	r4, [sp, #8]
	cmp	r2, r1
	bgt.n	.L_0816887e
	ldr	r2, [sp, #72]
	movs	r3, #24
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_08168842
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #8]
	adds	r3, r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	b.n	.L_08168858
.L_08168842:
	str	r4, [sp, #8]
	bl	sub_08014878
	ldr	r1, [sp, #24]
	movs	r3, #63
	ands	r3, r0
	adds	r3, r3, r1
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r4, [sp, #8]
.L_08168858:
	ldr	r2, [sp, #72]
	movs	r3, #46
	adds	r3, #255
	cmp	r2, r3
	ble.n	.L_08168868
	movs	r0, #0
	str	r0, [r5, #24]
	b.n	.L_0816887e
.L_08168868:
	ldr	r1, [sp, #72]
	cmp	r1, #245
	ble.n	.L_08168874
	movs	r3, #192
	lsls	r3, r3, #15
	b.n	.L_0816887c
.L_08168874:
	ldr	r2, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #16
	subs	r3, r3, r2
.L_0816887c:
	str	r3, [r5, #4]
.L_0816887e:
	adds	r7, #1
	adds	r6, #4
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_081687d0
	ldr	r2, [sp, #72]
	movs	r3, #24
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_08168920
	ldr	r5, [sp, #24]
	asrs	r2, r2, #31
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	movs	r4, #15
	asrs	r3, r3, #1
	movs	r7, #0
	mov	r8, r2
	mov	sl, r4
	mov	r9, r3
.L_081688a6:
	bl	sub_08014878
	movs	r2, #1
	ands	r2, r7
	lsls	r3, r2, #2
	lsrs	r5, r7, #31
	adds	r3, r3, r2
	mov	r6, sl
	adds	r5, r7, r5
	ands	r0, r6
	asrs	r5, r5, #1
	lsls	r3, r3, #2
	lsls	r2, r5, #2
	subs	r3, r3, r0
	adds	r3, r3, r2
	add	r3, r9
	adds	r6, r3, #0
	bl	sub_08014878
	ldr	r4, [sp, #72]
	mov	r2, r8
	lsrs	r3, r2, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	mov	r1, sl
	lsls	r5, r5, #5
	ands	r0, r1
	adds	r2, r3, r7
	subs	r6, #16
	subs	r5, r5, r0
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_081688ea
	adds	r3, r2, #3
.L_081688ea:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	ldr	r0, [sp, #88]
	lsls	r1, r1, #2
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #32
	movs	r2, #142
	adds	r1, r0, r1
	lsls	r2, r2, #7
	str	r3, [sp, #0]
	movs	r3, #54
	adds	r1, r1, r2
	str	r3, [sp, #4]
	ldr	r0, [sp, #92]
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #80]
	adds	r7, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f08
	bne.n	.L_081688a6
	b.n	.L_081689ae
.L_08168920:
	ldr	r0, [sp, #24]
	ldr	r5, [sp, #72]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r5, r5, #31
	movs	r6, #15
	asrs	r3, r3, #1
	movs	r7, #0
	mov	r8, r5
	mov	sl, r6
	mov	r9, r3
.L_08168936:
	bl	sub_08014878
	movs	r2, #1
	ands	r2, r7
	lsls	r3, r2, #2
	lsrs	r5, r7, #31
	adds	r3, r3, r2
	mov	r1, sl
	adds	r5, r7, r5
	ands	r0, r1
	asrs	r5, r5, #1
	lsls	r3, r3, #2
	lsls	r2, r5, #2
	adds	r3, r3, r0
	subs	r3, r3, r2
	add	r3, r9
	adds	r6, r3, #0
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	lsls	r5, r5, #5
	subs	r5, r5, r0
	ldr	r0, [sp, #72]
	mov	r4, r8
	lsrs	r3, r4, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r2, r3, r7
	subs	r6, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0816897a
	adds	r3, r2, #3
.L_0816897a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	ldr	r2, [sp, #88]
	lsls	r1, r1, #2
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #142
	adds	r1, r2, r1
	lsls	r3, r3, #7
	adds	r1, r1, r3
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #54
	str	r3, [sp, #4]
	ldr	r0, [sp, #92]
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #80]
	adds	r7, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f08
	bne.n	.L_08168936
.L_081689ae:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r5, [sp, #72]
	cmp	r5, #63
	ble.n	.L_081689c8
	bl	sub_08014878
	movs	r3, #3
	ldr	r2, [pc, #112]
	ands	r3, r0
	adds	r3, #30
	strh	r3, [r2, #6]
.L_081689c8:
	bl	sub_081434f8
	movs	r0, #240
	ldr	r6, [sp, #88]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r6, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #72]
	movs	r2, #168
	adds	r1, #1
	lsls	r2, r2, #1
	str	r1, [sp, #72]
	cmp	r1, r2
	beq.n	.L_081689f2
	bl	.L_08167d18
.L_081689f2:
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	movs	r7, #0
	adds	r5, r6, r3
.L_081689fc:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08020048
	cmp	r7, #16
	bne.n	.L_081689fc
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #188
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffc000
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x03001120
	.4byte 0x000fffff
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #180
	str	r0, [sp, #84]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	str	r0, [sp, #80]
	movs	r0, #0
	ldr	r1, [r5, #92]
	str	r1, [sp, #76]
	ldr	r2, [r5, #100]
	adds	r5, #176
	str	r2, [sp, #68]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r6, r3, r4
	movs	r1, #200
	movs	r3, #0
	str	r3, [r6, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	movs	r0, #1
	movs	r1, #0
	bl	sub_08163c2c
	movs	r1, #161
	b.n	.L_08168ab8
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x00000018
	.2byte 0x3001
	.2byte 0x0814
.L_08168ab8:
	lsls	r1, r1, #2
	movs	r0, #9
	movs	r2, #1
	bl	sub_08152404
	ldr	r2, [pc, #88]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r7, [sp, #76]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #240
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #0
	ldr	r1, [pc, #44]
	movs	r0, #1
	bl	sub_08118040
	movs	r0, #1
	movs	r1, #1
	bl	sub_08163c2c
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #68]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #224
	lsls	r2, r2, #3
	b.n	.L_08168b28
	movs	r0, r0
	.4byte 0x00002737
	.4byte 0x000000ca
	.4byte 0x030011e0
	.4byte 0x00000044
	.2byte 0x0134
	.2byte 0x0000
.L_08168b28:
	adds	r1, r7, r2
	ldr	r0, [pc, #80]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #64]
	str	r3, [sp, #60]
	ldr	r3, [pc, #40]
	movs	r4, #0
	ldrh	r3, [r3, #4]
	movs	r2, #1
	str	r3, [sp, #56]
	movs	r0, #238
	ldr	r5, [r5, #0]
	str	r4, [sp, #48]
	str	r5, [sp, #52]
	str	r2, [r6, #0]
	b.n	.L_08168b84
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x0000100e
	.4byte 0x00003f44
	.4byte 0x00000158
	.2byte 0x1120
	.2byte 0x0300
.L_08168b84:
	ldr	r1, [sp, #64]
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r3, r7, r0
	str	r1, [r3, #0]
	ldr	r3, [sp, #52]
	mov	r8, r4
	str	r2, [r3, #16]
	ldr	r5, [sp, #76]
	movs	r6, #31
.L_08168b98:
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #48
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r1, #48
	bl	sub_0800206c
	movs	r4, #1
	add	r8, r4
	adds	r0, #2
	mov	r7, r8
	str	r0, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_08168b98
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #72]
	ldr	r3, [pc, #12]
	lsls	r2, r2, #19
	movs	r0, #0
	adds	r2, #12
	mov	fp, r0
	strh	r3, [r2, #0]
	b.n	.L_08168f3a
	.2byte 0x0786
	.2byte 0x0000
.L_08168bf8:
	mov	r3, fp
	subs	r3, #24
	cmp	r3, #31
	bhi.n	.L_08168c06
	ldr	r1, [sp, #48]
	adds	r1, #1
	str	r1, [sp, #48]
.L_08168c06:
	ldr	r2, [sp, #48]
	cmp	r2, #24
	ble.n	.L_08168c10
	movs	r3, #24
	str	r3, [sp, #48]
.L_08168c10:
	mov	r4, fp
	cmp	r4, #135
	bgt.n	.L_08168c28
	ldr	r7, [pc, #488]
	ldr	r0, [sp, #48]
	ldrh	r3, [r7, #4]
	adds	r1, r7, #0
	subs	r3, r3, r0
	strh	r3, [r1, #4]
	ldr	r2, [sp, #60]
	adds	r2, r2, r0
	str	r2, [sp, #60]
.L_08168c28:
	mov	r3, fp
	cmp	r3, #149
	bgt.n	.L_08168d08
	ldr	r3, [pc, #468]
	mov	r7, fp
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #96]
	str	r4, [sp, #100]
	movs	r4, #0
	mov	sl, r4
	cmp	r7, #103
	ble.n	.L_08168c4a
	ldr	r0, [pc, #452]
	lsls	r3, r7, #4
	adds	r0, r0, r3
	mov	sl, r0
.L_08168c4a:
	mov	r3, fp
	subs	r3, #8
	cmp	r3, #23
	bhi.n	.L_08168c5c
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #48]
	adds	r3, r1, r2
	subs	r3, #8
	str	r3, [sp, #64]
.L_08168c5c:
	mov	r3, fp
	cmp	r3, #7
	ble.n	.L_08168caa
	movs	r5, #96
	cmp	r3, #104
	bgt.n	.L_08168c6a
	movs	r5, #32
.L_08168c6a:
	ldr	r7, [pc, #416]
	mov	r4, fp
	lsls	r3, r4, #10
	adds	r0, r3, r7
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	movs	r1, #128
	ands	r0, r3
	lsls	r1, r1, #8
	cmp	r0, r1
	ble.n	.L_08168c86
	ldr	r2, [pc, #396]
	adds	r0, r0, r2
.L_08168c86:
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [sp, #44]
	mov	r4, fp
	movs	r3, #31
	ands	r3, r4
	cmp	r3, #8
	bne.n	.L_08168caa
	ldr	r7, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r7, r0
	movs	r3, #4
	str	r3, [r2, #0]
.L_08168caa:
	add	r3, sp, #164
	movs	r2, #0
	str	r2, [r3, #12]
	movs	r2, #255
	lsls	r2, r2, #16
	str	r2, [r3, #4]
	adds	r6, r3, #0
	ldr	r2, [sp, #76]
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r1, #0
	adds	r3, #220
	mov	r8, r1
	add	r7, sp, #96
	adds	r5, r2, r3
.L_08168cc8:
	ldr	r3, [pc, #328]
	mov	r4, r8
	ldr	r0, [sp, #64]
	ldrb	r3, [r3, r4]
	mov	r1, sl
	adds	r3, r0, r3
	subs	r3, r3, r1
	movs	r2, #224
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #308]
	movs	r0, #144
	ldrb	r3, [r3, r4]
	ldr	r4, [sp, #44]
	lsls	r0, r0, #15
	subs	r3, r3, r4
	lsls	r3, r3, #16
	adds	r3, r3, r0
	str	r3, [r6, #8]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldmia	r5!, {r0}
	movs	r3, #0
	bl	sub_08020010
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L_08168cc8
.L_08168d08:
	mov	r3, fp
	cmp	r3, #26
	bgt.n	.L_08168da0
	ldr	r7, [sp, #60]
	lsls	r3, r3, #3
	adds	r7, #4
	mov	sl, r3
	cmp	r7, #10
	ble.n	.L_08168d1c
	movs	r7, #10
.L_08168d1c:
	mov	r4, sl
	cmp	r4, #64
	ble.n	.L_08168d26
	movs	r0, #64
	mov	sl, r0
.L_08168d26:
	movs	r1, #0
	mov	r2, sl
	mov	r8, r1
	cmp	r2, #0
	beq.n	.L_08168da0
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #60]
	lsls	r3, r3, #1
	str	r3, [sp, #40]
	adds	r3, r3, r4
	lsls	r3, r3, #2
	adds	r3, #48
	str	r3, [sp, #36]
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	str	r3, [sp, #32]
	lsls	r0, r7, #1
	mov	r9, r0
.L_08168d4c:
	mov	r1, r8
	lsls	r6, r1, #10
	adds	r0, r6, #0
	bl	sub_08002096
	ldr	r3, [sp, #40]
	ldr	r2, [sp, #60]
	adds	r3, #8
	adds	r5, r3, #0
	muls	r5, r0
	adds	r0, r6, #0
	asrs	r5, r5, #16
	adds	r5, r5, r2
	bl	sub_08002090
	ldr	r4, [sp, #36]
	mov	r2, r9
	adds	r3, r4, #0
	muls	r3, r0
	ldr	r0, [pc, #168]
	ldr	r4, [sp, #32]
	subs	r2, #2
	ldrh	r1, [r0, r2]
	ldr	r2, [sp, #68]
	adds	r5, #96
	asrs	r3, r3, #16
	subs	r5, r5, r4
	mov	r0, r9
	adds	r3, #64
	adds	r1, r2, r1
	str	r0, [sp, #4]
	subs	r3, r3, r7
	str	r7, [sp, #0]
	ldr	r0, [sp, #80]
	adds	r2, r5, #0
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	cmp	r8, sl
	bne.n	.L_08168d4c
.L_08168da0:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L_08168dc0
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r7, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r7, r0
	movs	r3, #50
	str	r3, [r2, #0]
.L_08168dc0:
	mov	r1, fp
	cmp	r1, #28
	bne.n	.L_08168dd0
	movs	r2, #128
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_08168dd0:
	mov	r2, fp
	cmp	r2, #17
	ble.n	.L_08168e9a
	ldr	r5, [sp, #76]
	movs	r3, #0
	mov	r8, r3
	movs	r6, #31
.L_08168dde:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_08168e54
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	ldr	r2, [pc, #44]
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r7, [sp, #68]
	ldr	r2, [r5, #0]
	b.n	.L_08168e20
	.4byte 0x00000784
	.4byte 0x03001120
	.4byte 0x08196e7c
	.4byte 0xfffff980
	.4byte 0xffffe000
	.4byte 0xffff8000
	.4byte 0x08198ada
	.4byte 0x08198ae3
	.2byte 0x7410
	.2byte 0x0819
.L_08168e20:
	adds	r1, r7, r1
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #16]
	adds	r3, #2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_08168e4e
	adds	r3, #63
.L_08168e4e:
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	b.n	.L_08168e58
.L_08168e54:
	subs	r3, #1
	str	r3, [r5, #24]
.L_08168e58:
	ldr	r3, [r5, #0]
	cmp	r3, #128
	bgt.n	.L_08168e64
	ldr	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L_08168e8e
.L_08168e64:
	bl	sub_08014878
	ldr	r7, [sp, #64]
	ands	r0, r6
	adds	r0, r0, r7
	adds	r0, #172
	str	r0, [r5, #0]
	bl	sub_08014878
	ldr	r1, [sp, #44]
	ands	r0, r6
	subs	r0, r0, r1
	adds	r0, #56
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #15
	str	r0, [r5, #16]
.L_08168e8e:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #48
	bne.n	.L_08168dde
.L_08168e9a:
	mov	r4, fp
	cmp	r4, #31
	ble.n	.L_08168ef0
	mov	r3, fp
	subs	r3, #32
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
	cmp	r5, #40
	ble.n	.L_08168eb0
	movs	r5, #40
.L_08168eb0:
	movs	r7, #0
	mov	r8, r7
	movs	r6, #0
	movs	r7, #120
.L_08168eb8:
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	lsls	r1, r0, #1
	adds	r1, r1, r0
	ldr	r0, [sp, #76]
	movs	r3, #48
	lsls	r1, r1, #9
	movs	r2, #224
	adds	r1, r0, r1
	lsls	r2, r2, #3
	str	r3, [sp, #0]
	movs	r3, #32
	adds	r1, r1, r2
	str	r3, [sp, #4]
	ldr	r0, [sp, #80]
	adds	r3, r6, #0
	subs	r2, r7, r5
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r6, #18
	cmp	r1, #6
	bne.n	.L_08168eb8
.L_08168ef0:
	ldr	r3, [sp, #76]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L_08168f1c
	subs	r3, #1
	str	r3, [r2, #0]
	bl	sub_08014878
	ldr	r3, [pc, #8]
	ldr	r7, [pc, #12]
	ands	r0, r3
	adds	r0, #28
	strh	r0, [r7, #6]
	b.n	.L_08168f22
	.4byte 0x00000007
	.2byte 0x1120
	.2byte 0x0300
.L_08168f1c:
	ldr	r0, [pc, #168]
	movs	r3, #32
	strh	r3, [r0, #6]
.L_08168f22:
	ldr	r1, [sp, #76]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	add	fp, r4
.L_08168f3a:
	mov	r7, fp
	cmp	r7, #120
	beq.n	.L_08168f94
	movs	r0, #0
	str	r0, [sp, #44]
	cmp	r7, #0
	bne.n	.L_08168f4e
	movs	r0, #136
	bl	sub_081c0010
.L_08168f4e:
	mov	r1, fp
	cmp	r1, #26
	bne.n	.L_08168f5a
	movs	r0, #141
	bl	sub_081c0010
.L_08168f5a:
	mov	r2, fp
	cmp	r2, #40
	bne.n	.L_08168f66
	movs	r0, #154
	bl	sub_081c0010
.L_08168f66:
	mov	r3, fp
	cmp	r3, #72
	bne.n	.L_08168f72
	movs	r0, #154
	bl	sub_081c0010
.L_08168f72:
	mov	r4, fp
	cmp	r4, #104
	bne.n	.L_08168f7e
	movs	r0, #154
	bl	sub_081c0010
.L_08168f7e:
	ldr	r3, [pc, #76]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08168f8c
	b.n	.L_08168bf8
.L_08168f8c:
	mov	r7, fp
	cmp	r7, #16
	bgt.n	.L_08168f94
	b.n	.L_08168bf8
.L_08168f94:
	add	r0, sp, #56
	ldr	r3, [pc, #48]
	ldrh	r0, [r0, #0]
	movs	r2, #0
	strh	r0, [r3, #4]
	ldr	r1, [sp, #52]
	str	r2, [r1, #16]
	bl	sub_0814cca8
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [sp, #76]
	movs	r4, #238
	lsls	r4, r4, #7
	movs	r2, #0
	adds	r4, #220
	mov	r8, r2
	movs	r0, #12
	adds	r1, r3, r4
	b.n	.L_08168fd0
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x03001120
	.2byte 0x1150
	.2byte 0x0300
.L_08168fd0:
	ldmia	r1!, {r2}
	movs	r7, #1
	ldrb	r3, [r2, #9]
	add	r8, r7
	orrs	r3, r0
	strb	r3, [r2, #9]
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L_08168fd0
	mov	r4, sp
	adds	r4, #120
	str	r4, [sp, #24]
	movs	r3, #224
	mov	r2, sp
	str	r3, [sp, #28]
	movs	r1, #0
	adds	r3, r4, #0
	adds	r2, #134
.L_08168ff4:
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r3, r2
	bne.n	.L_08168ff4
	mov	r7, sp
	adds	r7, #136
	str	r7, [sp, #20]
	ldr	r5, [sp, #20]
	movs	r7, #31
	add	r6, sp, #152
.L_08169008:
	bl	sub_08014878
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_08169008
	ldr	r3, [pc, #104]
	movs	r0, #0
	movs	r2, #160
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #1
.L_08169022:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08169022
	ldr	r7, [sp, #76]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	movs	r2, #0
	str	r3, [sp, #16]
	mov	fp, r2
.L_0816905e:
	mov	r4, fp
	cmp	r4, #23
	bgt.n	.L_08169120
	ldr	r3, [pc, #32]
	ldr	r7, [sp, #28]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	subs	r7, #16
	mov	r0, fp
	str	r3, [sp, #88]
	str	r4, [sp, #92]
	str	r7, [sp, #28]
	b.n	.L_0816908c
	.4byte 0x00000784
	.4byte 0x00001010
	.4byte 0x02010018
	.4byte 0xfffffe20
	.2byte 0x6e84
	.2byte 0x0819
.L_0816908c:
	cmp	r0, #8
	bgt.n	.L_081690ac
	movs	r1, #128
	lsls	r3, r0, #11
	lsls	r1, r1, #7
	movs	r2, #128
	adds	r0, r3, r1
	lsls	r2, r2, #8
	cmp	r0, r2
	ble.n	.L_081690a4
	ldr	r4, [pc, #756]
	adds	r0, r3, r4
.L_081690a4:
	bl	sub_08002096
	lsls	r0, r0, #6
	b.n	.L_081690c8
.L_081690ac:
	mov	r7, fp
	movs	r1, #128
	lsls	r3, r7, #11
	lsls	r1, r1, #7
	movs	r2, #128
	adds	r0, r3, r1
	lsls	r2, r2, #8
	cmp	r0, r2
	ble.n	.L_081690c2
	ldr	r4, [pc, #728]
	adds	r0, r3, r4
.L_081690c2:
	bl	sub_08002096
	lsls	r0, r0, #5
.L_081690c8:
	asrs	r4, r0, #16
	add	r3, sp, #104
	movs	r2, #0
	str	r2, [r3, #12]
	movs	r2, #255
	lsls	r2, r2, #16
	str	r2, [r3, #4]
	ldr	r0, [sp, #76]
	movs	r1, #238
	lsls	r1, r1, #7
	movs	r7, #0
	adds	r1, #220
	mov	r8, r7
	adds	r6, r3, #0
	add	r7, sp, #88
	adds	r5, r0, r1
.L_081690e8:
	ldr	r3, [pc, #688]
	mov	r2, r8
	ldr	r0, [sp, #28]
	ldrb	r3, [r3, r2]
	movs	r1, #144
	adds	r3, r0, r3
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #676]
	lsls	r1, r1, #15
	ldrb	r3, [r3, r2]
	ldmia	r5!, {r0}
	subs	r3, r3, r4
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r6, #8]
	adds	r2, r7, #0
	movs	r3, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	ldr	r4, [sp, #8]
	cmp	r3, #9
	bne.n	.L_081690e8
.L_08169120:
	mov	r4, fp
	cmp	r4, #8
	bne.n	.L_08169138
	ldr	r7, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r7, r0
	str	r4, [r3, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_08169138:
	mov	r1, fp
	cmp	r1, #11
	bne.n	.L_08169144
	movs	r0, #145
	bl	sub_081c0010
.L_08169144:
	mov	r2, fp
	cmp	r2, #46
	bne.n	.L_08169150
	movs	r0, #137
	bl	sub_081c0010
.L_08169150:
	ldr	r4, [sp, #84]
	movs	r3, #0
	ldr	r2, [r4, #20]
	mov	r8, r3
	cmp	r2, #0
	beq.n	.L_08169216
	str	r3, [sp, #12]
	movs	r7, #36
	mov	r9, r7
.L_08169162:
	ldr	r0, [sp, #24]
	mov	r1, r8
	ldrb	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_08169200
	ldr	r4, [sp, #84]
	add	r5, sp, #152
	mov	r2, r9
	ldrsh	r0, [r2, r4]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r3, [r5, #0]
	ldr	r7, [sp, #28]
	cmp	r3, r7
	ble.n	.L_081691fc
	ldr	r0, [sp, #24]
	movs	r3, #1
	mov	r1, r8
	strb	r3, [r0, r1]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #532]
	mov	sl, r5
	movs	r6, #0
	movs	r7, #255
	adds	r5, r2, r3
.L_08169196:
	mov	r4, sl
	ldr	r3, [r4, #0]
	adds	r6, #1
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	ldr	r3, [r4, #4]
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #192
	lsls	r3, r0, #11
	ldr	r2, [r5, #12]
	str	r3, [r5, #16]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L_08169196
	ldr	r2, [sp, #84]
	mov	r7, r9
	ldrsh	r0, [r7, r2]
	movs	r1, #1
	bl	sub_08118088
	movs	r0, #134
	bl	sub_081c0010
	ldr	r3, [sp, #84]
	ldr	r2, [r3, #20]
	b.n	.L_08169200
.L_081691fc:
	ldr	r4, [sp, #84]
	ldr	r2, [r4, #20]
.L_08169200:
	ldr	r0, [sp, #12]
	movs	r1, #224
	lsls	r1, r1, #2
	movs	r3, #1
	movs	r7, #2
	adds	r0, r0, r1
	add	r8, r3
	add	r9, r7
	str	r0, [sp, #12]
	cmp	r8, r2
	bne.n	.L_08169162
.L_08169216:
	ldr	r5, [pc, #396]
	movs	r4, #0
	mov	r8, r4
	movs	r7, #3
	movs	r6, #6
.L_08169220:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_0816925c
	ldr	r3, [pc, #384]
	ldr	r0, [sp, #68]
	ldrh	r1, [r3, #4]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r1, r0, r1
	subs	r2, #1
	subs	r3, #3
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0816925c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #192
	bne.n	.L_08169220
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L_08169274
	movs	r0, #136
	bl	sub_081c0010
.L_08169274:
	mov	r3, fp
	cmp	r3, #40
	ble.n	.L_081692dc
	ldr	r4, [sp, #76]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r4, r7
	movs	r3, #0
	adds	r0, #132
	ldr	r6, [sp, #16]
	str	r3, [r2, #0]
	adds	r2, r4, r0
	movs	r3, #75
	str	r3, [r2, #0]
	movs	r1, #0
	movs	r5, #8
	mov	r8, r1
	negs	r5, r5
.L_0816929c:
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	ldr	r2, [sp, #76]
	ldr	r4, [sp, #20]
	lsls	r1, r0, #1
	adds	r1, r1, r0
	mov	r7, r8
	lsls	r1, r1, #9
	adds	r1, r2, r1
	movs	r3, #224
	ldrb	r2, [r4, r7]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	movs	r3, #48
	str	r3, [sp, #0]
	subs	r2, r2, r6
	movs	r3, #32
	movs	r7, #1
	str	r3, [sp, #4]
	adds	r2, #120
	adds	r3, r5, #0
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	add	r8, r7
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	adds	r5, #8
	cmp	r0, #16
	bne.n	.L_0816929c
.L_081692dc:
	mov	r1, fp
	cmp	r1, #64
	ble.n	.L_081692ee
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
.L_081692ee:
	mov	r7, fp
	cmp	r7, #58
	bne.n	.L_08169324
	ldr	r1, [sp, #84]
	movs	r0, #0
	ldr	r3, [r1, #20]
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_08169324
	movs	r5, #36
	movs	r6, #0
.L_08169304:
	ldr	r2, [sp, #84]
	movs	r1, #14
	ldrsh	r0, [r5, r2]
	movs	r3, #1
	negs	r3, r3
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r7, [sp, #84]
	movs	r4, #1
	ldr	r3, [r7, #20]
	add	r8, r4
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08169304
.L_08169324:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #16]
	movs	r3, #1
	add	fp, r3
	adds	r2, #12
	mov	r4, fp
	str	r2, [sp, #16]
	cmp	r4, #96
	beq.n	.L_08169356
	b.n	.L_0816905e
.L_08169356:
	movs	r0, #134
	bl	sub_081180e8
	movs	r1, #238
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	movs	r7, #0
	adds	r1, #220
	mov	r8, r7
	adds	r6, r0, r1
.L_0816936a:
	ldmia	r6!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #9
	bne.n	.L_0816936a
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #180
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffc000
	.4byte 0x08198ada
	.4byte 0x08198ae3
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0x08143001
