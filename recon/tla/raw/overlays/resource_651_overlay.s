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
	bl Engine_EventPrepareSpeakers
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
	bl Engine_EventRequestExit
	bl Engine_EventCloseScreen
	bl Engine_EventWaitForScreen
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
	.section .text.x02008154,"ax",%progbits
	.global Func_02000154
	.thumb_func
Func_02000154:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #67
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_020081be
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r5, .L_020081d0
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #8
	bl Engine_EventOpenMessage
	bl Engine_PartyGetLeaderActor
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #0
	bne .L_0200819a
	movs r0, #10
	bl Engine_EventWait
	adds r0, r5, #1
	bl Engine_EventSetMessage
	b .L_020081a6
.L_0200819a:
	movs r0, #20
	bl Engine_EventWait
	adds r0, r5, #2
	bl Engine_EventSetMessage
.L_020081a6:
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #67
	bl Engine_GameFlagSet
	bl Engine_EventEnd
	b .L_020081cc
.L_020081be:
	ldr r0, .L_020081d4
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
.L_020081cc:
	pop {r5, pc}
	.2byte 0x0000
.L_020081d0:
	.4byte 0x00001712
.L_020081d4:
	.4byte 0x00001715
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
	bl Engine_EventPrepareSpeakers
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
	.4byte 0x0000174b
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
	bl Engine_EventPrepareSpeakers
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
	.4byte 0x00001837
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
	bl Engine_EventPrepareSpeakers
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
	.4byte 0x00001812
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
	bl Engine_EventPrepareSpeakers
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
	bl Engine_EventPrepareSpeakers
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
	bl Engine_EventPrepareSpeakers
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
	bl Engine_EventPrepareSpeakers
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
	bl Engine_EventPrepareSpeakers
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
	bl Engine_EventPrepareSpeakers
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
	.section .rodata.x02008d80,"a",%progbits
	.global Data_02000d80
Data_02000d80:
	.4byte 0x02160016
	.4byte 0x02170017
	.4byte 0x02180018
	.4byte 0x02190019
	.4byte 0x021a001a
	.4byte 0x021b001d
	.4byte 0x0000ffff
.L_02008d9c:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02008f44:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.global gDeriMuraEntrances
gDeriMuraEntrances:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000152
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000170
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x00000050
	.4byte 0x40000160
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001018
Data_02001018:
	.4byte 0x0000000a
	.4byte 0x1010100b
	.4byte 0xffffffff
	.4byte 0x1020200b
	.4byte 0xffffffff
	.4byte 0x1030300b
	.4byte 0xffffffff
	.4byte 0x1040400b
	.4byte 0xffffffff
	.4byte 0x1050500b
	.4byte 0xffffffff
	.4byte 0x1060600b
	.4byte 0xffffffff
	.4byte 0x1070100c
	.4byte 0xffffffff
	.4byte 0x10802002
	.4byte 0xffffffff
	.4byte 0x10903002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001068
Data_02001068:
	.4byte 0x0000000d
	.4byte 0x1010100b
	.4byte 0xffffffff
	.4byte 0x1020200b
	.4byte 0xffffffff
	.4byte 0x1030300b
	.4byte 0xffffffff
	.4byte 0x1040400b
	.4byte 0xffffffff
	.4byte 0x1050500b
	.4byte 0xffffffff
	.4byte 0x1060600b
	.4byte 0xffffffff
	.4byte 0x1070100c
	.4byte 0xffffffff
	.4byte 0x10802002
	.4byte 0xffffffff
	.4byte 0x10903002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020010d0
