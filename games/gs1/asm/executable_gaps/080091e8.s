.syntax unified
.text
	.thumb
	.global Veneer_080091e8
	.thumb_func
Veneer_080091e8:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800c549
