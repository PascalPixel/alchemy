@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017a64
	.thumb_func
Func_08017a64:
	push	{lr}
	ldrh	r2, [r0, #0]
	movs	r1, #0
	adds	r0, #2
	cmp	r2, #0
	beq.n	.L0
	ldr	r4, [pc, #44]
.L4:
	cmp	r2, #32
	bne.n	.L1
	adds	r1, #4
	b.n	.L2
.L1:
	cmp	r2, #255
	bhi.n	.L3
	adds	r3, r2, #0
	subs	r3, #222
	cmp	r3, #1
	bls.n	.L2
	adds	r3, #190
	lsls	r3, r3, #5
	ldrh	r3, [r3, r4]
	adds	r1, r1, r3
	b.n	.L2
.L3:
	adds	r1, #10
.L2:
	ldrh	r2, [r0, #0]
	adds	r0, #2
	cmp	r2, #0
	bne.n	.L4
.L0:
	adds	r0, r1, #0
	pop	{r1}
	bx	r1
	.4byte 0x08032224
