@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080187fc
	.thumb_func
Func_080187fc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r3
	ldr	r3, [pc, #68]
	adds	r6, r1, #0
	movs	r1, #0
	ldr	r5, [r3, #0]
	adds	r7, r2, #0
	sub	sp, #12
	bl	Func_08018038
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [sp, #32]
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #1
	str	r3, [sp, #8]
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	Func_0801868c
	movs	r0, #1
.L1:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e8c
