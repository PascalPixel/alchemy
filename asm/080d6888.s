@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d6888
	.thumb_func
Func_080d6888:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	mov	fp, r3
	str	r0, [sp, #16]
	mov	r9, r1
	str	r2, [sp, #12]
	bl	Func_080b5098
	ldr	r3, [pc, #176]
	ldr	r3, [r3, #0]
	mov	r8, r0
	movs	r0, #0
	str	r3, [sp, #8]
	str	r0, [sp, #4]
	b.n	.L0
.L8:
	movs	r2, #1
	negs	r2, r2
	cmp	fp, r2
	beq.n	.L1
	add	r1, sp, #52
	ldr	r3, [pc, #156]
	ldrb	r0, [r1, #0]
	ldr	r1, [sp, #8]
	add	r3, fp
	strb	r0, [r1, r3]
.L1:
	mov	r1, r8
	movs	r0, #42
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L2
	cmp	r9, r2
	beq.n	.L3
	movs	r3, #39
	adds	r3, r3, r7
	mov	sl, r3
	ldrb	r3, [r3, #0]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L3
	adds	r6, r7, #0
	adds	r6, #40
.L7:
	ldmia	r6!, {r5}
	cmp	r5, #0
	beq.n	.L4
	mov	r0, r8
	ldr	r3, [r0, #36]
	cmp	r5, r3
	beq.n	.L4
	ldr	r3, [r0, #32]
	cmp	r5, r3
	beq.n	.L4
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L5
	ldr	r0, [sp, #16]
	str	r2, [sp, #0]
	bl	Func_080b5100
	strb	r0, [r5, #5]
	ldr	r2, [sp, #0]
	b.n	.L6
.L5:
	mov	r3, r9
	strb	r3, [r5, #5]
.L6:
	movs	r3, #255
	strb	r3, [r5, #22]
.L4:
	mov	r0, sl
	ldrb	r3, [r0, #0]
	adds	r2, #1
	cmp	r2, r3
	bne.n	.L7
.L3:
	movs	r2, #1
	ldr	r1, [sp, #12]
	negs	r2, r2
	cmp	r1, r2
	beq.n	.L2
	adds	r0, r7, #0
	bl	Func_08009020
.L2:
	ldr	r3, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #4]
.L0:
	mov	r1, r8
	ldr	r0, [r1, #0]
	ldr	r1, [sp, #4]
	bl	Func_080b50d8
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L8
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007818
