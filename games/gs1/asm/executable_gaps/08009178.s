.syntax unified
.text
	.thumb
	.global Veneer_08009178
	.thumb_func
Veneer_08009178:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x08010561
