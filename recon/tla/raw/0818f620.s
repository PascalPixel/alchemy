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
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_08015778, 0x08015778
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118010, 0x08118010
	.set sub_08118038, 0x08118038
	.set sub_08118040, 0x08118040
	.set sub_08118078, 0x08118078
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08118118, 0x08118118
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504c0, 0x081504c0
	.set sub_08152404, 0x08152404
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_08158d68, 0x08158d68
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08164a4c, 0x08164a4c
	.set sub_08164abc, 0x08164abc
	.set sub_08178680, 0x08178680
	.set sub_0817d6c4, 0x0817d6c4
	.set sub_0819043c, 0x0819043c
	.set sub_0819076e, 0x0819076e
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0818f620
	.thumb_func
Func_0818f620:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r1, [sp, #56]
	str	r0, [sp, #60]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	ldr	r2, [sp, #56]
	str	r0, [sp, #52]
	ldr	r1, [r3, #96]
	str	r1, [sp, #48]
	ldr	r3, [r3, #100]
	str	r3, [sp, #32]
	cmp	r2, #1
	bhi.n	.L_0818f652
	movs	r0, #1
	bl	sub_081435e0
	b.n	.L_0818f658
.L_0818f652:
	movs	r0, #0
	bl	sub_081435e0
.L_0818f658:
	ldr	r2, [pc, #44]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r4, #224
	str	r3, [sp, #36]
	ldr	r3, [sp, #52]
	ldr	r6, [sp, #56]
	lsls	r4, r4, #3
	adds	r4, r3, r4
	movs	r5, #78
	str	r4, [sp, #28]
	str	r5, [sp, #24]
	cmp	r6, #0
	beq.n	.L_0818f69a
	b.n	.L_0818f68c
	.2byte 0x1010
	.2byte 0x0000
.L_0818f68c:
	ldr	r0, [sp, #56]
	movs	r7, #58
	str	r7, [sp, #24]
	cmp	r0, #3
	beq.n	.L_0818f69a
	movs	r1, #66
	str	r1, [sp, #24]
.L_0818f69a:
	ldr	r3, [sp, #52]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r5, [sp, #52]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #132
	adds	r2, r5, r6
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #644]
	bl	sub_080145a8
	ldr	r0, [pc, #644]
	ldr	r1, [sp, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #0
	movs	r7, #0
	mov	r8, r0
	movs	r6, #2
	movs	r5, #0
.L_0818f6d4:
	ldr	r1, [pc, #624]
	movs	r2, #167
	lsls	r2, r2, #9
	adds	r0, r7, r1
	adds	r2, #32
	adds	r1, r6, #0
	bl	sub_0815b434
	adds	r3, r5, #3
	muls	r3, r6
	ldr	r2, [sp, #28]
	adds	r6, #2
	strh	r7, [r5, r2]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r7, r7, r3
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #2
	cmp	r4, #32
	bne.n	.L_0818f6d4
	ldr	r5, [sp, #56]
	cmp	r5, #3
	bne.n	.L_0818f71e
	ldr	r0, [pc, #576]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #572]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe009
.L_0818f71e:
	ldr	r0, [pc, #564]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #548]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9e0d
	movs	r7, #240
	lsls	r7, r7, #4
	ldr	r0, [pc, #540]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #24]
	movs	r0, #0
	str	r0, [sp, #44]
	cmp	r1, #0
	bne.n	.L_0818f750
	b.n	.L_0818fda0
.L_0818f750:
	movs	r4, #192
	lsls	r4, r4, #3
	mov	r2, sp
	mov	r3, sp
	adds	r4, #228
	adds	r2, #76
	adds	r3, #64
	adds	r4, r6, r4
	str	r2, [sp, #12]
	str	r3, [sp, #16]
	str	r4, [sp, #20]
.L_0818f766:
	ldr	r5, [sp, #60]
	ldr	r1, [sp, #12]
	ldr	r0, [r5, #8]
	bl	sub_0815e21c
	ldr	r6, [sp, #56]
	cmp	r6, #3
	bne.n	.L_0818f77e
	ldr	r0, [sp, #16]
	bl	sub_0815e22c
	b.n	.L_0818f78a
.L_0818f77e:
	ldr	r1, [sp, #60]
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	ldr	r1, [sp, #16]
	bl	sub_0815e21c
.L_0818f78a:
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_0818f79c
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0818f814
	movs	r0, #140
	bl	sub_081c0010
.L_0818f79c:
	ldr	r4, [sp, #44]
	cmp	r4, #0
	bne.n	.L_0818f814
	ldr	r5, [sp, #56]
	cmp	r5, #0
	bne.n	.L_0818f7ce
	ldr	r5, [sp, #52]
	movs	r6, #0
	mov	r8, r6
	movs	r6, #31
.L_0818f7b0:
	mov	r7, r8
	lsls	r3, r7, #12
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #16
	str	r0, [r5, #24]
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_0818f7b0
	b.n	.L_0818f814
.L_0818f7ce:
	ldr	r2, [sp, #12]
	ldr	r0, [r2, #0]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [sp, #20]
	asrs	r0, r0, #1
	lsls	r0, r0, #16
	str	r0, [r3, #0]
	ldr	r4, [r2, #4]
	subs	r4, #36
	lsls	r4, r4, #16
	str	r4, [r3, #4]
	ldr	r5, [sp, #16]
	ldr	r3, [r2, #0]
	ldr	r1, [r5, #0]
	ldr	r6, [sp, #20]
	subs	r1, r1, r3
	lsls	r5, r1, #10
	str	r5, [r6, #12]
	ldr	r7, [sp, #16]
	ldr	r6, [sp, #12]
	ldr	r2, [r7, #4]
	ldr	r3, [r6, #4]
	ldr	r7, [sp, #20]
	subs	r2, r2, r3
	lsls	r3, r2, #10
	lsls	r1, r1, #13
	lsls	r2, r2, #13
	subs	r1, r1, r5
	subs	r2, r2, r3
	adds	r0, r0, r1
	adds	r4, r4, r2
	str	r3, [r7, #16]
	str	r0, [r7, #0]
	str	r4, [r7, #4]
.L_0818f814:
	ldr	r0, [sp, #56]
	cmp	r0, #0
	bne.n	.L_0818f918
	ldr	r3, [sp, #44]
	subs	r3, #18
	cmp	r3, #36
	bls.n	.L_0818f824
	b.n	.L_0818fa44
.L_0818f824:
	ldr	r1, [sp, #44]
	lsls	r3, r1, #1
	adds	r7, r3, #0
	subs	r7, #32
	cmp	r7, #64
	ble.n	.L_0818f832
	movs	r7, #64
.L_0818f832:
	ldr	r2, [sp, #16]
	movs	r4, #32
	ldr	r6, [r2, #0]
	negs	r4, r4
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	ldr	r3, [r2, #4]
	movs	r1, #19
	mov	sl, r3
	movs	r0, #188
	add	sl, r4
	bl	sub_081963ec
	subs	r5, r7, #2
	ldr	r0, [sp, #28]
	asrs	r5, r5, #1
	lsls	r5, r5, #1
	adds	r5, r5, r0
	movs	r2, #0
	ldrsh	r1, [r5, r2]
	ldr	r3, [pc, #236]
	asrs	r4, r7, #1
	mov	r8, r4
	mov	r2, r8
	str	r2, [sp, #0]
	str	r7, [sp, #4]
	adds	r1, r1, r3
	asrs	r6, r6, #1
	mov	r0, sl
	movs	r3, #192
	subs	r4, r6, r4
	subs	r0, r0, r7
	lsls	r3, r3, #18
	mov	fp, r4
	mov	r9, r0
	adds	r3, #188
	ldr	r0, [sp, #48]
	ldr	r4, [r3, #0]
	mov	r2, fp
	mov	r3, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	mov	r2, r8
	movs	r4, #0
	ldrsh	r1, [r5, r4]
	ldr	r0, [pc, #172]
	str	r2, [sp, #0]
	str	r7, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	adds	r1, r1, r0
	adds	r2, r6, #0
	ldr	r0, [sp, #48]
	mov	r3, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #27
	movs	r0, #188
	bl	sub_081963ec
	mov	r2, r8
	movs	r4, #0
	ldrsh	r1, [r5, r4]
	ldr	r0, [pc, #124]
	str	r2, [sp, #0]
	str	r7, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	adds	r1, r1, r0
	mov	r2, fp
	ldr	r0, [sp, #48]
	mov	r3, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	mov	r0, r8
	movs	r4, #0
	ldrsh	r1, [r5, r4]
	movs	r2, #192
	str	r0, [sp, #0]
	str	r7, [sp, #4]
	ldr	r5, [pc, #72]
	lsls	r2, r2, #18
	adds	r2, #188
	ldr	r4, [r2, #0]
	adds	r1, r1, r5
	ldr	r0, [sp, #48]
	adds	r2, r6, #0
	mov	r3, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	b.n	.L_0818fa44
.L_0818f918:
	ldr	r3, [sp, #44]
	subs	r3, #2
	cmp	r3, #30
	bls.n	.L_0818f922
	b.n	.L_0818fa44
.L_0818f922:
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bge.n	.L_0818f92a
	adds	r3, #3
.L_0818f92a:
	asrs	r3, r3, #2
	lsls	r3, r3, #1
	adds	r7, r3, #0
	ldr	r3, [sp, #56]
	adds	r7, #16
	cmp	r3, #1
	bne.n	.L_0818f95c
	cmp	r7, #24
	ble.n	.L_0818f962
	movs	r7, #24
	b.n	.L_0818f962
	.4byte 0x08143001
	.4byte 0x00000134
	.4byte 0x02010000
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x00000148
	.2byte 0x013e
	.2byte 0x0000
.L_0818f95c:
	cmp	r7, #40
	ble.n	.L_0818f962
	movs	r7, #40
.L_0818f962:
	ldr	r4, [sp, #20]
	subs	r5, r7, #2
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #12]
	ldr	r1, [r4, #4]
	asrs	r6, r2, #16
	adds	r2, r2, r3
	ldr	r3, [r4, #16]
	asrs	r0, r1, #16
	adds	r1, r1, r3
	mov	sl, r0
	str	r2, [r4, #0]
	str	r1, [r4, #4]
	movs	r0, #188
	movs	r1, #19
	bl	sub_081963ec
	ldr	r1, [sp, #28]
	asrs	r5, r5, #1
	lsls	r5, r5, #1
	adds	r5, r5, r1
	mov	r9, r6
	asrs	r6, r7, #1
	movs	r2, #0
	ldrsh	r1, [r5, r2]
	ldr	r3, [pc, #864]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	mov	r4, r9
	mov	r0, sl
	movs	r2, #192
	subs	r4, r4, r6
	subs	r0, r0, r7
	lsls	r2, r2, #18
	mov	fp, r4
	mov	r8, r0
	adds	r2, #188
	ldr	r0, [sp, #48]
	ldr	r4, [r2, #0]
	adds	r1, r1, r3
	mov	r2, fp
	mov	r3, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	ldr	r4, [pc, #808]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	mov	r2, r9
	adds	r1, r1, r4
	mov	r3, r8
	ldr	r4, [r0, #0]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #27
	movs	r0, #188
	bl	sub_081963ec
	movs	r2, #0
	ldrsh	r1, [r5, r2]
	ldr	r3, [pc, #764]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r0, #188
	ldr	r4, [r0, #0]
	mov	r2, fp
	adds	r1, r1, r3
	ldr	r0, [sp, #48]
	mov	r3, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #31
	movs	r0, #188
	bl	sub_081963ec
	movs	r2, #0
	ldrsh	r1, [r5, r2]
	ldr	r3, [pc, #720]
	str	r6, [sp, #0]
	str	r7, [sp, #4]
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r5, #188
	adds	r1, r1, r3
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #48]
	mov	r2, r9
	mov	r3, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_0818fa44:
	ldr	r6, [sp, #56]
	cmp	r6, #0
	bne.n	.L_0818fae8
	ldr	r7, [sp, #44]
	cmp	r7, #53
	bgt.n	.L_0818fae0
	ldr	r7, [sp, #52]
	movs	r0, #0
	mov	r8, r0
.L_0818fa56:
	ldr	r2, [r7, #24]
	subs	r3, r2, #1
	adds	r2, #31
	str	r3, [r7, #24]
	cmp	r2, #47
	bhi.n	.L_0818fad4
	ldr	r1, [sp, #16]
	movs	r2, #8
	movs	r3, #2
	movs	r4, #0
	mov	sl, r1
	mov	r9, r2
	mov	fp, r3
.L_0818fa70:
	ldr	r3, [r7, #24]
	lsls	r3, r3, #1
	adds	r3, r3, r4
	lsls	r6, r3, #1
	cmp	r6, #0
	blt.n	.L_0818face
	ldr	r0, [r7, #8]
	str	r4, [sp, #8]
	bl	sub_08002096
	mov	r1, sl
	ldr	r5, [r1, #0]
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r5, r5, #1
	asrs	r3, r3, #16
	ldr	r0, [r7, #8]
	adds	r5, r5, r3
	bl	sub_08002090
	adds	r2, r6, #0
	muls	r2, r0
	mov	r6, sl
	ldr	r3, [r6, #4]
	asrs	r2, r2, #16
	ldr	r1, [pc, #596]
	adds	r3, r3, r2
	mov	r2, r9
	subs	r2, #2
	ldrh	r1, [r1, r2]
	ldr	r0, [sp, #32]
	mov	r2, fp
	adds	r1, r0, r1
	subs	r5, r5, r2
	mov	r0, r9
	movs	r6, #4
	str	r0, [sp, #4]
	adds	r2, r5, #0
	subs	r3, #36
	str	r6, [sp, #0]
	ldr	r0, [sp, #48]
	ldr	r5, [sp, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9c02
.L_0818face:
	adds	r4, #1
	cmp	r4, #16
	bne.n	.L_0818fa70
.L_0818fad4:
	movs	r6, #1
	add	r8, r6
	mov	r0, r8
	adds	r7, #28
	cmp	r0, #16
	bne.n	.L_0818fa56
.L_0818fae0:
	ldr	r1, [sp, #56]
	movs	r3, #54
	cmp	r1, #0
	beq.n	.L_0818faf2
.L_0818fae8:
	ldr	r2, [sp, #56]
	movs	r3, #24
	cmp	r2, #3
	beq.n	.L_0818faf2
	movs	r3, #32
.L_0818faf2:
	ldr	r4, [sp, #44]
	cmp	r4, r3
	bge.n	.L_0818fafa
	b.n	.L_0818fd72
.L_0818fafa:
	cmp	r4, r3
	beq.n	.L_0818fb00
	b.n	.L_0818fc90
.L_0818fb00:
	ldr	r5, [sp, #56]
	cmp	r5, #0
	beq.n	.L_0818fb1a
	cmp	r5, #2
	beq.n	.L_0818fb1a
	cmp	r5, #3
	beq.n	.L_0818fb1a
	movs	r0, #133
	movs	r6, #0
	bl	sub_081180e8
	mov	r8, r6
	b.n	.L_0818fb5c
.L_0818fb1a:
	movs	r0, #145
	bl	sub_081180e8
	ldr	r0, [sp, #60]
	movs	r7, #0
	ldr	r3, [r0, #20]
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_0818fb64
	movs	r5, #36
.L_0818fb2e:
	ldr	r1, [sp, #60]
	ldrsh	r0, [r5, r1]
	movs	r1, #4
	bl	sub_08118088
	ldr	r4, [sp, #60]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0818fb2e
	b.n	.L_0818fb64
.L_0818fb48:
	mov	r5, r8
	ldr	r6, [sp, #60]
	lsls	r3, r5, #1
	adds	r3, #36
	ldrsh	r0, [r6, r3]
	movs	r1, #0
	bl	sub_08118088
	movs	r0, #1
	add	r8, r0
.L_0818fb5c:
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #20]
	cmp	r8, r3
	bne.n	.L_0818fb48
.L_0818fb64:
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0818fb8e
	movs	r6, #8
	movs	r5, #36
.L_0818fb70:
	ldr	r3, [sp, #60]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r2, #5
	mov	r3, r8
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r0, [sp, #60]
	movs	r7, #1
	ldr	r3, [r0, #20]
	add	r8, r7
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_0818fb70
.L_0818fb8e:
	ldr	r1, [sp, #52]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r7, [sp, #16]
	ldr	r6, [sp, #52]
	movs	r4, #0
	movs	r5, #4
	mov	r8, r4
	mov	sl, r5
.L_0818fba8:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r5, r0, #0
	adds	r3, #255
	ands	r5, r3
	ldr	r3, [r7, #0]
	adds	r0, r5, #0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r7, #4]
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08002096
	mov	r3, sl
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r6, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r3, sl
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #3
	str	r3, [r6, #16]
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002054
	movs	r1, #1
	adds	r0, #17
	add	r8, r1
	str	r0, [r6, #24]
	mov	r2, r8
	movs	r0, #4
	add	sl, r0
	adds	r6, #28
	cmp	r2, #16
	bne.n	.L_0818fba8
	ldr	r4, [sp, #16]
	ldr	r5, [sp, #52]
	movs	r6, #224
	movs	r3, #0
	lsls	r6, r6, #1
	mov	r8, r3
	mov	sl, r4
	adds	r7, r5, r6
.L_0818fc14:
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r6, r0, #0
	adds	r3, #255
	mov	r0, sl
	ands	r6, r3
	ldr	r3, [r0, #0]
	adds	r5, #128
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	adds	r0, r6, #0
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	negs	r1, r3
	asrs	r0, r1, #5
	str	r0, [r7, #16]
	ldr	r2, [sp, #56]
	cmp	r2, #1
	bne.n	.L_0818fc78
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	lsrs	r3, r1, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	str	r3, [r7, #16]
.L_0818fc78:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002054
	movs	r3, #1
	add	r8, r3
	adds	r0, #17
	mov	r4, r8
	str	r0, [r7, #24]
	adds	r7, #28
	cmp	r4, #47
	bne.n	.L_0818fc14
.L_0818fc90:
	ldr	r5, [sp, #56]
	cmp	r5, #0
	beq.n	.L_0818fc9e
	cmp	r5, #2
	beq.n	.L_0818fc9e
	cmp	r5, #3
	bne.n	.L_0818fd16
.L_0818fc9e:
	ldr	r7, [pc, #96]
	ldr	r5, [sp, #52]
	movs	r6, #0
	mov	r8, r6
	mov	sl, r7
.L_0818fca8:
	movs	r0, #2
	ldrsh	r6, [r5, r0]
	movs	r1, #6
	ldrsh	r7, [r5, r1]
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_0818fcf0
	movs	r1, #3
	bl	sub_08002054
	mov	r2, sl
	ldrb	r1, [r2, r0]
	ldr	r3, [sp, #52]
	movs	r0, #32
	lsls	r1, r1, #11
	movs	r4, #240
	adds	r1, r3, r1
	lsls	r4, r4, #4
	adds	r2, r6, #0
	str	r0, [sp, #0]
	adds	r3, r7, #0
	movs	r0, #64
	str	r0, [sp, #4]
	adds	r1, r1, r4
	subs	r2, #16
	subs	r3, #32
	ldr	r0, [sp, #48]
	ldr	r6, [sp, #36]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x69a8
.L_0818fcf0:
	cmp	r0, #0
	ble.n	.L_0818fd04
	subs	r3, r0, #1
	b.n	.L_0818fd08
	.4byte 0x02010000
	.4byte 0x08197410
	.2byte 0x9ec5
	.2byte 0x0819
.L_0818fd04:
	movs	r3, #1
	negs	r3, r3
.L_0818fd08:
	str	r3, [r5, #24]
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_0818fca8
.L_0818fd16:
	ldr	r2, [sp, #52]
	movs	r3, #224
	ldr	r6, [pc, #200]
	movs	r1, #0
	lsls	r3, r3, #1
	mov	r8, r1
	adds	r5, r2, r3
.L_0818fd24:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0818fd66
	asrs	r0, r0, #2
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #32]
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
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0818fd66:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #47
	bne.n	.L_0818fd24
.L_0818fd72:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #52]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #44]
	ldr	r5, [sp, #24]
	adds	r4, #1
	str	r4, [sp, #44]
	cmp	r4, r5
	beq.n	.L_0818fda0
	b.n	.L_0818f766
.L_0818fda0:
	ldr	r0, [pc, #68]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	ldr	r6, [sp, #56]
	cmp	r6, #0
	bne.n	.L_0818fdd6
	movs	r1, #240
	ldr	r5, [pc, #52]
	lsls	r1, r1, #6
	ldr	r0, [pc, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #48]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x480a
	bl	sub_08014644
	ldr	r0, [sp, #60]
	bl	sub_081504c0
	b.n	.L_0818feee
.L_0818fdd6:
	ldr	r7, [sp, #56]
	cmp	r7, #3
	bne.n	.L_0818fdf8
	bl	sub_08143bb8
	b.n	.L_0818feee
	movs	r0, r0
	.4byte 0x08197410
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
.L_0818fdf8:
	movs	r3, #192
	movs	r0, #195
	lsls	r3, r3, #18
	lsls	r0, r0, #1
	ldr	r7, [r3, #36]
	bl	sub_081c0010
	movs	r1, #238
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	adds	r1, #160
	adds	r3, r0, r1
	ldr	r2, [pc, #64]
	ldr	r3, [r3, #0]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #164
	strh	r3, [r2, #4]
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r1, #128
	strh	r3, [r2, #6]
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #36]
	ldr	r0, [pc, #40]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4809
	bl	sub_08014644
	ldr	r6, [pc, #36]
	ldr	r5, [pc, #36]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0818fe8e
	b.n	.L_0818fe6c
	.4byte 0x00000787
	.4byte 0x03001120
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x08143489
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0818fe6c:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #230
	adds	r3, r3, r6
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0818fe8e:
	strh	r1, [r5, #0]
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #206
	lsls	r0, r0, #3
	adds	r3, r7, r0
	ldrh	r1, [r3, #0]
	movs	r0, #2
	movs	r2, #0
	bl	sub_08118038
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_0818fee6
	b.n	.L_0818fec4
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0818fec4:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #234
	adds	r3, r3, r6
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0818fee6:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
.L_0818feee:
	add	sp, #88
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
	sub	sp, #220
	str	r0, [sp, #96]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	adds	r3, r5, #0
	str	r0, [sp, #92]
	adds	r3, #176
	ldr	r1, [r5, #96]
	movs	r4, #0
	str	r1, [sp, #88]
	ldr	r0, [sp, #96]
	ldr	r2, [r5, #100]
	str	r2, [sp, #68]
	movs	r2, #150
	ldr	r3, [r3, #0]
	str	r3, [sp, #64]
	ldr	r3, [r5, #48]
	str	r4, [sp, #56]
	str	r3, [sp, #60]
	movs	r3, #160
	ldr	r1, [r0, #8]
	lsls	r3, r3, #11
	adds	r0, r1, #0
	bl	sub_08118078
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r1, [sp, #68]
	ldr	r0, [pc, #52]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [sp, #92]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r4, #132
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	movs	r1, #19
	movs	r0, #104
	b.n	.L_0818ff90
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000134
	.2byte 0x3001
	.2byte 0x0814
.L_0818ff90:
	bl	sub_081963ec
	movs	r1, #204
	ldr	r0, [sp, #92]
	movs	r2, #212
	lsls	r1, r1, #7
	lsls	r2, r2, #7
	adds	r1, #64
	adds	r2, #64
	adds	r1, r0, r1
	adds	r2, r0, r2
	ldr	r3, [sp, #96]
	ldr	r5, [r5, #104]
	str	r1, [sp, #52]
	str	r2, [sp, #48]
	str	r5, [sp, #72]
	ldr	r0, [r3, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r4, #0
	str	r0, [sp, #40]
	movs	r0, #128
	lsls	r0, r0, #9
	str	r0, [sp, #28]
	movs	r1, #8
	ldr	r0, [pc, #432]
	movs	r2, #16
	movs	r3, #32
	str	r4, [sp, #36]
	str	r4, [sp, #32]
	bl	sub_08178680
	movs	r1, #2
	ldr	r0, [sp, #48]
	movs	r2, #32
	movs	r3, #15
	bl	sub_08178680
	movs	r1, #0
	mov	fp, r1
	mov	sl, r1
	mov	r9, r1
.L_0818ffe6:
	mov	r0, sl
	bl	sub_08002090
	negs	r0, r0
	lsls	r5, r0, #1
	adds	r5, r5, r0
	mov	r0, sl
	bl	sub_08002096
	lsls	r5, r5, #4
	lsls	r3, r0, #1
	ldr	r6, [sp, #52]
	asrs	r5, r5, #16
	adds	r3, r3, r0
	lsls	r3, r3, #4
	adds	r5, #24
	asrs	r7, r3, #16
	movs	r2, #0
	mov	r8, r5
	add	r6, r9
.L_0819000e:
	lsls	r5, r2, #13
	adds	r0, r5, #0
	str	r2, [sp, #12]
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #0]
	mov	r3, r8
	strb	r3, [r6, #1]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r2, [sp, #12]
	asrs	r3, r3, #16
	adds	r2, #1
	strb	r3, [r6, #2]
	adds	r6, #4
	cmp	r2, #2
	bne.n	.L_0819000e
	movs	r4, #128
	movs	r1, #1
	lsls	r4, r4, #3
	add	fp, r1
	adds	r4, #68
	movs	r0, #8
	mov	r2, fp
	add	sl, r4
	add	r9, r0
	cmp	r2, #16
	bne.n	.L_0818ffe6
	ldr	r3, [sp, #40]
	ldr	r0, [sp, #40]
	ldr	r3, [r3, #72]
	movs	r1, #0
	str	r3, [sp, #44]
	bl	sub_08020098
	ldr	r4, [sp, #40]
	mov	r0, sp
	mov	r1, sp
	movs	r3, #0
	adds	r0, #208
	adds	r1, #100
	str	r3, [r4, #36]
	str	r3, [r4, #40]
	str	r3, [r4, #44]
	str	r3, [r4, #52]
	str	r3, [r4, #72]
	str	r3, [sp, #84]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
.L_0819007c:
	ldr	r2, [sp, #84]
	cmp	r2, #4
	bne.n	.L_08190088
	movs	r0, #212
	bl	sub_081c0010
.L_08190088:
	ldr	r3, [sp, #84]
	cmp	r3, #28
	bne.n	.L_08190094
	movs	r0, #140
	bl	sub_081c0010
.L_08190094:
	ldr	r4, [sp, #84]
	cmp	r4, #84
	bne.n	.L_081900a0
	movs	r0, #104
	bl	sub_081c0010
.L_081900a0:
	ldr	r0, [sp, #84]
	cmp	r0, #143
	bne.n	.L_081900ac
	movs	r0, #191
	bl	sub_081c0010
.L_081900ac:
	ldr	r1, [sp, #84]
	cmp	r1, #171
	bne.n	.L_081900b8
	movs	r0, #145
	bl	sub_081c0010
.L_081900b8:
	ldr	r3, [pc, #192]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081900d2
	ldr	r2, [sp, #84]
	cmp	r2, #3
	ble.n	.L_081900d2
	cmp	r2, #142
	bgt.n	.L_081900d2
	movs	r3, #143
	str	r3, [sp, #84]
.L_081900d2:
	ldr	r4, [sp, #84]
	cmp	r4, #0
	beq.n	.L_081900da
	b.n	.L_081901d2
.L_081900da:
	movs	r1, #240
	ldr	r0, [sp, #88]
	lsls	r1, r1, #6
	ldr	r2, [pc, #156]
	ldr	r3, [pc, #160]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4827
	ldr	r1, [pc, #160]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #92]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	ldr	r0, [pc, #144]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #92]
	movs	r4, #240
	lsls	r4, r4, #4
	adds	r1, r3, r4
	ldr	r0, [pc, #132]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #92]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r1, r2, r3
	ldr	r0, [pc, #116]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #92]
	movs	r2, #172
	lsls	r2, r2, #6
	adds	r1, r4, r2
	ldr	r0, [pc, #104]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	bl	sub_0815b410
	movs	r3, #128
	ldr	r2, [pc, #48]
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	ldr	r4, [sp, #96]
	movs	r0, #160
	ldr	r3, [r4, #4]
	lsls	r0, r0, #16
	str	r0, [sp, #36]
	cmp	r3, #0
	beq.n	.L_0819015e
	movs	r1, #160
	lsls	r1, r1, #15
	str	r1, [sp, #36]
.L_0819015e:
	ldr	r2, [pc, #64]
	ldr	r1, [pc, #64]
	str	r2, [sp, #32]
	ldr	r0, [pc, #64]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_081901d0
	b.n	.L_081901ac
	.4byte 0x00001010
	.4byte 0x02012000
	.4byte 0x03001150
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x000000c2
	.4byte 0x02014000
	.4byte 0x000000e9
	.4byte 0x000000c9
	.4byte 0x00000103
	.4byte 0x000000cd
	.4byte 0xffa00000
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_081901ac:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #224
	adds	r3, r3, r1
	lsls	r2, r2, #3
	adds	r3, #4
	adds	r2, #132
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_081901d0:
	strh	r4, [r0, #0]
.L_081901d2:
	ldr	r3, [sp, #84]
	cmp	r3, #1
	bne.n	.L_08190214
	ldr	r4, [sp, #64]
	add	r0, sp, #180
	str	r3, [r4, #16]
	ldr	r1, [sp, #96]
	ldr	r3, [r1, #8]
	movs	r1, #0
	strh	r3, [r0, #0]
	movs	r3, #255
	strh	r3, [r0, #2]
	bl	sub_08118010
	movs	r0, #1
	ldr	r1, [pc, #404]
	movs	r2, #0
	bl	sub_08118040
	bl	sub_0817d6c4
	movs	r0, #8
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164abc
	movs	r1, #205
	lsls	r1, r1, #1
	movs	r0, #1
	adds	r1, #255
	movs	r2, #2
	bl	sub_08152404
.L_08190214:
	ldr	r2, [sp, #96]
	ldr	r1, [sp, #20]
	ldr	r0, [r2, #8]
	bl	sub_0815e20c
	ldr	r3, [sp, #84]
	cmp	r3, #3
	bne.n	.L_0819022c
	ldr	r4, [sp, #60]
	movs	r0, #54
	ldrsh	r4, [r4, r0]
	str	r4, [sp, #56]
.L_0819022c:
	ldr	r3, [sp, #84]
	subs	r3, #16
	cmp	r3, #61
	bhi.n	.L_08190258
	ldr	r0, [sp, #96]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0819024a
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #332]
	ldrh	r3, [r1, #54]
	adds	r4, r1, #0
	adds	r3, r3, r2
	strh	r3, [r4, #54]
	b.n	.L_08190258
.L_0819024a:
	ldr	r0, [sp, #60]
	movs	r1, #128
	ldrh	r3, [r0, #54]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r2, r0, #0
	strh	r3, [r2, #54]
.L_08190258:
	movs	r3, #0
	ldr	r4, [sp, #84]
	str	r3, [sp, #80]
	str	r3, [sp, #16]
	mov	sl, r4
	mov	fp, r3
.L_08190264:
	ldr	r5, [pc, #296]
	ldr	r7, [sp, #16]
	movs	r0, #0
	mov	r8, r0
	mov	r9, sl
	add	r5, fp
.L_08190270:
	mov	r1, r8
	movs	r6, #80
	cmp	r1, #1
	beq.n	.L_08190288
	mov	r2, r9
	lsls	r0, r2, #12
	bl	sub_08002096
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	adds	r6, r0, #0
	adds	r6, #120
.L_08190288:
	adds	r0, r7, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r5, #0]
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r4, #1
	asrs	r3, r3, #16
	add	r8, r4
	strb	r3, [r5, #1]
	mov	r0, r8
	movs	r3, #0
	strb	r3, [r5, #2]
	adds	r5, #4
	cmp	r0, #2
	bne.n	.L_08190270
	ldr	r1, [sp, #16]
	ldr	r0, [sp, #80]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r1, r1, r2
	movs	r3, #4
	movs	r4, #8
	adds	r0, #1
	str	r1, [sp, #16]
	add	sl, r3
	add	fp, r4
	str	r0, [sp, #80]
	cmp	r0, #33
	bne.n	.L_08190264
	ldr	r4, [pc, #192]
	ldrh	r1, [r4, #6]
	cmp	r1, #104
	ble.n	.L_081902dc
	ldr	r2, [pc, #188]
	adds	r1, r1, r2
.L_081902dc:
	ldr	r3, [sp, #84]
	cmp	r3, #63
	bgt.n	.L_081902ea
	movs	r0, #16
	adds	r5, r3, #0
	negs	r0, r0
	b.n	.L_08190342
.L_081902ea:
	ldr	r3, [sp, #84]
	subs	r3, #64
	cmp	r3, #7
	bhi.n	.L_081902fe
	ldr	r0, [sp, #84]
	ldr	r5, [sp, #84]
	lsls	r2, r0, #1
	adds	r0, r2, #0
	subs	r0, #144
	b.n	.L_08190342
.L_081902fe:
	ldr	r3, [sp, #84]
	subs	r3, #72
	cmp	r3, #11
	bhi.n	.L_0819030c
	ldr	r5, [sp, #84]
	movs	r0, #0
	b.n	.L_08190342
.L_0819030c:
	ldr	r2, [sp, #84]
	cmp	r2, #154
	ble.n	.L_0819032a
	adds	r3, r2, #0
	subs	r3, #155
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_08190340
	ldr	r5, [sp, #84]
	movs	r0, #0
	b.n	.L_08190342
.L_0819032a:
	ldr	r5, [sp, #84]
	subs	r5, #84
	adds	r0, r5, #0
	cmp	r5, #16
	ble.n	.L_08190336
	movs	r0, #16
.L_08190336:
	ldr	r2, [pc, #100]
	ldr	r3, [r2, #16]
	subs	r3, r3, r0
	str	r3, [r2, #16]
	b.n	.L_08190344
.L_08190340:
	ldr	r5, [sp, #84]
.L_08190342:
	subs	r5, #84
.L_08190344:
	movs	r3, #16
	adds	r1, r1, r0
	negs	r3, r3
	cmp	r1, r3
	bge.n	.L_08190350
	adds	r1, #120
.L_08190350:
	cmp	r1, #104
	ble.n	.L_08190356
	subs	r1, #120
.L_08190356:
	strh	r1, [r4, #6]
	ldr	r3, [sp, #84]
	subs	r3, #42
	cmp	r3, #37
	bhi.n	.L_08190370
	ldr	r4, [sp, #32]
	movs	r0, #128
	lsls	r0, r0, #10
	adds	r4, r4, r0
	ldr	r0, [pc, #52]
	str	r4, [sp, #32]
	bl	sub_0815f0a0
.L_08190370:
	cmp	r5, #58
	bhi.n	.L_081903bc
	ldr	r1, [sp, #96]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_081903a8
	ldr	r2, [sp, #36]
	ldr	r3, [pc, #36]
	adds	r2, r2, r3
	str	r2, [sp, #36]
	b.n	.L_081903b2
	movs	r0, r0
	.4byte 0x00000072
	.4byte 0xffffff00
	.4byte 0x02010000
	.4byte 0x03001120
	.4byte 0xffff0000
	.4byte 0x030011e0
	.4byte 0x00000148
	.2byte 0x0000
	.2byte 0xfffe
.L_081903a8:
	.2byte 0x9c09
	movs	r0, #128
	lsls	r0, r0, #10
	adds	r4, r4, r0
	str	r4, [sp, #36]
.L_081903b2:
	ldr	r1, [sp, #32]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r1, r1, r2
	str	r1, [sp, #32]
.L_081903bc:
	ldr	r3, [sp, #84]
	cmp	r3, #143
	bne.n	sub_0819043c
	movs	r2, #2
	negs	r2, r2
	movs	r0, #12
	adds	r1, r2, #0
	negs	r0, r0
	bl	sub_08164abc
	add	r4, sp, #56
	ldr	r0, [sp, #60]
	ldrh	r4, [r4, #0]
	movs	r1, #192
	strh	r4, [r0, #54]
	movs	r0, #240
	lsls	r0, r0, #15
	lsls	r1, r1, #14
	str	r0, [sp, #36]
	str	r1, [sp, #32]
	ldr	r2, [sp, #96]
	add	r0, sp, #152
	ldrh	r3, [r2, #36]
	movs	r1, #0
	strh	r3, [r0, #0]
	movs	r3, #255
	strh	r3, [r0, #2]
	bl	sub_08118010
	ldr	r2, [pc, #900]
	movs	r3, #139
	lsls	r3, r3, #2
	str	r3, [r2, #16]
	ldr	r4, [sp, #96]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118098
	ldr	r3, [r0, #0]
	movs	r0, #0
	str	r0, [r3, #8]
	str	r0, [r3, #16]
	ldr	r1, [sp, #92]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #852]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #848]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9815
	cmp	r0, #154
	ble.n	.L_0819045e
	adds	r3, r0, #0
	subs	r3, #155
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #16
	subs	r1, r2, r3
	cmp	r1, #0
	bge.n	.L_08190456
	movs	r1, #0
.L_08190456:
	ldr	r2, [pc, #804]
	ldr	r3, [r2, #16]
	subs	r3, r3, r1
	str	r3, [r2, #16]
.L_0819045e:
	ldr	r1, [sp, #84]
	cmp	r1, #84
	bne.n	.L_081904c8
	ldr	r1, [pc, #800]
	ldr	r0, [pc, #804]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08190498
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #224
	adds	r3, r3, r1
	lsls	r2, r2, #3
	adds	r3, #4
	adds	r2, #133
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08190498:
	strh	r4, [r0, #0]
	ldr	r0, [sp, #40]
	movs	r1, #128
	bl	sub_08020098
	movs	r1, #240
	lsls	r1, r1, #6
	ldr	r2, [pc, #744]
	ldr	r3, [pc, #744]
	ldr	r0, [sp, #88]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c17
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r4, r0
	movs	r3, #3
	movs	r1, #238
	str	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #724]
	adds	r1, #132
	adds	r2, r4, r1
	str	r3, [r2, #0]
.L_081904c8:
	ldr	r2, [sp, #84]
	cmp	r2, #171
	bne.n	.L_08190528
	ldr	r3, [sp, #92]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
	movs	r1, #240
	ldr	r2, [pc, #680]
	ldr	r0, [sp, #88]
	ldr	r3, [pc, #680]
	lsls	r1, r1, #6
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9918
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	movs	r1, #0
	bl	sub_08118088
	ldr	r3, [sp, #96]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r4, [sp, #92]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r4, r0
	movs	r3, #1
	movs	r1, #238
	str	r3, [r2, #0]
	lsls	r1, r1, #7
	ldr	r3, [pc, #632]
	adds	r1, #132
	adds	r2, r4, r1
	str	r3, [r2, #0]
.L_08190528:
	ldr	r2, [sp, #84]
	cmp	r2, #174
	ble.n	.L_08190538
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_08190538:
	ldr	r3, [sp, #84]
	cmp	r3, #1
	bgt.n	.L_08190540
	b.n	.L_081906e8
.L_08190540:
	cmp	r3, #142
	ble.n	.L_08190560
	movs	r2, #128
	lsls	r2, r2, #3
	subs	r3, #143
	adds	r2, #212
	muls	r3, r2
	movs	r4, #128
	lsls	r4, r4, #7
	movs	r0, #192
	adds	r4, r3, r4
	lsls	r0, r0, #8
	str	r4, [sp, #28]
	cmp	r4, r0
	ble.n	.L_08190560
	str	r0, [sp, #28]
.L_08190560:
	ldr	r1, [sp, #28]
	add	r2, sp, #116
	lsls	r3, r1, #1
	str	r3, [r2, #4]
	add	r1, sp, #136
	str	r3, [sp, #116]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	ldr	r4, [sp, #36]
	str	r4, [r1, #0]
	ldr	r0, [sp, #32]
	str	r0, [r1, #8]
	ldr	r4, [sp, #92]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #220
	adds	r3, r4, r0
	ldr	r0, [r3, #0]
	movs	r3, #32
	strb	r3, [r0, #23]
	movs	r3, #0
	strb	r3, [r0, #22]
	movs	r4, #208
	ldr	r3, [r1, #8]
	lsls	r4, r4, #14
	adds	r3, r3, r4
	ldr	r4, [pc, #516]
	cmp	r3, r4
	bhi.n	.L_081905a6
	movs	r3, #0
	bl	sub_08020010
.L_081905a6:
	ldr	r0, [sp, #84]
	cmp	r0, #1
	bgt.n	.L_081905ae
	b.n	.L_081906e8
.L_081905ae:
	movs	r0, #1
	bl	sub_081969f8
	ldr	r1, [pc, #492]
	add	r6, sp, #108
	adds	r7, r0, #0
	movs	r3, #6
	movs	r2, #0
	str	r3, [r7, #0]
	strb	r3, [r6, #0]
	movs	r3, #4
	str	r1, [r7, #12]
	strb	r3, [r6, #1]
	str	r6, [r7, #16]
	str	r2, [r7, #20]
	strb	r2, [r7, #25]
	ldr	r2, [sp, #84]
	movs	r1, #5
	lsrs	r0, r2, #31
	adds	r0, r2, r0
	asrs	r0, r0, #1
	bl	sub_08002064
	ldr	r3, [sp, #92]
	lsls	r0, r0, #10
	movs	r4, #184
	adds	r0, r3, r0
	lsls	r4, r4, #5
	adds	r0, r0, r4
	str	r0, [r6, #4]
	ldr	r0, [pc, #444]
	movs	r5, #128
	str	r0, [r7, #8]
	bl	sub_08014de4
	lsls	r5, r5, #8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r4, [pc, #424]
	ldr	r1, [sp, #36]
	ldr	r3, [sp, #32]
	ldr	r2, [pc, #420]
	adds	r0, r1, r2
	adds	r1, r3, r4
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #181
	lsls	r0, r0, #8
	adds	r0, #200
	bl	sub_0801521c
	ldr	r0, [sp, #28]
	bl	sub_0801521c
	ldr	r1, [pc, #380]
	movs	r2, #66
	ldr	r0, [pc, #392]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	movs	r3, #5
	strb	r3, [r6, #0]
	strb	r3, [r6, #1]
	ldr	r0, [sp, #92]
	movs	r1, #172
	lsls	r1, r1, #6
	adds	r3, r0, r1
	str	r3, [r6, #4]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r2, [sp, #48]
	movs	r3, #3
	str	r2, [r7, #8]
	movs	r2, #24
	str	r3, [r7, #4]
	negs	r2, r2
	movs	r3, #0
	str	r2, [r7, #20]
	strb	r3, [r7, #25]
	ldr	r4, [sp, #84]
	cmp	r4, #142
	ble.n	.L_081906e2
	adds	r1, r4, #0
	subs	r1, #143
	cmp	r1, #32
	ble.n	.L_0819066a
	movs	r1, #32
.L_0819066a:
	ldr	r3, [sp, #84]
	ldr	r4, [sp, #84]
	lsls	r2, r3, #1
	lsls	r3, r1, #1
	adds	r3, r3, r1
	ldr	r6, [pc, #320]
	movs	r0, #0
	lsls	r3, r3, #7
	mov	r8, r0
	mov	sl, r3
	adds	r5, r2, r4
.L_08190680:
	movs	r3, #127
	bics	r3, r5
	strb	r3, [r7, #24]
	bl	sub_08014de4
	ldr	r0, [pc, #304]
	ldr	r1, [pc, #304]
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #160
	lsls	r0, r0, #9
	movs	r2, #128
	ldr	r1, [pc, #296]
	lsls	r2, r2, #9
	adds	r0, #80
	bl	sub_080151e4
	ldr	r0, [sp, #28]
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_080150e4
	ldr	r0, [pc, #276]
	adds	r5, #20
	add	r0, sl
	bl	sub_08015024
	adds	r0, r6, #0
	bl	sub_08015068
	ldr	r1, [pc, #224]
	ldr	r0, [sp, #52]
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	movs	r1, #1
	movs	r0, #192
	add	r8, r1
	lsls	r0, r0, #5
	mov	r2, r8
	adds	r6, r6, r0
	cmp	r2, #10
	bne.n	.L_08190680
.L_081906e2:
	adds	r0, r7, #0
	bl	sub_08013164
.L_081906e8:
	ldr	r3, [sp, #84]
	cmp	r3, #8
	bne.n	.L_08190720
	movs	r1, #240
	ldr	r0, [sp, #88]
	lsls	r1, r1, #6
	ldr	r2, [pc, #152]
	ldr	r4, [pc, #156]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d17
	movs	r0, #0
	mov	r8, r0
	movs	r6, #127
.L_08190704:
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #1
	add	r8, r1
	ands	r0, r6
	mov	r2, r8
	str	r0, [r5, #4]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_08190704
.L_08190720:
	ldr	r3, [sp, #84]
	subs	r3, #8
	cmp	r3, #79
	bhi.n	.L_081907d0
	ldr	r4, [sp, #84]
	ldr	r1, [pc, #160]
	lsls	r3, r4, #1
	adds	r6, r3, #0
	ldr	r5, [sp, #92]
	subs	r6, #16
	movs	r3, #127
	movs	r0, #0
	ands	r6, r3
	mov	r8, r0
	mov	sl, r1
	movs	r7, #2
.L_08190740:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	sub_0819076e
	ldr	r3, [r5, #4]
	movs	r0, #1
	subs	r3, r3, r6
	ldr	r2, [r5, #0]
	cmp	r3, #127
	ble.n	.L_08190754
	subs	r3, #128
.L_08190754:
	cmp	r3, #0
	bge.n	.L_0819075a
	adds	r3, #128
.L_0819075a:
	mov	r4, sl
	ldrh	r1, [r4, #0]
	ldr	r4, [sp, #68]
	str	r0, [sp, #0]
	adds	r1, r4, r1
	str	r7, [sp, #4]
	ldr	r0, [sp, #88]
	ldr	r4, [sp, #72]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_08190740
	b.n	.L_081907d0
	.4byte 0x030011e0
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x06060606
	.4byte 0x10101010
	.4byte 0x00fbffff
	.4byte 0x02011000
	.4byte 0x02012000
	.4byte 0xffb40000
	.4byte 0xff820000
	.4byte 0x02010000
	.4byte 0xffffc000
	.4byte 0xfffe0000
	.4byte 0xffe00000
	.4byte 0x00035208
	.4byte 0xffffd000
	.2byte 0x7410
	.2byte 0x0819
.L_081907d0:
	ldr	r2, [sp, #84]
	cmp	r2, #142
	ble.n	.L_0819086c
	adds	r6, r2, #0
	subs	r6, #143
	cmp	r6, #32
	ble.n	.L_081907e0
	movs	r6, #32
.L_081907e0:
	ldr	r3, [sp, #84]
	cmp	r3, #143
	bne.n	.L_08190818
	ldr	r5, [sp, #92]
	movs	r4, #0
	mov	r8, r4
	movs	r7, #127
.L_081907ee:
	bl	sub_08014878
	ands	r0, r7
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #192
	bl	sub_0800206c
	subs	r0, #64
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r7
	str	r0, [r5, #8]
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_081907ee
.L_08190818:
	bl	sub_08014de4
	lsls	r0, r6, #1
	ldr	r2, [pc, #780]
	adds	r0, r0, r6
	lsls	r0, r0, #7
	adds	r0, r0, r2
	bl	sub_08015024
	ldr	r7, [pc, #772]
	ldr	r5, [sp, #92]
	movs	r3, #0
	movs	r4, #1
	mov	r8, r3
	mov	sl, r4
	add	r6, sp, #124
.L_08190838:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08015778
	ldrh	r1, [r7, #0]
	ldr	r0, [sp, #68]
	mov	r4, sl
	adds	r1, r0, r1
	movs	r0, #4
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #0]
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #72]
	ldr	r0, [sp, #88]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x686b
	movs	r0, #1
	add	r8, r0
	subs	r3, #2
	mov	r1, r8
	str	r3, [r5, #4]
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_08190838
.L_0819086c:
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #696]
	ldr	r3, [sp, #100]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #692]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #100]
	ldr	r2, [pc, #680]
	ldr	r3, [sp, #24]
	adds	r7, r0, #0
	str	r2, [r3, #4]
	ldr	r3, [pc, #676]
	str	r1, [r7, #0]
	str	r3, [r7, #8]
	ldr	r4, [sp, #24]
	mov	r0, fp
	str	r4, [r7, #16]
	str	r0, [r7, #12]
	ldr	r2, [sp, #20]
	movs	r1, #0
	mov	r8, r1
	mov	sl, r2
	movs	r6, #0
.L_081908b0:
	ldr	r3, [pc, #656]
	ldrh	r2, [r3, r6]
	ldr	r3, [sp, #84]
	cmp	r3, r2
	blt.n	.L_08190942
	ldr	r4, [sp, #84]
	adds	r3, r2, #0
	adds	r3, #20
	cmp	r4, r3
	bge.n	.L_08190942
	subs	r1, r4, r2
	lsls	r2, r1, #3
	movs	r3, #64
	subs	r4, r3, r2
	cmp	r4, #0
	ble.n	.L_081908d2
	movs	r4, #0
.L_081908d2:
	movs	r0, #64
	negs	r0, r0
	cmp	r4, r0
	ble.n	.L_08190942
	lsls	r3, r1, #14
	movs	r1, #128
	lsls	r1, r1, #7
	str	r4, [r7, #20]
	adds	r5, r3, r1
	bl	sub_08014de4
	mov	r2, r8
	cmp	r2, #1
	bgt.n	.L_0819090c
	mov	r3, sl
	ldr	r0, [r3, #0]
	mov	r4, sl
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08190918
.L_0819090c:
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #13
	movs	r2, #0
	bl	sub_08015160
.L_08190918:
	lsls	r1, r5, #1
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	ldr	r3, [pc, #548]
	ldrsh	r0, [r3, r6]
	bl	sub_08015024
	ldr	r3, [pc, #544]
	ldrsh	r0, [r3, r6]
	bl	sub_08015068
	ldr	r0, [pc, #540]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08190942:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r6, #2
	cmp	r4, #5
	bne.n	.L_081908b0
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r0, [sp, #24]
	add	r3, sp, #100
	strb	r4, [r0, #0]
	str	r3, [sp, #24]
	ldr	r1, [sp, #24]
	movs	r3, #6
	strb	r3, [r1, #1]
	ldr	r2, [sp, #92]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	str	r3, [r1, #4]
	ldr	r3, [pc, #488]
	movs	r0, #0
	str	r3, [r7, #8]
	ldr	r1, [sp, #20]
	movs	r2, #16
	mov	r8, r0
	mov	sl, r1
	movs	r6, #0
	mov	r9, r2
.L_0819097c:
	ldr	r3, [sp, #84]
	cmp	r3, r9
	bge.n	.L_08190984
	b.n	.L_08190a80
.L_08190984:
	ldr	r4, [sp, #84]
	adds	r3, r6, #0
	adds	r3, #80
	cmp	r4, r3
	blt.n	.L_08190990
	b.n	.L_08190a80
.L_08190990:
	mov	r0, r9
	subs	r2, r4, r0
	lsls	r3, r2, #3
	adds	r1, r3, #0
	subs	r1, #64
	lsls	r5, r2, #11
	cmp	r1, #0
	ble.n	.L_081909a2
	movs	r1, #0
.L_081909a2:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r5, r3
	ble.n	.L_081909ae
	movs	r5, #128
	lsls	r5, r5, #9
.L_081909ae:
	cmp	r2, #47
	ble.n	.L_081909bc
	lsls	r3, r2, #12
	movs	r4, #192
	subs	r3, r5, r3
	lsls	r4, r4, #10
	adds	r5, r3, r4
.L_081909bc:
	str	r1, [r7, #20]
	bl	sub_08014de4
	ldr	r0, [sp, #96]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_081909f6
	mov	r1, sl
	ldr	r0, [r1, #0]
	mov	r2, r8
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [pc, #384]
	asrs	r0, r0, #1
	ldrsb	r3, [r3, r2]
	mov	r4, sl
	adds	r0, r0, r3
	ldr	r3, [pc, #380]
	subs	r0, #64
	ldrsb	r1, [r3, r2]
	ldr	r3, [r4, #4]
	lsls	r0, r0, #16
	adds	r1, r1, r3
	subs	r1, #54
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08190a20
.L_081909f6:
	mov	r1, sl
	ldr	r0, [r1, #0]
	mov	r2, r8
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [pc, #340]
	asrs	r0, r0, #1
	ldrsb	r3, [r3, r2]
	mov	r4, sl
	subs	r0, r0, r3
	ldr	r3, [pc, #336]
	subs	r0, #62
	ldrsb	r1, [r3, r2]
	ldr	r3, [r4, #4]
	lsls	r0, r0, #16
	adds	r1, r1, r3
	subs	r1, #54
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
.L_08190a20:
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	ldr	r0, [sp, #96]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_08190a40
	ldr	r3, [pc, #296]
	ldrsh	r0, [r3, r6]
	bl	sub_080150e4
	b.n	.L_08190a4a
.L_08190a40:
	ldr	r3, [pc, #284]
	ldrsh	r0, [r3, r6]
	negs	r0, r0
	bl	sub_080150e4
.L_08190a4a:
	ldr	r3, [pc, #280]
	ldrsh	r0, [r3, r6]
	bl	sub_08015024
	ldr	r3, [pc, #276]
	ldrsh	r0, [r3, r6]
	bl	sub_08015068
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_08190a62
	adds	r0, r5, #3
.L_08190a62:
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r0, r0, #2
	lsls	r1, r5, #2
	asrs	r2, r2, #1
	bl	sub_080151e4
	ldr	r0, [pc, #248]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08190a80:
	movs	r3, #1
	add	r8, r3
	movs	r2, #2
	mov	r4, r8
	adds	r6, #2
	add	r9, r2
	cmp	r4, #8
	beq.n	.L_08190a92
	b.n	.L_0819097c
.L_08190a92:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
	movs	r1, #240
	ldr	r0, [sp, #92]
	lsls	r1, r1, #7
	adds	r1, #228
	adds	r2, r0, r1
.L_08190aa8:
	ldr	r3, [r2, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_08190aa8
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #92]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #84]
	adds	r0, #1
	str	r0, [sp, #84]
	cmp	r0, #188
	beq.n	.L_08190ad6
	bl	.L_0819007c
.L_08190ad6:
	ldr	r1, [sp, #92]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	sub_08020048
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #40]
	movs	r5, #0
	str	r3, [r4, #72]
	bl	sub_08118118
	movs	r3, #192
	ldr	r0, [sp, #64]
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	str	r5, [r0, #16]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r3, r1
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	sub_08118040
	ldr	r0, [pc, #96]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #220
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffffd000
	.4byte 0x08197410
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02014000
	.4byte 0x08199364
	.4byte 0x08199ee0
	.4byte 0x08199ecc
	.4byte 0x08199ed6
	.4byte 0x081991e0
	.4byte 0x081992b0
	.4byte 0x08199f1a
	.4byte 0x08199f22
	.4byte 0x08199f0a
	.4byte 0x08199eea
	.4byte 0x08199efa
	.4byte 0x081991f0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	str	r0, [sp, #60]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r7, #216
	str	r0, [sp, #56]
	movs	r0, #0
	ldr	r1, [r3, #96]
	lsls	r7, r7, #6
	str	r1, [sp, #52]
	adds	r7, #16
	ldr	r3, [r3, #100]
	str	r3, [sp, #48]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r1, [sp, #48]
	ldr	r0, [pc, #52]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #56]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #56]
	ldr	r0, [pc, #28]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	b.n	.L_08190bf0
	.4byte 0x00001010
	.4byte 0x00000134
	.4byte 0x00000102
	.2byte 0x00cd
	.2byte 0x0000
.L_08190bf0:
	lsls	r1, r1, #7
	adds	r2, r6, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #24]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08190c20
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_08190c28
	.2byte 0x3001
	.2byte 0x0814
.L_08190c20:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_08190c28:
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	adds	r5, #176
	str	r3, [sp, #76]
	ldr	r3, [sp, #60]
	movs	r6, #0
	ldr	r0, [r3, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r3, [pc, #160]
	str	r0, [sp, #44]
	mov	r7, sp
	ldr	r5, [r5, #0]
	mov	r0, sp
	str	r5, [sp, #40]
	mov	r1, sp
	ldrh	r3, [r3, #4]
	adds	r7, #112
	adds	r0, #124
	adds	r1, #68
	str	r3, [sp, #36]
	str	r7, [sp, #32]
	str	r0, [sp, #28]
	str	r1, [sp, #24]
	mov	fp, r6
.L_08190c5e:
	ldr	r2, [sp, #60]
	ldr	r1, [sp, #32]
	ldr	r0, [r2, #8]
	bl	sub_0815e20c
	ldr	r6, [sp, #60]
	ldr	r1, [sp, #28]
	movs	r3, #36
	ldrsh	r0, [r6, r3]
	mov	r7, fp
	bl	sub_0815e20c
	cmp	r7, #0
	bne.n	.L_08190cc8
	movs	r1, #240
	ldr	r0, [sp, #52]
	ldr	r3, [pc, #100]
	lsls	r1, r1, #6
	ldr	r2, [pc, #100]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4818
	ldr	r1, [pc, #100]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #92]
	movs	r2, #0
	movs	r3, #0
	ldr	r1, [pc, #92]
	bl	sub_08157cf4
	bl	sub_0815b410
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #56]
	movs	r0, #0
	movs	r2, #1
	mov	r8, r0
	negs	r2, r2
	adds	r3, #24
.L_08190cba:
	movs	r1, #1
	add	r8, r1
	mov	r6, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_08190cba
.L_08190cc8:
	mov	r7, fp
	cmp	r7, #2
	bne.n	.L_08190d58
	ldr	r1, [sp, #60]
	add	r0, sp, #84
	ldr	r3, [r1, #8]
	ldr	r5, [pc, #4]
	b.n	.L_08190cfc
	.4byte 0x00001010
	.4byte 0x0000001f
	.4byte 0x03001120
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x000000c2
	.4byte 0x02010000
	.4byte 0x000000e9
	.2byte 0x4000
	.2byte 0x0201
.L_08190cfc:
	strh	r3, [r0, #0]
	ldrh	r3, [r1, #36]
	movs	r1, #0
	strh	r3, [r0, #2]
	movs	r3, #255
	strh	r3, [r0, #4]
	bl	sub_08118010
	movs	r2, #0
	movs	r0, #1
	ldr	r1, [pc, #812]
	bl	sub_08118040
	bl	sub_0817d6c4
	movs	r4, #160
	lsls	r4, r4, #19
	movs	r2, #0
	adds	r4, #192
	mov	r8, r2
.L_08190d24:
	ldrh	r3, [r4, #0]
	movs	r0, #31
	ands	r0, r3
	lsls	r3, r3, #16
	lsrs	r1, r3, #26
	ands	r1, r5
	cmp	r0, #31
	ble.n	.L_08190d36
	movs	r0, #31
.L_08190d36:
	cmp	r1, #31
	ble.n	.L_08190d3c
	movs	r1, #31
.L_08190d3c:
	lsrs	r3, r0, #1
	lsls	r3, r3, #5
	lsls	r2, r0, #10
	orrs	r2, r3
	movs	r3, #1
	add	r8, r3
	orrs	r2, r1
	mov	r6, r8
	strh	r2, [r4, #0]
	adds	r4, #2
	cmp	r6, #128
	bne.n	.L_08190d24
	ldr	r7, [sp, #40]
	str	r3, [r7, #16]
.L_08190d58:
	mov	r0, fp
	cmp	r0, #1
	ble.n	.L_08190d66
	ldr	r2, [pc, #740]
	ldrh	r3, [r2, #4]
	adds	r3, #8
	strh	r3, [r2, #4]
.L_08190d66:
	mov	r1, fp
	cmp	r1, #8
	bne.n	.L_08190da6
	movs	r1, #240
	ldr	r2, [pc, #728]
	ldr	r3, [pc, #728]
	ldr	r0, [sp, #52]
	lsls	r1, r1, #6
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9d0e
	movs	r2, #0
	mov	r8, r2
.L_08190d80:
	bl	sub_08014878
	movs	r1, #216
	bl	sub_0800206c
	subs	r0, #96
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r1, #216
	bl	sub_0800206c
	movs	r3, #1
	add	r8, r3
	mov	r6, r8
	str	r0, [r5, #4]
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L_08190d80
.L_08190da6:
	mov	r7, fp
	cmp	r7, #86
	bne.n	.L_08190dbe
	ldr	r2, [sp, #56]
	movs	r3, #156
	lsls	r3, r3, #5
	adds	r1, r2, r3
	ldr	r0, [pc, #664]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_08190dbe:
	mov	r6, fp
	cmp	r6, #84
	bne.n	.L_08190dea
	movs	r0, #212
	bl	sub_081c0010
	movs	r1, #2
	ldr	r0, [sp, #44]
	bl	sub_08020090
	movs	r1, #48
	ldr	r0, [sp, #44]
	bl	sub_08020098
	ldr	r7, [sp, #60]
	movs	r3, #80
	movs	r2, #36
	ldrsh	r1, [r7, r2]
	ldr	r0, [r7, #8]
	movs	r2, #7
	bl	sub_08157530
.L_08190dea:
	mov	r3, fp
	cmp	r3, #90
	bne.n	.L_08190eca
	movs	r1, #240
	ldr	r3, [pc, #600]
	ldr	r0, [sp, #52]
	lsls	r1, r1, #6
	ldr	r2, [pc, #588]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2090
	bl	sub_081c0010
	ldr	r7, [sp, #60]
	movs	r3, #16
	movs	r6, #36
	ldrsh	r0, [r7, r6]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #238
	ldr	r0, [sp, #56]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r3, [sp, #28]
	ldr	r6, [pc, #552]
	movs	r2, #0
	mov	r8, r2
	mov	sl, r3
.L_08190e30:
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	adds	r7, r3, #0
	bl	sub_08014878
	movs	r3, #254
	adds	r5, r0, #0
	ldr	r0, [sp, #60]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r5, r3
	ldr	r3, [r0, #4]
	adds	r7, #32
	cmp	r3, #1
	bne.n	.L_08190e5c
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r5, r5, r1
.L_08190e5c:
	mov	r2, sl
	ldr	r3, [r2, #0]
	mov	r0, sl
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r0, #4]
	adds	r0, r5, #0
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #5
	str	r3, [r6, #16]
	bl	sub_08014878
	str	r0, [r6, #8]
	bl	sub_08014878
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r6, #20]
	bl	sub_08014878
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	movs	r2, #128
	ands	r3, r0
	lsls	r2, r2, #6
	adds	r3, r3, r2
	str	r3, [r6, #24]
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r6, #28
	cmp	r7, #128
	bne.n	.L_08190e30
.L_08190eca:
	mov	r0, fp
	cmp	r0, #85
	ble.n	.L_08190f62
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08190ef0
	movs	r3, #176
	lsls	r3, r3, #4
	lsls	r2, r0, #5
	adds	r3, #56
	subs	r5, r3, r2
	movs	r2, #38
	negs	r2, r2
	cmp	r5, r2
	bge.n	.L_08190efe
	movs	r5, #38
	negs	r5, r5
	b.n	.L_08190efe
.L_08190ef0:
	ldr	r7, [pc, #356]
	mov	r6, fp
	lsls	r3, r6, #5
	adds	r5, r3, r7
	cmp	r5, #0
	ble.n	.L_08190efe
	movs	r5, #0
.L_08190efe:
	movs	r3, #158
	ldr	r2, [sp, #56]
	str	r3, [sp, #0]
	movs	r3, #56
	str	r3, [sp, #4]
	movs	r3, #156
	lsls	r3, r3, #5
	adds	r1, r2, r3
	ldr	r4, [sp, #76]
	movs	r3, #8
	ldr	r0, [sp, #52]
	adds	r2, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e0f
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne.n	.L_08190f2c
	movs	r0, #188
	movs	r1, #27
	bl	sub_081963ec
	b.n	.L_08190f34
.L_08190f2c:
	movs	r0, #188
	movs	r1, #31
	bl	sub_081963ec
.L_08190f34:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	add	r0, sp, #76
	str	r3, [r0, #4]
	movs	r3, #158
	str	r3, [sp, #0]
	movs	r3, #56
	str	r3, [sp, #4]
	ldr	r7, [sp, #56]
	movs	r2, #156
	lsls	r2, r2, #5
	ldr	r4, [r0, #4]
	adds	r1, r7, r2
	ldr	r0, [sp, #52]
	adds	r2, r5, #0
	movs	r3, #64
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_08190f62:
	mov	r3, fp
	cmp	r3, #89
	bgt.n	.L_08190fbe
	movs	r6, #0
	mov	r8, r6
	ldr	r5, [sp, #56]
	ldr	r6, [pc, #236]
.L_08190f70:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08190fb2
	asrs	r0, r0, #2
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #48]
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
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #76]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08190fb2:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_08190f70
.L_08190fbe:
	mov	r6, fp
	lsls	r3, r6, #2
	add	r3, fp
	lsls	r3, r3, #1
	movs	r2, #0
	subs	r3, #240
	movs	r7, #24
	str	r3, [sp, #16]
	str	r7, [sp, #12]
	str	r2, [sp, #8]
	mov	r8, r2
.L_08190fd4:
	mov	r0, r8
	lsls	r5, r0, #4
	adds	r3, r5, #0
	adds	r3, #28
	cmp	fp, r3
	bne.n	.L_08190fe6
	movs	r0, #212
	bl	sub_081c0010
.L_08190fe6:
	adds	r3, r5, #0
	adds	r3, #32
	cmp	fp, r3
	bne.n	.L_081910de
	ldr	r6, [sp, #8]
	ldr	r2, [sp, #28]
	lsls	r3, r6, #3
	ldr	r7, [sp, #56]
	subs	r3, r3, r6
	movs	r1, #0
	lsls	r3, r3, #2
	mov	r9, r1
	mov	sl, r2
	adds	r6, r3, r7
.L_08191002:
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r7, r3, #0
	bl	sub_08014878
	movs	r3, #252
	adds	r5, r0, #0
	ldr	r0, [sp, #60]
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r5, r3
	ldr	r3, [r0, #4]
	adds	r7, #128
	cmp	r3, #0
	bne.n	.L_08191032
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0819106a
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r5, r5, r2
	b.n	.L_0819106a
.L_08191032:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_08191064
	ldr	r0, [pc, #36]
	adds	r5, r5, r0
	b.n	.L_0819106a
	movs	r0, r0
	.4byte 0x00000072
	.4byte 0x03001120
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x00000101
	.4byte 0x02016000
	.4byte 0xfffff4a2
	.4byte 0x08197410
	.2byte 0xc000
	.2byte 0xffff
.L_08191064:
	.2byte 0x2180
	lsls	r1, r1, #8
	adds	r5, r5, r1
.L_0819106a:
	mov	r2, sl
	ldr	r3, [r2, #0]
	mov	r0, sl
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r0, #4]
	adds	r0, r5, #0
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #5
	str	r3, [r6, #16]
	bl	sub_08014878
	movs	r3, #7
	movs	r1, #1
	ands	r3, r0
	add	r9, r1
	adds	r3, #17
	mov	r2, r9
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r2, #16
	bne.n	.L_08191002
	movs	r0, #133
	bl	sub_081c0010
	ldr	r6, [sp, #60]
	movs	r2, #5
	movs	r3, #36
	ldrsh	r0, [r6, r3]
	movs	r3, #64
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	movs	r0, #238
	ldr	r7, [sp, #56]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r7, r0
	movs	r3, #4
	str	r3, [r2, #0]
.L_081910de:
	ldr	r1, [sp, #12]
	cmp	fp, r1
	blt.n	.L_081911b2
	mov	r2, fp
	cmp	r2, #89
	bgt.n	.L_081911b2
	subs	r1, r2, r1
	cmp	r1, #8
	bgt.n	.L_08191138
	ldr	r3, [sp, #60]
	ldr	r4, [r3, #4]
	cmp	r4, #0
	bne.n	.L_0819110c
	ldr	r6, [sp, #28]
	ldr	r7, [sp, #16]
	ldr	r3, [r6, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, r3, r7
	adds	r6, r3, #0
	adds	r6, #80
	b.n	.L_08191122
.L_0819110c:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r2, r1, #2
	adds	r2, r2, r1
	asrs	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r6, r3, #0
	subs	r6, #80
.L_08191122:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08191130
	lsls	r3, r1, #4
	adds	r5, r3, #0
	subs	r5, #120
	b.n	.L_0819115a
.L_08191130:
	lsls	r2, r1, #4
	movs	r3, #168
	subs	r5, r3, r2
	b.n	.L_0819115a
.L_08191138:
	ldr	r3, [sp, #60]
	ldr	r4, [r3, #4]
	cmp	r4, #0
	bne.n	.L_08191146
	ldr	r6, [sp, #28]
	ldr	r3, [r6, #0]
	b.n	.L_0819114a
.L_08191146:
	ldr	r7, [sp, #28]
	ldr	r3, [r7, #0]
.L_0819114a:
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r6, r3, #1
	mov	r0, r8
	movs	r5, #8
	cmp	r0, #0
	beq.n	.L_0819115a
	movs	r5, #40
.L_0819115a:
	mov	r1, r8
	add	r0, sp, #76
	cmp	r1, #1
	bne.n	.L_08191184
	cmp	r4, #0
	bne.n	.L_08191170
	movs	r0, #188
	movs	r1, #27
	bl	sub_081963ec
	b.n	.L_08191178
.L_08191170:
	movs	r0, #188
	movs	r1, #31
	bl	sub_081963ec
.L_08191178:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	add	r0, sp, #76
	str	r3, [r0, #4]
.L_08191184:
	movs	r3, #40
	str	r3, [sp, #0]
	movs	r3, #80
	str	r3, [sp, #4]
	mov	r2, r8
	ldr	r3, [sp, #56]
	lsls	r1, r2, #2
	adds	r2, r6, #0
	movs	r6, #224
	lsls	r6, r6, #3
	ldr	r4, [r1, r0]
	subs	r2, #20
	adds	r1, r3, r6
	ldr	r0, [sp, #52]
	adds	r3, r5, #0
	mov	r7, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f01
	bne.n	.L_081911b2
	movs	r0, #188
	bl	sub_0801314c
.L_081911b2:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	movs	r3, #1
	add	r8, r3
	subs	r0, #160
	adds	r1, #16
	adds	r2, #32
	mov	r6, r8
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	cmp	r6, #2
	beq.n	.L_081911d0
	b.n	.L_08190fd4
.L_081911d0:
	movs	r0, #32
	bl	sub_08014dac
	mov	r9, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #792]
	ldr	r3, [sp, #68]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #788]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #68]
	ldr	r3, [pc, #776]
	adds	r7, r0, #0
	ldr	r2, [sp, #24]
	ldr	r0, [pc, #776]
	movs	r6, #0
	str	r3, [r7, #8]
	mov	r3, r9
	str	r0, [r2, #4]
	str	r1, [r7, #0]
	str	r2, [r7, #16]
	str	r3, [r7, #12]
	str	r6, [sp, #20]
	mov	r8, r6
	mov	sl, r6
.L_08191210:
	ldr	r6, [sp, #20]
	adds	r6, #2
	cmp	fp, r6
	bne.n	.L_0819121e
	movs	r0, #212
	bl	sub_081c0010
.L_0819121e:
	cmp	fp, r6
	blt.n	.L_08191292
	ldr	r3, [sp, #20]
	adds	r3, #18
	cmp	fp, r3
	bge.n	.L_08191292
	mov	r0, fp
	subs	r5, r0, r6
	lsls	r2, r5, #3
	movs	r3, #64
	subs	r3, r3, r2
	cmp	r3, #0
	ble.n	.L_0819123a
	movs	r3, #0
.L_0819123a:
	movs	r1, #64
	negs	r1, r1
	cmp	r3, r1
	ble.n	.L_08191292
	movs	r2, #128
	lsls	r2, r2, #7
	str	r3, [r7, #20]
	lsls	r5, r5, #14
	adds	r5, r5, r2
	bl	sub_08014de4
	ldr	r3, [sp, #32]
	movs	r1, #0
	ldr	r0, [r3, #0]
	movs	r2, #0
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r0, r0, #16
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r0, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r2, [pc, #660]
	mov	r6, sl
	ldrsh	r0, [r6, r2]
	bl	sub_08015024
	ldr	r3, [pc, #656]
	ldrsh	r0, [r3, r6]
	bl	sub_08015068
	ldr	r0, [pc, #652]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08191292:
	mov	r2, r8
	lsls	r5, r2, #3
	adds	r3, r5, #0
	adds	r6, r5, #0
	adds	r3, #68
	adds	r6, #64
	cmp	fp, r3
	bne.n	.L_081912a8
	movs	r0, #212
	bl	sub_081c0010
.L_081912a8:
	adds	r3, r5, #0
	adds	r3, #72
	cmp	fp, r3
	bne.n	.L_081912c6
	ldr	r3, [sp, #60]
	movs	r1, #0
	movs	r2, #1
	ldr	r0, [r3, #8]
	negs	r2, r2
	str	r1, [sp, #0]
	movs	r3, #0
	movs	r1, #7
	bl	sub_0814cd48
	b.n	.L_081912e0
.L_081912c6:
	adds	r3, r5, #0
	adds	r3, #76
	cmp	fp, r3
	bne.n	.L_081912e0
	ldr	r2, [sp, #60]
	movs	r3, #0
	ldr	r0, [r2, #8]
	movs	r2, #1
	movs	r1, #0
	negs	r2, r2
	str	r3, [sp, #0]
	bl	sub_0814cd48
.L_081912e0:
	cmp	fp, r6
	blt.n	.L_08191344
	adds	r3, r6, #0
	adds	r3, #11
	cmp	fp, r3
	bge.n	.L_08191344
	mov	r0, fp
	subs	r3, r0, r6
	movs	r1, #0
	movs	r5, #192
	lsls	r3, r3, #14
	str	r1, [r7, #20]
	lsls	r5, r5, #10
	subs	r5, r5, r3
	bl	sub_08014de4
	ldr	r2, [sp, #32]
	movs	r1, #0
	ldr	r0, [r2, #0]
	movs	r2, #0
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r0, r0, #16
	bl	sub_08015160
	adds	r2, r5, #0
	lsls	r1, r5, #1
	adds	r0, r5, #0
	bl	sub_080151e4
	ldr	r1, [pc, #484]
	mov	r3, sl
	ldrsh	r0, [r3, r1]
	bl	sub_08015024
	ldr	r3, [pc, #480]
	mov	r2, sl
	ldrsh	r0, [r3, r2]
	bl	sub_08015068
	ldr	r0, [pc, #472]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08191344:
	mov	r0, r8
	lsls	r3, r0, #4
	adds	r6, r3, #0
	adds	r6, #32
	cmp	fp, r6
	blt.n	.L_081913c0
	adds	r3, #48
	cmp	fp, r3
	bge.n	.L_081913c0
	mov	r1, fp
	subs	r5, r1, r6
	lsls	r2, r5, #3
	movs	r3, #64
	subs	r3, r3, r2
	cmp	r3, #0
	ble.n	.L_08191366
	movs	r3, #0
.L_08191366:
	movs	r2, #64
	negs	r2, r2
	cmp	r3, r2
	ble.n	.L_081913c0
	str	r3, [r7, #20]
	bl	sub_08014de4
	ldr	r1, [sp, #28]
	movs	r6, #128
	ldr	r0, [r1, #0]
	lsls	r6, r6, #7
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	lsls	r5, r5, #14
	subs	r0, #64
	adds	r5, r5, r6
	lsls	r0, r0, #16
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	lsls	r1, r5, #1
	adds	r0, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r6, [pc, #360]
	mov	r2, sl
	ldrsh	r0, [r2, r6]
	bl	sub_08015024
	ldr	r3, [pc, #356]
	mov	r1, sl
	ldrsh	r0, [r3, r1]
	bl	sub_08015068
	ldr	r0, [pc, #348]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081913c0:
	ldr	r6, [sp, #20]
	movs	r0, #1
	add	r8, r0
	movs	r3, #2
	adds	r6, #6
	mov	r1, r8
	add	sl, r3
	str	r6, [sp, #20]
	cmp	r1, #2
	beq.n	.L_081913d6
	b.n	.L_08191210
.L_081913d6:
	mov	r3, fp
	movs	r1, #1
	movs	r2, #3
	cmp	r3, #89
	ble.n	.L_0819147a
	ldr	r3, [pc, #304]
	mov	r6, fp
	str	r3, [r7, #8]
	str	r2, [r7, #0]
	cmp	r6, #99
	ble.n	.L_0819140c
	ldr	r0, [sp, #56]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r0, r2
	str	r1, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	ldr	r3, [pc, #280]
	movs	r0, #2
	str	r3, [r2, #0]
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164a4c
.L_0819140c:
	movs	r6, #0
	ldr	r5, [pc, #268]
	mov	r8, r6
	mov	r6, sp
	adds	r6, #67
.L_08191416:
	movs	r2, #7
	mov	r3, r8
	ands	r3, r2
	adds	r3, #32
	strb	r3, [r6, #0]
	mov	r3, sp
	adds	r3, #67
	adds	r6, r3, #0
	str	r6, [r7, #20]
	bl	sub_08014de4
	ldr	r3, [pc, #240]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	ldr	r2, [r5, #24]
	lsls	r0, r2, #1
	adds	r0, r0, r2
	lsls	r1, r0, #1
	bl	sub_080151e4
	ldr	r0, [r5, #8]
	bl	sub_08015024
	ldr	r0, [r5, #8]
	bl	sub_08015068
	mov	r1, r9
	movs	r2, #3
	ldr	r0, [pc, #200]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138058
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #128
	bne.n	.L_08191416
.L_0819147a:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r9
	bl	sub_08013164
	movs	r0, #8
	bl	sub_08158d68
	bl	sub_081434f8
	movs	r6, #240
	ldr	r3, [sp, #56]
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r2, r3, r6
	movs	r7, #1
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	fp, r7
	bl	sub_08013560
	mov	r0, fp
	cmp	r0, #114
	beq.n	.L_081914b2
	bl	.L_08190c5e
.L_081914b2:
	bl	sub_08118118
	add	r2, sp, #36
	ldr	r3, [pc, #108]
	ldrh	r2, [r2, #0]
	ldr	r1, [sp, #40]
	strh	r2, [r3, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r5, #0
	ldr	r3, [r3, #36]
	str	r5, [r1, #16]
	movs	r6, #206
	lsls	r6, r6, #3
	adds	r3, r3, r6
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	sub_08118040
	ldr	r0, [pc, #80]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0x02010000
	.4byte 0x08199f2a
	.4byte 0x08199f2e
	.4byte 0x081991e0
	.4byte 0x0819919c
	.4byte 0x02020202
	.4byte 0x02016000
	.4byte 0xffc00000
	.4byte 0x081991a4
	.4byte 0x03001120
	.4byte 0x08143001
