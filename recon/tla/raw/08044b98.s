.syntax unified
	.thumb
	.global Func_08044b98
	.thumb_func
Func_08044b98:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	sub sp, #28
	lsls r3, r3, #18
	movs r2, #0
	ldr r5, [r3, #60]
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
	beq .L_08044c6a
	movs r1, #4
	movs r2, #0
	movs r3, #4
	str r3, [sp, #0]
	bl UiWindow_DrawDividerLine
	movs r3, #1
	strb r3, [r5, #3]
	mov r0, r10
	bl Func_08044b80
	bl Localization_LookupEntryId
	movs r1, #14
	add r2, sp, #12
	add r3, sp, #8
	str r1, [sp, #0]
	movs r1, #0
	str r6, [sp, #4]
	bl Func_0803dab0
	ldr r3, .L_08044c74
	str r6, [r7]
	str r3, [sp, #20]
	ldr r3, [sp, #8]
	movs r2, #224
	lsls r2, r2, #8
	orrs r3, r2
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #132
	str r3, [sp, #24]
	adds r3, r5, r2
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	strh r6, [r3]
	movs r1, #1
	mov r0, r10
	bl Func_0803ccd0
	ldr r0, .L_08044c78
	bl Func_0803cf60
	movs r2, #36
	adds r1, r0, #0
	movs r3, #2
	mov r0, r8
	str r6, [sp, #0]
	bl UiText_QueueRenderEntries
	movs r0, #81
	bl Audio_PlayCue
	movs r5, #129
	ldr r6, .L_08044c7c
	lsls r5, r5, #2
	adds r5, #255
.L_08044c38:
	movs r1, #250
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
	movs r0, #1
	bl WaitFrames
	bl Audio_Check
	cmp r0, #0
	beq .L_08044c56
	ldr r3, [r6, #4]
	ands r3, r5
	cmp r3, #0
	beq .L_08044c38
.L_08044c56:
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #12]
	bl Resource_ResetEntry
.L_08044c6a:
	add sp, #28
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08044c74:
	.4byte 0x8014000c
.L_08044c78:
	.4byte 0x0000002b
.L_08044c7c:
	.4byte gInput
