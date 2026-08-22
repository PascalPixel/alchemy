.syntax unified
.text
	.thumb
	.global Veneer_08009170
	.thumb_func
Veneer_08009170:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0801173d
