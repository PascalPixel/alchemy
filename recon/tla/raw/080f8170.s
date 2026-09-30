.syntax unified
	.thumb
	.global Func_080f8170
	.thumb_func
Func_080f8170:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r2, [sp, #24]
	movs r2, #0
	str r3, [sp, #20]
	str r2, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r11, r1
	mov r0, r11
	mov r9, r3
	bl Owner_GetState
	ldr r3, [sp, #24]
	mov r10, r0
	lsls r3, r3, #1
	str r3, [sp, #8]
	adds r3, #216
	ldrh r3, [r0, r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	mov r8, r3
	bl Item_Get
	ldr r2, [sp, #20]
	movs r5, #128
	lsls r5, r5, #1
	ands r5, r2
	str r0, [sp, #12]
	cmp r5, #0
	bne .L_080f81da
	movs r3, #12
	str r3, [sp, #0]
	mov r0, r9
	adds r3, #246
	str r3, [sp, #4]
	adds r0, #40
	movs r1, #0
	movs r2, #5
	movs r3, #13
	bl UiWindow_UpdateOrCreate
	str r0, [sp, #16]
.L_080f81da:
	mov r3, r9
	ldr r7, [r3, #40]
	cmp r5, #0
	bne .L_080f8294
	ldr r5, [sp, #16]
	cmp r5, #0
	bne .L_080f8200
	movs r0, #1
	bl WaitFrames
	mov r2, r9
	movs r3, #32
	ldr r0, [r2, #40]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #0
	movs r3, #88
	bl UiWindow_ClearInteriorTilesFar
.L_080f8200:
	movs r3, #0
	mov r0, r10
	adds r1, r7, #0
	movs r2, #32
	bl UiText_DrawStringAtOffsetFar
	add r6, sp, #28
	adds r0, r6, #0
	movs r1, #1
	mov r2, r11
	bl CharacterMenu_BuildAvailability
	ldrb r3, [r6, #1]
	movs r5, #0
	cmp r3, #0
	beq .L_080f822e
	ldr r0, .L_080f8544
	adds r1, r7, #0
	movs r2, #32
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #1
.L_080f822e:
	ldrb r3, [r6, #2]
	cmp r3, #0
	beq .L_080f8244
	lsls r3, r5, #3
	ldr r0, .L_080f8548
	adds r3, #8
	adds r1, r7, #0
	movs r2, #32
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
.L_080f8244:
	ldrb r3, [r6, #3]
	cmp r3, #0
	beq .L_080f825a
	lsls r3, r5, #3
	ldr r0, .L_080f854c
	adds r3, #8
	adds r1, r7, #0
	movs r2, #32
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
.L_080f825a:
	ldrb r3, [r6, #4]
	cmp r3, #0
	beq .L_080f8270
	lsls r3, r5, #3
	ldr r0, .L_080f8550
	adds r3, #8
	adds r1, r7, #0
	movs r2, #32
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
.L_080f8270:
	cmp r5, #1
	bgt .L_080f8294
	mov r3, r10
	ldrb r6, [r3, #15]
	ldr r0, .L_080f8554
	adds r1, r7, #0
	movs r2, #40
	movs r3, #16
	bl UiText_DrawStringAtOffsetFar
	movs r3, #16
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #4
	adds r2, r7, #0
	movs r3, #56
	bl UiText_DrawNumberInWindowFar
.L_080f8294:
	ldr r5, [sp, #16]
	cmp r5, #0
	bne .L_080f82b2
	movs r0, #1
	bl WaitFrames
	mov r2, r9
	movs r3, #80
	ldr r0, [r2, #40]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #32
	movs r3, #88
	bl UiWindow_ClearInteriorTilesFar
.L_080f82b2:
	adds r0, r7, #0
	bl RenderOutput_ClearListFar
	ldr r5, [sp, #20]
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r5
	cmp r3, #0
	bne .L_080f82dc
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r0, r11
	adds r3, r7, #0
	movs r1, #0
	movs r2, #0
	bl Func_080380d8
	movs r3, #184
	lsls r3, r3, #1
	add r3, r9
	str r0, [r3]
.L_080f82dc:
	ldr r2, [sp, #20]
	movs r3, #255
	ands r3, r2
	cmp r3, #9
	bls .L_080f82e8
	b .L_080f85fe
.L_080f82e8:
	ldr r2, .L_080f8558
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080f82f0:
	.4byte .L_080f83f0
	.4byte .L_080f85fe
	.4byte .L_080f8458
	.4byte .L_080f8458
	.4byte .L_080f84dc
	.4byte .L_080f85fe
	.4byte .L_080f8436
	.4byte .L_080f85fe
	.4byte .L_080f8578
	.4byte .L_080f8318
.L_080f8318:
	movs r3, #128
	lsls r3, r3, #2
	mov r5, r8
	ands r3, r5
	cmp r3, #0
	beq .L_080f8336
	movs r3, #152
	lsls r3, r3, #2
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080f83f0
	b .L_080f8346
.L_080f8336:
	movs r3, #152
	lsls r3, r3, #2
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080f83f0
.L_080f8346:
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	ldr r2, .L_080f855c
	adds r1, r7, #0
	adds r6, r3, r2
	adds r0, r6, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	mov r0, r11
	mov r1, r8
	bl Item_CanOwnerEquip
	cmp r0, #0
	bne .L_080f8372
	ldr r0, .L_080f8560
	adds r1, r7, #0
	movs r2, #16
	b .L_080f846a
.L_080f8372:
	movs r5, #166
	lsls r5, r5, #1
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	ldr r3, .L_080f8564
	mov r1, r10
	adds r2, r5, #0
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	movs r3, #152
	lsls r3, r3, #2
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080f83b4
	ldr r1, [sp, #8]
	mov r3, r10
	adds r1, #216
	ldrh r2, [r3, r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	mov r5, r10
	strh r3, [r5, r1]
	mov r0, r11
	bl Owner_RefreshDerivedDataFar
	b .L_080f83bc
.L_080f83b4:
	mov r0, r11
	ldr r1, [sp, #24]
	bl Func_080ad048
.L_080f83bc:
	mov r0, r11
	bl Owner_RecalculateStatsFar
	movs r1, #241
	movs r3, #0
	lsls r1, r1, #8
	str r3, [sp, #0]
	adds r1, #42
	adds r0, r7, #0
	movs r2, #3
	movs r3, #5
	bl UiWindow_SetTilemapEntryFar
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r2, [r3]
	ldr r3, .L_080f855c
	adds r1, r7, #0
	adds r6, r2, r3
	adds r0, r6, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080f84c6
.L_080f83f0:
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r2, [r3]
	ldr r3, .L_080f855c
	adds r1, r7, #0
	adds r6, r2, r3
	adds r0, r6, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	mov r0, r10
	adds r1, r7, #0
	bl Func_080f8658
	movs r3, #146
	lsls r3, r3, #1
	add r3, r10
	ldr r6, [r3]
	ldr r0, .L_080f8568
	adds r1, r7, #0
	movs r2, #0
	movs r3, #64
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #72
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #8
	adds r2, r7, #0
	movs r3, #24
	bl UiText_DrawNumberInWindowFar
	b .L_080f85fe
.L_080f8436:
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r2, [r3]
	ldr r3, .L_080f855c
	adds r1, r7, #0
	adds r6, r2, r3
	adds r0, r6, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	mov r0, r10
	adds r1, r7, #0
	bl Func_080f8658
	b .L_080f85fe
.L_080f8458:
	mov r0, r11
	mov r1, r8
	bl Item_CanOwnerEquip
	cmp r0, #0
	bne .L_080f8472
	ldr r0, .L_080f8560
	adds r1, r7, #0
	movs r2, #0
.L_080f846a:
	movs r3, #48
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080f85fe
.L_080f8472:
	movs r5, #166
	lsls r5, r5, #1
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	ldr r3, .L_080f8564
	mov r1, r10
	adds r2, r5, #0
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	movs r3, #152
	lsls r3, r3, #2
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080f84ae
	ldr r1, [sp, #8]
	mov r3, r10
	adds r1, #216
	ldrh r2, [r3, r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	mov r5, r10
	strh r3, [r5, r1]
	b .L_080f84b6
.L_080f84ae:
	mov r0, r11
	ldr r1, [sp, #24]
	bl Func_080ad048
.L_080f84b6:
	mov r0, r11
	bl Owner_RecalculateStatsFar
	mov r0, r10
	mov r1, r8
	adds r2, r7, #0
	bl Func_080f8708
.L_080f84c6:
	movs r2, #166
	ldr r3, .L_080f8564
	mov r0, r10
	mov r1, r8
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
	mov r0, r8
	bl Sys_Free
	b .L_080f85fe
.L_080f84dc:
	ldr r2, [sp, #12]
	movs r3, #88
	mov r5, r10
	ldrh r4, [r2, #40]
	ldrh r2, [r5, r3]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r3, r2
	movs r0, #0
	movs r1, #0
	b .L_080f850a
.L_080f84f4:
	adds r1, #1
	cmp r1, #31
	bgt .L_080f8510
	lsls r3, r1, #2
	adds r3, #88
	mov r5, r10
	ldrh r2, [r5, r3]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r3, r2
.L_080f850a:
	cmp r3, r4
	bne .L_080f84f4
	movs r0, #1
.L_080f8510:
	cmp r0, #0
	beq .L_080f8526
	ldr r0, .L_080f856c
	adds r1, r7, #0
	adds r0, r4, r0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_080f8570
	b .L_080f8536
.L_080f8526:
	ldr r0, .L_080f856c
	adds r1, r7, #0
	adds r0, r4, r0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_080f8574
.L_080f8536:
	adds r1, r7, #0
	movs r2, #0
	movs r3, #56
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080f85fe
	.2byte 0x0000
.L_080f8544:
	.4byte 0x00001107
.L_080f8548:
	.4byte 0x00001108
.L_080f854c:
	.4byte 0x00001109
.L_080f8550:
	.4byte 0x0000110a
.L_080f8554:
	.4byte Data_08105938
.L_080f8558:
	.4byte .L_080f82f0
.L_080f855c:
	.4byte 0x00000b63
.L_080f8560:
	.4byte 0x00001050
.L_080f8564:
	.4byte IwramCopyWords
.L_080f8568:
	.4byte 0x0000103d
.L_080f856c:
	.4byte 0x000005a7
.L_080f8570:
	.4byte 0x00001052
.L_080f8574:
	.4byte 0x00001051
.L_080f8578:
	ldr r5, .L_080f860c
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #40
	bl UiText_DrawCharacterAtOffsetFar
	mov r2, r10
	ldrh r6, [r2, #60]
	movs r3, #40
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #3
	adds r2, r7, #0
	movs r3, #64
	bl UiText_DrawNumberInWindowFar
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r10
	ldrh r6, [r3, #62]
	movs r3, #48
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #3
	adds r2, r7, #0
	movs r3, #64
	bl UiText_DrawNumberInWindowFar
	adds r0, r5, #4
	adds r1, r7, #0
	movs r2, #0
	movs r3, #56
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r10
	adds r3, #64
	ldrh r6, [r3]
	movs r3, #56
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #3
	adds r2, r7, #0
	movs r3, #64
	adds r5, #3
	bl UiText_DrawNumberInWindowFar
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #0
	movs r3, #64
	bl UiText_DrawCharacterAtOffsetFar
	mov r3, r10
	adds r3, #66
	ldrb r6, [r3]
	movs r1, #3
	movs r3, #64
	adds r0, r6, #0
	adds r2, r7, #0
	str r3, [sp, #0]
	bl UiText_DrawNumberInWindowFar
.L_080f85fe:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080f860c:
	.4byte 0x0000104b
