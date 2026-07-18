@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003ed4
	.thumb_func
Func_08003ed4:
	push	{lr}
	movs	r2, #128
	ldr	r4, [pc, #36]
	movs	r1, #0
	movs	r0, #0
	lsls	r2, r2, #2
.L2:
	ldrb	r3, [r4, #0]
	adds	r4, #1
	cmp	r3, #255
	beq.n	.L0
	movs	r1, #0
	b.n	.L1
.L0:
	adds	r1, #1
	cmp	r0, r1
	bge.n	.L1
	adds	r0, r1, #0
.L1:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L2
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001810
