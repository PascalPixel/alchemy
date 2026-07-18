@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800b684
	.thumb_func
Func_0800b684:
	push	{lr}
	mov	ip, r0
	cmp	r0, #0
	beq.n	.L0
	mov	r3, ip
	adds	r3, #39
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	mov	r4, ip
	adds	r4, #40
	adds	r0, r3, #0
.L3:
	ldmia	r4!, {r2}
	ldrb	r3, [r2, #5]
	cmp	r3, #15
	beq.n	.L2
	strb	r1, [r2, #5]
.L2:
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L3
.L1:
	mov	r2, ip
	adds	r2, #37
	movs	r3, #1
	strb	r3, [r2, #0]
.L0:
	pop	{r0}
	bx	r0
