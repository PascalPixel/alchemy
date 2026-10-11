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
	.section .rodata.x02008188,"a",%progbits
	.balign 4
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
	.4byte MsgThereSNot
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgEveryoneInChampa
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgWhenMyFather
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte MsgMyDadWent
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgTheseRaftsAre
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgIfDadHadn
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgIHopeDad
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFatherWorriesSo
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgTheTidalWave
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgItLookedTo
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgMostOfThe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgTheFishAre
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgSomethingMustHave
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgYouHaveTo
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgWeVeSeen
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgAGroupOf
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgMoreVisitorsMeans
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgNowThatI
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgTheOceanHas
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgIVeHeardRumors
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgWeChampaRely
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgAllTheSailors
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte Func_0200008c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgIHaveSome
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgIfYouGo
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgOurProductsCost
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgTheStuffIn
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgListenIfYou
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgWeUsedTo
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgItSA
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgWatchingOurCatches
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
	.4byte MsgWeDonT
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgItSTrue
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFatherSActing
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte MsgWhenBriggsReturned
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgIDRather
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgBriggsIsA
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgIfFatherSits
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgQuittingTheirJobs
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgIHearThat
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgWhileIM
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgBriggsReallySaved
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgTheNerveOf
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgIShouldJust
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgIRefuseTo
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgIVeBeenWondering
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgWeVillageElders
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgANewImage
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgObservingBriggsRecently
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgItSeemsThe
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgIForOne
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgSureBriggsSaved
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgSupportingAnEntire
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte Func_0200008c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgTheJewelsBriggs
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgThanksToBriggs
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgBriggsSMen
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgNowSThe
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte Func_02000098
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgWeStillHaven
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgItSNice
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgMostPeopleHere
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgTheOnlyWay
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
	.4byte MsgDidYouKnow
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgIMSoGrateful
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
