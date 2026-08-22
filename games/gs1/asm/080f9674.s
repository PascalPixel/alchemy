@ 音響混合用IWRAM転送関数。ThumbとARMを切り替えて標本を累算する。
@ ROM配置と実行配置を分離し、混在命令列をそのまま再構築する。
.syntax unified
	.thumb
	.global Func_080f9674
	.thumb_func
Func_080f9674:
	ldrb	r3, [r0, #5]
	cmp	r3, #0
	beq .L_03007060
	add	r1, pc, #4
	bx	r1
	.arm
	cmp	r4, #2
	addeq	r7, r0, #848
	addne	r7, r5, r8
	mov	r4, r8
.L_0300701c:
	ldrsb	r0, [r5, r6]
	ldrsb	r1, [r5]
	add	r0, r0, r1
	ldrsb	r1, [r7, r6]
	add	r0, r0, r1
	ldrsb	r1, [r7], #1
	add	r0, r0, r1
	mul	r1, r0, r3
	asr	r0, r1, #9
	tst	r0, #128
	addne	r0, r0, #1
	strb	r0, [r5, r6]
	strb	r0, [r5], #1
	subs	r4, r4, #1
	bgt .L_0300701c
	add	r0, pc, #47
	bx	r0
	.thumb
.L_03007060:
	movs	r0, #0
	mov	r1, r8
	adds	r6, r6, r5
	lsrs	r1, r1, #3
	bcc .L_0300706e
	stmia	r5!, {r0}
	stmia	r6!, {r0}
.L_0300706e:
	lsrs	r1, r1, #1
	bcc .L_0300707a
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
.L_0300707a:
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	stmia	r5!, {r0}
	stmia	r6!, {r0}
	subs	r1, #1
	bgt .L_0300707a
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
.L_03007098:
	str	r0, [sp, #4]
	ldr	r3, [r4, #36]
	ldr	r0, [sp, #20]
	cmp	r0, #0
	beq .L_030070b8
	ldr	r1, [pc, #16]
	ldrb	r1, [r1, #0]
	cmp	r1, #160
	bcs .L_030070ac
	adds	r1, #228
.L_030070ac:
	cmp	r1, r0
	bcc .L_030070b8
	b .L_0300738a
	movs	r0, r0
	movs	r6, r0
	lsls	r0, r0, #16
.L_030070b8:
	ldrb	r6, [r4, #0]
	movs	r0, #199
	tst	r0, r6
	bne .L_030070c2
	b .L_03007380
.L_030070c2:
	movs	r0, #128
	tst	r0, r6
	beq .L_030070f2
	movs	r0, #64
	tst	r0, r6
	bne .L_03007102
	movs	r6, #3
	strb	r6, [r4, #0]
	adds	r0, r3, #0
	adds	r0, #16
	str	r0, [r4, #40]
	ldr	r0, [r3, #12]
	str	r0, [r4, #24]
	movs	r5, #0
	strb	r5, [r4, #9]
	str	r5, [r4, #28]
	ldrb	r2, [r3, #3]
	movs	r0, #192
	tst	r0, r2
	beq .L_0300714a
	movs	r0, #16
	orrs	r6, r0
	strb	r6, [r4, #0]
	b .L_0300714a
.L_030070f2:
	ldrb	r5, [r4, #9]
	movs	r0, #4
	tst	r0, r6
	beq .L_03007108
	ldrb	r0, [r4, #13]
	subs	r0, #1
	strb	r0, [r4, #13]
	bhi .L_03007158
.L_03007102:
	movs	r0, #0
	strb	r0, [r4, #0]
	b .L_03007380
.L_03007108:
	movs	r0, #64
	tst	r0, r6
	beq .L_03007128
	ldrb	r0, [r4, #7]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #12]
	cmp	r5, r0
	bhi .L_03007158
.L_0300711a:
	ldrb	r5, [r4, #12]
	cmp	r5, #0
	beq .L_03007102
	movs	r0, #4
	orrs	r6, r0
	strb	r6, [r4, #0]
	b .L_03007158
.L_03007128:
	movs	r2, #3
	ands	r2, r6
	cmp	r2, #2
	bne .L_03007146
	ldrb	r0, [r4, #5]
	muls	r5, r0
	lsrs	r5, r5, #8
	ldrb	r0, [r4, #6]
	cmp	r5, r0
	bhi .L_03007158
	adds	r5, r0, #0
	beq .L_0300711a
	subs	r6, #1
	strb	r6, [r4, #0]
	b .L_03007158
.L_03007146:
	cmp	r2, #3
	bne .L_03007158
.L_0300714a:
	ldrb	r0, [r4, #4]
	adds	r5, r5, r0
	cmp	r5, #255
	bcc .L_03007158
	movs	r5, #255
	subs	r6, #1
	strb	r6, [r4, #0]
.L_03007158:
	strb	r5, [r4, #9]
	ldr	r0, [sp, #24]
	ldrb	r0, [r0, #7]
	adds	r0, #1
	muls	r0, r5
	lsrs	r5, r0, #4
	ldrb	r0, [r4, #2]
	muls	r0, r5
	lsrs	r0, r0, #8
	strb	r0, [r4, #10]
	ldrb	r0, [r4, #3]
	muls	r0, r5
	lsrs	r0, r0, #8
	strb	r0, [r4, #11]
	movs	r0, #16
	ands	r0, r6
	str	r0, [sp, #16]
	beq .L_0300718c
	adds	r0, r3, #0
	adds	r0, #16
	ldr	r1, [r3, #8]
	adds	r0, r0, r1
	str	r0, [sp, #12]
	ldr	r0, [r3, #12]
	subs	r0, r0, r1
	str	r0, [sp, #16]
.L_0300718c:
	ldr	r5, [sp, #8]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #40]
	add	r0, pc, #4
	bx	r0
	.arm
	str	r8, [sp]
	ldrb	sl, [r4, #10]
	ldrb	fp, [r4, #11]
	lsl	sl, sl, #16
	lsl	fp, fp, #16
	ldrb	r0, [r4, #1]
	tst	r0, #8
	beq .L_030072d8
.L_030071b8:
	cmp	r2, #4
	ble .L_03007228
	subs	r2, r2, r8
	movgt	lr, #0
	bgt .L_030071e4
	mov	lr, r8
	add	r2, r2, r8
	sub	r8, r2, #4
	sub	lr, lr, r8
	ands	r2, r2, #3
	moveq	r2, #4
.L_030071e4:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.L_030071ec:
	ldrsb	r0, [r3], #1
	mul	r1, sl, r0
	bic	r1, r1, #16711680
	add	r6, r1, r6, ror #8
	mul	r1, fp, r0
	bic	r1, r1, #16711680
	add	r7, r1, r7, ror #8
	adds	r5, r5, #1073741824
	bcc .L_030071ec
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .L_030071e4
	adds	r8, r8, lr
	beq .L_0300736c
.L_03007228:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.L_03007230:
	ldrsb	r0, [r3], #1
	mul	r1, sl, r0
	bic	r1, r1, #16711680
	add	r6, r1, r6, ror #8
	mul	r1, fp, r0
	bic	r1, r1, #16711680
	add	r7, r1, r7, ror #8
	subs	r2, r2, #1
	beq .L_030072a0
.L_03007254:
	adds	r5, r5, #1073741824
	bcc .L_03007230
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .L_030071b8
	b .L_0300736c
.L_03007270:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	beq .L_03007294
	ldr	r3, [sp, #20]
	rsb	r9, r2, #0
.L_03007284:
	adds	r2, r0, r2
	bgt .L_0300733c
	sub	r9, r9, r0
	b .L_03007284
.L_03007294:
	pop	{r4, ip}
	mov	r2, #0
	b .L_030072b0
.L_030072a0:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	ldrne	r3, [sp, #12]
	bne .L_03007254
.L_030072b0:
	strb	r2, [r4]
	lsr	r0, r5, #30
	bic	r5, r5, #-1073741824
	rsb	r0, r0, #3
	lsl	r0, r0, #3
	ror	r6, r6, r0
	ror	r7, r7, r0
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	b .L_03007374
.L_030072d8:
	push	{r4, ip}
	ldr	lr, [r4, #28]
	ldr	r1, [r4, #32]
	mul	r4, ip, r1
	ldrsb	r0, [r3]
	ldrsb	r1, [r3, #1]!
	sub	r1, r1, r0
.L_030072f4:
	ldr	r6, [r5]
	ldr	r7, [r5, #1584]
.L_030072fc:
	mul	r9, lr, r1
	add	r9, r0, r9, asr #23
	mul	ip, sl, r9
	bic	ip, ip, #16711680
	add	r6, ip, r6, ror #8
	mul	ip, fp, r9
	bic	ip, ip, #16711680
	add	r7, ip, r7, ror #8
	add	lr, lr, r4
	lsrs	r9, lr, #23
	beq .L_03007348
	bic	lr, lr, #1065353216
	subs	r2, r2, r9
	ble .L_03007270
	subs	r9, r9, #1
	addeq	r0, r0, r1
.L_0300733c:
	ldrsbne	r0, [r3, r9]!
	ldrsb	r1, [r3, #1]!
	sub	r1, r1, r0
.L_03007348:
	adds	r5, r5, #1073741824
	bcc .L_030072fc
	str	r7, [r5, #1584]
	str	r6, [r5], #4
	subs	r8, r8, #4
	bgt .L_030072f4
	sub	r3, r3, #1
	pop	{r4, ip}
	str	lr, [r4, #28]
.L_0300736c:
	str	r2, [r4, #24]
	str	r3, [r4, #40]
.L_03007374:
	ldr	r8, [sp]
	add	r0, pc, #1
	bx	r0
	.thumb
.L_03007380:
	ldr	r0, [sp, #4]
	subs	r0, #1
	ble .L_0300738a
	adds	r4, #64
	b .L_03007098
.L_0300738a:
	ldr	r0, [sp, #24]
	ldr	r3, [pc, #16]
	str	r3, [r0, #0]
	add	sp, #28
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
	bx	r3
	.align 2
.L_030073a0:
	.4byte 0x68736d53
