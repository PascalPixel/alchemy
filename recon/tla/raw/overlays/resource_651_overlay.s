.syntax unified
	.thumb
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	ldr r3, .L_02008060
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008064
	cmp r2, r3
	bne .L_0200805c
	ldr r0, .L_02008068
	b .L_0200805e
.L_0200805c:
	ldr r0, .L_0200806c
.L_0200805e:
	pop {pc}
.L_02008060:
	.4byte gPartyState
.L_02008064:
	.4byte 0x0000000d
.L_02008068:
	.4byte Data_02001068
.L_0200806c:
	.4byte Data_02001018
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	ldr r3, .L_0200808c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008090
	cmp r2, r3
	bne .L_02008088
	ldr r0, .L_02008094
	b .L_0200808a
.L_02008088:
	ldr r0, .L_02008098
.L_0200808a:
	pop {pc}
.L_0200808c:
	.4byte gPartyState
.L_02008090:
	.4byte 0x0000000d
.L_02008094:
	.4byte Data_02001238
.L_02008098:
	.4byte Data_020010d0
	.section .text.x0200809c,"ax",%progbits
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	ldr r5, .L_02008114
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Engine_ActorSetSpeed
	movs r1, #2
	ldr r0, [r5]
	bl Engine_ActorSetSpritePriority
	ldr r0, [r5]
	bl Engine_ActorGet
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #123
	bl Engine_AudioPlayCue
	ldr r0, [r5]
	movs r1, #2
	bl Engine_ActorSetAnimation
	movs r2, #6
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl Engine_ActorCenterAndWalk
	movs r0, #10
	bl Engine_EventWait
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Engine_EventSetCountdown
	bl Engine_EventCloseScreen
	bl Engine_EventWaitDelay
	bl Engine_EventEnd
	pop {r5, r6, pc}
.L_02008114:
	.4byte gPartyState
	.section .text.x02008118,"ax",%progbits
	.global Func_02000118
	.thumb_func
Func_02000118:
	push {lr}
	movs r2, #192
	movs r1, #66
	lsls r2, r2, #2
	bl Engine_ActorReturnHome
	pop {pc}
	.2byte 0x0000
	.section .text.x02008128,"ax",%progbits
	.global Func_02000128
	.thumb_func
Func_02000128:
	push {lr}
	ldr r3, .L_02008144
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008148
	cmp r2, r3
	bne .L_02008140
	ldr r0, .L_0200814c
	b .L_02008142
.L_02008140:
	ldr r0, .L_02008150
.L_02008142:
	pop {pc}
.L_02008144:
	.4byte gPartyState
.L_02008148:
	.4byte 0x0000000d
.L_0200814c:
	.4byte Data_02001664
.L_02008150:
	.4byte Data_02001490
	.section .text.x02008288,"ax",%progbits
	.global Func_02000288
	.thumb_func
Func_02000288:
	push {r5, lr}
	ldr r3, .L_020082c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Engine_ActorGet
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_020082bc
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020082c4
	movs r0, #3
	adds r1, r5, #0
	bl Engine_ShopOpen
	b .L_020082e0
	.2byte 0x0000
.L_020082bc:
	.4byte 0xffffc000
.L_020082c0:
	.4byte gPartyState
.L_020082c4:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	ldr r0, .L_020082e4
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_020082e0:
	pop {r5, pc}
	.2byte 0x0000
.L_020082e4:
	.4byte MsgDeriTaviRikiWorry
	.section .text.x02008340,"ax",%progbits
	.global Func_02000340
	.thumb_func
Func_02000340:
	push {r5, lr}
	ldr r3, .L_02008378
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Engine_ActorGet
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008374
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200837c
	movs r0, #3
	adds r1, r5, #0
	bl Engine_ShopOpen
	b .L_02008398
	.2byte 0x0000
.L_02008374:
	.4byte 0xffffc000
.L_02008378:
	.4byte gPartyState
.L_0200837c:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	ldr r0, .L_0200839c
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_02008398:
	pop {r5, pc}
	.2byte 0x0000
.L_0200839c:
	.4byte MsgDeriTaviRikiHome
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
	.section .text.x02008924,"ax",%progbits
	.global Func_02000924
	.thumb_func
Func_02000924:
	push {lr}
	sub sp, #8
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	movs r3, #11
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #10
	movs r1, #19
	movs r2, #1
	bl Func_02000c48
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl Func_02000ca8
	movs r0, #132
	lsls r0, r0, #2
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	add sp, #8
	pop {pc}
	.section .text.x02008960,"ax",%progbits
	.global Func_02000960
	.thumb_func
