.syntax unified
	.thumb
	.global Func_081434d8
	.thumb_func
Func_081434d8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #220
	adds r0, r0, r3
	movs r1, #8
	ldr r3, .L_081434f4
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_081434f4:
	.4byte IwramClearWords
