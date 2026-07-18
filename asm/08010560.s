@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08010560
	.thumb_func
Func_08010560:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r0, [r7, #0]
	ldr	r3, [pc, #96]
	mov	ip, r0
	sub	sp, #8
	mov	sl, r1
	mov	r8, r2
	cmp	ip, r3
	beq.n	.L0
	mov	r9, r3
	adds	r6, r7, #2
.L1:
	movs	r2, #0
	ldrsh	r1, [r6, r2]
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	movs	r4, #4
	ldrsh	r2, [r6, r4]
	movs	r4, #6
	ldrsh	r5, [r6, r4]
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	lsls	r1, r1, #16
	lsls	r5, r5, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	lsrs	r1, r1, #16
	mov	r2, sl
	mov	r3, r8
	lsrs	r5, r5, #16
	bl	Func_08010424
	adds	r7, #10
	adds	r0, r5, #0
	bl	Func_080030f8
	ldrh	r0, [r7, #0]
	mov	ip, r0
	adds	r6, #10
	cmp	ip, r9
	bne.n	.L1
.L0:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ffff
