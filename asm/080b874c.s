@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b874c
	.thumb_func
Func_080b874c:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	adds	r0, r7, #0
	bl	Func_080b8f08
	ldr	r3, [pc, #140]
	ldr	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	strh	r0, [r7, #10]
	ldr	r1, [pc, #136]
	cmp	r3, #4
	bgt.n	.L2
	movs	r1, #128
	lsls	r1, r1, #6
.L2:
	movs	r3, #60
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	bl	Func_08015118
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	cmp	r3, #2
	beq.n	.L3
	cmp	r3, #2
	bgt.n	.L4
	cmp	r3, #0
	beq.n	.L5
	cmp	r3, #1
	beq.n	.L6
	b.n	.L5
.L4:
	cmp	r3, #3
	beq.n	.L7
	cmp	r3, #99
	bne.n	.L5
	ldr	r0, [pc, #88]
	bl	Func_080151c8
	adds	r0, r7, #0
	bl	Func_080b8824
	cmp	r0, #0
	beq.n	.L8
	movs	r0, #1
	b.n	.L1
.L7:
	movs	r0, #45
	bl	Func_080030f8
	adds	r0, r7, #0
	bl	Func_080b8888
	b.n	.L8
.L3:
	movs	r0, #45
	bl	Func_080030f8
	adds	r0, r7, #0
	bl	Func_080b8c1c
	b.n	.L8
.L5:
	ldr	r3, [pc, #32]
	ldr	r6, [r3, #0]
	movs	r5, #0
	str	r5, [r6, #20]
	adds	r0, r7, #0
	bl	Func_080b8c1c
	str	r5, [r6, #20]
	b.n	.L8
.L6:
	adds	r0, r7, #0
	bl	Func_080b88d0
.L8:
	bl	Func_08015220
	movs	r0, #0
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x00000843
