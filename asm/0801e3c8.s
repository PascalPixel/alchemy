@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e3c8
	.thumb_func
Func_0801e3c8:
	push	{lr}
	ldr	r3, [pc, #68]
	ldr	r1, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [pc, #64]
	movs	r4, #226
	adds	r2, r1, r3
	lsls	r4, r4, #4
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r3, #128
	adds	r2, r1, r4
.L1:
	adds	r3, #1
	strb	r0, [r2, #0]
	adds	r2, #1
	cmp	r3, #255
	ble.n	.L1
	b.n	.L2
.L0:
	ldr	r3, [pc, #32]
	movs	r4, #226
	adds	r2, r1, r3
	lsls	r4, r4, #4
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #0
	adds	r2, r1, r4
	movs	r3, #127
.L3:
	subs	r3, #1
	strb	r0, [r2, #0]
	adds	r2, #1
	cmp	r3, #0
	bge.n	.L3
.L2:
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea2
	bx	lr
