@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080df8b8
	.thumb_func
Func_080df8b8:
	push	{r5, r6, lr}
	bl	Func_080b5098
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r3, [pc, #48]
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r5, #68]
	strb	r3, [r2, #0]
	subs	r2, #2
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	Func_08009140
	ldr	r1, [r6, #12]
	adds	r0, r5, #0
	ldr	r3, [r6, #16]
	movs	r2, #0
	bl	Func_08009150
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08009080
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0000ab85
