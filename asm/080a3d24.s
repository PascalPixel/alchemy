@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3d24
	.thumb_func
Func_080a3d24:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #60]
	movs	r1, #13
	ldr	r7, [r3, #0]
	sub	sp, #4
	movs	r6, #72
	adds	r5, r0, #0
	mov	r8, r1
	movs	r2, #31
.L1:
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r3, #0
	bne.n	.L0
	ldr	r0, [r6, r7]
	str	r2, [sp, #0]
	bl	Func_080a17c4
	ldr	r3, [r6, r7]
	mov	r1, r8
	strb	r1, [r3, #5]
	ldr	r2, [sp, #0]
.L0:
	subs	r2, #1
	adds	r6, #4
	cmp	r2, #0
	bge.n	.L1
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
