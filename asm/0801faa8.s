@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801faa8
	.thumb_func
Func_0801faa8:
	push	{r5, r6, r7, lr}
	movs	r0, #128
	lsls	r0, r0, #5
	bl	Func_08004970
	ldr	r6, [pc, #124]
	adds	r5, r0, #0
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	movs	r3, #1
	negs	r3, r3
	movs	r7, #0
	cmp	r0, r3
	beq.n	.L0
	bl	Func_080056cc
	cmp	r0, #0
	beq.n	.L1
	ldr	r0, [pc, #100]
	movs	r1, #1
	movs	r7, #9
	bl	Func_0801776c
	b.n	.L2
.L1:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	adds	r1, r5, #0
	bl	Func_08005a78
	cmp	r0, #0
	beq.n	.L3
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	Func_0801776c
	movs	r7, #2
	negs	r7, r7
.L3:
	ldr	r1, [pc, #72]
	ldr	r3, [pc, #72]
	adds	r0, r5, r1
	subs	r0, r0, r3
	movs	r2, #16
	ldr	r3, [pc, #68]
	bl	Func_080072f0
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	adds	r1, r5, #0
	bl	Func_08005920
	cmp	r0, #0
	beq.n	.L4
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	Func_0801776c
	movs	r7, #3
.L2:
	negs	r7, r7
.L4:
	bl	Func_08005cf8
	adds	r0, r5, #0
	bl	Func_08002df0
	adds	r0, r7, #0
.L0:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02002004
	.4byte 0x0000000a
	.4byte 0x0000000b
	.4byte 0x020004e4
	.4byte 0x02000000
	.4byte 0x03001388
