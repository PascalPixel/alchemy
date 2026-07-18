@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801fc84
	.thumb_func
Func_0801fc84:
	push	{r5, r6, r7, lr}
	bl	Func_080056cc
	adds	r5, r0, #0
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r0, [pc, #144]
	movs	r1, #1
	bl	Func_0801776c
	subs	r7, #9
	b.n	.L1
.L0:
	bl	Func_08005c68
	movs	r0, #0
	movs	r1, #3
	bl	Func_08020244
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L2
	adds	r7, r6, #0
	b.n	.L1
.L2:
	movs	r1, #8
	movs	r2, #1
	movs	r3, #2
	ldr	r0, [pc, #104]
	bl	Func_08017658
	b.n	.L3
.L4:
	movs	r0, #1
	bl	Func_080030f8
.L3:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L4
	movs	r0, #1
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	bl	Func_08028df4
	cmp	r0, #0
	beq.n	.L5
	bl	Func_08019a54
	b.n	.L1
.L5:
	bl	Func_08019a54
	adds	r0, r6, #0
	bl	Func_08005ac0
	adds	r5, r0, #0
	adds	r0, r6, #3
	bl	Func_08005ac0
	orrs	r5, r0
	cmp	r5, #0
	beq.n	.L6
	ldr	r0, [pc, #40]
	movs	r1, #1
	movs	r7, #4
	bl	Func_0801776c
	negs	r7, r7
	b.n	.L1
.L6:
	ldr	r0, [pc, #28]
	movs	r1, #1
	bl	Func_0801776c
.L1:
	bl	Func_08005cf8
	adds	r0, r7, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00000018
