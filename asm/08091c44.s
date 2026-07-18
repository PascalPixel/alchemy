@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091c44
	.thumb_func
Func_08091c44:
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [r0, #80]
	adds	r6, r3, #0
	movs	r5, #0
	adds	r6, #36
	b.n	.L1
.L2:
	adds	r5, #1
.L1:
	cmp	r5, #89
	bgt.n	.L0
	movs	r0, #1
	bl	Func_080030f8
	ldrb	r3, [r6, #0]
	cmp	r7, r3
	beq.n	.L2
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
