@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b588
	.thumb_func
Func_0809b588:
	push	{r5, r6, lr}
	ldr	r3, [pc, #72]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	ldr	r3, [pc, #60]
	ldr	r5, [r0, #80]
	movs	r1, #5
	ldr	r0, [r3, #0]
	ldr	r6, [r5, #40]
	bl	Func_08002304
	cmp	r0, #0
	bne.n	.L0
	adds	r2, r5, #0
	adds	r2, #37
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	movs	r3, #3
	b.n	.L1
.L0:
	cmp	r0, #2
	bne.n	.L2
	movs	r3, #0
	adds	r2, r5, #0
	adds	r2, #37
	strb	r3, [r6, #5]
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, #1
.L1:
	strb	r3, [r2, #0]
.L2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001e40
