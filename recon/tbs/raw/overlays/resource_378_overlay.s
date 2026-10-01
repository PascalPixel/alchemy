.syntax unified
	.thumb
	.section .text.x0200a7d2,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a7d4,"ax",%progbits
	.global ShindenHeya_ChooseRestartOption
	.thumb_func
ShindenHeya_ChooseRestartOption:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	movs r1, #0
	movs r2, #0
	sub sp, #20
	bl Engine_ActorSetPosition
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Engine_ActorSetPosition
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Engine_ColorBufferApplyTarget
	movs r0, #1
	bl Engine_ColorBufferInterpolate
	movs r0, #1
	bl Engine_EventWait
	movs r3, #1
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #25
	movs r3, #5
	movs r0, #2
	bl UiWindow_Create
	ldr r5, .L_0200a8f8
	adds r7, r0, #0
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #16
	movs r3, #0
	bl UiText_DrawResource
	movs r0, #1
	bl SaveState_CountRecordsExcludingFlagged
	cmp r0, #0
	bne .L_0200a870
	adds r0, r5, #2
	adds r1, r7, #0
	movs r2, #16
	movs r3, #16
	bl UiText_DrawResource
	b .L_0200a87c
.L_0200a870:
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #16
	movs r3, #16
	bl UiText_DrawResource
.L_0200a87c:
	add r1, sp, #4
	add r0, sp, #8
	bl UiTextResource_Initialize
	movs r2, #60
	add r0, sp, #8
	movs r1, #72
	bl UiTextResource_SetPosition
	ldr r3, .L_0200a8fc
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	movs r5, #0
	cmp r3, #0
	bne .L_0200a8dc
	ldr r2, .L_0200a900
	movs r6, #1
	mov r8, r2
.L_0200a8a2:
	ldr r3, .L_0200a904
	ldr r3, [r3]
	movs r2, #192
	ands r3, r2
	cmp r3, #0
	beq .L_0200a8b0
	eors r5, r6
.L_0200a8b0:
	ldr r3, .L_0200a908
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #1
	ands r3, r2
	lsls r3, r3, #2
	mov r2, r8
	ldr r1, [r2, r3]
	lsls r2, r5, #4
	add r0, sp, #8
	adds r1, #24
	adds r2, #60
	bl UiTextResource_SetPosition
	movs r0, #1
	bl Engine_EventWait
	ldr r3, .L_0200a8fc
	ldr r3, [r3]
	ands r3, r6
	cmp r3, #0
	beq .L_0200a8a2
.L_0200a8dc:
	ldr r0, [sp, #4]
	bl UiTextResource_Release
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_Finalize
	adds r0, r5, #0
	add sp, #20
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0200a8f8:
	.4byte 0x0000116e
.L_0200a8fc:
	.4byte gKeyState
.L_0200a900:
	.4byte Data_0200411c
.L_0200a904:
	.4byte gKeysRepeat
.L_0200a908:
	.4byte gFrameTick
	.section .rodata.x0200b740,"a",%progbits
	.global ShindenHeya_LeaderCircleScript
ShindenHeya_LeaderCircleScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global ShindenHeya_GeraldCircleScript
ShindenHeya_GeraldCircleScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global ShindenHeya_ItemIconGrowScript
ShindenHeya_ItemIconGrowScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000f000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000f000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.global ShindenHeya_ItemIconEndScript
ShindenHeya_ItemIconEndScript:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001b
	.global ShindenHeya_TableA
ShindenHeya_TableA:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c7
	.4byte 0xc00000f6
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x000000c7
	.4byte 0x400000a4
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff000a
	.4byte 0x000000c7
	.4byte 0xc00000f6
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff000b
	.4byte 0x000000c7
	.4byte 0xc00000f6
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff000c
	.4byte 0x000000c7
	.4byte 0x400000a4
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff0014
	.4byte 0x000000c0
	.4byte 0xc00000a8
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff0015
	.4byte 0x000000c0
	.4byte 0xc00000a8
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff001d
	.4byte 0x000000c8
	.4byte 0xc00000f0
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff0020
	.4byte 0x000000c8
	.4byte 0xc00000f0
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff0023
	.4byte 0x000000c8
	.4byte 0xc00000f0
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_TableB
ShindenHeya_TableB:
	.4byte 0x00000009
	.4byte 0x00101004
	.4byte 0x00a0b014
	.4byte 0x00b0b017
	.4byte 0x01415009
	.4byte 0x01e0c005
	.4byte 0x0280304b
	.4byte 0x0290901e
	.4byte 0x02a0b048
	.4byte 0x03201001
	.4byte 0x000001ff
	.global ShindenHeya_PlacementA
ShindenHeya_PlacementA:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_PlacementB
ShindenHeya_PlacementB:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_PlacementSequenceB
ShindenHeya_PlacementSequenceB:
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_PlacementC
ShindenHeya_PlacementC:
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00013000
	.4byte 0xffff007a
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00005000
	.4byte 0xffff00dd
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_PlacementD
ShindenHeya_PlacementD:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0003d000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00003000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0000b000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x0000b000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x0003d000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00035000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_PlacementE
ShindenHeya_PlacementE:
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableA
ShindenHeya_SceneTableA:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunActorEightFacingDialogue
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001036
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001037
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableB
ShindenHeya_SceneTableB:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_DispatchBySceneId
	.4byte 0x00000000
	.4byte 0x08550009
	.4byte FieldScene_RunActorNineFlagDialogueA
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte FieldScene_RunActorNineFlagDialogueA
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001378
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte FieldScene_RunActorNineFlagDialogueB
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableC
ShindenHeya_SceneTableC:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_DispatchBySceneId
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte FieldScene_RunActorNineFlagDialogueA
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001cea
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte FieldScene_RunActorNineFlagDialogueB
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableD
ShindenHeya_SceneTableD:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunSupplementalSequenceOne
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00001168
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001169
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001167
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000116a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000116b
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte ShindenHeya_RunAltarScene
	.4byte 0x00000002
	.4byte 0x02000002
	.4byte FieldScene_RunActorEightResetSequence
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableE
ShindenHeya_SceneTableE:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunActorEightFacingDialogue
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000119e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte FieldScene_RunActorTenCountStep
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000011d8
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000011d9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000011da
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableF
ShindenHeya_SceneTableF:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunActorEightFacingDialogue
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte FieldScene_RunScene378SequenceB
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c02
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c03
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c04
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c05
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableG
ShindenHeya_SceneTableG:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte SceneDialogue_RunActorEightFollowupDialogue
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte SceneDialogue_RunActorEightFollowupDialogue
	.4byte 0x00008d15
	.4byte 0x09090008
	.4byte 0x0000190b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001952
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableH
ShindenHeya_SceneTableH:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte SceneDialogue_RunActorEightFlaggedDialogue
	.4byte 0x00000000
	.4byte 0x08450009
	.4byte 0x00001409
	.4byte 0x00008d15
	.4byte 0x08450008
	.4byte 0x0000140a
	.4byte 0x00008d15
	.4byte 0x08450009
	.4byte 0x0000140b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000171d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000171e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000171f
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000029
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShindenHeya_SceneTableI
ShindenHeya_SceneTableI:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000002a
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte SceneDialogue_RunActorEightDialogue
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001824
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200411c
Data_0200411c:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.global ShindenHeya_OwnerEffectScript
ShindenHeya_OwnerEffectScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff8000
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0xffff8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000060
	.4byte 0x00000000
	.4byte 0x0000001b
	.global ShindenHeya_SparkEndScript
ShindenHeya_SparkEndScript:
	.4byte 0x0000001b
