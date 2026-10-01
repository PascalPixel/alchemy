.syntax unified
	.thumb
	.global Func_08040ff4
	.thumb_func
Func_08040ff4:
	push {lr}
	ldr r0, .L_08041000
	movs r1, #20
	bl Event_SetPairWork1c0Far
	pop {pc}
.L_08041000:
	.4byte 0x0000011e
