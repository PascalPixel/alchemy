@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019da8
	.thumb_func
Func_08019da8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r3, #0
	ldr	r3, [pc, #132]
	adds	r6, r2, #0
	sub	sp, #8
	mov	sl, r0
	mov	fp, r1
	ldr	r5, [r3, #0]
	bl	Func_08019d2c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	movs	r3, #4
	ldr	r2, [pc, #108]
	negs	r3, r3
	mov	r9, r3
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	mov	r8, r9
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #2
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r3, #5
	adds	r1, r7, #0
	movs	r2, #6
	bl	Func_080162d4
	movs	r3, #0
	adds	r5, r0, #0
	mov	r8, r3
	b.n	.L3
.L2:
	movs	r3, #2
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r7, #0
	movs	r2, #5
	movs	r3, #5
	bl	Func_080162d4
	adds	r5, r0, #0
.L3:
	cmp	r5, #0
	beq.n	.L4
	mov	r3, r8
	movs	r2, #1
	str	r3, [sp, #0]
	mov	r3, r9
	str	r3, [sp, #4]
	negs	r2, r2
	mov	r0, sl
	mov	r1, fp
	adds	r3, r5, #0
	bl	Func_0801ec6c
.L4:
	adds	r0, r5, #0
.L1:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea4
