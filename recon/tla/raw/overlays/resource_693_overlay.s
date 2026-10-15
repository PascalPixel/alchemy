.syntax unified
	.thumb
	.section .text.x02008120,"ax",%progbits
	.global Func_02000120
	.thumb_func
Func_02000120:
	push {r5, r6, lr}
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	ldr r0, .L_020081fc
	bl Func_02001098
	movs r1, #0
	movs r0, #18
	bl UiText_OpenMessageAtObject
	ldr r5, .L_02008200
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r5, r2
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008192
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #0
	bl Func_020010a8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	movs r0, #157
	movs r2, #1
	strh r2, [r3]
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200818a
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_SetBit
.L_0200818a:
	movs r0, #4
	bl Func_020010f0
	b .L_020081f8
.L_02008192:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #18
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020010a8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #18
	ldr r1, .L_02008204
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_020081da
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #18
	bl ObjectMotion_ResetAndSetPosition
.L_020081da:
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02001018
.L_020081f8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081fc:
	.4byte MsgAreWeSetting
.L_02008200:
	.4byte gPartyState
.L_02008204:
	.4byte 0x00019999
	.section .text.x020083e4,"ax",%progbits
	.global Func_020003e4
	.thumb_func
Func_020003e4:
	push {lr}
	movs r0, #157
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083f4
	b .L_0200862c
.L_020083f4:
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02008630
	adds r1, #204
	bl Func_020010d0
	movs r0, #130
	movs r1, #1
	movs r2, #186
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_020010e0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #23
	bl Func_020010c8
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #6
	movs r0, #23
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02008634
	bl Func_02001098
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	bl Func_020010b8
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #23
	bl Func_020010a8
	bl Func_020010e8
	movs r1, #153
	movs r3, #0
	adds r0, #85
	lsls r1, r1, #8
	strb r3, [r0]
	adds r1, #153
	ldr r0, .L_02008638
	bl Func_020010d0
	movs r0, #220
	movs r1, #128
	movs r2, #184
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_020010e0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #216
	movs r1, #128
	movs r2, #232
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #14
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_020010e0
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02008638
	adds r1, #153
	bl Func_020010d0
	movs r0, #130
	movs r1, #128
	movs r2, #186
	movs r3, #1
	lsls r2, r2, #17
	lsls r1, r1, #14
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020010e0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #6
	bl Func_020010b8
	movs r0, #23
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #23
	bl Func_020010c8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #4
	bl Object_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	bl Func_020010b8
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #6
	bl Func_020010b8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #4
	bl Object_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	ldr r3, .L_0200863c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #2
	ldr r0, [r3]
	adds r1, #255
	movs r2, #40
	bl Func_020010c8
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #23
	bl Func_020010c8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #23
	bl Func_020010c8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #23
	bl ObjectMotion_SetSpeedParameters
	movs r0, #23
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #252
	movs r2, #180
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #23
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #23
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #6
	movs r0, #23
	bl Func_020010b8
	movs r0, #23
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #177
	bl GameFlag_SetBit
	bl Func_02001018
.L_0200862c:
	pop {pc}
	.2byte 0x0000
.L_02008630:
	.4byte 0x00026666
.L_02008634:
	.4byte MsgIVeBeenWaiting
.L_02008638:
	.4byte 0x0004cccc
.L_0200863c:
	.4byte gPartyState
	.section .text.x02008640,"ax",%progbits
	.global Func_02000640
	.thumb_func
Func_02000640:
	push {r5, r6, r7, lr}
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	ldr r7, .L_02008770
	movs r5, #133
	lsls r5, r5, #2
	adds r6, r7, r5
	ldr r1, [r6]
	movs r0, #18
	bl Func_02001070
	movs r0, #1
	bl WaitFrames
	movs r1, #160
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #18
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #248
	movs r0, #18
	adds r1, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #18
	bl Func_020010b8
	ldr r0, .L_02008774
	bl Func_02001098
	movs r1, #0
	movs r0, #18
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020086f2
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #0
	bl Func_020010a8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r7, r2
	movs r0, #157
	movs r2, #1
	strh r2, [r3]
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ea
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_SetBit
.L_020086ea:
	movs r0, #4
	bl Func_020010f0
	b .L_0200876e
.L_020086f2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #18
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020010a8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #18
	ldr r1, .L_02008778
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200873a
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #18
	bl ObjectMotion_ResetAndSetPosition
.L_0200873a:
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #138
	ldr r0, [r6]
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	bl Func_02001018
.L_0200876e:
	pop {r5, r6, r7, pc}
.L_02008770:
	.4byte gPartyState
.L_02008774:
	.4byte MsgAreWeSetting
.L_02008778:
	.4byte 0x00019999
	.section .text.x0200897c,"ax",%progbits
	.global Func_0200097c
	.thumb_func
Func_0200097c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	movs r2, #208
	lsls r2, r2, #8
	mov r8, r2
	movs r1, #252
	movs r2, #230
	mov r3, r8
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #24
	bl Func_02001068
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_02008bac
	bl Func_02001098
	movs r0, #24
	movs r1, #0
	bl Func_020010a8
	ldr r3, .L_02008bb0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #23
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	ldr r0, .L_02008bb4
	bl Func_020010d0
	bl Func_020010e8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #128
	movs r2, #190
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #24
	ldr r1, .L_02008bb4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #130
	movs r2, #216
	movs r0, #24
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #130
	movs r2, #194
	movs r0, #24
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #23
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #24
	movs r1, #0
	bl Func_020010a8
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #1
	movs r0, #23
	bl Func_020010c8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #24
	bl Func_020010c8
	movs r0, #24
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	mov r1, r8
	movs r0, #24
	bl Func_020010b8
	movs r0, #24
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #130
	movs r2, #192
	strb r3, [r0]
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #24
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #24
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r1, #2
	movs r0, #24
	bl Motion_SetVarCbAndRefresh
	movs r1, #130
	movs r2, #190
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #65
	bl Func_02001060
	movs r0, #1
	bl WaitFrames
	movs r0, #24
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #130
	ands r5, r3
	movs r2, #194
	lsls r1, r1, #2
	lsls r2, r2, #1
	strb r5, [r0]
	movs r0, #24
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #24
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #176
	movs r2, #10
	movs r0, #24
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #24
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #130
	movs r2, #217
	movs r0, #24
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #128
	movs r1, #1
	movs r2, #174
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #252
	movs r2, #230
	movs r0, #24
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #24
	movs r1, #0
	bl Func_02001060
	mov r1, r8
	movs r0, #23
	bl Func_020010b8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #23
	bl Func_020010c8
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_020010a8
	movs r0, #23
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #23
	bl Func_020010c8
	movs r1, #0
	movs r0, #23
	bl Func_020010a8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #173
	bl GameFlag_SetBit
	bl Func_02001018
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008bac:
	.4byte MsgHeyWait2
.L_02008bb0:
	.4byte gPartyState
.L_02008bb4:
	.4byte 0x00019999
