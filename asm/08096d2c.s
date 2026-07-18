@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096d2c
	.thumb_func
Func_08096d2c:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r3, #1
	ldr	r6, [r5, #104]
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #31
	ble.n	.L0
	ldr	r1, [pc, #56]
	adds	r0, r5, #0
	bl	Func_08009098
	b.n	.L1
.L0:
	lsls	r0, r0, #10
	bl	Func_08002322
	str	r0, [r5, #24]
	str	r0, [r5, #28]
	ldr	r3, [r6, #8]
	movs	r1, #128
	str	r3, [r5, #8]
	ldr	r3, [r5, #12]
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r5, #12]
	subs	r1, r1, r0
	ldr	r3, [r6, #16]
	lsls	r2, r1, #2
	adds	r2, r2, r1
	adds	r3, r3, r2
	movs	r2, #144
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #16]
.L1:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0809f0b0
