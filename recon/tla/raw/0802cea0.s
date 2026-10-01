.syntax unified
	.thumb
	.global Func_0802cea0
	.thumb_func
Func_0802cea0:
	push {lr}
	ldr r0, .L_0802ceac
	bl Scheduler_EnableCallbacks
	pop {pc}
	.2byte 0x0000
.L_0802ceac:
	.4byte Func_0802cd94
