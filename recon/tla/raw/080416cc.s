.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08014d78, 0x08014d78
	.global Func_080416cc
	.thumb_func
Func_080416cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r6, r0, #0
	adds	r0, r5, #0
	mov	fp, r3
	bl	sub_08014d78
	str	r0, [sp, #8]
	ldr	r0, [pc, #720]
	bl	sub_08013300
	str	r0, [sp, #4]
	mov	r1, fp
	ldrb	r3, [r1, #7]
	add	r0, sp, #20
	lsls	r3, r3, #12
	str	r3, [sp, #0]
	movs	r1, #16
	ldr	r3, [pc, #704]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a00
	movs	r3, #240
	lsls	r3, r3, #8
	cmp	r2, r3
	bne.n	.L_08041746
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #62
	add	r3, fp
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #684]
	movs	r3, #15
	ands	r3, r2
	ldrb	r3, [r1, r3]
	add	r4, sp, #20
	strb	r3, [r4, #1]
	movs	r3, #3
	strb	r3, [r4, #3]
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #668]
	adds	r1, r5, #0
	ldr	r2, [pc, #668]
	mov	sl, r4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe012
.L_08041746:
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #62
	add	r3, fp
	ldrb	r2, [r3, #0]
	movs	r7, #20
	movs	r3, #15
	add	r7, sp
	ands	r3, r2
	strb	r3, [r7, #1]
	movs	r3, #1
	strb	r3, [r7, #3]
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #628]
	adds	r1, r5, #0
	ldr	r2, [pc, #632]
	mov	sl, r7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2e00
	bne.n	.L_08041772
	b.n	.L_080418ca
.L_08041772:
	b.n	.L_080418be
.L_08041774:
	cmp	r1, #30
	bhi.n	.L_08041832
	subs	r3, r1, #3
	cmp	r3, #26
	bls.n	.L_08041780
	b.n	.L_080418be
.L_08041780:
	ldr	r2, [pc, #608]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08041826
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x08041810
	.4byte 0x080417f4
	.4byte 0x08041810
	.4byte 0x08041810
	.4byte 0x0804182e
	.4byte 0x0804182e
	.4byte 0x080418be
	.4byte 0x0804182c
	.4byte 0x0804182c
	.4byte 0x080418be
	.4byte 0x0804182e
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x080418be
	.4byte 0x0804182c
	.2byte 0x182e
	.2byte 0x0804
	movs	r3, #240
	ldrh	r1, [r6, #0]
	lsls	r3, r3, #4
	adds	r3, #62
	add	r3, fp
	ldr	r2, [pc, #468]
	strh	r1, [r3, #0]
	movs	r3, #15
	ands	r3, r1
	ldrb	r3, [r2, r3]
	mov	r1, sl
	adds	r6, #2
	strb	r3, [r1, #1]
	b.n	.L_080418be
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #62
	movs	r2, #15
	add	r3, fp
	strh	r2, [r3, #0]
	ldr	r3, [pc, #436]
	ldrb	r3, [r3, r2]
	mov	r2, sl
	strb	r3, [r2, #1]
	b.n	.L_080418be
	ldr	r3, [pc, #448]
	ldrb	r3, [r3, #0]
	b.n	.L_080418bc
	adds	r6, #2
	adds	r6, #2
	b.n	.L_080418be
.L_08041832:
	movs	r3, #255
	ldr	r4, [sp, #4]
	ldr	r0, [sp, #8]
	ands	r1, r3
	lsls	r3, r1, #5
	movs	r7, #0
	adds	r5, r4, r3
	add	r0, r8
	mov	r9, r7
	mov	lr, sl
.L_08041846:
	ldmia	r5!, {r2}
	movs	r4, #3
.L_0804184a:
	movs	r7, #15
	adds	r3, r2, #0
	ands	r3, r7
	mov	r7, lr
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_0804185a
	strb	r3, [r0, #0]
.L_0804185a:
	lsrs	r2, r2, #4
	movs	r7, #15
	adds	r3, r2, #0
	ands	r3, r7
	mov	r7, sl
	ldrb	r3, [r7, r3]
	adds	r0, #1
	cmp	r3, #0
	beq.n	.L_0804186e
	strb	r3, [r0, #0]
.L_0804186e:
	subs	r4, #1
	adds	r0, #1
	lsrs	r2, r2, #4
	cmp	r4, #0
	bge.n	.L_0804184a
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r0, #248
	cmp	r3, #7
	ble.n	.L_08041846
	movs	r4, #240
	lsls	r4, r4, #8
	adds	r4, #29
	cmp	ip, r4
	beq.n	.L_08041898
	movs	r7, #240
	lsls	r7, r7, #8
	adds	r7, #31
	cmp	ip, r7
	bne.n	.L_0804189e
.L_08041898:
	movs	r1, #8
	add	r8, r1
	b.n	.L_080418be
.L_0804189e:
	movs	r2, #240
	lsls	r2, r2, #8
	adds	r2, #30
	cmp	ip, r2
	bne.n	.L_080418ac
	movs	r3, #3
	b.n	.L_080418bc
.L_080418ac:
	cmp	r1, #31
	bls.n	.L_080418ba
	ldr	r2, [pc, #308]
	adds	r3, r1, #0
	subs	r3, #32
	ldrb	r3, [r2, r3]
	b.n	.L_080418bc
.L_080418ba:
	movs	r3, #1
.L_080418bc:
	add	r8, r3
.L_080418be:
	ldrh	r1, [r6, #0]
	adds	r6, #2
	mov	ip, r1
	cmp	r1, #0
	beq.n	.L_080418ca
	b.n	.L_08041774
.L_080418ca:
	mov	r3, r8
	adds	r3, #7
	ldr	r5, [sp, #8]
	lsrs	r6, r3, #3
	movs	r1, #128
	lsls	r4, r6, #2
	lsls	r7, r6, #3
	lsls	r1, r1, #1
	movs	r2, #7
	adds	r0, r5, #0
	mov	sl, r4
	mov	r8, r7
	mov	lr, r1
	mov	r9, r2
.L_080418e6:
	cmp	r6, #0
	beq.n	.L_08041922
	ldr	r3, [pc, #256]
	movs	r7, #255
	lsls	r7, r7, #16
	mov	ip, r3
	adds	r7, #255
	adds	r4, r6, #0
.L_080418f6:
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	lsls	r3, r1, #4
	orrs	r1, r3
	lsrs	r3, r2, #4
	orrs	r2, r3
	mov	r3, ip
	ands	r1, r3
	ands	r2, r7
	lsls	r3, r1, #8
	orrs	r1, r3
	lsrs	r3, r2, #8
	orrs	r2, r3
	lsls	r3, r1, #4
	lsrs	r3, r3, #16
	lsls	r2, r2, #16
	orrs	r3, r2
	subs	r4, #1
	adds	r5, #8
	stmia	r0!, {r3}
	cmp	r4, #0
	bne.n	.L_080418f6
.L_08041922:
	mov	r4, sl
	movs	r2, #1
	subs	r3, r0, r4
	mov	r7, lr
	mov	r1, r8
	negs	r2, r2
	adds	r0, r3, r7
	add	r9, r2
	subs	r3, r5, r1
	adds	r5, r3, r7
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_080418e6
	cmp	r6, #0
	bne.n	.L_08041942
	b.n	.L_08041aa6
.L_08041942:
	movs	r4, #192
	lsls	r4, r4, #2
	movs	r7, #224
	ldr	r0, [sp, #8]
	adds	r4, #255
	lsls	r7, r7, #4
	mov	ip, r4
	adds	r7, #56
	mov	r9, r6
.L_08041954:
	mov	r1, fp
	ldrb	r2, [r1, #2]
	movs	r5, #127
	cmp	r2, #0
	beq.n	.L_08041960
	movs	r5, #255
.L_08041960:
	ldr	r4, [sp, #16]
	mov	r1, ip
	ldrh	r3, [r4, #0]
	ands	r1, r3
	adds	r3, r1, #0
	subs	r3, #128
	cmp	r3, #127
	bls.n	.L_08041a00
	cmp	r2, #0
	beq.n	.L_08041984
	movs	r2, #128
	lsls	r2, r2, #2
	cmp	r1, r2
	bcc.n	.L_08041984
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r1, r3
	bcc.n	.L_08041a00
.L_08041984:
	mov	r4, fp
	ldrh	r1, [r4, #0]
	mov	r2, fp
	ands	r1, r5
	adds	r3, r1, r7
	ldrb	r3, [r2, r3]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080419b0
.L_08041996:
	adds	r1, #1
	adds	r4, #1
	ands	r1, r5
	cmp	r4, r5
	bhi.n	.L_080419b0
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r2, #56
	adds	r3, r1, r2
	mov	r2, fp
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_08041996
.L_080419b0:
	adds	r3, r1, #1
	ands	r3, r5
	mov	r4, fp
	strh	r3, [r4, #0]
	adds	r2, r1, r7
	movs	r3, #1
	strb	r3, [r4, r2]
	cmp	r1, #127
	bls.n	.L_080419f0
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r1, r1, r2
	b.n	.L_080419f4
	movs	r0, r0
	.4byte 0x00000013
	.4byte 0x03000258
	.4byte 0x0805f564
	.4byte 0x03000260
	.4byte 0x04040404
	.4byte 0x0e0e0e0e
	.4byte 0x08041788
	.4byte 0x0805f484
	.2byte 0x0ff0
	.2byte 0x0ff0
.L_080419f0:
	movs	r3, #128
	orrs	r1, r3
.L_080419f4:
	ldr	r3, [sp, #0]
	ldr	r4, [sp, #16]
	orrs	r3, r1
	strh	r3, [r4, #0]
	ldr	r2, [sp, #12]
	strh	r3, [r2, #0]
.L_08041a00:
	movs	r3, #192
	lsls	r2, r1, #5
	lsls	r3, r3, #19
	adds	r1, r2, r3
	movs	r4, #192
	ldr	r3, [r0, #0]
	lsls	r4, r4, #19
	adds	r4, #4
	str	r3, [r1, #0]
	adds	r1, r2, r4
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #8
	lsls	r4, r4, #2
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #12
	lsls	r4, r4, #2
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #16
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #20
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #24
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #224
	str	r3, [r1, #0]
	movs	r3, #192
	lsls	r3, r3, #19
	adds	r3, #28
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	adds	r0, #4
	str	r3, [r1, #0]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	adds	r1, #2
	adds	r2, #2
	mov	r4, r9
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	cmp	r4, #0
	beq.n	.L_08041aa6
	b.n	.L_08041954
.L_08041aa6:
	ldr	r0, [sp, #8]
	bl	sub_08013164
	adds	r0, r6, #0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	lsls	r1, r1, #6
	lsls	r0, r0, #1
	adds	r1, r5, r1
	ldrb	r3, [r5, #2]
	adds	r1, r1, r0
	adds	r0, r1, #0
	movs	r6, #0
	sub	sp, #4
	adds	r7, r2, #0
	adds	r0, #8
	mov	r8, r3
	cmp	r6, fp
	bcs.n	.L_08041b58
	movs	r3, #32
	subs	r3, r3, r7
	lsls	r3, r3, #1
	str	r3, [sp, #0]
.L_08041af4:
	movs	r4, #0
	cmp	r4, r7
	bcs.n	.L_08041b4e
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	mov	r9, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	sl, r3
	adds	r3, #128
	mov	lr, r3
	movs	r3, #255
	mov	ip, r3
.L_08041b12:
	ldrh	r3, [r0, #0]
	mov	r2, r9
	ands	r2, r3
	adds	r3, r2, #0
	subs	r3, #128
	adds	r0, #2
	cmp	r3, #127
	bls.n	.L_08041b30
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_08041b48
	cmp	r2, sl
	bls.n	.L_08041b48
	cmp	r2, lr
	bhi.n	.L_08041b48
.L_08041b30:
	mov	r3, ip
	ands	r2, r3
	movs	r3, #128
	eors	r2, r3
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r3, #56
	adds	r2, r2, r3
	ldrb	r1, [r5, r2]
	movs	r3, #252
	ands	r3, r1
	strb	r3, [r5, r2]
.L_08041b48:
	adds	r4, #1
	cmp	r4, r7
	bcc.n	.L_08041b12
.L_08041b4e:
	ldr	r3, [sp, #0]
	adds	r6, #1
	adds	r0, r0, r3
	cmp	r6, fp
	bcc.n	.L_08041af4
.L_08041b58:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
