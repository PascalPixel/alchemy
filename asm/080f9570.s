@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f9570
	.thumb_func
Func_080f9570:
	push	{lr}
	movs	r3, #128
	ands	r3, r0
	movs	r2, #127
	ands	r0, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [pc, #16]
	ldrb	r3, [r2, #0]
	eors	r0, r3
	strb	r0, [r2, #0]
	b.n	.L1
.L0:
	ldr	r3, [pc, #4]
	strb	r0, [r3, #0]
.L1:
	pop	{r0}
	bx	r0
	.4byte 0x02003040
