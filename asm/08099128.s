@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099128
	.thumb_func
Func_08099128:
	push	{lr}
	ldr	r3, [pc, #48]
	ldr	r2, [r3, #0]
	ldr	r1, [r2, #20]
	cmp	r1, #0
	beq.n	.L0
	adds	r3, r2, #0
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L1
	adds	r2, #32
	movs	r3, #1
	strb	r3, [r2, #0]
.L1:
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	bl	Func_08099160
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
