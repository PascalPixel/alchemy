@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800ba30
	.thumb_func
Func_0800ba30:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	movs	r2, #36
	adds	r6, r1, #0
	movs	r3, #127
	adds	r2, r2, r7
	movs	r4, #128
	ands	r4, r6
	ands	r6, r3
	ldrb	r3, [r2, #0]
	sub	sp, #8
	mov	sl, r2
	cmp	r3, r6
	beq.n	.L0
	movs	r3, #39
	adds	r3, r3, r7
	movs	r1, #0
	mov	r8, r3
	b.n	.L1
.L4:
	lsls	r3, r1, #2
	adds	r3, #40
	ldr	r5, [r7, r3]
	cmp	r5, #0
	beq.n	.L2
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L2
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_08185000
	ldrb	r3, [r0, #5]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r6, r3
	bge.n	.L2
	ldr	r2, [r5, #12]
	lsls	r3, r6, #2
	ldr	r2, [r3, r2]
	ldrb	r3, [r0, #4]
	strb	r3, [r5, #4]
	movs	r3, #16
	str	r2, [r5, #16]
	strb	r3, [r5, #21]
	cmp	r4, #0
	bne.n	.L3
	strb	r4, [r5, #20]
	strh	r4, [r5, #2]
.L3:
	cmp	r1, #0
	bne.n	.L2
	ldrb	r3, [r0, #7]
	adds	r2, r7, #0
	adds	r2, #35
	strb	r3, [r2, #0]
	ldrb	r3, [r0, #6]
	subs	r2, #1
	strb	r3, [r2, #0]
.L2:
	adds	r1, #1
.L1:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r1, r3
	blt.n	.L4
	mov	r3, sl
	strb	r6, [r3, #0]
.L0:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
