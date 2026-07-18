@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1438
	.thumb_func
Func_080c1438:
	push	{lr}
	ldr	r3, [pc, #44]
	adds	r2, r3, #0
	adds	r2, #160
	ldr	r0, [r2, #0]
	adds	r3, #156
	ldr	r3, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #158
	lsls	r1, r1, #5
	adds	r2, r3, r1
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r1, [pc, #12]
	lsls	r2, r2, #7
	bl	Func_080054e4
.L0:
	pop	{r1}
	bx	r1
	.4byte 0x03001e50
	.4byte 0x06004000
