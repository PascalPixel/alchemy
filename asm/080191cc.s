@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080191cc
	.thumb_func
Func_080191cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	ldr	r3, [r3, #0]
	movs	r2, #160
	mov	r9, r3
	lsls	r2, r2, #3
	add	r2, r9
	movs	r3, #0
	sub	sp, #24
	mov	sl, r2
	mov	fp, r3
.L16:
	mov	r3, sl
	ldrh	r2, [r3, #22]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [pc, #116]
	mov	r2, sl
	ldr	r3, [r3, #0]
	ldr	r6, [r2, #0]
	b.n	.L2
.L14:
	mov	r2, sl
	ldrh	r3, [r2, #18]
	adds	r7, r6, #0
	adds	r7, #16
	cmp	r3, #4
	bne.n	.L3
	movs	r3, #2
	strh	r3, [r6, #12]
	movs	r3, #8
	strb	r3, [r6, #5]
.L3:
	ldrb	r3, [r6, #5]
	subs	r3, #2
	cmp	r3, #16
	bls.n	.L4
	b.n	.L5
.L4:
	ldr	r2, [pc, #80]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801927c
	.4byte 0x080195e0
	.4byte 0x0801947c
	.4byte 0x08019310
	.4byte 0x08019380
	.4byte 0x080193fc
	.4byte 0x08019548
	.4byte 0x080195da
	.4byte 0x080195da
	.4byte 0x080195da
	.4byte 0x080195da
	.4byte 0x080195e0
	.4byte 0x080194f6
	.4byte 0x080194f6
	.4byte 0x080194f6
	.4byte 0x080194e0
	.4byte 0x0801950c
	.4byte 0x03001e8c
	.4byte 0x03001800
	.4byte 0x0801922c
	ldr	r1, [pc, #84]
	add	r1, r9
	ldrh	r3, [r1, #0]
	cmp	r3, #96
	bne.n	.L6
	b.n	.L5
.L6:
	ldr	r3, [pc, #76]
	lsls	r2, r0, #7
	adds	r2, r2, r3
	ldrh	r0, [r1, #0]
	movs	r1, #128
	bl	Func_08003fa4
	ldr	r3, [pc, #56]
	ldrh	r2, [r7, #8]
	ands	r0, r3
	ldr	r3, [pc, #60]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #8]
	strb	r3, [r6, #14]
	ldrb	r3, [r7, #5]
	movs	r5, #13
	negs	r5, r5
	ands	r5, r3
	movs	r3, #17
	negs	r3, r3
	ands	r5, r3
	movs	r3, #32
	ldrb	r2, [r7, #7]
	orrs	r5, r3
	movs	r3, #63
	adds	r4, r3, #0
	ands	r5, r3
	movs	r3, #128
	ands	r4, r2
	orrs	r5, r3
	strb	r4, [r7, #7]
	strb	r5, [r7, #5]
	ldrb	r3, [r6, #8]
	b.n	.L7
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x000012b6
	.4byte 0x080368d4
	.4byte 0xfffffc00
.L7:
	mov	r8, r3
	ldr	r3, [pc, #144]
	ldr	r2, [pc, #144]
	ldr	r0, [r3, #0]
	movs	r1, #80
	str	r2, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_08002304
	ldr	r2, [sp, #4]
	ldrb	r3, [r2, r0]
	mov	r2, r8
	adds	r3, r2, r3
	adds	r3, #2
	strb	r3, [r7, #4]
	movs	r3, #4
	negs	r3, r3
	ldr	r4, [sp, #0]
	ands	r5, r3
	subs	r3, #59
	ands	r3, r4
	strb	r5, [r7, #5]
	strb	r3, [r7, #7]
	b.n	.L5
	ldr	r3, [pc, #96]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L8
	b.n	.L5
.L8:
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	lsls	r2, r5, #1
	lsls	r3, r0, #1
	adds	r2, r2, r5
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	ldrh	r1, [r6, #6]
	adds	r2, r2, r3
	lsrs	r2, r2, #1
	adds	r1, r1, r2
	ldr	r3, [pc, #48]
	subs	r1, #1
	ands	r1, r3
	ldrh	r2, [r7, #6]
	ldr	r3, [pc, #52]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	lsls	r2, r5, #1
	lsls	r3, r0, #1
	adds	r2, r2, r5
	adds	r3, r3, r0
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	ldrb	r1, [r6, #8]
	adds	r2, r2, r3
	lsrs	r2, r2, #1
	adds	r1, r1, r2
	subs	r1, #1
	strb	r1, [r7, #4]
	b.n	.L5
	.4byte 0x000001ff
	.4byte 0x03001800
	.4byte 0x08033e60
	.4byte 0xfffffe00
	ldrh	r3, [r6, #12]
	cmp	r3, #0
	bne.n	.L9
	b.n	.L10
.L9:
	ldr	r1, [pc, #96]
	ldr	r3, [sp, #16]
	movs	r2, #128
	ands	r3, r1
	lsls	r2, r2, #2
	ldr	r5, [pc, #92]
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #18
	ands	r3, r5
	orrs	r3, r2
	str	r3, [sp, #16]
	add	r0, sp, #16
	ldr	r3, [r0, #4]
	ands	r3, r1
	str	r3, [r0, #4]
	bl	Func_08003d28
	movs	r3, #31
	ldrb	r2, [r7, #7]
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r7, #7]
	ldrb	r3, [r7, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldr	r3, [pc, #44]
	ldrh	r2, [r6, #6]
	adds	r2, r2, r3
	ldr	r3, [pc, #24]
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #36]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrb	r3, [r6, #8]
	adds	r3, #251
	strb	r3, [r7, #4]
	ldrh	r3, [r6, #12]
	adds	r3, r3, r5
	strh	r3, [r6, #12]
	b.n	.L5
	.4byte 0x000001ff
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.4byte 0x0000fffb
	.4byte 0xfffffe00
	movs	r3, #128
	add	r5, sp, #16
	lsls	r3, r3, #1
	strh	r3, [r5, #0]
	strh	r3, [r5, #2]
	movs	r2, #192
	ldrh	r3, [r6, #12]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strh	r3, [r6, #12]
	strh	r3, [r5, #4]
	adds	r0, r5, #0
	bl	Func_08003d28
	movs	r3, #31
	ldrb	r2, [r7, #7]
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	ldrb	r2, [r7, #5]
	strb	r3, [r7, #7]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldrh	r0, [r5, #4]
	movs	r3, #232
	lsls	r3, r3, #8
	adds	r0, r0, r3
	bl	Func_08002322
	ldrh	r2, [r6, #6]
	asrs	r0, r0, #14
	subs	r2, r2, r0
	ldr	r3, [pc, #40]
	subs	r2, #2
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #36]
	ands	r3, r1
	orrs	r3, r2
	ldrh	r0, [r5, #4]
	movs	r2, #208
	lsls	r2, r2, #7
	strh	r3, [r7, #6]
	adds	r0, r0, r2
	bl	Func_0800231c
	ldrb	r3, [r6, #8]
	asrs	r0, r0, #14
	subs	r3, r3, r0
	subs	r3, #2
	strb	r3, [r7, #4]
	b.n	.L5
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	ldrh	r3, [r6, #12]
	adds	r3, #1
	strh	r3, [r6, #12]
.L11:
	ldr	r4, [pc, #72]
	ldrh	r0, [r6, #12]
	movs	r1, #20
	str	r4, [sp, #0]
	bl	Func_08002304
	ldr	r4, [sp, #0]
	lsls	r0, r0, #16
	lsrs	r0, r0, #15
	ldrsb	r3, [r4, r0]
	ldrh	r2, [r6, #6]
	adds	r2, r2, r3
	ldr	r3, [pc, #40]
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	movs	r1, #20
	ldrh	r0, [r6, #12]
	bl	Func_08002304
	lsls	r0, r0, #16
	lsrs	r0, r0, #15
	ldr	r4, [sp, #0]
	adds	r0, #1
	ldrb	r5, [r6, #8]
	ldrb	r3, [r4, r0]
	adds	r5, r5, r3
	subs	r5, #2
	strb	r5, [r7, #4]
	b.n	.L5
	.4byte 0x000001ff
	.4byte 0x03001800
	.4byte 0x08033eb0
	.4byte 0xfffffe00
	ldrh	r3, [r6, #12]
	ldr	r0, [pc, #92]
	adds	r3, #1
	movs	r2, #15
	strh	r3, [r6, #12]
	ands	r3, r2
	ldrb	r1, [r6, #8]
	ldrb	r3, [r0, r3]
	subs	r1, r1, r3
	strb	r1, [r7, #4]
	b.n	.L5
	ldrh	r3, [r6, #12]
	ldr	r0, [pc, #68]
	adds	r3, #1
	movs	r2, #15
	strh	r3, [r6, #12]
	ands	r3, r2
	ldrb	r1, [r6, #8]
	ldrb	r3, [r0, r3]
	adds	r1, r1, r3
	strb	r1, [r7, #4]
	b.n	.L5
	ldrh	r3, [r6, #12]
	ldr	r4, [pc, #48]
	adds	r3, #1
	movs	r0, #15
	strh	r3, [r6, #12]
	ands	r3, r0
	ldrh	r2, [r6, #6]
	ldrsb	r3, [r4, r3]
	subs	r2, r2, r3
	ldr	r3, [pc, #28]
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrh	r3, [r6, #12]
	ands	r0, r3
	ldrb	r2, [r6, #8]
	ldrb	r3, [r4, r0]
	adds	r2, r2, r3
	strb	r2, [r7, #4]
	b.n	.L5
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x08033ee8
	.4byte 0xfffffe00
	ldrh	r3, [r6, #12]
	cmp	r3, #0
	beq.n	.L10
	movs	r3, #160
	lsls	r3, r3, #1
	add	r0, sp, #16
	movs	r2, #0
	strh	r3, [r0, #0]
	strh	r3, [r0, #2]
	strh	r2, [r0, #4]
	bl	Func_08003d28
	movs	r3, #31
	ldrb	r2, [r7, #7]
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r7, #7]
	ldrb	r3, [r7, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldr	r3, [pc, #40]
	ldrh	r2, [r6, #6]
	adds	r2, r2, r3
	ldr	r3, [pc, #28]
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #32]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrb	r3, [r6, #8]
	adds	r3, #248
	strb	r3, [r7, #4]
	ldr	r2, [pc, #20]
	ldrh	r3, [r6, #12]
	adds	r3, r3, r2
	strh	r3, [r6, #12]
	b.n	.L5
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x0000fff8
	.4byte 0xfffffe00
	.4byte 0x0000ffff
.L10:
	ldrb	r2, [r7, #7]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, #7]
	ldrb	r2, [r7, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, #5]
	ldr	r2, [pc, #116]
	ldrh	r3, [r6, #6]
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #112]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrh	r3, [r6, #8]
	strb	r3, [r7, #4]
	b.n	.L5
	adds	r0, r6, #0
	bl	Func_0801908c
.L5:
	ldrb	r3, [r6, #5]
	cmp	r3, #2
	bne.n	.L12
	ldr	r3, [pc, #92]
	add	r3, r9
	ldrh	r3, [r3, #0]
	cmp	r3, #96
	beq.n	.L13
	ldrb	r1, [r6, #15]
	adds	r0, r7, #0
	bl	Func_08003dec
	b.n	.L13
.L12:
	cmp	r3, #13
	beq.n	.L13
	ldrb	r1, [r6, #15]
	adds	r0, r7, #0
	bl	Func_08003dec
.L13:
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	ldr	r6, [r6, #0]
.L2:
	lsrs	r0, r3, #2
	movs	r3, #7
	ands	r0, r3
	cmp	r6, #0
	beq.n	.L1
	b.n	.L14
.L1:
	movs	r2, #1
	movs	r3, #36
	add	fp, r2
	add	sl, r3
	mov	r3, fp
	cmp	r3, #8
	beq.n	.L15
	b.n	.L16
.L15:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000012b6
	.4byte 0x03001800
