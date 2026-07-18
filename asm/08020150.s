@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08020150
	.thumb_func
Func_08020150:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	sub	sp, #8
	cmp	r7, #0
	beq.n	.L0
	movs	r3, #16
	adds	r5, r1, #0
	movs	r4, #0
	mov	r8, r3
	movs	r6, #3
	adds	r5, #40
.L1:
	ldrb	r0, [r5, #0]
	mov	r3, r8
	lsls	r0, r0, #24
	str	r3, [sp, #0]
	asrs	r0, r0, #24
	adds	r3, r4, #0
	movs	r1, #2
	adds	r2, r7, #0
	str	r4, [sp, #4]
	bl	Func_0801e9d4
	ldr	r4, [sp, #4]
	subs	r6, #1
	adds	r5, #1
	adds	r4, #24
	cmp	r6, #0
	bge.n	.L1
.L0:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
