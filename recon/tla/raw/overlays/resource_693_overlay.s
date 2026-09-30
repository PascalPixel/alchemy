.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #14
	movs r1, #0
	movs r2, #15
	bl Func_02001110
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	ldr r3, .L_020080bc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080c0
	cmp r2, r3
	bne .L_0200809e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200808a
	ldr r0, .L_020080c4
	b .L_020080ba
.L_0200808a:
	movs r0, #157
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200809a
	ldr r0, .L_020080c8
	b .L_020080ba
.L_0200809a:
	ldr r0, .L_020080cc
	b .L_020080ba
.L_0200809e:
	ldr r3, .L_020080d0
	cmp r2, r3
	bne .L_020080b8
	movs r0, #157
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080b4
	ldr r0, .L_020080d4
	b .L_020080ba
.L_020080b4:
	ldr r0, .L_020080d8
	b .L_020080ba
.L_020080b8:
	ldr r0, .L_020080dc
.L_020080ba:
	pop {pc}
.L_020080bc:
	.4byte gPartyState
.L_020080c0:
	.4byte 0x000000ea
.L_020080c4:
	.4byte Data_0200147c
.L_020080c8:
	.4byte Data_020012cc
.L_020080cc:
	.4byte Data_020011ac
.L_020080d0:
	.4byte 0x000000eb
.L_020080d4:
	.4byte Data_0200159c
.L_020080d8:
	.4byte Data_0200165c
.L_020080dc:
	.4byte Data_02001194
	.section .text.x020080e0,"ax",%progbits
	.global Func_020000e0
	.thumb_func
Func_020000e0:
	push {lr}
	ldr r3, .L_02008108
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200810c
	cmp r2, r3
	bne .L_020080f8
	ldr r0, .L_02008110
	b .L_02008104
.L_020080f8:
	ldr r3, .L_02008114
	cmp r2, r3
	bne .L_02008102
	ldr r0, .L_02008118
	b .L_02008104
.L_02008102:
	ldr r0, .L_0200811c
.L_02008104:
	pop {pc}
	.2byte 0x0000
.L_02008108:
	.4byte gPartyState
.L_0200810c:
	.4byte 0x000000ea
.L_02008110:
	.4byte Data_02001728
.L_02008114:
	.4byte 0x000000eb
.L_02008118:
	.4byte Data_020018f0
.L_0200811c:
	.4byte Data_0200171c
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
	.4byte 0x0000289a
.L_02008200:
	.4byte gPartyState
.L_02008204:
	.4byte 0x00019999
	.section .text.x02008208,"ax",%progbits
	.global Func_02000208
	.thumb_func
Func_02000208:
	push {r5, r6, lr}
	movs r0, #157
	lsls r0, r0, #4
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200821a
	b .L_020083dc
.L_0200821a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200822a
	b .L_020083dc
