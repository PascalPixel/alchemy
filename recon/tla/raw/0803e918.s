.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803e5a8, 0x0803e5a8
	.set sub_0803ebdc, 0x0803ebdc
	.set sub_0803ed98, 0x0803ed98
	.set sub_0803ef48, 0x0803ef48
	.set sub_0803ef8c, 0x0803ef8c
	.set sub_0803f004, 0x0803f004
	.set sub_0803f3c8, 0x0803f3c8
	.set sub_081c0010, 0x081c0010
	.global Func_0803e918
	.thumb_func
Func_0803e918:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #72]
	adds	r6, r0, #0
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_0803ef8c
.L_0803e92a:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #232
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0803e92a
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r6, r3
	beq.n	.L_0803e97e
	ldr	r1, [pc, #76]
	movs	r2, #16
	ldr	r3, [r1, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e95a
	adds	r0, r5, #0
	bl	sub_0803ebdc
	b.n	.L_0803e97e
.L_0803e95a:
	ldr	r3, [r1, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e96c
	adds	r0, r5, #0
	bl	sub_0803ed98
	b.n	.L_0803e97e
.L_0803e96c:
	ldr	r3, [r1, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e97e
	adds	r0, r5, #0
	bl	sub_0803f3c8
	b.n	.L_0803e992
.L_0803e97e:
	cmp	r6, #0
	beq.n	.L_0803e92a
	ldr	r3, [pc, #16]
	movs	r2, #2
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e92a
	movs	r0, #1
	negs	r0, r0
.L_0803e992:
	pop	{r5, r6, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #72]
	adds	r7, r0, #0
.L_0803e9a2:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #232
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0803e9a2
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r7, r2
	beq.n	.L_0803ea34
	ldr	r6, [pc, #144]
	movs	r2, #16
	ldr	r3, [r6, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e9d8
	movs	r0, #111
	bl	sub_081c0010
	adds	r0, r5, #0
	bl	sub_0803ebdc
	b.n	.L_0803e9ee
.L_0803e9d8:
	ldr	r3, [r6, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e9ee
	movs	r0, #111
	bl	sub_081c0010
	adds	r0, r5, #0
	bl	sub_0803ed98
.L_0803e9ee:
	ldr	r3, [r6, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803ea34
	movs	r1, #231
	lsls	r1, r1, #2
	adds	r3, r5, r1
	adds	r1, #2
	ldrh	r2, [r3, #0]
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r6, r2, r3
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldrh	r3, [r3, #10]
	cmp	r3, #6
	bne.n	.L_0803ea2a
	cmp	r6, #0
	bne.n	.L_0803ea22
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_0803ea30
.L_0803ea22:
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_0803ea30
.L_0803ea2a:
	movs	r0, #112
	bl	sub_081c0010
.L_0803ea30:
	adds	r0, r6, #0
	b.n	.L_0803ea4e
.L_0803ea34:
	cmp	r7, #0
	beq.n	.L_0803e9a2
	ldr	r3, [pc, #20]
	movs	r2, #2
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803e9a2
	movs	r0, #113
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
.L_0803ea4e:
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #192
	adds	r5, r0, #0
	lsls	r1, r1, #2
	movs	r0, #231
	adds	r1, #158
	lsls	r0, r0, #2
	adds	r7, r5, r0
	adds	r6, r5, r1
	ldrh	r3, [r7, #0]
	ldrh	r1, [r6, #0]
	movs	r2, #229
	adds	r3, r3, r1
	lsls	r2, r2, #2
	adds	r3, #1
	adds	r2, r2, r5
	mov	sl, r3
	ldrh	r3, [r2, #0]
	mov	r8, r2
	cmp	sl, r3
	beq.n	.L_0803eb22
	adds	r0, r5, #0
	bl	sub_0803ef48
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #162
	adds	r2, r5, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldrh	r1, [r6, #0]
	movs	r0, #128
	adds	r3, r1, #1
	strh	r3, [r6, #0]
	lsls	r0, r0, #11
	lsls	r3, r3, #16
	cmp	r3, r0
	bne.n	.L_0803eaea
	mov	r0, r8
	ldrh	r3, [r0, #0]
	mov	r2, sl
	adds	r2, #1
	cmp	r2, r3
	bcs.n	.L_0803eaea
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r1, r2
	strh	r3, [r6, #0]
	movs	r3, #8
	strh	r3, [r5, #60]
	ldrh	r3, [r7, #0]
	adds	r0, r5, #0
	adds	r3, #1
	strh	r3, [r7, #0]
	movs	r1, #1
	bl	sub_0803f004
	ldrh	r2, [r6, #0]
	ldrh	r3, [r7, #0]
	mov	r0, r8
	adds	r3, r3, r2
	ldrh	r2, [r0, #0]
	adds	r3, #2
	cmp	r3, r2
	bne.n	.L_0803eae6
	movs	r3, #0
	strh	r3, [r5, #62]
.L_0803eae6:
	movs	r3, #1
	strh	r3, [r5, #10]
.L_0803eaea:
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #162
	adds	r2, r5, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #158
	adds	r3, r5, r2
	ldrh	r1, [r3, #0]
	adds	r0, r5, #0
	bl	sub_0803ef8c
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #210
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	sub_0803e5a8
	movs	r0, #1
	bl	sub_08013560
.L_0803eb22:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
