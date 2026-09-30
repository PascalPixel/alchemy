.syntax unified
	.thumb
	.global Func_0803f800
	.thumb_func
Func_0803f800:
	push {lr}
	movs r0, #1
	bl Func_08042690
	movs r0, #1
	bl WaitFrames
	pop {pc}
