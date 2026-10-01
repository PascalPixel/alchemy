.syntax unified
	.thumb
	.section .text.x02008194,"ax",%progbits
	.global Func_02000194
	.thumb_func
Func_02000194:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008360
	ldr r3, [r3]
	mov r9, r3
	ldr r3, .L_02008364
	ldr r3, [r3]
	sub sp, #20
	cmp r3, #0
	beq .L_020081e0
	ldr r5, .L_02008368
	ldr r0, [r5]
	lsls r0, r0, #9
	bl Engine_MathSin
	ldr r3, .L_0200836c
	movs r1, #3
	mov r12, pc
	bx r3
	ldr r3, .L_02008370
	adds r0, #8
	ldr r3, [r3]
	mov r2, sp
	lsls r0, r0, #8
	adds r2, #18
	adds r3, r3, r0
	strh r3, [r2]
	ldrh r2, [r2]
	ldr r3, .L_02008374
	strh r2, [r3]
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
.L_020081e0:
	ldr r3, .L_02008378
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020082bc
	ldr r2, .L_0200837c
	ldr r0, [r2]
	lsls r0, r0, #9
	mov r11, r2
	bl Engine_MathSin
	ldr r3, .L_0200836c
	movs r1, #2
	mov r12, pc
	bx r3
	ldr r3, .L_02008380
	movs r2, #160
	ldr r3, [r3]
	lsls r6, r0, #16
	lsls r2, r2, #1
	add r2, r9
	adds r3, r3, r6
	str r3, [r2]
	ldr r3, .L_02008384
	movs r2, #184
	ldr r3, [r3]
	lsls r2, r2, #1
	add r2, r9
	adds r3, r3, r6
	str r3, [r2]
	ldr r3, .L_02008388
	ldr r2, .L_0200838c
	mov r8, r3
	ldr r3, [r3]
	mov r10, r2
	cmp r3, r10
	beq .L_02008242
	movs r0, #0
	bl Object_GetById
	mov r2, r8
	ldr r3, [r2]
	adds r5, r0, #0
	adds r3, r3, r6
	adds r2, r5, #0
	str r3, [r5, #12]
	str r3, [r5, #20]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008242:
	ldr r7, .L_02008390
	ldr r3, [r7]
	cmp r3, r10
	beq .L_02008268
	movs r0, #1
	bl Object_GetById
	ldr r3, [r7]
	adds r5, r0, #0
	adds r3, r3, r6
	str r3, [r5, #12]
	mov r2, r8
	ldr r3, [r2]
	adds r2, r5, #0
	adds r3, r3, r6
	str r3, [r5, #20]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008268:
	ldr r7, .L_02008394
	ldr r3, [r7]
	cmp r3, r10
	beq .L_0200828e
	movs r0, #3
	bl Object_GetById
	ldr r3, [r7]
	adds r5, r0, #0
	adds r3, r3, r6
	str r3, [r5, #12]
	mov r2, r8
	ldr r3, [r2]
	adds r2, r5, #0
	adds r3, r3, r6
	str r3, [r5, #20]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200828e:
	ldr r7, .L_02008398
	ldr r3, [r7]
	cmp r3, r10
	beq .L_020082b4
	movs r0, #2
	bl Object_GetById
	ldr r3, [r7]
	adds r5, r0, #0
	adds r3, r3, r6
	str r3, [r5, #12]
	mov r2, r8
	ldr r3, [r2]
	adds r2, r5, #0
	adds r3, r3, r6
	str r3, [r5, #20]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_020082b4:
	mov r2, r11
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_020082bc:
	ldr r3, .L_0200839c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020083ac
	ldr r3, .L_020083a0
	ldr r3, [r3]
	movs r7, #1
	ands r3, r7
	cmp r3, #0
	beq .L_020083ac
	mov r3, r9
	adds r3, #228
	ldr r6, [r3]
	adds r3, #4
	ldr r2, .L_0200838c
	ldr r5, [r3]
	ands r6, r2
	ands r5, r2
	bl Engine_RandomNext
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #4
	add r2, sp, #4
	adds r6, r6, r3
	movs r3, #0
	str r3, [r2, #4]
	str r6, [r2]
	str r2, [sp, #0]
	bl Engine_RandomNext
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #5
	adds r5, r5, r3
	movs r3, #240
	ldr r2, [sp, #0]
	lsls r3, r3, #13
	adds r5, r5, r3
	str r5, [r2, #8]
	ldr r1, [r2]
	adds r3, r5, #0
	ldr r2, [r2, #4]
	ldr r0, .L_020083a4
	bl Object_Create
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020083ac
	ldr r3, .L_020083a8
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #100
	movs r3, #60
	strh r3, [r2]
	adds r3, r5, #0
	ldr r1, .L_0200835c
	adds r3, #102
	strh r7, [r3]
	subs r3, #17
	strb r1, [r3]
	subs r2, #65
	movs r3, #2
	strb r3, [r2]
	ldr r1, [r5, #80]
	ldrb r2, [r1, #9]
	subs r3, #15
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #0
	bl Object_SetMode
	b .L_020083ac
	.2byte 0x0000
.L_0200835c:
	.4byte 0x00000000
.L_02008360:
	.4byte gMapWork
.L_02008364:
	.4byte Data_020017e8
.L_02008368:
	.4byte Data_020017ec
.L_0200836c:
	.4byte IwramMulQ16ReturnIp
.L_02008370:
	.4byte BabiFune_Count
.L_02008374:
	.4byte 0x04000052
.L_02008378:
	.4byte Data_020017fc
.L_0200837c:
	.4byte Data_02001800
.L_02008380:
	.4byte Data_02001804
.L_02008384:
	.4byte Data_02001808
.L_02008388:
	.4byte BabiFune_StoredSlot0
.L_0200838c:
	.4byte 0xffff0000
.L_02008390:
	.4byte BabiFune_StoredRecord1
.L_02008394:
	.4byte BabiFune_StoredSlot3
.L_02008398:
	.4byte BabiFune_StoredRecord2
.L_0200839c:
	.4byte Data_020017f8
.L_020083a0:
	.4byte gFrameCount
.L_020083a4:
	.4byte 0x000001f7
.L_020083a8:
	.4byte BabiFune_UpdateDriftingObject
.L_020083ac:
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
	.4byte Data_020017e8
.L_02008cc4:
	.4byte Data_020017ec
.L_02008cc8:
	.4byte Func_02000194
.L_02008ccc:
	.4byte Data_020017f8
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
	.4byte Data_02001804
.L_02008d98:
	.4byte Data_02001808
.L_02008d9c:
	.4byte Data_020017fc
.L_02008da0:
	.4byte BabiFune_CyclePalette
	.section .text.x020091c4,"ax",%progbits
	.global BabiFune_StepFade
	.thumb_func
BabiFune_StepFade:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009298
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r2, .L_0200929c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r2, .L_020092a0
	ldrh r3, [r3, #2]
	mov lr, r2
	lsrs r3, r3, #5
	mov r1, lr
	mov r12, r3
	movs r4, #0
	ldrsh r3, [r1, r4]
	ldr r0, .L_020092a4
	ldrh r2, [r2]
	cmp r3, #0
	beq .L_020091f0
	subs r3, r2, #1
	mov r2, lr
	strh r3, [r2]
.L_020091f0:
	movs r5, #0
.L_020091f2:
	mov r4, lr
	ldrh r3, [r4]
	lsls r4, r3, #16
	asrs r1, r4, #16
	negs r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #0
	movs r7, #255
	stmia r0!, {r2}
	ands r3, r7
	lsls r2, r5, #21
	ldr r6, .L_020092a8
	orrs r3, r2
	orrs r3, r6
	stmia r0!, {r3}
	mov r2, r12
	adds r5, #1
	stmia r0!, {r2}
	cmp r5, #7
	bls .L_020091f2
	lsrs r3, r4, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	adds r2, r3, #0
	adds r2, #136
	ands r2, r7
	movs r5, #0
	movs r7, #0
	adds r4, r6, #0
	adds r1, r0, #0
.L_02009232:
	lsls r3, r5, #21
	orrs r3, r2
	orrs r3, r4
	str r3, [r1, #4]
	adds r5, #1
	mov r3, r12
	str r7, [r1]
	str r3, [r1, #8]
	adds r0, #12
	adds r1, #12
	cmp r5, #7
	bls .L_02009232
	ldr r3, .L_020092a0
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	adds r2, #152
	movs r3, #255
	ldr r4, .L_020092a8
	movs r5, #0
	ands r2, r3
	movs r6, #0
	adds r1, r0, #0
.L_02009266:
	lsls r3, r5, #21
	orrs r3, r2
	orrs r3, r4
	str r3, [r1, #4]
	adds r5, #1
	mov r3, r12
	str r6, [r1]
	str r3, [r1, #8]
	adds r1, #12
	cmp r5, #7
	bls .L_02009266
	ldr r6, .L_020092a4
	movs r5, #0
.L_02009280:
	adds r0, r6, #0
	movs r1, #255
	adds r5, #1
	bl Runtime_PushSlotEntry
	adds r6, #12
	cmp r5, #23
	bls .L_02009280
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_02009298:
	.4byte BabiFune_FadeSlot
.L_0200929c:
	.4byte ResourceTableEntries
.L_020092a0:
	.4byte BabiFune_FadeStep
.L_020092a4:
	.4byte Data_02001af8
.L_020092a8:
	.4byte 0x80004000
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
	.global Data_020017e8
Data_020017e8:
	.4byte 0x00000000
	.global Data_020017ec
Data_020017ec:
	.4byte 0x00000000
	.global BabiFune_Count
BabiFune_Count:
	.4byte 0x00000010
	.global BabiFune_CountTicks
BabiFune_CountTicks:
	.4byte 0x00000000
	.global Data_020017f8
Data_020017f8:
	.4byte 0x00000000
	.global Data_020017fc
Data_020017fc:
	.4byte 0x00000000
	.global Data_02001800
Data_02001800:
	.4byte 0x00000000
	.global Data_02001804
Data_02001804:
	.4byte 0x00000000
	.global Data_02001808
Data_02001808:
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
	.global Data_02001af8
Data_02001af8:
	.space 288
	.global BabiFune_FadeStep
BabiFune_FadeStep:
	.space 2
	.global BabiFune_FadeSlot
BabiFune_FadeSlot:
	.space 2
