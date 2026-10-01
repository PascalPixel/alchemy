.syntax unified
	.thumb
	.global Func_080d0184
	.thumb_func
Func_080d0184:
	push {lr}
	ldr r0, .L_080d0194
	bl Scheduler_EnableCallbacks
	ldr r0, .L_080d0198
	bl Scheduler_EnableCallbacks
	pop {pc}
.L_080d0194:
	.4byte Func_080cf78c
.L_080d0198:
	.4byte Func_080cf6fc
