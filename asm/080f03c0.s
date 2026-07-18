@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f03c0
	.thumb_func
Func_080f03c0:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	ldr	r2, [pc, #24]
	ldr	r1, [pc, #24]
	ldrh	r3, [r2, #8]
	adds	r3, r3, r1
	strh	r3, [r2, #8]
	ldrh	r3, [r2, #12]
	adds	r3, r3, r1
	strh	r3, [r2, #12]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001800
	.4byte 0x03001ad0
	.4byte 0x0000ffff
