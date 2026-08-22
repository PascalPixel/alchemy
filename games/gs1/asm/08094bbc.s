@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08094bbc
	.thumb_func
Func_08094bbc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #328]
	ldr	r0, [r3, #0]
	subs	r3, #84
	ldr	r3, [r3, #0]
	sub	sp, #16
	movs	r1, #0
	mov	r9, r0
	ldr	r2, [pc, #316]
	mov	r7, r9
	str	r3, [sp, #12]
	str	r1, [sp, #8]
	str	r1, [sp, #4]
	adds	r7, #8
	mov	fp, r2
.L11:
	ldrh	r3, [r7, #28]
	ldr	r2, [pc, #304]
	adds	r1, r3, r2
	adds	r3, r1, #0
	ands	r3, r2
	strh	r1, [r7, #28]
	cmp	r3, r2
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [sp, #12]
	adds	r3, #228
	ldr	r6, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r0, #179
	mov	r8, r3
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	lsls	r0, r0, #1
	mov	sl, r3
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L2:
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	movs	r3, #1
	ldr	r1, [r7, #12]
	ands	r0, r3
	ands	r3, r5
	adds	r3, r3, r0
	subs	r1, r1, r6
	lsrs	r3, r3, #1
	asrs	r1, r1, #16
	adds	r1, r1, r3
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #16]
	mov	r0, sl
	subs	r2, r2, r3
	mov	r3, r8
	subs	r2, r2, r3
	lsls	r3, r0, #16
	subs	r4, r1, #1
	asrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r1, #15
	subs	r0, r2, r3
	cmp	r1, #255
	bhi.n	.L3
	movs	r1, #32
	negs	r1, r1
	cmp	r0, r1
	blt.n	.L3
	cmp	r0, #159
	bgt.n	.L3
	ldrh	r3, [r7, #28]
	cmp	r3, #59
	bhi.n	.L4
	mov	r2, r9
	ldr	r3, [r2, #4]
	ldr	r1, [pc, #176]
	ldrh	r2, [r7, #8]
	adds	r3, #16
	ands	r3, r1
	mov	r1, fp
	ands	r2, r1
	orrs	r2, r3
	ldr	r3, [r7, #24]
	adds	r3, #3
	b.n	.L5
.L4:
	cmp	r3, #89
	bhi.n	.L6
	mov	r2, r9
	ldr	r3, [r2, #4]
	ldr	r1, [pc, #148]
	ldrh	r2, [r7, #8]
	adds	r3, #8
	ands	r3, r1
	mov	r1, fp
	ands	r2, r1
	orrs	r2, r3
	ldr	r3, [r7, #24]
	adds	r3, #1
.L5:
	strh	r2, [r7, #8]
	str	r3, [r7, #24]
	b.n	.L7
.L6:
	mov	r3, r9
	ldr	r2, [r3, #4]
	ldr	r1, [pc, #120]
	ldrh	r3, [r7, #8]
	ands	r2, r1
	mov	r1, fp
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #8]
.L7:
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	movs	r2, #1
	lsrs	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	ldrh	r2, [r7, #8]
	lsls	r3, r2, #22
	ldr	r1, [pc, #88]
	lsrs	r3, r3, #22
	adds	r3, #4
	ands	r3, r1
	mov	r1, fp
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r7, #8]
.L8:
	ldr	r3, [pc, #52]
	ldr	r2, [pc, #52]
	ands	r4, r3
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r4
	strh	r3, [r7, #6]
	ldr	r3, [r7, #24]
	asrs	r3, r3, #2
	subs	r3, r0, r3
	ldrb	r1, [r7, #5]
	movs	r2, #63
	strb	r3, [r7, #4]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r7, #5]
	ldrb	r3, [r7, #7]
	ands	r2, r3
	movs	r3, #64
	orrs	r2, r3
	strb	r2, [r7, #7]
	adds	r0, r7, #0
	movs	r1, #240
	bl	Func_08003dec
	b.n	.L1
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001ec4
	.4byte 0xfffffc00
	.4byte 0x0000ffff
	.4byte 0x000003ff
	.4byte 0x03001e40
.L3:
	movs	r3, #0
	strh	r3, [r7, #28]
.L1:
	ldr	r2, [sp, #8]
	cmp	r2, #7
	bhi.n	.L9
	ldrh	r3, [r7, #28]
	mov	r8, r3
	cmp	r3, #0
	bne.n	.L9
	ldr	r0, [sp, #12]
	ldr	r6, [r0, #0]
	bl	Func_08004458
	ldr	r3, [r6, #0]
	lsls	r0, r0, #8
	ldr	r5, [pc, #84]
	adds	r3, r3, r0
	adds	r4, r3, r5
	str	r4, [sp, #0]
	bl	Func_08004458
	ldr	r3, [r6, #8]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	ldr	r4, [sp, #0]
	adds	r0, r3, r5
	asrs	r2, r0, #16
	str	r0, [r7, #20]
	asrs	r1, r4, #16
	str	r4, [r7, #12]
	movs	r0, #0
	bl	Func_080091a8
	movs	r3, #120
	lsls	r0, r0, #16
	mov	r1, r8
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	str	r1, [r7, #24]
	ldr	r2, [sp, #8]
	adds	r2, #1
	str	r2, [sp, #8]
.L9:
	ldr	r3, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #4]
	adds	r7, #32
	cmp	r3, #31
	bhi.n	.L10
	b.n	.L11
.L10:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xff800000
