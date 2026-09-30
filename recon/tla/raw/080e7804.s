.syntax unified
	.thumb
	.global Func_080e7804
	.thumb_func
Func_080e7804:
	push {lr}
	bl Func_080e74d8
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #10
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
