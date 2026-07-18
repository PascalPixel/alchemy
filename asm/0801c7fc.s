@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801c7fc
	.thumb_func
Func_0801c7fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #8]
	add	r5, sp, #12
	movs	r0, #0
	mov	r9, r0
	adds	r0, r5, #0
	bl	Func_08077158
	cmp	r9, r0
	bge.n	.L0
	mov	fp, r5
	mov	sl, r0
.L3:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	movs	r0, #2
	mov	r8, r3
	add	fp, r0
	mov	r0, r8
	bl	Func_08077008
	movs	r3, #88
	ldr	r2, [pc, #24]
	ldrh	r3, [r0, r3]
	adds	r5, r2, #0
	ands	r5, r3
	movs	r1, #0
	cmp	r5, #0
	beq.n	.L1
	adds	r7, r0, #0
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r0, [sp, #8]
	adds	r7, #88
	adds	r6, r3, r0
	b.n	.L2
	.4byte 0x00003fff
.L2:
	adds	r0, r5, #0
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	Func_08077080
	ldr	r1, [sp, #4]
	mov	r3, r8
	movs	r0, #1
	adds	r1, #1
	strh	r3, [r6, #0]
	strh	r5, [r6, #2]
	add	r9, r0
	adds	r6, #4
	ldr	r2, [sp, #0]
	cmp	r1, #31
	bgt.n	.L1
	adds	r7, #4
	ldrh	r3, [r7, #0]
	adds	r5, r2, #0
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L2
.L1:
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r0, sl
	cmp	r0, #0
	bne.n	.L3
.L0:
	mov	r0, r9
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
