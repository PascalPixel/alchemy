.syntax unified
.text
	.thumb
	.global Veneer_08009028
	.thumb_func
Veneer_08009028:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x0800bacd
