.syntax unified
	.thumb
	.global Func_080dc7cc
	.thumb_func
Func_080dc7cc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	ldr r0, [r3, #16]
	bl Func_080e1420
	movs r0, #1
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
