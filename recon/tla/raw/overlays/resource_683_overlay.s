.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r1, .L_0200807c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008080
	cmp r2, r3
	bne .L_02008078
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_02008078
	ldr r0, .L_02008084
	b .L_0200807a
.L_02008078:
	ldr r0, .L_02008088
.L_0200807a:
	pop {pc}
.L_0200807c:
	.4byte gPartyState
.L_02008080:
	.4byte 0x000000b7
.L_02008084:
	.4byte Data_020003a8
.L_02008088:
	.4byte Data_02000228
	.section .text.x0200808c,"ax",%progbits
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	adds r1, r0, #0
	movs r0, #25
	bl Func_02000178
	pop {pc}
	.section .text.x02008098,"ax",%progbits
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {lr}
	adds r1, r0, #0
	movs r0, #10
	bl Func_02000180
	pop {pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {lr}
	ldr r1, .L_020080e8
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020080ec
	cmp r2, r3
	bne .L_020080d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_020080d0
	ldr r0, .L_020080f0
	b .L_020080e4
.L_020080d0:
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080e2
	ldr r0, .L_020080f4
	b .L_020080e4
.L_020080e2:
	ldr r0, .L_020080f8
.L_020080e4:
	pop {pc}
	.2byte 0x0000
.L_020080e8:
	.4byte gPartyState
.L_020080ec:
	.4byte 0x000000b7
.L_020080f0:
	.4byte Data_02000780
.L_020080f4:
	.4byte Data_020005ac
.L_020080f8:
	.4byte Data_020003d8
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	.ifdef TLA_EDITION_JA
	movs r2, #129
	.else
	movs r2, #133
	.endif
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	ldr r1, .L_02008140
	str r2, [r3]
	.ifdef TLA_EDITION_JA
	subs r2, #33
	.else
	subs r2, #41
	.endif
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008144
	cmp r2, r3
	bne .L_0200813a
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_0200813a
	bl Func_0200014c
.L_0200813a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008140:
	.4byte gPartyState
.L_02008144:
	.4byte 0x000000b7
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #48
	adds r2, #93
	str r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.section .rodata.x02008188,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000001a0
	.4byte 0xc0000298
	.4byte 0x01200000
	.4byte 0x02100218
	.4byte 0x000002b8
	.4byte 0xffff0009
	.4byte 0x00000260
	.4byte 0xc0000298
	.4byte 0x01e80000
	.4byte 0x02d80210
	.4byte 0x000002c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000b7
	.4byte 0x101010b6
	.4byte 0xffffffff
	.4byte 0x102020b6
	.4byte 0xffffffff
	.4byte 0x103030b6
	.4byte 0xffffffff
	.4byte 0x104040b6
	.4byte 0xffffffff
	.4byte 0x105050b6
	.4byte 0xffffffff
	.4byte 0x106060b6
	.4byte 0xffffffff
	.4byte 0x0080b086
	.4byte 0x00902083
	.4byte 0x000001ff
	.global Data_02000228
Data_02000228:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00015000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00010000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001d000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00018000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0000c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00015000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00018000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00015000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x005b0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020003a8
Data_020003a8:
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020003d8
Data_020003d8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldThereSNotMuchSpace
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldEveryoneInChampaIsPoor
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldWhenMyFatherHeadsOut
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte MsgFieldMyDadWentOutTo
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldTheseRaftsAreAllWe
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldIfDadHadnTBeen
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIHopeDadMakesIt
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldFatherWorriesSoMuchAbout
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldTheTidalWaveWasTerrible
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldItLookedToMeLike
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldMostOfTheSailorsHere
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldTheFishAreVanishingI
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldSomethingMustHaveBlownUp
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldYouHaveToBeOne
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldWeVeSeenALot
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldAGroupOfTravelersCame
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldMoreVisitorsMeansMoreMoney
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldNowThatIThinkAbout
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldTheOceanHasBeenA
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldIVeHeardRumorsOf
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldWeChampaRelyEntirelyOn
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldAllTheSailorsAreSaying
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte Func_0200008c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldIHaveSomeLovelyArtifacts
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldIfYouGoToThe
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldOurProductsCostWhatThey
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldTheStuffInTheRuins
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldListenIfYouCameTo
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldWeUsedToDoGood
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldItSAShameThat
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldWatchingOurCatchesDwindleWith
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020005ac
Data_020005ac:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldWeDonTNeedTo
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldItSTrueItS
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldFatherSActingLikeA
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte MsgFieldWhenBriggsReturnedWithThe
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldIDRatherSpendMy
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldBriggsIsAHeroFor
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIfFatherSitsAroundThe
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldQuittingTheirJobsMakesThe
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldIHearThatBriggsSold
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldWhileIMGratefulFor
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldBriggsReallySavedOurHides
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldTheNerveOfThatBriggs
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldIShouldJustSignUp
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldIRefuseToLiveIn
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldIVeBeenWonderingAbout
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldWeVillageEldersHaveGuided
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldANewImageForChampa
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldObservingBriggsRecentlyTheLimits
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldItSeemsTheEldersAre
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldIForOneThinkA
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldSureBriggsSavedTheVillage
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldSupportingAnEntireVillageThrough
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte Func_0200008c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldTheJewelsBriggsFoundAre
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldThanksToBriggsTheChampa
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldBriggsSMenSaidThey
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldNowSTheTimeWe
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldWeStillHavenTBeen
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldItSNiceNotHaving
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldMostPeopleHereWouldLove
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldTheOnlyWayToFind
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000780
Data_02000780:
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldDidYouKnowIndraMoved
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldIMSoGratefulOur
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
