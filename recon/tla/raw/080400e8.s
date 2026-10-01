.syntax unified
	.thumb
	.global Func_080400e8
	.thumb_func
Func_080400e8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #1
	movs r1, #0
	sub sp, #20
	mov r11, r0
	mov r9, r1
	bl Func_0803fc38
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r7, [r3]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #2
	movs r2, #28
	movs r3, #3
	movs r0, #1
	bl UiWindow_Create
	str r0, [sp, #16]
	bl Func_0803fd28
	movs r3, #48
	mov r8, r0
	negs r3, r3
	movs r2, #64
	mov r1, r8
	movs r0, #7
	bl Func_08044f88
	str r0, [sp, #12]
	movs r0, #1
	bl WaitFrames
	movs r2, #160
	movs r3, #160
	lsls r2, r2, #3
	lsls r3, r3, #3
	adds r2, #148
	adds r3, #149
	adds r2, r7, r2
	adds r3, r7, r3
	str r2, [sp, #8]
	str r3, [sp, #4]
.L_0804014e:
	mov r0, r11
	cmp r0, #0
	bne .L_08040156
	b .L_080403b8
.L_08040156:
	movs r1, #0
	mov r0, r9
	mov r11, r1
	adds r0, #5
	movs r1, #5
	bl Math_Mod
	movs r5, #160
	lsls r5, r5, #3
	movs r2, #179
	mov r9, r0
	adds r5, #148
	lsls r2, r2, #3
	add r2, r9
	adds r3, r7, #1
	add r5, r9
	ldrsb r1, [r3, r2]
	ldrsb r0, [r7, r5]
	adds r0, r0, r1
	bl Math_Mod
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #116
	strb r0, [r7, r5]
	adds r3, r7, r2
	mov r0, r9
	strh r0, [r3]
	ldr r3, .L_080401b0
	ldr r2, .L_080401ac
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080401a0
	movs r1, #179
	lsls r1, r1, #3
	adds r3, r7, r1
	strb r2, [r3]
.L_080401a0:
	movs r6, #160
	lsls r6, r6, #3
	movs r5, #0
	adds r6, #236
	b .L_080401b4
	.2byte 0x0000
.L_080401ac:
	.4byte 0x00000000
.L_080401b0:
	.4byte Data_03001180
.L_080401b4:
	ldr r0, [r6, r7]
	movs r3, #251
	strb r3, [r0, #15]
	bl UiIcon_PrepareObjectFar
	ldr r3, [r6, r7]
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #150
	ldrb r1, [r3, #14]
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #0
	cmp r5, r3
	beq .L_080401d8
	movs r2, #1
.L_080401d8:
	ldr r3, .L_08040540
	adds r6, #4
	ldrsb r0, [r3, r5]
	adds r5, #1
	bl RenderResource_LoadFrame
	cmp r5, #2
	ble .L_080401b4
	movs r6, #191
	movs r5, #0
	lsls r6, r6, #3
.L_080401ee:
	ldr r0, [r6, r7]
	movs r3, #251
	strb r3, [r0, #15]
	bl UiIcon_PrepareObjectFar
	ldr r3, [r6, r7]
	movs r0, #147
	lsls r0, r0, #3
	adds r0, #255
	ldrb r1, [r3, #14]
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #0
	cmp r5, r3
	beq .L_08040212
	movs r2, #1
.L_08040212:
	ldr r3, .L_08040544
	adds r6, #4
	ldrsb r0, [r3, r5]
	adds r5, #1
	bl RenderResource_LoadFrame
	cmp r5, #1
	ble .L_080401ee
	movs r6, #192
	lsls r6, r6, #3
	movs r5, #0
	adds r6, #4
.L_0804022a:
	ldr r0, [r6, r7]
	movs r3, #251
	strb r3, [r0, #15]
	bl UiIcon_PrepareObjectFar
	ldr r3, [r6, r7]
	movs r2, #0
	ldrb r1, [r3, #14]
	movs r3, #179
	lsls r3, r3, #3
	adds r3, r3, r7
	mov r10, r3
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r5, r3
	beq .L_0804024e
	movs r2, #1
.L_0804024e:
	ldr r3, .L_08040548
	adds r6, #4
	ldrsb r0, [r3, r5]
	adds r5, #1
	bl RenderResource_LoadFrame
	cmp r5, #1
	ble .L_0804022a
	mov r1, r8
	movs r0, #12
	ldrsh r3, [r1, r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #3
	adds r5, r3, #0
	movs r3, #0
	ldrsb r3, [r2, r3]
	movs r1, #160
	lsls r1, r1, #3
	lsls r0, r3, #4
	adds r1, #153
	subs r0, r0, r3
	adds r3, r7, r1
	movs r1, #0
	ldrsb r1, [r3, r1]
	lsls r0, r0, #2
	bl Math_Div
	adds r5, #140
	adds r5, r5, r0
	mov r0, r8
	movs r2, #14
	ldrsh r3, [r0, r2]
	movs r1, #160
	lsls r1, r1, #3
	lsls r3, r3, #3
	adds r1, #180
	adds r2, r3, #4
	adds r0, r7, r1
	movs r3, #1
	adds r1, r5, #0
	bl Func_08108040
	mov r0, r8
	movs r2, #12
	ldrsh r3, [r0, r2]
	ldr r1, [sp, #4]
	lsls r3, r3, #3
	adds r5, r3, #0
	movs r3, #0
	ldrsb r3, [r1, r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #154
	lsls r0, r3, #4
	subs r0, r0, r3
	adds r3, r7, r2
	movs r1, #0
	ldrsb r1, [r3, r1]
	lsls r0, r0, #2
	bl Math_Div
	adds r5, #140
	mov r1, r8
	adds r5, r5, r0
	movs r0, #14
	ldrsh r3, [r1, r0]
	adds r1, r5, #0
	lsls r3, r3, #3
	adds r2, r3, #0
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #196
	adds r0, r7, r3
	adds r2, #20
	movs r3, #1
	bl Func_08108040
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #150
	adds r3, r7, r0
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r3, .L_0804054c
	mov r0, r8
	adds r5, r2, r3
	movs r3, #48
	str r3, [sp, #0]
	movs r1, #160
	movs r2, #40
	movs r3, #200
	bl UiWindow_ClearInteriorTiles
	adds r0, r5, #0
	mov r1, r8
	movs r2, #160
	movs r3, #40
	bl UiText_DrawCharacterAtOffset
	movs r1, #147
	lsls r1, r1, #3
	adds r1, #255
	adds r3, r7, r1
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r3, .L_08040550
	mov r0, r8
	adds r5, r2, r3
	movs r3, #72
	str r3, [sp, #0]
	movs r1, #160
	movs r2, #64
	movs r3, #184
	bl UiWindow_ClearInteriorTiles
	adds r0, r5, #0
	mov r1, r8
	movs r2, #160
	movs r3, #64
	bl UiText_DrawCharacterAtOffset
	mov r3, r10
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r3, .L_08040554
	mov r0, r8
	adds r5, r2, r3
	movs r3, #96
	str r3, [sp, #0]
	movs r1, #160
	movs r2, #88
	movs r3, #184
	bl UiWindow_ClearInteriorTiles
	adds r0, r5, #0
	movs r3, #88
	mov r1, r8
	movs r2, #160
	bl UiText_DrawCharacterAtOffset
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	movs r0, #0
	ldrsb r0, [r1, r0]
	movs r1, #0
	ldrsb r1, [r2, r1]
	bl Func_0803f9c0
	mov r1, r8
	movs r0, #12
	ldrsh r3, [r1, r0]
	mov r0, r9
	lsls r5, r3, #3
	movs r3, #14
	ldrsh r2, [r1, r3]
	lsls r3, r0, #1
	add r3, r9
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r2, r3, #4
	cmp r0, #0
	bne .L_08040394
	adds r2, #8
.L_08040394:
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #164
	adds r0, r7, r1
	movs r3, #3
	adds r1, r5, #0
	bl Func_08108040
	ldr r0, [sp, #16]
	bl RenderOutput_ClearList
	ldr r0, .L_08040558
	ldr r1, [sp, #16]
	add r0, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResource
.L_080403b8:
	ldr r0, [sp, #12]
	bl Func_08045018
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0804055c
	movs r3, #4
	ldr r2, [r5, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_0804040c
	movs r0, #112
	bl Audio_PlayCue
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #126
	adds r1, r7, r2
	ldrh r3, [r1]
	movs r0, #1
	adds r3, #1
	mov r11, r0
	movs r0, #160
	strh r3, [r1]
	lsls r0, r0, #11
	lsls r3, r3, #16
	movs r2, #0
	cmp r3, r0
	bls .L_080403f6
	strh r2, [r1]
.L_080403f6:
	ldrh r3, [r1]
	ldr r2, .L_08040560
	ldrb r3, [r2, r3]
	ldr r2, [sp, #8]
	strb r3, [r2]
	ldr r2, .L_08040564
	ldrh r3, [r1]
	ldr r0, [sp, #4]
	ldrb r3, [r2, r3]
	strb r3, [r0]
	b .L_0804014e
.L_0804040c:
	ldr r2, [r5, #4]
	movs r3, #9
	ands r2, r3
	cmp r2, #0
	beq .L_08040418
	b .L_08040536
.L_08040418:
	ldr r2, [r5, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_08040424
	b .L_0804052a
.L_08040424:
	ldr r2, [r5, #12]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_08040440
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #1
	negs r1, r1
	movs r2, #1
	add r9, r1
	mov r11, r2
	b .L_0804014e
.L_08040440:
	ldr r2, [r5, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_08040458
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	add r9, r3
	mov r11, r3
	b .L_0804014e
.L_08040458:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804047a
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #148
	add r2, r9
	ldrb r3, [r7, r2]
	movs r0, #1
	subs r3, #1
	strb r3, [r7, r2]
	mov r11, r0
.L_0804047a:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	bne .L_08040486
	b .L_0804014e
.L_08040486:
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #148
	add r2, r9
	ldrb r3, [r7, r2]
	movs r1, #1
	adds r3, #1
	strb r3, [r7, r2]
	mov r11, r1
	b .L_0804014e
.L_080404a0:
	ldr r0, [sp, #16]
	movs r1, #2
	bl UiWork_Finalize
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	cmp r6, #0
	bne .L_08040574
	ldr r5, .L_08040568
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080404dc
	movs r0, #179
	lsls r0, r0, #3
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_080404dc
	ldr r0, .L_0804056c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_080404dc:
	cmp r6, #0
	bne .L_08040576
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #148
	adds r3, r7, r1
	movs r0, #147
	ldrb r2, [r3]
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r5, r0
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	ldrb r2, [r3]
	adds r0, #1
	adds r3, r5, r0
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	ldrb r2, [r3]
	adds r0, #6
	adds r3, r5, r0
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	ldrb r2, [r3]
	subs r0, #2
	adds r3, r5, r0
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	ldrb r2, [r3]
	adds r0, #32
	adds r3, r5, r0
	strb r2, [r3]
	ldr r3, .L_08040570
	strb r2, [r3]
	b .L_0804058e
.L_0804052a:
	movs r6, #1
	movs r0, #113
	negs r6, r6
	bl Audio_PlayCue
	b .L_080404a0
.L_08040536:
	movs r0, #112
	movs r6, #0
	bl Audio_PlayCue
	b .L_080404a0
.L_08040540:
	.4byte Data_0805ea7c
.L_08040544:
	.4byte Data_0805ea7f
.L_08040548:
	.4byte Data_0805ea81
.L_0804054c:
	.4byte 0x0000113b
.L_08040550:
	.4byte 0x00001141
.L_08040554:
	.4byte 0x00001144
.L_08040558:
	.4byte 0x00001146
.L_0804055c:
	.4byte gInput
.L_08040560:
	.4byte Data_0805ea83
.L_08040564:
	.4byte Data_0805ea89
.L_08040568:
	.4byte gPartyState
.L_0804056c:
	.4byte 0x00001161
.L_08040570:
	.4byte Data_03001200
.L_08040574:
	ldr r5, .L_080405a8
.L_08040576:
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_0803f9c0
.L_0804058e:
	bl Func_0803fd14
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080405a8:
	.4byte gPartyState
