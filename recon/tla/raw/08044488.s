.syntax unified
	.thumb
	.global Func_08044488
	.thumb_func
Func_08044488:
	push {lr}
	ldr r2, .L_08044494
	movs r1, #128
	bl VramBlock_LoadResourceFar
	pop {pc}
.L_08044494:
	.4byte 0x00000202
