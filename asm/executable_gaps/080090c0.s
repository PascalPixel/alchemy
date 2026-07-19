.syntax unified
.text
	.thumb
	.global Veneer_080090c0
	.thumb_func
Veneer_080090c0:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800c0cd
