@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801f9b4
	.thumb_func
Func_0801f9b4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #112]
	movs	r3, #0
	mov	r8, r3
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L0
	bl	Func_080056cc
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L1
	ldr	r0, [pc, #88]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #9
	b.n	.L2
.L1:
	bl	Func_0801f818
	ldr	r5, [pc, #76]
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	adds	r1, r5, #0
	bl	Func_08005920
	adds	r6, r0, #0
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r5, r5, r3
	adds	r0, #3
	adds	r1, r5, #0
	bl	Func_08005920
	orrs	r6, r0
	cmp	r6, #0
	beq.n	.L3
	ldr	r0, [pc, #40]
	movs	r1, #1
	bl	Func_0801776c
	movs	r3, #3
.L2:
	negs	r3, r3
	mov	r8, r3
.L3:
	bl	Func_08005cf8
	mov	r0, r8
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02002004
	.4byte 0x0000000a
	.4byte 0x02000000
	.4byte 0x0000000b
