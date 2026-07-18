@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9cbc
	.thumb_func
Func_080a9cbc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	movs	r2, #248
	adds	r5, r3, #0
	mov	r8, r2
	adds	r5, #72
	movs	r7, #168
	movs	r6, #31
.L1:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L0
	mov	r3, r8
	strh	r3, [r0, #6]
	strh	r7, [r0, #8]
	bl	Func_080a17c4
.L0:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
