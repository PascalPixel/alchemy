@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f7db4
	.thumb_func
Func_080f7db4:
	push	{lr}
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #48]
	movs	r2, #0
	movs	r1, #0
	adds	r3, #4
.L0:
	str	r2, [r3, #4]
	adds	r2, #1
	str	r1, [r3, #0]
	adds	r3, #12
	cmp	r2, r0
	ble.n	.L0
	ldr	r3, [pc, #24]
	movs	r2, #192
	ldr	r3, [r3, #0]
	lsls	r2, r2, #6
	adds	r3, r3, r2
	movs	r1, #0
	movs	r2, #255
.L1:
	subs	r2, #1
	stmia	r3!, {r1}
	cmp	r2, #0
	bge.n	.L1
	pop	{r0}
	bx	r0
	.4byte 0x02004c00
	.4byte 0x000003ff
