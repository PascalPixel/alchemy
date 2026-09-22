.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_0802b1a0, 0x0802b1a0
	.set sub_0802b38c, 0x0802b38c
	.global Overlay_0802b63c
Overlay_0802b63c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #156]
	sub	sp, #12
	str	r3, [sp, #8]
	add	r3, sp, #8
	mov	r8, r3
	mov	r0, r8
	bl	.L_0802b6e8
	ldr	r3, [pc, #144]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	str	r0, [sp, #8]
.L_0802b65c:
	mov	r0, r8
	bl	.L_0802b6e8
	movs	r3, #255
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	lsls	r3, r3, #8
	adds	r2, r5, #1
	adds	r3, #255
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802b6d6
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #255
	ands	r0, r5
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0802b6ce
	ldr	r3, [sp, #8]
	ldrb	r0, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r1, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r7, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r6, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r2, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r4, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #5
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0802b6c0
	adds	r3, r4, #0
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_0802b38c
	b.n	.L_0802b65c
.L_0802b6c0:
	str	r2, [sp, #0]
	adds	r3, r6, #0
	adds	r2, r7, #0
	str	r4, [sp, #4]
	bl	sub_0802b1a0
	b.n	.L_0802b65c
.L_0802b6ce:
	ldr	r3, [sp, #8]
	adds	r3, #6
	str	r3, [sp, #8]
	b.n	.L_0802b65c
.L_0802b6d6:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0202e002
	.2byte 0xe000
	.2byte 0x0202
.L_0802b6e8:
	ldr	r3, [r0, #0]
	ldrb	r1, [r3, #0]
	adds	r3, #1
	str	r3, [r0, #0]
	ldrb	r2, [r3, #0]
	adds	r3, #1
	lsls	r2, r2, #8
	orrs	r1, r2
	str	r3, [r0, #0]
	adds	r0, r1, #0
	bx	lr
