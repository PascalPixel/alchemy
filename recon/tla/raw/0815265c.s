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
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118070, 0x08118070
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_081504b4, 0x081504b4
	.set sub_081504c0, 0x081504c0
	.set sub_08152c40, 0x08152c40
	.set sub_08153080, 0x08153080
	.set sub_081530a0, 0x081530a0
	.set sub_081534f8, 0x081534f8
	.set sub_0815585c, 0x0815585c
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e1fc, 0x0815e1fc
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Func_0815265c
	.thumb_func
Func_0815265c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #252
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	mov	fp, r1
	str	r0, [sp, #80]
	ldr	r1, [r3, #92]
	str	r1, [sp, #76]
	ldr	r3, [r3, #100]
	str	r3, [sp, #68]
	mov	r3, fp
	subs	r3, #10
	cmp	r3, #1
	bls.n	.L_081526a2
	mov	r2, fp
	cmp	r2, #13
	beq.n	.L_081526a2
	cmp	r2, #14
	beq.n	.L_081526a2
	cmp	r2, #15
	beq.n	.L_081526a2
	cmp	r2, #17
	beq.n	.L_081526a2
	cmp	r2, #18
	beq.n	.L_081526a2
	cmp	r2, #19
	bne.n	.L_081526aa
.L_081526a2:
	movs	r0, #1
	bl	sub_081435e0
	b.n	.L_081526b0
.L_081526aa:
	movs	r0, #0
	bl	sub_081435e0
.L_081526b0:
	ldr	r5, [sp, #84]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L_08152726
	mov	r6, fp
	cmp	r6, #9
	beq.n	.L_081526c6
	cmp	r6, #22
	beq.n	.L_081526c6
	cmp	r6, #24
	bne.n	.L_081526f4
.L_081526c6:
	ldr	r0, [sp, #84]
	movs	r3, #66
	ldr	r1, [r0, #4]
	add	r2, sp, #240
	lsls	r1, r1, #4
	orrs	r1, r3
	add	r3, sp, #228
	bl	sub_0815585c
	movs	r3, #216
	ldr	r2, [sp, #76]
	lsls	r3, r3, #7
	adds	r3, #192
	adds	r1, r2, r3
	ldr	r0, [pc, #12]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	b.n	.L_0815271c
	movs	r0, r0
	.2byte 0x0151
	.2byte 0x0000
.L_081526f4:
	ldr	r5, [sp, #84]
	movs	r3, #64
	ldr	r1, [r5, #4]
	add	r2, sp, #240
	lsls	r1, r1, #4
	orrs	r1, r3
	adds	r0, r5, #0
	add	r3, sp, #228
	bl	sub_0815585c
	movs	r2, #216
	ldr	r6, [sp, #76]
	lsls	r2, r2, #7
	adds	r2, #192
	adds	r1, r6, r2
	ldr	r0, [pc, #64]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
.L_0815271c:
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
.L_08152726:
	movs	r3, #0
	ldr	r0, [pc, #44]
	ldr	r1, [sp, #68]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r6, #128
	ldr	r3, [pc, #24]
	lsls	r6, r6, #19
	adds	r6, #82
	strh	r3, [r6, #0]
	ldr	r5, [sp, #84]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L_08152792
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0815276c
	b.n	.L_0815275c
	.4byte 0x00000785
	.4byte 0x00001010
	.4byte 0x00000157
	.2byte 0x0134
	.2byte 0x0000
.L_0815275c:
	mov	r6, sp
	adds	r6, #88
	movs	r0, #0
	adds	r1, r6, #0
	str	r6, [sp, #36]
	bl	sub_08144aac
	b.n	.L_08152808
.L_0815276c:
	movs	r1, #23
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #7
	str	r3, [sp, #88]
	bl	sub_081963ec
	mov	r0, sp
	adds	r0, #88
	str	r0, [sp, #36]
	adds	r5, #188
	ldr	r3, [r5, #0]
	str	r3, [r0, #4]
	b.n	.L_08152808
.L_08152792:
	mov	r1, fp
	cmp	r1, #20
	bne.n	.L_081527c8
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #39
	str	r3, [sp, #88]
	bl	sub_081963ec
	mov	r2, sp
	adds	r2, #88
	str	r2, [sp, #36]
	adds	r5, #188
	ldr	r3, [r5, #0]
	str	r3, [r2, #4]
	ldr	r3, [pc, #4]
	strh	r3, [r6, #0]
	b.n	.L_08152808
	movs	r0, r0
	.2byte 0x0310
	.2byte 0x0000
.L_081527c8:
	mov	r3, fp
	cmp	r3, #7
	beq.n	.L_081527d2
	cmp	r3, #21
	bne.n	.L_081527f8
.L_081527d2:
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #23
	str	r3, [sp, #88]
	bl	sub_081963ec
	mov	r6, sp
	adds	r6, #88
	str	r6, [sp, #36]
	adds	r5, #188
	ldr	r3, [r5, #0]
	str	r3, [r6, #4]
	b.n	.L_08152808
.L_081527f8:
	ldr	r1, [sp, #84]
	mov	r2, sp
	adds	r2, #88
	ldr	r0, [r1, #4]
	adds	r1, r2, #0
	str	r2, [sp, #36]
	bl	sub_08144aac
.L_08152808:
	ldr	r3, [sp, #76]
	movs	r6, #224
	lsls	r6, r6, #3
	adds	r5, r3, r6
	ldr	r0, [pc, #308]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	mov	r0, fp
	cmp	r0, #5
	beq.n	.L_08152826
	cmp	r0, #23
	bne.n	.L_08152834
.L_08152826:
	ldr	r2, [sp, #76]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #280]
	b.n	.L_081528aa
.L_08152834:
	mov	r6, fp
	cmp	r6, #9
	beq.n	.L_0815283e
	cmp	r6, #17
	bne.n	.L_08152884
.L_0815283e:
	ldr	r0, [pc, #272]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #0
	mov	r9, r0
	movs	r0, #192
	movs	r4, #1
	lsls	r0, r0, #4
	adds	r1, r5, #0
.L_08152856:
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L_08152862
	strb	r2, [r1, #0]
	b.n	.L_0815286e
.L_08152862:
	cmp	r2, #16
	bhi.n	.L_0815286a
	strb	r4, [r1, #0]
	b.n	.L_0815286e
.L_0815286a:
	adds	r3, #240
	strb	r3, [r1, #0]
.L_0815286e:
	movs	r2, #1
	add	r9, r2
	adds	r1, #1
	cmp	r9, r0
	bne.n	.L_08152856
	mov	r3, fp
	cmp	r3, #17
	beq.n	.L_08152880
	b.n	sub_08152c40
.L_08152880:
	ldr	r0, [pc, #208]
	b.n	.L_08152a64
.L_08152884:
	mov	r6, fp
	cmp	r6, #20
	bne.n	.L_08152898
	ldr	r2, [sp, #76]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #192]
	b.n	.L_081528aa
.L_08152898:
	mov	r6, fp
	cmp	r6, #7
	bne.n	.L_081528b4
	ldr	r2, [sp, #76]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #176]
.L_081528aa:
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	sub_08152c40
.L_081528b4:
	mov	r6, fp
	cmp	r6, #22
	bne.n	.L_081528d2
	ldr	r2, [sp, #76]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #148]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #132]
	b.n	.L_08152a64
.L_081528d2:
	mov	r6, fp
	cmp	r6, #21
	bne.n	.L_0815292e
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #19
	mov	r9, r0
	movs	r4, #15
	movs	r0, #31
	adds	r1, #2
.L_081528e6:
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_081528ee
	adds	r2, #3
.L_081528ee:
	mov	r5, r9
	lsrs	r3, r5, #31
	add	r3, r9
	asrs	r3, r3, #1
	asrs	r2, r2, #2
	subs	r3, r0, r3
	movs	r6, #1
	subs	r2, r4, r2
	lsls	r3, r3, #10
	add	r9, r6
	orrs	r3, r2
	mov	r2, r9
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #64
	bne.n	.L_081528e6
	ldr	r3, [sp, #76]
	movs	r5, #152
	lsls	r5, r5, #5
	adds	r5, #86
	adds	r1, r3, r5
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #60]
	bl	sub_08157cf4
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	b.n	sub_08152c40
.L_0815292e:
	mov	r6, fp
	cmp	r6, #18
	bne.n	.L_08152964
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #36]
	bl	sub_08157cf4
	ldr	r0, [pc, #16]
	b.n	.L_08152a64
	.4byte 0x00000410
	.4byte 0x00000192
	.4byte 0x0000011c
	.4byte 0x000000d7
	.4byte 0x00000148
	.4byte 0x0000014d
	.4byte 0x00000116
	.2byte 0x016e
	.2byte 0x0000
.L_08152964:
	mov	r0, fp
	cmp	r0, #19
	bne.n	.L_0815297a
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #344]
	bl	sub_08157cf4
	ldr	r0, [pc, #344]
	b.n	.L_08152a64
.L_0815297a:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L_081529ec
	ldr	r2, [sp, #76]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #320]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r5, #0
	str	r5, [sp, #20]
	movs	r7, #128
	mov	r9, r5
	lsls	r7, r7, #2
.L_0815299e:
	movs	r5, #0
	cmp	r7, #0
	beq.n	.L_081529ce
	ldr	r6, [sp, #20]
	movs	r0, #0
	mov	sl, r0
	mov	r8, r7
.L_081529ac:
	bl	sub_08014878
	movs	r1, #144
	lsls	r1, r1, #3
	bl	sub_0800206c
	movs	r1, #152
	lsls	r1, r1, #5
	ldr	r2, [sp, #76]
	adds	r0, r6, r0
	adds	r1, #86
	adds	r0, r0, r1
	mov	r3, sl
	adds	r5, #1
	strb	r3, [r2, r0]
	cmp	r5, r8
	bne.n	.L_081529ac
.L_081529ce:
	ldr	r5, [sp, #20]
	movs	r6, #144
	movs	r1, #1
	lsls	r6, r6, #3
	movs	r0, #128
	add	r9, r1
	adds	r5, r5, r6
	lsls	r0, r0, #1
	mov	r2, r9
	str	r5, [sp, #20]
	adds	r7, r7, r0
	cmp	r2, #6
	bne.n	.L_0815299e
	ldr	r0, [pc, #228]
	b.n	.L_08152a64
.L_081529ec:
	mov	r3, fp
	cmp	r3, #12
	beq.n	.L_081529f6
	cmp	r3, #16
	bne.n	.L_08152a78
.L_081529f6:
	ldr	r5, [sp, #76]
	movs	r6, #224
	lsls	r6, r6, #3
	adds	r1, r5, r6
	ldr	r0, [pc, #212]
	movs	r2, #1
	movs	r3, #1
	movs	r5, #0
	bl	sub_08157cf4
	movs	r7, #0
	mov	ip, r5
	movs	r6, #0
.L_08152a10:
	ldr	r0, [sp, #76]
	ldr	r2, [sp, #76]
	adds	r3, r6, r5
	lsls	r3, r3, #4
	movs	r1, #184
	adds	r3, r3, r0
	lsls	r1, r1, #5
	adds	r0, r3, r1
	adds	r3, r7, r2
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r2, #24
	adds	r1, r3, r2
	ldr	r2, [sp, #76]
	movs	r3, #224
	add	r2, ip
	lsls	r3, r3, #3
	movs	r4, #0
	adds	r2, r2, r3
.L_08152a36:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r4, #24
	bne.n	.L_08152a36
	movs	r0, #24
	adds	r5, #1
	add	ip, r0
	adds	r7, #48
	adds	r6, #2
	cmp	r5, #24
	bne.n	.L_08152a10
	mov	r1, fp
	cmp	r1, #12
	bne.n	.L_08152a62
	ldr	r0, [pc, #120]
	b.n	.L_08152a64
.L_08152a62:
	ldr	r0, [pc, #120]
.L_08152a64:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe0e3
.L_08152a78:
	ldr	r2, [sp, #76]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #96]
	movs	r2, #1
	movs	r3, #1
	mov	r5, fp
	bl	sub_08157cf4
	cmp	r5, #6
	bne.n	.L_08152ae8
	movs	r6, #0
	movs	r0, #160
	mov	r9, r6
	lsls	r0, r0, #19
.L_08152a9a:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_08152aa2
	adds	r3, #3
.L_08152aa2:
	asrs	r3, r3, #2
	lsls	r1, r3, #5
	lsls	r2, r3, #10
	orrs	r2, r1
	movs	r1, #1
	orrs	r2, r3
	add	r9, r1
	strh	r2, [r0, #0]
	mov	r2, r9
	adds	r0, #2
	cmp	r2, #64
	bne.n	.L_08152a9a
	movs	r2, #128
	ldr	r3, [pc, #8]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	b.n	sub_08152c40
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000161
	.4byte 0x00000148
	.4byte 0x00000190
	.4byte 0x00000163
	.4byte 0x0000017d
	.4byte 0x03000730
	.2byte 0x013e
	.2byte 0x0000
.L_08152ae8:
	mov	r3, fp
	cmp	r3, #16
	bls.n	.L_08152af0
	b.n	.L_08152c28
.L_08152af0:
	ldr	r2, [pc, #72]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08152be8
	.4byte 0x08152bec
	.4byte 0x08152bf0
	.4byte 0x08152bf4
	.4byte 0x08152c28
	.4byte 0x08152c28
	.4byte 0x08152c28
	.4byte 0x08152c28
	.4byte 0x08152bf8
	.4byte 0x08152c28
	.4byte 0x08152bfc
	.4byte 0x08152c24
	.4byte 0x08152c28
	.4byte 0x08152b40
	.4byte 0x08152b40
	.4byte 0x08152b40
	.4byte 0x08152bf0
	.2byte 0x2af8
	.2byte 0x0815
	movs	r5, #0
	movs	r7, #128
	mov	r9, r5
	lsls	r7, r7, #3
.L_08152b48:
	movs	r5, #0
	cmp	r7, #0
	beq.n	.L_08152b7e
	movs	r2, #224
	mov	r0, r9
	lsls	r2, r2, #3
	lsls	r6, r0, #11
	adds	r2, #255
	movs	r3, #0
	mov	r8, r7
.L_08152b5c:
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	bl	sub_08014878
	ldr	r2, [sp, #16]
	movs	r1, #152
	ands	r0, r2
	lsls	r1, r1, #5
	adds	r1, #86
	adds	r0, r6, r0
	adds	r0, r0, r1
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #76]
	adds	r5, #1
	strb	r3, [r1, r0]
	cmp	r5, r8
	bne.n	.L_08152b5c
.L_08152b7e:
	movs	r3, #1
	movs	r2, #128
	add	r9, r3
	lsls	r2, r2, #2
	mov	r5, r9
	adds	r7, r7, r2
	cmp	r5, #6
	bne.n	.L_08152b48
	mov	r6, fp
	cmp	r6, #15
	bne.n	.L_08152bd0
	movs	r2, #128
	ldr	r3, [pc, #24]
	lsls	r2, r2, #19
	adds	r2, #80
	movs	r0, #0
	strh	r3, [r2, #0]
	mov	r9, r0
	movs	r0, #160
	lsls	r0, r0, #19
.L_08152ba6:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_08152bb4
	adds	r3, #3
	b.n	.L_08152bb4
	.2byte 0x0000
	.2byte 0x0000
.L_08152bb4:
	asrs	r3, r3, #2
	lsls	r1, r3, #5
	lsls	r2, r3, #10
	orrs	r2, r1
	movs	r1, #1
	orrs	r2, r3
	add	r9, r1
	strh	r2, [r0, #0]
	mov	r2, r9
	adds	r0, #2
	cmp	r2, #64
	bne.n	.L_08152ba6
	movs	r1, #0
	b.n	.L_08152c30
.L_08152bd0:
	mov	r3, fp
	cmp	r3, #13
	bne.n	.L_08152be4
	ldr	r2, [pc, #40]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
.L_08152be4:
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	ldr	r0, [pc, #32]
	b.n	.L_08152c2a
	.4byte 0x00000000
	.4byte 0x00000187
	.4byte 0x00000148
	.4byte 0x0000013e
	.4byte 0x0000017d
	.4byte 0x0000012f
	.4byte 0x00000164
	.4byte 0x00000184
	.2byte 0x00b9
	.2byte 0x0000
	ldr	r0, [pc, #620]
	b.n	.L_08152c2a
.L_08152c28:
	ldr	r0, [pc, #620]
.L_08152c2a:
	bl	sub_08013300
	adds	r1, r0, #0
.L_08152c30:
	cmp	r1, #0
	beq.n	sub_08152c40
	movs	r0, #160
	ldr	r3, [pc, #612]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465d
	cmp	r5, #13
	beq.n	.L_08152c4e
	cmp	r5, #15
	beq.n	.L_08152c4e
	cmp	r5, #20
	bne.n	.L_08152c66
.L_08152c4e:
	ldr	r6, [sp, #76]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r6, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r6, r1
	movs	r3, #0
	b.n	.L_08152cb0
.L_08152c66:
	mov	r2, fp
	cmp	r2, #7
	beq.n	.L_08152c80
	cmp	r2, #9
	beq.n	.L_08152c80
	cmp	r2, #17
	beq.n	.L_08152c80
	cmp	r2, #18
	beq.n	.L_08152c80
	cmp	r2, #19
	beq.n	.L_08152c80
	cmp	r2, #24
	bne.n	.L_08152c9a
.L_08152c80:
	ldr	r3, [sp, #76]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r6, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r6, r0
	movs	r3, #50
	b.n	.L_08152cb0
.L_08152c9a:
	ldr	r1, [sp, #76]
	movs	r3, #239
	movs	r5, #238
	lsls	r3, r3, #7
	lsls	r5, r5, #7
	adds	r2, r1, r3
	adds	r5, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r5
	movs	r3, #75
.L_08152cb0:
	str	r3, [r2, #0]
	movs	r1, #200
	ldr	r0, [pc, #488]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r3, [sp, #76]
	movs	r6, #0
	movs	r2, #1
	mov	r9, r6
	negs	r2, r2
	adds	r3, #24
.L_08152cc8:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #64
	bne.n	.L_08152cc8
	mov	r2, fp
	cmp	r2, #8
	beq.n	.L_08152ce4
	cmp	r2, #11
	beq.n	.L_08152ce4
	cmp	r2, #14
	bne.n	.L_08152cea
.L_08152ce4:
	movs	r3, #32
	str	r3, [sp, #64]
	b.n	.L_08152d02
.L_08152cea:
	movs	r5, #64
	mov	r6, fp
	str	r5, [sp, #64]
	cmp	r6, #20
	beq.n	.L_08152d02
	movs	r0, #48
	mov	r1, fp
	str	r0, [sp, #64]
	cmp	r1, #18
	beq.n	.L_08152d02
	movs	r2, #56
	str	r2, [sp, #64]
.L_08152d02:
	ldr	r3, [sp, #84]
	mov	r5, sp
	adds	r5, #216
	ldr	r0, [r3, #8]
	adds	r1, r5, #0
	str	r5, [sp, #60]
	bl	sub_0815e1fc
	ldr	r0, [sp, #84]
	movs	r6, #0
	ldr	r3, [r0, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08152d3c
	add	r5, sp, #120
	movs	r6, #36
.L_08152d22:
	ldr	r1, [sp, #84]
	ldrsh	r0, [r6, r1]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r0, [sp, #84]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r0, #20]
	adds	r5, #12
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08152d22
.L_08152d3c:
	mov	r1, fp
	cmp	r1, #12
	beq.n	.L_08152d46
	cmp	r1, #15
	bne.n	.L_08152d60
.L_08152d46:
	ldr	r2, [sp, #84]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08152d58
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #0]
	subs	r3, #24
	str	r3, [r5, #0]
	b.n	.L_08152d60
.L_08152d58:
	ldr	r6, [sp, #60]
	ldr	r3, [r6, #0]
	adds	r3, #24
	str	r3, [r6, #0]
.L_08152d60:
	mov	r0, fp
	cmp	r0, #20
	bne.n	.L_08152d80
	ldr	r1, [sp, #84]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08152d78
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	subs	r3, #48
	str	r3, [r2, #0]
	b.n	.L_08152d80
.L_08152d78:
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #0]
	adds	r3, #48
	str	r3, [r5, #0]
.L_08152d80:
	mov	r6, fp
	cmp	r6, #21
	bne.n	.L_08152da8
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_08152d98
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #0]
	subs	r3, #16
	str	r3, [r1, #0]
	b.n	.L_08152da0
.L_08152d98:
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	adds	r3, #16
	str	r3, [r2, #0]
.L_08152da0:
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #4]
	subs	r3, #40
	str	r3, [r5, #4]
.L_08152da8:
	mov	r6, fp
	cmp	r6, #16
	bne.n	.L_08152dc8
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_08152dc0
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #0]
	subs	r3, #32
	str	r3, [r1, #0]
	b.n	.L_08152dc8
.L_08152dc0:
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	adds	r3, #32
	str	r3, [r2, #0]
.L_08152dc8:
	mov	r3, fp
	cmp	r3, #3
	bne.n	.L_08152dd6
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #4]
	subs	r3, #16
	str	r3, [r5, #4]
.L_08152dd6:
	mov	r6, fp
	cmp	r6, #8
	bne.n	.L_08152de4
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #4]
	subs	r3, #40
	str	r3, [r0, #4]
.L_08152de4:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L_08152df2
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #4]
	subs	r3, #48
	str	r3, [r2, #4]
.L_08152df2:
	mov	r3, fp
	cmp	r3, #22
	bne.n	.L_08152e00
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #4]
	subs	r3, #40
	str	r3, [r5, #4]
.L_08152e00:
	mov	r6, fp
	cmp	r6, #9
	bne.n	.L_08152e0e
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #4]
	subs	r3, #40
	str	r3, [r0, #4]
.L_08152e0e:
	mov	r1, fp
	cmp	r1, #17
	bne.n	.L_08152e1c
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #4]
	subs	r3, #16
	str	r3, [r2, #4]
.L_08152e1c:
	mov	r3, fp
	cmp	r3, #19
	bne.n	.L_08152e2a
	ldr	r5, [sp, #60]
	ldr	r3, [r5, #4]
	subs	r3, #16
	str	r3, [r5, #4]
.L_08152e2a:
	mov	r6, fp
	cmp	r6, #10
	bne.n	.L_08152e5c
	ldr	r0, [sp, #60]
	movs	r1, #0
	ldr	r3, [r0, #4]
	mov	r9, r1
	subs	r3, #24
	str	r3, [r0, #4]
	ldr	r2, [sp, #84]
	ldr	r3, [r2, #20]
	cmp	r3, #0
	beq.n	.L_08152e5c
	add	r1, sp, #120
	movs	r2, #4
.L_08152e48:
	ldr	r3, [r2, r1]
	adds	r3, #80
	str	r3, [r2, r1]
	ldr	r5, [sp, #84]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r5, #20]
	adds	r2, #12
	cmp	r9, r3
	bne.n	.L_08152e48
.L_08152e5c:
	mov	r6, fp
	cmp	r6, #4
	bne.n	.L_08152e7c
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_08152e74
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #0]
	adds	r3, #28
	str	r3, [r1, #0]
	b.n	.L_08152e7c
.L_08152e74:
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	subs	r3, #28
	str	r3, [r2, #0]
.L_08152e7c:
	mov	r3, fp
	cmp	r3, #7
	bne.n	.L_08152eac
	ldr	r5, [sp, #84]
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne.n	.L_08152ea4
	ldr	r6, [sp, #60]
	ldr	r3, [r6, #0]
	adds	r3, #16
	str	r3, [r6, #0]
	b.n	.L_08152eac
	.4byte 0x00000148
	.4byte 0x00000150
	.4byte 0x03000730
	.2byte 0x3001
	.2byte 0x0814
.L_08152ea4:
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #0]
	subs	r3, #16
	str	r3, [r0, #0]
.L_08152eac:
	mov	r1, fp
	cmp	r1, #5
	beq.n	.L_08152eb6
	cmp	r1, #23
	bne.n	.L_08152edc
.L_08152eb6:
	ldr	r2, [sp, #60]
	movs	r1, #3
	ldr	r0, [r2, #0]
	bl	sub_08002054
	ldr	r3, [sp, #60]
	mov	r5, fp
	str	r0, [r3, #0]
	cmp	r5, #23
	bne.n	.L_08152ed2
	ldr	r6, [sp, #60]
	ldr	r3, [r6, #4]
	subs	r3, #16
	str	r3, [r6, #4]
.L_08152ed2:
	movs	r2, #128
	ldr	r3, [pc, #52]
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
.L_08152edc:
	mov	r0, fp
	cmp	r0, #10
	bne.n	.L_08152f56
	ldr	r2, [sp, #84]
	movs	r7, #255
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r5, [sp, #84]
	ldr	r6, [r0, #0]
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	ldr	r5, [pc, #12]
	mov	r8, r0
	movs	r0, #0
	mov	r9, r0
	b.n	.L_08152f14
	movs	r0, r0
	.4byte 0x00000055
	.2byte 0x0000
	.2byte 0x0201
.L_08152f14:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, r8
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r1, #1
	ands	r0, r7
	subs	r0, #128
	add	r9, r1
	lsls	r0, r0, #10
	movs	r3, #0
	mov	r2, r9
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_08152f14
.L_08152f56:
	ldr	r5, [sp, #64]
	movs	r3, #0
	str	r3, [sp, #72]
	cmp	r5, #0
	bne.n	.L_08152f64
	bl	.L_0815394e
.L_08152f64:
	ldr	r6, [sp, #76]
	subs	r5, #8
	str	r5, [sp, #40]
	str	r6, [sp, #24]
.L_08152f6c:
	ldr	r0, [sp, #84]
	ldr	r1, [r0, #20]
	ldr	r0, [sp, #72]
	bl	sub_08002064
	ldr	r1, [sp, #72]
	ldr	r2, [sp, #40]
	mov	sl, r0
	cmp	r1, r2
	blt.n	.L_08152f94
	ldr	r5, [sp, #64]
	movs	r3, #128
	subs	r2, r1, r5
	ldr	r1, [pc, #32]
	lsls	r2, r2, #1
	lsls	r3, r3, #19
	negs	r2, r2
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_08152f94:
	mov	r6, fp
	cmp	r6, #8
	bne.n	.L_08152fb0
	ldr	r0, [sp, #72]
	cmp	r0, #15
	ble.n	.L_08152fb0
	ldr	r0, [pc, #8]
	bl	sub_0815f0a0
	b.n	.L_08152fb0
	.4byte 0x00001000
	.2byte 0x0166
	.2byte 0x0000
.L_08152fb0:
	mov	r1, fp
	cmp	r1, #10
	bne.n	.L_08152fc2
	ldr	r2, [sp, #72]
	cmp	r2, #31
	ble.n	.L_08152fc2
	ldr	r0, [pc, #820]
	bl	sub_0815f0a0
.L_08152fc2:
	ldr	r3, [sp, #72]
	cmp	r3, #4
	bne.n	.L_08152fe0
	mov	r5, fp
	cmp	r5, #9
	beq.n	.L_08152fd2
	cmp	r5, #17
	bne.n	.L_08152fda
.L_08152fd2:
	movs	r0, #92
	bl	sub_081c0010
	b.n	.L_08152fe0
.L_08152fda:
	movs	r0, #136
	bl	sub_081c0010
.L_08152fe0:
	mov	r6, fp
	cmp	r6, #8
	beq.n	.L_08153012
	cmp	r6, #6
	bne.n	.L_08152ff2
	ldr	r0, [sp, #72]
	cmp	r0, #60
	bne.n	.L_08153012
	b.n	.L_08152ffe
.L_08152ff2:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L_08153006
	ldr	r2, [sp, #72]
	cmp	r2, #48
	bne.n	.L_08153012
.L_08152ffe:
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_08153012
.L_08153006:
	ldr	r3, [sp, #72]
	cmp	r3, #24
	bne.n	.L_08153012
	movs	r0, #134
	bl	sub_081180e8
.L_08153012:
	ldr	r5, [sp, #84]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	sub_081530a0
	ldr	r0, [sp, #72]
	lsls	r6, r0, #11
	adds	r0, r6, #0
	bl	sub_08002096
	add	r5, sp, #240
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
	ldr	r1, [sp, #72]
	adds	r0, r0, r3
	adds	r5, r0, #0
	subs	r7, #10
	subs	r5, #24
	cmp	r1, #56
	ble.n	.L_0815305a
	lsls	r3, r1, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #112
.L_0815305a:
	ldr	r2, [sp, #72]
	cmp	r2, #3
	bgt.n	sub_08153080
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r6, [sp, #76]
	ldr	r3, [sp, #36]
	movs	r2, #216
	lsls	r2, r2, #7
	adds	r2, #192
	ldr	r4, [r3, #4]
	adds	r1, r6, r2
	ldr	r0, [sp, #80]
	adds	r2, r7, #0
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2314
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r6, [sp, #76]
	ldr	r3, [sp, #36]
	movs	r2, #216
	lsls	r2, r2, #7
	adds	r2, #192
	ldr	r4, [r3, #4]
	adds	r1, r6, r2
	ldr	r0, [sp, #80]
	adds	r2, r7, #0
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465b
	cmp	r3, #5
	beq.n	.L_081530aa
	cmp	r3, #23
	bne.n	.L_08153120
.L_081530aa:
	ldr	r5, [sp, #84]
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne.n	.L_081530e4
	ldr	r0, [sp, #72]
	movs	r1, #3
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	lsls	r1, r0, #3
	ldr	r3, [sp, #60]
	ldr	r6, [sp, #76]
	adds	r1, r1, r0
	movs	r0, #152
	lsls	r1, r1, #9
	lsls	r0, r0, #5
	ldr	r2, [r3, #0]
	adds	r0, #86
	ldr	r3, [r3, #4]
	adds	r1, r6, r1
	adds	r1, r1, r0
	movs	r0, #72
	str	r0, [sp, #0]
	movs	r0, #62
	str	r0, [sp, #4]
	subs	r2, #2
	b.n	.L_08153114
.L_081530e4:
	ldr	r0, [sp, #72]
	movs	r1, #3
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	lsls	r1, r0, #3
	adds	r1, r1, r0
	ldr	r0, [sp, #60]
	ldr	r5, [sp, #76]
	ldr	r2, [r0, #0]
	ldr	r3, [r0, #4]
	movs	r6, #152
	movs	r0, #72
	lsls	r1, r1, #9
	lsls	r6, r6, #5
	adds	r1, r5, r1
	adds	r6, #86
	str	r0, [sp, #0]
	movs	r0, #62
	str	r0, [sp, #4]
	adds	r1, r1, r6
	subs	r2, #70
.L_08153114:
	subs	r3, #32
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe09b
.L_08153120:
	bl	sub_08014878
	mov	r2, sl
	lsls	r6, r2, #1
	adds	r3, r6, r2
	add	r7, sp, #120
	lsls	r5, r3, #2
	ldr	r3, [r7, r5]
	movs	r2, #31
	ands	r2, r0
	movs	r1, #108
	adds	r3, r3, r2
	add	r1, sp
	subs	r3, #16
	str	r3, [r1, #0]
	mov	r3, fp
	mov	r8, r1
	mov	r9, r7
	cmp	r3, #9
	beq.n	.L_08153150
	cmp	r3, #17
	beq.n	.L_08153150
	cmp	r3, #19
	bne.n	.L_0815316e
.L_08153150:
	bl	sub_08014878
	mov	r5, sl
	adds	r3, r6, r5
	lsls	r3, r3, #2
	adds	r3, #4
	mov	r6, r9
	ldr	r3, [r6, r3]
	movs	r2, #127
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #32
	mov	r0, r8
	str	r3, [r0, #4]
	b.n	.L_0815320c
.L_0815316e:
	mov	r1, fp
	cmp	r1, #12
	beq.n	.L_0815318c
	cmp	r1, #15
	beq.n	.L_0815318c
	cmp	r1, #18
	beq.n	.L_0815318c
	cmp	r1, #20
	beq.n	.L_0815318c
	cmp	r1, #21
	beq.n	.L_0815318c
	cmp	r1, #22
	beq.n	.L_0815318c
	cmp	r1, #16
	bne.n	.L_081531aa
.L_0815318c:
	bl	sub_08014878
	mov	r2, sl
	adds	r3, r6, r2
	lsls	r3, r3, #2
	adds	r3, #4
	mov	r5, r9
	ldr	r3, [r5, r3]
	movs	r2, #127
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #64
	mov	r6, r8
	str	r3, [r6, #4]
	b.n	.L_0815320c
.L_081531aa:
	mov	r0, fp
	cmp	r0, #24
	bne.n	.L_081531c8
	bl	sub_08014878
	movs	r1, #96
	bl	sub_0800206c
	adds	r5, #4
	ldr	r3, [r7, r5]
	mov	r1, r8
	adds	r3, r3, r0
	subs	r3, #32
	str	r3, [r1, #4]
	b.n	.L_0815320c
.L_081531c8:
	mov	r2, fp
	cmp	r2, #11
	bne.n	.L_081531f8
	bl	sub_08014878
	adds	r5, #4
	ldr	r3, [r7, r5]
	movs	r2, #255
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #192
	mov	r6, r8
	str	r3, [r6, #4]
	ldr	r1, [sp, #72]
	lsls	r0, r1, #12
	bl	sub_08002096
	ldr	r3, [r7, r5]
	lsls	r0, r0, #8
	asrs	r0, r0, #16
	adds	r3, r3, r0
	subs	r3, #128
	str	r3, [r6, #4]
	b.n	.L_0815320c
.L_081531f8:
	bl	sub_08014878
	adds	r3, r5, #4
	ldr	r3, [r7, r3]
	movs	r2, #63
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #16
	mov	r2, r8
	str	r3, [r2, #4]
.L_0815320c:
	mov	r5, fp
	movs	r3, #16
	cmp	r5, #10
	beq.n	.L_08153216
	movs	r3, #48
.L_08153216:
	ldr	r6, [sp, #72]
	cmp	r6, r3
	bge.n	.L_08153258
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #24]
	ldr	r0, [r1, #0]
	mov	r3, r8
	lsls	r0, r0, #15
	str	r0, [r2, #0]
	mov	r6, r8
	ldr	r4, [r1, #4]
	lsls	r4, r4, #16
	str	r4, [r2, #4]
	ldr	r5, [sp, #60]
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #0]
	subs	r1, r1, r3
	lsls	r3, r1, #11
	str	r3, [r2, #12]
	lsls	r1, r1, #12
	ldr	r2, [r6, #4]
	ldr	r3, [r5, #4]
	ldr	r5, [sp, #24]
	subs	r2, r2, r3
	lsls	r3, r2, #11
	lsls	r2, r2, #12
	str	r3, [r5, #16]
	adds	r0, r0, r1
	adds	r4, r4, r2
	movs	r3, #0
	str	r0, [r5, #0]
	str	r4, [r5, #4]
	str	r3, [r5, #24]
.L_08153258:
	ldr	r7, [sp, #76]
	movs	r6, #0
	movs	r0, #32
	mov	r9, r6
	mov	r8, r0
.L_08153262:
	ldr	r6, [r7, #24]
	cmp	r6, #0
	bge.n	.L_0815326a
	b.n	.L_0815353a
.L_0815326a:
	mov	r1, fp
	cmp	r1, #7
	beq.n	.L_08153274
	cmp	r1, #21
	bne.n	.L_081532b4
.L_08153274:
	mov	r2, fp
	cmp	r2, #22
	bne.n	.L_08153286
	movs	r3, #1
	mov	r5, r9
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_08153286
	b.n	.L_0815353a
.L_08153286:
	cmp	r6, #7
	bgt.n	.L_0815328c
	b.n	sub_081534f8
.L_0815328c:
	ldr	r6, [sp, #84]
	movs	r1, #2
	ldrsh	r2, [r7, r1]
	ldr	r0, [r6, #4]
	movs	r1, #64
	mov	r6, r8
	movs	r5, #6
	ldrsh	r3, [r7, r5]
	str	r6, [sp, #0]
	str	r1, [sp, #4]
	ldr	r5, [sp, #76]
	ldr	r1, [sp, #36]
	movs	r6, #152
	lsls	r6, r6, #5
	lsls	r0, r0, #2
	adds	r6, #86
	ldr	r4, [r0, r1]
	subs	r2, #16
	subs	r3, #32
	b.n	.L_081532ea
.L_081532b4:
	mov	r0, fp
	cmp	r0, #22
	bne.n	.L_081532f8
	movs	r3, #1
	mov	r1, r9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_081532c6
	b.n	.L_0815353a
.L_081532c6:
	cmp	r6, #2
	bgt.n	.L_081532cc
	b.n	sub_081534f8
.L_081532cc:
	mov	r6, r8
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	movs	r5, #6
	ldrsh	r3, [r7, r5]
	str	r6, [sp, #0]
	ldr	r5, [sp, #76]
	movs	r6, #152
	lsls	r6, r6, #5
	movs	r1, #64
	adds	r6, #86
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #88]
.L_081532ea:
	ldr	r0, [sp, #80]
	adds	r1, r5, r6
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe101
	.2byte 0x0184
	.2byte 0x0000
.L_081532f8:
	mov	r0, fp
	cmp	r0, #12
	beq.n	.L_08153302
	cmp	r0, #16
	bne.n	.L_08153324
.L_08153302:
	cmp	r6, #1
	bgt.n	.L_08153308
	b.n	sub_081534f8
.L_08153308:
	movs	r1, #2
	ldrsh	r2, [r7, r1]
	movs	r5, #6
	ldrsh	r3, [r7, r5]
	ldr	r6, [sp, #76]
	movs	r1, #24
	movs	r5, #184
	str	r1, [sp, #0]
	lsls	r5, r5, #5
	movs	r1, #48
	str	r1, [sp, #4]
	subs	r2, #12
	subs	r3, #24
	b.n	.L_0815342e
.L_08153324:
	mov	r0, fp
	cmp	r0, #9
	beq.n	.L_0815332e
	cmp	r0, #17
	bne.n	.L_0815337e
.L_0815332e:
	mov	r1, r9
	lsrs	r0, r1, #31
	add	r0, r9
	asrs	r0, r0, #1
	movs	r1, #3
	bl	sub_08002064
	movs	r3, #1
	mov	r2, r9
	ands	r3, r2
	adds	r5, r0, #0
	cmp	r3, #0
	bne.n	.L_0815334a
	b.n	sub_081534f8
.L_0815334a:
	ldr	r3, [sp, #72]
	lsls	r0, r2, #1
	adds	r0, r0, r3
	lsls	r0, r0, #11
	bl	sub_08002096
	lsls	r1, r5, #10
	ldr	r5, [sp, #76]
	ldr	r3, [r7, #4]
	movs	r6, #224
	lsls	r0, r0, #5
	adds	r1, r5, r1
	lsls	r6, r6, #3
	mov	r5, r8
	adds	r3, r3, r0
	adds	r1, r1, r6
	movs	r0, #2
	ldrsh	r2, [r7, r0]
	ldr	r6, [sp, #36]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	asrs	r3, r3, #16
	subs	r2, #16
	subs	r3, #16
	ldr	r4, [r6, #4]
	b.n	.L_081534b2
.L_0815337e:
	mov	r0, fp
	cmp	r0, #20
	bne.n	.L_081533cc
	movs	r1, #5
	mov	r0, r9
	bl	sub_08002064
	lsls	r5, r0, #1
	movs	r1, #3
	adds	r5, r5, r0
	adds	r0, r6, #0
	bl	sub_08002064
	ldr	r3, [pc, #828]
	adds	r5, r5, r0
	ldrb	r4, [r3, r5]
	movs	r1, #2
	ldrsh	r2, [r7, r1]
	ldr	r1, [pc, #824]
	lsrs	r3, r4, #1
	ldrb	r0, [r1, r5]
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r7, r6]
	lsrs	r1, r0, #1
	subs	r3, r3, r1
	ldr	r1, [pc, #812]
	lsls	r5, r5, #1
	ldrh	r1, [r1, r5]
	ldr	r5, [sp, #76]
	movs	r6, #216
	lsls	r6, r6, #5
	adds	r1, r5, r1
	adds	r6, #146
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r1, r6
	ldr	r4, [sp, #88]
	b.n	.L_081534b2
.L_081533cc:
	mov	r0, fp
	cmp	r0, #19
	bne.n	.L_0815340a
	ldr	r2, [sp, #72]
	mov	r1, r9
	lsls	r0, r1, #1
	adds	r0, r0, r2
	lsls	r0, r0, #11
	bl	sub_08002096
	asrs	r5, r6, #2
	ldr	r3, [r7, #4]
	lsls	r1, r5, #3
	lsls	r0, r0, #5
	adds	r1, r1, r5
	ldr	r5, [sp, #76]
	adds	r3, r3, r0
	movs	r0, #2
	ldrsh	r2, [r7, r0]
	movs	r0, #24
	str	r0, [sp, #0]
	lsls	r1, r1, #7
	movs	r0, #48
	adds	r1, r5, r1
	str	r0, [sp, #4]
	ldr	r5, [sp, #36]
	movs	r6, #224
	asrs	r3, r3, #16
	lsls	r6, r6, #3
	adds	r1, r1, r6
	b.n	.L_081534ac
.L_0815340a:
	mov	r0, fp
	cmp	r0, #18
	bne.n	.L_0815343a
	cmp	r6, #1
	ble.n	sub_081534f8
	movs	r1, #2
	ldrsh	r2, [r7, r1]
	movs	r5, #6
	ldrsh	r3, [r7, r5]
	ldr	r6, [sp, #76]
	movs	r1, #22
	movs	r5, #224
	str	r1, [sp, #0]
	lsls	r5, r5, #3
	movs	r1, #42
	str	r1, [sp, #4]
	subs	r2, #11
	subs	r3, #21
.L_0815342e:
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #80]
	adds	r1, r6, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe05e
.L_0815343a:
	mov	r0, fp
	cmp	r0, #4
	bne.n	.L_08153474
	cmp	r6, #5
	ble.n	sub_081534f8
	adds	r1, r6, #0
	cmp	r6, #0
	bge.n	.L_0815344c
	adds	r1, r6, #3
.L_0815344c:
	ldr	r2, [sp, #76]
	asrs	r1, r1, #2
	movs	r3, #152
	lsls	r1, r1, #11
	lsls	r3, r3, #5
	adds	r1, r2, r1
	adds	r3, #86
	movs	r5, #2
	ldrsh	r2, [r7, r5]
	adds	r1, r1, r3
	movs	r6, #6
	ldrsh	r3, [r7, r6]
	mov	r0, r8
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #88]
	b.n	.L_081534b2
.L_08153474:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L_081534ba
	cmp	r6, #1
	ble.n	sub_081534f8
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_08153486
	adds	r3, r6, #3
.L_08153486:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	ldr	r2, [sp, #76]
	adds	r1, r1, r3
	movs	r3, #152
	lsls	r1, r1, #7
	lsls	r3, r3, #5
	adds	r1, r2, r1
	adds	r3, #86
	mov	r0, fp
	movs	r5, #2
	ldrsh	r2, [r7, r5]
	adds	r1, r1, r3
	movs	r6, #6
	ldrsh	r3, [r7, r6]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r5, [sp, #36]
.L_081534ac:
	subs	r2, #12
	subs	r3, #24
	ldr	r4, [r5, #4]
.L_081534b2:
	ldr	r0, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01e
.L_081534ba:
	mov	r0, fp
	cmp	r0, #5
	beq.n	sub_081534f8
	cmp	r6, #1
	ble.n	sub_081534f8
	adds	r0, r6, #0
	cmp	r0, #0
	bge.n	.L_081534cc
	adds	r0, #3
.L_081534cc:
	ldr	r2, [sp, #76]
	asrs	r1, r0, #2
	movs	r3, #152
	lsls	r1, r1, #11
	lsls	r3, r3, #5
	adds	r1, r2, r1
	adds	r3, #86
	movs	r5, #2
	ldrsh	r2, [r7, r5]
	adds	r1, r1, r3
	movs	r6, #6
	ldrsh	r3, [r7, r6]
	mov	r0, r8
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x683b
	ldr	r2, [r7, #12]
	mov	r1, fp
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	cmp	r1, #10
	bne.n	.L_08153514
	ldr	r5, [pc, #468]
	adds	r3, r2, r5
	str	r3, [r7, #16]
.L_08153514:
	ldr	r3, [r7, #24]
	mov	r6, fp
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r6, #9
	beq.n	.L_08153524
	cmp	r6, #17
	bne.n	.L_08153530
.L_08153524:
	cmp	r3, #32
	bne.n	.L_0815353a
	movs	r0, #1
	negs	r0, r0
	str	r0, [r7, #24]
	b.n	.L_0815353a
.L_08153530:
	cmp	r3, #24
	bne.n	.L_0815353a
	movs	r1, #1
	negs	r1, r1
	str	r1, [r7, #24]
.L_0815353a:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r7, #28
	cmp	r3, #64
	beq.n	.L_08153548
	b.n	.L_08153262
.L_08153548:
	mov	r5, fp
	cmp	r5, #24
	beq.n	.L_08153550
	b.n	.L_08153668
.L_08153550:
	ldr	r0, [sp, #84]
	movs	r6, #0
	ldr	r2, [r0, #20]
	mov	r9, r6
	cmp	r2, #0
	bne.n	.L_0815355e
	b.n	.L_08153668
.L_0815355e:
	movs	r1, #4
	str	r1, [sp, #32]
	str	r6, [sp, #28]
.L_08153564:
	mov	r5, r9
	ldr	r6, [sp, #72]
	lsls	r3, r5, #2
	adds	r3, #16
	cmp	r6, r3
	blt.n	.L_08153652
	ldr	r0, [sp, #40]
	cmp	r6, r0
	bge.n	.L_08153652
	ldr	r5, [sp, #28]
	ldr	r6, [sp, #32]
	mov	r3, sp
	movs	r1, #0
	adds	r3, #120
	str	r1, [sp, #56]
	str	r3, [sp, #52]
	str	r5, [sp, #48]
	str	r6, [sp, #44]
	mov	r7, r9
	movs	r2, #3
	ands	r7, r2
.L_0815358e:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [sp, #48]
	adds	r6, r0, #0
	ldr	r0, [sp, #52]
	lsls	r6, r6, #4
	ldr	r3, [r0, r1]
	asrs	r6, r6, #16
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [pc, #308]
	asrs	r3, r3, #1
	adds	r6, r6, r3
	ldrb	r3, [r2, r7]
	adds	r0, r5, #0
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	mov	sl, r2
	bl	sub_08002090
	ldr	r1, [sp, #44]
	adds	r5, r0, #0
	ldr	r0, [sp, #52]
	ldr	r2, [pc, #284]
	ldr	r3, [r0, r1]
	lsls	r5, r5, #4
	asrs	r5, r5, #16
	adds	r5, r5, r3
	ldrb	r3, [r2, r7]
	movs	r0, #188
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	mov	r8, r2
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08014878
	ldr	r3, [pc, #256]
	movs	r1, #3
	ands	r0, r1
	ldrb	r2, [r3, r0]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #0
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r2, [pc, #232]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	mov	r0, sl
	adds	r1, r1, r3
	ldrb	r3, [r0, r7]
	mov	r2, r8
	str	r3, [sp, #0]
	ldr	r0, [sp, #80]
	ldrb	r3, [r2, r7]
	adds	r2, r6, #0
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r4, [r3, #0]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	ldr	r5, [sp, #84]
	ldr	r1, [sp, #36]
	ldr	r0, [r5, #4]
	bl	sub_08144aac
	ldr	r6, [sp, #56]
	adds	r6, #1
	str	r6, [sp, #56]
	cmp	r6, #3
	bne.n	.L_0815358e
	ldr	r2, [r5, #20]
.L_08153652:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	movs	r3, #1
	adds	r0, #12
	adds	r1, #12
	add	r9, r3
	str	r0, [sp, #32]
	str	r1, [sp, #28]
	cmp	r9, r2
	beq.n	.L_08153668
	b.n	.L_08153564
.L_08153668:
	mov	r5, fp
	cmp	r5, #9
	bne.n	.L_08153670
	b.n	.L_08153834
.L_08153670:
	cmp	r5, #17
	bne.n	.L_08153676
	b.n	.L_08153834
.L_08153676:
	cmp	r5, #5
	beq.n	.L_0815367e
	cmp	r5, #23
	bne.n	.L_081536f8
.L_0815367e:
	ldr	r0, [sp, #84]
	movs	r6, #0
	ldr	r2, [r0, #20]
	mov	r9, r6
	cmp	r2, #0
	bne.n	.L_0815368c
	b.n	.L_08153834
.L_0815368c:
	ldr	r1, [sp, #72]
	ldr	r3, [sp, #76]
	movs	r0, #238
	lsls	r0, r0, #7
	movs	r7, #7
	adds	r0, #168
	ands	r7, r1
	movs	r6, #36
	movs	r5, #2
	adds	r4, r3, r0
.L_081536a0:
	ldr	r1, [sp, #72]
	cmp	r1, r5
	blt.n	.L_081536c8
	cmp	r7, r9
	bne.n	.L_081536c8
	movs	r3, #8
	str	r3, [r4, #0]
	ldr	r2, [sp, #84]
	movs	r1, #7
	ldrsh	r0, [r6, r2]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r9
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r0, [sp, #84]
	ldr	r4, [sp, #8]
	ldr	r2, [r0, #20]
.L_081536c8:
	movs	r1, #1
	add	r9, r1
	adds	r6, #2
	adds	r5, #4
	cmp	r9, r2
	bne.n	.L_081536a0
	b.n	.L_08153834
	movs	r0, r0
	.4byte 0x0819749e
	.4byte 0x081974ad
	.4byte 0x081974bc
	.4byte 0xffffc000
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x081983c8
	.2byte 0x7486
	.2byte 0x0819
.L_081536f8:
	mov	r2, fp
	cmp	r2, #10
	bne.n	.L_0815375a
	ldr	r3, [sp, #72]
	cmp	r3, #18
	bne.n	.L_08153724
	ldr	r5, [sp, #76]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	movs	r2, #4
	adds	r3, r5, r6
	str	r2, [r3, #0]
	ldr	r3, [sp, #84]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_08153724:
	ldr	r5, [sp, #72]
	cmp	r5, #46
	bne.n	.L_0815373e
	ldr	r6, [sp, #84]
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r6, #8]
	movs	r1, #7
	str	r3, [sp, #0]
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0815373e:
	ldr	r0, [sp, #72]
	cmp	r0, #54
	bne.n	.L_08153834
	ldr	r1, [sp, #84]
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r1, #8]
	negs	r2, r2
	str	r3, [sp, #0]
	movs	r1, #0
	subs	r3, #1
	bl	sub_0814cd48
	b.n	.L_08153834
.L_0815375a:
	mov	r2, fp
	cmp	r2, #8
	bne.n	.L_08153796
	ldr	r3, [sp, #72]
	cmp	r3, #20
	bne.n	.L_08153834
	ldr	r6, [sp, #84]
	movs	r5, #0
	ldr	r3, [r6, #20]
	mov	r9, r5
	cmp	r3, #0
	beq.n	.L_08153834
	movs	r5, #36
.L_08153774:
	ldr	r1, [sp, #84]
	movs	r3, #60
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r1, #6
	mov	r3, r9
	movs	r2, #5
	bl	sub_0814cd48
	ldr	r6, [sp, #84]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r6, #20]
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_08153774
	b.n	.L_08153834
.L_08153796:
	ldr	r1, [sp, #84]
	movs	r0, #0
	ldr	r2, [r1, #20]
	mov	r9, r0
	cmp	r2, #0
	beq.n	.L_08153834
	movs	r5, #36
.L_081537a4:
	mov	r6, r9
	ldr	r0, [sp, #72]
	lsls	r3, r6, #2
	adds	r3, #16
	cmp	r0, r3
	blt.n	.L_0815382a
	ldr	r1, [sp, #40]
	cmp	r0, r1
	bge.n	.L_0815382a
	movs	r1, #3
	adds	r3, r0, #0
	ands	r3, r1
	cmp	r3, r9
	bne.n	.L_0815382a
	ldr	r3, [sp, #76]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r2, r3, r6
	mov	r0, fp
	movs	r3, #8
	str	r3, [r2, #0]
	cmp	r0, #6
	bne.n	.L_081537e6
	ldr	r2, [sp, #84]
	ldrsh	r0, [r5, r2]
	str	r1, [sp, #0]
	movs	r2, #5
	movs	r1, #14
	mov	r3, r9
	bl	sub_0814cd48
	b.n	.L_081537f6
.L_081537e6:
	ldr	r6, [sp, #84]
	mov	r3, r9
	ldrsh	r0, [r5, r6]
	str	r1, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	bl	sub_0814cd48
.L_081537f6:
	mov	r3, fp
	cmp	r3, #11
	beq.n	.L_08153820
	cmp	r3, #14
	beq.n	.L_08153826
	ldr	r6, [sp, #84]
	movs	r3, #128
	lsls	r3, r3, #10
	ldrsh	r0, [r5, r6]
	str	r3, [sp, #0]
	ldr	r3, [sp, #72]
	movs	r2, #128
	adds	r3, #100
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r2, r2, #9
	movs	r1, #1
	lsls	r3, r3, #11
	bl	sub_0815f000
	b.n	.L_08153828
.L_08153820:
	ldr	r3, [sp, #84]
	ldr	r2, [r3, #20]
	b.n	.L_0815382a
.L_08153826:
	ldr	r6, [sp, #84]
.L_08153828:
	ldr	r2, [r6, #20]
.L_0815382a:
	movs	r0, #1
	add	r9, r0
	adds	r5, #2
	cmp	r9, r2
	bne.n	.L_081537a4
.L_08153834:
	mov	r1, fp
	cmp	r1, #10
	bne.n	.L_08153912
	ldr	r2, [sp, #72]
	cmp	r2, #15
	ble.n	.L_08153912
	ldr	r3, [sp, #84]
	movs	r6, #0
	ldr	r0, [r3, #8]
	bl	sub_08118098
	ldr	r5, [sp, #84]
	ldr	r7, [r0, #0]
	ldr	r0, [r5, #8]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	asrs	r0, r0, #1
	mov	r8, r0
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r0, r5, #0
	adds	r1, #12
	bl	sub_080156e8
	ldr	r5, [pc, #324]
	movs	r0, #12
	mov	r9, r6
	mov	sl, r0
	add	r6, sp, #96
.L_0815387a:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_08153906
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_0815e1ec
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #304]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldrh	r1, [r3, #10]
	ldr	r3, [sp, #68]
	movs	r0, #6
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	mov	r0, sl
	subs	r3, #6
	str	r0, [sp, #4]
	subs	r2, #3
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #80]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x213e
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_08138058
	ldr	r1, [sp, #72]
	mov	r3, r9
	adds	r3, #16
	cmp	r1, r3
	ble.n	.L_08153906
	ldr	r2, [r7, #12]
	ldr	r3, [r5, #4]
	add	r2, r8
	ldr	r1, [r7, #16]
	subs	r2, r2, r3
	ldr	r3, [r5, #8]
	ldr	r0, [r7, #8]
	ldr	r4, [r5, #0]
	subs	r1, r1, r3
	ldr	r3, [r5, #12]
	subs	r0, r0, r4
	asrs	r0, r0, #7
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r5, #16]
	ldr	r3, [r5, #20]
	asrs	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r5, #20]
	ldr	r2, [r7, #8]
	cmp	r2, #0
	bge.n	.L_081538f4
	negs	r2, r2
.L_081538f4:
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L_081538fc
	negs	r3, r3
.L_081538fc:
	cmp	r2, r3
	bge.n	.L_08153906
	movs	r2, #1
	negs	r2, r2
	str	r2, [r5, #24]
.L_08153906:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r5, #28
	cmp	r0, #24
	bne.n	.L_0815387a
.L_08153912:
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	mov	r1, fp
	cmp	r1, #6
	beq.n	.L_08153924
	bl	sub_081434f8
.L_08153924:
	ldr	r3, [sp, #76]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r6, [sp, #24]
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #64]
	adds	r6, #28
	adds	r0, #1
	str	r6, [sp, #24]
	str	r0, [sp, #72]
	cmp	r0, r1
	beq.n	.L_0815394e
	bl	.L_08152f6c
.L_0815394e:
	ldr	r0, [pc, #112]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	mov	r2, fp
	cmp	r2, #11
	beq.n	.L_0815396e
	cmp	r2, #14
	beq.n	.L_0815396e
	cmp	r2, #18
	bne.n	.L_081539a6
.L_0815396e:
	movs	r1, #240
	ldr	r5, [pc, #80]
	lsls	r1, r1, #6
	ldr	r0, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #80]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9d15
	movs	r3, #0
	str	r3, [r5, #28]
	ldr	r0, [pc, #64]
	mov	r6, fp
	bl	sub_08014644
	cmp	r6, #14
	bne.n	.L_0815399e
	ldr	r0, [sp, #84]
	bl	sub_081504c0
	b.n	.L_081539aa
.L_0815399e:
	ldr	r0, [sp, #84]
	bl	sub_081504b4
	b.n	.L_081539aa
.L_081539a6:
	bl	sub_08143bb8
.L_081539aa:
	add	sp, #252
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x08197410
	.4byte 0x08143001
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x3489
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	ldr	r3, [r3, #96]
	sub	sp, #52
	adds	r5, r0, #0
	movs	r0, #0
	str	r3, [sp, #16]
	mov	fp, r2
	bl	sub_081435e0
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #548]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [r5, #4]
	movs	r3, #1
	eors	r0, r3
	add	r1, sp, #20
	ldr	r5, [pc, #532]
	bl	sub_08144aac
	movs	r3, #255
	movs	r7, #0
	mov	r8, r3
	movs	r6, #0
.L_08153a1a:
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	mov	r3, r8
	ands	r0, r3
	subs	r0, #127
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	mov	r2, r8
	ands	r0, r2
	subs	r0, #127
	movs	r3, #128
	lsls	r0, r0, #16
	adds	r7, #1
	lsls	r3, r3, #1
	str	r0, [r5, #8]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r7, r3
	bne.n	.L_08153a1a
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	movs	r5, #0
	add	r3, fp
	movs	r1, #200
	str	r5, [r3, #0]
	ldr	r0, [pc, #436]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r3, #160
	mov	r2, sp
	adds	r2, #40
	lsls	r3, r3, #15
	str	r2, [sp, #12]
	str	r5, [r2, #0]
	str	r3, [r2, #4]
	str	r5, [r2, #8]
	mov	sl, r5
.L_08153a8a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	ldr	r0, [sp, #12]
	bl	sub_08015128
	mov	r3, sl
	negs	r3, r3
	mov	r2, sl
	ldr	r6, [pc, #376]
	str	r3, [sp, #8]
	lsls	r2, r2, #8
	lsls	r3, r3, #8
	movs	r7, #0
	mov	r8, r3
	mov	r9, r2
.L_08153ab8:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_08153ac0
	adds	r3, r7, #3
.L_08153ac0:
	asrs	r3, r3, #2
	cmp	sl, r3
	bgt.n	.L_08153ac8
	b.n	.L_08153bc8
.L_08153ac8:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	bne.n	.L_08153bc8
	bl	sub_08014e38
	movs	r3, #3
	ands	r3, r7
	cmp	r3, #1
	beq.n	.L_08153af6
	cmp	r3, #1
	bgt.n	.L_08153ae4
	cmp	r3, #0
	beq.n	.L_08153aee
	b.n	.L_08153b12
.L_08153ae4:
	cmp	r3, #2
	beq.n	.L_08153afe
	cmp	r3, #3
	beq.n	.L_08153b06
	b.n	.L_08153b12
.L_08153aee:
	mov	r0, r9
	bl	sub_08015068
	b.n	.L_08153b12
.L_08153af6:
	mov	r0, r8
	bl	sub_08015024
	b.n	.L_08153b12
.L_08153afe:
	mov	r0, r8
	bl	sub_080150e4
	b.n	.L_08153b12
.L_08153b06:
	mov	r0, r8
	bl	sub_08015024
	mov	r0, r8
	bl	sub_080150e4
.L_08153b12:
	add	r5, sp, #28
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08014ea8
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L_08153b32
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L_08153b32:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #122
	cmp	r2, r3
	ble.n	.L_08153b40
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_08153b40:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L_08153b4a
	adds	r3, #63
.L_08153b4a:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	movs	r3, #3
	ands	r3, r7
	lsls	r1, r3, #1
	ldr	r2, [pc, #212]
	adds	r1, r1, r3
	lsls	r4, r0, #1
	lsls	r1, r1, #7
	adds	r1, r1, r3
	subs	r3, r4, #2
	ldrh	r3, [r2, r3]
	lsls	r1, r1, #1
	adds	r1, r1, r3
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, fp
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_08153b9c
	adds	r3, r7, #3
.L_08153b9c:
	asrs	r3, r3, #2
	adds	r3, #30
	cmp	sl, r3
	ble.n	.L_08153bc8
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	negs	r2, r2
	asrs	r2, r2, #8
	ldr	r1, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	negs	r1, r1
	asrs	r1, r1, #8
	ldr	r0, [r6, #8]
	adds	r3, r3, r1
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	negs	r0, r0
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r6, #20]
.L_08153bc8:
	ldr	r2, [sp, #8]
	adds	r7, #1
	lsls	r3, r2, #3
	mov	r2, sl
	add	r8, r3
	lsls	r3, r2, #3
	add	r9, r3
	adds	r6, #28
	cmp	r7, #64
	beq.n	.L_08153bde
	b.n	.L_08153ab8
.L_08153bde:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	sl, r3
	mov	r2, sl
	cmp	r2, #160
	beq.n	.L_08153bfc
	b.n	.L_08153a8a
.L_08153bfc:
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0000014f
	.4byte 0x02010000
	.4byte 0x08143001
	.2byte 0x7410
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #72
	str	r0, [sp, #44]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	ldr	r2, [r5, #96]
	movs	r0, #0
	str	r2, [sp, #40]
	mov	r9, r1
	bl	sub_081435e0
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #588]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #136
	lsls	r1, r1, #6
	add	r1, r9
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #572]
	bl	sub_08157cf4
	ldr	r0, [pc, #568]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #564]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2103
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #19
	str	r3, [sp, #28]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	movs	r4, #0
	str	r5, [sp, #32]
	mov	sl, r4
	mov	r8, r4
	mov	r5, r9
.L_08153caa:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r7, r0, #0
	ands	r7, r3
	bl	sub_08014878
	mov	r1, r8
	adds	r6, r0, #0
	movs	r3, #255
	str	r1, [r5, #0]
	str	r1, [r5, #4]
	str	r1, [r5, #8]
	mov	r0, sl
	movs	r1, #6
	ands	r6, r3
	bl	sub_08002064
	cmp	r0, #5
	bne.n	.L_08153cde
	mov	r2, r8
	str	r2, [r5, #12]
	str	r2, [r5, #16]
	b.n	.L_08153cfa
.L_08153cde:
	adds	r0, r7, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r5, #12]
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r5, #16]
.L_08153cfa:
	movs	r4, #1
	add	sl, r4
	mov	r3, r8
	mov	r1, sl
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_08153caa
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #400]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #0
	str	r2, [sp, #36]
.L_08153d30:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	ldr	r4, [sp, #44]
	str	r3, [sp, #20]
	movs	r3, #0
	str	r3, [sp, #24]
	ldr	r3, [r4, #20]
	cmp	r3, #0
	bne.n	.L_08153d46
	b.n	.L_08153e66
.L_08153d46:
	ldr	r1, [sp, #20]
	movs	r3, #0
	adds	r1, #12
	movs	r4, #36
	movs	r2, #48
	str	r1, [sp, #16]
	str	r3, [sp, #12]
	str	r4, [sp, #8]
	add	r2, sp
	mov	r8, r2
.L_08153d5a:
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #44]
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	bl	sub_08014de4
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	bl	sub_080156e8
	ldr	r3, [r5, #8]
	mov	r4, r8
	str	r3, [r4, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r4, #4]
	mov	r0, r8
	ldr	r3, [r5, #16]
	str	r3, [r4, #8]
	bl	sub_08015128
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #36]
	lsls	r3, r1, #3
	cmp	r2, r3
	blt.n	.L_08153e4a
	adds	r3, #40
	cmp	r2, r3
	bge.n	.L_08153e4a
	ldr	r4, [sp, #12]
	movs	r3, #0
	mov	sl, r3
	lsls	r3, r4, #3
	subs	r3, r3, r4
	add	r7, sp, #60
	lsls	r3, r3, #2
	mov	r1, r9
	mov	fp, r7
	adds	r6, r3, r1
.L_08153dac:
	ldr	r0, [r6, #24]
	movs	r1, #6
	bl	sub_08002054
	adds	r5, r0, #0
	cmp	r5, #5
	ble.n	.L_08153dbc
	movs	r5, #5
.L_08153dbc:
	adds	r0, r6, #0
	mov	r1, fp
	bl	sub_0815e1ec
	mov	r2, fp
	ldr	r3, [r2, #0]
	asrs	r3, r3, #1
	str	r3, [r2, #0]
	mov	r3, sl
	cmp	r3, #5
	bne.n	.L_08153e06
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #4
	bl	sub_08138058
	lsls	r1, r5, #3
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r1, r1, r5
	movs	r0, #24
	lsls	r1, r1, #7
	movs	r4, #136
	lsls	r4, r4, #6
	add	r1, r9
	str	r0, [sp, #0]
	movs	r0, #48
	adds	r1, r1, r4
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #36
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe018
.L_08153e06:
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #60
	lsls	r2, r2, #2
	bl	sub_08138058
	lsls	r1, r5, #3
	adds	r1, r1, r5
	lsls	r1, r1, #7
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, r9
	ldr	r3, [r7, #4]
	adds	r1, r1, r2
	ldr	r2, [r7, #0]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #36
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
	movs	r1, #1
	add	sl, r1
	adds	r3, #1
	mov	r2, sl
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r2, #6
	bne.n	.L_08153dac
.L_08153e4a:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #24]
	adds	r3, #6
	adds	r4, #2
	adds	r1, #1
	str	r3, [sp, #12]
	str	r4, [sp, #8]
	str	r1, [sp, #24]
	ldr	r2, [sp, #44]
	ldr	r3, [r2, #20]
	cmp	r1, r3
	beq.n	.L_08153e66
	b.n	.L_08153d5a
.L_08153e66:
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #36]
	adds	r3, #1
	str	r3, [sp, #36]
	cmp	r3, #96
	beq.n	.L_08153e84
	b.n	.L_08153d30
.L_08153e84:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000161
	.4byte 0x0000012d
	.4byte 0x0000017f
	.4byte 0x03000730
	.4byte 0x08143001
