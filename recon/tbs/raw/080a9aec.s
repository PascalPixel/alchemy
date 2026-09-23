.syntax unified
	.thumb
	.set sub_08015080, 0x08015080
	.set sub_08077018, 0x08077018
	.global Func_080a9aec
	.thumb_func
Func_080a9aec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #56]
	mov	r8, r3
	movs	r3, #14
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	sl, r3
.L_080a9b00:
	ldrh	r0, [r7, #0]
	ldr	r3, [pc, #36]
	ands	r3, r0
	adds	r7, #2
	cmp	r3, #0
	beq.n	.L_080a9b7c
	ldr	r3, [pc, #28]
	adds	r5, r3, #0
	ands	r5, r0
	adds	r0, r5, #0
	bl	sub_08077018
	ldrb	r3, [r0, #2]
	cmp	r3, #2
	beq.n	.L_080a9b4e
	cmp	r3, #2
	bgt.n	.L_080a9b34
	cmp	r3, #1
	beq.n	.L_080a9b3e
	b.n	.L_080a9b7c
	.4byte 0x00000200
	.4byte 0x000001ff
	.2byte 0x0182
	.2byte 0x0000
.L_080a9b34:
	cmp	r3, #3
	beq.n	.L_080a9b5e
	cmp	r3, #4
	beq.n	.L_080a9b6e
	b.n	.L_080a9b7c
.L_080a9b3e:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #8
	bl	sub_08015080
	b.n	.L_080a9b7c
.L_080a9b4e:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #56
	bl	sub_08015080
	b.n	.L_080a9b7c
.L_080a9b5e:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #40
	bl	sub_08015080
	b.n	.L_080a9b7c
.L_080a9b6e:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #24
	bl	sub_08015080
.L_080a9b7c:
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080a9b00
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
