@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08094428
	.thumb_func
Func_08094428:
	push	{r5, r6, lr}
	movs	r5, #144
	lsls	r5, r5, #1
	adds	r0, r5, #0
	movs	r6, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #24
	bl	Func_08094380
	adds	r0, r5, #0
	bl	Func_080770d0
	movs	r6, #1
	b.n	.L1
.L0:
	ldr	r5, [pc, #152]
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
	movs	r0, #23
	bl	Func_08094380
	adds	r0, r5, #0
	bl	Func_080770d0
	movs	r6, #2
	b.n	.L1
.L2:
	movs	r5, #145
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	adds	r0, r5, #0
	bl	Func_080770d0
	ldr	r3, [pc, #108]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r6, [r3, #0]
	adds	r0, r6, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	movs	r2, #160
	lsls	r2, r2, #16
	adds	r3, r3, r2
	movs	r2, #1
	negs	r2, r2
	str	r3, [r5, #12]
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	Func_080933f8
	b.n	.L3
.L4:
	movs	r0, #1
	bl	Func_080030f8
.L3:
	ldr	r2, [r5, #40]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	ldr	r2, [r5, #20]
	cmp	r3, r2
	bgt.n	.L4
	movs	r0, #159
	bl	Func_080f9010
	ldr	r3, [r5, #20]
	movs	r1, #22
	str	r3, [r5, #12]
	adds	r0, r5, #0
	bl	Func_08009080
	movs	r0, #15
	bl	Func_0809163c
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_0809335c
	movs	r6, #3
.L1:
	adds	r0, r6, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000121
	.4byte 0x02000240
