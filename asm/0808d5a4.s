@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808d5a4
	.thumb_func
Func_0808d5a4:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #0
	bl	Func_0808d48c
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #36]
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	adds	r6, r0, #0
	cmp	r1, r5
	bne.n	.L0
	movs	r0, #7
	bl	Func_0808d48c
	cmp	r0, #0
	bne.n	.L1
.L0:
	adds	r0, r6, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000024a
	.4byte 0x02000240
