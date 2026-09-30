.syntax unified
	.thumb
	.global BattleEvent_Playback
	.thumb_func
BattleEvent_Playback:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080bdbbc
	ldr r3, [r3]
	sub sp, #44
	movs r1, #215
	movs r2, #128
	str r3, [sp, #8]
	lsls r1, r1, #3
	lsls r2, r2, #4
	adds r7, r3, r1
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r3, #0
	bne .L_080bd8c2
	b .L_080bdfb4
.L_080bd8c2:
	movs r3, #164
	lsls r3, r3, #1
	adds r5, r7, r3
	ldr r3, [r5]
	cmp r3, #4
	bne .L_080bd8d0
	b .L_080bdfb4
.L_080bd8d0:
	cmp r3, #1
	bne .L_080bd91e
	ldr r2, .L_080bdbc0
	movs r4, #160
	ldr r1, [sp, #8]
	lsls r4, r4, #1
	adds r3, r1, r2
	adds r6, r7, r4
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r3, [r6]
	cmp r3, r2
	bge .L_080bd918
	adds r4, #4
	movs r1, #166
	movs r2, #0
	adds r3, r7, r4
	lsls r1, r1, #1
	str r2, [r3]
	adds r4, #12
	adds r3, r7, r1
	str r2, [r3]
	adds r3, r7, r4
	str r2, [r3]
	ldr r1, [sp, #8]
	ldr r2, .L_080bdbc4
	adds r0, r1, r2
	ldr r1, [r6]
	bl Battle_ResolveTargetAction
	ldr r3, [r6]
	adds r3, #1
	str r3, [r6]
	movs r3, #2
	str r3, [r5]
	b .L_080bd8c2
.L_080bd918:
	movs r3, #4
	str r3, [r5]
	b .L_080bd8c2
.L_080bd91e:
	cmp r3, #2
	beq .L_080bd924
	b .L_080bdb7a
.L_080bd924:
	movs r4, #166
	movs r1, #162
	lsls r4, r4, #1
	lsls r1, r1, #1
	adds r3, r7, r4
	adds r2, r7, r1
	ldr r5, [r3]
	ldr r3, [r2]
	cmp r5, r3
	blt .L_080bd93a
	b .L_080bdb66
.L_080bd93a:
	adds r6, r5, #0
.L_080bd93c:
	movs r3, #168
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	cmp r3, #0
	beq .L_080bd94e
	subs r3, #1
	str r3, [r2]
	b .L_080bdfb4
.L_080bd94e:
	ldrb r3, [r7, r6]
	cmp r3, #14
	bls .L_080bd956
	b .L_080bdb3e
.L_080bd956:
	ldr r2, .L_080bdbc8
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080bd960:
	.4byte .L_080bd9b6
	.4byte .L_080bd9c4
	.4byte .L_080bd9d2
	.4byte .L_080bd9de
	.4byte .L_080bd9fa
	.4byte .L_080bda16
	.4byte .L_080bd9f0
	.4byte .L_080bda30
	.4byte .L_080bda42
	.4byte .L_080bda82
	.4byte .L_080bdb02
	.4byte .L_080bdb10
	.4byte .L_080bda36
	.4byte .L_080bd9a8
	.4byte .L_080bd99c
.L_080bd99c:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	bl Func_080f9010
	b .L_080bdb3e
.L_080bd9a8:
	lsls r3, r6, #2
	adds r3, #64
	ldr r1, [r7, r3]
	adds r0, r7, #0
	bl Battle_SetRuntimeFlagBit0
	b .L_080bdb3e
.L_080bd9b6:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r1, #1
	bl Func_08015120
	b .L_080bdb3e
.L_080bd9c4:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r1, #5
	bl Func_08015120
	b .L_080bdb3e
.L_080bd9d2:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	ldr r3, .L_080bdbcc
	movs r1, #2
	b .L_080bd9e8
.L_080bd9de:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	ldr r3, .L_080bdbd0
	movs r1, #4
.L_080bd9e8:
	ands r0, r3
	bl Func_08015120
	b .L_080bdb3e
.L_080bd9f0:
	ldr r3, .L_080bdbd4
	ldr r2, [r3]
	movs r3, #1
	str r3, [r2, #8]
	b .L_080bdb3e
.L_080bd9fa:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	cmp r0, #0
	blt .L_080bda08
	bl UiText_PrepareMessageWorkFar
.L_080bda08:
	movs r4, #164
	lsls r4, r4, #1
	adds r2, r7, r4
	movs r3, #3
	str r3, [r2]
	ldr r2, .L_080bdbd8
	b .L_080bda7c
.L_080bda16:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	cmp r0, #0
	blt .L_080bda24
	bl UiText_PrepareMessageWorkFar
.L_080bda24:
	movs r1, #164
	lsls r1, r1, #1
	adds r2, r7, r1
	movs r3, #13
	str r3, [r2]
	b .L_080bdb3e
.L_080bda30:
	bl UiWork_ClearValueNameTablesFar
	b .L_080bdb3e
.L_080bda36:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	bl BattleActor_DestroyTemporaryObject
	b .L_080bdb3e
.L_080bda42:
	movs r2, #180
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	cmp r0, #0
	ble .L_080bda52
	bl Func_080f9010
.L_080bda52:
	movs r3, #178
	lsls r3, r3, #1
	adds r2, r7, r3
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	str r0, [r2]
	bl GetBattleObjectSlot
	movs r1, #5
	ldr r0, [r0]
	bl Func_08009080
	movs r4, #164
	lsls r4, r4, #1
	movs r1, #168
	adds r2, r7, r4
	movs r3, #10
	lsls r1, r1, #1
	str r3, [r2]
	adds r2, r7, r1
.L_080bda7c:
	movs r3, #0
	str r3, [r2]
	b .L_080bdb3e
.L_080bda82:
	lsls r3, r6, #2
	adds r3, #64
	movs r2, #178
	lsls r2, r2, #1
	ldr r0, [r7, r3]
	adds r5, r7, r2
	movs r4, #182
	lsls r4, r4, #1
	str r0, [r5]
	adds r3, r7, r4
	ldr r1, [r3]
	bl BattleEnemy_RecordDefeat
	ldr r0, [r5]
	bl BattleActor_ResetRuntimeFields
	ldr r0, [r5]
	bl Func_08077008
	movs r5, #0
	adds r6, r0, #0
	b .L_080bdaca
.L_080bdaae:
	movs r1, #149
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_080bdac2
	movs r1, #4
	bl Object_InitializeMode
	b .L_080bdac8
.L_080bdac2:
	movs r1, #5
	bl Object_InitializeMode
.L_080bdac8:
	adds r5, #1
.L_080bdaca:
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	bl GetBattleObjectSlot
	adds r1, r5, #0
	ldr r0, [r0]
	bl GetMotionRecord
	cmp r0, #0
	bne .L_080bdaae
	movs r4, #149
	lsls r4, r4, #1
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080bdb3e
	movs r1, #164
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #11
	str r2, [r3]
	movs r2, #168
	lsls r2, r2, #1
	adds r3, r7, r2
	str r0, [r3]
	b .L_080bdb3e
.L_080bdb02:
	ldr r3, .L_080bdbbc
	ldr r3, [r3]
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_DrawPartyStatusContentsFar
	b .L_080bdb3e
.L_080bdb10:
	lsls r5, r6, #2
	adds r5, #64
	ldr r0, [r7, r5]
	bl GetBattleObjectSlot
	adds r1, r0, #0
	ldr r0, [r7, r5]
	bl BattleUnit_BuildStatusFlags
	ldr r0, [r7, r5]
	bl GetBattleObjectSlot
	adds r6, r0, #0
	ldr r0, [r7, r5]
	bl BattleMotion_GetSlotField14
	adds r1, r0, #0
	ldr r0, [r6]
	bl BattleMotion_SetRecordChildValues
	ldr r0, [r7, r5]
	bl BattlePres_SetActorModeAndAction
.L_080bdb3e:
	movs r3, #166
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	movs r4, #162
	adds r5, r3, #1
	str r5, [r2]
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r3, [r3]
	cmp r5, r3
	bge .L_080bdb66
	movs r1, #164
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r3, [r3]
	adds r6, r5, #0
	cmp r3, #2
	bne .L_080bdb66
	b .L_080bd93c
.L_080bdb66:
	movs r3, #164
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	cmp r3, #2
	beq .L_080bdb74
	b .L_080bd8c2
.L_080bdb74:
	movs r3, #1
	str r3, [r2]
	b .L_080bd8c2
.L_080bdb7a:
	cmp r3, #3
	beq .L_080bdb82
	cmp r3, #13
	bne .L_080bdbe0
.L_080bdb82:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	bne .L_080bdb8c
	b .L_080bdfb4
.L_080bdb8c:
	ldr r3, [r5]
	cmp r3, #13
	bne .L_080bdb9e
	movs r4, #168
	movs r3, #2
	lsls r4, r4, #1
	str r3, [r5]
	adds r2, r7, r4
	b .L_080bdd1c
.L_080bdb9e:
	movs r1, #176
	movs r3, #5
	lsls r1, r1, #1
	str r3, [r5]
	adds r2, r7, r1
	subs r3, #6
	str r3, [r2]
	movs r3, #168
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, .L_080bdbdc
	ldr r3, [r3]
	str r3, [r2]
	b .L_080bd8c2
	.2byte 0x0000
.L_080bdbbc:
	.4byte Data_03001e74_a
.L_080bdbc0:
	.4byte 0x00000655
.L_080bdbc4:
	.4byte 0x00000654
.L_080bdbc8:
	.4byte .L_080bd960
.L_080bdbcc:
	.4byte 0x000001ff
.L_080bdbd0:
	.4byte 0x00003fff
.L_080bdbd4:
	.4byte gBattleDisplayWork
.L_080bdbd8:
	.4byte gKeysPressedLatch
.L_080bdbdc:
	.4byte gFrameTick
.L_080bdbe0:
	cmp r3, #5
	beq .L_080bdbe6
	b .L_080bdd2c
.L_080bdbe6:
	ldr r2, .L_080bdc8c
	ldr r3, [r2]
	movs r2, #7
	lsrs r3, r3, #2
	ldr r1, .L_080bdc90
	ands r3, r2
	lsls r3, r3, #7
	adds r3, r3, r1
	mov r10, r3
	ldr r3, .L_080bdc94
	movs r4, #170
	ldr r3, [r3]
	lsls r4, r4, #1
	adds r4, r4, r7
	mov r9, r4
	ldr r4, [r3]
	ldr r3, [r3, #4]
	movs r2, #176
	str r3, [sp, #4]
	lsls r2, r2, #1
	adds r6, r7, r2
	mov r11, r4
	ldr r3, [r6]
	movs r4, #1
	movs r1, #0
	negs r4, r4
	mov r8, r1
	cmp r3, r4
	bne .L_080bdc26
	ldr r1, [sp, #8]
	ldr r3, [r1, #84]
	str r3, [r6]
.L_080bdc26:
	ldr r5, .L_080bdc98
	bl UiWork_ClearValueNameTablesFar
	adds r0, r5, #0
	movs r1, #4
	bl QueueIoWriteDelay10
	adds r0, r5, #0
	movs r1, #16
	bl QueueIoWriteDelay6
	movs r3, #160
	mov r2, r9
	lsls r3, r3, #8
	str r3, [r2, #4]
	mov r3, r8
	str r3, [r2, #8]
	mov r1, r10
	ldr r0, [r6]
	bl Resource_GetBuffer
	ldr r3, .L_080bdc84
	mov r4, r9
	ldrh r2, [r4, #8]
	ands r0, r3
	ldr r3, .L_080bdc9c
	ands r3, r2
	orrs r3, r0
	mov r1, r9
	strh r3, [r1, #8]
	ldr r4, [sp, #4]
	mov r3, r11
	ldrh r2, [r3, #12]
	ldrh r3, [r4, #4]
	lsls r2, r2, #3
	lsrs r3, r3, #8
	adds r2, r2, r3
	adds r2, #4
	mov r8, r2
	ldr r3, .L_080bdc88
	mov r1, r8
	ands r1, r3
	mov r3, r9
	ldrh r2, [r3, #6]
	ldr r3, .L_080bdca0
	ands r3, r2
	b .L_080bdca4
.L_080bdc84:
	.4byte 0x000003ff
.L_080bdc88:
	.4byte 0x000001ff
.L_080bdc8c:
	.4byte gFrameCount
.L_080bdc90:
	.4byte BattlePres_AdvanceArrowTiles
.L_080bdc94:
	.4byte gBattleDisplayWork
.L_080bdc98:
	.4byte 0x0400004a
.L_080bdc9c:
	.4byte 0xfffffc00
.L_080bdca0:
	.4byte 0xfffffe00
.L_080bdca4:
	orrs r3, r1
	ldr r1, .L_080bdfc8
	ldr r0, [r1]
	mov r4, r9
	strh r3, [r4, #6]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_080bdcbc
	ldr r2, .L_080bdfcc
	adds r0, r0, r2
.L_080bdcbc:
	mov r4, r11
	ldrh r3, [r4, #14]
	ldr r1, [sp, #4]
	asrs r2, r0, #15
	lsls r3, r3, #3
	adds r2, r2, r3
	ldrh r3, [r1, #6]
	lsrs r3, r3, #8
	adds r3, r3, r2
	ldr r0, .L_080bdfd0
	adds r3, #6
	mov r2, r9
	strb r3, [r2, #4]
	ldr r3, [r0]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_080bdd06
	ldr r3, .L_080bdfd4
	ldr r1, .L_080bdfd8
	ldr r3, [r3]
	ands r3, r1
	cmp r3, #0
	bne .L_080bdd06
	movs r4, #168
	ldr r3, .L_080bdfdc
	lsls r4, r4, #1
	adds r2, r7, r4
	ldr r3, [r3]
	ldr r2, [r2]
	subs r3, r3, r2
	cmp r3, #10
	bls .L_080bdd22
	ldr r3, [r0]
	ands r3, r1
	cmp r3, #0
	beq .L_080bdd22
.L_080bdd06:
	movs r0, #111
	bl Func_080f9010
	movs r1, #164
	lsls r1, r1, #1
	adds r2, r7, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #168
	lsls r3, r3, #1
	adds r2, r7, r3
.L_080bdd1c:
	movs r3, #0
	str r3, [r2]
	b .L_080bd8c2
.L_080bdd22:
	mov r0, r9
	movs r1, #240
	bl Runtime_PushSlotEntry
	b .L_080bdfb4
.L_080bdd2c:
	cmp r3, #10
	bne .L_080bddb8
	movs r4, #168
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080bdd96
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080bdd6e
	add r1, sp, #28
	mov r9, r1
	mov r2, r9
	movs r3, #255
	strh r3, [r2]
	adds r3, #101
	adds r5, r7, r3
	ldr r0, [r5]
	bl GetBattleObjectSlot
	adds r6, r0, #0
	ldr r0, [r5]
	bl BattleMotion_GetSlotField14
	adds r1, r0, #0
	ldr r0, [r6]
	bl BattleMotion_SetRecordChildValues
	b .L_080bdd90
.L_080bdd6e:
	movs r1, #178
	lsls r1, r1, #1
	movs r4, #28
	adds r3, r7, r1
	add r4, sp
	ldr r0, [r3]
	mov r9, r4
	mov r2, r9
	movs r3, #255
	strh r0, [r2]
	strh r3, [r4, #2]
	bl GetBattleObjectSlot
	movs r1, #7
	ldr r0, [r0]
	bl BattleMotion_SetRecordChildValues
.L_080bdd90:
	mov r0, r9
	bl Func_080152b8
.L_080bdd96:
	movs r2, #168
	lsls r2, r2, #1
	adds r1, r7, r2
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	cmp r3, #8
	bgt .L_080bdda8
	b .L_080bdfb4
.L_080bdda8:
	movs r4, #164
	lsls r4, r4, #1
	adds r3, r7, r4
	movs r2, #2
	str r2, [r3]
	movs r3, #0
	str r3, [r1]
	b .L_080bd8c2
.L_080bddb8:
	cmp r3, #11
	beq .L_080bddbe
	b .L_080bd8c2
.L_080bddbe:
	movs r1, #168
	lsls r1, r1, #1
	adds r5, r7, r1
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080bddd2
	movs r2, #128
	lsls r2, r2, #3
	cmp r3, r2
	blt .L_080bdeca
.L_080bddd2:
	movs r4, #6
	mov r10, r4
	cmp r3, #0
	bne .L_080bde1c
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080bde1c
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	bl Func_08077008
	movs r3, #148
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r0, [r0]
	bl Summon_GetEntryByte3Kind
	cmp r0, #0
	blt .L_080bde10
	subs r0, #1
	cmp r0, #0
	bge .L_080bde0a
	movs r0, #0
.L_080bde0a:
	adds r0, #146
	bl Func_080f9010
.L_080bde10:
	movs r4, #168
	lsls r4, r4, #1
	movs r3, #128
	adds r2, r7, r4
	lsls r3, r3, #3
	str r3, [r2]
.L_080bde1c:
	movs r1, #168
	lsls r1, r1, #1
	adds r2, r7, r1
	ldr r3, [r2]
	ldr r4, .L_080bdfe0
	cmp r3, r4
	ble .L_080bde2e
	movs r3, #0
	str r3, [r2]
.L_080bde2e:
	cmp r3, #0
	bne .L_080bde54
	movs r1, #178
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl Func_08077008
	movs r2, #148
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	bl Summon_GetEntryByte3Kind
	cmp r0, #0
	blt .L_080bde54
	adds r0, #146
	bl Func_080f9010
.L_080bde54:
	movs r4, #168
	lsls r4, r4, #1
	adds r3, r7, r4
	movs r1, #128
	ldr r3, [r3]
	lsls r1, r1, #3
	cmp r3, r1
	blt .L_080bde7c
	ldr r2, .L_080bdfe4
	adds r0, r3, r2
	cmp r0, #0
	bge .L_080bde70
	ldr r4, .L_080bdfe8
	adds r0, r3, r4
.L_080bde70:
	asrs r0, r0, #3
	movs r1, #5
	bl Func_080022fc
	adds r0, #1
	mov r10, r0
.L_080bde7c:
	mov r1, r10
	cmp r1, #6
	beq .L_080bde94
	movs r2, #168
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r3, [r3]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	beq .L_080bde94
	b .L_080bdfa2
.L_080bde94:
	movs r3, #255
	movs r6, #0
	add r5, sp, #12
	mov r8, r3
	b .L_080bdeb0
.L_080bde9e:
	ldr r2, [r0, #40]
	ldrb r3, [r2, #22]
	mov r1, r8
	mov r4, r10
	orrs r3, r1
	stmia r5!, {r0}
	strb r4, [r2, #5]
	strb r3, [r2, #22]
	adds r6, #1
.L_080bdeb0:
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	bl GetBattleObjectSlot
	adds r1, r6, #0
	ldr r0, [r0]
	bl GetMotionRecord
	cmp r0, #0
	bne .L_080bde9e
	b .L_080bdfa2
.L_080bdeca:
	cmp r3, #4
	bne .L_080bdede
	movs r4, #178
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r0, [r3]
	bl BattleActor_RemoveFromLists
	ldr r3, [r5]
	b .L_080bdfb0
.L_080bdede:
	cmp r3, #4
	ble .L_080bdfb0
	movs r1, #178
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl GetBattleObjectSlot
	movs r3, #1
	adds r6, r0, #0
	movs r2, #0
	add r5, sp, #12
	strh r3, [r6, #42]
	b .L_080bdefe
.L_080bdefa:
	stmia r5!, {r0}
	adds r2, #1
.L_080bdefe:
	adds r1, r2, #0
	ldr r0, [r6]
	str r2, [sp, #0]
	bl GetMotionRecord
	ldr r2, [sp, #0]
	cmp r0, #0
	bne .L_080bdefa
	movs r4, #168
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r3, [r3]
	movs r1, #20
	lsls r3, r3, #2
	negs r1, r1
	adds r1, r1, r3
	mov r8, r1
	cmp r1, #127
	ble .L_080bdf60
	cmp r2, #0
	ble .L_080bdf3e
	add r6, sp, #12
	adds r5, r2, #0
.L_080bdf2c:
	add r2, sp, #44
	ldmia r6!, {r0}
	mov r9, r2
	movs r1, #0
	subs r5, #1
	bl Func_080bd850
	cmp r5, #0
	bne .L_080bdf2c
.L_080bdf3e:
	movs r4, #178
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r0, [r3]
	bl ActivateBattleObjectSlot
	movs r1, #164
	lsls r1, r1, #1
	adds r2, r7, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #168
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #0
	str r3, [r2]
	b .L_080bdfb4
.L_080bdf60:
	cmp r2, #0
	ble .L_080bdfa2
	movs r4, #19
	movs r1, #18
	negs r4, r4
	negs r1, r1
	adds r4, r4, r3
	adds r1, r1, r3
	subs r3, #17
	mov r11, r4
	mov r9, r1
	mov r10, r3
	adds r6, r2, #0
	add r5, sp, #12
.L_080bdf7c:
	ldr r0, [r5]
	mov r1, r8
	bl render_animated_tile_frameFar
	ldr r0, [r5]
	mov r1, r11
	bl render_animated_tile_frameFar
	ldr r0, [r5]
	mov r1, r9
	bl render_animated_tile_frameFar
	subs r6, #1
	ldmia r5!, {r0}
	mov r1, r10
	bl render_animated_tile_frameFar
	cmp r6, #0
	bne .L_080bdf7c
.L_080bdfa2:
	movs r3, #168
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_080bdfb4
.L_080bdfb0:
	adds r3, #1
	str r3, [r5]
.L_080bdfb4:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080bdfc8:
	.4byte gFrameCount
.L_080bdfcc:
	.4byte 0x00007fff
.L_080bdfd0:
	.4byte Data_03001ae8
.L_080bdfd4:
	.4byte gKeysPressedLatch
.L_080bdfd8:
	.4byte 0x00000303
.L_080bdfdc:
	.4byte gFrameTick
.L_080bdfe0:
	.4byte 0x0000041d
.L_080bdfe4:
	.4byte 0xfffffc00
.L_080bdfe8:
	.4byte 0xfffffc07
