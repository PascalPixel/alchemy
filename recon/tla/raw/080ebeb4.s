.syntax unified
	.thumb
	.global Func_080ebeb4
	.thumb_func
Func_080ebeb4:
	ldr r2, .L_080ebec4
	movs r3, #0
	str r1, [r0, #52]
	strh r3, [r0, #58]
	strh r3, [r0, #56]
	adds r0, #64
	strb r2, [r0]
	bx lr
.L_080ebec4:
	.4byte 0x00000000
