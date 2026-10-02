@ Uncredited disassembly re-derived from the current owned TBS English ROM.
@ Complete nested selector and command planner; the canonical C draft omits
@ the unread eight-byte frame steering and remains a measured near miss.
	.syntax unified
	.thumb
	.text
	.balign 4
	.type BattleCommand_SelectTargets.0, %function
	.thumb_func
BattleCommand_SelectTargets.0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	add r3, sp, #24
	mov r1, r9
	str r1, [r3]
	mov r10, r1
	bl Ability_GetData
	movs r3, #0
	ldrb r2, [r0]
	str r3, [sp, #20]
	ldrb r3, [r0, #3]
	cmp r3, #5
	beq .LCommand20
	cmp r3, #5
	blt .LCommand21
	cmp r3, #57
	bgt .LCommand21
	cmp r3, #56
	blt .LCommand21
.LCommand20:
	movs r0, #1
	str r0, [sp, #20]
.LCommand21:
	cmp r2, #0
	beq .LCommand22
	cmp r2, #4
	beq .LCommand23
	movs r2, #12
	negs r2, r2
	movs r1, #0
	add r2, r10
	mov r9, r1
	movs r6, #0
	mov r11, r2
	b .LCommand24
.LCommand22:
	mov r3, r10
	subs r3, #4
	ldr r3, [r3]
	movs r1, #1
	strb r2, [r3, #16]
	strb r1, [r3, #1]
	mov r2, r10
	subs r2, #8
	ldr r2, [r2]
	strb r1, [r3, #30]
	strb r2, [r3, #2]
	b .LCommand25
.LCommand23:
	mov r3, r10
	subs r3, #4
	ldr r2, [r3]
	movs r1, #1
	movs r3, #0
	strb r3, [r2, #16]
	strb r1, [r2, #1]
	mov r3, r10
	subs r3, #8
	ldr r3, [r3]
	strb r1, [r2, #30]
	strb r3, [r2, #2]
	b .LCommand25
.LCommand26:
	adds r6, #1
.LCommand24:
	mov r3, r11
	ldr r1, [r3]
	lsls r3, r6, #1
	adds r3, #88
	ldrsh r3, [r1, r3]
	cmp r3, #255
	bne .LCommand26
	str r6, [sp, #16]
	movs r3, #100
	adds r2, r1, #2
	ldrsh r3, [r2, r3]
	movs r6, #0
	cmp r3, #255
	beq .LCommand27
	adds r2, #100
.LCommand28:
	adds r2, #2
	movs r1, #0
	ldrsh r3, [r2, r1]
	adds r6, #1
	cmp r3, #255
	bne .LCommand28
.LCommand27:
	mov r2, r10
	subs r2, #16
	str r6, [sp, #12]
	str r2, [sp, #8]
	ldr r2, [r2]
	ldrh r3, [r2, #10]
	movs r4, #15
	ands r4, r3
	movs r0, #12
	ldrsh r3, [r2, r0]
	subs r2, r4, r3
	adds r3, r4, r3
	subs r3, #1
	adds r6, r2, #1
	str r3, [sp, #4]
	cmp r6, r3
	bgt .LCommand29
	movs r1, #4
	negs r1, r1
	lsls r3, r6, #1
	add r1, r10
	adds r7, r3, #0
	mov r8, r1
	adds r7, #100
.LCommand34:
	cmp r6, #0
	blt .LCommand30
	ldr r2, [sp, #8]
	ldr r3, [r2]
	ldrh r2, [r3, #10]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .LCommand31
	ldr r3, [sp, #12]
	cmp r6, r3
	bge .LCommand30
	mov r0, r11
	ldr r3, [r0]
	adds r3, #2
	ldrsh r5, [r3, r7]
	cmp r5, #254
	beq .LCommand30
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .LCommand32
	adds r0, r5, #0
	str r4, [sp, #0]
	bl Owner_GetStateFar
	movs r1, #56
	ldrsh r3, [r0, r1]
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .LCommand30
.LCommand32:
	mov r2, r8
	ldr r0, [r2]
	mov r2, r9
	adds r1, r0, #2
	adds r2, #28
	movs r3, #1
	strb r3, [r1, r2]
	subs r3, r6, r4
	subs r2, #12
	strb r3, [r0, r2]
	mov r3, r9
	movs r0, #1
	strb r5, [r1, r3]
	add r9, r0
	b .LCommand30
.LCommand31:
	ldr r1, [sp, #16]
	cmp r6, r1
	bge .LCommand30
	mov r3, r11
	ldr r2, [r3]
	lsls r3, r6, #1
	adds r3, #88
	ldrsh r5, [r2, r3]
	cmp r5, #254
	beq .LCommand30
	ldr r1, [sp, #20]
	cmp r1, #0
	bne .LCommand33
	adds r0, r5, #0
	str r4, [sp, #0]
	bl Owner_GetStateFar
	movs r2, #56
	ldrsh r3, [r0, r2]
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .LCommand30
.LCommand33:
	mov r3, r8
	ldr r0, [r3]
	mov r2, r9
	adds r1, r0, #2
	adds r2, #28
	movs r3, #1
	strb r3, [r1, r2]
	subs r2, #12
	subs r3, r6, r4
	strb r3, [r0, r2]
	mov r0, r9
	strb r5, [r1, r0]
	movs r1, #1
	add r9, r1
.LCommand30:
	ldr r2, [sp, #4]
	adds r6, #1
	adds r7, #2
	cmp r6, r2
	ble .LCommand34
	b .LCommand35
.LCommand29:
	movs r3, #4
	negs r3, r3
	add r3, r10
	mov r8, r3
.LCommand35:
	mov r0, r8
	ldr r3, [r0]
	mov r1, r9
	mov r2, r9
	strb r1, [r3, #1]
	cmp r2, #0
	bgt .LCommand25
	ldr r0, [sp, #8]
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand152
	bl UiText_ShowMessageAndWaitCoreFar
	mov r3, r10
	subs r3, #20
	ldr r3, [r3]
	ldr r0, .LCommand153
	adds r2, r3, r0
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	bne .LCommand36
	movs r3, #1
	strb r3, [r2]
.LCommand36:
	movs r0, #1
	negs r0, r0
.LCommand25:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.balign 4, 0
.LCommand152:
	.4byte MsgActorDefends
.LCommand153:
	.4byte 0x0000012b
	.size BattleCommand_SelectTargets.0, .-BattleCommand_SelectTargets.0
	.global BattleCommand_BuildPlan
	.type BattleCommand_BuildPlan, %function
	.thumb_func
BattleCommand_BuildPlan:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	mov r3, sp
	add r2, sp, #32
	adds r3, #44
	str r0, [r2]
	str r3, [sp, #8]
	str r1, [r3]
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	mov r10, r2
	bl Owner_GetStateFar
	mov r1, sp
	adds r1, #28
	str r1, [sp, #12]
	ldr r3, .LCommand154
	ldr r3, [r3]
	add r7, sp, #36
	mov r2, r10
	str r3, [r7]
	ldr r3, [r2]
	str r0, [r1]
	movs r4, #10
	ldrsh r0, [r3, r4]
	bl Battle_GetTaggedSlotValue
	str r0, [sp, #40]
	bl BattleEventRuntime_Reset
	mov r4, r10
	ldr r0, [sp, #8]
	ldr r3, [r4]
	ldr r1, [r0]
	ldrh r3, [r3]
	movs r2, #0
	movs r5, #4
	strb r3, [r1]
	str r2, [r1, #96]
	strb r2, [r1, #1]
	str r2, [r1, #88]
	str r2, [r1, #92]
	str r5, [r1, #80]
	bl UiWork_ClearValueNameTablesFar
	ldr r0, [sp, #12]
	ldr r3, [r0]
	movs r1, #56
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .LCommand37
	bl .LCommand7
.LCommand37:
	ldr r3, .LCommand155
	ldrb r3, [r3]
	cmp r3, #0
	beq .LCommand38
	ldr r0, .LCommand156
	bl GameFlag_IsSet
	cmp r0, #0
	beq .LCommand38
	ldr r1, .LCommand157
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .LCommand38
	ldr r3, [r1]
	movs r2, #1
	ands r3, r5
	mov r8, r2
	cmp r3, #0
	beq .LCommand39
	movs r3, #0
	mov r8, r3
.LCommand39:
	movs r6, #100
	b .LCommand40
.LCommand44:
	cmp r5, #254
	beq .LCommand41
	movs r1, #192
	adds r0, r5, #0
	lsls r1, r1, #24
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .LCommand41
	adds r1, r5, #0
	movs r0, #8
	bl BattleEv_Push
	movs r0, #9
	adds r1, r5, #0
	bl BattleEv_Push
.LCommand41:
	adds r6, #2
.LCommand40:
	mov r4, r8
	cmp r4, #0
	beq .LCommand42
	ldr r3, [r7]
	adds r3, #2
	ldrsh r5, [r3, r6]
	b .LCommand43
.LCommand42:
	ldr r2, [r7]
	adds r3, r6, #0
	subs r3, #12
	ldrsh r5, [r2, r3]
.LCommand43:
	cmp r5, #255
	bne .LCommand44
	bl BattleEv_DispatchQueued
	bl .LCommand7
.LCommand38:
	bl UiWork_ClearValueNameTablesFar
	ldr r3, [sp, #12]
	ldr r4, .LCommand158
	ldr r2, [r3]
	adds r1, r2, r4
	ldrb r3, [r1]
	cmp r3, #0
	beq .LCommand45
	movs r3, #0
	mov r0, r10
	strb r3, [r1]
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand159
	bl UiText_ShowMessageAndWaitCoreFar
	bl .LCommand48
.LCommand45:
	movs r4, #158
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .LCommand46
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand160
	bl UiText_ShowMessageAndWaitCoreFar
	bl .LCommand48
.LCommand46:
	ldr r4, .LCommand161
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .LCommand47
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand162
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand48
.LCommand47:
	movs r4, #152
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .LCommand49
	mov r0, r10
	ldr r3, [r0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #3
	beq .LCommand49
	bl BattleRandom16Far
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .LCommand49
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiText_DrawQuantity
	ldr r0, .LCommand163
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand48
.LCommand49:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .LCommand50
	b .LCommand7
.LCommand50:
	ldr r4, [sp, #8]
	ldr r3, [r4]
	movs r2, #1
	mov r11, r2
	movs r1, #0
	adds r3, #44
	movs r2, #13
.LCommand51:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .LCommand51
	ldr r0, [sp, #8]
	movs r2, #1
	ldr r3, [r0]
	negs r2, r2
	adds r1, r2, #0
	adds r3, #58
	movs r2, #13
.LCommand52:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .LCommand52
	mov r4, r10
	ldr r3, [r4]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #99
	bls .LCommand53
	bl .LCommand8
.LCommand53:
	ldr r2, .LCommand164
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.LCommand149:
	.4byte .LCommand0
	.4byte .LCommand1
	.4byte .LCommand2
	.4byte .LCommand3
	.4byte .LCommand4
	.4byte .LCommand5
	.4byte .LCommand6
	.4byte .LCommand3
	.4byte .LCommand7
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand8
	.4byte .LCommand9
.LCommand9:
	mov r1, r10
	ldr r3, [r1]
	ldrh r3, [r3]
	movs r2, #224
	lsls r0, r3, #16
	lsls r2, r2, #11
	cmp r0, r2
	bhi .LCommand54
	ldr r0, .LCommand165
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand55
.LCommand54:
	asrs r0, r0, #16
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand166
	bl UiText_ShowMessageAndWaitCoreFar
.LCommand55:
	bl BattlePresentation_WaitForAdvance
	ldr r3, [sp, #8]
	ldr r2, [r3]
	movs r3, #7
	str r3, [r2, #84]
	bl .LCommand215
	movs r0, r0
.LCommand154:
	.4byte gBattleWork
.LCommand155:
	.4byte gDebugMode
.LCommand156:
	.4byte 0x0000016d
.LCommand157:
	.4byte gKeysHeld
.LCommand158:
	.4byte 0x00000145
.LCommand159:
	.4byte MsgUnableToMove
.LCommand160:
	.4byte MsgIsAsleep
.LCommand161:
	.4byte 0x0000013b
.LCommand162:
	.4byte MsgIsParalyzed
.LCommand163:
	.4byte MsgIsBound
.LCommand164:
	.4byte .LCommand149
.LCommand165:
	.4byte MsgPartyFlees
.LCommand166:
	.4byte MsgActorRuns
.LCommand0:
	ldr r4, [sp, #12]
	ldr r0, [r4]
	bl RollWeaponUnleashFar
	mov r11, r0
	add r0, sp, #48
	mov r9, r0
	mov r0, r11
	bl BattleCommand_SelectTargets.0
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .LCommand56
	bl .LCommand105
.LCommand56:
	mov r2, r11
	cmp r2, #1
	bne .LCommand57
	b .LCommand58
.LCommand57:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r2, [sp, #12]
	movs r1, #1
	ldr r0, [r2]
	bl Inventory_GetEquippedItemFar
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r5, .LCommand167
	adds r0, r5, #0
	bl UiText_ShowMessageAndWaitCoreFar
	adds r5, #1
	bl BattleEv_SetRuntimeField8
	mov r0, r11
	movs r1, #4
	bl UiText_DrawQuantity
	adds r0, r5, #0
.LCommand75:
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand8
.LCommand1:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #8
	ldrsh r0, [r3, r1]
	mov r11, r0
	bl Ability_GetData
	add r2, sp, #48
	adds r6, r0, #0
	mov r9, r2
	mov r0, r11
	bl BattleCommand_SelectTargets.0
	movs r3, #1
	negs r3, r3
	movs r5, #1
	cmp r0, r3
	bne .LCommand59
	bl .LCommand105
.LCommand59:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	movs r1, #4
	mov r0, r11
	bl UiText_DrawQuantity
	ldr r0, .LCommand168
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r2, [sp, #12]
	ldr r1, [r2]
	movs r3, #58
	ldrsh r2, [r1, r3]
	ldrb r3, [r6, #9]
	cmp r2, r3
	bge .LCommand60
	ldr r4, [sp, #8]
	ldr r2, [r4]
	movs r3, #2
	str r3, [r2, #92]
	movs r5, #0
.LCommand60:
	ldr r0, .LCommand169
	adds r3, r1, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .LCommand61
	ldr r1, [sp, #8]
	ldr r2, [r1]
	movs r3, #1
	str r3, [r2, #92]
	movs r5, #0
.LCommand61:
	cmp r5, #0
	bne .LCommand62
	b .LCommand8
.LCommand62:
	ldr r2, [sp, #8]
	ldr r3, [r2]
	movs r5, #0
	str r5, [r3, #92]
	ldr r3, [sp, #12]
	ldr r1, [r3]
	ldrb r2, [r6, #9]
	ldrh r3, [r1, #58]
	mov r4, r10
	subs r3, r3, r2
	strh r3, [r1, #58]
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Owner_RecalculateRatiosFar
	ldr r2, [sp, #12]
	ldr r1, [r2]
	movs r4, #58
	ldrsh r3, [r1, r4]
	cmp r3, #0
	bge .LCommand63
	strh r5, [r1, #58]
.LCommand63:
	movs r0, #58
	ldrsh r2, [r1, r0]
	movs r4, #54
	ldrsh r3, [r1, r4]
	ldrh r0, [r1, #54]
	cmp r2, r3
	bgt .LCommand64
	b .LCommand8
.LCommand64:
	strh r0, [r1, #58]
	b .LCommand8
.LCommand2:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #8
	ldrsh r2, [r3, r1]
	cmp r2, #0
	bge .LCommand65
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand170
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand48
.LCommand65:
	ldr r4, [sp, #12]
	lsls r2, r2, #1
	ldr r3, [r4]
	adds r2, #216
	ldrh r0, [r3, r2]
	bl Item_Get
	adds r5, r0, #0
	ldrh r0, [r5, #40]
	mov r11, r0
	cmp r0, #0
	beq .LCommand66
	ldr r1, [sp, #12]
	mov r3, r10
	ldr r2, [r1]
	ldr r1, [r3]
	movs r4, #8
	ldrsh r3, [r1, r4]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r2, [r2, r3]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .LCommand67
	b .LCommand68
.LCommand66:
	mov r0, r10
	ldr r1, [r0]
.LCommand68:
	movs r2, #0
	ldrsh r0, [r1, r2]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .LCommand171
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r4, [sp, #12]
	ldr r0, .LCommand172
	ldr r3, [r4]
	adds r2, r3, r0
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .LCommand69
	b .LCommand48
.LCommand69:
	movs r3, #1
	strb r3, [r2]
	b .LCommand48
.LCommand67:
	add r1, sp, #48
	mov r9, r1
	mov r0, r11
	bl BattleCommand_SelectTargets.0
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .LCommand70
	bl .LCommand105
.LCommand70:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r3, [sp, #12]
	mov r4, r10
	ldr r2, [r3]
	ldr r3, [r4]
	movs r0, #8
	ldrsh r3, [r3, r0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r2, r3]
	movs r1, #2
	bl UiText_DrawQuantity
	ldrb r3, [r5, #12]
	cmp r3, #2
	beq .LCommand71
	cmp r3, #0
	bne .LCommand72
.LCommand71:
	ldrb r0, [r5, #2]
	cmp r0, #3
	beq .LCommand73
	cmp r0, #3
	bgt .LCommand74
	cmp r0, #1
	beq .LCommand73
	b .LCommand72
.LCommand74:
	cmp r0, #8
	bgt .LCommand72
	cmp r0, #6
	blt .LCommand72
.LCommand73:
	ldr r0, .LCommand173
	b .LCommand75
.LCommand72:
	ldr r0, .LCommand174
	b .LCommand75
.LCommand3:
	mov r1, r10
	ldr r3, [r1]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl UiText_DrawQuantity
	ldr r0, .LCommand171
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand48
.LCommand4:
	mov r4, r10
	ldr r3, [r4]
	add r2, sp, #48
	movs r1, #8
	ldrsh r0, [r3, r1]
	mov r9, r2
	mov r11, r0
	bl BattleCommand_SelectTargets.0
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .LCommand76
	bl .LCommand105
.LCommand76:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r11
	movs r1, #4
	bl UiText_DrawQuantity
	mov r0, r11
	bl Ability_GetData
	ldrb r2, [r0, #1]
	movs r3, #15
	ands r3, r2
	cmp r3, #6
	bne .LCommand77
	ldr r0, .LCommand175
	b .LCommand78
.LCommand77:
	ldr r0, .LCommand176
.LCommand78:
	movs r3, #244
	lsls r3, r3, #1
	cmp r11, r3
	beq .LCommand79
	cmp r11, r3
	bgt .LCommand80
	ldr r2, .LCommand177
	cmp r11, r2
	bgt .LCommand81
	subs r3, #52
	cmp r11, r3
	bgt .LCommand82
	mov r4, r11
	cmp r4, #224
	beq .LCommand83
	cmp r4, #224
	bge .LCommand84
	b .LCommand75
.LCommand84:
	movs r1, #217
	lsls r1, r1, #1
	cmp r11, r1
	bgt .LCommand85
	b .LCommand75
.LCommand85:
	ldr r0, .LCommand178
	b .LCommand75
.LCommand81:
	movs r2, #222
	lsls r2, r2, #1
	cmp r11, r2
	ble .LCommand86
	movs r3, #236
	lsls r3, r3, #1
	cmp r11, r3
	beq .LCommand87
	b .LCommand75
.LCommand80:
	movs r3, #250
	lsls r3, r3, #1
	cmp r11, r3
	beq .LCommand88
	cmp r11, r3
	bgt .LCommand89
	subs r3, #6
	cmp r11, r3
	beq .LCommand90
	cmp r11, r3
	bgt .LCommand91
	movs r4, #246
	lsls r4, r4, #1
	cmp r11, r4
	beq .LCommand92
	b .LCommand75
.LCommand91:
	ldr r1, .LCommand179
	cmp r11, r1
	beq .LCommand93
	ldr r2, .LCommand180
	cmp r11, r2
	beq .LCommand94
	b .LCommand75
.LCommand89:
	ldr r3, .LCommand181
	cmp r11, r3
	beq .LCommand95
	cmp r11, r3
	bgt .LCommand96
	subs r3, #2
	cmp r11, r3
	beq .LCommand97
	b .LCommand75
.LCommand96:
	movs r4, #252
	lsls r4, r4, #1
	cmp r11, r4
	beq .LCommand98
	movs r1, #254
	lsls r1, r1, #1
	cmp r11, r1
	beq .LCommand99
	b .LCommand75
.LCommand83:
	ldr r0, .LCommand168
	b .LCommand75
.LCommand88:
	ldr r0, .LCommand182
	b .LCommand75
.LCommand97:
	ldr r0, .LCommand183
	b .LCommand75
.LCommand94:
	ldr r0, .LCommand184
	b .LCommand75
.LCommand90:
	ldr r0, .LCommand185
	b .LCommand75
.LCommand82:
	ldr r0, .LCommand186
	b .LCommand75
.LCommand86:
	ldr r0, .LCommand176
	b .LCommand75
.LCommand87:
	ldr r0, .LCommand187
	b .LCommand75
.LCommand79:
	ldr r0, .LCommand188
	b .LCommand75
.LCommand92:
	ldr r0, .LCommand189
	b .LCommand75
.LCommand93:
	ldr r0, .LCommand190
	b .LCommand75
.LCommand95:
	ldr r0, .LCommand191
	b .LCommand75
.LCommand98:
	ldr r0, .LCommand192
	b .LCommand75
.LCommand99:
	ldr r0, .LCommand193
	b .LCommand75
.LCommand167:
	.4byte MsgWeaponHowls
.LCommand168:
	.4byte MsgActorCasts
.LCommand169:
	.4byte 0x0000013d
.LCommand170:
	.4byte MsgItemAlreadyUsed
.LCommand171:
	.4byte MsgActorDefends
.LCommand172:
	.4byte 0x0000012b
.LCommand173:
	.4byte MsgActorRaisesItem
.LCommand174:
	.4byte MsgActorUsesBattleItem
.LCommand175:
	.4byte MsgActorUnleashesAbility
.LCommand176:
	.4byte MsgActorUsesAbility
.LCommand177:
	.4byte 0x000001b9
.LCommand178:
	.4byte MsgEmitsUltrasonicWaves
.LCommand179:
	.4byte 0x000001ef
.LCommand180:
	.4byte 0x000001f3
.LCommand181:
	.4byte 0x000001f7
.LCommand182:
	.4byte MsgGlowersFerociously
.LCommand183:
	.4byte MsgIsIntimidated
.LCommand184:
	.4byte MsgBalefulGaze
.LCommand185:
	.4byte MsgEatsWorms
.LCommand186:
	.4byte MsgLetsOutAbility
.LCommand187:
	.4byte MsgEnemyUnleashesAbility
.LCommand188:
	.4byte MsgGlowersMiserably
.LCommand189:
	.4byte MsgSmellOfDecay
.LCommand190:
	.4byte MsgFuriousRage
.LCommand191:
	.4byte MsgAttemptsToDivide
.LCommand192:
	.4byte MsgLooksForAllies
.LCommand193:
	.4byte MsgLooksForHelp
.LCommand5:
	mov r2, r10
	ldr r3, [r2]
	ldrh r3, [r3, #8]
	ldr r6, .LCommand194
	lsls r0, r3, #16
	movs r5, #255
	asrs r0, r0, #24
	adds r1, r5, #0
	ands r1, r3
	ands r0, r6
	bl Djinn_GetDefinitionHeaderFar
	mov r4, r10
	ldr r3, [r4]
	mov r11, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .LCommand100
	b .LCommand101
.LCommand100:
	b .LCommand102
	.balign 4, 0
.LCommand194:
	.4byte 0x0000000f
.LCommand102:
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .LCommand103
	b .LCommand104
.LCommand103:
	mov r0, r11
	bl Ability_GetData
	movs r1, #0
	movs r0, #0
	bl BattlePres_SetActorModes
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r2, r3
	ands r1, r6
	bl Djinn_ActivateFar
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl Trade_RemoveOfferFar
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Owner_RecalculateStatsFar
	bl BattleEventRuntime_Reset
	movs r0, #30
	bl BattleEventRuntime_SchedulePhase
	mov r2, r10
	ldr r3, [r2]
	movs r0, #0
	movs r4, #0
	ldrsh r1, [r3, r4]
	bl BattleEv_Push
	mov r0, r10
	ldr r3, [r0]
	ldrh r2, [r3, #8]
	lsls r3, r2, #16
	asrs r3, r3, #24
	ands r3, r6
	lsls r1, r3, #2
	adds r1, r1, r3
	adds r3, r5, #0
	ands r3, r2
	lsls r1, r1, #2
	movs r2, #150
	lsls r2, r2, #1
	adds r1, r1, r3
	adds r1, r1, r2
	movs r0, #3
	bl BattleEv_Push
	movs r1, #175
	movs r0, #14
	bl BattleEv_Push
	movs r1, #0
	movs r0, #10
	bl BattleEv_Push
	ldr r1, .LCommand195
	movs r0, #4
	bl BattleEv_Push
	mov r4, r10
	ldr r3, [r4]
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #11
	bl BattleEv_Push
	movs r0, #212
	bl AudioCommand_PlayFar
	mov r1, r10
	ldr r3, [r1]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Object_SetMode
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r2, r10
	ldr r3, [r2]
	ldrh r1, [r3, #8]
	lsls r1, r1, #16
	asrs r1, r1, #24
	movs r4, #0
	ldrsh r0, [r3, r4]
	ands r1, r6
	movs r2, #3
	movs r3, #0
	bl BattleFx_PlayUnitElementEffect
	bl BattleEventRuntime_WaitForReady
.LCommand7:
	movs r0, #2
	negs r0, r0
	b .LCommand105
.LCommand104:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	movs r1, #4
	mov r0, r11
	bl UiText_DrawQuantity
	movs r0, #114
	bl AudioCommand_PlayFar
	ldr r0, .LCommand196
	bl UiText_ShowMessageAndWaitCoreFar
	movs r0, #60
	bl WaitFrames
.LCommand48:
	movs r0, #1
	negs r0, r0
	b .LCommand105
.LCommand101:
	add r2, sp, #48
	mov r9, r2
	mov r0, r11
	bl BattleCommand_SelectTargets.0
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .LCommand106
	b .LCommand105
.LCommand106:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	adds r2, r5, #0
	asrs r1, r1, #24
	ands r2, r3
	ands r1, r6
	bl Trade_AddOfferFar
	mov r0, r11
	bl Ability_GetData
	mov r2, r10
	ldr r3, [r2]
	adds r5, r0, #0
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiText_DrawQuantity
	mov r0, r11
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .LCommand197
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r0, [sp, #8]
	ldrb r3, [r5, #2]
	ldr r2, [r0]
	str r3, [r2, #80]
	b .LCommand8
.LCommand6:
	mov r1, r10
	ldr r3, [r1]
	movs r2, #8
	ldrsh r0, [r3, r2]
	bl SummonDefinition_Get
	mov r4, r10
	movs r2, #24
	ldr r3, [r4]
	add r2, sp
	mov r8, r2
	mov r9, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	mov r1, r8
	bl BattlePlacement_CountValidEntries
	mov r4, r10
	ldr r3, [r4]
	ldrh r3, [r3]
	movs r0, #0
	cmp r3, #7
	bls .LCommand107
	movs r0, #1
.LCommand107:
	bl Trade_GetOfferStateFar
	adds r0, #8
	str r0, [sp, #4]
	mov r1, r9
	adds r1, #4
	mov r0, r8
	ldrb r2, [r0]
	ldrb r3, [r1]
	movs r7, #0
	cmp r2, r3
	bcc .LCommand108
	movs r5, #4
	mov r6, r8
	movs r4, #4
.LCommand109:
	mov r2, r9
	ldrb r3, [r2, r5]
	adds r7, #1
	strb r3, [r0]
	adds r4, #1
	adds r0, #1
	cmp r7, #3
	bgt .LCommand108
	adds r6, #1
	adds r1, #1
	ldrb r2, [r6]
	ldrb r3, [r1]
	adds r5, r4, #0
	cmp r2, r3
	bcs .LCommand109
.LCommand108:
	mov r3, r9
	ldrh r3, [r3]
	add r4, sp, #48
	mov r11, r3
	mov r9, r4
	mov r0, r11
	bl BattleCommand_SelectTargets.0
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .LCommand110
	b .LCommand105
.LCommand110:
	cmp r7, #4
	beq .LCommand111
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl UiText_DrawQuantity
	mov r0, r11
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .LCommand198
	bl UiText_ShowMessageAndWaitCoreFar
	adds r0, r5, #0
	b .LCommand105
.LCommand111:
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiText_DrawQuantity
	movs r1, #4
	mov r0, r11
	bl UiText_DrawQuantity
	ldr r0, .LCommand199
	bl UiText_ShowMessageAndWaitCoreFar
	movs r1, #128
	ldr r0, [sp, #4]
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	movs r7, #0
	cmp r3, #0
	beq .LCommand8
	mov r9, r5
	adds r5, r0, #0
.LCommand113:
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r9
	bne .LCommand112
	ldrb r0, [r5, #2]
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .LCommand112
	ldrb r1, [r5]
	mov r3, r8
	ldrb r2, [r3, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .LCommand112
	movs r3, #254
	strb r3, [r5, #3]
	adds r3, r2, #0
	adds r3, #255
	mov r4, r8
	strb r3, [r4, r1]
.LCommand112:
	ldr r0, [sp, #4]
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	adds r7, #1
	adds r5, #4
	cmp r7, r3
	bne .LCommand113
.LCommand8:
	mov r2, r11
	cmp r2, #1
	beq .LCommand58
	b .LCommand114
.LCommand58:
	ldr r4, [sp, #8]
	ldr r3, [r4]
	ldrb r0, [r3, #2]
	bl Owner_GetStateFar
	adds r6, r0, #0
	ldr r0, [sp, #8]
	ldr r2, [r0]
	mov r1, r10
	movs r3, #1
	str r3, [r2, #76]
	ldr r3, [r1]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Item_GetEquippedElementFar
	ldr r3, [sp, #8]
	ldr r1, [r3]
	movs r3, #2
	str r0, [r1, #80]
	str r3, [r1, #84]
	ldr r4, [sp, #12]
	ldr r0, .LCommand200
	ldr r2, [r4]
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .LCommand115
	movs r1, #148
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r0, [r3]
	bl Battle_GetEntryField2LowBits
	ldr r3, [sp, #8]
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #7
	orrs r3, r0
	b .LCommand116
.LCommand115:
	movs r3, #0
	movs r4, #148
	str r3, [r1, #88]
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #5
	bhi .LCommand14
	ldr r2, .LCommand201
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.LCommand150:
	.4byte .LCommand10
	.4byte .LCommand11
	.4byte .LCommand12
	.4byte .LCommand13
	.4byte .LCommand14
	.4byte .LCommand10
.LCommand11:
	ldr r1, [sp, #8]
	ldr r3, .LCommand202
	ldr r2, [r1]
	b .LCommand116
.LCommand12:
	ldr r3, [sp, #8]
	ldr r2, [r3]
	ldr r3, .LCommand203
	b .LCommand116
.LCommand13:
	ldr r4, [sp, #8]
	ldr r3, .LCommand203
	ldr r2, [r4]
	b .LCommand116
.LCommand10:
	ldr r0, [sp, #8]
	ldr r3, .LCommand202
	ldr r2, [r0]
.LCommand116:
	str r3, [r2, #88]
.LCommand14:
	mov r1, r10
	ldr r3, [r1]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl UiText_DrawQuantity
	ldr r0, .LCommand204
	bl UiText_ShowMessageAndWaitCoreFar
	b .LCommand117
.LCommand120:
	ldr r4, [sp, #12]
	movs r0, #156
	ldr r3, [r4]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .LCommand118
	bl BattleRandom16Far
	movs r3, #255
	ands r0, r3
	cmp r0, #152
	bgt .LCommand118
	ldr r1, [sp, #8]
	ldr r3, [r1]
	strb r5, [r3, #30]
.LCommand118:
	bl BattleRandom16Far
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .LCommand119
	ldr r2, [sp, #8]
	ldr r3, [r2]
	strb r0, [r3, #30]
	b .LCommand119
.LCommand117:
	movs r4, #56
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .LCommand119
	movs r0, #158
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .LCommand119
	ldr r1, .LCommand205
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .LCommand119
	ldr r2, .LCommand206
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .LCommand119
	movs r4, #157
	lsls r4, r4, #1
	adds r3, r6, r4
	ldrb r5, [r3]
	cmp r5, #0
	beq .LCommand120
.LCommand119:
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .LCommand121
	ldr r0, [sp, #8]
	ldr r2, [r0]
	movs r3, #0
	strb r3, [r2, #30]
.LCommand121:
	movs r1, #56
	ldrsh r3, [r6, r1]
	cmp r3, #0
	bne .LCommand122
	b .LCommand123
.LCommand122:
	bl BattleRandom16Far
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .LCommand124
	ldr r2, [sp, #8]
	ldr r3, [r2]
	b .LCommand125
.LCommand195:
	.4byte MsgDjinnSet
.LCommand196:
	.4byte MsgDjinnInRecovery
.LCommand197:
	.4byte MsgActorUnleashesDjinn
.LCommand198:
	.4byte MsgSummonLacksDjinn
.LCommand199:
	.4byte MsgActorSummons
.LCommand200:
	.4byte 0x00000129
.LCommand201:
	.4byte .LCommand150
.LCommand202:
	.4byte 0x00004001
.LCommand203:
	.4byte 0x00004004
.LCommand204:
	.4byte MsgActorAttacks
.LCommand205:
	.4byte 0x0000013b
.LCommand206:
	.4byte 0x00000145
.LCommand124:
	ldr r3, [sp, #12]
	ldr r0, [r3]
	bl Equipment_GetUnleashRateBonusFar
	movs r1, #200
	lsls r0, r0, #16
	bl __divsi3
	adds r5, r0, #0
	bl BattleRandom16Far
	ldr r3, .LCommand207
	ands r0, r3
	cmp r5, r0
	bgt .LCommand126
	b .LCommand123
.LCommand126:
	ldr r4, [sp, #8]
	ldr r3, [r4]
.LCommand125:
	movs r2, #1
	adds r3, #44
	strb r2, [r3]
	b .LCommand123
.LCommand114:
	mov r0, r11
	bl Ability_GetData
	adds r7, r0, #0
	ldr r0, [sp, #8]
	ldrb r2, [r7, #2]
	ldr r3, [r0]
	mov r1, r11
	str r2, [r3, #80]
	movs r2, #0
	str r2, [r3, #88]
	str r1, [r3, #76]
	ldrb r3, [r7, #3]
	adds r2, r3, #0
	cmp r2, #65
	beq .LCommand127
	cmp r2, #41
	beq .LCommand128
	cmp r2, #42
	beq .LCommand128
	cmp r2, #43
	beq .LCommand128
	cmp r2, #44
	beq .LCommand128
	cmp r2, #68
	bne .LCommand129
.LCommand128:
	adds r2, r3, #0
	cmp r2, #65
	beq .LCommand127
	cmp r2, #68
	bne .LCommand130
.LCommand127:
	movs r5, #153
	b .LCommand131
.LCommand130:
	cmp r2, #41
	beq .LCommand132
	movs r5, #64
	cmp r2, #43
	bne .LCommand131
.LCommand132:
	movs r5, #32
.LCommand131:
	cmp r3, #65
	beq .LCommand133
	cmp r3, #41
	beq .LCommand133
	movs r6, #2
	cmp r3, #42
	bne .LCommand134
.LCommand133:
	movs r6, #1
.LCommand134:
	bl BattleRandom16Far
	movs r3, #255
	ands r0, r3
	cmp r0, r5
	bge .LCommand135
	ldr r3, [sp, #8]
	ldr r2, [r3]
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .LCommand135
	adds r1, r2, #0
	adds r2, #30
.LCommand136:
	ldrb r3, [r2]
	adds r3, r3, r6
	strb r3, [r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r0, #1
	adds r2, #1
	cmp r0, r3
	blt .LCommand136
	b .LCommand135
.LCommand129:
	adds r3, #220
	movs r4, #128
	lsls r3, r3, #24
	lsls r4, r4, #19
	cmp r3, r4
	bhi .LCommand137
	ldrb r3, [r7, #3]
	subs r3, #36
	cmp r3, #4
	bhi .LCommand19
	ldr r2, .LCommand208
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.LCommand151:
	.4byte .LCommand15
	.4byte .LCommand16
	.4byte .LCommand17
	.4byte .LCommand18
	.4byte .LCommand19
.LCommand15:
	movs r5, #63
	b .LCommand138
.LCommand16:
	movs r5, #31
	b .LCommand138
.LCommand17:
	movs r5, #15
	b .LCommand138
.LCommand18:
	movs r5, #7
	b .LCommand138
.LCommand19:
	movs r5, #3
.LCommand138:
	bl BattleRandom16Far
	ands r0, r5
	cmp r0, #0
	bne .LCommand135
	ldr r1, [sp, #8]
	ldr r2, [r1]
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .LCommand135
	adds r1, r2, #0
	movs r4, #2
	adds r2, #44
.LCommand139:
	strb r4, [r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r0, #1
	adds r2, #1
	cmp r0, r3
	blt .LCommand139
	b .LCommand135
.LCommand137:
	mov r2, r11
	cmp r2, #178
	bne .LCommand135
	ldr r5, [sp, #8]
	ldr r3, [r5]
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r6, #0
	cmp r6, r3
	bge .LCommand135
.LCommand140:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r3, [r5]
	adds r3, #2
	ldrb r1, [r3, r6]
	ldrb r2, [r7, #2]
	ldrb r3, [r7, #3]
	movs r4, #100
	str r4, [sp, #0]
	bl Battle_HitCheck
	ldr r1, [r5]
	adds r2, r6, #0
	adds r3, r1, #2
	adds r2, #56
	strb r0, [r3, r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r6, #1
	cmp r6, r3
	blt .LCommand140
.LCommand135:
	ldr r2, .LCommand209
	cmp r11, r2
	bhi .LCommand141
	ldr r3, [sp, #8]
	ldr r2, .LCommand210
	ldr r1, [r3]
	mov r4, r11
	lsls r3, r4, #2
	ldr r2, [r2, r3]
	movs r3, #30
	ldrsb r3, [r1, r3]
	str r2, [r1, #88]
	cmp r3, #1
	ble .LCommand141
	lsls r3, r3, #12
	ldr r0, .LCommand211
	adds r3, r2, r3
	adds r3, r3, r0
	str r3, [r1, #88]
.LCommand141:
	ldr r1, .LCommand212
	cmp r11, r1
	bhi .LCommand142
	ldr r1, .LCommand213
	mov r2, r11
	ldrb r3, [r1, r2]
	cmp r3, #0
	beq .LCommand142
	ldr r3, [sp, #8]
	mov r4, r11
	ldr r2, [r3]
	ldrb r3, [r1, r4]
	b .LCommand143
.LCommand142:
	mov r0, r11
	bl Ability_CheckStatusOrSpecialId
	cmp r0, #0
	beq .LCommand144
	ldr r0, [sp, #8]
	ldr r2, [r0]
	movs r3, #3
	b .LCommand143
.LCommand144:
	ldr r1, [sp, #8]
	ldr r2, [r1]
	ldr r3, [r2, #88]
	cmp r3, #0
	beq .LCommand145
	ldr r4, [sp, #12]
	ldr r0, .LCommand214
	ldr r3, [r4]
	adds r3, r3, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .LCommand146
	movs r3, #8
	b .LCommand143
.LCommand146:
	movs r3, #3
	b .LCommand143
.LCommand145:
	movs r3, #1
.LCommand143:
	str r3, [r2, #84]
	ldrb r0, [r7, #3]
	bl BattleFx_IsReviveFar
	cmp r0, #0
	beq .LCommand147
	ldr r1, [sp, #8]
	ldr r3, [r1]
	movs r1, #128
	ldr r2, [r3, #88]
	lsls r1, r1, #9
	orrs r2, r1
	str r2, [r3, #88]
.LCommand147:
	mov r2, r11
	cmp r2, #178
	bne .LCommand123
	ldr r3, [sp, #8]
	ldr r1, [r3]
	adds r3, r1, #0
	adds r3, #58
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .LCommand123
	ldr r3, [r1, #88]
	movs r2, #128
	lsls r2, r2, #5
	orrs r3, r2
	str r3, [r1, #88]
.LCommand123:
	mov r4, r10
	ldr r3, [r4]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #2
	bne .LCommand148
	ldr r1, [sp, #8]
	ldr r2, [r1]
	ldr r3, [r2, #84]
	cmp r3, #5
	beq .LCommand148
	cmp r3, #9
	beq .LCommand148
	movs r3, #4
	str r3, [r2, #84]
.LCommand148:
	ldr r2, [sp, #8]
	mov r4, r10
	ldr r3, [r2]
	ldr r2, [r4]
	ldrh r2, [r2, #6]
	adds r3, #72
	strh r2, [r3]
.LCommand215:
	movs r0, #0
.LCommand105:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.LCommand207:
	.4byte 0x0000ffff
.LCommand208:
	.4byte .LCommand151
.LCommand209:
	.4byte 0x00000206
.LCommand210:
	.4byte Battle_ActionFlags
.LCommand211:
	.4byte 0xfffff000
.LCommand212:
	.4byte 0x00000205
.LCommand213:
	.4byte Battle_ActionStatus
.LCommand214:
	.4byte 0x00000129
	.size BattleCommand_BuildPlan, .-BattleCommand_BuildPlan
