.syntax unified
	.thumb
	.set sub_08014644, 0x08014644
	.set sub_08038348, 0x08038348
	.global Func_080dcadc
	.thumb_func
Func_080dcadc:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [pc, #76]
	ldr	r6, [r3, #60]
	bl	sub_08014644
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #48]
	ldr	r5, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	adds	r2, #20
	strh	r5, [r3, #0]
	ldr	r3, [pc, #44]
	movs	r1, #147
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #38
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	sub_08038348
	strb	r5, [r6, #4]
	b.n	.L_080dcb40
	.4byte 0x00007fff
	.4byte 0x00000000
	.4byte 0x0000294a
	.4byte 0x00005294
	.4byte 0x080dcdc9
	.4byte 0x050001e2
	.4byte 0x050001e6
	.2byte 0x0240
	.2byte 0x0200
.L_080dcb40:
	pop	{r5, r6, pc}
	.2byte 0x0000
