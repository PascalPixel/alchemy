@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078ee8
	.thumb_func
Func_08078ee8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #32
	ldr	r1, [pc, #152]
	mov	sl, sp
	mov	r8, r1
	movs	r6, #0
	mov	r9, sl
.L2:
	adds	r0, r6, #0
	bl	Func_08077394
	adds	r7, r0, #0
	ldr	r0, [pc, #140]
	mov	r1, r9
	adds	r0, r6, r0
	bl	Func_08015020
	mov	r2, r9
	ldrh	r3, [r2, #0]
	strb	r3, [r7, #0]
	ldrh	r3, [r2, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	mov	r1, sl
	adds	r2, r7, #0
	movs	r0, #0
.L1:
	adds	r5, #1
	adds	r0, #2
	cmp	r5, #13
	bgt.n	.L0
	ldrh	r3, [r0, r1]
	adds	r2, #1
	strb	r3, [r2, #0]
	ldrh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L1
.L0:
	movs	r3, #0
	adds	r6, #1
	strb	r3, [r7, #14]
	cmp	r6, #7
	ble.n	.L2
	mov	r3, r8
	movs	r1, #1
	ldr	r0, [r3, #0]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L3
.L7:
	bl	Func_08077394
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L4
	mov	r3, r8
	movs	r1, #148
	ldr	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	ldrb	r0, [r3, #0]
	bl	Func_08078ed8
	adds	r3, r7, #0
	ldr	r2, [pc, #28]
	mov	sl, r0
	movs	r5, #14
	adds	r3, #244
.L5:
	subs	r5, #1
	strh	r2, [r3, #0]
	subs	r3, #2
	cmp	r5, #0
	bge.n	.L5
	ldr	r2, [pc, #24]
	mov	r6, sl
	movs	r5, #0
	adds	r6, #152
	mov	r9, r2
	b.n	.L6
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0807b690
	.4byte 0x00000066
	.4byte 0x000001ff
.L6:
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldrh	r3, [r6, #0]
	mov	r1, r9
	ands	r1, r3
	bl	Func_08078588
	mov	r2, r8
	adds	r1, r0, #0
	adds	r5, #1
	ldr	r0, [r2, #0]
	adds	r6, #2
	bl	Func_08078708
	cmp	r5, #12
	bls.n	.L6
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl	Func_08079ae8
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #22]
	strh	r3, [r7, #20]
	mov	r3, sl
	mov	r1, r8
	adds	r3, #150
	ldr	r0, [r1, #0]
	ldrb	r1, [r3, #0]
	bl	Func_080792fc
	mov	r2, r8
	ldr	r0, [r2, #0]
	bl	Func_08077428
.L4:
	movs	r3, #4
	add	r8, r3
	mov	r1, r8
	ldr	r3, [r1, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r0, r3, #0
	cmp	r3, r2
	bne.n	.L7
.L3:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	bx	lr
