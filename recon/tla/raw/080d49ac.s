.syntax unified
	.thumb
	.global Func_080d49ac
	.thumb_func
Func_080d49ac:
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bx lr
	.2byte 0x0000
