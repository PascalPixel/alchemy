.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014694, 0x08014694
	.set sub_0801475c, 0x0801475c
	.set sub_080c8378, 0x080c8378
	.set sub_080c8390, 0x080c8390
	.global Overlay_080237f8
Overlay_080237f8:
	push	{lr}
	ldr	r0, [pc, #60]
	bl	sub_08014694
	ldr	r0, [pc, #56]
	bl	sub_08014694
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl	sub_080c8378
	movs	r0, #1
	bl	sub_080c8390
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #241
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	ldr	r2, [pc, #8]
	orrs	r3, r2
	strh	r3, [r1, #0]
	pop	{pc}
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x0802386d
	.2byte 0x3e19
	.2byte 0x0802
	push	{lr}
	ldr	r0, [pc, #28]
	bl	sub_0801475c
	ldr	r0, [pc, #24]
	bl	sub_0801475c
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #225
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	pop	{pc}
	.4byte 0x0802386d
	.4byte 0x08023e19
