@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091750
	.thumb_func
Func_08091750:
	push	{lr}
	ldr	r0, [pc, #28]
	bl	Func_08004278
	ldr	r3, [pc, #24]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_0809335c
	bl	Func_080772f0
	pop	{r0}
	bx	r0
	.4byte 0x080915ed
	.4byte 0x02000240
	bx	lr
