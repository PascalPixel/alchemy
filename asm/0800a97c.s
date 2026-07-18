@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800a97c
	.thumb_func
Func_0800a97c:
	push	{r5, r6, r7, lr}
	ldrb	r3, [r0, #1]
	ldrb	r4, [r0, #0]
	lsls	r3, r3, #8
	orrs	r4, r3
	adds	r5, r1, #0
	adds	r6, r0, #2
	cmp	r4, #0
	bne.n	.L0
	adds	r0, r6, #0
	b.n	.L1
.L0:
	adds	r0, r0, r4
	ldrb	r1, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r7, r6, #0
	mov	ip, r5
	b.n	.L2
.L8:
	asrs	r2, r4, #12
	cmp	r2, #0
	bne.n	.L3
	ldrb	r3, [r0, #0]
	adds	r2, r3, #0
	adds	r2, #16
	adds	r0, #1
.L3:
	ldr	r3, [pc, #88]
	adds	r2, #2
	ands	r4, r3
	subs	r4, r7, r4
	cmp	r2, #0
	beq.n	.L4
.L5:
	ldrb	r3, [r4, #0]
	subs	r2, #1
	strb	r3, [r5, #0]
	adds	r4, #1
	adds	r5, #1
	cmp	r2, #0
	bne.n	.L5
.L4:
	lsrs	r1, r1, #1
	cmp	r1, #0
	bne.n	.L6
	ldrb	r1, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #1
.L2:
	adds	r0, #1
	orrs	r1, r3
.L6:
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L7
	cmp	r1, #1
	beq.n	.L4
	ldrb	r3, [r6, #0]
	adds	r6, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	b.n	.L4
.L7:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	lsls	r4, r3, #8
	ldrb	r3, [r0, #0]
	orrs	r4, r3
	adds	r0, #1
	cmp	r4, #0
	bne.n	.L8
	mov	r0, ip
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000fff
