@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800bdd4
	.thumb_func
Func_0800bdd4:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	sub	sp, #4
	cmp	r7, #0
	beq.n	.L0
	ldrb	r2, [r7, #29]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L1
	ldrb	r0, [r7, #28]
	bl	Func_08003f3c
.L1:
	adds	r5, r7, #0
	adds	r5, #40
	movs	r6, #3
.L2:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	Func_0800bc48
	cmp	r6, #0
	bge.n	.L2
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r7, #0
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x8500000e
