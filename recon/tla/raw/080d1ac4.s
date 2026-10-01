.syntax unified
	.thumb
	.global Func_080d1ac4
	.thumb_func
Func_080d1ac4:
	push {lr}
	ldr r0, .L_080d1ad0
	bl Scheduler_EnableCallbacks
	pop {pc}
	.2byte 0x0000
.L_080d1ad0:
	.4byte Func_080d1840
