.syntax unified
	.thumb
	.global Func_0813ba50
	.thumb_func
Func_0813ba50:
	push {lr}
	ldr r3, .L_0813ba84
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0813ba88
	adds r2, #2
	strh r3, [r2]
	ldr r1, .L_0813ba8c
	movs r3, #128
	ldr r2, .L_0813ba90
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_0813ba94
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	b .L_0813ba98
.L_0813ba84:
	.4byte 0x00000000
.L_0813ba88:
	.4byte 0x0000100e
.L_0813ba8c:
	.4byte 0x000000f0
.L_0813ba90:
	.4byte 0x00001088
.L_0813ba94:
	.4byte 0x00003537
.L_0813ba98:
	strh r3, [r2]
	ldr r1, .L_0813bab8
	ldr r3, .L_0813bab4
	adds r2, #2
	strh r3, [r2]
	ldr r0, .L_0813babc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0813bae2
	b .L_0813bac0
	.2byte 0x0000
.L_0813bab4:
	.4byte 0x00003f21
.L_0813bab8:
	.4byte gIoWriteQueue
.L_0813babc:
	.4byte 0x04000208
.L_0813bac0:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0813bae2:
	strh r4, [r0]
	movs r0, #1
	bl WaitFrames
	pop {pc}
