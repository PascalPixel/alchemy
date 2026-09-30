.syntax unified
	.thumb
	.global Func_08041f70
	.thumb_func
Func_08041f70:
	ldr r2, .L_08041f88
	movs r3, #192
	lsls r3, r3, #18
	ands r0, r2
	ldr r3, [r3, #60]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #62
	adds r3, r3, r2
	strh r0, [r3]
	b .L_08041f8c
	.2byte 0x0000
.L_08041f88:
	.4byte 0x0000000f
.L_08041f8c:
	bx lr
	.2byte 0x0000
