@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa2a0
	.thumb_func
Func_080fa2a0:
	push	{r4, r5, r6, lr}
	ldr	r0, [pc, #80]
	movs	r1, #2
	negs	r1, r1
	ands	r0, r1
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #76]
	bl	Func_08006864
	ldr	r0, [pc, #76]
	bl	Func_080fa6a0
	ldr	r0, [pc, #72]
	bl	Func_080fa55c
	ldr	r0, [pc, #72]
	bl	Func_080fa83c
	ldr	r0, [pc, #68]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L0
	ldr	r5, [pc, #64]
	adds	r6, r0, #0
.L1:
	ldr	r4, [r5, #0]
	ldr	r1, [r5, #4]
	ldrb	r2, [r5, #8]
	adds	r0, r4, #0
	bl	Func_080fa9e0
	ldrh	r0, [r5, #10]
	strb	r0, [r4, #11]
	ldr	r0, [pc, #48]
	str	r0, [r4, #24]
	adds	r5, #12
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L1
.L0:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x080f9675
	.4byte 0x03007000
	.4byte 0x04000100
	.4byte 0x02003050
	.4byte 0x02004090
	.4byte 0x0097f800
	.4byte 0x00000008
	.4byte 0x080fc624
	.4byte 0x02004350
