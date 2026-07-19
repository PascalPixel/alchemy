.syntax unified
.text
	.arm
	.global Func_080155d0
Func_080155d0:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r7, r3
	sub	sp, sp, #44
	ldr	r3, [sp, #72]
	mov	r9, r2
	str	r0, [sp, #40]
	sub	ip, r1, #32
	ldrh	r0, [r0, #10]
	cmp	ip, #111
	str	r0, [sp, #28]
	movhi	r0, #1
	bhi .L_080158dc
	cmp	r1, #255
	bls .L_0801561c
	cmp	r3, #0
	sub	lr, r1, #222
	beq .L_08015750
	cmp	lr, #1
	bhi .L_08015750
.L_0801561c:
	cmp	r3, #0
	beq .L_0801572c
	add	r1, r3, r1, lsl #5
	str	r1, [sp, #24]
	mov	r1, #4352
	add	r1, r1, #17
	mov	r2, #13056
	str	r1, [sp, #20]
	add	r2, r2, #51
	str	r2, [sp, #16]
	ldr	r3, [pc, #416]
	mov	r2, #0
	ldrb	r3, [r3, ip]
	cmp	r7, #31
	str	r3, [sp, #12]
	bhi .L_08015724
	mov	sl, #9472
	add	sl, sl, #100663296
.L_08015664:
	mov	lr, #0
	add	r4, r2, #1
	add	r3, r7, r2
	lsr	r8, r3, #3
	and	r3, r3, #7
	lsl	r6, r3, #1
.L_0801567c:
	tst	lr, #1
	lsr	r5, r5, #4
	ldreq	r3, [sp, #24]
	ldrbeq	r5, [r3], #1
	streq	r3, [sp, #24]
	and	r3, r5, #13
	cmp	r3, #1
	bne .L_08015700
	and	r3, r5, #15
	cmp	r3, #1
	ldr	ip, [sp, #16]
	add	r1, r9, lr
	ldr	r0, [sp, #20]
	lsr	r2, r1, #3
	moveq	ip, r0
	cmp	r2, #29
	bhi .L_08015700
	ldr	r0, [sp, #28]
	sub	r3, r0, #2
	mla	r0, r3, r2, r8
	and	r3, r1, #7
	orr	r3, r6, r3, lsr #2
	and	r1, r1, #3
	lsl	r1, r1, #2
	add	r0, sl, r0, lsl #5
	orr	r0, r0, r3, lsl #1
	mov	r3, #15
	ldrh	r2, [r0]
	lsl	r3, r3, r1
	bic	r2, r2, r3
	and	r3, r3, ip
	orr	r2, r2, r3
	strh	r2, [r0]
.L_08015700:
	add	lr, lr, #1
	cmp	lr, #7
	bls .L_0801567c
	mov	r2, r4
	cmp	r4, #7
	bhi .L_08015724
	add	r3, r7, r4
	cmp	r3, #31
	bls .L_08015664
.L_08015724:
	ldr	r0, [sp, #12]
	b .L_080158dc
.L_0801572c:
	sub	lr, r1, #222
	add	r3, ip, ip, lsl #1
	add	r3, ip, r3, lsl #2
	ldr	r2, [pc, #176]
	lsl	r3, r3, #1
	add	r1, r3, r2
	add	r1, r1, #2
	str	r1, [sp, #36]
	b .L_08015768
.L_08015750:
	sub	r3, r1, #32
	ldr	r2, [pc, #148]
	lsl	r3, r3, #5
	add	r0, r3, r2
	add	r0, r0, #8
	str	r0, [sp, #36]
.L_08015768:
	ldrh	r0, [r3, r2]
	cmp	lr, #1
	bhi .L_08015788
	sub	r9, r9, #3
	sub	r7, r7, #2
	mov	r1, #0
	str	r1, [sp, #32]
	b .L_0801578c
.L_08015788:
	str	r0, [sp, #32]
.L_0801578c:
	ldr	r2, [sp, #36]
	mov	r1, #0
	add	r2, r2, #2
	str	r2, [sp, #4]
.L_0801579c:
	cmp	r1, #3
	moveq	r5, #4352
	addeq	r5, r5, #17
	movne	r5, #21760
	addne	r5, r5, #85
	ldr	r0, [sp, #40]
	ldrh	r3, [r0, #22]
	tst	r3, #64
	beq .L_080157f8
	cmp	r1, #1
	bhi .L_080157d8
	addeq	r9, r9, #2
	beq .L_080157f8
	sub	r9, r9, #1
	b .L_080157f8
.L_080157d8:
	cmp	r1, #2
	bne .L_080157f4
	sub	r9, r9, #1
	add	r7, r7, #1
	b .L_080157f8
	.4byte 0x080370d4
	.4byte 0x08032224
.L_080157f4:
	sub	r7, r7, #1
.L_080157f8:
	ldr	r3, [sp, #4]
	ldr	r0, [sp, #36]
	mov	r2, #0
	str	r3, [sp, #8]
	cmp	r7, #31
	ldrh	ip, [r0]
	add	r1, r1, #1
	str	r1, [sp]
	lsl	ip, ip, #16
	bhi .L_080158cc
	mov	r6, #9472
	add	r6, r6, #100663296
.L_08015828:
	mov	lr, #0
	add	r4, r2, #1
	add	r3, r7, r2
	lsr	sl, r3, #3
	and	r3, r3, #7
	lsl	r8, r3, #1
.L_08015840:
	cmp	ip, #0
	bge .L_08015890
	add	r1, r9, lr
	ldr	r0, [sp, #28]
	lsr	r2, r1, #3
	sub	r3, r0, #2
	mla	r0, r3, r2, sl
	and	r3, r1, #7
	orr	r3, r8, r3, lsr #2
	and	r1, r1, #3
	lsl	r1, r1, #2
	add	r0, r6, r0, lsl #5
	orr	r0, r0, r3, lsl #1
	mov	r3, #15
	ldrh	r2, [r0]
	lsl	r3, r3, r1
	bic	r2, r2, r3
	and	r3, r3, r5
	orr	r2, r2, r3
	strh	r2, [r0]
.L_08015890:
	lsls	ip, ip, #1
	beq .L_080158a4
	add	lr, lr, #1
	cmp	lr, #13
	bls .L_08015840
.L_080158a4:
	mov	r2, r4
	cmp	r4, #11
	bhi .L_080158cc
	ldr	r1, [sp, #8]
	add	r3, r7, r4
	ldrh	ip, [r1], #2
	cmp	r3, #31
	lsl	ip, ip, #16
	str	r1, [sp, #8]
	bls .L_08015828
.L_080158cc:
	ldr	r1, [sp]
	cmp	r1, #3
	bls .L_0801579c
	ldr	r0, [sp, #32]
.L_080158dc:
	add	sp, sp, #44
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
