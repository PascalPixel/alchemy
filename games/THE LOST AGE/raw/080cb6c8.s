.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_08014d00, 0x08014d00
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020268, 0x08020268
	.set sub_080ad0c0, 0x080ad0c0
	.set sub_080ad0c8, 0x080ad0c8
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080d172c, 0x080d172c
	.set sub_080d17ac, 0x080d17ac
	.set sub_081c0010, 0x081c0010
	.global Overlay_080cb6c8
Overlay_080cb6c8:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl	sub_080ad0f0
	cmp	r0, #0
	ble.n	.L_080cb6ee
	ldr	r3, [pc, #24]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r6, r3, r2
	adds	r5, r0, #0
.L_080cb6de:
	ldrb	r0, [r6, #0]
	adds	r1, r7, #0
	subs	r5, #1
	adds	r6, #1
	bl	sub_080ad0c8
	cmp	r5, #0
	bne.n	.L_080cb6de
.L_080cb6ee:
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	r8, r1
	cmp	r5, #0
	bge.n	.L_080cb72c
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	movs	r1, #0
	bl	sub_080d172c
	movs	r0, #4
	bl	sub_080d17ac
	movs	r2, #10
	negs	r2, r2
	cmp	r5, r2
	bge.n	.L_080cb724
	movs	r0, #134
	bl	sub_081c0010
	b.n	.L_080cb732
.L_080cb724:
	movs	r0, #133
	bl	sub_081c0010
	b.n	.L_080cb732
.L_080cb72c:
	movs	r0, #126
	bl	sub_081c0010
.L_080cb732:
	bl	sub_080ad0f0
	cmp	r0, #0
	ble.n	.L_080cb77c
	ldr	r3, [pc, #72]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r7, r3, r2
	adds	r6, r0, #0
.L_080cb744:
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	mov	r3, r8
	adds	r1, r5, #0
	cmp	r3, #0
	beq.n	.L_080cb76e
	movs	r2, #52
	ldrsh	r3, [r0, r2]
	movs	r1, #100
	adds	r0, r5, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r1, r0, #0
	cmp	r1, #0
	bne.n	.L_080cb76e
	adds	r1, r5, #0
	cmp	r1, #0
	bge.n	.L_080cb76e
	negs	r1, r1
.L_080cb76e:
	ldrb	r0, [r7, #0]
	subs	r6, #1
	bl	sub_080ad0c0
	adds	r7, #1
	cmp	r6, #0
	bne.n	.L_080cb744
.L_080cb77c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r7, #0
	bl	sub_080ad0f0
	cmp	r7, r0
	bge.n	.L_080cb806
	ldr	r3, [pc, #144]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r6, r3, r2
	adds	r5, r0, #0
.L_080cb79e:
	ldrb	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r2, #50
	adds	r2, #255
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	beq.n	.L_080cb7ba
	cmp	r3, #2
	beq.n	.L_080cb7d8
	b.n	.L_080cb7f6
.L_080cb7ba:
	movs	r3, #52
	ldrsh	r0, [r0, r3]
	movs	r1, #20
	adds	r0, #10
	bl	sub_08002054
	negs	r1, r0
	cmp	r1, #0
	bne.n	.L_080cb7d0
	movs	r1, #1
	negs	r1, r1
.L_080cb7d0:
	cmp	r7, #0
	bgt.n	.L_080cb7f8
	movs	r7, #1
	b.n	.L_080cb7f8
.L_080cb7d8:
	movs	r2, #52
	ldrsh	r0, [r0, r2]
	movs	r1, #10
	adds	r0, #5
	bl	sub_08002054
	negs	r1, r0
	cmp	r1, #0
	bne.n	.L_080cb7ee
	movs	r1, #1
	negs	r1, r1
.L_080cb7ee:
	cmp	r7, #1
	bgt.n	.L_080cb7f8
	movs	r7, #2
	b.n	.L_080cb7f8
.L_080cb7f6:
	movs	r1, #0
.L_080cb7f8:
	ldrb	r0, [r6, #0]
	subs	r5, #1
	bl	sub_080ad0c0
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L_080cb79e
.L_080cb806:
	cmp	r7, #0
	beq.n	.L_080cb822
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	movs	r1, #0
	bl	sub_080d172c
	movs	r0, #4
	bl	sub_080d17ac
	movs	r0, #133
	bl	sub_081c0010
.L_080cb822:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r1, #213
	lsls	r1, r1, #4
	movs	r0, #108
	bl	sub_08014d00
	movs	r1, #197
	adds	r2, r0, #0
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_080cb8a0
	adds	r1, #1
	adds	r3, r2, r1
	movs	r5, #1
	strb	r5, [r3, #0]
	adds	r1, #65
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	movs	r1, #168
	adds	r3, #91
	strb	r5, [r3, #0]
	lsls	r1, r1, #3
	movs	r0, #124
	movs	r6, #0
	bl	sub_08014d00
	cmp	r0, #0
	beq.n	.L_080cb89c
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #61
	adds	r1, r0, r2
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_080cb89c
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #58
	adds	r2, r0, r3
	movs	r3, #80
	strb	r3, [r2, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #59
	adds	r3, r0, r2
	adds	r2, #1
	strb	r6, [r3, #0]
	adds	r3, r0, r2
	strb	r5, [r3, #0]
	movs	r0, #2
	strb	r6, [r1, #0]
	bl	sub_08013560
.L_080cb89c:
	bl	sub_08020268
.L_080cb8a0:
	pop	{r5, r6, pc}
