@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021750
	.thumb_func
Func_08021750:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	Func_08004080
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #96
	beq.n	.L0
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_080216e8
	ldr	r3, [sp, #28]
	movs	r1, #128
	str	r3, [sp, #0]
	mov	r2, r8
	mov	r3, sl
	lsls	r1, r1, #24
	adds	r0, r5, #0
	bl	Func_0801eadc
	ldrb	r3, [r0, #21]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r0, #21]
	movs	r3, #251
	strb	r3, [r0, #15]
.L0:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
