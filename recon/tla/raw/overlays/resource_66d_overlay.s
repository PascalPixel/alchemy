.syntax unified
	.thumb
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #56
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_020080b4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #10
	beq .L_020080b0
	movs r3, #0
	strb r3, [r2]
.L_020080b0:
	pop {pc}
	.2byte 0x0000
.L_020080b4:
	.4byte gPartyState
	.section .text.x02008368,"ax",%progbits
	.global Func_02000368
	.thumb_func
Func_02000368:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	movs r5, #8
.L_0200837c:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200838e
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200838e:
	adds r5, #1
	cmp r5, #63
	bls .L_0200837c
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	bl Func_020005f4
	subs r5, #4
	ldr r0, .L_020083f8
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02000534
	ldr r5, .L_020083fc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020005a4
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02000554
	pop {r5, r6, pc}
.L_020083f8:
	.4byte Data_0200064c
.L_020083fc:
	.4byte gPartyState
	.section .rodata.x020085fc,"a",%progbits
	.4byte 0x0200001a
	.4byte 0x0000ffff
.L_02008604:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
.L_02008610:
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00180000
	.4byte 0x00000011
.L_02008634:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000011
.L_02008640:
	.4byte 0x002a0008
	.4byte 0x00020001
	.4byte 0xffff0006
	.global Data_0200064c
Data_0200064c:
	.4byte .L_02008640
	.4byte 0x00330012
	.4byte .L_02008640
	.4byte 0x00220015
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000e0
	.4byte 0x400000be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000069
	.4byte 0x1010106a
	.4byte 0xffffffff
	.4byte 0x1020206a
	.4byte 0xffffffff
	.4byte 0x1030306a
	.4byte 0xffffffff
	.4byte 0x1040406a
	.4byte 0xffffffff
	.4byte 0x1050506a
	.4byte 0xffffffff
	.4byte 0x10617002
	.4byte 0xffffffff
	.4byte 0x10718002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020006cc
Data_020006cc:
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte .L_02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020008c4
Data_020008c4:
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000954
Data_02000954:
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte .L_02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000b4c
Data_02000b4c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000368
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000368
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldWelcomeToNaribweItS
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_020000d8
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldIWouldNotVentureFar
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldItSBeenDangerousOut
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldWhenOeiaWasTheirWitch
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000140
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001a8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldOnTheFarSideOf
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldWhenIClimbedTheKibombo
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020001f0
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_020002b0
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldICanHardlyStandThe
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldAkafubuIsAlmostCertainlyLooking
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldItIsAwfulToThe
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldThatWasTheDayThe
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldKibomboMagicWasOnceWidely
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldTheWarriorWhoPassedThrough
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldToGetToKibomboYou
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldSometimesYouCanHearExplosions
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldIRememberWhenIFound
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldEvenIfIWantedMy
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgFieldAnywhereICanDoSome
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.2byte MsgFieldThereSOnlyALittle
	.2byte 0x0040
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte Func_02000088
	.4byte 0x00004e15
	.4byte 0x02010017
	.4byte Func_02000400
	.4byte 0x00008515
	.4byte 0x02020018
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000448
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000464
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000cfc
Data_02000cfc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000368
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000368
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldDonTTellMeYou
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldIDidnTSeeAny
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldISawSmokeRisingFrom
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldSomeSoldiersCameThroughHere
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldTheSmokeComingFromThe
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldTheFortuneTellerPredictedThat
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldAManCameThroughA
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldIMentionedMagmaRockTo
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldOuchSomeBigBruteCame
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000250
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200030c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldWhoWouldTravelToKibombo
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldIMSoGladThe
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldThatSmokeSBeenRising
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldWhatKindOfAName
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldTheSignalsSaySomethingHas
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldTheFortuneTellerHasNever
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldIfTheManTheyWere
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldIDonTThinkAnyone
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldWhatHasGoneWrongWith
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldThoseWhoBecomeLostShould
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgFieldTheColdOfTundariaStifles
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.2byte MsgFieldThereSOnlyALittle
	.2byte 0x0040
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte Func_02000088
	.4byte 0x00004e15
	.4byte 0x02010017
	.4byte Func_02000400
	.4byte 0x00008515
	.4byte 0x02020018
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000448
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000464
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
