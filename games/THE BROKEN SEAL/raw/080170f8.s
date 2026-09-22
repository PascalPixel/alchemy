.syntax unified
	.thumb
	.set sub_080170c4, 0x080170c4
	.set sub_0801e260, 0x0801e260
	.global Overlay_080170f8
Overlay_080170f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r3, #0
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	mov	fp, r3
	lsls	r3, r1, #5
	adds	r3, r3, r0
	mov	r8, r2
	lsls	r3, r3, #1
	mov	r2, fp
	adds	r5, r3, r2
	mov	r3, r8
	cmp	r3, #1
	bhi.n	.L_08017122
	b.n	.L_08017230
.L_08017122:
	cmp	r7, #1
	bhi.n	.L_08017128
	b.n	.L_08017230
.L_08017128:
	cmp	r3, #30
	bls.n	.L_0801712e
	b.n	.L_08017230
.L_0801712e:
	cmp	r7, #30
	bls.n	.L_08017134
	b.n	.L_08017230
.L_08017134:
	adds	r3, r7, #0
	mov	r2, r8
	bl	sub_0801e260
	ldr	r3, [pc, #64]
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801714a
	ldr	r3, [pc, #40]
	b.n	.L_0801714c
.L_0801714a:
	ldr	r3, [pc, #40]
.L_0801714c:
	strh	r3, [r5, #0]
	adds	r5, #2
	movs	r2, #2
	negs	r2, r2
	add	r2, r8
	adds	r0, r5, #0
	ldr	r1, [pc, #40]
	mov	sl, r2
	bl	sub_080170c4
	ldr	r3, [pc, #28]
	add	r3, fp
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_08017188
	ldr	r3, [pc, #8]
	b.n	.L_0801718a
	.4byte 0x0000f01c
	.4byte 0x0000f010
	.4byte 0x0000f41c
	.4byte 0x03001e8c
	.4byte 0x00000ea4
	.2byte 0xf011
	.2byte 0xf011
.L_08017188:
	ldr	r3, [pc, #48]
.L_0801718a:
	strh	r3, [r5, #0]
	adds	r5, #2
	movs	r3, #32
	mov	r2, r8
	subs	r3, r3, r2
	lsls	r3, r3, #1
	movs	r6, #1
	subs	r7, #1
	adds	r5, r5, r3
	cmp	r6, r7
	bcs.n	.L_080171d8
	mov	r9, r3
.L_080171a2:
	ldr	r3, [pc, #28]
	mov	r2, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r2, #2
	beq.n	.L_080171ca
	adds	r0, r5, #0
	ldr	r1, [pc, #16]
	mov	r2, sl
	bl	sub_080170c4
	b.n	.L_080171c8
	movs	r0, r0
	.4byte 0x0000f012
	.4byte 0x0000f016
	.2byte 0xf020
	.2byte 0xf020
.L_080171c8:
	adds	r5, r0, #0
.L_080171ca:
	ldr	r3, [pc, #28]
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	add	r5, r9
	cmp	r6, r7
	bcc.n	.L_080171a2
.L_080171d8:
	ldr	r3, [pc, #20]
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080171f4
	ldr	r3, [pc, #8]
	b.n	.L_080171f6
	movs	r0, r0
	.4byte 0x0000f017
	.4byte 0x0000f81c
	.2byte 0x0ea4
	.2byte 0x0000
.L_080171f4:
	ldr	r3, [pc, #28]
.L_080171f6:
	strh	r3, [r5, #0]
	adds	r5, #2
	adds	r0, r5, #0
	ldr	r1, [pc, #28]
	mov	r2, sl
	bl	sub_080170c4
	ldr	r3, [pc, #24]
	add	r3, fp
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_08017224
	ldr	r3, [pc, #4]
	b.n	.L_08017226
	.4byte 0x0000f013
	.4byte 0x0000fc1c
	.4byte 0xf014f014
	.2byte 0x0ea4
	.2byte 0x0000
.L_08017224:
	ldr	r3, [pc, #24]
.L_08017226:
	strh	r3, [r5, #0]
	ldr	r2, [pc, #24]
	movs	r3, #1
	add	r2, fp
	strb	r3, [r2, #0]
.L_08017230:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000f015
	.2byte 0x0ea3
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	lr, r3
	ldr	r3, [pc, #132]
	lsls	r1, r1, #5
	ldr	r3, [r3, #0]
	adds	r1, r1, r0
	sub	sp, #4
	lsls	r1, r1, #1
	adds	r6, r2, #0
	ldr	r4, [sp, #36]
	str	r3, [sp, #0]
	adds	r1, r1, r3
	cmp	r6, #1
	bls.n	.L_08017350
	mov	r2, lr
	cmp	r2, #1
	bls.n	.L_08017350
	cmp	r6, #30
	bhi.n	.L_08017350
	cmp	r2, #30
	bhi.n	.L_08017350
	adds	r1, #64
	cmp	r4, #0
	bne.n	.L_080172e8
	movs	r3, #1
	negs	r3, r3
	add	r3, lr
	movs	r5, #1
	mov	r8, r3
	cmp	r5, r8
	bcs.n	.L_08017334
	movs	r3, #32
	subs	r3, r3, r6
	lsls	r3, r3, #1
	subs	r2, r6, #1
	mov	sl, r3
	ldr	r3, [pc, #68]
	mov	ip, r2
	mov	r9, ip
	mov	fp, r3
.L_080172a4:
	movs	r4, #1
	adds	r1, #2
	cmp	r4, r9
	bcs.n	.L_080172cc
	mov	r3, lr
	subs	r3, #2
	adds	r2, r3, #0
	ldr	r7, [pc, #36]
	mov	r3, fp
	ldr	r6, [pc, #36]
	adds	r0, r3, r5
.L_080172ba:
	adds	r3, r0, #0
	ands	r3, r7
	orrs	r3, r6
	adds	r4, #1
	strh	r3, [r1, #0]
	adds	r0, r0, r2
	adds	r1, #2
	cmp	r4, ip
	bcc.n	.L_080172ba
.L_080172cc:
	adds	r1, #2
	adds	r5, #1
	add	r1, sl
	cmp	r5, r8
	bcc.n	.L_080172a4
	b.n	.L_08017334
	.4byte 0x00000fff
	.4byte 0x0000f000
	.4byte 0x03001e8c
	.2byte 0x0127
	.2byte 0x0000
.L_080172e8:
	mov	r0, lr
	movs	r5, #1
	subs	r0, #1
	cmp	r5, r0
	bcs.n	.L_08017334
	movs	r3, #32
	subs	r3, r3, r6
	ldr	r2, [pc, #76]
	lsls	r3, r3, #1
	mov	sl, r3
	mov	r9, r2
	mov	r8, r0
.L_08017300:
	movs	r4, #0
	cmp	r4, r6
	bcs.n	.L_0801732c
	movs	r3, #2
	negs	r3, r3
	add	r3, lr
	ldr	r2, [pc, #56]
	mov	fp, r3
	ldr	r7, [pc, #44]
	mov	r3, r9
	mov	ip, r2
	adds	r0, r5, r3
.L_08017318:
	adds	r3, r0, #0
	mov	r2, ip
	ands	r3, r2
	orrs	r3, r7
	adds	r4, #1
	strh	r3, [r1, #0]
	add	r0, fp
	adds	r1, #2
	cmp	r4, r6
	bcc.n	.L_08017318
.L_0801732c:
	adds	r5, #1
	add	r1, sl
	cmp	r5, r8
	bcc.n	.L_08017300
.L_08017334:
	ldr	r3, [sp, #0]
	ldr	r1, [pc, #20]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_08017350
	.4byte 0x0000f000
	.4byte 0x00000127
	.4byte 0x00000fff
	.2byte 0x0ea3
	.2byte 0x0000
.L_08017350:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
