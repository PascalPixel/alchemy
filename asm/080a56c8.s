@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a56c8
	.thumb_func
Func_080a56c8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #148]
	ldr	r3, [r3, #0]
	adds	r5, r2, #0
	sub	sp, #4
	adds	r6, r0, #0
	mov	sl, r3
	bl	Func_08015270
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r2, #11
	movs	r3, #16
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08015070
	ldr	r2, [r5, #8]
	lsls	r3, r2, #2
	adds	r7, r3, r2
	ldr	r3, [r5, #20]
	subs	r3, r3, r7
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #5
	bls.n	.L0
	movs	r2, #5
	mov	r8, r2
.L0:
	movs	r3, #34
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r0, #5
	adds	r1, r7, #0
	movs	r3, #116
	bl	Func_080a2324
	movs	r2, #15
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #20]
	adds	r0, r6, #0
	str	r2, [sp, #0]
	movs	r2, #5
	bl	Func_080a21b0
	mov	r3, r8
	movs	r6, #0
	cmp	r3, #0
	bls.n	.L1
	lsls	r3, r7, #1
	movs	r2, #228
	add	r3, sl
	lsls	r2, r2, #1
	ldr	r7, [pc, #40]
	adds	r5, r3, r2
.L2:
	ldrh	r3, [r5, #0]
	adds	r0, r7, #0
	ands	r0, r3
	ldr	r3, [pc, #40]
	adds	r0, r0, r3
	mov	r3, sl
	ldr	r1, [r3, #32]
	lsls	r3, r6, #4
	adds	r3, #8
	movs	r2, #24
	bl	Func_08015080
	adds	r3, r6, #1
	lsls	r3, r3, #24
	lsrs	r6, r3, #24
	adds	r5, #2
	cmp	r8, r6
	bhi.n	.L2
	b.n	.L1
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001f2c
	.4byte 0x00000182
.L1:
	movs	r0, #1
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, #1
	bx	lr
