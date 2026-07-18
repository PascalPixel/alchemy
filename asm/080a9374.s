@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9374
	.thumb_func
Func_080a9374:
	push	{r5, lr}
	ldr	r3, [pc, #32]
	adds	r0, r1, #0
	ldr	r5, [r3, #0]
	bl	Func_08077008
	bl	Func_080a345c
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl	Func_080a68a8
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	bx	lr
