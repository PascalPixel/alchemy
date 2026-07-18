@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809a6b8
	.thumb_func
Func_0809a6b8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #68
	mov	sl, r0
	add	r0, sp, #28
	movs	r3, #0
	str	r3, [r0, #4]
	ldr	r3, [pc, #96]
	str	r3, [r0, #36]
	ldr	r3, [pc, #96]
	str	r3, [r0, #8]
	str	r3, [r0, #12]
	mov	r8, r0
	movs	r7, #0
	add	r6, sp, #16
.L0:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	Func_0800231c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	adds	r0, r5, #0
	movs	r3, #0
	str	r3, [r6, #4]
	bl	Func_08002322
	str	r0, [r6, #8]
	mov	r2, sl
	ldr	r5, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r3, [r6, #0]
	ldr	r2, [r2, #16]
	ldr	r4, [r6, #4]
	str	r0, [sp, #4]
	ldr	r0, [pc, #40]
	str	r0, [sp, #8]
	mov	r0, r8
	str	r0, [sp, #12]
	adds	r7, #1
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_0809a484
	cmp	r7, #16
	bls.n	.L0
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0809a65d
	.4byte 0x0000cccc
	.4byte 0x01090001
