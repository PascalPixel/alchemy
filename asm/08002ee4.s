@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08002ee4
	.thumb_func
Func_08002ee4:
	push	{r5, lr}
	ldr	r5, [pc, #12]
.L0:
	movs	r0, #1
	ldr	r3, [r5, #0]
	bl	Func_080030f8
	b.n	.L0
	movs	r0, r0
	.4byte 0x03001c94
