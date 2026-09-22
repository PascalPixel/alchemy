.syntax unified
	.thumb
	.set sub_0815265c, 0x0815265c
	.global Overlay_08152474
Overlay_08152474:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r1, #128
	ldrh	r2, [r3, #10]
	movs	r2, #225
	lsls	r2, r2, #7
	lsls	r1, r1, #19
	adds	r0, r0, r2
	adds	r1, #40
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.2byte 0x0001
	.2byte 0xa660
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r1, #128
	ldrh	r2, [r3, #10]
	movs	r2, #225
	lsls	r2, r2, #7
	lsls	r1, r1, #19
	adds	r0, r0, r2
	adds	r1, #20
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.2byte 0x0001
	.2byte 0xa660
	push	{lr}
	movs	r1, #0
	bl	sub_0815265c
	pop	{pc}
