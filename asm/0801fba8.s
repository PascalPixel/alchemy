@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801fba8
	.thumb_func
Func_0801fba8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #0
	mov	r8, r3
	bl	Func_080056cc
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r0, [pc, #172]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #9
	b.n	.L1
.L0:
	bl	Func_08005c68
	movs	r0, #0
	movs	r1, #2
	bl	Func_08020244
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L2
	mov	r8, r5
	b.n	.L3
.L2:
	ldr	r7, [pc, #136]
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_08005a78
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, r3, r7
	mov	sl, r3
	adds	r6, r0, #0
	mov	r1, sl
	adds	r0, r5, #3
	bl	Func_08005a78
	orrs	r6, r0
	cmp	r6, #0
	beq.n	.L4
	ldr	r0, [pc, #108]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #2
	b.n	.L1
.L4:
	bl	Func_0801f704
	ldr	r3, [pc, #96]
	adds	r5, r0, #0
	cmp	r5, r3
	bne.n	.L5
	ldr	r0, [pc, #92]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #5
	b.n	.L1
.L5:
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_08005920
	mov	r1, sl
	adds	r6, r0, #0
	adds	r0, r5, #3
	bl	Func_08005920
	orrs	r6, r0
	cmp	r6, #0
	beq.n	.L6
	ldr	r0, [pc, #56]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #3
.L1:
	negs	r3, r3
	mov	r8, r3
	b.n	.L3
.L6:
	ldr	r0, [pc, #44]
	movs	r1, #1
	bl	Func_0801776c
.L3:
	bl	Func_08005cf8
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000000a
	.4byte 0x02000000
	.4byte 0x0000000c
	.4byte 0x000003e7
	.4byte 0x0000000d
	.4byte 0x00000019
