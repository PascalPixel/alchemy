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
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_080156e8, 0x080156e8
	.set sub_08015778, 0x08015778
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08118040, 0x08118040
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081180f0, 0x081180f0
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_08143354, 0x08143354
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143b20, 0x08143b20
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0816ae40, 0x0816ae40
	.set sub_0816b8f0, 0x0816b8f0
	.set sub_0816be64, 0x0816be64
	.set sub_0816bf12, 0x0816bf12
	.set sub_0816c508, 0x0816c508
	.set sub_0816cd22, 0x0816cd22
	.set sub_0816cf14, 0x0816cf14
	.set sub_0816d040, 0x0816d040
	.set sub_0816e0e6, 0x0816e0e6
	.set sub_0816e144, 0x0816e144
	.set sub_0816e65c, 0x0816e65c
	.set sub_08178680, 0x08178680
	.set sub_0818caa8, 0x0818caa8
	.set sub_0818de3c, 0x0818de3c
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969ac, 0x081969ac
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0816b6ec
Overlay_0816b6ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r0
	ldr	r0, [r3, #92]
	sub	sp, #84
	str	r0, [sp, #48]
	mov	r9, r1
	ldr	r1, [r3, #96]
	movs	r0, #0
	str	r1, [sp, #44]
	ldr	r2, [r3, #48]
	str	r2, [sp, #32]
	ldr	r5, [r3, #100]
	bl	sub_081435e0
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0816b748
	mov	r4, fp
	ldr	r1, [r4, #4]
	mov	r0, sp
	adds	r0, #60
	str	r0, [sp, #12]
	movs	r3, #3
	lsls	r1, r1, #4
	orrs	r1, r3
	ldr	r2, [sp, #12]
	add	r3, sp, #72
	mov	r0, fp
	bl	sub_0815585c
	movs	r2, #128
	ldr	r3, [pc, #8]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	b.n	.L_0816b762
	.2byte 0x0785
	.2byte 0x0000
.L_0816b748:
	mov	r2, fp
	ldr	r1, [r2, #4]
	movs	r3, #35
	lsls	r1, r1, #4
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #60
	str	r3, [sp, #12]
	mov	r0, fp
	add	r3, sp, #72
	ldr	r2, [sp, #12]
	bl	sub_0815585c
.L_0816b762:
	mov	r4, fp
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0816b774
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0816b77c
.L_0816b774:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_0816b77c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #36]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
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
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	movs	r4, #0
	movs	r0, #36
	mov	r1, r9
	str	r4, [sp, #28]
	str	r0, [sp, #24]
	cmp	r1, #1
	bne.n	.L_0816b7da
	b.n	.L_0816b7c8
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0816b7c8:
	mov	r3, fp
	ldr	r2, [r3, #20]
	cmp	r2, #4
	ble.n	.L_0816b7da
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #12
	str	r3, [sp, #24]
.L_0816b7da:
	ldr	r4, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	ldr	r0, [pc, #844]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #836]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #48]
	movs	r4, #156
	lsls	r4, r4, #6
	adds	r1, r3, r4
	ldr	r0, [pc, #820]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #816]
	movs	r1, #4
	movs	r2, #32
	movs	r3, #32
	bl	sub_08178680
	ldr	r1, [sp, #24]
	movs	r0, #0
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L_0816b822
	b.n	.L_0816bc34
.L_0816b822:
	ldr	r2, [sp, #32]
	mov	r3, sp
	adds	r2, #12
	adds	r3, #52
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	str	r0, [sp, #8]
.L_0816b830:
	mov	r4, sl
	cmp	r4, #0
	bne.n	.L_0816b872
	movs	r0, #0
	mov	r1, r9
	str	r0, [sp, #28]
	cmp	r1, #0
	bne.n	.L_0816b844
	movs	r5, #80
	b.n	.L_0816b846
.L_0816b844:
	movs	r5, #30
.L_0816b846:
	movs	r6, #32
	ldr	r4, [pc, #756]
	movs	r1, #0
	movs	r0, #0
.L_0816b84e:
	lsls	r3, r1, #3
	movs	r7, #0
	negs	r2, r5
	adds	r3, r3, r4
.L_0816b856:
	adds	r7, #1
	strb	r2, [r3, #1]
	strb	r0, [r3, #0]
	strb	r6, [r3, #2]
	subs	r2, #24
	adds	r3, #4
	cmp	r7, #2
	bne.n	.L_0816b856
	adds	r1, #1
	cmp	r1, #33
	bne.n	.L_0816b84e
	movs	r0, #179
	bl	sub_081c0010
.L_0816b872:
	mov	r2, sl
	cmp	r2, #19
	ble.n	.L_0816b892
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0816b886
	ldr	r0, [pc, #708]
	bl	sub_0815f0a0
	b.n	.L_0816b892
.L_0816b886:
	mov	r4, r9
	cmp	r4, #1
	bne.n	.L_0816b892
	ldr	r0, [pc, #696]
	bl	sub_0815f0a0
.L_0816b892:
	mov	r0, sl
	cmp	r0, #79
	bgt.n	sub_0816b8f0
	ldr	r0, [sp, #8]
	bl	sub_08002096
	ldr	r1, [sp, #12]
	lsls	r0, r0, #3
	ldr	r3, [r1, #0]
	asrs	r0, r0, #16
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r0, r0, r3
	adds	r5, r0, #0
	ldr	r0, [sp, #8]
	bl	sub_08002090
	ldr	r2, [sp, #12]
	lsls	r0, r0, #2
	ldr	r3, [r2, #4]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r4, r0, #0
	mov	r3, sl
	subs	r5, #10
	subs	r4, #24
	cmp	r3, #7
	ble.n	.L_0816b8d4
	lsls	r3, r3, #2
	subs	r3, r4, r3
	adds	r4, r3, #0
	adds	r4, #32
.L_0816b8d4:
	movs	r3, #20
	ldr	r2, [sp, #48]
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [sp, #44]
	adds	r3, r4, #0
	adds	r2, r5, #0
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2301
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_0816b902
	cmp	r0, #47
	bgt.n	.L_0816b902
	cmp	r0, #31
	bgt.n	.L_0816b926
.L_0816b902:
	ldr	r0, [pc, #584]
	movs	r1, #32
.L_0816b906:
	lsls	r3, r1, #3
	movs	r7, #0
	adds	r2, r3, r0
.L_0816b90c:
	ldrb	r3, [r2, #0]
	adds	r7, #1
	strb	r3, [r2, #8]
	ldrb	r3, [r2, #1]
	strb	r3, [r2, #9]
	ldrb	r3, [r2, #2]
	strb	r3, [r2, #10]
	adds	r2, #4
	cmp	r7, #2
	bne.n	.L_0816b90c
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L_0816b906
.L_0816b926:
	mov	r1, sl
	cmp	r1, #23
	ble.n	.L_0816b93c
	ldr	r2, [sp, #28]
	lsls	r3, r1, #4
	movs	r4, #152
	subs	r3, r2, r3
	lsls	r4, r4, #4
	adds	r4, r3, r4
	str	r4, [sp, #28]
	b.n	.L_0816b946
.L_0816b93c:
	ldr	r0, [sp, #28]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r0, r0, r1
	str	r0, [sp, #28]
.L_0816b946:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_0816b96a
	mov	r3, sl
	lsls	r3, r3, #12
	mov	r8, r3
	movs	r3, #205
	lsls	r3, r3, #2
	mov	r0, sl
	muls	r0, r3
	bl	sub_08002096
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	asrs	r3, r3, #16
	movs	r2, #80
	b.n	.L_0816b984
.L_0816b96a:
	ldr	r4, [sp, #28]
	movs	r3, #205
	lsls	r3, r3, #2
	mov	r0, sl
	muls	r0, r3
	mov	r8, r4
	bl	sub_08002096
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	asrs	r3, r3, #16
	movs	r2, #30
.L_0816b984:
	subs	r2, r2, r3
	ldr	r5, [pc, #440]
	movs	r7, #0
	negs	r6, r2
.L_0816b98c:
	mov	r0, r8
	bl	sub_08002096
	lsrs	r0, r0, #11
	strb	r0, [r5, #0]
	strb	r6, [r5, #1]
	mov	r0, r8
	bl	sub_08002090
	adds	r7, #1
	lsrs	r0, r0, #11
	strb	r0, [r5, #2]
	subs	r6, #24
	adds	r5, #4
	cmp	r7, #2
	bne.n	.L_0816b98c
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_0816b9e2
	mov	r1, sl
	cmp	r1, #28
	bne.n	.L_0816ba2a
	ldr	r3, [sp, #48]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #6
	str	r3, [r2, #0]
	movs	r0, #133
	bl	sub_081180e8
	mov	r2, fp
	movs	r3, #8
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	b.n	.L_0816ba2a
.L_0816b9e2:
	mov	r3, sl
	cmp	r3, #12
	bne.n	.L_0816b9f0
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_0816b9f0:
	mov	r4, fp
	ldr	r3, [r4, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_0816ba2a
	movs	r6, #36
	movs	r5, #12
.L_0816b9fe:
	cmp	sl, r5
	bne.n	.L_0816ba20
	movs	r0, #126
	bl	sub_081c0010
	mov	r1, fp
	ldrsh	r0, [r6, r1]
	movs	r3, #8
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	negs	r2, r2
	bl	sub_0814cd48
	mov	r4, fp
	ldr	r3, [r4, #20]
.L_0816ba20:
	adds	r7, #1
	adds	r6, #2
	adds	r5, #4
	cmp	r7, r3
	bne.n	.L_0816b9fe
.L_0816ba2a:
	bl	sub_08014de4
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #20]
	bl	sub_080156e8
	movs	r0, #1
	bl	sub_081969f8
	adds	r5, r0, #0
	mov	r0, sl
	ldr	r6, [pc, #268]
	movs	r1, #0
	cmp	r0, #15
	bgt.n	.L_0816ba56
	lsls	r3, r0, #2
	adds	r1, r3, #0
	subs	r1, #64
	cmp	r1, #0
	ble.n	.L_0816ba6e
	movs	r1, #0
	b.n	.L_0816ba6e
.L_0816ba56:
	mov	r2, sl
	cmp	r2, #27
	ble.n	.L_0816ba6e
	movs	r3, #28
	subs	r3, r3, r2
	lsls	r1, r3, #3
	movs	r3, #64
	negs	r3, r3
	cmp	r1, r3
	bge.n	.L_0816ba6e
	movs	r1, #64
	negs	r1, r1
.L_0816ba6e:
	ldr	r3, [sp, #52]
	ldr	r2, [pc, #224]
	ldr	r4, [sp, #48]
	ands	r3, r2
	movs	r2, #7
	orrs	r3, r2
	ldr	r2, [pc, #220]
	movs	r0, #224
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #16]
	lsls	r0, r0, #3
	str	r3, [sp, #52]
	adds	r3, r4, r0
	str	r3, [r2, #4]
	movs	r3, #6
	str	r3, [r5, #0]
	ldr	r3, [pc, #164]
	str	r2, [r5, #16]
	ldr	r2, [pc, #192]
	str	r3, [r5, #8]
	movs	r3, #0
	strb	r3, [r5, #24]
	strb	r3, [r5, #25]
	movs	r3, #104
	str	r3, [r2, #16]
	ldr	r3, [pc, #184]
	mov	r4, r9
	ldrb	r3, [r3, r4]
	str	r6, [r5, #12]
	str	r1, [r5, #20]
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_0816bab8
	b.n	.L_0816bbf0
.L_0816bab8:
	bl	sub_08014de4
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_0816bb68
	ldr	r1, [sp, #12]
	movs	r2, #0
	ldr	r0, [r1, #0]
	movs	r1, #160
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r1, r1, #14
	lsls	r0, r0, #16
	bl	sub_08015160
	lsls	r0, r7, #11
	bl	sub_08015024
	movs	r3, #164
	lsls	r3, r3, #7
	adds	r3, #8
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r2, [sp, #28]
	adds	r0, r0, r2
	bl	sub_08015068
	mov	r3, sl
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #4
	add	r0, sl
	lsls	r0, r0, #2
	bl	sub_08002096
	movs	r3, #167
	lsls	r3, r3, #9
	adds	r3, #32
	mov	r4, sl
	subs	r3, r3, r0
	cmp	r4, #27
	ble.n	.L_0816bb18
	ldr	r0, [sp, #8]
	ldr	r1, [pc, #80]
	adds	r3, r3, r0
	adds	r3, r3, r1
.L_0816bb18:
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	b.n	.L_0816bbd2
	movs	r0, r0
	.4byte 0x000000d3
	.4byte 0x00000134
	.4byte 0x00000153
	.4byte 0x02010318
	.4byte 0x02010000
	.4byte 0x00000166
	.4byte 0x00000167
	.4byte 0x0200fff8
	.4byte 0x02010108
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x030011e0
	.4byte 0x08198b24
	.2byte 0x2000
	.2byte 0xffff
.L_0816bb68:
	.2byte 0x9a03
	lsls	r1, r7, #20
	ldr	r0, [r2, #0]
	movs	r2, #0
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	movs	r3, #128
	lsls	r3, r3, #13
	subs	r0, #64
	adds	r1, r1, r3
	lsls	r0, r0, #16
	bl	sub_08015160
	ldr	r4, [pc, #204]
	lsls	r0, r7, #12
	adds	r0, r0, r4
	bl	sub_08015024
	movs	r3, #164
	lsls	r3, r3, #7
	adds	r3, #8
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r1, [sp, #28]
	adds	r0, r0, r1
	bl	sub_08015068
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	mov	r0, sl
	muls	r0, r3
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0816bbb4
	adds	r0, #3
.L_0816bbb4:
	movs	r1, #128
	asrs	r3, r0, #2
	lsls	r1, r1, #9
	mov	r2, sl
	subs	r0, r1, r3
	cmp	r2, #27
	ble.n	.L_0816bbca
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #144]
	adds	r3, r0, r4
	adds	r0, r3, r2
.L_0816bbca:
	lsls	r2, r0, #1
	adds	r0, r2, #0
	bl	sub_080151e4
.L_0816bbd2:
	ldr	r0, [pc, #136]
	ldr	r1, [pc, #136]
	movs	r2, #66
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	ldr	r3, [pc, #128]
	mov	r4, r9
	ldrb	r3, [r3, r4]
	adds	r7, #1
	cmp	r7, r3
	beq.n	.L_0816bbf0
	b.n	.L_0816bab8
.L_0816bbf0:
	ldr	r2, [pc, #116]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	adds	r0, r5, #0
	bl	sub_08013164
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #48]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #8]
	movs	r3, #128
	ldr	r0, [sp, #24]
	lsls	r3, r3, #4
	movs	r4, #1
	adds	r2, r2, r3
	add	sl, r4
	str	r2, [sp, #8]
	cmp	sl, r0
	beq.n	.L_0816bc34
	b.n	.L_0816b830
.L_0816bc34:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #48]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffff800
	.4byte 0xffff2000
	.4byte 0x02010000
	.4byte 0x02010108
	.4byte 0x08198b24
	.4byte 0x030011e0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #76
	str	r0, [sp, #40]
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r0, [r6, #92]
	str	r0, [sp, #36]
	movs	r0, #0
	ldr	r1, [r6, #96]
	str	r1, [sp, #32]
	ldr	r2, [r6, #100]
	mov	r8, r2
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #40]
	add	r5, sp, #64
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r3, [r5, #0]
	movs	r1, #19
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r0, #104
	str	r3, [sp, #20]
	bl	sub_081963ec
	ldr	r2, [sp, #36]
	ldr	r6, [r6, #104]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #12]
	movs	r2, #1
	movs	r3, #0
	str	r6, [sp, #24]
	b.n	.L_0816bce0
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x0193
	.2byte 0x0000
.L_0816bce0:
	bl	sub_08157cf4
	ldr	r4, [pc, #320]
	movs	r2, #156
	lsls	r2, r2, #7
	adds	r2, #32
	adds	r1, r4, r2
	ldr	r0, [pc, #316]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #308]
	ldr	r1, [pc, #300]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [pc, #288]
	movs	r4, #128
	lsls	r4, r4, #5
	adds	r1, r3, r4
	ldr	r0, [pc, #292]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #288]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #36]
	movs	r3, #222
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #272]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r4, [sp, #36]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r4, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #240]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #128
	negs	r2, r2
	str	r2, [sp, #16]
	movs	r3, #0
	mov	r9, r3
.L_0816bd60:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_0816bd86
	movs	r0, #104
	negs	r0, r0
	ldr	r3, [sp, #36]
	str	r0, [sp, #16]
	movs	r1, #0
	movs	r2, #1
	mov	r8, r1
	negs	r2, r2
	adds	r3, #24
.L_0816bd78:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #64
	bne.n	.L_0816bd78
.L_0816bd86:
	mov	r1, r9
	cmp	r1, #63
	bgt.n	.L_0816bd98
	ldr	r3, [sp, #40]
	add	r1, sp, #52
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_0815e21c
.L_0816bd98:
	mov	r4, r9
	cmp	r4, #20
	bne.n	.L_0816bdae
	ldr	r1, [sp, #40]
	movs	r3, #60
	ldr	r0, [r1, #8]
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #8
	bl	sub_08157530
.L_0816bdae:
	mov	r3, r9
	cmp	r3, #2
	bne.n	.L_0816bdba
	movs	r0, #212
	bl	sub_081c0010
.L_0816bdba:
	mov	r4, r9
	cmp	r4, #127
	ble.n	.L_0816bdc2
	b.n	sub_0816bf12
.L_0816bdc2:
	movs	r0, #104
	str	r0, [sp, #12]
	cmp	r4, #8
	bgt.n	.L_0816bdd2
	ldr	r1, [sp, #16]
	adds	r1, #16
	str	r1, [sp, #16]
	b.n	.L_0816bee4
.L_0816bdd2:
	mov	r6, r9
	subs	r6, #24
	cmp	r6, #64
	bls.n	.L_0816bddc
	b.n	.L_0816bee4
.L_0816bddc:
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0816bde6
	mov	r3, r9
	subs	r3, #17
.L_0816bde6:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r5, r6, r3
	cmp	r5, #4
	bne.n	.L_0816bdf6
	movs	r0, #212
	bl	sub_081c0010
.L_0816bdf6:
	cmp	r5, #7
	bne.n	.L_0816bee4
	ldr	r3, [sp, #36]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	mov	r0, r9
	movs	r3, #4
	str	r3, [r2, #0]
	cmp	r0, #79
	bgt.n	.L_0816be44
	movs	r0, #144
	bl	sub_081c0010
	ldr	r1, [sp, #40]
	movs	r3, #70
	ldr	r0, [r1, #8]
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #16
	bl	sub_08157530
	b.n	sub_0816be64
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x000000ea
	.4byte 0x000000c1
	.4byte 0x000000da
	.4byte 0x00000134
	.4byte 0x000000c2
	.2byte 0x3001
	.2byte 0x0814
.L_0816be44:
	ldr	r4, [sp, #40]
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118088
	movs	r0, #144
	bl	sub_081180e8
	movs	r1, #128
	ldr	r3, [pc, #896]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #896]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0a
	movs	r3, #6
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #20]
	adds	r3, #8
	str	r3, [sp, #16]
	movs	r4, #0
	subs	r0, r6, #7
	movs	r2, #127
	mov	r8, r4
	mov	fp, r0
	lsls	r7, r1, #16
	mov	sl, r2
.L_0816be8e:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_0816be96
	adds	r3, r6, #0
.L_0816be96:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	add	r3, r8
	movs	r2, #63
	adds	r3, #32
	ands	r3, r2
	lsls	r5, r3, #3
	subs	r5, r5, r3
	ldr	r3, [sp, #36]
	lsls	r5, r5, #2
	adds	r5, r3, r5
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	str	r7, [r5, #0]
	bl	sub_08014878
	mov	r4, sl
	ands	r0, r4
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08014878
	mov	r1, sl
	movs	r3, #32
	ands	r0, r1
	adds	r0, #64
	str	r3, [r5, #8]
	movs	r3, #1
	negs	r0, r0
	add	r8, r3
	lsls	r0, r0, #11
	movs	r2, #0
	mov	r4, r8
	str	r0, [r5, #16]
	str	r2, [r5, #24]
	cmp	r4, #8
	bne.n	.L_0816be8e
.L_0816bee4:
	ldr	r3, [sp, #16]
	adds	r3, #104
	cmp	r3, #108
	ble.n	.L_0816bef6
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #16]
	subs	r3, r0, r1
	adds	r3, #4
	str	r3, [sp, #12]
.L_0816bef6:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	ble.n	sub_0816bf12
	str	r2, [sp, #4]
	ldr	r2, [sp, #20]
	movs	r3, #48
	str	r3, [sp, #0]
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #732]
	subs	r2, #24
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4648
	cmp	r0, #7
	bne.n	.L_0816bf6e
	ldr	r1, [sp, #36]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #8
	movs	r0, #145
	str	r3, [r2, #0]
	bl	sub_081c0010
	ldr	r0, [sp, #20]
	ldr	r5, [sp, #36]
	movs	r4, #0
	mov	r8, r4
	lsls	r7, r0, #16
	movs	r6, #127
.L_0816bf38:
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	str	r7, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #64
	movs	r2, #1
	movs	r3, #32
	negs	r0, r0
	add	r8, r2
	str	r3, [r5, #8]
	lsls	r0, r0, #12
	movs	r1, #0
	mov	r3, r8
	str	r0, [r5, #16]
	str	r1, [r5, #24]
	adds	r5, #28
	cmp	r3, #32
	bne.n	.L_0816bf38
.L_0816bf6e:
	mov	r4, r9
	cmp	r4, #6
	ble.n	.L_0816c01c
	ldr	r7, [sp, #36]
	movs	r0, #0
	mov	r8, r0
.L_0816bf7a:
	movs	r1, #192
	ldr	r6, [r7, #24]
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r6, r1
	bhi.n	.L_0816c010
	movs	r1, #5
	mov	r0, r8
	bl	sub_08002064
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	adds	r0, r6, #0
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	ldr	r2, [pc, #580]
	adds	r5, r5, r0
	lsls	r3, r5, #2
	ldr	r1, [r2, r3]
	ldr	r2, [sp, #36]
	movs	r3, #240
	adds	r1, r2, r1
	lsls	r3, r3, #4
	adds	r1, r1, r3
	ldr	r3, [pc, #568]
	movs	r4, #2
	ldrsh	r2, [r7, r4]
	ldrb	r6, [r3, r5]
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #556]
	ldrb	r4, [r0, r5]
	str	r6, [sp, #0]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r7, #0
	movs	r1, #64
	lsls	r2, r2, #7
	bl	sub_08138086
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	cmp	r3, #104
	ble.n	.L_0816bff6
	ldr	r3, [r7, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #16]
.L_0816bff6:
	ldr	r3, [r7, #24]
	ldr	r2, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #24]
	cmp	r2, #1
	ble.n	.L_0816c010
	movs	r3, #1
	mov	r1, r9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0816c010
	subs	r3, r2, #1
	str	r3, [r7, #8]
.L_0816c010:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #64
	bne.n	.L_0816bf7a
.L_0816c01c:
	mov	r4, r9
	cmp	r4, #5
	bgt.n	.L_0816c024
	b.n	.L_0816c224
.L_0816c024:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	str	r0, [sp, #8]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #444]
	ldr	r3, [sp, #44]
	adds	r6, r0, #0
	ands	r3, r2
	ldr	r2, [pc, #440]
	movs	r0, #7
	orrs	r3, r0
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #44]
	movs	r3, #0
	str	r3, [r6, #4]
	ldr	r2, [pc, #424]
	ldr	r3, [pc, #428]
	add	r1, sp, #44
	str	r2, [r1, #4]
	str	r0, [r6, #0]
	str	r1, [r6, #16]
	str	r3, [r6, #8]
	ldr	r4, [sp, #8]
	movs	r0, #0
	str	r4, [r6, #12]
	strb	r0, [r6, #24]
	strb	r0, [r6, #25]
	ldr	r2, [sp, #20]
	mov	sl, r1
	subs	r2, #64
	movs	r1, #0
	mov	r8, r1
	mov	r7, r9
	mov	fp, r2
.L_0816c076:
	ldr	r3, [pc, #396]
	mov	r4, r8
	ldrb	r3, [r3, r4]
	adds	r1, r3, #6
	cmp	r9, r1
	ble.n	.L_0816c0f2
	ldr	r3, [pc, #388]
	mov	r0, r9
	ldrb	r3, [r3, r4]
	subs	r2, r0, r1
	muls	r2, r3
	movs	r3, #175
	lsls	r3, r3, #3
	muls	r3, r2
	movs	r2, #131
	lsls	r2, r2, #7
	adds	r5, r3, r2
	subs	r3, r1, r0
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r3, #0
	ble.n	.L_0816c0a4
	movs	r3, #0
.L_0816c0a4:
	movs	r4, #64
	negs	r4, r4
	cmp	r3, r4
	ble.n	.L_0816c0f2
	str	r3, [r6, #20]
	bl	sub_08014de4
	ldr	r3, [pc, #344]
	mov	r0, r8
	ldrsb	r1, [r3, r0]
	mov	r2, fp
	lsls	r0, r2, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r4, #7
	adds	r3, r7, #0
	ands	r3, r4
	movs	r0, #176
	lsls	r3, r3, #4
	lsls	r0, r0, #4
	strb	r3, [r6, #24]
	adds	r0, #184
	bl	sub_08015024
	ldr	r0, [pc, #300]
	ldr	r1, [sp, #8]
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0816c0f2:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r7, #5
	cmp	r1, #4
	bne.n	.L_0816c076
	movs	r2, #7
	mov	r3, sl
	strb	r2, [r3, #0]
	add	r3, sp, #44
	strb	r2, [r3, #1]
	ldr	r4, [sp, #36]
	movs	r0, #222
	lsls	r0, r0, #6
	adds	r2, r4, r0
	str	r2, [r3, #4]
	ldr	r3, [pc, #256]
	movs	r1, #0
	str	r3, [r6, #8]
	strb	r1, [r6, #24]
	movs	r3, #52
	strb	r1, [r6, #25]
	movs	r2, #0
	add	r3, sp
	mov	r7, r9
	mov	r8, r2
	mov	sl, r3
	subs	r7, #31
.L_0816c12a:
	cmp	r7, #0
	blt.n	.L_0816c1c2
	mov	r4, r8
	movs	r0, #128
	lsls	r3, r4, #11
	lsls	r0, r0, #6
	adds	r3, r3, r0
	muls	r3, r7
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r1, r1, r3
	mov	fp, r1
	movs	r3, #0
	cmp	r7, #3
	ble.n	.L_0816c14e
	movs	r3, #4
	subs	r3, r3, r7
	lsls	r3, r3, #4
.L_0816c14e:
	movs	r2, #64
	negs	r2, r2
	str	r3, [r6, #20]
	cmp	r3, r2
	ble.n	.L_0816c1c2
	bl	sub_08014de4
	mov	r3, sl
	ldr	r0, [r3, #0]
	mov	r4, sl
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #80
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl	sub_080151e4
	ldr	r1, [sp, #40]
	movs	r2, #128
	ldr	r0, [r1, #4]
	lsls	r2, r2, #8
	lsls	r0, r0, #15
	adds	r0, r0, r2
	bl	sub_08015068
	ldr	r3, [pc, #124]
	mov	r4, r8
	lsls	r5, r4, #1
	ldrsh	r0, [r3, r5]
	bl	sub_080150e4
	ldr	r3, [pc, #116]
	ldrsh	r0, [r3, r5]
	bl	sub_08015024
	mov	r0, fp
	bl	sub_0801521c
	ldr	r0, [pc, #108]
	ldr	r1, [sp, #8]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0816c1c2:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	subs	r7, #8
	cmp	r4, #8
	bne.n	.L_0816c12a
	adds	r0, r6, #0
	bl	sub_08013164
	ldr	r0, [sp, #8]
	bl	sub_08013164
	b.n	.L_0816c224
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02014e20
	.4byte 0x08197834
	.4byte 0x0819781a
	.4byte 0x08197826
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02010000
	.4byte 0x08198ec4
	.4byte 0x08198b26
	.4byte 0x08198b2a
	.4byte 0x08198b2e
	.4byte 0x08198cac
	.4byte 0x08199364
	.4byte 0x08198b32
	.4byte 0x08198b42
	.2byte 0x9210
	.2byte 0x0819
.L_0816c224:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r1, #240
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #148
	beq.n	.L_0816c250
	b.n	.L_0816bd60
.L_0816c250:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #104
	str	r0, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	movs	r2, #184
	str	r1, [sp, #52]
	lsls	r2, r2, #6
	ldr	r3, [r3, #96]
	adds	r2, #16
	adds	r2, r1, r2
	movs	r1, #128
	str	r3, [sp, #48]
	str	r2, [sp, #36]
	ldr	r3, [pc, #204]
	ldr	r0, [pc, #204]
	lsls	r1, r1, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	movs	r7, #0
	mov	r8, r3
	movs	r6, #2
	movs	r5, #0
.L_0816c2b2:
	ldr	r4, [pc, #188]
	movs	r2, #128
	adds	r1, r6, #0
	adds	r0, r7, r4
	lsls	r2, r2, #9
	bl	sub_0815b434
	adds	r3, r5, #3
	muls	r3, r6
	ldr	r1, [sp, #36]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r2, #1
	asrs	r3, r3, #1
	add	r8, r2
	strh	r7, [r5, r1]
	adds	r7, r7, r3
	mov	r3, r8
	adds	r6, #2
	adds	r5, #2
	cmp	r3, #16
	bne.n	.L_0816c2b2
	ldr	r4, [sp, #52]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r5, r4, r1
	ldr	r0, [pc, #140]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [pc, #132]
	movs	r2, #64
	movs	r3, #64
	adds	r0, r5, #0
	bl	sub_0816ae40
	movs	r0, #0
	bl	sub_081435e0
	ldr	r2, [sp, #56]
	movs	r3, #1
	ldr	r1, [r2, #4]
	adds	r0, r2, #0
	eors	r1, r3
	lsls	r1, r1, #4
	movs	r3, #32
	orrs	r1, r3
	mov	r3, sp
	adds	r3, #92
	str	r3, [sp, #32]
	ldr	r2, [sp, #32]
	add	r3, sp, #80
	bl	sub_0815585c
	movs	r2, #216
	ldr	r4, [sp, #52]
	lsls	r2, r2, #7
	adds	r2, #192
	adds	r1, r4, r2
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [pc, #72]
	bl	sub_08157cf4
	ldr	r0, [pc, #72]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #64]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b07
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r4, [sp, #56]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_0816c388
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_0816c390
	movs	r0, r0
	.4byte 0x0000100e
	.4byte 0x03000258
	.4byte 0x02010000
	.4byte 0x000000fb
	.4byte 0x02014000
	.4byte 0x00000157
	.4byte 0x00000184
	.2byte 0x0730
	.2byte 0x0300
.L_0816c388:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
.L_0816c390:
	ldr	r3, [sp, #52]
	movs	r5, #192
	lsls	r5, r5, #18
	movs	r4, #239
	lsls	r4, r4, #7
	ldr	r1, [r5, #104]
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	str	r1, [sp, #40]
	movs	r3, #238
	ldr	r1, [sp, #52]
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #196]
	bl	sub_080145a8
	ldr	r5, [r5, #48]
	ldr	r1, [sp, #56]
	str	r5, [sp, #28]
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	bl	sub_08118098
	ldr	r3, [sp, #28]
	ldr	r0, [r0, #0]
	mov	r4, sp
	adds	r3, #12
	adds	r4, #60
	str	r0, [sp, #24]
	str	r3, [sp, #16]
	str	r4, [sp, #20]
	movs	r2, #0
	mov	r9, r2
.L_0816c3de:
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_0816c45a
	ldr	r7, [sp, #52]
	movs	r2, #0
	mov	r8, r2
.L_0816c3ea:
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	movs	r5, #63
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	ldr	r4, [sp, #24]
	adds	r5, #80
	adds	r2, r5, #0
	muls	r2, r0
	ldr	r3, [r4, #8]
	asrs	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r7, #8]
	bl	sub_08014878
	str	r0, [r7, #12]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #4
	str	r3, [r7, #20]
	movs	r2, #1
	movs	r3, #1
	negs	r3, r3
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_0816c3ea
.L_0816c45a:
	mov	r4, r9
	cmp	r4, #95
	ble.n	.L_0816c480
	ldr	r3, [pc, #16]
	lsls	r2, r4, #1
	subs	r3, r3, r2
	ldr	r2, [pc, #16]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_0816c480
	.4byte 0x000000ce
	.4byte 0x00001000
	.2byte 0x3001
	.2byte 0x0814
.L_0816c480:
	bl	sub_08014de4
	ldr	r1, [sp, #16]
	ldr	r0, [sp, #28]
	bl	sub_080156e8
	mov	r1, r9
	cmp	r1, #19
	bgt.n	sub_0816c508
	lsls	r5, r1, #11
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r2, [sp, #32]
	lsls	r0, r0, #3
	ldr	r3, [r2, #0]
	asrs	r0, r0, #16
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r0, r0, r3
	subs	r0, #10
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r4, [sp, #32]
	lsls	r0, r0, #2
	ldr	r3, [r4, #4]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	mov	r1, r9
	adds	r5, r0, #0
	lsls	r3, r1, #1
	add	r3, r9
	subs	r5, #24
	ldr	r2, [sp, #52]
	adds	r5, r5, r3
	movs	r3, #216
	lsls	r3, r3, #7
	movs	r4, #20
	adds	r3, #192
	adds	r7, r2, r3
	str	r4, [sp, #0]
	mov	r8, r4
	movs	r4, #40
	str	r4, [sp, #4]
	adds	r1, r7, #0
	str	r4, [sp, #8]
	ldr	r6, [sp, #40]
	ldr	r0, [sp, #48]
	mov	r2, sl
	adds	r3, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	ldr	r4, [sp, #8]
	cmp	r1, #3
	bgt.n	sub_0816c508
	mov	r2, r8
	str	r2, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	adds	r1, r7, #0
	mov	r2, sl
	adds	r3, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #47
	ble.n	.L_0816c5b4
	ldr	r5, [sp, #52]
	movs	r4, #0
	movs	r1, #7
	mov	r8, r4
	mov	fp, r1
.L_0816c518:
	mov	r2, r8
	lsls	r3, r2, #1
	adds	r3, #48
	cmp	r9, r3
	blt.n	.L_0816c5a8
	add	r6, sp, #68
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_0815e1ec
	ldr	r3, [r6, #0]
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	ldr	r0, [r5, #12]
	ldr	r7, [r5, #20]
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #17
	adds	r7, r7, r3
	ldr	r3, [r5, #12]
	movs	r4, #128
	lsls	r4, r4, #3
	lsls	r7, r7, #1
	adds	r3, r3, r4
	subs	r2, r7, #2
	str	r3, [r5, #12]
	cmp	r2, #61
	bhi.n	.L_0816c5a0
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	mov	sl, r3
	ldr	r3, [r5, #24]
	ldr	r2, [r6, #0]
	ldr	r6, [r6, #4]
	cmp	r3, #0
	bne.n	.L_0816c58a
	str	r2, [sp, #12]
	bl	sub_08014878
	ldr	r2, [sp, #12]
	mov	r1, fp
	ands	r0, r1
	adds	r0, r2, r0
	subs	r2, r0, #4
	str	r2, [sp, #12]
	bl	sub_08014878
	mov	r3, fp
	ands	r0, r3
	ldr	r2, [sp, #12]
	adds	r0, r6, r0
	subs	r6, r0, #4
.L_0816c58a:
	ldr	r1, [sp, #36]
	mov	r4, sl
	lsls	r3, r4, #1
	ldrsh	r0, [r3, r1]
	ldr	r1, [pc, #320]
	adds	r3, r7, #0
	adds	r0, r0, r1
	adds	r1, r2, #0
	adds	r2, r6, #0
	bl	sub_0818caa8
.L_0816c5a0:
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_0816c5a8:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #32
	bne.n	.L_0816c518
.L_0816c5b4:
	ldr	r1, [sp, #56]
	movs	r4, #0
	ldr	r3, [r1, #20]
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_0816c5f2
	movs	r6, #36
	movs	r5, #64
.L_0816c5c4:
	cmp	r9, r5
	bne.n	.L_0816c5e6
	movs	r0, #126
	bl	sub_081c0010
	ldr	r2, [sp, #56]
	movs	r1, #7
	ldrsh	r0, [r6, r2]
	movs	r3, #24
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r4, [sp, #56]
	ldr	r3, [r4, #20]
.L_0816c5e6:
	movs	r1, #1
	add	r8, r1
	adds	r6, #2
	adds	r5, #4
	cmp	r8, r3
	bne.n	.L_0816c5c4
.L_0816c5f2:
	bl	sub_08014de4
	ldr	r2, [pc, #224]
	movs	r3, #104
	str	r3, [r2, #16]
	movs	r0, #32
	bl	sub_08014dac
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #6
	adds	r6, r0, #0
	str	r3, [r6, #0]
	ldr	r3, [pc, #200]
	ldr	r1, [sp, #20]
	movs	r2, #7
	str	r3, [r6, #8]
	add	r3, sp, #60
	str	r1, [r6, #16]
	str	r7, [r6, #12]
	str	r3, [sp, #20]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #188]
	strb	r2, [r1, #1]
	mov	r2, r9
	movs	r0, #0
	str	r3, [r1, #4]
	cmp	r2, #15
	ble.n	.L_0816c67c
	ldr	r4, [pc, #176]
	ldr	r1, [pc, #180]
	lsls	r3, r2, #12
	adds	r5, r3, r4
	cmp	r5, r1
	ble.n	.L_0816c63e
	ldr	r5, [pc, #168]
.L_0816c63e:
	str	r0, [r6, #20]
	ldr	r0, [pc, #168]
	bl	sub_08015024
	ldr	r2, [sp, #32]
	movs	r1, #160
	ldr	r0, [r2, #0]
	lsls	r1, r1, #14
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	movs	r2, #0
	lsls	r0, r0, #16
	bl	sub_08015160
	asrs	r0, r5, #1
	bl	sub_0801521c
	mov	r3, r9
	lsls	r0, r3, #9
	bl	sub_08015068
	ldr	r0, [pc, #128]
	adds	r1, r7, #0
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0816c67c:
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r7, #0
	bl	sub_08013164
	ldr	r2, [pc, #76]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	bl	sub_081434f8
	movs	r1, #240
	ldr	r4, [sp, #52]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r4, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #103
	beq.n	.L_0816c6b4
	b.n	.L_0816c3de
.L_0816c6b4:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x030011e0
	.4byte 0x08199364
	.4byte 0x02014000
	.4byte 0xffff0000
	.4byte 0x0001bd50
	.4byte 0xfffff448
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
	sub	sp, #152
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	movs	r0, #2
	str	r1, [sp, #80]
	ldr	r2, [r3, #96]
	adds	r3, #176
	str	r2, [sp, #76]
	ldr	r3, [r3, #0]
	str	r3, [sp, #64]
	bl	sub_081435e0
	ldr	r3, [sp, #84]
	add	r2, sp, #128
	ldr	r1, [r3, #4]
	movs	r3, #192
	lsls	r1, r1, #4
	orrs	r1, r3
	ldr	r0, [sp, #84]
	add	r3, sp, #140
	bl	sub_0815585c
	bl	sub_0813ba50
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [sp, #64]
	movs	r3, #1
	str	r3, [r1, #16]
	ldr	r2, [sp, #84]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0816c76c
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_0816c774
	movs	r0, r0
	.4byte 0x00003537
	.2byte 0x3f31
	.2byte 0x0000
.L_0816c76c:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
.L_0816c774:
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r2, #128
	str	r3, [sp, #68]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [sp, #80]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r1, #185
	movs	r3, #0
	lsls	r1, r1, #4
	str	r3, [r2, #0]
	adds	r1, #255
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	movs	r1, #192
	lsls	r1, r1, #4
	b.n	.L_0816c7c4
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00000000
	.2byte 0x3001
	.2byte 0x0814
.L_0816c7c4:
	adds	r1, #255
	ldr	r0, [pc, #348]
	bl	sub_080145a8
	ldr	r1, [sp, #80]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, r1, r2
	movs	r3, #240
	str	r2, [sp, #60]
	lsls	r3, r3, #4
	movs	r2, #152
	adds	r3, r1, r3
	lsls	r2, r2, #5
	str	r3, [sp, #56]
	adds	r2, r1, r2
	movs	r3, #0
	str	r2, [sp, #52]
	str	r3, [sp, #40]
	ldr	r1, [sp, #84]
	ldr	r5, [r5, #36]
	movs	r7, #0
	str	r5, [sp, #36]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r2, [pc, #300]
	ldr	r0, [r0, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #200
	ldr	r1, [pc, #296]
	str	r0, [sp, #32]
	strh	r3, [r2, #4]
	ldr	r3, [pc, #292]
	movs	r2, #7
	mov	sl, r2
	mov	r8, r1
.L_0816c810:
	adds	r5, r7, #0
	mov	r2, sl
	movs	r6, #0
	ands	r5, r2
.L_0816c818:
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_0816c820
	adds	r1, r7, #7
.L_0816c820:
	asrs	r1, r1, #3
	lsls	r1, r1, #8
	ldr	r2, [pc, #268]
	adds	r1, r1, r5
	lsls	r1, r1, #3
	adds	r1, r1, r6
	adds	r1, r1, r2
	adds	r0, r3, #0
	str	r3, [sp, #12]
	movs	r2, #8
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x21e0
	ldr	r3, [sp, #12]
	lsls	r1, r1, #3
	adds	r6, #64
	adds	r1, #255
	adds	r3, #8
	cmp	r6, r1
	ble.n	.L_0816c818
	adds	r7, #1
	cmp	r7, #119
	ble.n	.L_0816c810
	ldr	r2, [sp, #80]
	movs	r3, #216
	lsls	r3, r3, #7
	adds	r3, #192
	adds	r1, r2, r3
	ldr	r0, [pc, #220]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r3, #156
	ldr	r2, [sp, #80]
	lsls	r3, r3, #7
	adds	r3, #16
	adds	r1, r2, r3
	ldr	r0, [pc, #204]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #200]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #168]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x492e
	ldr	r5, [pc, #184]
	ldrh	r3, [r5, #0]
	adds	r0, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816c8b8
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
.L_0816c8b8:
	strh	r0, [r5, #0]
	ldr	r0, [sp, #32]
	movs	r1, #48
	bl	sub_08020098
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #16
	movs	r3, #16
	ldr	r0, [sp, #52]
	bl	sub_0818de3c
	mov	r2, sp
	mov	r3, sp
	adds	r2, #116
	adds	r3, #104
	str	r2, [sp, #24]
	str	r3, [sp, #28]
	movs	r1, #0
	mov	fp, r1
.L_0816c8e2:
	ldr	r1, [sp, #84]
	ldr	r0, [r1, #8]
	ldr	r1, [sp, #24]
	bl	sub_0815e21c
	ldr	r3, [sp, #84]
	ldr	r1, [sp, #28]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_0815e21c
	mov	r1, fp
	cmp	r1, #11
	bgt.n	.L_0816c9b2
	ldr	r2, [sp, #36]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #108
	adds	r2, r2, r3
	mov	ip, r2
	movs	r1, #160
	ldr	r2, [pc, #16]
	lsls	r1, r1, #19
	adds	r1, #192
	movs	r3, #31
	mov	r8, r1
	movs	r7, #0
	mov	lr, r2
	mov	sl, r3
	b.n	.L_0816c94c
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x08143425
	.4byte 0x03001120
	.4byte 0x03000730
	.4byte 0x02010000
	.4byte 0x06008000
	.4byte 0x00000157
	.4byte 0x000000da
	.4byte 0x00000148
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0816c94c:
	mov	r1, ip
	ldrh	r3, [r1, #0]
	mov	r4, sl
	ands	r4, r3
	mov	r1, r8
	lsls	r3, r3, #16
	lsrs	r5, r3, #21
	lsrs	r6, r3, #26
	ldrh	r3, [r1, #0]
	mov	r0, sl
	mov	r2, lr
	ands	r0, r3
	lsls	r3, r3, #16
	mov	r1, lr
	ands	r5, r2
	ands	r6, r2
	lsrs	r2, r3, #21
	ands	r2, r1
	lsrs	r1, r3, #26
	mov	r3, lr
	ands	r1, r3
	adds	r3, r4, #3
	cmp	r0, r3
	ble.n	.L_0816c980
	subs	r0, #4
	b.n	.L_0816c982
.L_0816c980:
	adds	r0, r4, #0
.L_0816c982:
	adds	r3, r5, #3
	cmp	r2, r3
	ble.n	.L_0816c98c
	subs	r2, #4
	b.n	.L_0816c98e
.L_0816c98c:
	adds	r2, r5, #0
.L_0816c98e:
	adds	r3, r6, #3
	cmp	r1, r3
	ble.n	.L_0816c998
	subs	r1, #4
	b.n	.L_0816c99a
.L_0816c998:
	adds	r1, r6, #0
.L_0816c99a:
	lsls	r3, r1, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	mov	r1, r8
	movs	r2, #2
	orrs	r3, r0
	adds	r7, #1
	strh	r3, [r1, #0]
	add	ip, r2
	add	r8, r2
	cmp	r7, #128
	bne.n	.L_0816c94c
.L_0816c9b2:
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L_0816c9f6
	ldr	r1, [pc, #220]
	ldr	r2, [pc, #224]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	lsls	r3, r3, #2
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816c9ee
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #234
	adds	r3, r3, r1
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
.L_0816c9ee:
	ldr	r3, [pc, #172]
	strh	r0, [r3, #0]
	bl	sub_08143354
.L_0816c9f6:
	mov	r1, fp
	cmp	r1, #34
	bne.n	.L_0816cab0
	ldr	r0, [pc, #160]
	bl	sub_08014644
	movs	r1, #185
	lsls	r1, r1, #4
	adds	r1, #255
	ldr	r0, [pc, #152]
	bl	sub_080145a8
	ldr	r1, [pc, #136]
	ldr	r2, [pc, #136]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	lsls	r3, r3, #2
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816ca44
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #230
	adds	r3, r3, r1
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
.L_0816ca44:
	ldr	r3, [pc, #84]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #92]
	movs	r1, #240
	lsls	r1, r1, #6
	ldr	r0, [sp, #76]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4915
	movs	r3, #78
	strh	r3, [r1, #6]
	ldr	r3, [sp, #24]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #40]
	subs	r3, r3, r2
	strh	r3, [r1, #4]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #28]
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #28]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #20]
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	b.n	.L_0816cab0
	movs	r0, r0
	.4byte 0x00000040
	.4byte 0x0000fffc
	.4byte 0x00000001
	.4byte 0x00000784
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x08143001
	.4byte 0x08143265
	.4byte 0x03000258
	.2byte 0x1120
	.2byte 0x0300
.L_0816cab0:
	mov	r1, fp
	cmp	r1, #35
	beq.n	.L_0816cab8
	b.n	.L_0816cbaa
.L_0816cab8:
	ldr	r0, [pc, #380]
	bl	sub_08014644
	ldr	r0, [pc, #380]
	bl	sub_08014644
	movs	r1, #185
	lsls	r1, r1, #4
	adds	r1, #255
	ldr	r0, [pc, #372]
	bl	sub_080145a8
	ldr	r1, [pc, #368]
	ldr	r2, [pc, #372]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	lsls	r3, r3, #2
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816cb04
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #0
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0816cb04:
	ldr	r3, [pc, #320]
	strh	r0, [r3, #0]
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	movs	r2, #130
	ldr	r3, [pc, #312]
	lsls	r2, r2, #2
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816cb3a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	ldr	r2, [pc, #292]
	adds	r3, r3, r1
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #44
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0816cb3a:
	ldr	r2, [pc, #268]
	strh	r0, [r2, #0]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	lsls	r3, r3, #2
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816cb6c
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
.L_0816cb6c:
	ldr	r3, [pc, #216]
	strh	r0, [r3, #0]
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	movs	r2, #130
	ldr	r3, [pc, #208]
	lsls	r2, r2, #2
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816cba6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #252
	adds	r3, r3, r1
	lsls	r2, r2, #6
	adds	r3, #4
	adds	r2, #68
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0816cba6:
	ldr	r1, [pc, #160]
	strh	r0, [r1, #0]
.L_0816cbaa:
	mov	r2, fp
	cmp	r2, #18
	bne.n	.L_0816cbc2
	ldr	r3, [sp, #84]
	movs	r1, #1
	ldr	r0, [r3, #8]
	negs	r1, r1
	movs	r2, #2
	movs	r3, #0
	str	r1, [sp, #0]
	bl	sub_0814cd48
.L_0816cbc2:
	mov	r1, fp
	cmp	r1, #36
	bne.n	.L_0816cc18
	ldr	r3, [sp, #80]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r3, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [sp, #80]
	adds	r1, #4
	adds	r2, r3, r1
	ldr	r1, [pc, #104]
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [pc, #100]
	ldrh	r3, [r2, #0]
	adds	r0, r3, #0
	movs	r3, #130
	lsls	r3, r3, #2
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0816cc14
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
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
.L_0816cc14:
	ldr	r3, [pc, #48]
	strh	r0, [r3, #0]
.L_0816cc18:
	mov	r3, fp
	subs	r3, #34
	cmp	r3, #5
	bhi.n	.L_0816cc50
	ldr	r1, [sp, #84]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816cc2c
	ldr	r2, [pc, #4]
	b.n	.L_0816cc58
.L_0816cc2c:
	ldr	r2, [pc, #4]
	b.n	.L_0816cc58
	.4byte 0x000020f0
	.4byte 0x000000d0
	.4byte 0x08143425
	.4byte 0x08143265
	.4byte 0x08143001
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0xf000
	.2byte 0xffff
.L_0816cc50:
	mov	r2, fp
	cmp	r2, #40
	bne.n	.L_0816cc64
	ldr	r2, [pc, #32]
.L_0816cc58:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
.L_0816cc64:
	mov	r0, fp
	subs	r0, #36
	cmp	r0, #25
	bhi.n	sub_0816cd22
	adds	r6, r0, #0
	cmp	r6, #12
	ble.n	.L_0816cc7c
	movs	r6, #12
	b.n	.L_0816cc7c
	movs	r0, r0
	.2byte 0x00f0
	.2byte 0x0000
.L_0816cc7c:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	ldr	r3, [r1, #0]
	ldr	r5, [r2, #0]
	movs	r1, #12
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	movs	r1, #12
	adds	r7, r5, r0
	lsls	r0, r6, #6
	bl	sub_08002054
	adds	r0, #16
	mov	r9, r0
	lsls	r0, r6, #1
	ldr	r3, [pc, #44]
	adds	r0, r0, r6
	ldr	r5, [pc, #44]
	lsls	r0, r0, #3
	subs	r0, r0, r6
	subs	r3, r3, r7
	lsls	r0, r0, #1
	strh	r3, [r5, #4]
	movs	r1, #12
	negs	r0, r0
	bl	sub_08002054
	adds	r0, #78
	strh	r0, [r5, #6]
	ldr	r1, [sp, #84]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816ccd8
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	adds	r7, r3, #0
	adds	r7, #20
	b.n	.L_0816cce2
	.4byte 0x00000040
	.2byte 0x1120
	.2byte 0x0300
.L_0816ccd8:
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	adds	r7, r3, #0
	subs	r7, #20
.L_0816cce2:
	ldr	r2, [sp, #80]
	movs	r3, #216
	lsls	r3, r3, #7
	ldr	r4, [sp, #68]
	adds	r3, #192
	movs	r1, #48
	adds	r2, r2, r3
	negs	r1, r1
	subs	r7, #10
	mov	r8, r2
	add	r9, r1
	mov	sl, r7
	movs	r6, #20
	movs	r5, #40
	mov	r1, r8
	mov	r2, sl
	mov	r3, r9
	str	r4, [sp, #8]
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #76]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4641
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #76]
	mov	r2, sl
	mov	r3, r9
	ldr	r4, [sp, #8]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465a
	cmp	r2, #34
	bls.n	.L_0816cd2a
	b.n	.L_0816cefa
.L_0816cd2a:
	str	r2, [sp, #44]
	cmp	r2, #16
	ble.n	.L_0816cd34
	movs	r3, #16
	str	r3, [sp, #44]
.L_0816cd34:
	ldr	r1, [sp, #44]
	movs	r3, #64
	lsls	r2, r1, #1
	subs	r7, r3, r2
	movs	r6, #16
	mov	r2, fp
	movs	r0, #120
	negs	r6, r6
	cmp	r2, #0
	blt.n	.L_0816cd72
	cmp	r2, #16
	ble.n	.L_0816cd4e
	movs	r2, #16
.L_0816cd4e:
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #0]
	subs	r3, #120
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0816cd5c
	adds	r3, #15
.L_0816cd5c:
	asrs	r3, r3, #4
	adds	r0, r3, #0
	movs	r3, #46
	muls	r3, r2
	adds	r0, #120
	cmp	r3, #0
	bge.n	.L_0816cd6c
	adds	r3, #15
.L_0816cd6c:
	asrs	r3, r3, #4
	adds	r6, r3, #0
	subs	r6, #16
.L_0816cd72:
	ldr	r2, [sp, #80]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #208
	adds	r1, r7, #0
	lsls	r0, r0, #5
	adds	r5, r2, r3
	bl	sub_08002054
	movs	r3, #64
	subs	r3, r3, r0
	lsls	r3, r3, #8
	str	r3, [r5, #0]
	ldr	r1, [sp, #80]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #212
	adds	r2, r1, r3
	movs	r0, #128
	lsls	r3, r6, #8
	str	r3, [r2, #0]
	adds	r1, r7, #0
	lsls	r0, r0, #6
	bl	sub_08002054
	movs	r2, #238
	ldr	r1, [sp, #80]
	lsls	r2, r2, #7
	adds	r2, #216
	adds	r3, r1, r2
	ldrh	r2, [r3, #0]
	strh	r0, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #218
	adds	r2, r1, r3
	ldrh	r3, [r2, #0]
	movs	r3, #128
	movs	r1, #0
	lsls	r3, r3, #1
	strh	r3, [r2, #0]
	movs	r2, #64
	str	r1, [sp, #48]
	str	r1, [sp, #20]
	negs	r2, r2
	mov	r9, r2
	mov	sl, r1
.L_0816cdd0:
	mov	r0, sl
	bl	sub_08002090
	negs	r0, r0
	lsls	r5, r0, #1
	adds	r5, r5, r0
	mov	r0, sl
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsls	r5, r5, #4
	ldr	r1, [sp, #44]
	asrs	r5, r5, #16
	asrs	r2, r3, #16
	mov	r3, r9
	subs	r5, r5, r3
	adds	r3, r1, #0
	muls	r3, r5
	cmp	r3, #0
	bge.n	.L_0816cdfe
	adds	r3, #15
.L_0816cdfe:
	asrs	r3, r3, #4
	add	r3, r9
	mov	r8, r3
	adds	r3, r2, #0
	ldr	r2, [sp, #44]
	subs	r3, #56
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0816ce12
	adds	r3, #15
.L_0816ce12:
	ldr	r1, [sp, #20]
	asrs	r3, r3, #4
	adds	r2, r3, #0
	lsls	r3, r1, #2
	ldr	r1, [sp, #56]
	adds	r2, #56
	movs	r7, #0
	adds	r6, r3, r1
.L_0816ce22:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	str	r2, [sp, #16]
	bl	sub_08002096
	ldr	r2, [sp, #16]
	adds	r7, #1
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #0]
	mov	r3, r8
	strb	r3, [r6, #1]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #16]
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r6, #2]
	adds	r6, #4
	cmp	r7, #16
	bne.n	.L_0816ce22
	movs	r1, #8
	ldr	r3, [sp, #20]
	add	r9, r1
	ldr	r1, [sp, #48]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #136
	adds	r3, #16
	adds	r1, #1
	add	sl, r2
	str	r3, [sp, #20]
	str	r1, [sp, #48]
	cmp	r1, #16
	bne.n	.L_0816cdd0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #644]
	ldr	r3, [sp, #96]
	movs	r1, #8
	ands	r3, r2
	ldr	r2, [pc, #640]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #96]
	ldr	r3, [pc, #632]
	add	r2, sp, #96
	adds	r5, r0, #0
	str	r3, [r2, #4]
	movs	r3, #3
	str	r3, [r5, #4]
	str	r1, [r5, #0]
	str	r2, [r5, #16]
	ldr	r1, [sp, #52]
	movs	r3, #0
	str	r1, [r5, #8]
	ldr	r2, [sp, #60]
	strb	r3, [r5, #24]
	strb	r3, [r5, #25]
	str	r2, [r5, #12]
	bl	sub_08014de4
	mov	r3, fp
	cmp	r3, #7
	ble.n	.L_0816ceda
	lsls	r0, r3, #3
	subs	r0, r0, r3
	lsls	r3, r0, #5
	ldr	r1, [pc, #588]
	adds	r0, r0, r3
	lsls	r0, r0, #2
	adds	r0, r0, r1
	bl	sub_08002096
	ldr	r2, [sp, #40]
	asrs	r0, r0, #5
	adds	r2, r2, r0
	lsls	r0, r2, #1
	adds	r0, r0, r2
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	str	r2, [sp, #40]
	bl	sub_08015024
.L_0816ceda:
	mov	r3, fp
	lsls	r0, r3, #9
	bl	sub_08015068
	movs	r2, #128
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #60]
	lsls	r2, r2, #1
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
.L_0816cefa:
	mov	r1, fp
	cmp	r1, #48
	bne.n	sub_0816cf14
	movs	r0, #145
	bl	sub_081c0010
	movs	r1, #128
	ldr	r3, [pc, #512]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	ldr	r2, [pc, #512]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465a
	cmp	r2, #64
	bne.n	.L_0816cf20
	movs	r0, #134
	bl	sub_081180e8
.L_0816cf20:
	mov	r3, fp
	cmp	r3, #47
	ble.n	.L_0816d022
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #452]
	ldr	r3, [sp, #88]
	ldr	r1, [sp, #80]
	ands	r3, r2
	movs	r2, #6
	orrs	r3, r2
	ldr	r2, [pc, #444]
	adds	r6, r0, #0
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	movs	r2, #156
	lsls	r2, r2, #7
	adds	r2, #16
	str	r3, [sp, #88]
	adds	r3, r1, r2
	add	r2, sp, #88
	str	r3, [r2, #4]
	movs	r3, #7
	str	r3, [r6, #0]
	ldr	r3, [pc, #432]
	str	r2, [r6, #16]
	str	r3, [r6, #8]
	mov	r3, r8
	str	r3, [r6, #12]
	movs	r3, #0
	strb	r3, [r6, #24]
	strb	r3, [r6, #25]
	movs	r7, #0
.L_0816cf72:
	ldr	r3, [pc, #420]
	ldrb	r3, [r3, r7]
	adds	r1, r3, #0
	adds	r1, #48
	cmp	fp, r1
	ble.n	.L_0816d010
	mov	r3, fp
	subs	r2, r3, r1
	ldr	r3, [pc, #408]
	ldrb	r3, [r3, r7]
	muls	r2, r3
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #8
	adds	r5, r3, r2
	mov	r2, fp
	subs	r3, r1, r2
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r3, #0
	ble.n	.L_0816cfa6
	movs	r3, #0
.L_0816cfa6:
	movs	r1, #64
	negs	r1, r1
	cmp	r3, r1
	ble.n	.L_0816d010
	str	r3, [r6, #20]
	bl	sub_08014de4
	ldr	r2, [sp, #28]
	ldr	r0, [r2, #0]
	movs	r2, #0
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [pc, #352]
	asrs	r0, r0, #1
	ldrsb	r1, [r3, r7]
	subs	r0, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	bl	sub_08015160
	asrs	r2, r5, #1
	adds	r0, r2, #0
	lsls	r1, r5, #2
	bl	sub_080151e4
	movs	r0, #250
	lsls	r0, r0, #2
	bl	sub_08015024
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_0816cff4
	lsls	r0, r7, #2
	add	r0, fp
	lsls	r0, r0, #11
	bl	sub_08015068
	b.n	.L_0816d000
.L_0816cff4:
	lsls	r0, r7, #2
	mov	r3, fp
	subs	r0, r0, r3
	lsls	r0, r0, #11
	bl	sub_08015068
.L_0816d000:
	ldr	r0, [pc, #288]
	mov	r1, r8
	movs	r2, #32
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0816d010:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L_0816cf72
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
.L_0816d022:
	mov	r1, fp
	cmp	r1, #61
	bne.n	sub_0816d040
	ldr	r2, [sp, #64]
	movs	r0, #160
	movs	r3, #0
	lsls	r0, r0, #19
	movs	r1, #128
	str	r3, [r2, #16]
	adds	r0, #192
	ldr	r3, [pc, #212]
	lsls	r1, r1, #1
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	cmp	r3, #62
	bne.n	.L_0816d05a
	ldr	r1, [sp, #36]
	movs	r2, #206
	lsls	r2, r2, #3
	adds	r3, r1, r2
	movs	r2, #1
	ldrh	r1, [r3, #0]
	movs	r0, #1
	negs	r2, r2
	bl	sub_08118040
.L_0816d05a:
	mov	r3, fp
	subs	r3, #63
	cmp	r3, #4
	bhi.n	.L_0816d08c
	ldr	r1, [sp, #36]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #108
	adds	r3, r1, r2
	mov	r1, fp
	lsls	r2, r1, #13
	ldr	r1, [pc, #180]
	adds	r2, r2, r1
	str	r2, [r3, #0]
	ldr	r3, [sp, #36]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #108
	adds	r0, r3, r1
	movs	r1, #160
	lsls	r1, r1, #19
	adds	r1, #192
	movs	r3, #128
	bl	sub_081180f0
.L_0816d08c:
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L_0816d0a0
	ldr	r3, [sp, #80]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r3, r1
	movs	r3, #16
	str	r3, [r2, #0]
.L_0816d0a0:
	mov	r2, fp
	cmp	r2, #47
	ble.n	.L_0816d0ae
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
.L_0816d0ae:
	bl	sub_081434f8
	movs	r1, #240
	ldr	r3, [sp, #80]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #74
	beq.n	.L_0816d0d4
	bl	.L_0816c8e2
.L_0816d0d4:
	ldr	r0, [sp, #32]
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #72]
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
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02010000
	.4byte 0xffffe320
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x081990d0
	.4byte 0x08198b52
	.4byte 0x08198b56
	.4byte 0x08198b5a
	.4byte 0x08199050
	.4byte 0xfff84000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	str	r0, [sp, #36]
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r0, [r6, #96]
	ldr	r7, [r6, #92]
	str	r0, [sp, #32]
	movs	r0, #1
	ldr	r2, [r6, #48]
	str	r2, [sp, #24]
	ldr	r3, [r6, #100]
	str	r3, [sp, #20]
	bl	sub_081435e0
	movs	r2, #0
	ldr	r1, [sp, #20]
	movs	r3, #0
	ldr	r0, [pc, #608]
	bl	sub_08157cf4
	ldr	r0, [pc, #608]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #600]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a09
	movs	r4, #36
	ldrsh	r0, [r2, r4]
	bl	sub_08118098
	ldr	r4, [sp, #36]
	ldr	r5, [r0, #0]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r5, #8]
	asrs	r0, r0, #1
	str	r3, [r7, #0]
	movs	r1, #19
	ldr	r3, [r5, #12]
	adds	r3, r3, r0
	str	r3, [r7, #4]
	movs	r0, #104
	ldr	r3, [r5, #16]
	str	r3, [r7, #8]
	movs	r3, #0
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	bl	sub_081963ec
	ldr	r6, [r6, #104]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r7, r0
	movs	r3, #2
	str	r6, [sp, #28]
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r7, r3
	movs	r1, #200
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #504]
	bl	sub_080145a8
	movs	r0, #142
	bl	sub_081c0010
	ldr	r0, [sp, #24]
	movs	r4, #0
	adds	r0, #12
	str	r0, [sp, #12]
	mov	r9, r4
.L_0816d1ea:
	ldr	r2, [sp, #36]
	ldr	r0, [r2, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r3, [sp, #36]
	mov	fp, r0
	ldr	r0, [r3, #8]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	mov	r4, r9
	str	r0, [sp, #16]
	cmp	r4, #64
	bne.n	.L_0816d212
	movs	r0, #134
	bl	sub_081180e8
.L_0816d212:
	bl	sub_08014de4
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #12]
	bl	sub_080156e8
	mov	r0, r9
	cmp	r0, #72
	bne.n	.L_0816d238
	ldr	r2, [sp, #36]
	movs	r3, #0
	ldr	r0, [r2, #8]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0816d238:
	mov	r4, r9
	cmp	r4, #79
	bne.n	.L_0816d252
	ldr	r2, [sp, #36]
	movs	r3, #0
	ldr	r0, [r2, #8]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0816d252:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_0816d26c
	ldr	r3, [sp, #36]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_0816d26c:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	beq.n	.L_0816d274
	b.n	.L_0816d382
.L_0816d274:
	mov	r2, r9
	lsls	r2, r2, #10
	mov	r0, r9
	str	r2, [sp, #8]
	movs	r4, #0
	lsls	r0, r0, #9
	mov	r8, r4
	mov	sl, r0
	add	r6, sp, #52
	add	r5, sp, #40
.L_0816d288:
	bl	sub_08014e38
	ldr	r0, [r7, #0]
	ldr	r1, [r7, #4]
	ldr	r2, [r7, #8]
	bl	sub_08015160
	mov	r3, r8
	cmp	r3, #11
	bgt.n	.L_0816d2a4
	mov	r0, sl
	bl	sub_08015024
	b.n	.L_0816d2ae
.L_0816d2a4:
	movs	r0, #128
	lsls	r0, r0, #7
	add	r0, sl
	bl	sub_08015024
.L_0816d2ae:
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #85
	ldr	r4, [sp, #8]
	mov	r0, r8
	muls	r0, r3
	adds	r0, r0, r4
	bl	sub_080150e4
	movs	r3, #154
	adds	r3, #255
	mov	r0, r9
	muls	r0, r3
	bl	sub_08002096
	adds	r1, r0, #0
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #0
	bl	sub_08015160
	movs	r0, #0
	adds	r1, r5, #0
	str	r0, [r6, #0]
	str	r0, [r6, #4]
	str	r0, [r6, #8]
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08014ea8
	ldr	r2, [pc, #224]
	movs	r3, #12
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	movs	r3, #3
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	movs	r4, #6
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	subs	r3, #6
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r2, r8
	cmp	r2, #24
	bne.n	.L_0816d288
	adds	r0, r7, #0
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138058
	mov	r3, r9
	cmp	r3, #0
	ble.n	.L_0816d382
	mov	r3, fp
	ldr	r2, [r3, #12]
	ldr	r3, [sp, #16]
	mov	r4, fp
	adds	r2, r2, r3
	ldr	r3, [r7, #4]
	ldr	r0, [r4, #8]
	subs	r2, r2, r3
	mov	r3, fp
	ldr	r1, [r3, #16]
	ldr	r3, [r7, #8]
	ldr	r4, [r7, #0]
	subs	r1, r1, r3
	ldr	r3, [r7, #12]
	subs	r0, r0, r4
	asrs	r0, r0, #10
	adds	r3, r3, r0
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	asrs	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	asrs	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r7, #20]
	mov	r2, fp
	ldr	r0, [r2, #8]
	cmp	r0, #0
	bge.n	.L_0816d370
	negs	r0, r0
.L_0816d370:
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L_0816d378
	negs	r3, r3
.L_0816d378:
	cmp	r0, r3
	bge.n	.L_0816d382
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_0816d382:
	bl	sub_081434f8
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r7, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	r9, r0
	mov	r2, r9
	cmp	r2, #80
	beq.n	.L_0816d3a4
	b.n	.L_0816d1ea
.L_0816d3a4:
	ldr	r0, [pc, #40]
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
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x0000017f
	.4byte 0x03000730
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
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	ldr	r2, [r5, #96]
	sub	sp, #68
	mov	sl, r0
	movs	r0, #1
	str	r2, [sp, #40]
	mov	fp, r1
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r1, #224
	adds	r2, #48
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	add	r1, fp
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r5, [sp, #36]
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	b.n	.L_0816d444
	.4byte 0x00000100
	.4byte 0x00000000
	.2byte 0x014d
	.2byte 0x0000
.L_0816d444:
	adds	r2, #132
	add	r2, fp
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #700]
	bl	sub_080145a8
	movs	r3, #0
	str	r3, [sp, #32]
	str	r3, [sp, #28]
	str	r3, [sp, #24]
	mov	r4, sl
	ldr	r0, [r4, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r1, #48
	str	r0, [sp, #20]
	bl	sub_08020098
	mov	r1, sp
	mov	r2, sp
	adds	r1, #56
	adds	r2, #44
	str	r1, [sp, #8]
	str	r2, [sp, #12]
	movs	r5, #0
	mov	r9, r5
.L_0816d480:
	mov	r3, sl
	ldr	r0, [r3, #8]
	ldr	r1, [sp, #8]
	bl	sub_0815e20c
	mov	r5, sl
	ldr	r1, [sp, #12]
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	bl	sub_0815e20c
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_0816d4a2
	movs	r0, #157
	bl	sub_081c0010
.L_0816d4a2:
	mov	r2, r9
	cmp	r2, #64
	bne.n	.L_0816d4ae
	movs	r0, #104
	bl	sub_081c0010
.L_0816d4ae:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0816d524
	movs	r4, #192
	lsls	r4, r4, #15
	movs	r7, #0
	movs	r6, #0
	mov	r8, r4
	mov	r5, fp
.L_0816d4c0:
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816d4da
	bl	sub_08014878
	movs	r1, #40
	bl	sub_0800206c
	adds	r0, #8
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	b.n	.L_0816d4ec
.L_0816d4da:
	bl	sub_08014878
	movs	r1, #40
	bl	sub_0800206c
	movs	r3, #120
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
.L_0816d4ec:
	bl	sub_08014878
	mov	r2, r8
	str	r2, [r5, #4]
	str	r6, [r5, #12]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r3, #32
	negs	r3, r3
	lsls	r3, r3, #11
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	str	r3, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r7, #1
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	add	r8, r3
	adds	r5, #28
	cmp	r7, #21
	bne.n	.L_0816d4c0
.L_0816d524:
	mov	r4, r9
	cmp	r4, #52
	bne.n	.L_0816d532
	ldr	r0, [sp, #20]
	movs	r1, #2
	bl	sub_08020090
.L_0816d532:
	mov	r5, r9
	cmp	r5, #64
	ble.n	.L_0816d56a
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816d54a
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #28]
	adds	r2, r2, r3
	str	r2, [sp, #32]
	b.n	.L_0816d552
.L_0816d54a:
	ldr	r4, [sp, #32]
	ldr	r5, [sp, #28]
	subs	r4, r4, r5
	str	r4, [sp, #32]
.L_0816d552:
	ldr	r1, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r1, r1, r2
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_0816d566
	adds	r3, #63
.L_0816d566:
	asrs	r3, r3, #6
	str	r3, [sp, #28]
.L_0816d56a:
	ldr	r3, [sp, #32]
	ldr	r4, [sp, #8]
	asrs	r3, r3, #16
	str	r3, [sp, #16]
	movs	r1, #128
	ldr	r2, [r4, #0]
	lsls	r1, r1, #19
	subs	r2, r2, r3
	movs	r3, #64
	subs	r3, r3, r2
	adds	r1, #40
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r5, #0
	mov	r8, r5
	movs	r7, #0
	mov	r6, fp
.L_0816d58c:
	movs	r1, #5
	adds	r0, r7, #0
	bl	sub_08002064
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	ldr	r0, [r6, #20]
	bl	sub_08002054
	movs	r1, #3
	bl	sub_08002064
	ldr	r3, [pc, #364]
	adds	r5, r5, r0
	ldrb	r4, [r3, r5]
	movs	r1, #2
	ldrsh	r2, [r6, r1]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [sp, #16]
	adds	r2, r2, r3
	movs	r1, #6
	ldrsh	r3, [r6, r1]
	ldr	r1, [pc, #344]
	ldrb	r0, [r1, r5]
	lsls	r5, r5, #1
	lsrs	r1, r0, #1
	subs	r3, r3, r1
	ldr	r1, [pc, #340]
	ldrh	r1, [r1, r5]
	movs	r5, #240
	lsls	r5, r5, #4
	add	r1, fp
	adds	r5, #60
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r1, r1, r5
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
	cmp	r3, #0
	bne.n	.L_0816d61e
	mov	r5, sl
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0816d604
	ldr	r4, [sp, #8]
	movs	r1, #2
	ldrsh	r3, [r6, r1]
	ldr	r2, [r4, #0]
	ldr	r5, [sp, #12]
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	subs	r3, #64
	cmp	r2, r3
	ble.n	.L_0816d61e
	b.n	.L_0816d618
.L_0816d604:
	ldr	r4, [sp, #8]
	movs	r1, #2
	ldrsh	r3, [r6, r1]
	ldr	r2, [r4, #0]
	ldr	r5, [sp, #12]
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	subs	r3, #64
	cmp	r2, r3
	bge.n	.L_0816d61e
.L_0816d618:
	movs	r3, #1
	str	r3, [r6, #24]
	mov	r8, r3
.L_0816d61e:
	movs	r2, #0
	adds	r0, r6, #0
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r6, #20]
	ldr	r2, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	adds	r3, #64
	cmp	r9, r3
	ble.n	.L_0816d660
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816d64c
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #216]
	adds	r3, r3, r2
	b.n	.L_0816d654
.L_0816d64c:
	ldr	r3, [r6, #12]
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r3, r3, r4
.L_0816d654:
	str	r3, [r6, #12]
	ldr	r3, [r6, #8]
	cmp	r3, #1
	ble.n	.L_0816d660
	subs	r3, #1
	str	r3, [r6, #8]
.L_0816d660:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #21
	bne.n	.L_0816d58c
	ldr	r5, [sp, #24]
	cmp	r5, #0
	ble.n	.L_0816d672
	subs	r5, #1
	str	r5, [sp, #24]
.L_0816d672:
	mov	r1, r8
	cmp	r1, #1
	bne.n	.L_0816d6c0
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_0816d688
	movs	r3, #8
	movs	r0, #134
	str	r3, [sp, #24]
	bl	sub_081c0010
.L_0816d688:
	mov	r5, sl
	movs	r3, #150
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	movs	r2, #128
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r2, r2, #10
	movs	r1, #1
	lsls	r3, r3, #11
	str	r2, [sp, #0]
	bl	sub_0815f000
	movs	r3, #4
	movs	r1, #36
	ldrsh	r0, [r5, r1]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
.L_0816d6c0:
	movs	r0, #2
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #107
	beq.n	.L_0816d6ea
	b.n	.L_0816d480
.L_0816d6ea:
	ldr	r0, [sp, #20]
	movs	r1, #16
	bl	sub_08020098
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08143001
	.4byte 0x0819749e
	.4byte 0x081974ad
	.4byte 0x081974bc
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #72
	str	r0, [sp, #36]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #96]
	ldr	r0, [r3, #92]
	str	r2, [sp, #32]
	mov	fp, r0
	ldr	r3, [r3, #100]
	movs	r0, #1
	str	r3, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #224
	adds	r2, #82
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #52]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #182
	lsls	r1, r1, #4
	ldr	r0, [pc, #40]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #28]
	movs	r3, #0
	ldr	r0, [pc, #24]
	bl	sub_08157cf4
	ldr	r0, [pc, #24]
	bl	sub_08013300
	b.n	.L_0816d7a0
	movs	r0, r0
	.4byte 0x00000c10
	.4byte 0x00000113
	.4byte 0x00000184
	.4byte 0x00000134
	.2byte 0x0148
	.2byte 0x0000
.L_0816d7a0:
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #612]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #580]
	bl	sub_080145a8
	ldr	r4, [sp, #36]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r3, [sp, #36]
	str	r0, [sp, #24]
	mov	r4, sp
	adds	r4, #60
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r4, #0
	str	r4, [sp, #20]
	bl	sub_0815e21c
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r2, #238
	lsls	r2, r2, #7
	asrs	r3, r3, #1
	adds	r2, #168
	str	r3, [r0, #0]
	add	r2, fp
	movs	r3, #8
	str	r3, [r2, #0]
	mov	r3, sp
	adds	r3, #40
	str	r3, [sp, #12]
	movs	r2, #0
	mov	r9, r2
.L_0816d810:
	mov	r4, r9
	cmp	r4, #0
	bne.n	.L_0816d8f6
	ldr	r0, [sp, #24]
	movs	r2, #0
	str	r4, [r0, #12]
	mov	r8, r2
	mov	r7, fp
.L_0816d820:
	bl	sub_08014878
	movs	r6, #255
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r6, r0
	adds	r6, r6, r3
	bl	sub_08014878
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	ands	r5, r0
	ldr	r0, [sp, #20]
	movs	r4, #128
	ldr	r3, [r0, #0]
	lsls	r4, r4, #7
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	adds	r5, r5, r4
	bl	sub_08014878
	movs	r2, #31
	ands	r2, r0
	movs	r3, #96
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	movs	r2, #1
	ands	r3, r0
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_0816d820
	movs	r7, #224
	movs	r4, #0
	lsls	r7, r7, #2
	mov	r8, r4
	add	r7, fp
.L_0816d894:
	bl	sub_08014878
	movs	r5, #127
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #254
	adds	r6, r0, #0
	ldr	r0, [sp, #24]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r6, r3
	ldr	r3, [r0, #8]
	adds	r5, #128
	str	r3, [r7, #0]
	movs	r3, #208
	lsls	r3, r3, #14
	str	r3, [r7, #4]
	ldr	r3, [r0, #16]
	str	r3, [r7, #8]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #16]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #1
	asrs	r3, r3, #7
	str	r3, [r7, #20]
	add	r8, r2
	movs	r3, #0
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_0816d894
.L_0816d8f6:
	mov	r4, r9
	cmp	r4, #4
	bne.n	.L_0816d902
	movs	r0, #212
	bl	sub_081c0010
.L_0816d902:
	mov	r0, r9
	cmp	r0, #10
	bne.n	.L_0816d93a
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #144
	bl	sub_081c0010
	ldr	r3, [sp, #36]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r4, [sp, #24]
	movs	r3, #0
	str	r3, [r4, #72]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r4, #12]
.L_0816d93a:
	mov	r0, r9
	cmp	r0, #80
	bne.n	.L_0816d94a
	movs	r3, #171
	ldr	r2, [sp, #24]
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r2, #72]
.L_0816d94a:
	mov	r3, r9
	cmp	r3, #24
	bne.n	.L_0816d964
	ldr	r4, [sp, #36]
	movs	r3, #0
	movs	r1, #1
	ldr	r0, [r4, #8]
	negs	r1, r1
	str	r3, [sp, #0]
	movs	r2, #3
	subs	r3, #1
	bl	sub_0814cd48
.L_0816d964:
	mov	r0, r9
	cmp	r0, #40
	bne.n	.L_0816d970
	movs	r0, #142
	bl	sub_081c0010
.L_0816d970:
	mov	r2, r9
	cmp	r2, #32
	bne.n	.L_0816d97c
	movs	r0, #134
	bl	sub_081180e8
.L_0816d97c:
	mov	r3, r9
	cmp	r3, #64
	bne.n	.L_0816d996
	ldr	r4, [sp, #36]
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r4, #8]
	movs	r1, #7
	str	r3, [sp, #0]
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0816d996:
	mov	r0, r9
	cmp	r0, #88
	bne.n	.L_0816d9b0
	ldr	r2, [sp, #36]
	movs	r3, #0
	ldr	r0, [r2, #8]
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_0816d9b0:
	mov	r3, r9
	cmp	r3, #7
	ble.n	.L_0816da56
	ldr	r4, [sp, #20]
	ldr	r1, [r4, #0]
	cmp	r3, #79
	bgt.n	.L_0816d9ca
	lsls	r3, r3, #1
	add	r3, r9
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #96
	b.n	.L_0816d9d8
.L_0816d9ca:
	mov	r0, r9
	lsls	r3, r0, #1
	add	r3, r9
	movs	r2, #140
	lsls	r3, r3, #1
	lsls	r2, r2, #2
	subs	r5, r2, r3
.L_0816d9d8:
	cmp	r5, #0
	ble.n	.L_0816da56
	cmp	r5, #80
	ble.n	.L_0816d9e6
	movs	r5, #80
	movs	r7, #1
	b.n	.L_0816d9e8
.L_0816d9e6:
	movs	r7, #0
.L_0816d9e8:
	movs	r2, #0
	adds	r6, r1, #0
	movs	r3, #104
	mov	r8, r2
	subs	r6, #14
	mov	sl, r3
.L_0816d9f4:
	mov	r4, r8
	cmp	r4, #0
	bne.n	.L_0816da14
	movs	r0, #104
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	str	r7, [sp, #0]
	bl	sub_08196404
	b.n	.L_0816da22
	movs	r0, r0
	.4byte 0x03000730
	.2byte 0x3001
	.2byte 0x0814
.L_0816da14:
	movs	r0, #104
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	str	r7, [sp, #0]
	bl	sub_08196404
.L_0816da22:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #224
	mov	ip, r3
	str	r3, [sp, #40]
	mov	r4, sl
	movs	r3, #14
	lsls	r1, r1, #3
	adds	r2, r6, #0
	str	r3, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #32]
	add	r1, fp
	subs	r3, r4, r5
	mov	lr, ip
	.2byte 0xf800
	.2byte 0x2068
	bl	sub_0801314c
	movs	r0, #1
	add	r8, r0
	mov	r2, r8
	adds	r6, #14
	cmp	r2, #2
	bne.n	.L_0816d9f4
.L_0816da56:
	ldr	r3, [sp, #36]
	ldr	r1, [sp, #12]
	ldr	r0, [r3, #4]
	bl	sub_08144aac
	movs	r4, #0
	mov	r8, r4
	mov	r5, fp
.L_0816da66:
	mov	r0, r8
	lsrs	r3, r0, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #8
	cmp	r9, r3
	blt.n	.L_0816dad6
	ldr	r0, [r5, #24]
	cmp	r0, #28
	bgt.n	.L_0816dad6
	movs	r2, #3
	movs	r1, #3
	str	r2, [sp, #8]
	bl	sub_08002054
	movs	r3, #2
	ldrsh	r6, [r5, r3]
	movs	r4, #6
	ldrsh	r7, [r5, r4]
	ldr	r2, [sp, #8]
	cmp	r0, #6
	ble.n	.L_0816da94
	movs	r0, #6
.L_0816da94:
	mov	r3, r8
	ands	r3, r2
	movs	r4, #4
	cmp	r3, #0
	beq.n	.L_0816daa0
	movs	r4, #0
.L_0816daa0:
	ldr	r3, [pc, #408]
	lsls	r2, r0, #1
	ldrh	r1, [r3, r2]
	ldr	r3, [pc, #408]
	movs	r0, #182
	lsls	r0, r0, #4
	add	r1, fp
	adds	r1, r1, r0
	ldrh	r0, [r3, r2]
	lsrs	r3, r0, #1
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #12]
	subs	r2, r6, r3
	ldr	r4, [r4, r0]
	subs	r3, r7, r3
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
	adds	r0, r5, #0
	adds	r3, #1
	str	r3, [r5, #24]
	movs	r1, #62
	ldr	r2, [pc, #368]
	bl	sub_08138086
.L_0816dad6:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #16
	bne.n	.L_0816da66
	mov	r4, r9
	cmp	r4, #31
	ble.n	.L_0816dbde
	ldr	r2, [sp, #36]
	movs	r6, #224
	ldr	r0, [r2, #8]
	bl	sub_08118098
	ldr	r3, [sp, #36]
	ldr	r7, [r0, #0]
	ldr	r0, [r3, #8]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	str	r0, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r0, r5, #0
	adds	r1, #12
	bl	sub_080156e8
	movs	r0, #48
	movs	r4, #0
	add	r0, sp
	lsls	r6, r6, #2
	mov	r8, r4
	mov	sl, r0
	add	r6, fp
.L_0816db26:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_0816dbd2
	mov	r2, r8
	mov	r1, sl
	movs	r5, #1
	adds	r0, r6, #0
	ands	r5, r2
	bl	sub_0815e1ec
	adds	r5, #9
	ldr	r2, [pc, #264]
	lsls	r0, r5, #1
	subs	r3, r0, #2
	mov	r4, sl
	ldrh	r1, [r2, r3]
	ldr	r2, [r4, #0]
	ldr	r3, [sp, #28]
	asrs	r2, r2, #1
	adds	r1, r3, r1
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [r4, #4]
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #62
	movs	r2, #0
	bl	sub_08138058
	mov	r3, r8
	adds	r3, #32
	cmp	r9, r3
	ble.n	.L_0816dbd2
	ldr	r3, [sp, #16]
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #16]
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	ldr	r0, [r7, #8]
	subs	r2, r2, r3
	ldr	r3, [r6, #8]
	ldr	r4, [r6, #0]
	subs	r1, r1, r3
	ldr	r3, [r6, #12]
	subs	r0, r0, r4
	asrs	r0, r0, #7
	adds	r3, r3, r0
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	asrs	r2, r2, #7
	adds	r3, r3, r2
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	asrs	r1, r1, #7
	adds	r3, r3, r1
	adds	r1, r4, #0
	str	r3, [r6, #20]
	cmp	r1, #0
	bge.n	.L_0816dbaa
	negs	r1, r1
.L_0816dbaa:
	ldr	r3, [r7, #8]
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_0816dbb4
	negs	r2, r3
.L_0816dbb4:
	cmp	r1, r2
	blt.n	.L_0816dbd2
	cmp	r3, #0
	bge.n	.L_0816dbc4
	lsrs	r3, r4, #31
	cmp	r3, #0
	bne.n	.L_0816dbcc
	b.n	.L_0816dbd2
.L_0816dbc4:
	mvns	r3, r4
	lsrs	r3, r3, #31
	cmp	r3, #0
	beq.n	.L_0816dbd2
.L_0816dbcc:
	movs	r4, #1
	negs	r4, r4
	str	r4, [r6, #24]
.L_0816dbd2:
	movs	r0, #1
	add	r8, r0
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L_0816db26
.L_0816dbde:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	mov	r3, r9
	cmp	r3, #7
	bgt.n	.L_0816dbfa
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	b.n	.L_0816dc02
.L_0816dbfa:
	movs	r0, #16
	movs	r1, #16
	bl	sub_08158ce0
.L_0816dc02:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, fp
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	movs	r4, #1
	add	r9, r4
	mov	r0, r9
	cmp	r0, #106
	beq.n	.L_0816dc24
	b.n	.L_0816d810
.L_0816dc24:
	ldr	r0, [pc, #36]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08198b5e
	.4byte 0x08198b6c
	.4byte 0xffffe000
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #160
	str	r0, [sp, #64]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	str	r0, [sp, #60]
	ldr	r3, [r3, #92]
	str	r3, [sp, #56]
	bl	sub_0813ba50
	ldr	r1, [sp, #64]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0816dc84
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08143b20
	b.n	.L_0816dc8c
.L_0816dc84:
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08143a88
.L_0816dc8c:
	ldr	r2, [sp, #64]
	movs	r3, #2
	ldr	r1, [r2, #4]
	adds	r3, #255
	lsls	r1, r1, #4
	add	r2, sp, #136
	ldr	r0, [sp, #64]
	orrs	r1, r3
	add	r3, sp, #148
	bl	sub_0815585c
	movs	r1, #240
	ldr	r5, [pc, #84]
	lsls	r1, r1, #6
	ldr	r0, [pc, #84]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #60]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	ldr	r5, [pc, #52]
	movs	r6, #128
	strh	r3, [r2, #0]
	lsls	r6, r6, #19
	mov	r3, sp
	adds	r3, #80
	adds	r6, #32
	strh	r5, [r6, #0]
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #48]
	bl	sub_08144aac
	ldr	r4, [sp, #56]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r4, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #50
	movs	r1, #200
	b.n	.L_0816dd04
	.4byte 0x00001010
	.4byte 0x00000100
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
.L_0816dd04:
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #552]
	bl	sub_080145a8
	ldr	r0, [pc, #552]
	ldr	r1, [pc, #552]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #56]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #540]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #56]
	movs	r2, #176
	lsls	r2, r2, #4
	adds	r1, r4, r2
	ldr	r0, [pc, #524]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #56]
	movs	r4, #142
	lsls	r4, r4, #7
	adds	r1, r3, r4
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #508]
	bl	sub_08157cf4
	ldr	r0, [pc, #504]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #496]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23c0
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	str	r3, [sp, #44]
	movs	r1, #54
	ldrsh	r0, [r3, r1]
	ldr	r3, [sp, #64]
	str	r0, [sp, #40]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118098
	movs	r2, #128
	ldr	r0, [r0, #0]
	lsls	r2, r2, #19
	movs	r3, #0
	adds	r2, #40
	str	r0, [sp, #36]
	strh	r5, [r6, #0]
	str	r3, [r2, #0]
	ldr	r4, [sp, #44]
	mov	r5, sp
	mov	r6, sp
	adds	r4, #12
	adds	r5, #112
	adds	r6, #124
	str	r3, [sp, #52]
	str	r4, [sp, #16]
	str	r5, [sp, #24]
	str	r6, [sp, #20]
.L_0816dda0:
	ldr	r0, [sp, #52]
	cmp	r0, #0
	beq.n	.L_0816dda8
	b.n	.L_0816ded4
.L_0816dda8:
	add	r1, sp, #40
	ldr	r2, [sp, #44]
	ldrh	r1, [r1, #0]
	movs	r3, #7
	strh	r1, [r2, #54]
	movs	r2, #0
	mov	r9, r2
	mov	fp, r3
.L_0816ddb8:
	mov	r5, r9
	movs	r4, #0
	lsls	r5, r5, #3
	mov	r8, r4
	mov	sl, r5
.L_0816ddc2:
	mov	r6, r9
	cmp	r6, #0
	bne.n	.L_0816ddd2
	ldr	r3, [pc, #392]
	ldrsb	r1, [r3, r6]
	ldr	r3, [pc, #392]
	ldrsb	r2, [r3, r6]
	b.n	.L_0816ddf6
.L_0816ddd2:
	bl	sub_08014878
	ldr	r3, [pc, #380]
	mov	r1, r9
	ldrsb	r3, [r3, r1]
	movs	r2, #3
	ands	r0, r2
	adds	r1, r3, r0
	str	r1, [sp, #12]
	bl	sub_08014878
	ldr	r3, [pc, #364]
	mov	r4, r9
	ldrsb	r3, [r3, r4]
	movs	r5, #3
	ldr	r1, [sp, #12]
	ands	r0, r5
	adds	r2, r3, r0
.L_0816ddf6:
	mov	r0, r8
	movs	r3, #128
	lsls	r3, r3, #5
	lsls	r6, r0, #13
	adds	r6, r6, r3
	adds	r0, r6, #0
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	bl	sub_08002096
	ldr	r2, [sp, #8]
	ldr	r4, [pc, #332]
	adds	r3, r2, #0
	muls	r3, r0
	mov	r7, sl
	ldr	r1, [sp, #12]
	add	r7, r8
	lsls	r5, r7, #2
	adds	r5, r5, r4
	asrs	r3, r3, #16
	strb	r3, [r5, #0]
	negs	r3, r1
	strb	r3, [r5, #1]
	adds	r0, r6, #0
	bl	sub_08002090
	ldr	r2, [sp, #8]
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r5, #2]
	mov	r5, r9
	cmp	r5, #6
	bgt.n	.L_0816deae
	ldr	r6, [pc, #292]
	lsls	r3, r7, #1
	mov	r1, r8
	adds	r3, r3, r7
	lsls	r2, r1, #4
	lsls	r3, r3, #3
	adds	r0, r3, r6
	mov	r5, fp
	adds	r1, r2, #0
	adds	r6, #12
	strb	r5, [r0, #9]
	strb	r5, [r0, #7]
	adds	r1, #15
	adds	r5, r3, r6
	mov	r4, sl
	mov	r6, r8
	strb	r2, [r0, #8]
	strb	r1, [r0, #4]
	strb	r4, [r0, #5]
	strb	r1, [r0, #6]
	adds	r6, #1
	strb	r2, [r5, #4]
	strb	r2, [r5, #8]
	mov	r2, fp
	strb	r4, [r5, #5]
	strb	r2, [r5, #9]
	strb	r1, [r5, #6]
	strb	r4, [r5, #7]
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_0816de7c
	mov	r3, r8
	adds	r3, #8
.L_0816de7c:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, r6, r3
	mov	r1, sl
	adds	r4, r3, r1
	mov	r3, r8
	strb	r4, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0816de90
	adds	r3, #7
.L_0816de90:
	asrs	r3, r3, #3
	mov	r2, r8
	lsls	r3, r3, #3
	subs	r3, r2, r3
	add	r3, sl
	adds	r1, r3, #0
	adds	r2, r4, #0
	adds	r1, #8
	adds	r2, #8
	strb	r1, [r0, #1]
	strb	r2, [r0, #2]
	strb	r3, [r5, #0]
	strb	r1, [r5, #1]
	strb	r4, [r5, #2]
	b.n	.L_0816deb2
.L_0816deae:
	mov	r6, r8
	adds	r6, #1
.L_0816deb2:
	mov	r8, r6
	cmp	r6, #8
	beq.n	.L_0816deba
	b.n	.L_0816ddc2
.L_0816deba:
	movs	r4, #1
	add	r9, r4
	movs	r3, #8
	mov	r5, r9
	add	fp, r3
	cmp	r5, #8
	beq.n	.L_0816deca
	b.n	.L_0816ddb8
.L_0816deca:
	ldr	r3, [pc, #152]
	movs	r6, #0
	strb	r6, [r3, #0]
	strb	r6, [r3, #1]
	strb	r6, [r3, #2]
.L_0816ded4:
	ldr	r3, [sp, #52]
	subs	r3, #4
	cmp	r3, #79
	bhi.n	.L_0816df80
	ldr	r0, [sp, #52]
	movs	r2, #160
	lsls	r2, r2, #1
	cmp	r0, #63
	ble.n	.L_0816def4
	ldr	r1, [sp, #52]
	movs	r2, #200
	lsls	r3, r1, #2
	adds	r3, r3, r1
	lsls	r3, r3, #2
	lsls	r2, r2, #3
	subs	r2, r2, r3
.L_0816def4:
	ldr	r4, [sp, #64]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0816df08
	ldr	r5, [sp, #44]
	ldrh	r3, [r5, #54]
	adds	r6, r5, #0
	subs	r3, r3, r2
	strh	r3, [r6, #54]
	b.n	.L_0816df12
.L_0816df08:
	ldr	r0, [sp, #44]
	ldrh	r3, [r0, #54]
	adds	r1, r0, #0
	adds	r3, r3, r2
	strh	r3, [r1, #54]
.L_0816df12:
	ldr	r2, [sp, #64]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0816df6c
	ldr	r5, [sp, #44]
	ldr	r6, [pc, #72]
	movs	r4, #54
	ldrsh	r3, [r5, r4]
	cmp	r3, r6
	bge.n	.L_0816df80
	movs	r3, #201
	lsls	r3, r3, #8
	adds	r3, #80
	adds	r0, r5, #0
	strh	r3, [r0, #54]
	b.n	.L_0816df80
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0x000000c1
	.4byte 0x02014000
	.4byte 0x000000cd
	.4byte 0x000000cf
	.4byte 0x00000154
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x08198b7a
	.4byte 0x08198b82
	.4byte 0x02010000
	.4byte 0x02012000
	.4byte 0x02012540
	.2byte 0xc950
	.2byte 0xffff
.L_0816df6c:
	.2byte 0x9a0b
	movs	r1, #54
	ldrsh	r3, [r2, r1]
	movs	r2, #216
	lsls	r2, r2, #6
	adds	r2, #176
	cmp	r3, r2
	ble.n	.L_0816df80
	ldr	r3, [sp, #44]
	strh	r2, [r3, #54]
.L_0816df80:
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r5, [pc, #928]
	mov	r9, r0
	ldr	r4, [r5, #12]
	str	r4, [sp, #32]
	ldr	r6, [r5, #16]
	adds	r3, r6, #0
	subs	r3, #16
	str	r6, [sp, #28]
	str	r3, [r5, #16]
	bl	sub_08014de4
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #16]
	bl	sub_080156e8
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #24]
	ldr	r3, [r0, #8]
	movs	r2, #0
	str	r2, [r1, #4]
	str	r3, [r1, #0]
	ldr	r3, [r0, #16]
	str	r3, [r1, #8]
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	bl	sub_08015778
	ldr	r3, [sp, #20]
	ldr	r2, [r3, #0]
	cmp	r2, #63
	bgt.n	.L_0816dfd6
	ldr	r4, [sp, #20]
	movs	r3, #64
	str	r3, [r4, #0]
	movs	r2, #64
.L_0816dfd6:
	cmp	r2, #176
	ble.n	.L_0816dfe2
	ldr	r6, [sp, #20]
	movs	r3, #176
	str	r3, [r6, #0]
	movs	r2, #176
.L_0816dfe2:
	ldr	r1, [pc, #852]
	movs	r3, #184
	subs	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r0, [pc, #848]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_0816e020
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	ldr	r3, [sp, #20]
	lsls	r2, r2, #2
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	movs	r3, #64
	subs	r3, r3, r1
	adds	r2, #4
	lsls	r3, r3, #8
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	stmia	r2!, {r3}
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_0816e020:
	strh	r4, [r0, #0]
	ldr	r4, [sp, #52]
	cmp	r4, #63
	bhi.n	.L_0816e0f2
	cmp	r4, #0
	bne.n	.L_0816e094
	ldr	r7, [sp, #56]
	movs	r5, #0
	mov	r8, r5
	mov	sl, r5
.L_0816e034:
	bl	sub_08014878
	movs	r5, #192
	lsls	r5, r5, #2
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r6, #128
	lsls	r6, r6, #3
	adds	r5, r5, r6
	movs	r3, #255
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r6, r3
	ldr	r3, [r0, #8]
	mov	r1, sl
	str	r1, [r7, #4]
	str	r3, [r7, #0]
	ldr	r3, [r0, #16]
	adds	r0, r6, #0
	str	r3, [r7, #8]
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	mov	r2, sl
	asrs	r3, r3, #9
	str	r3, [r7, #12]
	str	r2, [r7, #16]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r5, #1
	asrs	r3, r3, #9
	mov	r4, r8
	add	r8, r5
	str	r3, [r7, #20]
	mov	r6, r8
	negs	r3, r4
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r6, #32
	bne.n	.L_0816e034
.L_0816e094:
	ldr	r6, [sp, #56]
	movs	r0, #0
	mov	r8, r0
	add	r7, sp, #100
.L_0816e09c:
	ldr	r3, [r6, #24]
	adds	r0, r3, #1
	str	r0, [r6, #24]
	cmp	r0, #23
	bhi.n	sub_0816e0e6
	movs	r1, #6
	bl	sub_08002054
	adds	r1, r7, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08015778
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #60
	lsls	r2, r2, #6
	bl	sub_08138058
	ldr	r1, [sp, #56]
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	lsls	r5, r5, #10
	movs	r4, #176
	adds	r5, r1, r5
	lsls	r4, r4, #4
	movs	r1, #32
	adds	r5, r5, r4
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #60]
	adds	r1, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2501
	add	r8, r5
	mov	r0, r8
	adds	r6, #28
	cmp	r0, #32
	bne.n	.L_0816e09c
.L_0816e0f2:
	ldr	r1, [sp, #52]
	cmp	r1, #29
	bgt.n	sub_0816e144
	ldr	r2, [sp, #36]
	add	r6, sp, #100
	ldr	r3, [r2, #8]
	lsls	r2, r1, #17
	str	r3, [r6, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	subs	r3, r3, r2
	str	r3, [r6, #4]
	lsls	r0, r1, #9
	bl	sub_08002096
	ldr	r4, [sp, #36]
	lsls	r0, r0, #5
	ldr	r3, [r4, #16]
	add	r5, sp, #88
	adds	r3, r3, r0
	str	r3, [r6, #8]
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08015778
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	ldr	r5, [sp, #48]
	ldr	r6, [sp, #56]
	ldr	r4, [r5, #4]
	movs	r5, #142
	lsls	r5, r5, #7
	subs	r2, #20
	subs	r3, #30
	ldr	r0, [sp, #60]
	adds	r1, r6, r5
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e0d
	cmp	r6, #4
	bne.n	.L_0816e150
	movs	r0, #156
	bl	sub_081c0010
.L_0816e150:
	ldr	r0, [sp, #52]
	cmp	r0, #30
	bne.n	.L_0816e15c
	movs	r0, #139
	bl	sub_081c0010
.L_0816e15c:
	ldr	r1, [sp, #52]
	cmp	r1, #60
	bne.n	.L_0816e168
	movs	r0, #212
	bl	sub_081c0010
.L_0816e168:
	ldr	r2, [sp, #52]
	cmp	r2, #64
	bne.n	.L_0816e17c
	movs	r0, #134
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_0816e17c:
	ldr	r3, [sp, #52]
	cmp	r3, #30
	bne.n	.L_0816e198
	ldr	r5, [sp, #64]
	movs	r3, #1
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	movs	r6, #0
	movs	r1, #9
	movs	r2, #5
	negs	r3, r3
	str	r6, [sp, #0]
	bl	sub_0814cd48
.L_0816e198:
	ldr	r0, [sp, #52]
	cmp	r0, #26
	bne.n	.L_0816e1ac
	ldr	r1, [sp, #56]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #8
	str	r3, [r2, #0]
.L_0816e1ac:
	ldr	r4, [sp, #52]
	cmp	r4, #64
	bne.n	.L_0816e1d2
	ldr	r5, [sp, #56]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #168
	movs	r2, #4
	adds	r3, r5, r6
	str	r2, [r3, #0]
	ldr	r3, [sp, #64]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_0816e1d2:
	ldr	r4, [sp, #52]
	cmp	r4, #70
	bne.n	.L_0816e1ec
	ldr	r6, [sp, #64]
	movs	r3, #4
	movs	r5, #36
	ldrsh	r0, [r6, r5]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_0816e1ec:
	ldr	r0, [sp, #52]
	cmp	r0, #56
	bne.n	.L_0816e292
	ldr	r1, [sp, #64]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r3, [sp, #64]
	ldr	r6, [r0, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	ldr	r4, [r6, #8]
	ldr	r3, [r2, #8]
	movs	r1, #100
	subs	r3, r3, r4
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #4
	mov	sl, r4
	str	r2, [sp, #8]
	bl	sub_08002054
	ldr	r2, [sp, #8]
	adds	r5, r0, #0
	ldr	r3, [r2, #16]
	ldr	r0, [r6, #16]
	movs	r1, #100
	subs	r3, r3, r0
	mov	r8, r0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #4
	bl	sub_08002054
	add	sl, r5
	add	r8, r0
	asrs	r5, r5, #8
	asrs	r0, r0, #8
	adds	r3, r0, #0
	muls	r3, r0
	adds	r2, r5, #0
	muls	r2, r5
	adds	r2, r2, r3
	adds	r0, r2, #0
	ldr	r3, [pc, #244]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x210a
	lsls	r0, r0, #8
	bl	sub_08002054
	adds	r3, r6, #0
	movs	r2, #1
	adds	r3, #88
	str	r0, [r6, #52]
	str	r0, [r6, #48]
	strb	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	ldr	r3, [pc, #216]
	movs	r1, #0
	str	r3, [r6, #72]
	adds	r3, r6, #0
	adds	r3, #90
	str	r1, [r6, #68]
	adds	r0, r6, #0
	strb	r2, [r3, #0]
	bl	sub_08020138
	adds	r0, r6, #0
	mov	r1, sl
	movs	r2, #0
	mov	r3, r8
	bl	sub_08020148
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
.L_0816e292:
	ldr	r2, [sp, #52]
	cmp	r2, #63
	bgt.n	.L_0816e29a
	b.n	.L_0816e412
.L_0816e29a:
	cmp	r2, #64
	bne.n	.L_0816e388
	movs	r1, #128
	ldr	r3, [pc, #164]
	ldr	r0, [sp, #60]
	lsls	r1, r1, #7
	ldr	r2, [pc, #164]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24ff
	lsls	r4, r4, #8
	ldr	r6, [sp, #56]
	movs	r3, #0
	adds	r4, #255
	mov	r8, r3
	mov	sl, r4
.L_0816e2ba:
	movs	r5, #7
	mov	r3, r8
	ands	r3, r5
	lsls	r7, r3, #13
	adds	r0, r7, #0
	mov	r5, r8
	bl	sub_08002096
	cmp	r5, #0
	bge.n	.L_0816e2d0
	adds	r5, #7
.L_0816e2d0:
	asrs	r5, r5, #3
	adds	r2, r5, #0
	muls	r2, r0
	ldr	r0, [sp, #36]
	lsls	r2, r2, #2
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl	sub_08014878
	mov	r1, sl
	lsls	r3, r5, #2
	ands	r0, r1
	adds	r3, r3, r5
	lsls	r3, r3, #17
	lsls	r0, r0, #2
	movs	r2, #128
	subs	r0, r0, r3
	lsls	r2, r2, #15
	adds	r0, r0, r2
	str	r0, [r6, #4]
	adds	r0, r7, #0
	bl	sub_08002090
	ldr	r4, [sp, #36]
	adds	r2, r5, #0
	muls	r2, r0
	ldr	r3, [r4, #16]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r6, #8]
	bl	sub_08014878
	mov	r5, sl
	ands	r0, r5
	movs	r1, #128
	lsls	r1, r1, #9
	lsls	r0, r0, #1
	adds	r0, r0, r1
	str	r0, [r6, #12]
	bl	sub_08014878
	ldr	r2, [pc, #40]
	ands	r0, r5
	adds	r0, r0, r2
	lsls	r3, r0, #1
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_0816e354
	movs	r0, r0
	.4byte 0x030011e0
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x030002d4
	.4byte 0x00013333
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0xce20
	.2byte 0xffff
.L_0816e354:
	.2byte 0xf6a6
	.2byte 0xfa90
	.2byte 0x4ba9
	ands	r0, r5
	adds	r0, r0, r3
	ldr	r3, [r6, #0]
	lsls	r0, r0, #1
	str	r0, [r6, #20]
	cmp	r3, #0
	bge.n	.L_0816e36e
	ldr	r3, [r6, #12]
	negs	r3, r3
	str	r3, [r6, #12]
.L_0816e36e:
	bl	sub_08014878
	movs	r3, #31
	movs	r4, #1
	ands	r3, r0
	add	r8, r4
	adds	r3, #32
	mov	r5, r8
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r5, #64
	bne.n	.L_0816e2ba
	b.n	.L_0816e40a
.L_0816e388:
	mov	r6, r9
	movs	r3, #2
	str	r3, [r6, #0]
	ldr	r3, [pc, #628]
	movs	r0, #0
	mov	r1, fp
	str	r0, [r6, #4]
	str	r3, [r6, #8]
	str	r1, [r6, #12]
	ldr	r2, [sp, #52]
	ldr	r5, [sp, #56]
	mov	r7, sp
	mov	r8, r0
	adds	r7, #79
	lsls	r6, r2, #10
.L_0816e3a6:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0816e3f8
	strb	r3, [r7, #0]
	subs	r3, #2
	str	r3, [r5, #24]
	movs	r2, #128
	mov	r3, r9
	str	r7, [r3, #20]
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #3
	bl	sub_08138058
	bl	sub_08014e38
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	ldr	r0, [r5, #0]
	bl	sub_08015160
	adds	r0, r6, #0
	bl	sub_08015024
	adds	r0, r6, #0
	bl	sub_080150e4
	movs	r0, #128
	lsls	r0, r0, #9
	bl	sub_0801521c
	ldr	r0, [pc, #544]
	mov	r1, fp
	movs	r2, #3
	bl	sub_081969ac
	mov	r0, r9
	bl	sub_08196a7c
	bl	sub_08014ea8
.L_0816e3f8:
	movs	r0, #1
	movs	r4, #128
	add	r8, r0
	lsls	r4, r4, #3
	mov	r1, r8
	adds	r6, r6, r4
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_0816e3a6
.L_0816e40a:
	ldr	r2, [sp, #52]
	cmp	r2, #63
	ble.n	.L_0816e412
	b.n	.L_0816e598
.L_0816e412:
	ldr	r3, [sp, #52]
	cmp	r3, #25
	bgt.n	.L_0816e41a
	b.n	.L_0816e598
.L_0816e41a:
	ldr	r3, [sp, #68]
	ldr	r2, [pc, #492]
	ldr	r4, [sp, #56]
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #488]
	movs	r5, #224
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	lsls	r5, r5, #3
	str	r3, [sp, #68]
	add	r6, sp, #68
	adds	r3, r4, r5
	mov	r0, r9
	str	r3, [r6, #4]
	ldr	r1, [pc, #468]
	movs	r3, #6
	str	r3, [r0, #0]
	movs	r3, #3
	str	r3, [r0, #4]
	mov	r2, fp
	movs	r3, #0
	str	r2, [r0, #12]
	strb	r3, [r0, #24]
	str	r6, [r0, #16]
	str	r1, [r0, #8]
	ldr	r4, [sp, #52]
	movs	r2, #31
	asrs	r7, r4, #31
	lsrs	r3, r7, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	ands	r3, r2
	negs	r3, r3
	strb	r3, [r0, #25]
	bl	sub_08014e38
	ldr	r5, [sp, #52]
	movs	r2, #0
	cmp	r5, #31
	bgt.n	.L_0816e480
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r3, #16
	muls	r3, r5
	ldr	r0, [pc, #412]
	adds	r5, r3, r0
	b.n	.L_0816e486
.L_0816e480:
	movs	r5, #234
	lsls	r5, r5, #8
	adds	r5, #96
.L_0816e486:
	mov	r1, r9
	str	r2, [r1, #20]
	ldr	r2, [sp, #36]
	ldr	r1, [pc, #396]
	ldr	r0, [r2, #8]
	ldr	r2, [r2, #16]
	bl	sub_08015160
	adds	r0, r5, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	mov	r1, fp
	movs	r2, #64
	ldr	r0, [pc, #376]
	bl	sub_081969ac
	mov	r0, r9
	bl	sub_08196a7c
	movs	r2, #32
	mov	r3, r9
	movs	r4, #7
	negs	r2, r2
	movs	r5, #0
	str	r4, [r3, #0]
	strb	r5, [r3, #24]
	str	r2, [r3, #20]
	ldr	r0, [sp, #52]
	lsrs	r3, r7, #31
	adds	r3, r0, r3
	movs	r2, #31
	asrs	r3, r3, #1
	ands	r3, r2
	mov	r1, r9
	strb	r3, [r1, #25]
	movs	r3, #2
	str	r3, [r1, #4]
	mov	r0, r9
	bl	sub_08196a7c
	bl	sub_08014ea8
	ldr	r4, [pc, #324]
	movs	r2, #7
	strb	r2, [r6, #0]
	add	r3, sp, #68
	mov	r6, r9
	strb	r2, [r3, #1]
	str	r4, [r3, #4]
	str	r3, [r6, #16]
	ldr	r3, [pc, #312]
	movs	r0, #0
	movs	r5, #7
	mov	r1, fp
	movs	r2, #0
	str	r3, [r6, #8]
	str	r5, [r6, #0]
	str	r0, [r6, #4]
	str	r1, [r6, #12]
	strb	r0, [r6, #24]
	strb	r2, [r6, #25]
	ldr	r7, [sp, #52]
	movs	r3, #0
	mov	r8, r3
.L_0816e50a:
	ldr	r3, [pc, #288]
	mov	r4, r8
	ldrb	r3, [r3, r4]
	ldr	r5, [sp, #52]
	adds	r1, r3, #0
	adds	r1, #22
	cmp	r5, r1
	ble.n	.L_0816e58c
	ldr	r3, [pc, #276]
	subs	r2, r5, r1
	ldrb	r3, [r3, r4]
	movs	r0, #131
	muls	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	muls	r3, r2
	lsls	r0, r0, #7
	adds	r6, r3, r0
	subs	r3, r1, r5
	lsls	r3, r3, #3
	adds	r5, r3, #0
	adds	r5, #56
	cmp	r5, #0
	ble.n	.L_0816e53e
	movs	r5, #0
.L_0816e53e:
	movs	r1, #64
	negs	r1, r1
	cmp	r5, r1
	ble.n	.L_0816e58c
	bl	sub_08014e38
	mov	r2, r9
	str	r5, [r2, #20]
	ldr	r3, [sp, #36]
	mov	r4, r8
	ldr	r0, [r3, #8]
	ldr	r3, [pc, #220]
	ldr	r5, [sp, #36]
	ldrsb	r1, [r3, r4]
	ldr	r2, [r5, #16]
	lsls	r1, r1, #16
	bl	sub_08015160
	adds	r0, r6, #0
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	movs	r6, #7
	adds	r3, r7, #0
	ands	r3, r6
	lsls	r3, r3, #4
	mov	r0, r9
	strb	r3, [r0, #24]
	mov	r1, fp
	ldr	r0, [pc, #188]
	movs	r2, #32
	bl	sub_081969ac
	mov	r0, r9
	bl	sub_08196a7c
	bl	sub_08014ea8
.L_0816e58c:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r7, #5
	cmp	r2, #4
	bne.n	.L_0816e50a
.L_0816e598:
	mov	r0, r9
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
	ldr	r3, [pc, #148]
	ldr	r4, [sp, #32]
	movs	r1, #8
	str	r4, [r3, #12]
	ldr	r5, [sp, #28]
	movs	r0, #8
	str	r5, [r3, #16]
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r0, #240
	ldr	r6, [sp, #56]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r6, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #52]
	adds	r1, #1
	str	r1, [sp, #52]
	cmp	r1, #88
	beq.n	.L_0816e5dc
	bl	.L_0816dda0
.L_0816e5dc:
	ldr	r0, [pc, #96]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #160
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff8000
	.4byte 0x0819919c
	.4byte 0x081991a4
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02012000
	.4byte 0xfffc0860
	.4byte 0xfffc0000
	.4byte 0x02010000
	.4byte 0x02014000
	.4byte 0x08198ec4
	.4byte 0x08198b8a
	.4byte 0x08198b8e
	.4byte 0x08198b92
	.4byte 0x08198cac
	.4byte 0x030011e0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_0816e65c
	pop	{pc}
