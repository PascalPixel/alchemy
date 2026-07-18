@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08012d48
	.thumb_func
Func_08012d48:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r4, r3, #0
	adds	r4, #40
	movs	r0, #9
.L0:
	ldr	r3, [r2, r4]
	subs	r0, #1
	strb	r1, [r3, #6]
	adds	r2, #56
	cmp	r0, #0
	bge.n	.L0
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e60
