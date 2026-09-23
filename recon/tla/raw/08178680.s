.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_080156e8, 0x080156e8
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b410, 0x0815b410
	.set sub_0815e1fc, 0x0815e1fc
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0816ae40, 0x0816ae40
	.set sub_081791d4, 0x081791d4
	.set sub_08179292, 0x08179292
	.set sub_081796bc, 0x081796bc
	.set sub_0817970e, 0x0817970e
	.set sub_08179a10, 0x08179a10
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08178680
	.thumb_func
Func_08178680:
.L_08178680:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r0
	mov	ip, r1
	mov	lr, r3
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_081786f2
	subs	r5, r2, #1
	movs	r2, #3
	movs	r3, #1
	mov	r8, r2
	mov	r2, ip
	mov	sl, r3
	movs	r6, #2
	movs	r0, #1
	subs	r2, #1
	movs	r1, #0
	mov	r4, r9
.L_081786b0:
	lsls	r3, r7, #1
	mov	fp, r3
	mov	r3, sl
	strb	r3, [r4, #5]
	strb	r3, [r4, #9]
	strb	r3, [r4, #21]
	mov	r3, fp
	strb	r3, [r4, #0]
	mov	r3, r8
	strb	r3, [r4, #13]
	adds	r7, #1
	movs	r3, #2
	strb	r1, [r4, #4]
	strb	r1, [r4, #6]
	strb	r2, [r4, #8]
	strb	r1, [r4, #16]
	strb	r2, [r4, #18]
	strb	r2, [r4, #20]
	strb	r0, [r4, #1]
	strb	r6, [r4, #2]
	strb	r0, [r4, #12]
	strb	r6, [r4, #14]
	strb	r5, [r4, #7]
	strb	r5, [r4, #17]
	strb	r5, [r4, #19]
	add	r8, r3
	adds	r6, #2
	adds	r0, #2
	add	r2, ip
	add	r1, ip
	adds	r4, #24
	cmp	r7, lr
	bne.n	.L_081786b0
.L_081786f2:
	mov	r2, lr
	lsls	r3, r2, #1
	add	r3, lr
	lsls	r3, r3, #3
	add	r3, r9
	movs	r2, #0
	strb	r2, [r3, #0]
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
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
	sub	sp, #164
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #80]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #76]
	ldr	r2, [r3, #48]
	str	r2, [sp, #60]
	ldr	r3, [r3, #100]
	str	r3, [sp, #56]
	bl	sub_081435e0
	ldr	r4, [sp, #84]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0817874e
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_08178756
.L_0817874e:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_08178756:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #68]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r7, [sp, #80]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	movs	r3, #0
	movs	r4, #0
	str	r3, [sp, #104]
	str	r4, [sp, #108]
	str	r3, [sp, #96]
	str	r4, [sp, #100]
	ldr	r2, [sp, #84]
	ldr	r0, [r2, #8]
	b.n	.L_081787a4
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_081787a4:
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r1, #48
	str	r0, [sp, #52]
	bl	sub_08020098
	movs	r1, #2
	ldr	r0, [sp, #52]
	bl	sub_08020090
	ldr	r3, [sp, #84]
	mov	r4, sp
	adds	r4, #136
	ldr	r0, [r3, #8]
	adds	r1, r4, #0
	str	r4, [sp, #48]
	bl	sub_0815e21c
	ldr	r1, [sp, #84]
	mov	r2, sp
	adds	r2, #124
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	adds	r1, r2, #0
	str	r2, [sp, #44]
	bl	sub_0815e21c
	ldr	r3, [sp, #80]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #116]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #112]
	ldr	r1, [sp, #56]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r7, [sp, #80]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r7, r2
	movs	r3, #1
	movs	r2, #1
	ldr	r0, [pc, #92]
	bl	sub_08157cf4
	ldr	r0, [pc, #88]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #84]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0d
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	movs	r2, #32
	movs	r3, #32
	movs	r1, #4
	bl	.L_08178680
	movs	r3, #0
	str	r3, [sp, #64]
	ldr	r4, [sp, #84]
	movs	r7, #60
	ldr	r2, [r4, #24]
	negs	r7, r7
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, r7
	bne.n	.L_0817884e
	b.n	.L_08178ea4
.L_0817884e:
	ldr	r0, [sp, #60]
	adds	r0, #12
	str	r0, [sp, #32]
	b.n	.L_08178874
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x000000f4
	.4byte 0x00000134
	.4byte 0x000000e8
	.4byte 0x00000130
	.4byte 0x03000730
	.2byte 0x2000
	.2byte 0x0201
.L_08178874:
	bl	sub_08014de4
	ldr	r1, [sp, #32]
	ldr	r0, [sp, #60]
	bl	sub_080156e8
	ldr	r1, [sp, #64]
	cmp	r1, #0
	bne.n	.L_081788a8
	add	r3, sp, #148
	str	r1, [r3, #0]
	str	r1, [r3, #4]
	str	r1, [r3, #8]
	str	r1, [r3, #12]
	ldr	r3, [sp, #80]
	movs	r2, #0
	mov	sl, r2
	adds	r3, #24
	subs	r2, #1
.L_0817889a:
	movs	r4, #1
	add	sl, r4
	mov	r7, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_0817889a
.L_081788a8:
	ldr	r2, [sp, #84]
	movs	r3, #1
	ldr	r1, [r2, #24]
	movs	r0, #0
	negs	r3, r3
	mov	sl, r0
	cmp	r1, r3
	bne.n	.L_081788ba
	b.n	.L_081789ec
.L_081788ba:
	movs	r4, #28
	str	r4, [sp, #20]
	mov	fp, r0
.L_081788c0:
	ldr	r7, [sp, #64]
	ldr	r0, [sp, #20]
	cmp	r7, r0
	beq.n	.L_081788ca
	b.n	.L_081789d4
.L_081788ca:
	ldr	r2, [sp, #84]
	add	r5, sp, #112
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r7, [sp, #80]
	movs	r3, #0
	mov	r9, r3
	mov	r8, r5
	add	r7, fp
.L_081788e2:
	mov	r4, r8
	ldr	r3, [r4, #0]
	movs	r5, #255
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r4, #4]
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r9, r0
	adds	r3, #16
	mov	r1, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #32
	bne.n	.L_081788e2
	movs	r1, #128
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #80]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24ee
	ldr	r3, [sp, #80]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r7, [sp, #64]
	cmp	r7, #28
	bne.n	.L_08178998
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0817896e
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_08178974
.L_0817896e:
	movs	r0, #144
	bl	sub_081c0010
.L_08178974:
	ldr	r2, [sp, #84]
	movs	r3, #128
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r2, #150
	str	r2, [sp, #4]
	movs	r2, #128
	lsls	r3, r3, #11
	movs	r1, #1
	lsls	r2, r2, #10
	str	r3, [sp, #0]
	bl	sub_0815f000
	b.n	.L_081789bc
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_08178998:
	movs	r0, #144
	bl	sub_081180e8
	ldr	r4, [sp, #84]
	movs	r2, #128
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #224
	lsls	r3, r3, #11
	str	r3, [sp, #0]
	movs	r3, #200
	str	r3, [sp, #4]
	movs	r3, #128
	movs	r1, #1
	lsls	r2, r2, #10
	lsls	r3, r3, #11
	bl	sub_0815f000
.L_081789bc:
	ldr	r1, [sp, #84]
	movs	r3, #8
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r2, [sp, #84]
	ldr	r1, [r2, #24]
.L_081789d4:
	ldr	r3, [sp, #20]
	movs	r4, #224
	adds	r3, #10
	movs	r7, #1
	str	r3, [sp, #20]
	lsls	r4, r4, #2
	add	sl, r7
	adds	r3, r1, #1
	add	fp, r4
	cmp	sl, r3
	beq.n	.L_081789ec
	b.n	.L_081788c0
.L_081789ec:
	ldr	r6, [pc, #852]
	ldr	r5, [sp, #80]
	movs	r0, #0
	mov	sl, r0
.L_081789f4:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08178a38
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #56]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08178a38:
	movs	r7, #1
	add	sl, r7
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_081789f4
	ldr	r1, [sp, #64]
	cmp	r1, #15
	bgt.n	.L_08178a4c
	b.n	.L_08178e6c
.L_08178a4c:
	movs	r3, #44
	adds	r4, r1, #0
	adds	r1, r4, #0
	muls	r1, r3
	ldr	r7, [pc, #752]
	add	r2, sp, #148
	adds	r3, r1, r7
	mov	fp, r2
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #182
	cmp	r3, r2
	ble.n	.L_08178a6c
	mov	r0, fp
	str	r2, [r0, #0]
.L_08178a6c:
	ldr	r3, [sp, #64]
	cmp	r3, #31
	ble.n	.L_08178a80
	ldr	r4, [pc, #728]
	mov	r7, fp
	adds	r3, r1, r4
	str	r3, [r7, #4]
	cmp	r3, r2
	ble.n	.L_08178a80
	str	r2, [r7, #4]
.L_08178a80:
	ldr	r0, [sp, #64]
	cmp	r0, #29
	ble.n	.L_08178aa0
	movs	r3, #44
	muls	r3, r0
	ldr	r1, [pc, #708]
	mov	r2, fp
	adds	r3, r3, r1
	str	r3, [r2, #8]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #182
	cmp	r3, r2
	ble.n	.L_08178aa0
	mov	r3, fp
	str	r2, [r3, #8]
.L_08178aa0:
	ldr	r4, [sp, #64]
	cmp	r4, #45
	ble.n	.L_08178abe
	movs	r3, #44
	muls	r3, r4
	ldr	r7, [pc, #680]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r3, r3, r7
	mov	r0, fp
	adds	r2, #182
	str	r3, [r0, #12]
	cmp	r3, r2
	ble.n	.L_08178abe
	str	r2, [r0, #12]
.L_08178abe:
	movs	r1, #0
	str	r1, [sp, #24]
	mov	r9, r1
.L_08178ac4:
	ldr	r2, [sp, #48]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_08178ace
	adds	r3, #3
.L_08178ace:
	ldr	r4, [sp, #44]
	asrs	r0, r3, #2
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_08178ada
	adds	r3, #3
.L_08178ada:
	asrs	r3, r3, #2
	mov	r7, fp
	ldr	r2, [r7, #4]
	subs	r4, r0, r3
	ldr	r3, [r7, #0]
	subs	r3, r3, r2
	mov	r1, r9
	muls	r1, r3
	lsls	r3, r2, #5
	adds	r3, r3, r2
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r4, #0
	bge.n	.L_08178af8
	negs	r4, r4
.L_08178af8:
	adds	r0, r4, #0
	movs	r1, #3
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r1, [sp, #24]
	str	r0, [sp, #40]
	adds	r6, r1, #0
	adds	r6, #16
	ldr	r4, [sp, #8]
	cmp	r6, #32
	ble.n	.L_08178b12
	movs	r6, #32
.L_08178b12:
	movs	r0, #162
	lsls	r0, r0, #7
	adds	r0, #32
	adds	r3, r0, #0
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_08178b28
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_08178b28:
	asrs	r6, r3, #16
	lsrs	r3, r3, #31
	adds	r3, r6, r3
	ldr	r0, [sp, #40]
	asrs	r3, r3, #1
	negs	r3, r3
	movs	r7, #0
	mov	sl, r7
	adds	r2, r0, r3
	adds	r7, r4, r3
	lsls	r3, r1, #2
	ldr	r1, [pc, #536]
	adds	r5, r3, r1
.L_08178b42:
	mov	r0, r8
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	asrs	r3, r3, #16
	subs	r3, r3, r4
	strb	r3, [r5, #0]
	mov	r0, r8
	bl	sub_08002096
	ldr	r2, [sp, #12]
	adds	r7, r7, r6
	adds	r3, r2, #0
	muls	r3, r0
	movs	r0, #1
	asrs	r3, r3, #16
	negs	r3, r3
	add	sl, r0
	strb	r3, [r5, #1]
	mov	r1, sl
	movs	r3, #0
	strb	r3, [r5, #2]
	adds	r2, r2, r6
	adds	r5, #4
	ldr	r4, [sp, #8]
	cmp	r1, #2
	bne.n	.L_08178b42
	mov	r2, r9
	cmp	r2, #32
	bne.n	.L_08178baa
	mov	r0, r8
	bl	sub_08002090
	ldr	r4, [sp, #8]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r4
	mov	r0, r8
	str	r3, [sp, #104]
	bl	sub_08002096
	ldr	r4, [sp, #40]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	negs	r3, r3
	str	r3, [sp, #96]
.L_08178baa:
	ldr	r7, [sp, #24]
	movs	r0, #1
	add	r9, r0
	adds	r7, #2
	mov	r1, r9
	str	r7, [sp, #24]
	cmp	r1, #33
	bne.n	.L_08178ac4
	movs	r2, #0
	str	r2, [sp, #16]
	mov	r9, r2
.L_08178bc0:
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_08178bca
	adds	r3, #3
.L_08178bca:
	ldr	r7, [sp, #44]
	asrs	r0, r3, #2
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_08178bd6
	adds	r3, #3
.L_08178bd6:
	asrs	r3, r3, #2
	subs	r4, r0, r3
	mov	r0, fp
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #8]
	subs	r3, r3, r2
	mov	r1, r9
	muls	r1, r3
	lsls	r3, r2, #5
	adds	r3, r3, r2
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r4, #0
	bge.n	.L_08178bf4
	negs	r4, r4
.L_08178bf4:
	lsls	r0, r4, #5
	movs	r1, #36
	str	r4, [sp, #8]
	bl	sub_08002054
	mov	r1, r9
	lsls	r6, r1, #2
	str	r0, [sp, #36]
	ldr	r4, [sp, #8]
	cmp	r6, #24
	ble.n	.L_08178c0c
	movs	r6, #24
.L_08178c0c:
	movs	r2, #162
	lsls	r2, r2, #7
	adds	r2, #32
	adds	r3, r2, #0
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_08178c22
	movs	r7, #255
	lsls	r7, r7, #8
	adds	r7, #255
	adds	r3, r3, r7
.L_08178c22:
	asrs	r6, r3, #16
	lsrs	r3, r3, #31
	adds	r3, r6, r3
	ldr	r1, [sp, #36]
	asrs	r3, r3, #1
	negs	r3, r3
	movs	r0, #0
	mov	sl, r0
	adds	r2, r1, r3
	adds	r7, r4, r3
	ldr	r0, [pc, #292]
	ldr	r3, [sp, #16]
	adds	r5, r3, r0
.L_08178c3c:
	mov	r0, r8
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	asrs	r3, r3, #16
	subs	r3, r3, r4
	strb	r3, [r5, #0]
	mov	r0, r8
	bl	sub_08002096
	ldr	r2, [sp, #12]
	movs	r1, #0
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r5, #1]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	strb	r1, [r5, #2]
	adds	r2, r2, r6
	adds	r7, r7, r6
	adds	r5, #4
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_08178c3c
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_08178ca0
	mov	r0, r8
	bl	sub_08002090
	ldr	r4, [sp, #8]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r4
	mov	r0, r8
	str	r3, [sp, #108]
	bl	sub_08002096
	ldr	r2, [sp, #36]
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [sp, #100]
.L_08178ca0:
	ldr	r3, [sp, #16]
	movs	r4, #1
	add	r9, r4
	adds	r3, #8
	mov	r7, r9
	str	r3, [sp, #16]
	cmp	r7, #33
	bne.n	.L_08178bc0
	movs	r0, #1
	bl	sub_081969f8
	adds	r6, r0, #0
	ldr	r0, [pc, #164]
	movs	r2, #0
	movs	r3, #6
	add	r7, sp, #88
	str	r3, [r6, #0]
	strb	r2, [r6, #25]
	str	r7, [r6, #16]
	str	r2, [r6, #20]
	str	r0, [r6, #12]
	mov	sl, r2
	ldr	r2, [sp, #84]
	subs	r3, #7
	ldr	r1, [r2, #24]
	cmp	r1, r3
	bne.n	.L_08178cd8
	b.n	.L_08178e66
.L_08178cd8:
	ldr	r0, [pc, #124]
	ldr	r4, [sp, #48]
	str	r0, [sp, #28]
	movs	r2, #0
	mov	r8, r4
	mov	r9, r2
.L_08178ce4:
	mov	r3, r9
	adds	r3, #4
	mov	r4, fp
	mov	r0, r9
	ldr	r2, [r4, r3]
	ldr	r3, [r4, r0]
	cmp	r2, r3
	blt.n	.L_08178cf6
	b.n	.L_08178e4c
.L_08178cf6:
	movs	r1, #6
	strb	r1, [r7, #0]
	movs	r3, #5
	add	r7, sp, #88
	strb	r3, [r7, #1]
	ldr	r2, [sp, #80]
	ldr	r0, [pc, #96]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	str	r3, [r7, #4]
	str	r0, [r6, #8]
	ldr	r1, [sp, #64]
	movs	r3, #127
	lsls	r2, r1, #3
	bics	r3, r2
	strb	r3, [r6, #24]
	bl	sub_08014de4
	ldr	r2, [sp, #84]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08178d6c
	mov	r3, r8
	ldr	r0, [r3, #0]
	mov	r4, r8
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	ldr	r2, [pc, #56]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #68
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08178d8a
	.4byte 0x08197410
	.4byte 0xfffffd40
	.4byte 0xfffffa80
	.4byte 0xfffffad8
	.4byte 0xfffff818
	.4byte 0x02010000
	.4byte 0x02010108
	.4byte 0x02011000
	.4byte 0x02012000
	.2byte 0x0000
	.2byte 0xffa0
.L_08178d6c:
	.2byte 0x4643
	ldr	r0, [r3, #0]
	mov	r4, r8
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	ldr	r2, [pc, #340]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #52
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
.L_08178d8a:
	movs	r5, #128
	lsls	r5, r5, #8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r4, [sp, #84]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08178da8
	adds	r0, r5, #0
	bl	sub_08015068
.L_08178da8:
	movs	r3, #220
	lsls	r3, r3, #6
	adds	r3, #112
	mov	r0, sl
	muls	r0, r3
	ldr	r1, [pc, #284]
	adds	r0, r0, r1
	bl	sub_080150e4
	movs	r0, #128
	lsls	r0, r0, #11
	bl	sub_0801521c
	movs	r2, #66
	ldr	r0, [sp, #28]
	ldr	r1, [pc, #268]
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	mov	r2, sl
	ldr	r4, [sp, #64]
	lsls	r3, r2, #4
	adds	r3, #40
	cmp	r4, r3
	bge.n	.L_08178e48
	add	r0, sp, #88
	adds	r3, r0, #0
	movs	r7, #6
	strb	r7, [r0, #0]
	adds	r7, r3, #0
	ldr	r3, [pc, #236]
	movs	r2, #0
	movs	r1, #6
	strb	r2, [r6, #24]
	strb	r1, [r7, #1]
	str	r3, [r6, #8]
	ldr	r2, [sp, #80]
	movs	r4, #142
	lsls	r4, r4, #7
	adds	r3, r2, r4
	mov	r0, sl
	lsls	r2, r0, #2
	str	r3, [r7, #4]
	add	r3, sp, #104
	ldr	r0, [r2, r3]
	add	r3, sp, #96
	ldr	r1, [r2, r3]
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	mov	r1, r9
	mov	r2, fp
	ldr	r3, [r1, r2]
	movs	r0, #192
	lsls	r0, r0, #6
	lsls	r3, r3, #5
	adds	r0, #96
	subs	r0, r0, r3
	bl	sub_080150e4
	movs	r0, #162
	lsls	r0, r0, #7
	adds	r0, #32
	bl	sub_0801521c
	ldr	r1, [pc, #160]
	ldr	r0, [pc, #164]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	ldr	r3, [sp, #84]
	ldr	r1, [r3, #24]
	b.n	.L_08178e4c
.L_08178e48:
	ldr	r4, [sp, #84]
	ldr	r1, [r4, #24]
.L_08178e4c:
	ldr	r0, [sp, #28]
	movs	r2, #132
	movs	r3, #8
	lsls	r2, r2, #1
	movs	r4, #1
	adds	r0, r0, r2
	add	r9, r3
	add	sl, r4
	adds	r3, r1, #1
	str	r0, [sp, #28]
	cmp	sl, r3
	beq.n	.L_08178e66
	b.n	.L_08178ce4
.L_08178e66:
	adds	r0, r6, #0
	bl	sub_08013164
.L_08178e6c:
	bl	sub_081434f8
	movs	r1, #2
	movs	r0, #2
	bl	sub_08158ce0
	movs	r0, #240
	ldr	r7, [sp, #80]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r7, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #64]
	ldr	r3, [sp, #84]
	adds	r1, #1
	str	r1, [sp, #64]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #60
	cmp	r1, r3
	beq.n	.L_08178ea4
	b.n	.L_08178874
.L_08178ea4:
	ldr	r0, [sp, #52]
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #44]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #164
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffa00000
	.4byte 0xfffff000
	.4byte 0x02011000
	.4byte 0x08199340
	.4byte 0x081991e0
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
	ldr	r0, [r5, #92]
	ldr	r1, [r5, #96]
	mov	r8, r0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	mov	fp, r1
	ldr	r6, [pc, #68]
	bl	sub_08143a88
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #56]
	movs	r2, #1
	movs	r3, #1
	adds	r1, r6, #0
	bl	sub_08157cf4
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
	add	r2, sp, #16
	str	r3, [r2, #4]
	mov	sl, r2
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r8
	b.n	.L_08178f58
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x02010000
	.2byte 0x00dc
	.2byte 0x0000
.L_08178f58:
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #0
	add	r2, r8
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #84]
	bl	sub_080145a8
	ldr	r1, [sp, #12]
	mov	r2, sp
	adds	r2, #24
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	adds	r1, r2, #0
	str	r2, [sp, #8]
	bl	sub_0815e20c
	movs	r3, #0
	mov	r9, r3
.L_08178f88:
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_08178fea
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08178fa4
	ldr	r2, [sp, #8]
	mov	r0, r8
	ldr	r3, [r2, #0]
	adds	r3, #40
	lsls	r3, r3, #16
	str	r3, [r0, #0]
	b.n	.L_08178fb0
.L_08178fa4:
	ldr	r1, [sp, #8]
	mov	r2, r8
	ldr	r3, [r1, #0]
	subs	r3, #168
	lsls	r3, r3, #16
	str	r3, [r2, #0]
.L_08178fb0:
	ldr	r3, [pc, #20]
	mov	r0, r8
	str	r3, [r0, #4]
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08178fd0
	ldr	r3, [pc, #12]
	str	r3, [r0, #12]
	b.n	.L_08178fd8
	.4byte 0x08143001
	.4byte 0xff9c0000
	.2byte 0x0000
	.2byte 0xfffa
.L_08178fd0:
	.2byte 0x23c0
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #12]
.L_08178fd8:
	movs	r3, #128
	lsls	r3, r3, #12
	mov	r0, r8
	str	r3, [r0, #16]
	movs	r1, #128
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	adds	r1, #82
	strh	r3, [r1, #0]
.L_08178fea:
	mov	r2, r9
	cmp	r2, #31
	bgt.n	.L_08178ffa
	mov	r0, r8
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
.L_08178ffa:
	mov	r3, r9
	cmp	r3, #32
	bgt.n	.L_08179028
	lsrs	r3, r3, #31
	add	r3, r9
	asrs	r1, r3, #1
	ldr	r2, [pc, #20]
	lsrs	r3, r3, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r0, #128
	lsls	r2, r2, #8
	b.n	.L_08179020
	movs	r0, r0
	.4byte 0x00001000
	.2byte 0x0010
	.2byte 0x0000
.L_08179020:
	lsls	r0, r0, #19
	orrs	r2, r1
	adds	r0, #82
	strh	r2, [r0, #0]
.L_08179028:
	mov	r1, r9
	cmp	r1, #91
	ble.n	.L_08179060
	mov	r2, r9
	movs	r1, #108
	subs	r1, r1, r2
	lsrs	r2, r1, #31
	ldr	r3, [pc, #32]
	adds	r2, r1, r2
	asrs	r2, r2, #1
	subs	r3, r3, r2
	movs	r0, #128
	lsls	r3, r3, #8
	lsls	r0, r0, #19
	orrs	r3, r1
	adds	r0, #82
	strh	r3, [r0, #0]
	movs	r1, #60
	mov	r0, r8
	ldr	r2, [pc, #12]
	bl	sub_08138086
	b.n	.L_08179060
	movs	r0, r0
	.4byte 0x00000010
	.2byte 0x8000
	.2byte 0xffff
.L_08179060:
	.2byte 0x4649
	lsls	r6, r1, #8
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, r8
	movs	r2, #2
	ldrsh	r5, [r3, r2]
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	adds	r5, r5, r0
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r2, r8
	movs	r1, #6
	ldrsh	r3, [r2, r1]
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	adds	r7, r3, r0
	movs	r3, #128
	lsls	r3, r3, #19
	negs	r5, r5
	adds	r3, #40
	lsls	r5, r5, #8
	str	r5, [r3, #0]
	ldr	r3, [sp, #12]
	ldr	r1, [r3, #4]
	cmp	r1, #0
	bne.n	.L_081790fe
	movs	r2, #96
	str	r2, [sp, #0]
	movs	r2, #83
	str	r2, [sp, #4]
	mov	r0, sl
	ldr	r4, [r0, #0]
	ldr	r1, [pc, #688]
	adds	r3, r7, #2
	mov	r0, fp
	movs	r2, #25
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4649
	cmp	r1, #47
	bhi.n	.L_081790dc
	ldr	r2, [sp, #12]
	mov	r0, sl
	ldr	r1, [r2, #4]
	movs	r2, #48
	str	r2, [sp, #0]
	movs	r2, #51
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r7, #0
	ldr	r4, [r1, r0]
	adds	r3, #22
	mov	r0, fp
	ldr	r1, [pc, #652]
	movs	r2, #31
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe041
.L_081790dc:
	ldr	r2, [sp, #12]
	mov	r0, sl
	ldr	r1, [r2, #4]
	movs	r2, #42
	str	r2, [sp, #0]
	movs	r2, #35
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r7, #0
	ldr	r4, [r1, r0]
	adds	r3, #16
	mov	r0, fp
	ldr	r1, [pc, #620]
	movs	r2, #64
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe030
.L_081790fe:
	movs	r2, #96
	str	r2, [sp, #0]
	movs	r2, #83
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r2, sl
	ldr	r4, [r1, r2]
	adds	r3, r7, #2
	mov	r0, fp
	ldr	r1, [pc, #584]
	movs	r2, #7
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #47
	bhi.n	.L_08179140
	ldr	r0, [sp, #12]
	movs	r2, #48
	ldr	r1, [r0, #4]
	str	r2, [sp, #0]
	movs	r2, #51
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r2, sl
	adds	r3, r7, #0
	ldr	r4, [r1, r2]
	adds	r3, #22
	mov	r0, fp
	ldr	r1, [pc, #552]
	movs	r2, #49
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00f
.L_08179140:
	ldr	r3, [sp, #12]
	movs	r2, #42
	ldr	r1, [r3, #4]
	str	r2, [sp, #0]
	movs	r2, #35
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r0, sl
	adds	r3, r7, #0
	ldr	r4, [r1, r0]
	adds	r3, #16
	mov	r0, fp
	ldr	r1, [pc, #520]
	movs	r2, #22
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4648
	subs	r0, #28
	cmp	r0, #11
	bhi.n	sub_081791d4
	movs	r1, #3
	bl	sub_08002054
	ldr	r1, [sp, #12]
	adds	r6, r0, #0
	ldr	r5, [r1, #4]
	cmp	r5, #0
	bne.n	.L_081791a6
	ldr	r2, [pc, #492]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #492]
	ldr	r2, [pc, #492]
	ldrb	r4, [r3, r6]
	adds	r1, r1, r2
	lsrs	r3, r4, #1
	movs	r2, #46
	subs	r2, r2, r3
	ldr	r3, [pc, #484]
	ldrb	r0, [r3, r6]
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	lsrs	r3, r0, #1
	subs	r3, r7, r3
	mov	r0, sl
	ldr	r4, [r0, #0]
	adds	r3, #61
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe016
.L_081791a6:
	ldr	r2, [pc, #448]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #444]
	ldr	r2, [pc, #448]
	ldrb	r0, [r3, r6]
	ldr	r3, [pc, #448]
	adds	r1, r1, r2
	ldrb	r4, [r3, r6]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	lsrs	r2, r0, #1
	lsrs	r3, r4, #1
	subs	r2, r2, r0
	lsls	r5, r5, #2
	mov	r0, sl
	subs	r3, r7, r3
	ldr	r4, [r5, r0]
	adds	r2, #82
	adds	r3, #61
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4649
	cmp	r1, #39
	ble.n	sub_08179292
	cmp	r1, #47
	bgt.n	.L_08179220
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #4]
	cmp	r1, #0
	bne.n	.L_08179202
	movs	r2, #32
	str	r2, [sp, #0]
	movs	r2, #29
	str	r2, [sp, #4]
	mov	r0, sl
	adds	r3, r7, #0
	ldr	r4, [r0, #0]
	adds	r3, #46
	mov	r0, fp
	ldr	r1, [pc, #380]
	movs	r2, #30
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe047
.L_08179202:
	movs	r2, #32
	str	r2, [sp, #0]
	movs	r2, #29
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r2, sl
	adds	r3, r7, #0
	ldr	r4, [r1, r2]
	adds	r3, #46
	mov	r0, fp
	ldr	r1, [pc, #352]
	movs	r2, #66
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe038
.L_08179220:
	mov	r0, r9
	subs	r0, #56
	movs	r1, #6
	bl	sub_08002054
	adds	r4, r0, #0
	cmp	r4, #0
	bge.n	.L_08179232
	movs	r4, #0
.L_08179232:
	cmp	r4, #7
	bgt.n	sub_08179292
	ldr	r3, [sp, #12]
	ldr	r0, [r3, #4]
	cmp	r0, #0
	bne.n	.L_08179268
	ldr	r2, [pc, #316]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #312]
	movs	r2, #32
	ldrb	r3, [r3, r4]
	str	r2, [sp, #0]
	ldr	r2, [pc, #308]
	ldr	r0, [pc, #296]
	ldrb	r2, [r2, r4]
	adds	r3, r7, r3
	str	r2, [sp, #4]
	mov	r2, sl
	adds	r1, r1, r0
	ldr	r4, [r2, #0]
	subs	r3, #8
	mov	r0, fp
	movs	r2, #63
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe014
.L_08179268:
	ldr	r2, [pc, #272]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #264]
	movs	r2, #32
	adds	r1, r1, r3
	ldr	r3, [pc, #264]
	lsls	r0, r0, #2
	ldrb	r3, [r3, r4]
	str	r2, [sp, #0]
	ldr	r2, [pc, #260]
	adds	r3, r7, r3
	ldrb	r2, [r2, r4]
	subs	r3, #8
	str	r2, [sp, #4]
	mov	r2, sl
	ldr	r4, [r0, r2]
	mov	r0, fp
	movs	r2, #33
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #80
	bne.n	.L_081792be
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r8
	movs	r3, #6
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
	ldr	r2, [sp, #12]
	movs	r3, #16
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_081792be:
	mov	r3, r9
	cmp	r3, #30
	bne.n	.L_081792f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #188]
	ldr	r2, [pc, #192]
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r8
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r0, #212
	bl	sub_081c0010
	ldr	r2, [sp, #12]
	movs	r3, #16
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_081792f8:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #108
	beq.n	.L_08179322
	b.n	.L_08178f88
.L_08179322:
	ldr	r0, [pc, #108]
	bl	sub_08014644
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r3, r1
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	sub_08118040
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
	.4byte 0x02010000
	.4byte 0x020124de
	.4byte 0x02011f20
	.4byte 0x081993d8
	.4byte 0x081993d0
	.4byte 0x02012e6e
	.4byte 0x081993d4
	.4byte 0x02014e9d
	.4byte 0x081993f0
	.4byte 0x081993e8
	.4byte 0x081993e0
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
	movs	r5, #192
	lsls	r5, r5, #18
	mov	fp, r0
	ldr	r0, [r5, #92]
	sub	sp, #172
	str	r0, [sp, #64]
	movs	r0, #1
	ldr	r1, [r5, #96]
	mov	r6, fp
	str	r1, [sp, #60]
	ldr	r2, [r5, #100]
	str	r2, [sp, #44]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #7
	str	r3, [sp, #48]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r2, [r6, #20]
	movs	r4, #0
	str	r5, [sp, #52]
	mov	r8, r4
	cmp	r2, #0
	beq.n	.L_08179414
	add	r7, sp, #76
	adds	r6, r7, #0
	movs	r5, #36
	b.n	.L_081793f8
	.2byte 0x0a10
	.2byte 0x0000
.L_081793f8:
	mov	r1, fp
	ldrsh	r0, [r5, r1]
	adds	r1, r6, #0
	bl	sub_0815e20c
	mov	r4, fp
	ldr	r2, [r4, #20]
	movs	r3, #1
	add	r8, r3
	adds	r6, #12
	adds	r5, #2
	cmp	r8, r2
	bne.n	.L_081793f8
	b.n	.L_08179416
.L_08179414:
	add	r7, sp, #76
.L_08179416:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	subs	r3, #12
	ldr	r1, [r7, #0]
	ldr	r3, [r7, r3]
	ldr	r0, [pc, #928]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [sp, #40]
	ldr	r3, [r7, #4]
	ldr	r1, [pc, #916]
	subs	r3, #48
	str	r3, [sp, #36]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r6, [sp, #64]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r6, r7
	ldr	r0, [pc, #892]
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	ldr	r0, [pc, #888]
	ldr	r1, [sp, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r6, r2
	ldr	r0, [pc, #872]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #868]
	ldr	r1, [pc, #852]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #860]
	ldr	r1, [pc, #860]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r6, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r6, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #832]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r0, [sp, #40]
	mov	r7, sp
	movs	r6, #0
	adds	r7, #68
	lsls	r0, r0, #16
	str	r6, [sp, #56]
	str	r7, [sp, #16]
	str	r0, [sp, #12]
.L_081794b6:
	ldr	r1, [sp, #56]
	cmp	r1, #0
	bne.n	.L_081794f4
	ldr	r3, [sp, #64]
	movs	r2, #0
	mov	r8, r2
	adds	r3, #24
	movs	r2, #24
.L_081794c6:
	movs	r4, #1
	add	r8, r4
	mov	r6, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_081794c6
	ldr	r3, [pc, #780]
	movs	r7, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r7
	negs	r1, r1
	lsls	r2, r2, #1
.L_081794e2:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_081794e2
	movs	r0, #140
	bl	sub_081c0010
.L_081794f4:
	ldr	r3, [sp, #56]
	subs	r3, #40
	cmp	r3, #15
	bhi.n	.L_08179502
	ldr	r0, [pc, #744]
	bl	sub_0815f0a0
.L_08179502:
	ldr	r1, [sp, #56]
	cmp	r1, #56
	bne.n	.L_0817950e
	movs	r0, #144
	bl	sub_081c0010
.L_0817950e:
	ldr	r2, [sp, #56]
	cmp	r2, #55
	bgt.n	.L_08179516
	b.n	.L_0817979c
.L_08179516:
	ldr	r4, [sp, #36]
	ldr	r6, [pc, #720]
	ldr	r7, [pc, #724]
	lsls	r3, r2, #3
	adds	r3, r4, r3
	adds	r6, r6, r3
	adds	r3, r3, r7
	mov	sl, r6
	cmp	r3, #7
	bls.n	.L_0817952c
	b.n	.L_08179680
.L_0817952c:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #40]
	asrs	r1, r1, #31
	lsrs	r3, r1, #31
	adds	r3, r2, r3
	ldr	r6, [sp, #64]
	movs	r0, #0
	asrs	r3, r3, #1
	mov	r8, r0
	mov	r9, r1
	lsls	r7, r3, #16
.L_08179542:
	mov	r4, r8
	negs	r3, r4
	cmp	r3, #0
	bge.n	.L_0817954c
	adds	r3, #3
.L_0817954c:
	asrs	r3, r3, #2
	adds	r3, #2
	str	r3, [r6, #24]
	str	r7, [r6, #0]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	add	r3, sl
	adds	r3, #16
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08014878
	mov	r1, fp
	ldr	r3, [r1, #24]
	ldr	r2, [pc, #644]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r5, [r2, r3]
	adds	r1, r5, #0
	bl	sub_0800206c
	lsrs	r5, r5, #1
	subs	r0, r0, r5
	lsls	r0, r0, #12
	str	r0, [r6, #12]
	bl	sub_08014878
	movs	r2, #255
	ands	r2, r0
	movs	r3, #192
	subs	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r6, #16]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r6, #28
	cmp	r4, #32
	bne.n	.L_08179542
	mov	r7, fp
	ldr	r3, [r7, #24]
	ldr	r0, [pc, #592]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r0, r3]
	movs	r6, #0
	mov	r8, r6
	cmp	r3, #0
	beq.n	.L_08179634
	ldr	r2, [sp, #40]
	mov	r1, r9
	lsrs	r3, r1, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	ldr	r7, [pc, #568]
	str	r3, [sp, #32]
	movs	r3, #15
	mov	r9, r3
.L_081795c6:
	ldr	r4, [sp, #32]
	mov	r6, r9
	str	r4, [r7, #0]
	bl	sub_08014878
	ands	r0, r6
	add	r0, sl
	adds	r0, #24
	lsls	r0, r0, #16
	str	r0, [r7, #4]
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	ands	r6, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r6, r6, r0
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	mov	r1, r9
	ands	r0, r1
	adds	r0, #16
	str	r0, [r7, #24]
	mov	r4, fp
	ldr	r3, [r4, #24]
	ldr	r6, [pc, #464]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	movs	r2, #1
	add	r8, r2
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_081795c6
.L_08179634:
	movs	r0, #145
	bl	sub_081180e8
	mov	r0, fp
	ldr	r3, [r0, #20]
	movs	r7, #0
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_08179672
	movs	r5, #36
.L_08179648:
	mov	r1, fp
	ldrsh	r0, [r5, r1]
	movs	r1, #0
	bl	sub_08118088
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	movs	r2, #5
	mov	r7, fp
	bl	sub_0814cd48
	ldr	r3, [r7, #20]
	movs	r6, #1
	add	r8, r6
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08179648
.L_08179672:
	ldr	r0, [sp, #64]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #16
	str	r3, [r2, #0]
.L_08179680:
	mov	r2, sl
	cmp	r2, #52
	bgt.n	sub_081796bc
	ldr	r3, [sp, #40]
	movs	r4, #24
	lsrs	r5, r3, #31
	adds	r5, r3, r5
	asrs	r5, r5, #1
	movs	r6, #64
	adds	r2, r5, #0
	str	r4, [sp, #0]
	ldr	r1, [pc, #304]
	subs	r2, #24
	mov	r3, sl
	str	r4, [sp, #8]
	str	r6, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r7, [sp, #48]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9c02
	str	r6, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #276]
	adds	r2, r5, #0
	mov	r3, sl
	ldr	r4, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465f
	movs	r6, #0
	ldr	r2, [r7, #24]
	mov	r8, r6
	ldr	r6, [pc, #300]
	lsls	r3, r2, #2
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_08179730
	ldr	r5, [sp, #64]
	adds	r7, r6, #0
.L_081796d2:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0817971a
	cmp	r3, #1
	ble.n	sub_0817970e
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_081796e4
	adds	r1, r3, #3
.L_081796e4:
	ldr	r0, [sp, #64]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r2, #142
	adds	r1, r0, r1
	lsls	r2, r2, #7
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #232]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0817971a:
	adds	r3, #1
	str	r3, [r5, #24]
	mov	r1, fp
	ldr	r2, [r1, #24]
	movs	r0, #1
	lsls	r3, r2, #2
	ldrb	r3, [r7, r3]
	add	r8, r0
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_081796d2
.L_08179730:
	movs	r3, #0
	mov	r8, r3
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_0817979c
	ldr	r5, [pc, #184]
	ldr	r7, [pc, #188]
.L_08179742:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0817978c
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r7, r3]
	ldr	r6, [sp, #44]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r6, r1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	lsls	r2, r2, #5
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r5, #24]
	mov	r0, fp
	subs	r3, #1
	str	r3, [r5, #24]
	ldr	r6, [pc, #104]
	ldr	r2, [r0, #24]
.L_0817978c:
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	movs	r1, #1
	add	r8, r1
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_08179742
.L_0817979c:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #28]
	movs	r0, #1
	bl	sub_081969f8
	movs	r2, #0
	str	r2, [sp, #24]
	movs	r2, #7
	str	r2, [r0, #0]
	ldr	r1, [sp, #16]
	ldr	r3, [pc, #76]
	str	r1, [r0, #16]
	str	r3, [r0, #8]
	ldr	r3, [sp, #28]
	movs	r6, #68
	str	r3, [r0, #12]
	ldr	r4, [sp, #24]
	b.n	.L_08179808
	.4byte 0x000000ed
	.4byte 0x02010000
	.4byte 0x00000134
	.4byte 0x0000013e
	.4byte 0x0000017f
	.4byte 0x000000c2
	.4byte 0x02011000
	.4byte 0x08143001
	.4byte 0x02015018
	.4byte 0x00000148
	.4byte 0xfffffe20
	.4byte 0xfffffdec
	.4byte 0x08199400
	.4byte 0x02015000
	.4byte 0xfffff000
	.4byte 0x08197410
	.2byte 0x9364
	.2byte 0x0819
.L_08179808:
	add	r6, sp
	str	r4, [r0, #20]
	str	r6, [sp, #16]
	strb	r2, [r6, #0]
	strb	r2, [r1, #1]
	ldr	r3, [sp, #36]
	movs	r7, #0
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [sp, #20]
	mov	sl, r0
	mov	r9, r6
	mov	r8, r7
.L_08179822:
	mov	r0, fp
	ldr	r3, [r0, #24]
	ldr	r1, [pc, #340]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	lsls	r6, r3, #10
	lsls	r7, r3, #5
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08179860
	ldr	r3, [sp, #36]
	ldr	r2, [pc, #304]
	lsls	r1, r3, #16
	ldr	r4, [sp, #12]
	ldr	r3, [pc, #304]
	adds	r0, r4, r2
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08179872
.L_08179860:
	ldr	r1, [pc, #284]
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #12]
	ldr	r3, [pc, #284]
	adds	r0, r4, r1
	adds	r1, r2, r3
	movs	r2, #0
	bl	sub_08015160
.L_08179872:
	movs	r0, #250
	lsls	r0, r0, #3
	bl	sub_08015024
	ldr	r4, [sp, #56]
	cmp	r4, #47
	bgt.n	.L_0817988a
	movs	r0, #48
	subs	r0, r0, r4
	lsls	r0, r0, #9
	bl	sub_08015024
.L_0817988a:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_081798c8
	ldr	r3, [sp, #56]
	subs	r3, #16
	adds	r5, r7, #0
	muls	r5, r3
	cmp	r5, r6
	ble.n	.L_0817989e
	adds	r5, r6, #0
.L_0817989e:
	ldr	r1, [sp, #56]
	cmp	r1, #60
	ble.n	.L_081798ac
	lsls	r3, r1, #1
	subs	r3, #120
	muls	r3, r7
	subs	r5, r6, r3
.L_081798ac:
	ldr	r2, [sp, #64]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	mov	r6, r9
	str	r3, [r6, #4]
	ldr	r7, [sp, #56]
	negs	r3, r7
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #8
	bl	sub_08015068
	b.n	.L_081798f0
.L_081798c8:
	ldr	r0, [sp, #56]
	lsls	r3, r0, #2
	subs	r3, #224
	adds	r5, r7, #0
	muls	r5, r3
	cmp	r5, #0
	blt.n	.L_08179914
	movs	r2, #128
	lsls	r3, r0, #3
	lsls	r2, r2, #2
	subs	r2, r2, r3
	mov	r3, sl
	str	r2, [sp, #24]
	str	r2, [r3, #20]
	ldr	r3, [pc, #160]
	mov	r4, r9
	movs	r1, #128
	str	r3, [r4, #4]
	lsls	r1, r1, #7
	adds	r5, r5, r1
.L_081798f0:
	cmp	r5, #0
	ble.n	.L_08179914
	ldr	r6, [sp, #24]
	movs	r7, #64
	negs	r7, r7
	cmp	r6, r7
	ble.n	.L_08179914
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r0, [pc, #132]
	ldr	r1, [sp, #28]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08179914:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L_08179822
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #28]
	bl	sub_08013164
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #64]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #56]
	adds	r6, #1
	str	r6, [sp, #56]
	cmp	r6, #96
	beq.n	.L_08179956
	b.n	.L_081794b6
.L_08179956:
	ldr	r0, [pc, #56]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #172
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08199400
	.4byte 0xff800000
	.4byte 0xffc00000
	.4byte 0x02011000
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
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	sub	sp, #120
	mov	fp, r0
	ldr	r0, [r3, #92]
	str	r1, [sp, #60]
	mov	r8, r0
	ldr	r3, [r3, #100]
	movs	r0, #1
	str	r3, [sp, #52]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #224
	adds	r2, #82
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	add	r1, r8
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #52]
	bl	sub_08157cf4
	mov	r2, fp
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	sub_08179a10
	ldr	r0, [pc, #20]
	bl	sub_08013300
	adds	r1, r0, #0
	b.n	.L_08179a04
	.4byte 0x00001010
	.4byte 0x000000eb
	.4byte 0x00000134
	.2byte 0x0148
	.2byte 0x0000
.L_08179a04:
	movs	r0, #160
	ldr	r3, [pc, #856]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x466b
	adds	r3, #64
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #48]
	bl	sub_08144aac
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r8
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r8
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #808]
	bl	sub_080145a8
	movs	r7, #0
	str	r7, [sp, #44]
	ldr	r3, [pc, #804]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #192
	mov	r9, r0
	negs	r1, r1
	lsls	r2, r2, #2
.L_08179a50:
	movs	r7, #1
	add	r9, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_08179a50
	mov	r1, sp
	movs	r0, #0
	adds	r1, #108
	str	r0, [sp, #56]
	str	r1, [sp, #36]
	str	r0, [sp, #16]
.L_08179a68:
	ldr	r2, [sp, #56]
	cmp	r2, #16
	bne.n	.L_08179a74
	movs	r0, #104
	bl	sub_081c0010
.L_08179a74:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_08179a7e
	movs	r7, #0
	str	r7, [sp, #44]
.L_08179a7e:
	mov	r1, fp
	ldr	r0, [r1, #8]
	ldr	r1, [sp, #36]
	bl	sub_0815e21c
	ldr	r2, [sp, #56]
	cmp	r2, #7
	ble.n	.L_08179aa2
	mov	r3, fp
	ldr	r1, [r3, #24]
	ldr	r4, [pc, #728]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	ldrb	r3, [r4, r3]
	ldr	r7, [sp, #44]
	adds	r7, r7, r3
	str	r7, [sp, #44]
	b.n	.L_08179aa8
.L_08179aa2:
	mov	r0, fp
	ldr	r1, [r0, #24]
	ldr	r4, [pc, #708]
.L_08179aa8:
	lsls	r3, r1, #3
	subs	r3, r3, r1
	adds	r3, #1
	ldrb	r2, [r4, r3]
	ldr	r1, [sp, #44]
	cmp	r1, r2
	ble.n	.L_08179ab8
	str	r2, [sp, #44]
.L_08179ab8:
	ldr	r2, [sp, #56]
	cmp	r2, #63
	bgt.n	.L_08179ad6
	ldr	r7, [sp, #36]
	mov	r0, r8
	ldr	r3, [r7, #0]
	lsls	r3, r3, #16
	str	r3, [r0, #0]
	ldr	r3, [r7, #4]
	ldr	r1, [sp, #44]
	subs	r3, r3, r1
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r0, #4]
	b.n	.L_08179b6a
.L_08179ad6:
	ldr	r2, [sp, #56]
	cmp	r2, #64
	bne.n	.L_08179b28
	add	r5, sp, #96
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r3, [sp, #36]
	mov	r7, r8
	ldr	r2, [r3, #0]
	lsls	r2, r2, #16
	str	r2, [r7, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #44]
	ldr	r3, [r0, #4]
	subs	r3, r3, r1
	subs	r3, #32
	lsls	r0, r3, #16
	movs	r3, #0
	str	r0, [r7, #4]
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	ldr	r3, [r5, #0]
	lsls	r3, r3, #16
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_08179b0e
	adds	r3, #63
.L_08179b0e:
	asrs	r3, r3, #6
	mov	r2, r8
	str	r3, [r2, #8]
	ldr	r3, [r5, #4]
	subs	r3, #32
	lsls	r3, r3, #16
	subs	r0, r3, r0
	cmp	r0, #0
	bge.n	.L_08179b22
	adds	r0, #63
.L_08179b22:
	asrs	r3, r0, #6
	mov	r7, r8
	str	r3, [r7, #20]
.L_08179b28:
	mov	r0, r8
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #0]
	ldr	r1, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #20]
	str	r2, [r0, #12]
	adds	r1, r1, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	str	r1, [r0, #16]
	cmp	r3, #0
	bge.n	.L_08179b52
	adds	r3, #63
.L_08179b52:
	asrs	r3, r3, #6
	mov	r2, r8
	str	r3, [r2, #12]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r1, r3, #4
	cmp	r1, #0
	bge.n	.L_08179b64
	adds	r1, #63
.L_08179b64:
	asrs	r3, r1, #6
	mov	r7, r8
	str	r3, [r7, #16]
.L_08179b6a:
	mov	r1, fp
	ldr	r3, [r1, #24]
	ldr	r7, [pc, #508]
	lsls	r2, r3, #3
	subs	r2, r2, r3
	adds	r2, #2
	ldrb	r3, [r7, r2]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_08179c64
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #56]
	ldr	r2, [sp, #44]
	asrs	r0, r0, #31
	lsls	r1, r1, #2
	str	r0, [sp, #28]
	str	r1, [sp, #32]
	lsls	r2, r2, #16
	mov	sl, r2
	add	r5, sp, #84
.L_08179b94:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #56]
	lsrs	r3, r0, #31
	adds	r3, r1, r3
	mov	r0, r9
	lsls	r2, r0, #2
	asrs	r3, r3, #1
	adds	r6, r3, r2
	movs	r3, #3
	ands	r6, r3
	bl	sub_08014de4
	mov	r1, r8
	ldr	r3, [pc, #448]
	ldr	r0, [r1, #0]
	ldr	r2, [pc, #448]
	ldr	r1, [r1, #4]
	adds	r0, r0, r2
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [sp, #16]
	bl	sub_080150e4
	ldr	r1, [sp, #16]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r0, r1, r2
	bl	sub_08015024
	mov	r3, r9
	cmp	r3, #7
	ble.n	.L_08179be0
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
.L_08179be0:
	mov	r0, fp
	ldr	r2, [r0, #24]
	ldr	r1, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #3
	ldrb	r3, [r7, r3]
	mov	r0, r9
	muls	r0, r3
	adds	r0, r1, r0
	lsls	r0, r0, #8
	bl	sub_08015068
	movs	r3, #0
	mov	r2, sl
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	ldr	r3, [pc, #368]
	adds	r1, r5, #0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x682a
	asrs	r2, r2, #17
	adds	r3, r2, #0
	adds	r3, #64
	str	r3, [r5, #0]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	adds	r2, #56
	adds	r1, r3, #0
	adds	r1, #64
	str	r1, [r5, #4]
	mov	r1, fp
	ldr	r0, [r1, #24]
	adds	r3, #16
	lsls	r1, r0, #3
	subs	r1, r1, r0
	adds	r1, #6
	ldrb	r4, [r7, r1]
	movs	r0, #224
	lsls	r1, r6, #10
	lsls	r0, r0, #3
	add	r1, r8
	adds	r1, r1, r0
	movs	r0, #16
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r0, [sp, #48]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465b
	ldr	r2, [r3, #24]
	movs	r1, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r7, r3]
	add	r9, r1
	cmp	r9, r3
	bne.n	.L_08179b94
.L_08179c64:
	ldr	r7, [sp, #56]
	cmp	r7, #88
	beq.n	.L_08179c6c
	b.n	.L_08179d98
.L_08179c6c:
	movs	r0, #134
	bl	sub_081180e8
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_08179c80
	b.n	.L_08179d80
.L_08179c80:
	mov	r2, sp
	adds	r2, #72
	movs	r3, #36
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r0, [sp, #12]
.L_08179c8c:
	ldr	r7, [sp, #20]
	mov	r2, fp
	ldrsh	r0, [r7, r2]
	ldr	r1, [sp, #24]
	bl	sub_0815e1fc
	mov	r1, fp
	ldrsh	r0, [r7, r1]
	movs	r1, #4
	bl	sub_08118088
	mov	r3, fp
	ldrsh	r0, [r7, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r9
	movs	r2, #5
	bl	sub_0814cd48
	movs	r7, #0
	str	r7, [sp, #40]
	mov	r0, fp
	ldr	r1, [r0, #24]
	ldr	r4, [pc, #172]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	adds	r3, #4
	ldrb	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L_08179d40
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #172]
	mov	sl, r1
	adds	r7, r2, r3
.L_08179cd4:
	mov	r0, sl
	ldr	r3, [r0, #0]
	str	r4, [sp, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r5, #255
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #24
	str	r3, [r7, #24]
	ldr	r1, [sp, #40]
	mov	r2, fp
	adds	r1, #1
	str	r1, [sp, #40]
	ldr	r0, [pc, #64]
	ldr	r1, [r2, #24]
	ldr	r2, [sp, #40]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	adds	r3, #4
	ldrb	r3, [r0, r3]
	adds	r7, #28
	ldr	r4, [sp, #8]
	cmp	r2, r3
	bne.n	.L_08179cd4
.L_08179d40:
	ldr	r3, [sp, #20]
	ldr	r7, [sp, #12]
	movs	r0, #224
	lsls	r0, r0, #4
	adds	r3, #2
	adds	r7, r7, r0
	str	r7, [sp, #12]
	str	r3, [sp, #20]
	mov	r7, fp
	ldr	r3, [r7, #20]
	movs	r2, #1
	add	r9, r2
	cmp	r9, r3
	bne.n	.L_08179c8c
	b.n	.L_08179d86
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x0819940c
	.4byte 0xffc00000
	.4byte 0xff800000
	.4byte 0x03000354
	.2byte 0x0000
	.2byte 0x0201
.L_08179d80:
	mov	r0, fp
	ldr	r1, [r0, #24]
	ldr	r4, [pc, #208]
.L_08179d86:
	lsls	r2, r1, #3
	subs	r2, r2, r1
	movs	r3, #238
	adds	r2, #5
	lsls	r3, r3, #7
	ldrb	r2, [r4, r2]
	adds	r3, #168
	add	r3, r8
	str	r2, [r3, #0]
.L_08179d98:
	ldr	r2, [pc, #192]
	ldr	r6, [pc, #196]
	movs	r1, #0
	mov	r9, r1
	mov	sl, r2
.L_08179da2:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_08179df0
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	mov	r3, r9
	movs	r4, #1
	ands	r4, r3
	mov	r7, sl
	subs	r3, r5, #2
	ldrh	r1, [r7, r3]
	ldr	r2, [sp, #52]
	lsls	r4, r4, #2
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	ldr	r2, [pc, #124]
	bl	sub_08138086
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_08179df0:
	movs	r1, #1
	movs	r2, #192
	add	r9, r1
	lsls	r2, r2, #2
	adds	r6, #28
	cmp	r9, r2
	bne.n	.L_08179da2
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #56]
	movs	r7, #128
	lsls	r7, r7, #1
	adds	r3, r3, r7
	adds	r0, #1
	str	r3, [sp, #16]
	str	r0, [sp, #56]
	cmp	r0, #122
	beq.n	.L_08179e32
	b.n	.L_08179a68
.L_08179e32:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0819940c
	.4byte 0x08197410
	.4byte 0x02010000
	.4byte 0xffffc000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #96]
	movs	r7, #7
	mov	r8, r3
	bl	sub_0815b410
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #255
	movs	r1, #0
	mov	ip, r3
	b.n	.L_08179ec4
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x0000004e
	.4byte 0x00001010
	.2byte 0x3f44
	.2byte 0x0000
.L_08179ec4:
	adds	r5, r1, #0
	movs	r4, #0
	lsls	r6, r1, #1
	ands	r5, r7
.L_08179ecc:
	lsls	r0, r1, #1
	cmp	r1, #31
	ble.n	.L_08179edc
	mov	r3, ip
	subs	r0, r3, r6
	cmp	r1, #95
	bgt.n	.L_08179edc
	movs	r0, #63
.L_08179edc:
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_08179ee4
	adds	r2, r4, #7
.L_08179ee4:
	asrs	r2, r2, #3
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08179eee
	adds	r3, r1, #7
.L_08179eee:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r4, #0
	ands	r3, r7
	lsls	r2, r2, #3
	adds	r2, r2, r3
	lsls	r2, r2, #3
	adds	r2, r2, r5
	mov	r3, r8
	adds	r4, #1
	strb	r0, [r3, r2]
	cmp	r4, #128
	bne.n	.L_08179ecc
	adds	r1, #1
	cmp	r1, #128
	bne.n	.L_08179ec4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
