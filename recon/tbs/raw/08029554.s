.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08004080, 0x08004080
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016478, 0x08016478
	.set sub_08019da8, 0x08019da8
	.set sub_08019fcc, 0x08019fcc
	.set sub_0801a2ec, 0x0801a2ec
	.set sub_0801a404, 0x0801a404
	.set sub_0801a4fc, 0x0801a4fc
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e940, 0x0801e940
	.set sub_0801ea08, 0x0801ea08
	.set sub_0801eadc, 0x0801eadc
	.global Func_08029554
	.thumb_func
Func_08029554:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	movs	r2, #1
	movs	r1, #0
	str	r2, [sp, #4]
	ldr	r3, [pc, #500]
	mov	sl, r1
	mov	fp, r1
	mov	r9, r1
	add	r1, sp, #4
	ldr	r3, [r3, #0]
	ldrh	r1, [r1, #0]
	movs	r0, #1
	strh	r1, [r3, #4]
	bl	sub_080030f8
.L_08029580:
	ldr	r2, [pc, #480]
	ldr	r3, [r2, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029596
	movs	r3, #1
	movs	r1, #1
	negs	r3, r3
	str	r1, [sp, #4]
	add	fp, r3
.L_08029596:
	ldr	r2, [pc, #460]
	ldr	r3, [r2, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080295a8
	movs	r3, #1
	str	r3, [sp, #4]
	add	fp, r3
.L_080295a8:
	ldr	r1, [pc, #440]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080295c0
	movs	r2, #1
	movs	r3, #1
	negs	r2, r2
	str	r3, [sp, #4]
	add	r9, r2
.L_080295c0:
	ldr	r1, [pc, #416]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080295d4
	movs	r2, #1
	str	r2, [sp, #4]
	add	r9, r2
.L_080295d4:
	ldr	r1, [pc, #396]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080295e2
	b.n	.L_0802973c
.L_080295e2:
	ldr	r3, [r1, #0]
	movs	r5, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080295ee
	b.n	.L_0802973c
.L_080295ee:
	ldr	r2, [sp, #4]
	cmp	r2, #0
	bne.n	.L_080295f6
	b.n	.L_08029734
.L_080295f6:
	mov	r2, fp
	movs	r3, #0
	adds	r2, #8
	str	r3, [sp, #4]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08029608
	mov	r3, fp
	adds	r3, #15
.L_08029608:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	mov	r0, r9
	subs	r2, r2, r3
	movs	r1, #3
	adds	r0, #3
	mov	fp, r2
	bl	sub_080022fc
	movs	r1, #2
	mov	r9, r0
	mov	r0, sl
	bl	sub_08016418
	movs	r1, #0
	movs	r0, #10
	movs	r2, #18
	movs	r3, #12
	str	r5, [sp, #0]
	bl	sub_080162d4
	mov	r1, r9
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L_0802963e
	ldr	r0, [pc, #300]
	b.n	.L_08029646
.L_0802963e:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_08029652
	ldr	r0, [pc, #292]
.L_08029646:
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e940
	b.n	.L_0802965e
.L_08029652:
	ldr	r0, [pc, #284]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e940
.L_0802965e:
	ldr	r0, [pc, #276]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #8
	bl	sub_0801e940
	movs	r3, #8
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	mov	r2, sl
	movs	r3, #40
	bl	sub_0801ea08
	mov	r1, fp
	lsls	r1, r1, #5
	mov	r8, r1
	movs	r2, #8
	str	r2, [sp, #0]
	mov	r0, r8
	movs	r1, #3
	mov	r2, sl
	movs	r3, #64
	bl	sub_0801ea08
	ldr	r0, [pc, #228]
	mov	r1, sl
	movs	r2, #88
	movs	r3, #8
	bl	sub_0801e940
	movs	r3, #8
	mov	r0, r8
	str	r3, [sp, #0]
	adds	r0, #31
	movs	r1, #3
	mov	r2, sl
	movs	r3, #96
	bl	sub_0801ea08
	movs	r5, #0
.L_080296b0:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	adds	r2, r5, #0
	cmp	r5, #0
	bge.n	.L_080296be
	adds	r2, r5, #7
.L_080296be:
	asrs	r2, r2, #3
	lsls	r3, r2, #3
	lsls	r2, r2, #4
	subs	r3, r5, r3
	adds	r6, r2, #0
	mov	r1, r9
	lsls	r7, r3, #4
	adds	r6, #16
	cmp	r1, #0
	bne.n	.L_080296e4
	mov	r2, r8
	adds	r0, r2, r5
	str	r1, [sp, #0]
	add	r2, sp, #12
	movs	r1, #1
	add	r3, sp, #8
	bl	sub_08019fcc
	b.n	.L_080296fc
.L_080296e4:
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_0802970e
	mov	r1, r8
	movs	r3, #0
	adds	r0, r1, r5
	str	r3, [sp, #0]
	movs	r1, #1
	add	r2, sp, #12
	add	r3, sp, #8
	bl	sub_0801a404
.L_080296fc:
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #23
	mov	r2, sl
	adds	r3, r7, #0
	str	r6, [sp, #0]
	bl	sub_0801eadc
	b.n	.L_0802972e
.L_0802970e:
	bl	sub_08004080
	movs	r1, #0
	adds	r2, r0, #0
	adds	r0, r5, #0
	str	r2, [sp, #12]
	bl	sub_0801a2ec
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #23
	mov	r2, sl
	adds	r3, r7, #0
	str	r6, [sp, #0]
	bl	sub_0801eadc
.L_0802972e:
	adds	r5, #1
	cmp	r5, #31
	ble.n	.L_080296b0
.L_08029734:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_08029580
.L_0802973c:
	mov	r0, sl
	movs	r1, #2
	bl	sub_08016418
	ldr	r3, [pc, #24]
	ldr	r2, [r3, #0]
	movs	r3, #0
	movs	r0, #0
	strh	r3, [r2, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e68
	.4byte 0x03001b04
	.4byte 0x08037440
	.4byte 0x08037448
	.4byte 0x08037450
	.4byte 0x08037458
	.2byte 0x7460
	.2byte 0x0803
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #364]
	ldr	r3, [r3, #0]
	sub	sp, #24
	movs	r0, #1
	str	r0, [sp, #8]
	movs	r1, #0
	movs	r2, #10
	mov	fp, r3
	movs	r0, #0
	movs	r3, #5
	bl	sub_08019da8
	movs	r3, #2
	movs	r2, #14
	str	r0, [sp, #12]
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r3, #3
	movs	r0, #10
	bl	sub_080162d4
	adds	r7, r0, #0
	ldr	r0, [pc, #324]
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	movs	r2, #1
	negs	r2, r2
	movs	r1, #0
	cmp	r3, r2
	beq.n	.L_080297d6
	mov	ip, r2
	adds	r2, r0, #0
.L_080297ca:
	adds	r2, #4
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	adds	r1, #1
	cmp	r3, ip
	bne.n	.L_080297ca
.L_080297d6:
	ldr	r0, [pc, #296]
	mov	r8, r1
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080297f6
	mov	ip, r2
	adds	r2, r0, #0
.L_080297ea:
	adds	r2, #4
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	adds	r1, #1
	cmp	r3, ip
	bne.n	.L_080297ea
.L_080297f6:
	add	r1, r8
	mov	sl, r1
	ldr	r6, [pc, #264]
	movs	r1, #2
	mov	r9, r1
.L_08029800:
	ldr	r3, [r6, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029810
	movs	r2, #1
	str	r2, [sp, #8]
	subs	r5, #1
.L_08029810:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029820
	movs	r3, #1
	str	r3, [sp, #8]
	adds	r5, #1
.L_08029820:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029832
	movs	r0, #1
	str	r0, [sp, #8]
	subs	r5, #10
.L_08029832:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029844
	movs	r1, #1
	str	r1, [sp, #8]
	adds	r5, #10
.L_08029844:
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080298ce
	ldr	r3, [r6, #0]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080298ce
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080298c6
	movs	r0, #0
	mov	r1, sl
	str	r0, [sp, #8]
	adds	r0, r5, r1
	bl	sub_080022fc
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08016478
	cmp	r5, r8
	bge.n	.L_08029880
	ldr	r2, [pc, #132]
	lsls	r3, r5, #2
	adds	r3, #2
	ldrsh	r0, [r2, r3]
	b.n	.L_08029890
.L_08029880:
	mov	r0, r8
	subs	r2, r5, r0
	ldr	r3, [pc, #120]
	lsls	r2, r2, #2
	adds	r2, #2
	ldrsh	r3, [r3, r2]
	adds	r0, r3, #0
	adds	r0, #128
.L_08029890:
	ldr	r1, [pc, #116]
	mov	r2, fp
	ldrh	r3, [r2, r1]
	movs	r2, #15
	str	r3, [sp, #20]
	movs	r3, #1
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	add	r2, sp, #20
	add	r3, sp, #16
	movs	r1, #0
	bl	sub_0801a4fc
	movs	r3, #0
	adds	r0, r5, #0
	movs	r1, #2
	adds	r2, r7, #0
	str	r3, [sp, #0]
	bl	sub_0801ea08
	ldr	r0, [pc, #80]
	adds	r1, r7, #0
	adds	r0, r5, r0
	movs	r2, #24
	movs	r3, #0
	bl	sub_0801e7c0
.L_080298c6:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_08029800
.L_080298ce:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [sp, #12]
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x080367e4
	.4byte 0x0803680c
	.4byte 0x03001b04
	.4byte 0x000012f2
	.4byte 0x00000dd2
