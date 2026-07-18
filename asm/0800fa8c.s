@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800fa8c
	.thumb_func
Func_0800fa8c:
	push	{r5, r6, lr}
	movs	r4, #128
	movs	r0, #1
	ldr	r5, [pc, #44]
	ldr	r6, [pc, #44]
	lsls	r4, r4, #7
	negs	r0, r0
.L2:
	ldmia	r5!, {r1}
	adds	r2, r1, #0
	ands	r2, r6
	cmp	r2, r6
	bne.n	.L0
	cmp	r0, r2
	beq.n	.L1
	adds	r0, #1
.L1:
	adds	r3, r1, r0
	subs	r1, r3, r2
	subs	r3, r5, #4
	str	r1, [r3, #0]
.L0:
	subs	r4, #1
	cmp	r4, #0
	bne.n	.L2
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x00000fff
