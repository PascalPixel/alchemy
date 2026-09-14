.syntax unified
.text
	.thumb
	.global Veneer_080090d8
	.thumb_func
Veneer_080090d8:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800c3ed
