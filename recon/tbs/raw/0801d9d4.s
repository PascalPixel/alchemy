.syntax unified
	.thumb
	.global Menu_CreateWorkspaceWindows
	.thumb_func
Menu_CreateWorkspaceWindows:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0801db50
	movs r0, #3
	ldr r3, [r3]
	mov r10, r0
	movs r0, #191
	lsls r0, r0, #1
	sub sp, #12
	mov r11, r3
	bl Func_080770c0
	movs r2, #0
	mov r9, r0
	str r2, [sp, #8]
	cmp r0, #0
	beq .L_0801da08
	movs r0, #2
	movs r3, #1
	str r0, [sp, #8]
	mov r10, r3
.L_0801da08:
	ldr r3, .L_0801db54
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0801da14
	movs r2, #3
	add r10, r2
.L_0801da14:
	movs r3, #8
	mov r0, r10
	subs r1, r3, r0
	lsls r3, r0, #1
	add r3, r10
	adds r4, r3, #1
	adds r3, r1, r4
	cmp r3, #19
	ble .L_0801da2a
	movs r1, #1
	movs r4, #19
.L_0801da2a:
	movs r3, #2
	str r3, [sp, #0]
	movs r2, #20
	movs r0, #5
	adds r3, r4, #0
	bl UiWindow_Create
	mov r2, r10
	mov r8, r0
	cmp r2, #1
	ble .L_0801da5c
	mov r5, r10
	movs r6, #3
	subs r5, #1
.L_0801da46:
	adds r2, r6, #0
	mov r0, r8
	movs r1, #0
	movs r3, #19
	subs r5, #1
	str r6, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r6, #3
	cmp r5, #0
	bne .L_0801da46
.L_0801da5c:
	mov r3, r9
	movs r7, #4
	cmp r3, #0
	bne .L_0801da82
	ldr r5, .L_0801db58
	mov r1, r8
	adds r0, r5, #0
	movs r2, #48
	movs r3, #4
	adds r5, #1
	bl UiText_DrawResource
	adds r0, r5, #0
	mov r1, r8
	movs r2, #48
	movs r3, #28
	bl UiText_DrawResource
	movs r7, #52
.L_0801da82:
	adds r3, r7, #0
	ldr r0, .L_0801db5c
	mov r1, r8
	movs r2, #48
	bl UiText_DrawResource
	ldr r3, .L_0801db54
	ldrb r3, [r3]
	adds r7, #24
	cmp r3, #0
	beq .L_0801dac4
	ldr r5, .L_0801db60
	adds r3, r7, #0
	adds r0, r5, #0
	mov r1, r8
	movs r2, #48
	adds r7, #24
	bl UiText_DrawResource
	adds r0, r5, #1
	adds r3, r7, #0
	mov r1, r8
	movs r2, #48
	adds r7, #24
	adds r5, #2
	bl UiText_DrawResource
	adds r0, r5, #0
	mov r1, r8
	movs r2, #48
	adds r3, r7, #0
	bl UiText_DrawResource
.L_0801dac4:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_0801db02
	ldr r2, .L_0801db64
	movs r1, #128
	bl VramBlock_LoadCached
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r8
	adds r0, r5, #0
	str r3, [sp, #0]
	bl RenderOutput_Create
	ldr r2, .L_0801db68
	add r2, r11
	str r0, [r2]
	mov r0, r8
	ldrh r3, [r0, #14]
	lsls r3, r3, #3
	ldrh r1, [r0, #12]
	adds r7, r3, #0
	adds r7, #16
	adds r0, r2, #0
	lsls r1, r1, #3
	adds r2, r7, #0
	bl Func_080b0038
.L_0801db02:
	movs r7, #4
	mov r2, r10
	negs r7, r7
	cmp r2, #0
	ble .L_0801db3c
	ldr r3, .L_0801db6c
	movs r4, #194
	ldr r0, [sp, #8]
	lsls r4, r4, #3
	add r4, r11
	mov r5, r10
	adds r6, r0, r3
.L_0801db1a:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	movs r1, #0
	mov r2, r8
	movs r3, #12
	str r7, [sp, #0]
	str r4, [sp, #4]
	bl RenderResource_CreateFrame
	ldr r4, [sp, #4]
	subs r5, #1
	adds r6, #1
	stmia r4!, {r0}
	adds r7, #24
	cmp r5, #0
	bne .L_0801db1a
.L_0801db3c:
	mov r0, r8
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0801db50:
	.4byte gSelectionWork
.L_0801db54:
	.4byte gDebugMode
.L_0801db58:
	.4byte 0x00000c23
.L_0801db5c:
	.4byte 0x00000c25
.L_0801db60:
	.4byte 0x00000c27
.L_0801db64:
	.4byte Resource_FixedBlockBTiles
.L_0801db68:
	.4byte 0x000005a4
.L_0801db6c:
	.4byte Menu_WorkspaceIconFrames
