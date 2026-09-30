.syntax unified
	.thumb
	.global Func_080db9c0
	.thumb_func
Func_080db9c0:
	ldr r3, [r0, #80]
	ldrb r0, [r3, #9]
	lsls r0, r0, #28
	lsrs r0, r0, #30
	bx lr
	.2byte 0x0000
