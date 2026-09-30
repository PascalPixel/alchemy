.syntax unified
	.thumb
	.global Func_080f948c
	.thumb_func
Func_080f948c:
	push {lr}
	movs r0, #2
	bl UiText_DrawNumberInWindowFar + 0x8
	pop {pc}
	.2byte 0x0000
