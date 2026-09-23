.syntax unified
	.thumb
	.global Func_081234a4
	.thumb_func
Func_081234a4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #44
	movs	r1, #0
	adds	r3, r2, r0
	subs	r0, #4
	str	r1, [r3, #0]
	adds	r3, r2, r0
	adds	r0, #8
	str	r1, [r3, #0]
	adds	r3, r2, r0
	adds	r0, #4
	str	r1, [r3, #0]
	adds	r3, r2, r0
	subs	r0, #16
	str	r1, [r3, #0]
	adds	r3, r2, r0
	str	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #76
	adds	r0, r2, r3
	movs	r3, #134
	strh	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #78
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	movs	r3, #133
	lsls	r3, r3, #4
	adds	r2, r2, r3
	str	r1, [r2, #0]
	bx	lr
	.2byte 0x0000
