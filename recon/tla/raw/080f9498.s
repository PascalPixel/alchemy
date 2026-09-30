.syntax unified
	.thumb
	.global Func_080f9498
	.thumb_func
Func_080f9498:
	push {lr}
	movs r0, #4
	bl UiWork_SetParamNibbleFar
	pop {pc}
	.2byte 0x0000
