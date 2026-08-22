.syntax unified
.text
	.thumb
	.global Veneer_08009258
	.thumb_func
Veneer_08009258:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x080108c5
