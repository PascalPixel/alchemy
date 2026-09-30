.syntax unified
	.thumb
	.global Func_080cefd0
	.thumb_func
Func_080cefd0:
	push {lr}
	bl ObjectTable_Get
	movs r1, #2
	bl Object_SetMode
	pop {pc}
	.2byte 0x0000
