.syntax unified
	.thumb
	.global Func_081963ec
	.thumb_func
Func_081963ec:
	push {lr}
	movs r3, #15
	sub sp, #4
	ands r3, r1
	lsrs r1, r1, #4
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	bl Func_08196404
	add sp, #4
	pop {pc}
