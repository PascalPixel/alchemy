@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08093168
	.thumb_func
Func_08093168:
	push	{r5, r6, lr}
	adds	r0, r2, #0
	ldr	r2, [pc, #96]
	ldr	r6, [r2, #0]
	adds	r2, r3, #0
	adds	r1, r0, #0
	cmp	r2, #119
	ble.n	.L0
	adds	r2, #32
	b.n	.L1
.L0:
	subs	r2, #32
.L1:
	cmp	r0, #8
	bge.n	.L2
	movs	r1, #8
.L2:
	movs	r3, #156
	lsls	r3, r3, #1
	cmp	r1, r3
	ble.n	.L3
	adds	r1, r3, #0
.L3:
	cmp	r2, #20
	bge.n	.L4
	movs	r2, #20
.L4:
	cmp	r2, #220
	ble.n	.L5
	movs	r2, #220
.L5:
	movs	r0, #236
	lsls	r0, r0, #1
	adds	r3, r6, r0
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	movs	r3, #1
	bl	Func_08015038
	adds	r5, r0, #0
	b.n	.L6
.L7:
	movs	r0, #1
	bl	Func_080030f8
.L6:
	adds	r0, r5, #0
	bl	Func_08015050
	cmp	r0, #0
	beq.n	.L7
	movs	r0, #236
	lsls	r0, r0, #1
	adds	r2, r6, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
