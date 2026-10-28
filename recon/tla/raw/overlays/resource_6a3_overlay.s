.syntax unified
	.thumb
	.section .text.x020080c4,"ax",%progbits
	.balign 4
	.global Func_020000c4
	.thumb_func
Func_020000c4:
	push {r5, r6, lr}
	ldr r2, .L_0200814c
	movs r3, #0
	strb r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	sub sp, #12
	strh r3, [r1]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #3
	adds r3, #8
	strh r2, [r3]
	movs r0, #128
	lsls r0, r0, #4
	bl MemAlloc
	adds r5, r0, #0
	ldr r0, .L_02008150
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0200468c
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_02008154
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl MemFree
	ldr r3, .L_02008148
	mov r0, sp
	adds r0, #10
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_02008158
	ldr r2, .L_0200815c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	bl MemAlloc
	adds r5, r0, #0
	ldr r0, .L_02008160
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0200468c
	movs r6, #0
	adds r4, r5, #0
	b .L_02008164
.L_02008148:
	.4byte 0x00000000
.L_0200814c:
	.4byte gCpuLoadDisplayEnabled
.L_02008150:
	.4byte 0x000001c0
.L_02008154:
	.4byte 0x84000200
.L_02008158:
	.4byte 0x06002000
.L_0200815c:
	.4byte 0x81000400
.L_02008160:
	.4byte 0x000001c1
.L_02008164:
	ldrh r2, [r4]
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	adds r6, #1
	strh r3, [r4]
	adds r4, #2
	cmp r6, #64
	bne .L_02008164
	adds r4, r5, #0
	movs r6, #0
