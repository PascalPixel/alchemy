@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808f140
	.thumb_func
Func_0808f140:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r3, [pc, #104]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	movs	r3, #1
	ands	r3, r6
	adds	r7, r0, #0
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	ldr	r1, [pc, #76]
	adds	r0, r5, #0
	bl	Func_08009098
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #40]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #72]
	ldr	r3, [pc, #60]
	str	r3, [r5, #108]
.L1:
	cmp	r6, #3
	bne.n	.L2
	movs	r0, #60
	bl	Func_080030f8
.L2:
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L3
	adds	r0, r5, #0
	bl	Func_0808f0d8
.L3:
	cmp	r6, #3
	bne.n	.L4
	movs	r0, #80
	bl	Func_080030f8
.L4:
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_08009080
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0809e6c0
	.4byte 0x0808eee5
