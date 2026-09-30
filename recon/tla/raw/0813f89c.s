.syntax unified
	.thumb
	.global Func_0813f89c
	.thumb_func
Func_0813f89c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #92]
	ldr r0, [r3, #96]
	ldr r2, .L_0813f8cc
	ldrh r3, [r2]
	adds r1, r3, #0
	strh r2, [r2]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_0813f8c6
	movs r3, #0
	str r3, [r4, #4]
	strh r1, [r2]
	ldr r1, .L_0813f8d0
	movs r2, #128
	lsls r2, r2, #7
	bl ColorBuffer_Halve
	b .L_0813f8c8
.L_0813f8c6:
	strh r1, [r2]
.L_0813f8c8:
	pop {pc}
	.2byte 0x0000
.L_0813f8cc:
	.4byte 0x04000208
.L_0813f8d0:
	.4byte 0x06004000
