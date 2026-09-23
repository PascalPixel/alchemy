.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803e5a8, 0x0803e5a8
	.set sub_0803ef48, 0x0803ef48
	.set sub_0803ef8c, 0x0803ef8c
	.set sub_0803f004, 0x0803f004
	.set sub_0803f2e4, 0x0803f2e4
	.global Func_0803ebdc
	.thumb_func
Func_0803ebdc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #158
	adds	r5, r7, r0
	ldrh	r1, [r5, #0]
	adds	r0, r7, #0
	bl	sub_0803ef48
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #162
	movs	r3, #0
	adds	r2, r7, r1
	mov	sl, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldrh	r3, [r5, #0]
	movs	r0, #229
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r0, r0, #2
	adds	r0, r0, r7
	ldrh	r2, [r0, #0]
	mov	r8, r0
	cmp	r2, #5
	bhi.n	.L_0803ec22
	b.n	.L_0803ed4a
.L_0803ec22:
	movs	r1, #231
	lsls	r1, r1, #2
	adds	r6, r7, r1
	ldrh	r3, [r6, #0]
	ldrh	r2, [r5, #0]
	ldrh	r1, [r0, #0]
	adds	r3, r3, r2
	cmp	r3, r1
	bne.n	.L_0803ed0a
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r5, [r3, #0]
	mov	r3, sl
	strh	r3, [r7, #10]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_0803ec6c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #150
	adds	r1, r7, r0
	movs	r3, #230
	movs	r0, #255
	lsls	r3, r3, #2
	lsls	r0, r0, #8
	adds	r2, r7, r3
	adds	r0, #244
.L_0803ec5a:
	ldrh	r3, [r1, #0]
	strh	r3, [r5, #24]
	ldrh	r3, [r2, #0]
	strh	r0, [r5, #20]
	strh	r3, [r5, #26]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0803ec5a
.L_0803ec6c:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #150
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	movs	r1, #230
	strh	r2, [r5, #24]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #16
	strh	r3, [r5, #26]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #244
	strh	r3, [r5, #20]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	asrs	r2, r2, #16
	cmp	r2, r3
	beq.n	.L_0803eca8
.L_0803ec96:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #24
	ldrsh	r2, [r5, r1]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	cmp	r2, r3
	bne.n	.L_0803ec96
.L_0803eca8:
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0803ecce
	movs	r2, #213
	lsls	r2, r2, #2
	adds	r6, r7, r2
.L_0803ecba:
	ldrh	r0, [r6, #0]
	ldrh	r1, [r6, #32]
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_0803f2e4
	ldr	r5, [r5, #4]
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_0803ecba
.L_0803ecce:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #158
	movs	r1, #231
	adds	r3, r7, r0
	movs	r2, #0
	lsls	r1, r1, #2
	strh	r2, [r3, #0]
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r5, [r3, #0]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	ldr	r5, [r5, #4]
	adds	r3, #16
	cmp	r5, #0
	beq.n	.L_0803ed04
	movs	r2, #12
.L_0803ecf8:
	strh	r3, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	adds	r3, #16
	cmp	r5, #0
	bne.n	.L_0803ecf8
.L_0803ed04:
	movs	r3, #1
	strh	r3, [r7, #62]
	b.n	.L_0803ed56
.L_0803ed0a:
	cmp	r2, #4
	bne.n	.L_0803ed56
	adds	r3, #1
	cmp	r3, r1
	bcs.n	.L_0803ed56
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r5, #0]
	movs	r3, #8
	strh	r3, [r7, #60]
	adds	r0, r7, #0
	ldrh	r3, [r6, #0]
	movs	r1, #1
	adds	r3, #1
	strh	r3, [r6, #0]
	bl	sub_0803f004
	ldrh	r2, [r5, #0]
	ldrh	r3, [r6, #0]
	mov	r0, r8
	adds	r3, r3, r2
	ldrh	r2, [r0, #0]
	adds	r3, #2
	cmp	r3, r2
	bne.n	.L_0803ed44
	mov	r1, sl
	strh	r1, [r7, #62]
.L_0803ed44:
	movs	r3, #1
	strh	r3, [r7, #10]
	b.n	.L_0803ed56
.L_0803ed4a:
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_0803ed56
	mov	r2, sl
	strh	r2, [r5, #0]
.L_0803ed56:
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #162
	adds	r2, r7, r3
	movs	r0, #192
	movs	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #2
	adds	r0, #158
	adds	r3, r7, r0
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_0803ef8c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	sub_0803e5a8
	movs	r0, #1
	bl	sub_08013560
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
