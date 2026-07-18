@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080164d4
	.thumb_func
Func_080164d4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r7, r2, #0
	ldr	r2, [pc, #120]
	adds	r5, r1, #0
	ldr	r1, [sp, #24]
	ldr	r2, [r2, #0]
	lsrs	r4, r5, #3
	adds	r3, #7
	ldrh	r5, [r0, #12]
	adds	r1, #7
	ldrh	r0, [r0, #14]
	mov	r8, r2
	lsrs	r3, r3, #3
	lsrs	r2, r7, #3
	lsrs	r1, r1, #3
	adds	r2, r2, r0
	adds	r4, r4, r5
	adds	r3, r3, r5
	adds	r1, r1, r0
	adds	r5, r4, #1
	adds	r7, r2, #1
	subs	r6, r3, r4
	subs	r4, r1, r2
	adds	r3, r4, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_0801e260
	lsls	r3, r7, #5
	adds	r3, r3, r5
	ldr	r4, [sp, #0]
	lsls	r3, r3, #1
	mov	r2, r8
	movs	r1, #0
	adds	r0, r3, r2
	cmp	r1, r4
	bcs.n	.L0
	movs	r3, #32
	subs	r3, r3, r6
	lsls	r3, r3, #1
.L3:
	movs	r5, #0
	cmp	r5, r6
	bcs.n	.L1
	ldr	r2, [pc, #28]
.L2:
	adds	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, r6
	bcc.n	.L2
.L1:
	adds	r1, #1
	adds	r0, r0, r3
	cmp	r1, r4
	bcc.n	.L3
.L0:
	ldr	r2, [pc, #16]
	movs	r3, #1
	add	r2, r8
	strb	r3, [r2, #0]
	add	sp, #4
	b.n	.L4
	.4byte 0x0000f020
	.4byte 0x03001e8c
	.4byte 0x00000ea3
.L4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
