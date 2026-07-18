@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08098a84
	.thumb_func
Func_08098a84:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r1, #0
	mov	r8, r2
	movs	r0, #138
	adds	r7, r3, #0
	bl	Func_080f9010
	adds	r1, r5, #0
	movs	r0, #215
	adds	r2, r6, #0
	mov	r3, r8
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #3
	bl	Func_08009080
	movs	r2, #128
	ldr	r3, [r5, #24]
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L1
	ldr	r6, [pc, #32]
.L2:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldrh	r3, [r5, #6]
	adds	r3, r3, r6
	strh	r3, [r5, #6]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r5, #24]
	ldr	r2, [pc, #8]
	cmp	r3, r2
	ble.n	.L2
	b.n	.L1
	.4byte 0x00002000
	.4byte 0x0000ffff
.L1:
	strh	r7, [r5, #6]
.L0:
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
