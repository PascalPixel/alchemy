@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08092a1c
	.thumb_func
Func_08092a1c:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	movs	r0, #255
	ands	r0, r6
	bl	Func_0808ba1c
	cmp	r5, #0
	beq.n	.L0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r6
	str	r0, [r5, #104]
	cmp	r3, #0
	bne.n	.L1
	adds	r2, r5, #0
	movs	r3, #40
	adds	r2, #100
	strh	r3, [r2, #0]
	ldr	r3, [r0, #52]
	lsls	r3, r3, #1
	str	r3, [r5, #52]
	ldr	r3, [r0, #48]
	ldr	r1, [pc, #16]
	str	r3, [r5, #48]
	adds	r3, r5, #0
	adds	r3, #89
	strb	r1, [r3, #0]
.L1:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_08009098
	b.n	.L0
	.4byte 0x00000000
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
