.syntax unified
	.thumb
	.global Func_080149e0
	.thumb_func
Func_080149e0:
	push {lr}
	ldr r3, .L_080149ec
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #8
	pop {pc}
.L_080149ec:
	.4byte IwramFillWords + 0x74
