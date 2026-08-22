@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004c1c
	.thumb_func
Func_08004c1c:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #48
	adds	r5, r0, #0
	bl	Func_08002322
	mov	r8, r0
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r6, r0, #0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r2, r8
	negs	r3, r2
	str	r3, [r5, #8]
	str	r6, [r5, #0]
	str	r2, [r5, #24]
	str	r6, [r5, #32]
	ldr	r3, [pc, #16]
	adds	r0, r5, #0
	bl	Func_080072f0
	add	sp, #48
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x030002c0
