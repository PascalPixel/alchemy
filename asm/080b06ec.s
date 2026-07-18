@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b06ec
	.thumb_func
Func_080b06ec:
	push	{lr}
	ldr	r3, [pc, #76]
	lsls	r0, r0, #5
	adds	r0, r0, r3
	ldr	r3, [pc, #72]
	lsls	r2, r2, #1
	ldrh	r3, [r3, r2]
	adds	r1, r1, r3
	adds	r1, #2
	movs	r4, #3
.L1:
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	strb	r2, [r1, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	strb	r2, [r1, #1]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	strb	r2, [r1, #30]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	strb	r2, [r1, #31]
	adds	r0, #1
.L0:
	subs	r4, #1
	adds	r1, #4
	cmp	r4, #0
	bge.n	.L1
	pop	{r0}
	bx	r0
	.4byte 0x080b3d40
	.4byte 0x080b413c
