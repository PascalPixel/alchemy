@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005268
	.thumb_func
Func_08005268:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	ldr	r3, [pc, #108]
	bl	Func_080072f0
	ldr	r3, [r5, #8]
	ldr	r7, [pc, #104]
	negs	r1, r3
	ldr	r3, [r7, #4]
	movs	r0, #0
	cmp	r1, r3
	blt.n	.L0
	ldr	r3, [r7, #8]
	cmp	r1, r3
	bgt.n	.L0
	asrs	r3, r1, #16
	str	r3, [r5, #8]
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L1
	lsrs	r1, r1, #11
	lsls	r0, r0, #5
	ldr	r3, [pc, #76]
	bl	Func_080072f0
	adds	r4, r0, #0
	b.n	.L2
.L1:
	ldr	r4, [pc, #72]
.L2:
	ldr	r6, [pc, #72]
	ldr	r0, [r5, #0]
	adds	r1, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r6
	cmp	r0, #0
	bge.n	.L3
	ldr	r3, [pc, #60]
	adds	r0, r0, r3
.L3:
	ldr	r3, [r7, #12]
	asrs	r2, r0, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r0, [r5, #4]
	adds	r1, r4, #0
	mov	ip, pc
	bx	r6
	cmp	r0, #0
	bge.n	.L4
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L4:
	ldr	r3, [r7, #16]
	asrs	r2, r0, #16
	subs	r3, r3, r2
	str	r3, [r5, #4]
	adds	r0, r4, #0
.L0:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000250
	.4byte 0x03001ce0
	.4byte 0x030003f0
	.4byte 0x000151eb
	.4byte 0x03000118
	.4byte 0x0000ffff
