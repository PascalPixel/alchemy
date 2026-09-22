.syntax unified
	.thumb
	.set sub_080142d4, 0x080142d4
	.set sub_08021918, 0x08021918
	.set sub_080228bc, 0x080228bc
	.global Overlay_08022c84
Overlay_08022c84:
	push	{lr}
	ldr	r2, [pc, #12]
	movs	r1, #128
	movs	r0, #93
	bl	sub_080142d4
	pop	{pc}
	movs	r0, r0
	.2byte 0xe89c
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r4, #0
	str	r4, [sp, #0]
	adds	r7, r0, #0
	bl	sub_08021918
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r0, #0
	ldr	r2, [r3, #12]
	ldrb	r3, [r6, #0]
	movs	r5, #0
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_08022d10
	ldrb	r3, [r2, #4]
	movs	r1, #0
	b.n	.L_08022ccc
.L_08022cc2:
	adds	r1, #1
	adds	r2, #24
	cmp	r1, #63
	bgt.n	.L_08022cd2
	ldrb	r3, [r2, #4]
.L_08022ccc:
	cmp	r3, #0
	bne.n	.L_08022cc2
	adds	r4, r2, #0
.L_08022cd2:
	cmp	r4, #0
	beq.n	.L_08022d10
	ldr	r3, [pc, #52]
	ldr	r0, [r6, #12]
	adds	r5, r4, #0
	mov	r8, r3
	strh	r7, [r5, #0]
	cmp	r0, #0
	bne.n	.L_08022cea
	adds	r0, r7, #0
	bl	sub_080228bc
.L_08022cea:
	ldr	r2, [r6, #16]
	str	r0, [r5, #8]
	str	r2, [r5, #12]
	ldrb	r3, [r6, #10]
	strb	r3, [r5, #7]
	movs	r3, #255
	strb	r3, [r5, #22]
	ldr	r3, [r2, #0]
	str	r3, [r5, #16]
	mov	r3, r8
	strb	r3, [r5, #20]
	ldrb	r3, [r6, #4]
	strb	r3, [r5, #4]
	mov	r3, r8
	strb	r3, [r5, #5]
	b.n	.L_08022d10
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_08022d10:
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
