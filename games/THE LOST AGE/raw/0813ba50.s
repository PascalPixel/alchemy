.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.global Overlay_0813ba50
Overlay_0813ba50:
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #40]
	movs	r3, #128
	ldr	r2, [pc, #40]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	b.n	.L_0813ba98
	.4byte 0x00000000
	.4byte 0x0000100e
	.4byte 0x000000f0
	.4byte 0x00001088
	.2byte 0x3537
	.2byte 0x0000
.L_0813ba98:
	strh	r3, [r2, #0]
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #20]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #24]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0813bae2
	b.n	.L_0813bac0
	movs	r0, r0
	.4byte 0x00003f21
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0813bac0:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0813bae2:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	sub_08013560
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #144
	adds	r5, r4, r1
	ldr	r3, [r5, #0]
	adds	r1, #4
	adds	r3, #1
	str	r3, [r5, #0]
	adds	r2, r4, r1
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_0813bb32
	movs	r2, #238
	ldr	r1, [pc, #36]
	lsls	r2, r2, #7
	adds	r2, #152
	adds	r3, r4, r2
	ldr	r2, [r3, #0]
	ldrh	r3, [r1, #4]
	movs	r0, #0
	adds	r3, r3, r2
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #156
	strh	r3, [r1, #4]
	adds	r3, r4, r2
	ldr	r2, [r3, #0]
	ldrh	r3, [r1, #6]
	str	r0, [r5, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #6]
.L_0813bb32:
	pop	{r5, pc}
	.2byte 0x1120
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #144
	adds	r5, r4, r1
	ldr	r3, [r5, #0]
	adds	r1, #4
	adds	r3, #1
	str	r3, [r5, #0]
	adds	r2, r4, r1
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_0813bb9c
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #208
	adds	r1, r4, r3
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #212
	adds	r0, r4, r3
	ldr	r3, [r0, #0]
	adds	r2, #4
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #152
	adds	r2, r4, r3
	ldr	r2, [r2, #0]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #156
	adds	r2, r4, r1
	ldr	r3, [r0, #0]
	ldr	r2, [r2, #0]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r5, #0]
.L_0813bb9c:
	pop	{r5, pc}
