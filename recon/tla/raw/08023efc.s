.syntax unified
	.thumb
	.global Func_08023efc
	.thumb_func
Func_08023efc:
	movs r2, #4
	ldrsh r3, [r0, r2]
	ldr r2, [r0]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r3, #4]
	str r3, [r0]
	movs r3, #0
	strh r3, [r0, #4]
	movs r0, #1
	bx lr
	.2byte 0x0000
