@ 圧縮形式零・一の展開処理。転送後のアーム領域で実行する。
@ 呼出側の転送元と転送長により境界を確定した再構築コード。
.syntax unified
	.arm
	.global Func_08002544
Func_08002544:
	push	{r5, r6, r7, r9, lr}
	ldrb	r2, [r0], #1
	cmp	r2, #0
	bne	.L_08002788
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.L_08002768
	b	.L_0800258c
.L_08002574:
	strb	r2, [r1, #1]!
	lsr	r2, r2, #8
	subs	r4, r4, #9
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
.L_0800258c:
	lsrs	r2, r2, #1
	bcs	.L_08002574
	lsrs	r2, r2, #1
	bcc	.L_08002648
	lsrs	r2, r2, #1
	bcc	.L_0800260c
	lsrs	r2, r2, #1
	bcc	.L_08002618
	lsrs	r2, r2, #1
	bcc	.L_08002628
	lsrs	r2, r2, #1
	bcs	.L_080025d4
	lsrs	r2, r2, #1
	bcc	.L_08002638
	subs	r4, r4, #7
	mov	r5, #7
	bpl	.L_08002650
	blmi	.L_08002764
.L_080025d4:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.L_080025f0
	add	r5, r5, #7
	subs	r4, r4, #8
	bpl	.L_08002650
	blmi	.L_08002764
.L_080025f0:
	ands	r5, r2, #127
	beq	.L_08002778
	add	r5, r5, #10
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.L_08002650
	blmi	.L_08002764
.L_0800260c:
	subs	r4, r4, #3
	mov	r5, #3
	b	.L_08002650
.L_08002618:
	subs	r4, r4, #4
	mov	r5, #4
	bpl	.L_08002650
	blmi	.L_08002764
.L_08002628:
	subs	r4, r4, #5
	mov	r5, #5
	bpl	.L_08002650
	blmi	.L_08002764
.L_08002638:
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.L_08002650
	blmi	.L_08002764
.L_08002648:
	subs	r4, r4, #2
	mov	r5, #2
.L_08002650:
	lsrs	r2, r2, #1
	bcc	.L_08002698
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.L_08002664:
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
	lsrs	r5, r5, #1
	ldrbcs	r7, [r1, -r6]
	strbcs	r7, [r1, #1]!
.L_0800267c:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_0800267c
	b	.L_0800258c
.L_08002698:
	sub	r6, r1, r9
	cmp	r6, #2048
	bcc	.L_080026bc
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.L_08002664
.L_080026bc:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
.L_08002744:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.L_08002664
.L_08002764:
	sub	lr, pc, #284
.L_08002768:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08002778:
	sub	r9, r9, #33
.L_0800277c:
	sub	r0, r1, r9
	pop	{r5, r6, r7, r9, lr}
	bx	lr
.L_08002788:
	cmp	r2, #1
	beq	.L_08002798
	sub	r9, r1, #4
	b	.L_0800277c
.L_08002798:
	mov	r9, r1
.L_0800279c:
	ldrb	lr, [r0], #1
	mov	r3, #16777216
	orrs	lr, r3, lr, lsl #25
	bcs	.L_080027c0
.L_080027ac:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
.L_080027b4:
	lsls	lr, lr, #1
	bcc	.L_080027ac
	beq	.L_0800279c
.L_080027c0:
	ldrb	r3, [r0], #1
	ldrb	r4, [r0], #1
	and	r2, r3, #240
	orr	r2, r4, r2, lsl #4
	ands	r3, r3, #15
	beq	.L_080027f4
.L_080027d8:
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	subs	r3, r3, #1
	bne	.L_080027d8
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	b	.L_080027b4
.L_080027f4:
	movs	r2, r2
	beq	.L_0800277c
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	b	.L_080027d8
