.syntax unified
	.thumb
	.global Ui_BuildPairedPatternsToSlot
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	movs r1, #193
	sub sp, #4
	adds r6, r0, #0
	lsls r1, r1, #3
	movs r0, #68
	str r3, [sp, #0]
	mov r11, r2
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_0803d58c
	lsls r5, r5, #2
	movs r2, #192
	ldr r3, [r3, r5]
	lsls r2, r2, #3
	adds r7, r0, #0
	adds r2, #4
	adds r2, r2, r7
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, r3, r7
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #2
	adds r3, r3, r7
	movs r5, #2
	mov r8, r3
	mov r9, r2
	mov r2, r10
	strh r5, [r2]
	mov r2, r8
	strh r5, [r2]
	movs r1, #0
	bl UiGlyph_DecodeWithHeapRoutines
	ldr r3, .L_0803d590
	lsls r6, r6, #2
	ldr r3, [r3, r6]
	mov r2, r9
	str r3, [r2]
	mov r3, r10
	mov r2, r8
	strh r5, [r3]
	adds r0, r7, #0
	strh r5, [r2]
	movs r1, #1
	bl UiGlyph_DecodeWithHeapRoutines
	ldr r3, [sp, #36]
	cmp r3, #0
	bne .L_0803d564
	bl Resource_FindFreeEntry
	mov r2, r11
	str r0, [r2]
.L_0803d564:
	mov r3, r11
	ldr r0, [r3]
	movs r3, #128
	lsls r3, r3, #3
	adds r2, r7, r3
	movs r1, #128
	bl VramBlock_LoadCached
	ldr r2, [sp, #0]
	str r0, [r2]
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0803d58c:
	.4byte Data_0804e684
.L_0803d590:
	.4byte UiIcon_OverlayPointerTable
