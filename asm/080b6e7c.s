@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6e7c
	.thumb_func
Func_080b6e7c:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r4, #0
	ldr	r0, [pc, #40]
.L2:
	lsls	r1, r4, #1
	ldrh	r2, [r0, r1]
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r5, r3
	bne.n	.L0
	ldrh	r3, [r0, r1]
	lsrs	r0, r3, #9
	b.n	.L1
.L0:
	lsls	r3, r2, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	adds	r4, #1
	cmp	r3, r2
	bne.n	.L2
	movs	r0, #6
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x080c593c
	.4byte 0x000001ff
