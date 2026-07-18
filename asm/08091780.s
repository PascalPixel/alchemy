@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091780
	.thumb_func
Func_08091780:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_0809177c
	adds	r0, r5, #0
	bl	Func_0808b674
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #16]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
