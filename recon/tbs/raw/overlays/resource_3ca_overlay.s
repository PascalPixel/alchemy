.syntax unified
	.thumb
	.section .text.x02008b34,"ax",%progbits
	.global Func_02000b34
	.thumb_func
Func_02000b34:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008cb4
	ldr r3, [r3]
	sub sp, #8
	mov r10, r3
	bl Engine_EventBegin
	bl Battle_ResetEffectCounter
	movs r1, #156
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #232
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl Engine_ActorFaceDirection
	movs r0, #40
	bl Engine_EventWait
	movs r0, #140
	bl Engine_AudioPlayCue
	movs r6, #160
	movs r5, #0
	lsls r6, r6, #19
.L_02008b76:
	lsls r3, r5, #11
	lsls r2, r5, #5
	orrs r3, r2
	strh r3, [r6]
	movs r0, #10
	adds r5, #1
	bl Engine_EventWait
	cmp r5, #15
	ble .L_02008b76
	movs r2, #252
	movs r3, #160
	lsls r2, r2, #7
	lsls r3, r3, #19
	strh r2, [r3]
	ldr r2, .L_02008cb8
	movs r7, #129
	ldr r6, .L_02008cbc
	mov r8, r2
	lsls r7, r7, #4
	movs r5, #2
.L_02008ba0:
	movs r0, #212
	bl Engine_AudioPlayCue
	mov r3, r8
	strh r3, [r6]
	movs r0, #3
	bl Engine_EventWait
	strh r7, [r6]
	movs r0, #65
	subs r5, #1
	bl Engine_EventWait
	cmp r5, #0
	bge .L_02008ba0
	ldr r3, .L_02008cc0
	movs r5, #1
	str r5, [r3]
	ldr r3, .L_02008cc4
	movs r2, #0
	movs r1, #200
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_02008cc8
	mov r8, r2
	bl Scheduler_AddOrUpdateCallback
	ldr r6, .L_02008ccc
	movs r0, #20
	str r5, [r6]
	bl Engine_EventWait
	movs r0, #163
	bl Engine_AudioPlayCue
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Runtime_SetWorkTripleIfNonNegative
	movs r0, #60
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	str r5, [r6]
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Runtime_SetWorkTripleIfNonNegative
	movs r0, #60
	bl Engine_EventWait
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Runtime_SetWorkTripleIfNonNegative
	ldr r3, .L_02008cd0
	mov r2, r8
	movs r1, #200
	movs r6, #160
	movs r5, #184
	str r2, [r3]
	ldr r0, .L_02008cd4
	lsls r1, r1, #4
	lsls r6, r6, #1
	lsls r5, r5, #1
	ldr r7, .L_02008cd8
	bl Scheduler_AddOrUpdateCallback
	add r6, r10
	movs r2, #0
	add r5, r10
