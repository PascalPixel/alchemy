.syntax unified
.text
	.thumb
	.global Veneer_08009090
	.thumb_func
Veneer_08009090:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800c389
