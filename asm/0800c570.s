@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800c570
	.thumb_func
Func_0800c570:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r0, [r0, #80]
	movs	r3, #1
	ands	r1, r3
	ldrb	r2, [r0, #29]
	movs	r3, #3
	negs	r3, r3
	lsls	r1, r1, #1
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #29]
.L0:
	pop	{r0}
	bx	r0
