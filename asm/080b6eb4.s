@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6eb4
	.thumb_func
Func_080b6eb4:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	Func_08077008
	movs	r1, #1
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_08077228
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L0
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r3, [r6, r3]
	ldr	r0, [pc, #84]
	ands	r0, r3
	bl	Func_080b6e7c
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r3, r6, r2
	adds	r1, r0, #0
	ldrb	r0, [r3, #0]
	cmp	r0, #5
	bhi.n	.L0
	ldr	r2, [pc, #64]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080b6f08
	.4byte 0x080b6f0c
	.4byte 0x080b6f10
	.4byte 0x080b6f14
	.4byte 0x080b6f1e
	.4byte 0x080b6f18
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
.L1:
	lsls	r3, r1, #1
	ldrh	r5, [r2, r3]
.L0:
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x080b6ef0
	.4byte 0x080c2a1c
	.4byte 0x080c2a2a
	.4byte 0x080c2a38
	.4byte 0x080c2a46
	.4byte 0x080c2a54
