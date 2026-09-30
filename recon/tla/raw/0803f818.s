.syntax unified
	.thumb
	.global Func_0803f818
	.thumb_func
Func_0803f818:
	push {lr}
	bl Func_0803dc1c
	bl Func_0803dd24
	movs r0, #1
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
