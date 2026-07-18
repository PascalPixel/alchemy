@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801eadc
	.thumb_func
Func_0801eadc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	sl, r1
	mov	r8, r2
	adds	r6, r3, #0
	bl	Func_08015e8c
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L0
	adds	r0, r7, #0
	bl	Func_08003f3c
	movs	r0, #0
	b.n	.L1
.L0:
	mov	r0, r8
	ldrh	r1, [r0, #12]
	ldrh	r3, [r0, #14]
	ldr	r2, [sp, #24]
	lsls	r3, r3, #3
	lsls	r1, r1, #3
	adds	r2, r2, r3
	adds	r1, r6, r1
	ldr	r3, [pc, #72]
	adds	r1, #8
	ands	r1, r3
	adds	r2, #8
	movs	r3, #255
	ands	r2, r3
	lsls	r3, r1, #16
	orrs	r3, r2
	mov	r0, sl
	orrs	r3, r0
	ldr	r0, [pc, #56]
	str	r3, [r5, #20]
	lsls	r3, r7, #2
	adds	r3, r3, r0
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [r5, #24]
	movs	r3, #255
	strb	r3, [r5, #15]
	movs	r3, #0
	str	r3, [r5, #0]
	movs	r3, #1
	strh	r1, [r5, #6]
	strh	r2, [r5, #8]
	strb	r7, [r5, #14]
	strb	r3, [r5, #4]
	strb	r3, [r5, #5]
	mov	r0, r8
	adds	r1, r5, #0
	bl	Func_08016584
	adds	r0, r5, #0
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0x03001b10
