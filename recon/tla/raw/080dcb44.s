.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002096, 0x08002096
	.set sub_080148e8, 0x080148e8
	.set sub_08020000, 0x08020000
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_08020148, 0x08020148
	.set sub_08020278, 0x08020278
	.set sub_080cccb8, 0x080cccb8
	.set sub_080d170c, 0x080d170c
	.set sub_080d17ac, 0x080d17ac
	.set sub_080d2d84, 0x080d2d84
	.set sub_081c0010, 0x081c0010
	.global Func_080dcb44
	.thumb_func
Func_080dcb44:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #88]
	movs	r0, #165
	lsls	r0, r0, #2
	adds	r1, r7, r0
	ldrb	r2, [r1, #0]
	sub	sp, #24
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080dcb6e
	adds	r3, #255
	strb	r3, [r1, #0]
	b.n	.L_080dcdba
.L_080dcb6e:
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #138
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r6, r7, r3
	movs	r5, #0
.L_080dcb8a:
	movs	r2, #162
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	lsls	r3, r5, #3
	adds	r0, r0, r3
	movs	r1, #160
	lsls	r0, r0, #16
	bl	sub_08002054
	bl	sub_08002096
	adds	r5, #1
	asrs	r0, r0, #14
	strh	r0, [r6, #0]
	adds	r6, #2
	cmp	r5, #159
	ble.n	.L_080dcb8a
	movs	r3, #162
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #4
	strh	r3, [r2, #0]
	lsls	r0, r0, #2
	adds	r0, #138
	adds	r1, r7, r0
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080dcc0e
	bl	.L_080dce60
	movs	r1, #199
	lsls	r1, r1, #1
	adds	r1, #255
	movs	r2, #163
	adds	r3, r7, r1
	lsls	r2, r2, #2
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	subs	r1, #2
	lsls	r3, r3, #5
	lsls	r0, r0, #10
	orrs	r0, r3
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r1, #1
	orrs	r0, r3
	movs	r3, #128
	lsls	r3, r3, #14
	orrs	r0, r3
	bl	sub_080d170c
	movs	r0, #1
	bl	sub_080d17ac
.L_080dcc0e:
	movs	r2, #164
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	bl	sub_080d2d84
	movs	r1, #0
	bl	.L_080dcf2c
	movs	r0, #203
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080dcc38
	cmp	r3, #8
	beq.n	.L_080dcc38
	cmp	r3, #16
	beq.n	.L_080dcc38
	b.n	.L_080dcd8c
.L_080dcc38:
	movs	r1, #164
	lsls	r1, r1, #2
	adds	r5, r7, r1
	ldrh	r0, [r5, #0]
	bl	sub_080d2d84
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #146
	adds	r2, r2, r7
	adds	r6, r0, #0
	ldrh	r0, [r2, #0]
	mov	fp, r2
	bl	sub_080d2d84
	mov	r8, r0
	cmp	r6, #0
	bne.n	.L_080dcc5e
	b.n	.L_080dcd8c
.L_080dcc5e:
	cmp	r0, #0
	bne.n	.L_080dcc64
	b.n	.L_080dcd8c
.L_080dcc64:
	ldrh	r0, [r5, #0]
	bl	sub_080cccb8
	movs	r3, #0
	ldrsh	r0, [r0, r3]
	ldr	r3, [r6, #8]
	add	r1, sp, #12
	str	r3, [r1, #0]
	mov	sl, r1
	bl	sub_08020000
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #12]
	ldr	r5, [pc, #104]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	adds	r3, r3, r5
	mov	r2, sl
	str	r3, [r2, #4]
	mov	r1, fp
	ldr	r3, [r6, #16]
	ldrh	r0, [r1, #0]
	str	r3, [r2, #8]
	mov	r3, r8
	ldr	r3, [r3, #80]
	mov	r9, r3
	bl	sub_080cccb8
	mov	r1, r8
	ldr	r3, [r1, #8]
	mov	r6, sp
	movs	r2, #0
	ldrsh	r0, [r0, r2]
	str	r3, [r6, #0]
	bl	sub_08020000
	movs	r3, #8
	ldrsb	r3, [r0, r3]
	mov	r0, r8
	ldr	r2, [r0, #12]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	adds	r5, r2, r5
	str	r5, [r6, #4]
	mov	r1, r9
	ldr	r3, [r0, #16]
	movs	r0, #128
	str	r3, [r6, #8]
	lsls	r0, r0, #8
	ldrh	r3, [r1, #18]
	cmp	r3, r0
	bne.n	.L_080dccd2
	ldr	r1, [pc, #32]
	b.n	.L_080dcd0a
.L_080dccd2:
	movs	r0, #128
	lsls	r0, r0, #7
	cmp	r3, r0
	bne.n	.L_080dccf8
	ldr	r3, [r6, #0]
	movs	r1, #192
	ldr	r0, [pc, #20]
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r6, #0]
	adds	r3, r2, r0
	b.n	.L_080dcd0c
	movs	r0, r0
	.4byte 0xfffe0000
	.4byte 0xffe60000
	.2byte 0x0000
	.2byte 0xfff2
.L_080dccf8:
	.2byte 0x21c0
	lsls	r1, r1, #8
	cmp	r3, r1
	bne.n	.L_080dcd0e
	ldr	r3, [r6, #0]
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	adds	r3, r3, r0
	str	r3, [r6, #0]
.L_080dcd0a:
	adds	r3, r2, r1
.L_080dcd0c:
	str	r3, [r6, #4]
.L_080dcd0e:
	movs	r0, #211
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_080200c0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080dcd8c
	ldr	r2, [r5, #80]
	movs	r3, #0
	mov	r9, r2
	adds	r2, r5, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	mov	r3, sl
	ldr	r0, [r3, #8]
	ldr	r3, [r6, #8]
	mov	r2, sl
	ldr	r1, [r2, #0]
	subs	r0, r0, r3
	ldr	r3, [r6, #0]
	subs	r1, r1, r3
	bl	sub_080148e8
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #40]
	str	r3, [r5, #108]
	mov	r3, r9
	strb	r2, [r3, #26]
	ldrb	r2, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	strh	r0, [r5, #6]
	orrs	r3, r2
	mov	r0, r9
	strb	r3, [r0, #9]
	mov	r2, sl
	mov	r0, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #4]
	adds	r0, r5, #0
	bl	sub_08020148
	b.n	.L_080dcd8c
	.4byte 0x00000000
	.4byte 0xfffa0000
	.4byte 0xfff20000
	.2byte 0xcf09
	.2byte 0x080d
.L_080dcd8c:
	movs	r1, #203
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r5, r7, r1
	ldrb	r3, [r5, #0]
	adds	r2, r3, #0
	cmp	r2, #99
	beq.n	.L_080dcdba
	cmp	r2, #0
	bne.n	.L_080dcda8
	movs	r0, #130
	bl	sub_081c0010
	ldrb	r3, [r5, #0]
.L_080dcda8:
	adds	r3, #1
	movs	r2, #240
	strb	r3, [r5, #0]
	lsls	r2, r2, #22
	lsls	r3, r3, #24
	cmp	r3, r2
	bls.n	.L_080dcdba
	movs	r3, #0
	strb	r3, [r5, #0]
.L_080dcdba:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #88]
	movs	r2, #165
	lsls	r2, r2, #2
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080dce1a
	subs	r2, #10
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	movs	r2, #197
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #4
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r1, #128
	lsls	r0, r0, #2
	lsls	r1, r1, #19
	ldrh	r2, [r3, #10]
	adds	r0, r4, r0
	adds	r1, #16
	ldr	r2, [pc, #4]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080dce1a:
	pop	{pc}
	.2byte 0x0001
	.2byte 0xa260
.L_080dce20:
	push	{r5, lr}
	lsls	r0, r0, #16
	movs	r1, #180
	asrs	r2, r0, #16
	lsls	r1, r1, #1
	adds	r0, r2, r1
	bl	sub_08002064
	lsls	r0, r0, #16
	asrs	r2, r0, #16
	movs	r5, #0
	cmp	r2, #59
	bgt.n	.L_080dce40
	lsls	r0, r2, #5
	subs	r0, r0, r2
	b.n	.L_080dce54
.L_080dce40:
	cmp	r2, #179
	bgt.n	.L_080dce48
	movs	r5, #31
	b.n	.L_080dce5c
.L_080dce48:
	cmp	r2, #239
	bgt.n	.L_080dce5c
	movs	r3, #240
	subs	r3, r3, r2
	lsls	r0, r3, #5
	subs	r0, r0, r3
.L_080dce54:
	movs	r1, #60
	bl	sub_08002054
	adds	r5, r0, #0
.L_080dce5c:
	adds	r0, r5, #0
	pop	{r5, pc}
.L_080dce60:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #88]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #142
	adds	r6, r7, r0
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	bl	.L_080dce20
	adds	r5, r0, #0
	ldrh	r0, [r6, #0]
	lsls	r5, r5, #16
	adds	r0, #120
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl	.L_080dce20
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	mov	r8, r0
	ldrh	r0, [r6, #0]
	asrs	r5, r5, #16
	adds	r0, #240
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl	.L_080dce20
	lsls	r0, r0, #16
	asrs	r4, r0, #16
	cmp	r5, #0
	bge.n	.L_080dceaa
	adds	r5, #3
.L_080dceaa:
	movs	r0, #198
	asrs	r3, r5, #2
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, #4
	movs	r1, #31
	adds	r2, r7, r0
	ands	r3, r1
	mov	r0, r8
	strb	r3, [r2, #0]
	cmp	r0, #0
	bge.n	.L_080dcec4
	adds	r0, #3
.L_080dcec4:
	asrs	r3, r0, #2
	movs	r0, #163
	lsls	r0, r0, #2
	adds	r3, #4
	adds	r2, r7, r0
	ands	r3, r1
	adds	r0, r4, #0
	strb	r3, [r2, #0]
	cmp	r0, #0
	bge.n	.L_080dceda
	adds	r0, #3
.L_080dceda:
	asrs	r3, r0, #2
	adds	r3, #4
	ands	r3, r1
	movs	r1, #199
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r7, r1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #24]
	ldrh	r3, [r6, #0]
	adds	r3, #4
	strh	r3, [r6, #0]
	lsls	r3, r3, #16
	cmp	r3, r2
	bls.n	.L_080dcefc
	movs	r3, #0
	strh	r3, [r6, #0]
.L_080dcefc:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0167
	push	{lr}
	ldr	r3, [r0, #56]
	movs	r2, #128
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_080dcf26
	ldr	r2, [r0, #60]
	cmp	r2, r3
	bne.n	.L_080dcf26
	ldr	r3, [r0, #64]
	cmp	r3, r2
	bne.n	.L_080dcf26
	ldr	r1, [pc, #4]
	bl	sub_080200a8
.L_080dcf26:
	pop	{pc}
	.2byte 0x3974
	.2byte 0x080f
.L_080dcf2c:
	push	{lr}
	subs	r3, r1, #1
	cmp	r1, #0
	bne.n	.L_080dcf42
	ldr	r3, [pc, #20]
	ldr	r1, [pc, #24]
	ldr	r3, [r3, #0]
	movs	r2, #7
	lsrs	r3, r3, #1
	ands	r3, r2
	ldrb	r3, [r1, r3]
.L_080dcf42:
	adds	r1, r3, #0
	bl	sub_08020278
	pop	{pc}
	movs	r0, r0
	.4byte 0x0300122c
	.2byte 0x0e9c
	.2byte 0x080f
	ldr	r3, [pc, #16]
	ldr	r1, [pc, #20]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r0, [r1, r3]
	bx	lr
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x080f0ea4
