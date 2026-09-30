.syntax unified
	.thumb
	.global Func_080feec0
	.thumb_func
Func_080feec0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #48
	movs r1, #0
	str r3, [sp, #36]
	str r1, [sp, #32]
	adds r5, r3, #0
	ldr r2, [r3, #40]
	movs r3, #5
	str r3, [sp, #0]
	movs r0, #0
	adds r5, #48
	movs r3, #2
	str r2, [sp, #12]
	str r3, [sp, #4]
	movs r2, #0
	movs r3, #30
	mov r8, r0
	adds r0, r5, #0
	bl UiWindow_UpdateOrCreate
	movs r0, #1
	negs r0, r0
	ldr r5, [r5]
	bl Party_SumDjinnCountsFar
	negs r3, r0
	orrs r3, r0
	ldr r4, [sp, #36]
	movs r0, #128
	lsls r0, r0, #2
	ldr r1, .L_080fef48
	lsrs r3, r3, #31
	adds r0, #62
	str r3, [sp, #16]
	movs r7, #0
	mov r11, r5
	movs r2, #3
	adds r3, r4, r0
.L_080fef20:
	subs r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #0
	bge .L_080fef20
	movs r1, #0
	movs r0, #10
	str r1, [sp, #20]
	negs r0, r0
	movs r1, #88
	bl Func_080f8ab4
	movs r3, #128
	ldr r2, [sp, #36]
	lsls r3, r3, #2
	adds r3, #22
	adds r3, r2, r3
	str r3, [sp, #8]
	b .L_080ff1a2
	.2byte 0x0000
.L_080fef48:
	.4byte 0x00000068
.L_080fef4c:
	ldr r4, [sp, #8]
	ldrb r0, [r4]
	bl Owner_GetState
	ldr r0, [sp, #8]
	movs r2, #1
	ldrb r1, [r0]
	ldr r0, [sp, #12]
	bl Func_080ff370
	movs r1, #40
	ldr r3, [sp, #8]
	add r1, sp
	mov r9, r1
	ldrb r2, [r3]
	movs r1, #1
	mov r0, r9
	bl CharacterMenu_BuildAvailability
	lsls r0, r0, #24
	movs r4, #0
	lsrs r1, r0, #24
	str r4, [sp, #24]
	str r1, [sp, #28]
	cmp r0, #0
	bne .L_080fefa6
	movs r2, #1
	str r2, [sp, #28]
	b .L_080fefaa
.L_080fef86:
	movs r0, #112
	bl Audio_PlayCue
	movs r3, #1
	str r3, [sp, #20]
	str r3, [sp, #32]
	b .L_080ff1a2
.L_080fef94:
	movs r0, #113
	bl Audio_PlayCue
	movs r0, #1
	movs r4, #1
	negs r0, r0
	str r4, [sp, #20]
	str r0, [sp, #32]
	b .L_080ff1a2
.L_080fefa6:
	movs r1, #1
	str r1, [sp, #24]
.L_080fefaa:
	movs r2, #1
	mov r10, r2
	b .L_080ff194
.L_080fefb0:
	mov r3, r10
	cmp r3, #0
	beq .L_080ff066
	ldr r0, [sp, #28]
	mov r2, r8
	lsls r3, r0, #24
	adds r2, #2
	asrs r5, r3, #24
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r2, r2, r3
	movs r4, #0
	mov r8, r2
	mov r0, r11
	mov r10, r4
	bl RenderOutput_RedrawSavedRectFar
	mov r1, r8
	cmp r1, #0
	bne .L_080ff00c
	adds r0, r7, r5
	adds r1, r5, #0
	bl Math_Mod
	ldr r2, [sp, #24]
	adds r7, r0, #0
	cmp r2, #0
	bne .L_080ff032
	ldr r5, .L_080ff18c
	movs r6, #24
	negs r6, r6
	adds r0, r5, #0
	ldr r1, [sp, #12]
	movs r2, #0
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	subs r0, r5, #1
	ldr r1, [sp, #12]
	movs r2, #80
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080ff032
.L_080ff00c:
	ldr r3, [sp, #16]
	cmp r3, #0
	beq .L_080ff028
	adds r2, r7, #0
	adds r2, #8
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080ff020
	adds r3, r7, #0
	adds r3, #15
.L_080ff020:
	asrs r7, r3, #3
	lsls r3, r7, #3
	subs r7, r2, r3
	b .L_080ff032
.L_080ff028:
	adds r0, r7, #7
	movs r1, #7
	bl Math_Mod
	adds r7, r0, #0
.L_080ff032:
	adds r1, r7, #0
	mov r2, r9
	movs r3, #0
	mov r0, r8
	bl Func_080ff1f4
	mov r0, r11
	bl RenderOutput_ClearListFar
	movs r0, #1
	bl WaitFrames
	mov r4, r8
	cmp r4, #0
	bne .L_080ff05c
	mov r0, r11
	adds r1, r7, #0
	mov r2, r9
	bl Func_080ff27c
	b .L_080ff066
.L_080ff05c:
	mov r0, r11
	adds r1, r7, #0
	ldr r2, [sp, #16]
	bl Func_080ff2e8
.L_080ff066:
	mov r0, r8
	cmp r0, #0
	bne .L_080ff07a
	lsls r1, r7, #4
	movs r0, #10
	adds r1, #88
	negs r0, r0
	bl Func_080f8a44
	b .L_080ff094
.L_080ff07a:
	cmp r7, #3
	bgt .L_080ff08a
	lsls r1, r7, #3
	adds r1, #48
	movs r0, #24
	bl Func_080f8a44
	b .L_080ff094
.L_080ff08a:
	lsls r1, r7, #3
	adds r1, #80
	movs r0, #48
	bl Func_080f8a44
.L_080ff094:
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_080ff190
	movs r3, #240
	ldr r2, [r5, #12]
	ands r2, r3
	cmp r2, #0
	beq .L_080ff0b2
	mov r0, r8
	adds r1, r7, #0
	mov r2, r9
	movs r3, #1
	bl Func_080ff1f4
.L_080ff0b2:
	ldr r2, [r5, #4]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080ff0be
	b .L_080fef86
.L_080ff0be:
	ldr r2, [r5, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080ff0ca
	b .L_080fef94
.L_080ff0ca:
	ldr r2, [r5, #12]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_080ff0e0
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #1
	mov r10, r1
	subs r7, #1
.L_080ff0e0:
	ldr r2, [r5, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_080ff0f6
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #1
	mov r10, r2
	adds r7, #1
.L_080ff0f6:
	ldr r2, [r5, #12]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_080ff10c
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	mov r10, r3
	add r8, r3
.L_080ff10c:
	ldr r2, [r5, #12]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_080ff126
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #1
	movs r4, #1
	negs r0, r0
	mov r10, r4
	add r8, r0
.L_080ff126:
	ldr r3, [r5, #12]
	movs r6, #128
	lsls r6, r6, #1
	ands r3, r6
	cmp r3, #0
	bne .L_080ff13e
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080ff194
.L_080ff13e:
	movs r0, #111
	bl Audio_PlayCue
	ldr r3, [r5, #12]
	ldr r1, [sp, #36]
	ands r3, r6
	movs r0, #28
	ldrsb r0, [r1, r0]
	cmp r3, #0
	beq .L_080ff156
	adds r0, #1
	b .L_080ff158
.L_080ff156:
	subs r0, #1
.L_080ff158:
	ldr r2, [sp, #36]
	movs r4, #139
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r2, r4
	ldrb r1, [r3]
	adds r0, r0, r1
	bl Math_Mod
	movs r1, #129
	ldr r4, [sp, #36]
	lsls r2, r0, #1
	lsls r1, r1, #2
	adds r2, r2, r1
	ldrh r3, [r4, r2]
	str r3, [r4, #8]
	ldr r1, [sp, #8]
	ldrh r3, [r4, r2]
	strb r3, [r1]
	strb r0, [r4, #28]
	ldr r0, [sp, #36]
	ldrh r1, [r4, r2]
	bl Func_080f88c4
	b .L_080ff1a2
	.2byte 0x0000
.L_080ff18c:
	.4byte 0x00001036
.L_080ff190:
	.4byte gInput
.L_080ff194:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ff1a2
	b .L_080fefb0
.L_080ff1a2:
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_080ff1b6
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ff1b6
	b .L_080fef4c
.L_080ff1b6:
	ldr r3, [sp, #36]
	ldr r0, [r3, #48]
	bl RenderOutput_PrepareForRedrawFar
	ldr r0, [sp, #12]
	bl RenderOutput_RedrawSavedRectFar
	movs r1, #128
	ldr r4, [sp, #36]
	lsls r1, r1, #2
	ldr r3, .L_080ff1d4
	adds r1, #62
	movs r2, #3
	adds r0, r4, r1
	b .L_080ff1d8
.L_080ff1d4:
	.4byte 0xfffffff0
.L_080ff1d8:
	subs r2, #1
	strh r3, [r0]
	subs r0, #2
	cmp r2, #0
	bge .L_080ff1d8
	ldr r0, [sp, #32]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
