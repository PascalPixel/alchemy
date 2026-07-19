.syntax unified
.text
	.thumb
	.global Veneer_08009180
	.thumb_func
Veneer_08009180:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x08010425
