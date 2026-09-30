.syntax unified
	.thumb
	.global Func_08143d04
	.thumb_func
Func_08143d04:
	push {r5, lr}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #176
	ldr r1, [r3]
	movs r3, #1
	str r3, [r1, #12]
	movs r1, #168
	movs r0, #128
	lsls r1, r1, #5
	adds r1, #65
	lsls r0, r0, #19
	ldr r5, [r2, #36]
	bl QueueIoWriteDelay2
	movs r0, #1
	bl WaitFrames
	movs r3, #206
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrh r1, [r5]
	movs r0, #2
	movs r2, #0
	bl Func_08118038
	ldr r1, .L_08143d78
	ldr r0, .L_08143d7c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08143d6e
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #248
	adds r3, r3, r1
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #131
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08143d6e:
	strh r4, [r0]
	movs r0, #1
	bl WaitFrames
	pop {r5, pc}
.L_08143d78:
	.4byte Data_020038e0
.L_08143d7c:
	.4byte 0x04000208
