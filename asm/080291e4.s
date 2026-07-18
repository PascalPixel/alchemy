@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080291e4
	.thumb_func
Func_080291e4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	movs	r3, #8
	str	r3, [sp, #4]
	add	r6, sp, #12
	movs	r5, #0
	movs	r3, #2
	str	r5, [sp, #12]
	movs	r2, #28
	str	r5, [r6, #4]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r0, #1
	movs	r3, #20
	bl	Func_080162d4
	ldr	r1, [sp, #4]
	adds	r5, r0, #0
	bl	Func_080292c4
	add	r7, sp, #20
	add	r1, sp, #8
	adds	r0, r7, #0
	bl	Func_0801c0dc
	add	r3, sp, #4
	mov	r8, r3
	b.n	.L0
.L2:
	cmp	r0, #1
	bne.n	.L1
	ldr	r1, [sp, #4]
	adds	r0, r5, #0
	bl	Func_080292c4
.L1:
	ldr	r1, [sp, #12]
	ldr	r2, [r6, #4]
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #58
	adds	r2, #20
	adds	r0, r7, #0
	bl	Func_0801c154
.L0:
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r5, #0
	mov	r1, r8
	adds	r2, r6, #0
	bl	Func_0802938c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L2
	ldr	r0, [sp, #8]
	bl	Func_0801c17c
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08016418
	movs	r0, #0
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
