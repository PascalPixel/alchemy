.syntax unified
.text
	.thumb
	.global Veneer_08009200
	.thumb_func
Veneer_08009200:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800c409
