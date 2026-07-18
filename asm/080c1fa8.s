@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1fa8
	.thumb_func
Func_080c1fa8:
	push	{r5, r6, lr}
	movs	r3, #190
	lsls	r3, r3, #1
	sub	sp, #20
	movs	r5, #0
	cmp	r0, r3
	bcc.n	.L0
	movs	r0, #1
.L0:
	ldr	r2, [pc, #60]
	lsls	r3, r0, #4
	adds	r3, r3, r2
	mov	r6, sp
	adds	r0, r3, #0
	adds	r2, r3, #1
	adds	r0, #11
	adds	r4, r6, #0
	movs	r1, #4
.L2:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #0
	beq.n	.L1
	ldrb	r3, [r2, #0]
	adds	r3, #8
	stmia	r4!, {r3}
	adds	r5, #1
.L1:
	subs	r1, #1
	adds	r2, #1
	cmp	r1, #0
	bge.n	.L2
	bl	Func_08004458
	adds	r3, r5, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	ldr	r0, [r6, r3]
	add	sp, #20
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x080c5c38
