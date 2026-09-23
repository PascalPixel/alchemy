.syntax unified
	.thumb
	.global Func_08049a04
	.thumb_func
Func_08049a04:
	adds	r3, r0, #0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #255
	ands	r1, r0
	movs	r2, #192
	ands	r0, r3
	lsls	r2, r2, #19
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	adds	r0, r0, r2
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.2byte 0x0000
