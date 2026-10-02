.syntax unified
	.thumb
	.section .text.x02009ca4,"ax",%progbits
	.global Func_02001ca4
	.thumb_func
Func_02001ca4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #0
	bl Object_GetById
	ldr r1, .L_02009e2c
	ldr r3, [r0, #8]
	adds r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, .L_02009e30
	asrs r3, r3, #1
	mov r11, r2
	add r3, r11
	mov r9, r3
	ldr r1, .L_02009e34
	ldr r3, [r0, #16]
	adds r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r7, #166
	asrs r3, r3, #1
	lsls r7, r7, #19
	movs r0, #183
	adds r3, r3, r7
	lsls r0, r0, #1
	movs r5, #0
	mov r10, r3
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02009cf8
	b .L_0200a080
.L_02009cf8:
	movs r0, #1
	bl Owner_RefreshActiveRatios
	movs r0, #183
	lsls r0, r0, #1
	bl Engine_GameFlagSet
	bl Engine_EventBegin
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02009d1e
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #8
	bl Engine_ActorSetPosition
.L_02009d1e:
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Djinn_AddToOwner
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Trade_AddOffer
	bl Battle_SetObjectFlag5bWhenMode3
	movs r1, #8
	movs r2, #0
	movs r0, #0
	bl Engine_ActorFaceActor
	movs r0, #10
	bl Engine_EventWait
	movs r0, #0
	ldr r1, .L_02009e38
	movs r2, #60
	bl Engine_ActorShowEmote
	adds r2, r6, #0
	movs r3, #1
	adds r2, #102
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Engine_ActorFaceActor
	movs r0, #16
	bl Engine_TaskWait
	ldr r0, .L_02009e3c
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	bl Battle_ClearObjectFlag5bWhenMode3
	ldr r0, .L_02009e40
	movs r1, #6
	bl BattleFx_ScheduleRatioTransition
	bl Event_WaitForDisplayField358Clear
	bl Battle_SetObjectFlag5bWhenMode3
	movs r2, #85
	adds r2, r2, r6
	movs r3, #2
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r5, [r6, #40]
	str r5, [r6, #20]
	adds r3, r7, #0
	mov r8, r2
	adds r0, r6, #0
	mov r1, r11
	movs r2, #0
	bl Engine_ObjectSetPosition
	movs r7, #128
	lsls r7, r7, #4
	movs r5, #15
.L_02009db8:
	ldr r3, [r6, #24]
	adds r3, r3, r7
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r7
	str r3, [r6, #28]
	movs r0, #1
	subs r5, #1
	bl Engine_TaskWait
	cmp r5, #0
	bge .L_02009db8
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Engine_ActorFaceActor
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl Engine_ActorFaceActor
	movs r0, #16
	bl Engine_TaskWait
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Engine_ObjectSetPartPalettes
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	movs r1, #0
	movs r0, #8
	bl Engine_EventShowMessage
	movs r0, #131
	bl Engine_AudioPlayCue
	movs r0, #140
	movs r1, #0
	bl Engine_PsynergyBegin
	ldr r7, .L_02009e44
	movs r5, #59
.L_02009e16:
	ldr r3, [r7]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02009e48
	adds r0, r6, #0
	movs r1, #7
	bl Engine_ObjectSetPartPalettes
	b .L_02009e50
	.2byte 0x0000
.L_02009e2c:
	.4byte 0xea300000
.L_02009e30:
	.4byte 0x15d00000
.L_02009e34:
	.4byte 0xfad00000
.L_02009e38:
	.4byte 0x00000101
.L_02009e3c:
	.4byte MsgWorldMapOh
.L_02009e40:
	.4byte 0x00013333
.L_02009e44:
	.4byte gFrameCount
.L_02009e48:
	adds r0, r6, #0
	movs r1, #0
	bl Engine_ObjectSetPartPalettes
.L_02009e50:
	ldr r3, [r7]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02009e60
	adds r0, r6, #0
	bl WorldMap_CreateLinkedEffects
.L_02009e60:
	movs r0, #1
	subs r5, #1
	bl Engine_TaskWait
	cmp r5, #0
	bge .L_02009e16
	bl BattleEffect_CleanupSceneObjects
	adds r0, r6, #0
	movs r1, #0
	bl Engine_ObjectSetPartPalettes
	movs r0, #8
	movs r1, #2
	bl Engine_ActorRunRepeatedMotion
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #129
	movs r2, #30
	movs r0, #0
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #0
	ldr r1, .L_02009f70
	movs r2, #30
	bl Engine_ActorShowEmote
	mov r3, r9
	asrs r1, r3, #16
	mov r3, r10
	asrs r2, r3, #16
	movs r0, #8
	bl Engine_ActorWalkToAndWait
	movs r0, #0
	movs r1, #22
	bl Engine_ActorSetAnimation
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #0
	ldr r1, .L_02009f70
	movs r2, #40
	bl Engine_ActorShowEmote
	movs r2, #30
	movs r0, #8
	movs r1, #4
	bl Engine_ActorJump
	movs r0, #150
	lsls r0, r0, #1
	movs r1, #4
	bl UiWork_PushValueSlot
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #128
	movs r2, #30
	movs r0, #0
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #0
	movs r1, #2
	bl Engine_ActorRunRepeatedMotion
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #30
	movs r0, #8
	movs r1, #2
	bl Engine_ActorJump
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r5, #0
	mov r1, r8
	movs r2, #128
	strb r5, [r1]
	adds r0, r6, #0
	mov r1, r9
	lsls r2, r2, #13
	mov r3, r10
	bl Engine_ObjectSetPosition
	ldr r7, .L_02009f6c
	movs r5, #15
.L_02009f38:
	ldrh r3, [r6, #6]
	adds r3, r3, r7
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl Engine_TaskWait
	cmp r5, #0
	bge .L_02009f38
	movs r0, #0
	movs r1, #1
	bl Engine_ActorSetAnimation
	movs r1, #0
	movs r0, #8
	bl Engine_EventShowMessage
	movs r2, #0
	movs r3, #2
	mov r1, r8
	strb r3, [r1]
	ldr r7, .L_02009f6c
	str r2, [r6, #40]
	str r2, [r6, #20]
	b .L_02009f74
	.2byte 0x0000
.L_02009f6c:
	.4byte 0x00001000
.L_02009f70:
	.4byte 0x00000101
.L_02009f74:
	movs r5, #7
.L_02009f76:
	ldrh r3, [r6, #6]
	adds r3, r3, r7
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl Engine_TaskWait
	cmp r5, #0
	bge .L_02009f76
	movs r0, #0
	movs r1, #22
	bl Engine_ActorSetAnimation
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #30
	bl Engine_ActorShowEmote
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Engine_ActorFaceActor
	movs r0, #8
	movs r1, #2
	bl Engine_ActorRunRepeatedMotion
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #2
	movs r2, #30
	bl Engine_ActorJump
	movs r0, #8
	movs r1, #0
	bl Engine_EventOpenMessage
	movs r5, #0
.L_02009fd2:
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #1
	bne .L_0200a01a
	movs r0, #8
	movs r1, #2
	movs r2, #20
	bl Engine_ActorJump
	movs r0, #8
	movs r1, #2
	movs r2, #20
	bl Engine_ActorJump
	cmp r5, #6
	bne .L_0200a006
	ldr r0, .L_0200a24c
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	b .L_0200a044
.L_0200a006:
	ldr r0, .L_0200a250
	adds r0, r5, r0
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventOpenMessage
	adds r5, #1
	b .L_02009fd2
.L_0200a01a:
	movs r0, #0
	movs r1, #22
	bl Engine_ActorSetAnimation
	movs r0, #8
	movs r1, #2
	movs r2, #20
	bl Engine_ActorJump
	movs r1, #4
	movs r0, #8
	movs r2, #20
	bl Engine_ActorJump
	ldr r0, .L_0200a254
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
.L_0200a044:
	movs r0, #150
	movs r1, #4
	lsls r0, r0, #1
	bl UiWork_PushValueSlot
	movs r0, #81
	bl Engine_AudioPlayCue
	ldr r5, .L_0200a258
	movs r1, #3
	adds r0, r5, #0
	adds r5, #1
	bl Engine_MessageShowCentered
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r2, #20
	movs r0, #8
	movs r1, #2
	bl Engine_ActorJump
	movs r1, #0
	movs r0, #8
	bl Engine_EventShowMessage
	movs r0, #9
	bl Engine_AudioPlayCue
	b .L_0200a1a6
.L_0200a080:
	bl Engine_EventBegin
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a098
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #8
	bl Engine_ActorSetPosition
.L_0200a098:
	movs r3, #160
	lsls r3, r3, #12
	mov r1, r9
	movs r2, #0
	str r3, [r6, #40]
	adds r0, r6, #0
	mov r3, r10
	bl Engine_ObjectSetPosition
	movs r0, #30
	bl Engine_EventWait
	bl Battle_SetObjectFlag5bWhenMode3
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Engine_ActorFaceActor
	movs r2, #0
	movs r0, #0
	movs r1, #8
	bl Engine_ActorFaceActor
	movs r1, #22
	movs r0, #0
	bl Engine_ActorSetAnimation
	ldr r0, .L_0200a25c
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #2
	movs r2, #20
	bl Engine_ActorJump
	movs r2, #20
	movs r0, #8
	movs r1, #2
	bl Engine_ActorJump
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #2
	bl Engine_ActorRunRepeatedMotion
	movs r1, #0
	movs r0, #8
	bl Engine_EventShowMessage
	movs r0, #111
	bl Engine_AudioPlayCue
	movs r1, #2
	movs r0, #0
	bl Menu_AnimateSelectionToEntry
	ldr r0, .L_0200a260
	bl Engine_GameFlagSet
	ldr r0, .L_0200a264
	bl Engine_GameFlagClear
	bl ItemMenu_Open
	ldr r0, .L_0200a268
	bl Engine_EventSetMessage
	adds r3, r7, #0
	movs r2, #0
	mov r1, r11
	adds r0, r6, #0
	bl Engine_ObjectSetPosition
	movs r0, #30
	bl Engine_EventWait
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Engine_ActorFaceActor
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #0
	movs r0, #8
	bl Engine_EventOpenMessage
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #1
	bne .L_0200a1b0
	movs r0, #0
	movs r1, #22
	bl Engine_ActorSetAnimation
	movs r1, #2
	movs r0, #8
	bl Engine_ActorRunRepeatedMotion
	ldr r0, .L_0200a26c
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #8
	bl Engine_EventOpenMessage
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #1
	beq .L_0200a1b0
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	mov r3, r10
	mov r2, r9
	asrs r1, r2, #16
	movs r0, #8
	asrs r2, r3, #16
	bl Engine_ActorWalkToAndWait
.L_0200a1a6:
	bl FieldScene_RunScene371_02001c08
	bl Battle_ClearObjectFlag5bWhenMode3
	b .L_0200a23c
.L_0200a1b0:
	movs r1, #22
	movs r0, #0
	bl Engine_ActorSetAnimation
	ldr r0, .L_0200a270
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #2
	movs r2, #20
	bl Engine_ActorJump
	movs r2, #20
	movs r0, #8
	movs r1, #2
	bl Engine_ActorJump
	movs r0, #0
	movs r1, #3
	bl Engine_ActorSetAnimationAndWait
	movs r1, #128
	movs r2, #30
	movs r0, #8
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r1, #0
	movs r0, #8
	bl Engine_EventShowMessage
	ldr r0, .L_0200a260
	bl Engine_GameFlagSet
	ldr r0, .L_0200a264
	bl Engine_GameFlagSet
	bl ItemMenu_Open
	movs r2, #20
	movs r0, #8
	movs r1, #2
	bl Engine_ActorJump
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	bl Battle_ClearObjectFlag5bWhenMode3
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl BattleFx_RunPageEffectForSlot
	movs r0, #42
	bl Engine_AudioPlayCue
	bl Engine_EventEnd
	movs r0, #183
	lsls r0, r0, #1
	bl Engine_GameFlagClear
	ldr r0, .L_0200a260
	bl Engine_GameFlagClear
	ldr r0, .L_0200a264
	bl Engine_GameFlagClear
.L_0200a23c:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0200a24c:
	.4byte MsgWorldMapMeanieDontCare
.L_0200a250:
	.4byte MsgWorldMapComePromiseWont
.L_0200a254:
	.4byte MsgWorldMapSeeWontRegret
.L_0200a258:
	.4byte MsgWorldMapAbilityVenusDjinni
.L_0200a25c:
	.4byte MsgWorldMapSeeDjinnUseful
.L_0200a260:
	.4byte 0x0000016f
.L_0200a264:
	.4byte 0x00000171
.L_0200a268:
	.4byte MsgWorldMapGoSetStandby
.L_0200a26c:
	.4byte MsgWorldMapHmmmmExplainAgain
.L_0200a270:
	.4byte MsgWorldMapYeahWantLearn
	.section .rodata.x0200c4ac,"a",%progbits
	.global gWorldMapPalettes
gWorldMapPalettes:
	.4byte 0x7c1f7c1f
	.4byte 0x20a21861
	.4byte 0x45643503
	.4byte 0x6a2659c5
	.4byte 0x7ecd7e87
	.4byte 0x7f997f33
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0x7c1f7c1f
	.4byte 0x013300ce
	.4byte 0x01b80176
	.4byte 0x023b01fa
	.4byte 0x02de027d
	.4byte 0x337f033f
	.4byte 0x613d7fff
	.4byte 0x00006dff
	.global gWorldMapPackedFrames
gWorldMapPackedFrames:
	.4byte 0xe8802b01
	.4byte 0xec020200
	.4byte 0x0596020b
	.4byte 0x03280008
	.4byte 0x103c00c4
	.4byte 0xec870201
	.4byte 0x2c400086
	.4byte 0x873fe086
	.4byte 0x2b4000e0
	.4byte 0x4000be06
	.4byte 0x00be0826
	.4byte 0x120d2340
	.4byte 0x60800088
	.4byte 0x0192e091
	.4byte 0xe491e402
	.4byte 0x25400082
	.4byte 0xe0a1e0a0
	.4byte 0xe40201a2
	.4byte 0xa0e4a108
	.4byte 0x234000e4
	.4byte 0x21b1e0b0
	.4byte 0x0201b2e0
	.4byte 0xb0e4b1e4
	.4byte 0x09254000
	.4byte 0xb2e8b1e8
	.4byte 0xb1ec0201
	.4byte 0x00840a01
	.4byte 0xa1e825c0
	.4byte 0x0201a2e8
	.4byte 0x01cfa1ec
	.4byte 0x2740100a
	.4byte 0x020192e8
	.4byte 0x40200601
	.4byte 0x020e202c
	.4byte 0xff4000f7
	.4byte 0x007f4000
	.4byte 0x40000750
	.4byte 0x8000e049
	.4byte 0x0e40106e
	.4byte 0xe2ff0060
	.4byte 0x081b4000
	.4byte 0x628000be
	.4byte 0x019de09c
	.4byte 0xbf41e402
	.4byte 0xab264000
	.4byte 0xade0ace0
	.4byte 0xe4080201
	.4byte 0x50abe4ac
	.4byte 0xe0bb24c0
	.4byte 0xbde021bc
	.4byte 0xbce40201
	.4byte 0x4000bbe4
	.4byte 0xbce80925
	.4byte 0x0201bde8
	.4byte 0x0a01bcec
	.4byte 0x25c00084
	.4byte 0xade8ace8
	.4byte 0xacec0201
	.4byte 0x100a01cf
	.4byte 0x9de82740
	.4byte 0x06010201
	.4byte 0x0f278010
	.4byte 0x4000fa14
	.4byte 0xeb40609f
	.4byte 0x502f4050
	.4byte 0x40003c80
	.4byte 0x0060e04e
	.4byte 0x60aae02e
	.4byte 0x60e02e00
	.4byte 0x60e02e00
	.4byte 0x50e02e00
	.4byte 0xc4e02e80
	.4byte 0x002e0060
	.4byte 0xe08e8880
	.4byte 0xe402018f
	.4byte 0x00e4448e
	.4byte 0xe08c2740
	.4byte 0xe402018d
	.4byte 0x40009a8c
	.4byte 0x018de829
	.4byte 0xec060102
	.4byte 0xe828c000
	.4byte 0x02018f7f
	.4byte 0x00300601
	.4byte 0xff400030
	.4byte 0xb0ce4000
	.4byte 0x40001f40
	.4byte 0x00c0f8af
	.4byte 0x8700c0ff
	.4byte 0x00174400
	.4byte 0x40000780
	.4byte 0x9ae0ba20
	.4byte 0xe4020188
	.4byte 0x4000e4ba
	.4byte 0xaae0b927
	.4byte 0xe4020193
	.4byte 0x294000b9
	.4byte 0x0201aae8
	.4byte 0xec4f0601
	.4byte 0xe828c000
	.4byte 0x0102019a
	.4byte 0x2c401006
	.4byte 0xfe228490
	.4byte 0x40b34000
	.4byte 0x80002b44
	.4byte 0x2f045073
	.4byte 0x00373c00
	.4byte 0xb610a340
	.4byte 0x00888800
	.4byte 0xec822c40
	.4byte 0x2c4000b5
	.4byte 0xbe82e0b5
	.4byte 0x882c0060
	.4byte 0x082c0010
	.4byte 0x264000be
	.4byte 0x4000be08
	.4byte 0x40e18826
	.4byte 0xbe0a2c00
	.4byte 0x83244000
	.4byte 0x00e483e0
	.4byte 0x85112940
	.4byte 0x020193e0
	.4byte 0x00e485e4
	.4byte 0xe8392840
	.4byte 0x01020193
	.4byte 0x2bc00006
	.4byte 0xc05083e8
	.4byte 0xc030f6b9
	.4byte 0xff400028
	.4byte 0x001e3050
	.4byte 0x00e04f40
	.4byte 0x40106e40
	.4byte 0xec22b70e
	.4byte 0x2c4000b6
	.4byte 0x00a6eca7
	.4byte 0x3fa62c40
	.4byte 0x0060a7e0
	.4byte 0x00be082a
	.4byte 0xbe082640
	.4byte 0x08264000
	.4byte 0x4000febe
	.4byte 0x003e0a24
	.4byte 0xfc0a2240
	.4byte 0x0e268000
	.4byte 0x20c04042
	.4byte 0xb3e013b3
	.4byte 0x2c4000e4
	.4byte 0x40a0b3e8
	.4byte 0x1f309035
	.4byte 0xab4000f5
	.4byte 0x00ff0200
	.4byte 0x0060ff02
	.4byte 0x80709820
	.4byte 0x4000972e
	.4byte 0xb0977f2c
	.4byte 0xbe082c40
	.4byte 0x00298000
	.4byte 0xbe082c40
	.4byte 0x08268050
	.4byte 0x8000b0be
	.4byte 0x8000ec27
	.4byte 0x22c05038
	.4byte 0xe481e081
	.4byte 0x2c40009f
	.4byte 0x006081e8
	.4byte 0xff0200ff
	.4byte 0x00ff0200
	.4byte 0xa2412102
	.4byte 0xff0200df
	.4byte 0xb77a0200
	.4byte 0x00ff1210
	.4byte 0x0200ff02
	.4byte 0xff0200ff
	.4byte 0x80100200
	.2byte 0x0000
	.global gWorldMapPackedTiles
gWorldMapPackedTiles:
	.2byte 0xff00
	.4byte 0x1141387d
	.4byte 0x7ffe6715
	.4byte 0xe6889329
	.4byte 0x65ffaad0
	.4byte 0x72fbb552
	.4byte 0x8e4bd968
	.4byte 0xfbff3375
	.4byte 0x982f82bc
	.4byte 0x4f9985e1
	.4byte 0x947bc509
	.4byte 0x3e1c09f1
	.4byte 0x1567c0ff
	.4byte 0xea943994
	.4byte 0xab4499cb
	.4byte 0x4cb541dc
	.4byte 0x90cd1d9d
	.4byte 0x183b9574
	.4byte 0x465bbdda
	.4byte 0x4519b111
	.4byte 0x630f76cc
	.4byte 0x71176d4e
	.4byte 0x3177ddea
	.4byte 0xbaa666eb
	.4byte 0xc39531cc
	.4byte 0xbc9f60a7
	.4byte 0xb2350a43
	.4byte 0x3521ccf8
	.4byte 0x45c80c78
	.4byte 0x19ccf490
	.4byte 0x44c62e2a
	.4byte 0x4aac5489
	.4byte 0x2ad56a94
	.4byte 0x41d8658a
	.4byte 0x0547ccd1
	.4byte 0x611c0ab3
	.4byte 0x10c0a336
	.4byte 0x2a05033e
	.4byte 0x500a11c0
	.4byte 0x00a11c84
	.4byte 0x892fc845
	.4byte 0x905596e4
	.4byte 0x483bd970
	.4byte 0xa9e670f8
	.4byte 0xa6ab3176
	.4byte 0x5251886a
	.4byte 0x0eebacbd
	.4byte 0x7cb3bf9f
	.4byte 0xa59f8176
	.4byte 0x52c78d30
	.4byte 0x06a7ca38
	.4byte 0x8f9b3306
	.4byte 0x4238c682
	.4byte 0xf8473360
	.4byte 0x13e27104
	.4byte 0xd119b370
	.4byte 0x9890f139
	.4byte 0x644c4898
	.4byte 0xcd4a915e
	.4byte 0x10f8352a
	.4byte 0xcd3532cd
	.4byte 0x2b3870d4
	.4byte 0x045e40b2
	.4byte 0x4ef05917
	.4byte 0x854ef524
	.4byte 0x7d118782
	.4byte 0xd3e2f4fe
	.4byte 0xa038cc0c
	.4byte 0xdb034c2b
	.4byte 0xb87032c0
	.4byte 0x2c213398
	.4byte 0xfc99aaec
	.4byte 0xb37b0a8d
	.4byte 0x656367c3
	.4byte 0x6ac38f1e
	.4byte 0x5268c2cc
	.4byte 0x04c3c1a9
	.4byte 0x1d3f2227
	.4byte 0x1c9af02e
	.4byte 0xa0014098
	.4byte 0x0a40a398
	.4byte 0x8a398a08
	.4byte 0x75180e8f
	.4byte 0x0be60b60
	.4byte 0x48dc87be
	.4byte 0x88876728
	.4byte 0xe7289db1
	.4byte 0x02e81d9d
	.4byte 0xc10ee0a0
	.4byte 0x7687ba82
	.4byte 0x42227088
	.4byte 0xd267118f
	.4byte 0x40899ccc
	.4byte 0x834aacce
	.4byte 0x1be0706f
	.4byte 0x61b62448
	.4byte 0x6571c787
	.4byte 0x05d814c3
	.4byte 0xc4c327c8
	.4byte 0x7c808af9
	.4byte 0x4e5ea933
	.4byte 0xe7d551d5
	.4byte 0x103e350f
	.4byte 0xe5ce7182
	.4byte 0xe7107013
	.4byte 0x0f87be1e
	.4byte 0x127ccbc6
	.4byte 0xf80c066e
	.4byte 0x65cfa21d
	.4byte 0xb264e43a
	.4byte 0x7989e132
	.4byte 0x29e70e8a
	.4byte 0x942246e6
	.4byte 0x622eb888
	.4byte 0x27e0a182
	.4byte 0x827e2e98
	.4byte 0xfc18fa90
	.4byte 0x041687c4
	.4byte 0xfa0421f0
	.4byte 0xd7b0ebc2
	.4byte 0x3f863edd
	.4byte 0xea6f07c0
	.4byte 0xfe7d8873
	.4byte 0xf03fcf81
	.4byte 0x953e07f9
	.4byte 0xf86b8fc0
	.4byte 0x00ff0e38
	.4byte 0xc3bc61fa
	.4byte 0xfe7cf0ff
	.4byte 0x6cbfe050
	.4byte 0x00737848
	.4byte 0xd7f76b76
	.4byte 0xd001ce01
	.4byte 0x1f780049
	.4byte 0x183c1f85
	.4byte 0xfcfb9bc0
	.4byte 0xe07f9f03
	.4byte 0x3e1df763
	.4byte 0x1fe7c0ff
	.4byte 0x9f03fcf8
	.4byte 0x0ff3e07f
	.4byte 0xcf81fe7c
	.4byte 0x04e1f03f
	.4byte 0x0000003e
	.global gTransferArrive10
gTransferArrive10:
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x064e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16e80000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gTransferDepart10
gTransferDepart10:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000010
	.global gTransferReturn10
gTransferReturn10:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global gTransferGather
gTransferGather:
	.4byte 0x00000002
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06a80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16f00000
	.4byte 0x00000000
	.4byte 0x05f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16f80000
	.4byte 0x00000000
	.4byte 0x05480000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global gTransferArrive11
gTransferArrive11:
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x065e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16c80000
	.4byte 0x00000000
	.4byte 0x06680000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gTransferDepart11
gTransferDepart11:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16e70000
	.4byte 0x00000000
	.4byte 0x06680000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16080000
	.4byte 0x00000000
	.4byte 0x06e80000
	.4byte 0x00000010
	.global gTransferReturn11
gTransferReturn11:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16080000
	.4byte 0x00000000
	.4byte 0x07030000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global gTransferArrive12
gTransferArrive12:
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x064e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16c80000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gTransferDepart12
gTransferDepart12:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16e70000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x06e80000
	.4byte 0x00000010
	.global gTransferReturn12
gTransferReturn12:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x07030000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global gTransferArrive13
gTransferArrive13:
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x063e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16c80000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gTransferDepart13
gTransferDepart13:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16e70000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16280000
	.4byte 0x00000000
	.4byte 0x06e80000
	.4byte 0x00000010
	.global gTransferReturn13
gTransferReturn13:
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x16280000
	.4byte 0x00000000
	.4byte 0x07030000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global gTransferGuide14
gTransferGuide14:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x15e80000
	.4byte 0x00000000
	.4byte 0x06b80000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000028f
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000028f
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0xc0010000
	.4byte 0x00000010
	.global gBlackOrbLeaderScript
gBlackOrbLeaderScript:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x17780000
	.4byte 0x00000000
	.4byte 0x0d480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x17840000
	.4byte 0x00000000
	.4byte 0x0d480000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gPresentGuide9
gPresentGuide9:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x1d8c0000
	.4byte 0x00000000
	.4byte 0x0d940000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1d980000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1dd80000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1de80000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e280000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e480000
	.4byte 0x00000000
	.4byte 0x0da80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gPresentGuide8
gPresentGuide8:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x1d8c0000
	.4byte 0x00000000
	.4byte 0x0d940000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1d980000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1dd80000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1de80000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e180000
	.4byte 0x00000000
	.4byte 0x0db80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gPresentGuide5
gPresentGuide5:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x1d8c0000
	.4byte 0x00000000
	.4byte 0x0d940000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1d980000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1dd80000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1de80000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e080000
	.4byte 0x00000000
	.4byte 0x0dc80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gTransitionSparkScript
gTransitionSparkScript:
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001b
	.global gTransferLeaderIdle
gTransferLeaderIdle:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gTransferLeaderTurn
gTransferLeaderTurn:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfffe0000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfffe0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfffe0000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gOpeningLeaderRise
gOpeningLeaderRise:
	.4byte 0x00000004
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
.L_0200d270:
	.4byte 0x00000022
	.4byte StoryActor_Initialize
	.4byte 0x00000010
	.global gWorldMapEntrances
gWorldMapEntrances:
	.4byte 0xffff0000
	.4byte 0x00001618
	.4byte 0x400004e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00001630
	.4byte 0x400004ca
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00001638
	.4byte 0x400004c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000016d8
	.4byte 0x40000648
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00001918
	.4byte 0x40000528
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000019b8
	.4byte 0x400004c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00001b10
	.4byte 0x80000558
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00001ca0
	.4byte 0x800004d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00001be0
	.4byte 0x8000041e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00001728
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00001788
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00001d90
	.4byte 0x40000588
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00001ec8
	.4byte 0x40000638
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00001eac
	.4byte 0xc0000698
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x00001d78
	.4byte 0x400007d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x00001be8
	.4byte 0x000006c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x00001b60
	.4byte 0x00000658
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x00001ae8
	.4byte 0x400006c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x00001ae8
	.4byte 0x00000724
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x000015b8
	.4byte 0x80000858
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x000015d8
	.4byte 0x40000878
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0015
	.4byte 0x00001530
	.4byte 0x400008d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0016
	.4byte 0x0000138c
	.4byte 0x80000918
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0017
	.4byte 0x00001328
	.4byte 0x40000908
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0018
	.4byte 0x000012e8
	.4byte 0x400007c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0019
	.4byte 0x0000134c
	.4byte 0xc0000a58
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001a
	.4byte 0x00001618
	.4byte 0x40000978
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001b
	.4byte 0x0000150c
	.4byte 0x80000b18
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001c
	.4byte 0x00001548
	.4byte 0xc0000b68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001d
	.4byte 0x000016e8
	.4byte 0x40000d48
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001e
	.4byte 0x0000176c
	.4byte 0x40000b72
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001f
	.4byte 0x0000176c
	.4byte 0x40000b18
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0020
	.4byte 0x00001790
	.4byte 0x80000c78
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0021
	.4byte 0x00001758
	.4byte 0x40000d68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0022
	.4byte 0x00001708
	.4byte 0x40000488
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0023
	.4byte 0x00001766
	.4byte 0x40000324
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0026
	.4byte 0x00001cd0
	.4byte 0x000004d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0027
	.4byte 0x00001744
	.4byte 0x0000010a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0028
	.4byte 0x00001954
	.4byte 0x400004bc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0029
	.4byte 0x00001568
	.4byte 0x40000a48
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002a
	.4byte 0x00001728
	.4byte 0x00000cc8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002b
	.4byte 0x00001e70
	.4byte 0x40000838
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002c
	.4byte 0x000016d8
	.4byte 0xc0000608
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002d
	.4byte 0x00001868
	.4byte 0x40000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002e
	.4byte 0x00001d90
	.4byte 0xc0000548
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002f
	.4byte 0x00001b40
	.4byte 0x00000550
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0030
	.4byte 0x00001854
	.4byte 0x40000228
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0031
	.4byte 0x000016e4
	.4byte 0x400004bc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0032
	.4byte 0x00001b90
	.4byte 0xc0000678
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0033
	.4byte 0x00001b58
	.4byte 0x800006c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0034
	.4byte 0x00001b08
	.4byte 0x400006a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0035
	.4byte 0x000017ca
	.4byte 0x400008b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0036
	.4byte 0x0000134c
	.4byte 0x40000aa8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0037
	.4byte 0x00001528
	.4byte 0x00000cc8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0038
	.4byte 0x00001528
	.4byte 0xc0000af8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0039
	.4byte 0x00001544
	.4byte 0x00000b18
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003a
	.4byte 0x00001530
	.4byte 0xc0000898
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003b
	.4byte 0x0000176c
	.4byte 0xc0000b28
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003c
	.4byte 0x00001568
	.4byte 0x40000848
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003d
	.4byte 0x00001808
	.4byte 0x00000c8c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0040
	.4byte 0x000016d8
	.4byte 0x40000648
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0041
	.4byte 0x00001508
	.4byte 0x400008c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0042
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0043
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0044
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0045
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffff0080
	.4byte 0x0000ffff
	.4byte 0xffff0046
	.4byte 0x000013e8
	.4byte 0x40000918
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0047
	.4byte 0x00001598
	.4byte 0x40000848
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0048
	.4byte 0x00001d28
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0049
	.4byte 0x000017c8
	.4byte 0x80000c68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004a
	.4byte 0x00001278
	.4byte 0x800001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004b
	.4byte 0x00001308
	.4byte 0x80000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004c
	.4byte 0x000011e0
	.4byte 0x400001fc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004d
	.4byte 0x00001128
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004e
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0050
	.4byte 0x00001d80
	.4byte 0x40000db0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff005a
	.4byte 0x00001788
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff005b
	.4byte 0x000017c8
	.4byte 0x40000c60
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0061
	.4byte 0x00001868
	.4byte 0x40000368
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x00001098
	.4byte 0x40000978
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x000015c8
	.4byte 0x40000828
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapExits
gWorldMapExits:
	.4byte 0x00000002
	.4byte 0x0010a005
	.4byte 0x00202002
	.4byte 0x00301014
	.4byte 0x00401019
	.4byte 0x0050101e
	.4byte 0x00601023
	.4byte 0x00701024
	.4byte 0x00804028
	.4byte 0x00901032
	.4byte 0x00a0b039
	.4byte 0x00b01027
	.4byte 0x00c0103c
	.4byte 0x00d01044
	.4byte 0x00e01048
	.4byte 0x00f0104a
	.4byte 0x0100104b
	.4byte 0x01101058
	.4byte 0x01201059
	.4byte 0x01313002
	.4byte 0x01409063
	.4byte 0x0150106b
	.4byte 0x01602070
	.4byte 0x01701087
	.4byte 0x01801092
	.4byte 0x01901098
	.4byte 0x01a0109e
	.4byte 0x01b0a0a7
	.4byte 0x01c010a4
	.4byte 0x01d010a9
	.4byte 0x01e140b2
	.4byte 0x01f010b1
	.4byte 0x020010b5
	.4byte 0x021040ab
	.4byte 0x02201068
	.4byte 0x02301031
	.4byte 0x0280201d
	.4byte 0x0290609e
	.4byte 0x02a050a9
	.4byte 0x02b0d046
	.4byte 0x02c03014
	.4byte 0x02d01031
	.4byte 0x02e02027
	.4byte 0x02f02023
	.4byte 0x0300302f
	.4byte 0x0310106a
	.4byte 0x0320204a
	.4byte 0x0330304a
	.4byte 0x03404057
	.4byte 0x0350405c
	.4byte 0x03602098
	.4byte 0x037020a5
	.4byte 0x0380406b
	.4byte 0x03b150b2
	.4byte 0x03c02099
	.4byte 0x03d030b5
	.4byte 0x04a0606d
	.4byte 0x04b01071
	.4byte 0x06401002
	.4byte 0x06540002
	.4byte 0x0660b06e
	.4byte 0x0670c06e
	.4byte 0x0680d06e
	.4byte 0x0691506f
	.4byte 0x06a0e06e
	.4byte 0x06b1206d
	.4byte 0x06c0a099
	.4byte 0x06d040bb
	.4byte 0x06e1706f
	.4byte 0x06f50002
	.4byte 0x0700a069
	.4byte 0x000001ff
.L_0200db4c:
	.4byte 0x00000022
	.4byte StoryActor_ApplyFlaggedMode
	.4byte 0x00000010
.L_0200db58:
	.4byte 0x00000022
	.4byte StoryActor_ApplyMapRotation
	.4byte 0x00000010
.L_0200db64:
	.4byte 0x00000022
	.4byte StoryActor_ApplyMapRotationWithCollision
	.4byte 0x00000010
.L_0200db70:
	.4byte 0x00000022
	.4byte StoryActor_ResetPosition
	.4byte 0x00000022
	.4byte StoryActor_ClearActiveFlag
	.4byte 0x00000010
	.global gWorldMapPlacements
gWorldMapPlacements:
	.4byte 0x0033005a
	.4byte .L_0200d270
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x005a005c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0048005b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x162e0000
	.4byte 0x00000000
	.4byte 0x04b00000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x1b400000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x15280000
	.4byte 0x00000000
	.4byte 0x0b180000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x17040000
	.4byte 0x00000000
	.4byte 0x04680000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte .L_0200db58
	.4byte 0x152e0000
	.4byte 0x00000000
	.4byte 0x08b80000
	.4byte 0x00020000
	.4byte 0xffff0115
	.4byte .L_0200db58
	.4byte 0x13ac0000
	.4byte 0x00000000
	.4byte 0x09180000
	.4byte 0x00028000
	.4byte 0xffff0109
	.4byte .L_0200db58
	.4byte 0x17280000
	.4byte 0x00000000
	.4byte 0x010e0000
	.4byte 0x00024000
	.4byte 0xffff0108
	.4byte .L_0200db58
	.4byte 0x1cb80000
	.4byte 0x00000000
	.4byte 0x04d80000
	.4byte 0x00024000
	.4byte 0xffff010a
	.4byte .L_0200db58
	.4byte 0x1d780000
	.4byte 0x00000000
	.4byte 0x07b80000
	.4byte 0x00024000
	.4byte 0xffff010b
	.4byte .L_0200db58
	.4byte 0x1ae80000
	.4byte 0x00000000
	.4byte 0x06b00000
	.4byte 0x00024000
	.4byte 0xffff010b
	.4byte .L_0200db58
	.4byte 0x1ec80000
	.4byte 0x00000000
	.4byte 0x06180000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte .L_0200db58
	.4byte 0x19b80000
	.4byte 0x00000000
	.4byte 0x04a80000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte .L_0200db58
	.4byte 0x15d80000
	.4byte 0x00000000
	.4byte 0x08580000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte .L_0200db58
	.4byte 0x176c0000
	.4byte 0x00000000
	.4byte 0x0b4e0000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte .L_0200db58
	.4byte 0x13280000
	.4byte 0x00000000
	.4byte 0x08e80000
	.4byte 0x00024000
	.4byte 0xffff010d
	.4byte .L_0200db58
	.4byte 0x17880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff010f
	.4byte .L_0200db58
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x04580000
	.4byte 0x00024000
	.4byte 0xffff0114
	.4byte .L_0200db58
	.4byte 0x17c80000
	.4byte 0x00000000
	.4byte 0x0c680000
	.4byte 0x00024000
	.4byte 0xffff010e
	.4byte .L_0200db58
	.4byte 0x1b280000
	.4byte 0x00000000
	.4byte 0x05520000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte .L_0200db58
	.4byte 0x176c0000
	.4byte 0x00000000
	.4byte 0x0af80000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x19180000
	.4byte 0x00000000
	.4byte 0x05160000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x19540000
	.4byte 0x00000000
	.4byte 0x04a00000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x12e80000
	.4byte 0x00000000
	.4byte 0x07a80000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x09580000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x15680000
	.4byte 0x00000000
	.4byte 0x0a280000
	.4byte 0x00024000
	.4byte 0xffff0113
	.4byte .L_0200db58
	.4byte 0x1e6e0000
	.4byte 0x00000000
	.4byte 0x08200000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x17580000
	.4byte 0x00000000
	.4byte 0x0d480000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x17640000
	.4byte 0x00000000
	.4byte 0x03060000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x18500000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x16e00000
	.4byte 0x00000000
	.4byte 0x04960000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x1b080000
	.4byte 0x00000000
	.4byte 0x06800000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x17cc0000
	.4byte 0x00000000
	.4byte 0x08980000
	.4byte 0x00024000
	.4byte 0xffff0113
	.4byte .L_0200db58
	.4byte 0x1bfe0000
	.4byte 0x00000000
	.4byte 0x041e0000
	.4byte 0x00024000
	.4byte 0xffff0113
	.4byte .L_0200db58
	.4byte 0x1eb00000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x15680000
	.4byte 0x00000000
	.4byte 0x08280000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0112
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0116
	.4byte .L_0200db4c
	.4byte 0x1d900000
	.4byte 0x00000000
	.4byte 0x05680000
	.4byte 0x00024000
	.4byte 0xffff0117
	.4byte .L_0200db58
	.4byte 0x134e0000
	.4byte 0x00000000
	.4byte 0x0a820000
	.4byte 0x00024000
	.4byte 0xffff01f5
	.4byte .L_0200db58
	.4byte 0x17940000
	.4byte 0x00000000
	.4byte 0x0d820000
	.4byte 0x00028000
	.4byte 0x18a000a1
	.4byte .L_0200db58
	.4byte 0x12980000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002c000
	.4byte 0x02f10121
	.4byte .L_0200db70
	.4byte 0x11280000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte .L_0200db64
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements64
gWorldMapPlacements64:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff009a
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff009a
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff009a
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff009a
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements49
gWorldMapPlacements49:
	.4byte 0xffff009a
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0105
	.4byte .L_0200db58
	.4byte 0x17040000
	.4byte 0x00000000
	.4byte 0x04680000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x16e00000
	.4byte 0x00000000
	.4byte 0x04960000
	.4byte 0x00024000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements65
gWorldMapPlacements65:
	.4byte 0xffff00a1
	.4byte .L_0200db58
	.4byte 0x15080000
	.4byte 0x00000000
	.4byte 0x08c80000
	.4byte 0x00028000
	.4byte 0xffff0107
	.4byte .L_0200db58
	.4byte 0x15d80000
	.4byte 0x00000000
	.4byte 0x08580000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte .L_0200db58
	.4byte 0x152e0000
	.4byte 0x00000000
	.4byte 0x08b80000
	.4byte 0x00020000
	.4byte 0xffff0115
	.4byte .L_0200db58
	.4byte 0x13a40000
	.4byte 0x00000000
	.4byte 0x09180000
	.4byte 0x01028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements66
gWorldMapPlacements66:
	.4byte 0xffff00a1
	.4byte .L_0200db58
	.4byte 0x15380000
	.4byte 0x00000000
	.4byte 0x09080000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements71
gWorldMapPlacements71:
	.4byte 0xffff0107
	.4byte .L_0200db58
	.4byte 0x15d80000
	.4byte 0x00000000
	.4byte 0x08580000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte .L_0200db58
	.4byte 0x15680000
	.4byte 0x00000000
	.4byte 0x08280000
	.4byte 0x00024000
	.4byte 0xffff009a
	.4byte .L_0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements80
gWorldMapPlacements80:
	.4byte 0xffff0005
	.4byte .L_0200db58
	.4byte 0x1d880000
	.4byte 0x00000000
	.4byte 0x0db80000
	.4byte 0x00015000
	.4byte 0xffff0006
	.4byte .L_0200db58
	.4byte 0x1e880000
	.4byte 0x00000000
	.4byte 0x0dc80000
	.4byte 0x00010000
	.4byte 0xffff001e
	.4byte .L_0200db58
	.4byte 0x1d780000
	.4byte 0x00000000
	.4byte 0x0da80000
	.4byte 0x0001d000
	.4byte 0xffff002b
	.4byte .L_0200db58
	.4byte 0x1e1c0000
	.4byte 0x00000000
	.4byte 0x0d800000
	.4byte 0x00015000
	.4byte 0xffff0023
	.4byte .L_0200db58
	.4byte 0x1e780000
	.4byte 0x00000000
	.4byte 0x0dd80000
	.4byte 0x0001b000
	.4byte 0xffff01f5
	.4byte .L_0200db58
	.4byte 0x1db40000
	.4byte 0x00000000
	.4byte 0x0dc20000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements72
gWorldMapPlacements72:
	.4byte 0xffff01f8
	.4byte .L_0200db58
	.4byte 0x1d280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapPlacements73
gWorldMapPlacements73:
	.4byte 0xffff0114
	.4byte .L_0200db58
	.4byte 0x17c00000
	.4byte 0x00000000
	.4byte 0x0c600000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapEvents
gWorldMapEvents:
	.4byte 0x00000002
	.4byte 0x0030005a
	.4byte Func_02001ca4
	.4byte 0x00000002
	.4byte 0x02f1005f
	.4byte StoryScene_SetReferenceActor
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte StoryProgress_TriggerEvent0808
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte StoryProgress_TriggerEvent0809
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte StoryProgress_TriggerEvent080A
	.4byte 0x00000002
	.4byte 0x0033005c
	.4byte StoryProgress_TriggerEvent0808
	.4byte 0x00000002
	.4byte 0x005a005b
	.4byte StoryProgress_TriggerEvent0809
	.4byte 0x00000002
	.4byte 0x0048005d
	.4byte StoryProgress_TriggerEvent080A
	.4byte 0x00000003
	.4byte 0xffff0060
	.4byte StoryScene_ShowRewardDialogue
	.4byte 0x0000f204
	.4byte 0x085d0060
	.4byte WorldMap_UseBlackOrb
	.4byte 0x00000053
	.4byte 0x0fcf0064
	.4byte 0x00100103
	.4byte 0x00000000
	.4byte 0xffff0037
	.4byte FieldScene_RunScene371_0200281c
	.4byte 0x00008d15
	.4byte 0xffff0437
	.4byte FieldScene_RunScene371_0200281c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0032
	.4byte 0x00000032
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000033
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff0037
	.4byte 0x00000037
	.4byte 0x00000001
	.4byte 0xffff001d
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000003d
	.4byte 0x00000001
	.4byte 0xffff002f
	.4byte 0x0000002f
	.4byte 0x00000001
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0070
	.4byte SceneState_ApplyFlag85aBranch
	.4byte 0x00000001
	.4byte 0xffff0084
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0085
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff007b
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0082
	.4byte SceneState_SetValues130_6_47
	.4byte 0x00000001
	.4byte 0xffff0077
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0xffff0096
	.4byte SceneState_ApplyValues150And46And11
	.4byte 0x00000001
	.4byte 0xffff0090
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff008b
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0xffff008c
	.4byte 0x00000030
	.4byte 0x00000001
	.4byte 0xffff0076
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff007f
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff007a
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff0091
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0089
	.4byte 0x0000002b
	.4byte 0x00000001
	.4byte 0xffff0078
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff0071
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff008e
	.4byte 0x00000034
	.4byte 0x00000001
	.4byte 0xffff0079
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff008f
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0xffff007c
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0073
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0xffff008d
	.4byte 0x00000031
	.4byte 0x00000002
	.4byte 0xffff0074
	.4byte SceneState_ApplyValues116And56And21
	.4byte 0x00000001
	.4byte 0xffff0075
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff007e
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0086
	.4byte 0x00000018
	.4byte 0x00000002
	.4byte 0xffff0097
	.4byte SceneState_ApplyValues151And25And54
	.4byte 0x00000001
	.4byte 0xffff0092
	.4byte 0x0000003c
	.4byte 0x00000001
	.4byte 0xffff0087
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff0088
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff0072
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff008a
	.4byte 0x00000021
	.4byte 0x00000001
	.4byte 0xffff0083
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0xffff007d
	.4byte FieldScene_RunStep7D3B1E
	.4byte 0x00000602
	.4byte 0x18a0004a
	.4byte FieldScene_RunStep74
	.4byte 0x0000c401
	.4byte 0xffff004b
	.4byte 0x0000004b
	.4byte 0x10002115
	.4byte 0x12f00036
	.4byte StoryScene_ActivateSharedState
	.4byte 0x00002115
	.4byte 0x12f00036
	.4byte StoryScene_CompleteActor98
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte WorldMap_RaiseActors
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gWorldMapRewards
gWorldMapRewards:
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000008
	.4byte 0x00000004
	.4byte 0x00000009
	.4byte 0x0000000c
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00000000
	.global gActorEightPuffScript
gActorEightPuffScript:
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000e00
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000030
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
