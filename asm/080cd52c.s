@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cd52c
	.thumb_func
Func_080cd52c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #84]
	movs	r1, #1
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #80]
	negs	r1, r1
	sub	sp, #8
	movs	r4, #0
	mov	r8, r1
	movs	r7, #36
	adds	r5, r6, r3
.L1:
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	lsrs	r2, r3, #24
	cmp	r2, #0
	bne.n	.L0
	ldr	r1, [pc, #52]
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	ldrsh	r0, [r3, r7]
	str	r2, [sp, #0]
	movs	r1, #0
	mov	r2, r8
	mov	r3, r8
	str	r4, [sp, #4]
	bl	Func_080d6888
	ldr	r4, [sp, #4]
.L0:
	adds	r4, #1
	adds	r7, #2
	adds	r5, #1
	cmp	r4, #8
	bne.n	.L1
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007818
	.4byte 0x00007828
