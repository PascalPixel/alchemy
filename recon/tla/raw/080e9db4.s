.syntax unified
	.thumb
	.global Func_080e9db4
	.thumb_func
Func_080e9db4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #92]
	bl Func_080cdf5c
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, .L_080e9e84
	bl Scheduler_RemoveCallback
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotionFar
	adds r1, r5, #0
	adds r1, #85
	movs r3, #3
	strb r3, [r1]
	ldr r3, .L_080e9e88
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	movs r2, #0
	adds r3, r3, r1
	strb r2, [r3]
	movs r6, #0
	b .L_080e9dfe
.L_080e9df2:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #59
	bgt .L_080e9e06
.L_080e9dfe:
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_080e9df2
.L_080e9e06:
	movs r0, #132
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	cmp r2, r3
	ble .L_080e9e30
.L_080e9e1c:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #19
	bgt .L_080e9e30
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_080e9e1c
.L_080e9e30:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	cmp r2, r3
	ble .L_080e9e54
.L_080e9e40:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #19
	bgt .L_080e9e54
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_080e9e40
.L_080e9e54:
	movs r0, #1
	bl WaitFrames
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #40]
	ldr r3, [r5, #20]
	movs r2, #128
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #72]
	lsls r2, r2, #5
	adds r2, #44
	adds r3, r7, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Resource_ResetEntry
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e9e84:
	.4byte Func_080e9940
.L_080e9e88:
	.4byte gPartyState
