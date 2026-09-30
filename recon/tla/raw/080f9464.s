.syntax unified
	.thumb
	.global Func_080f9464
	.thumb_func
Func_080f9464:
	push {lr}
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080f9478
	ldr r0, .L_080f947c
	bl Scheduler_RemoveCallback
.L_080f9478:
	pop {pc}
	.2byte 0x0000
.L_080f947c:
	.4byte Func_080f93c4
