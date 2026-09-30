.syntax unified
	.thumb
	.global Func_080ae3fc
	.thumb_func
Func_080ae3fc:
	lsls r3, r1, #20
	movs r2, #7
	ands r2, r1
	lsrs r1, r3, #23
	ldrb r0, [r0, r1]
	movs r3, #1
	asrs r0, r2
	ands r0, r3
	bx lr
	.2byte 0x0000
