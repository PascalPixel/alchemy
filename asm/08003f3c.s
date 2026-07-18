@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003f3c
	.thumb_func
Func_08003f3c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #48]
	lsls	r2, r0, #2
	adds	r5, r2, r3
	cmp	r0, #95
	bls.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldrh	r3, [r5, #2]
	ldr	r6, [pc, #32]
	cmp	r3, r6
	beq.n	.L2
	bl	Func_08003f04
	ldrh	r3, [r5, #2]
	adds	r2, r6, #0
	orrs	r2, r3
	movs	r3, #0
	strh	r2, [r5, #2]
	strh	r3, [r5, #0]
.L2:
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b10
	.4byte 0x0000ffff
