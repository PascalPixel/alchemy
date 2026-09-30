.syntax unified
	.thumb
	.section .text.x020095b4,"ax",%progbits
	.global ImiruMura_SwayAndSpark
	.thumb_func
ImiruMura_SwayAndSpark:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #100
	movs r2, #0
	ldrsh r0, [r5, r2]
	lsls r0, r0, #10
	sub sp, #12
	bl Engine_MathSin
	adds r1, r0, #0
	movs r0, #192
	ldr r3, .L_0200968c
	lsls r0, r0, #11
	mov r12, pc
	bx r3
	ldr r3, .L_02009690
	ldr r3, [r3]
	adds r3, r3, r0
	str r3, [r6, #8]
	ldrh r3, [r5]
	adds r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	asrs r1, r3, #16
	adds r2, r1, #0
	adds r2, #64
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020095f4
	adds r3, r1, #0
	adds r3, #127
.L_020095f4:
	asrs r3, r3, #6
	lsls r3, r3, #6
	subs r3, r2, r3
	strh r3, [r5]
	ldr r3, .L_02009694
	movs r1, #3
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_020096a4
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Engine_RandomNext
	adds r6, r0, #0
	bl Engine_RandomNext
	adds r1, r0, #0
	lsls r0, r6, #1
	adds r0, r0, r6
	adds r2, r5, #0
	lsls r0, r0, #1
	bl Vector_AddPolarOffset
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	ldr r0, .L_02009698
	bl Engine_ObjectCreate
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020096a4
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #9]
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetMode
	ldr r3, .L_0200969c
	adds r2, r5, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r3, .L_02009688
	adds r2, #50
	adds r0, r5, #0
	movs r1, #9
	strb r3, [r2]
	bl ObjectGroup_SetChildValue
	ldr r1, .L_020096a0
	adds r0, r5, #0
	bl Engine_ObjectSetScript
	b .L_020096a4
.L_02009688:
	.4byte 0x00000000
.L_0200968c:
	.4byte IwramMulQ16ReturnIp
.L_02009690:
	.4byte ImiruMura_ArcOrigin
.L_02009694:
	.4byte gFrameCount
.L_02009698:
	.4byte 0x0000011d
.L_0200969c:
	.4byte 0x00009999
.L_020096a0:
	.4byte ImiruMura_SparkScript
.L_020096a4:
	add sp, #12
	pop {r5, r6}
	pop {r0}
	bx r0
	.section .rodata.x0200a3f0,"a",%progbits
	.global ImiruMura_KeyHeadings
ImiruMura_KeyHeadings:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global ImiruMura_SwayHeadings
ImiruMura_SwayHeadings:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
.L_0200a450:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffb000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ImiruMura_ActorScriptA
ImiruMura_ActorScriptA:
	.4byte 0x00000022
	.4byte OverlayObject_UpdateWobbleByCounter
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ImiruMura_ActorScriptB
ImiruMura_ActorScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global ImiruMura_ActorScriptC
ImiruMura_ActorScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x009b0000
	.4byte 0x00000000
	.4byte 0x00870000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000010
	.global ImiruMura_TurnScript
ImiruMura_TurnScript:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000010
	.global ImiruMura_SparkScript
ImiruMura_SparkScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff999a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000048
	.4byte 0x00000000
	.4byte 0x0000001b
	.global ImiruMura_MiaScriptA
ImiruMura_MiaScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0x008b0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x00bd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global ImiruMura_MiaScriptB
ImiruMura_MiaScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a50000
	.4byte 0x00000000
	.4byte 0x00cf0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00930000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global ImiruMura_PrimaryScript
ImiruMura_PrimaryScript:
	.4byte 0x00000022
	.4byte OverlayObject_UpdateFacingTowardTarget
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ImiruMura_PrimaryScript2
ImiruMura_PrimaryScript2:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gImiruMuraEntrancesOther
gImiruMuraEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000120
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0x80000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000138
	.4byte 0x400001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001c8
	.4byte 0x400000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x400000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000040
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000e5
	.4byte 0x40000058
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gImiruMuraEntrances2
gImiruMuraEntrances2:
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0xc00000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000090
	.4byte 0xc00000f2
	.4byte 0x00200000
	.4byte 0x01000034
	.4byte 0x00000118
	.4byte 0xffff0002
	.4byte 0x0000008f
	.4byte 0xc0000254
	.4byte 0x002a0000
	.4byte 0x010001a8
	.4byte 0x00000277
	.4byte 0xffff0003
	.4byte 0x0000018f
	.4byte 0xc0000142
	.4byte 0x01400000
	.4byte 0x0218003a
	.4byte 0x00000168
	.4byte 0xffff0004
	.4byte 0x00000190
	.4byte 0xc0000293
	.4byte 0x01330000
	.4byte 0x0221017e
	.4byte 0x000002b9
	.4byte 0xffff0005
	.4byte 0x00000081
	.4byte 0xc0000393
	.4byte 0x00320000
	.4byte 0x015e02c2
	.4byte 0x000003b5
	.4byte 0xffff0006
	.4byte 0x000001be
	.4byte 0x40000339
	.4byte 0x01920000
	.4byte 0x029e02e4
	.4byte 0x00000399
	.4byte 0xffff0007
	.4byte 0x000002af
	.4byte 0xc0000108
	.4byte 0x02520000
	.4byte 0x03260031
	.4byte 0x00000127
	.4byte 0xffff0008
	.4byte 0x000000b6
	.4byte 0x40000304
	.4byte 0x00320000
	.4byte 0x015e02c2
	.4byte 0x000003b5
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ImiruMura_SceneTable
ImiruMura_SceneTable:
	.4byte 0x00000032
	.4byte 0x00101033
	.4byte 0x00205033
	.4byte 0x00303033
	.4byte 0x00404033
	.4byte 0x00502033
	.4byte 0x00607033
	.4byte 0x00801034
	.4byte 0x00b09002
	.4byte 0x00c27002
	.4byte 0x00000033
	.4byte 0x00103032
	.4byte 0x00207032
	.4byte 0x00305032
	.4byte 0x00406032
	.4byte 0x00504032
	.4byte 0x00608033
	.4byte 0x00708032
	.4byte 0x00806033
	.4byte 0x000001ff
	.global gImiruMuraPlacements
gImiruMuraPlacements:
	.4byte 0xffff006c
	.4byte .L_0200a450
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00003000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00023000
	.4byte 0xffff00ea
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gImiruMuraPlacementsFlag881
gImiruMuraPlacementsFlag881:
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x0000f000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00023000
	.4byte 0xffff00ea
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gImiruMuraPlacements2
gImiruMuraPlacements2:
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x006f0000
	.4byte 0x00000000
	.4byte 0x00d70000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00024000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00ae0000
	.4byte 0x00000000
	.4byte 0x00ad0000
	.4byte 0x00024000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00760000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0000c000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00024000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01de0000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00024000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x01cc0000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00005000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00014000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00004000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00f90000
	.4byte 0x00000000
	.4byte 0x03400000
	.4byte 0x00010000
	.4byte 0x0000006b
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00004000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x02cd0000
	.4byte 0x00000000
	.4byte 0x00e50000
	.4byte 0x0000b000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x000000df
	.4byte 0x00000007
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gImiruMuraEventsOther
gImiruMuraEventsOther:
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000c
	.4byte 0x00008c15
	.4byte 0x0200000b
	.4byte SceneState_UpdateActor11WithFlag203
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte ImiruMura_RunExitDoor
	.4byte 0x0000c602
	.4byte 0xffff0034
	.4byte ImiruMura_RunExitDoor
	.4byte 0x0000c602
	.4byte 0xffff0035
	.4byte ImiruMura_RunExitDoor
	.4byte 0x0000c602
	.4byte 0xffff0036
	.4byte ImiruMura_RunExitDoor
	.4byte 0x0000c602
	.4byte 0xffff0037
	.4byte ImiruMura_RunExitDoor
	.4byte 0x0000c602
	.4byte 0xffff0038
	.4byte ImiruMura_RunExitDoor
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte SceneActor_SetActorZeroFacingC000AndRun
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte FieldScene_Forward2188
	.4byte 0x00000000
	.4byte 0x08810008
	.4byte SceneDialogue_RunActorEightFlagGatedDialogue
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000163a
	.4byte 0x00000000
	.4byte 0x08810009
	.4byte SceneDialogue_RunActor9Line
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000163b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte SceneDialogue_RunActorTenFlag881Dialogue
	.4byte 0x00008d15
	.4byte 0x08810008
	.4byte SceneDialogue_ShowLine1571Or152F
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001640
	.4byte 0x00008d15
	.4byte 0x08810009
	.4byte 0x00001530
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001641
	.4byte 0x00008d15
	.4byte 0x0881000a
	.4byte 0x00001531
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001642
	.4byte 0x00000023
	.4byte 0x0f5f0064
	.4byte 0x00200009
	.4byte 0x00000013
	.4byte 0x0f600065
	.4byte 0x001000b6
	.4byte 0x00000083
	.4byte 0x0f610066
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gImiruMuraEvents2
gImiruMuraEvents2:
	.4byte 0x00000000
	.4byte 0x082c0008
	.4byte 0x00001532
	.4byte 0x00000000
	.4byte 0x082b0008
	.4byte 0x00001539
	.4byte 0x00000000
	.4byte 0x08810008
	.4byte FieldScene_RunScene399_020005dc
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001643
	.4byte 0x00000000
	.4byte 0x082c0009
	.4byte FieldScene_RunSupplementalSequenceOne
	.4byte 0x00000000
	.4byte 0x082b0009
	.4byte 0x00001538
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte FieldScene_RunSupplementalSequenceOne
	.4byte 0x00000000
	.4byte 0x182b000a
	.4byte FieldScene_RunSingleStep
	.4byte 0x00000000
	.4byte 0x0881000b
	.4byte 0x0000153c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001649
	.4byte 0x00000000
	.4byte 0x0881000c
	.4byte SceneDialogue_RunActor12Line
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte ImiruMura_RunWeaponShop
	.4byte 0x00000000
	.4byte 0x0881000d
	.4byte 0x00001544
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte ImiruMura_RunArmorShop
	.4byte 0x00000000
	.4byte 0x0881000f
	.4byte 0x00001548
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte ImiruMura_RunItemShop
	.4byte 0x00000000
	.4byte 0x0881000e
	.4byte ImiruMura_RunItemShop
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001650
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte FieldScene_RunScene399_02000a3c
	.4byte 0x00000000
	.4byte 0x08810011
	.4byte 0x0000154c
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001656
	.4byte 0x00000000
	.4byte 0x08810012
	.4byte SceneDialogue_RunActor18Line
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001657
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte FieldScene_RunScene399_02000abc
	.4byte 0x00000000
	.4byte 0x08810014
	.4byte SceneDialogue_RunActor20BranchScene
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001672
	.4byte 0x00008d15
	.4byte 0x08810008
	.4byte SceneDialogue_RunActorEightBranchedDialogue
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001647
	.4byte 0x00008d15
	.4byte 0x082c0409
	.4byte FieldScene_RunSupplementalSequenceOne
	.4byte 0x00008d15
	.4byte 0x082b0009
	.4byte 0x0000153a
	.4byte 0x00008d15
	.4byte 0x08810009
	.4byte 0x0000156e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001648
	.4byte 0x00008d15
	.4byte 0x182b000a
	.4byte SceneDialogue_ShowLine156E
	.4byte 0x00008d15
	.4byte 0x0881000b
	.4byte 0x0000153d
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000164a
	.4byte 0x00008d15
	.4byte 0x0881000c
	.4byte 0x00001542
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000164c
	.4byte 0x00008d15
	.4byte 0x0881000d
	.4byte 0x00001545
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000164e
	.4byte 0x00008d15
	.4byte 0x0881000f
	.4byte 0x0000154a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001651
	.4byte 0x00008d15
	.4byte 0x0881000e
	.4byte 0x00001549
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001652
	.4byte 0x00008d15
	.4byte 0x08810010
	.4byte 0x00001550
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001658
	.4byte 0x00008d15
	.4byte 0x08810011
	.4byte 0x00001551
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001659
	.4byte 0x00008d15
	.4byte 0x08810012
	.4byte 0x00001552
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000165a
	.4byte 0x00008d15
	.4byte 0x08810013
	.4byte SceneDialogue_ShowLine1573Or155A
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001673
	.4byte 0x00008d15
	.4byte 0x08810014
	.4byte SceneDialogue_RunActor20FlaggedLine
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001674
	.4byte 0x0000c403
	.4byte 0x0881000a
	.4byte ImiruMura_RunWeaponShop
	.4byte 0x0000c403
	.4byte 0x0881000b
	.4byte ImiruMura_RunArmorShop
	.4byte 0x00000013
	.4byte 0x0f260064
	.4byte 0x001000b9
	.4byte 0x00000033
	.4byte 0x0f620065
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0x0f630066
	.4byte 0x001000c4
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00402999
	.4byte 0x00000173
	.4byte 0xffff0066
	.4byte 0x0040299a
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200b0f4:
	.4byte 0x00250028
	.4byte 0x00020004
	.4byte 0x00280005
	.4byte 0x00040027
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00230021
	.4byte 0x00020002
	.4byte 0xffff0005
.L_0200b120:
	.4byte 0x00250023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020025
	.4byte 0x00050002
	.2byte 0xffff
.L_0200b136:
	.2byte 0x0023
	.4byte 0x0002002b
	.4byte 0x00050002
	.4byte 0x002b0021
	.4byte 0x00020002
	.4byte 0xffff0005
.L_0200b14c:
	.4byte 0x00270023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020027
	.4byte 0x00050002
	.2byte 0xffff
.L_0200b162:
	.2byte 0x002a
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x00300028
	.4byte 0x00020002
	.4byte 0xffff0005
.L_0200b178:
	.4byte 0x002e002a
	.4byte 0x00020002
	.4byte 0x00280005
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global ImiruMura_ExitCellSteps
ImiruMura_ExitCellSteps:
	.4byte .L_0200b14c
	.4byte .L_0200b136
	.4byte .L_0200b162
	.4byte .L_0200b178
	.4byte .L_0200b120
	.4byte .L_0200b0f4
	.global ImiruMura_ExitCellPoints
ImiruMura_ExitCellPoints:
	.4byte 0x00180032
	.4byte 0x00160039
	.4byte 0x000d003b
	.4byte 0x000a0036
	.4byte 0x00080026
	.4byte 0x00130022
	.global ImiruMura_CellStepsA
ImiruMura_CellStepsA:
	.4byte 0x007d0001
	.4byte 0x00020001
	.4byte 0x0000000a
	.4byte 0x0001007d
	.4byte 0x000a0002
	.2byte 0xffff
	.global ImiruMura_CellStepsB
ImiruMura_CellStepsB:
	.2byte 0x0001
	.4byte 0x0001007d
	.4byte 0x000a0002
	.4byte 0x007d0002
	.4byte 0x00020001
	.4byte 0xffff000a
	.section .bss,"aw",%nobits
	.space 4
	.global ImiruMura_ArcOrigin
ImiruMura_ArcOrigin:
	.space 12
