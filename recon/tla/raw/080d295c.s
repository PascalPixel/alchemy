.syntax unified
	.thumb
	.global Func_080d295c
	.thumb_func
Func_080d295c:
	push {lr}
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Menu_RunConfirmSelectionFar
	pop {pc}
	.2byte 0x0000
