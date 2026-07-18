@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6ae0
	.thumb_func
Func_080b6ae0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r2, #0
	mov	r8, r2
	movs	r7, #6
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L0
	movs	r0, #182
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	movs	r7, #3
.L1:
	movs	r5, #128
	adds	r7, #128
	cmp	r5, r7
	bge.n	.L2
.L4:
	adds	r0, r5, #0
	bl	Func_08077008
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #1
	strh	r5, [r6, #0]
	add	r8, r3
	adds	r6, #2
.L3:
	adds	r5, #1
	cmp	r5, r7
	blt.n	.L4
.L2:
	ldr	r3, [pc, #16]
	strh	r3, [r6, #0]
	mov	r0, r8
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000000ff
