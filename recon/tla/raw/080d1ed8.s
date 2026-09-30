.syntax unified
	.thumb
	.global Func_080d1ed8
	.thumb_func
Func_080d1ed8:
	push {lr}
	bl Func_080d1e84
	bl Func_080d1e60
	ldrb r0, [r0, #3]
	pop {pc}
	.2byte 0x0000
