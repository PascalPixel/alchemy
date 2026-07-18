@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807961c
	.thumb_func
Func_0807961c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_080795fc
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_08079358
	movs	r2, #0
	cmp	r2, r5
	bge.n	.L0
	ldr	r0, [pc, #44]
	movs	r3, #252
	lsls	r3, r3, #1
	adds	r1, r0, r3
.L2:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, r6
	beq.n	.L1
	adds	r2, #1
	cmp	r2, r5
	blt.n	.L2
	b.n	.L3
.L1:
	adds	r0, r5, #0
	b.n	.L4
.L0:
	ldr	r0, [pc, #16]
.L3:
	movs	r1, #252
	lsls	r1, r1, #1
	adds	r3, r2, r1
	strb	r6, [r0, r3]
	adds	r0, r5, #1
.L4:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
