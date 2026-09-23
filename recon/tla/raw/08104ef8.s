.syntax unified
	.thumb
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.set sub_080ad100, 0x080ad100
	.set sub_080c82b8, 0x080c82b8
	.global Func_08104ef8
	.thumb_func
Func_08104ef8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r7, #172
	movs	r2, #1
	mov	r8, r3
	movs	r1, #0
	lsls	r7, r7, #1
	negs	r2, r2
	movs	r3, #7
	mov	fp, r1
	add	r7, r8
	movs	r6, #248
	mov	sl, r2
	mov	r9, r3
.L_08104f2a:
	mov	r1, r8
	ldr	r5, [r6, r1]
	cmp	r5, #0
	beq.n	.L_08104f42
	adds	r0, r5, #0
	bl	sub_08020048
	mov	r3, fp
	mov	r2, r8
	mov	r1, sl
	str	r3, [r6, r2]
	strh	r1, [r7, #0]
.L_08104f42:
	movs	r2, #1
	negs	r2, r2
	add	r9, r2
	mov	r3, r9
	adds	r7, #2
	adds	r6, #4
	cmp	r3, #0
	bge.n	.L_08104f2a
	add	r5, sp, #8
	adds	r0, r5, #0
	bl	sub_080ad100
	mov	r1, r8
	strb	r0, [r1, #30]
	ldr	r3, [sp, #4]
	movs	r2, #0
	lsls	r0, r3, #1
	ldrh	r3, [r5, r0]
	mov	r9, r2
	cmp	r3, #255
	beq.n	.L_08104fd0
	mov	r2, r8
	adds	r2, #248
	mov	fp, r5
	adds	r1, r0, #0
	movs	r7, #172
	str	r2, [sp, #0]
	add	r1, fp
	lsls	r7, r7, #1
	mov	sl, r1
	add	r7, r8
	adds	r6, r0, #0
.L_08104f82:
	mov	r1, fp
	ldrh	r3, [r6, r1]
	ldrh	r0, [r6, r1]
	strh	r3, [r7, #0]
	bl	sub_080c82b8
	bl	sub_08020040
	adds	r5, r0, #0
	adds	r7, #2
	cmp	r5, #0
	beq.n	.L_08104fb0
	movs	r1, #1
	bl	sub_08020030
	ldrb	r3, [r5, #9]
	movs	r1, #13
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #0
	strb	r3, [r5, #26]
.L_08104fb0:
	ldr	r3, [sp, #0]
	movs	r1, #1
	stmia	r3!, {r5}
	add	r9, r1
	adds	r2, r3, #0
	str	r2, [sp, #0]
	mov	r2, r9
	adds	r6, #2
	cmp	r2, #3
	bgt.n	.L_08104fd0
	movs	r3, #2
	add	sl, r3
	mov	r1, sl
	ldrh	r3, [r1, #0]
	cmp	r3, #255
	bne.n	.L_08104f82
.L_08104fd0:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
