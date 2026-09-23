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
	.set sub_08014de4, 0x08014de4
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_080156e8, 0x080156e8
	.set sub_08118070, 0x08118070
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_08178680, 0x08178680
	.set sub_0817b970, 0x0817b970
	.set sub_0817c4d2, 0x0817c4d2
	.set sub_0817ce58, 0x0817ce58
	.set sub_0817d5d4, 0x0817d5d4
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0817b9b8
	.thumb_func
Func_0817b9b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r0, [sp, #32]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	ldr	r1, [r5, #96]
	mov	r9, r0
	movs	r0, #0
	str	r1, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #139
	adds	r2, #82
	lsls	r1, r1, #7
	strh	r3, [r2, #0]
	ldr	r0, [pc, #52]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #36]
	add	r1, r9
	bl	sub_08157cf4
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r2, [r5, #104]
	movs	r3, #2
	str	r2, [sp, #16]
	movs	r2, #239
	lsls	r2, r2, #7
	b.n	.L_0817ba28
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000012e
	.2byte 0x0161
	.2byte 0x0000
.L_0817ba28:
	add	r2, r9
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #50
	add	r2, r9
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #824]
	bl	sub_080145a8
	ldr	r4, [sp, #32]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #32]
	mov	fp, r0
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r3, #128
	str	r0, [sp, #12]
	lsls	r3, r3, #4
	ldr	r5, [r5, #36]
	adds	r3, #102
	adds	r2, r5, r3
	movs	r4, #0
	movs	r3, #1
	str	r5, [sp, #8]
	strb	r3, [r2, #0]
	str	r4, [sp, #24]
.L_0817ba70:
	ldr	r1, [sp, #32]
	ldr	r0, [r1, #8]
	movs	r1, #3
	bl	sub_0817b970
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_0817baec
	ldr	r3, [sp, #32]
	add	r5, sp, #48
	ldr	r0, [r3, #8]
	adds	r1, r5, #0
	bl	sub_0815e21c
	movs	r4, #0
	mov	r8, r4
	mov	sl, r5
	mov	r7, r9
.L_0817ba94:
	movs	r3, #0
	str	r3, [r7, #24]
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r5, #255
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	subs	r3, #8
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
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r1, #1
	add	r8, r1
	asrs	r3, r3, #5
	mov	r2, r8
	str	r3, [r7, #16]
	adds	r7, #28
	cmp	r2, #32
	bne.n	.L_0817ba94
	movs	r0, #136
	bl	sub_081c0010
.L_0817baec:
	ldr	r3, [sp, #24]
	cmp	r3, #0
	blt.n	.L_0817bb84
	movs	r4, #0
	mov	r8, r4
	mov	r5, r9
.L_0817baf8:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0817bb00
	adds	r3, #7
.L_0817bb00:
	ldr	r0, [sp, #24]
	asrs	r3, r3, #3
	cmp	r0, r3
	blt.n	.L_0817bb50
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bgt.n	.L_0817bb50
	cmp	r3, #0
	bge.n	.L_0817bb14
	adds	r3, #3
.L_0817bb14:
	asrs	r3, r3, #2
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, r9
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	subs	r3, #24
	str	r0, [sp, #4]
	subs	r2, #12
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #48
	ldr	r2, [pc, #564]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817bb50:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #24
	bne.n	.L_0817baf8
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_0817bb84
	mov	r4, fp
	ldr	r3, [r4, #16]
	ldr	r0, [sp, #12]
	movs	r1, #100
	str	r3, [r0, #16]
	ldr	r3, [r4, #8]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #5
	bl	sub_08002054
	ldr	r1, [sp, #12]
	mov	r2, fp
	str	r0, [r1, #8]
	ldr	r4, [sp, #12]
	ldrh	r3, [r2, #6]
	strh	r3, [r4, #6]
.L_0817bb84:
	ldr	r0, [sp, #24]
	cmp	r0, #16
	bne.n	.L_0817bba2
	ldr	r1, [sp, #32]
	movs	r3, #120
	ldr	r0, [r1, #8]
	movs	r2, #36
	ldrsh	r1, [r1, r2]
	movs	r2, #16
	bl	sub_08157530
	ldr	r4, [sp, #12]
	movs	r3, #224
	lsls	r3, r3, #11
	str	r3, [r4, #40]
.L_0817bba2:
	ldr	r0, [sp, #24]
	cmp	r0, #32
	beq.n	.L_0817bbaa
	b.n	.L_0817bcb6
.L_0817bbaa:
	ldr	r0, [pc, #468]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #460]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4659
	ldr	r2, [sp, #12]
	ldr	r3, [r1, #16]
	movs	r7, #224
	str	r3, [r2, #16]
	lsls	r7, r7, #2
	ldr	r3, [r1, #8]
	movs	r1, #100
	lsls	r0, r3, #4
	subs	r0, r0, r3
	lsls	r0, r0, #3
	bl	sub_08002054
	ldr	r3, [sp, #12]
	mov	r4, fp
	str	r0, [r3, #8]
	movs	r1, #100
	ldr	r3, [r4, #8]
	add	r7, r9
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #4
	bl	sub_08002054
	mov	r1, fp
	str	r0, [r1, #8]
	ldrh	r3, [r1, #6]
	ldr	r2, [sp, #12]
	add	r5, sp, #36
	strh	r3, [r2, #6]
	ldr	r4, [sp, #32]
	adds	r1, r5, #0
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_0815e20c
	movs	r0, #0
	mov	r8, r0
	mov	sl, r5
.L_0817bc0c:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r6, #254
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r6, r6, #7
	ldr	r3, [r1, #4]
	adds	r6, #255
	subs	r3, #8
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r6, r0
	adds	r6, r6, r2
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #16
	bne.n	.L_0817bc0c
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
	ldr	r2, [sp, #32]
	mov	r3, r8
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r1, [sp, #32]
	movs	r3, #192
	lsls	r3, r3, #11
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	str	r3, [sp, #0]
	movs	r3, #10
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #160
	movs	r1, #1
	lsls	r2, r2, #12
	lsls	r3, r3, #13
	bl	sub_0815f000
.L_0817bcb6:
	ldr	r3, [sp, #24]
	subs	r3, #32
	cmp	r3, #23
	bhi.n	.L_0817bd22
	movs	r5, #224
	movs	r2, #0
	lsls	r5, r5, #2
	mov	r8, r2
	add	r5, r9
.L_0817bcc8:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0817bd16
	ldr	r0, [sp, #24]
	add	r0, r8
	cmp	r0, #0
	bge.n	.L_0817bcd8
	adds	r0, #3
.L_0817bcd8:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	sub_08002064
	adds	r1, r0, #0
	lsls	r1, r1, #8
	movs	r3, #139
	lsls	r3, r3, #7
	add	r1, r9
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #16
	subs	r3, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #8
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #56
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0817bd16:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_0817bcc8
.L_0817bd22:
	movs	r0, #4
	movs	r1, #4
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
	ldr	r2, [sp, #24]
	adds	r2, #1
	str	r2, [sp, #24]
	cmp	r2, #60
	beq.n	.L_0817bd4c
	b.n	.L_0817ba70
.L_0817bd4c:
	ldr	r4, [sp, #8]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #102
	adds	r2, r4, r0
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08143001
	.4byte 0xffffe000
	.4byte 0x0000012e
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r0, [sp, #68]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #64]
	movs	r0, #0
	ldr	r1, [r3, #96]
	str	r1, [sp, #60]
	ldr	r2, [r3, #48]
	str	r2, [sp, #52]
	ldr	r5, [r3, #100]
	bl	sub_081435e0
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0817bdc4
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_0817bdcc
.L_0817bdc4:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_0817bdcc:
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r1, [sp, #64]
	movs	r3, #239
	movs	r0, #238
	lsls	r3, r3, #7
	lsls	r0, r0, #7
	adds	r2, r1, r3
	adds	r0, #132
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r0
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	ldr	r2, [sp, #68]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	movs	r3, #0
	ldr	r1, [r0, #0]
	str	r3, [sp, #48]
	b.n	.L_0817be14
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0817be14:
	str	r3, [sp, #44]
	ldr	r0, [sp, #64]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #228
	adds	r2, r0, r3
	ldr	r3, [r1, #8]
	str	r3, [r2, #0]
	movs	r3, #221
	lsls	r3, r3, #3
	adds	r2, r0, r3
	ldr	r3, [r1, #12]
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #236
	adds	r2, r0, r3
	ldr	r3, [r1, #16]
	str	r3, [r2, #0]
	movs	r3, #222
	lsls	r3, r3, #3
	adds	r2, r0, r3
	ldr	r3, [r1, #24]
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #244
	adds	r2, r0, r3
	ldr	r3, [r1, #28]
	ldr	r0, [pc, #108]
	str	r3, [r2, #0]
	ldr	r2, [sp, #64]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #92]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #64]
	movs	r3, #142
	lsls	r3, r3, #7
	adds	r1, r2, r3
	ldr	r0, [pc, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #68]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #64]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b08
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	movs	r1, #4
	movs	r2, #32
	movs	r3, #32
	bl	sub_08178680
	ldr	r1, [sp, #52]
	movs	r0, #0
	adds	r1, #12
	str	r0, [sp, #56]
	str	r1, [sp, #28]
	b.n	.L_0817bed4
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x000000d3
	.4byte 0x00000134
	.4byte 0x000000f2
	.4byte 0x00000154
	.4byte 0x03000730
	.2byte 0x2000
	.2byte 0x0201
.L_0817bed4:
	bl	sub_08014de4
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #28]
	bl	sub_080156e8
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_0817befa
	ldr	r3, [sp, #68]
	movs	r1, #6
	ldr	r0, [r3, #8]
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #0]
	movs	r2, #4
	movs	r3, #0
	bl	sub_0814cd48
.L_0817befa:
	ldr	r0, [sp, #56]
	cmp	r0, #8
	bne.n	.L_0817bf06
	movs	r0, #103
	bl	sub_081c0010
.L_0817bf06:
	ldr	r1, [sp, #56]
	cmp	r1, #100
	bne.n	.L_0817bf20
	ldr	r2, [sp, #68]
	movs	r3, #1
	negs	r3, r3
	ldr	r0, [r2, #8]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0814cd48
.L_0817bf20:
	ldr	r3, [sp, #56]
	cmp	r3, #54
	bne.n	.L_0817bf96
	movs	r1, #128
	ldr	r3, [pc, #800]
	lsls	r1, r1, #7
	ldr	r2, [pc, #800]
	ldr	r0, [sp, #60]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20d4
	bl	sub_081c0010
	movs	r0, #212
	bl	sub_081180e8
	movs	r1, #238
	ldr	r0, [sp, #64]
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r0, [sp, #68]
	movs	r2, #0
	ldr	r3, [r0, #20]
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_0817bf96
	movs	r6, #128
	lsls	r6, r6, #11
	movs	r5, #36
.L_0817bf60:
	ldr	r1, [sp, #68]
	movs	r3, #150
	ldrsh	r0, [r5, r1]
	movs	r2, #128
	str	r3, [sp, #4]
	movs	r1, #1
	lsls	r2, r2, #10
	adds	r3, r6, #0
	str	r6, [sp, #0]
	bl	sub_0815f000
	ldr	r3, [sp, #68]
	movs	r2, #5
	ldrsh	r0, [r5, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, sl
	bl	sub_0814cd48
	ldr	r0, [sp, #68]
	movs	r2, #1
	ldr	r3, [r0, #20]
	add	sl, r2
	adds	r5, #2
	cmp	sl, r3
	bne.n	.L_0817bf60
.L_0817bf96:
	ldr	r1, [sp, #56]
	cmp	r1, #15
	bgt.n	.L_0817bf9e
	b.n	.L_0817c202
.L_0817bf9e:
	movs	r3, #44
	adds	r2, r1, #0
	muls	r2, r3
	ldr	r3, [pc, #684]
	movs	r0, #178
	adds	r3, r2, r3
	lsls	r0, r0, #3
	str	r3, [sp, #40]
	cmp	r3, r0
	ble.n	.L_0817bfb4
	str	r0, [sp, #40]
.L_0817bfb4:
	ldr	r1, [sp, #56]
	cmp	r1, #73
	ble.n	.L_0817bfc4
	movs	r3, #144
	lsls	r3, r3, #5
	adds	r3, #72
	subs	r3, r3, r2
	str	r3, [sp, #40]
.L_0817bfc4:
	ldr	r3, [sp, #40]
	movs	r2, #0
	lsls	r3, r3, #5
	str	r3, [sp, #32]
	str	r3, [sp, #36]
	str	r2, [sp, #24]
	str	r2, [sp, #20]
	mov	r9, r2
.L_0817bfd4:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #94
	mov	r0, r9
	muls	r0, r3
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #214
	adds	r0, r0, r1
	bl	sub_08002096
	movs	r3, #44
	muls	r3, r0
	ldr	r0, [sp, #56]
	asrs	r3, r3, #16
	movs	r2, #50
	add	r0, r9
	subs	r2, r2, r3
	lsls	r0, r0, #11
	str	r2, [sp, #12]
	bl	sub_08002096
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	mov	r3, r9
	muls	r3, r0
	ldr	r2, [sp, #12]
	cmp	r3, #0
	bge.n	.L_0817c010
	adds	r3, #31
.L_0817c010:
	asrs	r1, r3, #5
	adds	r0, r2, r1
	movs	r3, #0
	mov	r8, r0
	mov	sl, r3
	lsrs	r3, r0, #31
	add	r3, r8
	asrs	r3, r3, #1
	mov	r0, r9
	mov	fp, r3
	adds	r3, r0, r1
	adds	r7, r3, #0
	ldr	r0, [pc, #556]
	ldr	r3, [sp, #20]
	ldr	r4, [sp, #24]
	adds	r6, r1, #0
	adds	r7, #32
	adds	r6, #16
	adds	r5, r3, r0
.L_0817c036:
	adds	r0, r4, #0
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	mov	r0, fp
	asrs	r3, r3, #16
	subs	r3, r3, r0
	subs	r3, #16
	strb	r3, [r5, #0]
	adds	r0, r4, #0
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #16
	negs	r3, r3
	ldr	r2, [sp, #12]
	movs	r0, #1
	strb	r3, [r5, #1]
	add	sl, r0
	movs	r3, #0
	strb	r3, [r5, #2]
	mov	r3, sl
	adds	r7, r7, r2
	adds	r6, r6, r2
	adds	r5, #4
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r3, #2
	bne.n	.L_0817c036
	mov	r0, r9
	cmp	r0, #32
	bne.n	.L_0817c0c0
	ldr	r0, [sp, #36]
	bl	sub_08002090
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]
	lsrs	r5, r2, #31
	adds	r5, r2, r5
	asrs	r5, r5, #1
	adds	r3, r5, r1
	adds	r3, #16
	adds	r2, r3, #0
	muls	r2, r0
	mov	r0, r8
	lsrs	r3, r0, #31
	add	r3, r8
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	subs	r2, r2, r3
	subs	r2, #16
	ldr	r0, [sp, #36]
	str	r2, [sp, #48]
	bl	sub_08002096
	ldr	r1, [sp, #16]
	adds	r5, r5, r1
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	negs	r3, r3
	str	r3, [sp, #44]
.L_0817c0c0:
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #20]
	movs	r0, #1
	adds	r1, r1, r2
	add	r9, r0
	str	r1, [sp, #24]
	adds	r3, #8
	mov	r1, r9
	str	r3, [sp, #20]
	cmp	r1, #33
	beq.n	.L_0817c0da
	b.n	.L_0817bfd4
.L_0817c0da:
	bl	sub_081969f8
	movs	r2, #0
	ldr	r3, [sp, #72]
	mov	r8, r2
	ldr	r2, [pc, #372]
	adds	r6, r0, #0
	ands	r3, r2
	movs	r2, #7
	orrs	r3, r2
	ldr	r2, [pc, #368]
	ldr	r0, [sp, #64]
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r1, #224
	orrs	r3, r2
	lsls	r1, r1, #3
	str	r3, [sp, #72]
	add	r7, sp, #72
	adds	r3, r0, r1
	str	r3, [r7, #4]
	ldr	r0, [pc, #348]
	ldr	r3, [pc, #348]
	movs	r2, #6
	mov	r1, r8
	str	r3, [r6, #8]
	str	r0, [r6, #12]
	str	r1, [r6, #20]
	str	r2, [r6, #0]
	str	r7, [r6, #16]
	mov	sl, r2
	ldr	r2, [sp, #68]
	add	r5, sp, #80
	adds	r1, r5, #0
	ldr	r0, [r2, #8]
	bl	sub_0815e20c
	mov	r3, r8
	strb	r3, [r6, #25]
	ldr	r0, [sp, #56]
	movs	r3, #127
	lsls	r2, r0, #2
	bics	r3, r2
	strb	r3, [r6, #24]
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r2, [pc, #296]
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	movs	r5, #128
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [sp, #68]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_0817c170
	adds	r0, r5, #0
	bl	sub_08015068
.L_0817c170:
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_080150e4
	ldr	r1, [sp, #56]
	cmp	r1, #73
	ble.n	.L_0817c18a
	movs	r0, #212
	lsls	r3, r1, #11
	lsls	r0, r0, #10
	subs	r0, r0, r3
	bl	sub_0801521c
.L_0817c18a:
	ldr	r1, [pc, #216]
	movs	r2, #66
	ldr	r0, [pc, #200]
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	ldr	r3, [pc, #212]
	mov	r2, sl
	str	r3, [r6, #8]
	ldr	r3, [pc, #192]
	mov	r0, r8
	strb	r2, [r7, #0]
	strb	r2, [r7, #1]
	str	r3, [r6, #12]
	strb	r0, [r6, #24]
	strb	r0, [r6, #25]
	ldr	r1, [sp, #64]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r3, r1, r2
	str	r3, [r7, #4]
	mov	r3, r8
	str	r3, [r6, #20]
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
	lsls	r0, r1, #16
	lsls	r1, r2, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #174
	ldr	r3, [sp, #32]
	lsls	r0, r0, #7
	adds	r0, #112
	subs	r0, r0, r3
	bl	sub_080150e4
	ldr	r1, [sp, #40]
	movs	r2, #128
	lsls	r0, r1, #1
	adds	r0, r0, r1
	lsls	r0, r0, #3
	subs	r0, r0, r1
	lsls	r2, r2, #8
	adds	r0, r0, r2
	bl	sub_0801521c
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #116]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	adds	r0, r6, #0
	bl	sub_08013164
.L_0817c202:
	bl	sub_081434f8
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	movs	r0, #240
	ldr	r3, [sp, #64]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #56]
	adds	r1, #1
	str	r1, [sp, #56]
	cmp	r1, #103
	beq.n	.L_0817c22e
	b.n	.L_0817bed4
.L_0817c22e:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #64]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0xfffffd40
	.4byte 0x02010000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02011000
	.4byte 0x02012000
	.4byte 0xffd60000
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
	sub	sp, #40
	str	r0, [sp, #24]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	ldr	r1, [r5, #96]
	ldr	r2, [r5, #100]
	mov	r9, r0
	movs	r0, #1
	mov	fp, r1
	str	r2, [sp, #8]
	bl	sub_081435e0
	ldr	r4, [sp, #24]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0817c2b2
	movs	r0, #104
	movs	r1, #19
	b.n	.L_0817c2b6
.L_0817c2b2:
	movs	r0, #104
	movs	r1, #23
.L_0817c2b6:
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	str	r5, [sp, #12]
	ldr	r1, [sp, #8]
	ldr	r0, [pc, #848]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #224
	lsls	r1, r1, #3
	add	r1, r9
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #832]
	bl	sub_08157cf4
	ldr	r0, [pc, #832]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #824]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #796]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r6, #0
	movs	r0, #28
	str	r6, [sp, #20]
	add	r0, sp
	mov	r8, r0
.L_0817c318:
	ldr	r1, [sp, #24]
	ldr	r0, [r1, #8]
	mov	r1, r8
	bl	sub_0815e21c
	ldr	r2, [sp, #24]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0817c400
	ldr	r0, [sp, #20]
	movs	r1, #6
	bl	sub_08002054
	cmp	r0, #4
	bls.n	.L_0817c338
	b.n	sub_0817c4d2
.L_0817c338:
	ldr	r2, [pc, #748]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0817c354
	.4byte 0x0817c37e
	.4byte 0x0817c39e
	.4byte 0x0817c3c0
	.2byte 0xc3e0
	.2byte 0x0817
	mov	r3, r8
	ldr	r2, [r3, #0]
	mov	r4, r8
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r4, #4]
	movs	r0, #48
	movs	r1, #224
	lsls	r1, r1, #3
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #96
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #48
	subs	r3, #120
	mov	r0, fp
	ldr	r6, [sp, #12]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe0a9
	mov	r0, r8
	ldr	r2, [r0, #0]
	movs	r1, #200
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #38
	lsls	r1, r1, #5
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #92
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #38
	subs	r3, #120
	b.n	.L_0817c4a2
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r1, #152
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #30
	lsls	r1, r1, #6
	adds	r1, #168
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #76
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #38
	subs	r3, #100
	b.n	.L_0817c4a2
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r1, #188
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #56
	lsls	r1, r1, #6
	adds	r1, #144
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #96
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #64
	b.n	.L_0817c4a0
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r1, #136
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #56
	lsls	r1, r1, #7
	adds	r1, #144
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #88
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #64
	b.n	.L_0817c4a0
.L_0817c400:
	ldr	r0, [sp, #20]
	movs	r1, #6
	bl	sub_08002054
	cmp	r0, #4
	bhi.n	sub_0817c4d2
	ldr	r2, [pc, #540]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0817c428
	.4byte 0x0817c446
	.4byte 0x0817c464
	.4byte 0x0817c484
	.2byte 0xc4ac
	.2byte 0x0817
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r0, #48
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r1, #224
	lsls	r1, r1, #3
	str	r0, [sp, #0]
	movs	r0, #96
	str	r0, [sp, #4]
	add	r1, r9
	asrs	r2, r2, #1
	subs	r3, #120
	b.n	.L_0817c4a2
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r0, #38
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r1, #200
	lsls	r1, r1, #5
	str	r0, [sp, #0]
	movs	r0, #92
	str	r0, [sp, #4]
	add	r1, r9
	asrs	r2, r2, #1
	subs	r3, #120
	b.n	.L_0817c4a2
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r1, #152
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #30
	lsls	r1, r1, #6
	adds	r1, #168
	str	r0, [sp, #0]
	movs	r0, #76
	str	r0, [sp, #4]
	add	r1, r9
	asrs	r2, r2, #1
	subs	r3, #100
	b.n	.L_0817c4a2
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r1, #188
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #56
	lsls	r1, r1, #6
	adds	r1, #144
	str	r0, [sp, #0]
	movs	r0, #96
	str	r0, [sp, #4]
	add	r1, r9
	asrs	r2, r2, #1
.L_0817c4a0:
	subs	r3, #88
.L_0817c4a2:
	mov	r0, fp
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe012
	mov	r6, r8
	ldr	r2, [r6, #0]
	movs	r1, #136
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #56
	lsls	r1, r1, #7
	adds	r1, #144
	str	r0, [sp, #0]
	movs	r0, #88
	str	r0, [sp, #4]
	add	r1, r9
	asrs	r2, r2, #1
	subs	r3, #88
	mov	r0, fp
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e05
	cmp	r6, #18
	bne.n	.L_0817c5bc
	ldr	r2, [sp, #24]
	ldr	r7, [pc, #340]
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #1
	bl	sub_08118088
	ldr	r4, [sp, #24]
	movs	r2, #5
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	movs	r0, #134
	bl	sub_081180e8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r6, #0
	mov	sl, r6
.L_0817c510:
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0817c526
	mov	r1, r8
	ldr	r3, [r1, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, #48
	b.n	.L_0817c532
.L_0817c526:
	mov	r2, r8
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r3, #48
.L_0817c532:
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	mov	r4, r8
	ldr	r3, [r4, #4]
	movs	r2, #7
	ands	r2, r0
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r6, r3, r0
	bl	sub_08014878
	ldr	r1, [sp, #24]
	movs	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0817c57e
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [pc, #188]
	asrs	r3, r3, #4
	adds	r3, r3, r2
	b.n	.L_0817c590
.L_0817c57e:
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	movs	r4, #128
	asrs	r3, r3, #4
	lsls	r4, r4, #12
	adds	r3, r3, r4
.L_0817c590:
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	adds	r3, #64
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r6, #1
	movs	r0, #128
	adds	r3, #16
	add	sl, r6
	lsls	r0, r0, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	sl, r0
	bne.n	.L_0817c510
.L_0817c5bc:
	ldr	r1, [sp, #20]
	cmp	r1, #17
	ble.n	.L_0817c660
	ldr	r5, [pc, #108]
	movs	r2, #0
	mov	sl, r2
.L_0817c5c8:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0817c652
	asrs	r0, r0, #3
	adds	r0, #1
	ldr	r2, [pc, #100]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #8]
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	mov	r0, fp
	ldr	r4, [sp, #12]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4656
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0817c640
	adds	r0, r5, #0
	movs	r1, #60
	ldr	r2, [pc, #48]
	bl	sub_08138086
	b.n	.L_0817c64c
	movs	r0, r0
	.4byte 0x00000134
	.4byte 0x000000f3
	.4byte 0x00000149
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x0817c340
	.4byte 0x0817c414
	.4byte 0x02010000
	.4byte 0xfff80000
	.4byte 0x08197410
	.2byte 0xe000
	.2byte 0xffff
.L_0817c640:
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_08138086
.L_0817c64c:
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0817c652:
	movs	r0, #1
	movs	r1, #128
	add	sl, r0
	lsls	r1, r1, #1
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L_0817c5c8
.L_0817c660:
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
	bl	sub_08013560
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #52
	beq.n	.L_0817c68a
	b.n	.L_0817c318
.L_0817c68a:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #116
	str	r0, [sp, #80]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	str	r0, [sp, #76]
	movs	r0, #1
	ldr	r1, [r5, #96]
	str	r1, [sp, #72]
	bl	sub_081435e0
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #76]
	movs	r1, #238
	lsls	r1, r1, #7
	ldr	r5, [r5, #104]
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	str	r5, [sp, #64]
	bl	sub_080145a8
	movs	r2, #0
	str	r2, [sp, #52]
	str	r2, [sp, #48]
	ldr	r3, [sp, #80]
	mov	r4, sp
	b.n	.L_0817c720
	movs	r0, r0
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_0817c720:
	adds	r4, #104
	adds	r1, r4, #0
	ldr	r0, [r3, #8]
	str	r4, [sp, #36]
	bl	sub_0815e21c
	ldr	r2, [sp, #80]
	mov	r3, sp
	adds	r3, #92
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r3, #0
	str	r3, [sp, #32]
	bl	sub_0815e21c
	ldr	r4, [sp, #76]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	ldr	r0, [pc, #96]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r3, [sp, #76]
	movs	r4, #240
	lsls	r4, r4, #4
	adds	r1, r3, r4
	ldr	r0, [pc, #80]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #76]
	movs	r3, #142
	lsls	r3, r3, #7
	adds	r1, r2, r3
	ldr	r0, [pc, #68]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #52]
	movs	r1, #4
	movs	r2, #32
	movs	r3, #32
	bl	sub_08178680
	movs	r4, #0
	str	r4, [sp, #56]
.L_0817c78e:
	ldr	r0, [sp, #56]
	cmp	r0, #0
	bne.n	.L_0817c7c6
	movs	r1, #0
	ldr	r3, [sp, #76]
	str	r1, [sp, #40]
	str	r1, [sp, #44]
	mov	sl, r1
	movs	r2, #24
	adds	r3, #24
	b.n	.L_0817c7b8
	.4byte 0x00000080
	.4byte 0x000000f4
	.4byte 0x0000013e
	.4byte 0x000000e8
	.2byte 0x2000
	.2byte 0x0201
.L_0817c7b8:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #64
	bne.n	.L_0817c7b8
.L_0817c7c6:
	ldr	r1, [sp, #56]
	cmp	r1, #22
	bne.n	.L_0817c894
	movs	r1, #128
	ldr	r0, [sp, #72]
	lsls	r1, r1, #7
	ldr	r3, [pc, #908]
	ldr	r2, [pc, #908]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2090
	bl	sub_081180e8
	movs	r4, #238
	ldr	r3, [sp, #76]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r2, [sp, #80]
	movs	r3, #120
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #10
	lsls	r3, r3, #11
	movs	r1, #1
	str	r2, [sp, #0]
	bl	sub_0815f000
	ldr	r4, [sp, #80]
	movs	r1, #7
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r1, [pc, #840]
	ldr	r7, [sp, #32]
	ldr	r6, [sp, #76]
	movs	r0, #0
	mov	sl, r0
	mov	r8, r1
.L_0817c828:
	mov	r2, sl
	negs	r3, r2
	cmp	r3, #0
	bge.n	.L_0817c832
	adds	r3, #3
.L_0817c832:
	asrs	r3, r3, #2
	adds	r3, #2
	str	r3, [r6, #24]
	ldr	r3, [r7, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	bl	sub_08014878
	ldr	r3, [r7, #4]
	movs	r2, #15
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08014878
	ldr	r3, [sp, #80]
	mov	r4, r8
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #6
	ldrh	r5, [r4, r3]
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
	movs	r0, #1
	subs	r3, r3, r2
	add	sl, r0
	lsls	r3, r3, #10
	mov	r1, sl
	str	r3, [r6, #16]
	adds	r6, #28
	cmp	r1, #32
	bne.n	.L_0817c828
.L_0817c894:
	ldr	r3, [sp, #80]
	movs	r2, #0
	mov	sl, r2
	ldr	r2, [r3, #24]
	ldr	r0, [pc, #712]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #8
	ldrh	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_0817c926
	ldr	r5, [sp, #76]
	mov	r8, r0
.L_0817c8b0:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0817c908
	cmp	r3, #1
	ble.n	.L_0817c8f2
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_0817c8c2
	adds	r1, r3, #3
.L_0817c8c2:
	ldr	r4, [sp, #76]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r0, #240
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r1, r4, r1
	lsls	r0, r0, #4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r1, r1, r0
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r2, #16
	str	r0, [sp, #4]
	subs	r3, #32
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9914
	ldr	r0, [pc, #632]
	ldr	r2, [r1, #24]
.L_0817c8f2:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #10
	ldrh	r2, [r0, r3]
	adds	r0, r5, #0
	negs	r2, r2
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0817c908:
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [sp, #80]
	movs	r2, #1
	add	sl, r2
	ldr	r2, [r3, #24]
	mov	r0, r8
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #8
	ldrh	r3, [r0, r3]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_0817c8b0
.L_0817c926:
	ldr	r4, [sp, #56]
	cmp	r4, #0
	bge.n	.L_0817c92e
	b.n	.L_0817cbd4
.L_0817c92e:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldrh	r1, [r0, r3]
	str	r1, [sp, #28]
	movs	r1, #44
	adds	r2, r4, #0
	muls	r2, r1
	str	r2, [sp, #40]
	ldr	r4, [sp, #40]
	movs	r2, #247
	lsls	r2, r2, #2
	cmp	r4, r2
	ble.n	.L_0817c94c
	str	r2, [sp, #40]
.L_0817c94c:
	adds	r3, #2
	ldrh	r3, [r0, r3]
	ldr	r0, [sp, #56]
	cmp	r0, r3
	blt.n	.L_0817c964
	subs	r3, r0, r3
	adds	r4, r3, #0
	muls	r4, r1
	str	r4, [sp, #44]
	cmp	r4, r2
	ble.n	.L_0817c964
	str	r2, [sp, #44]
.L_0817c964:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #44]
	movs	r0, #0
	subs	r1, r1, r2
	str	r0, [sp, #60]
	str	r1, [sp, #24]
	str	r0, [sp, #20]
	str	r0, [sp, #16]
.L_0817c974:
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_0817c97e
	adds	r3, #7
.L_0817c97e:
	ldr	r0, [sp, #32]
	asrs	r2, r3, #3
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0817c98a
	adds	r3, #7
.L_0817c98a:
	ldr	r1, [sp, #44]
	asrs	r3, r3, #3
	subs	r4, r2, r3
	ldr	r2, [sp, #20]
	lsls	r3, r1, #5
	adds	r3, r3, r1
	adds	r2, r2, r3
	ldr	r1, [sp, #60]
	ldr	r3, [sp, #56]
	mov	r8, r2
	adds	r0, r3, r1
	lsls	r0, r0, #12
	str	r4, [sp, #8]
	bl	sub_08002096
	ldr	r2, [sp, #60]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r3, r2, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bge.n	.L_0817c9ba
	adds	r3, #31
.L_0817c9ba:
	asrs	r3, r3, #5
	mov	r9, r3
	cmp	r4, #0
	bge.n	.L_0817c9c4
	negs	r4, r4
.L_0817c9c4:
	ldr	r3, [sp, #80]
	ldr	r1, [pc, #416]
	ldr	r2, [r3, #24]
	ldr	r0, [sp, #60]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #4
	ldrh	r1, [r1, r3]
	lsls	r6, r0, #2
	mov	fp, r1
	cmp	r6, #24
	ble.n	.L_0817c9e0
	movs	r6, #24
.L_0817c9e0:
	ldr	r1, [sp, #28]
	adds	r3, r1, #0
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_0817c9f2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_0817c9f2:
	asrs	r6, r3, #16
	lsrs	r3, r3, #31
	adds	r3, r6, r3
	asrs	r3, r3, #1
	movs	r0, #0
	negs	r3, r3
	mov	r1, fp
	adds	r2, r1, r3
	mov	sl, r0
	adds	r3, r4, r3
	mov	r0, r9
	ldr	r1, [sp, #16]
	adds	r7, r3, r0
	ldr	r0, [pc, #348]
	lsls	r3, r1, #2
	add	r2, r9
	adds	r5, r3, r0
.L_0817ca14:
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
	negs	r3, r3
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
	bne.n	.L_0817ca14
	ldr	r1, [sp, #60]
	cmp	r1, #32
	bne.n	.L_0817ca7e
	mov	r0, r8
	bl	sub_08002090
	ldr	r4, [sp, #8]
	mov	r2, r9
	adds	r3, r4, r2
	muls	r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r4
	mov	r0, r8
	str	r3, [sp, #52]
	bl	sub_08002096
	mov	r3, fp
	add	r3, r9
	muls	r3, r0
	asrs	r3, r3, #16
	negs	r3, r3
	str	r3, [sp, #48]
.L_0817ca7e:
	ldr	r3, [sp, #20]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #60]
	ldr	r4, [sp, #24]
	adds	r0, #2
	adds	r3, r3, r4
	adds	r1, #1
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	str	r1, [sp, #60]
	cmp	r1, #33
	beq.n	.L_0817ca98
	b.n	.L_0817c974
.L_0817ca98:
	ldr	r2, [sp, #56]
	movs	r5, #0
	cmp	r2, #33
	ble.n	.L_0817caa6
	movs	r3, #34
	subs	r3, r3, r2
	lsls	r5, r3, #3
.L_0817caa6:
	movs	r3, #64
	negs	r3, r3
	cmp	r5, r3
	bgt.n	.L_0817cab0
	b.n	.L_0817cbd4
.L_0817cab0:
	movs	r0, #1
	bl	sub_081969f8
	movs	r4, #6
	adds	r6, r0, #0
	mov	r8, r4
	str	r4, [r6, #0]
	movs	r0, #0
	ldr	r2, [pc, #172]
	ldr	r3, [sp, #40]
	ldr	r4, [sp, #44]
	mov	sl, r0
	add	r7, sp, #84
	mov	r1, sl
	str	r7, [r6, #16]
	str	r5, [r6, #20]
	strb	r1, [r6, #25]
	str	r2, [r6, #12]
	cmp	r3, r4
	ble.n	.L_0817cbce
	mov	r0, r8
	movs	r3, #5
	strb	r0, [r7, #0]
	strb	r3, [r7, #1]
	ldr	r1, [sp, #76]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r3, r1, r2
	str	r3, [r7, #4]
	ldr	r3, [pc, #136]
	movs	r5, #128
	str	r3, [r6, #8]
	ldr	r4, [sp, #56]
	movs	r3, #127
	lsls	r2, r4, #3
	bics	r3, r2
	strb	r3, [r6, #24]
	bl	sub_08014de4
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #116]
	ldr	r0, [r1, #0]
	ldr	r1, [r1, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	lsls	r5, r5, #8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r4, [sp, #80]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_0817cb36
	adds	r0, r5, #0
	bl	sub_08015068
.L_0817cb36:
	movs	r0, #128
	lsls	r0, r0, #11
	bl	sub_0801521c
	ldr	r0, [pc, #44]
	ldr	r1, [pc, #44]
	movs	r2, #66
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	ldr	r0, [sp, #56]
	cmp	r0, #22
	bgt.n	.L_0817cbce
	ldr	r3, [pc, #36]
	mov	r1, r8
	mov	r2, sl
	str	r3, [r6, #8]
	strb	r1, [r7, #0]
	b.n	.L_0817cb80
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x08199484
	.4byte 0x02010000
	.4byte 0x02011000
	.4byte 0x02012000
	.4byte 0xffc00000
	.2byte 0x9340
	.2byte 0x0819
.L_0817cb80:
	strb	r1, [r7, #1]
	strb	r2, [r6, #24]
	ldr	r4, [sp, #76]
	movs	r0, #142
	lsls	r0, r0, #7
	adds	r3, r4, r0
	str	r3, [r7, #4]
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #48]
	lsls	r0, r1, #16
	lsls	r1, r2, #16
	movs	r2, #0
	bl	sub_08015160
	ldr	r4, [sp, #40]
	movs	r0, #128
	lsls	r3, r4, #5
	lsls	r0, r0, #7
	subs	r0, r0, r3
	bl	sub_080150e4
	ldr	r0, [sp, #28]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r0, r3, #2
	cmp	r0, #0
	bge.n	.L_0817cbb8
	adds	r0, #31
.L_0817cbb8:
	asrs	r0, r0, #5
	bl	sub_0801521c
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #100]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0817cbce:
	adds	r0, r6, #0
	bl	sub_08013164
.L_0817cbd4:
	bl	sub_081434f8
	ldr	r2, [sp, #80]
	ldr	r1, [r2, #24]
	lsls	r1, r1, #1
	adds	r1, #2
	adds	r0, r1, #0
	bl	sub_08158ce0
	movs	r4, #240
	ldr	r3, [sp, #76]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #56]
	adds	r0, #1
	str	r0, [sp, #56]
	cmp	r0, #50
	beq.n	.L_0817cc06
	b.n	.L_0817c78e
.L_0817cc06:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #28]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x081991e0
	.4byte 0x02011000
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
	str	r0, [sp, #32]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	ldr	r5, [pc, #84]
	str	r0, [sp, #28]
	ldr	r1, [r3, #92]
	str	r1, [sp, #24]
	ldr	r3, [r3, #100]
	str	r3, [sp, #20]
	bl	sub_0813ba50
	movs	r0, #1
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #36
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #16]
	bl	sub_08144aac
	ldr	r0, [pc, #44]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r3, [sp, #24]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #28]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #20]
	b.n	.L_0817ccac
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x02014000
	.4byte 0x0000013e
	.2byte 0x00b7
	.2byte 0x0000
.L_0817ccac:
	movs	r3, #0
	ldr	r0, [pc, #908]
	bl	sub_08157cf4
	ldr	r0, [pc, #904]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #900]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9806
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
	ldr	r0, [pc, #864]
	bl	sub_080145a8
	ldr	r1, [sp, #32]
	mov	r2, sp
	adds	r2, #44
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	adds	r1, r2, #0
	str	r2, [sp, #12]
	bl	sub_0815e21c
	movs	r3, #0
	str	r3, [sp, #8]
	mov	fp, r3
.L_0817cd02:
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L_0817cd48
	ldr	r3, [pc, #832]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #2
.L_0817cd16:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0817cd16
	ldr	r2, [sp, #24]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #804]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #145
	bl	sub_081c0010
	movs	r0, #238
	ldr	r4, [sp, #24]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r4, r0
	movs	r3, #4
	str	r3, [r2, #0]
.L_0817cd48:
	mov	r1, fp
	cmp	r1, #32
	bne.n	.L_0817cd7c
	ldr	r3, [sp, #32]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r1, [sp, #32]
	movs	r3, #240
	lsls	r3, r3, #12
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	bl	sub_0815f000
.L_0817cd7c:
	mov	r2, fp
	cmp	r2, #38
	bne.n	.L_0817cd96
	movs	r0, #134
	bl	sub_081180e8
	movs	r4, #238
	ldr	r3, [sp, #24]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #16
	str	r3, [r2, #0]
.L_0817cd96:
	mov	r3, fp
	subs	r3, #32
	cmp	r3, #11
	bhi.n	.L_0817cdce
	movs	r1, #3
	mov	r0, fp
	bl	sub_08002064
	lsls	r3, r0, #1
	adds	r3, r3, r0
	ldr	r0, [sp, #24]
	lsls	r3, r3, #11
	movs	r1, #224
	adds	r3, r0, r3
	lsls	r1, r1, #3
	adds	r2, r3, r1
	movs	r3, #0
	movs	r1, #192
	mov	r8, r3
	lsls	r1, r1, #5
.L_0817cdbe:
	ldrb	r3, [r2, #0]
	movs	r4, #1
	asrs	r3, r3, #1
	add	r8, r4
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r8, r1
	bne.n	.L_0817cdbe
.L_0817cdce:
	mov	r0, fp
	cmp	r0, #41
	bhi.n	.L_0817ce62
	ldr	r1, [sp, #8]
	movs	r3, #120
	subs	r6, r3, r1
	movs	r1, #3
	bl	sub_08002064
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r9, r3
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bge.n	.L_0817cdf4
	adds	r3, #63
.L_0817cdf4:
	ldr	r4, [sp, #8]
	asrs	r7, r3, #6
	lsls	r3, r7, #6
	subs	r7, r4, r3
	cmp	r6, #0
	bge.n	.L_0817ce02
	movs	r6, #0
.L_0817ce02:
	lsls	r3, r0, #1
	ldr	r2, [sp, #24]
	adds	r3, r3, r0
	lsls	r3, r3, #11
	movs	r1, #0
	adds	r2, r2, r3
	mov	r8, r1
	mov	sl, r2
.L_0817ce12:
	mov	r4, r8
	lsls	r3, r4, #6
	subs	r5, r3, r7
	movs	r3, #108
	subs	r2, r3, r5
	cmp	r2, #64
	ble.n	.L_0817ce22
	movs	r2, #64
.L_0817ce22:
	cmp	r2, #0
	ble.n	sub_0817ce58
	adds	r3, r6, #0
	subs	r3, #64
	cmp	r5, r3
	blt.n	sub_0817ce58
	cmp	r5, r6
	bge.n	.L_0817ce38
	subs	r3, r6, r5
	subs	r2, r2, r3
	adds	r5, r6, #0
.L_0817ce38:
	movs	r0, #96
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r1, [sp, #16]
	mov	r3, r9
	ldr	r1, [r1, #4]
	movs	r4, #48
	mov	ip, r1
	movs	r1, #224
	lsls	r1, r1, #3
	subs	r2, r3, r4
	ldr	r0, [sp, #28]
	add	r1, sl
	adds	r3, r5, #0
	mov	lr, ip
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	cmp	r1, #3
	bne.n	.L_0817ce12
.L_0817ce62:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_0817ce84
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [sp, #24]
	movs	r2, #4
	adds	r3, #24
	negs	r2, r2
.L_0817ce74:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	str	r2, [r3, #0]
	adds	r3, #28
	subs	r2, #1
	cmp	r0, #32
	bne.n	.L_0817ce74
.L_0817ce84:
	ldr	r5, [sp, #24]
	movs	r1, #0
	mov	r8, r1
.L_0817ce8a:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_0817cec6
	movs	r1, #3
	bl	sub_08002054
	ldr	r2, [pc, #444]
	adds	r1, r0, #0
	lsls	r1, r1, #11
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
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #408]
	bl	sub_08138086
	ldr	r0, [r5, #24]
.L_0817cec6:
	adds	r6, r0, #1
	str	r6, [r5, #24]
	cmp	r6, #0
	bne.n	.L_0817cf06
	bl	sub_08014878
	movs	r2, #31
	ands	r2, r0
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	adds	r2, r2, r3
	subs	r2, #16
	lsls	r2, r2, #16
	str	r2, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	str	r6, [r5, #16]
.L_0817cf06:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #24
	bne.n	.L_0817ce8a
	mov	r3, fp
	cmp	r3, #28
	bne.n	.L_0817cf9c
	ldr	r7, [pc, #320]
	movs	r4, #0
	movs	r0, #63
	mov	r8, r4
	mov	sl, r0
.L_0817cf22:
	ldr	r3, [r7, #24]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_0817cf8e
	bl	sub_08014878
	mov	r2, sl
	adds	r6, r0, #0
	ands	r6, r2
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
	movs	r4, #128
	lsls	r4, r4, #14
	asrs	r3, r3, #3
	adds	r3, r3, r4
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #192
	lsls	r0, r0, #15
	asrs	r3, r3, #2
	adds	r3, r3, r0
	str	r3, [r7, #4]
	bl	sub_08014878
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08014878
	mov	r2, sl
	ands	r0, r2
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L_0817cf8e:
	movs	r3, #1
	movs	r4, #128
	add	r8, r3
	lsls	r4, r4, #1
	adds	r7, #28
	cmp	r8, r4
	bne.n	.L_0817cf22
.L_0817cf9c:
	mov	r0, fp
	cmp	r0, #31
	bhi.n	.L_0817d006
	ldr	r5, [pc, #184]
	movs	r6, #0
	mov	r8, r6
	movs	r7, #63
.L_0817cfaa:
	ldr	r3, [r5, #24]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_0817cff8
	bl	sub_08014878
	ldr	r4, [sp, #12]
	movs	r2, #31
	ldr	r3, [r4, #0]
	ands	r2, r0
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	adds	r2, r2, r3
	subs	r2, #16
	movs	r3, #216
	lsls	r3, r3, #15
	lsls	r2, r2, #16
	str	r3, [r5, #4]
	str	r2, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	adds	r6, #1
	str	r0, [r5, #16]
	str	r3, [r5, #24]
	cmp	r6, #16
	beq.n	.L_0817d006
.L_0817cff8:
	movs	r0, #1
	movs	r1, #128
	add	r8, r0
	lsls	r1, r1, #2
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_0817cfaa
.L_0817d006:
	ldr	r5, [pc, #84]
	movs	r2, #0
	mov	r8, r2
.L_0817d00c:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0817d0d2
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	ldr	r2, [pc, #68]
	adds	r6, r0, #3
	lsls	r4, r6, #1
	mov	r3, r8
	movs	r0, #1
	ands	r0, r3
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsls	r0, r0, #2
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	asrs	r3, r3, #1
	b.n	.L_0817d064
	.4byte 0x00000134
	.4byte 0x00000130
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x000000b7
	.4byte 0x02014000
	.4byte 0xffffe000
	.4byte 0x02010000
	.2byte 0x7410
	.2byte 0x0819
.L_0817d064:
	subs	r2, r2, r3
	mov	lr, r2
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	str	r6, [sp, #0]
	subs	r2, r2, r6
	mov	ip, r2
	str	r4, [sp, #4]
	ldr	r2, [sp, #16]
	mov	r3, ip
	ldr	r4, [r0, r2]
	ldr	r0, [sp, #28]
	mov	r2, lr
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465b
	movs	r7, #3
	cmp	r3, #31
	ble.n	.L_0817d098
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #7
	bl	sub_08138086
	b.n	.L_0817d0aa
.L_0817d098:
	ldr	r3, [pc, #208]
	mov	r2, r8
	ands	r2, r7
	lsls	r2, r2, #2
	ldr	r2, [r3, r2]
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_08138086
.L_0817d0aa:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	ble.n	.L_0817d0c6
	ldr	r2, [r5, #4]
	asrs	r3, r2, #16
	cmp	r3, #104
	ble.n	.L_0817d0c8
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
	b.n	.L_0817d0c8
.L_0817d0c6:
	ldr	r2, [r5, #4]
.L_0817d0c8:
	cmn	r2, r6
	bge.n	.L_0817d0d2
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_0817d0d2:
	movs	r4, #1
	movs	r0, #128
	add	r8, r4
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L_0817d00c
	mov	r1, fp
	cmp	r1, #37
	bgt.n	.L_0817d0f4
	ldr	r3, [sp, #24]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #16
	str	r3, [r2, #0]
.L_0817d0f4:
	mov	r0, fp
	cmp	r0, #7
	bgt.n	.L_0817d104
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	.L_0817d11c
.L_0817d104:
	mov	r1, fp
	cmp	r1, #37
	bgt.n	.L_0817d114
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	b.n	.L_0817d11c
.L_0817d114:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
.L_0817d11c:
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #24]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #8]
	movs	r1, #1
	add	fp, r1
	adds	r0, #8
	mov	r2, fp
	str	r0, [sp, #8]
	cmp	r2, #92
	beq.n	.L_0817d146
	b.n	.L_0817cd02
.L_0817d146:
	ldr	r0, [pc, #40]
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
	movs	r0, r0
	.4byte 0x081994a8
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
	str	r0, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	movs	r5, #0
	str	r0, [sp, #48]
	movs	r0, #1
	ldr	r1, [r3, #92]
	mov	r9, r5
	str	r1, [sp, #44]
	ldr	r3, [r3, #100]
	str	r3, [sp, #36]
	bl	sub_081435e0
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	add	r1, sp, #56
	movs	r0, #0
	bl	sub_08144aac
	ldr	r2, [sp, #44]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #20]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r7, [sp, #44]
	str	r5, [sp, #24]
	mov	sl, r7
	b.n	.L_0817d1d4
	.4byte 0x00001010
	.2byte 0x0126
	.2byte 0x0000
.L_0817d1d4:
	movs	r0, #200
	ldr	r3, [sp, #24]
	lsls	r0, r0, #5
	add	r0, sl
	movs	r1, #141
	add	r3, r9
	str	r0, [sp, #8]
	movs	r5, #0
	lsls	r1, r1, #3
	lsls	r3, r3, #3
	mov	r8, r1
	mov	lr, r5
	mov	fp, r3
.L_0817d1ee:
	ldr	r7, [sp, #8]
	mov	r2, lr
	adds	r3, r2, r5
	lsls	r3, r3, #3
	adds	r3, r3, r7
	movs	r0, #23
	adds	r0, r0, r3
	adds	r4, r3, #0
	mov	r3, fp
	ldr	r1, [sp, #44]
	add	r3, lr
	adds	r2, r7, #0
	adds	r3, r3, r5
	lsls	r3, r3, #3
	add	r2, r8
	movs	r7, #224
	mov	ip, r0
	adds	r3, r3, r1
	adds	r0, r2, #0
	lsls	r7, r7, #3
	movs	r6, #0
	adds	r0, #23
	adds	r1, r3, r7
.L_0817d21c:
	ldrb	r3, [r1, #1]
	mov	r7, ip
	strb	r3, [r4, #0]
	adds	r6, #1
	ldrb	r3, [r1, #0]
	adds	r4, #1
	strb	r3, [r7, #0]
	movs	r3, #1
	negs	r3, r3
	add	ip, r3
	ldrb	r3, [r1, #1]
	strb	r3, [r2, #0]
	adds	r2, #1
	ldrb	r3, [r1, #0]
	adds	r1, #2
	strb	r3, [r0, #0]
	subs	r0, #1
	cmp	r6, #12
	bne.n	.L_0817d21c
	movs	r7, #24
	negs	r7, r7
	movs	r0, #2
	adds	r5, #1
	add	r8, r7
	add	lr, r0
	cmp	r5, #24
	bne.n	.L_0817d1ee
	ldr	r1, [sp, #24]
	movs	r3, #1
	movs	r2, #144
	add	r9, r3
	adds	r1, #8
	lsls	r2, r2, #3
	mov	r5, r9
	str	r1, [sp, #24]
	add	sl, r2
	cmp	r5, #2
	bne.n	.L_0817d1d4
	ldr	r7, [sp, #44]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r7, r2
	ldr	r0, [pc, #904]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	ldr	r1, [sp, #36]
	movs	r3, #0
	ldr	r0, [pc, #892]
	bl	sub_08157cf4
	ldr	r0, [pc, #892]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #884]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x25ef
	lsls	r5, r5, #7
	adds	r3, r7, r5
	mov	r7, r9
	str	r7, [r3, #0]
	ldr	r0, [sp, #44]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #852]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #0
	str	r2, [sp, #40]
.L_0817d2c0:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	mov	sl, r3
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne.n	.L_0817d378
	ldr	r6, [sp, #44]
	movs	r7, #255
	movs	r5, #0
	mov	r8, r7
	mov	r9, r5
	movs	r7, #0
.L_0817d2da:
	ldr	r1, [sp, #52]
	ldr	r0, [r1, #8]
	bl	sub_08118098
	ldr	r2, [sp, #52]
	ldr	r5, [r0, #0]
	ldr	r0, [r2, #8]
	bl	sub_08118070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r5, #8]
	asrs	r0, r0, #1
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	adds	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	mov	r3, r8
	ands	r0, r3
	ldr	r3, [r6, #0]
	subs	r0, #128
	lsls	r0, r0, #8
	str	r0, [r6, #12]
	cmp	r3, #0
	ble.n	.L_0817d318
	negs	r3, r0
	str	r3, [r6, #12]
.L_0817d318:
	str	r7, [r6, #12]
	bl	sub_08014878
	movs	r1, #200
	lsls	r1, r1, #1
	bl	sub_0800206c
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r6, #16]
	bl	sub_08014878
	mov	r5, r8
	ands	r0, r5
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r6, #20]
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	str	r7, [r6, #24]
	adds	r6, #28
	cmp	r1, #6
	bne.n	.L_0817d2da
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	r9, r2
	adds	r3, #192
	subs	r2, #1
.L_0817d352:
	movs	r5, #1
	add	r9, r5
	mov	r7, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #48
	bne.n	.L_0817d352
	ldr	r3, [pc, #684]
	movs	r0, #0
	movs	r2, #1
	mov	r9, r0
	negs	r2, r2
.L_0817d36a:
	movs	r1, #1
	add	r9, r1
	mov	r5, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #192
	bne.n	.L_0817d36a
.L_0817d378:
	bl	sub_08014de4
	mov	r1, sl
	adds	r1, #12
	mov	r0, sl
	bl	sub_080156e8
	movs	r1, #0
	ldr	r0, [sp, #44]
	str	r1, [sp, #20]
	movs	r7, #0
	mov	r9, r7
	mov	r8, r0
	mov	fp, r1
.L_0817d394:
	ldr	r2, [sp, #52]
	mov	r0, r9
	ldr	r1, [r2, #20]
	bl	sub_08002064
	lsls	r3, r0, #1
	str	r3, [sp, #28]
	ldr	r7, [sp, #52]
	str	r0, [sp, #32]
	adds	r5, r3, #0
	adds	r5, #36
	ldrsh	r0, [r7, r5]
	bl	sub_08118098
	ldr	r2, [sp, #52]
	ldr	r7, [r0, #0]
	ldrsh	r0, [r2, r5]
	bl	sub_08118070
	mov	r5, r8
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r5, #24]
	asrs	r0, r0, #1
	mov	sl, r0
	cmp	r3, #0
	bge.n	.L_0817d3cc
	b.n	.L_0817d574
.L_0817d3cc:
	ldr	r5, [sp, #40]
	add	r6, sp, #64
	add	r5, r9
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	adds	r1, r6, #0
	movs	r3, #1
	asrs	r5, r5, #1
	mov	r0, r8
	ands	r5, r3
	bl	sub_0815e1ec
	ldr	r2, [r6, #0]
	lsls	r1, r5, #3
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r0, [sp, #44]
	adds	r1, r1, r5
	lsls	r1, r1, #7
	movs	r3, #200
	adds	r1, r0, r1
	lsls	r3, r3, #5
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	movs	r0, #24
	str	r0, [sp, #0]
	movs	r0, #48
	subs	r3, #24
	str	r0, [sp, #4]
	subs	r2, #12
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4640
	movs	r1, #63
	movs	r2, #0
	bl	sub_08138058
	ldr	r5, [sp, #40]
	mov	r3, fp
	adds	r3, #10
	cmp	r5, r3
	bgt.n	.L_0817d426
	b.n	.L_0817d574
.L_0817d426:
	ldr	r2, [r7, #12]
	mov	r1, r8
	ldr	r3, [r1, #4]
	mov	r5, r8
	add	r2, sl
	ldr	r4, [r1, #0]
	subs	r2, r2, r3
	ldr	r1, [r7, #16]
	ldr	r3, [r5, #8]
	ldr	r0, [r7, #8]
	subs	r1, r1, r3
	ldr	r3, [r5, #12]
	subs	r0, r0, r4
	asrs	r0, r0, #10
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	asrs	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #16]
	ldr	r3, [r5, #20]
	asrs	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r5, #20]
	cmp	r4, #0
	bge.n	.L_0817d45c
	negs	r4, r4
.L_0817d45c:
	ldr	r0, [r7, #8]
	cmp	r0, #0
	bge.n	.L_0817d464
	negs	r0, r0
.L_0817d464:
	cmp	r4, r0
	bgt.n	.L_0817d46a
	b.n	.L_0817d574
.L_0817d46a:
	mov	r0, fp
	lsls	r3, r0, #3
	ldr	r2, [sp, #44]
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r3, r3, r2
	adds	r5, r3, #0
	movs	r1, #0
	adds	r7, r6, #0
	adds	r5, #168
.L_0817d47e:
	ldr	r3, [r7, #0]
	str	r1, [sp, #16]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #64
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	bl	sub_08014878
	movs	r2, #255
	ands	r2, r0
	movs	r3, #192
	ldr	r1, [sp, #16]
	subs	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r5, #16]
	adds	r1, #1
	movs	r3, #0
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_0817d47e
	ldr	r3, [sp, #20]
	ldr	r5, [pc, #344]
	movs	r1, #0
	mov	sl, r6
	adds	r7, r3, r5
.L_0817d4c2:
	mov	r0, sl
	ldr	r3, [r0, #0]
	str	r1, [sp, #16]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r6, #254
	ldr	r3, [r0, #4]
	lsls	r6, r6, #7
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, #255
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r6, r0
	adds	r6, r6, r2
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
	ldr	r1, [sp, #16]
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	adds	r1, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #32
	bne.n	.L_0817d4c2
	movs	r0, #134
	bl	sub_081180e8
	movs	r0, #145
	bl	sub_081c0010
	ldr	r5, [sp, #28]
	ldr	r3, [sp, #52]
	adds	r5, #36
	mov	r1, r9
	ldrsh	r0, [r3, r5]
	lsls	r3, r1, #2
	add	r3, r9
	adds	r3, #110
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #10
	lsls	r3, r3, #12
	movs	r1, #1
	str	r2, [sp, #0]
	bl	sub_0815f000
	ldr	r2, [sp, #52]
	movs	r1, #7
	ldrsh	r0, [r2, r5]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r2, #5
	ldr	r3, [sp, #32]
	bl	sub_0814cd48
	movs	r7, #238
	ldr	r5, [sp, #44]
	lsls	r7, r7, #7
	adds	r7, #168
	movs	r3, #4
	adds	r2, r5, r7
	str	r3, [r2, #0]
	mov	r0, r8
	subs	r3, #5
	str	r3, [r0, #24]
.L_0817d574:
	ldr	r3, [sp, #20]
	movs	r5, #224
	movs	r7, #1
	lsls	r5, r5, #2
	add	r9, r7
	movs	r1, #28
	movs	r2, #8
	adds	r3, r3, r5
	mov	r0, r9
	add	r8, r1
	add	fp, r2
	str	r3, [sp, #20]
	cmp	r0, #6
	beq.n	.L_0817d592
	b.n	.L_0817d394
.L_0817d592:
	ldr	r5, [sp, #44]
	movs	r1, #0
	mov	r9, r1
	adds	r5, #168
.L_0817d59a:
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bhi.n	.L_0817d5e4
	cmp	r1, #1
	ble.n	sub_0817d5d4
	cmp	r1, #0
	bge.n	.L_0817d5aa
	adds	r1, #3
.L_0817d5aa:
	ldr	r2, [sp, #44]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r3, #142
	adds	r1, r2, r1
	lsls	r3, r3, #7
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #60]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817d5e4:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #28
	cmp	r2, #48
	bne.n	.L_0817d59a
	ldr	r6, [pc, #40]
	ldr	r5, [pc, #32]
	movs	r3, #0
	mov	r9, r3
	b.n	.L_0817d620
	movs	r0, r0
	.4byte 0x0000013e
	.4byte 0x00000134
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0xfffff000
	.2byte 0x7410
	.2byte 0x0819
.L_0817d620:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0817d664
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #36]
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
	ldr	r4, [sp, #56]
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
.L_0817d664:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r5, #28
	cmp	r1, #192
	bne.n	.L_0817d620
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r5, #240
	ldr	r3, [sp, #44]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [sp, #40]
	adds	r7, #1
	str	r7, [sp, #40]
	cmp	r7, #144
	beq.n	.L_0817d69c
	b.n	.L_0817d2c0
.L_0817d69c:
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
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
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #40]
	movs	r7, #0
	movs	r6, #0
.L_0817d6cc:
	cmp	r7, #14
	ble.n	.L_0817d6f4
	ldr	r4, [pc, #24]
	movs	r2, #0
	adds	r0, r6, #0
.L_0817d6d6:
	adds	r3, r2, #0
	ands	r3, r4
	adds	r3, r0, r3
	adds	r3, #48
	adds	r2, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #32
	bne.n	.L_0817d6d6
	b.n	.L_0817d718
	movs	r0, r0
	.4byte 0x0000000f
	.2byte 0xf800
	.2byte 0x0600
.L_0817d6f4:
	ldr	r5, [pc, #24]
	ldr	r4, [pc, #28]
	movs	r2, #0
	adds	r0, r6, #0
.L_0817d6fc:
	adds	r3, r2, #0
	ands	r3, r5
	adds	r3, r0, r3
	adds	r3, r3, r4
	adds	r2, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #32
	bne.n	.L_0817d6fc
	b.n	.L_0817d718
	.4byte 0x0000000f
	.2byte 0x0210
	.2byte 0x0000
.L_0817d718:
	adds	r7, #1
	adds	r6, #32
	cmp	r7, #30
	bne.n	.L_0817d6cc
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
