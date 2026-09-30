.syntax unified
	.thumb
	.global Func_080d49a0
	.thumb_func
Func_080d49a0:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r0, #0
	pop {pc}
