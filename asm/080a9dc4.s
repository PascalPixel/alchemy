@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9dc4
	.thumb_func
Func_080a9dc4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #100]
	ldr	r6, [r3, #0]
	adds	r7, r0, #0
	movs	r5, #0
.L3:
	ldrb	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L0
	cmp	r5, #4
	bhi.n	.L1
	ldr	r3, [pc, #84]
	lsls	r2, r5, #2
	ldr	r3, [r2, r3]
	mov	pc, r3
	.4byte 0x080a9df4
	.4byte 0x080a9df8
	.4byte 0x080a9dfc
	.4byte 0x080a9e00
	.4byte 0x080a9e04
	movs	r1, #16
	b.n	.L2
	movs	r1, #1
	b.n	.L2
	movs	r1, #2
	b.n	.L2
	movs	r1, #15
	b.n	.L2
	movs	r1, #7
	b.n	.L2
.L1:
	movs	r1, #0
	lsls	r2, r5, #2
.L2:
	adds	r3, r2, #0
	adds	r3, #200
	ldr	r3, [r6, r3]
	movs	r0, #8
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	Func_08015298
.L0:
	adds	r5, #1
	cmp	r5, #4
	ble.n	.L3
	movs	r0, #1
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x080a9de0
