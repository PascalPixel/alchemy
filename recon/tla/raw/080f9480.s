.syntax unified
	.thumb
	.global Func_080f9480
	.thumb_func
Func_080f9480:
	push {lr}
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	pop {pc}
	.2byte 0x0000
