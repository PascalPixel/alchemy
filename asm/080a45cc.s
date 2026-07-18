@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a45cc
	.thumb_func
Func_080a45cc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #15
	adds	r6, r1, #0
	bl	Func_080150b8
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	mov	r8, r2
	cmp	r3, r8
	bne.n	.L0
	movs	r0, #14
	bl	Func_080150b8
.L0:
	ldr	r7, [pc, #188]
	movs	r3, #24
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_08015080
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #1
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L1
	movs	r0, #14
	bl	Func_080150b8
.L1:
	movs	r3, #24
	adds	r0, r7, #1
	adds	r1, r6, #0
	movs	r2, #32
	bl	Func_08015080
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L2
	movs	r0, #14
	bl	Func_080150b8
.L2:
	movs	r3, #32
	adds	r0, r7, #2
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_08015080
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #5
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L3
	movs	r0, #14
	bl	Func_080150b8
.L3:
	movs	r3, #32
	adds	r0, r7, #3
	adds	r1, r6, #0
	movs	r2, #80
	bl	Func_08015080
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #2
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L4
	movs	r0, #14
	bl	Func_080150b8
.L4:
	movs	r3, #24
	adds	r0, r7, #4
	adds	r1, r6, #0
	movs	r2, #80
	bl	Func_08015080
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #4
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L5
	movs	r0, #14
	bl	Func_080150b8
.L5:
	adds	r0, r7, #5
	adds	r1, r6, #0
	movs	r2, #32
	movs	r3, #32
	bl	Func_08015080
	movs	r0, #15
	bl	Func_080150b8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000b33
