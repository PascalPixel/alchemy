@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8b48
	.thumb_func
Func_080b8b48:
	push	{r5, r6, lr}
	ldr	r3, [pc, #200]
	ldr	r2, [r3, #0]
	movs	r1, #128
	ldr	r3, [r2, #0]
	lsls	r1, r1, #6
	sub	sp, #84
	adds	r6, r0, #0
	cmp	r3, r1
	bne.n	.L0
	str	r1, [r2, #0]
	movs	r0, #10
	bl	Func_080030f8
	b.n	.L1
.L0:
	str	r1, [r2, #0]
	movs	r0, #30
	bl	Func_080030f8
.L1:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	mov	r5, sp
	str	r0, [r5, #8]
	bl	Func_080b8808
	cmp	r0, #0
	blt.n	.L2
	ldrh	r0, [r6, #10]
	strh	r0, [r5, #36]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl	Func_080b8808
	cmp	r0, #0
	bge.n	.L3
.L2:
	movs	r0, #1
	negs	r0, r0
	b.n	.L4
.L3:
	ldr	r0, [r5, #8]
	bl	Func_08077008
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	Func_08077008
	bl	Func_08004458
	movs	r1, #1
	ldr	r0, [r5, #8]
	bl	Func_08015120
	ldr	r0, [pc, #104]
	bl	Func_080151c8
	movs	r2, #13
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	movs	r3, #0
	bl	Func_080b82c4
	ldr	r0, [r5, #8]
	bl	Func_080b7dd0
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	Func_08009088
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	Func_080b7dd0
	ldrh	r3, [r5, #36]
	movs	r2, #1
	str	r2, [r5, #20]
	cmp	r3, #7
	bhi.n	.L5
	str	r2, [r5, #4]
	b.n	.L6
.L5:
	movs	r3, #0
	str	r3, [r5, #4]
.L6:
	movs	r3, #0
	movs	r0, #4
	str	r3, [r5, #28]
	bl	Func_080030f8
	adds	r0, r5, #0
	bl	Func_080c9008
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	Func_080b8000
	ldr	r0, [r5, #8]
	bl	Func_080b8000
	movs	r0, #0
.L4:
	add	sp, #84
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0x00000814
