.syntax unified
.text
	.thumb
	.global Veneer_08009018
	.thumb_func
Veneer_08009018:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800b7c1
