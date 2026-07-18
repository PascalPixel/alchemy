@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3d9c
	.thumb_func
Func_080a3d9c:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	bl	Func_08077008
	ldr	r4, [pc, #48]
	movs	r5, #0
	movs	r1, #0
	adds	r0, #216
.L2:
	ldrh	r2, [r0, #0]
	adds	r3, r2, #0
	adds	r0, #2
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, r6
	bne.n	.L0
	movs	r3, #248
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r5, r3, #11
	adds	r5, #1
	b.n	.L1
.L0:
	adds	r1, #1
	cmp	r1, #14
	ble.n	.L2
.L1:
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
