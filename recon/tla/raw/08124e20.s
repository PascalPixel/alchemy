.syntax unified
	.thumb
	.global Func_08124e20
	.thumb_func
Func_08124e20:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	sub sp, #52
	str r3, [sp, #8]
	adds r3, #68
	str r3, [sp, #4]
	movs r1, #1
	ldrb r2, [r3]
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	mov r11, r3
	movs r2, #0
	add r11, r1
	mov r9, r2
	cmp r9, r11
	blt .L_08124e54
	b .L_08124f9a
.L_08124e54:
	mov r0, r9
	bl Trade_GetOfferStateFar
	movs r2, #148
	adds r3, r0, #0
	movs r5, #8
	lsls r2, r2, #1
	adds r5, r5, r3
	adds r3, r3, r2
	ldr r3, [r3]
	movs r1, #0
	mov r8, r1
	mov r10, r5
	cmp r8, r3
	bge .L_08124eaa
.L_08124e72:
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, #0
	ble .L_08124e98
	ldrb r0, [r5, #2]
	bl GetBattleObjectSlot
	cmp r0, #0
	beq .L_08124e98
	ldrb r0, [r5, #2]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_08124e98
	ldrb r3, [r5, #3]
	subs r3, #1
	strb r3, [r5, #3]
.L_08124e98:
	movs r3, #144
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	movs r2, #1
	add r8, r2
	adds r5, #4
	cmp r8, r3
	blt .L_08124e72
.L_08124eaa:
	movs r3, #0
	mov r8, r3
	movs r3, #144
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	cmp r8, r3
	bge .L_08124f90
	mov r6, r10
.L_08124ebc:
	movs r3, #3
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bne .L_08124f7e
	ldrb r5, [r6, #2]
	adds r0, r5, #0
	bl GetBattleObjectSlot
	cmp r0, #0
	beq .L_08124f84
	bl Func_081234a4
	movs r0, #30
	bl Func_08122c88
	movs r0, #0
	adds r1, r5, #0
	bl BattleEv_Push
	ldrb r3, [r6]
	movs r2, #150
	lsls r1, r3, #2
	adds r1, r1, r3
	ldrb r3, [r6, #1]
	lsls r1, r1, #2
	adds r1, r1, r3
	lsls r2, r2, #1
	adds r1, r1, r2
	movs r0, #3
	bl BattleEv_Push
	movs r0, #14
	movs r1, #175
	bl BattleEv_Push
	movs r0, #10
	movs r1, #0
	bl BattleEv_Push
	movs r0, #4
	ldr r1, .L_0812510c
	bl BattleEv_Push
	movs r0, #11
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #212
	bl Audio_PlayCue
	adds r0, r5, #0
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124f46
	adds r0, r5, #0
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Object_SetMode
	adds r0, r5, #0
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_08124f46:
	ldrb r7, [r6]
	ldrb r2, [r6, #1]
	adds r1, r7, #0
	adds r0, r5, #0
	bl Djinn_ActivateFar
	ldrb r1, [r6]
	ldrb r2, [r6, #1]
	adds r0, r5, #0
	bl Trade_RemoveOfferFar
	adds r0, r5, #0
	bl Owner_RecalculateStatsFar
	adds r0, r5, #0
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124f78
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #3
	movs r3, #0
	bl Func_08127308
.L_08124f78:
	bl Func_081234f0
	b .L_08124f84
.L_08124f7e:
	movs r3, #1
	adds r6, #4
	add r8, r3
.L_08124f84:
	movs r3, #144
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	cmp r8, r3
	blt .L_08124ebc
.L_08124f90:
	movs r5, #1
	add r9, r5
	cmp r9, r11
	bge .L_08124f9a
	b .L_08124e54
.L_08124f9a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r1, #206
	lsls r1, r1, #3
	adds r3, r3, r1
	ldrh r1, [r3]
	movs r2, #0
	movs r0, #2
	bl Func_0812628c
	ldr r3, .L_08125110
	ldr r2, [sp, #4]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #12]
	str r4, [sp, #16]
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08124fd8
	ldr r3, [sp, #8]
	adds r3, #80
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08124fd8
	movs r3, #2
	str r3, [sp, #12]
	add r0, sp, #12
	movs r3, #1
	str r3, [r0, #4]
	b .L_08124fda
.L_08124fd8:
	add r0, sp, #12
.L_08124fda:
	movs r5, #1
	movs r3, #20
	str r5, [sp, #0]
	add r3, sp
	mov r11, r3
	mov r9, r0
.L_08124fe6:
	mov r1, r9
	ldr r0, [r1]
	mov r1, r11
	bl BattleParty_ListActorIds
	mov r2, r9
	ldr r3, [r2]
	mov r8, r0
	cmp r3, #1
	bne .L_08125004
	lsls r0, r0, #1
	add r0, r11
	bl BattleParty_PrepareReserveOwners
	add r8, r0
.L_08125004:
	movs r3, #0
	mov r10, r3
	cmp r10, r8
	blt .L_0812500e
	b .L_0812533e
.L_0812500e:
	mov r5, r10
	lsls r3, r5, #1
	mov r1, r11
	ldrh r6, [r1, r3]
	movs r5, #162
	adds r0, r6, #0
	bl Owner_GetState
	movs r3, #68
	adds r7, r0, #0
	adds r3, #255
	adds r2, r7, r3
	movs r3, #0
	strb r3, [r2]
	lsls r5, r5, #1
	adds r1, r7, r5
	ldrb r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0812503a
	adds r3, #255
	strb r3, [r1]
.L_0812503a:
	movs r3, #56
	ldrsh r2, [r7, r3]
	ldrh r1, [r7, #56]
	cmp r2, #0
	beq .L_08125130
	adds r0, r7, #0
	adds r0, #68
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_081250c0
	movs r5, #52
	ldrsh r3, [r7, r5]
	ldrh r4, [r7, #52]
	cmp r2, r3
	beq .L_081250c0
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r3, [r3]
	ldrb r5, [r0]
	cmp r3, #91
	blt .L_08125074
	cmp r3, #97
	ble .L_0812506e
	cmp r3, #218
	bne .L_08125074
.L_0812506e:
	lsls r3, r5, #2
	adds r3, r3, r5
	lsls r5, r3, #1
.L_08125074:
	lsls r3, r1, #16
	asrs r1, r3, #16
	lsls r3, r4, #16
	adds r2, r1, r5
	asrs r3, r3, #16
	cmp r2, r3
	ble .L_08125084
	subs r5, r3, r1
.L_08125084:
	adds r1, r5, #0
	adds r0, r6, #0
	bl Owner_AdjustFirstValueFar
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r5, #0
	movs r1, #5
	bl UiText_DrawQuantity
	movs r3, #56
	ldrsh r2, [r7, r3]
	movs r5, #52
	ldrsh r3, [r7, r5]
	cmp r2, r3
	bne .L_081250b0
	ldr r0, .L_08125114
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_081250b6
.L_081250b0:
	ldr r0, .L_08125118
	bl UiText_ShowMessageAndWaitCoreFar
.L_081250b6:
	movs r0, #175
	bl Audio_PlayCue
	bl BattlePresentation_WaitForAdvance
.L_081250c0:
	adds r0, r7, #0
	adds r0, #69
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_08125130
	movs r2, #58
	ldrsh r1, [r7, r2]
	movs r3, #54
	ldrsh r2, [r7, r3]
	cmp r1, r2
	beq .L_08125130
	ldrb r5, [r0]
	adds r3, r1, r5
	cmp r3, r2
	ble .L_081250e0
	subs r5, r2, r1
.L_081250e0:
	adds r1, r5, #0
	adds r0, r6, #0
	bl Owner_AdjustSecondValueFar
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r5, #0
	movs r1, #5
	bl UiText_DrawQuantity
	movs r5, #58
	ldrsh r2, [r7, r5]
	movs r1, #54
	ldrsh r3, [r7, r1]
	cmp r2, r3
	bne .L_08125120
	ldr r0, .L_0812511c
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_08125126
.L_0812510c:
	.4byte 0x00000cf7
.L_08125110:
	.4byte Data_081297c4
.L_08125114:
	.4byte 0x00000c6c
.L_08125118:
	.4byte 0x00000c69
.L_0812511c:
	.4byte 0x00000c6d
.L_08125120:
	ldr r0, .L_0812535c
	bl UiText_ShowMessageAndWaitCoreFar
.L_08125126:
	movs r0, #175
	bl Audio_PlayCue
	bl BattlePresentation_WaitForAdvance
.L_08125130:
	adds r0, r6, #0
	bl BattleUnit_TickCounter146
	cmp r0, #0
	beq .L_0812515a
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125360
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_0812515a:
	adds r0, r6, #0
	bl BattleUnit_TickCounter132
	cmp r0, #0
	beq .L_08125184
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125364
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08125184:
	adds r0, r6, #0
	bl BattleUnit_TickCounter134
	cmp r0, #0
	beq .L_081251ae
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125368
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081251ae:
	adds r0, r6, #0
	bl BattleUnit_TickCounter136
	cmp r0, #0
	beq .L_081251d8
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0812536c
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081251d8:
	adds r0, r6, #0
	bl BattleUnit_TickCounter138
	cmp r0, #0
	beq .L_08125202
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125370
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08125202:
	adds r0, r6, #0
	bl BattleUnit_TickCounter139
	cmp r0, #0
	beq .L_0812522c
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125374
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_0812522c:
	adds r0, r6, #0
	bl BattleUnit_TickCounter13a
	cmp r0, #0
	beq .L_08125256
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125378
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08125256:
	adds r0, r6, #0
	bl BattleUnit_TickCounter13b
	cmp r0, #0
	beq .L_08125286
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	movs r1, #1
	adds r0, r6, #0
	bl UiText_DrawQuantity
	adds r0, r6, #0
	bl BattlePres_SetActorModeAndAction
	ldr r0, .L_0812537c
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08125286:
	adds r0, r6, #0
	bl BattleUnit_TickCounter13c
	cmp r0, #0
	beq .L_081252b6
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	movs r1, #1
	adds r0, r6, #0
	bl UiText_DrawQuantity
	adds r0, r6, #0
	bl BattlePres_SetActorModeAndAction
	ldr r0, .L_08125380
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081252b6:
	adds r0, r6, #0
	bl Battle_AdvanceCounterAndCheckChance
	cmp r0, #0
	beq .L_081252e0
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125384
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_081252e0:
	adds r0, r6, #0
	bl sub_08124af8
	cmp r0, #0
	beq .L_0812530a
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08125388
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_0812530a:
	adds r0, r6, #0
	bl BattleUnit_TickCounter13f
	cmp r0, #0
	beq .L_08125334
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r6, #0
	bl BattleUnit_BuildStatusFlags
	adds r0, r6, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0812538c
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_08125334:
	movs r2, #1
	add r10, r2
	cmp r10, r8
	bge .L_0812533e
	b .L_0812500e
.L_0812533e:
	ldr r5, [sp, #0]
	movs r3, #4
	subs r5, #1
	add r9, r3
	str r5, [sp, #0]
	cmp r5, #0
	blt .L_0812534e
	b .L_08124fe6
.L_0812534e:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0812535c:
	.4byte 0x00000c6a
.L_08125360:
	.4byte 0x00000ce9
.L_08125364:
	.4byte 0x00000ce7
.L_08125368:
	.4byte 0x00000ce8
.L_0812536c:
	.4byte 0x00000ce6
.L_08125370:
	.4byte 0x00000ceb
.L_08125374:
	.4byte 0x00000cea
.L_08125378:
	.4byte 0x00000cee
.L_0812537c:
	.4byte 0x00000ced
.L_08125380:
	.4byte 0x00000ce3
.L_08125384:
	.4byte 0x00000cec
.L_08125388:
	.4byte 0x00000cf1
.L_0812538c:
	.4byte 0x00000cf2
