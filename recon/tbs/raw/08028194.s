.syntax unified
	.thumb
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.global AffineEffect_UpdateFrame
	.global Func_08028194
	.thumb_func
AffineEffect_UpdateFrame:
Func_08028194:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #760]
	ldr	r3, [r3, #0]
	mov	r8, r3
	ldr	r3, [pc, #756]
	ldr	r3, [r3, #0]
	movs	r2, #31
	lsls	r3, r3, #1
	ldr	r1, [pc, #752]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r7, [r1, r3]
	ldr	r0, [pc, #748]
	adds	r3, r7, r0
	sub	sp, #12
	mov	r6, r8
	cmp	r3, #0
	bge.n	.L_080281c4
	adds	r3, r7, #0
	subs	r3, #253
.L_080281c4:
	movs	r1, #152
	asrs	r3, r3, #2
	lsls	r1, r1, #1
	ldr	r4, [pc, #732]
	adds	r7, r3, r1
	ldr	r3, [sp, #4]
	lsls	r1, r7, #16
	ands	r3, r4
	ldr	r2, [pc, #724]
	lsrs	r1, r1, #16
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #4]
	add	r0, sp, #4
	ldr	r3, [r0, #4]
	ands	r3, r4
	str	r3, [r0, #4]
	bl	sub_08003d28
	movs	r2, #142
	add	r2, r8
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r5, #0
	mov	r9, r0
	mov	sl, r2
	cmp	r5, r3
	bcs.n	.L_080282bc
	ldr	r4, [pc, #684]
.L_08028202:
	movs	r0, #12
	ldrsh	r2, [r6, r0]
	cmp	r2, #0
	beq.n	.L_080282ae
	mov	r3, r8
	adds	r3, #140
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r5, r3
	bne.n	.L_08028282
	lsls	r3, r7, #3
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_08028222
	ldr	r0, [pc, #660]
	adds	r3, r3, r0
.L_08028222:
	asrs	r3, r3, #9
	adds	r3, r2, r3
	movs	r2, #14
	ldrsh	r1, [r6, r2]
	subs	r3, #20
	mov	ip, r3
	cmp	r1, #0
	beq.n	.L_08028246
	lsls	r3, r7, #1
	adds	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_0802823c
	adds	r3, #255
.L_0802823c:
	asrs	r3, r3, #8
	adds	r3, r1, r3
	adds	r1, r3, #0
	subs	r1, #20
	b.n	.L_0802825a
.L_08028246:
	lsls	r3, r7, #4
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_08028250
	adds	r3, #255
.L_08028250:
	asrs	r3, r3, #8
	adds	r1, r3, #0
	subs	r1, #30
	movs	r3, #255
	ands	r1, r3
.L_0802825a:
	adds	r0, r6, #0
	movs	r3, #0
	mov	r2, r9
	stmia	r0!, {r3}
	lsls	r3, r2, #25
	orrs	r3, r1
	mov	r1, ip
	lsls	r2, r1, #16
	orrs	r3, r2
	ldr	r2, [pc, #584]
	orrs	r3, r2
	stmia	r0!, {r3}
	ldrh	r3, [r6, #18]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [r0, #0]
	movs	r1, #246
	b.n	.L_080282a4
.L_08028282:
	adds	r0, r6, #0
	movs	r3, #14
	ldrsh	r1, [r6, r3]
	movs	r3, #0
	stmia	r0!, {r3}
	lsls	r3, r2, #16
	orrs	r1, r3
	ldr	r3, [pc, #552]
	orrs	r1, r3
	ldrh	r3, [r6, #18]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #2]
	stmia	r0!, {r1}
	lsrs	r3, r3, #5
	str	r3, [r0, #0]
	movs	r1, #245
.L_080282a4:
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	sub_08003dec
	ldr	r4, [sp, #0]
.L_080282ae:
	mov	r1, sl
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	adds	r5, #1
	adds	r6, #20
	cmp	r5, r3
	bcc.n	.L_08028202
.L_080282bc:
	mov	r3, r8
	adds	r3, #148
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080283ae
	ldr	r3, [pc, #500]
	ldr	r1, [r3, #0]
	cmp	r1, #0
	bne.n	.L_080282d2
	b.n	.L_08028488
.L_080282d2:
	mov	r0, sl
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	bne.n	.L_080282de
	b.n	.L_08028488
.L_080282de:
	ldr	r2, [pc, #484]
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	movs	r3, #140
	add	r3, r8
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	mov	lr, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	mov	sl, r1
	adds	r1, r3, #0
	lsls	r4, r7, #1
	mov	r2, r8
	adds	r1, #12
	ldrsh	r0, [r2, r1]
	mov	ip, r4
	adds	r3, r4, r7
	ldr	r4, [pc, #436]
	lsls	r3, r3, #2
	adds	r2, r3, r4
	cmp	r2, #0
	bge.n	.L_0802831e
	adds	r4, #255
	adds	r2, r3, r4
.L_0802831e:
	asrs	r2, r2, #8
	subs	r3, r0, r2
	lsls	r3, r3, #8
	adds	r2, r0, r2
	adds	r3, r3, r2
	mov	r2, r8
	adds	r0, r3, #0
	adds	r3, r2, r1
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	ldr	r1, [pc, #408]
	lsls	r2, r7, #5
	adds	r4, r3, #0
	adds	r3, r2, r1
	adds	r0, #23
	adds	r4, #24
	cmp	r3, #0
	bge.n	.L_08028346
	ldr	r1, [pc, #396]
	adds	r3, r2, r1
.L_08028346:
	asrs	r3, r3, #9
	adds	r3, r4, r3
	mov	r1, sl
	adds	r4, r3, #1
	movs	r5, #24
	adds	r1, #102
	cmp	r5, r4
	bcs.n	.L_0802836a
	movs	r6, #255
.L_08028358:
	ldrh	r2, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, r4
	bcc.n	.L_08028358
.L_0802836a:
	mov	r3, r8
	mov	r1, lr
	movs	r2, #12
	ldrsh	r0, [r3, r2]
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_08028390
	mov	r2, ip
	adds	r3, r2, r7
	ldr	r4, [pc, #328]
	lsls	r1, r3, #2
	adds	r3, r1, r4
	cmp	r3, #0
	bge.n	.L_0802838c
	ldr	r2, [pc, #328]
	adds	r3, r1, r2
.L_0802838c:
	asrs	r3, r3, #8
	subs	r0, r0, r3
.L_08028390:
	mov	r1, sl
	lsls	r0, r0, #8
	adds	r1, #6
	movs	r5, #0
	movs	r4, #255
.L_0802839a:
	ldrh	r2, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #23
	bls.n	.L_0802839a
	b.n	.L_08028488
.L_080283ae:
	ldr	r3, [pc, #272]
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_08028488
	mov	r0, sl
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L_08028488
	ldr	r2, [pc, #256]
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	movs	r3, #140
	add	r3, r8
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	mov	ip, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	mov	lr, r1
	adds	r1, r3, #0
	mov	r2, r8
	adds	r1, #12
	lsls	r6, r7, #1
	ldrsh	r0, [r2, r1]
	ldr	r4, [pc, #216]
	adds	r3, r6, r7
	lsls	r2, r3, #2
	adds	r3, r2, r4
	cmp	r3, #0
	bge.n	.L_080283fe
	adds	r4, #255
	adds	r3, r2, r4
.L_080283fe:
	asrs	r3, r3, #8
	subs	r2, r0, r3
	lsls	r2, r2, #8
	adds	r3, r0, r3
	adds	r2, r2, r3
	adds	r0, r2, #0
	mov	r2, r8
	adds	r3, r2, r1
	movs	r4, #2
	ldrsh	r2, [r3, r4]
	ldr	r4, [pc, #184]
	lsls	r1, r7, #5
	adds	r3, r1, r4
	adds	r0, #23
	cmp	r3, #0
	bge.n	.L_08028422
	ldr	r4, [pc, #176]
	adds	r3, r1, r4
.L_08028422:
	asrs	r3, r3, #9
	subs	r3, r2, r3
	subs	r2, r3, #1
	lsls	r3, r2, #2
	add	r3, lr
	adds	r5, r2, #0
	adds	r1, r3, #6
	cmp	r5, #135
	bhi.n	.L_08028448
	movs	r4, #255
.L_08028436:
	ldrh	r2, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #135
	bls.n	.L_08028436
.L_08028448:
	mov	r2, r8
	movs	r1, #12
	ldrsh	r0, [r2, r1]
	mov	r1, ip
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_0802846c
	adds	r3, r6, r7
	ldr	r2, [pc, #108]
	lsls	r1, r3, #2
	adds	r3, r1, r2
	cmp	r3, #0
	bge.n	.L_08028468
	ldr	r4, [pc, #108]
	adds	r3, r1, r4
.L_08028468:
	asrs	r3, r3, #8
	subs	r0, r0, r3
.L_0802846c:
	ldr	r1, [pc, #104]
	lsls	r0, r0, #8
	add	r1, lr
	movs	r5, #136
	movs	r4, #255
.L_08028476:
	ldrh	r2, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r2
	orrs	r3, r0
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #159
	bls.n	.L_08028476
.L_08028488:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f38
	.4byte 0x03001e40
	.4byte 0x080366f8
	.4byte 0xffffff00
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.4byte 0x03001b10
	.4byte 0x000001ff
	.4byte 0x80002300
	.4byte 0x80002000
	.4byte 0x03001ecc
	.4byte 0x00000539
	.4byte 0xfffff4ff
	.4byte 0xffffe0ff
	.4byte 0xffffe2fe
	.4byte 0xfffff5fe
	.4byte 0x00000226
