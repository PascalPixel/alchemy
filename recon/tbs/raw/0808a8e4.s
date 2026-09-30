.syntax unified
	.thumb
	.global Game_ResetForNewGame
	.thumb_func
Game_ResetForNewGame:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0808a980
	ldr r1, .L_0808a984
	ldrb r3, [r3]
	mov r11, r1
	cmp r3, #0
	beq .L_0808a932
	cmp r0, #1
	bne .L_0808a918
	ldr r1, .L_0808a988
	movs r4, #224
	ldr r2, .L_0808a98c
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r2, [r3]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r0, [r3]
	b .L_0808a94c
.L_0808a918:
	cmp r0, #2
	bne .L_0808a932
	ldr r1, .L_0808a988
	movs r4, #224
	ldr r3, .L_0808a990
	lsls r4, r4, #1
	movs r0, #225
	adds r2, r1, r4
	lsls r0, r0, #1
	strh r3, [r2]
	adds r2, r1, r0
	movs r3, #1
	b .L_0808a94a
.L_0808a932:
	bl GameState_InitDefaultsFar
	ldr r1, .L_0808a988
	movs r4, #224
	ldr r3, .L_0808a994
	lsls r4, r4, #1
	movs r0, #225
	adds r2, r1, r4
	lsls r0, r0, #1
	strh r3, [r2]
	adds r2, r1, r0
	movs r3, #2
.L_0808a94a:
	strh r3, [r2]
.L_0808a94c:
	ldr r3, .L_0808a988
	ldr r1, .L_0808a998
	adds r2, r3, r1
	ldrb r0, [r2]
	ldr r2, .L_0808a99c
	adds r3, r3, r2
	ldrb r1, [r3]
	bl PaletteGlow_UpdateFar
	bl Resource_InitializeTable
	bl Scheduler_ResetTaskTable
	bl Scheduler_ResetTaskTable
	ldr r3, .L_0808a9a0
	mov r9, r3
.L_0808a96e:
	ldr r0, .L_0808a9a4
	bl Func_080770c0
	cmp r0, #0
	beq .L_0808a9a8
	ldr r0, .L_0808a9a4
	bl GameFlag_ClearBitFar
	b .L_0808a9b0
.L_0808a980:
	.4byte gDebugMode
.L_0808a984:
	.4byte Field_SceneTable
.L_0808a988:
	.4byte gCell
.L_0808a98c:
	.4byte 0x00000005
.L_0808a990:
	.4byte 0x00000001
.L_0808a994:
	.4byte 0x00000000
.L_0808a998:
	.4byte 0x00000205
.L_0808a99c:
	.4byte 0x00000206
.L_0808a9a0:
	.4byte 0x050001c0
.L_0808a9a4:
	.4byte 0x00000101
.L_0808a9a8:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_080f9010
.L_0808a9b0:
	ldr r7, .L_0808aa10
	movs r4, #224
	lsls r4, r4, #1
	adds r4, r4, r7
	movs r0, #0
	ldrsh r3, [r4, r0]
	movs r1, #225
	lsls r3, r3, #3
	add r3, r11
	lsls r1, r1, #1
	mov r10, r3
	adds r3, r7, r1
	ldr r1, .L_0808aa14
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldrh r3, [r1, #10]
	ldr r2, .L_0808aa08
	ands r3, r2
	strh r3, [r1, #10]
	ldr r2, .L_0808aa0c
	ldrh r3, [r1, #10]
	ands r3, r2
	strh r3, [r1, #10]
	mov r8, r4
	ldrh r3, [r1, #10]
	bl Scheduler_ResetTaskTable
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	bl Runtime_InitializeHeap
	bl Bg0_ClearTilemap
	bl Resource_InitializeTable
	b .L_0808aa18
.L_0808aa08:
	.4byte 0x0000c5ff
.L_0808aa0c:
	.4byte 0x00007fff
.L_0808aa10:
	.4byte gCell
.L_0808aa14:
	.4byte 0x040000b0
.L_0808aa18:
	mov r0, r8
	movs r1, #253
	movs r4, #0
	ldrsh r3, [r0, r4]
	lsls r1, r1, #1
	cmp r3, r1
	ble .L_0808aaa8
	movs r2, #254
	lsls r2, r2, #1
	cmp r3, r2
	beq .L_0808aa6e
	cmp r3, r2
	bgt .L_0808aa3a
	subs r2, #1
	cmp r3, r2
	beq .L_0808aa9e
	b .L_0808aaa0
.L_0808aa3a:
	ldr r4, .L_0808ab2c
	cmp r3, r4
	beq .L_0808aa52
	movs r0, #255
	lsls r0, r0, #1
	cmp r3, r0
	bne .L_0808aaa0
	adds r0, r6, #0
	bl Battle_RunEncounterFar
	adds r6, r0, #0
	b .L_0808aaa0
.L_0808aa52:
	movs r0, #64
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r3, .L_0808ab30
	mov r0, r9
	adds r1, r5, #0
	ldr r2, .L_0808ab34
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Func_080f4000
	b .L_0808aa88
.L_0808aa6e:
	movs r0, #64
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r3, .L_0808ab30
	mov r0, r9
	adds r1, r5, #0
	ldr r2, .L_0808ab34
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Func_080f6000
.L_0808aa88:
	ldr r3, .L_0808ab30
	adds r6, r0, #0
	mov r1, r9
	adds r0, r5, #0
	ldr r2, .L_0808ab34
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Runtime_BumpFree
	b .L_0808aaa0
.L_0808aa9e:
	movs r6, #0
.L_0808aaa0:
	adds r0, r6, #0
	bl Party_SetReturnPoint
	b .L_0808a96e
.L_0808aaa8:
	ldr r5, .L_0808ab38
	adds r0, r5, #0
	bl Func_080770c0
	mov r3, r8
	adds r1, r0, #0
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Scene_ResetFlagsOnEnter
	bl Scene_ResolveInteractionResult
	adds r0, r5, #0
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808aaf0
	movs r0, #141
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808aae6
	ldr r0, .L_0808ab3c
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808aae6
	bl Audio_PlayCueFromEventWork
	b .L_0808ab0a
.L_0808aae6:
	movs r0, #141
	lsls r0, r0, #1
	bl GameFlag_ClearBitFar
	b .L_0808ab0a
.L_0808aaf0:
	ldr r4, .L_0808ab40
	adds r3, r7, r4
	movs r2, #1
	movs r1, #0
	ldrsh r0, [r3, r1]
	negs r2, r2
	cmp r0, r2
	beq .L_0808ab06
	bl Func_080f9010
	b .L_0808ab0a
.L_0808ab06:
	bl Audio_PlayCueFromEventWork
.L_0808ab0a:
	mov r4, r10
	ldr r3, .L_0808ab44
	movs r0, #237
	ldrh r2, [r4, #4]
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r2, [r3]
	movs r0, #0
	bl BattleFx_LoadResourceGroup
	adds r0, r6, #0
	bl Func_0808c4f8
	bl MapGroupTable_SelectEntry
	b .L_0808a96e
	.2byte 0x0000
.L_0808ab2c:
	.4byte 0x000001fd
.L_0808ab30:
	.4byte 0x040000d4
.L_0808ab34:
	.4byte 0x84000010
.L_0808ab38:
	.4byte 0x00000109
.L_0808ab3c:
	.4byte 0x0000011b
.L_0808ab40:
	.4byte 0x0000021e
.L_0808ab44:
	.4byte gCell
