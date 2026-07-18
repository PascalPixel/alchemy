@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b36c
	.thumb_func
Func_0801b36c:
	push	{lr}
	movs	r2, #210
	lsls	r2, r2, #2
	ldr	r4, [pc, #32]
	adds	r3, r0, r2
	ldr	r2, [r3, #0]
	adds	r3, r0, r4
	ldrh	r3, [r3, #0]
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r0, r4
	ldrh	r0, [r3, #0]
.L1:
	adds	r1, #1
	ldr	r2, [r2, #4]
	cmp	r1, r0
	bne.n	.L1
.L0:
	adds	r0, r2, #0
	pop	{r1}
	bx	r1
	.4byte 0x0000039e
