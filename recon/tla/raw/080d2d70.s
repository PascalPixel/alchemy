.syntax unified
	.thumb
	.global Func_080d2d70
	.thumb_func
Func_080d2d70:
	ldr r2, .L_080d2d80
	movs r3, #157
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	bx lr
.L_080d2d80:
	.4byte gPartyState
