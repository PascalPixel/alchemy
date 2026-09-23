.syntax unified
	.thumb
	.global Func_0803a404
	.thumb_func
Func_0803a404:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #62
	adds	r1, r2, r3
	movs	r3, #15
	strh	r3, [r1, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #56
	adds	r1, r2, r3
	movs	r3, #10
	strh	r3, [r1, #0]
	movs	r3, #154
	lsls	r3, r3, #5
	adds	r1, r2, r3
	movs	r3, #9
	strh	r3, [r1, #0]
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r1, #60
	adds	r3, r2, r1
	movs	r0, #0
	strh	r0, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #58
	adds	r2, r2, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	bx	lr
	.align 2, 0
