.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_08038260, 0x08038260
	.set sub_080ad010, 0x080ad010
	.set sub_080ad058, 0x080ad058
	.set sub_080f811c, 0x080f811c
	.set sub_080f8ce8, 0x080f8ce8
	.set sub_080f92ac, 0x080f92ac
	.set sub_080fb8ac, 0x080fb8ac
	.set sub_081c0010, 0x081c0010
	.global Func_080fb6d4
	.thumb_func
Func_080fb6d4:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad010
	ldrb	r3, [r0, #12]
	cmp	r3, #2
	bne.n	.L_080fb730
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #6
	cmp	r0, r3
	bcs.n	.L_080fb730
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r5, r2
	subs	r2, #174
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrh	r1, [r3, #0]
	bl	sub_080ad058
	movs	r0, #138
	bl	sub_081c0010
	movs	r2, #1
	ldr	r0, [pc, #16]
	negs	r2, r2
	movs	r1, #0
	bl	sub_080f8ce8
	ldr	r0, [r5, #48]
	bl	sub_08038260
.L_080fb730:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x10b7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	sub	sp, #8
	movs	r3, #10
	adds	r7, r6, #0
	str	r3, [sp, #0]
	adds	r7, #52
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r5, r0, #0
	movs	r3, #13
	movs	r1, #0
	movs	r2, #0
	adds	r0, r7, #0
	bl	sub_080f811c
	bl	sub_080f92ac
	movs	r3, #182
	lsls	r3, r3, #1
	lsls	r5, r5, #1
	adds	r5, r5, r3
	ldrh	r3, [r6, r5]
	cmp	r3, #0
	beq.n	.L_080fb778
	ldr	r0, [r7, #0]
	adds	r1, r3, #0
	bl	sub_080fb8ac
.L_080fb778:
	movs	r0, #1
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.align 2, 0
