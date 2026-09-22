.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_08016ce4, 0x08016ce4
	.set sub_080d181c, 0x080d181c
	.set sub_080d1830, 0x080d1830
	.global Overlay_080d0a28
Overlay_080d0a28:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #124]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #60
	adds	r4, r6, r1
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	cmp	r2, #0
	beq.n	.L_080d0aba
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #61
	adds	r1, r6, r3
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L_080d0a7e
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r0, [pc, #272]
	bl	sub_08014644
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	ldrh	r1, [r2, #10]
	movs	r3, #197
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r3, #254
	ldrh	r1, [r2, #10]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	b.n	.L_080d0b64
.L_080d0a7e:
	movs	r7, #160
	lsls	r7, r7, #3
	adds	r7, #59
	adds	r3, r6, r7
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #58
	adds	r5, r6, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r7, #17
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #192]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	ldrsb	r3, [r5, r3]
	adds	r2, r6, r7
	adds	r3, r3, r0
	strh	r3, [r2, #0]
.L_080d0aba:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #42
	adds	r3, r6, r1
	movs	r2, #160
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #3
	adds	r2, #57
	adds	r1, r6, r2
	subs	r0, r3, #1
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #32
	ands	r3, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080d0ae2
	movs	r5, #15
.L_080d0ae2:
	movs	r3, #31
	ands	r3, r0
	lsls	r0, r3, #1
	ldr	r3, [pc, #132]
	movs	r7, #63
	movs	r4, #0
	mov	ip, r3
	mov	lr, r7
.L_080d0af2:
	mov	r1, lr
	adds	r3, r0, #0
	ands	r3, r1
	mov	r7, ip
	ldrb	r2, [r7, r3]
	movs	r7, #161
	lsrs	r3, r2, #1
	adds	r3, r6, r3
	lsls	r7, r7, #3
	adds	r1, r3, r7
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d0b1c
	ldrb	r3, [r1, #0]
	movs	r2, #15
	ands	r2, r3
	lsls	r3, r5, #4
	orrs	r2, r3
	strb	r2, [r1, #0]
	b.n	.L_080d0b26
.L_080d0b1c:
	ldrb	r2, [r1, #0]
	movs	r3, #240
	ands	r3, r2
	orrs	r3, r5
	strb	r3, [r1, #0]
.L_080d0b26:
	adds	r4, #1
	adds	r0, #1
	cmp	r4, #1
	bls.n	.L_080d0af2
	ldr	r1, [pc, #68]
	ldr	r0, [pc, #68]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080d0b62
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	movs	r1, #161
	lsls	r1, r1, #3
	adds	r3, #4
	adds	r2, r6, r1
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #8
	str	r2, [r3, #0]
.L_080d0b62:
	strh	r4, [r0, #0]
.L_080d0b64:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080d0a29
	.4byte 0x03000528
	.4byte 0x080f0206
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_080d0b7c:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #124]
	ldr	r3, [pc, #88]
	sub	sp, #4
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #80]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #1
	negs	r2, r2
	cmp	r4, r2
	beq.n	.L_080d0bdc
	movs	r1, #0
	movs	r3, #7
.L_080d0ba8:
	lsls	r1, r1, #4
	subs	r3, #1
	orrs	r1, r4
	cmp	r3, #0
	bge.n	.L_080d0ba8
	movs	r3, #161
	lsls	r3, r3, #3
	adds	r2, r5, r3
	movs	r3, #7
.L_080d0bba:
	subs	r3, #1
	stmia	r2!, {r1}
	cmp	r3, #0
	bge.n	.L_080d0bba
	movs	r2, #161
	lsls	r2, r2, #3
	adds	r0, r5, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #192
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080d0bdc:
	add	sp, #4
	pop	{r5, pc}
	.4byte 0xf000f000
	.4byte 0x06002000
	.2byte 0x0140
	.2byte 0x8500
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r1, #168
	mov	r8, r0
	lsls	r1, r1, #3
	movs	r0, #124
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	adds	r5, r0, #0
	movs	r6, #0
	mov	r0, sp
	lsls	r3, r3, #19
	str	r6, [r0, #0]
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #0
	bl	.L_080d0b7c
	movs	r2, #165
	lsls	r2, r2, #3
	adds	r3, r5, r2
	mov	r2, r8
	strh	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #42
	adds	r5, r5, r3
	movs	r1, #144
	lsls	r1, r1, #3
	strh	r6, [r5, #0]
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	movs	r0, #120
	bl	sub_08013560
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x85000150
	.2byte 0x0a29
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r7, r0, #0
	adds	r6, r1, #0
	adds	r5, r2, #0
	mov	r8, r3
	cmp	r3, #0
	ble.n	.L_080d0c92
	ldr	r1, [pc, #52]
	movs	r2, #136
	lsls	r2, r2, #3
	mov	sl, r1
	adds	r2, #255
.L_080d0c70:
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	str	r2, [sp, #0]
	subs	r0, r0, r3
	mov	r1, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9a00
	strh	r0, [r5, #0]
	subs	r2, #1
	adds	r7, #2
	adds	r6, #2
	adds	r5, #2
	cmp	r2, #0
	bge.n	.L_080d0c70
.L_080d0c92:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0528
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r6, [r3, #0]
	movs	r1, #196
	movs	r0, #169
	lsls	r1, r1, #5
	lsls	r0, r0, #1
	adds	r5, r6, r1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d0cbe
	b.n	.L_080d0e0c
.L_080d0cbe:
	movs	r2, #168
	lsls	r2, r2, #6
	adds	r2, #1
	adds	r1, r6, r2
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080d0cd0
	b.n	.L_080d0e0c
.L_080d0cd0:
	adds	r2, #1
	adds	r3, r6, r2
	ldrb	r2, [r3, #0]
	adds	r2, #1
	strb	r2, [r3, #0]
	lsls	r2, r2, #24
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	asrs	r2, r2, #24
	cmp	r2, r3
	bge.n	.L_080d0d08
	movs	r3, #224
	movs	r4, #136
	lsls	r3, r3, #2
	lsls	r4, r4, #3
	adds	r1, r6, r3
	movs	r0, #0
	adds	r4, #255
.L_080d0cf4:
	ldrh	r3, [r1, #0]
	ldrh	r2, [r5, #0]
	adds	r0, #1
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	adds	r5, #2
	adds	r1, #2
	cmp	r0, r4
	ble.n	.L_080d0cf4
	b.n	.L_080d0d2a
.L_080d0d08:
	movs	r1, #224
	movs	r2, #224
	lsls	r1, r1, #2
	lsls	r2, r2, #4
	adds	r0, r6, r1
	adds	r1, r6, r2
	movs	r2, #168
	ldr	r3, [pc, #76]
	lsls	r2, r2, #4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23a8
	lsls	r3, r3, #6
	adds	r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_080d0d2a:
	movs	r1, #168
	lsls	r1, r1, #6
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #7
	movs	r2, #140
	adds	r3, r6, r3
	lsls	r2, r2, #6
	adds	r4, r3, r2
	ldr	r7, [pc, #20]
	movs	r3, #248
	movs	r2, #224
	ldr	r5, [pc, #20]
	lsls	r3, r3, #7
	movs	r0, #224
	lsls	r2, r2, #2
	mov	ip, r3
	lsls	r0, r0, #1
	adds	r1, r6, r2
	b.n	.L_080d0d68
	movs	r0, r0
	.4byte 0x000003e0
	.4byte 0x0000001f
	.2byte 0x0730
	.2byte 0x0300
.L_080d0d68:
	ldrh	r3, [r1, #0]
	mov	r2, ip
	ands	r2, r3
	ldrh	r3, [r1, #2]
	subs	r0, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #21
	ands	r3, r7
	orrs	r2, r3
	ldrh	r3, [r1, #4]
	adds	r1, #6
	lsls	r3, r3, #16
	asrs	r3, r3, #26
	ands	r3, r5
	orrs	r2, r3
	strh	r2, [r4, #0]
	adds	r4, #2
	cmp	r0, #0
	bne.n	.L_080d0d68
	movs	r3, #168
	lsls	r3, r3, #6
	adds	r1, r6, r3
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	ldr	r5, [pc, #112]
	ldrb	r2, [r1, #0]
	movs	r1, #140
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #7
	adds	r0, r6, r3
	lsls	r1, r1, #6
	adds	r6, r0, r1
	ldr	r4, [pc, #100]
	ldrh	r3, [r4, #0]
	adds	r1, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_080d0dda
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r5, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #112
	str	r2, [r3, #0]
.L_080d0dda:
	strh	r1, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_080d0e0a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r1, #147
	adds	r2, #1
	adds	r3, r3, r5
	lsls	r1, r1, #6
	adds	r3, #4
	strh	r2, [r5, #0]
	adds	r2, r0, r1
	stmia	r3!, {r2}
	ldr	r2, [pc, #24]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #112
	str	r2, [r3, #0]
.L_080d0e0a:
	strh	r6, [r4, #0]
.L_080d0e0c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x0200
	.2byte 0x0500
.L_080d0e1c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	mov	r8, r2
	movs	r1, #224
	adds	r2, r3, #0
	movs	r3, #128
	sub	sp, #48
	lsls	r1, r1, #1
	lsls	r3, r3, #8
	str	r1, [sp, #44]
	cmp	r0, r3
	bne.n	.L_080d0e46
	movs	r3, #160
	lsls	r3, r3, #19
	ldrh	r0, [r3, #0]
.L_080d0e46:
	cmp	r2, #1
	bne.n	.L_080d0e50
	movs	r1, #224
	str	r1, [sp, #44]
	b.n	.L_080d0e64
.L_080d0e50:
	cmp	r2, #2
	bne.n	.L_080d0e64
	movs	r3, #168
	lsls	r3, r3, #3
	add	r8, r3
	movs	r3, #224
	movs	r1, #224
	str	r3, [sp, #44]
	lsls	r1, r1, #1
	add	r9, r1
.L_080d0e64:
	movs	r1, #128
	lsls	r1, r1, #8
	cmp	r0, r1
	bcs.n	.L_080d0ec4
	ldr	r2, [pc, #56]
	adds	r3, r0, #0
	ands	r3, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #2
	add	r8, r3
	adds	r3, r0, #0
	ands	r3, r2
	mov	r1, r8
	lsls	r3, r3, #5
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	movs	r2, #2
	add	r8, r2
	ands	r0, r3
	lsls	r3, r0, #10
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r3, [sp, #44]
	add	r8, r2
	subs	r3, #1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #1
	movs	r4, #128
	movs	r3, #128
	b.n	.L_080d0eb4
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080d0eb4:
	lsls	r4, r4, #24
	mov	r0, r8
	lsrs	r2, r2, #1
	lsls	r3, r3, #19
	adds	r3, #212
	subs	r0, #6
	bl	.L_080d166e
.L_080d0ec4:
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r0, r3
	bcc.n	.L_080d0ece
	b.n	.L_080d144e
.L_080d0ece:
	ldr	r1, [pc, #732]
	adds	r0, r0, r1
	cmp	r0, #8
	bls.n	.L_080d0ed8
	b.n	.L_080d1400
.L_080d0ed8:
	ldr	r2, [pc, #724]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080d0f04
	.4byte 0x080d0fd2
	.4byte 0x080d106e
	.4byte 0x080d1108
	.4byte 0x080d11c4
	.4byte 0x080d124e
	.4byte 0x080d12c6
	.4byte 0x080d134e
	.2byte 0x0f52
	.2byte 0x080d
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_080d0f10
	b.n	.L_080d1676
.L_080d0f10:
	ldr	r7, [pc, #672]
	mov	r5, r8
.L_080d0f14:
	mov	r1, r9
	ldrh	r6, [r1, #0]
	movs	r3, #248
	lsls	r0, r6, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	r9, r2
	lsls	r2, r6, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r6
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #7
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c06
	strh	r6, [r5, #0]
	strh	r6, [r5, #2]
	strh	r6, [r5, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	adds	r5, #6
	cmp	fp, r1
	bcc.n	.L_080d0f14
	b.n	.L_080d1676
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_080d0f5e
	b.n	.L_080d1676
.L_080d0f5e:
	ldr	r1, [pc, #600]
	mov	sl, r1
.L_080d0f62:
	mov	r2, r9
	ldrh	r6, [r2, #0]
	movs	r1, #31
	movs	r3, #2
	adds	r4, r6, #0
	lsrs	r0, r6, #5
	ands	r4, r1
	ands	r0, r1
	add	r9, r3
	lsrs	r3, r6, #10
	ands	r3, r1
	adds	r0, r4, r0
	adds	r0, r0, r3
	movs	r1, #3
	ldr	r2, [pc, #564]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1c06
	subs	r4, r6, #1
	adds	r7, r4, #0
	adds	r0, r7, #0
	bl	sub_080d181c
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_080d181c
	subs	r5, r6, #3
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d181c
	adds	r5, r0, #0
	mov	r1, sl
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	ldr	r4, [sp, #0]
	mov	r1, r8
	strh	r3, [r1, #2]
	mov	r2, sl
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	movs	r2, #6
	strh	r3, [r1, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_080d0f62
	b.n	.L_080d1676
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_080d0fde
	b.n	.L_080d1676
.L_080d0fde:
	ldr	r2, [pc, #472]
	movs	r1, #31
	mov	sl, r1
.L_080d0fe4:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r1, #2
	mov	r3, sl
	adds	r4, r6, #0
	lsrs	r0, r6, #5
	ands	r4, r3
	ands	r0, r3
	add	r9, r1
	lsrs	r3, r6, #10
	mov	r1, sl
	ands	r3, r1
	adds	r0, r4, r0
	adds	r0, r0, r3
	str	r2, [sp, #4]
	ldr	r3, [pc, #432]
	movs	r1, #10
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c06
	lsls	r3, r6, #2
	adds	r4, r3, #5
	lsls	r3, r6, #1
	adds	r3, r3, r6
	adds	r5, r3, #5
	adds	r7, r5, #0
	ldr	r2, [sp, #4]
	cmp	r4, #7
	bgt.n	.L_080d1020
	movs	r4, #8
.L_080d1020:
	cmp	r5, #7
	bgt.n	.L_080d102c
	movs	r7, #8
	cmp	r5, #7
	bgt.n	.L_080d102c
	movs	r5, #8
.L_080d102c:
	cmp	r4, #28
	ble.n	.L_080d1032
	movs	r4, #28
.L_080d1032:
	cmp	r7, #28
	ble.n	.L_080d1038
	movs	r7, #28
.L_080d1038:
	cmp	r5, #28
	ble.n	.L_080d103e
	movs	r5, #28
.L_080d103e:
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	ldr	r3, [sp, #44]
	movs	r1, #1
	add	fp, r1
	cmp	fp, r3
	bcc.n	.L_080d0fe4
	b.n	.L_080d1676
	ldr	r2, [sp, #44]
	movs	r1, #0
	mov	fp, r1
	cmp	fp, r2
	bcc.n	.L_080d107a
	b.n	.L_080d1676
.L_080d107a:
	movs	r3, #31
	mov	sl, r3
.L_080d107e:
	mov	r1, r9
	ldrh	r6, [r1, #0]
	mov	r3, sl
	adds	r4, r6, #0
	ands	r4, r3
	lsrs	r7, r6, #5
	lsrs	r5, r6, #10
	ands	r7, r3
	ands	r5, r3
	adds	r3, r4, #0
	orrs	r3, r7
	movs	r2, #2
	orrs	r3, r5
	add	r9, r2
	cmp	r3, #0
	beq.n	.L_080d10d2
	lsrs	r3, r4, #1
	subs	r4, r4, r3
	movs	r1, #3
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	subs	r7, r7, r0
	adds	r4, #10
	adds	r0, r4, #0
	bl	sub_080d181c
	adds	r7, #8
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_080d181c
	subs	r5, #7
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d181c
	ldr	r4, [sp, #0]
	adds	r5, r0, #0
.L_080d10d2:
	ldr	r2, [pc, #232]
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	movs	r2, #2
	strh	r3, [r1, #0]
	add	r8, r2
	ldr	r2, [pc, #212]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	movs	r2, #2
	strh	r3, [r1, #0]
	add	r8, r2
	ldr	r2, [pc, #208]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [sp, #44]
	movs	r3, #1
	movs	r2, #2
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_080d107e
	b.n	.L_080d1676
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_080d1114
	b.n	.L_080d1676
.L_080d1114:
	ldr	r1, [pc, #168]
	mov	sl, r1
.L_080d1118:
	mov	r2, r9
	ldrh	r6, [r2, #0]
	movs	r1, #31
	movs	r3, #2
	adds	r4, r6, #0
	lsrs	r7, r6, #5
	lsrs	r5, r6, #10
	mov	r2, fp
	add	r9, r3
	ands	r4, r1
	ands	r7, r1
	ands	r5, r1
	cmp	r2, #0
	beq.n	.L_080d1178
	cmp	r4, #7
	bgt.n	.L_080d113a
	movs	r4, #8
.L_080d113a:
	cmp	r7, #13
	bgt.n	.L_080d1140
	movs	r7, #14
.L_080d1140:
	cmp	r5, #13
	bgt.n	.L_080d1146
	movs	r5, #14
.L_080d1146:
	cmp	r4, #29
	ble.n	.L_080d114c
	movs	r4, #29
.L_080d114c:
	cmp	r7, #25
	ble.n	.L_080d1152
	movs	r7, #25
.L_080d1152:
	cmp	r5, #27
	ble.n	.L_080d1158
	movs	r5, #27
.L_080d1158:
	adds	r0, r4, #0
	bl	sub_080d181c
	adds	r7, #2
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_080d181c
	adds	r5, #2
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d181c
	ldr	r4, [sp, #0]
	adds	r5, r0, #0
.L_080d1178:
	mov	r1, sl
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	mov	r1, sl
	lsls	r3, r4, #1
	ldrh	r3, [r1, r3]
	movs	r2, #2
	add	r8, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [sp, #44]
	movs	r1, #1
	movs	r3, #2
	add	fp, r1
	add	r8, r3
	cmp	fp, r2
	bcc.n	.L_080d1118
	b.n	.L_080d1676
	.4byte 0xfffeffff
	.4byte 0x080d0ee0
	.4byte 0x03000528
	.4byte 0x080f0286
	.4byte 0x080f02c6
	.2byte 0x0246
	.2byte 0x080f
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcc.n	.L_080d11d0
	b.n	.L_080d1676
.L_080d11d0:
	ldr	r2, [pc, #544]
	mov	sl, r2
.L_080d11d4:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r2, #31
	adds	r4, r6, #0
	lsrs	r7, r6, #5
	ands	r4, r2
	ands	r7, r2
	lsrs	r5, r6, #10
	ands	r5, r2
	adds	r0, r4, r7
	movs	r1, #2
	adds	r0, r0, r5
	add	r9, r1
	movs	r1, #3
	str	r4, [sp, #0]
	bl	sub_08002054
	bl	sub_080d181c
	ldr	r4, [sp, #0]
	asrs	r3, r4, #1
	adds	r4, r3, r0
	asrs	r3, r7, #1
	adds	r7, r3, r0
	asrs	r3, r5, #1
	adds	r5, r3, r0
	adds	r0, r4, #0
	bl	sub_080d181c
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_080d181c
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d181c
	adds	r5, r0, #0
	mov	r1, sl
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	ldr	r4, [sp, #0]
	mov	r1, r8
	strh	r3, [r1, #2]
	mov	r2, sl
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	movs	r2, #6
	strh	r3, [r1, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_080d11d4
	b.n	.L_080d1676
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_080d125a
	b.n	.L_080d1676
.L_080d125a:
	movs	r1, #31
	mov	sl, r1
.L_080d125e:
	mov	r2, r9
	ldrh	r6, [r2, #0]
	mov	r1, sl
	lsrs	r7, r6, #5
	lsrs	r5, r6, #10
	ands	r7, r1
	ands	r5, r1
	movs	r3, #2
	asrs	r2, r5, #3
	add	r9, r3
	adds	r4, r6, #0
	asrs	r3, r7, #3
	adds	r3, r3, r2
	ands	r4, r1
	adds	r4, r4, r3
	adds	r0, r4, #0
	bl	sub_080d181c
	movs	r1, #3
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_08002054
	movs	r1, #3
	subs	r7, r7, r0
	adds	r0, r5, #0
	bl	sub_08002054
	ldr	r2, [pc, #348]
	subs	r5, r5, r0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	ldr	r4, [sp, #0]
	mov	r2, r8
	strh	r3, [r2, #2]
	ldr	r2, [pc, #332]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	movs	r2, #6
	strh	r3, [r1, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_080d125e
	b.n	.L_080d1676
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_080d12d2
	b.n	.L_080d1676
.L_080d12d2:
	movs	r1, #31
	mov	sl, r1
.L_080d12d6:
	mov	r2, r9
	ldrh	r6, [r2, #0]
	mov	r1, sl
	adds	r4, r6, #0
	ands	r4, r1
	movs	r3, #2
	lsrs	r7, r6, #5
	ands	r7, r1
	add	r9, r3
	lsrs	r5, r6, #10
	lsrs	r3, r4, #1
	subs	r4, r4, r3
	ands	r5, r1
	adds	r0, r7, #0
	movs	r1, #3
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	subs	r7, r7, r0
	adds	r4, #6
	adds	r0, r4, #0
	bl	sub_080d181c
	adds	r7, #4
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_080d181c
	subs	r5, #6
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d181c
	ldr	r2, [pc, #212]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [pc, #208]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	ldr	r4, [sp, #0]
	strh	r3, [r1, #2]
	ldr	r2, [pc, #192]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #4]
	ldr	r2, [sp, #44]
	movs	r1, #1
	movs	r3, #6
	add	fp, r1
	add	r8, r3
	cmp	fp, r2
	bcc.n	.L_080d12d6
	b.n	.L_080d1676
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcc.n	.L_080d135a
	b.n	.L_080d1676
.L_080d135a:
	ldr	r2, [pc, #156]
	mov	sl, r2
.L_080d135e:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r2, #31
	adds	r4, r6, #0
	lsrs	r7, r6, #5
	ands	r4, r2
	ands	r7, r2
	lsrs	r5, r6, #10
	ands	r5, r2
	adds	r3, r4, r7
	movs	r1, #2
	adds	r3, r3, r5
	add	r9, r1
	cmp	r3, #60
	ble.n	.L_080d13a2
	adds	r0, r4, #0
	movs	r1, #5
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	movs	r1, #5
	subs	r4, r4, r0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_08002054
	movs	r1, #5
	subs	r7, r7, r0
	adds	r0, r5, #0
	bl	sub_08002054
	ldr	r4, [sp, #0]
	subs	r5, r5, r0
.L_080d13a2:
	adds	r0, r4, #0
	bl	sub_080d181c
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_080d181c
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d181c
	adds	r5, r0, #0
	mov	r1, sl
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	ldr	r4, [sp, #0]
	mov	r1, r8
	strh	r3, [r1, #0]
	mov	r1, sl
	lsls	r3, r4, #1
	ldrh	r3, [r1, r3]
	movs	r2, #2
	add	r8, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [sp, #44]
	movs	r1, #1
	movs	r3, #2
	add	fp, r1
	add	r8, r3
	cmp	fp, r2
	bcc.n	.L_080d135e
	b.n	.L_080d1676
	movs	r0, r0
	.4byte 0x080f02c6
	.4byte 0x080f0246
	.2byte 0x0286
	.2byte 0x080f
.L_080d1400:
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcc.n	.L_080d140c
	b.n	.L_080d1676
.L_080d140c:
	ldr	r4, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_080d1424
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080d1424:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r3, #2
	add	r9, r3
	adds	r3, r6, #0
	ands	r3, r4
	strh	r3, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r6, r2
	strh	r3, [r1, #2]
	lsls	r3, r6, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	fp, r3
	ldr	r3, [sp, #44]
	adds	r1, #6
	cmp	fp, r3
	bcc.n	.L_080d1424
	b.n	.L_080d1676
.L_080d144e:
	movs	r3, #128
	lsls	r3, r3, #14
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080d14f4
	movs	r3, #31
	adds	r1, r0, #0
	ands	r1, r3
	str	r0, [sp, #40]
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	str	r1, [sp, #40]
	adds	r1, r0, #0
	ands	r2, r3
	ands	r1, r3
	ldr	r3, [sp, #44]
	str	r2, [sp, #36]
	movs	r2, #0
	mov	fp, r2
	str	r0, [sp, #32]
	str	r1, [sp, #32]
	cmp	fp, r3
	bcc.n	.L_080d147e
	b.n	.L_080d1676
.L_080d147e:
	mov	sl, r8
.L_080d1480:
	mov	r1, r9
	ldrh	r6, [r1, #0]
	movs	r3, #248
	lsls	r0, r6, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	r9, r2
	lsls	r2, r6, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r6
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #96
	ldr	r3, [pc, #328]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0a
	adds	r6, r0, #0
	adds	r1, r2, #0
	muls	r1, r6
	ldr	r3, [sp, #36]
	mov	r8, r1
	ldr	r1, [sp, #32]
	mov	r0, r8
	adds	r7, r3, #0
	muls	r7, r6
	adds	r5, r1, #0
	muls	r5, r6
	bl	sub_080d1830
	mov	r8, r0
	adds	r0, r7, #0
	bl	sub_080d1830
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080d1830
	mov	r2, sl
	mov	r3, sl
	mov	r1, r8
	adds	r5, r0, #0
	strh	r5, [r2, #0]
	strh	r7, [r3, #2]
	strh	r1, [r2, #4]
	ldr	r2, [sp, #44]
	movs	r1, #1
	movs	r3, #6
	add	fp, r1
	add	sl, r3
	cmp	fp, r2
	bcc.n	.L_080d1480
	b.n	.L_080d1676
.L_080d14f4:
	movs	r3, #128
	lsls	r3, r3, #15
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080d1500
	b.n	.L_080d15fc
.L_080d1500:
	movs	r3, #31
	str	r0, [sp, #28]
	adds	r1, r0, #0
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	ands	r1, r3
	mov	sl, r0
	str	r1, [sp, #28]
	mov	r1, sl
	ands	r2, r3
	ands	r1, r3
	ldr	r3, [sp, #44]
	str	r2, [sp, #24]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r1
	cmp	fp, r3
	bcc.n	.L_080d1526
	b.n	.L_080d1676
.L_080d1526:
	ldr	r2, [sp, #24]
	ldr	r1, [sp, #28]
	ldr	r3, [sp, #28]
	adds	r1, r1, r2
	str	r1, [sp, #20]
	lsls	r1, r2, #16
	mov	r2, sl
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	mov	r7, r8
.L_080d1540:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r2, #31
	movs	r1, #2
	adds	r4, r6, #0
	lsrs	r0, r6, #5
	ands	r4, r2
	ands	r0, r2
	lsrs	r3, r6, #10
	add	r9, r1
	ldr	r1, [sp, #20]
	adds	r0, r4, r0
	ands	r3, r2
	adds	r0, r0, r3
	add	r1, sl
	ldr	r3, [pc, #144]
	lsls	r0, r0, #4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b07
	adds	r6, r0, #0
	adds	r0, r3, #0
	muls	r0, r6
	ldr	r2, [sp, #16]
	lsrs	r0, r0, #4
	asrs	r1, r2, #4
	ldr	r3, [pc, #124]
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9906
	mov	r8, r0
	adds	r0, r1, #0
	muls	r0, r6
	ldr	r2, [sp, #12]
	lsrs	r0, r0, #4
	asrs	r1, r2, #4
	ldr	r3, [pc, #104]
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	mov	r0, sl
	muls	r0, r6
	ldr	r2, [sp, #8]
	lsrs	r0, r0, #4
	asrs	r1, r2, #4
	ldr	r3, [pc, #84]
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	lsrs	r1, r1, #16
	mov	r8, r1
	adds	r6, r0, #0
	mov	r0, r8
	bl	sub_080d181c
	lsrs	r5, r5, #16
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_080d181c
	lsrs	r6, r6, #16
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_080d181c
	ldr	r2, [pc, #44]
	lsls	r0, r0, #1
	ldrh	r3, [r2, r0]
	lsls	r5, r5, #1
	strh	r3, [r7, #0]
	movs	r1, #1
	ldrh	r3, [r2, r5]
	add	fp, r1
	strh	r3, [r7, #2]
	mov	r3, r8
	lsls	r3, r3, #1
	mov	r8, r3
	ldrh	r3, [r2, r3]
	strh	r3, [r7, #4]
	ldr	r2, [sp, #44]
	adds	r7, #6
	cmp	fp, r2
	bcc.n	.L_080d1540
	b.n	.L_080d1676
	movs	r0, r0
	.4byte 0x03000528
	.4byte 0x0300021c
	.2byte 0x0246
	.2byte 0x080f
.L_080d15fc:
	movs	r3, #128
	lsls	r3, r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080d1652
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcs.n	.L_080d1676
	ldr	r4, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_080d1628
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080d1628:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r3, #2
	add	r9, r3
	adds	r3, r6, #0
	ands	r3, r4
	strh	r3, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r6, r2
	strh	r3, [r1, #2]
	lsls	r3, r6, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	fp, r3
	ldr	r3, [sp, #44]
	adds	r1, #6
	cmp	fp, r3
	bcc.n	.L_080d1628
	b.n	.L_080d1676
.L_080d1652:
	cmp	r2, #2
	bne.n	.L_080d165c
	movs	r1, #168
	lsls	r1, r1, #3
	adds	r0, r0, r1
.L_080d165c:
	ldr	r3, [sp, #44]
	movs	r4, #132
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #1
	lsls	r3, r3, #19
	adds	r3, #212
.L_080d166e:
	mov	r1, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080d1676:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r1, #168
	lsls	r1, r1, #6
	adds	r1, #4
	movs	r0, #128
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
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #160
	lsls	r2, r2, #24
	lsls	r0, r0, #19
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #44]
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #4
	movs	r0, #128
	adds	r2, r4, r3
	adds	r1, r4, #0
	movs	r3, #0
	lsls	r0, r0, #9
	bl	.L_080d0e1c
	movs	r1, #228
	lsls	r1, r1, #2
	adds	r1, #255
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x85000a81
	.4byte 0x05000200
	.2byte 0x0ca1
	.2byte 0x080d
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #128
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x0ca1
	.2byte 0x080d
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	adds	r4, r1, #0
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_080d1728
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r2, r1, r3
	adds	r3, r4, #0
	bl	.L_080d0e1c
.L_080d1728:
	pop	{pc}
