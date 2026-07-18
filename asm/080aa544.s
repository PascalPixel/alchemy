@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080aa544
	.thumb_func
Func_080aa544:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #154
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r0, #61
	movs	r1, #32
	movs	r2, #3
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #16]
	adds	r1, #56
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L0
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
