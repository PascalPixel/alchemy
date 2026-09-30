.syntax unified
	.thumb
	.global Func_0815e20c
	.thumb_func
Func_0815e20c:
	push {r5, lr}
	adds r5, r1, #0
	bl Func_081180b0
	ldr r3, [r5, #4]
	subs r3, #16
	str r3, [r5, #4]
	pop {r5, pc}
