.syntax unified
	.thumb
	.global Func_080f9480
	.thumb_func
Func_080f9480:
	push {lr}
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	pop {pc}
	.2byte 0x0000
