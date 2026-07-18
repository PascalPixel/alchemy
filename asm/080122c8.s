@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080122c8
	.thumb_func
Func_080122c8:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl	Func_08012204
	ldr	r3, [r5, #0]
	adds	r7, r0, #0
	movs	r4, #0
	cmp	r3, #0
	bge.n	.L0
	ldr	r2, [pc, #68]
	adds	r3, r3, r2
.L0:
	ldr	r0, [r5, #8]
	asrs	r2, r3, #21
	movs	r1, #31
	ands	r2, r1
	cmp	r0, #0
	bge.n	.L1
	ldr	r3, [pc, #52]
	adds	r0, r0, r3
.L1:
	asrs	r3, r0, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r2, [pc, #44]
	lsls	r3, r3, #2
	adds	r0, r3, r2
	ldrb	r2, [r0, #3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r4, #16
.L2:
	ldr	r3, [r0, #0]
	lsls	r3, r3, #1
	lsrs	r3, r3, #25
	str	r3, [r6, #0]
	cmp	r3, #21
	bne.n	.L3
	movs	r4, #32
.L3:
	ldr	r3, [pc, #16]
	adds	r2, r4, r7
	ldrb	r0, [r3, r2]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x0801353c
