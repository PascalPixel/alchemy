@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801c154
	.thumb_func
Func_0801c154:
	push	{lr}
	ldr	r3, [pc, #24]
	ldrh	r4, [r0, #6]
	ands	r1, r3
	ldr	r3, [pc, #20]
	ands	r3, r4
	orrs	r3, r1
	movs	r1, #252
	strh	r3, [r0, #6]
	strb	r2, [r0, #4]
	bl	Func_08003dec
	b.n	.L0
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
.L0:
	pop	{r0}
	bx	r0
