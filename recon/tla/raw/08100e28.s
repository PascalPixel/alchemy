.syntax unified
	.thumb
	.global Func_08100e28
	.thumb_func
Func_08100e28:
	push {lr}
	adds r0, r0, r1
	bl Math_Mod
	pop {pc}
	.2byte 0x0000
