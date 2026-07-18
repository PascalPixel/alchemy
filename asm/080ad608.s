@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ad608
	.thumb_func
Func_080ad608:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #64]
	ldr	r7, [r3, #0]
	movs	r3, #137
	lsls	r0, r0, #2
	lsls	r3, r3, #2
	adds	r6, r0, r3
	ldr	r0, [r7, r6]
	adds	r5, r1, #0
	mov	r8, r2
	cmp	r0, #0
	beq.n	.L0
	bl	Func_08009038
	movs	r3, #0
	str	r3, [r7, r6]
.L0:
	ldr	r3, [pc, #36]
	lsls	r2, r5, #2
	ldr	r0, [r3, r2]
	bl	Func_08009030
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L1
	mov	r1, r8
	bl	Func_08009020
.L1:
	str	r5, [r7, r6]
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x080af304
