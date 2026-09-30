.syntax unified
	.thumb
	.global Func_081269bc
	.thumb_func
Func_081269bc:
	push {lr}
	ldr r3, .L_081269c8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_081269c8:
	.4byte IwramMulQ16
