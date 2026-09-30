.syntax unified
	.thumb
	.global Func_0802cf90
	.thumb_func
Func_0802cf90:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0802cfa0
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_0802cfa0:
	.4byte Func_0802cfa4
