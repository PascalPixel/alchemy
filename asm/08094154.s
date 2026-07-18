@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08094154
	.thumb_func
Func_08094154:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	Func_0808ba1c
	adds	r4, r0, #0
	cmp	r4, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #0]
	adds	r3, #228
	ldr	r1, [pc, #100]
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r3, [r4, #16]
	ands	r2, r1
	ands	r0, r1
	ldr	r1, [r4, #8]
	subs	r3, r3, r2
	ldr	r2, [r4, #12]
	subs	r1, r1, r0
	subs	r6, r3, r2
	adds	r2, r5, #0
	adds	r5, #4
	cmp	r1, #0
	bge.n	.L2
	ldr	r3, [pc, #72]
	adds	r1, r1, r3
.L2:
	asrs	r3, r1, #16
	str	r3, [r2, #0]
	adds	r3, r6, #0
	cmp	r3, #0
	bge.n	.L3
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
.L3:
	asrs	r3, r3, #16
	str	r3, [r5, #0]
	adds	r3, r4, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L4
	ldr	r3, [r4, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	Func_08185000
	ldr	r3, [r5, #0]
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	subs	r3, r3, r2
	str	r3, [r5, #0]
.L4:
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0xffff0000
	.4byte 0x0000ffff
	bx	lr
