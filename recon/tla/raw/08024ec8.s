.syntax unified
	.thumb
	.global Func_08024ec8
	.thumb_func
Func_08024ec8:
	push {lr}
	bl ObjectDispatch_Release
	movs r0, #0
	pop {pc}
	.2byte 0x0000
