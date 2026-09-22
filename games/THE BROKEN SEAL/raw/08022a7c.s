.syntax unified
	.thumb
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_08015e8c, 0x08015e8c
	.set sub_08016584, 0x08016584
	.global Overlay_08022a7c
Overlay_08022a7c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	mov	r3, sp
	mov	r4, r9
	str	r4, [r3, #0]
	mov	r8, r0
	mov	sl, r1
	mov	fp, r2
	bl	sub_08015e8c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08022b30
	movs	r3, #1
	strb	r3, [r6, #5]
	strb	r3, [r6, #4]
	movs	r0, #128
	bl	sub_080040b4
	movs	r3, #240
	strb	r3, [r6, #15]
	movs	r3, #120
	adds	r7, r6, #0
	strh	r3, [r6, #6]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #60]
	adds	r7, #16
	movs	r5, #0
	strb	r0, [r6, #14]
	str	r3, [r7, #4]
	str	r5, [r7, #8]
	mov	r5, r9
	subs	r5, #4
	ldr	r0, [r5, #0]
	ldrh	r2, [r0, #12]
	ldr	r3, [pc, #36]
	lsls	r2, r2, #3
	add	r2, r8
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #32]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrh	r3, [r0, #14]
	lsls	r3, r3, #3
	add	r3, sl
	strb	r3, [r7, #4]
	mov	r3, fp
	ldrb	r0, [r6, #14]
	cmp	r3, #0
	beq.n	.L_08022b04
	ldr	r1, [pc, #12]
	b.n	.L_08022b06
	.4byte 0x000001ff
	.4byte 0x40000400
	.4byte 0xfffffe00
	.2byte 0x13a4
	.2byte 0x0803
.L_08022b04:
	ldr	r1, [pc, #32]
.L_08022b06:
	bl	sub_080040d0
	ldr	r3, [pc, #24]
	ldrh	r2, [r7, #8]
	ands	r0, r3
	ldr	r3, [pc, #24]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #8]
	ldr	r0, [r5, #0]
	adds	r1, r6, #0
	bl	sub_08016584
	b.n	.L_08022b30
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x08031424
	.2byte 0xfc00
	.2byte 0xffff
.L_08022b30:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
