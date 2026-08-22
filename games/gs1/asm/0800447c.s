@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800447c
	.thumb_func
Func_0800447c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	sl, r0
	movs	r0, #128
	mov	r8, r1
	lsls	r0, r0, #7
	add	r0, r8
	adds	r5, r2, #0
	bl	Func_08002322
	ldr	r6, [pc, #52]
	adds	r1, r0, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r6
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
	stmia	r5!, {r3}
	mov	r0, r8
	bl	Func_08002322
	adds	r5, #4
	adds	r1, r0, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r6
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
