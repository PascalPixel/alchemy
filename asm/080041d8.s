@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080041d8
	.thumb_func
Func_080041d8:
	push	{r5, r6, lr}
	ldr	r3, [pc, #84]
	movs	r5, #1
	ldr	r4, [pc, #84]
	ldrb	r3, [r3, #0]
	negs	r5, r5
	ldr	r3, [pc, #80]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r2, #0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L0
	strh	r1, [r4, #4]
	movs	r5, #0
	b.n	.L1
.L0:
	adds	r2, #1
	adds	r4, #8
	cmp	r2, #19
	bgt.n	.L1
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L0
	strh	r1, [r4, #4]
	adds	r5, r2, #0
.L1:
	movs	r3, #1
	negs	r3, r3
	ldr	r4, [pc, #32]
	cmp	r5, r3
	bne.n	.L2
	ldr	r3, [r4, #0]
	movs	r2, #0
	cmp	r3, #0
	bne.n	.L3
	ldr	r3, [pc, #12]
	str	r0, [r4, #0]
	strh	r1, [r4, #4]
	strb	r3, [r4, #6]
	movs	r5, #0
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001a10
	.4byte 0x03001a20
	.4byte 0x04000208
.L3:
	adds	r2, #1
	adds	r4, #8
	cmp	r2, #19
	bgt.n	.L2
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L3
	ldr	r3, [pc, #20]
	str	r0, [r4, #0]
	strh	r1, [r4, #4]
	strb	r3, [r4, #6]
	adds	r5, r2, #0
.L2:
	bl	Func_08004144
	ldr	r3, [pc, #8]
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	b.n	.L4
	.4byte 0x00000000
	.4byte 0x04000208
.L4:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	bx	lr
