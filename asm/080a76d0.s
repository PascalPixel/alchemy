@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a76d0
	.thumb_func
Func_080a76d0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #196]
	movs	r5, #0
	movs	r2, #13
	ldr	r7, [r3, #0]
	mov	r8, r5
	movs	r6, #0
	mov	sl, r2
	b.n	.L0
.L9:
	cmp	r5, #1
	beq.n	.L1
	cmp	r5, #1
	bgt.n	.L2
	cmp	r5, #0
	beq.n	.L3
	b.n	.L4
.L2:
	cmp	r5, #2
	beq.n	.L5
	cmp	r5, #3
	beq.n	.L6
	b.n	.L4
.L3:
	movs	r2, #186
	lsls	r2, r2, #1
	adds	r3, r7, r2
	mov	r2, r8
	strh	r2, [r3, #0]
	movs	r0, #0
	bl	Func_080a77a4
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L7
	movs	r2, #1
	adds	r6, r0, #0
	mov	r8, r2
.L7:
	movs	r5, #1
	b.n	.L0
.L1:
	ldr	r3, [r7, #20]
	mov	r2, sl
	strb	r2, [r3, #5]
	bl	Func_080a8114
	adds	r6, r0, #0
	mvns	r2, r6
	negs	r3, r2
	orrs	r3, r2
	lsrs	r5, r3, #31
	lsls	r5, r5, #1
	b.n	.L0
.L5:
	ldr	r3, [r7, #20]
	mov	r2, sl
	strb	r2, [r3, #5]
	bl	Func_080a90bc
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	movs	r5, #0
	cmp	r6, r3
	beq.n	.L0
	movs	r5, #3
	b.n	.L0
.L6:
	ldr	r3, [r7, #20]
	mov	r2, sl
	strb	r2, [r3, #5]
	bl	Func_080a96d8
	adds	r6, r0, #0
	mvns	r2, r6
	negs	r3, r2
	orrs	r3, r2
	lsrs	r5, r3, #31
	b.n	.L0
.L4:
	movs	r3, #1
	mov	r8, r3
.L0:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L8
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L9
.L8:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L10
	movs	r6, #1
	negs	r6, r6
.L10:
	adds	r0, r6, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
