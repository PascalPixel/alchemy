@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080992f0
	.thumb_func
Func_080992f0:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #100
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	lsls	r0, r0, #9
	bl	Func_08002322
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r3, [pc, #52]
	lsls	r0, r0, #11
	movs	r0, r0
	mov	ip, pc
	bx	r3
	ldr	r3, [r5, #56]
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	adds	r2, r1, #0
	adds	r2, #128
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L0
	adds	r3, r1, #0
	adds	r3, #255
.L0:
	asrs	r3, r3, #7
	lsls	r3, r3, #7
	subs	r3, r2, r3
	strh	r3, [r6, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03000118
