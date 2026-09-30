.syntax unified
	.thumb
	.global Func_08044478
	.thumb_func
Func_08044478:
	push {lr}
	ldr r2, .L_08044484
	movs r1, #128
	bl VramBlock_LoadResourceFar
	pop {pc}
.L_08044484:
	.4byte 0x000001fa
