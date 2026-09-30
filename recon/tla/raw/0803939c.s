.syntax unified
	.thumb
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L_080393fa
	bl	RenderOutput_PrepareForRedraw
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
	bl	0x0803911c
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
