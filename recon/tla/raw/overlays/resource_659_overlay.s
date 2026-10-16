.syntax unified
	.thumb
	.section .text.x02008570,"ax",%progbits
	.balign 4
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085b4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085a6
	bl Func_020007dc
	b .L_02008614
.L_020085a6:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #10
	b .L_020085e2
.L_020085b4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ec
	movs r3, #224
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
.L_020085e2:
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_02008614
.L_020085ec:
	movs r3, #160
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
.L_02008614:
	pop {pc}
	.2byte 0x0000
.L_02008618:
	.4byte gEwramHeap
.L_0200861c:
	.4byte 0x03490000
.L_02008620:
	.4byte 0x036d0000
	.section .text.x02009008,"ax",%progbits
	.balign 4
	.global Func_02001008
	.thumb_func
Func_02001008:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_SetBit
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #224
	lsls r0, r0, #1
	bl PartyInventory_Remove
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #11
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #205
	movs r2, #109
	movs r0, #11
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #190
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_02009074
	ldr r0, [r0, #8]
	cmp r0, #0
	bge .L_0200906a
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_0200906a:
	asrs r1, r0, #16
	movs r2, #95
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
.L_02009074:
	movs r1, #181
	movs r2, #95
	movs r0, #4
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02001e40
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, .L_020092f4
	bl Func_02001e18
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #3
	movs r0, #11
	bl Func_02001e48
	movs r1, #189
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #109
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #11
	movs r2, #0
	movs r0, #10
	bl Object_LinkPair
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02001e48
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #0
	movs r1, #11
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02001e48
	movs r0, #25
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #25
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020092f8
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020092fc
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl FieldEvent_WaitFrames
	ldr r1, .L_02009300
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #120
	bl FieldEvent_WaitFrames
	ldr r1, .L_02009304
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #11
	movs r0, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #60
	bl FieldEvent_WaitFrames
	ldr r1, .L_02009308
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200930c
	movs r0, #25
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #11
	bl Object_RefreshSelectorById
	movs r0, #25
	bl Object_RefreshSelectorById
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r0, #25
	movs r1, #2
	movs r2, #10
	bl Func_02001ec0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #4
	adds r3, #1
	strh r3, [r2]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	bl Func_02001d58
	pop {pc}
	.2byte 0x0000
.L_020092f4:
	.4byte MsgMadraInteriors110
.L_020092f8:
	.4byte gData_02001f18
.L_020092fc:
	.4byte gData_02001f4c
.L_02009300:
	.4byte gData_02001f90
.L_02009304:
	.4byte gData_02001fc4
.L_02009308:
	.4byte gData_02002054
.L_0200930c:
	.4byte gData_020020e4
