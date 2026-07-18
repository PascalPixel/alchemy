@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f3804
	.thumb_func
Func_080f3804:
	push	{lr}
	ldr	r3, [pc, #24]
	adds	r4, r1, #0
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r2, r1, r3
	adds	r3, r4, #0
	bl	Func_080f3078
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
