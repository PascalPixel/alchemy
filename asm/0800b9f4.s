@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800b9f4
	.thumb_func
Func_0800b9f4:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	adds	r6, r1, #0
	movs	r7, #128
	ands	r7, r6
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	Func_08185000
	ldrb	r3, [r0, #5]
	cmp	r6, r3
	bge.n	.L0
	ldr	r2, [r5, #12]
	lsls	r3, r6, #2
	ldr	r2, [r3, r2]
	ldrb	r3, [r0, #4]
	strb	r3, [r5, #4]
	movs	r3, #16
	str	r2, [r5, #16]
	strb	r3, [r5, #21]
	cmp	r7, #0
	bne.n	.L0
	strb	r7, [r5, #20]
	strh	r7, [r5, #2]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
