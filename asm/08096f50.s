@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096f50
	.thumb_func
Func_08096f50:
	push	{r5, r6, lr}
	ldr	r5, [pc, #52]
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	adds	r6, r0, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [r5, #0]
	movs	r1, #6
	lsrs	r0, r0, #1
	bl	Func_08002304
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	Func_08009240
.L0:
	ldr	r3, [r5, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L1
	adds	r0, r6, #0
	bl	Func_08096ddc
.L1:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e40
