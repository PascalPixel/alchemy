.syntax unified
	.thumb
	.global Func_080d16f8
	.thumb_func
Func_080d16f8:
	push {lr}
	ldr r0, .L_080d1708
	bl Scheduler_RemoveCallback
	movs r0, #128
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_080d1708:
	.4byte Func_080d0ca0
