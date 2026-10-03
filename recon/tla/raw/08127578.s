.syntax unified
	.thumb
	.global Func_08127578
	.thumb_func
Func_08127578:
	push {lr}
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	pop {pc}
	.2byte 0x0000
