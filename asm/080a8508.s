@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8508
	.thumb_func
Func_080a8508:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	movs	r7, #0
	movs	r6, #0
.L2:
	mov	r2, sl
	ldrb	r3, [r2, r6]
	cmp	r3, #0
	beq.n	.L0
	cmp	r9, r7
	bne.n	.L1
	ldr	r3, [pc, #68]
	lsls	r5, r6, #1
	adds	r5, r5, r3
	movs	r3, #1
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #0
	negs	r3, r3
	adds	r5, #1
	bl	Func_08015078
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #0
	movs	r3, #15
	bl	Func_08015078
.L1:
	adds	r7, #1
.L0:
	adds	r6, #1
	cmp	r6, #4
	ble.n	.L2
	cmp	r7, #0
	bne.n	.L3
	ldr	r0, [pc, #28]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015078
.L3:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000bdc
	.4byte 0x00000bda
