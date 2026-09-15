.syntax unified
.text
	.thumb
	.global Veneer_080091b8
	.thumb_func
Veneer_080091b8:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x080105d5
