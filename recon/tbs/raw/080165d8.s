.syntax unified
	.thumb
	.global UiText_QueueRenderEntries
	.thumb_func
UiText_QueueRenderEntries:
Func_080165d8:
	push	{r5, r6, r7, lr}
	adds	r7, r3, #0
	ldr	r3, [pc, #104]
	mov	ip, r1
	ldr	r3, [r3, #0]
	movs	r1, #196
	lsls	r1, r1, #3
	adds	r4, r3, r1
	ldr	r3, [r4, #0]
	adds	r6, r0, #0
	ldr	r5, [sp, #16]
	movs	r0, #0
	movs	r1, #0
	b.n	.L_080165fe
.L_080165f4:
	adds	r1, #1
	adds	r4, #40
	cmp	r1, #3
	beq.n	.L_08016604
	ldr	r3, [r4, #0]
.L_080165fe:
	cmp	r3, #0
	bne.n	.L_080165f4
	adds	r0, r4, #0
.L_08016604:
	cmp	r0, #0
	beq.n	.L_08016668
	lsls	r3, r2, #8
	strh	r3, [r0, #30]
	strh	r3, [r0, #4]
	lsls	r3, r7, #8
	strh	r3, [r0, #6]
	mov	r3, ip
	strh	r3, [r0, #18]
	movs	r3, #15
	strh	r3, [r0, #22]
	movs	r3, #10
	strh	r3, [r0, #26]
	ldr	r3, [sp, #20]
	movs	r2, #0
	str	r6, [r0, #0]
	strh	r2, [r0, #20]
	strh	r2, [r0, #24]
	strh	r2, [r0, #32]
	strh	r3, [r0, #36]
	cmp	r5, #0
	beq.n	.L_0801664c
	adds	r2, r0, #0
	movs	r1, #0
	adds	r2, #8
.L_08016636:
	ldrh	r3, [r5, #0]
	adds	r1, #1
	strh	r3, [r2, #0]
	adds	r5, #2
	adds	r2, #2
	cmp	r1, #3
	bls.n	.L_08016636
	b.n	.L_08016664
	movs	r0, r0
	.2byte 0x1e8c
	.2byte 0x0300
.L_0801664c:
	adds	r3, r0, #0
	ldr	r2, [pc, #16]
	movs	r1, #0
	adds	r3, #8
.L_08016654:
	adds	r1, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r1, #3
	bls.n	.L_08016654
	b.n	.L_08016664
	.2byte 0x0000
	.2byte 0x0000
.L_08016664:
	movs	r3, #0
	strh	r3, [r0, #16]
.L_08016668:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
