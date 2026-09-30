.syntax unified
	.thumb
	.global Func_0811c710
	.thumb_func
Func_0811c710:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r0, [sp, #12]
	ldr r1, [sp, #12]
	movs r0, #0
	movs r2, #0
	ldrsh r1, [r1, r2]
	mov r9, r0
	adds r0, r1, #0
	str r1, [sp, #8]
	bl Func_0811c650
	cmp r0, #0
	blt .L_0811c74a
	ldr r2, [sp, #12]
	movs r3, #10
	ldrsh r2, [r2, r3]
	adds r0, r2, #0
	str r2, [sp, #4]
	bl Func_0811c650
	cmp r0, #0
	bge .L_0811c750
.L_0811c74a:
	movs r0, #1
	negs r0, r0
	b .L_0811c976
.L_0811c750:
	ldr r1, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r2, [r3]
	movs r0, #0
	ldrsh r3, [r1, r0]
	movs r1, #160
	lsls r1, r1, #7
	cmp r3, #4
	bgt .L_0811c76a
	movs r1, #128
	lsls r1, r1, #6
.L_0811c76a:
	movs r3, #60
	str r1, [r2]
	str r3, [r2, #4]
	movs r0, #10
	bl WaitFrames
	bl Random16
	ldr r0, [sp, #8]
	bl GetBattleObjectSlot
	ldr r2, [sp, #4]
	ldr r6, [r0]
	cmp r2, #7
	bhi .L_0811c79c
	add r3, sp, #104
	mov r10, r3
	movs r0, #2
	mov r1, r10
	bl BattleParty_ListLivingUnits
	mov r11, r0
	movs r0, #128
	str r0, [sp, #0]
	b .L_0811c7ac
.L_0811c79c:
	add r1, sp, #104
	movs r0, #1
	mov r10, r1
	bl BattleParty_ListLivingUnits
	movs r2, #0
	str r2, [sp, #0]
	mov r11, r0
.L_0811c7ac:
	mov r3, r11
	movs r5, #0
	cmp r3, #0
	beq .L_0811c7cc
.L_0811c7b4:
	ldr r0, [sp, #0]
	ldr r1, [sp, #8]
	adds r3, r5, r0
	cmp r3, r1
	bne .L_0811c7c6
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetMode
.L_0811c7c6:
	adds r5, #1
	cmp r5, r11
	bne .L_0811c7b4
.L_0811c7cc:
	movs r0, #30
	bl WaitFrames
	movs r2, #128
	ldr r3, .L_0811c804
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	mov r2, r11
	movs r5, #0
	cmp r2, #0
	beq .L_0811c7f4
.L_0811c7e4:
	ldr r3, [sp, #0]
	movs r1, #1
	adds r0, r5, r3
	adds r5, #1
	bl BattlePres_SetActorRecordMode
	cmp r5, r11
	bne .L_0811c7e4
.L_0811c7f4:
	movs r0, #128
	lsls r0, r0, #19
	ldr r7, .L_0811c808
	ldr r6, .L_0811c80c
	adds r0, #82
	movs r5, #0
	mov r8, r0
	b .L_0811c810
.L_0811c804:
	.4byte 0x00003f40
.L_0811c808:
	.4byte 0x00000010
.L_0811c80c:
	.4byte 0x00001000
.L_0811c810:
	subs r3, r7, r5
	orrs r3, r6
	mov r1, r8
	strh r3, [r1]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #16
	bne .L_0811c810
	movs r0, #9
	bl UiWindow_DrawPartyStatusContentsFar
	ldr r2, [sp, #4]
	cmp r2, #127
	ble .L_0811c86a
	movs r0, #2
	mov r1, r10
	bl BattleParty_ListLivingUnits
	mov r8, r0
	movs r5, #0
	cmp r9, r8
	beq .L_0811c89e
	mov r0, r9
	lsls r3, r0, #1
	mov r1, r10
	adds r7, r3, r1
.L_0811c848:
	adds r6, r5, #0
	adds r6, #128
	adds r0, r6, #0
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	ble .L_0811c862
	movs r3, #1
	strh r6, [r7]
	add r9, r3
	adds r7, #2
.L_0811c862:
	adds r5, #1
	cmp r9, r8
	bne .L_0811c848
	b .L_0811c89e
.L_0811c86a:
	movs r0, #1
	mov r1, r10
	bl BattleParty_ListLivingUnits
	adds r7, r0, #0
	movs r5, #0
	cmp r9, r7
	beq .L_0811c89e
	mov r0, r9
	lsls r3, r0, #1
	mov r1, r10
	adds r6, r3, r1
.L_0811c882:
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	ble .L_0811c898
	movs r3, #1
	strh r5, [r6]
	add r9, r3
	adds r6, #2
.L_0811c898:
	adds r5, #1
	cmp r9, r7
	bne .L_0811c882
.L_0811c89e:
	ldr r2, .L_0811c8d0
	mov r0, r9
	lsls r3, r0, #1
	mov r1, r10
	strh r2, [r1, r3]
	mov r0, r10
	movs r1, #0
	bl BattleActor_SpawnObjectsForList
	ldr r1, [sp, #12]
	add r0, sp, #16
	movs r2, #8
	ldrsh r3, [r1, r2]
	movs r5, #0
	str r3, [r0]
	ldr r2, [sp, #8]
	mov r3, r9
	str r2, [r0, #8]
	cmp r3, #0
	beq .L_0811c8e2
	mov r1, r10
	add r2, sp, #52
	movs r4, #0
	b .L_0811c8d4
	.2byte 0x0000
.L_0811c8d0:
	.4byte 0x000000ff
.L_0811c8d4:
	ldrh r3, [r4, r1]
	adds r5, #1
	strh r3, [r2]
	adds r4, #2
	adds r2, #2
	cmp r5, r9
	bne .L_0811c8d4
.L_0811c8e2:
	mov r1, r9
	str r1, [r0, #20]
	ldr r2, [sp, #4]
	cmp r2, #7
	bhi .L_0811c8f0
	movs r3, #1
	b .L_0811c8f2
.L_0811c8f0:
	movs r3, #0
.L_0811c8f2:
	str r3, [r0, #4]
	bl Resource_FarCall00C + 0x10
	movs r0, #10
	bl WaitFrames
	bl BattleActor_CommitPlacement
	ldr r3, .L_0811c930
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	mov r3, r11
	movs r5, #0
	cmp r3, #0
	beq .L_0811c924
.L_0811c914:
	ldr r1, [sp, #0]
	adds r0, r5, r1
	movs r1, #1
	adds r5, #1
	bl BattlePres_SetActorRecordMode
	cmp r5, r11
	bne .L_0811c914
.L_0811c924:
	movs r7, #128
	ldr r6, .L_0811c934
	lsls r7, r7, #19
	movs r5, #0
	adds r7, #82
	b .L_0811c938
.L_0811c930:
	.4byte 0x00003f40
.L_0811c934:
	.4byte 0x00001000
.L_0811c938:
	adds r3, r5, #0
	orrs r3, r6
	strh r3, [r7]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #16
	bne .L_0811c938
	mov r2, r11
	movs r5, #0
	cmp r2, #0
	beq .L_0811c962
.L_0811c952:
	ldr r3, [sp, #0]
	movs r1, #0
	adds r0, r5, r3
	adds r5, #1
	bl BattlePres_SetActorRecordMode
	cmp r5, r11
	bne .L_0811c952
.L_0811c962:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl BattlePres_SetupTransitionScene
	movs r0, #3
	bl WaitFrames
	movs r0, #0
.L_0811c976:
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
