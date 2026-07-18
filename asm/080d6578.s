@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d6578
	.thumb_func
Func_080d6578:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [pc, #212]
	movs	r0, #41
	bl	Func_080048b0
	ldr	r1, [pc, #208]
	movs	r0, #39
	bl	Func_080048b0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L0
	subs	r3, #1
	cmp	r3, #11
	bhi.n	.L1
	ldr	r2, [pc, #184]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080d65dc
	.4byte 0x080d65e4
	.4byte 0x080d65ec
	.4byte 0x080d65f4
	.4byte 0x080d65fc
	.4byte 0x080d6604
	.4byte 0x080d660c
	.4byte 0x080d6614
	.4byte 0x080d661c
	.4byte 0x080d6624
	.4byte 0x080d662c
	.4byte 0x080d6634
	adds	r0, r5, #0
	bl	Func_080e823c
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080d2d98
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080eb754
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080dc968
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080d6970
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080ec100
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080d2458
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080d1714
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080ea0d8
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080d765c
	b.n	.L1
.L0:
	adds	r0, r5, #0
	bl	Func_080e7320
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080e15e8
.L1:
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
	movs	r0, #41
	bl	Func_08002dd8
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000302
	.4byte 0x0000782c
	.4byte 0x080d65ac
