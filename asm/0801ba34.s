@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ba34
	.thumb_func
Func_0801ba34:
	push	{lr}
	movs	r3, #210
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	sub	sp, #12
	movs	r2, #0
	cmp	r0, #0
	beq.n	.L0
.L1:
	ldr	r0, [r0, #4]
	adds	r2, #1
	cmp	r0, #0
	bne.n	.L1
.L0:
	ldr	r3, [pc, #20]
	mov	r0, sp
	lsls	r2, r2, #1
	strh	r3, [r0, r2]
	movs	r1, #0
	bl	Func_080b50e0
	add	sp, #12
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000000ff
