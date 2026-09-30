.syntax unified
	.thumb
	.global Func_0804dabc
	.thumb_func
Func_0804dabc:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0804dacc
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_0804dacc:
	.4byte Func_0804db74
