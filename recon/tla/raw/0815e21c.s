.syntax unified
	.thumb
	.global Func_0815e21c
	.thumb_func
Func_0815e21c:
	push {r5, lr}
	adds r5, r1, #0
	bl Func_081180a8
	ldr r3, [r5, #4]
	subs r3, #16
	str r3, [r5, #4]
	pop {r5, pc}
