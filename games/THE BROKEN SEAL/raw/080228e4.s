.syntax unified
	.thumb
	.set sub_08077080, 0x08077080
	.global Overlay_080228e4
Overlay_080228e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	mov	r2, r8
	ldrh	r3, [r2, #0]
	mov	sl, r0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_0802298e
	ldr	r3, [pc, #104]
	ldr	r6, [sp, #12]
	mov	fp, r3
	mov	r5, r8
	subs	r6, #2
.L_08022916:
	ldrh	r0, [r5, #0]
	bl	sub_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802297e
	ldrh	r2, [r5, #0]
	mov	r3, fp
	ands	r3, r2
	strh	r3, [r6, #2]
	movs	r1, #1
	add	r9, r1
	mov	r1, sl
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	eors	r3, r2
	mov	r2, fp
	ands	r3, r2
	adds	r6, #2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0802295e
	ldr	r7, [pc, #40]
	adds	r4, r5, #0
.L_0802294a:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L_0802295e
	adds	r1, #4
	ldrh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_0802294a
.L_0802295e:
	cmp	r0, #32
	bne.n	.L_0802297e
	ldr	r3, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #4]
	ldr	r2, [pc, #8]
	ldrh	r3, [r6, #0]
	orrs	r3, r2
	b.n	.L_0802297c
	.4byte 0x00003fff
	.4byte 0x00008000
	.2byte 0x3fff
	.2byte 0x0000
.L_0802297c:
	strh	r3, [r6, #0]
.L_0802297e:
	mov	r3, r8
	adds	r5, #4
	adds	r3, #124
	cmp	r5, r3
	bgt.n	.L_0802298e
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08022916
.L_0802298e:
	mov	r1, sl
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08022a18
	mov	r2, r9
	lsls	r3, r2, #1
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #88]
	mov	r5, sl
	adds	r7, r3, r1
	mov	fp, r2
.L_080229a4:
	ldrh	r0, [r5, #0]
	bl	sub_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08022a08
	mov	r1, r8
	ldrh	r2, [r1, #0]
	ldrh	r3, [r5, #0]
	eors	r3, r2
	mov	r2, fp
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080229de
	ldr	r6, [pc, #40]
	adds	r4, r5, #0
.L_080229ca:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L_080229de
	adds	r1, #4
	ldrh	r3, [r4, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080229ca
.L_080229de:
	cmp	r0, #32
	bne.n	.L_08022a08
	ldr	r3, [sp, #0]
	adds	r3, #1
	str	r3, [sp, #0]
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #8]
	mov	r1, fp
	b.n	.L_080229fc
	.4byte 0x00003fff
	.4byte 0x00004000
	.2byte 0x3fff
	.2byte 0x0000
.L_080229fc:
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #1
	strh	r3, [r7, #0]
	add	r9, r2
	adds	r7, #2
.L_08022a08:
	mov	r3, sl
	adds	r5, #4
	adds	r3, #124
	cmp	r5, r3
	bgt.n	.L_08022a18
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080229a4
.L_08022a18:
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #8]
	str	r3, [r1, #0]
	ldr	r2, [sp, #0]
	ldr	r3, [sp, #48]
	mov	r0, r9
	str	r2, [r3, #0]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
