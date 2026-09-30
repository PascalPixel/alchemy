.syntax unified
	.thumb
	.global Func_080d2a8c
	.thumb_func
Func_080d2a8c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
