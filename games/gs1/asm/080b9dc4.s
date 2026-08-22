@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9dc4
	.thumb_func
Func_080b9dc4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #232]
	ldr	r1, [r3, #0]
	subs	r3, #140
	ldr	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r1, #0]
	movs	r3, #1
	str	r3, [r1, #16]
	adds	r5, r0, #0
	mov	r8, r1
	movs	r0, #0
	movs	r1, #0
	sub	sp, #32
	bl	Func_080c10e8
	ldrb	r3, [r5, #0]
	movs	r7, #0
	cmp	r3, #7
	bhi.n	.L0
	adds	r3, r6, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	adds	r2, r7, #0
	cmp	r3, #2
	beq.n	.L1
	movs	r2, #1
.L1:
	cmp	r2, #0
	bne.n	.L2
	ldr	r0, [pc, #176]
	bl	Func_080151c8
	bl	Func_080bb65c
	b.n	.L3
.L2:
	add	r7, sp, #4
	movs	r0, #1
	adds	r1, r7, #0
	bl	Func_080b6b40
	movs	r2, #1
	subs	r6, r0, #1
	negs	r2, r2
	cmp	r6, r2
	beq.n	.L4
	lsls	r5, r6, #1
.L6:
	ldrsh	r0, [r7, r5]
	bl	Func_08077008
	ldr	r1, [pc, #140]
	adds	r2, r0, #0
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L5
	adds	r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L5
	ldrsh	r0, [r5, r7]
	bl	Func_080b8064
	movs	r0, #8
	bl	Func_080030f8
.L5:
	movs	r3, #1
	subs	r6, #1
	negs	r3, r3
	subs	r5, #2
	cmp	r6, r3
	bne.n	.L6
.L4:
	movs	r0, #22
	bl	Func_080030f8
	movs	r7, #1
	b.n	.L3
.L0:
	bl	Func_08004458
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	cmp	r3, #6
	bhi.n	.L7
	ldrb	r0, [r5, #0]
	mov	r2, sp
	movs	r3, #255
	strh	r0, [r2, #0]
	strh	r3, [r2, #2]
	bl	Func_080b8064
	movs	r0, #8
	bl	Func_080030f8
	ldrb	r0, [r5, #0]
	bl	Func_080bac6c
	ldrb	r0, [r5, #0]
	bl	Func_080b7e60
	b.n	.L3
.L7:
	ldr	r0, [pc, #32]
	bl	Func_080151c8
	bl	Func_080bb65c
.L3:
	movs	r3, #0
	mov	r1, r8
	adds	r0, r7, #0
	str	r3, [r1, #16]
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f00
	.4byte 0x00000847
	.4byte 0x0000013b
