.syntax unified
.text
	.thumb
	.global Veneer_08009198
	.thumb_func
Veneer_08009198:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x08011af1
