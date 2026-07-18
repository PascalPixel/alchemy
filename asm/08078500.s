@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078500
	.thumb_func
Func_08078500:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #72]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #20
	bl	Func_080784d8
	cmp	r0, #15
	beq.n	.L0
.L3:
	movs	r0, #1
	b.n	.L1
.L0:
	mov	r5, sp
	adds	r0, r5, #0
	bl	Func_080796c4
	adds	r7, r5, #0
	adds	r6, r0, #0
	movs	r5, #0
	cmp	r5, r6
	bge.n	.L2
.L4:
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	adds	r7, #2
	bl	Func_080784d8
	cmp	r0, #15
	bne.n	.L3
	adds	r5, #1
	cmp	r5, r6
	blt.n	.L4
.L2:
	movs	r0, #0
.L1:
	add	sp, #20
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
