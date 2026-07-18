@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079bf8
	.thumb_func
Func_08079bf8:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	subs	r5, r5, r1
	adds	r6, r2, #0
	adds	r0, r3, #0
	cmp	r5, #0
	bge.n	.L0
	movs	r5, #0
.L0:
	movs	r1, #1
	bl	Func_08079b24
	lsls	r3, r6, #1
	adds	r3, r5, r3
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L1
	ldr	r3, [pc, #16]
	adds	r0, r0, r3
.L1:
	asrs	r0, r0, #9
	cmp	r0, #0
	bge.n	.L2
	movs	r0, #0
.L2:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
