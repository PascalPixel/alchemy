.syntax unified
	.thumb
	.global Func_0802cf7c
	.thumb_func
Func_0802cf7c:
	push {lr}
	ldr r0, .L_0802cf8c
	bl Scheduler_RemoveCallback
	movs r0, #112
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_0802cf8c:
	.4byte Func_0802cfa4
