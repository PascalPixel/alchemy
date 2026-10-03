.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_02008068
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200806c
	cmp r2, r3
	bne .L_02008064
	ldr r0, .L_02008070
	b .L_02008066
.L_02008064:
	ldr r0, .L_02008074
.L_02008066:
	pop {pc}
.L_02008068:
	.4byte gPartyState
.L_0200806c:
	.4byte 0x0000002a
.L_02008070:
	.4byte Data_0200235c
.L_02008074:
	.4byte Data_020022cc
	.section .text.x02008078,"ax",%progbits
	.global Func_02000078
	.thumb_func
Func_02000078:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #133
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008132
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	movs r5, #8
.L_0200809c:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_020080b4
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
.L_020080b4:
	adds r5, #1
	cmp r5, #63
	bls .L_0200809c
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020080e2
	movs r0, #158
	bl Func_020020d4
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #51
	movs r1, #38
	movs r2, #70
	movs r3, #22
	bl Func_02001f2c
.L_020080e2:
	ldr r5, .L_02008138
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
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
	movs r0, #123
	bl Func_020020d4
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_0200207c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001f64
.L_02008132:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008138:
	.4byte gPartyState
	.section .text.x0200813c,"ax",%progbits
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {lr}
	ldr r3, .L_02008158
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200815c
	cmp r2, r3
	bne .L_02008154
	ldr r0, .L_02008160
	b .L_02008156
.L_02008154:
	ldr r0, .L_02008164
.L_02008156:
	pop {pc}
.L_02008158:
	.4byte gPartyState
.L_0200815c:
	.4byte 0x0000002b
.L_02008160:
	.4byte Data_020023f8
.L_02008164:
	.4byte Data_020023ec
	.section .text.x02008168,"ax",%progbits
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {lr}
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	ldr r0, .L_02008184
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02001f64
	pop {pc}
	.2byte 0x0000
.L_02008184:
	.4byte 0x0000185d
	.section .text.x02008188,"ax",%progbits
	.global Func_02000188
	.thumb_func
Func_02000188:
	push {lr}
	ldr r3, .L_020081bc
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020081b8
	ldr r3, .L_020081c0
	movs r1, #15
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_020081a0:
	ldr r4, .L_020081c4
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #12
	bne .L_020081a0
	ldr r3, .L_020081c8
	strh r0, [r3]
.L_020081b8:
	pop {pc}
	.2byte 0x0000
.L_020081bc:
	.4byte gFrameCount
.L_020081c0:
	.4byte 0x0500017e
.L_020081c4:
	.4byte 0x05000160
.L_020081c8:
	.4byte 0x05000178
	.section .text.x020081cc,"ax",%progbits
	.global Func_020001cc
	.thumb_func
Func_020001cc:
	push {lr}
	ldr r3, .L_02008200
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020081fc
	ldr r3, .L_02008204
	movs r1, #9
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_020081e4:
	ldr r4, .L_02008208
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #4
	bne .L_020081e4
	ldr r3, .L_0200820c
	strh r0, [r3]
.L_020081fc:
	pop {pc}
	.2byte 0x0000
.L_02008200:
	.4byte gFrameCount
.L_02008204:
	.4byte 0x05000172
.L_02008208:
	.4byte 0x05000160
.L_0200820c:
	.4byte 0x05000168
	.section .text.x02008210,"ax",%progbits
	.global Func_02000210
	.thumb_func
Func_02000210:
	push {lr}
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	bl Event_SetStatus1c6
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008254
	bl Scheduler_AddOrUpdateCallback
	bl Event_WaitValue1c8Frames
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #13
	bl Func_02001f34
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #14
	bl Func_02001f34
	bl Func_02001f64
	pop {pc}
	.2byte 0x0000
.L_02008254:
	.4byte Func_020001cc
	.section .text.x02008258,"ax",%progbits
	.global Func_02000258
	.thumb_func
