.syntax unified
	.thumb
	.global Func_080f9448
	.thumb_func
Func_080f9448:
	push {lr}
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080f9460
	bl Func_080145a8
	pop {pc}
	.2byte 0x0000
.L_080f9460:
	.4byte Func_080f93c4
