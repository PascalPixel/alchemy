@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080798b4
	.thumb_func
Func_080798b4:
	push	{lr}
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	Func_080773d8
	adds	r0, #52
	ldrb	r1, [r0, #0]
	cmp	r1, #43
	bls.n	.L0
	movs	r1, #0
.L0:
	lsls	r2, r1, #1
	ldr	r3, [pc, #12]
	adds	r2, r2, r1
	lsls	r2, r2, #3
	ldr	r0, [r3, r2]
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08088e38
