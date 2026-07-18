@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b1ec
	.thumb_func
Func_0801b1ec:
	push	{lr}
	ldr	r3, [pc, #48]
	ldr	r4, [pc, #48]
	ldr	r3, [r3, #0]
	adds	r2, r3, r4
	adds	r4, #2
	strh	r0, [r2, #0]
	adds	r2, r3, r4
	strh	r1, [r2, #0]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
.L1:
	strh	r0, [r3, #16]
	strh	r0, [r3, #24]
	strh	r1, [r3, #18]
	strh	r1, [r3, #26]
	ldr	r3, [r3, #4]
	adds	r0, #16
	cmp	r3, #0
	bne.n	.L1
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e98
	.4byte 0x00000396
