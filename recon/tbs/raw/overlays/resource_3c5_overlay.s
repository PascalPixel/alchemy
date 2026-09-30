.syntax unified
	.thumb
	.section .text.x0200a8a0,"ax",%progbits
	.global BabiIriguchi_SetupScene
	.thumb_func
BabiIriguchi_SetupScene:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #1
	sub sp, #8
	bl WaitFrames
	ldr r3, .L_0200a904
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r6, .L_0200a908
	ldr r3, .L_0200a90c
	ldrsh r1, [r6, r2]
	cmp r1, r3
	beq .L_0200a8dc
	adds r2, #130
	adds r3, r6, r2
	movs r0, #144
	movs r2, #1
	strh r2, [r3]
	lsls r0, r0, #2
	ldr r2, .L_0200a910
	adds r3, r6, r0
	strh r2, [r3]
	mov r12, r1
	b .L_0200a914
.L_0200a8dc:
	movs r0, #12
	bl Object_GetById
	adds r1, r0, #0
	ldr r3, [r1, #8]
	asrs r2, r3, #20
	cmp r2, #20
	beq .L_0200a8ee
	b .L_0200ad3c
.L_0200a8ee:
	ldr r3, [r1, #16]
	asrs r0, r3, #20
	cmp r0, #12
	beq .L_0200a8f8
	b .L_0200ad3c
.L_0200a8f8:
	str r2, [sp, #0]
	str r0, [sp, #4]
	movs r1, #12
	movs r0, #38
	b .L_0200aaf2
	.2byte 0x0000
.L_0200a904:
	.4byte Data_03001ebc
.L_0200a908:
	.4byte gCell
.L_0200a90c:
	.4byte 0x000000b1
.L_0200a910:
	.4byte 0x000000b0
.L_0200a914:
	cmp r12, r2
	beq .L_0200a91a
	b .L_0200aa7a
.L_0200a91a:
	movs r0, #8
	movs r1, #6
	bl Engine_ActorSetChildValue
	movs r0, #9
	movs r1, #6
	bl Engine_ActorSetChildValue
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #5
	bne .L_0200a950
	ldr r0, .L_0200a9f0
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_0200a950
	movs r1, #156
	movs r2, #164
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
.L_0200a950:
	bl SceneState_ApplyRectsAtActors8And9
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #20]
	movs r0, #192
	str r3, [r5, #12]
	lsls r0, r0, #2
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_0200a98a
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #10
	bl Object_GetById
	movs r3, #254
	adds r0, #89
	strb r3, [r0]
	bl SceneState_ConfigureRegion82_7AndApply768
.L_0200a98a:
	movs r0, #11
	bl Object_GetById
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #54
	strb r3, [r2]
	adds r2, #59
	strh r3, [r2]
	ldr r2, [r1, #80]
	ldrb r3, [r2, #9]
	movs r6, #12
	orrs r3, r6
	strb r3, [r2, #9]
	ldr r3, [r1, #80]
	ldr r5, .L_0200a9ec
	adds r3, #38
	strb r5, [r3]
	movs r3, #192
	ldr r2, [r1, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r1, #0
	movs r0, #11
	bl Object_SetModeById
	movs r0, #12
	bl Object_GetById
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #89
	strb r5, [r3]
	adds r2, r1, #0
	subs r3, #54
	strb r5, [r3]
	adds r2, #94
	movs r3, #30
	strh r3, [r2]
	ldr r2, [r1, #80]
	ldrb r3, [r2, #9]
	orrs r3, r6
	strb r3, [r2, #9]
	ldr r3, [r1, #80]
	adds r3, #38
	b .L_0200a9f4
.L_0200a9ec:
	.4byte 0x00000000
.L_0200a9f0:
	.4byte 0x00000109
.L_0200a9f4:
	strb r5, [r3]
	movs r3, #128
	ldr r2, [r1, #80]
	lsls r3, r3, #7
	strh r3, [r2, #30]
	movs r1, #0
	movs r0, #12
	bl Object_SetModeById
	movs r0, #13
	bl Object_GetById
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #89
	strb r5, [r3]
	adds r2, r1, #0
	subs r3, #54
	strb r5, [r3]
	adds r2, #94
	movs r3, #60
	strh r3, [r2]
	ldr r2, [r1, #80]
	ldrb r3, [r2, #9]
	orrs r3, r6
	strb r3, [r2, #9]
	ldr r3, [r1, #80]
	adds r3, #38
	strb r5, [r3]
	movs r2, #128
	lsls r2, r2, #8
	ldr r3, [r1, #80]
	mov r8, r2
	mov r0, r8
	strh r0, [r3, #30]
	movs r1, #0
	movs r0, #13
	bl Object_SetModeById
	movs r0, #14
	bl Object_GetById
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #89
	strb r5, [r3]
	adds r2, r1, #0
	subs r3, #54
	strb r5, [r3]
	adds r2, #94
	movs r3, #90
	strh r3, [r2]
	ldr r2, [r1, #80]
	ldrb r3, [r2, #9]
	orrs r3, r6
	strb r3, [r2, #9]
	ldr r3, [r1, #80]
	adds r3, #38
	strb r5, [r3]
	ldr r3, [r1, #80]
	mov r2, r8
	strh r2, [r3, #30]
	movs r0, #14
	movs r1, #0
	bl Object_SetModeById
	b .L_0200ad3c
.L_0200aa7a:
	ldr r3, .L_0200ad4c
	cmp r12, r3
	bne .L_0200ab7e
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r6, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #10
	cmp r3, #7
	bls .L_0200aa92
	b .L_0200ad3c
.L_0200aa92:
	ldr r2, .L_0200ad50
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200aa9c:
	.4byte .L_0200aabc
	.4byte .L_0200aac4
	.4byte .L_0200ad24
	.4byte .L_0200ad24
	.4byte .L_0200aafc
	.4byte .L_0200ad24
	.4byte .L_0200ab28
	.4byte .L_0200ab54
.L_0200aabc:
	movs r0, #152
	lsls r0, r0, #4
	bl Engine_GameFlagSet
.L_0200aac4:
	movs r0, #152
	lsls r0, r0, #4
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_0200aad2
	b .L_0200ad3c
.L_0200aad2:
	movs r3, #1
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #120
	movs r1, #7
	movs r2, #109
	movs r3, #7
	bl Map_CopyMetatileIndicesRect
	movs r3, #45
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #8
.L_0200aaf2:
	movs r2, #1
	movs r3, #1
	bl Map_CopyCellAttributeRect
	b .L_0200ad3c
.L_0200aafc:
	movs r0, #220
	movs r2, #145
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #17
	bl OverlayObject_SpawnConfiguredObject
	movs r3, #27
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Map_CopyCellAttributeRect
	movs r0, #14
	bl FieldScene_RunScene3c5SequenceA
	b .L_0200ad3c
.L_0200ab28:
	movs r0, #224
	movs r2, #145
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #17
	bl OverlayObject_SpawnConfiguredObject
	movs r3, #28
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Map_CopyCellAttributeRect
	movs r0, #16
	bl FieldScene_RunScene3c5SequenceA
	b .L_0200ad3c
.L_0200ab54:
	movs r0, #232
	ldr r2, .L_0200ad54
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #16
	bl OverlayObject_SpawnConfiguredObject
	movs r3, #14
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Map_CopyCellAttributeRect
	movs r0, #17
	bl FieldScene_RunScene3c5SequenceA
	b .L_0200ad3c
.L_0200ab7e:
	ldr r3, .L_0200ad58
	cmp r1, r3
	beq .L_0200ab86
	b .L_0200ad34
.L_0200ab86:
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #128
	ands r5, r3
	movs r2, #128
	strb r5, [r0]
	lsls r1, r1, #9
	movs r0, #8
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, .L_0200ad5c
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_0200abe8
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r6, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200abe2
	ldr r0, .L_0200ad60
	bl Engine_GameFlagSet
	b .L_0200abe8
.L_0200abe2:
	ldr r0, .L_0200ad60
	bl Engine_GameFlagClear
.L_0200abe8:
	ldr r0, .L_0200ad64
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_0200ac6a
	movs r0, #10
	ldr r1, .L_0200ad68
	ldr r2, .L_0200ad68
	bl Engine_ActorSetPosition
	movs r1, #140
	movs r2, #148
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r1, #156
	movs r2, #248
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Engine_ActorSetPosition
	movs r1, #148
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Engine_ActorSetPosition
	movs r1, #160
	movs r2, #148
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #5
	bl Battle_WaitMode0
	b .L_0200acc4
.L_0200ac6a:
	ldr r0, .L_0200ad6c
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_0200acc4
	movs r1, #156
	movs r2, #156
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #5
	bl Battle_WaitMode0
.L_0200acc4:
	ldr r0, .L_0200ad70
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_0200ad14
	movs r1, #140
	movs r2, #240
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Engine_ActorSetPosition
	movs r1, #164
	movs r2, #240
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Engine_ActorSetPosition
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r3, #17
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	movs r1, #14
	movs r2, #4
	movs r3, #1
	bl Map_CopyCellAttributeRect
.L_0200ad14:
	ldr r3, .L_0200ad74
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200ad3c
.L_0200ad24:
	ldr r0, .L_0200ad5c
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_0200ad3c
	bl FieldScene_RunSupplementalSequenceOne
	b .L_0200ad3c
.L_0200ad34:
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
.L_0200ad3c:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_0200ad4c:
	.4byte 0x000000af
.L_0200ad50:
	.4byte .L_0200aa9c
.L_0200ad54:
	.4byte 0x02520000
.L_0200ad58:
	.4byte 0x000000ae
.L_0200ad5c:
	.4byte 0x00000109
.L_0200ad60:
	.4byte 0x00000301
.L_0200ad64:
	.4byte 0x00000988
.L_0200ad68:
	.4byte 0xffc00000
.L_0200ad6c:
	.4byte 0x00000989
.L_0200ad70:
	.4byte 0x00000985
.L_0200ad74:
	.4byte gCell
	.section .rodata.x0200afd4,"a",%progbits
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
.L_0200b08c:
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
.L_0200b0c4:
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
.L_0200b0fc:
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
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200b08c
	.4byte .L_0200b0c4
	.4byte .L_0200b0fc
.L_0200b140:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gBabiIriguchiEntrances3
gBabiIriguchiEntrances3:
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00001a8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0x400000c8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0x400000b8
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0004
	.4byte 0x00000068
	.4byte 0x40000138
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0005
	.4byte 0x00000128
	.4byte 0x40000138
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x000001c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEntrances2
gBabiIriguchiEntrances2:
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x400001e8
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x400001e8
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x40000288
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x000001c8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x000001f8
	.4byte 0x40000078
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0007
	.4byte 0x000001d8
	.4byte 0xc0000120
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0x40000078
	.4byte 0x00300000
	.4byte 0x01200040
	.4byte 0x00000140
	.4byte 0xffff0009
	.4byte 0x000000b8
	.4byte 0xc0000120
	.4byte 0x00300000
	.4byte 0x01200040
	.4byte 0x00000140
	.4byte 0xffff000a
	.4byte 0x000002d8
	.4byte 0x400000a8
	.4byte 0x02700000
	.4byte 0x03600060
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x00000308
	.4byte 0x400000b8
	.4byte 0x02700000
	.4byte 0x03600060
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000001e8
	.4byte 0x400000b8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff000d
	.4byte 0x000000f8
	.4byte 0x40000248
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff000e
	.4byte 0x000001b8
	.4byte 0x400000d8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff000f
	.4byte 0x000000d8
	.4byte 0x40000248
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0xffff0010
	.4byte 0x000001c8
	.4byte 0x400000a8
	.4byte 0x01600000
	.4byte 0x02500040
	.4byte 0x00000140
	.4byte 0xffff0011
	.4byte 0x000000e8
	.4byte 0x40000218
	.4byte 0x00800000
	.4byte 0x017001b0
	.4byte 0x000002a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEntrances1
gBabiIriguchiEntrances1:
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000078
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc0000170
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0xffff0003
	.4byte 0x000000a8
	.4byte 0x40000158
	.4byte 0x00600000
	.4byte 0x01800030
	.4byte 0x000001b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEntrancesOther
gBabiIriguchiEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x00000148
	.4byte 0xc00000f0
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0xffff0001
	.4byte 0x00000070
	.4byte 0xc0000168
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000020
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiRegions3
gBabiIriguchiRegions3:
	.4byte 0xffec0060
	.4byte 0x00700140
	.4byte 0x0150fffc
	.4byte 0x0004ffff
	.4byte 0xffec0120
	.4byte 0x01300140
	.4byte 0x0150fffc
	.4byte 0x0005ffff
	.4byte 0xffec0130
	.4byte 0x01400060
	.4byte 0x0070fffc
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiExits
gBabiIriguchiExits:
	.4byte 0x000000b0
	.4byte 0x001020b1
	.4byte 0x002070af
	.4byte 0x003090af
	.4byte 0x004010ad
	.4byte 0x005030ad
	.4byte 0x006020ad
	.4byte 0x000000af
	.4byte 0x001040af
	.4byte 0x002030b0
	.4byte 0x003080af
	.4byte 0x004010af
	.4byte 0x005020af
	.4byte 0x006020b0
	.4byte 0x007050af
	.4byte 0x008060af
	.4byte 0x0090b0af
	.4byte 0x00a030af
	.4byte 0x00b0c0af
	.4byte 0x00c0d0af
	.4byte 0x00d0e0af
	.4byte 0x00e0f0af
	.4byte 0x00f100af
	.4byte 0x010110af
	.4byte 0x011030ae
	.4byte 0x012020ae
	.4byte 0x000000ae
	.4byte 0x001010ac
	.4byte 0x0020a0af
	.4byte 0x000000b1
	.4byte 0x0011f002
	.4byte 0x002010b0
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacements3
gBabiIriguchiPlacements3:
	.4byte 0xffff00fd
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00fd
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte .L_0200b140
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0074
	.4byte .L_0200b140
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte .L_0200b140
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte .L_0200b140
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacements2
gBabiIriguchiPlacements2:
	.4byte 0x0072005d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacements1
gBabiIriguchiPlacements1:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff0128
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00028000
	.4byte 0xffff0128
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00020000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00018000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff0046
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiPlacementsOther
gBabiIriguchiPlacementsOther:
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEvents3
gBabiIriguchiEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte FieldScene_RunFourCallSequence
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte SceneState_BranchOnActorEightOrNineTile
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026fa
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000026fb
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000026fc
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000026fd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026fe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000026ff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002700
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002701
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte BabiIriguchi_JumpFromLedge
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte BabiIriguchi_JumpFromLedge
	.4byte 0x00000c15
	.4byte 0x0300000a
	.4byte SceneState_ConfigureRegion82_7AndApply768
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEvents2
gBabiIriguchiEvents2:
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000012
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte FieldScene_RunStep11
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte FieldScene_RunStep12WithPosition
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte FieldScene_RunStep13WithTwoPositions
	.4byte 0x00004602
	.4byte 0xffff001c
	.4byte FieldScene_RunStep15
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte SceneState_SetValue8Mode66
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunStepWithValue2693
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEvents1
gBabiIriguchiEvents1:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0988000a
	.4byte FieldScene_RunBranchingActorSequence
	.4byte 0x00000002
	.4byte 0x13010014
	.4byte BabiIriguchi_CloseTruthDoor
	.4byte 0x00000002
	.4byte 0x03010015
	.4byte BabiIriguchi_CloseTruthDoor
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte BabiIriguchi_OpenTruthDoor
	.4byte 0x00000000
	.4byte 0x0989000a
	.4byte 0x00002725
	.4byte 0x00000000
	.4byte 0x0989000b
	.4byte 0x00002726
	.4byte 0x00000000
	.4byte 0x0989000c
	.4byte 0x00002727
	.4byte 0x00000000
	.4byte 0x0989000d
	.4byte 0x00002728
	.4byte 0x00000000
	.4byte 0x0989000e
	.4byte 0x00002729
	.4byte 0x00008d15
	.4byte 0x0989000a
	.4byte 0x0000272a
	.4byte 0x00008d15
	.4byte 0x0989000b
	.4byte 0x0000272b
	.4byte 0x00008d15
	.4byte 0x0989000c
	.4byte 0x0000272c
	.4byte 0x00008d15
	.4byte 0x0989000d
	.4byte 0x0000272d
	.4byte 0x00008d15
	.4byte 0x0989000e
	.4byte 0x0000272e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000274c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000274d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000274e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000274f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002750
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002751
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002752
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002753
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002754
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002755
	.4byte 0x00000003
	.4byte 0xffff003c
	.4byte BabiIriguchi_FlipTruthDoorSwitch
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte ActorPresentation_SetSceneCellByFlag985
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte SceneState_ApplyRectAt32x78
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiIriguchiEventsOther
gBabiIriguchiEventsOther:
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte SceneActor_PushObjectAheadIfLevel
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte SceneActor_RunSlotZeroFacingCheck
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte ActorPresentation_PlaceActorTwelveAtTile20And12
	.4byte 0x10009315
	.4byte 0xffff000c
	.4byte SceneState_SetRuntimeByte34
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte ActorPresentation_PlaceActorTwelveAtTile20And12
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000026b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000026b8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000026b9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026ba
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000026c5
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000026c6
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000026c7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026c8
	.4byte 0x00000413
	.4byte 0x0fbc0064
	.4byte 0x001000c4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
