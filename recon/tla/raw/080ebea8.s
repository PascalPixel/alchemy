.syntax unified
	.thumb
	.global Func_080ebea8
	.thumb_func
Func_080ebea8:
	push {lr}
	ldr r0, [r0]
	bl Animation_ApplyChildArgumentFar
	pop {pc}
	.2byte 0x0000
