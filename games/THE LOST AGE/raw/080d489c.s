.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_08020218, 0x08020218
	.set sub_080cad84, 0x080cad84
	.set sub_080d4868, 0x080d4868
	.global Overlay_080d489c
Overlay_080d489c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	mov	r8, r1
	bl	sub_080cad84
	adds	r7, r0, #0
	movs	r5, #0
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_080d4984
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080d48e0
	cmp	r3, #2
	beq.n	.L_080d48ce
	ldr	r0, [r7, #104]
	bl	sub_080d4868
	cmp	r0, #0
	beq.n	.L_080d48f0
.L_080d48ce:
	movs	r0, #132
	lsls	r0, r0, #1
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	bl	sub_080200c0
	adds	r5, r0, #0
	b.n	.L_080d48f0
.L_080d48e0:
	ldr	r5, [r7, #104]
	cmp	r5, #0
	beq.n	.L_080d4984
	adds	r0, r5, #0
	bl	sub_080200c8
	str	r6, [r7, #104]
	b.n	.L_080d4984
.L_080d48f0:
	cmp	r5, #0
	beq.n	.L_080d4984
	movs	r6, #3
	mov	r2, r8
	ands	r6, r2
	cmp	r6, #1
	beq.n	.L_080d4904
	cmp	r6, #2
	beq.n	.L_080d4916
	b.n	.L_080d492e
.L_080d4904:
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
	adds	r3, r5, #0
	adds	r3, #100
	str	r5, [r7, #104]
	strh	r6, [r3, #0]
	b.n	.L_080d492e
.L_080d4916:
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
	ldr	r1, [pc, #72]
	adds	r0, r5, #0
	bl	sub_080200a8
	adds	r2, r5, #0
	adds	r2, #100
	movs	r3, #1
	strh	r3, [r2, #0]
.L_080d492e:
	ldr	r2, [pc, #52]
	adds	r3, r5, #0
	adds	r3, #102
	mov	r1, sl
	strh	r1, [r3, #0]
	subs	r3, #17
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r6, [r5, #80]
	str	r3, [r5, #108]
	movs	r3, #128
	strb	r2, [r6, #26]
	lsls	r3, r3, #1
	mov	r2, r8
	ands	r3, r2
	str	r7, [r5, #104]
	cmp	r3, #0
	beq.n	.L_080d4970
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r6, #9]
	b.n	.L_080d4984
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x080f341c
	.2byte 0x476d
	.2byte 0x080d
.L_080d4970:
	ldr	r3, [r7, #80]
	ldrb	r1, [r6, #9]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r6, #9]
.L_080d4984:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r1, #0
	adds	r5, r0, #0
	bl	sub_08020218
	adds	r5, #89
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #0
	pop	{r5, pc}
	push	{lr}
	movs	r1, #0
	bl	sub_08020218
	movs	r0, #0
	pop	{pc}
