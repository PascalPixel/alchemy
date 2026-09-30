.syntax unified
	.thumb
	.global Ui_FillVramBlockPattern
	.thumb_func
Ui_FillVramBlockPattern:
	push {lr}
	movs r1, #240
	ldr r3, .L_080396d0
	lsls r1, r1, #4
	ldr r2, .L_080396d4
	ldr r0, .L_080396d8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_080396d0:
	.4byte IwramFillWords
.L_080396d4:
	.4byte 0x44444444
.L_080396d8:
	.4byte 0x06002500
