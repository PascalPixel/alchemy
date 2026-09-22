.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08015090, 0x08015090
	.set sub_08015098, 0x08015098
	.set sub_080150b0, 0x080150b0
	.set sub_080150b8, 0x080150b8
	.set sub_08015280, 0x08015280
	.set sub_08077008, 0x08077008
	.set sub_080771f8, 0x080771f8
	.set sub_08077290, 0x08077290
	.set sub_080a8914, 0x080a8914
	.set sub_080a8b10, 0x080a8b10
	.set sub_080a9d3c, 0x080a9d3c
	.set sub_080a9dc4, 0x080a9dc4
	.global Overlay_080a8604
Overlay_080a8604:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r2, [sp, #24]
	str	r1, [sp, #28]
	ldr	r3, [pc, #700]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	movs	r0, #1
	negs	r0, r0
	mov	r8, r3
	bl	sub_08077290
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	ldr	r0, [sp, #28]
	str	r3, [sp, #12]
	bl	sub_08077008
	ldr	r2, [sp, #24]
	movs	r3, #255
	ands	r3, r2
	movs	r2, #7
	str	r0, [sp, #20]
	str	r2, [sp, #16]
	cmp	r3, #1
	beq.n	.L_080a864a
	movs	r3, #10
	str	r3, [sp, #16]
.L_080a864a:
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	adds	r0, r7, #0
	bl	sub_080a8914
	add	r5, sp, #32
	ldr	r2, [sp, #28]
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_080a8b10
	adds	r0, r5, #0
	bl	sub_080a9dc4
	movs	r6, #128
	ldr	r2, [sp, #24]
	lsls	r6, r6, #1
	ands	r6, r2
	cmp	r6, #0
	bne.n	.L_080a868c
	movs	r3, #96
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #40
	str	r3, [sp, #0]
	bl	sub_08015068
.L_080a868c:
	movs	r3, #0
	mov	sl, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080a86a6
	movs	r2, #16
	ldr	r0, [pc, #576]
	adds	r1, r7, #0
	movs	r3, #40
	bl	sub_08015080
	movs	r2, #1
	mov	sl, r2
.L_080a86a6:
	ldrb	r3, [r5, #1]
	cmp	r3, #0
	beq.n	.L_080a86c0
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #556]
	adds	r1, r7, #0
	movs	r2, #16
	bl	sub_08015080
	movs	r3, #1
	add	sl, r3
.L_080a86c0:
	ldrb	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L_080a86da
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #532]
	adds	r1, r7, #0
	movs	r2, #16
	bl	sub_08015080
	movs	r3, #1
	add	sl, r3
.L_080a86da:
	ldrb	r3, [r5, #3]
	cmp	r3, #0
	beq.n	.L_080a86f4
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #512]
	adds	r1, r7, #0
	movs	r2, #16
	bl	sub_08015080
	movs	r3, #1
	add	sl, r3
.L_080a86f4:
	ldrb	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_080a870e
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #488]
	adds	r1, r7, #0
	movs	r2, #16
	bl	sub_08015080
	movs	r3, #1
	add	sl, r3
.L_080a870e:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080a8720
	ldr	r0, [pc, #472]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #40
	bl	sub_08015080
.L_080a8720:
	adds	r0, r5, #0
	bl	sub_080a9dc4
	adds	r0, r5, #0
	bl	sub_080a9d3c
	movs	r3, #136
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_080a873a
	b.n	.L_080a88c6
.L_080a873a:
	cmp	r6, #0
	bne.n	.L_080a8754
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #96
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #64
	movs	r2, #56
	movs	r3, #224
	bl	sub_08015068
.L_080a8754:
	movs	r0, #15
	bl	sub_080150b8
	ldr	r3, [sp, #24]
	cmp	r3, #1
	beq.n	.L_080a8766
	ldr	r2, [sp, #12]
	cmp	r2, #1
	bne.n	.L_080a87a0
.L_080a8766:
	movs	r5, #4
	ldr	r3, [sp, #16]
	adds	r0, r7, #0
	movs	r1, #1
	movs	r2, #15
	str	r5, [sp, #0]
	bl	sub_08015280
	adds	r0, r7, #0
	ldr	r3, [sp, #16]
	movs	r1, #2
	movs	r2, #19
	str	r5, [sp, #0]
	bl	sub_08015280
	adds	r0, r7, #0
	movs	r1, #3
	movs	r2, #23
	ldr	r3, [sp, #16]
	str	r5, [sp, #0]
	bl	sub_08015280
	adds	r0, r7, #0
	movs	r1, #4
	movs	r2, #27
	ldr	r3, [sp, #16]
	str	r5, [sp, #0]
	bl	sub_08015280
.L_080a87a0:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_080a87b8
	ldr	r2, [sp, #16]
	lsls	r6, r2, #3
	adds	r3, r6, #0
	ldr	r0, [pc, #324]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08015080
.L_080a87b8:
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne.n	.L_080a87fc
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_080a87ca
	ldr	r3, [sp, #16]
	subs	r3, #1
	str	r3, [sp, #16]
.L_080a87ca:
	ldr	r2, [sp, #16]
	lsls	r6, r2, #3
	adds	r3, r6, #0
	ldr	r0, [pc, #292]
	adds	r3, #16
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08015090
	ldr	r5, [pc, #284]
	adds	r3, r6, #0
	adds	r0, r5, #0
	adds	r3, #24
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08015080
	adds	r5, #1
	adds	r3, r6, #0
	adds	r3, #32
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08015080
.L_080a87fc:
	ldr	r2, [sp, #16]
	movs	r3, #0
	lsls	r2, r2, #3
	mov	sl, r3
	ldr	r3, [sp, #20]
	str	r2, [sp, #8]
	adds	r2, #8
	adds	r3, #72
	str	r2, [sp, #4]
	movs	r2, #104
	mov	r8, r3
	mov	fp, r2
	movs	r3, #120
	ldr	r2, [sp, #20]
	mov	r9, r3
	adds	r3, #160
	adds	r5, r2, r3
.L_080a881e:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_080a8834
	ldr	r3, [sp, #4]
	ldrb	r0, [r5, #0]
	movs	r1, #1
	str	r3, [sp, #0]
	adds	r2, r7, #0
	mov	r3, r9
	bl	sub_080150b0
.L_080a8834:
	ldr	r2, [sp, #24]
	movs	r3, #255
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080a88b0
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_080a8866
	ldr	r2, [sp, #4]
	ldrb	r0, [r5, #4]
	movs	r1, #1
	str	r2, [sp, #0]
	mov	r3, fp
	adds	r2, r7, #0
	ldr	r6, [sp, #8]
	bl	sub_080150b0
	mov	r2, r9
	subs	r2, #8
	ldr	r0, [pc, #164]
	adds	r1, r7, #0
	ldr	r3, [sp, #4]
	bl	sub_08015098
	b.n	.L_080a886a
.L_080a8866:
	ldr	r3, [sp, #16]
	lsls	r6, r3, #3
.L_080a886a:
	ldr	r0, [sp, #28]
	mov	r1, sl
	bl	sub_080771f8
	adds	r2, r6, #0
	adds	r2, #16
	mov	r3, r9
	str	r2, [sp, #0]
	subs	r3, #8
	adds	r2, r7, #0
	movs	r1, #2
	bl	sub_080150b0
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	adds	r3, r6, #0
	adds	r3, #24
	adds	r2, r7, #0
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r3, fp
	bl	sub_080150b0
	mov	r3, r8
	movs	r2, #2
	ldrsh	r0, [r3, r2]
	adds	r3, r6, #0
	adds	r3, #32
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r2, r7, #0
	mov	r3, fp
	bl	sub_080150b0
.L_080a88b0:
	movs	r2, #4
	add	r8, r2
	movs	r2, #1
	movs	r3, #32
	add	sl, r2
	add	fp, r3
	add	r9, r3
	mov	r3, sl
	adds	r5, #1
	cmp	r3, #3
	ble.n	.L_080a881e
.L_080a88c6:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000bd5
	.4byte 0x00000bd6
	.4byte 0x00000bd7
	.4byte 0x00000bd8
	.4byte 0x00000bd9
	.4byte 0x00000bd4
	.4byte 0x00000afd
	.4byte 0x080af22c
	.4byte 0x00000afe
	.4byte 0x080af230
