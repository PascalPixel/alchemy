.syntax unified
	.thumb
	.global Func_080d1ad4
	.thumb_func
Func_080d1ad4:
	push {lr}
	ldr r0, .L_080d1ae0
	bl Scheduler_DisableCallbacks
	pop {pc}
	.2byte 0x0000
.L_080d1ae0:
	.4byte Func_080d1840
