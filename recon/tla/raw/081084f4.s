.syntax unified
	.thumb
	.balign 4
	.global Func_081084f4
	.thumb_func
Func_081084f4:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #250
	adds	r3, r5, r2
	adds	r6, r0, #0
	ldrh	r0, [r3, #0]
	bl	Func_080c85c8
	adds	r7, r0, #0
	bl	0x08038140
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #6
	adds	r5, r5, r3
	movs	r1, #0
	ldrsb	r1, [r5, r1]
	cmp	r1, #3
	bne.n	.L_0810852c
	ldr	r3, [pc, #68]
	ldr	r2, [pc, #72]
	subs	r3, r3, r2
	adds	r6, r6, r3
.L_0810852c:
	cmp	r1, #2
	bne.n	.L_08108538
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #60]
	subs	r3, r3, r2
	adds	r6, r6, r3
.L_08108538:
	cmp	r1, #0
	bne.n	.L_08108544
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #48]
	subs	r3, r3, r2
	adds	r6, r6, r3
.L_08108544:
	lsls	r3, r7, #16
	movs	r2, #34
	orrs	r3, r2
	adds	r0, r6, #0
	movs	r1, #5
	movs	r2, #0
	bl	0x08038038
	b.n	.L_0810855c
.L_08108556:
	movs	r0, #1
	bl	WaitFrames
.L_0810855c:
	bl	0x08038048
	cmp	r0, #0
	beq.n	.L_08108556
	movs	r0, #1
	bl	WaitFrames
	pop	{r5, r6, r7, pc}
	.4byte 0x00001317
	.4byte 0x0000124c
	.4byte 0x00001277
	.2byte 0x12a2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #220
	adds	r7, r6, r2
	ldr	r3, [r7, #0]
	adds	r2, #30
	ldrb	r3, [r3, #5]
	adds	r5, r0, #0
	mov	sl, r3
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	Func_080c85c8
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #6
	adds	r3, r6, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	mov	r8, r0
	cmp	r1, #3
	bne.n	.L_081085c0
	ldr	r3, [pc, #100]
	ldr	r2, [pc, #104]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L_081085c0:
	cmp	r1, #2
	bne.n	.L_081085cc
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #92]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L_081085cc:
	cmp	r1, #0
	bne.n	.L_081085d8
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #80]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L_081085d8:
	ldr	r2, [r7, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	0x08038140
	mov	r2, r8
	lsls	r3, r2, #16
	movs	r2, #34
	orrs	r3, r2
	adds	r0, r5, #0
	movs	r1, #5
	movs	r2, #0
	bl	0x08038038
	b.n	.L_081085fc
.L_081085f6:
	movs	r0, #1
	bl	WaitFrames
.L_081085fc:
	bl	0x08038048
	cmp	r0, #0
	beq.n	.L_081085f6
	movs	r0, #1
	bl	WaitFrames
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #220
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	mov	r2, sl
	strb	r2, [r3, #5]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x00001317
	.4byte 0x0000124c
	.4byte 0x00001277
	.2byte 0x12a2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	adds	r5, r5, r3
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	ldrb	r6, [r0, #5]
	bl	UiIcon_PrepareObjectFar
	adds	r2, r7, #0
	movs	r1, #5
	movs	r0, #7
	bl	Func_08038390
	ldr	r3, [r5, #0]
	adds	r7, r0, #0
	strb	r6, [r3, #5]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000