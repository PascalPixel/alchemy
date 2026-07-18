@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6378
	.thumb_func
Func_080b6378:
	push	{r5, r6, lr}
	sub	sp, #16
	ldr	r3, [pc, #44]
	mov	r5, sp
	adds	r0, r5, #0
	ldr	r6, [r3, #0]
	bl	Func_080b6a60
	movs	r1, #0
	adds	r4, r0, #0
	cmp	r1, r4
	bge.n	.L0
	adds	r0, r5, #0
.L1:
	ldrh	r2, [r0, #0]
	adds	r3, r1, #0
	adds	r2, #72
	subs	r3, #128
	adds	r1, #1
	adds	r0, #2
	strb	r3, [r6, r2]
	cmp	r1, r4
	blt.n	.L1
.L0:
	add	sp, #16
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
