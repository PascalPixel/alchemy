@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809a484
	.thumb_func
Func_0809a484:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r6, r1, #0
	ldr	r1, [sp, #48]
	adds	r5, r0, #0
	movs	r0, #0
	mov	r8, r2
	str	r3, [sp, #4]
	mov	sl, r1
	ldr	r7, [sp, #52]
	bl	Func_08092054
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r2, sl
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L0
	cmp	r7, #0
	beq.n	.L0
	movs	r3, #24
	ldrsh	r0, [r7, r3]
	adds	r2, r6, #0
	b.n	.L1
.L0:
	adds	r2, r6, #0
	movs	r0, #222
.L1:
	adds	r1, r5, #0
	mov	r3, r8
	bl	Func_080090c8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L2
	b.n	.L3
.L2:
	ldr	r1, [r6, #80]
	mov	r8, r1
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	bl	Func_08009080
	mov	r3, sl
	ldr	r2, [pc, #356]
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl	Func_08009098
	adds	r3, r6, #0
	movs	r0, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	mov	r3, r8
	adds	r3, #38
	strb	r0, [r3, #0]
	ldr	r3, [pc, #328]
	str	r3, [r6, #108]
	ldr	r3, [sp, #4]
	str	r3, [r6, #68]
	ldr	r3, [sp, #40]
	str	r3, [r6, #72]
	ldr	r3, [sp, #44]
	mov	r1, r9
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	mov	r9, r3
	ands	r3, r1
	orrs	r3, r2
	adds	r2, r6, #0
	mov	r1, r8
	adds	r2, #100
	strb	r3, [r1, #9]
	adds	r3, r2, #0
	str	r0, [r6, #48]
	str	r0, [r6, #52]
	str	r2, [sp, #0]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #276]
	mov	r1, sl
	ands	r3, r1
	movs	r5, #3
	cmp	r3, #0
	beq.n	.L3
	cmp	r7, #0
	beq.n	.L3
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L4
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl	Func_080929d8
.L4:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	mov	r3, r8
	ldrb	r2, [r7, #0]
	ldrb	r1, [r3, #9]
	ands	r2, r5
	mov	r3, r9
	ands	r3, r1
	lsls	r2, r2, #2
	orrs	r3, r2
	mov	r1, r8
	strb	r3, [r1, #9]
.L5:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L6
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L6:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L7
	ldr	r3, [pc, #156]
	mov	r1, fp
	ldr	r5, [r3, r1]
	cmp	r2, #0
	beq.n	.L8
	ldr	r0, [r7, #16]
	ldr	r3, [r6, #24]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	bl	Func_080022ec
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L9
.L8:
	ldr	r0, [r7, #16]
	ldr	r2, [pc, #128]
	ldr	r1, [r5, #12]
	adds	r0, r0, r2
	bl	Func_080022ec
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L9:
	bl	Func_080022ec
	str	r0, [r6, #52]
.L7:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L10
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl	Func_08009098
.L10:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #30]
.L11:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	ldrh	r3, [r7, #34]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #0]
.L12:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L3:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080a012c
	.4byte 0x0809a44d
	.4byte 0xffff0000
