.syntax unified
	.thumb
	.set sub_08013438, 0x08013438
	.set sub_08013560, 0x08013560
	.set sub_08013e70, 0x08013e70
	.set sub_08013eb4, 0x08013eb4
	.set sub_080145a8, 0x080145a8
	.set sub_08014694, 0x08014694
	.set sub_08014d00, 0x08014d00
	.set sub_080d0b7c, 0x080d0b7c
	.set sub_080d170c, 0x080d170c
	.set sub_080d172c, 0x080d172c
	.set sub_080d174c, 0x080d174c
	.set sub_080d17ac, 0x080d17ac
	.global Func_080d0184
	.thumb_func
Func_080d0184:
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014694
	ldr	r0, [pc, #8]
	bl	sub_08014694
	pop	{pc}
	.4byte 0x080cf78d
	.2byte 0xf6fd
	.2byte 0x080c
.L_080d019c:
	push	{lr}
	movs	r1, #168
	lsls	r1, r1, #3
	movs	r0, #124
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #12]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	add	sp, #4
	pop	{pc}
	movs	r0, r0
	.2byte 0x0150
	.2byte 0x8500
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #32]
	asrs	r2, r0, #8
	movs	r3, #255
	adds	r6, r3, #0
	ands	r2, r3
	mov	sl, r1
	ands	r6, r0
	cmp	r2, #4
	bls.n	.L_080d01ee
	b.n	.L_080d04d2
.L_080d01ee:
	lsls	r3, r2, #2
	ldr	r2, [pc, #132]
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	lsls	r4, r1, #8
	lsrs	r5, r1, #32
	lsls	r0, r4, #8
	lsrs	r5, r1, #32
	lsls	r4, r0, #10
	lsrs	r5, r1, #32
	lsls	r4, r5, #13
	lsrs	r5, r1, #32
	lsls	r0, r1, #16
	lsrs	r5, r1, #32
	movs	r0, #0
	bl	sub_08013e70
	mov	r0, sl
	bl	sub_08013eb4
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080d04d2
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r0, #128
	ldrh	r1, [r3, #0]
	lsls	r0, r0, #8
	bl	sub_080d172c
	mov	r0, sl
	bl	sub_080d17ac
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #64]
	ldr	r4, [pc, #64]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_080d026c
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_080d026c:
	strh	r5, [r4, #0]
	movs	r0, #0
	bl	sub_080d174c
	b.n	.L_080d0506
	movs	r0, r0
	.4byte 0x080d01f8
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	bl	.L_080d019c
	movs	r1, #0
	adds	r5, r0, #0
	movs	r2, #160
	movs	r0, #165
	mov	r9, r1
	lsls	r0, r0, #3
	lsls	r2, r2, #3
	movs	r1, #160
	adds	r3, r5, r0
	adds	r2, #42
	lsls	r1, r1, #3
	strh	r6, [r3, #0]
	mov	r0, r9
	adds	r3, r5, r2
	adds	r1, #52
	strh	r0, [r3, #0]
	adds	r2, r5, r1
	movs	r3, #63
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #74
	ldrh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	ands	r3, r2
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #60]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #54
	orrs	r3, r1
	mov	r8, r2
	movs	r1, #144
	adds	r2, r5, r0
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl	sub_080145a8
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #118
	ldr	r0, [pc, #36]
	bl	sub_080145a8
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #28]
	ldr	r4, [pc, #28]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	b.n	.L_080d0310
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x080cf78d
	.4byte 0x080cf6fd
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_080d0310:
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_080d0338
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_080d0338:
	strh	r6, [r4, #0]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #58
	adds	r3, r5, r1
	mov	r2, r9
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #59
	adds	r2, r5, r3
	movs	r0, #160
	movs	r3, #32
	strb	r3, [r2, #0]
	lsls	r0, r0, #3
	movs	r2, #160
	adds	r0, #60
	lsls	r2, r2, #3
	adds	r3, r5, r0
	mov	r1, sl
	adds	r2, #61
	strb	r1, [r3, #0]
	mov	r0, r8
	adds	r3, r5, r2
	strb	r0, [r3, #0]
	b.n	.L_080d0506
	bl	.L_080d019c
	movs	r1, #165
	movs	r2, #160
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #32
	lsls	r2, r2, #3
	adds	r2, #42
	adds	r3, r5, r1
	mov	r8, r0
	mov	r1, r8
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r1, [r3, #0]
	movs	r0, #15
	bl	sub_080d0b7c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #96]
	bl	sub_080145a8
	ldr	r1, [pc, #92]
	ldr	r4, [pc, #96]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_080d03d2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_080d03d2:
	strh	r6, [r4, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r0, #160
	adds	r2, #58
	lsls	r0, r0, #3
	adds	r3, r5, r2
	adds	r0, #59
	movs	r2, #0
	strb	r2, [r3, #0]
	mov	r1, r8
	adds	r3, r5, r0
	adds	r0, #1
	strb	r1, [r3, #0]
	adds	r3, r5, r0
	mov	r1, sl
	adds	r0, #1
	strb	r1, [r3, #0]
	adds	r3, r5, r0
	strb	r2, [r3, #0]
	b.n	.L_080d0506
	.4byte 0x080d0a29
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #32]
	bl	.L_080d019c
	ldr	r3, [pc, #52]
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r5, r0, #0
	ldr	r0, [pc, #48]
	adds	r2, r7, r1
	mov	r8, r3
	adds	r1, #2
	movs	r3, #80
	strh	r3, [r2, #0]
	adds	r2, r7, r1
	mov	r9, r0
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	cmp	r6, #0
	bne.n	.L_080d048e
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	ldr	r2, [pc, #16]
	movs	r0, #1
	movs	r1, #0
	b.n	.L_080d0458
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x080d0789
	.2byte 0x0955
	.2byte 0x080d
.L_080d0458:
	bl	sub_08013438
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r1, #160
	adds	r2, #58
	lsls	r1, r1, #3
	adds	r3, r5, r2
	mov	r0, r9
	adds	r1, #59
	strb	r0, [r3, #0]
	mov	r2, r8
	adds	r3, r5, r1
	movs	r0, #160
	strb	r2, [r3, #0]
	lsls	r0, r0, #3
	movs	r2, #160
	adds	r0, #60
	lsls	r2, r2, #3
	adds	r3, r5, r0
	mov	r1, sl
	adds	r2, #61
	strb	r1, [r3, #0]
	mov	r0, r8
	adds	r3, r5, r2
	strb	r0, [r3, #0]
	b.n	.L_080d04d2
.L_080d048e:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #124]
	bl	sub_080145a8
	ldr	r2, [pc, #120]
	movs	r0, #1
	movs	r1, #0
	bl	sub_08013438
	movs	r1, #160
	lsls	r1, r1, #3
	movs	r0, #160
	adds	r1, #58
	lsls	r0, r0, #3
	adds	r3, r5, r1
	mov	r2, r9
	adds	r0, #59
	strb	r2, [r3, #0]
	mov	r1, r8
	adds	r3, r5, r0
	movs	r2, #160
	strb	r1, [r3, #0]
	lsls	r2, r2, #3
	movs	r1, #160
	adds	r2, #60
	lsls	r1, r1, #3
	adds	r3, r5, r2
	mov	r0, sl
	adds	r1, #61
	strb	r0, [r3, #0]
	mov	r2, r8
	adds	r3, r5, r1
	strb	r2, [r3, #0]
.L_080d04d2:
	ldr	r1, [pc, #68]
	ldr	r4, [pc, #68]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_080d0504
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_080d0504:
	strh	r5, [r4, #0]
.L_080d0506:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x080d085d
	.4byte 0x080d0955
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #255
	asrs	r2, r0, #8
	adds	r6, r3, #0
	ands	r2, r3
	mov	sl, r1
	ands	r6, r0
	cmp	r2, #4
	bls.n	.L_080d053a
	b.n	.L_080d0718
.L_080d053a:
	lsls	r3, r2, #2
	ldr	r2, [pc, #56]
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	lsls	r0, r3, #21
	lsrs	r5, r1, #32
	lsls	r6, r4, #21
	lsrs	r5, r1, #32
	lsls	r4, r7, #21
	lsrs	r5, r1, #32
	lsls	r0, r5, #24
	lsrs	r5, r1, #32
	lsls	r2, r3, #26
	lsrs	r5, r1, #32
	movs	r0, #0
	bl	sub_08013eb4
	mov	r0, sl
	bl	sub_08013e70
	b.n	.L_080d0718
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	bl	sub_080d170c
	mov	r0, sl
	bl	sub_080d17ac
	b.n	.L_080d0718
	.2byte 0x0544
	.2byte 0x080d
	bl	.L_080d019c
	adds	r5, r0, #0
	movs	r0, #165
	lsls	r0, r0, #3
	adds	r3, r5, r0
	strh	r6, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #42
	adds	r2, r5, r3
	movs	r3, #32
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #52
	adds	r2, r5, r3
	movs	r3, #63
	strh	r3, [r2, #0]
	movs	r3, #128
	ldr	r0, [pc, #52]
	lsls	r3, r3, #19
	adds	r3, #74
	ldrh	r2, [r3, #0]
	mov	r8, r0
	ldr	r1, [pc, #48]
	movs	r3, #255
	movs	r0, #160
	lsls	r3, r3, #8
	lsls	r0, r0, #3
	ands	r3, r2
	adds	r0, #54
	adds	r2, r5, r0
	orrs	r3, r1
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #118
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	movs	r0, #1
	b.n	.L_080d05ec
	.4byte 0x00000020
	.4byte 0x00000001
	.4byte 0x080cf78d
	.2byte 0xf6fd
	.2byte 0x080c
.L_080d05ec:
	bl	sub_08013560
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #58
	adds	r3, r5, r2
	mov	r0, r8
	strb	r0, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	movs	r0, #160
	adds	r3, #59
	lsls	r0, r0, #3
	adds	r2, r5, r3
	adds	r0, #60
	movs	r3, #64
	strb	r3, [r2, #0]
	adds	r3, r5, r0
	mov	r2, sl
	strb	r2, [r3, #0]
	movs	r3, #160
	ldr	r6, [pc, #12]
	lsls	r3, r3, #3
	adds	r3, #61
	adds	r5, r5, r3
	strb	r6, [r5, #0]
	b.n	.L_080d0718
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
	bl	.L_080d019c
	adds	r5, r0, #0
	movs	r0, #165
	lsls	r0, r0, #3
	adds	r3, r5, r0
	strh	r6, [r3, #0]
	ldr	r2, [pc, #60]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #42
	mov	r8, r2
	adds	r2, r5, r3
	movs	r3, #32
	strh	r3, [r2, #0]
	movs	r0, #0
	bl	sub_080d0b7c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #144
	ldr	r0, [pc, #36]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r0, #160
	ldr	r6, [pc, #24]
	lsls	r0, r0, #3
	adds	r0, #58
	adds	r3, r5, r0
	strb	r6, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #59
	adds	r2, r5, r3
	b.n	.L_080d0680
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000020
	.2byte 0x0a29
	.2byte 0x080d
.L_080d0680:
	adds	r0, #2
	movs	r3, #64
	strb	r3, [r2, #0]
	adds	r3, r5, r0
	mov	r2, sl
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #61
	adds	r5, r5, r3
	mov	r0, r8
	strb	r0, [r5, #0]
	b.n	.L_080d0718
	bl	.L_080d019c
	movs	r3, #136
	lsls	r3, r3, #3
	adds	r3, #255
	adds	r5, r0, #0
	adds	r2, r5, r3
	movs	r3, #1
	movs	r7, #0
	strb	r3, [r2, #0]
	cmp	r6, #0
	bne.n	.L_080d06dc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #104]
	bl	sub_080145a8
	ldr	r2, [pc, #100]
	movs	r0, #1
	movs	r1, #0
	bl	sub_08013438
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #58
	adds	r3, r5, r0
	strb	r7, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #59
	adds	r2, r5, r3
	adds	r0, #2
	b.n	.L_080d0708
.L_080d06dc:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #68]
	bl	sub_080145a8
	ldr	r2, [pc, #60]
	movs	r0, #1
	movs	r1, #0
	bl	sub_08013438
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #58
	adds	r3, r5, r2
	strb	r7, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	movs	r0, #160
	adds	r3, #59
	lsls	r0, r0, #3
	adds	r2, r5, r3
	adds	r0, #60
.L_080d0708:
	movs	r3, #80
	strb	r3, [r2, #0]
	adds	r3, r5, r0
	mov	r2, sl
	adds	r0, #1
	strb	r2, [r3, #0]
	adds	r3, r5, r0
	strb	r7, [r3, #0]
.L_080d0718:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x080d0789
	.4byte 0x080d0955
	.2byte 0x085d
	.2byte 0x080d
	ldr	r3, [pc, #16]
	movs	r0, #1
	ldr	r2, [r3, #4]
	ldr	r3, [pc, #16]
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	subs	r0, r0, r3
	bx	lr
	.4byte 0x030001e4
	.2byte 0x0955
	.2byte 0x080d
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	beq.n	.L_080d0784
	cmp	r2, #0
	beq.n	.L_080d0764
	ldrh	r2, [r4, #20]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r4, #20]
.L_080d0764:
	cmp	r1, #0
	beq.n	.L_080d0774
	ldrh	r2, [r4, #20]
	movs	r3, #251
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r4, #20]
.L_080d0774:
	cmp	r0, #0
	beq.n	.L_080d0784
	ldrh	r2, [r4, #20]
	movs	r3, #247
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r4, #20]
.L_080d0784:
	pop	{pc}
	.2byte 0x0000
