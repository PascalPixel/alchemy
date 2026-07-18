@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9a44
	.thumb_func
Func_080b9a44:
	push	{lr}
	ldr	r3, [pc, #36]
	ldr	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #1
	adds	r3, #100
	adds	r2, #2
	b.n	.L1
.L0:
	movs	r3, #15
	ands	r3, r0
	lsls	r3, r3, #1
	adds	r3, #88
.L1:
	ldrsh	r0, [r2, r3]
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
