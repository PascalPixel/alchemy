.syntax unified
	.thumb
	.set sub_081c1750, 0x081c1750
	.set sub_081c1752, 0x081c1752
	.set sub_081c1bd8, 0x081c1bd8
	.set sub_081c1e98, 0x081c1e98
	.set sub_081c2314, 0x081c2314
	.set sub_081c2328, 0x081c2328
	.set sub_081c2818, 0x081c2818
	.set sub_081c28e0, 0x081c28e0
	.global Overlay_081c189c
Overlay_081c189c:
	mov	ip, lr
	bl	sub_081c1750
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L_081c18b2
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_081c18b2:
	bx	ip
	mov	ip, lr
	bl	sub_081c1750
	subs	r3, #64
	strb	r3, [r1, #12]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	ldr	r0, [pc, #8]
	adds	r0, r0, r3
	bl	sub_081c1752
	strb	r3, [r0, #0]
	bx	ip
	.2byte 0x0060
	.2byte 0x0400
	ldr	r0, [pc, #680]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #680]
	ldr	r3, [r0, #0]
	subs	r3, r3, r2
	cmp	r3, #1
	bhi.n	.L_081c1920
	ldrb	r1, [r0, #4]
	subs	r1, #1
	strb	r1, [r0, #4]
	bgt.n	.L_081c1920
	ldrb	r1, [r0, #11]
	strb	r1, [r0, #4]
	ldr	r2, [pc, #40]
	ldr	r1, [r2, #8]
	lsls	r1, r1, #7
	bcc.n	.L_081c1906
	ldr	r1, [pc, #36]
	str	r1, [r2, #8]
.L_081c1906:
	ldr	r1, [r2, #20]
	lsls	r1, r1, #7
	bcc.n	.L_081c1910
	ldr	r1, [pc, #24]
	str	r1, [r2, #20]
.L_081c1910:
	movs	r1, #4
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
	movs	r1, #182
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
.L_081c1920:
	bx	lr
	movs	r0, r0
	.4byte 0x040000bc
	.2byte 0x0004
	.2byte 0x8440
	ldr	r2, [pc, #608]
	ldr	r3, [r0, #52]
	cmp	r2, r3
	beq.n	.L_081c1936
	bx	lr
.L_081c1936:
	adds	r3, #1
	str	r3, [r0, #52]
	push	{r0, lr}
	ldr	r3, [r0, #56]
	cmp	r3, #0
	beq.n	.L_081c1948
	ldr	r0, [r0, #60]
	bl	.L_081c1b84
.L_081c1948:
	pop	{r0}
	push	{r4, r5, r6, r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L_081c1960
	b.n	.L_081c1b74
.L_081c1960:
	ldr	r0, [pc, #552]
	ldr	r0, [r0, #0]
	mov	r8, r0
	adds	r0, r7, #0
	bl	sub_081c2818
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L_081c1974
	b.n	.L_081c1b74
.L_081c1974:
	ldrh	r0, [r7, #34]
	ldrh	r1, [r7, #32]
	adds	r0, r0, r1
	b.n	.L_081c1ac4
.L_081c197c:
	ldrb	r6, [r7, #8]
	ldr	r5, [r7, #44]
	movs	r3, #1
	movs	r4, #0
.L_081c1984:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	bne.n	.L_081c198e
	b.n	.L_081c1aa0
.L_081c198e:
	mov	sl, r3
	orrs	r4, r3
	mov	fp, r4
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c19c2
.L_081c199a:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c19b6
	ldrb	r0, [r4, #16]
	cmp	r0, #0
	beq.n	.L_081c19bc
	subs	r0, #1
	strb	r0, [r4, #16]
	bne.n	.L_081c19bc
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
	b.n	.L_081c19bc
.L_081c19b6:
	adds	r0, r4, #0
	bl	sub_081c2314
.L_081c19bc:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c199a
.L_081c19c2:
	ldrb	r3, [r5, #0]
	movs	r0, #64
	tst	r0, r3
	beq.n	.L_081c1a40
	adds	r0, r5, #0
	bl	sub_081c2328
	movs	r0, #128
	strb	r0, [r5, #0]
	movs	r0, #2
	strb	r0, [r5, #15]
	movs	r0, #64
	strb	r0, [r5, #19]
	movs	r0, #22
	strb	r0, [r5, #25]
	movs	r0, #1
	adds	r1, r5, #6
	strb	r0, [r1, #30]
	b.n	.L_081c1a40
.L_081c19e8:
	ldr	r2, [r5, #64]
	ldrb	r1, [r2, #0]
	cmp	r1, #128
	bcs.n	.L_081c19f4
	ldrb	r1, [r5, #7]
	b.n	.L_081c19fe
.L_081c19f4:
	adds	r2, #1
	str	r2, [r5, #64]
	cmp	r1, #189
	bcc.n	.L_081c19fe
	strb	r1, [r5, #7]
.L_081c19fe:
	cmp	r1, #207
	bcc.n	.L_081c1a14
	mov	r0, r8
	ldr	r3, [r0, #56]
	adds	r0, r1, #0
	subs	r0, #207
	adds	r1, r7, #0
	adds	r2, r5, #0
	bl	.L_081c1b84
	b.n	.L_081c1a40
.L_081c1a14:
	cmp	r1, #176
	bls.n	.L_081c1a36
	adds	r0, r1, #0
	subs	r0, #177
	strb	r0, [r7, #10]
	mov	r3, r8
	ldr	r3, [r3, #52]
	lsls	r0, r0, #2
	ldr	r3, [r3, r0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	.L_081c1b84
	ldrb	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_081c1a9c
	b.n	.L_081c1a40
.L_081c1a36:
	ldr	r0, [pc, #336]
	subs	r1, #128
	adds	r1, r1, r0
	ldrb	r0, [r1, #0]
	strb	r0, [r5, #1]
.L_081c1a40:
	ldrb	r0, [r5, #1]
	cmp	r0, #0
	beq.n	.L_081c19e8
	subs	r0, #1
	strb	r0, [r5, #1]
	ldrb	r1, [r5, #25]
	cmp	r1, #0
	beq.n	.L_081c1a9c
	ldrb	r0, [r5, #23]
	cmp	r0, #0
	beq.n	.L_081c1a9c
	ldrb	r0, [r5, #28]
	cmp	r0, #0
	beq.n	.L_081c1a62
	subs	r0, #1
	strb	r0, [r5, #28]
	b.n	.L_081c1a9c
.L_081c1a62:
	ldrb	r0, [r5, #26]
	adds	r0, r0, r1
	strb	r0, [r5, #26]
	adds	r1, r0, #0
	subs	r0, #64
	lsls	r0, r0, #24
	bpl.n	.L_081c1a76
	lsls	r2, r1, #24
	asrs	r2, r2, #24
	b.n	.L_081c1a7a
.L_081c1a76:
	movs	r0, #128
	subs	r2, r0, r1
.L_081c1a7a:
	ldrb	r0, [r5, #23]
	muls	r0, r2
	asrs	r2, r0, #6
	ldrb	r0, [r5, #22]
	eors	r0, r2
	lsls	r0, r0, #24
	beq.n	.L_081c1a9c
	strb	r2, [r5, #22]
	ldrb	r0, [r5, #0]
	ldrb	r1, [r5, #24]
	cmp	r1, #0
	bne.n	.L_081c1a96
	movs	r1, #12
	b.n	.L_081c1a98
.L_081c1a96:
	movs	r1, #3
.L_081c1a98:
	orrs	r0, r1
	strb	r0, [r5, #0]
.L_081c1a9c:
	mov	r3, sl
	mov	r4, fp
.L_081c1aa0:
	subs	r6, #1
	ble.n	.L_081c1aac
	movs	r0, #80
	adds	r5, r5, r0
	lsls	r3, r3, #1
	b.n	.L_081c1984
.L_081c1aac:
	ldr	r0, [r7, #12]
	adds	r0, #1
	str	r0, [r7, #12]
	cmp	r4, #0
	bne.n	.L_081c1abe
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	b.n	.L_081c1b74
.L_081c1abe:
	str	r4, [r7, #4]
	ldrh	r0, [r7, #34]
	subs	r0, #150
.L_081c1ac4:
	strh	r0, [r7, #34]
	cmp	r0, #150
	bcc.n	.L_081c1acc
	b.n	.L_081c197c
.L_081c1acc:
	ldrb	r2, [r7, #8]
	ldr	r5, [r7, #44]
.L_081c1ad0:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	beq.n	.L_081c1b6a
	movs	r1, #15
	tst	r1, r0
	beq.n	.L_081c1b6a
	mov	r9, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_081c28e0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c1b60
.L_081c1aee:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	bne.n	.L_081c1afe
	adds	r0, r4, #0
	bl	sub_081c2314
	b.n	.L_081c1b5a
.L_081c1afe:
	ldrb	r0, [r4, #1]
	movs	r6, #7
	ands	r6, r0
	ldrb	r3, [r5, #0]
	movs	r0, #3
	tst	r0, r3
	beq.n	.L_081c1b1c
	bl	sub_081c1bd8
	cmp	r6, #0
	beq.n	.L_081c1b1c
	ldrb	r0, [r4, #29]
	movs	r1, #1
	orrs	r0, r1
	strb	r0, [r4, #29]
.L_081c1b1c:
	ldrb	r3, [r5, #0]
	movs	r0, #12
	tst	r0, r3
	beq.n	.L_081c1b5a
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r2, r1, r0
	bpl.n	.L_081c1b30
	movs	r2, #0
.L_081c1b30:
	cmp	r6, #0
	beq.n	.L_081c1b4e
	mov	r0, r8
	ldr	r3, [r0, #48]
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	adds	r0, r6, #0
	bl	.L_081c1b84
	str	r0, [r4, #32]
	ldrb	r0, [r4, #29]
	movs	r1, #2
	orrs	r0, r1
	strb	r0, [r4, #29]
	b.n	.L_081c1b5a
.L_081c1b4e:
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	ldr	r0, [r4, #36]
	bl	sub_081c1e98
	str	r0, [r4, #32]
.L_081c1b5a:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c1aee
.L_081c1b60:
	ldrb	r0, [r5, #0]
	movs	r1, #240
	ands	r0, r1
	strb	r0, [r5, #0]
	mov	r2, r9
.L_081c1b6a:
	subs	r2, #1
	ble.n	.L_081c1b74
	movs	r0, #80
	adds	r5, r5, r0
	bgt.n	.L_081c1ad0
.L_081c1b74:
	ldr	r0, [pc, #24]
	str	r0, [r7, #52]
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
.L_081c1b84:
	bx	r3
