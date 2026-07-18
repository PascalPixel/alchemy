@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808feb0
	.thumb_func
Func_0808feb0:
	push	{lr}
	ldr	r0, [pc, #16]
	bl	Func_080042c8
	ldr	r0, [pc, #12]
	bl	Func_080042c8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0808f52d
	.4byte 0x0808f499
