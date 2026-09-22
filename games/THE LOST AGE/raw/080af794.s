.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038020, 0x08038020
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080aed6c, 0x080aed6c
	.set sub_080aef34, 0x080aef34
	.set sub_080afb80, 0x080afb80
	.set sub_080afbb4, 0x080afbb4
	.set sub_080b0298, 0x080b0298
	.global Overlay_080af794
Overlay_080af794:
	push	{lr}
	bl	sub_080afb80
	pop	{pc}
.L_080af79c:
	movs	r3, #180
	ldr	r2, [pc, #8]
	muls	r0, r3
	adds	r0, r0, r2
	bx	lr
	movs	r0, r0
	.2byte 0x0f4c
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r1, [pc, #160]
	sub	sp, #32
	mov	sl, sp
	mov	r8, r1
	movs	r6, #0
	mov	r9, sl
.L_080af7c2:
	adds	r0, r6, #0
	bl	sub_08016ca4
	adds	r7, r0, #0
	ldr	r0, [pc, #144]
	mov	r1, r9
	adds	r0, r6, r0
	bl	sub_08038020
	mov	r2, r9
	ldrh	r3, [r2, #0]
	movs	r5, #0
	strb	r3, [r7, #0]
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080af7fc
	mov	r1, sl
	adds	r2, r7, #0
	movs	r0, #0
.L_080af7e8:
	adds	r5, #1
	adds	r0, #2
	cmp	r5, #13
	bgt.n	.L_080af7fc
	ldrh	r3, [r0, r1]
	adds	r2, #1
	strb	r3, [r2, #0]
	ldrh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_080af7e8
.L_080af7fc:
	movs	r3, #0
	adds	r6, #1
	strb	r3, [r7, #14]
	cmp	r6, #7
	ble.n	.L_080af7c2
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080af8be
.L_080af812:
	bl	sub_08016ca4
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_080af8ae
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	movs	r5, #14
	ldrh	r0, [r2, #0]
	bl	.L_080af79c
	ldr	r2, [pc, #32]
	adds	r3, r7, #0
	mov	sl, r0
	adds	r3, #244
.L_080af838:
	subs	r5, #1
	strh	r2, [r3, #0]
	subs	r3, #2
	cmp	r5, #0
	bge.n	.L_080af838
	movs	r2, #128
	lsls	r2, r2, #1
	mov	r6, sl
	adds	r2, #255
	movs	r5, #0
	adds	r6, #152
	mov	r9, r2
	b.n	.L_080af860
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x080b2340
	.2byte 0x0083
	.2byte 0x0000
.L_080af860:
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldrh	r3, [r6, #0]
	mov	r1, r9
	ands	r1, r3
	bl	sub_080aed6c
	mov	r2, r8
	adds	r1, r0, #0
	adds	r5, #1
	ldr	r0, [r2, #0]
	adds	r6, #2
	bl	sub_080aef34
	cmp	r5, #12
	bls.n	.L_080af860
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_080b0298
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #22]
	strh	r3, [r7, #20]
	mov	r3, sl
	adds	r3, #150
	mov	r1, r8
	ldr	r0, [r1, #0]
	ldrb	r1, [r3, #0]
	bl	sub_080afbb4
	mov	r2, r8
	ldr	r0, [r2, #0]
	bl	sub_080b0298
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_080ad3f8
.L_080af8ae:
	movs	r1, #4
	add	r8, r1
	mov	r2, r8
	ldr	r3, [r2, #0]
	subs	r1, #5
	adds	r0, r3, #0
	cmp	r3, r1
	bne.n	.L_080af812
.L_080af8be:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
