@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08097f80
	.thumb_func
Func_08097f80:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r2, #64
	adds	r2, r2, r6
	sub	sp, #12
	mov	r8, r2
.L4:
	mov	r3, r8
	movs	r7, #0
	ldrsb	r7, [r3, r7]
	cmp	r7, #0
	bne.n	.L0
	ldr	r3, [r6, #20]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #24]
	str	r3, [r5, #8]
	bl	Func_08004458
	adds	r1, r0, #0
	lsls	r1, r1, #16
	movs	r0, #240
	adds	r2, r5, #0
	lsls	r0, r0, #13
	lsrs	r1, r1, #16
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #36]
	str	r3, [r6, #32]
	adds	r3, r6, #0
	adds	r3, #66
	strb	r7, [r3, #0]
	b.n	.L1
.L0:
	cmp	r7, #1
	bne.n	.L2
	adds	r0, r6, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L3
	mov	r2, r8
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L4
.L2:
	cmp	r7, #2
	bne.n	.L5
	ldr	r3, [r6, #20]
	str	r3, [r6, #12]
	ldr	r3, [r6, #24]
	str	r3, [r6, #16]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r6, #0
	strh	r3, [r6, #50]
	adds	r2, #66
	movs	r3, #1
	strb	r3, [r2, #0]
.L1:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L3
.L5:
	cmp	r7, #3
	bne.n	.L3
	adds	r0, r6, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L3
	adds	r0, r6, #0
	bl	Func_0809bb34
.L3:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
