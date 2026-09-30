.syntax unified
	.thumb
	.global Func_080409dc
	.thumb_func
Func_080409dc:
	push {lr}
	ldr r0, .L_080409ec
	bl Scheduler_RemoveCallback
	movs r0, #208
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_080409ec:
	.4byte Func_0804098c
