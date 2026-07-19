.syntax unified
.text
	.thumb
	.global Veneer_08009188
	.thumb_func
Veneer_08009188:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x080118a9
