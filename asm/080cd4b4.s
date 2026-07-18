@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cd4b4
	.thumb_func
Func_080cd4b4:
	push	{r5, lr}
	ldr	r3, [pc, #64]
	ldr	r1, [pc, #64]
	ldr	r2, [r3, #120]
	adds	r5, r2, r1
	ldr	r0, [r3, #0]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L0
	adds	r1, #4
	adds	r3, r2, r1
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r0, r0, r3
	lsls	r3, r2, #4
	adds	r3, r3, r2
	lsls	r3, r3, #4
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #9
	subs	r2, r2, r3
	ldr	r1, [pc, #28]
	movs	r3, #128
	bl	Func_080b50f0
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001e74
	.4byte 0x000077b4
	.4byte 0x00000544
	.4byte 0x050000c0
