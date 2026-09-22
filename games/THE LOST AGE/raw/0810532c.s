.syntax unified
	.thumb
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08038288, 0x08038288
	.global Overlay_0810532c
Overlay_0810532c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r2, [r3, #0]
	movs	r4, #136
	lsls	r3, r0, #2
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldr	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_0810534e
	lsls	r3, r0, #1
	movs	r0, #148
	lsls	r0, r0, #2
	adds	r3, r3, r0
	strh	r1, [r2, r3]
.L_0810534e:
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r4, [r3, #0]
	lsls	r2, r0, #2
	movs	r0, #136
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L_0810536e
	adds	r0, #32
	adds	r3, r2, r0
	str	r1, [r4, r3]
.L_0810536e:
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #18
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_081053a6
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #204
	adds	r5, r0, r3
	movs	r6, #4
.L_08105396:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r6, #1
	bl	sub_08014128
	adds	r5, #12
	cmp	r6, #0
	bge.n	.L_08105396
.L_081053a6:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #96]
	movs	r6, #176
	movs	r7, #192
	lsls	r6, r6, #4
	lsls	r7, r7, #4
	mov	sl, r3
	adds	r6, #204
	mov	fp, r2
	adds	r7, #8
	movs	r3, #0
	movs	r2, #4
	add	r6, sl
	add	r7, sl
	mov	r9, r3
	mov	r8, r2
.L_081053dc:
	bl	sub_080143ac
	adds	r5, r0, #0
	cmp	r5, #95
	bgt.n	.L_08105428
	mov	r3, r9
	mov	r2, fp
	ldrh	r1, [r3, r2]
	movs	r0, #8
	adds	r2, r5, #0
	movs	r3, #0
	bl	sub_08038288
	movs	r3, #128
	adds	r2, r6, #4
	lsls	r3, r3, #23
	stmia	r2!, {r3}
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #32]
	lsls	r3, r5, #2
	adds	r3, r3, r2
	ldrh	r1, [r3, #2]
	ldr	r2, [pc, #16]
	ldrh	r3, [r6, #8]
	lsls	r1, r1, #17
	lsrs	r1, r1, #22
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	b.n	.L_08105428
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x08105a50
	.2byte 0x36e0
	.2byte 0x0200
.L_08105428:
	movs	r2, #1
	negs	r2, r2
	movs	r3, #2
	add	r8, r2
	add	r9, r3
	mov	r3, r8
	strh	r5, [r7, #0]
	adds	r6, #12
	adds	r7, #2
	cmp	r3, #0
	bge.n	.L_081053dc
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #18
	movs	r1, #128
	add	r2, sl
	movs	r3, #1
	lsls	r1, r1, #3
	strb	r3, [r2, #0]
	adds	r1, #138
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x5371
	.2byte 0x0810
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #32]
	ldr	r5, [r3, #0]
	bl	sub_08014644
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #8
	adds	r5, r5, r3
	movs	r6, #4
.L_08105482:
	ldrh	r0, [r5, #0]
	subs	r6, #1
	adds	r5, #2
	bl	sub_08014274
	cmp	r6, #0
	bge.n	.L_08105482
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x5371
	.2byte 0x0810
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r4, [r3, #0]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r0, #189
	lsls	r0, r0, #4
	lsls	r3, r3, #2
	adds	r3, r3, r0
	adds	r4, r4, r3
	ldr	r3, [pc, #16]
	ldrh	r0, [r4, #2]
	ands	r1, r3
	ldr	r3, [pc, #12]
	strb	r2, [r4, #0]
	ands	r3, r0
	orrs	r3, r1
	strh	r3, [r4, #2]
	b.n	.L_081054c8
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_081054c8:
	bx	lr
