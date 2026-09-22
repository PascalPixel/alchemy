.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_08039454, 0x08039454
	.set sub_0803f800, 0x0803f800
	.set sub_0803f810, 0x0803f810
	.set sub_08041fa4, 0x08041fa4
	.set sub_08042010, 0x08042010
	.set sub_080432a4, 0x080432a4
	.set sub_0804d0dc, 0x0804d0dc
	.set sub_0804d118, 0x0804d118
	.set sub_0804d16c, 0x0804d16c
	.set sub_0804d28c, 0x0804d28c
	.set sub_0804d38c, 0x0804d38c
	.set sub_0804d3e8, 0x0804d3e8
	.set sub_080ad250, 0x080ad250
	.global Overlay_0804d484
Overlay_0804d484:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #232
	ldr	r5, [r1, #0]
	movs	r1, #144
	adds	r1, r1, r5
	mov	lr, r1
	adds	r2, #2
	mov	r4, lr
	strh	r2, [r4, #0]
	adds	r2, r5, #0
	adds	r2, #146
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #148
	strh	r6, [r3, #0]
	movs	r7, #142
	adds	r7, r7, r5
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r1, #0
	sub	sp, #4
	mov	ip, r7
	cmp	r1, r3
	bge.n	.L_0804d4dc
	lsls	r3, r6, #3
	mov	r8, r3
	adds	r2, r5, #0
.L_0804d4c4:
	lsls	r3, r0, #3
	mov	r4, r8
	strh	r4, [r2, #14]
	strh	r3, [r2, #12]
	mov	r4, ip
	movs	r7, #0
	ldrsh	r3, [r4, r7]
	adds	r1, #1
	adds	r0, #3
	adds	r2, #20
	cmp	r1, r3
	blt.n	.L_0804d4c4
.L_0804d4dc:
	mov	r1, lr
	movs	r3, #2
	movs	r7, #0
	ldrsh	r2, [r1, r7]
	str	r3, [sp, #0]
	adds	r1, r6, #0
	movs	r3, #3
	bl	sub_08039260
	str	r0, [r5, #120]
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r0, #1
	negs	r0, r0
	movs	r6, #0
	bl	sub_080ad250
	cmp	r0, #0
	bne.n	.L_0804d50c
	movs	r6, #1
.L_0804d50c:
	lsls	r3, r6, #1
	adds	r3, r3, r6
	ldr	r2, [pc, #84]
	lsls	r7, r3, #1
	adds	r3, r5, r7
	ldrsb	r3, [r2, r3]
	subs	r5, r3, #1
	cmp	r5, #0
	bge.n	.L_0804d520
	movs	r5, #0
.L_0804d520:
	bl	sub_0804d0dc
	movs	r0, #1
	bl	sub_0804d38c
	cmp	r6, #0
	bne.n	.L_0804d534
	movs	r0, #15
	bl	sub_0804d38c
.L_0804d534:
	movs	r0, #2
	bl	sub_0804d38c
	movs	r0, #7
	bl	sub_0804d38c
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	cmp	r5, #0
	blt.n	.L_0804d562
	ldr	r2, [pc, #16]
	adds	r3, r5, r7
	adds	r3, #1
	ldrsb	r5, [r2, r3]
.L_0804d562:
	adds	r0, r5, #0
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805f8b3
	.2byte 0xf8a7
	.2byte 0x0805
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	sub_0803f800
	bl	sub_0804d0dc
	movs	r0, #1
	bl	sub_0804d38c
	movs	r0, #15
	bl	sub_0804d38c
	movs	r0, #2
	bl	sub_0804d38c
	movs	r0, #7
	bl	sub_0804d38c
	subs	r1, r6, #1
	adds	r0, r5, #0
	bl	sub_0804d28c
	adds	r6, r0, #0
	bl	sub_0804d118
	bl	sub_0803f810
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	movs	r6, #0
	movs	r5, #0
	bl	sub_080432a4
	cmp	r0, #0
	bge.n	.L_0804d5c0
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0804d668
.L_0804d5c0:
	cmp	r0, #0
	bne.n	.L_0804d5c8
	movs	r0, #0
	b.n	.L_0804d668
.L_0804d5c8:
	cmp	r0, #3
	bne.n	.L_0804d5d0
	movs	r6, #1
	b.n	.L_0804d5e2
.L_0804d5d0:
	cmp	r0, #103
	bne.n	.L_0804d5d8
	movs	r6, #2
	b.n	.L_0804d5e2
.L_0804d5d8:
	cmp	r0, #100
	ble.n	.L_0804d5e0
	movs	r6, #3
	b.n	.L_0804d5e2
.L_0804d5e0:
	movs	r5, #1
.L_0804d5e2:
	bl	sub_0804d0dc
	cmp	r6, #0
	beq.n	.L_0804d5ee
	cmp	r6, #3
	bne.n	.L_0804d5f4
.L_0804d5ee:
	movs	r0, #21
	bl	sub_0804d38c
.L_0804d5f4:
	cmp	r6, #1
	bhi.n	.L_0804d5fe
	movs	r0, #22
	bl	sub_0804d38c
.L_0804d5fe:
	cmp	r6, #0
	beq.n	.L_0804d606
	cmp	r6, #3
	bne.n	.L_0804d60c
.L_0804d606:
	movs	r0, #23
	bl	sub_0804d38c
.L_0804d60c:
	movs	r0, #24
	bl	sub_0804d38c
	ldr	r7, [pc, #88]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_0804d622
	movs	r0, #29
	bl	sub_0804d38c
.L_0804d622:
	ldr	r3, [pc, #76]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0804d632
	movs	r0, #31
	bl	sub_0804d38c
.L_0804d632:
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	cmp	r5, #0
	blt.n	.L_0804d658
	lsls	r3, r6, #1
	ldr	r2, [pc, #36]
	adds	r3, r3, r6
	lsls	r3, r3, #1
	adds	r3, r5, r3
	ldrsb	r5, [r2, r3]
.L_0804d658:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	bne.n	.L_0804d666
	cmp	r5, #4
	bne.n	.L_0804d666
	movs	r5, #5
.L_0804d666:
	adds	r0, r5, #0
.L_0804d668:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020036d8
	.4byte 0x02005350
	.2byte 0xf8bf
	.2byte 0x0805
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r6, [r3, #0]
	sub	sp, #4
	adds	r0, r6, #0
	adds	r5, r6, #0
	adds	r5, #140
	adds	r0, #150
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #0]
	cmp	r2, r3
	beq.n	.L_0804d74c
	strh	r1, [r0, #0]
	movs	r3, #80
	ldr	r0, [r6, #124]
	movs	r2, #40
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #128
	bl	sub_08039454
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #1
	beq.n	.L_0804d706
	cmp	r3, #1
	bgt.n	.L_0804d6be
	cmp	r3, #0
	beq.n	.L_0804d6c4
	b.n	.L_0804d74c
.L_0804d6be:
	cmp	r3, #2
	beq.n	.L_0804d730
	b.n	.L_0804d74c
.L_0804d6c4:
	ldr	r5, [pc, #136]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #40
	bl	sub_08042010
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #8
	movs	r3, #48
	bl	sub_08042010
	adds	r0, r5, #2
	ldr	r1, [r6, #124]
	movs	r2, #8
	movs	r3, #56
	bl	sub_08042010
	adds	r0, r5, #3
	ldr	r1, [r6, #124]
	movs	r2, #8
	movs	r3, #64
	adds	r5, #4
	bl	sub_08042010
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #72
	bl	sub_08042010
	b.n	.L_0804d74c
.L_0804d706:
	ldr	r5, [pc, #72]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #40
	bl	sub_08042010
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #8
	movs	r3, #48
	adds	r5, #2
	bl	sub_08042010
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #56
	bl	sub_08042010
	b.n	.L_0804d74c
.L_0804d730:
	ldr	r5, [pc, #28]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #40
	adds	r5, #1
	bl	sub_08042010
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #48
	bl	sub_08042010
.L_0804d74c:
	add	sp, #4
	pop	{r5, r6, pc}
	.2byte 0x117c
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r6, [r3, #0]
	adds	r0, r6, #0
	adds	r5, r6, #0
	adds	r5, #140
	adds	r0, #150
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #0]
	cmp	r2, r3
	beq.n	.L_0804d7ee
	strh	r1, [r0, #0]
	ldr	r0, [r6, #124]
	bl	sub_080393fc
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0804d78a
	cmp	r3, #1
	beq.n	.L_0804d7a8
	b.n	.L_0804d7d2
.L_0804d78a:
	ldr	r5, [pc, #100]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	adds	r5, #1
	bl	sub_08041fa4
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08041fa4
	b.n	.L_0804d7ee
.L_0804d7a8:
	ldr	r5, [pc, #72]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	bl	sub_08041fa4
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #0
	movs	r3, #16
	adds	r5, #2
	bl	sub_08041fa4
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #28
	bl	sub_08041fa4
	b.n	.L_0804d7ee
.L_0804d7d2:
	ldr	r5, [pc, #36]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	adds	r5, #1
	bl	sub_08041fa4
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08041fa4
.L_0804d7ee:
	pop	{r5, r6, pc}
	.4byte 0x00001171
	.4byte 0x00001173
	.2byte 0x1176
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r0
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	mov	r8, r2
	sub	sp, #4
	bl	sub_0804d0dc
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r7, [r3, #0]
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0804d830
	movs	r0, #44
	bl	sub_0804d38c
	movs	r0, #45
	bl	sub_0804d38c
	b.n	.L_0804d842
.L_0804d830:
	movs	r0, #46
	bl	sub_0804d38c
	movs	r0, #47
	bl	sub_0804d38c
	movs	r0, #48
	bl	sub_0804d38c
.L_0804d842:
	movs	r2, #0
	movs	r0, #17
	movs	r1, #7
	bl	sub_0804d3e8
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_0804d8be
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #118
	ldr	r0, [pc, #252]
	bl	sub_080145a8
	movs	r3, #255
	adds	r2, r7, #0
	lsls	r3, r3, #8
	adds	r2, #150
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r1, #0
	movs	r2, #14
	movs	r3, #4
	movs	r0, #8
	bl	sub_08039260
	ldr	r5, [pc, #220]
	adds	r6, r7, #0
	adds	r1, r0, #0
	adds	r6, #128
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #4
	bl	sub_08041fa4
	mov	r2, sl
	str	r2, [sp, #0]
	movs	r1, #4
	movs	r2, #22
	movs	r3, #12
	movs	r0, #4
	bl	sub_08039260
	adds	r1, r0, #0
	str	r1, [r7, #124]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #4
	adds	r5, #2
	bl	sub_08041fa4
	ldr	r1, [r7, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08041fa4
	b.n	.L_0804d908
.L_0804d8be:
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #118
	ldr	r0, [pc, #152]
	bl	sub_080145a8
	movs	r3, #255
	adds	r2, r7, #0
	lsls	r3, r3, #8
	adds	r2, #150
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r5, #2
	movs	r1, #0
	movs	r2, #18
	movs	r3, #4
	movs	r0, #6
	str	r5, [sp, #0]
	bl	sub_08039260
	adds	r6, r7, #0
	adds	r1, r0, #0
	adds	r6, #128
	str	r1, [r6, #0]
	ldr	r0, [pc, #116]
	movs	r2, #12
	movs	r3, #4
	bl	sub_08041fa4
	movs	r0, #5
	movs	r1, #5
	movs	r2, #21
	movs	r3, #7
	str	r5, [sp, #0]
	bl	sub_08039260
	str	r0, [r7, #124]
.L_0804d908:
	movs	r0, #0
	bl	sub_0804d16c
	mov	r3, r8
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_0804d91e
	ldr	r0, [pc, #64]
	bl	sub_08014644
	b.n	.L_0804d924
.L_0804d91e:
	ldr	r0, [pc, #64]
	bl	sub_08014644
.L_0804d924:
	ldr	r0, [r6, #0]
	bl	sub_080393fc
	ldr	r0, [r7, #124]
	bl	sub_080393fc
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [r7, #124]
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	bl	sub_0804d118
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0804d679
	.4byte 0x00001179
	.4byte 0x0804d755
	.2byte 0x1178
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_0804d0dc
	movs	r0, #17
	bl	sub_0804d38c
	movs	r0, #18
	bl	sub_0804d38c
	movs	r0, #19
	bl	sub_0804d38c
	movs	r0, #20
	bl	sub_0804d38c
	movs	r1, #7
	movs	r2, #0
	movs	r0, #17
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	adds	r0, r5, #0
	pop	{r5, pc}
