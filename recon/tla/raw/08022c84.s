.syntax unified
	.thumb
	.global Func_08022c84
	.thumb_func
Func_08022c84:
	push {lr}
	ldr r2, .L_08022c94
	movs r1, #128
	movs r0, #93
	bl VramBlock_LoadCached
	pop {pc}
	.2byte 0x0000
.L_08022c94:
	.4byte Data_0802e89c
