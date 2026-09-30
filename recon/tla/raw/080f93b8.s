.syntax unified
	.thumb
	.global Func_080f93b8
	.thumb_func
Func_080f93b8:
	push {lr}
	bl Audio_PlayCue
	movs r0, #1
	pop {pc}
	.2byte 0x0000
