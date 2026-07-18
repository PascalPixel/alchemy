@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8000
	.thumb_func
Func_080b8000:
	push	{r5, r6, lr}
	bl	Func_080b7dd0
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #48]
	ldr	r3, [pc, #72]
	str	r3, [r5, #72]
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #90
	str	r2, [r5, #40]
	str	r2, [r5, #68]
	strb	r2, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #88
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	Func_08009140
	adds	r0, r5, #0
	ldr	r1, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r2, #0
	bl	Func_08009150
	ldr	r0, [r6, #16]
	cmp	r0, #0
	bge.n	.L0
	adds	r0, #7
.L0:
	ldr	r1, [r6, #12]
	asrs	r0, r0, #3
	bl	Func_080044d0
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r0, r0, r3
	strh	r0, [r5, #6]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ab85
