.syntax unified
	.thumb
	.global Func_080f94a4
	.thumb_func
Func_080f94a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #236
	movs r0, #220
	sub sp, #16
	bl HeapSlot_Alloc
	movs r1, #128
	lsls r1, r1, #6
	mov r10, r1
	adds r5, r0, #0
	mov r0, r10
	bl Heap_AllocEwram
	movs r7, #192
	ldr r3, .L_080f9510
	lsls r7, r7, #18
	ldr r2, [r7, #24]
	mov r11, r3
	movs r3, #1
	movs r1, #0
	strh r3, [r2, #4]
	adds r6, r0, #0
	movs r2, #30
	movs r3, #20
	movs r0, #0
	bl Window_DrawBorderFar
	movs r0, #1
	bl wait
	movs r0, #0
	bl Window_InitializeWork
	movs r1, #129
	lsls r1, r1, #2
	adds r0, r5, r1
	bl Party_ListActiveOwnersFar
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	movs r1, #3
	movs r2, #0
	b .L_080f9514
	.2byte 0x0000
.L_080f9510:
	.4byte 0x00000000
.L_080f9514:
	strb r0, [r3]
	movs r3, #7
	movs r0, #0
	bl Func_080fa368
	bl Resource_LoadPairedBlocks
	movs r0, #14
	bl Palette_LightenHighlight
	ldr r0, .L_080f95f0
	bl Link_DrawShiftedTilePairFar
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #17
	movs r3, #3
	movs r0, #13
	bl Window_CreateFar
	adds r3, r5, #0
	adds r3, #240
	str r0, [r3]
	bl Menu_SuspendFieldTasks
	ldr r3, .L_080f95f4
	ldr r1, .L_080f95f8
	mov r9, r3
	mov r2, r10
	adds r0, r6, #0
	mov lr, r9
	.2byte 0xf800
	ldr r3, .L_080f95fc
	mov r1, r10
	ldr r2, .L_080f9600
	ldr r0, .L_080f95f8
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	bl Text_UseExtraTilesFar
	bl Menu_CancelSoundReset
	add r1, sp, #8
	add r0, sp, #12
	add r2, sp, #4
	bl Func_080f9644
	mov r8, r0
	bl Menu_EnsureCancelSound
	mov r1, r8
	cmp r1, #1
	bne .L_080f95a4
	ldr r1, [sp, #12]
	ldr r3, [sp, #4]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	ldr r0, [r7, #108]
	lsls r1, r1, #10
	orrs r1, r3
	movs r3, #180
	lsls r3, r3, #1
	strh r1, [r0, r3]
	ldrh r3, [r5, r3]
	movs r1, #193
	lsls r1, r1, #1
	adds r2, r0, r1
	strh r3, [r2]
.L_080f95a4:
	ldr r0, [r5, #40]
	bl WindowSprite_ClearListFar
	ldr r2, [r7, #60]
	ldr r3, .L_080f95ec
	strb r3, [r2, #6]
	bl ItemMenu_Close
	movs r1, #0
	movs r2, #30
	movs r3, #20
	movs r0, #0
	bl Window_DrawBorderFar
	bl Menu_ResetTwoResourceEntries
	movs r0, #220
	bl HeapSlot_Release
	ldr r2, [r7, #24]
	movs r3, #0
	strh r3, [r2, #4]
	bl Func_08038290
	movs r0, #0
	bl Text_UseExtraTilesFar
	adds r1, r6, #0
	mov r2, r10
	ldr r0, .L_080f95f8
	mov lr, r9
	.2byte 0xf800
	ldr r3, [r7, #60]
	mov r2, r11
	b .L_080f9604
	.2byte 0x0000
.L_080f95ec:
	.4byte 0x00000001
.L_080f95f0:
	.4byte 0x06002500
.L_080f95f4:
	.4byte IwramCopyWords
.L_080f95f8:
	.4byte 0x06004000
.L_080f95fc:
	.4byte IwramFillWords
.L_080f9600:
	.4byte 0x33333333
.L_080f9604:
	strb r2, [r3, #6]
	adds r0, r6, #0
	bl Heap_Release
	movs r0, #1
	bl wait
	bl Menu_ResumeFieldTasks
	movs r0, #1
	bl wait
	movs r0, #0
	movs r2, #30
	movs r1, #0
	movs r3, #20
	bl Window_EraseBorderRectFar
	ldr r3, [r7, #60]
	mov r1, r11
	strb r1, [r3, #6]
	bl Event_ClearInvalidPackedValuesFar
	mov r0, r8
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
