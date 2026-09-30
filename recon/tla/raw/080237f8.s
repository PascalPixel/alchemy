.syntax unified
	.thumb
	.global Func_080237f8
	.thumb_func
Func_080237f8:
	push	{lr}
	ldr	r0, [pc, #60]
	bl	0x08014694
	ldr	r0, [pc, #56]
	bl	0x08014694
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl	Func_080c8378
	movs	r0, #1
	bl	Func_080c8390
	movs	r0, #1
	bl	WaitFrames
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
	bl	0x0801475c
	ldr	r0, [pc, #24]
	bl	0x0801475c
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
	.2byte 0x3e19
	.2byte 0x0802
	movs	r0, #1
	bx	lr
