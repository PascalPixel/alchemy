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
	.set sub_0801587c, 0x0801587c
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08154b6c, 0x08154b6c
	.set sub_08154f28, 0x08154f28
	.set sub_081557d0, 0x081557d0
	.set sub_08155e5a, 0x08155e5a
	.set sub_08156140, 0x08156140
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_08164abc, 0x08164abc
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_081c0010, 0x081c0010
	.global Func_081548d0
	.thumb_func
Func_081548d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #40]
	str	r0, [sp, #44]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #36]
	movs	r0, #1
	ldr	r3, [r3, #96]
	str	r3, [sp, #32]
	bl	sub_081435e0
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r1, [sp, #40]
	cmp	r1, #1
	bne.n	.L_0815492c
	ldr	r2, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #16]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	b.n	.L_0815493e
	.4byte 0x00000100
	.4byte 0x00000000
	.2byte 0x0144
	.2byte 0x0000
.L_0815492c:
	ldr	r5, [sp, #36]
	movs	r6, #224
	lsls	r6, r6, #3
	ldr	r0, [pc, #340]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
.L_0815493e:
	ldr	r0, [sp, #44]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_08154950
	movs	r2, #128
	ldr	r3, [pc, #320]
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
.L_08154950:
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #48]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r1, sp
	adds	r1, #48
	str	r1, [sp, #24]
	str	r3, [r1, #4]
	ldr	r5, [sp, #44]
	ldr	r2, [pc, #280]
	ldr	r3, [r5, #24]
	movs	r6, #0
	ldrb	r3, [r2, r3]
	movs	r1, #1
	lsls	r3, r3, #2
	adds	r3, #56
	str	r3, [sp, #28]
	ldr	r3, [pc, #268]
	movs	r2, #128
	mov	sl, r6
	negs	r1, r1
	lsls	r2, r2, #3
.L_08154990:
	movs	r0, #1
	add	sl, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_08154990
	ldr	r7, [pc, #248]
	ldr	r6, [sp, #36]
	movs	r1, #0
	mov	sl, r1
.L_081549a4:
	bl	sub_08014878
	ldr	r3, [pc, #240]
	mov	r5, sl
	ldrb	r2, [r3, r5]
	movs	r3, #7
	ands	r3, r0
	adds	r2, r2, r3
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	adds	r3, #108
	subs	r2, #4
	str	r3, [r6, #4]
	str	r2, [r6, #0]
	bl	sub_08014878
	movs	r5, #63
	ands	r5, r0
	adds	r5, #55
	str	r5, [r6, #16]
	mov	r0, sl
	movs	r1, #3
	bl	sub_08002064
	ldrb	r3, [r7, r0]
	cmp	r3, r5
	bge.n	.L_081549de
	str	r3, [r6, #16]
.L_081549de:
	mov	r0, sl
	movs	r1, #1
	lsls	r3, r0, #2
	add	sl, r1
	adds	r3, #8
	mov	r2, sl
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r2, #16
	bne.n	.L_081549a4
	ldr	r3, [sp, #36]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r6, [sp, #36]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r6, r0
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #140]
	bl	sub_080145a8
	ldr	r2, [sp, #28]
	movs	r1, #0
	mov	fp, r1
	cmp	r2, #0
	bne.n	.L_08154a22
	b.n	.L_08154d0c
.L_08154a22:
	ldr	r3, [sp, #28]
	ldr	r5, [sp, #28]
	subs	r2, #64
	subs	r3, #20
	subs	r5, #4
	str	r2, [sp, #12]
	str	r3, [sp, #20]
	str	r5, [sp, #16]
.L_08154a32:
	ldr	r6, [sp, #12]
	cmp	fp, r6
	bne.n	.L_08154a3e
	movs	r0, #133
	bl	sub_081180e8
.L_08154a3e:
	ldr	r0, [sp, #20]
	cmp	fp, r0
	blt.n	.L_08154a6a
	ldr	r1, [sp, #16]
	cmp	fp, r1
	blt.n	.L_08154a4c
	b.n	.L_08154c1e
.L_08154a4c:
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r2, [sp, #28]
	mov	r5, fp
	subs	r3, r2, r5
	ldr	r2, [pc, #36]
	movs	r1, #128
	lsls	r1, r1, #19
	subs	r3, #5
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_08154a6a:
	ldr	r6, [sp, #16]
	cmp	fp, r6
	blt.n	.L_08154a72
	b.n	.L_08154c1e
.L_08154a72:
	ldr	r1, [sp, #44]
	ldr	r3, [pc, #24]
	ldr	r2, [r1, #24]
	movs	r0, #0
	ldrb	r3, [r3, r2]
	mov	sl, r0
	b.n	.L_08154aa4
	.4byte 0x00003f44
	.4byte 0x00001000
	.4byte 0x00000145
	.4byte 0xffff9000
	.4byte 0x081983ea
	.4byte 0x02010018
	.4byte 0x081983d7
	.4byte 0x081983ed
	.2byte 0x3001
	.2byte 0x0814
.L_08154aa4:
	cmp	r3, #0
	bne.n	.L_08154aaa
	b.n	.L_08154c1e
.L_08154aaa:
	ldr	r3, [sp, #36]
	movs	r2, #8
	mov	r9, r2
	mov	r8, r3
.L_08154ab2:
	mov	r5, sl
	lsls	r3, r5, #2
	adds	r3, #9
	cmp	fp, r3
	bne.n	.L_08154aca
	ldr	r6, [sp, #36]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r6, r0
	movs	r3, #2
	str	r3, [r2, #0]
.L_08154aca:
	cmp	fp, r9
	ble.n	sub_08154b6c
	movs	r1, #3
	mov	r0, sl
	bl	sub_08002064
	mov	r1, fp
	mov	r2, r9
	subs	r3, r1, r2
	mov	r6, r8
	lsls	r5, r3, #3
	ldr	r3, [r6, #16]
	cmp	r5, r3
	ble.n	.L_08154ae8
	adds	r5, r3, #0
.L_08154ae8:
	ldr	r1, [sp, #40]
	cmp	r1, #0
	bne.n	.L_08154b26
	mov	r2, sl
	movs	r4, #1
	ands	r4, r2
	ldr	r2, [pc, #568]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #36]
	movs	r6, #224
	adds	r1, r3, r1
	ldr	r3, [pc, #560]
	lsls	r6, r6, #3
	ldrb	r0, [r3, r0]
	adds	r1, r1, r6
	mov	r6, r8
	ldr	r2, [r6, #0]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	subs	r3, r3, r5
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe022
.L_08154b26:
	ldr	r2, [pc, #528]
	movs	r3, #7
	mov	r1, sl
	ands	r3, r1
	ldrsb	r3, [r2, r3]
	cmp	r5, r3
	ble.n	.L_08154b36
	adds	r5, r3, #0
.L_08154b36:
	mov	r2, sl
	movs	r4, #1
	ands	r4, r2
	ldr	r2, [pc, #508]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #36]
	movs	r6, #224
	adds	r1, r3, r1
	ldr	r3, [pc, #500]
	lsls	r6, r6, #3
	ldrb	r0, [r3, r0]
	adds	r1, r1, r6
	mov	r6, r8
	ldr	r2, [r6, #0]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	subs	r3, r3, r5
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x990b
	movs	r5, #0
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_08154bb8
	mov	r6, r9
	movs	r4, #1
	mov	r2, sl
	adds	r6, #4
	ands	r4, r2
	movs	r7, #36
.L_08154b82:
	cmp	fp, r6
	bne.n	.L_08154bae
	cmp	r4, #0
	bne.n	.L_08154b94
	movs	r0, #133
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r4, [sp, #8]
.L_08154b94:
	ldr	r3, [sp, #44]
	movs	r2, #5
	ldrsh	r0, [r7, r3]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r5, #0
	str	r4, [sp, #8]
	bl	sub_0814cd48
	ldr	r2, [sp, #44]
	ldr	r4, [sp, #8]
	ldr	r3, [r2, #20]
.L_08154bae:
	adds	r5, #1
	adds	r7, #2
	cmp	r5, r3
	bne.n	.L_08154b82
	b.n	.L_08154bbc
.L_08154bb8:
	mov	r6, r9
	adds	r6, #4
.L_08154bbc:
	cmp	fp, r6
	beq.n	.L_08154bc8
	mov	r3, r9
	adds	r3, #8
	cmp	fp, r3
	bne.n	.L_08154c04
.L_08154bc8:
	ldr	r6, [pc, #376]
	movs	r5, #0
	movs	r7, #15
	b.n	.L_08154bd4
.L_08154bd0:
	adds	r6, #28
	adds	r5, #1
.L_08154bd4:
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r5, r3
	beq.n	.L_08154c04
	ldr	r3, [r6, #24]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_08154bd0
	bl	sub_08014878
	mov	r1, r8
	ldr	r3, [r1, #0]
	ands	r0, r7
	adds	r0, r0, r3
	subs	r0, #8
	str	r0, [r6, #0]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #80
	movs	r3, #0
	str	r0, [r6, #4]
	str	r3, [r6, #24]
.L_08154c04:
	ldr	r6, [sp, #44]
	movs	r2, #4
	movs	r3, #28
	add	r9, r2
	add	r8, r3
	ldr	r2, [r6, #24]
	ldr	r3, [pc, #308]
	movs	r5, #1
	ldrb	r3, [r3, r2]
	add	sl, r5
	cmp	sl, r3
	beq.n	.L_08154c1e
	b.n	.L_08154ab2
.L_08154c1e:
	ldr	r1, [pc, #292]
	movs	r0, #0
	mov	sl, r0
	mov	r8, r1
.L_08154c26:
	mov	r3, r8
	ldr	r2, [r3, #24]
	cmp	r2, #0
	blt.n	.L_08154cd0
	movs	r5, #240
	ldr	r6, [sp, #40]
	lsrs	r3, r2, #31
	lsls	r5, r5, #5
	adds	r3, r2, r3
	adds	r5, #89
	asrs	r7, r3, #1
	mov	r9, r5
	cmp	r6, #0
	beq.n	.L_08154c4a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #255
	mov	r9, r0
.L_08154c4a:
	ldr	r2, [pc, #256]
	lsls	r6, r7, #1
	ldrh	r1, [r2, r6]
	ldr	r3, [sp, #36]
	add	r1, r9
	adds	r1, r3, r1
	mov	r3, r8
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #244]
	ldr	r3, [pc, #244]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r1, r5
	ldrsb	r5, [r0, r7]
	ldrb	r0, [r3, r7]
	mov	r3, r8
	lsls	r0, r0, #24
	asrs	r4, r0, #24
	ldr	r3, [r3, #4]
	lsrs	r0, r0, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r5
	subs	r3, r3, r0
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4d31
	movs	r0, #224
	ldrh	r1, [r5, r6]
	ldr	r6, [sp, #36]
	ldr	r5, [pc, #196]
	add	r1, r9
	adds	r1, r6, r1
	lsls	r0, r0, #3
	adds	r1, r1, r0
	ldrb	r0, [r5, r7]
	mov	r3, r8
	lsls	r0, r0, #24
	ldr	r2, [r3, #0]
	asrs	r4, r0, #24
	ldr	r3, [r3, #4]
	ldr	r6, [pc, #168]
	lsrs	r0, r0, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r3, r3, r0
	ldrsb	r0, [r6, r7]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	ldr	r0, [sp, #24]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4641
	ldr	r3, [r1, #24]
	adds	r3, #1
	str	r3, [r1, #24]
	cmp	r3, #14
	bne.n	.L_08154cd0
	movs	r3, #1
	negs	r3, r3
	str	r3, [r1, #24]
.L_08154cd0:
	movs	r3, #1
	movs	r5, #128
	movs	r2, #28
	add	sl, r3
	lsls	r5, r5, #2
	add	r8, r2
	cmp	sl, r5
	bne.n	.L_08154c26
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r0, #240
	ldr	r6, [sp, #36]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r6, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #28]
	movs	r1, #1
	add	fp, r1
	cmp	fp, r2
	beq.n	.L_08154d0c
	b.n	.L_08154a32
.L_08154d0c:
	ldr	r0, [pc, #72]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081983da
	.4byte 0x081983d4
	.4byte 0x081983fd
	.4byte 0x081983e4
	.4byte 0x081983e0
	.4byte 0x02010000
	.4byte 0x081983ea
	.4byte 0x08198414
	.4byte 0x08198405
	.4byte 0x0819840c
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r0
	movs	r0, #192
	lsls	r0, r0, #18
	ldr	r1, [r0, #92]
	ldr	r2, [r0, #96]
	sub	sp, #72
	mov	r8, r0
	movs	r0, #0
	str	r2, [sp, #12]
	mov	sl, r1
	bl	sub_081435e0
	mov	r3, r9
	ldr	r1, [r3, #4]
	movs	r3, #132
	lsls	r1, r1, #4
	add	r2, sp, #60
	orrs	r1, r3
	mov	r0, r9
	add	r3, sp, #48
	bl	.L_0815585c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #20
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r1, #224
	adds	r2, #48
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	add	r1, sl
	movs	r3, #1
	ldr	r0, [pc, #36]
	movs	r2, #1
	bl	sub_08157cf4
	mov	r2, r9
	add	r6, sp, #36
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r6, #0
	bl	sub_0815e20c
	mov	r0, r9
	ldr	r3, [r0, #20]
	b.n	.L_08154de0
	.4byte 0x00000785
	.4byte 0x00000100
	.4byte 0x00000000
	.2byte 0x016b
	.2byte 0x0000
.L_08154de0:
	add	r5, sp, #24
	lsls	r3, r3, #1
	adds	r3, #34
	ldrsh	r0, [r0, r3]
	adds	r1, r5, #0
	bl	sub_0815e20c
	ldr	r1, [r6, #0]
	ldr	r3, [r5, #0]
	movs	r0, #104
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #128
	movs	r3, #64
	subs	r3, r3, r1
	lsls	r2, r2, #19
	adds	r2, #40
	lsls	r3, r3, #8
	str	r1, [r6, #0]
	str	r3, [r2, #0]
	movs	r1, #35
	bl	sub_081963ec
	mov	r2, r8
	ldr	r3, [r2, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #16]
	bl	sub_081963ec
	movs	r3, #188
	add	r8, r3
	mov	r0, r8
	ldr	r3, [r0, #0]
	add	r1, sp, #16
	mov	fp, r1
	str	r3, [r1, #4]
	ldr	r1, [pc, #328]
	movs	r6, #0
	mov	r2, sl
.L_08154e36:
	ldrb	r3, [r1, #0]
	adds	r6, #1
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #64
	str	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #28
	cmp	r6, #16
	bne.n	.L_08154e36
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #284]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r7, #0
.L_08154e6c:
	cmp	r7, #32
	bne.n	.L_08154ea4
	movs	r0, #143
	bl	sub_081c0010
	mov	r2, r9
	ldr	r3, [r2, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_08154ea4
	movs	r4, #16
	movs	r5, #36
.L_08154e84:
	mov	r3, r9
	ldrsh	r0, [r5, r3]
	movs	r2, #5
	adds	r3, r6, #0
	movs	r1, #7
	str	r4, [sp, #0]
	str	r4, [sp, #8]
	bl	sub_0814cd48
	mov	r2, r9
	ldr	r3, [r2, #20]
	adds	r6, #1
	adds	r5, #2
	ldr	r4, [sp, #8]
	cmp	r6, r3
	bne.n	.L_08154e84
.L_08154ea4:
	movs	r6, #0
	mov	r8, sl
.L_08154ea8:
	lsls	r3, r6, #2
	adds	r3, #5
	cmp	r7, r3
	bne.n	.L_08154ebc
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
.L_08154ebc:
	lsls	r5, r6, #1
	adds	r3, r5, #4
	cmp	r7, r3
	ble.n	sub_08154f28
	adds	r0, r7, #0
	cmp	r7, #0
	bge.n	.L_08154ecc
	adds	r0, r7, #3
.L_08154ecc:
	asrs	r0, r0, #2
	adds	r0, r0, r6
	movs	r1, #5
	bl	sub_08002064
	adds	r3, r5, #0
	adds	r3, #32
	cmp	r7, r3
	bge.n	.L_08154eee
	subs	r3, r7, r5
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #16
	cmp	r5, #32
	ble.n	.L_08154ef6
	movs	r5, #32
	b.n	.L_08154ef6
.L_08154eee:
	subs	r3, r7, r5
	lsls	r3, r3, #2
	movs	r2, #160
	subs	r5, r2, r3
.L_08154ef6:
	cmp	r5, #0
	ble.n	sub_08154f28
	lsls	r1, r0, #10
	movs	r3, #224
	mov	r0, r8
	ldr	r2, [r0, #0]
	lsls	r3, r3, #3
	movs	r0, #32
	add	r1, sl
	str	r0, [sp, #0]
	movs	r4, #1
	adds	r1, r1, r3
	str	r5, [sp, #4]
	movs	r3, #7
	ands	r4, r6
	ands	r3, r6
	mov	r0, fp
	lsls	r4, r4, #2
	subs	r3, r3, r5
	ldr	r4, [r4, r0]
	subs	r2, #16
	adds	r3, #104
	ldr	r0, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x211c
	adds	r6, #1
	add	r8, r1
	cmp	r6, #16
	bne.n	.L_08154ea8
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r7, #1
	bl	sub_08013560
	cmp	r7, #70
	bne.n	.L_08154e6c
	ldr	r0, [pc, #40]
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
	movs	r0, r0
	.4byte 0x08198422
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #52]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	ldr	r1, [r5, #96]
	mov	r9, r0
	movs	r0, #1
	str	r1, [sp, #48]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	movs	r1, #224
	adds	r2, #2
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	movs	r2, #1
	movs	r3, #1
	add	r1, r9
	bl	sub_08157cf4
	ldr	r2, [sp, #52]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08154ff8
	movs	r2, #128
	ldr	r3, [pc, #20]
	lsls	r2, r2, #19
	b.n	.L_08154ff4
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00001010
	.4byte 0x0000013f
	.2byte 0x9000
	.2byte 0xffff
.L_08154ff4:
	.2byte 0x3228
	str	r3, [r2, #0]
.L_08154ff8:
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #36]
	bl	sub_081963ec
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #40]
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #156]
	bl	sub_080145a8
	ldr	r4, [sp, #52]
	ldr	r2, [pc, #152]
	ldr	r3, [r4, #24]
	movs	r6, #0
	ldrb	r3, [r2, r3]
	movs	r1, #1
	lsls	r3, r3, #3
	adds	r3, #56
	str	r3, [sp, #32]
	ldr	r3, [pc, #140]
	movs	r2, #128
	mov	fp, r6
	negs	r1, r1
	lsls	r2, r2, #3
.L_08155050:
	movs	r0, #1
	add	fp, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	fp, r2
	bne.n	.L_08155050
	ldr	r2, [sp, #32]
	movs	r1, #0
	str	r1, [sp, #44]
	cmp	r2, #0
	bne.n	.L_08155068
	b.n	.L_0815533c
.L_08155068:
	ldr	r3, [sp, #32]
	subs	r2, #64
	subs	r3, #16
	str	r2, [sp, #28]
	str	r3, [sp, #24]
.L_08155072:
	ldr	r4, [sp, #44]
	ldr	r6, [sp, #28]
	cmp	r4, r6
	bne.n	.L_08155080
	movs	r0, #133
	bl	sub_081180e8
.L_08155080:
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #24]
	cmp	r0, r1
	blt.n	.L_081550a4
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r2, [sp, #32]
	movs	r1, #128
	subs	r3, r2, r0
	ldr	r2, [pc, #44]
	lsls	r1, r1, #19
	subs	r3, #1
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_081550a4:
	ldr	r4, [sp, #52]
	movs	r3, #0
	mov	fp, r3
	ldr	r2, [r4, #24]
	ldr	r3, [pc, #32]
	ldrb	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_081550b6
	b.n	.L_08155276
.L_081550b6:
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #44]
	movs	r6, #12
	movs	r2, #8
	str	r6, [sp, #20]
	b.n	.L_081550dc
	movs	r0, r0
	.4byte 0x00003f44
	.4byte 0x00001000
	.4byte 0x08143001
	.4byte 0x08198442
	.4byte 0x02010018
	.2byte 0x843a
	.2byte 0x0819
.L_081550dc:
	str	r0, [sp, #16]
	str	r2, [sp, #12]
	subs	r1, #8
	mov	sl, r1
.L_081550e4:
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #12]
	cmp	r3, r4
	bgt.n	.L_081550ee
	b.n	.L_08155212
.L_081550ee:
	ldr	r3, [pc, #624]
	mov	r6, fp
	ldrb	r2, [r3, r6]
	adds	r3, r2, #0
	cmp	r3, #1
	bhi.n	.L_08155148
	mov	r1, sl
	lsls	r3, r1, #1
	lsls	r0, r1, #4
	add	r3, sl
	lsls	r1, r3, #1
	cmp	r0, #80
	ble.n	.L_0815510a
	movs	r0, #80
.L_0815510a:
	cmp	r1, #30
	ble.n	.L_08155110
	movs	r1, #30
.L_08155110:
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08155126
	ldr	r3, [sp, #16]
	movs	r4, #108
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r6, [sp, #40]
	subs	r2, r2, r1
	b.n	.L_08155132
.L_08155126:
	ldr	r3, [sp, #16]
	movs	r4, #108
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r6, [sp, #36]
	adds	r2, r2, r1
.L_08155132:
	movs	r1, #48
	str	r1, [sp, #0]
	movs	r1, #224
	lsls	r1, r1, #3
	subs	r3, r4, r0
	str	r0, [sp, #4]
	add	r1, r9
	ldr	r0, [sp, #48]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe02d
.L_08155148:
	mov	r1, sl
	lsls	r0, r1, #3
	cmp	r0, #64
	ble.n	.L_08155152
	movs	r0, #64
.L_08155152:
	mov	r3, sl
	cmp	r3, #8
	ble.n	.L_0815515a
	movs	r1, #8
.L_0815515a:
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08155184
	ldr	r4, [sp, #16]
	movs	r6, #108
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	subs	r3, r6, r0
	subs	r2, r2, r1
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #176
	lsls	r1, r1, #5
	str	r0, [sp, #4]
	add	r1, r9
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00f
.L_08155184:
	ldr	r6, [sp, #16]
	ldr	r4, [sp, #36]
	movs	r2, #0
	ldrsb	r2, [r6, r2]
	str	r0, [sp, #4]
	adds	r2, r2, r1
	movs	r1, #108
	subs	r3, r1, r0
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #176
	lsls	r1, r1, #5
	ldr	r0, [sp, #48]
	add	r1, r9
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b03
	ldr	r6, [sp, #44]
	adds	r3, #1
	cmp	r6, r3
	bne.n	.L_081551ba
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #3
	str	r3, [r2, #0]
.L_081551ba:
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #44]
	adds	r3, #3
	cmp	r0, r3
	bge.n	.L_08155212
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	ldr	r7, [pc, #404]
	adds	r1, r3, #0
	adds	r1, #72
	movs	r6, #0
	b.n	.L_081551dc
.L_081551d6:
	adds	r7, r5, #0
	adds	r7, #28
	adds	r6, #1
.L_081551dc:
	cmp	r6, #64
	beq.n	.L_08155212
	adds	r5, r7, #0
	ldr	r3, [r5, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_081551d6
	str	r1, [sp, #8]
	bl	sub_08014878
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #8]
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r3, #31
	ands	r3, r0
	adds	r2, r2, r3
	adds	r2, #32
	str	r2, [r5, #0]
	cmp	r2, #96
	ble.n	.L_0815520c
	movs	r3, #96
	str	r3, [r5, #0]
.L_0815520c:
	movs	r3, #0
	str	r1, [r7, #4]
	str	r3, [r7, #24]
.L_08155212:
	ldr	r4, [sp, #52]
	movs	r6, #0
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq.n	.L_0815524c
	ldr	r0, [sp, #20]
	movs	r5, #36
	mov	r8, r0
.L_08155222:
	ldr	r1, [sp, #44]
	cmp	r1, r8
	bne.n	.L_08155244
	movs	r0, #133
	bl	sub_081c0010
	ldr	r2, [sp, #52]
	movs	r1, #7
	ldrsh	r0, [r5, r2]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r6, #0
	bl	sub_0814cd48
	ldr	r4, [sp, #52]
	ldr	r3, [r4, #20]
.L_08155244:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	bne.n	.L_08155222
.L_0815524c:
	ldr	r6, [sp, #20]
	ldr	r0, [sp, #16]
	ldr	r2, [sp, #12]
	adds	r6, #8
	adds	r2, #8
	adds	r0, #1
	str	r6, [sp, #20]
	str	r0, [sp, #16]
	str	r2, [sp, #12]
	ldr	r4, [sp, #52]
	movs	r3, #1
	add	fp, r3
	ldr	r2, [r4, #24]
	ldr	r3, [pc, #256]
	movs	r1, #8
	ldrb	r3, [r3, r2]
	negs	r1, r1
	add	sl, r1
	cmp	fp, r3
	beq.n	.L_08155276
	b.n	.L_081550e4
.L_08155276:
	ldr	r0, [pc, #244]
	ldr	r7, [pc, #232]
	movs	r6, #0
	mov	fp, r6
	mov	sl, r0
.L_08155280:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_08155304
	lsrs	r5, r3, #31
	ldr	r2, [pc, #228]
	adds	r5, r3, r5
	asrs	r5, r5, #1
	lsls	r1, r5, #1
	mov	r8, r1
	ldrh	r1, [r2, r1]
	movs	r3, #224
	lsls	r3, r3, #3
	add	r1, r9
	adds	r1, r1, r3
	mov	r3, sl
	ldrb	r0, [r3, r5]
	ldr	r4, [pc, #208]
	lsls	r0, r0, #24
	ldrsb	r6, [r4, r5]
	ldr	r2, [r7, #0]
	asrs	r4, r0, #24
	ldr	r3, [r7, #4]
	lsrs	r0, r0, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r6
	subs	r3, r3, r0
	str	r6, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4e2b
	mov	r0, r8
	mov	r3, sl
	ldrh	r1, [r6, r0]
	ldrb	r0, [r3, r5]
	ldr	r6, [pc, #164]
	lsls	r0, r0, #24
	asrs	r4, r0, #24
	ldr	r3, [r7, #4]
	lsrs	r0, r0, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r3, r3, r0
	ldrsb	r0, [r6, r5]
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, r9
	adds	r1, r1, r2
	ldr	r2, [r7, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69bb
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #14
	bne.n	.L_08155304
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_08155304:
	movs	r6, #1
	add	fp, r6
	mov	r0, fp
	adds	r7, #28
	cmp	r0, #64
	bne.n	.L_08155280
	movs	r1, #8
	movs	r0, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #32]
	adds	r1, #1
	str	r1, [sp, #44]
	cmp	r1, r2
	beq.n	.L_0815533c
	b.n	.L_08155072
.L_0815533c:
	ldr	r0, [pc, #56]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08198432
	.4byte 0x02010000
	.4byte 0x08198442
	.4byte 0x0819844c
	.4byte 0x08198454
	.4byte 0x08198445
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
	sub	sp, #64
	str	r0, [sp, #48]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #44]
	ldr	r2, [r5, #100]
	str	r2, [sp, #24]
	bl	sub_081435e0
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [sp, #48]
	ldr	r5, [r5, #104]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #616]
	movs	r2, #1
	movs	r3, #0
	str	r5, [sp, #28]
	bl	sub_08157cf4
	movs	r3, #152
	ldr	r2, [sp, #48]
	lsls	r3, r3, #5
	adds	r3, #86
	adds	r1, r2, r3
	ldr	r0, [pc, #596]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #24]
	movs	r3, #0
	ldr	r0, [pc, #584]
	bl	sub_08157cf4
	ldr	r3, [pc, #584]
	movs	r4, #0
	movs	r2, #128
	mov	r9, r4
	movs	r1, #0
	lsls	r2, r2, #3
.L_081553ee:
	movs	r0, #1
	add	r9, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_081553ee
	ldr	r3, [sp, #48]
	movs	r1, #0
	movs	r2, #1
	mov	r9, r1
	negs	r2, r2
	adds	r3, #24
.L_08155406:
	movs	r4, #1
	add	r9, r4
	mov	r0, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #64
	bne.n	.L_08155406
	ldr	r1, [sp, #48]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r1, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #508]
	bl	sub_080145a8
	movs	r0, #138
	bl	sub_081c0010
	movs	r0, #0
	str	r0, [sp, #36]
	mov	r1, fp
	ldr	r2, [r1, #20]
	movs	r4, #40
	lsls	r3, r2, #3
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_08155450
	b.n	.L_08155814
.L_08155450:
	ldr	r0, [sp, #36]
	cmp	r0, #24
	bne.n	.L_08155460
	movs	r0, #133
	bl	sub_081180e8
	mov	r1, fp
	ldr	r2, [r1, #20]
.L_08155460:
	movs	r3, #0
	str	r3, [sp, #40]
	cmp	r2, #0
	beq.n	.L_0815548e
	ldr	r5, [pc, #456]
.L_0815546a:
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	lsls	r3, r4, #3
	cmp	r0, r3
	bne.n	.L_08155484
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #444]
	ldr	r0, [sp, #44]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4659
	ldr	r2, [r1, #20]
.L_08155484:
	ldr	r3, [sp, #40]
	adds	r3, #1
	str	r3, [sp, #40]
	cmp	r3, r2
	bne.n	.L_0815546a
.L_0815548e:
	movs	r4, #0
	str	r4, [sp, #40]
	cmp	r2, #0
	bne.n	.L_08155498
	b.n	.L_0815575e
.L_08155498:
	mov	r0, sp
	adds	r0, #52
	movs	r1, #36
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r4, [sp, #12]
.L_081554a4:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #40]
	mov	r1, fp
	ldrsh	r0, [r3, r1]
	lsls	r2, r2, #3
	ldr	r1, [sp, #20]
	mov	r8, r2
	bl	sub_0815e21c
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #20]
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r4, #0]
	ldr	r0, [sp, #36]
	mov	r3, r8
	adds	r3, #1
	cmp	r0, r3
	bne.n	.L_081554dc
	ldr	r1, [sp, #48]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	adds	r2, r1, r3
	movs	r3, #4
	str	r3, [r2, #0]
.L_081554dc:
	ldr	r4, [sp, #36]
	mov	r3, r8
	adds	r3, #4
	cmp	r4, r3
	bne.n	.L_08155506
	ldr	r1, [sp, #16]
	mov	r3, fp
	ldrsh	r0, [r1, r3]
	movs	r3, #6
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	ldr	r3, [sp, #40]
	bl	sub_0814cd48
	ldr	r4, [sp, #16]
	mov	r2, fp
	ldrsh	r0, [r4, r2]
	movs	r1, #6
	bl	sub_08118088
.L_08155506:
	ldr	r4, [sp, #36]
	movs	r3, #2
	add	r3, r8
	mov	sl, r3
	cmp	r4, r8
	bge.n	.L_08155514
	b.n	.L_0815564e
.L_08155514:
	mov	r3, r8
	adds	r3, #16
	cmp	r4, r3
	blt.n	.L_0815551e
	b.n	.L_08155648
.L_0815551e:
	mov	r0, r8
	subs	r3, r4, r0
	lsls	r5, r3, #6
	cmp	r5, #104
	ble.n	.L_0815552a
	movs	r5, #104
.L_0815552a:
	mov	r3, fp
	ldr	r2, [r3, #24]
	ldr	r6, [pc, #268]
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_08155594
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	mov	sl, r6
	adds	r7, r4, r0
.L_08155546:
	mov	r1, r9
	adds	r3, r7, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r1, r2, #4
	ldr	r4, [sp, #20]
	subs	r1, r1, r2
	ldr	r2, [sp, #48]
	lsls	r1, r1, #6
	movs	r3, #152
	adds	r1, r2, r1
	lsls	r3, r3, #5
	ldr	r2, [r4, #0]
	adds	r3, #86
	adds	r1, r1, r3
	movs	r3, #24
	subs	r2, #12
	str	r3, [sp, #0]
	ldr	r4, [sp, #28]
	movs	r3, #0
	str	r5, [sp, #4]
	ldr	r0, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4659
	ldr	r2, [r1, #24]
	mov	r4, sl
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r4, r3]
	movs	r0, #1
	add	r9, r0
	cmp	r9, r3
	bne.n	.L_08155546
.L_08155594:
	movs	r0, #2
	ldr	r1, [sp, #36]
	add	r0, r8
	mov	sl, r0
	cmp	r1, sl
	bne.n	.L_0815564e
	movs	r3, #0
	mov	r9, r3
	lsls	r3, r2, #2
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_0815564e
	ldr	r4, [sp, #12]
	ldr	r0, [pc, #144]
	adds	r7, r4, r0
.L_081555b2:
	bl	sub_08014878
	movs	r6, #128
	lsls	r6, r6, #1
	adds	r6, #255
	ands	r6, r0
	bl	sub_08014878
	ldr	r2, [sp, #20]
	movs	r5, #254
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #120]
	lsls	r5, r5, #7
	lsls	r3, r3, #16
	adds	r5, #255
	str	r3, [r7, #0]
	ands	r5, r0
	movs	r3, #208
	adds	r5, r5, r1
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #64
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
	mov	r4, fp
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r4, #24]
	ldr	r0, [pc, #40]
	lsls	r3, r3, #2
	ldrb	r3, [r0, r3]
	adds	r7, #28
	cmp	r9, r3
	bne.n	.L_081555b2
	b.n	.L_0815564e
	movs	r0, r0
	.4byte 0x00000192
	.4byte 0x00000188
	.4byte 0x00000134
	.4byte 0x02010018
	.4byte 0x08143001
	.4byte 0x03000260
	.4byte 0x10101010
	.4byte 0x08198462
	.4byte 0x02010000
	.2byte 0xc000
	.2byte 0xffff
.L_08155648:
	.2byte 0x2102
	add	r1, r8
	mov	sl, r1
.L_0815564e:
	ldr	r2, [sp, #36]
	cmp	r2, sl
	blt.n	.L_0815573e
	mov	r3, r8
	adds	r3, #24
	cmp	r2, r3
	bge.n	.L_0815573e
	movs	r3, #0
	mov	r4, fp
	mov	r9, r3
	ldr	r3, [r4, #24]
	ldr	r0, [pc, #460]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_0815573e
	ldr	r7, [sp, #20]
.L_08155672:
	movs	r1, #3
	mov	r4, r9
	ands	r4, r1
	str	r4, [sp, #8]
	bl	sub_08014878
	mov	r2, fp
	ldr	r3, [r2, #24]
	ldr	r1, [pc, #432]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrb	r5, [r1, r3]
	adds	r1, r5, #0
	bl	sub_0800206c
	ldr	r2, [r7, #4]
	ldr	r4, [sp, #8]
	mov	r8, r2
	mov	r3, r8
	subs	r3, r3, r0
	subs	r5, r5, r0
	ldr	r0, [pc, #408]
	mov	r8, r3
	ldrb	r3, [r0, r4]
	mov	r1, r8
	lsrs	r3, r3, #1
	subs	r1, r1, r3
	movs	r2, #8
	adds	r5, #1
	mov	r8, r1
	add	r8, r2
	mov	sl, r0
	bl	sub_08014878
	adds	r1, r5, #0
	bl	sub_0800206c
	ldr	r6, [r7, #0]
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	adds	r6, r6, r0
	asrs	r5, r5, #1
	ldr	r4, [sp, #8]
	subs	r6, r6, r5
	ldr	r5, [pc, #368]
	ldrb	r3, [r5, r4]
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	bl	sub_08014878
	ldr	r3, [pc, #360]
	movs	r1, #3
	ands	r0, r1
	ldrb	r2, [r3, r0]
	mov	r0, fp
	movs	r3, #3
	orrs	r3, r2
	ldr	r1, [pc, #348]
	ldr	r2, [r0, #24]
	movs	r0, #188
	ldrb	r2, [r1, r2]
	movs	r1, #7
	str	r2, [sp, #0]
	movs	r2, #7
	bl	sub_08196404
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #332]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #48]
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldrb	r3, [r5, r4]
	mov	r0, sl
	str	r3, [sp, #0]
	movs	r2, #192
	ldrb	r3, [r0, r4]
	lsls	r2, r2, #18
	str	r3, [sp, #4]
	adds	r2, #188
	ldr	r4, [r2, #0]
	mov	r3, r8
	ldr	r0, [sp, #44]
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r3, #1
	mov	r4, fp
	add	r9, r3
	ldr	r3, [r4, #24]
	ldr	r0, [pc, #256]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r3, [r0, r3]
	cmp	r9, r3
	bne.n	.L_08155672
.L_0815573e:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #40]
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r2, r2, r3
	adds	r1, #2
	adds	r4, #1
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #40]
	mov	r0, fp
	ldr	r3, [r0, #20]
	cmp	r4, r3
	beq.n	.L_0815575e
	b.n	.L_081554a4
.L_0815575e:
	ldr	r6, [pc, #236]
	movs	r1, #0
	mov	r9, r1
.L_08155764:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	sub_081557d0
	subs	r3, #1
	movs	r2, #128
	str	r3, [r6, #24]
	lsls	r2, r2, #5
	adds	r0, r6, #0
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r6, #4]
	movs	r2, #208
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_08155792
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	sub_081557d0
.L_08155792:
	ldr	r2, [r6, #0]
	ldr	r4, [pc, #184]
	cmp	r2, r4
	bhi.n	sub_081557d0
	cmp	r3, #0
	blt.n	sub_081557d0
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bge.n	.L_081557a6
	adds	r4, #15
.L_081557a6:
	asrs	r4, r4, #4
	adds	r4, #1
	ldr	r0, [pc, #168]
	lsls	r5, r4, #1
	subs	r1, r5, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #24]
	asrs	r2, r2, #16
	adds	r1, r0, r1
	lsrs	r0, r4, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	asrs	r3, r3, #16
	subs	r2, r2, r0
	subs	r3, r3, r4
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	movs	r1, #128
	add	r9, r0
	lsls	r1, r1, #3
	adds	r6, #28
	cmp	r9, r1
	bne.n	.L_08155764
	movs	r1, #8
	movs	r0, #2
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #36]
	mov	r1, fp
	adds	r0, #1
	str	r0, [sp, #36]
	ldr	r3, [r1, #20]
	adds	r2, r3, #0
	lsls	r3, r2, #3
	adds	r3, #40
	cmp	r0, r3
	beq.n	.L_08155814
	b.n	.L_08155450
.L_08155814:
	ldr	r0, [pc, #64]
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
	.4byte 0x08198462
	.4byte 0x08197498
	.4byte 0x08197492
	.4byte 0x0819846e
	.4byte 0x08198472
	.4byte 0x08197486
	.4byte 0x02010000
	.4byte 0x007effff
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
.L_0815585c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #120
	str	r3, [sp, #48]
	str	r0, [sp, #56]
	str	r2, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	lsrs	r6, r1, #4
	str	r0, [sp, #44]
	movs	r5, #7
	ldr	r2, [r3, #96]
	ands	r5, r1
	str	r2, [sp, #40]
	lsrs	r1, r1, #5
	ldr	r3, [r3, #100]
	str	r3, [sp, #28]
	movs	r3, #1
	ands	r6, r3
	movs	r3, #15
	ands	r1, r3
	movs	r3, #40
	str	r1, [sp, #20]
	str	r3, [sp, #24]
	cmp	r1, #6
	beq.n	.L_081558ec
	ldr	r4, [sp, #20]
	cmp	r4, #8
	bne.n	.L_081558c0
	ldr	r7, [sp, #44]
	movs	r0, #238
	lsls	r0, r0, #7
	movs	r1, #238
	adds	r0, #180
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #24
	adds	r1, #184
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #54
	b.n	.L_081558ea
.L_081558c0:
	ldr	r3, [sp, #44]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #180
	adds	r2, r3, r4
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r7, [sp, #44]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #184
	adds	r2, r7, r0
	movs	r3, #0
	str	r3, [r2, #0]
	cmp	r5, #3
	ble.n	.L_081558e8
	movs	r1, #84
	subs	r5, #4
	str	r1, [sp, #24]
	b.n	.L_081558ec
.L_081558e8:
	movs	r2, #55
.L_081558ea:
	str	r2, [sp, #24]
.L_081558ec:
	cmp	r5, #1
	beq.n	.L_08155904
	cmp	r5, #1
	bgt.n	.L_081558fa
	cmp	r5, #0
	beq.n	.L_08155900
	b.n	.L_0815590c
.L_081558fa:
	cmp	r5, #2
	beq.n	.L_08155908
	b.n	.L_0815590c
.L_08155900:
	ldr	r0, [pc, #580]
	b.n	.L_0815590e
.L_08155904:
	ldr	r0, [pc, #580]
	b.n	.L_0815590e
.L_08155908:
	ldr	r0, [pc, #580]
	b.n	.L_0815590e
.L_0815590c:
	ldr	r0, [pc, #580]
.L_0815590e:
	bl	sub_08013300
	adds	r2, r0, #0
	movs	r0, #160
	adds	r1, r2, #0
	ldr	r3, [pc, #572]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2d01
	beq.n	.L_0815593a
	cmp	r5, #1
	bgt.n	.L_08155930
	cmp	r5, #0
	beq.n	.L_08155936
	b.n	.L_08155942
.L_08155930:
	cmp	r5, #2
	beq.n	.L_0815593e
	b.n	.L_08155942
.L_08155936:
	ldr	r0, [pc, #548]
	b.n	.L_08155944
.L_0815593a:
	ldr	r0, [pc, #528]
	b.n	.L_08155944
.L_0815593e:
	ldr	r0, [pc, #528]
	b.n	.L_08155944
.L_08155942:
	ldr	r0, [pc, #528]
.L_08155944:
	bl	sub_08013300
	adds	r2, r0, #0
	ldr	r3, [sp, #44]
	movs	r4, #224
	adds	r2, #128
	lsls	r4, r4, #3
	adds	r1, r3, r4
	adds	r0, r2, #0
	bl	sub_0801587c
	ldr	r0, [pc, #516]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	cmp	r6, #1
	bne.n	.L_0815597c
	movs	r1, #7
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #23
	bl	sub_081963ec
	b.n	.L_0815598c
.L_0815597c:
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #19
	bl	sub_081963ec
.L_0815598c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [sp, #56]
	ldr	r5, [r3, #104]
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r0, [r7, #8]
	str	r5, [sp, #32]
	str	r3, [sp, #36]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	mov	r9, r0
	movs	r1, #36
	ldrsh	r0, [r7, r1]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r7, [sp, #44]
	movs	r2, #0
	mov	fp, r0
	mov	r8, r2
	mov	sl, r2
.L_081559ba:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	mov	r3, sl
	str	r3, [r7, #0]
	movs	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #20
	lsls	r3, r3, #16
	mov	r4, sl
	str	r3, [r7, #4]
	str	r4, [r7, #8]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, sl
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	str	r0, [r7, #16]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #1
	asrs	r3, r3, #5
	add	r8, r2
	str	r3, [r7, #20]
	mov	r1, sl
	mov	r3, r8
	str	r1, [r7, #24]
	adds	r7, #28
	cmp	r3, #64
	bne.n	.L_081559ba
	ldr	r4, [sp, #44]
	movs	r5, #239
	movs	r7, #238
	lsls	r5, r5, #7
	lsls	r7, r7, #7
	adds	r2, r4, r5
	movs	r3, #2
	adds	r7, #132
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r4, r7
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #300]
	bl	sub_080145a8
	mov	r0, sp
	adds	r0, #108
	str	r0, [sp, #16]
	mov	r1, r9
	ldr	r3, [r1, #8]
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	ldr	r3, [r1, #16]
	str	r3, [r0, #8]
	ldr	r2, [sp, #20]
	cmp	r2, #10
	bls.n	.L_08155a56
	b.n	.L_08155b6c
.L_08155a56:
	lsls	r3, r2, #2
	ldr	r2, [pc, #268]
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	ldrh	r4, [r1, r2]
	lsrs	r5, r2, #32
	ldrh	r0, [r5, r2]
	lsrs	r5, r2, #32
	ldrh	r6, [r1, r3]
	lsrs	r5, r2, #32
	ldrh	r4, [r4, r3]
	lsrs	r5, r2, #32
	ldrh	r2, [r6, r3]
	lsrs	r5, r2, #32
	ldrh	r2, [r3, r4]
	lsrs	r5, r2, #32
	ldrh	r2, [r6, r3]
	lsrs	r5, r2, #32
	ldrh	r6, [r0, r4]
	lsrs	r5, r2, #32
	ldrh	r4, [r4, r2]
	lsrs	r5, r2, #32
	ldrh	r6, [r6, r2]
	lsrs	r5, r2, #32
	ldrh	r0, [r6, r4]
	lsrs	r5, r2, #32
	add	r3, sp, #96
	mov	r4, fp
	mov	sl, r3
	ldr	r3, [r4, #8]
	mov	r5, sl
	str	r3, [r5, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	ldr	r3, [r4, #16]
	str	r3, [r5, #8]
	b.n	.L_08155b70
	mov	r0, fp
	b.n	.L_08155ad0
	mov	r2, fp
	ldr	r3, [r2, #8]
	add	r1, sp, #96
	str	r3, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	b.n	.L_08155b3c
	add	r3, sp, #96
	mov	r4, fp
	mov	sl, r3
	ldr	r3, [r4, #8]
	mov	r5, sl
	str	r3, [r5, #0]
	movs	r3, #240
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	b.n	.L_08155b70
	mov	r0, r9
.L_08155ad0:
	ldr	r3, [r0, #8]
	add	r7, sp, #96
	str	r3, [r7, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r7, #4]
	mov	sl, r7
	ldr	r3, [r0, #16]
	str	r3, [r7, #8]
	b.n	.L_08155b70
	mov	r2, r9
	ldr	r3, [r2, #8]
	add	r1, sp, #96
	str	r3, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	b.n	.L_08155b3c
	add	r3, sp, #96
	movs	r2, #0
	str	r2, [r3, #0]
	mov	sl, r3
	movs	r3, #240
	mov	r4, sl
	lsls	r3, r3, #14
	str	r3, [r4, #4]
	str	r2, [r4, #8]
	b.n	.L_08155b70
	movs	r5, #96
	movs	r3, #160
	add	r5, sp
	movs	r2, #0
	lsls	r3, r3, #13
	mov	sl, r5
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	b.n	.L_08155b70
	mov	r0, r9
	ldr	r3, [r0, #8]
	add	r7, sp, #96
	str	r3, [r7, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	movs	r3, #0
	mov	sl, r7
	str	r3, [r7, #8]
	b.n	.L_08155b70
	mov	r2, r9
	ldr	r3, [r2, #8]
	add	r1, sp, #96
	str	r3, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #13
.L_08155b3c:
	str	r3, [r1, #4]
	movs	r3, #0
	mov	sl, r1
	str	r3, [r1, #8]
	b.n	.L_08155b70
	movs	r0, r0
	.4byte 0x00000184
	.4byte 0x00000155
	.4byte 0x00000151
	.4byte 0x00000153
	.4byte 0x03000730
	.4byte 0x00000157
	.4byte 0x00000134
	.4byte 0x08143001
	.2byte 0x5a60
	.2byte 0x0815
.L_08155b6c:
	add	r3, sp, #96
	mov	sl, r3
.L_08155b70:
	mov	r4, sp
	adds	r4, #84
	ldr	r7, [sp, #16]
	str	r4, [sp, #12]
	mov	r5, sl
	ldr	r3, [r7, #0]
	ldr	r0, [r5, #0]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_08002054
	ldr	r1, [sp, #12]
	str	r0, [r1, #0]
	ldr	r3, [r7, #4]
	ldr	r0, [r5, #4]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_08002054
	ldr	r2, [sp, #12]
	movs	r1, #40
	str	r0, [r2, #4]
	ldr	r3, [r7, #8]
	ldr	r0, [r5, #8]
	subs	r0, r0, r3
	bl	sub_08002054
	ldr	r3, [sp, #12]
	movs	r4, #0
	str	r0, [r3, #8]
	ldr	r5, [sp, #24]
	mov	r9, r4
	cmp	r5, #0
	bne.n	.L_08155bb6
	b.n	.L_08155f3c
.L_08155bb6:
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r7, r9
	ldr	r5, [r3, #48]
	cmp	r7, #75
	ble.n	.L_08155bd4
	ldr	r3, [pc, #40]
	lsls	r2, r7, #1
	subs	r3, r3, r2
	ldr	r2, [pc, #36]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_08155bd4:
	ldr	r0, [sp, #20]
	cmp	r0, #6
	bne.n	.L_08155bf4
	mov	r1, r9
	cmp	r1, #23
	ble.n	.L_08155bf4
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_08164abc
	b.n	.L_08155bf4
	.4byte 0x000000a8
	.2byte 0x1000
	.2byte 0x0000
.L_08155bf4:
	mov	r2, r9
	cmp	r2, #8
	bne.n	.L_08155c00
	movs	r0, #212
	bl	sub_081c0010
.L_08155c00:
	bl	sub_08014de4
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080156e8
	mov	r3, r9
	subs	r3, #6
	cmp	r3, #39
	bhi.n	.L_08155c32
	ldr	r4, [sp, #16]
	ldr	r5, [sp, #12]
	ldr	r3, [r4, #0]
	ldr	r2, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r4, #0]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r4, #4]
	ldr	r3, [r4, #8]
	ldr	r2, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r4, #8]
.L_08155c32:
	ldr	r0, [sp, #16]
	mov	r7, r9
	bl	sub_08015128
	cmp	r7, #0
	bne.n	.L_08155c52
	ldr	r1, [sp, #56]
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [r1, #8]
	negs	r2, r2
	movs	r1, #7
	negs	r3, r3
	str	r7, [sp, #0]
	bl	sub_0814cd48
.L_08155c52:
	mov	r2, r9
	cmp	r2, #24
	bne.n	.L_08155c6c
	ldr	r3, [sp, #56]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_0814cd48
.L_08155c6c:
	mov	r4, r9
	cmp	r4, #63
	ble.n	.L_08155c74
	b.n	.L_08155dee
.L_08155c74:
	negs	r4, r4
	mov	r0, r9
	str	r4, [sp, #8]
	ldr	r6, [sp, #44]
	movs	r5, #0
	lsls	r0, r0, #8
	mov	r8, r5
	lsls	r7, r4, #8
	mov	fp, r0
.L_08155c86:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08155c8e
	adds	r3, #7
.L_08155c8e:
	asrs	r3, r3, #3
	cmp	r9, r3
	bge.n	.L_08155c96
	b.n	.L_08155dd4
.L_08155c96:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L_08155c9e
	b.n	.L_08155dd4
.L_08155c9e:
	bl	sub_08014e38
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #1
	beq.n	.L_08155cc8
	cmp	r3, #1
	bgt.n	.L_08155cb6
	cmp	r3, #0
	beq.n	.L_08155cc0
	b.n	.L_08155ce4
.L_08155cb6:
	cmp	r3, #2
	beq.n	.L_08155cd0
	cmp	r3, #3
	beq.n	.L_08155cd8
	b.n	.L_08155ce4
.L_08155cc0:
	mov	r0, fp
	bl	sub_08015068
	b.n	.L_08155ce4
.L_08155cc8:
	adds	r0, r7, #0
	bl	sub_08015024
	b.n	.L_08155ce4
.L_08155cd0:
	adds	r0, r7, #0
	bl	sub_080150e4
	b.n	.L_08155ce4
.L_08155cd8:
	adds	r0, r7, #0
	bl	sub_08015024
	adds	r0, r7, #0
	bl	sub_080150e4
.L_08155ce4:
	add	r5, sp, #60
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0815e1ec
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08014ea8
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L_08155d04
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L_08155d04:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #122
	cmp	r2, r3
	ble.n	.L_08155d12
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_08155d12:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L_08155d1c
	adds	r3, #63
.L_08155d1c:
	asrs	r3, r3, #6
	movs	r0, #8
	subs	r0, r0, r3
	ldr	r2, [pc, #604]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	lsrs	r3, r0, #31
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08155d5a
	adds	r3, #7
.L_08155d5a:
	asrs	r3, r3, #3
	adds	r3, #24
	cmp	r9, r3
	blt.n	.L_08155dd4
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #4]
	negs	r3, r3
	asrs	r5, r3, #7
	ldr	r3, [r6, #8]
	negs	r2, r2
	negs	r3, r3
	asrs	r4, r3, #7
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #12]
	asrs	r2, r2, #7
	adds	r2, r3, r2
	ldr	r3, [r6, #20]
	adds	r1, r1, r5
	adds	r0, r3, r4
	lsls	r3, r1, #5
	subs	r3, r3, r1
	lsls	r3, r3, #1
	str	r1, [r6, #12]
	str	r2, [r6, #16]
	str	r0, [r6, #20]
	cmp	r3, #0
	bge.n	.L_08155d92
	adds	r3, #63
.L_08155d92:
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_08155da2
	adds	r3, #63
.L_08155da2:
	asrs	r3, r3, #6
	str	r3, [r6, #16]
	lsls	r3, r0, #5
	subs	r3, r3, r0
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_08155db2
	adds	r3, #63
.L_08155db2:
	movs	r0, #224
	lsls	r0, r0, #3
	movs	r2, #240
	asrs	r3, r3, #6
	adds	r0, #255
	lsls	r2, r2, #4
	str	r3, [r6, #20]
	adds	r2, #254
	adds	r3, r5, r0
	cmp	r3, r2
	bhi.n	.L_08155dd4
	adds	r3, r4, r0
	cmp	r3, r2
	bhi.n	.L_08155dd4
	movs	r1, #1
	negs	r1, r1
	str	r1, [r6, #24]
.L_08155dd4:
	ldr	r2, [sp, #8]
	movs	r5, #1
	lsls	r3, r2, #5
	mov	r4, r9
	add	r8, r5
	adds	r7, r7, r3
	mov	r0, r8
	lsls	r3, r4, #5
	add	fp, r3
	adds	r6, #28
	cmp	r0, #32
	beq.n	.L_08155dee
	b.n	.L_08155c86
.L_08155dee:
	mov	r3, r9
	subs	r3, #54
	cmp	r3, #15
	bhi.n	sub_08155e5a
	mov	r1, r9
	lsls	r5, r1, #10
	adds	r0, r5, #0
	bl	sub_08002096
	movs	r3, #0
	add	r6, sp, #72
	lsls	r0, r0, #2
	str	r0, [r6, #0]
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r4, [sp, #48]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	str	r3, [r4, #0]
	add	r5, sp, #60
	ldr	r3, [r2, #4]
	adds	r1, r5, #0
	str	r3, [r4, #4]
	adds	r0, r6, #0
	ldr	r3, [r2, #8]
	str	r3, [r4, #8]
	bl	sub_0815e1ec
	ldr	r2, [r5, #0]
	ldr	r7, [sp, #52]
	movs	r1, #20
	str	r2, [r7, #0]
	asrs	r2, r2, #1
	ldr	r3, [r5, #4]
	str	r3, [r7, #4]
	str	r2, [r5, #0]
	ldr	r4, [sp, #44]
	movs	r5, #224
	str	r1, [sp, #0]
	lsls	r5, r5, #3
	movs	r1, #40
	str	r1, [sp, #4]
	subs	r2, #10
	subs	r3, #20
	ldr	r0, [sp, #40]
	adds	r1, r4, r5
	ldr	r7, [sp, #32]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4648
	cmp	r0, #64
	bne.n	.L_08155ec0
	ldr	r7, [sp, #44]
	movs	r1, #0
	mov	r8, r1
.L_08155e66:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	ldr	r2, [sp, #52]
	movs	r5, #255
	ldr	r3, [r2, #0]
	ands	r5, r0
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	adds	r0, r6, #0
	ldr	r3, [r2, #4]
	adds	r5, #128
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
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #64
	bne.n	.L_08155e66
.L_08155ec0:
	mov	r5, r9
	cmp	r5, #63
	ble.n	.L_08155f1c
	ldr	r6, [pc, #184]
	ldr	r5, [sp, #44]
	movs	r7, #0
	mov	r8, r7
.L_08155ece:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08155f10
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #28]
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
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08155f10:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_08155ece
.L_08155f1c:
	ldr	r1, [sp, #44]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #24]
	movs	r4, #1
	add	r9, r4
	cmp	r9, r5
	beq.n	.L_08155f3c
	b.n	.L_08155bb6
.L_08155f3c:
	ldr	r0, [pc, #68]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r1, #128
	ldr	r5, [pc, #52]
	lsls	r1, r1, #7
	ldr	r0, [pc, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	ldr	r0, [sp, #40]
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4b05
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001010
	.4byte 0x08197410
	.4byte 0x08143001
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
	push	{lr}
	movs	r1, #0
	bl	sub_08156140
	pop	{pc}
	.2byte 0x0000
