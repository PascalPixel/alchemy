.syntax unified
	.thumb
	.global Func_08044c80
	.thumb_func
Func_08044c80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #0
	mov r8, r3
	add r3, sp, #24
	str r2, [sp, #0]
	mov r11, r0
	mov r10, r3
	movs r0, #1
	movs r1, #1
	movs r2, #28
	movs r3, #5
	bl UiWindow_Create
	movs r6, #0
	mov r9, r0
	cmp r0, #0
	beq .L_08044daa
	movs r1, #8
	movs r3, #4
	movs r2, #0
	str r3, [sp, #0]
	bl UiWindow_DrawDividerLine
	movs r3, #1
	mov r2, r8
	strb r3, [r2, #3]
	mov r0, r11
	bl Func_08044b80
	bl Func_0803d2f0
	movs r3, #14
	add r5, sp, #16
	add r2, sp, #20
	str r3, [sp, #0]
	movs r1, #0
	adds r3, r5, #0
	str r6, [sp, #4]
	bl Func_0803dab0
	mov r3, r10
	str r6, [r3]
	ldr r3, .L_08044db8
	movs r2, #224
	str r3, [sp, #28]
	ldr r3, [sp, #16]
	lsls r2, r2, #8
	ldr r0, [sp, #8]
	orrs r3, r2
	str r3, [sp, #32]
	add r7, sp, #36
	bl Func_08044b80
	bl Func_0803d2f0
	movs r3, #15
	add r2, sp, #12
	str r3, [sp, #0]
	movs r1, #0
	adds r3, r5, #0
	str r6, [sp, #4]
	bl Func_0803dab0
	ldr r3, .L_08044dbc
	str r6, [r7]
	str r3, [sp, #40]
	ldr r3, [sp, #16]
	movs r2, #240
	lsls r2, r2, #8
	orrs r3, r2
	str r3, [sp, #44]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #132
	add r3, r8
	strh r6, [r3]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #134
	add r3, r8
	strh r6, [r3]
	mov r0, r11
	movs r1, #1
	bl Func_0803ccd0
	movs r1, #1
	ldr r0, [sp, #8]
	bl Func_0803ccd0
	ldr r0, .L_08044dc0
	bl Func_0803cf60
	movs r2, #68
	adds r1, r0, #0
	movs r3, #2
	mov r0, r9
	str r6, [sp, #0]
	bl UiText_QueueRenderEntries
	movs r0, #81
	bl Audio_PlayCue
.L_08044d62:
	movs r1, #250
	mov r0, r10
	bl Runtime_PushSlotEntry
	movs r1, #250
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
	movs r0, #1
	bl WaitFrames
	bl Audio_Check
	cmp r0, #0
	beq .L_08044d90
	ldr r3, .L_08044dc4
	movs r2, #129
	ldr r3, [r3, #4]
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	beq .L_08044d62
.L_08044d90:
	movs r1, #2
	mov r0, r9
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #20]
	bl Resource_ResetEntry
	ldr r0, [sp, #12]
	bl Resource_ResetEntry
.L_08044daa:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08044db8:
	.4byte 0x800c000c
.L_08044dbc:
	.4byte 0x802c000c
.L_08044dc0:
	.4byte 0x0000002d
.L_08044dc4:
	.4byte gInput
