@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b2da8
	.thumb_func
Func_080b2da8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r5, r1, #0
	bl	Func_08077008
	adds	r2, r0, #0
	cmp	r5, #0
	bne.n	.L0
	ldrh	r3, [r2, #52]
	adds	r0, r7, #0
	strh	r3, [r2, #56]
	bl	Func_08077128
	b.n	.L1
.L0:
	cmp	r5, #1
	bne.n	.L2
	ldr	r3, [pc, #92]
	b.n	.L3
.L2:
	cmp	r5, #2
	bne.n	.L4
	movs	r3, #160
	lsls	r3, r3, #1
.L3:
	adds	r2, r2, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L1
.L4:
	cmp	r5, #3
	bne.n	.L1
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r5, r2, #0
	mov	r8, r3
	movs	r6, #14
	adds	r5, #216
.L6:
	ldrh	r2, [r5, #0]
	mov	r3, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	ldrh	r0, [r5, #0]
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	ldrh	r2, [r5, #0]
	mov	r3, r8
	eors	r3, r2
	strh	r3, [r5, #0]
	adds	r0, r7, #0
	bl	Func_08077010
.L5:
	subs	r6, #1
	adds	r5, #2
	cmp	r6, #0
	bge.n	.L6
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000131
