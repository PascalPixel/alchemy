@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800dac0
	.thumb_func
Func_0800dac0:
	push	{r5, lr}
	sub	sp, #12
	adds	r5, r0, #0
	mov	r0, sp
	ldr	r3, [r0, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	adds	r1, r1, r3
	ldr	r3, [r0, #4]
	ldr	r0, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	adds	r0, r5, #0
	bl	Func_0800d14c
	ldrh	r3, [r5, #4]
	adds	r3, #3
	movs	r0, #1
	strh	r3, [r5, #4]
	add	sp, #12
	pop	{r5}
	pop	{r1}
	bx	r1
