.syntax unified
	.thumb
	.global Func_0810021c
	.thumb_func
Func_0810021c:
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
	ldr r7, [r3]
	sub sp, #80
	ldr r1, [r7, #40]
	movs r4, #1
	mov r9, r1
	movs r2, #0
	movs r1, #128
	str r2, [sp, #16]
	str r4, [sp, #12]
	lsls r1, r1, #2
	adds r1, #22
	movs r3, #2
	mov r10, r3
	adds r3, r7, r1
	ldrb r0, [r3]
	bl Owner_GetState
	mov r2, r10
	mov r11, r0
	movs r3, #10
	adds r0, r7, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #10
	movs r3, #15
	adds r0, #52
	bl UiWindow_UpdateOrCreate
	adds r3, r7, #0
	adds r3, #240
	str r3, [sp, #8]
	adds r0, r7, #0
	ldr r1, [r3]
	bl Func_080fa3d4
	ldr r5, .L_081004b0
	movs r6, #24
	negs r6, r6
	adds r0, r5, #0
	mov r1, r9
	adds r3, r6, #0
	movs r2, #0
	subs r5, #3
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	mov r1, r9
	movs r2, #64
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_0810047e
.L_0810029a:
	add r4, sp, #52
	mov r1, r10
	mov r8, r4
	cmp r1, #0
	beq .L_08100374
	cmp r1, #2
	bne .L_08100330
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r7, r2
	ldrb r0, [r3]
	bl Owner_GetState
	movs r3, #226
	lsls r3, r3, #1
	adds r1, r7, r3
	movs r2, #0
	mov r11, r0
	bl Func_080fad88
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r7, r4
	strb r0, [r3]
	mov r0, r9
	bl RenderOutput_RedrawSavedRectFar
	mov r2, r11
	add r4, sp, #20
	adds r2, #216
	movs r0, #0
	movs r1, #14
.L_081002dc:
	ldrh r3, [r2]
	subs r1, #1
	strh r3, [r0, r4]
	adds r2, #2
	adds r0, #2
	cmp r1, #0
	bge .L_081002dc
	movs r1, #226
	lsls r1, r1, #1
	adds r5, r7, r1
	adds r0, r5, #0
	movs r1, #0
	bl InventoryMenu_SortByListOrder
	add r2, sp, #52
	mov r8, r2
	movs r1, #0
	mov r0, r8
	bl Func_080ff7b4
	movs r1, #0
	adds r0, r5, #0
	bl Func_080fadd0
	ldr r0, [r7, #52]
	bl RenderOutput_RedrawSavedRectFar
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #22
	adds r3, r7, r4
	ldr r0, [r7, #52]
	ldrb r1, [r3]
	movs r2, #1
	bl Func_081004b8
	movs r1, #1
	movs r0, #1
	str r1, [sp, #12]
	bl WaitFrames
	b .L_08100334
.L_08100330:
	add r2, sp, #52
	mov r8, r2
.L_08100334:
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_08100348
	movs r4, #0
	mov r0, r9
	movs r1, #0
	mov r2, r8
	str r4, [sp, #12]
	bl Func_081000e0
.L_08100348:
	movs r1, #0
	mov r2, r8
	mov r0, r9
	bl ItemMenu_DrawEquipPage
	ldr r5, .L_081004b0
	movs r6, #24
	negs r6, r6
	adds r0, r5, #0
	mov r1, r9
	movs r2, #0
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r1, r9
	subs r0, r5, #3
	movs r2, #64
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #0
	mov r10, r1
.L_08100374:
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	add r3, sp, #60
	ldr r1, [r2, #20]
	movs r0, #0
	str r3, [sp, #0]
	movs r2, #5
	add r3, sp, #68
	bl Func_080f8f9c
	mov r3, r8
	ldr r1, [r3, #16]
	adds r5, r0, #0
	lsls r1, r1, #4
	adds r1, #52
	movs r0, #96
	bl Func_080f8a44
	cmp r5, #1
	bne .L_081003a6
	movs r4, #1
	str r4, [sp, #12]
	mov r10, r4
.L_081003a6:
	cmp r5, #0
	bne .L_081003ae
	movs r1, #1
	mov r10, r1
.L_081003ae:
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	bne .L_081003ba
	movs r3, #0
	mov r10, r3
.L_081003ba:
	ldr r5, .L_081004b4
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_081003d2
	movs r0, #112
	bl Audio_PlayCue
	movs r4, #1
	str r4, [sp, #16]
	b .L_0810048c
.L_081003d2:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_081003ea
	movs r0, #113
	bl Audio_PlayCue
	movs r1, #1
	negs r1, r1
	str r1, [sp, #16]
	b .L_0810048c
.L_081003ea:
	ldr r3, [r5, #12]
	movs r6, #128
	lsls r6, r6, #1
	ands r3, r6
	cmp r3, #0
	bne .L_08100402
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810047e
.L_08100402:
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #28
	ldrsb r0, [r7, r0]
	movs r2, #129
	lsls r2, r2, #2
	lsls r3, r0, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	mov r1, r8
	movs r4, #153
	ldr r2, [r1, #24]
	lsls r4, r4, #2
	adds r3, r3, r4
	strb r2, [r7, r3]
	ldr r3, [r5, #12]
	ands r3, r6
	cmp r3, #0
	beq .L_0810042e
	adds r0, #1
	b .L_08100430
.L_0810042e:
	subs r0, #1
.L_08100430:
	mov r2, r11
	add r5, sp, #20
	adds r2, #216
	movs r4, #0
	movs r1, #14
.L_0810043a:
	ldrh r3, [r4, r5]
	subs r1, #1
	strh r3, [r2]
	adds r4, #2
	adds r2, #2
	cmp r1, #0
	bge .L_0810043a
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	ldrb r1, [r3]
	adds r0, r0, r1
	bl __modsi3
	movs r3, #129
	lsls r2, r0, #1
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrh r3, [r7, r2]
	movs r4, #128
	str r3, [r7, #8]
	ldrh r1, [r7, r2]
	lsls r4, r4, #2
	adds r4, #22
	adds r3, r7, r4
	strb r1, [r3]
	strb r0, [r7, #28]
	adds r0, r7, #0
	ldrh r1, [r7, r2]
	bl Func_080f88c4
	movs r1, #2
	mov r10, r1
.L_0810047e:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0810048c
	b .L_0810029a
.L_0810048c:
	ldr r0, [r7, #48]
	bl RenderOutput_PrepareForRedrawFar
	mov r0, r9
	bl RenderOutput_RedrawSavedRectFar
	ldr r2, [sp, #8]
	ldr r0, [r2]
	bl RenderOutput_ClearListFar
	ldr r0, [sp, #16]
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081004b0:
	.4byte 0x00001038
.L_081004b4:
	.4byte gInput
