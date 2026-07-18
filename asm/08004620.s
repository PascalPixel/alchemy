@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004620
	.thumb_func
Func_08004620:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	sub	sp, #8
	ldr	r4, [pc, #104]
	movs	r1, #32
	ldr	r7, [pc, #104]
	cmp	r6, #0
	bge.n	.L0
	negs	r6, r6
	movs	r1, #45
.L0:
	ldmia	r4!, {r5}
	movs	r2, #9
	cmp	r6, r5
	bcs.n	.L1
	movs	r3, #32
.L2:
	subs	r2, #1
	strb	r3, [r7, #0]
	adds	r7, #1
	cmp	r2, #0
	beq.n	.L1
	ldmia	r4!, {r5}
	cmp	r6, r5
	bcc.n	.L2
.L1:
	strb	r1, [r7, #0]
	subs	r4, #4
	adds	r7, #1
	cmp	r2, #0
	beq.n	.L3
.L4:
	ldmia	r4!, {r5}
	adds	r0, r6, #0
	adds	r1, r5, #0
	str	r2, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_080022f4
	adds	r3, r0, #0
	adds	r3, #48
	strb	r3, [r7, #0]
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #4]
	subs	r2, #1
	adds	r7, #1
	subs	r6, r6, r3
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bne.n	.L4
.L3:
	adds	r3, r6, #0
	adds	r3, #48
	strb	r3, [r7, #0]
	movs	r3, #0
	strb	r3, [r7, #1]
	add	sp, #8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x08007970
	.4byte 0x03001f70
