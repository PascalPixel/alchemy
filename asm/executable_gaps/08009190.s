.syntax unified
.text
	.thumb
	.global Veneer_08009190
	.thumb_func
Veneer_08009190:
	ldr r4, [pc, #0]
	bx r4
	.4byte 0x080118c1
