.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_08038410, 0x08038410
	.global Func_0810bea8
	.thumb_func
Func_0810bea8:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #230
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	bx	lr
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
	ldr	r7, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #4
	ldr	r6, [pc, #36]
	adds	r3, #200
	sub	sp, #4
	mov	r9, r0
	mov	sl, r1
	mov	fp, r2
	movs	r4, #14
	adds	r5, r7, r3
.L_0810bee6:
	ldrh	r3, [r5, #0]
	cmp	r3, #96
	beq.n	.L_0810bf00
	adds	r0, r3, #0
	str	r4, [sp, #0]
	bl	sub_08014274
	strh	r6, [r5, #0]
	ldr	r4, [sp, #0]
	b.n	.L_0810bf00
	movs	r0, r0
	.2byte 0x0060
	.2byte 0x0000
.L_0810bf00:
	subs	r4, #1
	adds	r5, #2
	cmp	r4, #0
	bge.n	.L_0810bee6
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #20
	adds	r6, r7, r3
	mov	r3, sl
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_0810bf88
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #200
	adds	r3, r3, r7
	mov	r8, r3
	mov	r7, r9
.L_0810bf24:
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0810bf6a
	str	r4, [sp, #0]
	bl	sub_080143ac
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	cmp	r5, #95
	bgt.n	.L_0810bf66
	mov	r1, fp
	adds	r2, r5, #0
	ldrh	r0, [r7, #0]
	bl	sub_08038410
	movs	r3, #128
	adds	r2, r6, #4
	lsls	r3, r3, #23
	stmia	r2!, {r3}
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #52]
	lsls	r3, r5, #2
	adds	r3, r3, r2
	ldrh	r1, [r3, #2]
	ldr	r2, [pc, #40]
	ldrh	r3, [r6, #8]
	lsls	r1, r1, #17
	lsrs	r1, r1, #22
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	ldr	r4, [sp, #0]
.L_0810bf66:
	mov	r3, r8
	strh	r5, [r3, #0]
.L_0810bf6a:
	movs	r3, #2
	adds	r4, #1
	add	r8, r3
	adds	r7, #2
	adds	r6, #12
	cmp	r4, #14
	bgt.n	.L_0810bf88
	cmp	r4, sl
	bne.n	.L_0810bf24
	b.n	.L_0810bf88
	movs	r0, r0
	.4byte 0xfffffc00
	.2byte 0x36e0
	.2byte 0x0200
.L_0810bf88:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
