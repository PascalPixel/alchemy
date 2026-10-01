.syntax unified
	.thumb
	.global Ui_ClearVramBlock
	.thumb_func
Ui_ClearVramBlock:
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
