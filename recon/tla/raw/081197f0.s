.syntax unified
	.thumb
	.global Func_081197f0
	.thumb_func
Func_081197f0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	str r0, [sp, #40]
	movs r1, #76
	movs r0, #48
	bl Runtime_AllocateBlock
	mov r10, r0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #124
	mov r8, r0
	mov r1, r8
	movs r0, #36
	bl Runtime_AllocateBlock
	movs r5, #249
	lsls r5, r5, #3
	adds r1, r5, #0
	mov r9, r0
	movs r0, #216
	bl Runtime_AllocateBlock
	movs r1, #32
	movs r0, #176
	bl Runtime_AllocateBlock
	movs r6, #192
	movs r1, #160
	str r0, [sp, #36]
	lsls r1, r1, #2
	lsls r6, r6, #18
	movs r0, #44
	bl Runtime_AllocateBlock
	adds r3, r6, #0
	adds r3, #216
	adds r1, r5, #0
	ldr r0, [r3]
	ldr r3, .L_08119ab0
	mov lr, r3
	.2byte 0xf800
	bl Scheduler_ResetTaskTable
	ldr r1, [sp, #36]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r1]
	movs r3, #128
	movs r2, #1
	lsls r3, r3, #19
	movs r7, #0
	movs r0, #4
	str r7, [r1, #4]
	str r2, [r1, #20]
	str r7, [r1, #24]
	str r7, [r1, #28]
	strh r2, [r3]
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #106
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_08014de4
	ldr r5, .L_08119ab4
	movs r1, #76
	mov r0, r10
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	mov r0, r9
	mov r1, r8
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	movs r3, #1
	mov r2, r9
	negs r3, r3
	str r3, [r2, #84]
	ldr r3, [sp, #40]
	movs r1, #12
	str r3, [r2]
	movs r0, #148
	bl Runtime_AllocateBlock
	adds r6, #148
	ldr r0, [r6]
	movs r1, #12
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	bl Func_080c8520
	movs r3, #206
	lsls r3, r3, #3
	add r3, r9
	movs r1, #224
	strh r0, [r3]
	lsls r1, r1, #4
	movs r0, #16
	bl Runtime_AllocateBlock
	movs r1, #192
	lsls r1, r1, #3
	movs r0, #12
	bl Runtime_AllocateBlock
	movs r0, #4
	bl Func_08020088
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_081198f2
	movs r0, #1
	bl UiWork_InitializeFar
	b .L_081198f8
.L_081198f2:
	movs r0, #0
	bl UiWork_InitializeFar
.L_081198f8:
	mov r5, r9
	bl Func_081196fc
	ldr r0, [r5]
	bl Func_08127cd4
	adds r6, r0, #0
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0811992a
	adds r5, #68
	movs r3, #1
	str r5, [sp, #16]
	strb r3, [r5]
	ldr r3, .L_08119ab8
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r3, r0
	movs r2, #4
	strb r2, [r3]
	b .L_08119930
.L_0811992a:
	mov r1, r9
	adds r1, #68
	str r1, [sp, #16]
.L_08119930:
	ldr r2, [sp, #16]
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081199ae
	ldr r2, .L_08119abc
	movs r3, #0
	str r3, [r2]
	movs r5, #0
	ldr r3, .L_08119ac0
	mov r6, r9
	movs r0, #1
	mov r8, r3
	adds r6, #82
	movs r7, #3
	mov r10, r0
.L_0811994e:
	mov r1, r8
	ldrh r2, [r1]
	adds r3, r7, #0
	ands r3, r2
	cmp r3, #3
	beq .L_0811996a
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #24
	ble .L_0811994e
	mov r2, r10
	strb r2, [r6]
.L_0811996a:
	ldr r3, .L_08119ac4
	mov r2, r9
	ldr r3, [r3]
	adds r2, #80
	lsls r3, r3, #26
	lsrs r3, r3, #30
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #216
	movs r4, #217
	ldr r2, [r3]
	ldr r1, .L_08119ac8
	lsls r4, r4, #3
	movs r0, #0
	adds r4, #255
.L_0811998a:
	ldrb r3, [r1]
	adds r0, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r0, r4
	bls .L_0811998a
	movs r0, #252
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r6, r0, #0
	bl Func_081195a0
	mov r2, r9
	adds r2, #66
	movs r3, #0
	strb r3, [r2]
.L_081199ae:
	movs r1, #224
	lsls r1, r1, #2
	adds r1, #255
	ldr r0, .L_08119acc
	bl Scheduler_AddOrUpdateCallback
	movs r5, #128
	ldr r3, .L_08119ab8
	lsls r5, r5, #2
	adds r5, #14
	adds r3, r3, r5
	movs r1, #0
	ldrsh r0, [r3, r1]
	cmp r0, #0
	beq .L_081199ea
	bl Audio_PlayCue
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_081199f0
	movs r0, #55
	bl Audio_PlayCue
	movs r0, #4
	bl Sound_LoadPresetParameters
	b .L_081199f0
.L_081199ea:
	movs r0, #50
	bl Audio_PlayCue
.L_081199f0:
	bl Func_08118c68
	bl Func_0811b37c
	bl Func_08118eb0
	bl Func_08118f6c
	movs r0, #0
	bl Resource_FarCall005
	ldrh r3, [r0]
	cmp r3, #0
	beq .L_08119a18
	mov r2, r9
	adds r2, #65
	movs r3, #3
	str r2, [sp, #20]
	strb r3, [r2]
	b .L_08119a24
.L_08119a18:
	mov r3, r9
	adds r3, #65
	str r3, [sp, #20]
	ldr r5, [sp, #20]
	movs r3, #1
	strb r3, [r5]
.L_08119a24:
	ldr r3, [sp, #40]
	subs r3, #75
	cmp r3, #1
	bhi .L_08119a30
	bl Func_081185c4
.L_08119a30:
	movs r0, #9
	bl Func_08038128
	bl Func_0811bddc
	bl BattleActor_CommitPlacement
	bl Func_081263c4
	movs r3, #206
	lsls r3, r3, #3
	add r3, r9
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #0
	bl Func_081263fc
	movs r3, #128
	lsls r3, r3, #10
	movs r0, #160
	movs r1, #160
	str r3, [sp, #0]
	lsls r0, r0, #16
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #0
	bl BattleCamera_SetRange
	movs r1, #0
	movs r2, #0
	movs r3, #190
	movs r0, #0
	bl BattlePres_SetupTransitionScene
	movs r0, #1
	bl Func_08118d6c
	ldr r5, .L_08119aac
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r5, [r3]
	bl Func_081281ec
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	mov r1, r9
	str r0, [r1, #84]
	adds r1, #69
	movs r0, #183
	str r1, [sp, #24]
	lsls r0, r0, #1
	strb r5, [r1]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08119ad0
	ldr r2, [sp, #24]
	movs r3, #1
	b .L_08119b10
	.2byte 0x0000
.L_08119aac:
	.4byte 0x00000000
.L_08119ab0:
	.4byte IwramClearWords
.L_08119ab4:
	.4byte IwramFillWords
.L_08119ab8:
	.4byte gPartyState
.L_08119abc:
	.4byte Data_020054c8
.L_08119ac0:
	.4byte gLinkStatus
.L_08119ac4:
	.4byte 0x04000128
.L_08119ac8:
	.4byte Data_02018000
.L_08119acc:
	.4byte Func_08118adc
.L_08119ad0:
	ldr r3, .L_08119cc8
	movs r5, #166
	lsls r5, r5, #1
	adds r5, #255
	adds r3, r3, r5
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08119b08
	bl BattleRandom16Far
	movs r3, #15
	ands r0, r3
	cmp r0, #0
	bne .L_08119af4
	ldr r0, [sp, #24]
	movs r3, #1
	strb r3, [r0]
	b .L_08119b12
.L_08119af4:
	bl BattleRandom16Far
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_08119b12
	ldr r1, [sp, #24]
	movs r3, #2
	strb r3, [r1]
	b .L_08119b12
.L_08119b08:
	cmp r3, #6
	bne .L_08119b12
	ldr r2, [sp, #24]
	movs r3, #2
.L_08119b10:
	strb r3, [r2]
.L_08119b12:
	ldr r1, [sp, #40]
	adds r0, r6, #0
	bl Func_08125d74
	ldr r3, [sp, #36]
	movs r2, #0
	str r2, [r3, #20]
	ldr r3, .L_08119ccc
	movs r1, #144
	strb r2, [r3]
	ldr r0, .L_08119cd0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r5, #1
	str r5, [sp, #32]
.L_08119b32:
	movs r0, #1
	movs r1, #0
	bl Func_0811a39c
	cmp r0, #0
	bne .L_08119b42
	bl Func_081195ec
.L_08119b42:
	bl Func_0811d7e4
	bl Func_08118f6c
	movs r0, #0
	bl Resource_FarCall005
	ldrh r3, [r0]
	cmp r3, #0
	beq .L_08119b5e
	ldr r0, [sp, #20]
	movs r3, #3
	strb r3, [r0]
	b .L_08119b64
.L_08119b5e:
	ldr r1, [sp, #20]
	movs r3, #1
	strb r3, [r1]
.L_08119b64:
	ldr r2, [sp, #36]
	movs r3, #160
	lsls r3, r3, #6
	str r3, [r2]
	movs r3, #60
	str r3, [r2, #4]
	ldr r3, [sp, #20]
	movs r5, #187
	ldrb r0, [r3]
	lsls r5, r5, #2
	bl UiWindow_DrawPartyStatusContentsFar
	add r5, r9
	movs r1, #160
	ldr r3, .L_08119cd4
	lsls r1, r1, #1
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r0, [r1, #84]
	bl Resource_ResetEntry
	movs r0, #181
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08119bb4
	adds r0, r5, #0
	bl Func_0811d61c
	b .L_08119bba
.L_08119ba6:
	ldr r0, [sp, #40]
	adds r2, r0, #0
	adds r2, #1
	str r2, [sp, #40]
	bl Func_081197a8
	b .L_08119dde
.L_08119bb4:
	adds r0, r5, #0
	bl Func_0811c3bc
.L_08119bba:
	str r0, [sp, #28]
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	mov r3, r9
	str r0, [r3, #84]
	ldr r5, [sp, #20]
	ldrb r0, [r5]
	bl UiWindow_DrawPartyStatusContentsFar
	ldr r0, [sp, #28]
	cmp r0, #0
	bge .L_08119bd6
	b .L_08119f48
.L_08119bd6:
	movs r1, #0
	mov r8, r1
	cmp r8, r0
	blt .L_08119be0
	b .L_08119dde
.L_08119be0:
	mov r2, r9
	adds r2, #71
	movs r3, #189
	movs r4, #187
	movs r5, #187
	str r2, [sp, #12]
	str r1, [sp, #8]
	lsls r3, r3, #2
	lsls r4, r4, #2
	lsls r5, r5, #2
	mov r10, r3
	add r4, r9
	mov r11, r5
.L_08119bfa:
	movs r3, #128
	mov r0, r11
	mov r2, r9
	lsls r3, r3, #4
	ldrsh r5, [r0, r2]
	adds r3, #88
	movs r2, #1
	add r3, r9
	negs r2, r2
	str r2, [r3]
	ldr r0, [sp, #12]
	movs r3, #0
	strb r3, [r0]
	movs r0, #181
	lsls r0, r0, #1
	str r4, [sp, #4]
	bl GameFlag_Test
	ldr r4, [sp, #4]
	cmp r0, #0
	bne .L_08119c46
	ldr r3, [sp, #8]
	movs r1, #187
	lsls r1, r1, #2
	add r3, r9
	mov r2, r8
	adds r0, r3, r1
	movs r1, #10
	cmp r2, #0
	beq .L_08119c38
	movs r1, #0
.L_08119c38:
	str r4, [sp, #4]
	bl Func_0811d9cc
	ldr r4, [sp, #4]
	cmp r0, #1
	bne .L_08119c5e
	b .L_08119fd0
.L_08119c46:
	ldr r0, [sp, #8]
	movs r3, #187
	lsls r3, r3, #2
	add r0, r9
	adds r0, r0, r3
	str r4, [sp, #4]
	bl Func_0811c594
	ldr r4, [sp, #4]
	cmp r0, #1
	bne .L_08119c5e
	b .L_08119fd0
.L_08119c5e:
	movs r1, #128
	ldr r0, [sp, #40]
	lsls r1, r1, #2
	adds r1, #126
	cmp r0, r1
	bne .L_08119c6c
	b .L_08119f78
.L_08119c6c:
	movs r0, #1
	movs r1, #0
	str r4, [sp, #4]
	bl Func_0811a24c
	cmp r0, #0
	bne .L_08119c7c
	b .L_08119f78
.L_08119c7c:
	movs r0, #1
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	bne .L_08119c8a
	b .L_08119dde
.L_08119c8a:
	movs r0, #2
	movs r1, #0
	bl BattleParty_ListLivingUnits
	ldr r4, [sp, #4]
	cmp r0, #0
	bne .L_08119cd8
	ldr r3, [sp, #40]
	subs r3, #78
	cmp r3, #1
	bls .L_08119ba6
	ldr r2, [sp, #40]
	cmp r2, #80
	bne .L_08119cac
	movs r0, #80
	bl Func_081197a8
.L_08119cac:
	cmp r5, #7
	bls .L_08119cb2
	b .L_08119eb8
.L_08119cb2:
	movs r3, #171
	lsls r3, r3, #3
	add r3, r9
	ldr r3, [r3]
	cmp r3, #1
	beq .L_08119cc0
	b .L_08119eb8
.L_08119cc0:
	mov r5, r9
	movs r3, #3
	strh r3, [r5, #62]
	b .L_08119eb8
.L_08119cc8:
	.4byte gPartyState
.L_08119ccc:
	.4byte Data_0300123c
.L_08119cd0:
	.4byte Func_0811b9fc
.L_08119cd4:
	.4byte IwramClearWords
.L_08119cd8:
	movs r5, #128
	lsls r5, r5, #4
	adds r5, #88
	add r5, r9
	ldr r3, [r5]
	movs r7, #1
	negs r7, r7
	cmp r3, r7
	beq .L_08119d40
	movs r6, #128
	lsls r6, r6, #4
	adds r6, #92
	add r6, r9
	ldr r0, [r6]
	str r4, [sp, #4]
	bl Func_0811d748
	ldr r4, [sp, #4]
	cmp r0, r7
	beq .L_08119d40
	ldr r3, [r5]
	mov r0, r11
	mov r1, r9
	strh r3, [r0, r1]
	movs r3, #9
	strh r3, [r4, #6]
	ldr r3, .L_08119d3c
	mov r2, r10
	mov r5, r9
	strh r3, [r2, r5]
	ldr r0, [r6]
	bl Func_0811d748
	ldr r4, [sp, #4]
	movs r3, #1
	strh r0, [r4, #10]
	strh r3, [r4, #12]
	ldr r1, [sp, #8]
	movs r0, #16
	subs r1, #16
	movs r2, #1
	str r1, [sp, #8]
	negs r0, r0
	negs r2, r2
	add r10, r0
	subs r4, #16
	add r11, r0
	add r8, r2
	b .L_08119d40
	.2byte 0x0000
.L_08119d3c:
	.4byte 0x00000000
.L_08119d40:
	str r4, [sp, #4]
	bl Func_08119374
	ldr r4, [sp, #4]
	cmp r0, #0
	bge .L_08119d4e
	b .L_08119f48
.L_08119d4e:
	ldr r3, [sp, #28]
	subs r3, #1
	cmp r8, r3
	bne .L_08119dbc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #100
	add r0, r9
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_08119dbc
	movs r2, #134
	lsls r2, r2, #4
	add r2, r9
	ldrh r1, [r2]
	mov r5, r10
	lsrs r3, r1, #1
	strh r3, [r2]
	movs r3, #1
	ands r1, r3
	ldrb r3, [r0]
	mov r2, r9
	adds r3, #255
	strb r3, [r0]
	ldr r0, .L_08119db4
	lsls r3, r1, #7
	strh r3, [r4]
	movs r3, #10
	strh r3, [r4, #6]
	strh r0, [r5, r2]
	mov r3, r10
	ldr r2, .L_08119db8
	cmp r1, #0
	beq .L_08119d94
	ldr r2, .L_08119db4
.L_08119d94:
	add r3, r9
	strh r2, [r3, #2]
	movs r3, #255
	strh r3, [r4, #12]
	ldr r5, [sp, #8]
	movs r3, #16
	subs r5, #16
	movs r0, #1
	str r5, [sp, #8]
	negs r3, r3
	negs r0, r0
	add r10, r3
	subs r4, #16
	add r11, r3
	add r8, r0
	b .L_08119dbc
.L_08119db4:
	.4byte 0x00000000
.L_08119db8:
	.4byte 0x00000080
.L_08119dbc:
	ldr r1, [sp, #12]
	ldrb r3, [r1]
	cmp r3, #0
	bne .L_08119dde
	ldr r3, [sp, #8]
	ldr r0, [sp, #28]
	movs r5, #1
	movs r2, #16
	adds r3, #16
	add r8, r5
	add r10, r2
	adds r4, #16
	str r3, [sp, #8]
	add r11, r2
	cmp r8, r0
	bge .L_08119dde
	b .L_08119bfa
.L_08119dde:
	bl Func_08118e64
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #98
	add r1, r9
	ldrh r0, [r1]
	movs r2, #134
	lsls r2, r2, #4
	add r2, r9
	movs r3, #0
	strh r0, [r2]
	strh r3, [r1]
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #101
	add r1, r9
	movs r3, #128
	ldrb r2, [r1]
	ldr r4, .L_08119e30
	lsls r3, r3, #4
	adds r3, #100
	add r3, r9
	strb r2, [r3]
	strb r4, [r1]
	ldr r1, [sp, #24]
	strb r4, [r1]
	bl Func_08124cc0
	bl Func_08124e20
	bl Func_0811bc98
	ldr r5, [sp, #40]
	subs r5, #74
	cmp r5, #1
	bhi .L_08119e34
	ldr r2, [sp, #32]
	cmp r2, #2
	beq .L_08119ebc
	b .L_08119e34
.L_08119e30:
	.4byte 0x00000000
.L_08119e34:
	ldr r0, [sp, #16]
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_08119e46
	bl Func_08119374
	cmp r0, #0
	bge .L_08119e4c
	b .L_08119f4c
.L_08119e46:
	movs r0, #20
	bl WaitFrames
.L_08119e4c:
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08119eb0
	bl Func_081197d0
	movs r1, #0
	adds r0, #7
	movs r2, #4
	movs r3, #1
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_08119e72
.L_08119e6c:
	movs r0, #1
	bl WaitFrames
.L_08119e72:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08119e6c
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	bl Func_081197d0
	movs r2, #4
	movs r3, #1
	movs r1, #10
	adds r0, #8
	bl UiText_OpenMessageWindowFar
	movs r1, #24
	adds r5, r0, #0
	movs r0, #44
	bl Func_08120060
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
.L_08119eb0:
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
	b .L_08119b32
.L_08119eb8:
	ldr r5, [sp, #40]
	subs r5, #74
.L_08119ebc:
	bl Func_081195d4
	cmp r5, #1
	bls .L_08119f34
	ldr r2, [sp, #40]
	cmp r2, #80
	beq .L_08119f2e
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08119f2e
	ldr r0, [sp, #16]
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_08119ee4
	movs r0, #58
	bl Audio_PlayCue
.L_08119ee4:
	movs r3, #171
	lsls r3, r3, #3
	add r3, r9
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08119f2a
	movs r0, #58
	bl Audio_PlayCue
	mov r1, r9
	ldrh r3, [r1, #62]
	cmp r3, #1
	bhi .L_08119f2a
	ldrh r3, [r1, #60]
	movs r2, #26
	lsls r3, r3, #1
	adds r3, #16
	ldrh r1, [r1, r3]
	movs r0, #128
	bl BattleUnit_AssignFar
	bl UiWork_ClearValueNameTablesFar
	movs r0, #128
	movs r1, #1
	bl UiText_DrawQuantity
	mov r2, r9
	ldrh r0, [r2, #62]
	ldr r3, .L_08119f74
	adds r0, r0, r3
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08119f2a:
	bl Func_081284c0
.L_08119f2e:
	movs r0, #78
	bl Audio_PlayCue
.L_08119f34:
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r3, #171
	lsls r3, r3, #3
	add r3, r9
	ldr r7, [r3]
	b .L_08119fea
.L_08119f48:
	ldr r5, [sp, #40]
	subs r5, #74
.L_08119f4c:
	bl Func_081195d4
	movs r0, #0
	bl Scheduler_EnableCallbacks
	ldr r3, .L_08119f70
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r3, #171
	lsls r3, r3, #3
	movs r0, #250
	add r3, r9
	lsls r0, r0, #2
	ldr r7, [r3]
	bl GameFlag_SetBit
	b .L_08119fea
.L_08119f70:
	.4byte 0x00000001
.L_08119f74:
	.4byte 0x00000c84
.L_08119f78:
	bl Func_081195d4
	ldr r5, [sp, #40]
	subs r5, #74
	cmp r5, #1
	bls .L_08119fc0
	movs r0, #128
	ldr r3, [sp, #40]
	lsls r0, r0, #2
	adds r0, #126
	cmp r3, r0
	beq .L_08119fc0
	cmp r3, #76
	beq .L_08119fba
	movs r0, #59
	bl Audio_PlayCue
	bl UiWork_ClearValueNameTablesFar
	movs r0, #0
	bl BattleParty_PrepareActiveOwners
	cmp r0, #1
	bne .L_08119fb0
	ldr r0, .L_0811a028
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_08119fb6
.L_08119fb0:
	ldr r0, .L_0811a02c
	bl UiText_ShowMessageAndWaitCoreFar
.L_08119fb6:
	bl BattlePresentation_WaitForAdvance
.L_08119fba:
	movs r0, #78
	bl Audio_PlayCue
.L_08119fc0:
	movs r0, #30
	movs r7, #1
	bl Blend_SetDarkenTarget16
	negs r7, r7
	bl Blend_WaitForTransition
	b .L_08119fea
.L_08119fd0:
	movs r0, #78
	bl Audio_PlayCue
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	ldr r5, [sp, #40]
	movs r7, #186
	lsls r7, r7, #2
	adds r7, #255
	subs r5, #74
.L_08119fea:
	cmp r5, #1
	bhi .L_08119ff2
	bl Func_0811843c
.L_08119ff2:
	bl BattleParty_ResetActiveRuntimeFields
	bl Func_08124cc0
	bl BattlePlacement_UpdateTimedEntries
	ldr r3, .L_0811a030
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #0
	strb r2, [r3]
	ldr r0, .L_0811a034
	bl Scheduler_RemoveCallback
	bl Func_081263f0
	adds r0, r7, #0
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811a028:
	.4byte 0x00000c89
.L_0811a02c:
	.4byte 0x00000c83
.L_0811a030:
	.4byte gPartyState
.L_0811a034:
	.4byte Func_0811b9fc
