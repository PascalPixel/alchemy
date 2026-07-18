@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080faa58
	.thumb_func
Func_080faa58:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r7, r1, #0
	ldr	r1, [r5, #52]
	ldr	r0, [pc, #208]
	cmp	r1, r0
	bne.n	.L0
	ldrb	r0, [r5, #11]
	ldrb	r2, [r7, #2]
	cmp	r0, #0
	beq.n	.L1
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L2
	ldr	r1, [r5, #44]
	movs	r0, #64
	ldrb	r1, [r1, #0]
	ands	r0, r1
	cmp	r0, #0
	bne.n	.L3
.L2:
	ldr	r1, [r5, #4]
	ldrh	r0, [r5, #4]
	cmp	r0, #0
	beq.n	.L1
	cmp	r1, #0
	blt.n	.L1
.L3:
	ldrb	r0, [r7, #2]
	adds	r2, r0, #0
	ldrb	r0, [r5, #9]
	cmp	r0, r2
	bhi.n	.L0
.L1:
	ldr	r0, [r5, #52]
	adds	r0, #1
	str	r0, [r5, #52]
	movs	r1, #0
	str	r1, [r5, #4]
	str	r7, [r5, #0]
	ldr	r0, [r7, #4]
	str	r0, [r5, #48]
	strb	r2, [r5, #9]
	str	r1, [r5, #12]
	movs	r0, #150
	strh	r0, [r5, #28]
	strh	r0, [r5, #32]
	adds	r0, #106
	strh	r0, [r5, #30]
	strh	r1, [r5, #34]
	strh	r1, [r5, #36]
	movs	r6, #0
	ldr	r4, [r5, #44]
	ldrb	r1, [r7, #0]
	cmp	r6, r1
	bge.n	.L4
	ldrb	r0, [r5, #8]
	cmp	r6, r0
	bge.n	.L5
	mov	r8, r6
.L6:
	adds	r0, r5, #0
	adds	r1, r4, #0
	bl	Func_080f9ef8
	movs	r0, #192
	strb	r0, [r4, #0]
	mov	r1, r8
	str	r1, [r4, #32]
	lsls	r1, r6, #2
	adds	r0, r7, #0
	adds	r0, #8
	adds	r0, r0, r1
	ldr	r0, [r0, #0]
	str	r0, [r4, #64]
	adds	r6, #1
	adds	r4, #80
	ldrb	r0, [r7, #0]
	cmp	r6, r0
	bge.n	.L4
	ldrb	r1, [r5, #8]
	cmp	r6, r1
	blt.n	.L6
.L4:
	ldrb	r0, [r5, #8]
	cmp	r6, r0
	bge.n	.L5
	movs	r1, #0
	mov	r8, r1
.L7:
	adds	r0, r5, #0
	adds	r1, r4, #0
	bl	Func_080f9ef8
	mov	r0, r8
	strb	r0, [r4, #0]
	adds	r6, #1
	adds	r4, #80
	ldrb	r1, [r5, #8]
	cmp	r6, r1
	blt.n	.L7
.L5:
	movs	r0, #128
	ldrb	r1, [r7, #3]
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L8
	ldrb	r0, [r7, #3]
	bl	Func_080fa83c
.L8:
	ldr	r0, [pc, #12]
	str	r0, [r5, #52]
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x68736d53
