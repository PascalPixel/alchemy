.syntax unified
	.thumb
	.global Func_08100d40
	.thumb_func
Func_08100d40:
	push {lr}
	bl Item_Get
	ldrh r3, [r0, #40]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl Func_08100d58
	pop {pc}
	.2byte 0x0000
