.syntax unified
	.thumb
	.global Func_08101d5c
	.thumb_func
Func_08101d5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #220
	ldr r3, [r3]
	sub sp, #36
	str r3, [sp, #24]
	movs r6, #0
	ldr r2, [r2, #60]
	str r6, [sp, #16]
	str r2, [sp, #20]
	str r6, [sp, #12]
	ldr r0, [r3, #52]
	bl RenderOutput_ClearListFar
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #24]
	adds r2, #240
	str r2, [sp, #8]
	ldr r0, [r2]
	bl RenderOutput_PrepareForRedrawFar
	ldr r5, .L_08101ff4
	ldr r3, [sp, #8]
	adds r0, r5, #0
	ldr r1, [r3]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r2, [sp, #8]
	adds r5, #1
	ldr r1, [r2]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #6
	movs r0, #1
	movs r1, #1
	movs r2, #11
	movs r3, #3
	str r5, [sp, #0]
	bl Func_08101c7c
	ldr r3, [sp, #24]
	movs r1, #0
	ldr r0, [r3, #52]
	movs r3, #10
	str r3, [sp, #0]
	movs r2, #0
	movs r3, #28
	str r5, [sp, #4]
	bl Func_08101d34
	movs r1, #9
	movs r2, #8
	movs r3, #10
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r5, #2
	movs r1, #12
	movs r2, #22
	movs r3, #7
	mov r8, r0
	movs r0, #8
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r1, #9
	str r0, [sp, #32]
	movs r2, #22
	movs r3, #3
	movs r0, #8
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	str r0, [sp, #28]
	bl Func_08038290
	ldr r7, .L_08101ff8
	movs r5, #0
.L_08101e18:
	lsls r3, r5, #3
	adds r0, r5, r7
	mov r1, r8
	movs r2, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	cmp r5, #6
	ble .L_08101e18
	ldr r7, .L_08101ffc
	movs r2, #1
	movs r3, #0
	mov r11, r2
	mov r10, r3
.L_08101e34:
	ldr r0, [sp, #28]
	bl RenderOutput_PrepareForRedrawFar
	ldr r0, .L_08101ff8
	ldr r1, [sp, #28]
	movs r2, #0
	movs r3, #0
	adds r0, r6, r0
	bl UiText_DrawResourceFar
	ldr r1, .L_08102000
	ldr r0, [sp, #32]
	adds r1, r6, r1
	bl Func_080383d8
	mov r2, r11
	movs r3, #15
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #12]
	movs r1, #0
	movs r3, #6
	mov r9, r0
	mov r0, r8
	bl Func_08101c18
	mov r3, r11
	str r3, [sp, #0]
	movs r3, #14
	str r3, [sp, #4]
	mov r0, r8
	movs r1, #0
	adds r2, r6, #0
	movs r3, #6
	bl Func_08101c18
	str r6, [sp, #12]
	b .L_08101efa
.L_08101e80:
	ldr r2, [r7, #12]
	movs r3, #96
	ands r2, r3
	cmp r2, #0
	beq .L_08101e9e
	subs r6, #1
	adds r0, r6, #0
	movs r1, #7
	bl Func_08100e28
	adds r6, r0, #0
	movs r0, #111
	bl Audio_PlayCue
	b .L_08101f30
.L_08101e9e:
	ldr r2, [r7, #4]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_08101eb6
	movs r0, #113
	bl Audio_PlayCue
	movs r2, #2
	negs r2, r2
	str r2, [sp, #16]
	b .L_08101f30
.L_08101eb6:
	ldr r2, [r7, #4]
	movs r3, #6
	ands r2, r3
	cmp r2, #0
	beq .L_08101ece
	movs r0, #113
	bl Audio_PlayCue
	movs r3, #1
	negs r3, r3
	str r3, [sp, #16]
	b .L_08101f30
.L_08101ece:
	ldr r3, [r7, #4]
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_08101efa
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08101ef4
	adds r6, #1
	adds r0, r6, #0
	movs r1, #7
	bl Func_08100e28
	adds r6, r0, #0
	movs r0, #112
	bl Audio_PlayCue
	b .L_08101f30
.L_08101ef4:
	movs r0, #111
	bl Audio_PlayCue
.L_08101efa:
	mov r2, r8
	movs r3, #14
	ldrsh r1, [r2, r3]
	movs r0, #12
	adds r1, r1, r6
	lsls r1, r1, #3
	adds r1, #8
	negs r0, r0
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r2, [r7, #12]
	movs r3, #144
	ands r2, r3
	cmp r2, #0
	beq .L_08101e80
	adds r6, #1
	adds r0, r6, #0
	movs r1, #7
	bl Func_08100e28
	adds r6, r0, #0
	movs r0, #111
	bl Audio_PlayCue
.L_08101f30:
	ldr r3, [sp, #20]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #70
	adds r5, r3, r2
	ldrh r3, [r5]
	cmp r3, #99
	beq .L_08101f4a
	adds r0, r3, #0
	bl Resource_ResetEntry
	movs r3, #99
	strh r3, [r5]
.L_08101f4a:
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #60]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #136
	adds r3, r3, r2
	mov r2, r10
	strb r2, [r3]
	ldr r0, [sp, #32]
	bl RenderOutput_PrepareForRedrawFar
	mov r2, r9
	ldr r3, [r2]
	mov r2, r10
	strh r2, [r3, #24]
	strh r2, [r3, #26]
	strh r2, [r3, #20]
	mov r3, r10
	mov r2, r9
	str r3, [r2]
	ldr r3, [sp, #16]
	cmp r3, #0
	bne .L_08101f7c
	b .L_08101e34
.L_08101f7c:
	ldr r2, [r5, #60]
	movs r3, #1
	strb r3, [r2, #6]
	ldr r0, [sp, #28]
	bl RenderOutput_ClearListFar
	ldr r0, [sp, #32]
	bl RenderOutput_ClearListFar
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	ldr r0, [sp, #28]
	bl UiWork_FinalizeFar
	mov r0, r8
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r1, #1
	ldr r0, [sp, #32]
	bl UiWork_FinalizeFar
	bl Func_08038290
	ldr r2, [sp, #16]
	movs r3, #2
	negs r3, r3
	cmp r2, r3
	bne .L_08101fd8
	ldr r2, [sp, #8]
	ldr r0, [r2]
	bl RenderOutput_PrepareForRedrawFar
	ldr r3, [sp, #24]
	ldr r0, [r3, #52]
	bl RenderOutput_PrepareForRedrawFar
	ldr r2, [sp, #24]
	ldr r0, [r2, #16]
	bl RenderOutput_PrepareForRedrawFar
	ldr r2, [r5, #60]
	movs r3, #0
	strb r3, [r2, #6]
.L_08101fd8:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08102004
	bl Scheduler_AddOrUpdateCallback
	ldr r0, [sp, #16]
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08101ff4:
	.4byte 0x00001168
.L_08101ff8:
	.4byte 0x0000116a
.L_08101ffc:
	.4byte gInput
.L_08102000:
	.4byte 0x00002fcb
.L_08102004:
	.4byte Menu_UpdateEntryObjectTransforms
