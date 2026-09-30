.syntax unified
	.thumb
	.global Resource_ScheduleOwnerReset
	.thumb_func
Resource_ScheduleOwnerReset:
	push {lr}
	ldr r0, .L_0803df20
	bl Scheduler_RemoveCallback
	pop {pc}
	.2byte 0x0000
.L_0803df20:
	.4byte Func_0803df24
