@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b280c
	.thumb_func
Func_080b280c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #96]
	ldr	r1, [pc, #100]
	ldr	r5, [r3, #0]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	subs	r1, #3
	mov	sl, r3
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	sub	sp, #4
	movs	r7, #0
	cmp	r2, r3
	bge.n	.L0
	adds	r3, r5, #2
	movs	r6, #219
	mov	r8, r3
	lsls	r6, r6, #2
.L2:
	mov	r1, r8
	ldrsh	r0, [r1, r6]
	mov	r1, sl
	str	r2, [sp, #0]
	bl	Func_080b27b0
	ldr	r2, [sp, #0]
	cmp	r0, #0
	beq.n	.L1
	adds	r2, #1
.L1:
	ldr	r1, [pc, #40]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	blt.n	.L2
.L0:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003aa
	.4byte 0x000003a7