Data_020010d0:
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff0070
	.4byte .L_02008d9c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0001c000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff00e0
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00015000
	.4byte 0xffff0092
	.4byte .L_02008f44
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001238
Data_02001238:
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00018000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0001c000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00010000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00018000
	.4byte 0xffff00e0
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff00bb
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00018000
	.4byte 0xffff0092
	.4byte .L_02008f44
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00024000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0x02100122
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x02110122
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x02120122
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x02130122
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x02140122
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x02150122
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001490
Data_02001490:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte Func_0200009c
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000154
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte DeriMura_TalkIndraMoved
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldISawABrightLightIn
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldThankGoodnessForThatMountainRange
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldOhhhThatWaveKnockedMeOver
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldThatAncientTowerToTheEast
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldRikiAndTaviSureAreOut
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldIWonderIfThoseTwoAre
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldOurVillageSurvivedTheWaveBut
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte DeriMura_TalkBoat
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldThatTidalWaveLeftALot
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldYaaayPuddlesInTheVillageYaaay
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldBleahIFellInAPuddle
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_02000288
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldThatWaveTurnedThisPlaceInto
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldThatMustHaveBeenOneSerious
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIThoughtISawSomeLights
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldTheSeaGodMustHaveSaved
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldMyHipsKillingMeAndAll
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldICantBelieveThoseChildrenPlay
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldTheShrineOfTheSeaGod
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldICantBelieveThoseLosersWere
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldThatWaveWasBrutalItSmashed
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldWithAllTheBoatsGoneWe
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldWithAllThisSaltWaterOn
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldYayWeNeverGetPuddlesLike
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldBleccchThisWatersSaaalty
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldImSureThoseTwoWentTo
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001664
Data_02001664:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte Func_0200009c
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_020003a0
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte Func_02000c30
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte Func_02000c30
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte Func_02000c30
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_02000c30
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldAfterThatWaveHitIWas
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldSureAreALotOfTravelers
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldThosePuddlesOfSeaWaterThe
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldAhTheSeaIsCalmAgain
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte DeriMura_TalkHip
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIveAlwaysHeardThatTheShrine
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldTheDreadPirateBriggsWasCaptured
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldThoseTwoLosersAreBackHome
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldIThoughtISawSomethingWhen
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldSoIHearTheyCaughtThe
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldItWasCreepyRightBeforeThe
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldThePuddlesAreGoneAndNow
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldIGotHurtWhenIWas
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_02000340
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldThankOurLuckyStarsOurLittle
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldIsThatGuyStillMeetingWith
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldThosePuddlesStankIThoughtId
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldImGladEverythingIsBackTo
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldNobodyBelievesAWaveCouldBe
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldIdSureLikeToSeeWhat
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldIWonderWhatHappenedToBriggs
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldTaviAndRikiMustHaveBeen
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldItsProbablyNothingButJunkAnd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldTheSeasArePeacefulAndFree
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldBeforeTheWaveHitTheTide
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldIHopeSomeOfTheLittle
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldICaughtMyFootOnSomething
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001838
	.4byte 0x50008a05
	.4byte 0x0210003c
	.4byte Func_02000924
	.4byte 0x50008a05
	.4byte 0x0211003d
	.4byte Func_020009e0
	.4byte 0x50008a05
	.4byte 0x0212003e
	.4byte Func_02000960
	.4byte 0x50008a05
	.4byte 0x0213003f
	.4byte Func_020009a0
	.4byte 0x50008a05
	.4byte 0x02140040
	.4byte Func_02000a20
	.4byte 0x50008a05
	.4byte 0x02150041
	.4byte Func_02000a5c
	.4byte 0x00001815
	.4byte 0x02160016
	.4byte Func_02000a9c
	.4byte 0x00001815
	.4byte 0x02170017
	.4byte Func_02000a9c
	.4byte 0x00001815
	.4byte 0x02180018
	.4byte Func_02000a9c
	.4byte 0x00001815
	.4byte 0x02190019
	.4byte Func_02000a9c
	.4byte 0x00001815
	.4byte 0x021a001a
	.4byte Func_02000a9c
	.4byte 0x00001815
	.4byte 0x021b001d
	.4byte Func_02000a9c
	.4byte 0x50008805
	.4byte 0x03000064
	.4byte Func_02000118
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
