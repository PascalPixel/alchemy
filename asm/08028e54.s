@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028e54
	.thumb_func
Func_08028e54:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #72]
	adds	r7, r2, #0
	adds	r6, r1, #0
	mov	r8, r3
	adds	r5, r0, #0
	bl	Func_080284dc
	movs	r0, #5
	bl	Func_080287a8
	movs	r0, #6
	bl	Func_080287a8
	mov	r3, r8
	adds	r1, r6, #0
	movs	r2, #3
	adds	r0, r5, #0
	bl	Func_080288a8
	adds	r0, r7, #0
	bl	Func_08028574
	adds	r7, r0, #0
	bl	Func_0802851c
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L0
	movs	r7, #1
.L0:
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000024
