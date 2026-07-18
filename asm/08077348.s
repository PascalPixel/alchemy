@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08077348
	.thumb_func
Func_08077348:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	bl	Func_080795fc
	adds	r7, r0, #0
	movs	r6, #0
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L0
	cmp	r6, r7
	bge.n	.L1
	ldr	r3, [pc, #48]
	movs	r1, #252
	lsls	r1, r1, #1
	adds	r2, r3, r1
	adds	r5, r7, #0
.L2:
	ldrb	r0, [r2, #0]
	adds	r2, #1
	str	r2, [sp, #0]
	bl	Func_08077394
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, r6, r3
	ldr	r2, [sp, #0]
	cmp	r5, #0
	bne.n	.L2
.L1:
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Func_080022ec
	adds	r6, r0, #0
.L0:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
