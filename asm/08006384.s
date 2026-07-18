@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006384
	.thumb_func
Func_08006384:
	push	{r5, r6, lr}
	ldr	r1, [pc, #44]
	adds	r5, r0, #0
	ldrh	r2, [r1, #0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, r5
	beq.n	.L0
	adds	r6, r1, #0
.L1:
	movs	r0, #1
	bl	Func_080030f8
	ldrh	r2, [r6, #0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, r5
	bne.n	.L1
.L0:
	ldr	r3, [pc, #16]
	ldr	r0, [r3, #0]
	lsls	r0, r0, #26
	lsrs	r0, r0, #30
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001f64
	.4byte 0x04000128
