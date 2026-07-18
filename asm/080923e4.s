@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080923e4
	.thumb_func
Func_080923e4:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	bl	Func_08009140
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #60]
	str	r3, [r5, #56]
	str	r6, [r5, #8]
	str	r7, [r5, #16]
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #34
	adds	r1, r6, #0
	ldrb	r0, [r3, #0]
	cmp	r1, #0
	bge.n	.L1
	ldr	r3, [pc, #40]
	adds	r1, r1, r3
.L1:
	adds	r2, r7, #0
	asrs	r1, r1, #16
	cmp	r2, #0
	bge.n	.L2
	ldr	r3, [pc, #28]
	adds	r2, r2, r3
.L2:
	asrs	r2, r2, #16
	bl	Func_080091a8
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #20]
	lsls	r0, r0, #16
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r5, #12]
	str	r0, [r5, #20]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000ffff
