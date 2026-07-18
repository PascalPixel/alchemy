@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808adf0
	.thumb_func
Func_0808adf0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	mov	r1, sp
	ldr	r5, [pc, #108]
	bl	Func_08009268
	movs	r1, #0
	mov	sl, r1
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldr	r1, [pc, #100]
	adds	r6, r0, #0
	asrs	r0, r1, #16
	ldrh	r2, [r5, #0]
	cmp	r3, r0
	beq.n	.L0
	adds	r7, r1, #0
	mov	r8, r0
.L4:
	lsls	r3, r2, #16
	ldr	r2, [sp, #0]
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L1
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	asrs	r2, r7, #16
	cmp	r3, r2
	beq.n	.L2
	cmp	r3, r6
	bne.n	.L1
.L2:
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	cmp	r0, r2
	beq.n	.L3
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L1
.L3:
	movs	r2, #6
	ldrsh	r1, [r5, r2]
	mov	sl, r1
	b.n	.L0
.L1:
	adds	r5, #8
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, r8
	bne.n	.L4
.L0:
	adds	r0, r6, #0
	bl	Func_0808b2b0
	mov	r0, sl
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0809d7a8
	.4byte 0xffff0000
