.syntax unified
	.thumb
	.set sub_080cad84, 0x080cad84
	.set sub_080eaf98, 0x080eaf98
	.global Overlay_080d1e18
Overlay_080d1e18:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [pc, #60]
	movs	r4, #189
	lsls	r4, r4, #1
	adds	r0, r0, r4
	ldrh	r0, [r0, #0]
	sub	sp, #4
	lsrs	r0, r0, #5
	str	r0, [sp, #0]
	adds	r0, r5, #0
	bl	sub_080eaf98
	movs	r3, #0
	strh	r3, [r5, #30]
	movs	r0, #13
	ldrb	r3, [r5, #9]
	negs	r0, r0
	ldrb	r2, [r5, #5]
	adds	r1, r0, #0
	ands	r1, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r2
	movs	r2, #15
	ands	r1, r2
	ands	r3, r0
	movs	r2, #4
	orrs	r3, r2
	strb	r1, [r5, #9]
	strb	r3, [r5, #5]
	add	sp, #4
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x36e0
	.2byte 0x0200
	push	{lr}
	adds	r2, r0, #0
	ldr	r0, [pc, #24]
	movs	r1, #0
	ldrh	r3, [r0, #0]
	cmp	r3, r2
	beq.n	.L_080d1e7c
.L_080d1e6e:
	adds	r1, #1
	adds	r0, #4
	cmp	r1, #242
	bhi.n	.L_080d1e7c
	ldrh	r3, [r0, #0]
	cmp	r3, r2
	bne.n	.L_080d1e6e
.L_080d1e7c:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0308
	.2byte 0x080f
	push	{lr}
	bl	sub_080cad84
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d1ea0
	ldr	r0, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080d1ea0
	ldr	r0, [r0, #40]
	cmp	r0, #0
	bne.n	.L_080d1ea4
.L_080d1ea0:
	movs	r0, #0
	b.n	.L_080d1ea8
.L_080d1ea4:
	movs	r3, #0
	ldrsh	r0, [r0, r3]
.L_080d1ea8:
	pop	{pc}
