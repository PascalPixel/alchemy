@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006408
	.thumb_func
Func_08006408:
	push	{r5, r6, lr}
	ldr	r5, [pc, #56]
	ldr	r4, [r5, #0]
	ldr	r6, [pc, #56]
	cmp	r4, #0
	beq.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r2, [pc, #48]
	ldrh	r1, [r2, #0]
	strh	r2, [r2, #0]
	movs	r3, #129
	strb	r3, [r6, #1]
	ldr	r3, [pc, #40]
	strh	r4, [r3, #0]
	movs	r3, #1
	strb	r3, [r6, #0]
	str	r0, [r5, #0]
	ldr	r3, [pc, #36]
	ldr	r0, [pc, #12]
	strb	r0, [r3, #0]
	strh	r1, [r2, #0]
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x020023ac
	.4byte 0x02002220
	.4byte 0x04000208
	.4byte 0x02002238
	.4byte 0x020023a4
