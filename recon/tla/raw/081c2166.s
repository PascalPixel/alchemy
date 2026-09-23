.syntax unified
	.thumb
	.set sub_081c2328, 0x081c2328
	.2byte 0x0000
	.4byte 0x04091c02
	.4byte 0x6b530c09
	.4byte 0x42834803
	.4byte 0x84d1d103
	.4byte 0x48028491
	.4byte 0x47708510
	.4byte 0x68736d53
	.4byte 0x00000101
	.4byte 0x04091c02
	.4byte 0x6b530c09
	.4byte 0x42834805
	.4byte 0x84d1d107
	.4byte 0x20028491
	.4byte 0x68508510
	.4byte 0x40084902
	.4byte 0x47706050
	.4byte 0x68736d53
	.2byte 0xffff
	.2byte 0x7fff
	push	{r4, r5, r6, r7, lr}
	ldrb	r5, [r0, #8]
	ldr	r4, [r0, #44]
	cmp	r5, #0
	ble.n	.L_081c21f2
	movs	r7, #128
.L_081c21bc:
	ldrb	r1, [r4, #0]
	adds	r0, r7, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_081c21ea
	movs	r6, #64
	adds	r0, r6, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_081c21ea
	adds	r0, r4, #0
	bl	sub_081c2328
	strb	r7, [r4, #0]
	movs	r0, #2
	strb	r0, [r4, #15]
	strb	r6, [r4, #19]
	movs	r0, #22
	strb	r0, [r4, #25]
	adds	r1, r4, #0
	adds	r1, #36
	movs	r0, #1
	strb	r0, [r1, #0]
.L_081c21ea:
	subs	r5, #1
	adds	r4, #80
	cmp	r5, #0
	bgt.n	.L_081c21bc
.L_081c21f2:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
