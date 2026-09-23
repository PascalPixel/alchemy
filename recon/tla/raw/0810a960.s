.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038140, 0x08038140
	.set sub_080c85c8, 0x080c85c8
	.global Func_0810a960
	.thumb_func
Func_0810a960:
.L_0810a960:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #129
	lsls	r2, r2, #3
	adds	r2, #255
	adds	r3, r3, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	cmp	r1, #1
	bne.n	.L_0810a982
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	subs	r3, r3, r2
	adds	r0, r0, r3
.L_0810a982:
	cmp	r1, #2
	bne.n	.L_0810a98e
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #20]
	subs	r3, r3, r2
	adds	r0, r0, r3
.L_0810a98e:
	cmp	r1, #3
	bne.n	.L_0810a99a
	ldr	r3, [pc, #20]
	ldr	r2, [pc, #8]
	subs	r3, r3, r2
	adds	r0, r0, r3
.L_0810a99a:
	pop	{pc}
	.4byte 0x000012df
	.4byte 0x000012d5
	.4byte 0x000012e9
	.2byte 0x12f3
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #250
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldrh	r0, [r3, #0]
	bl	sub_080c85c8
	adds	r5, r0, #0
	bl	sub_08038140
	adds	r0, r6, #0
	bl	.L_0810a960
	lsls	r5, r5, #16
	movs	r3, #34
	orrs	r5, r3
	movs	r1, #5
	movs	r2, #0
	adds	r3, r5, #0
	adds	r6, r0, #0
	bl	sub_08038038
	b.n	.L_0810a9ec
.L_0810a9e6:
	movs	r0, #1
	bl	sub_08013560
.L_0810a9ec:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0810a9e6
	movs	r0, #1
	bl	sub_08013560
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r6, #128
	lsls	r6, r6, #3
	mov	r8, r3
	adds	r6, #220
	add	r6, r8
	ldr	r3, [r6, #0]
	adds	r7, r0, #0
	ldrb	r3, [r3, #5]
	mov	sl, r3
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #250
	add	r3, r8
	ldrh	r0, [r3, #0]
	bl	sub_080c85c8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	.L_0810a960
	ldr	r2, [r6, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r7, r0, #0
	lsls	r5, r5, #16
	bl	sub_08038140
	movs	r3, #34
	orrs	r5, r3
	adds	r0, r7, #0
	movs	r1, #5
	movs	r2, #0
	adds	r3, r5, #0
	bl	sub_08038038
	b.n	.L_0810aa5a
.L_0810aa54:
	movs	r0, #1
	bl	sub_08013560
.L_0810aa5a:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0810aa54
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	add	r3, r8
	ldr	r3, [r3, #0]
	mov	r2, sl
	strb	r2, [r3, #5]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
