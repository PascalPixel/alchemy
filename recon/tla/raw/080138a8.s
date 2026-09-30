.syntax unified
	.thumb
	.global Runtime_SetMainState19
	.thumb_func
Runtime_SetMainState19:
	ldr r2, .L_080138b0
	movs r3, #19
	str r3, [r2, #32]
	bx lr
.L_080138b0:
	.4byte gInput
