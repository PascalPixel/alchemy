@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9acc
	.thumb_func
Func_080b9acc:
	push	{lr}
	ldr	r3, [pc, #80]
	ldr	r0, [pc, #80]
	ldr	r1, [r3, #0]
	adds	r3, #128
	ldr	r4, [r3, #0]
	movs	r2, #128
	ldr	r3, [r0, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	sub	sp, #4
	cmp	r3, #0
	beq.n	.L0
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
.L0:
	ldr	r3, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	ldrh	r3, [r1, #54]
	ldr	r2, [pc, #44]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
.L1:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	bne.n	.L2
	movs	r1, #240
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r1, r1, #15
	str	r3, [sp, #0]
	adds	r0, r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080c0a24
.L2:
	add	sp, #4
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x03001ae8
	.4byte 0xfffffe00
