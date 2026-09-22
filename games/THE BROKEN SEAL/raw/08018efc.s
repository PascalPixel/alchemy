.syntax unified
	.thumb
	.set sub_08004080, 0x08004080
	.set sub_08015e8c, 0x08015e8c
	.set sub_08016584, 0x08016584
	.global Overlay_08018efc
Overlay_08018efc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	ldr	r3, [pc, #156]
	adds	r7, r0, #0
	ldr	r5, [r3, #0]
	ldrh	r3, [r7, #10]
	subs	r3, #2
	sub	sp, #4
	adds	r0, r2, #0
	adds	r6, r5, #0
	cmp	r4, r3
	bhi.n	.L_08018ff0
	ldrh	r3, [r7, #8]
	subs	r3, #2
	cmp	r0, r3
	bhi.n	.L_08018ff0
	ldr	r2, [sp, #24]
	cmp	r2, #1
	bne.n	.L_08018fcc
	bl	sub_08015e8c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08018ff0
	ldr	r1, [pc, #116]
	subs	r3, r5, r6
	ldr	r2, [pc, #116]
	adds	r3, r3, r1
	adds	r1, r3, #0
	muls	r1, r2
	movs	r3, #2
	ldr	r2, [pc, #112]
	strb	r3, [r5, #5]
	adds	r6, r6, r2
	ldrh	r3, [r6, #0]
	adds	r4, r5, #0
	mov	r8, r1
	adds	r4, #16
	cmp	r3, #99
	bne.n	.L_08018f5a
	str	r4, [sp, #0]
	bl	sub_08004080
	strh	r0, [r6, #0]
	ldr	r4, [sp, #0]
.L_08018f5a:
	ldrh	r3, [r7, #8]
	ldr	r1, [pc, #84]
	ldrh	r2, [r7, #12]
	adds	r3, r3, r1
	adds	r2, r2, r3
	lsls	r2, r2, #3
	ldr	r3, [pc, #56]
	adds	r2, #4
	ands	r2, r3
	ldr	r1, [pc, #72]
	ldrh	r3, [r4, #6]
	ands	r1, r3
	orrs	r1, r2
	ldrb	r2, [r7, #10]
	ldrb	r3, [r7, #14]
	adds	r2, #254
	adds	r3, r3, r2
	strh	r1, [r4, #6]
	lsls	r3, r3, #3
	lsls	r1, r1, #23
	subs	r3, #1
	lsrs	r1, r1, #23
	strb	r3, [r4, #4]
	strh	r1, [r5, #6]
	ldrb	r3, [r4, #4]
	strh	r3, [r5, #8]
	movs	r3, #0
	str	r3, [r5, #0]
	ldrb	r3, [r5, #5]
	mov	r2, r8
	strb	r2, [r5, #14]
	cmp	r3, #0
	bne.n	.L_08018fc2
	b.n	.L_08018fbc
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffff968
	.4byte 0xb6db6db7
	.4byte 0x000012b6
	.4byte 0x0000fffe
	.2byte 0xfe00
	.2byte 0xffff
.L_08018fbc:
	add	r3, sp, #24
	ldrb	r3, [r3, #0]
	strb	r3, [r5, #5]
.L_08018fc2:
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08016584
	b.n	.L_08018ff0
.L_08018fcc:
	cmp	r1, #255
	bhi.n	.L_08018ff0
	ldrh	r2, [r7, #14]
	adds	r4, #1
	ldrh	r3, [r7, #12]
	adds	r0, #1
	adds	r2, r2, r4
	lsls	r2, r2, #5
	adds	r3, r3, r0
	adds	r0, r2, r3
	movs	r2, #160
	lsls	r2, r2, #2
	cmp	r0, r2
	bcs.n	.L_08018ff0
	ldr	r3, [pc, #16]
	lsls	r2, r0, #1
	orrs	r1, r3
	strh	r1, [r5, r2]
.L_08018ff0:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000f000
