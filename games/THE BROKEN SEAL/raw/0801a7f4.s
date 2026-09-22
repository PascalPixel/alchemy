.syntax unified
	.thumb
	.set sub_0801a910, 0x0801a910
	.set sub_0801bd98, 0x0801bd98
	.set sub_0801c188, 0x0801c188
	.global Overlay_0801a7f4
Overlay_0801a7f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	mov	r9, r3
	movs	r3, #229
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r3, #0]
	sub	sp, #8
	str	r3, [sp, #4]
	movs	r3, #231
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r2, #213
	mov	sl, r3
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, r9
	lsls	r2, r2, #2
	mov	fp, r6
	adds	r4, r3, r2
	b.n	.L_0801a86c
.L_0801a82e:
	adds	r2, r5, #0
	movs	r3, #0
	adds	r0, r7, #0
	mov	r1, r8
	str	r4, [sp, #0]
	bl	sub_0801bd98
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bne.n	.L_0801a854
	str	r5, [r3, #0]
	str	r2, [r5, #0]
	b.n	.L_0801a858
	.2byte 0x1e98
	.2byte 0x0300
.L_0801a854:
	str	r5, [r6, #4]
	str	r6, [r5, #0]
.L_0801a858:
	movs	r3, #0
	str	r3, [r5, #4]
	movs	r3, #1
	add	fp, r3
	mov	r2, fp
	adds	r6, r5, #0
	cmp	r2, #5
	beq.n	.L_0801a888
	adds	r4, #2
	add	sl, r3
.L_0801a86c:
	ldr	r3, [sp, #4]
	cmp	sl, r3
	bcs.n	.L_0801a888
	ldrh	r2, [r4, #32]
	movs	r0, #0
	ldrh	r7, [r4, #0]
	str	r4, [sp, #0]
	mov	r8, r2
	bl	sub_0801a910
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	cmp	r5, #0
	bne.n	.L_0801a82e
.L_0801a888:
	mov	r3, fp
	lsls	r2, r3, #3
	ldr	r3, [pc, #48]
	ldr	r1, [pc, #52]
	subs	r3, r3, r2
	movs	r2, #230
	add	r1, r9
	lsls	r2, r2, #2
	strh	r3, [r1, #0]
	add	r2, r9
	movs	r3, #140
	strh	r3, [r2, #0]
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r6, [r3, #0]
	movs	r3, #0
	mov	fp, r3
	cmp	r6, #0
	beq.n	.L_0801a8f4
	movs	r0, #238
	lsls	r0, r0, #2
	adds	r5, r1, #0
	adds	r4, r2, #0
	add	r0, r9
	movs	r1, #0
	b.n	.L_0801a8c8
	movs	r0, r0
	.4byte 0x00000064
	.2byte 0x0396
	.2byte 0x0000
.L_0801a8c8:
	ldrh	r3, [r5, #0]
	add	r3, fp
	strh	r3, [r6, #16]
	ldrh	r2, [r4, #0]
	strh	r2, [r6, #18]
	strh	r2, [r6, #26]
	ldrh	r2, [r6, #10]
	strh	r3, [r6, #24]
	cmp	r2, #6
	bne.n	.L_0801a8e6
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0801a8e6
	strh	r2, [r6, #18]
	strh	r2, [r6, #26]
.L_0801a8e6:
	strh	r1, [r6, #20]
	strh	r1, [r6, #22]
	ldr	r6, [r6, #4]
	movs	r2, #16
	add	fp, r2
	cmp	r6, #0
	bne.n	.L_0801a8c8
.L_0801a8f4:
	bl	sub_0801c188
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
