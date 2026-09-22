.syntax unified
	.thumb
	.global Overlay_080005e8
Overlay_080005e8:
	ldr	r0, [pc, #100]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #92]
	ldr	r3, [r0, #0]
	subs	r3, r3, r2
	cmp	r3, #1
	bhi.n	.L_08000626
	ldrb	r1, [r0, #4]
	subs	r1, #1
	strb	r1, [r0, #4]
	bgt.n	.L_08000626
	ldrb	r1, [r0, #11]
	strb	r1, [r0, #4]
	ldr	r2, [pc, #36]
	ldr	r1, [r2, #8]
	ldr	r0, [pc, #36]
	lsls	r1, r1, #7
	bcc.n	.L_0800060e
	str	r0, [r2, #8]
.L_0800060e:
	ldr	r1, [r2, #20]
	lsls	r1, r1, #7
	bcc.n	.L_08000616
	str	r0, [r2, #20]
.L_08000616:
	movs	r1, #4
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
	movs	r1, #182
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
.L_08000626:
	bx	lr
	.4byte 0x040000bc
	.2byte 0x0004
	.2byte 0x8440
	add	r1, pc, #20
	ldmia	r1!, {r0, r2, r3}
	mov	r1, sp
	cmp	r1, r0
	bcc.n	.L_08000642
	ldr	r0, [r3, #0]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_08000654
.L_08000642:
	movs	r0, #1
	bx	lr
	movs	r0, r0
	ldrb	r0, [r0, #4]
	lsls	r0, r0, #12
	.4byte 0x68736d53
	.2byte 0x7ff0
	.2byte 0x0300
.L_08000654:
	push	{r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L_08000670
	ldr	r0, [r0, #36]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9806
.L_08000670:
	ldr	r3, [r0, #40]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9806
	ldr	r3, [r0, #16]
	str	r3, [sp, #0]
	ldr	r5, [pc, #40]
	ldr	r4, [pc, #52]
	ldrb	r4, [r4, #1]
	subs	r7, r4, #1
	bls.n	.L_08000690
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	lsls	r2, r3, #1
	muls	r2, r1
	adds	r5, r5, r2
.L_08000690:
	str	r5, [sp, #8]
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #20]
	mov	r9, r0
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
	ldr	r3, [pc, #0]
	bx	r3
	.4byte 0x030007b5
	.2byte 0x3a90
	.2byte 0x0200
	lsls	r0, r2, #13
	movs	r0, r0
	movs	r6, r0
	lsls	r0, r0, #16
	.4byte 0x03001138
