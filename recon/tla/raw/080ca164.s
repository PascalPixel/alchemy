.syntax unified
	.thumb
	.global Func_080ca164
	.thumb_func
Func_080ca164:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #198
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrb r0, [r3, r0]
	bl Func_080c9fd8
	pop {pc}
	.2byte 0x0000
