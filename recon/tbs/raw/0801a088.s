.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0801a5a4, 0x0801a5a4
	.set sub_08077018, 0x08077018
	.global Func_0801a088
	.thumb_func
Func_0801a088:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	ldr	r2, [sp, #8]
	ldr	r0, [pc, #484]
	mov	fp, r1
	ands	r0, r2
	movs	r1, #0
	str	r1, [sp, #4]
	mov	sl, r1
	bl	sub_08077018
	ldr	r3, [pc, #472]
	str	r0, [sp, #0]
	ldr	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_0801a0bc
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0801a272
.L_0801a0bc:
	movs	r3, #1
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801a0ec
	ldr	r3, [pc, #452]
	adds	r2, r5, r3
	ldr	r3, [pc, #452]
	movs	r1, #192
	ldr	r3, [r3, #8]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	movs	r2, #2
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0801a5a4
	movs	r2, #1
	str	r2, [sp, #4]
.L_0801a0ec:
	ldr	r3, [pc, #412]
	ldr	r1, [sp, #0]
	adds	r3, r3, r5
	mov	r9, r3
	ldrh	r3, [r1, #6]
	ldr	r2, [pc, #412]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	mov	r2, r9
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, r3, r5
	ldr	r2, [pc, #400]
	mov	r8, r3
	movs	r6, #2
	mov	r1, r8
	adds	r7, r5, r2
	strh	r6, [r1, #0]
	strh	r6, [r7, #0]
	ldr	r1, [sp, #4]
	adds	r0, r5, #0
	bl	sub_0801a5a4
	movs	r3, #8
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801a148
	movs	r3, #128
	ldr	r2, [sp, #8]
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0801a148
	ldr	r3, [pc, #360]
	ldr	r3, [r3, #4]
	mov	r1, r9
	mov	r2, r8
	str	r3, [r1, #0]
	adds	r0, r5, #0
	strh	r6, [r2, #0]
	movs	r1, #1
	strh	r6, [r7, #0]
	bl	sub_0801a5a4
.L_0801a148:
	movs	r3, #16
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801a180
	movs	r3, #128
	ldr	r2, [sp, #8]
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0801a180
	ldr	r3, [pc, #300]
	adds	r2, r5, r3
	ldr	r3, [pc, #312]
	movs	r1, #192
	ldr	r3, [r3, #0]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	adds	r3, r5, r1
	movs	r2, #2
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0801a5a4
.L_0801a180:
	movs	r3, #32
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0801a1cc
	movs	r3, #128
	ldr	r1, [sp, #8]
	lsls	r3, r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801a1cc
	ldr	r2, [sp, #0]
	ldrb	r0, [r2, #3]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0801a1cc
	movs	r1, #2
	adds	r3, r1, #0
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0801a1cc
	ldr	r3, [pc, #220]
	adds	r2, r5, r3
	ldr	r3, [pc, #232]
	ldr	r3, [r3, #8]
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r3, r5, r2
	adds	r2, #2
	strh	r1, [r3, #0]
	adds	r3, r5, r2
	strh	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0801a5a4
.L_0801a1cc:
	movs	r3, #2
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801a1f0
	movs	r3, #248
	ldr	r2, [sp, #8]
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r3, r3, #11
	mov	sl, r3
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	cmp	r1, #1
	bgt.n	.L_0801a1f0
	movs	r2, #0
	mov	sl, r2
.L_0801a1f0:
	movs	r3, #4
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801a20a
	movs	r3, #248
	ldr	r2, [sp, #8]
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r3, r3, #11
	mov	sl, r3
	movs	r3, #1
	add	sl, r3
.L_0801a20a:
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_0801a26e
	cmp	r1, #30
	bgt.n	.L_0801a26e
	movs	r1, #10
	mov	r0, sl
	bl	sub_080022fc
	ldr	r3, [pc, #128]
	lsls	r0, r0, #2
	ldr	r2, [pc, #104]
	mov	fp, r3
	movs	r1, #192
	ldr	r3, [r3, r0]
	adds	r2, r2, r5
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	adds	r1, r1, r5
	ldr	r3, [pc, #100]
	mov	r8, r1
	movs	r6, #2
	adds	r7, r5, r3
	mov	r9, r2
	mov	r2, r8
	strh	r6, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #1
	strh	r6, [r7, #0]
	bl	sub_0801a5a4
	mov	r0, sl
	movs	r1, #10
	bl	sub_080022ec
	cmp	r0, #0
	beq.n	.L_0801a26e
	lsls	r3, r0, #2
	mov	r1, fp
	adds	r3, #36
	ldr	r3, [r1, r3]
	mov	r2, r9
	str	r3, [r2, #0]
	mov	r3, r8
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	strh	r6, [r7, #0]
	movs	r1, #1
	bl	sub_0801a5a4
.L_0801a26e:
	movs	r0, #128
	lsls	r0, r0, #1
.L_0801a272:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0x03001e94
	.4byte 0x00000604
	.4byte 0x08029a10
	.4byte 0x08029ee4
	.4byte 0x00000602
	.4byte 0x08029acc
	.4byte 0x08029b68
