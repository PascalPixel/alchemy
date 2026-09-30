.syntax unified
	.thumb
	.section .text.x02008cc0,"ax",%progbits
	.global TakaraHashira_CopyCellBlock
	.thumb_func
TakaraHashira_CopyCellBlock:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	lsls r1, r1, #7
	ldr r4, [sp, #48]
	mov r10, r2
	adds r1, r1, r0
	ldr r2, .L_02008d60
	lsls r1, r1, #2
	adds r3, r4, r3
	adds r5, r1, r2
	cmp r4, r3
	bge .L_02008d4e
	str r3, [sp, #4]
	mov r6, r10
	movs r3, #128
	subs r3, r3, r6
	lsls r3, r3, #2
	mov r11, r3
	ldr r3, [sp, #40]
	lsls r3, r3, #4
	mov r9, r3
.L_02008cf6:
	ldr r0, [sp, #44]
	mov r1, r10
	adds r2, r0, r1
	cmp r0, r2
	bge .L_02008d44
	ldr r3, .L_02008d64
	movs r7, #15
	mov r8, r3
	adds r3, r4, #0
	ands r3, r7
	add r3, r9
	lsls r3, r3, #5
	ldr r6, .L_02008d68
	str r3, [sp, #0]
	mov lr, r6
	mov r12, r2
.L_02008d16:
	ldr r6, [sp, #0]
	ldmia r5!, {r1}
	adds r3, r0, #0
	mov r2, r8
	ands r3, r7
	ands r1, r2
	adds r3, r6, r3
	ldr r6, .L_02008d6c
	lsls r1, r1, #3
	adds r2, r1, r6
	ldr r2, [r2]
	lsls r3, r3, #2
	mov r6, lr
	str r2, [r3, r6]
	ldr r6, .L_02008d70
	adds r2, r1, r6
	ldr r1, .L_02008d74
	ldr r2, [r2]
	adds r3, r3, r1
	adds r0, #1
	str r2, [r3]
	cmp r0, r12
	blt .L_02008d16
.L_02008d44:
	ldr r2, [sp, #4]
	adds r4, #1
	add r5, r11
	cmp r4, r2
	blt .L_02008cf6
.L_02008d4e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_02008d60:
	.4byte gMapCellBuffer
.L_02008d64:
	.4byte 0x00000fff
.L_02008d68:
	.4byte 0x06002800
.L_02008d6c:
	.4byte gMapBlocks
.L_02008d70:
	.4byte Data_02020004
.L_02008d74:
	.4byte 0x06002840
	.section .text.x02009d84,"ax",%progbits
	.global TakaraHashira_UpdatePillarActors
	.thumb_func
TakaraHashira_UpdatePillarActors:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_02009fc0
	sub sp, #20
	movs r0, #8
	movs r2, #16
	mov r8, r1
	add r2, sp
	movs r3, #0
	str r0, [sp, #12]
	str r0, [sp, #4]
	mov r9, r2
	mov r10, r3
	mov r11, r8
.L_02009daa:
	ldr r0, [sp, #12]
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
	ldr r0, [sp, #12]
	subs r0, #8
	str r0, [sp, #8]
	mov r1, r10
	ldr r3, [r6, #8]
	mov r0, r8
	ldr r2, [r1, r0]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_02009de2
	ldr r1, [sp, #4]
	ldr r3, [r6, #16]
	ldr r2, [r1, r0]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_02009de2
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02009de2
	b .L_02009f90
.L_02009de2:
	adds r0, r6, #0
	ldr r3, .L_02009fc4
	adds r0, #8
	ldr r1, .L_02009fc8
	ldr r2, .L_02009fcc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	ldr r1, .L_02009fc4
	lsls r2, r2, #24
.L_02009df6:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_02009df6
	adds r0, r6, #0
	ldr r1, .L_02009fc8
	bl Object_CheckMovementCollision
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_02009e18
	adds r7, r6, #0
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
	b .L_02009e1c
.L_02009e18:
	adds r7, r6, #0
	adds r7, #85
.L_02009e1c:
	mov r0, r8
	mov r3, r10
	mov r5, r8
	ldr r1, [r3, r0]
	adds r5, #12
	ldr r3, [sp, #4]
	add r5, r10
	ldr r2, [r3, r0]
	movs r0, #0
	adds r3, r5, #0
	bl TakaraHashira_SetCellAttributes
	mov r0, r11
	ldr r3, [sp, #4]
	ldr r1, [r0]
	mov r0, r8
	ldr r2, [r3, r0]
	adds r3, r5, #0
	movs r0, #2
	bl TakaraHashira_SetCellAttributes
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009ed0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #2
	bl GetMapCellCollision
	cmp r0, #50
	bne .L_02009e82
	movs r0, #189
	bl Engine_AudioPlayCue
	adds r5, r6, #0
	adds r5, #35
	ldrb r3, [r5]
	movs r2, #254
	ands r2, r3
	strb r2, [r5]
	movs r1, #1
	ldr r0, [sp, #12]
	bl TakaraHashira_LowerActorToLedge
	ldrb r3, [r5]
	movs r1, #1
	orrs r3, r1
	strb r3, [r5]
	b .L_02009ecc
.L_02009e82:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #2
	bl GetMapCellCollision
	cmp r0, #51
	bne .L_02009ec6
	movs r1, #0
	adds r0, r6, #0
	bl SceneActor_WaitHeightBelowLimit
	movs r0, #189
	bl Engine_AudioPlayCue
	movs r2, #0
	str r2, [r6, #12]
	adds r5, r6, #0
	adds r5, #35
	ldrb r3, [r5]
	movs r2, #254
	ands r2, r3
	strb r2, [r5]
	ldr r0, [sp, #12]
	bl StagedActor_StepDownUntilClamp
	movs r3, #0
	str r3, [r6, #8]
	str r3, [r6, #12]
	str r3, [r6, #16]
	ldrb r3, [r5]
	movs r0, #1
	orrs r3, r0
	strb r3, [r5]
	b .L_02009ecc
.L_02009ec6:
	adds r0, r6, #0
	bl OverlayObject_WaitUntilSettledAndReset
.L_02009ecc:
	movs r1, #0
	strb r1, [r7]
.L_02009ed0:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	ldr r3, .L_02009fd0
	asrs r2, r2, #20
	asrs r1, r1, #20
	add r3, r10
	movs r0, #0
	bl TakaraHashira_ReadMapCell
	ldr r2, [r6, #12]
	cmp r2, #0
	blt .L_02009f2a
	asrs r2, r2, #20
	adds r2, #6
	movs r0, #0
	movs r1, #27
	mov r3, r9
	bl TakaraHashira_ReadMapCell
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	mov r3, r9
	bl TakaraHashira_SetCellAttributes
	mov r3, r11
	ldrb r2, [r3, #13]
	mov r0, r9
	ldrb r1, [r0, #1]
	lsrs r2, r2, #6
	movs r3, #63
	lsls r2, r2, #6
	ands r3, r1
	orrs r3, r2
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	strb r3, [r0, #1]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	mov r3, r9
	bl TakaraHashira_SetCellAttributes
.L_02009f2a:
	ldr r3, [r6, #8]
	mov r1, r8
	asrs r3, r3, #20
	mov r2, r10
	str r3, [r1, r2]
	ldr r3, [r6, #12]
	adds r2, #4
	asrs r3, r3, #20
	str r3, [r1, r2]
	ldr r3, [r6, #16]
	adds r2, #4
	asrs r3, r3, #20
	str r3, [r1, r2]
	movs r5, #0
	movs r7, #16
.L_02009f48:
	ldr r3, [sp, #8]
	cmp r5, r3
	beq .L_02009f88
	ldr r1, .L_02009fc0
	ldr r0, [r1, r7]
	str r1, [sp, #0]
	bl Engine_GameFlagClear
	adds r0, r5, #0
	adds r0, #8
	bl Object_GetById
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	ldr r1, [sp, #0]
	cmp r2, r3
	bne .L_02009f88
	ldr r2, [r6, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009f88
	ldr r2, [r6, #12]
	ldr r3, [r0, #12]
	cmp r2, r3
	ble .L_02009f88
	ldr r0, [r1, r7]
	bl Engine_GameFlagSet
.L_02009f88:
	adds r5, #1
	adds r7, #20
	cmp r5, #3
	bls .L_02009f48
.L_02009f90:
	ldr r1, [sp, #4]
	ldr r2, [sp, #12]
	movs r0, #20
	adds r1, #20
	adds r2, #1
	add r10, r0
	add r11, r0
	str r1, [sp, #4]
	str r2, [sp, #12]
	cmp r2, #11
	bhi .L_02009fa8
	b .L_02009daa
.L_02009fa8:
	bl TakaraHashira_SortPillarActors
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_02009fc0:
	.4byte TakaraHashira_PillarSlots
.L_02009fc4:
	.4byte 0x040000d4
.L_02009fc8:
	.4byte TakaraHashira_PillarSlots + 0x50
.L_02009fcc:
	.4byte 0x84000003
.L_02009fd0:
	.4byte TakaraHashira_PillarSlots + 0xc
	.section .rodata.x0200abf8,"a",%progbits
.L_0200abf8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_0200ac30:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_0200ac68:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200abf8
	.4byte .L_0200ac30
	.4byte .L_0200ac68
	.global TakaraHashira_ActionTable
TakaraHashira_ActionTable:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x00000022
	.4byte SceneActor_ApplyCounterLowBitsAsMode
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte SceneActor_ApplyCounterLowBitsAsMode
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global TakaraHashira_ShiftSteps1
TakaraHashira_ShiftSteps1:
	.4byte 0x0022004c
	.4byte 0x00020001
	.4byte 0x004d0004
	.4byte 0x00010022
	.4byte 0x00040002
	.4byte 0x0022004e
	.4byte 0x00020001
	.4byte 0x004f0004
	.4byte 0x00010022
	.4byte 0x00040002
	.2byte 0xffff
	.global TakaraHashira_ShiftSteps2
TakaraHashira_ShiftSteps2:
	.2byte 0x004f
	.4byte 0x00010022
	.4byte 0x00040002
	.4byte 0x0022004e
	.4byte 0x00020001
	.4byte 0x004d0004
	.4byte 0x00010022
	.4byte 0x00040002
	.4byte 0x0022004c
	.4byte 0x00020001
	.4byte 0xffff0004
	.global TakaraHashira_ShiftSteps3
TakaraHashira_ShiftSteps3:
	.4byte 0x00220043
	.4byte 0x00050002
	.4byte 0x00450006
	.4byte 0x00020022
	.4byte 0x00060005
	.4byte 0x00220047
	.4byte 0x00050002
	.4byte 0x00490006
	.4byte 0x00020022
	.4byte 0x00060005
	.2byte 0xffff
	.global TakaraHashira_ShiftSteps4
TakaraHashira_ShiftSteps4:
	.2byte 0x0047
	.4byte 0x00020032
	.4byte 0x00060005
	.4byte 0x00320045
	.4byte 0x00050002
	.4byte 0x00430006
	.4byte 0x00020032
	.4byte 0x00060005
	.4byte 0x00320041
	.4byte 0x00050002
	.4byte 0xffff0006
	.global TakaraHashira_ShiftSteps5
TakaraHashira_ShiftSteps5:
	.4byte 0x0026004b
	.4byte 0x00010002
	.4byte 0x004d0004
	.4byte 0x00020026
	.4byte 0x00060001
	.4byte 0x0026004f
	.4byte 0x00010002
	.4byte 0x00410008
	.4byte 0x00020035
	.4byte 0x00010001
	.4byte 0x0000ffff
	.global gTakaraHashiraEntrancesOther
gTakaraHashiraEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEntrances1
gTakaraHashiraEntrances1:
	.4byte 0xffff0001
	.4byte 0x000002c8
	.4byte 0xc00002f8
	.4byte 0x02300000
	.4byte 0x03e00170
	.4byte 0x00000318
	.4byte 0xffff0002
	.4byte 0x00000258
	.4byte 0xc00002f8
	.4byte 0x02300000
	.4byte 0x03e00170
	.4byte 0x00000318
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEntrances2
gTakaraHashiraEntrances2:
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000168
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000068
	.4byte 0xc0000168
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEntrances3
gTakaraHashiraEntrances3:
	.4byte 0xffff0001
	.4byte 0x000000b8
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000040
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEntrances4
gTakaraHashiraEntrances4:
	.4byte 0xffff0001
	.4byte 0x000002e8
	.4byte 0xc0000208
	.4byte 0x01b80000
	.4byte 0x03c00010
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000278
	.4byte 0xc0000208
	.4byte 0x01b80000
	.4byte 0x03c00010
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEntrances5
gTakaraHashiraEntrances5:
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0xc00001e8
	.4byte 0x00100000
	.4byte 0x01d00020
	.4byte 0x00000210
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0xc00001e8
	.4byte 0x00100000
	.4byte 0x01d00020
	.4byte 0x00000210
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraExits
gTakaraHashiraExits:
	.4byte 0x00000073
	.4byte 0x0010307e
	.4byte 0x0020407e
	.4byte 0x00000074
	.4byte 0x0010307f
	.4byte 0x0020407f
	.4byte 0x00000077
	.4byte 0x00103082
	.4byte 0x00204082
	.4byte 0x00000079
	.4byte 0x00103084
	.4byte 0x00204084
	.4byte 0x0000007a
	.4byte 0x00103085
	.4byte 0x00204085
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraPlacements1
gTakaraHashiraPlacements1:
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraPlacements2
gTakaraHashiraPlacements2:
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraPlacements3
gTakaraHashiraPlacements3:
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
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
	.global gTakaraHashiraPlacementsOther
gTakaraHashiraPlacementsOther:
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0xffff0100
	.4byte 0x00000001
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
	.global gTakaraHashiraPlacements5
gTakaraHashiraPlacements5:
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEventsOther
gTakaraHashiraEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEvents1
gTakaraHashiraEvents1:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte FieldScene_RunTransitionOrFallback
	.4byte 0x00000013
	.4byte 0x0ec20064
	.4byte 0x001000bb
	.4byte 0x00000013
	.4byte 0x0ec30065
	.4byte 0x001000b5
	.4byte 0x00000013
	.4byte 0x0ec40066
	.4byte 0x0020006f
	.4byte 0x00000013
	.4byte 0x0ec50067
	.4byte 0x001000c2
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEvents2
gTakaraHashiraEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte SceneState_ApplyPlacementResult
	.4byte 0x00000013
	.4byte 0x0ec60064
	.4byte 0x001000c4
	.4byte 0x00000013
	.4byte 0x0ec70065
	.4byte 0x00100018
	.4byte 0x00000013
	.4byte 0x0ec80066
	.4byte 0x002000de
	.4byte 0x00000013
	.4byte 0x0ec90067
	.4byte 0x001000bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEvents3
gTakaraHashiraEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte SceneState_ApplyPlacementResult
	.4byte 0x00000413
	.4byte 0x0ed20064
	.4byte 0x0020022b
	.4byte 0x0000c413
	.4byte 0x0ed20064
	.4byte 0x0020022b
	.4byte 0x00000413
	.4byte 0x0ed30065
	.4byte 0x001000e5
	.4byte 0x0000e413
	.4byte 0x0ed30065
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0ed40066
	.4byte 0x001000b7
	.4byte 0x00000013
	.4byte 0x0ed50067
	.4byte 0x00100062
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunFlaggedDisplayScene
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEvents4
gTakaraHashiraEvents4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte FieldScene_RunScene3b3_02001fd4
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte FieldScene_RunSingleStep
	.4byte 0x00004602
	.4byte 0xffff001f
	.4byte TakaraHashira_RunStagedCellScene
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte FieldScene_RunScene3b3_02001fd4
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte SceneState_LinkActorZeroToWork24
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte SceneState_ClearWord24AndObjectByte62
	.4byte 0x00000013
	.4byte 0x0eda0064
	.4byte 0x00200309
	.4byte 0x00000013
	.4byte 0x0edb0065
	.4byte 0x001000e5
	.4byte 0x00000413
	.4byte 0x0edc0066
	.4byte 0x001000ba
	.4byte 0x00000013
	.4byte 0x0edd0067
	.4byte 0x00100032
	.4byte 0x00008c15
	.4byte 0x02000008
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00008c15
	.4byte 0x02010009
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00008c15
	.4byte 0x0202000a
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00008c15
	.4byte 0x0203000b
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00009315
	.4byte 0x02000008
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00009315
	.4byte 0x02010009
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00009315
	.4byte 0x0202000a
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0x00009315
	.4byte 0x0203000b
	.4byte TakaraHashira_UpdatePillarActors
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraEvents5
gTakaraHashiraEvents5:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte TakaraHashira_RunPushScene
	.4byte 0x00000013
	.4byte 0x0ede0064
	.4byte 0x00200378
	.4byte 0x00000013
	.4byte 0x0edf0065
	.4byte 0x001000b7
	.4byte 0x00000013
	.4byte 0x0ee00066
	.4byte 0x0010010c
	.4byte 0x00000013
	.4byte 0x0ee10067
	.4byte 0x001000e2
	.4byte 0x10008c15
	.4byte 0x0204000a
	.4byte StagedActor_PlaceAtObjectTenCell
	.4byte 0x00008c15
	.4byte 0x0204000a
	.4byte TakaraHashira_DropActorTen
	.4byte 0x00001815
	.4byte 0x0200000b
	.4byte FieldScene_RunActor11Step
	.4byte 0x00001815
	.4byte 0x0201000c
	.4byte FieldScene_RunActor12Step
	.4byte 0x10001815
	.4byte 0x0202000d
	.4byte FieldScene_RunScene3b3_0200215c
	.4byte 0x50001815
	.4byte 0x0202000d
	.4byte TakaraHashira_RaiseActorFourteen
	.4byte 0x00001815
	.4byte 0x0202000d
	.4byte TakaraHashira_RunMapShiftScene
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 4
	.global TakaraHashira_PillarSlots
TakaraHashira_PillarSlots:
	.space 80
	.space 12
	.global TakaraHashira_ShakenScroll
TakaraHashira_ShakenScroll:
	.space 12
	.global TakaraHashira_ShakeChance
TakaraHashira_ShakeChance:
	.space 4
