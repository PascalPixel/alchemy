.syntax unified
	.thumb
	.global Func_080ceb94
	.thumb_func
Func_080ceb94:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Func_080201c0
	str r0, [r5, #12]
	str r0, [r5, #20]
	pop {r5, pc}
