@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08092454
	.thumb_func
Func_08092454:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	adds	r6, r2, #0
	mov	r8, r3
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
	mov	r2, r8
	str	r3, [r5, #60]
	str	r3, [r5, #56]
	str	r7, [r5, #8]
	str	r6, [r5, #12]
	str	r2, [r5, #16]
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #34
	adds	r1, r7, #0
	ldrb	r0, [r3, #0]
	cmp	r1, #0
	bge.n	.L1
	ldr	r3, [pc, #44]
	adds	r1, r1, r3
.L1:
	mov	r3, r8
	asrs	r1, r1, #16
	cmp	r3, #0
	bge.n	.L2
	ldr	r2, [pc, #32]
	adds	r3, r3, r2
.L2:
	asrs	r2, r3, #16
	bl	Func_080091a8
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #20]
	lsls	r0, r0, #16
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r5, #12]
	str	r0, [r5, #20]
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ffff
