.syntax unified
	.thumb
	.set sub_08077018, 0x08077018
	.set sub_080a17c4, 0x080a17c4
	.set sub_080a9cbc, 0x080a9cbc
	.global ItemMenu_ArrangeCategoryItemIcons
	.global Func_080a9c18
	.thumb_func
ItemMenu_ArrangeCategoryItemIcons:
Func_080a9c18:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	adds	r7, r6, #0
	sub	sp, #4
	bl	sub_080a9cbc
	movs	r3, #14
	movs	r1, #216
	adds	r7, #72
	adds	r6, r5, #0
	mov	r8, r3
.L_080a9c36:
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	adds	r6, #2
	cmp	r3, #0
	beq.n	.L_080a9ca0
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a9ca0
	ldr	r5, [r7, #0]
	cmp	r5, #0
	beq.n	.L_080a9ca0
	ldr	r0, [pc, #28]
	ands	r0, r2
	str	r1, [sp, #0]
	bl	sub_08077018
	ldrb	r3, [r0, #2]
	ldr	r1, [sp, #0]
	cmp	r3, #2
	beq.n	.L_080a9c88
	cmp	r3, #2
	bgt.n	.L_080a9c7a
	b.n	.L_080a9c74
	movs	r0, r0
	.4byte 0x00000200
	.4byte 0x000001ff
	.2byte 0x1f2c
	.2byte 0x0300
.L_080a9c74:
	cmp	r3, #1
	beq.n	.L_080a9c84
	b.n	.L_080a9c96
.L_080a9c7a:
	cmp	r3, #3
	beq.n	.L_080a9c8c
	cmp	r3, #4
	beq.n	.L_080a9c90
	b.n	.L_080a9c96
.L_080a9c84:
	movs	r3, #32
	b.n	.L_080a9c92
.L_080a9c88:
	movs	r3, #80
	b.n	.L_080a9c92
.L_080a9c8c:
	movs	r3, #64
	b.n	.L_080a9c92
.L_080a9c90:
	movs	r3, #48
.L_080a9c92:
	strh	r1, [r5, #6]
	strh	r3, [r5, #8]
.L_080a9c96:
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	sub_080a17c4
	ldr	r1, [sp, #0]
.L_080a9ca0:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	adds	r7, #4
	cmp	r3, #0
	bge.n	.L_080a9c36
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
