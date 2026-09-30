.syntax unified
	.thumb
	.global ItemMenu_ArrangeCategoryItemIcons
	.thumb_func
ItemMenu_ArrangeCategoryItemIcons:
.L_08100660:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	adds	r7, r6, #0
	sub	sp, #4
	bl	Func_08100700
	movs	r3, #14
	movs	r1, #216
	adds	r7, #76
	adds	r6, r5, #0
	mov	r8, r3
.L_08100682:
	ldrh	r2, [r6, #0]
	adds	r6, #2
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081006e8
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081006e8
	ldr	r5, [r7, #0]
	cmp	r5, #0
	beq.n	.L_081006e8
	ldr	r0, [pc, #28]
	str	r1, [sp, #0]
	ands	r0, r2
	bl	Item_Get
	ldrb	r3, [r0, #2]
	ldr	r1, [sp, #0]
	cmp	r3, #2
	beq.n	.L_081006d0
	cmp	r3, #2
	bgt.n	.L_081006c2
	b.n	.L_081006bc
	movs	r0, r0
	.4byte 0x00000200
	.2byte 0x01ff
	.2byte 0x0000
.L_081006bc:
	cmp	r3, #1
	beq.n	.L_081006cc
	b.n	.L_081006de
.L_081006c2:
	cmp	r3, #3
	beq.n	.L_081006d4
	cmp	r3, #4
	beq.n	.L_081006d8
	b.n	.L_081006de
.L_081006cc:
	movs	r3, #32
	b.n	.L_081006da
.L_081006d0:
	movs	r3, #80
	b.n	.L_081006da
.L_081006d4:
	movs	r3, #64
	b.n	.L_081006da
.L_081006d8:
	movs	r3, #48
.L_081006da:
	strh	r1, [r5, #6]
	strh	r3, [r5, #8]
.L_081006de:
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	UiIcon_PrepareObject
	ldr	r1, [sp, #0]
.L_081006e8:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	adds	r7, #4
	cmp	r3, #0
	bge.n	.L_08100682
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