.L_0200822a:
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	movs r0, #192
	movs r1, #1
	movs r2, #154
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	movs r3, #208
	movs r1, #240
	movs r2, #181
	lsls r3, r3, #8
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r0, #9
	bl Func_02001068
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #168
	movs r0, #9
	movs r1, #64
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r2, #154
	movs r0, #9
	movs r1, #70
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r3, #160
	movs r1, #188
	movs r2, #136
	lsls r3, r3, #7
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r0, #10
	bl Func_02001068
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #144
	movs r0, #10
	movs r1, #90
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r2, #158
	lsls r2, r2, #1
	movs r0, #10
	movs r1, #80
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r0, #10
	bl Func_020010b8
	movs r0, #9
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_020083e0
	bl Func_02001098
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #9
	movs r1, #0
	bl Func_020010a8
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #10
	bl Func_020010c8
	movs r0, #10
	movs r1, #0
	bl Func_020010a8
	movs r1, #192
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #9
	movs r1, #0
	bl Func_020010a8
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	bl Func_020010b8
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #0
	bl Func_020010a8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_020010c8
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #9
	movs r1, #0
	bl Func_020010a8
	movs r0, #10
	movs r1, #0
	bl Func_020010a8
	movs r3, #3
	str r3, [sp, #0]
	movs r6, #19
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02000ff8
	movs r5, #4
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02000ff8
	movs r3, #20
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02000ff8
	movs r3, #21
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02000ff8
	movs r0, #155
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_02001018
.L_020083dc:
	add sp, #8
	pop {r5, r6, pc}
.L_020083e0:
	.4byte 0x00002bbc
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
	.4byte 0x00002bd4
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
	.4byte 0x0000289a
.L_02008778:
	.4byte 0x00019999
	.section .text.x0200877c,"ax",%progbits
	.global Func_0200077c
	.thumb_func
Func_0200077c:
	push {lr}
	movs r0, #130
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087aa
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #177
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087aa
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #173
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087aa
	bl Func_0200097c
.L_020087aa:
	pop {pc}
	.section .text.x020087ac,"ax",%progbits
	.global Func_020007ac
	.thumb_func
Func_020007ac:
	push {r5, r6, r7, lr}
	sub sp, #12
	movs r3, #7
	movs r2, #23
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	movs r0, #63
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008854
	ldr r3, .L_02008858
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	ldr r3, [r5, #8]
	movs r7, #0
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_02008816
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02008816
	ldr r0, [r6]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #188
	ldr r0, [r6]
	movs r1, #104
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r7, #1
.L_02008816:
	movs r1, #240
	movs r2, #188
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r0, #14
	bl Func_02001060
	movs r0, #1
	bl WaitFrames
	movs r0, #14
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	cmp r7, #0
	beq .L_02008850
	ldr r3, .L_02008858
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_020010b8
.L_02008850:
	bl Func_02001018
.L_02008854:
	add sp, #12
	pop {r5, r6, r7, pc}
.L_02008858:
	.4byte gPartyState
	.section .text.x0200885c,"ax",%progbits
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	push {lr}
	sub sp, #12
	movs r3, #4
	movs r2, #10
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x0200887c,"ax",%progbits
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {lr}
	sub sp, #12
	movs r3, #6
	movs r2, #9
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x0200889c,"ax",%progbits
	.global Func_0200089c
	.thumb_func
Func_0200089c:
	push {lr}
	sub sp, #12
	movs r3, #6
	movs r2, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	str r2, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x020088bc,"ax",%progbits
	.global Func_020008bc
	.thumb_func
Func_020008bc:
	push {lr}
	sub sp, #12
	movs r3, #12
	movs r2, #6
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #12
	movs r1, #5
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x020088dc,"ax",%progbits
	.global Func_020008dc
	.thumb_func
Func_020008dc:
	push {lr}
	sub sp, #12
	movs r3, #13
	movs r2, #7
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x020088fc,"ax",%progbits
	.global Func_020008fc
	.thumb_func
Func_020008fc:
	push {lr}
	sub sp, #12
	movs r3, #14
	movs r2, #6
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x0200891c,"ax",%progbits
	.global Func_0200091c
	.thumb_func
Func_0200091c:
	push {lr}
	sub sp, #12
	movs r3, #21
	movs r2, #6
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x0200893c,"ax",%progbits
	.global Func_0200093c
	.thumb_func
Func_0200093c:
	push {lr}
	sub sp, #12
	movs r3, #22
	movs r2, #6
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	push {lr}
	sub sp, #12
	movs r3, #23
	movs r2, #7
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #23
	movs r2, #1
	movs r3, #1
	bl Func_02001120
	add sp, #12
	pop {pc}
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
	.4byte 0x00002c85
.L_02008bb0:
	.4byte gPartyState
.L_02008bb4:
	.4byte 0x00019999
	.section .text.x02008bb8,"ax",%progbits
	.global Func_02000bb8
	.thumb_func
Func_02000bb8:
	push {r5, lr}
	ldr r3, .L_02008bec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #16
	ldrb r5, [r3, #9]
	lsls r5, r5, #28
	lsrs r5, r5, #30
	adds r1, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r1, r5, #0
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	adds r1, r5, #0
	bl ObjectMotion_SetActionVariant
	pop {r5, pc}
	.2byte 0x0000
.L_02008bec:
	.4byte gPartyState
	.section .text.x02008bf0,"ax",%progbits
	.global Func_02000bf0
	.thumb_func
Func_02000bf0:
	push {lr}
	ldr r3, .L_02008c10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #13
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl ObjectMotion_SetActionVariant
	pop {pc}
.L_02008c10:
	.4byte gPartyState
	.section .text.x02008c14,"ax",%progbits
	.global Func_02000c14
	.thumb_func
Func_02000c14:
	push {lr}
	ldr r3, .L_02008c74
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008c44
	ldr r3, .L_02008c78
	movs r1, #9
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008c2c:
	ldr r4, .L_02008c7c
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #4
	bne .L_02008c2c
	ldr r3, .L_02008c80
	strh r0, [r3]
.L_02008c44:
	ldr r3, .L_02008c74
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008c72
	ldr r3, .L_02008c84
	movs r1, #15
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008c5a:
	ldr r4, .L_02008c7c
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #12
	bne .L_02008c5a
	ldr r3, .L_02008c88
	strh r0, [r3]
.L_02008c72:
	pop {pc}
.L_02008c74:
	.4byte Data_0300122c
.L_02008c78:
	.4byte 0x05000172
.L_02008c7c:
	.4byte 0x05000160
.L_02008c80:
	.4byte 0x05000168
.L_02008c84:
	.4byte 0x0500017e
.L_02008c88:
	.4byte 0x05000178
	.section .text.x02008c8c,"ax",%progbits
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #8
	ldr r6, [r3, #108]
	bl Func_02001010
	movs r0, #0
	bl Func_02001108
	movs r0, #158
	bl Func_02001128
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #51
	movs r2, #70
	movs r1, #38
	movs r3, #22
	bl Func_02000ff0
	ldr r5, .L_02008d04
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #4
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #8
	bl Battle_WaitMode0
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020010f0
	add sp, #8
	pop {r5, r6, pc}
.L_02008d04:
	.4byte gPartyState
	.section .text.x02008d08,"ax",%progbits
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push {lr}
	ldr r3, .L_02008d30
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008d34
	cmp r2, r3
	bne .L_02008d22
	bl Func_02000dc4
	b .L_02008d2c
.L_02008d22:
	ldr r3, .L_02008d38
	cmp r2, r3
	bne .L_02008d2c
	bl Func_02000ed0
.L_02008d2c:
	movs r0, #0
	pop {pc}
.L_02008d30:
	.4byte gPartyState
.L_02008d34:
	.4byte 0x000000ea
.L_02008d38:
	.4byte 0x000000eb
	.section .text.x02008d3c,"ax",%progbits
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push {r5, r6, lr}
	ldr r3, .L_02008dbc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dc0
	sub sp, #8
	cmp r2, r3
	bne .L_02008db4
	movs r0, #157
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008db4
	movs r5, #10
	movs r6, #4
	movs r0, #54
	movs r1, #47
	movs r2, #4
	movs r3, #15
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl Func_02000ff0
	movs r0, #54
	movs r1, #38
	movs r2, #73
	movs r3, #7
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl Func_02000ff0
	movs r6, #3
	movs r0, #54
	movs r1, #43
	movs r2, #21
	movs r3, #55
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02000ff0
	movs r0, #65
	movs r1, #43
	movs r2, #21
	movs r3, #20
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02000ff0
	movs r0, #4
	movs r1, #47
	movs r2, #4
	movs r3, #50
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02000ff0
.L_02008db4:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008dbc:
	.4byte gPartyState
.L_02008dc0:
	.4byte 0x000000ea
	.section .text.x02008dc4,"ax",%progbits
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	push {r5, r6, lr}
	movs r0, #144
	movs r3, #192
	lsls r0, r0, #4
	lsls r3, r3, #18
	adds r0, #180
	ldr r5, [r3, #32]
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #173
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008dee
	movs r0, #65
	movs r1, #0
	movs r2, #0
	bl Func_02001060
.L_02008dee:
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #23]
	ldr r3, .L_02008ec4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #3
	ldr r0, .L_02008ec8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008ecc
	bl Scheduler_AddOrUpdateCallback
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #17
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e82
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	b .L_02008ec2
.L_02008e82:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #177
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ea2
	movs r3, #192
	movs r1, #252
	movs r2, #180
	lsls r3, r3, #6
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001068
.L_02008ea2:
	ldr r1, [r6]
	movs r0, #19
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r6]
	movs r0, #20
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r6]
	movs r0, #21
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r6]
	movs r0, #22
	bl Object_LinkObjectAndSetCallback
.L_02008ec2:
	pop {r5, r6, pc}
.L_02008ec4:
	.4byte gPartyState
.L_02008ec8:
	.4byte Func_02000c14
.L_02008ecc:
	.4byte Func_02000bb8
	.section .text.x02008ed0,"ax",%progbits
	.global Func_02000ed0
	.thumb_func
Func_02000ed0:
	push {r5, r6, lr}
	ldr r3, .L_02008fc0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #3
	bne .L_02008eec
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008eec:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f1a
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02001060
	b .L_02008f96
.L_02008f1a:
	movs r0, #155
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f96
	movs r1, #140
	movs r2, #154
	movs r0, #9
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02001068
	movs r1, #160
	movs r2, #158
	movs r0, #10
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02001068
	movs r3, #3
	str r3, [sp, #0]
	movs r6, #19
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02000ff8
	movs r5, #4
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02000ff8
	movs r3, #20
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02000ff8
	movs r3, #21
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02000ff8
	movs r0, #1
	bl WaitFrames
.L_02008f96:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fb0
	movs r1, #240
	movs r2, #188
	movs r0, #14
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02001060
.L_02008fb0:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008fc4
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008fc0:
	.4byte gPartyState
.L_02008fc4:
	.4byte Func_02000bf0
	.section .rodata.x02009130,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0x00000127
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000ea
	.4byte 0x00101028
	.4byte 0x002010eb
	.4byte 0x003050eb
	.4byte 0x00434002
	.4byte 0x000000eb
	.4byte 0x001020ea
	.4byte 0x00235002
	.4byte 0x003150ed
	.4byte 0x004040eb
	.4byte 0x005030ea
	.4byte 0x00635002
	.4byte 0x000001ff
	.global Data_02001194
Data_02001194:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020011ac
Data_020011ac:
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020012cc
Data_020012cc:
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00370000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01040000
	.4byte 0x00000000
	.4byte 0x009e0000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00030000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00038000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00030000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00030000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x02040000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00035000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200147c
Data_0200147c:
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff014d
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x006f0000
	.4byte 0x00000000
	.4byte 0x01470000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00004000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200159c
Data_0200159c:
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00008000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x017c0000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x003f00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200165c
Data_0200165c:
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0x190a01a4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x003f00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200171c
Data_0200171c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001728
Data_02001728:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000c8c
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x09b1000a
	.4byte Func_020003e4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000120
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_02000640
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_0200077c
	.4byte 0x00000000
	.4byte 0x09d0000d
	.4byte 0x000028c4
	.4byte 0x00000000
	.4byte 0x09b2000d
	.4byte 0x00002bb8
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002c51
	.4byte 0x00008d15
	.4byte 0x09d0000d
	.4byte 0x000028c8
	.4byte 0x00008d15
	.4byte 0x09b2000d
	.4byte 0x00002bc5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002c55
	.4byte 0x00000000
	.4byte 0x09d0000e
	.4byte 0x000028c5
	.4byte 0x00000000
	.4byte 0x09b2000e
	.4byte 0x00002bb9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002c52
	.4byte 0x00008d15
	.4byte 0x09d0000e
	.4byte 0x000028c9
	.4byte 0x00008d15
	.4byte 0x09b2000e
	.4byte 0x00002bc6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c56
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000028c6
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000028ca
	.4byte 0x00000000
	.4byte 0x09d00010
	.4byte 0x000028c7
	.4byte 0x00000000
	.4byte 0x09b20010
	.4byte 0x00002bbb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002c54
	.4byte 0x00008d15
	.4byte 0x09d00010
	.4byte 0x000028cb
	.4byte 0x00008d15
	.4byte 0x09b20010
	.4byte 0x00002bc8
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002c58
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002bcc
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002bd0
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002bcd
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002bd1
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002bce
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002bd2
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002bcf
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002bd3
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002be2
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002be3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020018f0
Data_020018f0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c401
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c401
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x09b0000a
	.4byte Func_02000208
	.4byte 0x00000400
	.4byte 0xffff000e
	.4byte Func_02000038
	.4byte 0x00000000
	.4byte 0x09b20008
	.4byte 0x00002bba
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002c53
	.4byte 0x00008d15
	.4byte 0x09b20008
	.4byte 0x00002bc7
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002c57
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002bc2
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002bc9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002bc3
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002bca
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002bc4
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002bcb
	.4byte 0x00008f15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte Func_020007ac
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte Func_0200085c
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte Func_0200087c
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte Func_0200089c
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte Func_020008bc
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte Func_020008dc
	.4byte 0x50008905
	.4byte 0xffff001a
	.4byte Func_020008fc
	.4byte 0x50008905
	.4byte 0xffff001b
	.4byte Func_0200091c
	.4byte 0x50008905
	.4byte 0xffff001c
	.4byte Func_0200093c
	.4byte 0x50008905
	.4byte 0xffff001d
	.4byte Func_0200095c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
