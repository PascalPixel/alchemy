.syntax unified
	.thumb
	.set sub_0803a404, 0x0803a404
	.set sub_0803a9b2, 0x0803a9b2
	.set sub_0803aa3e, 0x0803aa3e
	.set sub_0803bde4, 0x0803bde4
	.global Func_0803a8fc
	.thumb_func
Func_0803a8fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #388
	str	r1, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	lsls	r3, r0, #8
	asrs	r3, r3, #16
	mov	sl, r3
	movs	r3, #255
	ands	r3, r0
	ldr	r2, [pc, #372]
	subs	r3, #32
	lsls	r3, r3, #5
	adds	r7, r3, r2
	ldrh	r0, [r7, #0]
	movs	r1, #192
	ldr	r3, [pc, #364]
	mov	r8, r0
	lsls	r1, r1, #1
	add	r0, sp, #4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x7933
	adds	r7, #2
	cmp	r3, #0
	beq.n	.L_0803a946
	movs	r2, #0
	movs	r3, #8
	mov	r9, r2
	b.n	.L_0803a954
.L_0803a946:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #62
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	movs	r0, #1
	mov	r9, r0
.L_0803a954:
	mov	fp, r3
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #60
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0803a99a
	mov	r1, sp
	ldr	r5, [pc, #308]
	adds	r1, #53
	mov	r2, r9
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4669
	adds	r1, #54
	mov	r2, r9
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465a
	add	r1, sp, #36
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4669
	mov	r2, fp
	adds	r1, #37
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2201
	add	r8, r2
	b.n	sub_0803a9b2
.L_0803a99a:
	mov	r1, sp
	adds	r1, #53
	mov	r2, r9
	ldr	r5, [pc, #248]
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c38
	add	r1, sp, #36
	mov	r2, fp
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4650
	lsls	r3, r0, #16
	lsrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L_0803aa46
	ldr	r3, [pc, #224]
	movs	r0, #240
	lsls	r2, r2, #5
	lsls	r0, r0, #4
	adds	r7, r2, r3
	adds	r0, #60
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	mov	sl, r2
	adds	r7, #2
	cmp	r3, #1
	bne.n	.L_0803aa20
	add	r3, sp, #4
	mov	r2, r8
	adds	r6, r3, r2
	adds	r1, r6, #0
	ldr	r5, [pc, #184]
	adds	r1, #49
	mov	r2, r9
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	adds	r1, #50
	mov	r2, r9
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	adds	r1, #32
	mov	r2, fp
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c31
	mov	r2, fp
	adds	r1, #33
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4650
	movs	r2, #128
	lsls	r3, r0, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	asrs	r3, r3, #16
	mov	sl, r3
	b.n	sub_0803aa3e
.L_0803aa20:
	add	r5, sp, #4
	add	r5, r8
	adds	r1, r5, #0
	adds	r1, #49
	mov	r2, r9
	ldr	r6, [pc, #112]
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c29
	adds	r1, #32
	adds	r0, r7, #0
	mov	r2, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4650
	lsls	r3, r0, #16
	lsrs	r3, r3, #16
	add	r8, r3
.L_0803aa46:
	mov	r1, sp
	adds	r1, #11
	movs	r6, #0
.L_0803aa4c:
	movs	r5, #0
.L_0803aa4e:
	movs	r4, #0
.L_0803aa50:
	movs	r2, #0
	movs	r0, #7
.L_0803aa54:
	ldrb	r3, [r1, #0]
	lsls	r2, r2, #4
	subs	r0, #1
	adds	r2, r2, r3
	subs	r1, #1
	cmp	r0, #0
	bge.n	.L_0803aa54
	ldr	r0, [sp, #0]
	adds	r4, #1
	stmia	r0!, {r2}
	adds	r1, #24
	adds	r3, r0, #0
	str	r3, [sp, #0]
	cmp	r4, #7
	ble.n	.L_0803aa50
	adds	r5, #1
	subs	r1, #120
	cmp	r5, #1
	ble.n	.L_0803aa4e
	adds	r6, #1
	adds	r1, #112
	cmp	r6, #1
	ble.n	.L_0803aa4c
	mov	r0, r8
	add	sp, #388
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805a4e0
	.4byte 0x03000258
	.4byte 0x03000314
	.2byte 0xa0e0
	.2byte 0x0805
	push	{lr}
	ldrh	r2, [r0, #0]
	movs	r1, #0
	adds	r0, #2
	cmp	r2, #0
	beq.n	.L_0803aada
	ldr	r4, [pc, #44]
.L_0803aab2:
	cmp	r2, #32
	bne.n	.L_0803aaba
	adds	r1, #4
	b.n	.L_0803aad2
.L_0803aaba:
	cmp	r2, #255
	bhi.n	.L_0803aad0
	adds	r3, r2, #0
	subs	r3, #222
	cmp	r3, #1
	bls.n	.L_0803aad2
	adds	r3, #190
	lsls	r3, r3, #5
	ldrh	r3, [r3, r4]
	adds	r1, r1, r3
	b.n	.L_0803aad2
.L_0803aad0:
	adds	r1, #10
.L_0803aad2:
	ldrh	r2, [r0, #0]
	adds	r0, #2
	cmp	r2, #0
	bne.n	.L_0803aab2
.L_0803aada:
	adds	r0, r1, #0
	pop	{pc}
	movs	r0, r0
	.2byte 0xa4e0
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r7, r2, #0
	ldr	r6, [r3, #60]
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	adds	r5, r0, #0
	sub	sp, #4
	mov	sl, r1
	movs	r4, #0
	mov	r9, r3
	cmp	r5, #0
	beq.n	.L_0803ab0c
	b.n	.L_0803ac3a
.L_0803ab0c:
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #66
	adds	r1, r6, r3
	ldrh	r3, [r1, #0]
	movs	r2, #244
	lsls	r2, r2, #4
	lsls	r3, r3, #1
	adds	r3, r3, r2
	strh	r4, [r6, r3]
	adds	r5, r6, r2
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #8]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_0803ac3a
	movs	r0, r0
	.2byte 0x01ff
	.2byte 0x0000
.L_0803ab34:
	cmp	r4, #30
	bls.n	.L_0803ab3c
	cmp	r4, #176
	bne.n	.L_0803abee
.L_0803ab3c:
	subs	r1, r4, #3
	cmp	r1, #26
	bls.n	.L_0803ab44
	b.n	.L_0803ac3a
.L_0803ab44:
	ldr	r2, [pc, #272]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0803abe2
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803abdc
	.4byte 0x0803abb8
	.4byte 0x0803abc2
	.4byte 0x0803abcc
	.4byte 0x0803abd8
	.4byte 0x0803abd8
	.4byte 0x0803ac3a
	.4byte 0x0803abea
	.4byte 0x0803abea
	.4byte 0x0803ac3a
	.4byte 0x0803abd8
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803ac3a
	.4byte 0x0803abea
	.2byte 0xabd8
	.2byte 0x0803
	movs	r1, #240
	ldrh	r2, [r5, #0]
	lsls	r1, r1, #4
	adds	r1, #62
	b.n	.L_0803abd4
	movs	r1, #240
	ldrh	r2, [r5, #0]
	lsls	r1, r1, #4
	adds	r1, #60
	b.n	.L_0803abd4
	movs	r1, #240
	ldrh	r2, [r5, #0]
	lsls	r1, r1, #4
	adds	r1, #56
.L_0803abd4:
	adds	r3, r6, r1
	strh	r2, [r3, #0]
.L_0803abd8:
	adds	r5, #2
	b.n	.L_0803ac3a
	bl	sub_0803a404
	b.n	.L_0803ac3a
	movs	r2, #15
	mov	r7, r9
	add	r8, r2
	b.n	.L_0803ac3a
	adds	r5, #2
	b.n	.L_0803abd8
.L_0803abee:
	mov	r3, sl
	ldrh	r2, [r3, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0803ac28
	ldrh	r0, [r5, #0]
	cmp	r4, #32
	bls.n	.L_0803ac28
	cmp	r0, #32
	bls.n	.L_0803ac28
	ldr	r1, [pc, #84]
	adds	r3, r4, #0
	adds	r2, r0, #0
	subs	r3, #32
	subs	r2, #32
	lsls	r3, r3, #5
	lsls	r2, r2, #5
	ldrh	r3, [r1, r3]
	ldrh	r2, [r1, r2]
	movs	r1, #240
	adds	r3, r3, r2
	lsls	r3, r3, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bhi.n	.L_0803ac28
	lsls	r3, r0, #8
	orrs	r4, r3
	adds	r5, #2
.L_0803ac28:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r7, #0
	mov	r0, sl
	adds	r1, r4, #0
	mov	r3, r8
	bl	sub_0803bde4
	adds	r7, r7, r0
.L_0803ac3a:
	ldrh	r4, [r5, #0]
	adds	r5, #2
	cmp	r4, #255
	bls.n	.L_0803ac44
	movs	r4, #64
.L_0803ac44:
	cmp	r4, #0
	beq.n	.L_0803ac4a
	b.n	.L_0803ab34
.L_0803ac4a:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0803ab4c
	.4byte 0x0805a4e0
