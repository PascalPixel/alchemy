.syntax unified
	.thumb
	.set sub_08039754, 0x08039754
	.global Func_080395fc
	.thumb_func
Func_080395fc:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r4, #215
	lsls	r4, r4, #3
	adds	r6, r1, #0
	movs	r5, #0
	adds	r1, r3, r4
	movs	r4, #0
	b.n	.L_08039616
.L_08039612:
	adds	r1, #40
	adds	r4, #1
.L_08039616:
	cmp	r4, #3
	beq.n	.L_08039628
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08039626
	ldrh	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_08039612
.L_08039626:
	adds	r5, r1, #0
.L_08039628:
	cmp	r5, #0
	beq.n	.L_0803969c
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08039640
	movs	r3, #160
	lsls	r3, r3, #4
	strh	r3, [r5, #6]
	movs	r3, #192
	lsls	r3, r3, #2
	str	r0, [r5, #0]
	b.n	.L_0803966c
.L_08039640:
	cmp	r2, #0
	bne.n	.L_0803966e
	ldrh	r3, [r5, #6]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L_08039654
	movs	r3, #160
	lsls	r3, r3, #4
	strh	r3, [r5, #6]
	b.n	.L_08039668
.L_08039654:
	movs	r1, #208
	lsls	r1, r1, #4
	cmp	r2, r1
	bcs.n	.L_08039662
	adds	r3, r3, r1
	strh	r3, [r5, #6]
	b.n	.L_08039668
.L_08039662:
	adds	r0, r5, #0
	bl	sub_08039754
.L_08039668:
	movs	r3, #192
	lsls	r3, r3, #2
.L_0803966c:
	strh	r3, [r5, #4]
.L_0803966e:
	movs	r3, #192
	lsls	r3, r3, #2
	strh	r3, [r5, #30]
	ldr	r3, [r5, #0]
	movs	r2, #0
	strh	r2, [r3, #20]
	movs	r3, #15
	strh	r3, [r5, #22]
	strh	r6, [r5, #18]
	movs	r3, #10
	strh	r2, [r5, #20]
	strh	r2, [r5, #24]
	strh	r2, [r5, #16]
	strh	r2, [r5, #32]
	strh	r3, [r5, #26]
	adds	r3, r5, #0
	movs	r4, #0
	adds	r3, #8
.L_08039692:
	adds	r4, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r4, #3
	bls.n	.L_08039692
.L_0803969c:
	adds	r0, r5, #0
	pop	{r5, r6, pc}
.L_080396a0:
	push	{lr}
	movs	r1, #240
	ldr	r3, [pc, #12]
	lsls	r1, r1, #4
	movs	r2, #0
	ldr	r0, [pc, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.4byte 0x03000260
	.2byte 0x2500
	.2byte 0x0600
	push	{lr}
	movs	r1, #240
	ldr	r3, [pc, #12]
	lsls	r1, r1, #4
	ldr	r2, [pc, #12]
	ldr	r0, [pc, #16]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.4byte 0x03000260
	.4byte 0x44444444
	.2byte 0x2500
	.2byte 0x0600
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r1, #215
	lsls	r1, r1, #3
	adds	r2, r3, r1
	movs	r5, #0
	movs	r1, #0
	b.n	.L_080396f4
.L_080396f0:
	adds	r2, #40
	adds	r1, #1
.L_080396f4:
	cmp	r1, #3
	beq.n	.L_08039706
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08039704
	ldrh	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080396f0
.L_08039704:
	adds	r5, r2, #0
.L_08039706:
	cmp	r5, #0
	beq.n	.L_08039728
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08039718
	bl	.L_080396a0
	movs	r3, #0
	strh	r3, [r5, #6]
.L_08039718:
	movs	r3, #0
	strh	r3, [r5, #4]
	strh	r3, [r5, #20]
	movs	r2, #15
	strh	r3, [r5, #24]
	movs	r3, #10
	strh	r2, [r5, #22]
	strh	r3, [r5, #26]
.L_08039728:
	pop	{r5, pc}
	.align 2, 0
