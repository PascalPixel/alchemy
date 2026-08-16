@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809bb34
	.thumb_func
Func_0809bb34:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	sub	sp, #4
	cmp	r0, #0
	beq.n	.L0
	bl	Func_08009038
.L0:
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x85000012
