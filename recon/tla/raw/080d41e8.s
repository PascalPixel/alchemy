.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014d00, 0x08014d00
	.set sub_080200d8, 0x080200d8
	.set sub_08020120, 0x08020120
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038050, 0x08038050
	.set sub_080380f8, 0x080380f8
	.set sub_08038100, 0x08038100
	.set sub_08038140, 0x08038140
	.set sub_080cad84, 0x080cad84
	.set sub_080d1eac, 0x080d1eac
	.set sub_080d3bf8, 0x080d3bf8
	.global Func_080d41e8
	.thumb_func
Func_080d41e8:
	push	{lr}
	bl	sub_080d3bf8
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080d41fa
	bl	sub_08038100
.L_080d41fa:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	mov	r8, r2
	adds	r6, r1, #0
	ldr	r5, [sp, #40]
	ldr	r7, [sp, #60]
	mov	sl, r3
	bl	sub_080d3bf8
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_080d3bf8
	mov	r9, r0
	mov	r0, fp
	bl	sub_080d1eac
	movs	r1, #226
	lsls	r1, r1, #1
	add	r1, sl
	adds	r3, r0, #0
	ldrh	r0, [r1, #0]
	lsls	r3, r3, #16
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r1, #0]
	asrs	r0, r0, #16
	mov	r2, r8
	adds	r1, r6, #0
	bl	sub_08038038
	movs	r1, #0
	mov	r8, r0
	ldr	r2, [sp, #0]
	mov	r0, fp
	ldr	r3, [sp, #36]
	bl	sub_080380f8
	cmp	r7, #0
	beq.n	.L_080d4270
	b.n	.L_080d4268
.L_080d4262:
	adds	r0, r7, #0
	bl	sub_08013560
.L_080d4268:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_080d4262
.L_080d4270:
	mov	r0, r9
	bl	sub_080d1eac
	movs	r1, #226
	lsls	r1, r1, #1
	add	r1, sl
	adds	r3, r0, #0
	ldrh	r0, [r1, #0]
	lsls	r3, r3, #16
	adds	r2, r0, #1
	strh	r2, [r1, #0]
	lsls	r0, r0, #16
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #48]
	asrs	r0, r0, #16
	bl	sub_08038038
	movs	r1, #0
	adds	r7, r0, #0
	ldr	r2, [sp, #52]
	mov	r0, r9
	ldr	r3, [sp, #56]
	bl	sub_080380f8
	b.n	.L_080d42a8
.L_080d42a2:
	movs	r0, #1
	bl	sub_08013560
.L_080d42a8:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_080d42a2
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #116]
	movs	r2, #129
	ldr	r3, [r1, #4]
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d42d8
	adds	r6, r1, #0
	adds	r5, r2, #0
.L_080d42ca:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #4]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080d42ca
.L_080d42d8:
	movs	r0, #1
	bl	sub_08013560
	mov	r0, fp
	bl	sub_08038100
	mov	r0, r9
	bl	sub_08038100
	bl	sub_08038140
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080d42fc
.L_080d42f6:
	movs	r0, #1
	bl	sub_08013560
.L_080d42fc:
	mov	r0, r8
	bl	sub_08038050
	cmp	r0, #0
	beq.n	.L_080d42f6
	b.n	.L_080d430e
.L_080d4308:
	movs	r0, #1
	bl	sub_08013560
.L_080d430e:
	adds	r0, r7, #0
	bl	sub_08038050
	cmp	r0, #0
	beq.n	.L_080d4308
	movs	r0, #1
	bl	sub_08013560
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	movs	r1, #128
	lsls	r3, r3, #18
	lsls	r1, r1, #24
	ldr	r5, [r3, #60]
	cmp	r0, r1
	bne.n	.L_080d4354
	movs	r2, #152
	lsls	r2, r2, #5
	movs	r1, #152
	adds	r2, #132
	lsls	r1, r1, #5
	adds	r3, r5, r2
	adds	r1, #134
	movs	r2, #0
	strh	r2, [r3, #0]
	b.n	.L_080d4376
.L_080d4354:
	bl	sub_080d3bf8
	bl	sub_080d1eac
	ldr	r3, [pc, #28]
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #24]
	movs	r1, #152
	lsls	r1, r1, #5
	ldrb	r2, [r2, r3]
	adds	r1, #132
	adds	r3, r5, r1
	adds	r1, #2
	strh	r0, [r3, #0]
.L_080d4376:
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x330c
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r1
	bl	sub_080cad84
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	adds	r5, r0, #0
	mov	r8, r3
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r6, [r3, #0]
	ldr	r3, [r2, #32]
	cmp	r5, #0
	beq.n	.L_080d43e4
	adds	r7, r6, #0
	adds	r7, #8
	str	r7, [r3, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080200d8
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080d43e4
	ldr	r3, [r5, #8]
	movs	r0, #1
	str	r3, [r7, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #12]
	ldr	r3, [r5, #16]
	str	r3, [r6, #16]
	bl	sub_08013560
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080d43e4
	bl	sub_08020120
.L_080d43e4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r6, r1, #0
	movs	r1, #213
	adds	r5, r0, #0
	lsls	r1, r1, #4
	movs	r0, #108
	bl	sub_08014d00
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r3, [r0, #0]
	str	r5, [r3, #48]
	str	r6, [r3, #52]
	pop	{r5, r6, pc}
	.2byte 0x0000
