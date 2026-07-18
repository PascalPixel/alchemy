@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08095240
	.thumb_func
Func_08095240:
	push	{lr}
	ldr	r1, [pc, #28]
	movs	r0, #30
	bl	Func_080048f4
	movs	r3, #252
	lsls	r3, r3, #5
	adds	r2, r0, r3
	movs	r3, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	movs	r1, #0
	adds	r0, r0, r3
	strh	r1, [r0, #0]
	pop	{r0}
	bx	r0
	.4byte 0x00001f88
	.4byte 0x00001f82
