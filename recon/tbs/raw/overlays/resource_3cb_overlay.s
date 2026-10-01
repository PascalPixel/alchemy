.syntax unified
	.thumb
	.section .text.x02008580,"ax",%progbits
	.global LinkLobby_SendPartyRecords
	.thumb_func
LinkLobby_SendPartyRecords:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #170
	lsls r5, r5, #1
	adds r0, r5, #0
	sub sp, #32
	bl Runtime_BumpAllocateAlternatePool
	add r5, sp, #16
	movs r1, #0
	mov r8, r0
	adds r0, r5, #0
	str r1, [sp, #4]
	bl SceneData_CopyUpToThreeEntries
	mov r2, sp
	adds r2, #8
	str r2, [sp, #0]
	ldr r1, [sp, #0]
	movs r6, #150
	mov r3, sp
	lsls r6, r6, #2
	mov r11, r0
	movs r2, #0
	adds r3, #15
	mov r12, r1
.L_020085be:
	strb r2, [r3]
	subs r3, #1
	cmp r3, r12
	bge .L_020085be
	movs r7, #0
	cmp r7, r11
	bge .L_0200868a
	movs r2, #0
	mov r9, r5
	mov r10, r2
.L_020085d2:
	mov r3, r10
	mov r1, r9
	ldrh r0, [r3, r1]
	bl Owner_GetState
	movs r2, #170
	adds r1, r0, #0
	lsls r2, r2, #1
	ldr r3, .L_020087a8
	mov r0, r8
	bl _call_via_r3
	movs r2, #149
	lsls r2, r2, #1
	add r2, r8
	movs r3, #2
	strb r3, [r2]
	mov r1, r10
	mov r3, r9
	ldrh r2, [r1, r3]
	ldr r1, [sp, #0]
	adds r3, r7, #0
	subs r3, #128
	strb r3, [r1, r2]
	movs r1, #170
	mov r0, r8
	lsls r1, r1, #1
	bl SerialRuntime_BeginTransferA
	movs r2, #1
	negs r2, r2
	movs r5, #0
	cmp r0, r2
	bne .L_0200863a
	str r0, [sp, #4]
	b .L_0200878e
.L_0200861a:
	movs r0, #1
	subs r6, #1
	bl Engine_TaskWait
	cmp r6, #0
	blt .L_02008632
	ldr r3, .L_020087ac
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0200863a
.L_02008632:
	adds r5, #1
	cmp r5, #24
	ble .L_0200863a
	b .L_02008772
.L_0200863a:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_0200861a
	movs r0, #2
	bl Engine_TaskWait
	adds r7, #1
	movs r1, #2
	add r10, r1
	cmp r7, r11
	blt .L_020085d2
	b .L_0200868a
.L_02008654:
	movs r0, #1
	subs r6, #1
	bl Engine_TaskWait
	cmp r6, #0
	blt .L_0200866c
	ldr r3, .L_020087ac
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0200867a
.L_0200866c:
	adds r5, #1
	cmp r5, #24
	ble .L_0200867a
	movs r2, #1
	negs r2, r2
	str r2, [sp, #4]
	b .L_0200878e
.L_0200867a:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_02008654
	movs r0, #2
	bl Engine_TaskWait
	adds r7, #1
.L_0200868a:
	cmp r7, #2
	bgt .L_020086ae
	movs r3, #149
	lsls r3, r3, #1
	add r3, r8
	movs r5, #0
	movs r1, #170
	strb r5, [r3]
	mov r0, r8
	lsls r1, r1, #1
	bl SerialRuntime_BeginTransferA
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0200867a
	str r0, [sp, #4]
	b .L_0200878e
.L_020086ae:
	movs r5, #160
	mov r0, r8
	lsls r5, r5, #1
	bl Runtime_BumpFree
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #0
	bl Trade_GetOfferState
	ldr r3, .L_020087a8
	adds r1, r0, #0
	adds r2, r5, #0
	mov r0, r8
	bl _call_via_r3
	mov r5, r8
	movs r2, #132
	lsls r2, r2, #1
	add r2, r8
	movs r1, #0
	ldr r3, [r2]
	mov r10, r1
	movs r7, #150
	movs r1, #128
	adds r5, #8
	lsls r7, r7, #2
	movs r4, #0
	lsls r1, r1, #1
	cmp r10, r3
	bge .L_0200873a
	ldr r3, [sp, #0]
	adds r6, r2, #0
	mov r12, r3
	adds r0, r5, #0
.L_020086f8:
	ldrb r3, [r0, #2]
	mov r2, r12
	ldrb r3, [r2, r3]
	strb r3, [r0, #2]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200872c
	ldr r3, [r6]
	subs r3, #1
	cmp r4, r3
	bge .L_02008722
	ldr r2, [r5, r1]
	lsls r3, r4, #2
	subs r2, #1
	adds r1, r3, r5
	subs r2, r2, r4
.L_02008718:
	ldr r3, [r1, #4]
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bne .L_02008718
.L_02008722:
	ldr r3, [r6]
	subs r3, #1
	str r3, [r6]
	subs r0, #4
	subs r4, #1
.L_0200872c:
	movs r1, #128
	lsls r1, r1, #1
	ldr r3, [r5, r1]
	adds r4, #1
	adds r0, #4
	cmp r4, r3
	blt .L_020086f8
.L_0200873a:
	movs r1, #160
	mov r0, r8
	lsls r1, r1, #1
	bl SerialRuntime_BeginTransferA
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0200877a
	str r0, [sp, #4]
	b .L_0200878e
.L_02008750:
	movs r0, #1
	subs r7, #1
	bl Engine_TaskWait
	cmp r7, #0
	blt .L_02008768
	ldr r3, .L_020087ac
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0200877a
.L_02008768:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #24
	ble .L_0200877a
.L_02008772:
	movs r3, #1
	negs r3, r3
	str r3, [sp, #4]
	b .L_0200878e
.L_0200877a:
	bl SerialRuntime_GetActiveTransfers
	cmp r0, #0
	bne .L_02008750
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #2
	bl Engine_TaskWait
.L_0200878e:
	mov r0, r8
	bl Runtime_BumpFree
	ldr r0, [sp, #4]
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_020087a8:
	.4byte IwramCopyWords
.L_020087ac:
	.4byte gLinkStatus
	.section .rodata.x02009928,"a",%progbits
	.global LinkLobby_SlotColumns
LinkLobby_SlotColumns:
	.4byte 0x01010100
	.4byte 0x00000001
	.global gLinkLobbyEntrances
gLinkLobbyEntrances:
	.4byte 0xffff000b
	.4byte 0x00000080
	.4byte 0xc00000a4
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff000a
	.4byte 0x00000160
	.4byte 0xc0000094
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0008
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000120
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0000
	.4byte 0x000000d8
	.4byte 0xc00000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gLinkLobbyExits
gLinkLobbyExits:
	.4byte 0x000001ff
	.global gLinkLobbyPlacements
gLinkLobbyPlacements:
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00010000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00010000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00010000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00018000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gLinkLobbyBattlePlacements
gLinkLobbyBattlePlacements:
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00010000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00010000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x0001c000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00010000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00018000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gLinkLobbyEvents
gLinkLobbyEvents:
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte LinkLobby_TalkByProgress
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte LinkLobby_TalkByProgress
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte LinkLobby_TalkByProgress
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte LinkLobby_TalkByProgress
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte LinkLobby_RunBattleApplication
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte LinkLobby_TalkLinkedWins
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte State_ApplyValueAndGetResult
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldWelcomeToBattleArenaWherePeople
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte LinkLobby_TalkToAttendant
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte LinkLobby_TalkToAttendant
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte LinkLobby_TalkToAttendant
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte LinkLobby_RunChoicePrompt
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte LinkLobby_TalkAlternating
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldInTheLinkedFinalsYouCant
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldToUseSummonQuicklyDjinnMust
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldDoYouPutDjinnOnStandby
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldRemoveDjinnYouRarelyUseSo
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldMonsterBattleRulesSayYouAll
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte LinkLobby_ShowCountMessage
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFieldAllTheWarriorsAreImpressiveSo
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgFieldICouldEnterButIdJust
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte LinkLobby_RunConnectionSequence
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte LinkLobby_StartExchange
	.4byte 0x00000006
	.4byte 0xffff0001
	.4byte LinkLobby_CallIntoCircle
	.4byte 0x00000006
	.4byte 0xffff0002
	.4byte Scene_ClearFlagsAndPlayCue2927
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
