.syntax unified
	.thumb
	.global Func_080fbe24
	.thumb_func
Func_080fbe24:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #4
	adds r3, #240
	ldr r0, [r3]
	movs r3, #7
	str r3, [sp, #0]
	movs r1, #13
	movs r2, #0
	movs r3, #17
	bl UiWindow_SetBounds
	add sp, #4
	pop {pc}
	.2byte 0x0000
