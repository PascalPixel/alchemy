@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099920
	.thumb_func
Func_08099920:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bgt.n	.L0
	adds	r2, r6, #0
	adds	r2, #94
	movs	r3, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #104]
	bl	Func_08009098
	movs	r5, #0
	movs	r7, #0
	str	r5, [r6, #108]
	b.n	.L1
.L2:
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r2, r5, #0
	adds	r2, #85
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #40]
	bl	Func_08004458
	ldr	r3, [pc, #68]
	adds	r0, r0, r3
	str	r0, [r5, #48]
	bl	Func_08004458
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #14
	adds	r0, r5, #0
	bl	Func_08096bec
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #6
	strh	r3, [r2, #0]
	adds	r0, r5, #0
	ldr	r1, [pc, #32]
	bl	Func_08009098
	adds	r7, #1
.L1:
	cmp	r7, #2
	bgt.n	.L0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #240
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L2
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0809f0b0
	.4byte 0x00013333
