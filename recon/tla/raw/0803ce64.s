.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_0803954c, 0x0803954c
	.set sub_0803a3b8, 0x0803a3b8
	.set sub_0803a3e4, 0x0803a3e4
	.set sub_0803b094, 0x0803b094
	.set sub_0803b880, 0x0803b880
	.global Func_0803ce64
	.thumb_func
Func_0803ce64:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	sub	sp, #24
	mov	r8, r3
	adds	r6, r1, #0
	movs	r3, #8
	movs	r1, #1
	mov	r9, r2
	str	r3, [sp, #20]
	str	r3, [sp, #16]
	adds	r7, r0, #0
	bl	sub_0803b094
	movs	r2, #244
	lsls	r2, r2, #4
	lsls	r3, r0, #1
	adds	r3, r3, r2
	mov	r2, r8
	ldrh	r3, [r2, r3]
	movs	r5, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_0803cf52
	add	r0, sp, #8
	add	r1, sp, #20
	add	r2, sp, #16
	add	r3, sp, #12
	str	r0, [sp, #0]
	adds	r0, r7, #0
	bl	sub_0803b880
	ldr	r2, [sp, #12]
	movs	r3, #30
	ldr	r4, [sp, #8]
	subs	r3, r3, r2
	asrs	r0, r3, #1
	movs	r3, #15
	subs	r3, r3, r4
	asrs	r3, r3, #1
	mov	r7, r9
	adds	r1, r3, r7
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	cmp	r6, #0
	beq.n	.L_0803ced6
	adds	r3, r4, #0
	str	r5, [sp, #0]
	bl	sub_08039260
	adds	r5, r0, #0
	b.n	.L_0803cee8
.L_0803ced6:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08039260
	adds	r5, r0, #0
	strh	r6, [r5, #8]
	strh	r6, [r5, #10]
.L_0803cee8:
	movs	r3, #0
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	bl	sub_0803954c
	cmp	r0, #0
	bne.n	.L_0803cf0c
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0803939c
	b.n	.L_0803cf52
.L_0803cf06:
	movs	r0, #1
	bl	sub_08013560
.L_0803cf0c:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_0803cf06
	cmp	r6, #0
	beq.n	.L_0803cf34
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0803939c
	b.n	.L_0803cf28
.L_0803cf22:
	movs	r0, #1
	bl	sub_08013560
.L_0803cf28:
	adds	r0, r5, #0
	bl	sub_0803a3e4
	cmp	r0, #0
	beq.n	.L_0803cf22
	b.n	.L_0803cf3c
.L_0803cf34:
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0803939c
.L_0803cf3c:
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #132
	add	r3, r8
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #134
	add	r3, r8
	strh	r2, [r3, #0]
.L_0803cf52:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
