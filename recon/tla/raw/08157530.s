.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08118098, 0x08118098
	.global Func_08157530
	.thumb_func
Func_08157530:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	mov	r9, r3
	adds	r6, r1, #0
	mov	fp, r2
	bl	sub_08118098
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08118098
	ldr	r6, [r5, #0]
	ldr	r0, [r0, #0]
	ldr	r2, [r6, #8]
	ldr	r3, [r0, #8]
	mov	r8, r0
	subs	r3, r3, r2
	mov	r0, r9
	muls	r0, r3
	movs	r1, #100
	mov	sl, r2
	bl	sub_08002054
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r2, [r6, #16]
	adds	r5, r0, #0
	subs	r3, r3, r2
	mov	r0, r9
	muls	r0, r3
	movs	r1, #100
	mov	r8, r2
	bl	sub_08002054
	add	sl, r5
	add	r8, r0
	asrs	r5, r5, #8
	asrs	r0, r0, #8
	adds	r2, r0, #0
	muls	r2, r0
	adds	r3, r5, #0
	muls	r3, r5
	adds	r3, r3, r2
	adds	r0, r3, #0
	ldr	r2, [pc, #84]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4659
	lsls	r0, r0, #8
	bl	sub_08002054
	adds	r3, r6, #0
	movs	r1, #1
	adds	r3, #88
	str	r0, [r6, #52]
	str	r0, [r6, #48]
	strb	r1, [r3, #0]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r6, #72]
	adds	r3, r6, #0
	movs	r2, #0
	adds	r3, #90
	str	r2, [r6, #40]
	str	r2, [r6, #68]
	adds	r0, r6, #0
	strb	r1, [r3, #0]
	bl	sub_08020138
	adds	r0, r6, #0
	mov	r1, sl
	movs	r2, #0
	mov	r3, r8
	bl	sub_08020148
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	adds	r5, r2, #0
	adds	r0, r3, #0
	movs	r2, #0
	mov	lr, r1
	mov	ip, r2
	cmp	r0, #0
	beq.n	.L_08157630
	movs	r7, #0
	adds	r6, r0, #0
.L_08157606:
	movs	r1, #0
	cmp	r5, #0
	beq.n	.L_08157624
	mov	r2, lr
	adds	r3, r6, r2
	subs	r2, r3, #1
	mov	r3, r8
	adds	r4, r7, r3
.L_08157616:
	ldrb	r3, [r4, #0]
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r4, #1
	adds	r2, r2, r0
	cmp	r1, r5
	bne.n	.L_08157616
.L_08157624:
	movs	r2, #1
	add	ip, r2
	adds	r7, r7, r5
	subs	r6, #1
	cmp	ip, r0
	bne.n	.L_08157606
.L_08157630:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
