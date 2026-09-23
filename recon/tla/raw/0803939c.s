.syntax unified
	.thumb
	.set sub_0803911c, 0x0803911c
	.set sub_08039418, 0x08039418
	.set sub_08039430, 0x08039430
	.global Func_0803939c
	.thumb_func
Func_0803939c:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L_080393fa
	bl	.L_080393fc
	ldrh	r3, [r5, #8]
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	strh	r3, [r5, #32]
	ldrh	r3, [r5, #10]
	movs	r6, #0
	strh	r6, [r5, #22]
	strh	r0, [r5, #28]
	strh	r1, [r5, #30]
	strh	r3, [r5, #34]
	cmp	r7, #0
	beq.n	.L_080393f4
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	asrs	r0, r0, #16
	asrs	r1, r1, #16
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	sub_0803911c
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	strh	r6, [r5, #8]
	strh	r6, [r5, #10]
	strh	r6, [r5, #12]
	strh	r6, [r5, #14]
	strh	r6, [r5, #16]
	strh	r6, [r5, #18]
	strh	r6, [r5, #20]
	strh	r6, [r5, #22]
	strh	r6, [r5, #24]
	strh	r6, [r5, #26]
	strh	r6, [r5, #28]
	strh	r6, [r5, #30]
	strh	r6, [r5, #32]
	strh	r6, [r5, #34]
	b.n	.L_080393fa
.L_080393f4:
	movs	r3, #4
	strh	r7, [r5, #24]
	strh	r3, [r5, #26]
.L_080393fa:
	pop	{r5, r6, r7, pc}
.L_080393fc:
	push	{r5, lr}
	adds	r5, r0, #0
	ldrh	r2, [r5, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08039414
	bl	sub_08039418
	adds	r0, r5, #0
	bl	sub_08039430
.L_08039414:
	pop	{r5, pc}
	.align 2, 0
