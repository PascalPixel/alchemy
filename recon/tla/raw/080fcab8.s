.syntax unified
	.thumb
	.global Func_080fcab8
	.thumb_func
Func_080fcab8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #236
	movs r0, #220
	sub sp, #16
	bl Runtime_AllocateHeapBlock
	movs r1, #192
	lsls r1, r1, #18
	ldr r2, [r1, #24]
	movs r3, #1
	adds r6, r0, #0
	strh r3, [r2, #4]
	movs r0, #0
	movs r3, #20
	movs r2, #30
	mov r8, r1
	movs r1, #0
	bl UiWindow_DrawFrameFar
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	bl UiWindow_InitializeWork
	movs r2, #129
	lsls r2, r2, #2
	adds r0, r6, r2
	bl Party_ListActiveOwnersFar
	movs r1, #139
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r6, r1
	strb r0, [r3]
	movs r1, #3
	movs r0, #0
	movs r2, #0
	movs r3, #7
	bl Func_080fa368
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #17
	movs r3, #3
	movs r0, #13
	bl UiWindow_CreateFar
	adds r3, r6, #0
	adds r3, #240
	str r0, [r3]
	movs r0, #14
	bl Func_080f9108
	ldr r0, .L_080fcbc4
	bl Link_DrawShiftedTilePairFar
	bl Func_080f9448
	add r0, sp, #12
	add r1, sp, #8
	add r2, sp, #4
	bl Func_080fcbd8
	adds r7, r0, #0
	bl Func_080f9464
	cmp r7, #1
	bne .L_080fcb74
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	mov r2, r8
	ands r0, r3
	ldr r5, [r2, #108]
	bl BattleAction_Get
	ldr r3, [sp, #12]
	ldr r2, [sp, #4]
	movs r1, #179
	lsls r3, r3, #10
	lsls r1, r1, #1
	orrs r2, r3
	adds r3, r5, r1
	strh r2, [r3]
.L_080fcb74:
	ldr r0, [r6, #40]
	bl RenderOutput_ClearListFar
	mov r3, r8
	ldr r2, [r3, #60]
	ldr r3, .L_080fcbbc
	strb r3, [r2, #6]
	bl Func_080fa478
	movs r1, #0
	movs r2, #30
	movs r3, #20
	movs r0, #0
	bl UiWindow_DrawFrameFar
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	mov r1, r8
	ldr r2, [r1, #24]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	movs r1, #0
	movs r2, #30
	movs r3, #20
	bl Func_080383f0
	mov r3, r8
	ldr r2, [r3, #60]
	ldr r3, .L_080fcbc0
	b .L_080fcbc8
	.2byte 0x0000
.L_080fcbbc:
	.4byte 0x00000001
.L_080fcbc0:
	.4byte 0x00000000
.L_080fcbc4:
	.4byte 0x06002500
.L_080fcbc8:
	strb r3, [r2, #6]
	bl Event_ClearInvalidPackedValuesFar
	adds r0, r7, #0
	add sp, #16
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
