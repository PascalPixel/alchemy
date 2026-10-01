.syntax unified
	.thumb
	.section .text.x02009db4,"ax",%progbits
	.global FieldScene_RunPrimarySequence
	.thumb_func
FieldScene_RunPrimarySequence:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #211
	sub sp, #64
	bl Audio_PlayCue
	cmp r6, #0
	bne .L_02009df4
	movs r5, #1
	movs r0, #111
	movs r1, #57
	movs r2, #113
	movs r3, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Engine_MapCopyCellsTo
	movs r0, #111
	movs r1, #59
	movs r2, #113
	movs r3, #43
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Engine_MapCopyCellsTo
	b .L_02009e3c
.L_02009df4:
	cmp r6, #1
	bne .L_02009e1a
	movs r0, #113
	movs r1, #58
	movs r2, #112
	movs r3, #46
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Engine_MapCopyCellsTo
	movs r0, #115
	movs r1, #58
	movs r2, #113
	movs r3, #46
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Engine_MapCopyCellsTo
	b .L_02009e3c
.L_02009e1a:
	movs r5, #1
	movs r0, #115
	movs r1, #57
	movs r2, #116
	movs r3, #44
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Engine_MapCopyCellsTo
	movs r0, #113
	movs r1, #57
	movs r2, #115
	movs r3, #44
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Engine_MapCopyCellsTo
.L_02009e3c:
	mov r2, sp
	adds r2, #24
	movs r3, #7
	str r2, [sp, #16]
	str r3, [r2, #4]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	ldr r2, .L_0200a020
	mov r10, r3
	movs r3, #1
	mov r11, r2
	mov r9, r3
.L_02009e5a:
	movs r2, #0
	mov r3, r10
	str r2, [sp, #20]
	lsls r2, r3, #20
	movs r3, #203
	lsls r3, r3, #18
	subs r3, r3, r2
	mov r8, r3
	movs r3, #176
	lsls r3, r3, #18
	adds r7, r2, r3
.L_02009e70:
	ldr r3, [sp, #20]
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	beq .L_02009f68
	cmp r6, #0
	bne .L_02009ec8
	bl Engine_RandomNext
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r5, r0, #1
	adds r5, r5, r0
	lsls r3, r5, #4
	adds r5, r5, r3
	lsls r3, r5, #8
	adds r5, r5, r3
	bl Engine_RandomNext
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	str r3, [sp, #4]
	movs r3, #144
	lsls r3, r3, #12
	str r3, [sp, #8]
	ldr r3, [sp, #16]
	add r5, r11
	movs r0, #198
	str r3, [sp, #12]
	lsls r0, r0, #18
	movs r1, #0
	adds r2, r7, #0
	adds r3, r5, #0
	str r6, [sp, #0]
	bl Effect_Spawn
	b .L_02009f62
.L_02009ec8:
	cmp r6, #1
	bne .L_02009f1a
	bl Engine_RandomNext
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r5, r0, #1
	adds r5, r5, r0
	lsls r3, r5, #4
	adds r5, r5, r3
	lsls r3, r5, #8
	adds r5, r5, r3
	bl Engine_RandomNext
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	str r3, [sp, #4]
	movs r3, #144
	lsls r3, r3, #12
	movs r2, #192
	lsls r2, r2, #15
	str r3, [sp, #8]
	ldr r3, [sp, #16]
	add r5, r11
	adds r0, r7, r2
	movs r2, #0
	str r2, [sp, #0]
	str r3, [sp, #12]
	movs r1, #0
	ldr r2, .L_0200a024
	adds r3, r5, #0
	bl Effect_Spawn
	b .L_02009f62
.L_02009f1a:
	bl Engine_RandomNext
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r5, r0, #1
	adds r5, r5, r0
	lsls r3, r5, #4
	adds r5, r5, r3
	lsls r3, r5, #8
	adds r5, r5, r3
	bl Engine_RandomNext
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	movs r2, #0
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #16]
	movs r3, #144
	lsls r3, r3, #12
	add r5, r11
	str r3, [sp, #8]
	str r2, [sp, #12]
	mov r0, r8
	movs r1, #0
	ldr r2, .L_0200a028
	adds r3, r5, #0
	bl Effect_Spawn
.L_02009f62:
	movs r0, #1
	bl Battle_WaitMode0
.L_02009f68:
	ldr r3, .L_0200a02c
	add r8, r3
	ldr r3, [sp, #20]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, #1
	adds r7, r7, r2
	str r3, [sp, #20]
	cmp r3, #7
	bhi .L_02009f7e
	b .L_02009e70
.L_02009f7e:
	cmp r6, #0
	bne .L_02009fac
	mov r2, r9
	mov r3, r10
	adds r3, #43
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r0, #111
	movs r1, #58
	movs r2, #113
	bl Engine_MapCopyCellsTo
	mov r2, r9
	mov r3, r10
	str r2, [sp, #0]
	str r2, [sp, #4]
	adds r3, #44
	movs r0, #111
	movs r1, #59
	movs r2, #113
	bl Engine_MapCopyCellsTo
	b .L_0200a002
.L_02009fac:
	cmp r6, #1
	bne .L_02009fd6
	mov r2, r10
	adds r2, #113
	movs r0, #114
	movs r1, #58
	movs r3, #46
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Engine_MapCopyCellsTo
	mov r2, r10
	adds r2, #114
	movs r0, #115
	movs r1, #58
	movs r3, #46
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Engine_MapCopyCellsTo
	b .L_0200a002
.L_02009fd6:
	mov r3, r10
	movs r2, #115
	subs r2, r2, r3
	mov r3, r9
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #114
	movs r1, #57
	movs r3, #44
	bl Engine_MapCopyCellsTo
	mov r3, r10
	movs r2, #114
	subs r2, r2, r3
	mov r3, r9
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #113
	movs r1, #57
	movs r3, #44
	bl Engine_MapCopyCellsTo
.L_0200a002:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #1
	bhi .L_0200a00e
	b .L_02009e5a
.L_0200a00e:
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0200a020:
	.4byte 0xffff3334
.L_0200a024:
	.4byte 0x02ea0000
.L_0200a028:
	.4byte 0x02ca0000
.L_0200a02c:
	.4byte 0xffff0000
	.section .rodata.x0200dca8,"a",%progbits
.L_0200dca8:
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
.L_0200dce0:
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
.L_0200dd18:
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
	.global MakyuriHeya_RiseScript
MakyuriHeya_RiseScript:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
	.global MakyuriHeya_SparkScript
MakyuriHeya_SparkScript:
	.4byte 0x00000022
	.4byte SceneEffect_AdvanceAnchoredRiseFrame
	.4byte 0x0000001b
	.global Makyuri_RampScript
Makyuri_RampScript:
	.4byte 0x00000022
	.4byte SceneEffect_AdvanceScaleOverSixteenFrames
	.4byte 0x00000010
	.global Makyuri_ScaleCounterScript
Makyuri_ScaleCounterScript:
	.4byte 0x00000022
	.4byte OverlayObject_StepScaleUpSixteenFrames
	.4byte 0x0000001b
	.global Makyuri_PillarScript
Makyuri_PillarScript:
	.4byte 0x00000022
	.4byte Makyuri_FollowLeader
	.4byte 0x00000010
	.global Makyuri_MoveAngles
Makyuri_MoveAngles:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200dca8
	.4byte .L_0200dce0
	.4byte .L_0200dd18
	.global MakyuriHeya_SparkBurstScript
MakyuriHeya_SparkBurstScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x00000004
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
	.global MakyuriHeya_PushScriptA
MakyuriHeya_PushScriptA:
	.4byte 0x00000000
	.4byte 0x0000002d
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000022
	.4byte SceneData_LoadBlockA2c5
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000010
	.global MakyuriHeya_PushScriptB
MakyuriHeya_PushScriptB:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000022
	.4byte SceneData_LoadBlockA2c5
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte SceneData_ApplyTableA2c5AndReturnZero
	.4byte 0x00000010
	.global MakyuriHeya_PushScriptD
MakyuriHeya_PushScriptD:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000022
	.4byte SceneData_LoadBlockA2c5
	.4byte 0x00000003
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte SceneData_ApplyTableA2c5AndReturnZero
	.4byte 0x00000010
	.global MakyuriHeya_PushScriptC
MakyuriHeya_PushScriptC:
	.4byte 0x00000000
	.4byte 0x0000002d
	.4byte 0x00000022
	.4byte SceneData_LoadBlockA2c5
	.4byte 0x00000010
	.global gPaletteCycleRed
gPaletteCycleRed:
	.4byte 0x00000000
	.global gPaletteCycleGreen
gPaletteCycleGreen:
	.4byte 0x00000000
	.global gPaletteCycleBlue
gPaletteCycleBlue:
	.4byte 0x00000000
	.global MakyuriHeya_GateCells
MakyuriHeya_GateCells:
	.4byte 0x0001000f
	.4byte 0x00020001
	.4byte 0x000d000a
	.4byte 0x00010001
	.4byte 0x00060002
	.4byte 0x0001000b
	.4byte 0x00020001
	.4byte 0x00090006
	.4byte 0x00010001
	.4byte 0x00060002
	.2byte 0xffff
	.global MakyuriHeya_GateCloseCells
MakyuriHeya_GateCloseCells:
	.2byte 0x0009
	.4byte 0x00010001
	.4byte 0x000a0002
	.4byte 0x0001000b
	.4byte 0x00020001
	.4byte 0x000d0006
	.4byte 0x00010001
	.4byte 0x00060002
	.4byte 0x0001000f
	.4byte 0x00020001
	.4byte 0xffff0006
	.global MakyuriHeya_FloorSwitchCells
MakyuriHeya_FloorSwitchCells:
	.4byte 0x001c0010
	.4byte 0x00020001
	.4byte 0x000e000a
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x001c000c
	.4byte 0x00020001
	.4byte 0x000a0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.2byte 0xffff
	.global MakyuriHeya_FloorSwitchCloseCells
MakyuriHeya_FloorSwitchCloseCells:
	.2byte 0x000a
	.4byte 0x0001001c
	.4byte 0x000a0002
	.4byte 0x001c000c
	.4byte 0x00020001
	.4byte 0x000e0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x001c0010
	.4byte 0x00020001
	.4byte 0xffff0006
	.global MakyuriHeya_ColumnCells
MakyuriHeya_ColumnCells:
	.4byte 0x0023007e
	.4byte 0x00020001
	.4byte 0x007e0002
	.4byte 0x00010026
	.4byte 0x00020002
	.4byte 0x0029007e
	.4byte 0x00020001
	.4byte 0x007e0002
	.4byte 0x0001002c
	.4byte 0x00020002
	.4byte 0x002f007e
	.4byte 0x00020001
	.4byte 0xffff0002
	.global gMakyuriHeyaEntrancesOther
gMakyuriHeyaEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000038
	.4byte 0xc00001b8
	.4byte 0x00100000
	.4byte 0x01f00010
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0xc00001b8
	.4byte 0x00100000
	.4byte 0x01f00010
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0x00100000
	.4byte 0x01f00010
	.4byte 0x000001c0
	.4byte 0xffff0004
	.4byte 0x000002f8
	.4byte 0xc0000158
	.4byte 0x02200000
	.4byte 0x03300010
	.4byte 0x00000160
	.4byte 0xffff0005
	.4byte 0x00000078
	.4byte 0xc00003a8
	.4byte 0x00400000
	.4byte 0x01800310
	.4byte 0x000003b0
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0xc00003a8
	.4byte 0x00400000
	.4byte 0x01800310
	.4byte 0x000003b0
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x40000248
	.4byte 0x00500000
	.4byte 0x01400210
	.4byte 0x000002b0
	.4byte 0xffff0008
	.4byte 0x00000118
	.4byte 0x40000248
	.4byte 0x00500000
	.4byte 0x01400210
	.4byte 0x000002b0
	.4byte 0xffff0009
	.4byte 0x00000308
	.4byte 0x400001d8
	.4byte 0x02800000
	.4byte 0x038001a0
	.4byte 0x000002c0
	.4byte 0xffff000a
	.4byte 0x00000358
	.4byte 0x40000208
	.4byte 0x02800000
	.4byte 0x038001a0
	.4byte 0x000002c0
	.4byte 0xffff000b
	.4byte 0x000001d8
	.4byte 0xc00002b8
	.4byte 0x01600000
	.4byte 0x02500200
	.4byte 0x000002d0
	.4byte 0xffff000f
	.4byte 0x000002c8
	.4byte 0x40000108
	.4byte 0x02200000
	.4byte 0x03300010
	.4byte 0x00000160
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEntrances2
gMakyuriHeyaEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x400001d8
	.4byte 0x00200000
	.4byte 0x01e00190
	.4byte 0x00000350
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0xc0000338
	.4byte 0x00200000
	.4byte 0x01e00190
	.4byte 0x00000350
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x400000b8
	.4byte 0x00300000
	.4byte 0x02500010
	.4byte 0x000001a0
	.4byte 0xffff0004
	.4byte 0x000001e8
	.4byte 0x40000088
	.4byte 0x00300000
	.4byte 0x02500010
	.4byte 0x000001a0
	.4byte 0xffff0005
	.4byte 0x000002d8
	.4byte 0x40000058
	.4byte 0x02900000
	.4byte 0x03800010
	.4byte 0x00000110
	.4byte 0xffff0006
	.4byte 0x000002d8
	.4byte 0x400000d8
	.4byte 0x02900000
	.4byte 0x03800010
	.4byte 0x00000110
	.4byte 0xffff0007
	.4byte 0x00000278
	.4byte 0xc00002c8
	.4byte 0x02400000
	.4byte 0x034001e0
	.4byte 0x000002e0
	.4byte 0xffff0008
	.4byte 0x00000318
	.4byte 0xc00002c8
	.4byte 0x02400000
	.4byte 0x034001e0
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x000002c8
	.4byte 0x40000238
	.4byte 0x02400000
	.4byte 0x034001e0
	.4byte 0x000002e0
	.4byte 0xffff000a
	.4byte 0x00000378
	.4byte 0xc00001b8
	.4byte 0x02f80000
	.4byte 0x04100130
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEntrances3
gMakyuriHeyaEntrances3:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000058
	.4byte 0xc00000b8
	.4byte 0x00200000
	.4byte 0x02200020
	.4byte 0x000000d0
	.4byte 0xffff0002
	.4byte 0x000001f8
	.4byte 0xc00000b8
	.4byte 0x00200000
	.4byte 0x02200020
	.4byte 0x000000d0
	.4byte 0xffff0003
	.4byte 0x00000108
	.4byte 0x40000078
	.4byte 0x00200000
	.4byte 0x02200020
	.4byte 0x000000d0
	.4byte 0xffff0004
	.4byte 0x00000288
	.4byte 0xc00000a8
	.4byte 0x02500000
	.4byte 0x03e00010
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x00000308
	.4byte 0x40000058
	.4byte 0x02500000
	.4byte 0x03e00010
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x000003a8
	.4byte 0x40000058
	.4byte 0x02500000
	.4byte 0x03e00010
	.4byte 0x00000140
	.4byte 0xffff0007
	.4byte 0x00000058
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x01200110
	.4byte 0x000001b0
	.4byte 0xffff0008
	.4byte 0x000000a8
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x01200110
	.4byte 0x000001b0
	.4byte 0xffff0009
	.4byte 0x000000f8
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x01200110
	.4byte 0x000001b0
	.4byte 0xffff000a
	.4byte 0x000001d8
	.4byte 0x40000148
	.4byte 0x01600000
	.4byte 0x02500110
	.4byte 0x000001b0
	.4byte 0xffff000b
	.4byte 0x000001a8
	.4byte 0xc0000198
	.4byte 0x01600000
	.4byte 0x02500110
	.4byte 0x000001b0
	.4byte 0xffff000c
	.4byte 0x00000058
	.4byte 0x40000258
	.4byte 0x00300000
	.4byte 0x02200200
	.4byte 0x000002a0
	.4byte 0xffff000d
	.4byte 0x000001c8
	.4byte 0x40000258
	.4byte 0x00300000
	.4byte 0x02200200
	.4byte 0x000002a0
	.4byte 0xffff000e
	.4byte 0x00000378
	.4byte 0x400002c8
	.4byte 0x02b00000
	.4byte 0x03a00180
	.4byte 0x00000330
	.4byte 0xffff000f
	.4byte 0x000000a8
	.4byte 0x40000170
	.4byte 0x00280000
	.4byte 0x01280100
	.4byte 0x000001c0
	.4byte 0xffff0010
	.4byte 0x000002f8
	.4byte 0x40000248
	.4byte 0x02b00000
	.4byte 0x03a00180
	.4byte 0x00000330
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEntrances4
gMakyuriHeyaEntrances4:
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x40000068
	.4byte 0x00400000
	.4byte 0x01200020
	.4byte 0x000001f8
	.4byte 0xffff0002
	.4byte 0x000000e8
	.4byte 0xc00001e8
	.4byte 0x00400000
	.4byte 0x01200020
	.4byte 0x000001f8
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x40000068
	.4byte 0x01600000
	.4byte 0x02500020
	.4byte 0x00000148
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc0000138
	.4byte 0x01600000
	.4byte 0x02500020
	.4byte 0x00000148
	.4byte 0xffff000a
	.4byte 0x00000318
	.4byte 0x400000d8
	.4byte 0x02980000
	.4byte 0x0398007c
	.4byte 0x00000124
	.4byte 0xffff000b
	.4byte 0x00000348
	.4byte 0xc0000358
	.4byte 0x02400000
	.4byte 0x03c00148
	.4byte 0x00000390
	.4byte 0xffff000c
	.4byte 0x00000348
	.4byte 0x40000258
	.4byte 0x02400000
	.4byte 0x03c00148
	.4byte 0x00000390
	.4byte 0xffff000d
	.4byte 0x00000368
	.4byte 0x40000258
	.4byte 0x02d00000
	.4byte 0x03c00220
	.4byte 0x00000390
	.4byte 0xffff000f
	.4byte 0x000002a8
	.4byte 0x40000248
	.4byte 0x02400000
	.4byte 0x03c00148
	.4byte 0x00000390
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriHeya_SceneTable
MakyuriHeya_SceneTable:
	.4byte 0x00000036
	.4byte 0x0010b035
	.4byte 0x0020c035
	.4byte 0x00309036
	.4byte 0x0040f035
	.4byte 0x0050e035
	.4byte 0x00607038
	.4byte 0x00703037
	.4byte 0x00806035
	.4byte 0x00903036
	.4byte 0x00a05036
	.4byte 0x00b0a036
	.4byte 0x00c01037
	.4byte 0x00d19035
	.4byte 0x00000037
	.4byte 0x0010b036
	.4byte 0x00202037
	.4byte 0x00301038
	.4byte 0x00401039
	.4byte 0x00503038
	.4byte 0x00606036
	.4byte 0x00704037
	.4byte 0x00805038
	.4byte 0x00906038
	.4byte 0x00a02038
	.4byte 0x00000038
	.4byte 0x00105037
	.4byte 0x00209037
	.4byte 0x0030a037
	.4byte 0x00408038
	.4byte 0x00507037
	.4byte 0x00608037
	.4byte 0x00708036
	.4byte 0x0080e038
	.4byte 0x00904038
	.4byte 0x00a03039
	.4byte 0x00b0d038
	.4byte 0x00c02039
	.4byte 0x00d0b038
	.4byte 0x00e09038
	.4byte 0x00f0f038
	.4byte 0x01010038
	.4byte 0x00000039
	.4byte 0x00106037
	.4byte 0x0020c038
	.4byte 0x0030a038
	.4byte 0x0040a039
	.4byte 0x00504039
	.4byte 0x0060a002
	.4byte 0x00701035
	.4byte 0x0085a002
	.4byte 0x0090503a
	.4byte 0x000001ff
	.global gMakyuriHeyaPlacementsOther
gMakyuriHeyaPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements1
gMakyuriHeyaPlacements1:
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x012b0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements2
gMakyuriHeyaPlacements2:
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x024b0000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x0045005b
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements3
gMakyuriHeyaPlacements3:
	.4byte 0x187700e0
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0x087700e0
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000007
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x027b0000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaPlacements4
gMakyuriHeyaPlacements4:
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x0002c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x01024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x188100e0
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x1881006c
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00014000
	.4byte 0x1881006a
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0x18810067
	.4byte 0x00000002
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0x1881006f
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEvents1
gMakyuriHeyaEvents1:
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0035
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff003b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff003e
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff0039
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0037
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0038
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x18750020
	.4byte FieldScene_Forward646c
	.4byte 0x00000202
	.4byte 0x1875001f
	.4byte FieldScene_RunSingleStep
	.4byte 0x00000002
	.4byte 0x1875001f
	.4byte FieldScene_Forward646c
	.4byte 0x00000a02
	.4byte 0x18750015
	.4byte FieldScene_RunSingleStep
	.4byte 0x00000002
	.4byte 0x18750015
	.4byte FieldScene_CallHelper67e8
	.4byte 0x00000202
	.4byte 0x1875000b
	.4byte FieldScene_RunSingleStep
	.4byte 0x00000002
	.4byte 0x1875000b
	.4byte FieldScene_CallHelper6364
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte FieldScene_RunScene39cSequenceB
	.4byte 0x00000202
	.4byte 0xffff0019
	.4byte MakyuriHeya_RunPushedBlockScene
	.4byte 0x00000000
	.4byte 0x08730003
	.4byte MsgMakyuriHeyaThePathIsBlockedAgainWhat
	.4byte 0x00008d15
	.4byte 0x08730003
	.4byte MsgMakyuriHeyaThisStatueWasntHereBeforeI
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte OverlayObject_ReleasePublishedAttachmentB
	.4byte 0x00008c15
	.4byte 0x08730008
	.4byte MakyuriHeya_RunPartyScene
	.4byte 0x00000013
	.4byte 0x0f640064
	.4byte 0x001000e3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEvents2
gMakyuriHeyaEvents2:
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0038
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0039
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0035
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0037
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff003a
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff003b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000a
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte FieldScene_RunScene39cSequenceB
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte MakyuriHeya_RunSequenceE
	.4byte 0x00000002
	.4byte 0x13020002
	.4byte MakyuriHeya_WalkLeaderIn
	.4byte 0x00000002
	.4byte 0x13020003
	.4byte MakyuriHeya_WalkLeaderIn
	.4byte 0x00000002
	.4byte 0x13020004
	.4byte MakyuriHeya_WalkLeaderIn
	.4byte 0x00000202
	.4byte 0x0874000a
	.4byte FieldScene_RunFourCallSequence
	.4byte 0x00000002
	.4byte 0x0874000c
	.4byte MakyuriHeya_TriggerSmallFloorSwitch
	.4byte 0x00000002
	.4byte 0x0874000a
	.4byte MakyuriHeya_CloseFloorSwitch
	.4byte 0x00000002
	.4byte 0x0874000d
	.4byte MakyuriHeya_CloseFloorSwitch
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte MakyuriHeya_RunSequenceE
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte MakyuriHeya_WalkLeaderIn
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte SceneState_ApplyPair12And21
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunScriptedSteps0And953
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte OverlayObject_ReleasePublishedAttachmentB
	.4byte 0x00008c15
	.4byte 0x0874000b
	.4byte FieldScene_RunActorElevenAtTile5And13
	.4byte 0x00000013
	.4byte 0x0f660065
	.4byte 0x001000ba
	.4byte 0x00000013
	.4byte 0x0f670066
	.4byte 0x001000b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEvents3
gMakyuriHeyaEvents3:
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0035
	.4byte SceneState_ApplyWork16cMinus50A
	.4byte 0x00000031
	.4byte 0xffff0039
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff003b
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff003a
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000b
	.4byte 0x00000021
	.4byte 0xffff003c
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff003e
	.4byte 0x0000000c
	.4byte 0x0000c602
	.4byte 0xffff003f
	.4byte SceneState_ApplyWork16cMinus50
	.4byte 0x00000001
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0037
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0038
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0040
	.4byte 0x0000000e
	.4byte 0x00000202
	.4byte 0x0878000a
	.4byte FieldScene_RunFourSteps
	.4byte 0x00000202
	.4byte 0x0878000c
	.4byte FieldScene_RunFourSteps
	.4byte 0x00000002
	.4byte 0x0878000c
	.4byte MakyuriHeya_TriggerFloorSwitch
	.4byte 0x00000002
	.4byte 0x0878000a
	.4byte FieldScene_RunScene39cSequenceA
	.4byte 0x00000202
	.4byte 0xffff0028
	.4byte MakyuriHeya_RunColumnProbeScene
	.4byte 0x00000202
	.4byte 0xffff002f
	.4byte FieldScene_RunFourStepSequence
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunScriptedSteps0And953
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte OverlayObject_ReleasePublishedAttachmentB
	.4byte 0x00008c15
	.4byte 0x08780008
	.4byte SceneState_RunWhenActor8AtTile10x23
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte MakyuriHeya_SyncBlockFlags
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte MakyuriHeya_SyncBlockFlags
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte MakyuriHeya_SyncBlockFlags
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte MakyuriHeya_SyncBlockFlags
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte MakyuriHeya_SyncBlockFlags
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte SceneState_ApplyRectWhenActor20AtColumn28
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gMakyuriHeyaEventsOther
gMakyuriHeyaEventsOther:
	.4byte 0x00000001
	.4byte 0xffff002c
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0034
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0035
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0036
	.4byte 0x00000004
	.4byte 0x0000c602
	.4byte 0xffff0037
	.4byte SceneState_ApplyWork16cMinus50B
	.4byte 0x00000000
	.4byte 0x08700003
	.4byte FieldScene_RunActorThreeBranchSequence
	.4byte 0x00008d15
	.4byte 0x08700403
	.4byte FieldScene_RunActorThreeBranchSequence
	.4byte 0x00000000
	.4byte 0x08710003
	.4byte SceneDialogue_RunActor3TimedLine
	.4byte 0x00008d15
	.4byte 0x08710003
	.4byte MsgMakyuriHeyaSomeoneDoesntWantMeToGet
	.4byte 0x00000000
	.4byte 0x1881000e
	.4byte MsgMakyuriHeyaTheLegendsSaidThatIfThe
	.4byte 0x00000000
	.4byte 0x1881000f
	.4byte MsgMakyuriHeyaOurFountainIsBackTheySay
	.4byte 0x00000000
	.4byte 0x18810010
	.4byte MsgMakyuriHeyaIDrankTheHealingWaterAnd
	.4byte 0x00000000
	.4byte 0x18810011
	.4byte MsgMakyuriHeyaWeDontHaveToFearAny
	.4byte 0x00008d15
	.4byte 0x1881000e
	.4byte MsgMakyuriHeyaHowDidTheFountainKnowThat
	.4byte 0x00008d15
	.4byte 0x1881000f
	.4byte MsgMakyuriHeyaAllThatLivesThatMeansIt
	.4byte 0x00008d15
	.4byte 0x18810010
	.4byte MsgMakyuriHeyaIKnewItWouldWorkBut
	.4byte 0x00008d15
	.4byte 0x18810011
	.4byte MsgMakyuriHeyaINeverBelievedThoseStoriesBut
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte SceneDialogue_RunLine1637
	.4byte 0x00000003
	.4byte 0xffff002a
	.4byte FieldScene_RunScriptedSteps0And1576
	.4byte 0x00000003
	.4byte 0x02000005
	.4byte FieldScene_RunFlag881Dialogue
	.4byte 0x00008c15
	.4byte 0x082b000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0x0871000a
	.4byte FieldScene_RunColumnChoreography
	.4byte 0x00005d15
	.4byte 0xffff000c
	.4byte FieldScene_RunRandomEffectActorSequence
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte OverlayObject_ReleasePublishedAttachmentB
	.4byte 0x0000b904
	.4byte 0x18810005
	.4byte FieldScene_RunActor184Sequence
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
