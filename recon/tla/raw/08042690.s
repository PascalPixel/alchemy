.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_080149f0, 0x080149f0
	.set sub_08014d00, 0x08014d00
	.set sub_08016ca4, 0x08016ca4
	.set sub_0803911c, 0x0803911c
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08039418, 0x08039418
	.set sub_0803a084, 0x0803a084
	.set sub_0803c378, 0x0803c378
	.set sub_08041f70, 0x08041f70
	.set sub_080420fc, 0x080420fc
	.set sub_08042274, 0x08042274
	.set sub_08042630, 0x08042630
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_081180c8, 0x081180c8
	.set sub_08118130, 0x08118130
	.global Func_08042690
	.thumb_func
Func_08042690:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	movs	r1, #16
	movs	r0, #64
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r3, #0
	mov	sl, r3
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r6, #6]
	mov	r0, r8
	bl	sub_08042630
	ldrh	r3, [r5, #10]
	ldrh	r1, [r5, #6]
	ldrh	r2, [r5, #8]
	movs	r4, #6
	ldrh	r0, [r5, #4]
	str	r4, [sp, #0]
	bl	sub_08039260
	str	r0, [r5, #0]
	mov	r0, r8
	bl	.L_0804297c
	mov	r3, sl
	strb	r3, [r6, #6]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
.L_080426e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	adds	r5, r0, #0
	mov	ip, r3
	movs	r3, #0
	mov	fp, r3
	ldrh	r3, [r5, #8]
	movs	r2, #1
	subs	r3, #1
	mov	r9, r3
	adds	r3, r1, #0
	mov	r7, ip
	ands	r3, r2
	sub	sp, #4
	ldrb	r4, [r7, #5]
	ldrh	r6, [r5, #10]
	cmp	r3, #0
	bne.n	.L_0804271a
	movs	r3, #3
	negs	r3, r3
	ands	r1, r3
.L_0804271a:
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08042728
	movs	r1, #5
	mov	fp, r1
	movs	r2, #0
.L_08042728:
	ldr	r7, [pc, #92]
	adds	r0, r2, #0
	adds	r2, r7, #0
	ldrsb	r3, [r2, r0]
	cmp	r3, #0
	blt.n	.L_080427a6
.L_08042734:
	ldrsb	r3, [r2, r0]
	mov	r2, fp
	adds	r4, r3, r2
	cmp	r4, r9
	bcs.n	.L_08042798
	movs	r1, #0
	cmp	r6, #0
	beq.n	.L_08042798
	movs	r3, #240
	lsls	r3, r3, #8
	subs	r7, r6, #1
	movs	r2, #240
	adds	r3, #24
	lsls	r2, r2, #8
	str	r7, [sp, #0]
	mov	sl, r3
	adds	r2, #25
	subs	r3, #9
	mov	r8, r2
	mov	lr, r3
.L_0804275c:
	movs	r7, #14
	ldrsh	r2, [r5, r7]
	movs	r7, #12
	ldrsh	r3, [r5, r7]
	adds	r2, r2, r1
	lsls	r2, r2, #6
	adds	r3, r3, r4
	add	r2, ip
	lsls	r3, r3, #1
	adds	r2, r2, r3
	adds	r2, #8
	cmp	r1, #0
	bne.n	.L_0804277c
	mov	r3, sl
	strh	r3, [r2, #0]
	b.n	.L_08042790
.L_0804277c:
	ldr	r7, [sp, #0]
	cmp	r1, r7
	bne.n	.L_0804278c
	mov	r3, r8
	strh	r3, [r2, #0]
	b.n	.L_08042790
	.2byte 0xf574
	.2byte 0x0805
.L_0804278c:
	mov	r7, lr
	strh	r7, [r2, #0]
.L_08042790:
	adds	r1, #1
	cmp	r1, r6
	bne.n	.L_0804275c
	ldr	r7, [pc, #88]
.L_08042798:
	adds	r0, #1
	adds	r2, r7, #0
	ldrsb	r3, [r2, r0]
	cmp	r3, #0
	bge.n	.L_08042734
	mov	r1, ip
	ldrb	r4, [r1, #5]
.L_080427a6:
	adds	r3, r4, #0
	cmp	r3, #0
	beq.n	.L_080427f4
	movs	r3, #14
	ldrsh	r2, [r5, r3]
	ldrh	r3, [r5, #10]
	movs	r4, #1
	adds	r2, r2, r3
	movs	r7, #12
	ldrsh	r3, [r5, r7]
	lsls	r2, r2, #6
	add	r2, ip
	lsls	r3, r3, #1
	adds	r1, r2, r3
	ldr	r3, [pc, #32]
	adds	r2, r1, #0
	subs	r2, #56
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r4, r9
	bcs.n	.L_080427dc
	ldr	r3, [pc, #20]
.L_080427d2:
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r4, r9
	bcc.n	.L_080427d2
.L_080427dc:
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_080427f4
	movs	r0, r0
	.4byte 0x0000f080
	.4byte 0x0000f081
	.4byte 0x0000f082
	.2byte 0xf574
	.2byte 0x0805
.L_080427f4:
	movs	r3, #1
	mov	r1, ip
	strb	r3, [r1, #3]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_08042808:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #16]
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	adds	r6, r0, #0
	mov	r0, r9
	str	r3, [sp, #8]
	str	r0, [sp, #4]
	adds	r5, r2, #0
	ldrb	r3, [r3, #5]
	cmp	r3, #0
	bne.n	.L_08042850
	bl	sub_080149f0
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #284]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #280]
	ldr	r2, [pc, #284]
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #0]
.L_08042850:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	ldr	r2, [sp, #16]
	movs	r1, #4
	adds	r2, r2, r3
	str	r2, [sp, #16]
	movs	r0, #14
	ldrsh	r3, [r6, r0]
	str	r1, [sp, #12]
	adds	r5, r5, r3
	lsls	r5, r5, #5
	str	r5, [sp, #0]
.L_08042868:
	ldr	r2, [sp, #0]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #8]
	adds	r3, r2, r0
	lsls	r3, r3, #1
	adds	r3, #8
	ldrh	r3, [r1, r3]
	ldr	r2, [pc, #244]
	mov	sl, r3
	ldr	r3, [pc, #244]
	mov	r0, sl
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r0, r3
	mov	r1, r9
	mov	r8, r2
	mov	sl, r0
	cmp	r1, #7
	ble.n	.L_0804289c
	ldr	r2, [pc, #224]
	ldr	r3, [pc, #224]
	mov	r8, r2
	mov	ip, r3
	b.n	.L_080428c8
.L_0804289c:
	mov	r0, r9
	cmp	r0, #0
	blt.n	.L_080428c8
	lsls	r1, r0, #2
	mov	r2, r8
	lsls	r2, r1
	ldr	r3, [pc, #200]
	mov	r8, r2
	movs	r2, #32
	subs	r2, r2, r1
	lsrs	r3, r2
	mov	r0, r8
	orrs	r0, r3
	mov	r3, ip
	lsls	r3, r1
	mov	ip, r3
	ldr	r3, [pc, #184]
	mov	r8, r0
	lsrs	r3, r2
	mov	r0, ip
	orrs	r0, r3
	mov	ip, r0
.L_080428c8:
	movs	r2, #192
	lsls	r2, r2, #19
	movs	r1, #0
	adds	r2, #28
	mov	lr, r1
	mov	fp, r2
	movs	r7, #0
	b.n	.L_08042926
.L_080428d8:
	mov	r3, sl
	lsls	r6, r3, #5
	mov	r0, fp
	subs	r3, r6, r7
	ldr	r4, [r3, r0]
	movs	r1, #0
	movs	r0, #0
	movs	r5, #15
.L_080428e8:
	adds	r2, r4, #0
	ands	r2, r5
	cmp	r2, #14
	bne.n	.L_080428fa
	lsls	r2, r1, #2
	adds	r3, r5, #0
	lsls	r3, r2
	mov	r2, r8
	b.n	.L_08042906
.L_080428fa:
	cmp	r2, #1
	bne.n	.L_0804290c
	lsls	r2, r1, #2
	adds	r3, r5, #0
	lsls	r3, r2
	mov	r2, ip
.L_08042906:
	ands	r3, r2
	orrs	r0, r3
	b.n	.L_08042912
.L_0804290c:
	lsls	r3, r1, #2
	lsls	r2, r3
	orrs	r0, r2
.L_08042912:
	adds	r1, #1
	lsrs	r4, r4, #4
	cmp	r1, #7
	ble.n	.L_080428e8
	subs	r3, r6, r7
	mov	r1, fp
	str	r0, [r3, r1]
	movs	r2, #1
	adds	r7, #4
	add	lr, r2
.L_08042926:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_08042934
	mov	r0, lr
	cmp	r0, #2
	ble.n	.L_080428d8
	b.n	.L_0804293a
.L_08042934:
	mov	r1, lr
	cmp	r1, #0
	ble.n	.L_080428d8
.L_0804293a:
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #16]
	movs	r2, #8
	negs	r2, r2
	subs	r3, #1
	adds	r0, #1
	add	r9, r2
	str	r3, [sp, #12]
	str	r0, [sp, #16]
	cmp	r3, #0
	bge.n	.L_08042868
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x050001c0
	.4byte 0x050001e8
	.4byte 0x050001dc
	.4byte 0x22222222
	.4byte 0xcccccccc
	.4byte 0x88888888
	.2byte 0xdddd
	.2byte 0xdddd
.L_0804297c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #64]
	ldr	r6, [r3, #36]
	ldr	r3, [r3, #60]
	sub	sp, #48
	movs	r1, #0
	mov	sl, r0
	ldr	r0, [r5, #0]
	str	r3, [sp, #28]
	str	r1, [sp, #20]
	mov	r9, r0
	ldrb	r3, [r3, #5]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_080429fa
	movs	r0, #0
	bl	sub_081180c8
	movs	r2, #1
	negs	r2, r2
	movs	r7, #0
	str	r0, [sp, #24]
	str	r2, [sp, #20]
	cmp	r7, r0
	bcs.n	.L_08042a46
	mov	r3, sp
	adds	r3, #36
	str	r3, [sp, #12]
	movs	r3, #88
	ldrh	r3, [r6, r3]
	ldr	r4, [sp, #12]
	movs	r0, #255
	strh	r3, [r4, #0]
	lsls	r0, r0, #16
	lsls	r3, r3, #16
	cmp	r3, r0
	beq.n	.L_08042a46
	ldr	r0, [sp, #12]
	adds	r2, r6, #0
	adds	r2, #88
	movs	r1, #0
.L_080429de:
	ldr	r3, [sp, #24]
	adds	r7, #1
	adds	r1, #2
	cmp	r7, r3
	bcs.n	.L_08042a46
	adds	r2, #2
	ldrh	r3, [r2, #0]
	movs	r4, #255
	strh	r3, [r1, r0]
	lsls	r4, r4, #16
	lsls	r3, r3, #16
	cmp	r3, r4
	bne.n	.L_080429de
	b.n	.L_08042a46
.L_080429fa:
	bl	sub_080ad0f0
	str	r0, [sp, #24]
	cmp	r0, #4
	bls.n	.L_08042a08
	movs	r0, #4
	str	r0, [sp, #24]
.L_08042a08:
	ldr	r1, [sp, #24]
	movs	r7, #0
	cmp	r7, r1
	bcs.n	.L_08042a38
	ldr	r3, [pc, #32]
	mov	r2, sp
	movs	r4, #134
	adds	r2, #36
	lsls	r4, r4, #2
	str	r2, [sp, #12]
	adds	r1, r2, #0
	adds	r2, r3, r4
.L_08042a20:
	ldrb	r3, [r2, #0]
	adds	r7, #1
	strh	r3, [r1, #0]
	ldr	r0, [sp, #24]
	adds	r2, #1
	adds	r1, #2
	cmp	r7, r0
	bcc.n	.L_08042a20
	b.n	.L_08042a3e
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
.L_08042a38:
	mov	r1, sp
	adds	r1, #36
	str	r1, [sp, #12]
.L_08042a3e:
	ldr	r3, [pc, #40]
	ldr	r4, [sp, #12]
	lsls	r2, r7, #1
	strh	r3, [r4, r2]
.L_08042a46:
	movs	r0, #1
	negs	r0, r0
	str	r7, [sp, #24]
	cmp	sl, r0
	bne.n	.L_08042a54
	ldrh	r1, [r5, #12]
	mov	sl, r1
.L_08042a54:
	movs	r3, #1
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08042a6c
	movs	r3, #3
	negs	r3, r3
	ands	r2, r3
	mov	sl, r2
	b.n	.L_08042a6c
	.2byte 0x00ff
	.2byte 0x0000
.L_08042a6c:
	ldr	r4, [sp, #28]
	ldrb	r3, [r4, #5]
	cmp	r3, #0
	beq.n	.L_08042a80
	movs	r0, #0
	movs	r1, #0
	bl	sub_08118130
	cmp	r0, #0
	bne.n	.L_08042a8a
.L_08042a80:
	movs	r3, #3
	mov	r0, sl
	negs	r3, r3
	ands	r0, r3
	mov	sl, r0
.L_08042a8a:
	mov	r1, sl
	cmp	r1, #9
	bne.n	.L_08042a9e
	ldrh	r0, [r5, #4]
	ldrh	r1, [r5, #6]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	sub_0803911c
	b.n	.L_08042d94
.L_08042a9e:
	ldr	r2, [sp, #28]
	movs	r3, #1
	strb	r3, [r2, #6]
	ldrh	r3, [r5, #12]
	cmp	r3, sl
	bne.n	.L_08042aba
	mov	r0, r9
	bl	sub_08039418
	mov	r0, r9
	mov	r1, sl
	bl	.L_080426e0
	b.n	.L_08042af2
.L_08042aba:
	ldrh	r1, [r5, #6]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	ldrh	r0, [r5, #4]
	bl	sub_0803911c
	mov	r0, sl
	bl	sub_08042630
	ldrh	r3, [r5, #8]
	mov	r4, r9
	strh	r3, [r4, #8]
	ldrh	r3, [r5, #10]
	mov	r0, r9
	strh	r3, [r0, #10]
	ldrh	r3, [r5, #4]
	mov	r1, r9
	strh	r3, [r1, #12]
	ldrh	r0, [r5, #4]
	ldrh	r1, [r5, #6]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	sub_0803a084
	mov	r0, r9
	mov	r1, sl
	bl	.L_080426e0
.L_08042af2:
	movs	r3, #2
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08042b00
	movs	r3, #5
	mov	r8, r3
.L_08042b00:
	ldr	r4, [sp, #24]
	movs	r7, #0
	cmp	r4, #0
	bne.n	.L_08042b0a
	b.n	.L_08042c3c
.L_08042b0a:
	ldr	r1, [sp, #20]
	mov	r0, sp
	lsls	r1, r1, #3
	adds	r0, #36
	movs	r4, #0
	mov	r3, r8
	movs	r2, #1
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r1, [sp, #16]
	str	r4, [sp, #4]
	add	r2, r8
	lsls	r3, r3, #3
	mov	fp, r2
	mov	r8, r3
.L_08042b28:
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #12]
	ldrh	r0, [r1, r2]
	bl	sub_08016ca4
	adds	r6, r0, #0
	movs	r3, #56
	ldrsh	r5, [r6, r3]
	movs	r4, #52
	ldrsh	r3, [r6, r4]
	cmp	r5, #0
	bne.n	.L_08042b48
	movs	r0, #2
	bl	sub_08041f70
	b.n	.L_08042b62
.L_08042b48:
	cmp	r3, #0
	bge.n	.L_08042b4e
	adds	r3, #3
.L_08042b4e:
	asrs	r3, r3, #2
	cmp	r5, r3
	bgt.n	.L_08042b5c
	movs	r0, #4
	bl	sub_08041f70
	b.n	.L_08042b62
.L_08042b5c:
	movs	r0, #15
	bl	sub_08041f70
.L_08042b62:
	ldr	r0, [sp, #28]
	movs	r3, #14
	strb	r3, [r0, #7]
	ldrb	r3, [r0, #5]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_08042b74
	movs	r3, #5
	strb	r3, [r0, #7]
.L_08042b74:
	ldr	r3, [sp, #16]
	str	r2, [sp, #0]
	mov	r1, r9
	mov	r2, r8
	adds	r3, #8
	adds	r0, r5, #0
	bl	sub_08042274
	ldr	r1, [sp, #28]
	movs	r3, #15
	strb	r3, [r1, #7]
	mov	r2, r8
	mov	r1, r9
	adds	r0, r6, #0
	ldr	r3, [sp, #16]
	bl	sub_080420fc
	movs	r0, #15
	bl	sub_08041f70
	movs	r2, #52
	ldrsh	r1, [r6, r2]
	cmp	r1, #0
	beq.n	.L_08042bca
	movs	r3, #56
	ldrsh	r5, [r6, r3]
	lsls	r0, r5, #2
	adds	r0, r0, r5
	lsls	r0, r0, #3
	bl	sub_08002054
	adds	r3, r0, #0
	cmp	r3, #0
	bne.n	.L_08042bbe
	cmp	r5, #0
	beq.n	.L_08042bbe
	movs	r3, #1
.L_08042bbe:
	ldr	r2, [sp, #20]
	mov	r0, r9
	mov	r1, fp
	adds	r2, #2
	bl	.L_08042808
.L_08042bca:
	movs	r2, #1
	mov	r3, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08042c24
	ldr	r4, [sp, #28]
	movs	r3, #14
	strb	r3, [r4, #7]
	ldrb	r3, [r4, #5]
	cmp	r3, #0
	beq.n	.L_08042be4
	movs	r3, #5
	strb	r3, [r4, #7]
.L_08042be4:
	ldr	r3, [sp, #8]
	movs	r1, #58
	ldrsh	r0, [r6, r1]
	adds	r3, #16
	str	r2, [sp, #0]
	mov	r1, r9
	mov	r2, r8
	bl	sub_08042274
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	cmp	r1, #0
	beq.n	.L_08042c24
	movs	r3, #58
	ldrsh	r5, [r6, r3]
	lsls	r0, r5, #2
	adds	r0, r0, r5
	lsls	r0, r0, #3
	bl	sub_08002054
	adds	r3, r0, #0
	cmp	r3, #0
	bne.n	.L_08042c18
	cmp	r5, #0
	beq.n	.L_08042c18
	movs	r3, #1
.L_08042c18:
	ldr	r2, [sp, #20]
	mov	r0, r9
	mov	r1, fp
	adds	r2, #3
	bl	.L_08042808
.L_08042c24:
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #24]
	movs	r4, #6
	movs	r0, #48
	adds	r1, #2
	adds	r7, #1
	add	fp, r4
	add	r8, r0
	str	r1, [sp, #4]
	cmp	r7, r2
	beq.n	.L_08042c3c
	b.n	.L_08042b28
.L_08042c3c:
	ldr	r4, [sp, #28]
	movs	r3, #15
	strb	r3, [r4, #7]
	ldrb	r3, [r4, #5]
	cmp	r3, #0
	bne.n	.L_08042c4a
	b.n	.L_08042d8e
.L_08042c4a:
	movs	r3, #2
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08042c56
	b.n	.L_08042d8e
.L_08042c56:
	movs	r3, #1
	ands	r3, r0
	ldr	r6, [sp, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_08042c64
	adds	r6, #1
.L_08042c64:
	add	r5, sp, #32
	adds	r1, r5, #0
	movs	r0, #0
	bl	sub_08118130
	movs	r1, #160
	lsls	r1, r1, #7
	adds	r1, #1
	mov	r0, r9
	movs	r2, #0
	adds	r3, r6, #0
	str	r7, [sp, #0]
	bl	sub_0803c378
	movs	r1, #160
	lsls	r1, r1, #7
	adds	r1, #2
	mov	r0, r9
	movs	r2, #2
	adds	r3, r6, #0
	str	r7, [sp, #0]
	bl	sub_0803c378
	adds	r2, r6, #1
	movs	r1, #160
	mov	r8, r2
	lsls	r1, r1, #7
	adds	r1, #3
	mov	r0, r9
	movs	r2, #0
	mov	r3, r8
	str	r7, [sp, #0]
	bl	sub_0803c378
	movs	r1, #160
	lsls	r1, r1, #7
	mov	r3, r8
	adds	r1, #4
	mov	r0, r9
	movs	r2, #2
	str	r7, [sp, #0]
	bl	sub_0803c378
	ldrb	r3, [r5, #0]
	cmp	r3, #9
	bhi.n	.L_08042cd2
	ldrb	r3, [r5, #1]
	cmp	r3, #9
	bhi.n	.L_08042cd2
	ldrb	r3, [r5, #2]
	cmp	r3, #9
	bhi.n	.L_08042cd2
	ldrb	r3, [r5, #3]
	cmp	r3, #9
	bls.n	.L_08042ce6
.L_08042cd2:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #196]
	ldr	r1, [pc, #200]
	adds	r2, #72
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08042ce6:
	ldrb	r3, [r5, #0]
	cmp	r3, #9
	bls.n	.L_08042cf6
	movs	r4, #241
	lsls	r4, r4, #8
	adds	r4, #150
	adds	r1, r3, r4
	b.n	.L_08042d02
.L_08042cf6:
	ldrb	r3, [r5, #0]
	adds	r1, r3, #0
	movs	r3, #240
	adds	r1, #48
	lsls	r3, r3, #8
	orrs	r1, r3
.L_08042d02:
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, r9
	adds	r3, r6, #0
	movs	r2, #1
	bl	sub_0803c378
	ldrb	r3, [r5, #1]
	cmp	r3, #9
	bls.n	.L_08042d20
	movs	r0, #241
	lsls	r0, r0, #8
	adds	r0, #150
	adds	r1, r3, r0
	b.n	.L_08042d2c
.L_08042d20:
	ldrb	r3, [r5, #1]
	adds	r1, r3, #0
	movs	r3, #240
	adds	r1, #48
	lsls	r3, r3, #8
	orrs	r1, r3
.L_08042d2c:
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, r9
	adds	r3, r6, #0
	movs	r2, #3
	bl	sub_0803c378
	ldrb	r3, [r5, #2]
	cmp	r3, #9
	bls.n	.L_08042d4a
	movs	r2, #241
	lsls	r2, r2, #8
	adds	r2, #150
	adds	r1, r3, r2
	b.n	.L_08042d56
.L_08042d4a:
	ldrb	r3, [r5, #2]
	adds	r1, r3, #0
	movs	r3, #240
	adds	r1, #48
	lsls	r3, r3, #8
	orrs	r1, r3
.L_08042d56:
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r3, r8
	movs	r2, #1
	bl	sub_0803c378
	ldrb	r3, [r5, #3]
	cmp	r3, #9
	bls.n	.L_08042d74
	movs	r4, #241
	lsls	r4, r4, #8
	adds	r4, #150
	adds	r1, r3, r4
	b.n	.L_08042d80
.L_08042d74:
	ldrb	r3, [r5, #3]
	adds	r1, r3, #0
	movs	r3, #240
	adds	r1, #48
	lsls	r3, r3, #8
	orrs	r1, r3
.L_08042d80:
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, r9
	movs	r2, #3
	mov	r3, r8
	bl	sub_0803c378
.L_08042d8e:
	ldr	r0, [sp, #28]
	movs	r3, #0
	strb	r3, [r0, #6]
.L_08042d94:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080aa1b8
	.2byte 0x3400
	.2byte 0x0600
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #64]
	movs	r1, #1
	ldr	r0, [r3, #0]
	bl	sub_0803939c
	movs	r0, #64
	bl	sub_0801314c
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r2, [r3, #0]
	movs	r1, #196
	lsls	r1, r1, #6
	adds	r1, #66
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	bgt.n	.L_08042de0
	lsls	r0, r0, #1
.L_08042de0:
	movs	r3, #197
	lsls	r3, r3, #6
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, r3, r0
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #6
	bls.n	.L_08042df6
	movs	r0, #6
.L_08042df6:
	lsls	r3, r0, #1
	ldr	r2, [pc, #16]
	adds	r3, r3, r0
	ldr	r1, [pc, #16]
	ldr	r4, [pc, #8]
	lsls	r3, r3, #2
	adds	r0, r3, r2
	movs	r2, #11
	b.n	.L_08042e14
	.4byte 0x0000f080
	.4byte 0x0805f5ec
	.2byte 0x2252
	.2byte 0x0600
.L_08042e14:
	ldrb	r3, [r0, #0]
	subs	r2, #1
	adds	r3, r3, r4
	strh	r3, [r1, #0]
	adds	r0, #1
	adds	r1, #2
	cmp	r2, #0
	bge.n	.L_08042e14
	pop	{pc}
	.align 2, 0
