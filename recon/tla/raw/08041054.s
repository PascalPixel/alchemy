.syntax unified
	.thumb
	.global Func_08041054
	.thumb_func
Func_08041054:
	push {lr}
	ldr r0, .L_08041064
	bl Scheduler_RemoveCallback
	movs r0, #208
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_08041064:
	.4byte Func_08041004
