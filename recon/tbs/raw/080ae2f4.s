.syntax unified
	.thumb
	.global Unnamed_080ae2f4
	.thumb_func
Unnamed_080ae2f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080ae338
	sub sp, #84
	movs r1, #1
	movs r2, #0
	movs r0, #0
	ldr r3, [r3]
	str r1, [sp, #52]
	str r0, [sp, #56]
	str r2, [sp, #48]
	ldr r2, [r3, #20]
	mov r9, r3
	movs r3, #13
	strb r3, [r2, #5]
	mov r3, sp
	adds r3, #76
	movs r7, #0
	str r3, [sp, #20]
	str r7, [sp, #76]
	str r7, [r3, #4]
	movs r3, #165
	lsls r3, r3, #1
	ldr r1, .L_080ae334
	movs r2, #3
	add r3, r9
	b .L_080ae33c
.L_080ae334:
	.4byte 0x000000c8
.L_080ae338:
	.4byte Data_03001f2c
.L_080ae33c:
	subs r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #0
	bge .L_080ae33c
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_ClearListFar
	movs r0, #1
	bl WaitFrames
	add r2, sp, #68
	movs r7, #1
	str r7, [sp, #68]
	movs r0, #96
	str r7, [r2, #4]
	mov r11, r2
	bl Runtime_BumpAllocateAlternatePool
	adds r5, r0, #0
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	ldr r3, .L_080ae6c8
	add r3, r9
	adds r6, r0, #0
	ldrb r0, [r3]
	bl Func_08077008
	adds r1, r0, #0
	add r2, sp, #60
	adds r1, #88
	add r3, sp, #64
	str r2, [sp, #0]
	adds r0, r1, #0
	adds r2, r5, #0
	bl OwnerAction_DiffSlots
	mov r3, r11
	str r0, [sp, #68]
	str r0, [r3, #4]
	adds r0, r6, #0
	bl Runtime_BumpFree
	adds r0, r5, #0
	bl Runtime_BumpFree
	ldr r0, [sp, #68]
	movs r1, #6
	subs r0, #1
	bl FixedPoint_Ratio
	adds r0, #1
	str r0, [sp, #68]
	cmp r0, #0
	bne .L_080ae3b2
	str r7, [sp, #68]
.L_080ae3b2:
	mov r1, r11
	ldr r0, [r1, #4]
	movs r1, #6
	subs r0, #1
	bl FixedPoint_Ratio
	mov r2, r11
	adds r0, #1
	str r0, [r2, #4]
	cmp r0, #0
	bne .L_080ae3ca
	str r7, [r2, #4]
.L_080ae3ca:
	mov r3, r9
	adds r3, #36
	str r3, [sp, #44]
	movs r5, #2
	movs r6, #15
	adds r0, r3, #0
	movs r1, #0
	movs r2, #5
	movs r3, #15
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl UiWindow_UpdateOrCreate
	mov r0, r9
	movs r2, #5
	movs r1, #15
	adds r0, #52
	movs r3, #15
	str r5, [sp, #4]
	str r0, [sp, #40]
	str r6, [sp, #0]
	bl UiWindow_UpdateOrCreate
	movs r3, #134
	lsls r3, r3, #1
	add r3, r9
	ldr r0, [r3]
	bl RenderOutput_RedrawSavedRectFar
	mov r1, r9
	ldr r0, [r1, #16]
	bl RenderOutput_RedrawSavedRectFar
	ldr r5, .L_080ae6cc
	mov r2, r9
	ldr r1, [r2, #16]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #2
	mov r3, r9
	ldr r1, [r3, #16]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #150
	lsls r2, r2, #2
	ldr r1, [sp, #20]
	add r2, r9
	movs r0, #0
	str r2, [sp, #24]
	mov r8, r0
	mov r10, r1
.L_080ae43c:
	ldr r3, .L_080ae6d0
	ldr r3, [r3]
	str r3, [sp, #36]
	ldr r3, .L_080ae6d4
	ldr r3, [r3]
	str r3, [sp, #32]
	ldr r3, .L_080ae6d8
	ldr r3, [r3]
	str r3, [sp, #28]
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_080ae4b6
	ldr r0, [sp, #36]
	ldr r1, .L_080ae6dc
	movs r2, #1
	adds r5, r0, r1
	strb r2, [r5]
	mov r3, r9
	ldr r0, [r3, #36]
	bl RenderOutput_PrepareForRedrawFar
	mov r1, r9
	ldr r0, [r1, #52]
	bl RenderOutput_PrepareForRedrawFar
	mov r2, r9
	ldr r1, [sp, #24]
	ldr r0, [r2, #36]
	mov r2, r8
	ldrb r3, [r1]
	str r2, [sp, #0]
	str r2, [sp, #4]
	str r2, [sp, #12]
	movs r1, #3
	movs r2, #1
	str r1, [sp, #8]
	str r2, [sp, #16]
	movs r1, #0
	movs r2, #0
	bl DjinnMenu_DrawStatPreview
	ldr r1, [sp, #24]
	mov r2, r8
	mov r3, r9
	ldr r0, [r3, #52]
	ldrb r3, [r1]
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r2, [sp, #76]
	adds r2, #1
	movs r1, #3
	str r2, [sp, #12]
	movs r2, #1
	str r1, [sp, #8]
	str r2, [sp, #16]
	movs r1, #0
	movs r2, #0
	bl DjinnMenu_DrawStatPreview
	mov r3, r8
	strb r3, [r5]
.L_080ae4b6:
	movs r0, #0
	mov r1, r11
	ldr r3, [r0, r1]
	cmp r3, #1
	ble .L_080ae54a
	mov r2, r9
	movs r5, #0
	ldr r6, [r2, #52]
	cmp r5, r3
	bge .L_080ae506
	movs r7, #0
	add r7, r11
.L_080ae4ce:
	ldr r3, .L_080ae6e0
	adds r1, r5, r3
	cmp r5, #9
	ble .L_080ae4d8
	ldr r1, .L_080ae6e4
.L_080ae4d8:
	ldr r2, [sp, #20]
	movs r0, #0
	ldr r3, [r0, r2]
	cmp r5, r3
	bne .L_080ae4e6
	ldr r3, .L_080ae6e8
	adds r1, r1, r3
.L_080ae4e6:
	ldr r3, [r7]
	ldrh r2, [r6, #8]
	subs r2, r2, r3
	adds r2, r2, r5
	mov r0, r8
	movs r3, #1
	str r0, [sp, #0]
	negs r3, r3
	subs r2, #2
	adds r0, r6, #0
	bl UiWindow_SetTilemapEntryFar
	ldr r3, [r7]
	adds r5, #1
	cmp r5, r3
	blt .L_080ae4ce
.L_080ae506:
	mov r0, r11
	movs r1, #0
	ldr r3, [r1, r0]
	ldrh r2, [r6, #8]
	movs r5, #1
	negs r5, r5
	mov r1, r8
	subs r2, r2, r3
	str r1, [sp, #0]
	adds r0, r6, #0
	adds r3, r5, #0
	subs r2, #3
	ldr r1, .L_080ae6ec
	bl UiWindow_SetTilemapEntryFar
	ldrh r2, [r6, #8]
	mov r3, r8
	str r3, [sp, #0]
	subs r2, #2
	adds r0, r6, #0
	ldr r1, .L_080ae6f0
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntryFar
	ldr r0, [sp, #36]
	ldr r2, .L_080ae6f4
	adds r1, r0, r2
	ldrh r2, [r6, #14]
	movs r3, #2
	lsrs r2, r2, #2
	lsls r3, r2
	ldrb r2, [r1]
	orrs r3, r2
	strb r3, [r1]
.L_080ae54a:
	ldr r3, [sp, #48]
	adds r3, #1
	adds r0, r3, #0
	movs r1, #60
	str r3, [sp, #48]
	bl Func_080022fc
	subs r0, #5
	movs r0, #0
	movs r1, #32
	movs r2, #200
	movs r3, #0
	bl FourObjectMotion_SetSlotPosition
	ldr r0, [sp, #52]
	cmp r0, #0
	beq .L_080ae57a
	movs r1, #0
	str r1, [sp, #52]
	ldr r0, [sp, #56]
	movs r1, #2
	bl Menu_GetModuloOfSum
	str r0, [sp, #56]
.L_080ae57a:
	ldr r3, [sp, #48]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080ae5a8
	ldr r0, [sp, #48]
	movs r3, #4
	ands r3, r0
	cmp r3, #0
	beq .L_080ae59c
	ldr r3, .L_080ae6f8
	ldr r0, .L_080ae6fc
	ldr r1, .L_080ae700
	movs r2, #32
	bl _call_via_r3
	b .L_080ae5a8
.L_080ae59c:
	ldr r3, .L_080ae704
	ldr r0, .L_080ae6fc
	movs r1, #32
	ldr r2, .L_080ae708
	bl _call_via_r3
.L_080ae5a8:
	ldr r1, [sp, #32]
	movs r3, #8
	ands r3, r1
	cmp r3, #0
	beq .L_080ae5b8
	movs r0, #113
	movs r7, #2
	b .L_080ae5c6
.L_080ae5b8:
	ldr r3, .L_080ae70c
	ldr r2, [sp, #32]
	ands r3, r2
	cmp r3, #0
	beq .L_080ae5ce
	movs r0, #113
	movs r7, #1
.L_080ae5c6:
	bl Func_080f9010
	negs r7, r7
	b .L_080ae638
.L_080ae5ce:
	ldr r0, [sp, #28]
	movs r3, #32
	ands r3, r0
	cmp r3, #0
	beq .L_080ae5fe
	mov r1, r10
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	mov r2, r8
	mov r3, r11
	ldr r1, [r2, r3]
	bl Menu_GetModuloOfSum
	mov r1, r10
	str r0, [r1]
	movs r0, #111
	bl Func_080f9010
	bl Runtime_SetMainState19
	movs r2, #1
	str r2, [sp, #52]
	b .L_080ae630
.L_080ae5fe:
	ldr r0, [sp, #28]
	movs r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_080ae630
	mov r1, r10
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	movs r0, #111
	bl Func_080f9010
	bl Runtime_SetMainState19
	movs r2, #1
	str r2, [sp, #52]
	mov r3, r10
	ldr r0, [r3]
	mov r2, r8
	mov r3, r11
	ldr r1, [r2, r3]
	bl Menu_GetModuloOfSum
	mov r1, r10
	str r0, [r1]
.L_080ae630:
	movs r0, #1
	bl WaitFrames
	b .L_080ae43c
.L_080ae638:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_080ae710
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080ae6d0
	movs r5, #134
	mov r8, r2
	ldr r6, .L_080ae6dc
	ldr r2, [r2]
	movs r3, #0
	lsls r5, r5, #1
	mov r10, r3
	add r5, r9
	movs r3, #1
	strb r3, [r2, r6]
	adds r0, r5, #0
	movs r1, #1
	bl UiWindow_CloseIfOpen
	movs r0, #1
	bl WaitFrames
	movs r3, #5
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r2, #0
	movs r3, #17
	adds r0, r5, #0
	movs r1, #13
	bl UiWindow_UpdateOrCreate
	movs r1, #1
	ldr r0, [sp, #44]
	bl UiWindow_CloseIfOpen
	movs r1, #1
	ldr r0, [sp, #40]
	bl UiWindow_CloseIfOpen
	mov r1, r9
	ldr r0, [r1, #48]
	bl RenderOutput_RedrawSavedRectFar
	mov r2, r9
	ldr r0, [r2, #40]
	bl RenderOutput_RedrawSavedRectFar
	mov r3, r9
	ldr r0, [r3, #16]
	bl RenderOutput_RedrawSavedRectFar
	mov r0, r8
	ldr r3, [r0]
	mov r1, r10
	adds r3, r3, r6
	strb r1, [r3]
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080ae6c8:
	.4byte 0x0000021a
.L_080ae6cc:
	.4byte 0x00000baa
.L_080ae6d0:
	.4byte Data_03001e8c
.L_080ae6d4:
	.4byte gKeyState
.L_080ae6d8:
	.4byte gKeysRepeat
.L_080ae6dc:
	.4byte 0x00000ea6
.L_080ae6e0:
	.4byte 0x0000f031
.L_080ae6e4:
	.4byte 0x0000f030
.L_080ae6e8:
	.4byte 0xfffff000
.L_080ae6ec:
	.4byte 0x0000f128
.L_080ae6f0:
	.4byte 0x0000f129
.L_080ae6f4:
	.4byte 0x00000ea3
.L_080ae6f8:
	.4byte IwramCopyWords
.L_080ae6fc:
	.4byte 0x060052c0
.L_080ae700:
	.4byte Menu_BackdropFrameTile
.L_080ae704:
	.4byte IwramFillWords
.L_080ae708:
	.4byte 0x44444444
.L_080ae70c:
	.4byte 0x00000303
.L_080ae710:
	.4byte Menu_UpdateEntryObjectTransforms
