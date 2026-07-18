@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078a34
	.thumb_func
Func_08078a34:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r2, [r0, r5]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r3, [pc, #12]
	orrs	r3, r2
	strh	r3, [r0, r5]
	movs	r0, #0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x00000400
