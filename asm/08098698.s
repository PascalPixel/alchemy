@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08098698
	.thumb_func
Func_08098698:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	sub	sp, #12
	mov	r9, r3
	ldr	r7, [r3, #16]
	bl	Func_08097384
	movs	r0, #23
	mov	r8, sp
	mov	sl, r8
	mov	fp, r0
.L8:
	mov	r2, r9
	movs	r5, #128
	ldr	r3, [r2, #0]
	lsls	r5, r5, #7
	cmp	r3, r5
	bne.n	.L0
	ldr	r3, [r7, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r2, #160
	lsls	r2, r2, #12
	b.n	.L1
.L0:
	movs	r5, #192
	lsls	r5, r5, #8
	cmp	r3, r5
	bne.n	.L2
	ldr	r3, [r7, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r2, #192
	lsls	r2, r2, #13
.L1:
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	b.n	.L3
	.4byte 0x03001f30
.L2:
	ldr	r3, [r7, #8]
	mov	r5, sl
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #160
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	mov	r2, r9
	ldr	r1, [r2, #0]
	mov	r2, sl
	bl	Func_0800447c
.L3:
	mov	r3, sl
	movs	r0, #142
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r0, r0, #1
	ldr	r3, [r3, #8]
	bl	Func_08096c80
	adds	r6, r0, #0
	ldr	r4, [r6, #80]
	ldrb	r3, [r4, #5]
	adds	r0, r4, #0
	adds	r0, #12
	movs	r1, #32
	movs	r5, #33
	ands	r1, r3
	negs	r5, r5
	ldrb	r3, [r0, #5]
	adds	r2, r5, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #5]
	ldrb	r2, [r4, #5]
	movs	r1, #63
	lsrs	r2, r2, #6
	lsls	r2, r2, #6
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #5]
	ldrb	r3, [r4, #7]
	ldrb	r2, [r0, #7]
	lsrs	r3, r3, #6
	lsls	r3, r3, #6
	ands	r1, r2
	orrs	r1, r3
	strb	r1, [r0, #7]
	ldrh	r1, [r4, #8]
	ldrh	r3, [r0, #8]
	ldr	r2, [pc, #56]
	lsls	r1, r1, #22
	lsrs	r1, r1, #22
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	ldrb	r2, [r4, #9]
	ldrb	r1, [r0, #9]
	lsrs	r2, r2, #4
	movs	r3, #15
	lsls	r2, r2, #4
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	cmp	r6, #0
	beq.n	.L4
	ldr	r3, [pc, #28]
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	movs	r3, #192
	lsls	r3, r3, #9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	str	r3, [r6, #48]
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	b.n	.L5
	.4byte 0xfffffc00
	.4byte 0x0000b333
.L5:
	movs	r1, #11
	bl	Func_08009240
	adds	r0, r6, #0
	movs	r1, #7
	bl	Func_08009080
	adds	r0, r6, #0
	ldr	r1, [pc, #140]
	bl	Func_08009098
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_080091e0
	mov	r0, r9
	ldr	r3, [r0, #4]
	mov	r2, r8
	str	r3, [r2, #0]
	ldr	r3, [r0, #8]
	str	r3, [r2, #4]
	ldr	r3, [r0, #12]
	str	r3, [r2, #8]
	movs	r3, #192
	ldr	r1, [r0, #0]
	lsls	r3, r3, #8
	cmp	r1, r3
	bne.n	.L6
	movs	r0, #224
	lsls	r0, r0, #12
	bl	Func_0800447c
.L6:
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #11
	lsls	r5, r5, #1
	adds	r5, r5, r0
	bl	Func_08004458
	mov	r2, r8
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	mov	r5, r8
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r3, [r5, #8]
	ldr	r2, [r2, #4]
	adds	r0, r6, #0
	bl	Func_08009150
.L4:
	movs	r0, #131
	bl	Func_080f9010
	movs	r0, #2
	bl	Func_080030f8
	movs	r0, #1
	negs	r0, r0
	add	fp, r0
	mov	r2, fp
	cmp	r2, #0
	blt.n	.L7
	b.n	.L8
.L7:
	movs	r0, #8
	bl	Func_080030f8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0809f0b4
