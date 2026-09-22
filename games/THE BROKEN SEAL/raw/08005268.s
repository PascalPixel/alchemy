.syntax unified
	.thumb
	.set sub_080072f0, 0x080072f0
	.global Overlay_08005268
Overlay_08005268:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	ldr	r3, [pc, #108]
	bl	sub_080072f0
	ldr	r3, [r5, #8]
	ldr	r7, [pc, #104]
	negs	r1, r3
	ldr	r3, [r7, #4]
	movs	r0, #0
	cmp	r1, r3
	blt.n	.L_080052d6
	ldr	r3, [r7, #8]
	cmp	r1, r3
	bgt.n	.L_080052d6
	asrs	r3, r1, #16
	str	r3, [r5, #8]
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L_0800529e
	lsrs	r1, r1, #11
	lsls	r0, r0, #5
	ldr	r3, [pc, #76]
	bl	sub_080072f0
	adds	r4, r0, #0
	b.n	.L_080052a0
.L_0800529e:
	ldr	r4, [pc, #72]
.L_080052a0:
	ldr	r6, [pc, #72]
	ldr	r0, [r5, #0]
	adds	r1, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r6
	cmp	r0, #0
	bge.n	.L_080052b4
	ldr	r3, [pc, #60]
	adds	r0, r0, r3
.L_080052b4:
	ldr	r3, [r7, #12]
	asrs	r2, r0, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r0, [r5, #4]
	adds	r1, r4, #0
	mov	ip, pc
	bx	r6
	cmp	r0, #0
	bge.n	.L_080052cc
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_080052cc:
	ldr	r3, [r7, #16]
	asrs	r2, r0, #16
	subs	r3, r3, r2
	str	r3, [r5, #4]
	adds	r0, r4, #0
.L_080052d6:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000250
	.4byte 0x03001ce0
	.4byte 0x030003f0
	.4byte 0x000151eb
	.4byte 0x03000118
