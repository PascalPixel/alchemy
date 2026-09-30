.syntax unified
	.thumb
	.global Func_081195d4
	.thumb_func
Func_081195d4:
	push {lr}
	ldr r3, .L_081195e4
	movs r1, #16
	ldr r0, .L_081195e8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_081195e4:
	.4byte IwramClearWords
.L_081195e8:
	.4byte Data_02003a74
