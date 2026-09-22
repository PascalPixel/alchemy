.syntax unified
	.thumb
	.set sub_081c2a3c, 0x081c2a3c
	.set sub_081c2a8c, 0x081c2a8c
	.global Overlay_081c2af4
Overlay_081c2af4:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #28
	ldr	r0, [pc, #16]
	ldr	r0, [r0, #0]
	str	r0, [sp, #4]
	ldrb	r0, [r0, #10]
	cmp	r0, #0
	beq.n	.L_081c2b18
	subs	r0, #1
	ldr	r1, [sp, #4]
	strb	r0, [r1, #10]
	b.n	.L_081c2b1e
	.2byte 0x7ff0
	.2byte 0x0300
.L_081c2b18:
	movs	r0, #14
	ldr	r2, [sp, #4]
	strb	r0, [r2, #10]
.L_081c2b1e:
	movs	r6, #1
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
.L_081c2b24:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	ands	r0, r1
	adds	r2, r6, #1
	mov	sl, r2
	movs	r2, #64
	adds	r2, r2, r4
	mov	r9, r2
	cmp	r0, #0
	bne.n	.L_081c2b3a
	b.n	.L_081c2f24
.L_081c2b3a:
	cmp	r6, #2
	beq.n	.L_081c2b6c
	cmp	r6, #2
	bgt.n	.L_081c2b48
	cmp	r6, #1
	beq.n	.L_081c2b4e
	b.n	.L_081c2ba4
.L_081c2b48:
	cmp	r6, #3
	beq.n	.L_081c2b84
	b.n	.L_081c2ba4
.L_081c2b4e:
	ldr	r0, [pc, #16]
	str	r0, [sp, #8]
	ldr	r7, [pc, #16]
	ldr	r2, [pc, #16]
	str	r2, [sp, #12]
	adds	r0, #4
	str	r0, [sp, #16]
	adds	r2, #2
	b.n	.L_081c2bb4
	.4byte 0x04000060
	.4byte 0x04000062
	.2byte 0x0063
	.2byte 0x0400
.L_081c2b6c:
	ldr	r0, [pc, #8]
	str	r0, [sp, #8]
	ldr	r7, [pc, #8]
	ldr	r2, [pc, #12]
	b.n	.L_081c2bac
	movs	r0, r0
	.4byte 0x04000061
	.4byte 0x04000068
	.2byte 0x0069
	.2byte 0x0400
.L_081c2b84:
	ldr	r0, [pc, #16]
	str	r0, [sp, #8]
	ldr	r7, [pc, #16]
	ldr	r2, [pc, #20]
	str	r2, [sp, #12]
	adds	r0, #4
	str	r0, [sp, #16]
	adds	r2, #2
	b.n	.L_081c2bb4
	movs	r0, r0
	.4byte 0x04000070
	.4byte 0x04000072
	.2byte 0x0073
	.2byte 0x0400
.L_081c2ba4:
	ldr	r0, [pc, #92]
	str	r0, [sp, #8]
	ldr	r7, [pc, #92]
	ldr	r2, [pc, #96]
.L_081c2bac:
	str	r2, [sp, #12]
	adds	r0, #11
	str	r0, [sp, #16]
	adds	r2, #4
.L_081c2bb4:
	str	r2, [sp, #20]
	ldr	r0, [sp, #4]
	ldrb	r0, [r0, #10]
	str	r0, [sp, #0]
	ldr	r2, [sp, #12]
	ldrb	r0, [r2, #0]
	mov	r8, r0
	adds	r2, r1, #0
	movs	r0, #128
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L_081c2caa
	movs	r3, #64
	adds	r0, r3, #0
	ands	r0, r2
	lsls	r0, r0, #24
	lsrs	r5, r0, #24
	adds	r0, r6, #1
	mov	sl, r0
	movs	r1, #64
	adds	r1, r1, r4
	mov	r9, r1
	cmp	r5, #0
	bne.n	.L_081c2cce
	movs	r0, #3
	strb	r0, [r4, #0]
	strb	r0, [r4, #29]
	adds	r0, r4, #0
	str	r3, [sp, #24]
	bl	sub_081c2a8c
	ldr	r3, [sp, #24]
	cmp	r6, #2
	beq.n	.L_081c2c1c
	cmp	r6, #2
	bgt.n	.L_081c2c10
	cmp	r6, #1
	beq.n	.L_081c2c16
	b.n	.L_081c2c70
	movs	r0, r0
	.4byte 0x04000071
	.4byte 0x04000078
	.2byte 0x0079
	.2byte 0x0400
.L_081c2c10:
	cmp	r6, #3
	beq.n	.L_081c2c28
	b.n	.L_081c2c70
.L_081c2c16:
	ldrb	r0, [r4, #31]
	ldr	r2, [sp, #8]
	strb	r0, [r2, #0]
.L_081c2c1c:
	ldr	r0, [r4, #36]
	lsls	r0, r0, #6
	ldrb	r1, [r4, #30]
	adds	r0, r1, r0
	strb	r0, [r7, #0]
	b.n	.L_081c2c7c
.L_081c2c28:
	ldr	r1, [r4, #36]
	ldr	r0, [r4, #40]
	cmp	r1, r0
	beq.n	.L_081c2c50
	ldr	r2, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r1, [pc, #44]
	ldr	r2, [r4, #36]
	ldr	r0, [r2, #0]
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [r2, #4]
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [r2, #8]
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [r2, #12]
	str	r0, [r1, #0]
	str	r2, [r4, #40]
.L_081c2c50:
	ldr	r0, [sp, #8]
	strb	r5, [r0, #0]
	ldrb	r0, [r4, #30]
	strb	r0, [r7, #0]
	ldrb	r0, [r4, #30]
	cmp	r0, #0
	beq.n	.L_081c2c68
	movs	r0, #192
	b.n	.L_081c2c8a
	movs	r0, r0
	.2byte 0x0090
	.2byte 0x0400
.L_081c2c68:
	movs	r1, #128
	negs	r1, r1
	strb	r1, [r4, #26]
	b.n	.L_081c2c8c
.L_081c2c70:
	ldrb	r0, [r4, #30]
	strb	r0, [r7, #0]
	ldr	r0, [r4, #36]
	lsls	r0, r0, #3
	ldr	r2, [sp, #16]
	strb	r0, [r2, #0]
.L_081c2c7c:
	ldrb	r0, [r4, #4]
	adds	r0, #8
	mov	r8, r0
	ldrb	r0, [r4, #30]
	cmp	r0, #0
	beq.n	.L_081c2c8a
	movs	r0, #64
.L_081c2c8a:
	strb	r0, [r4, #26]
.L_081c2c8c:
	ldrb	r1, [r4, #4]
	movs	r2, #0
	strb	r1, [r4, #11]
	movs	r0, #255
	ands	r0, r1
	adds	r1, r6, #1
	mov	sl, r1
	movs	r1, #64
	adds	r1, r1, r4
	mov	r9, r1
	cmp	r0, #0
	bne.n	.L_081c2ca6
	b.n	.L_081c2de2
.L_081c2ca6:
	strb	r2, [r4, #9]
	b.n	.L_081c2e10
.L_081c2caa:
	movs	r0, #4
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L_081c2cdc
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	movs	r2, #255
	ands	r0, r2
	lsls	r0, r0, #24
	adds	r1, r6, #1
	mov	sl, r1
	movs	r2, #64
	adds	r2, r2, r4
	mov	r9, r2
	cmp	r0, #0
	ble.n	.L_081c2cce
	b.n	.L_081c2e22
.L_081c2cce:
	lsls	r0, r6, #24
	lsrs	r0, r0, #24
	bl	sub_081c2a3c
	movs	r0, #0
	strb	r0, [r4, #0]
	b.n	.L_081c2f20
.L_081c2cdc:
	movs	r0, #64
	ands	r0, r1
	adds	r2, r6, #1
	mov	sl, r2
	movs	r2, #64
	adds	r2, r2, r4
	mov	r9, r2
	cmp	r0, #0
	beq.n	.L_081c2d1c
	movs	r0, #3
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_081c2d1c
	movs	r0, #252
	ands	r0, r1
	movs	r2, #0
	strb	r0, [r4, #0]
	ldrb	r1, [r4, #7]
	strb	r1, [r4, #11]
	movs	r0, #255
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_081c2d4e
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
	cmp	r6, #3
	beq.n	.L_081c2e10
	ldrb	r2, [r4, #7]
	mov	r8, r2
	b.n	.L_081c2e10
.L_081c2d1c:
	ldrb	r0, [r4, #11]
	cmp	r0, #0
	bne.n	.L_081c2e10
	cmp	r6, #3
	bne.n	.L_081c2d2e
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
.L_081c2d2e:
	adds	r0, r4, #0
	bl	sub_081c2a8c
	movs	r0, #3
	ldrb	r2, [r4, #0]
	ands	r0, r2
	cmp	r0, #0
	bne.n	.L_081c2d82
	ldrb	r0, [r4, #9]
	subs	r0, #1
	strb	r0, [r4, #9]
	movs	r1, #255
	ands	r0, r1
	lsls	r0, r0, #24
	cmp	r0, #0
	bgt.n	.L_081c2d7e
.L_081c2d4e:
	ldrb	r2, [r4, #12]
	ldrb	r1, [r4, #10]
	adds	r0, r2, #0
	muls	r0, r1
	adds	r0, #255
	asrs	r0, r0, #8
	movs	r1, #0
	strb	r0, [r4, #9]
	lsls	r0, r0, #24
	cmp	r0, #0
	beq.n	.L_081c2cce
	movs	r0, #4
	ldrb	r2, [r4, #0]
	orrs	r0, r2
	strb	r0, [r4, #0]
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
	cmp	r6, #3
	beq.n	.L_081c2e22
	movs	r2, #8
	mov	r8, r2
	b.n	.L_081c2e22
.L_081c2d7e:
	ldrb	r0, [r4, #7]
	b.n	.L_081c2e0e
.L_081c2d82:
	cmp	r0, #1
	bne.n	.L_081c2d8e
.L_081c2d86:
	ldrb	r0, [r4, #25]
	strb	r0, [r4, #9]
	movs	r0, #7
	b.n	.L_081c2e0e
.L_081c2d8e:
	cmp	r0, #2
	bne.n	.L_081c2dd2
	ldrb	r0, [r4, #9]
	subs	r0, #1
	strb	r0, [r4, #9]
	movs	r1, #255
	ands	r0, r1
	lsls	r0, r0, #24
	ldrb	r2, [r4, #25]
	lsls	r1, r2, #24
	cmp	r0, r1
	bgt.n	.L_081c2dce
.L_081c2da6:
	ldrb	r0, [r4, #6]
	cmp	r0, #0
	bne.n	.L_081c2db6
	movs	r0, #252
	ldrb	r1, [r4, #0]
	ands	r0, r1
	strb	r0, [r4, #0]
	b.n	.L_081c2d4e
.L_081c2db6:
	ldrb	r0, [r4, #0]
	subs	r0, #1
	strb	r0, [r4, #0]
	movs	r0, #1
	ldrb	r2, [r4, #29]
	orrs	r0, r2
	strb	r0, [r4, #29]
	cmp	r6, #3
	beq.n	.L_081c2d86
	movs	r0, #8
	mov	r8, r0
	b.n	.L_081c2d86
.L_081c2dce:
	ldrb	r0, [r4, #5]
	b.n	.L_081c2e0e
.L_081c2dd2:
	ldrb	r0, [r4, #9]
	adds	r0, #1
	strb	r0, [r4, #9]
	movs	r1, #255
	ands	r0, r1
	ldrb	r2, [r4, #10]
	cmp	r0, r2
	bcc.n	.L_081c2e0c
.L_081c2de2:
	ldrb	r0, [r4, #0]
	subs	r0, #1
	movs	r2, #0
	strb	r0, [r4, #0]
	ldrb	r1, [r4, #5]
	strb	r1, [r4, #11]
	movs	r0, #255
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_081c2da6
	movs	r0, #1
	ldrb	r1, [r4, #29]
	orrs	r0, r1
	strb	r0, [r4, #29]
	ldrb	r0, [r4, #10]
	strb	r0, [r4, #9]
	cmp	r6, #3
	beq.n	.L_081c2e10
	ldrb	r2, [r4, #5]
	mov	r8, r2
	b.n	.L_081c2e10
.L_081c2e0c:
	ldrb	r0, [r4, #4]
.L_081c2e0e:
	strb	r0, [r4, #11]
.L_081c2e10:
	ldrb	r0, [r4, #11]
	subs	r0, #1
	strb	r0, [r4, #11]
	ldr	r0, [sp, #0]
	cmp	r0, #0
	bne.n	.L_081c2e22
	subs	r0, #1
	str	r0, [sp, #0]
	b.n	.L_081c2d1c
.L_081c2e22:
	movs	r0, #2
	ldrb	r1, [r4, #29]
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_081c2e9a
	cmp	r6, #3
	bgt.n	.L_081c2e62
	movs	r0, #8
	ldrb	r2, [r4, #1]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L_081c2e62
	ldr	r0, [pc, #16]
	ldrb	r0, [r0, #0]
	cmp	r0, #63
	bgt.n	.L_081c2e54
	ldr	r0, [r4, #32]
	adds	r0, #2
	ldr	r1, [pc, #8]
	b.n	.L_081c2e5e
	movs	r0, r0
	.4byte 0x04000089
	.2byte 0x07fc
	.2byte 0x0000
.L_081c2e54:
	cmp	r0, #127
	bgt.n	.L_081c2e62
	ldr	r0, [r4, #32]
	adds	r0, #1
	ldr	r1, [pc, #16]
.L_081c2e5e:
	ands	r0, r1
	str	r0, [r4, #32]
.L_081c2e62:
	cmp	r6, #4
	beq.n	.L_081c2e74
	ldr	r0, [r4, #32]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
	b.n	.L_081c2e82
	movs	r0, r0
	.2byte 0x07fe
	.2byte 0x0000
.L_081c2e74:
	ldr	r2, [sp, #16]
	ldrb	r0, [r2, #0]
	movs	r1, #8
	ands	r1, r0
	ldr	r0, [r4, #32]
	orrs	r0, r1
	strb	r0, [r2, #0]
.L_081c2e82:
	movs	r0, #192
	ldrb	r1, [r4, #26]
	ands	r0, r1
	adds	r1, r4, #0
	adds	r1, #33
	ldrb	r1, [r1, #0]
	adds	r0, r1, r0
	strb	r0, [r4, #26]
	movs	r2, #255
	ands	r0, r2
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
.L_081c2e9a:
	movs	r0, #1
	ldrb	r2, [r4, #29]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L_081c2f20
	ldr	r1, [pc, #60]
	ldrb	r0, [r1, #0]
	ldrb	r2, [r4, #28]
	bics	r0, r2
	ldrb	r2, [r4, #27]
	orrs	r0, r2
	strb	r0, [r1, #0]
	cmp	r6, #3
	bne.n	.L_081c2eec
	ldr	r0, [pc, #48]
	ldrb	r1, [r4, #9]
	adds	r0, r1, r0
	ldrb	r0, [r0, #0]
	ldr	r2, [sp, #12]
	strb	r0, [r2, #0]
	movs	r1, #128
	adds	r0, r1, #0
	ldrb	r2, [r4, #26]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L_081c2f20
	ldr	r0, [sp, #8]
	strb	r1, [r0, #0]
	ldrb	r0, [r4, #26]
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
	movs	r0, #127
	ldrb	r2, [r4, #26]
	ands	r0, r2
	strb	r0, [r4, #26]
	b.n	.L_081c2f20
	movs	r0, r0
	.4byte 0x04000081
	.2byte 0x36a0
	.2byte 0x081c
.L_081c2eec:
	movs	r0, #15
	mov	r1, r8
	ands	r1, r0
	mov	r8, r1
	ldrb	r2, [r4, #9]
	lsls	r0, r2, #4
	add	r0, r8
	ldr	r1, [sp, #12]
	strb	r0, [r1, #0]
	movs	r2, #128
	ldrb	r0, [r4, #26]
	orrs	r0, r2
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
	cmp	r6, #1
	bne.n	.L_081c2f20
	ldr	r0, [sp, #8]
	ldrb	r1, [r0, #0]
	movs	r0, #8
	ands	r0, r1
	cmp	r0, #0
	bne.n	.L_081c2f20
	ldrb	r0, [r4, #26]
	orrs	r0, r2
	ldr	r1, [sp, #20]
	strb	r0, [r1, #0]
.L_081c2f20:
	movs	r0, #0
	strb	r0, [r4, #29]
.L_081c2f24:
	mov	r6, sl
	mov	r4, r9
	cmp	r6, #4
	bgt.n	.L_081c2f2e
	b.n	.L_081c2b24
.L_081c2f2e:
	add	sp, #28
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
