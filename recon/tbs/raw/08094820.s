.syntax unified
	.thumb
	.set sub_08003dec, 0x08003dec
	.set sub_08004458, 0x08004458
	.set sub_080091a8, 0x080091a8
	.set sub_080770c0, 0x080770c0
	.global Unnamed_08094820
	.global Func_08094820
	.thumb_func
Unnamed_08094820:
Func_08094820:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #236]
	ldr	r0, [r3, #0]
	ldr	r3, [r3, #84]
	mov	sl, r0
	mov	r8, r3
	mov	r3, sl
	adds	r3, #228
	ldr	r1, [r3, #0]
	sub	sp, #12
	str	r1, [sp, #8]
	ldr	r3, [r3, #4]
	mov	r7, r8
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #63
	adds	r7, #8
	mov	r9, r2
	mov	fp, r3
.L_08094852:
	ldrh	r3, [r7, #28]
	ldr	r1, [pc, #200]
	adds	r3, r3, r1
	adds	r2, r1, #0
	ands	r2, r3
	strh	r3, [r7, #28]
	cmp	r2, r1
	bne.n	.L_08094864
	b.n	.L_08094984
.L_08094864:
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_08094876
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
.L_08094876:
	ldrh	r2, [r7, #28]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #164]
	lsls	r3, r3, #1
	adds	r4, r3, r2
	ldr	r5, [sp, #8]
	ldr	r3, [r7, #12]
	subs	r2, r3, r5
	cmp	r2, #0
	bge.n	.L_08094890
	ldr	r0, [pc, #144]
	adds	r2, r2, r0
.L_08094890:
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	asrs	r2, r2, #16
	adds	r1, r2, r3
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #20]
	subs	r3, r3, r2
	ldr	r2, [sp, #4]
	subs	r3, r3, r2
	adds	r4, #2
	cmp	r3, #0
	bge.n	.L_080948ac
	ldr	r5, [pc, #116]
	adds	r3, r3, r5
.L_080948ac:
	movs	r0, #0
	ldrsh	r2, [r4, r0]
	asrs	r3, r3, #16
	adds	r0, r3, r2
	adds	r3, r1, #0
	adds	r3, #16
	adds	r4, #2
	cmp	r3, #255
	bhi.n	.L_08094946
	movs	r2, #32
	negs	r2, r2
	cmp	r0, r2
	blt.n	.L_08094946
	cmp	r0, #159
	bgt.n	.L_08094946
	movs	r5, #13
	ldrb	r2, [r7, #9]
	negs	r5, r5
	adds	r3, r5, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldr	r3, [pc, #48]
	strb	r2, [r7, #9]
	ands	r1, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	strb	r0, [r7, #4]
	mov	r0, r8
	ldrh	r3, [r4, #0]
	ldr	r1, [r0, #4]
	adds	r1, r1, r3
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	ands	r1, r3
	ldrh	r3, [r7, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	adds	r4, #2
	ldrb	r2, [r4, #0]
	ldrb	r1, [r7, #5]
	mov	r3, fp
	b.n	.L_08094928
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x03001e70
	.4byte 0x0000ffff
	.2byte 0xef84
	.2byte 0x0809
.L_08094928:
	lsls	r2, r2, #6
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldrb	r1, [r7, #7]
	ldrb	r2, [r4, #2]
	mov	r3, fp
	ands	r3, r1
	lsls	r2, r2, #6
	orrs	r3, r2
	strb	r3, [r7, #7]
	adds	r0, r7, #0
	movs	r1, #240
	bl	sub_08003dec
.L_08094946:
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_08094984
	mov	r1, sl
	ldr	r6, [r1, #0]
	bl	sub_08004458
	ldr	r3, [r6, #0]
	lsls	r0, r0, #8
	ldr	r5, [pc, #72]
	adds	r3, r3, r0
	adds	r1, r3, r5
	str	r1, [sp, #0]
	bl	sub_08004458
	ldr	r3, [r6, #8]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	ldr	r1, [sp, #0]
	adds	r0, r3, r5
	str	r1, [r7, #12]
	str	r0, [r7, #20]
	asrs	r2, r0, #16
	asrs	r1, r1, #16
	movs	r0, #0
	bl	sub_080091a8
	movs	r3, #16
	lsls	r0, r0, #16
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
.L_08094984:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r7, #32
	cmp	r3, #31
	bhi.n	.L_08094992
	b.n	.L_08094852
.L_08094992:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xff800000
