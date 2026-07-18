@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b81c8
	.thumb_func
Func_080b81c8:
	push	{r5, r6, lr}
	bl	Func_080b7dd0
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	str	r3, [r5, #40]
	ldr	r3, [pc, #56]
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r5, #68]
	adds	r0, r5, #0
	strb	r3, [r2, #0]
	bl	Func_08009140
	ldr	r3, [pc, #40]
	ldr	r0, [r6, #12]
	ldr	r1, [pc, #40]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	movs	r2, #0
	bl	Func_08009150
	adds	r0, r5, #0
	movs	r1, #5
	bl	Func_08009080
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00009999
	.4byte 0x03000118
	.4byte 0x00014ccc
