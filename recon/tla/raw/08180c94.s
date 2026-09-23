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
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118010, 0x08118010
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143b20, 0x08143b20
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08179e6c, 0x08179e6c
	.set sub_08180600, 0x08180600
	.set sub_08180d8a, 0x08180d8a
	.set sub_08180e38, 0x08180e38
	.set sub_08181846, 0x08181846
	.set sub_081818a6, 0x081818a6
	.set sub_08181ed4, 0x08181ed4
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08180c94
	.thumb_func
Func_08180c94:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r0, [sp, #32]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	str	r0, [sp, #28]
	ldr	r2, [r5, #92]
	mov	sl, r2
	bl	sub_0813ba50
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #68
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #4
	strh	r3, [r2, #0]
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #128
	adds	r2, #82
	lsls	r1, r1, #2
	strh	r3, [r2, #0]
	adds	r1, #150
	movs	r2, #3
	movs	r0, #1
	bl	sub_08152404
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, sl
	ldr	r0, [r3, #0]
	b.n	.L_08180cfc
	.4byte 0x00001080
	.4byte 0x00002737
	.2byte 0x1010
	.2byte 0x0000
.L_08180cfc:
	bl	sub_0814cc4c
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #656]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	str	r5, [sp, #20]
	bl	sub_08157cf4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #620]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #180
	add	r2, sl
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	movs	r3, #0
	add	r2, sl
	str	r3, [r2, #0]
	mov	fp, r3
	ldr	r3, [pc, #592]
	str	r3, [sp, #8]
.L_08180d5a:
	mov	r4, fp
	cmp	r4, #0
	bne.n	sub_08180d8a
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #564]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #556]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4658
	cmp	r0, #56
	bne.n	.L_08180d96
	movs	r0, #140
	bl	sub_081c0010
.L_08180d96:
	movs	r6, #0
.L_08180d98:
	ldr	r3, [pc, #536]
	ldrb	r3, [r3, r6]
	cmp	fp, r3
	bne.n	.L_08180dfa
	ldr	r4, [sp, #32]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_08180de8
	movs	r7, #128
	lsls	r7, r7, #10
	movs	r5, #36
.L_08180db2:
	ldr	r2, [sp, #32]
	movs	r1, #1
	ldrsh	r0, [r5, r2]
	movs	r3, #120
	str	r3, [sp, #4]
	movs	r3, #128
	adds	r2, r7, #0
	lsls	r3, r3, #12
	str	r7, [sp, #0]
	bl	sub_0815f000
	ldr	r4, [sp, #32]
	movs	r3, #7
	ldrsh	r0, [r5, r4]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r4, [sp, #32]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08180db2
.L_08180de8:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_08180dfa:
	adds	r6, #1
	cmp	r6, #7
	bne.n	.L_08180d98
	mov	r0, fp
	cmp	r0, #79
	bgt.n	.L_08180e08
	b.n	.L_08180f20
.L_08180e08:
	cmp	r0, #80
	bne.n	sub_08180e38
	ldr	r3, [pc, #424]
	movs	r2, #1
	movs	r6, #0
	negs	r2, r2
.L_08180e14:
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_08180e14
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, sl
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r1, #240
	ldr	r3, [pc, #396]
	ldr	r0, [sp, #28]
	lsls	r1, r1, #6
	ldr	r2, [pc, #396]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #80
	cmp	r3, #7
	bhi.n	.L_08180ed0
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #28
	lsls	r3, r3, #16
	str	r3, [sp, #16]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	ldr	r4, [sp, #8]
	adds	r3, #80
	lsls	r3, r3, #16
	str	r3, [sp, #12]
	movs	r3, #15
	mov	r9, r3
	ldr	r0, [pc, #352]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	movs	r2, #0
	lsls	r3, r3, #2
	mov	r8, r2
	adds	r7, r3, r0
.L_08180e70:
	bl	sub_08014878
	movs	r5, #31
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	mov	r2, r9
	ldr	r3, [sp, #16]
	ands	r0, r2
	subs	r0, #8
	lsls	r0, r0, #16
	adds	r0, r3, r0
	str	r0, [r7, #0]
	bl	sub_08014878
	mov	r4, r9
	ands	r0, r4
	ldr	r2, [sp, #12]
	subs	r0, #8
	lsls	r0, r0, #16
	adds	r0, r2, r0
	str	r0, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #8
	bne.n	.L_08180e70
.L_08180ed0:
	ldr	r5, [pc, #240]
	movs	r6, #0
.L_08180ed4:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_08180f18
	movs	r1, #3
	bl	sub_08002054
	adds	r1, r0, #0
	lsls	r1, r1, #11
	movs	r0, #224
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsls	r0, r0, #3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	add	r1, sl
	adds	r1, r1, r0
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #48
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #184]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08180f18:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L_08180ed4
.L_08180f20:
	mov	r0, fp
	cmp	r0, #91
	bgt.n	.L_08180ff4
	ldr	r3, [pc, #164]
	movs	r1, #0
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	str	r4, [sp, #48]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #32
	strb	r3, [r2, #23]
	movs	r3, #136
	lsls	r3, r3, #8
	strh	r3, [r2, #18]
	movs	r3, #255
	add	r4, sp, #52
	lsls	r3, r3, #16
	strb	r1, [r2, #22]
	str	r1, [r4, #12]
	str	r3, [r4, #4]
	cmp	r0, #63
	bgt.n	.L_08180f72
	mov	r3, fp
	cmp	r0, #0
	bge.n	.L_08180f5e
	adds	r3, #3
.L_08180f5e:
	asrs	r3, r3, #2
	movs	r2, #152
	subs	r2, r2, r3
	lsls	r2, r2, #16
	ldr	r0, [pc, #104]
	str	r2, [r4, #0]
	mov	r2, fp
	lsls	r3, r2, #17
	adds	r3, r3, r0
	b.n	.L_08180fde
.L_08180f72:
	mov	r1, fp
	subs	r1, #64
	cmp	r1, #63
	bhi.n	.L_08180fd4
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08180f84
	mov	r2, fp
	subs	r2, #49
.L_08180f84:
	asrs	r2, r2, #4
	movs	r3, #136
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #15
	adds	r3, r3, r2
	b.n	.L_08180fde
	movs	r0, r0
	.4byte 0x0000013e
	.4byte 0x08143001
	.4byte 0xfffffd80
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x081995ec
	.4byte 0x02014018
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02014000
	.4byte 0xfffff800
	.4byte 0x08196ee8
	.2byte 0x0000
	.2byte 0xffc0
.L_08180fd4:
	.2byte 0x2388
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #15
.L_08180fde:
	str	r3, [r4, #8]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	adds	r1, r4, #0
	add	r2, sp, #44
	movs	r3, #0
	bl	sub_08020010
.L_08180ff4:
	mov	r3, fp
	subs	r3, #56
	cmp	r3, #35
	bls.n	.L_08180ffe
	b.n	.L_08181158
.L_08180ffe:
	mov	r4, fp
	lsls	r3, r4, #1
	movs	r6, #128
	subs	r3, #104
	lsls	r6, r6, #15
	movs	r5, #128
	cmp	r3, #64
	bgt.n	.L_0818101c
	lsls	r5, r3, #1
	movs	r2, #128
	ldr	r0, [pc, #416]
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
.L_0818101c:
	movs	r3, #160
	lsls	r3, r3, #15
	asrs	r2, r3, #16
	ldr	r0, [pc, #400]
	asrs	r1, r6, #16
	adds	r3, r5, #0
	bl	sub_0818caa8
	mov	r4, fp
	cmp	r4, #56
	bne.n	.L_08181080
	movs	r6, #0
	movs	r7, #0
	mov	r5, sl
.L_08181038:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_08181064
	ldr	r3, [r5, #4]
	negs	r3, r3
	str	r3, [r5, #4]
.L_08181064:
	adds	r6, #1
	str	r7, [r5, #24]
	subs	r7, #4
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L_08181038
	movs	r1, #142
	lsls	r1, r1, #7
	ldr	r0, [pc, #320]
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_08181080:
	movs	r0, #32
	bl	sub_08014dac
	mov	r9, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #0
	adds	r7, r0, #0
	ldr	r2, [pc, #296]
	str	r3, [r7, #20]
	ldr	r3, [sp, #36]
	mov	r0, r9
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #284]
	str	r0, [r7, #12]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	orrs	r3, r2
	str	r3, [sp, #36]
	movs	r3, #142
	lsls	r3, r3, #7
	add	r2, sp, #36
	add	r3, sl
	str	r3, [r2, #4]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r3, [pc, #260]
	str	r2, [r7, #16]
	str	r3, [r7, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r6, #0
	mov	r8, r2
	mov	r5, sl
.L_081810cc:
	ldr	r2, [r5, #4]
	ldr	r1, [r5, #24]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #1
	adds	r1, #2
	adds	r3, r3, r2
	str	r1, [r5, #24]
	str	r3, [r5, #0]
	cmp	r1, #47
	bhi.n	.L_08181144
	bl	sub_08014de4
	movs	r1, #3
	ands	r1, r6
	ldr	r4, [pc, #220]
	lsls	r0, r1, #17
	movs	r3, #132
	adds	r1, #2
	movs	r2, #160
	lsls	r3, r3, #15
	lsls	r2, r2, #15
	lsls	r1, r1, #16
	adds	r0, r0, r3
	subs	r1, r2, r1
	adds	r0, r0, r4
	adds	r1, r1, r4
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	mov	r1, r8
	mov	r2, r8
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r0, [r5, #0]
	bl	sub_080150e4
	ldr	r2, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002096
	movs	r1, #128
	add	r0, r8
	lsls	r1, r1, #10
	mov	r2, r8
	bl	sub_080151e4
	ldr	r0, [pc, #148]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08181144:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #6
	bne.n	.L_081810cc
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r9
	bl	sub_08013164
.L_08181158:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #8]
	movs	r4, #1
	add	fp, r4
	adds	r3, #8
	mov	r0, fp
	str	r3, [sp, #8]
	cmp	r0, #108
	beq.n	.L_08181188
	b.n	.L_08180d5a
.L_08181188:
	ldr	r0, [pc, #68]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_08020048
	bl	sub_08143bb8
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x000000c9
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0xffc00000
	.4byte 0x081991b0
	.2byte 0x3001
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
	mov	sl, r0
	ldr	r0, [r3, #92]
	sub	sp, #68
	str	r0, [sp, #52]
	movs	r0, #0
	ldr	r3, [r3, #96]
	movs	r5, #7
	mov	r8, r3
	bl	sub_08143a88
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #50
	strh	r3, [r2, #0]
	movs	r1, #0
	mov	fp, r1
	movs	r0, #0
.L_08181210:
	movs	r1, #0
	subs	r4, r0, #3
	b.n	.L_08181220
	movs	r0, r0
	.4byte 0x00000100
	.2byte 0x0410
	.2byte 0x0000
.L_08181220:
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181238
	cmp	r1, #7
	bgt.n	.L_0818123c
	cmp	r4, #3
	bhi.n	.L_08181256
	adds	r3, r0, #0
	adds	r2, r1, #0
	adds	r3, #9
	b.n	.L_0818125a
.L_08181238:
	cmp	r1, #7
	bgt.n	.L_08181248
.L_0818123c:
	cmp	r0, #11
	bgt.n	.L_08181256
	adds	r2, r1, #0
	ands	r2, r5
	adds	r3, r0, #0
	b.n	.L_0818125a
.L_08181248:
	cmp	r4, #3
	bhi.n	.L_08181256
	adds	r2, r1, #0
	adds	r3, r0, #0
	ands	r2, r5
	adds	r3, #9
	b.n	.L_0818125a
.L_08181256:
	movs	r2, #0
	movs	r3, #0
.L_0818125a:
	lsls	r3, r3, #3
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r2, r3, #1
	lsls	r2, r2, #8
	orrs	r2, r3
	ldr	r3, [pc, #104]
	lsls	r2, r2, #16
	add	r3, fp
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	adds	r1, #1
	movs	r3, #2
	add	fp, r3
	cmp	r1, #16
	bne.n	.L_08181220
	adds	r0, #1
	cmp	r0, #16
	bne.n	.L_08181210
	ldr	r3, [pc, #80]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #44
	str	r3, [r2, #0]
	ldr	r1, [pc, #52]
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #52]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	movs	r3, #1
	b.n	.L_081812d8
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00002888
	.4byte 0x00003737
	.4byte 0x00002033
	.4byte 0x06003800
	.2byte 0xd800
	.2byte 0xffff
.L_081812d8:
	.2byte 0x2201
	ldr	r0, [pc, #200]
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #40]
	bl	sub_081963ec
	ldr	r4, [sp, #52]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	adds	r5, #188
	lsls	r1, r1, #7
	adds	r2, r4, r0
	ldr	r5, [r5, #0]
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #140]
	str	r5, [sp, #44]
	bl	sub_080145a8
	movs	r2, #172
	lsls	r2, r2, #15
	ldr	r1, [sp, #52]
	str	r2, [sp, #36]
	movs	r2, #146
	movs	r3, #128
	mov	r0, sp
	lsls	r2, r2, #7
	lsls	r3, r3, #13
	movs	r4, #0
	adds	r0, #56
	adds	r2, r1, r2
	str	r3, [sp, #32]
	str	r4, [sp, #48]
	str	r0, [sp, #12]
	str	r2, [sp, #20]
.L_08181340:
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r4, sl
	ldr	r5, [r3, #48]
	ldr	r1, [sp, #12]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_0815e20c
	ldr	r0, [sp, #48]
	cmp	r0, #31
	bgt.n	.L_0818136c
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08181366
	ldrh	r3, [r5, #54]
	subs	r3, #128
	b.n	.L_0818136a
.L_08181366:
	ldrh	r3, [r5, #54]
	adds	r3, #128
.L_0818136a:
	strh	r3, [r5, #54]
.L_0818136c:
	ldr	r2, [sp, #48]
	cmp	r2, #108
	blt.n	.L_08181384
	movs	r4, #124
	ldr	r1, [pc, #40]
	movs	r3, #128
	subs	r2, r4, r2
	lsls	r3, r3, #19
	subs	r2, #1
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_08181384:
	ldr	r0, [sp, #48]
	cmp	r0, #0
	bne.n	.L_08181422
	mov	r1, sl
	ldr	r6, [r1, #4]
	cmp	r6, #0
	bne.n	.L_081813ac
	movs	r2, #172
	movs	r3, #128
	lsls	r2, r2, #15
	lsls	r3, r3, #13
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	b.n	.L_081813b4
	.4byte 0x00001000
	.4byte 0x000000b0
	.2byte 0x3001
	.2byte 0x0814
.L_081813ac:
	ldr	r4, [pc, #188]
	ldr	r0, [pc, #192]
	str	r4, [sp, #36]
	str	r0, [sp, #32]
.L_081813b4:
	movs	r1, #0
	mov	fp, r1
	b.n	.L_081813be
.L_081813ba:
	mov	r2, sl
	ldr	r6, [r2, #4]
.L_081813be:
	mov	r4, fp
	lsls	r3, r4, #3
	ldr	r0, [sp, #52]
	subs	r3, r3, r4
	lsls	r3, r3, #2
	adds	r5, r0, r3
	cmp	r6, #0
	bne.n	.L_081813da
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	b.n	.L_081813e2
.L_081813da:
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #0]
	ldr	r3, [pc, #144]
.L_081813e2:
	str	r3, [r5, #12]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #4]
	mov	r3, sl
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #136]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	add	r3, fp
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_081813fe
	movs	r3, #1
.L_081813fe:
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	ldr	r4, [pc, #112]
	adds	r3, #255
	ands	r3, r0
	movs	r0, #1
	adds	r3, r3, r4
	add	fp, r0
	str	r3, [r5, #16]
	mov	r1, fp
	movs	r3, #0
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	cmp	r1, #7
	bne.n	.L_081813ba
.L_08181422:
	ldr	r2, [sp, #48]
	cmp	r2, #24
	bgt.n	.L_08181480
	ldr	r4, [sp, #36]
	movs	r3, #128
	asrs	r0, r4, #16
	lsls	r3, r3, #19
	negs	r2, r0
	adds	r3, #40
	lsls	r2, r2, #8
	str	r2, [r3, #0]
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0818145a
	ldr	r3, [pc, #36]
	adds	r2, r0, #0
	adds	r2, #120
	lsls	r2, r2, #8
	orrs	r2, r3
	movs	r1, #128
	movs	r3, #128
	lsls	r1, r1, #19
	lsls	r3, r3, #19
	adds	r1, #64
	adds	r3, #66
	strh	r2, [r1, #0]
	b.n	.L_0818149c
.L_0818145a:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r2, r0, #0
	adds	r3, #64
	adds	r2, #240
	b.n	.L_08181498
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0xffaa0000
	.4byte 0xfff00000
	.4byte 0xfff40000
	.4byte 0x081995f3
	.2byte 0x8000
	.2byte 0xffff
.L_08181480:
	.2byte 0x9c09
	movs	r2, #128
	asrs	r3, r4, #16
	lsls	r2, r2, #19
	negs	r3, r3
	adds	r2, #40
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r2, [pc, #32]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #64
.L_08181498:
	strh	r2, [r3, #0]
	adds	r3, #2
.L_0818149c:
	strh	r2, [r3, #0]
	ldr	r1, [sp, #32]
	ldr	r0, [sp, #36]
	movs	r3, #52
	muls	r3, r1
	subs	r0, r0, r1
	str	r0, [sp, #36]
	cmp	r3, #0
	bge.n	.L_081814b8
	adds	r3, #63
	b.n	.L_081814b8
	movs	r0, r0
	.2byte 0x00f0
	.2byte 0x0000
.L_081814b8:
	asrs	r3, r3, #6
	str	r3, [sp, #32]
	ldr	r3, [sp, #48]
	movs	r2, #0
	str	r2, [sp, #28]
	cmp	r3, #105
	ble.n	.L_081814cc
	movs	r4, #1
	str	r4, [sp, #28]
	b.n	.L_081814e2
.L_081814cc:
	ldr	r0, [sp, #48]
	cmp	r0, #21
	ble.n	.L_081814e2
	subs	r0, #22
	movs	r1, #12
	bl	sub_08002064
	cmp	r0, #3
	ble.n	.L_081814e2
	movs	r1, #1
	str	r1, [sp, #28]
.L_081814e2:
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181520
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne.n	.L_08181502
	ldr	r4, [sp, #52]
	movs	r3, #88
	movs	r2, #224
	lsls	r2, r2, #3
	str	r3, [sp, #0]
	movs	r3, #96
	adds	r1, r4, r2
	str	r3, [sp, #4]
	b.n	.L_08181512
.L_08181502:
	movs	r3, #88
	ldr	r2, [sp, #52]
	str	r3, [sp, #0]
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #6
	adds	r1, r2, r3
.L_08181512:
	mov	r0, r8
	movs	r2, #32
	movs	r3, #0
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01f
.L_08181520:
	ldr	r0, [sp, #28]
	cmp	r0, #0
	bne.n	.L_08181544
	movs	r3, #88
	ldr	r2, [sp, #52]
	str	r3, [sp, #0]
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #8
	movs	r3, #0
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00d
.L_08181544:
	movs	r3, #88
	ldr	r2, [sp, #52]
	str	r3, [sp, #0]
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #6
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #8
	movs	r3, #0
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b0a
	ldr	r4, [sp, #44]
	ldr	r1, [sp, #52]
	movs	r2, #146
	movs	r0, #0
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	str	r0, [sp, #8]
	ldr	r5, [sp, #52]
	adds	r1, r1, r2
	mov	fp, r0
	mov	r9, r1
.L_0818157c:
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #48]
	adds	r3, #22
	cmp	r0, r3
	bge.n	.L_08181594
	cmp	r0, #21
	ble.n	.L_0818158c
	b.n	sub_08181846
.L_0818158c:
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_08181594
	b.n	sub_08181846
.L_08181594:
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #48]
	adds	r3, #26
	cmp	r2, r3
	blt.n	.L_081815b6
	ldr	r4, [r5, #24]
	cmp	r4, #0
	bne.n	.L_081815b8
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	subs	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	b.n	.L_081815b8
.L_081815b6:
	ldr	r4, [r5, #24]
.L_081815b8:
	cmp	r4, #4
	ble.n	.L_081815be
	b.n	sub_08181846
.L_081815be:
	cmp	r4, #0
	ble.n	.L_081815c8
	adds	r3, r4, #1
	str	r3, [r5, #24]
	adds	r4, r3, #0
.L_081815c8:
	ldr	r3, [r5, #8]
	cmp	r3, #1
	bne.n	.L_081816b4
	cmp	r4, #0
	bne.n	.L_081816ba
	mov	r3, sl
	ldr	r6, [r3, #4]
	movs	r7, #0
	cmp	r6, #0
	bne.n	.L_081815f2
	ldr	r1, [sp, #12]
	ldr	r0, [r5, #0]
	ldr	r2, [r1, #0]
	asrs	r3, r0, #16
	lsrs	r1, r2, #31
	adds	r2, r2, r1
	asrs	r2, r2, #1
	adds	r2, #16
	cmp	r3, r2
	bge.n	.L_08181608
	b.n	.L_08181606
.L_081815f2:
	ldr	r1, [sp, #12]
	ldr	r0, [r5, #0]
	ldr	r2, [r1, #0]
	asrs	r3, r0, #16
	lsrs	r1, r2, #31
	adds	r2, r2, r1
	asrs	r2, r2, #1
	adds	r2, #48
	cmp	r3, r2
	ble.n	.L_08181608
.L_08181606:
	movs	r7, #1
.L_08181608:
	cmp	r7, #1
	bne.n	.L_081816c0
	ldr	r3, [sp, #52]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	mov	r0, fp
	movs	r3, #4
	str	r3, [r2, #0]
	cmp	r0, #6
	bne.n	.L_08181634
	mov	r2, sl
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0818165c
.L_08181634:
	mov	r4, sl
	mov	r1, fp
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	lsls	r3, r1, #2
	add	r3, fp
	lsls	r3, r3, #1
	adds	r3, #110
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #10
	movs	r1, #1
	lsls	r3, r3, #12
	str	r2, [sp, #0]
	bl	sub_0815f000
	movs	r0, #133
	bl	sub_081c0010
.L_0818165c:
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #5
	bl	sub_0814cd48
	movs	r3, #1
	str	r3, [r5, #24]
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181696
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #2
	ldrsh	r2, [r5, r1]
	asrs	r3, r3, #1
	subs	r3, r3, r2
	str	r3, [r5, #0]
	adds	r0, r3, #0
	ldr	r6, [r4, #4]
	movs	r4, #1
	b.n	.L_081816c0
.L_08181696:
	ldr	r2, [sp, #12]
	mov	r0, sl
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	asrs	r3, r3, #1
	subs	r3, r3, r2
	adds	r3, #128
	str	r3, [r5, #0]
	movs	r4, #1
	ldr	r6, [r0, #4]
	adds	r0, r3, #0
	b.n	.L_081816c0
.L_081816b4:
	mov	r1, sl
	ldr	r6, [r1, #4]
	b.n	.L_081816be
.L_081816ba:
	mov	r2, sl
	ldr	r6, [r2, #4]
.L_081816be:
	ldr	r0, [r5, #0]
.L_081816c0:
	cmp	r6, #0
	bne.n	.L_08181784
	cmp	r4, #0
	bne.n	.L_08181738
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	asrs	r2, r0, #16
	movs	r1, #12
	movs	r0, #51
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #24]
	mov	r1, r9
	subs	r2, #111
	adds	r3, #24
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #51
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #24]
	adds	r2, #17
	adds	r3, #96
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #17
	movs	r0, #12
	ldr	r1, [sp, #20]
	subs	r2, #128
	adds	r3, #24
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2106
	ldrsh	r3, [r5, r1]
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r4, #17
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, #96
	mov	r0, r8
	ldr	r1, [sp, #20]
	b.n	.L_0818177c
.L_08181738:
	ldr	r2, [sp, #56]
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r2, r2, r0
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	subs	r2, #111
	adds	r3, #24
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #24]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a0e
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #0]
	asrs	r2, r2, #1
	adds	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	adds	r2, #17
	adds	r3, #96
	mov	r0, r8
	mov	r1, r9
.L_0818177c:
	ldr	r4, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe060
.L_08181784:
	cmp	r4, #0
	bne.n	.L_081817fe
	asrs	r2, r0, #16
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r1, #51
	movs	r4, #12
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #16]
	adds	r3, #24
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #51
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #16]
	subs	r2, #128
	adds	r3, #96
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #17
	movs	r0, #12
	ldr	r1, [sp, #20]
	adds	r2, #51
	adds	r3, #24
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #16]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #17
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #77
	adds	r3, #96
	mov	r0, r8
	ldr	r1, [sp, #20]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe023
.L_081817fe:
	ldr	r2, [sp, #56]
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r2, r2, r0
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	adds	r3, #24
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #16]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a0e
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #0]
	asrs	r2, r2, #1
	adds	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #128
	adds	r3, #96
	mov	r0, r8
	mov	r1, r9
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9802
	movs	r1, #1
	add	fp, r1
	adds	r0, #12
	mov	r2, fp
	str	r0, [sp, #8]
	adds	r5, #28
	cmp	r2, #7
	beq.n	.L_0818185a
	b.n	.L_0818157c
.L_0818185a:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne.n	sub_081818a6
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181888
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r3, #13
	ldr	r2, [sp, #52]
	str	r3, [sp, #4]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #48
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #88
	movs	r3, #29
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00e
.L_08181888:
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r3, #13
	ldr	r2, [sp, #52]
	str	r3, [sp, #4]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #48
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #26
	movs	r3, #29
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xf7c1
	.2byte 0xfe27
	.2byte 0x2001
	movs	r1, #4
	bl	sub_08158ce0
	movs	r1, #240
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #48]
	adds	r2, #1
	str	r2, [sp, #48]
	cmp	r2, #124
	beq.n	.L_081818d2
	b.n	.L_08181340
.L_081818d2:
	ldr	r2, [pc, #40]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #68
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #68
	b.n	.L_08181904
	movs	r0, r0
	.4byte 0x00001088
	.2byte 0x3001
	.2byte 0x0814
.L_08181904:
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
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #96]
	sub	sp, #64
	str	r3, [sp, #40]
	mov	sl, r0
	ldr	r3, [r0, #4]
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08181940
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	bl	sub_08143a88
	b.n	.L_0818194a
.L_08181940:
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	bl	sub_08143b20
.L_0818194a:
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r0, [pc, #36]
	movs	r2, #1
	movs	r3, #1
	ldr	r1, [pc, #32]
	bl	sub_08157cf4
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181988
	movs	r0, #104
	movs	r1, #35
	bl	sub_081963ec
	b.n	.L_08181990
	.4byte 0x00000100
	.4byte 0x00000610
	.4byte 0x000000f5
	.2byte 0x0000
	.2byte 0x0201
.L_08181988:
	movs	r0, #104
	movs	r1, #39
	bl	sub_081963ec
.L_08181990:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #32]
	add	r2, r8
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r8
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #520]
	bl	sub_080145a8
	movs	r3, #0
	str	r3, [sp, #28]
	str	r3, [sp, #24]
	str	r3, [sp, #20]
	mov	r2, sp
	adds	r2, #52
	mov	r1, sl
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	adds	r1, r2, #0
	str	r2, [sp, #16]
	bl	sub_0815e20c
	movs	r3, #0
	mov	r9, r3
.L_081819d6:
	movs	r4, #0
	mov	r1, r9
	str	r4, [sp, #12]
	cmp	r1, #0
	bne.n	.L_08181a40
	movs	r0, #206
	bl	sub_081c0010
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181a00
	movs	r3, #128
	mov	r4, r8
	lsls	r3, r3, #14
	str	r3, [r4, #0]
	ldr	r3, [pc, #456]
	str	r3, [r4, #4]
	ldr	r3, [pc, #456]
	str	r3, [r4, #8]
	b.n	.L_08181a0e
.L_08181a00:
	ldr	r3, [pc, #452]
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [pc, #440]
	str	r3, [r1, #4]
	ldr	r3, [pc, #440]
	str	r3, [r1, #8]
.L_08181a0e:
	movs	r3, #0
	mov	r2, r8
	str	r3, [r2, #20]
	str	r3, [r2, #24]
	movs	r3, #192
	lsls	r3, r3, #13
	str	r3, [sp, #24]
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181a30
	ldr	r1, [sp, #16]
	movs	r2, #240
	ldr	r3, [r1, #0]
	lsls	r2, r2, #12
	adds	r3, #176
	b.n	.L_08181a3a
.L_08181a30:
	ldr	r4, [sp, #16]
	ldr	r1, [pc, #408]
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #408]
	adds	r3, r3, r1
.L_08181a3a:
	lsls	r3, r3, #16
	str	r3, [sp, #28]
	str	r2, [sp, #20]
.L_08181a40:
	mov	r3, r9
	cmp	r3, #61
	ble.n	.L_08181a6a
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181a5a
	ldr	r1, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r1, r1, r2
	str	r1, [sp, #28]
	b.n	.L_08181a62
.L_08181a5a:
	ldr	r3, [sp, #28]
	ldr	r4, [pc, #372]
	adds	r3, r3, r4
	str	r3, [sp, #28]
.L_08181a62:
	ldr	r1, [sp, #24]
	ldr	r2, [pc, #368]
	adds	r1, r1, r2
	str	r1, [sp, #24]
.L_08181a6a:
	ldr	r4, [sp, #20]
	ldr	r3, [sp, #28]
	subs	r3, r3, r4
	str	r3, [sp, #28]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_08181a7e
	adds	r3, #63
.L_08181a7e:
	ldr	r1, [sp, #28]
	asrs	r3, r3, #6
	asrs	r2, r1, #16
	str	r3, [sp, #20]
	cmp	r2, #120
	ble.n	.L_08181a92
	subs	r2, #120
	str	r2, [sp, #12]
	movs	r2, #120
	b.n	.L_08181a9a
.L_08181a92:
	cmp	r2, #0
	bge.n	.L_08181a9a
	str	r2, [sp, #12]
	movs	r2, #0
.L_08181a9a:
	movs	r3, #128
	lsls	r3, r3, #19
	negs	r2, r2
	adds	r3, #40
	lsls	r2, r2, #8
	str	r2, [r3, #0]
	mov	r3, r9
	subs	r3, #24
	cmp	r3, #39
	bhi.n	.L_08181ad6
	mov	r2, sl
	ldr	r3, [r2, #4]
	movs	r2, #64
	cmp	r3, #0
	beq.n	.L_08181aba
	movs	r2, #5
.L_08181aba:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #24]
	movs	r1, #59
	str	r1, [sp, #0]
	movs	r1, #89
	adds	r2, r2, r3
	str	r1, [sp, #4]
	asrs	r3, r4, #16
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #268]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01f
.L_08181ad6:
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08181aee
	mov	r2, r9
	lsls	r3, r2, #1
	adds	r2, r3, #0
	adds	r2, #16
	cmp	r2, #64
	ble.n	.L_08181afc
	movs	r2, #64
	b.n	.L_08181afc
.L_08181aee:
	mov	r3, r9
	lsls	r2, r3, #1
	movs	r3, #57
	subs	r2, r3, r2
	cmp	r2, #8
	bgt.n	.L_08181afc
	movs	r2, #9
.L_08181afc:
	ldr	r1, [sp, #24]
	ldr	r4, [sp, #12]
	asrs	r3, r1, #16
	movs	r1, #55
	str	r1, [sp, #0]
	movs	r1, #88
	adds	r2, r2, r4
	str	r1, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #208]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2600
	movs	r7, #0
	mov	r5, r8
.L_08181b1c:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_08181c04
	cmp	r3, #1
	bne.n	.L_08181b2e
	str	r7, [r5, #12]
	str	r7, [r5, #16]
.L_08181b2e:
	ldr	r2, [r5, #24]
	cmp	r2, #24
	bne.n	.L_08181b3e
	ldr	r3, [pc, #172]
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #16]
.L_08181b3e:
	cmp	r2, #33
	bne.n	.L_08181b78
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	ldr	r3, [pc, #156]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	str	r3, [r5, #16]
	add	r2, r8
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #133
	str	r1, [sp, #20]
	bl	sub_081c0010
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r2, [r5, #24]
.L_08181b78:
	cmp	r2, #35
	bne.n	.L_08181b80
	str	r7, [r5, #12]
	str	r7, [r5, #16]
.L_08181b80:
	mov	r4, r9
	cmp	r4, #63
	ble.n	.L_08181ba6
	ldr	r1, [pc, #100]
	lsls	r3, r4, #7
	adds	r2, r3, r1
	ldr	r4, [pc, #96]
	ldr	r3, [r5, #16]
	movs	r1, #128
	adds	r3, r3, r4
	lsls	r1, r1, #3
	str	r3, [r5, #16]
	cmp	r2, r1
	ble.n	.L_08181ba0
	movs	r2, #128
	lsls	r2, r2, #3
.L_08181ba0:
	ldr	r3, [r5, #8]
	subs	r3, r3, r2
	str	r3, [r5, #8]
.L_08181ba6:
	ldr	r3, [r5, #24]
	cmp	r3, #8
	ble.n	.L_08181c04
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181bf4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	b.n	.L_08181bfa
	.4byte 0x08143001
	.4byte 0xffc00000
	.4byte 0xffffe3d0
	.4byte 0xffe00000
	.4byte 0xfffffed0
	.4byte 0xfff10000
	.4byte 0xfffe0000
	.4byte 0xfffb5556
	.4byte 0x020112e8
	.4byte 0x02010000
	.4byte 0xfffc0000
	.4byte 0xfff80000
	.4byte 0xffffe000
	.2byte 0xc000
	.2byte 0xffff
.L_08181bf4:
	.2byte 0x682b
	ldr	r2, [r5, #12]
	subs	r3, r3, r2
.L_08181bfa:
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_08181c04:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #1
	bne.n	.L_08181b1c
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	mov	r3, r9
	movs	r4, #4
	subs	r3, #50
	negs	r4, r4
	adds	r7, r0, #0
	movs	r5, #0
	str	r3, [sp, #8]
	cmp	r3, r4
	bne.n	.L_08181c32
	movs	r0, #212
	bl	sub_081c0010
.L_08181c32:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_08181c78
	movs	r0, #134
	bl	sub_081180e8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r8
	movs	r3, #12
	str	r3, [r2, #0]
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #528]
	ldr	r0, [sp, #40]
	ldr	r2, [pc, #528]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	movs	r1, #4
	bl	sub_08118088
.L_08181c78:
	mov	r3, r8
	ldr	r2, [r3, #24]
	mov	r3, r9
	subs	r3, #44
	cmp	r3, #13
	bhi.n	.L_08181c8c
	mov	r4, r8
	ldr	r3, [r4, #20]
	adds	r3, #1
	str	r3, [r4, #20]
.L_08181c8c:
	cmp	r2, #0
	bge.n	.L_08181c92
	b.n	.L_08181e20
.L_08181c92:
	movs	r6, #177
	lsls	r6, r6, #8
	mov	r1, r9
	adds	r6, #224
	cmp	r1, #63
	ble.n	.L_08181ca8
	lsls	r3, r1, #9
	movs	r2, #128
	subs	r3, r6, r3
	lsls	r2, r2, #8
	adds	r6, r3, r2
.L_08181ca8:
	cmp	r6, #0
	ble.n	.L_08181d6e
	movs	r3, #9
	str	r5, [r7, #20]
	str	r3, [r7, #0]
	add	r5, sp, #44
	mov	r3, fp
	str	r3, [r7, #12]
	str	r5, [r7, #16]
	bl	sub_08014de4
	mov	r1, r8
	ldr	r4, [sp, #12]
	ldr	r0, [r1, #0]
	lsls	r3, r4, #16
	adds	r0, r0, r3
	movs	r2, #0
	ldr	r1, [r1, #4]
	bl	sub_08015160
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08181ce0
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_08181ce0:
	mov	r3, r8
	ldr	r0, [r3, #8]
	bl	sub_080150e4
	adds	r0, r6, #0
	bl	sub_0801521c
	movs	r3, #5
	strb	r3, [r5, #1]
	ldr	r3, [pc, #392]
	movs	r4, #7
	str	r3, [r5, #4]
	ldr	r3, [pc, #388]
	strb	r4, [r5, #0]
	str	r3, [r7, #8]
	bl	sub_08014e38
	mov	r1, r8
	ldr	r0, [r1, #20]
	lsls	r0, r0, #11
	bl	sub_08002096
	ldr	r2, [pc, #372]
	asrs	r0, r0, #3
	adds	r0, r0, r2
	bl	sub_080150e4
	mov	r1, fp
	movs	r2, #4
	ldr	r0, [pc, #364]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	bl	sub_08014ea8
	ldr	r3, [pc, #352]
	mov	r4, r8
	str	r3, [r7, #8]
	movs	r3, #7
	strb	r3, [r5, #0]
	movs	r3, #6
	ldr	r0, [r4, #20]
	strb	r3, [r5, #1]
	ldr	r3, [pc, #340]
	lsls	r0, r0, #11
	str	r3, [r5, #4]
	bl	sub_08002096
	adds	r3, r0, #0
	movs	r0, #128
	asrs	r3, r3, #3
	lsls	r0, r0, #6
	subs	r0, r0, r3
	bl	sub_080150e4
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r2, #0
	ldr	r1, [pc, #312]
	bl	sub_080151e4
	ldr	r0, [pc, #296]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08181d6e:
	ldr	r1, [sp, #8]
	cmp	r1, #7
	bhi.n	.L_08181e20
	mov	r2, r9
	lsls	r3, r2, #13
	ldr	r2, [pc, #284]
	subs	r6, r2, r3
	bl	sub_08014de4
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181d94
	ldr	r0, [pc, #272]
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08181da0
.L_08181d94:
	movs	r0, #128
	lsls	r0, r0, #13
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
.L_08181da0:
	adds	r1, r6, #0
	adds	r0, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_08181dba
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_08181dba:
	movs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [pc, #224]
	add	r2, sp, #44
	str	r3, [r7, #8]
	movs	r3, #5
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	ldr	r3, [pc, #216]
	movs	r5, #172
	str	r3, [r2, #4]
	lsls	r5, r5, #7
	movs	r6, #0
	adds	r5, #80
.L_08181dd6:
	bl	sub_08014e38
	adds	r0, r5, #0
	bl	sub_080150e4
	movs	r1, #1
	ands	r1, r6
	movs	r2, #192
	lsls	r2, r2, #8
	lsls	r1, r1, #15
	adds	r1, r1, r2
	movs	r0, #128
	movs	r2, #128
	lsls	r2, r2, #8
	lsls	r0, r0, #7
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #9
	bl	sub_0801521c
	mov	r1, fp
	movs	r2, #4
	ldr	r0, [pc, #160]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	bl	sub_08014ea8
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #4
	bne.n	.L_08181dd6
.L_08181e20:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
	movs	r1, #8
	movs	r0, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r8
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	add	r9, r4
	mov	r1, r9
	cmp	r1, #90
	beq.n	.L_08181e56
	b.n	.L_081819d6
.L_08181e56:
	ldr	r0, [pc, #84]
	bl	sub_08014644
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
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x0201476b
	.4byte 0x0819962c
	.4byte 0xffffe000
	.4byte 0x08199650
	.4byte 0x08199608
	.4byte 0x0201276b
	.4byte 0x00015f90
	.4byte 0x00078e20
	.4byte 0xfff00000
	.4byte 0x08199244
	.4byte 0x0201576b
	.4byte 0x081991f0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08181ed4
	pop	{pc}
	.2byte 0x0000
