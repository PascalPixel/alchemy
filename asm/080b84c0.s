@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b84c0
	.thumb_func
Func_080b84c0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	mov	r8, r0
	bl	Func_080b7dd0
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_080b7f70
	adds	r5, #8
	adds	r6, r0, #0
	bl	Func_080b7ed8
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_08005268
	ldr	r5, [pc, #64]
	ldr	r1, [r6, #24]
	mov	ip, pc
	bx	r5
	adds	r6, r0, #0
	mov	r0, r8
	bl	Func_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	Func_080c23c0
	cmp	r0, #0
	beq.n	.L0
	adds	r0, r6, #0
	movs	r1, #24
	b.n	.L1
.L0:
	adds	r0, r6, #0
	movs	r1, #48
.L1:
	movs	r0, r0
	mov	ip, pc
	bx	r5
	ldr	r3, [r7, #4]
	subs	r3, r3, r0
	str	r3, [r7, #4]
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03000118
