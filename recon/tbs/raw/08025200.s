.syntax unified
	.thumb
	.global ItemList_SelectEntry
	.thumb_func
ItemList_SelectEntry:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #304
	str r0, [sp, #80]
	str r2, [sp, #72]
	str r1, [sp, #76]
	ldr r5, .L_080252bc
	movs r2, #1
	ldr r1, [r5]
	negs r2, r2
	movs r0, #128
	str r1, [sp, #68]
	str r2, [sp, #64]
	str r2, [sp, #60]
	bl Resource_LoadIntoFreeSlot
	movs r3, #42
	str r0, [sp, #56]
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #30
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	movs r3, #0
	str r0, [sp, #52]
	str r3, [sp, #48]
	adds r5, #168
	ldr r3, [r5]
	ldr r1, [r3, #52]
	ldr r2, [r3, #48]
	ldr r3, [r3, #56]
	mov r9, r1
	mov r8, r2
	str r3, [sp, #44]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #9
	movs r3, #11
	movs r0, #15
	movs r2, #15
	bl UiWindow_Create
	mov r3, sp
	adds r3, #84
	ldr r1, .L_080252c0
	movs r6, #128
	str r3, [sp, #16]
	mov r11, r0
	movs r7, #0
	mov r12, r1
	adds r4, r3, #0
	lsls r6, r6, #23
	movs r5, #0
.L_08025278:
	lsls r0, r7, #1
	str r6, [r4, #4]
	str r5, [r4, #8]
	mov r3, r11
	ldrh r2, [r3, #12]
	ldr r3, .L_080252b8
	lsls r2, r2, #3
	ldrh r1, [r4, #6]
	adds r2, #8
	ands r2, r3
	mov r3, r12
	ands r3, r1
	orrs r3, r2
	mov r1, r11
	strh r3, [r4, #6]
	ldrh r3, [r1, #14]
	adds r0, r0, r3
	lsls r0, r0, #3
	adds r0, #4
	adds r7, #1
	strb r0, [r4, #4]
	adds r4, #12
	cmp r7, #4
	ble .L_08025278
	mov r2, sp
	adds r2, #144
	ldr r3, .L_080252c4
	str r2, [sp, #28]
	ldr r6, [sp, #16]
	str r2, [sp, #8]
	movs r5, #8
	b .L_080252c8
.L_080252b8:
	.4byte 0x000001ff
.L_080252bc:
	.4byte gWindowWork
.L_080252c0:
	.4byte 0xfffffe00
.L_080252c4:
	.4byte 0xfffffc00
.L_080252c8:
	mov r10, r3
	movs r7, #4
.L_080252cc:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r2, [sp, #8]
	stmia r2!, {r0}
	adds r1, r2, #0
	str r1, [sp, #8]
	movs r1, #1
	negs r1, r1
	bl Resource_GetBuffer
	ldr r3, .L_08025318
	ands r0, r3
	ldrh r3, [r5, r6]
	mov r1, r10
	ands r3, r1
	orrs r3, r0
	subs r7, #1
	strh r3, [r5, r6]
	adds r5, #12
	cmp r7, #0
	bge .L_080252cc
	ldr r5, .L_0802531c
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Vram_CopyTile
	adds r0, r5, #0
	ldr r1, .L_08025320
	bl Vram_CopyTile
	adds r5, #1
	movs r1, #132
	lsls r1, r1, #2
	adds r0, r5, #0
	b .L_08025324
	.2byte 0x0000
.L_08025318:
	.4byte 0x000003ff
.L_0802531c:
	.4byte 0x0000f018
.L_08025320:
	.4byte 0x00000201
.L_08025324:
	bl Vram_CopyTile
	ldr r1, .L_080253c0
	adds r0, r5, #0
	bl Vram_CopyTile
	movs r2, #146
	lsls r2, r2, #1
	mov r3, r8
	mov r1, sp
	add r2, sp
	lsls r3, r3, #1
	adds r1, #164
	str r2, [sp, #24]
	str r3, [sp, #20]
	str r1, [sp, #32]
.L_08025344:
	ldr r2, [sp, #64]
	cmp r9, r2
	bne .L_08025352
	ldr r3, [sp, #60]
	cmp r8, r3
	bne .L_08025352
	b .L_0802552c
.L_08025352:
	ldr r1, [sp, #68]
	ldr r3, .L_080253c4
	adds r2, r1, r3
	movs r3, #1
	strb r3, [r2]
	ldr r2, [sp, #60]
	mov r1, r11
	ldrh r0, [r1, #12]
	ldrh r1, [r1, #14]
	lsls r3, r2, #1
	adds r1, r1, r3
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r3, #15
	adds r1, #1
	str r3, [sp, #0]
	adds r0, #1
	subs r2, #2
	movs r3, #1
	bl Ui_SetRectHighlight
	bl Ui_FillVramBlockPattern
	ldr r1, [sp, #72]
	cmp r1, #0
	beq .L_080253d0
	mov r3, r9
	add r3, r8
	ldr r2, [sp, #76]
	lsls r3, r3, #1
	adds r5, r3, r2
	ldrh r1, [r5]
	ldr r0, [sp, #80]
	bl Item_ClassifyUseAbility
	cmp r0, #2
	bne .L_080253a4
	ldr r5, [sp, #32]
	ldr r0, .L_080253c8
	adds r1, r5, #0
	b .L_080253b2
.L_080253a4:
	ldrh r3, [r5]
	ldr r0, .L_080253bc
	ldr r5, [sp, #32]
	ands r0, r3
	ldr r3, .L_080253cc
	adds r1, r5, #0
	adds r0, r0, r3
.L_080253b2:
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_080253dc
	.2byte 0x0000
.L_080253bc:
	.4byte 0x000001ff
.L_080253c0:
	.4byte 0x00000211
.L_080253c4:
	.4byte 0x00000ea6
.L_080253c8:
	.4byte 0x000008ee
.L_080253cc:
	.4byte 0x00000075
.L_080253d0:
	ldr r5, [sp, #32]
	ldr r0, .L_08025438
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_080253dc:
	ldr r1, [sp, #52]
	movs r3, #4
	adds r0, r5, #0
	movs r2, #0
	bl UiText_RenderWideStringAtOffset
	ldr r1, [sp, #64]
	mov r3, r8
	str r3, [sp, #60]
	cmp r9, r1
	beq .L_080254b2
	mov r0, r11
	bl RenderOutput_RedrawSavedRect
	mov r2, r9
	ldr r1, [sp, #76]
	lsls r3, r2, #1
	adds r3, r3, r1
	ldrh r5, [r3]
	movs r7, #0
	cmp r5, #0
	beq .L_080254ac
	ldr r1, [sp, #28]
	ldr r2, [sp, #16]
	adds r6, r3, #0
	movs r3, #8
	str r3, [sp, #12]
	str r1, [sp, #4]
	mov r10, r2
.L_08025416:
	adds r0, r5, #0
	bl Item_Get
	movs r0, #15
	bl UiWork_SetParamNibble
	adds r1, r5, #0
	ldr r0, [sp, #80]
	bl Item_ClassifyUseAbility
	cmp r0, #0
	beq .L_0802543c
	movs r0, #4
	bl UiWork_SetParamNibble
	b .L_0802544c
	.2byte 0x0000
.L_08025438:
	.4byte 0x000008e5
.L_0802543c:
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r5
	cmp r3, #0
	beq .L_0802544c
	movs r0, #2
	bl UiWork_SetParamNibble
.L_0802544c:
	ldr r0, .L_080254a4
	ldr r3, .L_080254a8
	ands r0, r5
	adds r0, r0, r3
	mov r1, r11
	lsls r3, r7, #4
	movs r2, #16
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl UiWork_SetParamNibble
	ldr r3, [sp, #4]
	ldmia r3!, {r1}
	adds r0, r5, #0
	adds r2, r3, #0
	str r2, [sp, #4]
	bl Resource_LoadKind26EntryToBuffer
	ldr r3, .L_0802549c
	ldr r1, [sp, #12]
	mov r2, r10
	ands r0, r3
	ldrh r3, [r1, r2]
	ldr r2, .L_080254a0
	ands r3, r2
	orrs r3, r0
	mov r2, r10
	strh r3, [r1, r2]
	adds r7, #1
	adds r1, #12
	str r1, [sp, #12]
	cmp r7, #4
	bgt .L_080254ac
	adds r6, #2
	ldrh r5, [r6]
	cmp r5, #0
	bne .L_08025416
	b .L_080254ac
	.2byte 0x0000
.L_0802549c:
	.4byte 0x000003ff
.L_080254a0:
	.4byte 0xfffffc00
.L_080254a4:
	.4byte 0x000001ff
.L_080254a8:
	.4byte 0x00000182
.L_080254ac:
	mov r3, r9
	str r7, [sp, #48]
	str r3, [sp, #64]
.L_080254b2:
	ldr r1, [sp, #72]
	cmp r1, #5
	ble .L_080254fc
	movs r7, #0
	adds r1, #4
	mov r10, r1
	b .L_080254ee
.L_080254c0:
	ldr r2, .L_0802553c
	mov r0, r9
	movs r1, #5
	adds r6, r7, r2
	bl __divsi3
	cmp r7, r0
	bne .L_080254d4
	ldr r3, .L_08025540
	adds r6, r7, r3
.L_080254d4:
	mov r1, r11
	ldrh r2, [r1, #8]
	subs r2, r2, r5
	adds r2, r2, r7
	movs r3, #0
	str r3, [sp, #0]
	subs r2, #2
	mov r0, r11
	adds r1, r6, #0
	subs r3, #1
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_080254ee:
	mov r0, r10
	movs r1, #5
	bl __divsi3
	adds r5, r0, #0
	cmp r7, r5
	blt .L_080254c0
.L_080254fc:
	mov r1, r11
	ldrh r0, [r1, #12]
	ldr r2, [sp, #20]
	ldrh r1, [r1, #14]
	mov r3, r11
	adds r1, r1, r2
	ldrh r2, [r3, #8]
	movs r3, #14
	adds r1, #1
	subs r2, #2
	str r3, [sp, #0]
	adds r0, #1
	movs r3, #1
	bl Ui_SetRectHighlight
	ldr r3, .L_08025544
	ldr r1, [sp, #68]
	adds r2, r1, r3
	movs r3, #1
	strb r3, [r2]
	ldr r2, .L_08025548
	adds r3, r1, r2
	movs r1, #0
	strb r1, [r3]
.L_0802552c:
	ldr r2, [sp, #72]
	cmp r2, #5
	ble .L_080255e4
	movs r7, #0
	adds r2, #4
	mov r10, r2
	b .L_08025592
	.2byte 0x0000
.L_0802553c:
	.4byte 0x0000f301
.L_08025540:
	.4byte 0x0000f30b
.L_08025544:
	.4byte 0x00000ea3
.L_08025548:
	.4byte 0x00000ea6
.L_0802554c:
	ldr r3, .L_0802566c
	ldr r1, .L_08025670
	adds r6, r7, r3
	ldr r3, [r1]
	movs r2, #15
	ands r3, r2
	cmp r3, #11
	bhi .L_0802556c
	mov r0, r9
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	bne .L_0802556c
	ldr r2, .L_08025674
	adds r6, r7, r2
.L_0802556c:
	mov r3, r11
	movs r1, #5
	mov r0, r10
	ldrh r5, [r3, #8]
	bl __divsi3
	subs r5, r5, r0
	adds r5, r5, r7
	movs r1, #0
	subs r5, #2
	movs r3, #1
	str r1, [sp, #0]
	mov r0, r11
	adds r1, r6, #0
	adds r2, r5, #0
	negs r3, r3
	bl UiWindow_SetTilemapEntry
	adds r7, #1
.L_08025592:
	mov r0, r10
	movs r1, #5
	bl __divsi3
	cmp r7, r0
	blt .L_0802554c
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r5, #1
	negs r5, r5
	subs r2, r2, r0
	movs r1, #0
	str r1, [sp, #0]
	mov r0, r11
	adds r3, r5, #0
	subs r2, #3
	ldr r1, .L_08025678
	bl UiWindow_SetTilemapEntry
	mov r3, r11
	ldrh r2, [r3, #8]
	movs r1, #0
	str r1, [sp, #0]
	subs r2, #2
	mov r0, r11
	ldr r1, .L_0802567c
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntry
	ldr r2, [sp, #68]
	ldr r3, .L_08025680
	adds r1, r2, r3
	mov r2, r11
	ldrh r3, [r2, #14]
	subs r3, #1
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1]
	orrs r2, r3
	strb r2, [r1]
.L_080255e4:
	ldr r3, [sp, #48]
	cmp r3, #0
	ble .L_080255fe
	ldr r5, [sp, #16]
	adds r7, r3, #0
.L_080255ee:
	adds r0, r5, #0
	movs r1, #240
	subs r7, #1
	bl Runtime_PushSlotEntry
	adds r5, #12
	cmp r7, #0
	bne .L_080255ee
.L_080255fe:
	mov r1, r11
	ldrh r3, [r1, #12]
	lsls r3, r3, #3
	subs r3, #2
	ldr r2, [sp, #20]
	str r3, [sp, #36]
	ldrh r3, [r1, #14]
	adds r3, r2, r3
	lsls r3, r3, #3
	adds r3, #20
	ldr r1, [sp, #24]
	str r3, [sp, #40]
	movs r3, #128
	lsls r3, r3, #23
	movs r2, #0
	str r3, [r1, #4]
	str r2, [r1, #8]
	ldr r0, [sp, #56]
	ldr r1, .L_08025684
	bl Resource_GetBuffer
	ldr r3, .L_0802565c
	ldr r1, [sp, #24]
	ands r0, r3
	ldr r2, .L_08025660
	ldrh r3, [r1, #8]
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	ldr r3, .L_08025670
	ldr r2, [r3]
	movs r0, #4
	ldr r1, [sp, #36]
	ands r2, r0
	ldr r3, .L_08025688
	lsrs r2, r2, #1
	adds r2, r1, r2
	adds r2, r2, r3
	ldr r1, [sp, #24]
	ldr r3, .L_08025664
	ands r2, r3
	ldrh r3, [r1, #6]
	ldr r1, .L_08025668
	ands r3, r1
	b .L_0802568c
	.2byte 0x0000
.L_0802565c:
	.4byte 0x000003ff
.L_08025660:
	.4byte 0xfffffc00
.L_08025664:
	.4byte 0x000001ff
.L_08025668:
	.4byte 0xfffffe00
.L_0802566c:
	.4byte 0x0000f301
.L_08025670:
	.4byte gFrameCount
.L_08025674:
	.4byte 0x0000f30b
.L_08025678:
	.4byte 0x0000f334
.L_0802567c:
	.4byte 0x0000f335
.L_08025680:
	.4byte 0x00000ea3
.L_08025684:
	.4byte Resource_FixedBlockBTiles
.L_08025688:
	.4byte 0x0000fffc
.L_0802568c:
	orrs r3, r2
	ldr r1, .L_08025910
	ldr r2, [sp, #24]
	strh r3, [r2, #6]
	ldr r3, [r1]
	ldr r2, [sp, #40]
	ands r3, r0
	lsrs r3, r3, #2
	subs r3, r2, r3
	ldr r1, [sp, #24]
	adds r3, #248
	strb r3, [r1, #4]
	ldr r2, [sp, #72]
	cmp r2, #0
	beq .L_080256b2
	ldr r0, [sp, #24]
	movs r1, #242
	bl Runtime_PushSlotEntry
.L_080256b2:
	ldr r3, .L_08025914
	ldr r1, [r3]
	mov r2, r8
	mov r3, r9
	str r3, [r1, #52]
	str r2, [r1, #48]
	ldr r3, [sp, #44]
	str r3, [r1, #56]
	ldr r0, .L_08025918
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08025754
	ldr r1, [sp, #72]
	cmp r1, #0
	beq .L_0802574c
	mov r2, r9
	add r2, r8
	ldr r1, [sp, #76]
	lsls r3, r2, #1
	adds r5, r3, r1
	ldrh r0, [r5]
	movs r7, #128
	mov r10, r2
	lsls r7, r7, #3
	bl Item_Get
	ldrh r2, [r5]
	adds r3, r7, #0
	ands r3, r2
	movs r6, #0
	cmp r3, #0
	bne .L_08025706
	ldrh r1, [r5]
	ldr r0, [sp, #80]
	bl Item_ClassifyUseAbility
	adds r6, r0, #0
	cmp r6, #0
	bne .L_08025706
	b .L_080258c8
.L_08025706:
	movs r0, #114
	bl AudioCommand_PlayFar
	cmp r6, #2
	bne .L_08025716
	ldr r5, [sp, #32]
	ldr r0, .L_0802591c
	b .L_08025724
.L_08025716:
	ldrh r2, [r5]
	adds r3, r7, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0802572e
	ldr r5, [sp, #32]
	ldr r0, .L_08025920
.L_08025724:
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
	b .L_0802573a
.L_0802572e:
	ldr r5, [sp, #32]
	ldr r0, .L_08025924
	adds r1, r5, #0
	movs r2, #52
	bl UiText_CopyMessageString
.L_0802573a:
	bl Ui_FillVramBlockPattern
	adds r0, r5, #0
	ldr r1, [sp, #52]
	movs r2, #0
	movs r3, #4
	bl UiText_RenderWideStringAtOffset
	b .L_08025772
.L_0802574c:
	movs r2, #1
	negs r2, r2
	mov r10, r2
	b .L_080258c8
.L_08025754:
	ldr r3, [r1, #76]
	cmp r3, #0
	beq .L_08025764
	ldr r3, [r0]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08025772
.L_08025764:
	movs r0, #113
	bl AudioCommand_PlayFar
	movs r3, #1
	negs r3, r3
	mov r10, r3
	b .L_080258c8
.L_08025772:
	ldr r1, [sp, #72]
	cmp r1, #0
	bne .L_0802577a
	b .L_080258c0
.L_0802577a:
	ldr r1, .L_08025928
	ldr r3, [r1]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080257b0
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #5
	beq .L_080257a0
	mov r3, r9
	ldr r1, [sp, #72]
	add r3, r8
	cmp r3, r1
	bne .L_080257a4
.L_080257a0:
	movs r2, #0
	mov r8, r2
.L_080257a4:
	mov r1, r8
	mov r3, r8
	lsls r1, r1, #1
	str r3, [sp, #44]
	str r1, [sp, #20]
	b .L_080258c0
.L_080257b0:
	ldr r3, [r1]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080257f6
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	bge .L_080257ec
	ldr r0, [sp, #72]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r9, r3
	bne .L_080257e8
	ldr r1, [sp, #72]
	mov r2, r9
	subs r3, r1, r2
	subs r3, #1
	b .L_080257ea
.L_080257e8:
	movs r3, #4
.L_080257ea:
	mov r8, r3
.L_080257ec:
	mov r2, r8
	mov r1, r8
	lsls r2, r2, #1
	str r1, [sp, #44]
	b .L_080258be
.L_080257f6:
	ldr r3, [r1]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_08025854
	movs r0, #111
	bl AudioCommand_PlayFar
	bl Runtime_SetMainState19
	mov r3, r9
	ldr r1, [sp, #72]
	adds r3, #5
	cmp r3, r1
	blt .L_08025828
	mov r2, r9
	cmp r2, #0
	beq .L_080258c0
	ldr r1, [sp, #44]
	mov r8, r1
	mov r2, r8
	movs r3, #0
	lsls r2, r2, #1
	mov r9, r3
	b .L_080258be
.L_08025828:
	ldr r0, [sp, #72]
	mov r9, r3
	ldr r3, [sp, #44]
	subs r0, #1
	movs r1, #5
	mov r8, r3
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	cmp r9, r3
	bne .L_080258ba
	ldr r1, [sp, #72]
	mov r2, r9
	subs r3, r1, r2
	subs r3, #1
	mov r8, r3
	ldr r3, [sp, #44]
	cmp r8, r3
	ble .L_080258aa
	mov r8, r3
	b .L_080258b2
.L_08025854:
	ldr r3, [r1]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080258c0
	movs r0, #111
	bl AudioCommand_PlayFar
	bl Runtime_SetMainState19
	mov r2, r9
	cmp r2, #0
	beq .L_0802587e
	ldr r1, [sp, #44]
	mov r8, r1
	movs r3, #5
	mov r2, r8
	negs r3, r3
	lsls r2, r2, #1
	add r9, r3
	b .L_080258be
.L_0802587e:
	ldr r0, [sp, #72]
	movs r1, #5
	subs r0, #1
	bl __divsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	mov r9, r3
	ldr r3, [sp, #44]
	mov r1, r9
	mov r8, r3
	cmp r1, #0
	beq .L_080258b2
	ldr r2, [sp, #72]
	subs r3, r2, r1
	subs r3, #1
	mov r8, r3
	ldr r3, [sp, #44]
	cmp r8, r3
	ble .L_080258ba
	mov r8, r3
	b .L_080258b2
.L_080258aa:
	mov r3, r8
	lsls r3, r3, #1
	str r3, [sp, #20]
	b .L_080258c0
.L_080258b2:
	mov r1, r8
	lsls r1, r1, #1
	str r1, [sp, #20]
	b .L_080258c0
.L_080258ba:
	mov r2, r8
	lsls r2, r2, #1
.L_080258be:
	str r2, [sp, #20]
.L_080258c0:
	movs r0, #1
	bl WaitFrames
	b .L_08025344
.L_080258c8:
	ldr r0, [sp, #52]
	movs r1, #1
	bl UiWork_Finalize
	movs r1, #1
	mov r0, r11
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #56]
	bl Resource_ResetEntry
	ldr r5, [sp, #28]
	movs r7, #4
.L_080258e8:
	ldmia r5!, {r0}
	subs r7, #1
	bl Resource_ResetEntry
	cmp r7, #0
	bge .L_080258e8
	movs r0, #1
	bl WaitFrames
	mov r0, r10
	add sp, #304
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08025910:
	.4byte gFrameCount
.L_08025914:
	.4byte gLinkCountdownWork
.L_08025918:
	.4byte gKeyState
.L_0802591c:
	.4byte 0x000008ee
.L_08025920:
	.4byte 0x000008ec
.L_08025924:
	.4byte 0x000008eb
.L_08025928:
	.4byte gKeysRepeat
