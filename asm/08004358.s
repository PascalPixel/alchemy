@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004358
	.thumb_func
Func_08004358:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	movs	r5, #1
	ldr	r1, [pc, #52]
	negs	r5, r5
	ldr	r3, [pc, #52]
	ldrh	r2, [r3, #0]
	strh	r3, [r3, #0]
	movs	r4, #0
	ldr	r3, [r1, #0]
	cmp	r3, r0
	bne.n	.L0
	strb	r6, [r1, #6]
	movs	r5, #0
	b.n	.L1
.L0:
	adds	r4, #1
	adds	r1, #8
	cmp	r4, #19
	bgt.n	.L1
	ldr	r3, [r1, #0]
	cmp	r3, r0
	bne.n	.L0
	strb	r6, [r1, #6]
	adds	r5, r4, #0
.L1:
	ldr	r3, [pc, #12]
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001a20
	.4byte 0x04000208
