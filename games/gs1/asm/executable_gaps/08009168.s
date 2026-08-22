.syntax unified
.text
	.thumb
	.global Veneer_08009168
	.thumb_func
Veneer_08009168:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x080111b5
