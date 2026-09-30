.syntax unified
	.thumb
	.global RunAssetSelectionScreen
	.thumb_func
RunAssetSelectionScreen:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #6
	mov r9, r1
	mov r0, r9
	sub sp, #16
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #167
	adds r7, r0, #0
	lsls r1, r1, #4
	movs r0, #55
	bl Runtime_AllocateHeapBlock
	ldr r2, .L_080a2614
	mov r8, r2
	ldr r2, [r2]
	movs r3, #1
	movs r1, #0
	adds r5, r0, #0
	strh r3, [r2, #4]
	movs r0, #0
	movs r2, #30
	movs r3, #20
	bl UiWindow_DrawFrameFar
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	bl UiWindow_InitializeWork
	movs r3, #130
	lsls r3, r3, #2
	adds r0, r5, r3
	bl Party_ListActiveOwnersFar
	ldr r1, .L_080a2618
	adds r3, r5, r1
	movs r2, #0
	movs r1, #3
	strb r0, [r3]
	movs r3, #7
	movs r0, #0
	bl ItemMenu_Init
	bl Resource_LoadPairedBlocks
	movs r0, #14
	bl Palette_LightenBankHighlight
	ldr r0, .L_080a261c
	bl Link_DrawShiftedTilePairFar
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #17
	movs r3, #3
	movs r0, #13
	bl UiWindow_CreateFar
	movs r2, #134
	lsls r2, r2, #1
	adds r3, r5, r2
	str r0, [r3]
	bl Scheduler_EnableOverlayCallbacksWithFlags
	ldr r3, .L_080a2620
	ldr r1, .L_080a2624
	mov r11, r3
	mov r2, r9
	adds r0, r7, #0
	bl _call_via_fp
	ldr r3, .L_080a2628
	mov r1, r9
	ldr r2, .L_080a262c
	ldr r0, .L_080a2624
	bl _call_via_r3
	movs r0, #1
	bl Func_080153e0
	bl Menu_CancelSoundReset
	add r1, sp, #8
	add r0, sp, #12
	add r2, sp, #4
	bl ItemMenu_RunCommands
	mov r10, r0
	bl Menu_EnsureCancelSound
	mov r1, r10
	cmp r1, #1
	bne .L_080a25c2
	mov r2, r8
	ldr r0, [r2, #84]
	ldr r1, [sp, #12]
	ldr r3, [sp, #4]
	ldr r2, .L_080a2630
	lsls r1, r1, #10
	ands r3, r2
	subs r2, #127
	orrs r1, r3
	adds r3, r0, r2
	strh r1, [r3]
	movs r1, #186
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r1, #38
	adds r2, r0, r1
	strh r3, [r2]
.L_080a25c2:
	mov r6, r8
	ldr r0, [r5, #36]
	adds r6, #36
	bl RenderOutput_ClearListFar
	ldr r5, .L_080a2634
	ldr r2, [r6]
	ldr r3, .L_080a2610
	strb r3, [r2, r5]
	bl ItemMenu_Close
	movs r1, #0
	movs r2, #30
	movs r3, #20
	movs r0, #0
	bl UiWindow_DrawFrameFar
	bl Menu_ResetTwoResourceEntries
	movs r0, #55
	bl Runtime_ReleaseHeapBlock
	mov r3, r8
	ldr r2, [r3]
	movs r3, #0
	strh r3, [r2, #4]
	bl UiWindow_MarkVisibleTileAttributesFar
	movs r0, #0
	bl Func_080153e0
	mov r2, r9
	adds r1, r7, #0
	ldr r0, .L_080a2624
	bl _call_via_fp
	ldr r3, [r6]
	b .L_080a2638
	.2byte 0x0000
.L_080a2610:
	.4byte 0x00000001
.L_080a2614:
	.4byte gMenuCtrlWork
.L_080a2618:
	.4byte 0x00000219
.L_080a261c:
	.4byte 0x06002500
.L_080a2620:
	.4byte IwramCopyWords
.L_080a2624:
	.4byte 0x06004000
.L_080a2628:
	.4byte IwramFillWords
.L_080a262c:
	.4byte 0x33333333
.L_080a2630:
	.4byte 0x000001ff
.L_080a2634:
	.4byte 0x00000ea6
.L_080a2638:
	movs r1, #0
	adds r3, r3, r5
	strb r1, [r3]
	adds r0, r7, #0
	bl Runtime_BumpFree
	movs r0, #1
	bl WaitFrames
	bl Scheduler_DisableOverlayCallbacksWithFlags
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	movs r1, #0
	movs r2, #30
	movs r3, #20
	bl UiWindow_EraseBorderRectFar
	ldr r3, [r6]
	movs r2, #0
	adds r3, r3, r5
	strb r2, [r3]
	bl Event_ClearInvalidPackedValuesFar
	mov r0, r10
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
