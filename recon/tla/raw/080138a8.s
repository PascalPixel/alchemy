.syntax unified
	.thumb
	.global Func_080138a8
	.thumb_func
Func_080138a8:
	ldr r2, .L_080138b0
	movs r3, #19
	str r3, [r2, #32]
	bx lr
.L_080138b0:
	.4byte gInput
