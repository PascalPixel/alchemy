.syntax unified
	.thumb
	.global Func_08041254
	.thumb_func
Func_08041254:
	push {lr}
	ldr r0, .L_08041264
	bl Scheduler_RemoveCallback
	movs r0, #208
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_08041264:
	.4byte Func_08041204
