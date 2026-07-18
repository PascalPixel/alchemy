@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bb7c0
	.thumb_func
Func_080bb7c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #0
	sub	sp, #20
	adds	r6, r1, #0
	bl	Func_080153f0
	mov	fp, r0
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #128
	add	r5, sp, #8
	bl	Func_080040b4
	mov	r2, sp
	ldrh	r2, [r2, #0]
	ldr	r3, [pc, #120]
	mov	r9, r2
	ldr	r7, [pc, #120]
	movs	r2, #4
	adds	r6, r0, #0
	mov	sl, r3
	mov	r8, r2
.L4:
	adds	r0, r7, #0
	movs	r1, #4
	bl	Func_080039fc
	adds	r0, r7, #0
	movs	r1, #16
	bl	Func_0800393c
	movs	r2, #16
	ldr	r3, [pc, #96]
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r1, fp
	adds	r0, r6, #0
	bl	Func_080040d0
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #52]
	ands	r0, r3
	ldrh	r3, [r5, #8]
	ands	r3, r2
	orrs	r3, r0
	mov	r2, sl
	strh	r3, [r5, #8]
	ldr	r3, [r2, #0]
	mov	r2, r8
	ands	r3, r2
	lsrs	r3, r3, #1
	ldr	r2, [pc, #56]
	add	r3, r9
	adds	r3, r3, r2
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #32]
	ands	r3, r2
	ldrh	r2, [r5, #6]
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r5, #6]
	mov	r2, sl
	ldr	r3, [r2, #0]
	mov	r2, r8
	b.n	.L2
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e40
	.4byte 0x0400004a
	.4byte 0x04000052
	.4byte 0x0000fffc
.L2:
	ands	r3, r2
	ldr	r2, [sp, #4]
	lsrs	r3, r3, #2
	subs	r3, r2, r3
	adds	r3, #248
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #240
	bl	Func_08003dec
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #52]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L4
.L3:
	adds	r0, r6, #0
	bl	Func_08003f3c
	movs	r0, #1
	bl	Func_080030f8
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x00000303
