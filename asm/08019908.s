@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019908
	.thumb_func
Func_08019908:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #44]
	adds	r7, r1, #0
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #40]
	ldr	r4, [pc, #44]
	adds	r6, r0, #0
	movs	r5, #8
	movs	r0, #0
	adds	r2, r1, r3
.L2:
	ldrh	r3, [r4, r1]
	cmp	r3, #0
	bne.n	.L0
	str	r6, [r2, #0]
	strh	r7, [r4, r1]
	b.n	.L1
.L0:
	adds	r0, #1
	adds	r2, #4
	adds	r4, #2
	cmp	r0, r5
	bne.n	.L2
.L1:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x000012bc
	.4byte 0x000012dc
