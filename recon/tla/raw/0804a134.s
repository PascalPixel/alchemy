.syntax unified
	.thumb
	.global Func_0804a134
	.thumb_func
Func_0804a134:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #300
	str r1, [sp, #72]
	str r2, [sp, #68]
	movs r5, #192
	lsls r5, r5, #18
	adds r6, r0, #0
	ldr r0, [r5, #60]
	movs r1, #1
	str r0, [sp, #64]
	negs r1, r1
	movs r0, #128
	str r1, [sp, #60]
	str r1, [sp, #56]
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #52]
	adds r0, r6, #0
	bl Owner_GetState
	movs r3, #42
	str r0, [sp, #48]
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	movs r2, #5
	str r0, [sp, #44]
	str r2, [sp, #40]
	adds r5, #228
	ldr r3, [r5]
	ldr r0, [r3, #52]
	ldr r1, [r3, #48]
	ldr r3, [r3, #56]
	mov r11, r0
	mov r8, r1
	str r3, [sp, #36]
	movs r3, #6
	str r3, [sp, #0]
	movs r2, #21
	movs r0, #9
	movs r1, #9
	movs r3, #11
	bl UiWindow_Create
	mov r2, sp
	adds r2, #80
	str r2, [sp, #20]
	ldr r6, .L_0804a1fc
	movs r5, #128
	mov r9, r0
	movs r7, #0
	adds r4, r2, #0
	lsls r5, r5, #23
.L_0804a1b2:
	movs r3, #0
	lsls r0, r7, #1
	str r5, [r4, #4]
	str r3, [r4, #8]
	mov r1, r9
	movs r3, #12
	ldrsh r2, [r1, r3]
	ldr r3, .L_0804a1f8
	ldrh r1, [r4, #6]
	lsls r2, r2, #3
	adds r2, #8
	ands r2, r3
	adds r3, r6, #0
	ands r3, r1
	orrs r3, r2
	mov r1, r9
	strh r3, [r4, #6]
	movs r2, #14
	ldrsh r3, [r1, r2]
	adds r7, #1
	adds r0, r0, r3
	lsls r0, r0, #3
	adds r0, #4
	strb r0, [r4, #4]
	adds r4, #12
	cmp r7, #4
	ble .L_0804a1b2
	mov r2, sp
	adds r2, #140
	ldr r3, .L_0804a200
	str r2, [sp, #16]
	ldr r6, [sp, #20]
	str r2, [sp, #4]
	movs r5, #8
	b .L_0804a204
.L_0804a1f8:
	.4byte 0x000001ff
.L_0804a1fc:
	.4byte 0xfffffe00
.L_0804a200:
	.4byte 0xfffffc00
.L_0804a204:
	mov r10, r3
	movs r7, #4
.L_0804a208:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r2, [sp, #4]
	subs r7, #1
	stmia r2!, {r0}
	adds r1, r2, #0
	str r1, [sp, #4]
	movs r1, #1
	negs r1, r1
	bl Resource_GetBuffer
	ldr r3, .L_0804a238
	mov r1, r10
	ands r0, r3
	ldrh r3, [r5, r6]
	ands r3, r1
	orrs r3, r0
	strh r3, [r5, r6]
	adds r5, #12
	cmp r7, #0
	bge .L_0804a208
	b .L_0804a23c
	.2byte 0x0000
.L_0804a238:
	.4byte 0x000003ff
.L_0804a23c:
	movs r5, #240
	lsls r5, r5, #8
	adds r5, #24
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_08049a04
	movs r1, #129
	lsls r1, r1, #1
	adds r0, r5, #0
	adds r1, #255
	bl Func_08049a04
	adds r5, #1
	movs r1, #132
	lsls r1, r1, #2
	adds r0, r5, #0
	bl Func_08049a04
	movs r1, #137
	lsls r1, r1, #1
	adds r1, #255
	adds r0, r5, #0
	bl Func_08049a04
	movs r2, #144
	lsls r2, r2, #1
	mov r3, r8
	add r2, sp
	lsls r3, r3, #1
	str r2, [sp, #12]
	str r3, [sp, #8]
.L_0804a27e:
	ldr r0, [sp, #60]
	cmp r11, r0
	bne .L_0804a28c
	ldr r1, [sp, #56]
	cmp r8, r1
	bne .L_0804a28c
	b .L_0804a4dc
.L_0804a28c:
	ldr r2, [sp, #64]
	movs r3, #1
	strb r3, [r2, #6]
	mov r1, r9
	movs r3, #12
	ldrsh r0, [r1, r3]
	movs r2, #14
	ldrsh r1, [r1, r2]
	ldr r2, [sp, #56]
	adds r0, #1
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r3, #15
	str r3, [sp, #0]
	adds r1, #1
	subs r2, #2
	movs r3, #1
	bl Func_08046134
	bl Ui_FillVramBlockPattern
	ldr r0, [sp, #68]
	cmp r0, #0
	beq .L_0804a2e8
	ldr r1, [sp, #72]
	mov r3, r11
	add r3, r8
	lsls r3, r3, #1
	ldrh r3, [r3, r1]
	ldr r0, .L_0804a2e0
	add r5, sp, #160
	ands r0, r3
	ldr r3, .L_0804a2e4
	adds r1, r5, #0
	adds r0, r0, r3
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_0804a2f4
	.2byte 0x0000
.L_0804a2e0:
	.4byte 0x00003fff
.L_0804a2e4:
	.4byte 0x00000885
.L_0804a2e8:
	add r5, sp, #160
	ldr r0, .L_0804a3ac
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_0804a2f4:
	movs r2, #0
	movs r3, #4
	adds r0, r5, #0
	ldr r1, [sp, #44]
	bl UiText_RenderWideStringAtOffset
	ldr r3, [sp, #60]
	mov r2, r8
	str r2, [sp, #56]
	cmp r11, r3
	bne .L_0804a30c
	b .L_0804a45c
.L_0804a30c:
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	ldr r1, [sp, #72]
	mov r0, r11
	lsls r3, r0, #1
	ldrh r5, [r3, r1]
	movs r7, #0
	cmp r5, #0
	bne .L_0804a322
	b .L_0804a456
.L_0804a322:
	mov r2, sp
	adds r2, #76
	str r2, [sp, #24]
.L_0804a328:
	adds r0, r5, #0
	bl BattleAction_Get
	movs r1, #240
	adds r6, r0, #0
	lsls r1, r1, #8
	movs r0, #0
	str r0, [sp, #0]
	lsls r3, r7, #1
	mov r0, r9
	adds r1, #31
	movs r2, #11
	mov r10, r3
	bl UiWindow_SetTilemapEntry
	movs r1, #0
	str r1, [sp, #0]
	movs r1, #240
	lsls r1, r1, #8
	mov r0, r9
	adds r1, #30
	movs r2, #12
	mov r3, r10
	bl UiWindow_SetTilemapEntry
	ldr r3, [sp, #16]
	movs r0, #252
	lsls r0, r0, #6
	lsls r2, r7, #2
	adds r2, r3, r2
	adds r0, #255
	movs r3, #1
	ands r0, r5
	str r3, [sp, #0]
	movs r1, #0
	ldr r3, [sp, #24]
	bl Ability_LoadGlyph
	mov r0, r10
	ldr r2, [sp, #20]
	adds r1, r0, r7
	ldr r3, .L_0804a3a4
	ldr r0, [sp, #76]
	lsls r1, r1, #2
	adds r1, #8
	ands r0, r3
	ldrh r3, [r2, r1]
	ldr r2, .L_0804a3a8
	ands r3, r2
	orrs r3, r0
	ldr r0, [sp, #20]
	ldrb r2, [r6, #1]
	strh r3, [r0, r1]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	bne .L_0804a3b0
	movs r0, #4
	bl Func_08041f70
	b .L_0804a3d8
	.2byte 0x0000
.L_0804a3a4:
	.4byte 0x000003ff
.L_0804a3a8:
	.4byte 0xfffffc00
.L_0804a3ac:
	.4byte 0x00000d48
.L_0804a3b0:
	ldr r0, [sp, #48]
	ldrb r2, [r6, #9]
	movs r1, #58
	ldrsh r3, [r0, r1]
	cmp r2, r3
	ble .L_0804a3c4
	movs r0, #2
	bl Func_08041f70
	b .L_0804a3d8
.L_0804a3c4:
	ldr r1, [sp, #48]
	movs r2, #62
	adds r2, #255
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804a3d8
	movs r0, #9
	bl Func_08041f70
.L_0804a3d8:
	ldr r0, [sp, #64]
	movs r3, #5
	strb r3, [r0, #7]
	ldr r0, .L_0804a4ec
	mov r1, r9
	adds r0, r5, r0
	lsls r5, r7, #4
	movs r2, #16
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffset
	ldrb r0, [r6, #9]
	movs r1, #2
	movs r3, #104
	mov r2, r9
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
	movs r0, #15
	bl Func_08041f70
	ldr r1, [sp, #64]
	movs r3, #15
	strb r3, [r1, #7]
	ldrb r3, [r6, #2]
	cmp r3, #4
	beq .L_0804a426
	movs r2, #160
	lsls r2, r2, #7
	adds r1, r3, #0
	adds r2, #1
	movs r3, #0
	adds r1, r1, r2
	str r3, [sp, #0]
	mov r0, r9
	movs r2, #15
	mov r3, r10
	bl UiWindow_SetTilemapEntry
.L_0804a426:
	ldrb r3, [r6, #8]
	cmp r3, #255
	bne .L_0804a430
	movs r3, #11
	b .L_0804a432
.L_0804a430:
	subs r3, #1
.L_0804a432:
	movs r0, #0
	str r0, [sp, #0]
	movs r1, #16
	mov r0, r9
	mov r2, r10
	adds r7, #1
	bl UiWindow_DrawThreeTileColumn
	cmp r7, #4
	bgt .L_0804a456
	mov r1, r11
	ldr r2, [sp, #72]
	adds r3, r1, r7
	lsls r3, r3, #1
	ldrh r5, [r3, r2]
	cmp r5, #0
	beq .L_0804a456
	b .L_0804a328
.L_0804a456:
	mov r3, r11
	str r7, [sp, #40]
	str r3, [sp, #60]
.L_0804a45c:
	ldr r0, [sp, #68]
	cmp r0, #5
	ble .L_0804a4b0
	movs r7, #0
	adds r0, #4
	mov r10, r0
	b .L_0804a4a2
.L_0804a46a:
	movs r1, #243
	lsls r1, r1, #8
	adds r1, #1
	adds r6, r7, r1
	mov r0, r11
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	bne .L_0804a486
	movs r2, #243
	lsls r2, r2, #8
	adds r2, #11
	adds r6, r7, r2
.L_0804a486:
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r0, #0
	subs r2, r2, r5
	adds r2, r2, r7
	movs r3, #1
	str r0, [sp, #0]
	subs r2, #2
	mov r0, r9
	adds r1, r6, #0
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_0804a4a2:
	mov r0, r10
	movs r1, #5
	bl __divsi3
	adds r5, r0, #0
	cmp r7, r5
	blt .L_0804a46a
.L_0804a4b0:
	mov r2, r9
	movs r1, #12
	ldrsh r0, [r2, r1]
	movs r3, #14
	ldrsh r1, [r2, r3]
	ldr r2, [sp, #8]
	mov r3, r9
	adds r1, r1, r2
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r0, #1
	adds r1, #1
	str r3, [sp, #0]
	subs r2, #2
	movs r3, #1
	bl Func_08046134
	ldr r0, [sp, #64]
	movs r3, #1
	movs r1, #0
	strb r3, [r0, #3]
	strb r1, [r0, #6]
.L_0804a4dc:
	ldr r2, [sp, #68]
	cmp r2, #5
	ble .L_0804a596
	movs r7, #0
	adds r2, #4
	mov r10, r2
	b .L_0804a53e
	.2byte 0x0000
.L_0804a4ec:
	.4byte 0x000005a7
.L_0804a4f0:
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #1
	adds r6, r7, r3
	ldr r3, .L_0804a61c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #11
	bhi .L_0804a518
	mov r0, r11
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	bne .L_0804a518
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r6, r7, r0
.L_0804a518:
	mov r1, r9
	ldrh r5, [r1, #8]
	mov r0, r10
	movs r1, #5
	bl __divsi3
	subs r5, r5, r0
	adds r5, r5, r7
	movs r2, #0
	subs r5, #2
	movs r3, #1
	str r2, [sp, #0]
	mov r0, r9
	adds r1, r6, #0
	adds r2, r5, #0
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_0804a53e:
	mov r0, r10
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	blt .L_0804a4f0
	mov r3, r9
	ldrh r2, [r3, #8]
	movs r5, #1
	movs r1, #243
	negs r5, r5
	subs r2, r2, r0
	lsls r1, r1, #8
	movs r0, #0
	str r0, [sp, #0]
	adds r3, r5, #0
	mov r0, r9
	subs r2, #3
	adds r1, #52
	bl UiWindow_SetTilemapEntry
	mov r1, r9
	ldrh r2, [r1, #8]
	movs r1, #243
	movs r3, #0
	lsls r1, r1, #8
	str r3, [sp, #0]
	subs r2, #2
	mov r0, r9
	adds r1, #53
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	mov r1, r9
	movs r0, #14
	ldrsh r3, [r1, r0]
	ldr r0, [sp, #64]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r0, #3]
	orrs r2, r3
	strb r2, [r0, #3]
.L_0804a596:
	ldr r1, [sp, #40]
	cmp r1, #0
	ble .L_0804a5b0
	ldr r5, [sp, #20]
	adds r7, r1, #0
.L_0804a5a0:
	adds r0, r5, #0
	movs r1, #240
	subs r7, #1
	bl Runtime_PushSlotEntry
	adds r5, #12
	cmp r7, #0
	bne .L_0804a5a0
.L_0804a5b0:
	mov r0, r9
	movs r2, #12
	ldrsh r3, [r0, r2]
	ldr r2, [sp, #8]
	lsls r3, r3, #3
	subs r3, #4
	str r3, [sp, #28]
	movs r1, #14
	ldrsh r3, [r0, r1]
	ldr r0, [sp, #12]
	adds r3, r2, r3
	lsls r3, r3, #3
	adds r3, #20
	str r3, [sp, #32]
	movs r3, #128
	lsls r3, r3, #23
	movs r1, #0
	str r3, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [sp, #52]
	ldr r1, .L_0804a620
	bl Resource_GetBuffer
	ldr r3, .L_0804a610
	ldr r2, [sp, #12]
	ands r0, r3
	ldrh r3, [r2, #8]
	ldr r2, .L_0804a614
	ldr r1, [sp, #28]
	ands r3, r2
	orrs r3, r0
	ldr r0, [sp, #12]
	strh r3, [r0, #8]
	ldr r3, .L_0804a61c
	ldr r0, [r3]
	movs r3, #4
	ands r0, r3
	movs r3, #255
	lsrs r2, r0, #1
	lsls r3, r3, #8
	adds r2, r1, r2
	adds r3, #252
	ldr r1, [sp, #12]
	adds r2, r2, r3
	ldr r3, .L_0804a618
	lsrs r0, r0, #2
	ands r2, r3
	b .L_0804a624
.L_0804a610:
	.4byte 0x000003ff
.L_0804a614:
	.4byte 0xfffffc00
.L_0804a618:
	.4byte 0x000001ff
.L_0804a61c:
	.4byte Data_0300122c
.L_0804a620:
	.4byte Data_080597f8
.L_0804a624:
	ldrh r3, [r1, #6]
	ldr r1, .L_0804a660
	ands r3, r1
	orrs r3, r2
	ldr r2, [sp, #12]
	strh r3, [r2, #6]
	ldr r3, [sp, #32]
	subs r0, r3, r0
	adds r0, #248
	strb r0, [r2, #4]
	ldr r0, [sp, #68]
	cmp r0, #0
	beq .L_0804a646
	ldr r0, [sp, #12]
	movs r1, #242
	bl Runtime_PushSlotEntry
.L_0804a646:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r1, [r3]
	mov r2, r11
	mov r3, r8
	str r2, [r1, #52]
	str r3, [r1, #48]
	ldr r0, [sp, #36]
	str r0, [r1, #56]
	ldr r0, .L_0804a664
	movs r2, #1
	b .L_0804a668
.L_0804a660:
	.4byte 0xfffffe00
.L_0804a664:
	.4byte gInput
.L_0804a668:
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0804a698
	ldr r1, [sp, #68]
	cmp r1, #0
	beq .L_0804a692
	ldr r2, [sp, #72]
	mov r6, r11
	add r6, r8
	lsls r3, r6, #1
	ldrh r0, [r3, r2]
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0804a690
	b .L_0804a816
.L_0804a690:
	b .L_0804a6b4
.L_0804a692:
	movs r6, #1
	negs r6, r6
	b .L_0804a816
.L_0804a698:
	ldr r3, [r1, #76]
	cmp r3, #0
	beq .L_0804a6a8
	ldr r3, [r0, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0804a6b4
.L_0804a6a8:
	movs r0, #113
	movs r6, #1
	bl Audio_PlayCue
	negs r6, r6
	b .L_0804a816
.L_0804a6b4:
	ldr r3, [sp, #68]
	cmp r3, #0
	bne .L_0804a6bc
	b .L_0804a80e
.L_0804a6bc:
	ldr r1, .L_0804a858
	movs r2, #128
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0804a6f2
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #5
	beq .L_0804a6e2
	ldr r2, [sp, #68]
	mov r3, r11
	add r3, r8
	cmp r3, r2
	bne .L_0804a6e6
.L_0804a6e2:
	movs r3, #0
	mov r8, r3
.L_0804a6e6:
	mov r1, r8
	mov r0, r8
	lsls r1, r1, #1
	str r0, [sp, #36]
	str r1, [sp, #8]
	b .L_0804a80e
.L_0804a6f2:
	ldr r3, [r1, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_0804a73c
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	bge .L_0804a730
	ldr r0, [sp, #68]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r11, r3
	bne .L_0804a72c
	ldr r0, [sp, #68]
	mov r1, r11
	subs r3, r0, r1
	subs r3, #1
	mov r8, r3
	b .L_0804a730
.L_0804a72c:
	movs r2, #4
	mov r8, r2
.L_0804a730:
	mov r0, r8
	mov r3, r8
	lsls r0, r0, #1
	str r3, [sp, #36]
	str r0, [sp, #8]
	b .L_0804a80e
.L_0804a73c:
	ldr r3, [r1, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804a79c
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	ldr r1, [sp, #68]
	mov r3, r11
	adds r3, #5
	cmp r3, r1
	blt .L_0804a770
	mov r2, r11
	cmp r2, #0
	beq .L_0804a80e
	ldr r0, [sp, #36]
	movs r3, #0
	mov r8, r0
	mov r1, r8
	lsls r1, r1, #1
	mov r11, r3
	str r1, [sp, #8]
	b .L_0804a80e
.L_0804a770:
	ldr r0, [sp, #68]
	ldr r2, [sp, #36]
	subs r0, #1
	movs r1, #5
	mov r11, r3
	mov r8, r2
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r11, r3
	bne .L_0804a7f0
	ldr r0, [sp, #68]
	mov r1, r11
	subs r3, r0, r1
	ldr r2, [sp, #36]
	subs r3, #1
	mov r8, r3
	cmp r8, r2
	ble .L_0804a7f8
	mov r8, r2
	b .L_0804a7f0
.L_0804a79c:
	ldr r3, [r1, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804a80e
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	mov r0, r11
	cmp r0, #0
	beq .L_0804a7c8
	ldr r2, [sp, #36]
	movs r1, #5
	mov r8, r2
	mov r3, r8
	negs r1, r1
	lsls r3, r3, #1
	add r11, r1
	str r3, [sp, #8]
	b .L_0804a80e
.L_0804a7c8:
	ldr r0, [sp, #68]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r0, [sp, #36]
	mov r11, r3
	mov r8, r0
	cmp r3, #0
	beq .L_0804a800
	ldr r1, [sp, #68]
	subs r3, r1, r3
	subs r3, #1
	mov r8, r3
	cmp r8, r0
	ble .L_0804a808
	mov r8, r0
	b .L_0804a808
.L_0804a7f0:
	mov r3, r8
	lsls r3, r3, #1
	str r3, [sp, #8]
	b .L_0804a80e
.L_0804a7f8:
	mov r0, r8
	lsls r0, r0, #1
	str r0, [sp, #8]
	b .L_0804a80e
.L_0804a800:
	mov r1, r8
	lsls r1, r1, #1
	str r1, [sp, #8]
	b .L_0804a80e
.L_0804a808:
	mov r2, r8
	lsls r2, r2, #1
	str r2, [sp, #8]
.L_0804a80e:
	movs r0, #1
	bl WaitFrames
	b .L_0804a27e
.L_0804a816:
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWork_Finalize
	mov r0, r9
	movs r1, #1
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #16]
	movs r7, #4
.L_0804a830:
	ldmia r5!, {r0}
	subs r7, #1
	bl Resource_ResetEntry
	cmp r7, #0
	bge .L_0804a830
	ldr r0, [sp, #52]
	bl Resource_ResetEntry
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #300
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804a858:
	.4byte gInput
