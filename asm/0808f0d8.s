@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808f0d8
	.thumb_func
Func_0808f0d8:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r3, [pc, #84]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r2, r6, #0
	str	r3, [r6, #48]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	movs	r3, #144
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl	Func_08009150
	movs	r0, #3
	bl	Func_080030f8
	adds	r0, r5, #0
	movs	r1, #28
	bl	Func_08009080
	ldr	r1, [pc, #20]
	adds	r0, r6, #0
	bl	Func_08009098
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #6]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x0809e75c