Func_02000258:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	ldr r1, .L_02008284
	movs r3, #132
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r3, [r1]
	str r3, [r2, #8]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #14
	str r3, [r1]
	cmp r3, r2
	bne .L_02008280
	movs r3, #0
	str r3, [r1]
.L_02008280:
	pop {pc}
	.2byte 0x0000
.L_02008284:
	.4byte Data_020024b4
	.section .text.x02008288,"ax",%progbits
	.global Func_02000288
	.thumb_func
Func_02000288:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r5, r5, r2
	bl Func_02002074
	ldr r6, .L_020082c8
	mov r8, r0
	ldr r0, [r6]
	bl Math_Sine
	mov r2, r8
	ldr r3, [r2, #12]
	asrs r0, r0, #2
	adds r3, r3, r0
	str r3, [r2, #12]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #2
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, [r6]
	adds r3, r3, r2
	str r3, [r6]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_020082c8:
	.4byte Data_020024b0
	.section .text.x020082cc,"ax",%progbits
	.global Func_020002cc
	.thumb_func
Func_020002cc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002074
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #192
	movs r0, #171
	movs r2, #204
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r3, #0
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	movs r3, #7
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #60
	movs r1, #53
	movs r2, #67
	movs r3, #16
	bl Func_02001f2c
	ldr r6, .L_020086ac
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	movs r1, #138
	movs r2, #188
	ldr r0, [r6]
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02001fcc
	movs r3, #208
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_020086b0
	ldr r2, .L_020086b4
	bl Func_02001fcc
	movs r2, #202
	movs r0, #7
	ldr r1, .L_020086b8
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02001fcc
	movs r3, #128
	movs r1, #142
	movs r2, #204
	lsls r3, r3, #6
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	mov r8, r3
	bl Func_02001fcc
	movs r3, #128
	movs r2, #220
	lsls r2, r2, #17
	lsls r3, r3, #7
	ldr r1, .L_020086bc
	movs r0, #5
	bl Func_02001fcc
	bl Func_02001f1c
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_020086c0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020086c4
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020086c8
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	bl Event_SetStatus1c6
	ldr r3, .L_020086cc
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_020086d0
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_020086d4
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_020086d8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200205c
	movs r1, #176
	movs r2, #198
	movs r3, #1
	lsls r2, r2, #17
	lsls r1, r1, #14
	ldr r0, .L_020086dc
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #80
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002044
	ldr r0, .L_020086e0
	bl Func_0200201c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r5, #192
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r6]
	lsls r5, r5, #18
	bl Func_0200204c
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #192
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #7
	movs r2, #20
	ldr r0, [r6]
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #7
	bl Func_02002034
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_0200204c
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	mov r1, r8
	bl Func_02002044
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #0
	movs r0, #7
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020084ec
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200851a
.L_020084ec:
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #9
	bl Func_0200204c
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02002034
.L_0200851a:
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_0200204c
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	bl Func_02002044
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200204c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_02002054
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_020086ac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02002034
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r5, r3
	movs r3, #7
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	movs r0, #128
	adds r5, r5, r2
	movs r3, #1
	lsls r0, r0, #4
	strh r3, [r5]
	adds r0, #222
	bl GameFlag_SetBit
	movs r0, #10
	bl Func_0200207c
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020086ac:
	.4byte gPartyState
.L_020086b0:
	.4byte 0x022e0000
.L_020086b4:
	.4byte 0x01590000
.L_020086b8:
	.4byte 0x02160000
.L_020086bc:
	.4byte 0x02220000
.L_020086c0:
	.4byte Data_0200218c
.L_020086c4:
	.4byte Data_020021e8
.L_020086c8:
	.4byte Data_0200221c
.L_020086cc:
	.4byte Data_020024b4
.L_020086d0:
	.4byte Func_02000258
.L_020086d4:
	.4byte Data_020024b0
.L_020086d8:
	.4byte Func_02000288
.L_020086dc:
	.4byte 0x021e0000
.L_020086e0:
	.4byte 0x0000216b
	.section .text.x020086e4,"ax",%progbits
	.global Func_020006e4
	.thumb_func
Func_020006e4:
	push {r5, lr}
	sub sp, #8
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002074
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #192
	movs r0, #229
	movs r2, #204
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r3, #0
	lsls r0, r0, #17
	bl Motion_CamBounds
	ldr r0, .L_02008b10
	bl Func_0200201c
	movs r1, #235
	movs r2, #196
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02001fcc
	movs r2, #24
	movs r3, #128
	movs r0, #6
	movs r1, #0
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r1, #16
	movs r2, #16
	movs r3, #128
	movs r0, #5
	negs r1, r1
	negs r2, r2
	lsls r3, r3, #6
	bl Func_020020b4
	movs r1, #24
	movs r3, #240
	movs r0, #9
	negs r1, r1
	movs r2, #8
	lsls r3, r3, #8
	bl Func_020020b4
	movs r1, #4
	movs r3, #224
	movs r0, #7
	negs r1, r1
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020020b4
	movs r3, #7
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #53
	movs r2, #67
	movs r3, #16
	movs r0, #60
	bl Func_02001f2c
	bl Func_02001f1c
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r3, .L_02008b14
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_02008b18
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02008b1c
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_02008b20
	bl Scheduler_AddOrUpdateCallback
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_0200204c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #6
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #6
	movs r1, #0
	bl Func_0200202c
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #5
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #4
	bl Func_0200204c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #5
	bl Func_02002044
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #5
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200202c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200202c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	bl Func_02002044
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200204c
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_0200202c
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #5
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200204c
	movs r2, #5
	movs r0, #6
	movs r1, #0
	bl Func_0200202c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200202c
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_0200202c
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200204c
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_0200202c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #5
	bl Func_0200202c
	movs r1, #192
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_0200202c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02002044
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_0200202c
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #7
	movs r2, #0
	bl Object_LinkPair
	movs r2, #0
	movs r0, #6
	movs r1, #5
	bl Object_LinkPair
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02008b24
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008a92
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02008a92:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02008b24
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008ad0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02008ad0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02008b24
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008b28
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
	b .L_02008b28
.L_02008b10:
	.4byte 0x000026b2
.L_02008b14:
	.4byte Data_020024b4
.L_02008b18:
	.4byte Func_02000258
.L_02008b1c:
	.4byte Data_020024b0
.L_02008b20:
	.4byte Func_02000288
.L_02008b24:
	.4byte 0x00013333
.L_02008b28:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02008b80
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008b66
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_02008b66:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_02001fc4
	movs r0, #1
	bl Func_0200207c
	add sp, #8
	pop {r5, pc}
.L_02008b80:
	.4byte 0x00013333
	.section .text.x02008b84,"ax",%progbits
	.global Func_02000b84
	.thumb_func
Func_02000b84:
	push {r5, lr}
	ldr r5, .L_02008bd4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #3
	ldr r0, .L_02008bd8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	ldr r0, .L_02008bdc
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r3, #241
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #10
	bne .L_02008bc4
	bl Func_020002cc
.L_02008bc4:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #20
	bne .L_02008bd0
	bl Func_020006e4
.L_02008bd0:
	pop {r5, pc}
	.2byte 0x0000
.L_02008bd4:
	.4byte gPartyState
.L_02008bd8:
	.4byte Func_02000188
.L_02008bdc:
	.4byte Func_020001cc
	.section .text.x02008be0,"ax",%progbits
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	movs r3, #13
	ldrb r2, [r1, #23]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	ldr r5, .L_02008c6c
	strb r3, [r1, #23]
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c70
	cmp r2, r3
	bne .L_02008c0e
	bl Func_02000b84
	b .L_02008c66
.L_02008c0e:
	ldr r3, [r0, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	ldr r0, .L_02008c74
	bl Func_020020c4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008c78
	bl Scheduler_AddOrUpdateCallback
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #99
	bne .L_02008c5c
	bl Func_02000210
	b .L_02008c66
.L_02008c5c:
	movs r1, #144
	ldr r0, .L_02008c7c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_02008c66:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008c6c:
	.4byte gPartyState
.L_02008c70:
	.4byte 0x0000002a
.L_02008c74:
	.4byte Data_020020dc
.L_02008c78:
	.4byte Func_02000188
.L_02008c7c:
	.4byte Func_020001cc
	.section .text.x02008c80,"ax",%progbits
	.global Func_02000c80
	.thumb_func
Func_02000c80:
	push {lr}
	ldr r3, .L_02008cd8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008cdc
	sub sp, #8
	cmp r2, r3
	bne .L_02008cd0
	movs r3, #18
	movs r2, #10
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #2
	movs r3, #24
	str r2, [sp, #4]
	bl Func_02001f2c
	movs r3, #3
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #62
	movs r2, #26
	movs r3, #62
	bl Func_02001f2c
	movs r3, #6
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #65
	movs r2, #25
	movs r3, #61
	bl Func_02001f2c
.L_02008cd0:
	movs r0, #0
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_02008cd8:
	.4byte gPartyState
.L_02008cdc:
	.4byte 0x0000002a
	.section .text.x02008ce0,"ax",%progbits
	.global Func_02000ce0
	.thumb_func
Func_02000ce0:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008cfa
	movs r0, #152
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02009048
.L_02008cfa:
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #136
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	ldr r6, .L_0200904c
	adds r0, r6, #0
	bl Func_0200201c
	movs r2, #16
	movs r3, #128
	movs r0, #5
	movs r1, #16
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r2, #16
	movs r3, #128
	movs r0, #9
	movs r1, #32
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r1, #16
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #7
	negs r1, r1
	negs r2, r2
	movs r0, #6
	bl Func_020020b4
	movs r0, #80
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_0200204c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009050
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #150
	lsls r1, r1, #1
	movs r2, #152
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009054
	adds r1, #153
	bl Func_0200205c
	movs r0, #200
	movs r1, #1
	movs r2, #172
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_02009058
	adds r1, #102
	bl Func_0200205c
	movs r0, #150
	movs r1, #1
	movs r2, #172
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #204
	lsls r1, r1, #8
	ldr r0, .L_0200905c
	adds r1, #204
	bl Func_0200205c
	movs r0, #132
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_0200206c
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002044
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02009060
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	ldr r5, [r5]
	cmp r0, #0
	bne .L_02008ece
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008ee2
.L_02008ece:
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	adds r0, r6, #5
	bl Func_0200201c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
.L_02008ee2:
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002044
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #1
	movs r2, #144
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009058
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009058
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009058
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008fd8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_02008fd8:
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008ff8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02008ff8:
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009018
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009018:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	bl Func_02001f64
	movs r0, #152
	lsls r0, r0, #4
	bl GameFlag_SetBit
.L_02009048:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200904c:
	.4byte 0x00001846
.L_02009050:
	.4byte 0x00019999
.L_02009054:
	.4byte 0x0004cccc
.L_02009058:
	.4byte 0x00013333
.L_0200905c:
	.4byte 0x00066666
.L_02009060:
	.4byte gPartyState
	.section .text.x02009064,"ax",%progbits
	.global Func_02001064
	.thumb_func
Func_02001064:
	push {lr}
	ldr r0, .L_02009070
	bl Func_020020cc
	pop {pc}
	.2byte 0x0000
.L_02009070:
	.4byte Data_020020dc
	.section .text.x02009074,"ax",%progbits
	.global Func_02001074
	.thumb_func
Func_02001074:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200916c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #136
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200916c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_SetBit
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200914c
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	ldr r5, .L_02009170
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r1, [r5]
	movs r0, #9
	bl Func_02001fd4
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #208
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #9
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02002044
	ldr r0, .L_02009174
	bl Func_0200201c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200912c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_0200912c:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #136
	bl GameFlag_SetBit
	bl Func_02001f64
	b .L_0200916c
.L_0200914c:
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009164
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_0200916c
.L_02009164:
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200916c:
	pop {r5, pc}
	.2byte 0x0000
.L_02009170:
	.4byte gPartyState
.L_02009174:
	.4byte 0x0000185e
	.section .text.x02009178,"ax",%progbits
	.global Func_02001178
	.thumb_func
Func_02001178:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02009188,"ax",%progbits
	.global Func_02001188
	.thumb_func
Func_02001188:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #12
	movs r3, #180
	movs r7, #128
	movs r6, #212
	mov r5, sp
	lsls r3, r3, #17
	lsls r7, r7, #15
	lsls r6, r6, #17
	str r3, [r5]
	str r7, [r5, #4]
	str r6, [r5, #8]
	mov r8, r3
	bl Random16Far
	adds r1, r0, #0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #14
	bl Vector_AddPolarOffsetFar
	movs r0, #128
	lsls r0, r0, #2
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, #162
	bl Func_02001f0c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009202
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001efc
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	adds r3, r6, #0
	bl Func_02001f24
	ldr r1, .L_0200920c
	adds r0, r5, #0
	bl Func_02001f04
.L_02009202:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200920c:
	.4byte Data_0200247c
	.section .text.x02009210,"ax",%progbits
	.global Func_02001210
	.thumb_func
Func_02001210:
	push {r5, r6, r7, lr}
	movs r0, #234
	movs r1, #180
	movs r2, #128
	movs r3, #212
	adds r0, #255
	lsls r1, r1, #17
	lsls r2, r2, #15
	lsls r3, r3, #17
	bl Func_02001f0c
	adds r6, r0, #0
	movs r7, #0
	movs r0, #0
	cmp r6, #0
	beq .L_02009288
	ldr r5, [r6, #80]
	movs r3, #33
	ldrb r2, [r5, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r6, #0
	adds r3, #85
	adds r2, r6, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	movs r1, #193
	strb r3, [r2]
	lsls r1, r1, #3
	strb r7, [r5, #26]
	strb r7, [r5, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r7, r0, #0
	movs r0, #242
	bl Func_02001f4c
	movs r3, #128
	lsls r3, r3, #3
	adds r2, r7, r3
	movs r1, #128
	ldrb r0, [r5, #16]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	adds r0, r6, #0
.L_02009288:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200928c,"ax",%progbits
	.global Func_0200128c
	.thumb_func
Func_0200128c:
	push {r5, r6, r7, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #133
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020092a0
	b .L_020094a8
.L_020092a0:
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020092ac
	b .L_020094a8
.L_020092ac:
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	ldr r3, .L_020094ac
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r1, [r6]
	movs r0, #7
	bl Func_02001fd4
	movs r0, #1
	bl WaitFrames
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_020094b0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #206
	movs r2, #212
	lsls r2, r2, #1
	movs r0, #7
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002044
	ldr r0, .L_020094b4
	bl Func_0200201c
	movs r2, #10
	movs r0, #7
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #193
	movs r2, #212
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	ldr r1, [r6]
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	ldr r0, [r6]
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	ldr r1, .L_020094b8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #180
	movs r2, #212
	lsls r2, r2, #1
	movs r0, #7
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002044
	movs r0, #182
	bl Func_020020d4
	bl Func_02001210
	adds r7, r0, #0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #220
	bl Func_020020d4
	ldr r5, .L_020094bc
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02002084
	movs r0, #20
	bl Func_0200208c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #6
	bl Func_02002084
	movs r0, #20
	bl Func_0200208c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02002084
	movs r0, #20
	bl Func_0200208c
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #51
	movs r1, #38
	movs r2, #70
	movs r3, #22
	bl Func_02001f2c
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	cmp r7, #0
	beq .L_0200942c
	adds r0, r7, #0
	bl Func_02001f14
.L_0200942c:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02002084
	movs r0, #20
	bl Func_0200208c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200948a
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200948a:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02001fc4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #133
	bl GameFlag_SetBit
	bl Func_02001f64
.L_020094a8:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_020094ac:
	.4byte gPartyState
.L_020094b0:
	.4byte 0x00019999
.L_020094b4:
	.4byte 0x00002163
.L_020094b8:
	.4byte Data_02002120
.L_020094bc:
	.4byte Func_02001188
	.section .text.x020094c0,"ax",%progbits
	.global Func_020014c0
	.thumb_func
Func_020014c0:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020094d4
	bl .L_02009d50
.L_020094d4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094e6
	bl .L_02009d50
.L_020094e6:
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	ldr r0, .L_02009770
	bl Func_0200201c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #176
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #128
	movs r2, #8
	lsls r3, r3, #7
	movs r0, #7
	movs r1, #16
	bl Func_020020b4
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200205c
	movs r0, #144
	movs r1, #1
	movs r2, #208
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	movs r3, #128
	movs r0, #9
	movs r1, #32
	movs r2, #0
	lsls r3, r3, #7
	bl Func_020020b4
	movs r2, #8
	movs r3, #128
	movs r0, #6
	movs r1, #48
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020020b4
	movs r1, #16
	movs r2, #8
	movs r3, #128
	lsls r3, r3, #7
	negs r1, r1
	negs r2, r2
	movs r0, #5
	bl Func_020020b4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #7
	ldr r1, .L_02009774
	ldr r2, .L_02009778
	bl ObjectMotion_SetSpeedParameters
	movs r1, #140
	lsls r1, r1, #1
	movs r2, #224
	movs r0, #7
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #20
	movs r0, #7
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200205c
	movs r0, #144
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	negs r2, r2
	movs r0, #7
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #129
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_0200204c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002054
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	ldr r3, .L_0200977c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009780
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_020097ca
.L_02009770:
	.4byte 0x00002132
.L_02009774:
	.4byte 0x00026666
.L_02009778:
	.4byte 0x00013333
.L_0200977c:
	.4byte gPartyState
.L_02009780:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #160
	adds r3, #2
	strh r3, [r2]
	movs r0, #7
	movs r2, #0
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
.L_020097ca:
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #144
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	ldr r5, .L_02009c04
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200204c
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #6
	bl Func_0200204c
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002044
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02002054
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200204c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_0200204c
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200204c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200204c
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Func_02002044
	movs r1, #128
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #3
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02002054
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_0200204c
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #4
	bl Func_0200204c
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #5
	bl Func_0200204c
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02002034
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #7
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #6
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_0200204c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Func_0200204c
	movs r2, #16
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	movs r1, #0
	bl Func_02002034
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #20
	movs r0, #7
	movs r1, #0
	bl Func_0200202c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #0
	b .L_02009c08
	.2byte 0x0000
.L_02009c04:
	.4byte gPartyState
.L_02009c08:
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02009c3e
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009c60
.L_02009c3e:
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02002034
.L_02009c60:
	ldr r5, .L_02009d54
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	ldr r0, [r5]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02002044
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_0200204c
	movs r0, #9
	movs r1, #0
	bl Func_02002034
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200204c
	movs r0, #7
	movs r1, #0
	bl Func_02002034
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02002044
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009d58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009d58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009d58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_02009d58
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02009d5c
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #9
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #6
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #129
	bl GameFlag_SetBit
	bl Func_02001f64
.L_02009d50:
	pop {r5, pc}
	.2byte 0x0000
.L_02009d54:
	.4byte gPartyState
.L_02009d58:
	.4byte 0x00013333
.L_02009d5c:
	.4byte Data_020020e4
	.section .text.x02009d60,"ax",%progbits
	.global Func_02001d60
	.thumb_func
Func_02001d60:
	push {r5, lr}
	ldr r3, .L_02009d98
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009d96
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	bl Func_02001f0c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009d96
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Func_02001efc
	ldr r1, .L_02009d9c
	adds r0, r5, #0
	bl Func_02001f04
.L_02009d96:
	pop {r5, pc}
.L_02009d98:
	.4byte gFrameCount
.L_02009d9c:
	.4byte Data_02002498
	.section .text.x02009da0,"ax",%progbits
	.global Func_02001da0
	.thumb_func
Func_02001da0:
	push {r5, r6, r7, lr}
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e8e
	bl Func_02001f5c
	movs r0, #0
	bl Func_020020ac
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #4
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	bl Object_GetById
	ldr r6, .L_02009e90
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #8
	movs r1, #16
	str r6, [r0, #108]
	movs r0, #7
	bl Func_020020b4
	movs r0, #7
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r5, .L_02009e94
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02009e98
	bl Func_0200201c
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	bl Object_GetById
	movs r7, #0
	str r7, [r0, #108]
	movs r1, #0
	movs r0, #7
	bl Func_02002034
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	movs r2, #16
	str r6, [r0, #108]
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	str r7, [r0, #108]
	movs r0, #7
	bl Object_GetById
	movs r1, #2
	str r6, [r0, #108]
	movs r0, #7
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009e66
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009e66:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02001fc4
	movs r0, #7
	bl Object_GetById
	str r7, [r0, #108]
	movs r0, #7
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	bl Func_02001f64
.L_02009e8e:
	pop {r5, r6, r7, pc}
.L_02009e90:
	.4byte Func_02001d60
.L_02009e94:
	.4byte gPartyState
.L_02009e98:
	.4byte 0x00002162
	.section .rodata.x0200a0dc,"a",%progbits
	.global Data_020020dc
Data_020020dc:
	.4byte 0x02000008
	.4byte 0x0000ffff
	.global Data_020020e4
Data_020020e4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02002120
Data_02002120:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_0200218c
Data_0200218c:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_020021e8
Data_020021e8:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200221c
Data_0200221c:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
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
	.4byte 0x0000002b
	.4byte 0x00101028
	.4byte 0x00144002
	.4byte 0x00208002
	.4byte 0x0000002a
	.4byte 0x0012a002
	.4byte 0x00208002
	.4byte 0x00a47002
	.4byte 0x000001ff
	.global Data_020022cc
Data_020022cc:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200235c
Data_0200235c:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020023ec
Data_020023ec:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020023f8
Data_020023f8:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000078
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0980000a
	.4byte Func_02000ce0
	.4byte 0x00000002
	.4byte 0x0981000b
	.4byte Func_020014c0
	.4byte 0x00000002
	.4byte 0x0989000c
	.4byte Func_02001da0
	.4byte 0x00000002
	.4byte 0x09850001
	.4byte Func_0200128c
	.4byte 0x00000003
	.4byte 0x0985000e
	.4byte Func_02000168
	.4byte 0x00000002
	.4byte 0x0985000f
	.4byte Func_02001074
	.4byte 0x00000002
	.4byte 0x09850010
	.4byte Func_02001178
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte Func_02001064
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200247c
Data_0200247c:
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02002498
Data_02002498:
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.section .bss,"aw",%nobits
	.global Data_020024b0
Data_020024b0:
	.space 0x00000004
	.global Data_020024b4
Data_020024b4:
