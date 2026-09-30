.syntax unified
	.thumb
	.global Func_08040cb0
	.thumb_func
Func_08040cb0:
	push {lr}
	ldr r0, .L_08040cc0
	bl Scheduler_RemoveCallback
	movs r0, #208
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_08040cc0:
	.4byte Func_08040c60
