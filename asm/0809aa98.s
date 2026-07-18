@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809aa98
	.thumb_func
Func_0809aa98:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #240]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	movs	r2, #64
	adds	r2, r2, r7
	sub	sp, #12
	mov	r8, r3
	mov	sl, r2
.L4:
	mov	r3, sl
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	cmp	r6, #0
	bne.n	.L0
	ldr	r3, [r7, #20]
	str	r3, [sp, #0]
	ldr	r3, [r7, #24]
	mov	r8, sp
	str	r3, [sp, #8]
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	lsls	r5, r5, #16
	adds	r3, r0, #0
	lsls	r0, r3, #4
	asrs	r5, r5, #16
	subs	r0, r0, r3
	movs	r2, #160
	lsls	r5, r5, #16
	lsls	r2, r2, #14
	lsls	r0, r0, #1
	lsrs	r5, r5, #16
	adds	r0, r0, r2
	adds	r1, r5, #0
	mov	r2, r8
	bl	Func_0800447c
	mov	r2, r8
	ldr	r3, [r2, #0]
	str	r3, [r7, #12]
	ldr	r3, [r2, #8]
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	b.n	.L1
.L0:
	cmp	r6, #1
	bne.n	.L2
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L3
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L4
.L2:
	cmp	r6, #2
	bne.n	.L5
	mov	r2, r8
	ldr	r3, [r2, #4]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r2, r8
	ldr	r3, [r2, #12]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	Func_080974d8
	bl	Func_08004458
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r2, r7, #0
	strh	r3, [r7, #50]
	adds	r2, #66
	movs	r3, #1
	strb	r3, [r2, #0]
.L1:
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L3
.L5:
	cmp	r6, #3
	bne.n	.L3
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L3
	adds	r0, r7, #0
	bl	Func_0809bb34
.L3:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