.L_0200817c:
	ldr r2, .L_020081e8
	lsls r1, r6, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #1
	adds r4, #8
	cmp r6, #16
	bne .L_0200817c
	adds r0, r5, #0
	bl MemFree
	ldr r3, .L_020081e0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_020081e4
	subs r2, #2
	strh r3, [r2]
	ldr r2, .L_020081ec
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #152
	strh r3, [r2]
	strh r6, [r2, #2]
	movs r0, #30
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02004924
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200491c
	b .L_020081f0
	.2byte 0x0000
.L_020081e0:
	.4byte 0x00001010
.L_020081e4:
	.4byte 0x00003f41
.L_020081e8:
	.4byte 0x06002000
.L_020081ec:
	.4byte gBgScroll
.L_020081f0:
	movs r0, #1
	bl Func_0200492c
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02004924
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_0200491c
	movs r0, #8
	bl Func_0200492c
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02008260
	movs r0, #144
	orrs r3, r2
	strh r3, [r1]
	bl Func_020049c4
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_0200475c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_0200475c
	ldr r3, .L_02008264
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #128
	movs r1, #128
	ldr r0, [r3]
	b .L_02008268
	.2byte 0x0000
.L_02008260:
	.4byte 0x00000100
.L_02008264:
	.4byte gAdventure
.L_02008268:
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #6
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_Launch
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #16
	strb r3, [r0]
	negs r1, r1
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #20
	bl Object_GetById
	ldr r3, .L_020082cc
	movs r6, #0
	str r3, [r0, #108]
.L_020082a2:
	ldr r2, .L_020082c4
	lsrs r3, r6, #1
	subs r2, r2, r3
	ldr r3, .L_020082c8
	movs r5, #128
	lsls r5, r5, #19
	orrs r2, r3
	adds r5, #82
	strh r2, [r5]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #32
	bne .L_020082a2
	b .L_020082d0
	.2byte 0x0000
.L_020082c4:
	.4byte 0x00000010
.L_020082c8:
	.4byte 0x00001000
.L_020082cc:
	.4byte Func_0200354c
.L_020082d0:
	movs r3, #61
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #42
	movs r2, #5
	movs r3, #11
	bl Func_0200474c
	movs r3, #63
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #106
	movs r1, #52
	movs r2, #1
	movs r3, #1
	bl Func_0200474c
	movs r0, #130
	movs r1, #160
	movs r2, #2
	movs r3, #236
	lsls r1, r1, #16
	lsls r0, r0, #19
	bl Func_020049ac
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02004924
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_0200491c
	movs r0, #60
	bl Func_0200492c
	bl Func_020046b4
	bl Func_020046a4
	ldr r3, .L_02008388
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02004784
	ldr r3, .L_0200837c
	movs r2, #128
	strh r3, [r5]
	ldr r3, .L_02008380
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02008384
	movs r0, #1
	orrs r3, r2
	ldr r2, .L_0200838c
	strh r3, [r1]
	movs r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
	bl WaitFrames
	ldr r2, .L_02008390
	movs r3, #1
	strb r3, [r2]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	b .L_02008394
.L_0200837c:
	.4byte 0x00001008
.L_02008380:
	.4byte 0x00003f10
.L_02008384:
	.4byte 0x00000100
.L_02008388:
	.4byte gAdventure
.L_0200838c:
	.4byte gBgScroll
.L_02008390:
	.4byte gCpuLoadDisplayEnabled
.L_02008394:
	bl Func_0200475c
	add sp, #12
	pop {r5, r6, pc}
	.section .text.x020083b8,"ax",%progbits
	.balign 4
	.global Func_020003b8
	.thumb_func
Func_020003b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008604
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	adds r5, r1, #0
	bl Object_GetById
	mov r8, r0
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #63
	beq .L_020083e2
	b .L_020085ee
.L_020083e2:
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #15
	beq .L_020083ec
	b .L_020085ee
.L_020083ec:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #12
	adds r7, r6, #0
	bl GameFlag_SetBit
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
	movs r0, #2
	bl WaitFrames
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	movs r5, #0
	b .L_0200841c
.L_0200840c:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #29
	bgt .L_02008426
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
.L_0200841c:
	cmp r2, r3
	bgt .L_0200840c
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_0200840c
.L_02008426:
	movs r3, #0
	strb r3, [r7]
	movs r0, #134
	bl Func_020049c4
	bl Func_020047d4
	movs r0, #0
	bl Func_0200494c
	movs r0, #254
	movs r1, #1
	movs r2, #232
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	bl Func_020048f4
	ldr r3, .L_02008604
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r2, r2, #8
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	mov r2, r8
	ldr r3, [r2, #8]
	asrs r0, r3, #20
	cmp r0, #64
	bne .L_0200848a
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #3
	movs r2, #232
	bl ObjectMotion_SetPositionAndReset
	movs r1, #250
	ldr r0, [r5]
	lsls r1, r1, #2
	movs r2, #232
	bl ObjectMotion_SetPositionAndReset
	b .L_0200849a
.L_0200848a:
	cmp r0, #63
	bne .L_0200849a
	movs r1, #250
	ldr r0, [r5]
	lsls r1, r1, #2
	movs r2, #232
	bl ObjectMotion_SetPositionAndReset
.L_0200849a:
	ldr r5, .L_02008604
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #250
	movs r2, #248
	ldr r0, [r5]
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_020048b4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004924
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_0200491c
	movs r0, #60
	bl Func_0200492c
	movs r0, #50
	bl FieldEvent_WaitFrames
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r2, #50
	ldr r0, [r5]
	adds r1, #255
	bl Func_020048cc
	ldr r0, [r5]
	movs r1, #0
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #22
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #0
	bl Func_02002efc
	bl Func_020000c4
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #4
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020048d4
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_020048e4
	movs r0, #131
	movs r2, #174
	movs r1, #0
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #19
	bl Camera_MoveWithinMapBounds
	movs r5, #0
	bl Func_020048f4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #80
	str r5, [r6, #108]
	bl Func_020049c4
	bl Sound_WaitForJingle
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #246
	movs r2, #248
	ldr r1, .L_02008608
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020048f4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #1
	bl Func_02002efc
	bl Func_020047dc
.L_020085ee:
	movs r3, #6
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #0
	negs r3, r3
	bl Func_02004774
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008604:
	.4byte gAdventure
.L_02008608:
	.4byte 0xffc00000
	.section .text.x02008a78,"ax",%progbits
	.global Func_02000a78
	.thumb_func
Func_02000a78:
	push {r5, r6, lr}
	movs r0, #1
	bl Object_GetById
	cmp r0, #0
	beq .L_02008ac4
	ldr r6, .L_02008ac8
	movs r2, #127
	ldr r3, [r6]
	ldr r5, [r0, #80]
	ands r3, r2
	cmp r3, #0
	bne .L_02008a9c
	movs r1, #129
	movs r0, #29
	lsls r1, r1, #1
	bl Func_020048d4
.L_02008a9c:
	cmp r5, #0
	beq .L_02008ac4
	movs r3, #30
	strb r3, [r5, #23]
	movs r3, #252
	strb r3, [r5, #22]
	movs r3, #182
	ldr r2, [r6]
	lsls r3, r3, #2
	adds r0, r2, #0
	muls r0, r3
	bl Math_Sine
	cmp r0, #0
	bge .L_02008abc
	adds r0, #127
.L_02008abc:
	ldr r2, .L_02008acc
	asrs r3, r0, #7
	adds r3, r3, r2
	strh r3, [r5, #18]
.L_02008ac4:
	movs r0, #1
	pop {r5, r6, pc}
.L_02008ac8:
	.4byte gFrameCount
.L_02008acc:
	.4byte 0xfffff800
	.section .text.x02009ed0,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02001ed0
	.thumb_func
Func_02001ed0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_020047d4
	movs r0, #0
	bl Func_0200494c
	movs r2, #209
	movs r1, #0
	ldr r0, .L_ja_0200a2e0
	lsls r2, r2, #16
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	ldr r3, .L_ja_0200a2e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #4
	str r2, [r3]
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_AddActiveOwner
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	movs r1, #166
	movs r2, #176
	lsls r3, r3, #7
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #15
	movs r6, #128
	mov r8, r3
	lsls r6, r6, #6
	bl Func_0200484c
	movs r2, #208
	adds r3, r6, #0
	movs r0, #27
	ldr r1, .L_ja_0200a2e8
	lsls r2, r2, #16
	bl Func_0200484c
	movs r2, #195
	adds r3, r6, #0
	movs r0, #26
	ldr r1, .L_ja_0200a2e0
	lsls r2, r2, #16
	movs r5, #160
	bl Func_0200484c
	lsls r5, r5, #8
	movs r2, #219
	adds r3, r5, #0
	movs r0, #0
	ldr r1, .L_ja_0200a2ec
	lsls r2, r2, #16
	bl Func_0200484c
	movs r2, #232
	adds r3, r5, #0
	lsls r2, r2, #16
	movs r0, #2
	ldr r1, .L_ja_0200a2f0
	bl Func_0200484c
	ldr r1, .L_ja_0200a2f4
	movs r0, #3
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #39
	movs r0, #0
	bl Object_SetModeById
	movs r0, #2
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #32
	movs r0, #2
	bl Object_SetModeById
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_ja_0200a2f8
	bl Func_0200488c
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #8
	movs r0, #26
	bl Object_SetModeById
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #0
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r0, #0
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #1
	movs r0, #26
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #26
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	bl Func_020048fc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	movs r0, #20
	bl FieldEvent_WaitFrames
	mov r3, r8
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_02004964
	movs r0, #6
	movs r1, #1
	bl Camera_FollowActor
	mov r3, r8
	movs r0, #7
	movs r1, #16
	movs r2, #0
	bl Func_02004964
	movs r1, #16
	adds r3, r6, #0
	movs r0, #28
	negs r1, r1
	movs r2, #24
	bl Func_02004964
	movs r1, #16
	adds r3, r6, #0
	movs r0, #5
	negs r1, r1
	movs r2, #0
	bl Func_02004964
	movs r0, #4
	movs r1, #16
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	mov r1, r8
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r2, #5
	movs r0, #28
	movs r1, #0
	bl Func_0200489c
	movs r1, #0
	movs r0, #28
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #35
	movs r0, #28
	movs r1, #0
	bl Func_0200489c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #214
	lsls r1, r1, #1
	movs r0, #4
	adds r1, #255
	movs r2, #186
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #206
	lsls r1, r1, #1
	movs r2, #202
	movs r0, #28
	adds r1, #255
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r1, .L_ja_0200a2fc
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_ja_0200a300
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020048cc
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200489c
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #6
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndReset
	ldr r1, .L_ja_0200a304
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_ja_0200a308
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_ja_0200a30c
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	bl Func_020048f4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	mov r1, r8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_020048b4
	movs r0, #26
	movs r1, #0
	movs r2, #5
	b .L_ja_0200a310
.L_ja_0200a2e0:
	.4byte 0x02e10000
.L_ja_0200a2e4:
	.4byte gAdventure
.L_ja_0200a2e8:
	.4byte 0x02cd0000
.L_ja_0200a2ec:
	.4byte 0x02f90000
.L_ja_0200a2f0:
	.4byte 0x02e50000
.L_ja_0200a2f4:
	.4byte gData_02005548
.L_ja_0200a2f8:
	.4byte 0x00002bb8
.L_ja_0200a2fc:
	.4byte gData_02004a6c
.L_ja_0200a300:
	.4byte gData_02004a90
.L_ja_0200a304:
	.4byte gData_02004ab4
.L_ja_0200a308:
	.4byte gData_02004ae8
.L_ja_0200a30c:
	.4byte gData_02004b1c
.L_ja_0200a310:
	bl Func_0200489c
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r2, #5
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #27
	bl Func_020048cc
	movs r1, #0
	movs r0, #27
	bl UiText_OpenMessageAtObject
	movs r0, #28
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #4
	movs r0, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200a3ea
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_0200a412
.L_ja_0200a3ea:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #27
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
.L_ja_0200a412:
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020048cc
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #27
	movs r2, #0
	movs r0, #26
	bl Object_TurnToFaceEachOther
	movs r0, #26
	bl Object_RefreshSelectorById
	movs r0, #27
	bl Object_RefreshSelectorById
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #26
	bl Func_020048cc
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r0, #28
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #28
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #28
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #27
	movs r1, #28
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r2, #5
	movs r0, #28
	bl Func_0200489c
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r1, #0
	movs r0, #27
	bl Func_0200489c
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #7
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #5
	movs r2, #0
	bl Object_TurnToFaceEachOther
	movs r1, #28
	movs r2, #0
	movs r0, #4
	bl Object_TurnToFaceEachOther
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #28
	bl Func_020048cc
	movs r0, #4
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #28
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #26
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200a6b6
	movs r1, #4
	movs r0, #26
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r2, #5
	movs r1, #0
	bl Func_0200489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_0200a6e0
.L_ja_0200a6b6:
	movs r1, #4
	movs r0, #26
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
.L_ja_0200a6e0:
	movs r1, #4
	movs r2, #0
	movs r0, #28
	bl Object_TurnToFaceEachOther
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #28
	movs r2, #0
	movs r0, #4
	bl Object_TurnToFaceEachOther
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #27
	movs r2, #0
	movs r0, #26
	bl Object_TurnToFaceEachOther
	movs r0, #26
	bl Object_RefreshSelectorById
	movs r0, #27
	bl Object_RefreshSelectorById
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #27
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #4
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #222
	lsls r1, r1, #1
	movs r0, #27
	adds r1, #255
	movs r2, #202
	bl ObjectMotion_SetPositionAndReset
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #4
	movs r1, #7
	movs r2, #0
	bl Object_TurnToFaceEachOther
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Object_TurnToFaceEachOther
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #5
	bl Object_RefreshSelectorById
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #8
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r2, #4
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #27
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r1, #0
	movs r0, #0
	bl Func_0200489c
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #4
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #27
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #4
	movs r1, #5
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #2
	negs r2, r2
	movs r1, #56
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #0
	bl ObjectMotion_SetVariantCallback
	movs r0, #4
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #223
	bl PartyInventory_Remove
	movs r0, #245
	bl PartyInventory_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r0, [r3]
	movs r1, #5
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl UiText_ShowPositionedMessageAndWait
	movs r5, #0
	movs r6, #216
	b .L_ja_0200a9f4
.L_ja_0200a9f0:
	adds r6, #2
	adds r5, #1
.L_ja_0200a9f4:
	cmp r5, #14
	bgt .L_ja_0200aa18
	movs r0, #4
	bl Unit_Get
	ldr r3, .L_ja_0200aa14
	ldrh r2, [r0, r6]
	ands r3, r2
	cmp r3, #245
	bne .L_ja_0200a9f0
	movs r0, #4
	adds r1, r5, #0
	bl Func_020047a4
	b .L_ja_0200aa18
	.2byte 0x0000
.L_ja_0200aa14:
	.4byte 0x000001ff
.L_ja_0200aa18:
	movs r1, #3
	movs r0, #27
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #27
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #4
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r6, .L_ja_0200ac48
	movs r0, #26
	adds r1, r6, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #16
	movs r2, #0
	negs r1, r1
	movs r0, #7
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #10
	bl FieldEvent_WaitFrames
	adds r1, r6, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #110
	bl FieldEvent_WaitFrames
	adds r1, r6, #0
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #26
	bl Object_RefreshSelectorById
	ldr r5, .L_ja_0200ac4c
	movs r0, #26
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #27
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Object_SetModeById
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200489c
	movs r0, #6
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020048cc
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020048cc
	movs r2, #5
	movs r0, #6
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #7
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	adds r1, r6, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #7
	bl Object_RefreshSelectorById
	movs r0, #7
	bl Party_AddActiveOwner
	movs r0, #4
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02004844
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #166
	movs r0, #4
	lsls r1, r1, #2
	movs r2, #70
	bl ObjectMotion_SetPositionAndReset
	bl Func_020047dc
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_ja_0200ac48:
	.4byte gData_020059a4
.L_ja_0200ac4c:
	.4byte gData_02005a00
	.else
	.global Func_02001ed0
	.thumb_func
Func_02001ed0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_020047d4
	movs r0, #0
	bl Func_0200494c
	movs r2, #209
	movs r1, #0
	ldr r0, .L_0200a2e0
	lsls r2, r2, #16
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	ldr r3, .L_0200a2e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #4
	str r2, [r3]
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_AddActiveOwner
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	movs r1, #166
	movs r2, #176
	lsls r3, r3, #7
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #15
	movs r6, #128
	mov r8, r3
	lsls r6, r6, #6
	bl Func_0200484c
	movs r2, #208
	adds r3, r6, #0
	movs r0, #27
	ldr r1, .L_0200a2e8
	lsls r2, r2, #16
	bl Func_0200484c
	movs r2, #195
	adds r3, r6, #0
	movs r0, #26
	ldr r1, .L_0200a2e0
	lsls r2, r2, #16
	movs r5, #160
	bl Func_0200484c
	lsls r5, r5, #8
	movs r2, #219
	adds r3, r5, #0
	movs r0, #0
	ldr r1, .L_0200a2ec
	lsls r2, r2, #16
	bl Func_0200484c
	movs r2, #232
	adds r3, r5, #0
	lsls r2, r2, #16
	movs r0, #2
	ldr r1, .L_0200a2f0
	bl Func_0200484c
	ldr r1, .L_0200a2f4
	movs r0, #3
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #39
	movs r0, #0
	bl Object_SetModeById
	movs r0, #2
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #32
	movs r0, #2
	bl Object_SetModeById
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_0200a2f8
	bl Func_0200488c
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #8
	movs r0, #26
	bl Object_SetModeById
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #0
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r0, #0
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #1
	movs r0, #26
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #26
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	bl Func_020048fc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	movs r0, #20
	bl FieldEvent_WaitFrames
	mov r3, r8
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_02004964
	movs r0, #6
	movs r1, #1
	bl Camera_FollowActor
	mov r3, r8
	movs r0, #7
	movs r1, #16
	movs r2, #0
	bl Func_02004964
	movs r1, #16
	adds r3, r6, #0
	movs r0, #28
	negs r1, r1
	movs r2, #24
	bl Func_02004964
	movs r1, #16
	adds r3, r6, #0
	movs r0, #5
	negs r1, r1
	movs r2, #0
	bl Func_02004964
	movs r0, #4
	movs r1, #16
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	mov r1, r8
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r2, #5
	movs r0, #28
	movs r1, #0
	bl Func_0200489c
	movs r1, #0
	movs r0, #28
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #35
	movs r0, #28
	movs r1, #0
	bl Func_0200489c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #214
	lsls r1, r1, #1
	movs r0, #4
	adds r1, #255
	movs r2, #186
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #206
	lsls r1, r1, #1
	movs r2, #202
	movs r0, #28
	adds r1, #255
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r1, .L_0200a2fc
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a300
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020048cc
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200489c
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #6
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndReset
	ldr r1, .L_0200a304
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a308
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a30c
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	bl Func_020048f4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	mov r1, r8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_020048b4
	movs r0, #26
	movs r1, #0
	movs r2, #5
	b .L_0200a310
.L_0200a2e0:
	.4byte 0x02e10000
.L_0200a2e4:
	.4byte gAdventure
.L_0200a2e8:
	.4byte 0x02cd0000
.L_0200a2ec:
	.4byte 0x02f90000
.L_0200a2f0:
	.4byte 0x02e50000
.L_0200a2f4:
	.4byte gData_02005548
.L_0200a2f8:
	.4byte MsgWeDidItAgatio
.L_0200a2fc:
	.4byte gData_02004a6c
.L_0200a300:
	.4byte gData_02004a90
.L_0200a304:
	.4byte gData_02004ab4
.L_0200a308:
	.4byte gData_02004ae8
.L_0200a30c:
	.4byte gData_02004b1c
.L_0200a310:
	bl Func_0200489c
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r2, #5
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #27
	bl Func_020048cc
	movs r1, #0
	movs r0, #27
	bl UiText_OpenMessageAtObject
	movs r0, #28
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #4
	movs r0, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200a3ea
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a412
.L_0200a3ea:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #27
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
.L_0200a412:
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020048cc
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #27
	movs r2, #0
	movs r0, #26
	bl Object_TurnToFaceEachOther
	movs r0, #26
	bl Object_RefreshSelectorById
	movs r0, #27
	bl Object_RefreshSelectorById
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #26
	bl Func_020048cc
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r0, #28
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #28
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #28
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #27
	movs r1, #28
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r2, #5
	movs r0, #28
	bl Func_0200489c
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r1, #0
	movs r0, #27
	bl Func_0200489c
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #7
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #5
	movs r2, #0
	bl Object_TurnToFaceEachOther
	movs r1, #28
	movs r2, #0
	movs r0, #4
	bl Object_TurnToFaceEachOther
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #28
	bl Func_020048cc
	movs r0, #4
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #28
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #26
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200a6b6
	movs r1, #4
	movs r0, #26
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r2, #5
	movs r1, #0
	bl Func_0200489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a6e0
.L_0200a6b6:
	movs r1, #4
	movs r0, #26
	bl Object_SetModeById
	movs r0, #33
	bl FieldEvent_WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
.L_0200a6e0:
	movs r1, #4
	movs r2, #0
	movs r0, #28
	bl Object_TurnToFaceEachOther
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #28
	movs r2, #0
	movs r0, #4
	bl Object_TurnToFaceEachOther
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #27
	movs r2, #0
	movs r0, #26
	bl Object_TurnToFaceEachOther
	movs r0, #26
	bl Object_RefreshSelectorById
	movs r0, #27
	bl Object_RefreshSelectorById
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #27
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #4
	movs r1, #27
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #222
	lsls r1, r1, #1
	movs r0, #27
	adds r1, #255
	movs r2, #202
	bl ObjectMotion_SetPositionAndReset
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #4
	movs r1, #7
	movs r2, #0
	bl Object_TurnToFaceEachOther
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Object_TurnToFaceEachOther
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #5
	bl Object_RefreshSelectorById
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #26
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #8
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r2, #4
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #27
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r1, #0
	movs r0, #0
	bl Func_0200489c
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #4
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #26
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #27
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #4
	movs r1, #5
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #2
	negs r2, r2
	movs r1, #56
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #0
	bl ObjectMotion_SetVariantCallback
	movs r0, #4
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #223
	bl PartyInventory_Remove
	movs r0, #245
	bl PartyInventory_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r0, [r3]
	movs r1, #5
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl UiText_ShowPositionedMessageAndWait
	movs r5, #0
	movs r6, #216
	b .L_0200a9f4
.L_0200a9f0:
	adds r6, #2
	adds r5, #1
.L_0200a9f4:
	cmp r5, #14
	bgt .L_0200aa18
	movs r0, #4
	bl Unit_Get
	ldr r3, .L_0200aa14
	ldrh r2, [r0, r6]
	ands r3, r2
	cmp r3, #245
	bne .L_0200a9f0
	movs r0, #4
	adds r1, r5, #0
	bl Func_020047a4
	b .L_0200aa18
	.2byte 0x0000
.L_0200aa14:
	.4byte 0x000001ff
.L_0200aa18:
	movs r1, #3
	movs r0, #27
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #27
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #4
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r6, .L_0200ac4c
	movs r0, #26
	adds r1, r6, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #16
	movs r2, #0
	negs r1, r1
	movs r0, #7
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #10
	bl FieldEvent_WaitFrames
	adds r1, r6, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #110
	bl FieldEvent_WaitFrames
	adds r1, r6, #0
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #26
	bl Object_RefreshSelectorById
	ldr r5, .L_0200ac50
	movs r0, #26
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #27
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Object_SetModeById
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200489c
	movs r0, #6
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020048cc
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020048cc
	movs r2, #5
	movs r0, #6
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #7
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	adds r1, r6, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #7
	bl Object_RefreshSelectorById
	movs r0, #7
	bl Party_AddActiveOwner
	movs r0, #4
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02004844
	movs r0, #20
	bl FieldEvent_WaitFrames
	bl Event_ClearInvalidPackedValues
	movs r1, #166
	movs r0, #4
	lsls r1, r1, #2
	movs r2, #70
	bl ObjectMotion_SetPositionAndReset
	bl Func_020047dc
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200ac4c:
	.4byte gData_020059a4
.L_0200ac50:
	.4byte gData_02005a00
	.endif
	.section .text.x0200aefa,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200af08,"ax",%progbits
	.global Func_02002f08
	.thumb_func
Func_02002f08:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200afb4
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_0200af1a
	adds r0, #3
.L_0200af1a:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_0200afb8
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200af6e
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0200af4e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200afaa
.L_0200af4e:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200afaa
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200afaa
.L_0200af6e:
	movs r5, #0
	movs r6, #4
.L_0200af72:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_0200afbc
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_0200af72
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_0200afc0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_0200afb4
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200afaa:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200afb4:
	.4byte gData_02005a14
.L_0200afb8:
	.4byte gData_02005a18
.L_0200afbc:
	.4byte gData_02005a40
.L_0200afc0:
	.4byte 0x05000184
	.section .text.x0200b09c,"ax",%progbits
	.balign 4
	.global Func_0200309c
	.thumb_func
Func_0200309c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200b1a8
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_020047d4
	movs r0, #0
	bl Func_0200494c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200b13c
.L_0200b0f6:
	ldr r3, [r7, #8]
	ldr r2, .L_0200b1ac
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_0200b112
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_0200b112:
	ldr r3, .L_0200b1b0
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_0200b0f6
.L_0200b13c:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_0200b1a4
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_0200b1b4
.L_0200b1a4:
	.4byte 0x00000001
.L_0200b1a8:
	.4byte gAdventure
.L_0200b1ac:
	.4byte 0x0003ffff
.L_0200b1b0:
	.4byte gFrameCount
.L_0200b1b4:
	bl Camera_MoveWithinMapBounds
	bl Func_020048fc
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_0200b1fc
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02004724
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_0200b21a
	b .L_0200b200
	.2byte 0x0000
.L_0200b1fc:
	.4byte 0x00000000
.L_0200b200:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200b21a
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_0200b200
.L_0200b21a:
	movs r0, #127
	bl Func_020049c4
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_0200b23a
.L_0200b228:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200b23a
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_0200b228
.L_0200b23a:
	adds r0, r7, #0
	bl Func_0200472c
	ldr r5, .L_0200b284
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Camera_FollowActor
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_020047dc
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200b284:
	.4byte gAdventure
	.section .text.x0200b2b4,"ax",%progbits
	.global Func_020032b4
	.thumb_func
Func_020032b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200b31c
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_0200b312
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200b312
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200b312
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200b320
.L_0200b312:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetAnimationSpeed
	b .L_0200b45e
.L_0200b31c:
	.4byte gAdventure
.L_0200b320:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_SetAnimationSpeed
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_0200b340
	movs r0, #231
	bl Func_020049c4
.L_0200b340:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_020049a4
	cmp r0, #255
	beq .L_0200b442
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02004954
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_0200b442
	ldr r2, .L_0200b430
	cmp r5, r2
	blt .L_0200b442
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0200b408
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_0200b3a0
	subs r5, r3, r2
.L_0200b3a0:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02003288
	cmp r0, #12
	bgt .L_0200b3c0
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_0200b3c0
	movs r2, #1
	mov r8, r2
.L_0200b3c0:
	mov r3, r8
	cmp r3, #0
	beq .L_0200b408
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b408
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_0200b434
	movs r2, #128
	ldr r0, .L_0200b42c
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_0200b408:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_0200b438
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_0200b43c
.L_0200b42c:
	.4byte 0x00000000
.L_0200b430:
	.4byte 0xffe00000
.L_0200b434:
	.4byte gAdventure
.L_0200b438:
	.4byte IwramMulQ16
.L_0200b43c:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0200b45e
.L_0200b442:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_0200b46c
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02004704
	movs r0, #228
	bl Func_020049c4
	ldr r3, .L_0200b470
	str r5, [r3]
.L_0200b45e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b46c:
	.4byte gData_02005a1c
.L_0200b470:
	.4byte gData_02005a3c
	.section .text.x0200b474,"ax",%progbits
	.global Func_02003474
	.thumb_func
Func_02003474:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_020049c4
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_0200b528
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_0200470c
	movs r1, #2
	adds r7, r0, #0
	bl Func_020046f4
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_0200b524
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_0200b52c
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_0200b530
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_0200b534
.L_0200b524:
	.4byte 0x00000000
.L_0200b528:
	.4byte IwramMulQ16
.L_0200b52c:
	.4byte Func_020032b4
.L_0200b530:
	.4byte 0xfffa0000
.L_0200b534:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02003b7c
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b5ec,"ax",%progbits
	.global Func_020035ec
	.thumb_func
Func_020035ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004844
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004844
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004844
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02004844
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6c8
	ldr r3, [r6, #12]
	ldr r2, .L_0200b760
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_0200b764
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200b768
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_0200b6c8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b712
	ldr r3, [r7, #12]
	ldr r2, .L_0200b760
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200b764
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200b76c
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_0200b770
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_0200b712:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b752
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b752
	ldr r3, [r6, #12]
	ldr r2, .L_0200b774
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200b778
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_0200b752:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b760:
	.4byte 0x00066640
.L_0200b764:
	.4byte 0x0001eb80
.L_0200b768:
	.4byte 0xfffd70c0
.L_0200b76c:
	.4byte 0x00028f40
.L_0200b770:
	.4byte 0xfffff800
.L_0200b774:
	.4byte 0x00199900
.L_0200b778:
	.4byte 0x001b8480
	.section .text.x0200b77c,"ax",%progbits
	.global Func_0200377c
	.thumb_func
Func_0200377c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_0200b910
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_0200b914
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_0200b918
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_0200b91c
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_0200b920
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_0200b7ce
	b .L_0200b902
.L_0200b7ce:
	ldr r2, .L_0200b924
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_0200b7dc
	b .L_0200b8f2
.L_0200b7dc:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_0200b7e4
	b .L_0200b8f2
.L_0200b7e4:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_0200b928
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200b85a
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_0200b8f2
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_0200b8f2
	cmp r4, #239
	bgt .L_0200b8f2
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200b92c
	b .L_0200b896
.L_0200b85a:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_0200b8f2
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_0200b8f2
	cmp r4, #175
	bgt .L_0200b8f2
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200b930
.L_0200b896:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_0200b934
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_0200b8d4
	adds r0, r5, #0
	bl Func_0200499c
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_0200b8e8
.L_0200b8d4:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_0200b8e8:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020046ac
	adds r6, #12
.L_0200b8f2:
	ldr r3, .L_0200b920
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_0200b902
	b .L_0200b7ce
.L_0200b902:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b910:
	.4byte 0xffff0000
.L_0200b914:
	.4byte gData_02005a80
.L_0200b918:
	.4byte gResourceTableEntries
.L_0200b91c:
	.4byte Data_02005ac4
.L_0200b920:
	.4byte gData_02005a82
.L_0200b924:
	.4byte gData_02005a84
.L_0200b928:
	.4byte gData_02005b84
.L_0200b92c:
	.4byte 0x40002000
.L_0200b930:
	.4byte 0xc000a000
.L_0200b934:
	.4byte gData_02005b86
	.section .text.x0200bb42,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200bb7a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200be70,"ax",%progbits
	.global Func_02003e70
	.thumb_func
Func_02003e70:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200c0a0
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200c0a4
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200bff4
.L_0200bf24:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200bfe8
.L_0200bf38:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200bfd8
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200bfd8
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200bfd8
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bf8c
	cmp r5, r10
	bne .L_0200bfca
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200bfca
.L_0200bf8c:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bfca
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200473c
.L_0200bfca:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200bfd8:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200bf38
.L_0200bfe8:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200bf24
.L_0200bff4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c04c
	ldr r3, .L_0200c0a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200c04c
.L_0200c026:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200c03c
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200c03c
	mov r0, r8
	strh r2, [r0, #12]
.L_0200c03c:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200c026
.L_0200c04c:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200c05a:
	ldr r3, .L_0200c0ac
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200c05a
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramCache_Load
	adds r0, r5, #0
	bl MemFree
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c0b0
	bl Task_Add
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c0a0:
	.4byte Data_02005b88
.L_0200c0a4:
	.4byte IwramClearWords
.L_0200c0a8:
	.4byte gAdventure
.L_0200c0ac:
	.4byte 0x11111111
.L_0200c0b0:
	.4byte Func_02003d44
	.section .text.x0200c1ac,"ax",%progbits
	.global Func_020041ac
	.thumb_func
Func_020041ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200c25c
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020040b4
	movs r0, #229
	bl Func_020049c4
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200473c
	movs r0, #12
	bl FieldEvent_WaitFrames
	movs r3, #128
	ldr r2, .L_0200c254
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200c258
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200c260
	.2byte 0x0000
.L_0200c254:
	.4byte 0x00000000
.L_0200c258:
	.4byte 0x00008000
.L_0200c25c:
	.4byte gAdventure
.L_0200c260:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl FieldEvent_WaitFrames
.L_0200c274:
	cmp r7, #5
	bne .L_0200c27e
	movs r0, #204
	bl Func_020049c4
.L_0200c27e:
	ldr r3, [r6, #24]
	ldr r1, .L_0200c2dc
	ldr r2, .L_0200c2e0
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200c2e4
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200c274
	ldr r3, .L_0200c2e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c2dc:
	.4byte 0xfffffc00
.L_0200c2e0:
	.4byte 0xfffffd00
.L_0200c2e4:
	.4byte 0xffff6667
.L_0200c2e8:
	.4byte gAdventure
	.section .text.x0200c342,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200c344,"ax",%progbits
	.global Func_02004344
	.thumb_func
Func_02004344:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c4c8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020047d4
	movs r0, #0
	bl Func_0200494c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_0200471c
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_020049c4
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_WaitFrames
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200c4cc
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200c3de:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200c4d0
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200c4d4
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200c4d8
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02003b7c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200c3de
	movs r0, #188
	bl Func_020049c4
	ldr r5, .L_0200c4c8
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020048d4
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_0200475c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_0200475c
	bl Func_02004764
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020048d4
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_020047dc
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c4c8:
	.4byte gAdventure
.L_0200c4cc:
	.4byte Effect_AdvanceMotion
.L_0200c4d0:
	.4byte 0xffffa000
.L_0200c4d4:
	.4byte 0xffffd000
.L_0200c4d8:
	.4byte 0x01090001
	.section .text.x0200c4dc,"ax",%progbits
	.global Func_020044dc
	.thumb_func
Func_020044dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c584
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200c588
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200c578
.L_0200c510:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200c56c
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200c56c
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c540
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0200413c
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200c578
.L_0200c540:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200c578
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020041ac
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200c57a
.L_0200c56c:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200c510
.L_0200c578:
	movs r0, #0
.L_0200c57a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c584:
	.4byte gAdventure
.L_0200c588:
	.4byte Data_02005b88
	.section .bss,"aw",%nobits
	.global gData_02005a3c
gData_02005a3c:
	.space 0x00000004
	.global gData_02005a40
gData_02005a40:
	.space 0x00000020
	.global gData_02005a60
gData_02005a60:
	.space 0x00000004
	.global gData_02005a64
gData_02005a64:
	.space 0x0000001c
	.global gData_02005a80
gData_02005a80:
	.space 0x00000002
	.global gData_02005a82
gData_02005a82:
	.space 0x00000002
	.global gData_02005a84
gData_02005a84:
	.space 0x00000040
	.global Data_02005ac4
Data_02005ac4:
	.space 0x000000c0
	.global gData_02005b84
gData_02005b84:
	.space 0x00000002
	.global gData_02005b86
gData_02005b86:
	.space 0x00000002
	.global Data_02005b88
Data_02005b88:
