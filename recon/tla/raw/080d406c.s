.syntax unified
	.thumb
	.global Func_080d406c
	.thumb_func
Func_080d406c:
	push {r5, lr}
	adds r5, r2, #0
	bl Func_080d3fb0
	adds r0, r5, #0
	bl Battle_WaitMode0
	pop {r5, pc}
