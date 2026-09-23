.syntax unified
	.thumb
	.set sub_08026f80, 0x08026f80
	.global Func_08024f20
	.thumb_func
Func_08024f20:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	sub	sp, #4
	adds	r6, r5, #0
	adds	r7, r0, #0
	mov	r8, r1
	movs	r4, #0
	adds	r6, #89
.L_08024f38:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08024f6c
	ldrb	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024f6c
	cmp	r5, r7
	beq.n	.L_08024f6c
	ldrh	r1, [r5, #32]
	ldrh	r3, [r7, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r1, #2
	subs	r3, #2
	mov	r2, r8
	str	r4, [sp, #0]
	bl	sub_08026f80
	ldr	r4, [sp, #0]
	cmp	r0, #0
	blt.n	.L_08024f6c
	movs	r0, #1
	negs	r0, r0
	b.n	.L_08024f78
.L_08024f6c:
	adds	r4, #1
	adds	r6, #128
	adds	r5, #128
	cmp	r4, #63
	ble.n	.L_08024f38
	movs	r0, #0
.L_08024f78:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	sub	sp, #4
	adds	r6, r5, #0
	adds	r7, r0, #0
	mov	r8, r1
	movs	r4, #0
	adds	r6, #89
.L_08024f98:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08024fca
	ldrb	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024fca
	cmp	r5, r7
	beq.n	.L_08024fca
	ldrh	r1, [r5, #32]
	ldrh	r3, [r7, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r1, #2
	subs	r3, #2
	mov	r2, r8
	str	r4, [sp, #0]
	bl	sub_08026f80
	ldr	r4, [sp, #0]
	cmp	r0, #0
	blt.n	.L_08024fca
	adds	r0, r5, #0
	b.n	.L_08024fd6
.L_08024fca:
	adds	r4, #1
	adds	r6, #128
	adds	r5, #128
	cmp	r4, #63
	ble.n	.L_08024f98
	movs	r0, #0
.L_08024fd6:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
