.syntax unified
	.thumb
	.global Func_080dbd48
	.thumb_func
Func_080dbd48:
	push {lr}
	bl Func_080dbb78
	ldrb r3, [r0, #3]
	movs r0, #64
	ands r0, r3
	lsls r0, r0, #24
	lsrs r0, r0, #24
	pop {pc}
	.2byte 0x0000
