@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b2720
	.thumb_func
Func_080b2720:
	push	{r5, lr}
	lsls	r3, r0, #5
	adds	r3, r3, r0
	adds	r5, r1, #0
	ldr	r1, [pc, #52]
	lsls	r2, r3, #1
	ldrsh	r3, [r1, r2]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r5, #0
	adds	r2, r2, r1
.L1:
	ldrh	r3, [r2, #0]
	adds	r4, #1
	strh	r3, [r0, #0]
	adds	r2, #2
	adds	r0, #2
	cmp	r4, #23
	bgt.n	.L0
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L1
.L0:
	ldr	r3, [pc, #12]
	lsls	r2, r4, #1
	strh	r3, [r2, r5]
	adds	r0, r4, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x00000000
	.4byte 0x080b41ac
