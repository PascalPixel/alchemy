@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096f14
	.thumb_func
Func_08096f14:
	push	{r5, lr}
	ldr	r3, [pc, #52]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L0
	movs	r1, #7
	bl	Func_08009240
	b.n	.L1
.L0:
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009240
.L1:
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L2
	adds	r0, r5, #0
	bl	Func_08096ddc
.L2:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001e40
