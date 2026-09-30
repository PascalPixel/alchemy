.syntax unified
	.thumb
	.global Func_0810b520
	.thumb_func
Func_0810b520:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #16
	mov r8, sp
	adds r5, r0, #0
	mov r0, r8
	bl Party_ListActiveOwnersFar
	negs r5, r5
	adds r7, r0, #0
	adds r0, r5, #0
	bl Party_AdjustSixDigitCounterAFar
	cmp r7, #0
	ble .L_0810b570
	mov r10, r8
	movs r6, #0
	adds r5, r7, #0
.L_0810b548:
	mov r2, r10
	ldrsh r0, [r6, r2]
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_0810b568
	ldrh r3, [r0, #52]
	strh r3, [r0, #56]
	ldrh r3, [r0, #54]
	strh r3, [r0, #58]
	mov r3, r8
	ldrsh r0, [r6, r3]
	bl Owner_RecalculateRatiosFar
.L_0810b568:
	subs r5, #1
	adds r6, #2
	cmp r5, #0
	bne .L_0810b548
.L_0810b570:
	movs r6, #192
	lsls r6, r6, #18
	ldr r1, [r6, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r5, #218
	adds r3, #93
	str r3, [r2]
	lsls r5, r5, #1
	movs r3, #60
	str r3, [r1, r5]
	movs r0, #20
	bl WaitFrames
	bl Event_ClearStatus1c6Far
	bl Event_WaitValue1c8FramesFar
	movs r0, #86
	bl Audio_PlayCue
	bl AudioCommand_WaitForStateByteClear
	movs r0, #10
	bl WaitFrames
	bl Event_SetStatus1c6Far
	bl Event_WaitValue1c8FramesFar
	movs r0, #30
	bl WaitFrames
	ldr r2, [r6, #108]
	movs r3, #16
	str r3, [r2, r5]
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
