.syntax unified
	.thumb
	.section .text.x020083a0,"ax",%progbits
	.global Func_020003a0
	.thumb_func
Func_020003a0:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #75
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_020083b2
	b .L_0200891e
.L_020083b2:
	movs r0, #145
	lsls r0, r0, #4
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_020083c0
	b .L_0200891e
.L_020083c0:
	movs r0, #7
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_020083cc
	b .L_0200891e
.L_020083cc:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	ldr r6, .L_02008658
	adds r0, r6, #0
	bl Engine_EventSetMessage
	movs r1, #236
	movs r2, #132
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #27
	bl Func_02000ca8
	movs r0, #30
	bl Engine_EventWait
	ldr r5, .L_0200865c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #252
	ldr r0, [r5]
	lsls r1, r1, #1
	subs r2, #236
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #236
	movs r2, #140
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02000d08
	movs r0, #60
	bl Engine_EventWait
	movs r0, #27
	movs r1, #0
	bl Engine_EventShowMessage
	movs r3, #160
	movs r0, #5
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02000d48
	movs r3, #160
	movs r0, #6
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02000d48
	movs r2, #8
	movs r3, #128
	lsls r3, r3, #8
	negs r2, r2
	movs r1, #32
	movs r0, #28
	bl Func_02000d48
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_02000d08
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r2, #0
	movs r1, #28
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Engine_EventWait
	movs r1, #3
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #6
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r1, #28
	movs r2, #0
	ldr r0, [r5]
	bl Object_LinkPair
	movs r0, #30
	bl Engine_EventWait
	movs r0, #28
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #27
	ldr r0, [r5]
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #28
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r2, #0
	movs r0, #6
	movs r1, #27
	bl ObjectMotion_SetAngleToward
	movs r1, #4
	movs r0, #27
	bl Engine_ActorSetAnimation
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Engine_EventWait
	movs r0, #27
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #0
	ldr r1, [r5]
	movs r0, #27
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Engine_EventWait
	movs r1, #1
	movs r0, #28
	bl ObjectMotion_SetVariantCallback
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #28
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r1, #3
	movs r0, #27
	bl Engine_ActorSetAnimation
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #30
	bl Engine_EventWait
	movs r2, #0
	ldr r1, [r5]
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r0, #5
	bl Engine_EventOpenMessage
	bl Engine_PartyGetLeaderActor
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #0
	bne .L_02008660
	movs r0, #30
	bl Engine_EventWait
	adds r0, r6, #0
	adds r0, #11
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02000d08
	movs r0, #60
	bl Engine_EventWait
	movs r0, #5
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	adds r0, r6, #0
	adds r0, #15
	bl Engine_EventSetMessage
	b .L_02008696
	.2byte 0x0000
.L_02008658:
	.4byte MsgDeriGarciaCall
.L_0200865c:
	.4byte gPartyState
.L_02008660:
	movs r0, #30
	bl Engine_EventWait
	adds r0, r6, #0
	adds r0, #13
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	movs r0, #5
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
.L_02008696:
	movs r1, #4
	movs r0, #27
	bl Engine_ActorSetAnimation
	movs r0, #60
	bl Engine_EventWait
	movs r0, #27
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #1
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #6
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	movs r2, #0
	movs r1, #6
	movs r0, #27
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #27
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02000d08
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #28
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	movs r1, #3
	movs r0, #27
	bl Engine_ActorSetAnimation
	movs r0, #60
	bl Engine_EventWait
	movs r0, #27
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #252
	movs r2, #164
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r3, .L_02008920
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #28
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #228
	movs r2, #164
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #28
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #212
	movs r2, #236
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #27
	movs r2, #0
	movs r0, #28
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r2, #0
	movs r0, #27
	bl Func_02000ca8
	movs r0, #120
	bl Engine_EventWait
	movs r0, #5
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #4
	movs r2, #20
	movs r0, #5
	bl ObjectMotion_Launch
	movs r0, #30
	bl Engine_EventWait
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_02000d08
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	movs r1, #3
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #60
	bl Engine_EventWait
	movs r1, #0
	movs r0, #6
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	ldr r1, [r5]
	movs r2, #0
	movs r0, #28
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #28
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #28
	bl Engine_EventShowMessage
	movs r0, #60
	bl Engine_EventWait
	ldr r0, [r5]
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #5
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r1, #3
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #120
	bl Engine_EventWait
	movs r0, #5
	movs r1, #2
	bl Engine_ActorSetAnimation
	ldr r0, [r5]
	bl Engine_ActorGet
	cmp r0, #0
	beq .L_020088a0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_020088a0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02000ca8
	movs r0, #6
	movs r1, #2
	bl Engine_ActorSetAnimation
	ldr r0, [r5]
	bl Engine_ActorGet
	cmp r0, #0
	beq .L_020088d0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_020088d0:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02000ca8
	movs r0, #28
	movs r1, #2
	bl Engine_ActorSetAnimation
	ldr r0, [r5]
	bl Engine_ActorGet
	cmp r0, #0
	beq .L_02008900
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #28
	bl ObjectMotion_ResetAndSetPosition
.L_02008900:
	movs r0, #28
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02000ca8
	bl Engine_EventEnd
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #75
	bl Engine_GameFlagSet
.L_0200891e:
	pop {r5, r6, pc}
.L_02008920:
	.4byte gPartyState
