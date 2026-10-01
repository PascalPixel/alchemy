.syntax unified
	.thumb
	.global Func_08040fe4
	.thumb_func
Func_08040fe4:
	push {lr}
	ldr r0, .L_08040ff0
	movs r1, #1
	bl Event_SetPairWork1c0Far
	pop {pc}
.L_08040ff0:
	.4byte 0x00000129
