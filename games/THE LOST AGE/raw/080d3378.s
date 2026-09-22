.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_08014878, 0x08014878
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_080d3744, 0x080d3744
	.global Overlay_080d3378
Overlay_080d3378:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r0, #30
	adds	r5, r1, #0
	adds	r0, #255
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	bl	sub_080200c0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d3458
	ldr	r2, [r6, #80]
	mov	r8, r2
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #1
	bne.n	.L_080d33bc
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
	ldr	r1, [pc, #8]
	adds	r0, r6, #0
	bl	sub_080200a8
	b.n	.L_080d33cc
	.2byte 0x085c
	.2byte 0x080f
.L_080d33bc:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08020090
	ldr	r1, [pc, #132]
	adds	r0, r6, #0
	bl	sub_080200a8
.L_080d33cc:
	cmp	r5, #0
	beq.n	.L_080d33d8
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080d3744
.L_080d33d8:
	adds	r3, r6, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl	sub_08014878
	movs	r1, #10
	bl	sub_0800206c
	ldr	r3, [pc, #100]
	adds	r0, #5
	muls	r3, r0
	str	r3, [r6, #52]
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	movs	r3, #200
	subs	r0, #7
	lsls	r3, r3, #5
	lsls	r0, r0, #1
	adds	r3, #153
	muls	r3, r0
	str	r3, [r6, #48]
	adds	r3, r6, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	adds	r0, r6, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	adds	r2, r6, #0
	strb	r3, [r0, #0]
	adds	r2, #97
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #32]
	str	r3, [r6, #108]
	mov	r3, r8
	strb	r1, [r3, #26]
	ldr	r3, [r7, #80]
	movs	r2, #12
	ldrb	r3, [r3, #9]
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r2, #9]
	b.n	.L_080d3458
	.4byte 0x00000000
	.4byte 0x080f0874
	.4byte 0xffffe667
	.2byte 0x333d
	.2byte 0x080d
.L_080d3458:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
