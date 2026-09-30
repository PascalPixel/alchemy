.syntax unified
	.thumb
	.global Func_080fbd9c
	.thumb_func
Func_080fbd9c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #4
	ldr r0, [r3, #36]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #13
	movs r2, #5
	movs r3, #17
	bl UiWindow_SetBounds
	add sp, #4
	pop {pc}
