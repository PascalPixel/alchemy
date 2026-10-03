.syntax unified
	.thumb
	.global EventRuntime_Begin
	.thumb_func
EventRuntime_Begin:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl UiTimedNotice_CloseIfActiveFar
	bl EventRuntime_PrepareCurrentObject
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #162
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080d22cc
	bl EventRuntime_ResolveAllPendingActions
.L_080d22cc:
	movs r1, #203
	movs r2, #192
	lsls r1, r1, #4
	lsls r2, r2, #4
	movs r5, #0
	adds r3, r6, r1
	adds r2, #178
	strh r5, [r3]
	adds r1, #4
	adds r3, r6, r2
	strh r5, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r3, #218
	lsls r3, r3, #1
	movs r1, #220
	adds r2, r6, r3
	lsls r1, r1, #1
	movs r3, #16
	str r3, [r2]
	adds r3, r6, r1
	str r5, [r3]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r2]
	adds r1, #16
	movs r3, #1
	adds r2, r6, r1
	negs r3, r3
	strh r3, [r2]
	movs r3, #229
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	negs r3, r3
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d2348
	bl Scheduler_AddOrUpdateCallback
	movs r0, #153
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_080d234c
	movs r1, #240
	lsls r1, r1, #1
	adds r2, r6, r1
	adds r1, #52
	adds r3, r3, r1
	ldr r3, [r3]
	str r3, [r2]
	movs r2, #242
	lsls r2, r2, #1
	adds r3, r6, r2
	str r5, [r3]
	pop {r5, r6, pc}
.L_080d2348:
	.4byte EventRuntime_UpdateWaitMode
.L_080d234c:
	.4byte gPartyState
