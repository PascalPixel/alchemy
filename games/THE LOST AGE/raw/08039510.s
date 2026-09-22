.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_08038edc, 0x08038edc
	.global Overlay_08039510
Overlay_08039510:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_08038edc
	ldrb	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_08039540
	ldrb	r0, [r5, #14]
	bl	sub_08014274
	ldrb	r3, [r5, #4]
	cmp	r3, #2
	bne.n	.L_08039540
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #60]
	ldrb	r3, [r5, #25]
	movs	r2, #155
	lsrs	r3, r3, #4
	lsls	r2, r2, #5
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #8]
	strh	r2, [r1, r3]
.L_08039540:
	movs	r3, #0
	strb	r3, [r5, #5]
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x03e7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	mov	lr, r1
	movs	r1, #215
	lsls	r1, r1, #3
	adds	r4, r6, r1
	ldr	r3, [r4, #0]
	adds	r7, r0, #0
	ldr	r5, [sp, #16]
	movs	r0, #0
	movs	r1, #0
	b.n	.L_08039574
.L_0803956a:
	adds	r1, #1
	adds	r4, #40
	cmp	r1, #3
	beq.n	.L_0803957a
	ldr	r3, [r4, #0]
.L_08039574:
	cmp	r3, #0
	bne.n	.L_0803956a
	adds	r0, r4, #0
.L_0803957a:
	cmp	r0, #0
	beq.n	.L_080395f8
	lsls	r3, r2, #8
	mov	r1, ip
	strh	r3, [r0, #30]
	strh	r3, [r0, #4]
	lsls	r3, r1, #8
	strh	r3, [r0, #6]
	mov	r3, lr
	strh	r3, [r0, #18]
	movs	r3, #15
	strh	r3, [r0, #22]
	movs	r3, #10
	strh	r3, [r0, #26]
	ldr	r3, [sp, #20]
	movs	r2, #0
	movs	r1, #152
	strh	r3, [r0, #36]
	str	r7, [r0, #0]
	strh	r2, [r0, #20]
	strh	r2, [r0, #24]
	strh	r2, [r0, #32]
	strh	r2, [r0, #38]
	lsls	r1, r1, #5
	adds	r1, #140
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080395c0
	cmp	r5, #0
	beq.n	.L_080395da
	ldrh	r3, [r5, #6]
	strh	r3, [r0, #38]
.L_080395c0:
	cmp	r5, #0
	beq.n	.L_080395da
	adds	r2, r0, #0
	movs	r1, #0
	adds	r2, #8
.L_080395ca:
	ldrh	r3, [r5, #0]
	adds	r1, #1
	strh	r3, [r2, #0]
	adds	r5, #2
	adds	r2, #2
	cmp	r1, #3
	bls.n	.L_080395ca
	b.n	.L_080395f4
.L_080395da:
	ldr	r2, [pc, #20]
	adds	r3, r0, #0
	movs	r1, #0
	adds	r3, #8
.L_080395e2:
	adds	r1, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r1, #3
	bls.n	.L_080395e2
	b.n	.L_080395f4
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080395f4:
	movs	r3, #0
	strh	r3, [r0, #16]
.L_080395f8:
	pop	{r5, r6, r7, pc}
