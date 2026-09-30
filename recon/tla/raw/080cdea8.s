.syntax unified
	.thumb
	.global Func_080cdea8
	.thumb_func
Func_080cdea8:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	bl Func_080d22a8
	adds r0, r5, #0
	bl Func_080d3be8
	adds r0, r6, #0
	movs r1, #0
	bl Func_080d407c
	bl Func_080d2350
	pop {r5, r6, pc}
	.2byte 0x0000