Func_02000960:
	push {lr}
	sub sp, #8
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	movs r3, #26
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #25
	movs r1, #20
	movs r2, #1
	bl Func_02000c48
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl Func_02000ca8
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020089a0,"ax",%progbits
	.global Func_020009a0
	.thumb_func
Func_020009a0:
	push {lr}
	sub sp, #8
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	movs r3, #29
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #28
	movs r1, #23
	movs r2, #1
	bl Func_02000c48
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl Func_02000ca8
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020089e0,"ax",%progbits
	.global Func_020009e0
	.thumb_func
Func_020009e0:
	push {lr}
	sub sp, #8
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	movs r3, #18
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #17
	movs r1, #11
	movs r2, #1
	bl Func_02000c48
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl Func_02000ca8
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008a20,"ax",%progbits
	.global Func_02000a20
	.thumb_func
Func_02000a20:
	push {lr}
	sub sp, #8
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	movs r3, #31
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #30
	movs r1, #17
	movs r2, #1
	bl Func_02000c48
	movs r1, #0
	movs r2, #0
	movs r0, #26
	bl Func_02000ca8
	movs r0, #133
	lsls r0, r0, #2
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	add sp, #8
	pop {pc}
	.section .text.x02008a5c,"ax",%progbits
	.global Func_02000a5c
	.thumb_func
Func_02000a5c:
	push {lr}
	sub sp, #8
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventResolvePendingActions
	movs r3, #17
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #16
	movs r1, #23
	movs r2, #1
	bl Func_02000c48
	movs r1, #0
	movs r2, #0
	movs r0, #31
	bl Func_02000ca8
	movs r0, #139
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008a9c,"ax",%progbits
	.global Func_02000a9c
	.thumb_func
Func_02000a9c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_02008ac4
	ldr r5, [r3, #108]
	bl Func_02000d58
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #188
	adds r5, r5, r3
	ldr r1, [r5]
	movs r3, #1
	adds r1, #35
	ldrb r2, [r1]
	orrs r3, r2
	movs r2, #253
	ands r3, r2
	strb r3, [r1]
	pop {r5, pc}
.L_02008ac4:
	.4byte Data_02000d80
	.section .text.x02008ac8,"ax",%progbits
	.global Func_02000ac8
	.thumb_func
Func_02000ac8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	str r2, [r3]
	ldr r3, .L_02008b34
	adds r2, #224
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b38
	cmp r2, r3
	bne .L_02008af6
	movs r0, #12
	movs r1, #0
	bl Func_02000d68
	b .L_02008b02
.L_02008af6:
	ldr r3, .L_02008b3c
	cmp r2, r3
	bne .L_02008b02
	ldr r0, .L_02008b40
	bl Func_02000d50
.L_02008b02:
	movs r1, #1
	movs r0, #21
	bl Engine_ActorSetSpritePriority
	ldr r3, .L_02008b34
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b3c
	cmp r2, r3
	bne .L_02008b30
	movs r0, #192
	lsls r0, r0, #2
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_02008b30
	movs r0, #66
	movs r1, #0
	bl Object_SetWideSprite
.L_02008b30:
	movs r0, #0
	pop {pc}
.L_02008b34:
	.4byte gPartyState
.L_02008b38:
	.4byte 0x0000000a
.L_02008b3c:
	.4byte 0x0000000d
.L_02008b40:
	.4byte Data_02000d80
	.section .text.x02008b44,"ax",%progbits
	.global Func_02000b44
	.thumb_func
Func_02000b44:
	push {lr}
	ldr r3, .L_02008c28
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c2c
	sub sp, #8
	cmp r2, r3
	bne .L_02008c22
	movs r0, #132
	lsls r0, r0, #2
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008b7a
	movs r3, #11
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #19
	movs r2, #1
	movs r3, #1
	bl Func_02000c48
.L_02008b7a:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008b9c
	movs r3, #26
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl Func_02000c48
.L_02008b9c:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008bbe
	movs r3, #29
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02000c48
.L_02008bbe:
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008be0
	movs r3, #18
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02000c48
.L_02008be0:
	movs r0, #133
	lsls r0, r0, #2
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008c00
	movs r3, #31
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl Func_02000c48
.L_02008c00:
	movs r0, #139
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008c22
	movs r3, #17
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02000c48
.L_02008c22:
	movs r0, #0
	add sp, #8
	pop {pc}
.L_02008c28:
	.4byte gPartyState
.L_02008c2c:
	.4byte 0x0000000d
	.section .text.x02008c30,"ax",%progbits
	.global Func_02000c30
	.thumb_func
Func_02000c30:
	push {lr}
	bl Func_02000d18
	pop {pc}
