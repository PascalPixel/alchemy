.syntax unified
	.thumb
	.global Func_080d1d34
	.thumb_func
Func_080d1d34:
	push {lr}
	ldr r0, .L_080d1d40
	bl Scheduler_EnableCallbacks
	pop {pc}
	.2byte 0x0000
.L_080d1d40:
	.4byte Func_080d1ae4
