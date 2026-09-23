.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08020218, 0x08020218
	.set sub_080cdf5c, 0x080cdf5c
	.global Func_080cad84
	.thumb_func
Func_080cad84:
.L_080cad84:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	cmp	r0, #191
	bls.n	.L_080cad94
	movs	r0, #0
	b.n	.L_080cad9a
.L_080cad94:
	lsls	r3, r0, #2
	adds	r3, #20
	ldr	r0, [r2, r3]
.L_080cad9a:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #172]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, r3, r6
	mov	fp, r3
	movs	r3, #129
	lsls	r3, r3, #5
	adds	r3, r3, r6
	mov	r9, r3
	movs	r3, #130
	lsls	r3, r3, #5
	adds	r3, r3, r6
	adds	r7, r6, #0
	mov	sl, r3
	movs	r3, #0
	subs	r7, #32
	mov	r8, r3
	movs	r5, #0
.L_080cadce:
	adds	r0, r5, #0
	bl	.L_080cad84
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_080cae30
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	strb	r5, [r7, #0]
	adds	r3, #212
	adds	r7, #1
	adds	r1, r6, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r3, r4, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080cae08
	ldr	r3, [r4, #80]
	ldrb	r2, [r3, #24]
	ldrb	r1, [r3, #26]
	ldrb	r3, [r3, #9]
	lsls	r3, r3, #28
	lsrs	r0, r3, #30
	b.n	.L_080cae0e
.L_080cae08:
	movs	r2, #0
	movs	r1, #0
	movs	r0, #0
.L_080cae0e:
	mov	r3, fp
	strb	r2, [r3, #0]
	movs	r3, #1
	add	fp, r3
	mov	r3, r9
	strb	r1, [r3, #0]
	movs	r3, #1
	add	r9, r3
	mov	r3, sl
	strb	r0, [r3, #0]
	movs	r3, #1
	add	r8, r3
	add	sl, r3
	mov	r3, r8
	adds	r6, #128
	cmp	r3, #31
	bhi.n	.L_080cae36
.L_080cae30:
	adds	r5, #1
	cmp	r5, #80
	blt.n	.L_080cadce
.L_080cae36:
	mov	r5, r8
	cmp	r5, #31
	bgt.n	.L_080cae4c
	movs	r3, #32
	movs	r2, #255
	subs	r5, r3, r5
.L_080cae42:
	subs	r5, #1
	strb	r2, [r7, #0]
	adds	r7, #1
	cmp	r5, #0
	bne.n	.L_080cae42
.L_080cae4c:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1024
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #252]
	movs	r3, #32
	mov	r8, r2
	negs	r3, r3
	add	r3, r8
	movs	r2, #128
	mov	sl, r3
	lsls	r2, r2, #5
	movs	r3, #129
	sub	sp, #8
	add	r2, r8
	lsls	r3, r3, #5
	str	r2, [sp, #4]
	add	r3, r8
	movs	r2, #130
	str	r3, [sp, #0]
	lsls	r2, r2, #5
	add	r2, r8
	mov	fp, r2
	mov	r2, sl
	movs	r3, #0
	ldrb	r7, [r2, #0]
	mov	r9, r3
	subs	r3, #31
	add	r3, r8
	mov	sl, r3
	cmp	r7, #255
	beq.n	.L_080caf5a
.L_080caea2:
	adds	r0, r7, #0
	bl	.L_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080caf32
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	ldr	r5, [r6, #80]
	adds	r3, #212
	mov	r0, r8
	adds	r1, r6, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r5, [r6, #80]
	ldr	r2, [sp, #4]
	adds	r0, r6, #0
	ldrb	r1, [r2, #0]
	bl	sub_08020090
	ldr	r3, [sp, #0]
	adds	r0, r6, #0
	ldrb	r1, [r3, #0]
	bl	sub_08020218
	mov	r2, fp
	ldrb	r1, [r2, #0]
	movs	r3, #3
	ldrb	r2, [r5, #9]
	ands	r1, r3
	movs	r3, #13
	negs	r3, r3
	lsls	r1, r1, #2
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r5, #9]
	adds	r5, #37
	ldrb	r2, [r5, #0]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r5, #0]
	bl	sub_080cdf5c
	cmp	r7, r0
	bne.n	.L_080caf32
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #230
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r1, [r3, #0]
	ldr	r3, [r2, #32]
	ldr	r2, [r6, #12]
	ldr	r0, [r3, #0]
	ldr	r3, [r6, #8]
	str	r2, [r1, #20]
	str	r3, [r1, #8]
	str	r2, [r1, #12]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	str	r2, [r0, #4]
	adds	r0, r6, #0
	bl	sub_08020138
.L_080caf32:
	ldr	r2, [sp, #4]
	movs	r3, #128
	add	r8, r3
	ldr	r3, [sp, #0]
	adds	r2, #1
	str	r2, [sp, #4]
	movs	r2, #1
	adds	r3, #1
	add	r9, r2
	str	r3, [sp, #0]
	mov	r3, r9
	add	fp, r2
	cmp	r3, #31
	bgt.n	.L_080caf5a
	mov	r2, sl
	ldrb	r7, [r2, #0]
	movs	r3, #1
	add	sl, r3
	cmp	r7, #255
	bne.n	.L_080caea2
.L_080caf5a:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02001024
