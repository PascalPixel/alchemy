.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_08004938, 0x08004938
	.set sub_080072f0, 0x080072f0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016498, 0x08016498
	.set sub_08016738, 0x08016738
	.set sub_08017248, 0x08017248
	.set sub_08017aa4, 0x08017aa4
	.set sub_08019000, 0x08019000
	.set sub_0801965c, 0x0801965c
	.set sub_08019908, 0x08019908
	.set sub_0801e318, 0x0801e318
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801ea08, 0x0801ea08
	.set sub_0801f200, 0x0801f200
	.set sub_08021ab0, 0x08021ab0
	.set sub_08021b80, 0x08021b80
	.set sub_08022768, 0x08022768
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_080771f8, 0x080771f8
	.set sub_08077258, 0x08077258
	.set sub_08077290, 0x08077290
	.set sub_080b5098, 0x080b5098
	.set sub_080f9010, 0x080f9010
	.global Func_08023178
	.thumb_func
Func_08023178:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #352
	str	r0, [sp, #92]
	str	r2, [sp, #84]
	str	r1, [sp, #88]
	ldr	r5, [pc, #360]
	movs	r3, #1
	ldr	r1, [r5, #0]
	movs	r0, #128
	movs	r2, #1
	negs	r3, r3
	lsls	r0, r0, #2
	str	r1, [sp, #80]
	mov	sl, r2
	str	r3, [sp, #68]
	bl	sub_080040b4
	movs	r4, #0
	str	r0, [sp, #52]
	ldr	r0, [sp, #68]
	str	r4, [sp, #48]
	bl	sub_08077290
	str	r0, [sp, #44]
	adds	r5, #168
	ldr	r5, [r5, #0]
	ldr	r0, [r5, #68]
	mov	r1, sl
	str	r1, [r5, #72]
	cmp	r0, #0
	beq.n	.L_080231ca
	bl	sub_08016418
	ldr	r2, [sp, #48]
	str	r2, [r5, #68]
.L_080231ca:
	movs	r0, #112
	bl	sub_080f9010
	movs	r6, #4
	movs	r2, #0
	add	r3, sp, #100
.L_080231d6:
	subs	r6, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r6, #0
	bge.n	.L_080231d6
	movs	r3, #0
	add	r2, sp, #96
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	movs	r0, #128
	bl	sub_080040b4
	movs	r3, #142
	movs	r4, #136
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	add	r3, sp
	add	r4, sp
	movs	r1, #1
	negs	r1, r1
	str	r0, [sp, #64]
	str	r3, [sp, #16]
	str	r4, [sp, #20]
	mov	r8, r1
	adds	r5, r4, #0
	adds	r7, r3, #0
	movs	r6, #10
.L_0802320e:
	movs	r0, #128
	bl	sub_080040b4
	subs	r6, #1
	mov	r2, r8
	strb	r2, [r5, #0]
	stmia	r7!, {r0}
	adds	r5, #1
	cmp	r6, #0
	bge.n	.L_0802320e
	ldr	r3, [sp, #88]
	cmp	r3, #0
	beq.n	.L_08023258
	ldr	r4, [sp, #92]
	ldrh	r3, [r4, #0]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_08023258
	cmp	r3, #254
	beq.n	.L_0802323c
	ldr	r1, [sp, #84]
	cmp	r3, r1
	beq.n	.L_08023256
.L_0802323c:
	adds	r6, #1
	cmp	r6, #5
	bgt.n	.L_08023258
	ldr	r2, [sp, #92]
	lsls	r3, r6, #1
	ldrh	r3, [r3, r2]
	cmp	r3, #255
	beq.n	.L_08023258
	cmp	r3, #254
	beq.n	.L_0802323c
	ldr	r4, [sp, #84]
	cmp	r3, r4
	bne.n	.L_0802323c
.L_08023256:
	str	r6, [sp, #68]
.L_08023258:
	movs	r3, #6
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	str	r1, [sp, #56]
	str	r1, [sp, #60]
	bl	sub_080162d4
	movs	r3, #10
	str	r0, [sp, #76]
	str	r3, [sp, #0]
	movs	r1, #14
	movs	r2, #30
	movs	r3, #6
	movs	r0, #0
	bl	sub_080162d4
	str	r0, [sp, #72]
	bl	sub_0801e318
	movs	r3, #170
	movs	r4, #164
	mov	r2, sp
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	mov	r1, sp
	adds	r2, #128
	add	r3, sp
	add	r4, sp
	adds	r1, #140
	str	r2, [sp, #28]
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	str	r1, [sp, #24]
.L_080232a0:
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #28]
	str	r3, [sp, #40]
	ldr	r3, [pc, #84]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #84]
	bl	sub_08021b80
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldr	r3, [sp, #28]
	ldrh	r2, [r3, #8]
	ldr	r3, [pc, #64]
	ldr	r4, [sp, #28]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #8]
	ldrh	r2, [r4, #6]
	ldr	r3, [pc, #56]
	ands	r3, r2
	ldr	r2, [pc, #32]
	ldr	r1, [sp, #28]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldrb	r2, [r1, #9]
	movs	r3, #24
	strb	r3, [r1, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r0, [sp, #28]
	movs	r1, #240
	b.n	.L_0802330c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x00000008
	.4byte 0x03001e8c
	.4byte 0x03001b04
	.4byte 0x80000400
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_0802330c:
	bl	sub_08003dec
	ldr	r3, [sp, #104]
	cmp	r3, #21
	bhi.n	.L_08023378
	ldr	r2, [pc, #516]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.2byte 0x339c
	.2byte 0x0802
.L_08023378:
	movs	r3, #0
	add	r2, sp, #96
	str	r3, [r2, #16]
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L_0802338a
	movs	r3, #9
	str	r3, [r2, #20]
	b.n	.L_080233a6
.L_0802338a:
	movs	r3, #7
	str	r3, [r2, #20]
	b.n	.L_080233a6
	add	r2, sp, #96
	movs	r3, #1
	str	r3, [r2, #16]
	movs	r3, #4
	str	r3, [r2, #20]
	b.n	.L_080233a6
	add	r2, sp, #96
	movs	r3, #2
	str	r3, [r2, #16]
	ldr	r4, [sp, #48]
	str	r4, [r2, #20]
.L_080233a6:
	ldr	r1, [sp, #76]
	ldr	r3, [sp, #76]
	ldrh	r0, [r1, #12]
	ldrh	r2, [r3, #8]
	movs	r4, #15
	ldrh	r1, [r1, #14]
	ldrh	r3, [r3, #10]
	str	r4, [sp, #0]
	bl	sub_08022768
	ldr	r4, [sp, #48]
	cmp	r4, #0
	bne.n	.L_080233c2
	b.n	.L_080234ce
.L_080233c2:
	add	r6, sp, #96
	ldr	r3, [r6, #16]
	cmp	r3, #1
	bhi.n	.L_08023458
	ldr	r1, [sp, #40]
	ldrsb	r5, [r6, r3]
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080233ee
	movs	r2, #0
	movs	r0, #111
	str	r2, [sp, #40]
	bl	sub_080f9010
	ldr	r3, [r6, #20]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_080234c8
	ldr	r3, [r6, #16]
	movs	r5, #0
	b.n	.L_0802340e
.L_080233ee:
	ldr	r4, [sp, #40]
	movs	r3, #64
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0802341a
	movs	r1, #0
	movs	r0, #111
	subs	r5, #1
	str	r1, [sp, #40]
	bl	sub_080f9010
	cmp	r5, #0
	bge.n	.L_080234c8
	ldr	r3, [r6, #20]
	subs	r5, r3, #1
	ldr	r3, [r6, #16]
.L_0802340e:
	cmp	r3, #1
	bne.n	.L_080234c8
	movs	r3, #2
	str	r3, [r6, #16]
	ldrsb	r5, [r6, r3]
	b.n	.L_080234c8
.L_0802341a:
	ldr	r2, [sp, #40]
	movs	r3, #49
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080234c8
	movs	r3, #0
	movs	r0, #111
	str	r3, [sp, #40]
	bl	sub_080f9010
	ldr	r1, [r6, #16]
	movs	r3, #2
	eors	r1, r3
	str	r1, [r6, #16]
	ldr	r0, [pc, #232]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023446
	ldrsb	r5, [r6, r1]
	b.n	.L_080234c8
.L_08023446:
	ldr	r2, [r0, #0]
	movs	r3, #32
	ldr	r5, [sp, #48]
	ands	r2, r3
	subs	r5, #1
	cmp	r2, #0
	bne.n	.L_080234c8
	movs	r5, #0
	b.n	.L_080234c8
.L_08023458:
	cmp	r3, #2
	bne.n	.L_080234ce
	ldrsb	r5, [r6, r3]
	ldr	r2, [r6, #20]
	cmp	r5, r2
	blt.n	.L_08023466
	subs	r5, r2, #1
.L_08023466:
	cmp	r5, #0
	bge.n	.L_08023474
	movs	r3, #0
	str	r3, [r6, #16]
	movs	r5, #0
	ldrsb	r5, [r6, r5]
	b.n	.L_080234c8
.L_08023474:
	ldr	r4, [sp, #40]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0802348c
	movs	r1, #0
	adds	r5, #1
	str	r1, [sp, #40]
	cmp	r5, r2
	blt.n	.L_080234a6
	str	r1, [r6, #16]
	b.n	.L_080234a2
.L_0802348c:
	ldr	r2, [sp, #40]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080234ae
	movs	r3, #0
	subs	r5, #1
	str	r3, [sp, #40]
	cmp	r5, #0
	bge.n	.L_080234a6
	str	r3, [r6, #16]
.L_080234a2:
	movs	r5, #0
	ldrsb	r5, [r6, r5]
.L_080234a6:
	movs	r0, #111
	bl	sub_080f9010
	b.n	.L_080234c8
.L_080234ae:
	ldr	r4, [sp, #40]
	movs	r3, #193
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080234c8
	movs	r1, #0
	str	r1, [sp, #40]
	movs	r0, #111
	str	r1, [r6, #16]
	movs	r5, #0
	ldrsb	r5, [r6, r5]
	bl	sub_080f9010
.L_080234c8:
	add	r2, sp, #96
	ldr	r3, [r2, #16]
	strb	r5, [r2, r3]
.L_080234ce:
	add	r0, sp, #96
	ldr	r2, [r0, #16]
	cmp	r2, #0
	bne.n	.L_08023512
	ldr	r3, [sp, #44]
	ldrsb	r2, [r0, r2]
	cmp	r3, #0
	bne.n	.L_080234e0
	adds	r2, #9
.L_080234e0:
	ldr	r3, [pc, #64]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	str	r3, [r0, #8]
	ldrb	r3, [r2, #1]
	str	r3, [r0, #24]
	ldrb	r3, [r2, #2]
	str	r3, [r0, #28]
	ldr	r4, [sp, #76]
	ldrb	r3, [r2, #3]
	ldrh	r0, [r4, #12]
	ldrh	r1, [r4, #14]
	adds	r0, r0, r3
	ldrb	r3, [r2, #4]
	adds	r1, r1, r3
	movs	r3, #14
	ldrb	r2, [r2, #5]
	adds	r0, #1
	str	r3, [sp, #0]
	adds	r1, #1
	movs	r3, #1
	bl	sub_08022768
	b.n	.L_08023544
.L_08023512:
	cmp	r2, #1
	bne.n	.L_0802352c
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #12]
	b.n	.L_08023534
	.4byte 0x08023320
	.4byte 0x03001b04
	.4byte 0x08037328
	.2byte 0x73a8
	.2byte 0x0803
.L_0802352c:
	cmp	r2, #2
	bne.n	.L_08023544
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #156]
.L_08023534:
	lsls	r3, r3, #2
	ldrb	r2, [r1, r3]
	adds	r3, r3, r1
	str	r2, [r0, #8]
	ldrb	r2, [r3, #1]
	ldrb	r3, [r3, #2]
	str	r2, [r0, #24]
	str	r3, [r0, #28]
.L_08023544:
	add	r2, sp, #96
	ldr	r3, [r2, #12]
	ldr	r1, [r2, #8]
	cmp	r3, r1
	beq.n	.L_08023554
	str	r1, [r2, #12]
	movs	r1, #2
	mov	sl, r1
.L_08023554:
	ldr	r3, [r2, #24]
	lsls	r3, r3, #3
	str	r3, [sp, #56]
	ldr	r3, [r2, #28]
	ldr	r0, [sp, #84]
	lsls	r3, r3, #3
	str	r3, [sp, #60]
	bl	sub_080b5098
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #8]
	ldr	r2, [r3, #80]
	ldr	r3, [pc, #100]
	movs	r7, #0
	str	r3, [r4, #4]
	str	r7, [r4, #8]
	ldr	r1, [pc, #96]
	ldrh	r2, [r2, #8]
	ldr	r5, [pc, #96]
	mov	r8, r1
	ldrh	r1, [r4, #8]
	adds	r3, r5, #0
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #8]
	strh	r3, [r2, #8]
	ldr	r3, [sp, #8]
	ldr	r6, [pc, #80]
	ldrh	r2, [r3, #6]
	adds	r3, r6, #0
	ands	r3, r2
	ldr	r2, [pc, #52]
	ldr	r4, [sp, #8]
	orrs	r3, r2
	strh	r3, [r4, #6]
	movs	r3, #56
	strb	r3, [r4, #4]
	ldr	r0, [sp, #8]
	movs	r1, #240
	bl	sub_08003dec
	ldr	r3, [pc, #56]
	ldr	r1, [sp, #12]
	str	r3, [r1, #4]
	str	r7, [r1, #8]
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #48]
	bl	sub_080040d0
	ldr	r4, [sp, #12]
	ldrh	r3, [r4, #8]
	mov	r2, r8
	ands	r2, r0
	ands	r5, r3
	adds	r1, r4, #0
	orrs	r5, r2
	strh	r5, [r1, #8]
	b.n	.L_080235ec
	.4byte 0x000000ac
	.4byte 0x080373b8
	.4byte 0xc0002400
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.4byte 0x40000400
	.2byte 0x10a4
	.2byte 0x0803
.L_080235ec:
	ldr	r3, [sp, #76]
	ldr	r0, [pc, #84]
	ldrh	r2, [r3, #12]
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #56]
	movs	r1, #4
	ands	r3, r1
	lsls	r2, r2, #3
	adds	r2, r4, r2
	lsrs	r3, r3, #2
	ldr	r4, [sp, #12]
	subs	r2, r2, r3
	ldr	r3, [pc, #56]
	adds	r2, #16
	ands	r2, r3
	ldrh	r3, [r4, #6]
	ands	r6, r3
	orrs	r6, r2
	adds	r2, r4, #0
	strh	r6, [r2, #6]
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #14]
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #60]
	ands	r3, r1
	lsls	r2, r2, #3
	adds	r2, r4, r2
	lsrs	r3, r3, #2
	ldr	r1, [sp, #12]
	subs	r2, r2, r3
	adds	r2, #16
	strb	r2, [r1, #4]
	movs	r3, #63
	ldrb	r2, [r1, #7]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #16
	orrs	r3, r2
	strb	r3, [r1, #7]
	ldr	r0, [sp, #12]
	b.n	.L_08023648
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0x1e40
	.2byte 0x0300
.L_08023648:
	movs	r1, #241
	bl	sub_08003dec
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_08023656
	b.n	.L_08023cfa
.L_08023656:
	ldr	r0, [sp, #84]
	bl	sub_08077008
	mov	r9, r0
	bl	sub_0801e318
	bl	sub_08016738
	movs	r3, #1
	mov	r4, sl
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_08023672
	b.n	.L_08023b42
.L_08023672:
	ldr	r0, [sp, #76]
	bl	sub_08016498
	ldr	r1, [sp, #72]
	ldr	r3, [sp, #72]
	ldrh	r0, [r1, #12]
	ldrh	r2, [r3, #8]
	ldrh	r1, [r1, #14]
	ldrh	r3, [r3, #10]
	str	r7, [sp, #0]
	bl	sub_08017248
	movs	r3, #14
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #29
	bl	sub_0801e41c
	mov	r0, r9
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e8b0
	ldr	r0, [pc, #616]
	ldr	r1, [sp, #76]
	movs	r2, #56
	movs	r3, #0
	bl	sub_0801e8b0
	mov	r4, r9
	ldr	r2, [sp, #76]
	ldrb	r0, [r4, #15]
	movs	r1, #2
	movs	r3, #72
	str	r7, [sp, #0]
	bl	sub_0801ea08
	ldr	r7, [pc, #592]
	ldr	r1, [sp, #76]
	adds	r0, r7, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_0801e7c0
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	movs	r3, #8
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #8
	movs	r3, #40
	bl	sub_0801ea08
	ldr	r0, [pc, #560]
	ldr	r1, [sp, #76]
	movs	r2, #40
	movs	r3, #24
	bl	sub_0801e8b0
	movs	r3, #24
	mov	r2, r9
	movs	r1, #56
	ldrsh	r0, [r2, r1]
	mov	r8, r3
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #4
	movs	r3, #56
	bl	sub_0801ea08
	ldr	r5, [pc, #528]
	ldr	r1, [sp, #76]
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #24
	bl	sub_0801e8b0
	mov	r1, r9
	mov	r2, r8
	movs	r4, #52
	ldrsh	r0, [r1, r4]
	movs	r3, #96
	str	r2, [sp, #0]
	movs	r1, #4
	ldr	r2, [sp, #76]
	bl	sub_0801ea08
	movs	r3, #32
	ldr	r1, [sp, #76]
	ldr	r0, [pc, #496]
	movs	r2, #40
	bl	sub_0801e8b0
	mov	r4, r9
	ldr	r2, [sp, #76]
	movs	r3, #58
	ldrsh	r0, [r4, r3]
	movs	r6, #32
	movs	r1, #4
	movs	r3, #56
	str	r6, [sp, #0]
	bl	sub_0801ea08
	adds	r0, r5, #0
	ldr	r1, [sp, #76]
	movs	r2, #88
	movs	r3, #32
	bl	sub_0801e8b0
	mov	r2, r9
	movs	r1, #54
	ldrsh	r0, [r2, r1]
	movs	r3, #96
	ldr	r2, [sp, #76]
	movs	r1, #4
	str	r6, [sp, #0]
	bl	sub_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #10
	movs	r2, #136
	movs	r3, #16
	bl	sub_0801e7c0
	mov	r3, r9
	ldrh	r0, [r3, #60]
	movs	r3, #16
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #3
	movs	r3, #184
	bl	sub_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #9
	movs	r2, #136
	movs	r3, #24
	bl	sub_0801e7c0
	mov	r4, r9
	mov	r1, r8
	ldrh	r0, [r4, #62]
	ldr	r2, [sp, #76]
	str	r1, [sp, #0]
	movs	r3, #184
	movs	r1, #3
	bl	sub_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #8
	movs	r2, #136
	movs	r3, #32
	bl	sub_0801e7c0
	mov	r3, r9
	adds	r3, #64
	ldr	r2, [sp, #76]
	ldrh	r0, [r3, #0]
	movs	r1, #3
	movs	r3, #184
	str	r6, [sp, #0]
	bl	sub_0801ea08
	subs	r0, r7, #7
	ldr	r1, [sp, #76]
	movs	r2, #136
	movs	r3, #40
	bl	sub_0801e7c0
	mov	r3, r9
	adds	r3, #66
	ldrb	r0, [r3, #0]
	movs	r3, #40
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #3
	movs	r3, #184
	bl	sub_0801ea08
	ldr	r3, [pc, #312]
	add	r3, r9
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #312]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [sp, #76]
	movs	r3, #48
	bl	sub_0801e7c0
	ldr	r2, [sp, #44]
	cmp	r2, #0
	beq.n	.L_0802380e
	subs	r0, r7, #1
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #72
	bl	sub_0801e7c0
.L_0802380e:
	ldr	r1, [sp, #76]
	subs	r0, r7, #5
	movs	r2, #0
	movs	r3, #80
	bl	sub_0801e7c0
	subs	r0, r7, #4
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #88
	bl	sub_0801e7c0
	movs	r6, #140
	subs	r0, r7, #3
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #96
	bl	sub_0801e7c0
	lsls	r6, r6, #1
	movs	r3, #72
	movs	r1, #40
	movs	r2, #7
	movs	r4, #0
	mov	fp, r3
	mov	r8, r1
	add	r6, r9
	movs	r7, #48
	mov	sl, r2
.L_08023848:
	ldr	r1, [sp, #44]
	movs	r3, #1
	cmp	r1, #0
	beq.n	.L_08023852
	movs	r3, #0
.L_08023852:
	ldr	r2, [pc, #216]
	adds	r1, r4, r2
	movs	r2, #0
	adds	r3, #8
	str	r2, [sp, #0]
	ldr	r0, [sp, #76]
	mov	r2, sl
	lsls	r5, r4, #2
	str	r4, [sp, #4]
	bl	sub_08019000
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L_080238a0
	mov	r1, fp
	ldr	r2, [sp, #76]
	ldrb	r0, [r6, #4]
	mov	r3, r8
	str	r1, [sp, #0]
	movs	r1, #1
	bl	sub_0801ea08
	ldr	r0, [pc, #152]
	ldr	r1, [sp, #76]
	adds	r2, r7, #0
	movs	r3, #72
	bl	sub_0801e8b0
	mov	r2, fp
	adds	r3, r7, #0
	ldrb	r0, [r6, #0]
	adds	r3, #8
	str	r2, [sp, #0]
	movs	r1, #1
	ldr	r2, [sp, #76]
	bl	sub_0801ea08
	ldr	r4, [sp, #4]
.L_080238a0:
	adds	r1, r4, #0
	ldr	r0, [sp, #84]
	str	r4, [sp, #4]
	bl	sub_080771f8
	movs	r3, #80
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #2
	adds	r3, r7, #0
	bl	sub_0801ea08
	adds	r5, #72
	mov	r3, r9
	ldrsh	r0, [r3, r5]
	movs	r3, #88
	ldr	r2, [sp, #76]
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r3, r8
	bl	sub_0801ea08
	add	r5, r9
	movs	r3, #96
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #3
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	mov	r3, r8
	bl	sub_0801ea08
	ldr	r4, [sp, #4]
	movs	r3, #32
	movs	r1, #4
	adds	r4, #1
	add	r8, r3
	adds	r6, #1
	adds	r7, #32
	add	sl, r1
	cmp	r4, #3
	ble.n	.L_08023848
	mov	r4, r9
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_08023908
	ldr	r1, [sp, #20]
	movs	r3, #16
	strb	r3, [r1, #0]
	movs	r6, #1
.L_08023908:
	ldr	r3, [sp, #20]
	adds	r2, r6, r3
	b.n	.L_08023aa0
	movs	r0, r0
	.4byte 0x080373d8
	.4byte 0x000008ba
	.4byte 0x080373dc
	.4byte 0x080373e0
	.4byte 0x080373e4
	.4byte 0x00000129
	.4byte 0x00000741
	.2byte 0x5001
	.2byte 0x0000
.L_08023930:
	ldr	r1, [pc, #812]
	add	r1, r9
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_08023942
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023942:
	cmp	r6, #7
	ble.n	.L_08023948
	b.n	.L_08023ad4
.L_08023948:
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #2
	bne.n	.L_08023956
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023956:
	cmp	r6, #7
	ble.n	.L_0802395c
	b.n	.L_08023ad4
.L_0802395c:
	ldr	r3, [pc, #772]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802396e
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_0802396e:
	cmp	r6, #7
	ble.n	.L_08023974
	b.n	.L_08023ad4
.L_08023974:
	ldr	r3, [pc, #752]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023986
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023986:
	cmp	r6, #7
	ble.n	.L_0802398c
	b.n	.L_08023ad4
.L_0802398c:
	movs	r3, #158
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080239a0
	movs	r3, #5
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239a0:
	cmp	r6, #7
	ble.n	.L_080239a6
	b.n	.L_08023ad4
.L_080239a6:
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080239ba
	movs	r3, #7
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239ba:
	cmp	r6, #7
	ble.n	.L_080239c0
	b.n	.L_08023ad4
.L_080239c0:
	movs	r3, #156
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080239d4
	movs	r3, #6
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239d4:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	movs	r3, #153
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023a0a
	ldr	r3, [pc, #644]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080239fc
	movs	r3, #9
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239fc:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023a0a
	movs	r3, #10
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a0a:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	movs	r3, #154
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023a40
	ldr	r3, [pc, #596]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023a32
	movs	r3, #11
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a32:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023a40
	movs	r3, #12
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a40:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	movs	r3, #155
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023a76
	ldr	r3, [pc, #544]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023a68
	movs	r3, #13
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a68:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023a76
	movs	r3, #14
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a76:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	ldr	r3, [pc, #508]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023a92
	movs	r3, #17
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a92:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023ad4
	movs	r3, #18
	strb	r3, [r2, #0]
	adds	r6, #1
	b.n	.L_08023ad4
.L_08023aa0:
	movs	r3, #152
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08023ab8
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023ab8:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	ldr	r3, [pc, #444]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023ace
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023ace:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	b.n	.L_08023930
.L_08023ad4:
	cmp	r6, #0
	ble.n	.L_08023af6
	ldr	r7, [sp, #16]
	ldr	r5, [sp, #20]
	adds	r4, r6, #0
.L_08023ade:
	ldrb	r0, [r5, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	ldmia	r7!, {r1}
	str	r4, [sp, #4]
	bl	sub_08021ab0
	ldr	r4, [sp, #4]
	subs	r4, #1
	adds	r5, #1
	cmp	r4, #0
	bne.n	.L_08023ade
.L_08023af6:
	cmp	r6, #0
	bne.n	.L_08023b00
	ldr	r4, [sp, #20]
	strb	r6, [r4, #0]
	movs	r6, #1
.L_08023b00:
	cmp	r6, #10
	bgt.n	.L_08023b1c
	ldr	r3, [sp, #20]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r2, #0
	adds	r2, r6, r3
	movs	r3, #11
	subs	r4, r3, r6
.L_08023b12:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_08023b12
.L_08023b1c:
	str	r6, [sp, #48]
	ldr	r4, [sp, #20]
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L_08023b42
	mov	r2, r9
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_08023b36
	ldr	r0, [pc, #332]
	b.n	.L_08023b38
.L_08023b36:
	ldr	r0, [pc, #332]
.L_08023b38:
	ldr	r1, [sp, #76]
	movs	r2, #112
	movs	r3, #0
	bl	sub_0801e7c0
.L_08023b42:
	movs	r0, #128
	lsls	r0, r0, #1
	bl	sub_08004938
	ldr	r3, [sp, #104]
	str	r0, [sp, #36]
	cmp	r3, #13
	bhi.n	.L_08023b54
	b.n	.L_08023c94
.L_08023b54:
	ldr	r4, [sp, #20]
	subs	r3, #14
	ldrsb	r3, [r4, r3]
	str	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_08023b6e
	mov	r2, r9
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_08023b6e
	movs	r3, #16
	str	r3, [sp, #32]
.L_08023b6e:
	movs	r4, #166
	lsls	r4, r4, #1
	mov	r8, r4
	mov	r0, r8
	bl	sub_08004938
	movs	r4, #0
	str	r4, [sp, #4]
	mov	r1, r9
	mov	r2, r8
	ldr	r3, [pc, #260]
	adds	r6, r0, #0
	bl	sub_080072f0
	ldr	r3, [pc, #224]
	ldr	r4, [sp, #4]
	mov	r5, r9
	adds	r5, #64
	add	r3, r9
	mov	r1, r9
	mov	r2, r9
	ldrh	r1, [r1, #60]
	ldrh	r2, [r2, #62]
	ldrh	r7, [r5, #0]
	strb	r4, [r3, #0]
	ldr	r3, [pc, #204]
	add	r3, r9
	strb	r4, [r3, #0]
	ldr	r3, [pc, #208]
	add	r3, r9
	strb	r4, [r3, #0]
	ldr	r0, [sp, #84]
	mov	sl, r1
	mov	fp, r2
	bl	sub_08077010
	mov	r1, r9
	ldrh	r3, [r1, #60]
	mov	r2, sl
	subs	r2, r2, r3
	ldrh	r3, [r1, #62]
	mov	r1, fp
	subs	r1, r1, r3
	ldrh	r3, [r5, #0]
	mov	sl, r2
	mov	fp, r1
	mov	r2, r8
	adds	r1, r6, #0
	subs	r7, r7, r3
	mov	r0, r9
	ldr	r3, [pc, #180]
	bl	sub_080072f0
	adds	r0, r6, #0
	bl	sub_08002df0
	ldr	r3, [sp, #32]
	subs	r3, #8
	ldr	r4, [sp, #4]
	cmp	r3, #10
	bhi.n	.L_08023c4e
	ldr	r2, [pc, #160]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08023c1c
	.4byte 0x08023c24
	.4byte 0x08023c28
	.4byte 0x08023c2e
	.4byte 0x08023c32
	.4byte 0x08023c40
	.4byte 0x08023c40
	.4byte 0x08023c4e
	.4byte 0x08023c4e
	.4byte 0x08023c38
	.2byte 0x3c3c
	.2byte 0x0802
	ldr	r3, [pc, #92]
	add	r3, r9
	ldrb	r4, [r3, #0]
	b.n	.L_08023c4e
	mov	r4, sl
	b.n	.L_08023c4e
	mov	r1, sl
	negs	r4, r1
	b.n	.L_08023c4e
	mov	r4, fp
	b.n	.L_08023c4e
	mov	r2, fp
	negs	r4, r2
	b.n	.L_08023c4e
	adds	r4, r7, #0
	b.n	.L_08023c4e
	negs	r4, r7
	b.n	.L_08023c4e
	ldr	r3, [pc, #48]
	add	r3, r9
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r4, r3, #2
.L_08023c4e:
	adds	r0, r4, #0
	movs	r1, #5
	bl	sub_08019908
	ldr	r0, [pc, #56]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #36]
	adds	r0, r3, r0
	b.n	.L_08023cbe
	.4byte 0x00000131
	.4byte 0x0000013d
	.4byte 0x0000013b
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000147
	.4byte 0x00000141
	.4byte 0x000008bd
	.4byte 0x000008be
	.4byte 0x03001388
	.4byte 0x08023bf0
	.2byte 0x08d2
	.2byte 0x0000
.L_08023c94:
	cmp	r3, #2
	bne.n	.L_08023ccc
	mov	r4, r9
	ldrb	r3, [r4, #15]
	cmp	r3, #98
	bhi.n	.L_08023ccc
	adds	r1, r3, #0
	adds	r1, #1
	ldr	r0, [sp, #84]
	bl	sub_08077258
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r1, #5
	subs	r0, r0, r3
	bl	sub_08019908
	ldr	r0, [pc, #12]
	ldr	r1, [sp, #36]
.L_08023cbe:
	movs	r2, #128
	bl	sub_0801965c
	b.n	.L_08023cda
	movs	r0, r0
	.2byte 0x08bf
	.2byte 0x0000
.L_08023ccc:
	ldr	r0, [sp, #104]
	ldr	r3, [pc, #128]
	ldr	r1, [sp, #36]
	adds	r0, r0, r3
	movs	r2, #128
	bl	sub_0801965c
.L_08023cda:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #4
	bl	sub_08017aa4
	ldr	r0, [sp, #36]
	bl	sub_08002df0
	ldr	r4, [pc, #100]
	ldr	r3, [sp, #80]
	adds	r2, r3, r4
	movs	r3, #1
	movs	r1, #0
	strb	r3, [r2, #0]
	mov	sl, r1
.L_08023cfa:
	ldr	r1, [pc, #92]
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #16]
	movs	r6, #0
	mov	r8, r1
	movs	r4, #112
.L_08023d06:
	ldr	r3, [pc, #84]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	ldmia	r7!, {r3}
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r2, [r3, #2]
	ldr	r1, [pc, #44]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r3, [pc, #32]
	adds	r1, r4, #0
	ands	r1, r3
	ldr	r2, [pc, #32]
	ldrh	r3, [r5, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #6]
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #14]
	lsls	r3, r3, #3
	adds	r3, #8
	strb	r3, [r5, #4]
	ldr	r1, [sp, #20]
	ldrsb	r3, [r1, r6]
	b.n	.L_08023d60
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000008c0
	.4byte 0x00000ea3
	.4byte 0x03001b10
	.2byte 0x0400
	.2byte 0x4000
.L_08023d60:
	cmp	r3, #0
	ble.n	.L_08023d70
	adds	r0, r5, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	sub_08003dec
	ldr	r4, [sp, #4]
.L_08023d70:
	adds	r6, #1
	adds	r4, #15
	adds	r5, #12
	cmp	r6, #10
	ble.n	.L_08023d06
	ldr	r3, [pc, #232]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_08023e00
	ldr	r3, [pc, #224]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023e00
	ldr	r2, [sp, #88]
	cmp	r2, #0
	beq.n	.L_08023df6
	movs	r3, #128
	ldr	r4, [sp, #40]
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08023dc6
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	cmp	r1, r2
	blt.n	.L_08023db0
	movs	r2, #0
	str	r2, [sp, #68]
.L_08023db0:
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #92]
	lsls	r3, r4, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #84]
	mov	sl, r2
	bl	sub_080f9010
	b.n	.L_08023df6
.L_08023dc6:
	movs	r3, #128
	ldr	r4, [sp, #40]
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08023df6
	ldr	r1, [sp, #68]
	subs	r1, #1
	str	r1, [sp, #68]
	cmp	r1, #0
	bge.n	.L_08023de2
	ldr	r2, [sp, #88]
	subs	r2, #1
	str	r2, [sp, #68]
.L_08023de2:
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #92]
	lsls	r3, r4, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #84]
	mov	sl, r2
	bl	sub_080f9010
.L_08023df6:
	movs	r0, #1
	bl	sub_080030f8
	bl	.L_080232a0
.L_08023e00:
	ldr	r5, [sp, #16]
	movs	r6, #10
.L_08023e04:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	sub_08003f3c
	cmp	r6, #0
	bge.n	.L_08023e04
	ldr	r0, [sp, #52]
	bl	sub_08003f3c
	ldr	r0, [sp, #64]
	bl	sub_08003f3c
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_0801e318
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #72]
	bl	sub_08016418
	ldr	r5, [pc, #52]
	ldr	r3, [r5, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	adds	r5, #192
	bl	sub_0801f200
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r3, [r2, #72]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	add	sp, #352
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x03001e74
