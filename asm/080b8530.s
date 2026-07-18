@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8530
	.thumb_func
Func_080b8530:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	Func_080c2454
	lsls	r0, r0, #24
	lsrs	r3, r0, #8
	cmp	r3, #0
	bne.n	.L0
	adds	r0, r5, #0
	bl	Func_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	Func_080c23c0
	movs	r3, #192
	lsls	r3, r3, #13
	cmp	r0, #0
	bne.n	.L0
	movs	r3, #192
	lsls	r3, r3, #14
.L0:
	adds	r0, r3, #0
	pop	{r5}
	pop	{r1}
	bx	r1
