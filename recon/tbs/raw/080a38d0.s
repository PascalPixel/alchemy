.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08015068, 0x08015068
	.set sub_08015070, 0x08015070
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_08015270, 0x08015270
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080770d0, 0x080770d0
	.set sub_080a112c, 0x080a112c
	.set sub_080a17c4, 0x080a17c4
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a23f4, 0x080a23f4
	.set sub_080a3c98, 0x080a3c98
	.set sub_080a3ce4, 0x080a3ce4
	.set sub_080a3d6c, 0x080a3d6c
	.set sub_080a3d9c, 0x080a3d9c
	.set sub_080a3e88, 0x080a3e88
	.set sub_080a3ef0, 0x080a3ef0
	.set sub_080f9010, 0x080f9010
	.global Func_080a38d0
	.thumb_func
Func_080a38d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r0, [sp, #24]
	ldr	r3, [pc, #84]
	ldr	r6, [r3, #0]
	ldr	r1, [r6, #32]
	mov	r8, r1
	ldr	r1, [pc, #80]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	movs	r2, #29
	ldrsb	r2, [r6, r2]
	str	r3, [sp, #20]
	movs	r3, #0
	str	r3, [sp, #12]
	str	r3, [sp, #8]
	mov	sl, r2
	movs	r3, #12
	movs	r2, #1
	str	r2, [sp, #16]
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r0, r8
	movs	r1, #13
	movs	r3, #17
	bl	sub_080a23f4
	ldr	r0, [r6, #32]
	bl	sub_08015270
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	movs	r1, #130
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r0, [r6, r3]
	bl	sub_08077008
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #16]
	bl	sub_080041d8
	b.n	.L_080a3ba0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000219
	.2byte 0x3c09
	.2byte 0x080a
.L_080a3944:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L_080a394c
	b.n	.L_080a3b08
.L_080a394c:
	ldr	r0, [sp, #20]
	movs	r3, #0
	ldr	r1, [sp, #20]
	add	r0, sl
	str	r3, [sp, #16]
	bl	sub_080022fc
	mov	sl, r0
	mov	r2, sl
	lsls	r2, r2, #1
	movs	r3, #130
	ldr	r1, [r6, #32]
	lsls	r3, r3, #2
	str	r2, [sp, #4]
	adds	r7, r2, r3
	ldrh	r0, [r6, r7]
	mov	r8, r1
	mov	r9, r2
	bl	sub_08077008
	ldr	r3, [r6, #16]
	ldr	r2, [sp, #4]
	ldrh	r1, [r3, #12]
	add	r2, sl
	adds	r1, r1, r2
	ldr	r2, [pc, #44]
	ldr	r5, [r6, #24]
	ldr	r3, [pc, #32]
	lsls	r1, r1, #3
	subs	r1, #2
	mov	fp, r2
	strh	r1, [r5, #6]
	ands	r1, r3
	mov	r3, fp
	ands	r1, r3
	ldr	r2, [pc, #20]
	ldrh	r3, [r5, #22]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #22]
	ldr	r1, [sp, #24]
	cmp	r1, #1
	bne.n	.L_080a3a5c
	b.n	.L_080a39b0
	.4byte 0x0000ffff
	.4byte 0xfffffe00
	.2byte 0x01ff
	.2byte 0x0000
.L_080a39b0:
	ldrh	r0, [r6, r7]
	movs	r1, #1
	bl	sub_080a3e88
	movs	r3, #9
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #0
	movs	r2, #9
	movs	r3, #16
	bl	sub_08015070
	movs	r3, #80
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r3, #120
	movs	r1, #0
	movs	r2, #72
	bl	sub_08015068
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	cmp	sl, r3
	beq.n	.L_080a3a40
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	mov	r1, fp
	ldrh	r0, [r6, r7]
	ands	r1, r3
	bl	sub_080a3d9c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080a3a14
	movs	r3, #72
	str	r3, [sp, #0]
	movs	r1, #2
	mov	r2, r8
	movs	r3, #8
	bl	sub_080150b0
	ldr	r0, [pc, #140]
	mov	r1, r8
	movs	r2, #24
	movs	r3, #72
	bl	sub_08015080
	b.n	.L_080a3a20
.L_080a3a14:
	ldr	r0, [pc, #128]
	mov	r1, r8
	movs	r2, #16
	movs	r3, #72
	bl	sub_08015080
.L_080a3a20:
	movs	r3, #130
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r0, [r6, r3]
	bl	sub_080a3d6c
	cmp	r0, #15
	bne.n	.L_080a3a40
	cmp	r5, #0
	bne.n	.L_080a3a40
	ldr	r0, [pc, #100]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #72
	bl	sub_08015080
.L_080a3a40:
	ldr	r1, [pc, #92]
	movs	r2, #186
	adds	r3, r6, r1
	lsls	r2, r2, #1
	ldrb	r0, [r3, #0]
	adds	r3, r6, r2
	ldrh	r1, [r3, #0]
	movs	r3, #130
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r6, r3]
	movs	r2, #0
	bl	sub_080a3ef0
.L_080a3a5c:
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L_080a3b0e
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #36]
	ands	r0, r3
	bl	sub_080a3ce4
	cmp	r0, #0
	beq.n	.L_080a3aa4
	movs	r3, #130
	lsls	r3, r3, #2
	movs	r2, #186
	add	r3, r9
	lsls	r2, r2, #1
	ldrh	r1, [r6, r3]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	ldr	r0, [r6, #36]
	movs	r3, #8
	bl	sub_080a112c
	b.n	.L_080a3abc
	.4byte 0x000001ff
	.4byte 0x00000b2f
	.4byte 0x00000b31
	.4byte 0x00000b30
	.2byte 0x021a
	.2byte 0x0000
.L_080a3aa4:
	movs	r3, #130
	lsls	r3, r3, #2
	movs	r2, #186
	add	r3, r9
	lsls	r2, r2, #1
	ldrh	r1, [r6, r3]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	ldr	r0, [r6, #36]
	movs	r3, #0
	bl	sub_080a112c
.L_080a3abc:
	ldr	r0, [pc, #64]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a3af2
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080a3af2
	ldr	r0, [r6, #44]
	bl	sub_08015270
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #32]
	ands	r0, r3
	ldr	r3, [pc, #36]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [r6, #44]
	movs	r3, #0
	bl	sub_08015080
	movs	r2, #1
	str	r2, [sp, #8]
	b.n	.L_080a3b0e
.L_080a3af2:
	ldr	r0, [pc, #12]
	bl	sub_080770d0
	b.n	.L_080a3b0e
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000151
	.2byte 0x0075
	.2byte 0x0000
.L_080a3b08:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #4]
.L_080a3b0e:
	ldr	r0, [sp, #4]
	add	r0, sl
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #220]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a3b58
	ldr	r1, [sp, #24]
	cmp	r1, #1
	bne.n	.L_080a3b44
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	cmp	sl, r3
	bne.n	.L_080a3b44
	movs	r0, #114
	bl	sub_080f9010
	b.n	.L_080a3ba0
.L_080a3b44:
	movs	r0, #112
	bl	sub_080f9010
	movs	r1, #130
	ldr	r2, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldrb	r3, [r6, r3]
	str	r3, [sp, #12]
	b.n	.L_080a3bb4
.L_080a3b58:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a3b6e
	movs	r0, #113
	bl	sub_080f9010
	movs	r2, #255
	str	r2, [sp, #12]
	b.n	.L_080a3bb4
.L_080a3b6e:
	ldr	r5, [pc, #148]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a3b8a
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	movs	r1, #1
	negs	r3, r3
	str	r1, [sp, #16]
	add	sl, r3
.L_080a3b8a:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a3ba0
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	str	r2, [sp, #16]
	add	sl, r2
.L_080a3ba0:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a3bae
	b.n	.L_080a3944
.L_080a3bae:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #4]
.L_080a3bb4:
	ldr	r5, [r6, #24]
	mov	r1, sl
	strb	r1, [r6, #29]
	adds	r0, r5, #0
	bl	sub_080a17c4
	movs	r3, #13
	strb	r3, [r5, #5]
	bl	sub_080a3c98
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, sl
	strb	r2, [r6, #29]
	ldr	r3, [sp, #4]
	movs	r1, #130
	lsls	r1, r1, #2
	adds	r2, r3, r1
	ldrh	r3, [r6, r2]
	str	r3, [r6, #8]
	adds	r1, #19
	ldrh	r2, [r6, r2]
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldr	r2, [sp, #12]
	lsls	r0, r2, #24
	asrs	r0, r0, #24
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x03001b04
