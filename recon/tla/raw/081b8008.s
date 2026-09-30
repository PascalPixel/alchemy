.syntax unified
	.thumb
	.global Func_081b8008
	.thumb_func
Func_081b8008:
	push {lr}
	ldr r3, .L_081b801c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	bl Func_081b83c4
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_081b801c:
	.4byte 0x00000040
