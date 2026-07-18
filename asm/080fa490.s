@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa490
	.thumb_func
Func_080fa490:
	push	{r4, r5, lr}
	ldr	r0, [pc, #32]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L0
	ldr	r5, [pc, #24]
	adds	r4, r0, #0
.L1:
	ldr	r0, [r5, #0]
	bl	Func_080fa264
	adds	r5, #12
	subs	r4, #1
	cmp	r4, #0
	bne.n	.L1
.L0:
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	.4byte 0x00000008
	.4byte 0x080fc624
