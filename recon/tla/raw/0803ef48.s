.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_0803d4e4, 0x0803d4e4
	.set sub_0803deac, 0x0803deac
	.set sub_0803f2e4, 0x0803f2e4
	.set sub_0803f6c8, 0x0803f6c8
	.set sub_081180e0, 0x081180e0
	.global Func_0803ef48
	.thumb_func
Func_0803ef48:
	push	{lr}
	movs	r3, #210
	lsls	r3, r3, #2
	adds	r0, r0, r3
	sub	sp, #12
	ldr	r2, [r0, #0]
	cmp	r1, #0
	beq.n	.L_0803ef60
.L_0803ef58:
	subs	r1, #1
	ldr	r2, [r2, #4]
	cmp	r1, #0
	bne.n	.L_0803ef58
.L_0803ef60:
	ldrh	r3, [r2, #10]
	cmp	r3, #1
	beq.n	.L_0803ef6a
	cmp	r3, #6
	bne.n	.L_0803ef82
.L_0803ef6a:
	ldrh	r0, [r2, #32]
	ldr	r3, [pc, #24]
	movs	r1, #1
	subs	r0, r0, r3
	ldrh	r3, [r2, #12]
	str	r1, [sp, #0]
	str	r3, [sp, #8]
	add	r2, sp, #8
	add	r3, sp, #4
	movs	r1, #0
	bl	sub_0803d4e4
.L_0803ef82:
	add	sp, #12
	pop	{pc}
	movs	r0, r0
	.2byte 0x003a
	.2byte 0x0000
	push	{lr}
	movs	r3, #210
	lsls	r3, r3, #2
	adds	r0, r0, r3
	sub	sp, #12
	ldr	r2, [r0, #0]
	cmp	r1, #0
	beq.n	.L_0803efa4
.L_0803ef9c:
	subs	r1, #1
	ldr	r2, [r2, #4]
	cmp	r1, #0
	bne.n	.L_0803ef9c
.L_0803efa4:
	ldrh	r3, [r2, #10]
	cmp	r3, #1
	beq.n	.L_0803efae
	cmp	r3, #6
	bne.n	.L_0803efca
.L_0803efae:
	ldrh	r0, [r2, #32]
	ldr	r3, [pc, #28]
	movs	r1, #1
	subs	r0, r0, r3
	ldrh	r3, [r2, #12]
	str	r1, [sp, #0]
	str	r3, [sp, #8]
	add	r2, sp, #8
	add	r3, sp, #4
	movs	r1, #0
	bl	sub_0803d4e4
	bl	sub_0803f6c8
.L_0803efca:
	add	sp, #12
	pop	{pc}
	movs	r0, r0
	.2byte 0x003a
	.2byte 0x0000
	push	{lr}
	movs	r3, #210
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	sub	sp, #12
	movs	r2, #0
	cmp	r0, #0
	beq.n	.L_0803efee
.L_0803efe6:
	ldr	r0, [r0, #4]
	adds	r2, #1
	cmp	r0, #0
	bne.n	.L_0803efe6
.L_0803efee:
	ldr	r3, [pc, #16]
	mov	r0, sp
	lsls	r2, r2, #1
	strh	r3, [r0, r2]
	movs	r1, #0
	bl	sub_081180e0
	add	sp, #12
	pop	{pc}
	.2byte 0x00ff
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	sl, r1
	cmp	r1, #0
	beq.n	.L_0803f0e6
	movs	r0, #231
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	movs	r1, #221
	adds	r3, #4
	lsls	r2, r3, #1
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldrh	r3, [r7, r3]
	movs	r4, #213
	lsls	r4, r4, #2
	mov	r8, r3
	movs	r0, #0
	adds	r3, r2, r4
	ldrh	r6, [r7, r3]
	bl	sub_0803deac
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0803f040
	b.n	.L_0803f1ca
.L_0803f040:
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	r1, r8
	movs	r3, #0
	bl	sub_0803f2e4
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #150
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	movs	r4, #230
	adds	r3, r2, #0
	adds	r3, #80
	strh	r3, [r5, #16]
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldrh	r3, [r3, #0]
	adds	r2, #64
	strh	r2, [r5, #24]
	movs	r2, #255
	strh	r3, [r5, #18]
	strh	r3, [r5, #26]
	lsls	r2, r2, #8
	movs	r3, #32
	strh	r3, [r5, #36]
	adds	r2, #254
	strh	r3, [r5, #34]
	adds	r3, #224
	strh	r3, [r5, #38]
	strh	r2, [r5, #20]
	subs	r4, #80
	adds	r3, r7, r4
	adds	r0, r5, #0
	ldr	r5, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #224
	strh	r3, [r5, #36]
	ldrh	r3, [r5, #16]
	movs	r1, #0
	subs	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	strh	r1, [r5, #38]
	strh	r2, [r5, #20]
	cmp	r3, #0
	beq.n	.L_0803f0b0
.L_0803f0a0:
	adds	r5, r3, #0
	ldrh	r3, [r5, #16]
	strh	r2, [r5, #20]
	subs	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0803f0a0
.L_0803f0b0:
	movs	r3, #0
	str	r0, [r5, #4]
	str	r3, [r0, #4]
	str	r5, [r0, #0]
	movs	r0, #210
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r5, [r3, #0]
.L_0803f0c0:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #34
	ldrsh	r6, [r5, r1]
	cmp	r6, #0
	bne.n	.L_0803f0c0
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r2, [r5, #4]
	str	r2, [r3, #0]
	ldrh	r0, [r5, #12]
	bl	sub_08014274
	strh	r6, [r5, #10]
	ldr	r5, [r5, #4]
	str	r6, [r5, #0]
	b.n	.L_0803f1ca
.L_0803f0e6:
	movs	r4, #231
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldrh	r3, [r3, #0]
	movs	r0, #221
	lsls	r2, r3, #1
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r7, r3]
	movs	r1, #213
	lsls	r1, r1, #2
	mov	r8, r3
	movs	r0, #0
	adds	r3, r2, r1
	ldrh	r6, [r7, r3]
	bl	sub_0803deac
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0803f1ca
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	r1, r8
	movs	r3, #0
	bl	sub_0803f2e4
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #150
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #240
	adds	r3, r2, r4
	strh	r3, [r5, #16]
	movs	r0, #230
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	movs	r1, #128
	strh	r3, [r5, #18]
	strh	r3, [r5, #26]
	movs	r3, #2
	strh	r3, [r5, #20]
	lsls	r1, r1, #9
	movs	r3, #32
	strh	r3, [r5, #34]
	strh	r3, [r5, #36]
	adds	r2, r2, r1
	adds	r3, #224
	strh	r2, [r5, #24]
	strh	r3, [r5, #38]
	movs	r4, #210
	lsls	r4, r4, #2
	adds	r2, r7, r4
	adds	r3, r5, #0
	ldr	r5, [r2, #0]
	mov	r0, sl
	str	r3, [r5, #0]
	str	r5, [r3, #4]
	str	r0, [r3, #0]
	str	r3, [r2, #0]
	adds	r5, r3, #0
	ldrh	r3, [r5, #16]
	movs	r2, #2
	adds	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	strh	r2, [r5, #20]
	cmp	r3, #0
	beq.n	.L_0803f186
.L_0803f176:
	adds	r5, r3, #0
	ldrh	r3, [r5, #16]
	strh	r2, [r5, #20]
	adds	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0803f176
.L_0803f186:
	movs	r3, #0
	strh	r3, [r5, #38]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #224
	strh	r3, [r5, #36]
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r5, [r3, #0]
	movs	r6, #128
	lsls	r6, r6, #1
.L_0803f19e:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #34
	ldrsh	r3, [r5, r2]
	cmp	r3, r6
	bne.n	.L_0803f19e
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq.n	.L_0803f1bc
.L_0803f1b2:
	adds	r5, r2, #0
	ldr	r3, [r5, #4]
	adds	r2, r3, #0
	cmp	r3, #0
	bne.n	.L_0803f1b2
.L_0803f1bc:
	ldrh	r0, [r5, #12]
	bl	sub_08014274
	ldr	r3, [r5, #0]
	movs	r2, #0
	strh	r2, [r5, #10]
	str	r2, [r3, #4]
.L_0803f1ca:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
