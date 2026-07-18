@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800bacc
	.thumb_func
Func_0800bacc:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	lsls	r1, r1, #4
	adds	r0, #40
	adds	r4, r3, #0
.L2:
	ldmia	r0!, {r2}
	cmp	r2, #0
	beq.n	.L1
	ldr	r3, [r2, #12]
	cmp	r3, #0
	beq.n	.L1
	strh	r1, [r2, #2]
.L1:
	subs	r4, #1
	cmp	r4, #0
	bne.n	.L2
.L0:
	movs	r0, #0
	pop	{r1}
	bx	r1
