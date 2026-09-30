.syntax unified
	.thumb
	.global Func_080b039c
	.thumb_func
Func_080b039c:
	push {lr}
	bl Func_080b0378
	movs r3, #100
	muls r0, r3
	lsrs r0, r0, #16
	pop {pc}
	.2byte 0x0000
