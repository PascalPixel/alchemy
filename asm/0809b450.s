@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b450
	.thumb_func
Func_0809b450:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #0]
	ldr	r1, [r3, #16]
	sub	sp, #12
	movs	r2, #4
	mov	sl, r3
	add	r2, sp
	movs	r3, #63
	str	r1, [sp, #0]
	adds	r6, r0, #0
	movs	r7, #0
	mov	r9, r2
	mov	fp, r3
.L2:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #15
	ldr	r1, [r6, #8]
	adds	r2, r2, r3
	movs	r0, #26
	ldr	r3, [r6, #16]
	bl	Func_080090c8
	lsls	r3, r7, #2
	mov	r1, r9
	str	r0, [r3, r1]
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r6, #20]
	str	r3, [r0, #20]
	adds	r3, r0, #0
	adds	r3, #85
	movs	r2, #0
	ldr	r5, [r0, #80]
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	ldr	r1, [pc, #56]
	ldr	r3, [pc, #60]
	mov	r8, r1
	str	r6, [r0, #104]
	str	r3, [r0, #28]
	str	r3, [r0, #24]
	cmp	r5, #0
	beq.n	.L0
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_08009020
	adds	r3, r5, #0
	adds	r3, #38
	mov	r2, r8
	strb	r2, [r3, #0]
	ldrb	r0, [r5, #28]
	bl	Func_08003f3c
	ldr	r3, [pc, #28]
	add	r3, sl
	ldrh	r3, [r3, #0]
	strb	r3, [r5, #28]
	ldrb	r3, [r5, #29]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r5, #29]
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001f30
	.4byte 0x00006666
	.4byte 0x0000071a
.L1:
	ldrb	r3, [r5, #28]
	ldr	r2, [pc, #64]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r1, [r3, #2]
	ldr	r2, [pc, #52]
	ldrh	r3, [r5, #8]
	lsls	r1, r1, #17
	lsrs	r1, r1, #22
	ands	r3, r2
	orrs	r3, r1
	movs	r1, #33
	negs	r1, r1
	strh	r3, [r5, #8]
	ldrb	r3, [r5, #5]
	adds	r2, r1, #0
	ands	r3, r2
	mov	r2, fp
	ands	r3, r2
	movs	r2, #64
	orrs	r3, r2
	ldrb	r2, [r5, #7]
	strb	r3, [r5, #5]
	mov	r3, fp
	ands	r3, r2
	movs	r2, #128
	orrs	r3, r2
	strb	r3, [r5, #7]
	ldr	r3, [r5, #40]
	mov	r1, r8
	strb	r1, [r3, #22]
	b.n	.L0
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L0:
	adds	r7, #1
	cmp	r7, #1
	ble.n	.L2
	ldr	r2, [sp, #4]
	ldr	r3, [pc, #60]
	ldr	r0, [r2, #80]
	str	r3, [r2, #108]
	movs	r2, #13
	ldrb	r1, [r0, #9]
	negs	r2, r2
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r0, #9]
	mov	r3, r9
	ldr	r1, [r3, #4]
	ldr	r3, [pc, #44]
	str	r3, [r1, #108]
	ldr	r0, [r1, #80]
	ldr	r1, [sp, #0]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	movs	r3, #12
	ands	r3, r1
	ldrb	r1, [r0, #9]
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r0, #9]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0809b3d9
	.4byte 0x0809b365
