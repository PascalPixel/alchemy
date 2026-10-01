.syntax unified
	.thumb
	.global Func_08013be0
	.thumb_func
Func_08013be0:
	push {r5, r6, lr}
	ldr r4, .L_08013c14
	adds r6, r0, #0
	adds r0, r1, #0
	ldr r1, .L_08013c18
	ldrh r3, [r1]
	adds r5, r3, #0
	strh r1, [r1]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_08013c0e
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r4
	adds r3, #4
	stmia r3!, {r0}
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r4]
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_08013c0e:
	strh r5, [r1]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08013c14:
	.4byte gIoWriteQueue
.L_08013c18:
	.4byte 0x04000208
