@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801f77c
	.thumb_func
Func_0801f77c:
	push	{r5, r6, r7, lr}
	bl	Func_080056cc
	movs	r6, #9
	movs	r5, #0
	negs	r6, r6
	cmp	r0, #0
	bne.n	.L0
	bl	Func_08005c68
	ldr	r3, [pc, #64]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #68]
	strh	r5, [r3, #0]
	adds	r7, r3, #0
	ldr	r3, [pc, #64]
	strh	r5, [r2, #0]
	ldr	r4, [pc, #44]
	adds	r6, r0, #0
	adds	r1, r1, r3
	movs	r0, #2
.L3:
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L1
	strh	r4, [r7, #0]
	adds	r5, #1
.L1:
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L2
	strh	r4, [r2, #0]
.L2:
	subs	r0, #1
	adds	r1, #64
	cmp	r0, #0
	bge.n	.L3
	ldr	r3, [pc, #28]
	movs	r2, #144
	ldr	r3, [r3, #0]
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001f1c
	.4byte 0x02002010
	.4byte 0x0200200c
	.4byte 0x00001070
	.4byte 0x03001ae8
.L4:
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, r2
	beq.n	.L0
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #20]
	strh	r3, [r2, #0]
.L0:
	bl	Func_08005cf8
	cmp	r6, #0
	beq.n	.L5
	cmp	r5, r6
	bne.n	.L5
	adds	r0, r6, #0
	adds	r0, #100
	b.n	.L6
	.4byte 0x00000000
	.4byte 0x02002010
.L5:
	adds	r0, r6, #0
.L6:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
