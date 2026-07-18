@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8088
	.thumb_func
Func_080a8088:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #132]
	ldr	r6, [r3, #0]
	ldr	r5, [r6, #36]
	adds	r7, r0, #0
	sub	sp, #8
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L0
	movs	r3, #15
	adds	r5, r6, #0
	adds	r5, #36
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #5
	movs	r3, #30
	bl	Func_080a10d0
	ldr	r5, [r5, #0]
.L0:
	cmp	r0, #0
	beq.n	.L1
	movs	r3, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #0
	adds	r3, r5, #0
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_080150d8
	movs	r2, #190
	lsls	r2, r2, #1
	adds	r3, r6, r2
	str	r0, [r3, #0]
	adds	r2, #164
	movs	r3, #240
	strb	r3, [r0, #15]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L2
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080a33d4
.L2:
	adds	r0, r5, #0
	bl	Func_080a9cf8
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_080a8604
	b.n	.L3
.L1:
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl	Func_080a8604
.L3:
	add	sp, #8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
