@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08020088
	.thumb_func
Func_08020088:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #52]
	ldr	r7, [r3, #0]
	movs	r5, #137
	bl	Func_08004278
	movs	r3, #0
	mov	r8, r3
	lsls	r5, r5, #2
	movs	r6, #3
.L1:
	ldr	r0, [r5, r7]
	cmp	r0, #0
	beq.n	.L0
	bl	Func_08009038
	mov	r3, r8
	str	r3, [r5, r7]
.L0:
	subs	r6, #1
	adds	r5, #4
	cmp	r6, #0
	bge.n	.L1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x080200cd
