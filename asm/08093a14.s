@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08093a14
	.thumb_func
Func_08093a14:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [r5, #104]
	cmp	r1, #0
	beq.n	.L0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [r1, #16]
	ldr	r3, [r5, #16]
	ldr	r1, [r1, #8]
	subs	r0, r0, r3
	ldr	r3, [r5, #8]
	subs	r1, r1, r3
	bl	Func_080044d0
	ldrh	r3, [r5, #6]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #128
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L1
	adds	r0, r2, #0
.L1:
	ldr	r2, [pc, #20]
	cmp	r0, r2
	bge.n	.L2
	adds	r0, r2, #0
.L2:
	adds	r3, r3, r0
	strh	r3, [r5, #6]
.L0:
	movs	r0, #1
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0xfffff000
