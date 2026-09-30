.syntax unified
	.thumb
	.global UiTextResource_SetPosition
	.thumb_func
UiTextResource_SetPosition:
	push {lr}
	ldr r3, .L_0803f6b4
	ldrh r4, [r0, #6]
	ands r1, r3
	ldr r3, .L_0803f6b8
	strb r2, [r0, #4]
	ands r3, r4
	orrs r3, r1
	strh r3, [r0, #6]
	movs r1, #252
	bl Func_08014128
	b .L_0803f6bc
	.2byte 0x0000
.L_0803f6b4:
	.4byte 0x000001ff
.L_0803f6b8:
	.4byte 0xfffffe00
.L_0803f6bc:
	pop {pc}
	.2byte 0x0000
