@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011be0
	.thumb_func
Func_08011be0:
	push	{lr}
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl	Func_080041d8
	pop	{r0}
	bx	r0
	.4byte 0x08011bf5
