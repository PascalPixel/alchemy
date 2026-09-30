.syntax unified
	.thumb
	.global Func_080cec24
	.thumb_func
Func_080cec24:
	push {lr}
	movs r3, #0
	str r3, [r1]
	str r3, [r1, #4]
	str r3, [r1, #8]
	bl Func_080ca9cc
	pop {pc}
