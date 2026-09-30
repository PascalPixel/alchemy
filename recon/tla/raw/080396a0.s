.syntax unified
	.thumb
	.global Func_080396a0
	.thumb_func
Func_080396a0:
	push {lr}
	movs r1, #240
	ldr r3, .L_080396b4
	lsls r1, r1, #4
	movs r2, #0
	ldr r0, .L_080396b8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_080396b4:
	.4byte IwramFillWords
.L_080396b8:
	.4byte 0x06002500
