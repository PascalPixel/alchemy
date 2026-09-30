.syntax unified
	.thumb
	.global Func_0803b794
	.thumb_func
Func_0803b794:
.L_0803b794:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r2
	ldr	r2, [sp, #32]
	adds	r7, r3, #0
	mov	r8, r2
	movs	r2, #30
	mov	fp, r2
	ldr	r2, [sp, #40]
	mov	r3, r9
	ldr	r6, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	mov	sl, r1
	ldr	r4, [sp, #36]
	ldr	r5, [r1, #0]
	cmp	r3, #0
	bne.n	.L_0803b7e0
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803b7d6
	adds	r1, r7, #0
	mov	r2, r8
	adds	r3, r4, #0
	bl	Func_0803bb58
	b.n	.L_0803b7e0
.L_0803b7d6:
	adds	r1, r7, #0
	mov	r2, r8
	adds	r3, r4, #0
	bl	UiText_MeasureEntryDimensions
.L_0803b7e0:
	ldr	r1, [r7, #0]
	cmp	r1, #0
	bne.n	.L_0803b7ee
	mov	r0, r8
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0803b85c
.L_0803b7ee:
	ldr	r2, [sp, #40]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0803b80c
	adds	r3, r1, #0
	adds	r3, #19
	lsrs	r3, r3, #3
	str	r3, [r7, #0]
	mov	r0, r8
	ldr	r3, [r0, #0]
	adds	r3, #15
	lsrs	r3, r3, #3
	str	r3, [r0, #0]
	ldr	r1, [r7, #0]
.L_0803b80c:
	adds	r3, r5, r1
	cmp	r3, fp
	ble.n	.L_0803b81e
	subs	r3, #30
	subs	r3, r5, r3
	adds	r5, r3, #0
	cmp	r3, #0
	bge.n	.L_0803b81e
	movs	r5, #0
.L_0803b81e:
	mov	r3, r8
	ldr	r2, [r3, #0]
	adds	r3, r6, r2
	cmp	r3, #20
	ble.n	.L_0803b834
	subs	r3, #20
	subs	r3, r6, r3
	adds	r6, r3, #0
	cmp	r3, #0
	bge.n	.L_0803b834
	movs	r6, #0
.L_0803b834:
	cmp	r5, #0
	bge.n	.L_0803b83a
	movs	r5, #0
.L_0803b83a:
	cmp	r6, #0
	bge.n	.L_0803b840
	movs	r6, #0
.L_0803b840:
	mov	r0, fp
	subs	r3, r0, r1
	cmp	r5, r3
	bls.n	.L_0803b84a
	adds	r5, r3, #0
.L_0803b84a:
	movs	r3, #20
	subs	r2, r3, r2
	cmp	r6, r2
	bls.n	.L_0803b854
	adds	r6, r2, #0
.L_0803b854:
	mov	r2, sl
	mov	r3, r9
	str	r5, [r2, #0]
	str	r6, [r3, #0]
.L_0803b85c:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
