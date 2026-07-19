.syntax unified
.text
	.thumb
	.global Veneer_08009010
	.thumb_func
Veneer_08009010:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800b389
