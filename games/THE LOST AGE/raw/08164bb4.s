.syntax unified
	.thumb
	.set sub_08164cc4, 0x08164cc4
	.global Overlay_08164bb4
Overlay_08164bb4:
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
	movs	r1, #160
	ldrh	r2, [r3, #10]
	movs	r2, #236
	lsls	r2, r2, #7
	adds	r2, #64
	adds	r0, r0, r2
	lsls	r1, r1, #19
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.2byte 0x0001
	.2byte 0xa260
	push	{lr}
	movs	r1, #0
	bl	sub_08164cc4
	pop	{pc}