.L_02008c44:
	ldr r3, [r6]
	adds r3, r3, r7
	str r3, [r6]
	ldr r3, [r5]
	adds r3, r3, r7
	adds r2, r2, r7
	str r3, [r5]
	movs r0, #1
	str r2, [sp, #0]
	bl WaitFrames
	ldr r3, .L_02008cdc
	ldr r2, [sp, #0]
	cmp r2, r3
	ble .L_02008c44
	ldr r0, .L_02008cd4
	bl Engine_TaskRemoveCallback
	ldr r0, .L_02008ce0
	ldr r3, .L_02008ccc
	ldr r2, .L_02008ce4
	ldrh r1, [r0]
	movs r6, #0
	ldr r4, .L_02008cac
	str r6, [r3]
	adds r3, r2, #0
	mov r5, sp
	ands r3, r1
	orrs r3, r4
	adds r5, #6
	strh r3, [r5]
	strh r3, [r0]
	subs r0, #2
	ldrh r1, [r0]
	adds r3, r2, #0
	ands r3, r1
	orrs r3, r4
	strh r3, [r5]
	ldr r1, .L_02008ce8
	strh r3, [r0]
	ldrh r3, [r1]
	ands r2, r3
	ldr r3, .L_02008cb0
	orrs r2, r3
	ldr r3, .L_02008cc0
	movs r0, #144
	strh r2, [r5]
	str r6, [r3]
	strh r2, [r1]
	lsls r0, r0, #1
	b .L_02008cec
	.2byte 0x0000
.L_02008cac:
	.4byte 0x00000003
.L_02008cb0:
	.4byte 0x00000002
.L_02008cb4:
	.4byte gMapWork
.L_02008cb8:
	.4byte 0x00001010
.L_02008cbc:
	.4byte 0x04000052
.L_02008cc0:
	.4byte BabiFune_ShimmerActive
.L_02008cc4:
	.4byte BabiFune_ShimmerPhase
.L_02008cc8:
	.4byte BabiFune_UpdateWaves
.L_02008ccc:
	.4byte BabiFune_DriftActive
.L_02008cd0:
	.4byte BabiFune_CountTicks
.L_02008cd4:
	.4byte SceneState_CountDownEveryFortyTicks
.L_02008cd8:
	.4byte 0x00003333
.L_02008cdc:
	.4byte 0x0059ffff
.L_02008ce0:
	.4byte 0x0400000e
.L_02008ce4:
	.4byte 0x0000fffc
.L_02008ce8:
	.4byte 0x0400000a
.L_02008cec:
	bl Engine_AudioPlayCue
	movs r0, #1
	bl WaitFrames
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r2, #191
	ldr r3, .L_02008d88
	strh r2, [r3]
	movs r5, #0
	ldr r6, .L_02008d8c
.L_02008d06:
	strh r5, [r6]
	movs r0, #1
	adds r5, #1
	bl Engine_EventWait
	cmp r5, #16
	ble .L_02008d06
	movs r0, #40
	bl Engine_EventWait
	movs r0, #1
	movs r1, #1
	ldr r2, .L_02008d90
	negs r0, r0
	negs r1, r1
	bl Runtime_SetWorkTripleIfNonNegative
	movs r3, #160
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	ldr r2, .L_02008d94
	str r3, [r2]
	movs r3, #184
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	ldr r2, .L_02008d98
	str r3, [r2]
	ldr r2, .L_02008d9c
	movs r3, #1
	str r3, [r2]
	ldr r6, .L_02008d8c
	movs r5, #16
.L_02008d4a:
	strh r5, [r6]
	movs r0, #8
	subs r5, #1
	bl Engine_EventWait
	cmp r5, #0
	bge .L_02008d4a
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02008da0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #80
	bl Engine_AudioPlayCue
	bl AudioCommand_WaitForStateByteClear
	movs r0, #20
	bl Engine_EventWait
	bl BattleFx_FinishAction
	bl Scene_RunExtendedPresentationSequence
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_02008d88:
	.4byte 0x04000050
.L_02008d8c:
	.4byte 0x04000054
.L_02008d90:
	.4byte 0x0000e666
.L_02008d94:
	.4byte BabiFune_SwellLayerSixY
.L_02008d98:
	.4byte BabiFune_SwellLayerSevenY
.L_02008d9c:
	.4byte BabiFune_SwellActive
.L_02008da0:
	.4byte BabiFune_CyclePalette
	.section .rodata.x020094ac,"a",%progbits
	.global BabiFune_PaletteFrames
BabiFune_PaletteFrames:
	.4byte 0x69c05860
	.4byte 0x69c07f20
	.4byte 0x48005860
	.4byte 0x69c05860
	.4byte 0x69c07f20
	.4byte 0x00005860
	.global BabiFune_DriftScript
BabiFune_DriftScript:
	.4byte 0x0000001b
	.global BabiFune_WaveSine
BabiFune_WaveSine:
	.4byte 0x00640000
	.4byte 0x012d00c8
	.4byte 0x01f50191
	.4byte 0x02bc0259
	.4byte 0x0381031f
	.4byte 0x044403e3
	.4byte 0x050404a5
	.4byte 0x05c20563
	.4byte 0x067b061f
	.4byte 0x073106d7
	.4byte 0x07e2078a
	.4byte 0x088f0839
	.4byte 0x093608e3
	.4byte 0x09d70987
	.4byte 0x0a730a26
	.4byte 0x0b080abe
	.4byte 0x0b960b50
	.4byte 0x0c1d0bda
	.4byte 0x0c9d0c5e
	.4byte 0x0d140cd9
	.4byte 0x0d840d4d
	.4byte 0x0deb0db9
	.4byte 0x0e4a0e1c
	.4byte 0x0ea00e76
	.4byte 0x0eed0ec8
	.4byte 0x0f310f10
	.4byte 0x0f6b0f4f
	.4byte 0x0f9c0f85
	.4byte 0x0fc30fb1
	.4byte 0x0fe10fd3
	.4byte 0x0ff40fec
	.4byte 0x0ffe0ffb
	.4byte 0x0ffe1000
	.4byte 0x0ff40ffb
	.4byte 0x0fe10fec
	.4byte 0x0fc30fd3
	.4byte 0x0f9c0fb1
	.4byte 0x0f6b0f85
	.4byte 0x0f310f4f
	.4byte 0x0eed0f10
	.4byte 0x0ea00ec8
	.4byte 0x0e4a0e76
	.4byte 0x0deb0e1c
	.4byte 0x0d840db9
	.4byte 0x0d140d4d
	.4byte 0x0c9d0cd9
	.4byte 0x0c1d0c5e
	.4byte 0x0b960bda
	.4byte 0x0b080b50
	.4byte 0x0a730abe
	.4byte 0x09d70a26
	.4byte 0x09360987
	.4byte 0x088f08e3
	.4byte 0x07e20839
	.4byte 0x0731078a
	.4byte 0x067b06d7
	.4byte 0x05c2061f
	.4byte 0x05040563
	.4byte 0x044404a5
	.4byte 0x038103e3
	.4byte 0x02bc031f
	.4byte 0x01f50259
	.4byte 0x012d0191
	.4byte 0x006400c8
	.4byte 0xff9c0000
	.4byte 0xfed3ff38
	.4byte 0xfe0bfe6f
	.4byte 0xfd44fda7
	.4byte 0xfc7ffce1
	.4byte 0xfbbcfc1d
	.4byte 0xfafcfb5b
	.4byte 0xfa3efa9d
	.4byte 0xf985f9e1
	.4byte 0xf8cff929
	.4byte 0xf81ef876
	.4byte 0xf771f7c7
	.4byte 0xf6caf71d
	.4byte 0xf629f679
	.4byte 0xf58df5da
	.4byte 0xf4f8f542
	.4byte 0xf46af4b0
	.4byte 0xf3e3f426
	.4byte 0xf363f3a2
	.4byte 0xf2ecf327
	.4byte 0xf27cf2b3
	.4byte 0xf215f247
	.4byte 0xf1b6f1e4
	.4byte 0xf160f18a
	.4byte 0xf113f138
	.4byte 0xf0cff0f0
	.4byte 0xf095f0b1
	.4byte 0xf064f07b
	.4byte 0xf03df04f
	.4byte 0xf01ff02d
	.4byte 0xf00cf014
	.4byte 0xf002f005
	.4byte 0xf002f000
	.4byte 0xf00cf005
	.4byte 0xf01ff014
	.4byte 0xf03df02d
	.4byte 0xf064f04f
	.4byte 0xf095f07b
	.4byte 0xf0cff0b1
	.4byte 0xf113f0f0
	.4byte 0xf160f138
	.4byte 0xf1b6f18a
	.4byte 0xf215f1e4
	.4byte 0xf27cf247
	.4byte 0xf2ecf2b3
	.4byte 0xf363f327
	.4byte 0xf3e3f3a2
	.4byte 0xf46af426
	.4byte 0xf4f8f4b0
	.4byte 0xf58df542
	.4byte 0xf629f5da
	.4byte 0xf6caf679
	.4byte 0xf771f71d
	.4byte 0xf81ef7c7
	.4byte 0xf8cff876
	.4byte 0xf985f929
	.4byte 0xfa3ef9e1
	.4byte 0xfafcfa9d
	.4byte 0xfbbcfb5b
	.4byte 0xfc7ffc1d
	.4byte 0xfd44fce1
	.4byte 0xfe0bfda7
	.4byte 0xfed3fe6f
	.4byte 0xff9cff38
	.global BabiFune_SceneTableA
BabiFune_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x00000148
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_SceneTableB
BabiFune_SceneTableB:
	.4byte 0x000000bc
	.4byte 0x0011f0b4
	.4byte 0x000001ff
	.global BabiFune_SceneTableC
BabiFune_SceneTableC:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01f6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01f6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_SceneTableD
BabiFune_SceneTableD:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte PlayWorkspaceCueAndClearPaletteZero
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte FieldScene_Forward11fc
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte FieldScene_ConfigureFixedPointValues
	.4byte 0x0000f204
	.4byte 0xffff000a
	.4byte Func_02000b34
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_ShimmerActive
BabiFune_ShimmerActive:
	.4byte 0x00000000
	.global BabiFune_ShimmerPhase
BabiFune_ShimmerPhase:
	.4byte 0x00000000
	.global BabiFune_Count
BabiFune_Count:
	.4byte 0x00000010
	.global BabiFune_CountTicks
BabiFune_CountTicks:
	.4byte 0x00000000
	.global BabiFune_DriftActive
BabiFune_DriftActive:
	.4byte 0x00000000
	.global BabiFune_SwellActive
BabiFune_SwellActive:
	.4byte 0x00000000
	.global BabiFune_SwellPhase
BabiFune_SwellPhase:
	.4byte 0x00000000
	.global BabiFune_SwellLayerSixY
BabiFune_SwellLayerSixY:
	.4byte 0x00000000
	.global BabiFune_SwellLayerSevenY
BabiFune_SwellLayerSevenY:
	.4byte 0x00000000
	.global BabiFune_StoredSlot0
BabiFune_StoredSlot0:
	.4byte 0xffff0000
	.global BabiFune_StoredRecord1
BabiFune_StoredRecord1:
	.4byte 0xffff0000
	.global BabiFune_StoredSlot3
BabiFune_StoredSlot3:
	.4byte 0xffff0000
	.global BabiFune_StoredRecord2
BabiFune_StoredRecord2:
	.4byte 0xffff0000
	.global BabiFune_PaletteStep
BabiFune_PaletteStep:
	.4byte 0x00000000
	.global BabiFune_ActionScriptA
BabiFune_ActionScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte SceneState_StoreSlotZeroField12
	.4byte 0x00000010
	.global BabiFune_ActionScriptB
BabiFune_ActionScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffd80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte SceneData_StoreRecord1Field12
	.4byte 0x00000010
	.global BabiFune_ActionScriptC
BabiFune_ActionScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffec0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte SceneData_StoreRecord2Field12
	.4byte 0x00000010
	.global BabiFune_ActionScriptD
BabiFune_ActionScriptD:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte SceneState_StoreSlotThreeField12
	.4byte 0x00000010
	.section .bss,"aw",%nobits
	.global BabiFune_FadeSprites
BabiFune_FadeSprites:
	.space 288
	.global BabiFune_FadeStep
BabiFune_FadeStep:
	.space 2
	.global BabiFune_FadeSlot
BabiFune_FadeSlot:
	.space 2
