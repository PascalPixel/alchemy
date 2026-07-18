@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f37ec
	.thumb_func
Func_080f37ec:
	push	{lr}
	ldr	r0, [pc, #16]
	bl	Func_08004278
	movs	r0, #32
	bl	Func_08002dd8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080f2f11
