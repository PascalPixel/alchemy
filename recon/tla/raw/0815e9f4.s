.syntax unified
	.thumb
	.global Func_0815e9f4
	.thumb_func
Func_0815e9f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	sub sp, #116
	str r1, [sp, #56]
	mov r11, r0
	ldr r2, [r3, #96]
	str r2, [sp, #52]
	ldr r5, [r3, #100]
	str r5, [sp, #44]
	movs r5, #1
	ldr r3, [r3, #48]
	str r3, [sp, #40]
	ldr r3, [r0]
	cmp r3, #199
	bgt .L_0815ea24
	movs r5, #0
.L_0815ea24:
	mov r1, r11
	movs r2, #130
	ldr r0, [r1, #8]
	ldr r1, [r1, #12]
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	bl Func_08143d80
	ldr r3, .L_0815ea60
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	mov r2, r11
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0815ea64
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #19
	bl Func_081963ec
	b .L_0815ea74
	.2byte 0x0000
.L_0815ea60:
	.4byte 0x00001f80
.L_0815ea64:
	movs r1, #7
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #23
	bl Func_081963ec
.L_0815ea74:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #104]
	mov r1, sp
	str r2, [sp, #60]
	adds r1, #60
	adds r3, #188
	ldr r3, [r3]
	str r1, [sp, #16]
	mov r2, r11
	str r3, [r1, #4]
	ldr r0, [r2, #8]
	ldr r1, [r2, #12]
	movs r2, #130
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #56]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r0, .L_0815ed64
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	mov r3, r11
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	movs r2, #130
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0815ed68
	ldr r1, .L_0815ed6c
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	mov r1, r11
	ldr r0, [r1, #8]
	cmp r0, #7
	ble .L_0815eaf0
	ldr r0, .L_0815ed70
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r11
	ldr r0, [r2, #8]
.L_0815eaf0:
	mov r3, r11
	ldr r1, [r3, #12]
	movs r2, #130
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	movs r3, #0
	ldr r1, [sp, #44]
	ldr r0, .L_0815ed74
	movs r2, #0
	bl Resource_LoadAndDecompress
	mov r1, r11
	ldr r0, [r1, #8]
	movs r2, #130
	ldr r1, [r1, #12]
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #56]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r3, r1
	movs r3, #1
	str r3, [r2]
	ldr r3, [sp, #56]
	adds r1, #4
	adds r2, r3, r1
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0815ed78
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	cmp r5, #1
	beq .L_0815eb44
	b .L_0815ed04
.L_0815eb44:
	mov r2, r11
	ldr r0, [r2, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r6, #255
	ldr r5, [sp, #56]
	movs r3, #0
	lsls r6, r6, #8
	mov r8, r0
	mov r10, r3
	movs r7, #0
	adds r6, #255
.L_0815eb5e:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #16
	str r3, [r5]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	add r10, r1
	ands r0, r6
	mov r2, r10
	str r0, [r5, #20]
	adds r5, #28
	cmp r2, #64
	bne .L_0815eb5e
	mov r0, r8
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r3, r8
	ldr r3, [r3, #36]
	mov r5, r8
	str r3, [sp, #36]
	mov r1, r8
	ldr r5, [r5, #40]
	mov r2, r8
	str r5, [sp, #32]
	mov r3, r8
	ldr r1, [r1, #44]
	movs r5, #0
	str r1, [sp, #28]
	mov r1, r8
	ldr r2, [r2, #72]
	mov r9, r5
	str r2, [sp, #20]
	mov r2, r11
	ldr r3, [r3, #52]
	str r3, [sp, #24]
	str r5, [r1, #36]
	str r5, [r1, #40]
	str r5, [r1, #44]
	str r5, [r1, #52]
	str r5, [r1, #72]
	mov r3, sp
	adds r3, #104
	ldr r0, [r2, #8]
	adds r1, r3, #0
	str r3, [sp, #12]
	bl Func_0815e20c
	ldr r1, [sp, #12]
	mov r2, r10
	ldr r3, [r1]
	ldr r0, .L_0815ed7c
	subs r2, r2, r3
	str r2, [sp, #48]
	add r3, sp, #48
	ldr r2, .L_0815ed80
	ldrh r3, [r3]
	strh r3, [r2, #4]
	movs r3, #80
	strh r3, [r2, #6]
	ldr r1, [sp, #56]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #180
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #184
	adds r3, r1, r2
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #212
	bl Audio_PlayCue
.L_0815ec16:
	mov r3, r11
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	movs r2, #130
	bl BattlePres_SetupTransitionAtPairMidpointFar
	ldr r6, [sp, #56]
	movs r5, #0
	mov r10, r5
.L_0815ec28:
	ldr r3, [r6]
	cmp r3, #0
	blt .L_0815ecb4
	mov r3, r10
	cmp r3, #0
	bge .L_0815ec36
	adds r3, #3
.L_0815ec36:
	asrs r3, r3, #2
	cmp r9, r3
	blt .L_0815ecb4
	mov r1, r10
	movs r3, #1
	ands r3, r1
	adds r7, r3, #5
	bl Func_08014de4
	ldr r0, [r6, #20]
	bl Func_080150e4
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #16]
	bl Func_08015068
	add r5, sp, #80
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	adds r3, #64
	str r3, [r5]
	ldr r2, [sp, #108]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	adds r3, #24
	str r3, [r5, #4]
	ldr r3, [r5, #8]
	movs r2, #60
	negs r2, r2
	cmp r3, r2
	bge .L_0815ec82
	str r2, [r5, #8]
	adds r3, r2, #0
.L_0815ec82:
	cmp r3, #60
	ble .L_0815ec8a
	movs r3, #60
	str r3, [r5, #8]
.L_0815ec8a:
	ldr r2, .L_0815ed84
	lsls r0, r7, #1
	adds r3, #60
	str r3, [r5, #8]
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #44]
	ldr r3, [r5, #4]
	adds r1, r2, r1
	ldr r2, [r5]
	subs r3, r3, r7
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, r2, r7
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6]
	subs r3, #4
	str r3, [r6]
.L_0815ecb4:
	movs r3, #1
	add r10, r3
	mov r5, r10
	adds r6, #28
	cmp r5, #64
	bne .L_0815ec28
	ldr r1, [sp, #56]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r5, #1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	add r9, r5
	bl WaitFrames
	mov r1, r9
	cmp r1, #32
	bne .L_0815ec16
	ldr r0, .L_0815ed7c
	bl Scheduler_RemoveCallback
	movs r1, #16
	mov r0, r8
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r2, [sp, #36]
	mov r3, r8
	str r2, [r3, #36]
	ldr r5, [sp, #32]
	str r5, [r3, #40]
	ldr r1, [sp, #28]
	str r1, [r3, #44]
	ldr r2, [sp, #24]
	str r2, [r3, #52]
	ldr r5, [sp, #20]
	str r5, [r3, #72]
	b .L_0815ed0a
.L_0815ed04:
	mov r1, sp
	adds r1, #104
	str r1, [sp, #12]
.L_0815ed0a:
	movs r1, #240
	ldr r5, .L_0815ed88
	ldr r0, [sp, #52]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	lsls r1, r1, #6
	ldr r0, .L_0815ed8c
	mov lr, r5
	.2byte 0xf800
	ldr r3, [sp, #56]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #56]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #75
	str r3, [r2]
	ldr r3, .L_0815ed60
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	mov r1, r11
	movs r5, #36
	ldrsh r0, [r1, r5]
	add r5, sp, #92
	adds r1, r5, #0
	bl Func_0815e20c
	mov r2, r11
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0815ed90
	ldr r2, [r5]
	movs r3, #32
	b .L_0815ed94
.L_0815ed60:
	.4byte 0x00001f81
.L_0815ed64:
	.4byte 0x0000010c
.L_0815ed68:
	.4byte 0x0000010b
.L_0815ed6c:
	.4byte gMapCellBuffer
.L_0815ed70:
	.4byte 0x00000151
.L_0815ed74:
	.4byte 0x00000137
.L_0815ed78:
	.4byte Func_08143000
.L_0815ed7c:
	.4byte Func_08143488
.L_0815ed80:
	.4byte Data_03001120
.L_0815ed84:
	.4byte Data_08197424
.L_0815ed88:
	.4byte IwramClearWords
.L_0815ed8c:
	.4byte 0x06004000
.L_0815ed90:
	ldr r2, [r5]
	movs r3, #96
.L_0815ed94:
	subs r3, r3, r2
	str r3, [sp, #48]
	ldr r3, [sp, #48]
	cmp r3, #0
	ble .L_0815eda2
	movs r1, #0
	str r1, [sp, #48]
.L_0815eda2:
	ldr r2, [sp, #48]
	movs r3, #128
	negs r3, r3
	cmp r2, r3
	bge .L_0815edae
	str r3, [sp, #48]
.L_0815edae:
	ldr r1, [sp, #48]
	ldr r3, [r5]
	ldr r2, .L_0815efe8
	adds r3, r3, r1
	str r3, [r5]
	add r3, sp, #48
	ldrh r3, [r3]
	mov r1, r11
	strh r3, [r2, #4]
	movs r3, #80
	strh r3, [r2, #6]
	movs r5, #36
	ldrsh r0, [r1, r5]
	bl GetBattleObjectSlotFar
	mov r3, r11
	ldr r6, [r0]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Battle_GetObjectTableValueFar
	movs r5, #0
	lsrs r3, r0, #31
	adds r0, r0, r3
	mov r10, r5
	ldr r5, [sp, #56]
	asrs r0, r0, #1
	mov r8, r0
	movs r7, #255
.L_0815ede8:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	add r3, r8
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r7
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ldr r3, [r5]
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #20]
	cmp r3, #0
	ble .L_0815ee22
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_0815ee22:
	movs r1, #1
	mov r3, r10
	add r10, r1
	adds r3, #16
	mov r2, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	bne .L_0815ede8
	ldr r5, [sp, #40]
	movs r3, #0
	adds r5, #12
	str r5, [sp, #8]
	mov r9, r3
.L_0815ee3e:
	mov r1, r9
	cmp r1, #5
	bne .L_0815ee4a
	movs r0, #134
	bl Func_081180e8
.L_0815ee4a:
	mov r2, r9
	cmp r2, #4
	bne .L_0815ee5c
	mov r5, r11
	movs r3, #36
	ldrsh r0, [r5, r3]
	movs r1, #0
	bl Func_08118088
.L_0815ee5c:
	mov r1, r11
	ldr r0, [r1, #8]
	ldr r1, [sp, #12]
	bl Func_0815e20c
	ldr r2, [sp, #12]
	ldr r3, [r2, #4]
	adds r3, #16
	str r3, [r2, #4]
	mov r3, r9
	cmp r3, #1
	bgt .L_0815ee8a
	ldr r5, [sp, #56]
	movs r2, #224
	movs r3, #120
	lsls r2, r2, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r1, r5, r2
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	movs r2, #0
	b .L_0815eeb6
.L_0815ee8a:
	mov r3, r9
	cmp r3, #3
	bgt .L_0815eea0
	movs r3, #128
	movs r2, #4
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	ldr r1, .L_0815efec
	b .L_0815eeb4
.L_0815eea0:
	mov r5, r9
	cmp r5, #5
	bgt .L_0815eebe
	movs r3, #128
	movs r2, #4
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	ldr r1, .L_0815eff0
.L_0815eeb4:
	negs r2, r2
.L_0815eeb6:
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
	b .L_0815eede
.L_0815eebe:
	mov r1, r9
	cmp r1, #7
	bgt .L_0815eede
	movs r3, #120
	ldr r2, [sp, #56]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #253
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	movs r2, #0
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
.L_0815eede:
	bl Func_08014de4
	ldr r0, [sp, #40]
	ldr r1, [sp, #8]
	bl Graphics_PrepareTransferInIwramWork
	mov r3, r9
	subs r3, #4
	cmp r3, #27
	bhi .L_0815ef70
	movs r1, #68
	movs r5, #0
	add r1, sp
	mov r10, r5
	mov r8, r1
.L_0815eefc:
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r5, r3, #1
	lsls r3, r5, #3
	ldr r1, [sp, #56]
	subs r3, r3, r5
	lsls r3, r3, #2
	adds r7, r1, r3
	ldr r6, [r7, #24]
	cmp r6, #0
	ble .L_0815ef66
	mov r1, r8
	adds r0, r7, #0
	bl Func_0815e1ec
	mov r3, r8
	ldr r2, [r3]
	ldr r1, [sp, #48]
	asrs r6, r6, #3
	adds r2, r2, r1
	str r2, [r3]
	ldr r3, [r3, #4]
	movs r0, #1
	adds r6, #2
	ands r0, r5
	ldr r5, .L_0815eff4
	lsls r4, r6, #1
	adds r3, #16
	mov r1, r8
	str r3, [r1, #4]
	subs r1, r4, #2
	ldrh r1, [r5, r1]
	ldr r5, [sp, #44]
	str r4, [sp, #0]
	adds r1, r5, r1
	str r4, [sp, #4]
	ldr r5, [sp, #16]
	lsls r0, r0, #2
	subs r3, r3, r6
	ldr r4, [r0, r5]
	subs r2, r2, r6
	ldr r0, [sp, #52]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #60
	ldr r2, .L_0815eff8
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_0815ef66:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #64
	bne .L_0815eefc
.L_0815ef70:
	ldr r3, [sp, #56]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #32
	beq .L_0815ef90
	b .L_0815ee3e
.L_0815ef90:
	ldr r0, .L_0815effc
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_0815efe8
	mov r5, r9
	strh r5, [r3, #6]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r2, #206
	movs r1, #0
	lsls r2, r2, #3
	mov r9, r1
	adds r5, r3, r2
	movs r6, #6
.L_0815efba:
	mov r3, r9
	subs r1, r6, r3
	ldrh r0, [r5]
	bl Func_08118048
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #7
	bne .L_0815efba
	bl Func_08143d04
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0815efe8:
	.4byte Data_03001120
.L_0815efec:
	.4byte gMapCellBuffer
.L_0815eff0:
	.4byte Data_02014000
.L_0815eff4:
	.4byte Data_08197424
.L_0815eff8:
	.4byte 0xfffffc00
.L_0815effc:
	.4byte Func_08143000
