.syntax unified
	.thumb
	.global Party_ShowJoinedMessage
	.thumb_func
Party_ShowJoinedMessage:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0802146c
	sub sp, #28
	movs r2, #0
	ldr r5, [r3]
	mov r10, r0
	str r2, [sp, #0]
	movs r0, #2
	movs r1, #1
	movs r2, #26
	movs r3, #5
	add r7, sp, #16
	bl UiWindow_Create
	movs r6, #0
	mov r8, r0
	cmp r0, #0
	beq .L_0802145c
	movs r1, #4
	movs r3, #4
	movs r2, #0
	str r3, [sp, #0]
	bl UiWindow_DrawDividerLine
	ldr r3, .L_08021470
	adds r2, r5, r3
	movs r3, #1
	strb r3, [r2]
	mov r0, r10
	bl Party_LookupCharacterValueByFlag32
	bl Localization_LookupEntryId
	movs r1, #14
	add r2, sp, #12
	add r3, sp, #8
	str r1, [sp, #0]
	movs r1, #0
	str r6, [sp, #4]
	bl UiGlyph_LoadEntryWithPalette
	ldr r3, .L_08021474
	str r6, [r7]
	movs r2, #224
	str r3, [sp, #20]
	ldr r3, [sp, #8]
	lsls r2, r2, #8
	orrs r3, r2
	ldr r2, .L_08021478
	str r3, [sp, #24]
	adds r3, r5, r2
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	strh r6, [r3]
	movs r1, #1
	mov r0, r10
	bl UiWork_PushValueSlot
	ldr r0, .L_0802147c
	bl UiText_BuildRenderEntriesMode1
	movs r2, #36
	adds r1, r0, #0
	movs r3, #2
	mov r0, r8
	str r6, [sp, #0]
	bl UiText_QueueRenderEntries
	movs r0, #81
	bl AudioCommand_PlayFar
	ldr r5, .L_08021480
	ldr r6, .L_08021484
.L_0802142a:
	adds r0, r7, #0
	movs r1, #250
	bl Runtime_PushSlotEntry
	movs r0, #1
	bl WaitFrames
	bl Audio_Check
	cmp r0, #0
	beq .L_08021448
	ldr r3, [r6]
	ands r3, r5
	cmp r3, #0
	beq .L_0802142a
.L_08021448:
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #12]
	bl Resource_ResetEntry
.L_0802145c:
	add sp, #28
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0802146c:
	.4byte gWindowWork
.L_08021470:
	.4byte 0x00000ea3
.L_08021474:
	.4byte 0x8014000c
.L_08021478:
	.4byte 0x000012f4
.L_0802147c:
	.4byte 0x0000001b
.L_08021480:
	.4byte 0x00000303
.L_08021484:
	.4byte gKeyState
