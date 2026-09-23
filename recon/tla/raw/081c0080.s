.syntax unified
	.thumb
	.set sub_081bff90, 0x081bff90
	.set sub_081bff98, 0x081bff98
	.set sub_081c0198, 0x081c0198
	.set sub_081c01ec, 0x081c01ec
	.set sub_081c04ac, 0x081c04ac
	.set sub_081c0928, 0x081c0928
	.set sub_081c0ef4, 0x081c0ef4
	.global Func_081c0080
	.thumb_func
Func_081c0080:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1001
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0e31
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1189
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x11ad
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x11cd
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0c19
	.2byte 0x081c
	mov	r3, sl
	mov	r4, fp
	subs	r6, #1
	ble.n	.L_081c00c0
	movs	r0, #80
	adds	r5, r5, r0
	lsls	r3, r3, #1
	b.n	sub_081bff98
.L_081c00c0:
	ldr	r0, [r7, #12]
	adds	r0, #1
	str	r0, [r7, #12]
	cmp	r4, #0
	bne.n	.L_081c00d2
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	b.n	.L_081c0188
.L_081c00d2:
	str	r4, [r7, #4]
	ldrh	r0, [r7, #34]
	subs	r0, #150
	strh	r0, [r7, #34]
	cmp	r0, #150
	bcc.n	.L_081c00e0
	b.n	sub_081bff90
.L_081c00e0:
	ldrb	r2, [r7, #8]
	ldr	r5, [r7, #44]
.L_081c00e4:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	beq.n	.L_081c017e
	movs	r1, #15
	tst	r1, r0
	beq.n	.L_081c017e
	mov	r9, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_081c0ef4
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c0174
.L_081c0102:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	bne.n	.L_081c0112
	adds	r0, r4, #0
	bl	sub_081c0928
	b.n	.L_081c016e
.L_081c0112:
	ldrb	r0, [r4, #1]
	movs	r6, #7
	ands	r6, r0
	ldrb	r3, [r5, #0]
	movs	r0, #3
	tst	r0, r3
	beq.n	.L_081c0130
	bl	sub_081c01ec
	cmp	r6, #0
	beq.n	.L_081c0130
	ldrb	r0, [r4, #29]
	movs	r1, #1
	orrs	r0, r1
	strb	r0, [r4, #29]
.L_081c0130:
	ldrb	r3, [r5, #0]
	movs	r0, #12
	tst	r0, r3
	beq.n	.L_081c016e
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r2, r1, r0
	bpl.n	.L_081c0144
	movs	r2, #0
.L_081c0144:
	cmp	r6, #0
	beq.n	.L_081c0162
	mov	r0, r8
	ldr	r3, [r0, #48]
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	adds	r0, r6, #0
	bl	sub_081c0198
	str	r0, [r4, #32]
	ldrb	r0, [r4, #29]
	movs	r1, #2
	orrs	r0, r1
	strb	r0, [r4, #29]
	b.n	.L_081c016e
.L_081c0162:
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	ldr	r0, [r4, #36]
	bl	sub_081c04ac
	str	r0, [r4, #32]
.L_081c016e:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c0102
.L_081c0174:
	ldrb	r0, [r5, #0]
	movs	r1, #240
	ands	r0, r1
	strb	r0, [r5, #0]
	mov	r2, r9
.L_081c017e:
	subs	r2, #1
	ble.n	.L_081c0188
	movs	r0, #80
	adds	r5, r5, r0
	bgt.n	.L_081c00e4
.L_081c0188:
	ldr	r0, [pc, #24]
	str	r0, [r7, #52]
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
