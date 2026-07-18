@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080292c4
	.thumb_func
Func_080292c4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r0
	sub	sp, #36
	adds	r5, r1, #0
	bl	Func_08016478
	mov	r1, sl
	movs	r2, #48
	movs	r3, #0
	ldr	r0, [pc, #160]
	bl	Func_0801e940
	add	r2, sp, #8
	mov	r8, r2
	mov	r2, sp
	movs	r1, #0
	movs	r3, #28
	adds	r2, #33
	str	r1, [sp, #4]
	add	r3, sp
	movs	r1, #16
	str	r2, [sp, #0]
	lsls	r7, r5, #8
	mov	fp, r3
	mov	r9, r1
.L2:
	mov	r3, fp
.L0:
	movs	r1, #0
	strb	r1, [r3, #0]
	ldr	r2, [sp, #0]
	adds	r3, #1
	cmp	r3, r2
	bne.n	.L0
	adds	r0, r7, #0
	movs	r1, #3
	mov	r2, fp
	bl	Func_08029274
	mov	r0, fp
	mov	r1, sl
	movs	r2, #0
	mov	r3, r9
	bl	Func_0801e940
	ldr	r0, [pc, #96]
	mov	r1, sl
	movs	r2, #32
	mov	r3, r9
	bl	Func_0801e940
	mov	r6, r8
	mov	r5, r8
	adds	r6, #15
.L1:
	adds	r0, r7, #0
	bl	Func_080770c0
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	adds	r3, #48
	strb	r3, [r5, #0]
	adds	r5, #1
	adds	r7, #1
	cmp	r5, r6
	ble.n	.L1
	movs	r3, #16
	movs	r2, #0
	mov	r1, r8
	strb	r2, [r1, r3]
	mov	r0, r8
	mov	r3, r9
	mov	r1, sl
	movs	r2, #48
	bl	Func_0801e940
	ldr	r1, [sp, #4]
	movs	r3, #8
	adds	r1, #1
	add	r9, r3
	str	r1, [sp, #4]
	cmp	r1, #16
	bne.n	.L2
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0803742c
	.4byte 0x08037428
