@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800d710
	.thumb_func
Func_0800d710:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r4}
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #52]
	cmp	r4, r3
	beq.n	.L0
	adds	r0, r5, #0
	adds	r0, #93
	ldrb	r2, [r0, #0]
	adds	r2, #1
	strb	r2, [r0, #0]
	lsls	r3, r4, #16
	lsls	r2, r2, #24
	lsrs	r2, r2, #24
	asrs	r3, r3, #16
	cmp	r2, r3
	bge.n	.L1
	adds	r0, r5, #0
.L0:
	bl	Func_0800d6d8
	strh	r0, [r5, #4]
	b.n	.L2
.L1:
	movs	r3, #0
	strb	r3, [r0, #0]
	ldrh	r3, [r5, #4]
	adds	r3, #3
	strh	r3, [r5, #4]
.L2:
	movs	r0, #1
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x0000ffff
