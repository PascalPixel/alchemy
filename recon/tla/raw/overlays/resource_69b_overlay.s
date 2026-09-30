.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	ldr r3, .L_020080b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #34
	ldrb r3, [r0]
	cmp r3, #1
	bne .L_0200808e
	movs r0, #144
	movs r1, #144
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #234
	lsls r0, r0, #16
	bl Func_02002304
	movs r0, #128
	movs r1, #140
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #234
	bl Func_02002304
	b .L_020080ae
.L_0200808e:
	movs r0, #144
	movs r1, #144
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #235
	lsls r0, r0, #16
	bl Func_02002304
	movs r0, #128
	movs r1, #140
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #235
	bl Func_02002304
.L_020080ae:
	pop {pc}
.L_020080b0:
	.4byte gPartyState
	.section .text.x020080b4,"ax",%progbits
	.global Func_020000b4
	.thumb_func
Func_020000b4:
	push {lr}
	ldr r3, .L_020080c8
	ldr r0, .L_020080cc
	ldr r1, [r3]
	ldr r2, [r3, #4]
	ldr r3, [r3, #8]
	bl Func_0200231c
	pop {pc}
	.2byte 0x0000
.L_020080c8:
	.4byte gOverlayArea + 0x2b88
.L_020080cc:
	.4byte gOverlayArea + 0x2b60
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200810c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	adds r5, r0, #0
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
	ldr r2, .L_02008110
	cmp r5, #32
	bne .L_02008114
	movs r3, #136
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #148
	b .L_02008122
.L_0200810c:
	.4byte gPartyState
.L_02008110:
	.4byte gOverlayArea + 0x2b88
.L_02008114:
	movs r3, #248
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #144
.L_02008122:
	lsls r3, r3, #18
	str r3, [r2, #8]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200821c
	bl Scheduler_AddOrUpdateCallback
	mov r0, r8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r5, #35
.L_0200813a:
	ldr r3, [r6, #12]
	ldr r2, .L_02008220
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200813a
	movs r5, #128
	lsls r5, r5, #9
	movs r0, #140
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	str r5, [r6, #24]
	str r5, [r6, #28]
	lsls r0, r0, #1
	bl Object_Spawn
	movs r1, #2
	adds r7, r0, #0
	bl Func_020021b4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #24]
	str r5, [r7, #28]
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_020081be
.L_0200818e:
	ldr r2, .L_02008220
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	ldr r3, [r6, #8]
	ldr r2, .L_02008224
	str r3, [r7, #8]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r6, #16]
	str r3, [r7, #16]
	bl WaitFrames
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_0200818e
.L_020081be:
	adds r0, r7, #0
	movs r1, #6
	bl Func_020021b4
	movs r5, #14
.L_020081c8:
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_020081c8
	adds r0, r7, #0
	bl Func_020021d4
	mov r0, r8
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	ldr r2, .L_02008218
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #34
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	ldr r0, .L_0200821c
	bl Scheduler_RemoveCallbackFar
	bl Func_02000054
	bl Func_02002254
	b .L_02008228
	.2byte 0x0000
.L_02008218:
	.4byte 0x00000000
.L_0200821c:
	.4byte Func_020000b4
.L_02008220:
	.4byte 0xffff0000
.L_02008224:
	.4byte 0xfffa0000
.L_02008228:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008230,"ax",%progbits
	.global Func_02000230
	.thumb_func
Func_02000230:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020083dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	adds r5, r0, #0
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r7, r0, #0
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
	ldr r2, .L_020083e0
	cmp r5, #30
	bne .L_0200826c
	movs r3, #136
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #148
	b .L_0200827a
.L_0200826c:
	movs r3, #248
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #144
.L_0200827a:
	lsls r3, r3, #18
	str r3, [r2, #8]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020083e4
	bl Scheduler_AddOrUpdateCallback
	movs r5, #128
	mov r0, r8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	lsls r5, r5, #9
	movs r0, #140
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	str r5, [r7, #24]
	str r5, [r7, #28]
	lsls r0, r0, #1
	bl Object_Spawn
	movs r1, #2
	adds r6, r0, #0
	bl Func_020021b4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r5, [r6, #28]
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [r7, #12]
	movs r2, #216
	lsls r2, r2, #15
	cmp r3, r2
	bge .L_02008300
.L_020082d0:
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	ldr r3, [r7, #8]
	ldr r2, .L_020083e8
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	bl WaitFrames
	ldr r3, [r7, #12]
	ldr r2, .L_020083ec
	cmp r3, r2
	ble .L_020082d0
.L_02008300:
	movs r3, #0
	str r3, [r7, #24]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	movs r2, #144
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_0200832c
.L_02008310:
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	ldr r3, [r7, #12]
	ldr r2, .L_020083f0
	cmp r3, r2
	ble .L_02008310
.L_0200832c:
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #24]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r6, #28]
	movs r5, #31
.L_0200833a:
	ldr r3, [r6, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #28]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200833a
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	adds r0, r6, #0
	movs r1, #6
	bl Func_020021b4
	movs r3, #192
	lsls r3, r3, #7
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r5, #19
.L_02008378:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	ldr r3, [r7, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008378
	adds r0, r6, #0
	bl Func_020021d4
	mov r0, r8
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #7
	adds r2, r7, #0
	strh r3, [r7, #6]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	adds r2, #51
	movs r3, #3
	strb r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r7, #12]
	str r3, [r7, #20]
	ldr r0, .L_020083e4
	bl Scheduler_RemoveCallbackFar
	bl Func_02000054
	bl Func_02002254
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020083dc:
	.4byte gPartyState
.L_020083e0:
	.4byte gOverlayArea + 0x2b88
.L_020083e4:
	.4byte Func_020000b4
.L_020083e8:
	.4byte 0xfffa0000
.L_020083ec:
	.4byte 0x006bffff
.L_020083f0:
	.4byte 0x008fffff
	.section .text.x020083f4,"ax",%progbits
	.global Func_020003f4
	.thumb_func
Func_020003f4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	adds r3, #224
	ldr r2, [r3]
	movs r3, #181
	lsls r3, r3, #1
	adds r1, r1, r3
	movs r3, #30
	strh r3, [r1]
	adds r2, #52
	movs r3, #1
	strb r3, [r2]
	bx lr
	.section .text.x02008410,"ax",%progbits
	.global Func_02000410
	.thumb_func
Func_02000410:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	adds r3, #224
	ldr r2, [r3]
	movs r3, #181
	lsls r3, r3, #1
	adds r1, r1, r3
	movs r3, #31
	strh r3, [r1]
	adds r2, #52
	movs r3, #1
	strb r3, [r2]
	bx lr
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #32
	strh r3, [r2]
	bx lr
	.2byte 0x0000
	.section .text.x02008440,"ax",%progbits
	.global Func_02000440
	.thumb_func
Func_02000440:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #33
	strh r3, [r2]
	bx lr
	.2byte 0x0000
	.section .text.x02008454,"ax",%progbits
	.global Func_02000454
	.thumb_func
Func_02000454:
	push {r5, r6, lr}
	ldr r3, .L_020084cc
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	blt .L_020084ca
	movs r1, #128
	movs r2, #128
	ldr r0, [r6]
	lsls r2, r2, #9
	lsls r1, r1, #10
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #170
	ldr r0, [r6]
	lsls r2, r2, #2
	movs r1, #216
	bl ObjectMotion_ResetAndSetPositionInMode2
	adds r0, r5, #0
	movs r1, #64
	bl ObjectDispatch_ApplyValueToChildren
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r6, #109
.L_020084a4:
	ldr r3, [r5, #12]
	movs r2, #192
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
	ldrh r3, [r5, #6]
	movs r2, #192
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #2
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_020084a4
	movs r0, #12
	bl Func_020022bc
.L_020084ca:
	pop {r5, r6, pc}
.L_020084cc:
	.4byte gPartyState
	.section .text.x020084d0,"ax",%progbits
	.global Func_020004d0
	.thumb_func
Func_020004d0:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_020084fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	pop {r5, pc}
	.2byte 0x0000
.L_020084fc:
	.4byte gPartyState
	.section .text.x02008500,"ax",%progbits
	.global Func_02000500
	.thumb_func
Func_02000500:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r0, r7, #0
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	adds r0, r7, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	ldr r3, .L_02008670
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_02008554
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #39
	bne .L_02008554
	adds r0, r7, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02008554:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #20]
	cmp r3, r0
	beq .L_02008596
	movs r0, #2
	bl WaitFrames
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	movs r5, #0
	b .L_02008586
.L_02008576:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #29
	bgt .L_02008590
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
.L_02008586:
	cmp r2, r3
	bgt .L_02008576
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008576
.L_02008590:
	movs r0, #188
	bl Func_0200232c
.L_02008596:
	ldr r3, [r6, #8]
	asrs r5, r3, #20
	cmp r5, #20
	bne .L_0200866a
	ldr r3, [r6, #16]
	asrs r6, r3, #20
	cmp r6, #39
	bne .L_0200866a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #2
	bl GameFlag_SetBit
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r7, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	bl Func_0200227c
	movs r0, #28
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200220c
	movs r0, #28
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002214
	movs r5, #84
	movs r1, #39
	movs r2, #1
	movs r3, #1
	movs r0, #92
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl Func_02002214
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #167
	bl Func_0200232c
	movs r3, #40
	str r3, [sp, #4]
	movs r1, #40
	movs r2, #1
	movs r3, #1
	movs r0, #92
	str r5, [sp, #0]
	bl Func_02002214
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #167
	bl Func_0200232c
	movs r6, #41
	movs r1, #41
	movs r2, #1
	movs r3, #1
	movs r0, #92
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002214
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #167
	bl Func_0200232c
	movs r3, #85
	str r3, [sp, #0]
	movs r0, #93
	movs r1, #41
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002214
	movs r0, #168
	movs r1, #164
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #236
	bl Func_02002304
.L_0200866a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008670:
	.4byte gPartyState
	.section .text.x02008674,"ax",%progbits
	.global Func_02000674
	.thumb_func
Func_02000674:
	push {lr}
	adds r0, r1, #0
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	pop {pc}
	.section .text.x02008680,"ax",%progbits
	.global Func_02000680
	.thumb_func
Func_02000680:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r0, r7, #0
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086d8
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #18
	bne .L_020086d8
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_020086d8
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r6, #15
.L_020086b2:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #1
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_020086b2
	adds r0, r7, #0
	bl Func_02000b28
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_SetBit
.L_020086d8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020086dc,"ax",%progbits
	.global Func_020006dc
	.thumb_func
Func_020006dc:
	push {lr}
	ldr r3, .L_02008700
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #10
	bne .L_020086fe
	movs r0, #168
	movs r1, #148
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #0
	movs r3, #255
	bl Func_02002304
.L_020086fe:
	pop {pc}
.L_02008700:
	.4byte gPartyState
	.section .text.x02008704,"ax",%progbits
	.global Func_02000704
	.thumb_func
Func_02000704:
	push {lr}
	movs r0, #168
	movs r1, #148
	lsls r1, r1, #18
	movs r2, #0
	movs r3, #4
	lsls r0, r0, #17
	bl Func_02002304
	pop {pc}
	.section .text.x02008720,"ax",%progbits
	.global Func_02000720
	.thumb_func
Func_02000720:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	movs r1, #100
	adds r1, r1, r6
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r7, r6, #0
	mov r10, r2
	adds r7, #102
	mov r0, r10
	mov r8, r1
	movs r1, #0
	ldrsh r5, [r7, r1]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r3, .L_020087cc
	mov lr, r3
	.2byte 0xf800
	lsls r3, r5, #1
	adds r3, r3, r5
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #12
	adds r3, r3, r1
	movs r2, #214
	lsls r2, r2, #16
	str r3, [r6, #12]
	movs r3, #170
	adds r0, r0, r2
	lsls r3, r3, #18
	str r0, [r6, #8]
	str r3, [r6, #16]
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	bne .L_02008778
	adds r0, r6, #0
	movs r1, #7
	bl Func_020021b4
.L_02008778:
	ldr r3, [r6, #12]
	movs r1, #170
	lsls r1, r1, #17
	cmp r3, r1
	ble .L_0200878a
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r7]
.L_0200878a:
	mov r0, r10
	bl Math_Sine
	ldr r1, [r6, #80]
	cmp r0, #0
	bge .L_020087a2
	ldrb r3, [r1, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #4
	b .L_020087ac
.L_020087a2:
	ldrb r3, [r1, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #8
.L_020087ac:
	orrs r2, r3
	strb r2, [r1, #9]
	mov r1, r8
	ldr r3, [r6, #52]
	ldrh r2, [r1]
	adds r2, r2, r3
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087cc:
	.4byte IwramMulQ16
	.section .text.x020087d0,"ax",%progbits
	.global Func_020007d0
	.thumb_func
Func_020007d0:
	push {r5, r6, r7, lr}
	movs r7, #0
	movs r6, #0
.L_020087d6:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Func_020021cc
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	bl Random16Far
	movs r3, #192
	lsls r3, r3, #11
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [r5, #48]
	bl Random16Far
	movs r3, #128
	lsls r0, r0, #11
	lsls r3, r3, #4
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, .L_02008850
	str r0, [r5, #52]
	str r3, [r5, #108]
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r3, #2
	strh r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r7, #1
	strb r3, [r1, #9]
	adds r6, #5
	cmp r7, #22
	bne .L_020087d6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008850:
	.4byte Func_02000720
	.section .text.x02008854,"ax",%progbits
	.global Func_02000854
	.thumb_func
Func_02000854:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #13
	movs r1, #3
	sub sp, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #252
	movs r3, #128
	lsls r2, r2, #6
	lsls r3, r3, #19
	adds r2, #6
	adds r3, #80
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, #2
	strh r2, [r3]
	movs r0, #0
	bl Func_02002314
	movs r3, #128
	ldr r0, .L_02008994
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #24
	bl Func_02002324
	bl Func_02000054
	bl Func_020007d0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200890c
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_0200227c
	movs r5, #39
	movs r6, #20
	movs r0, #28
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200220c
	movs r0, #28
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002214
	movs r3, #84
	str r3, [sp, #0]
	movs r0, #92
	movs r1, #39
	movs r2, #2
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02002214
	movs r0, #168
	movs r1, #164
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #236
	bl Func_02002304
	b .L_02008918
.L_0200890c:
	bl Func_02001498
	movs r0, #17
	movs r1, #0
	bl Func_020015a0
.L_02008918:
	ldr r2, .L_02008998
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	beq .L_02008930
	cmp r3, #6
	beq .L_02008930
	cmp r3, #7
	bne .L_02008946
.L_02008930:
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
.L_02008946:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200896a
	movs r0, #8
	bl Func_02000b28
	movs r0, #9
	bl Func_02000b28
	movs r0, #10
	bl Func_02000b28
	movs r0, #11
	bl Func_02000b28
.L_0200896a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200898c
	movs r1, #148
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_0200227c
	movs r0, #12
	bl Func_02000b28
.L_0200898c:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008994:
	.4byte gOverlayArea + 0x2b60
.L_02008998:
	.4byte gPartyState
	.section .text.x0200899c,"ax",%progbits
	.global Func_0200099c
	.thumb_func
Func_0200099c:
	push {lr}
	movs r0, #0
	bl Scene_SetArrivalFlags
	movs r0, #0
	bl Func_02000aa4
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020089da,"ax",%progbits
	.2byte 0x0000
	.section .text.x020089dc,"ax",%progbits
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	ldr r3, .L_020089e4
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_020089e4:
	.4byte Data_02002b38
	.section .text.x020089e8,"ax",%progbits
	.global Func_020009e8
	.thumb_func
Func_020009e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008a94
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_020089fa
	adds r0, #3
.L_020089fa:
	asrs r0, r0, #2
	movs r1, #5
	bl Engine_MathRemainder
	ldr r3, .L_02008a98
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008a4e
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
	beq .L_02008a2e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02008a8a
.L_02008a2e:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02008a8a
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008a8a
.L_02008a4e:
	movs r5, #0
	movs r6, #4
.L_02008a52:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl Engine_MathRemainder
	ldr r3, .L_02008a9c
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02008a52
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02008aa0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008a94
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008a8a:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a94:
	.4byte Data_02002b34
.L_02008a98:
	.4byte Data_02002b38
.L_02008a9c:
	.4byte gOverlayArea + 0x2b98
.L_02008aa0:
	.4byte 0x05000184
	.section .text.x02008aa4,"ax",%progbits
	.global Func_02000aa4
	.thumb_func
Func_02000aa4:
	push {r5, r6, lr}
	ldr r2, .L_02008b00
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_02008ac8
	ldr r1, .L_02008b04
	movs r2, #32
	ldr r0, .L_02008b08
	ldr r5, .L_02008b0c
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008b10
	ldr r1, .L_02008b14
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_02008ac8:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008ad8
	cmp r6, #1
	bne .L_02008aea
.L_02008ad8:
	ldr r3, .L_02008b18
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_02008b1c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_02008afe
.L_02008aea:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008b20
	ldr r1, .L_02008b24
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02008afe:
	pop {r5, r6, pc}
.L_02008b00:
	.4byte Data_02002b38
.L_02008b04:
	.4byte 0x05000180
.L_02008b08:
	.4byte gOverlayArea + 0x2b98
.L_02008b0c:
	.4byte IwramCopyWords
.L_02008b10:
	.4byte gOverlayArea + 0x2bb8
.L_02008b14:
	.4byte 0x050001a0
.L_02008b18:
	.4byte Data_02002b34
.L_02008b1c:
	.4byte Func_020009e8
.L_02008b20:
	.4byte gOverlayArea + 0x2bbc
.L_02008b24:
	.4byte 0x05000184
	.section .text.x02008b28,"ax",%progbits
	.global Func_02000b28
	.thumb_func
Func_02000b28:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #85
	movs r3, #4
	strb r3, [r1]
	movs r2, #0
	ldr r3, [r5, #20]
	str r2, [r5, #68]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r1, #50
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r3, r0, #0
	asrs r3, r3, #19
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	adds r3, #6
	movs r2, #0
	bl Func_0200223c
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_0200230c
	pop {r5, pc}
	.section .text.x02008b7c,"ax",%progbits
	.global Func_02000b7c
	.thumb_func
Func_02000b7c:
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
	ldr r3, .L_02008c88
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
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
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02008c1c
.L_02008bd6:
	ldr r3, [r7, #8]
	ldr r2, .L_02008c8c
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02008bf2
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02008bf2:
	ldr r3, .L_02008c90
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
	bne .L_02008bd6
.L_02008c1c:
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
	ldr r0, .L_02008c84
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
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02008c94
.L_02008c84:
	.4byte 0x00000001
.L_02008c88:
	.4byte gPartyState
.L_02008c8c:
	.4byte 0x0003ffff
.L_02008c90:
	.4byte Data_0300122c
.L_02008c94:
	bl Motion_CamBounds
	bl Func_020022b4
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008cdc
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
	bl Func_020021ec
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_02008cfa
	b .L_02008ce0
	.2byte 0x0000
.L_02008cdc:
	.4byte 0x00000000
.L_02008ce0:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008cfa
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008ce0
.L_02008cfa:
	movs r0, #127
	bl Func_0200232c
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_02008d1a
.L_02008d08:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008d1a
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008d08
.L_02008d1a:
	adds r0, r7, #0
	bl Func_020021f4
	ldr r5, .L_02008d64
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
	bl Object_AttachWorkTargetToObject
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
	bl Func_02002254
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008d64:
	.4byte gPartyState
	.section .text.x02008d68,"ax",%progbits
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	push {lr}
	ldr r3, [r1]
	ldr r4, [r0]
	ldr r2, [r1, #8]
	subs r4, r4, r3
	ldr r3, [r0, #8]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_02008d90
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_02008d90:
	.4byte IwramFillWords + 0x74
	.section .text.x02008d94,"ax",%progbits
	.global Func_02000d94
	.thumb_func
Func_02000d94:
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
	ldr r3, .L_02008dfc
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
	bne .L_02008df2
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008df2
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008df2
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008e00
.L_02008df2:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02008f3e
.L_02008dfc:
	.4byte gPartyState
.L_02008e00:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_02008e20
	movs r0, #231
	bl Func_0200232c
.L_02008e20:
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
	bl Func_020022fc
	cmp r0, #255
	beq .L_02008f22
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_020022e4
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_02008f22
	ldr r2, .L_02008f10
	cmp r5, r2
	blt .L_02008f22
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02008ee8
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02008e80
	subs r5, r3, r2
.L_02008e80:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02000d68
	cmp r0, #12
	bgt .L_02008ea0
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_02008ea0
	movs r2, #1
	mov r8, r2
.L_02008ea0:
	mov r3, r8
	cmp r3, #0
	beq .L_02008ee8
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008ee8
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
	ldr r3, .L_02008f14
	movs r2, #128
	ldr r0, .L_02008f0c
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
.L_02008ee8:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02008f18
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
	b .L_02008f1c
.L_02008f0c:
	.4byte 0x00000000
.L_02008f10:
	.4byte 0xffe00000
.L_02008f14:
	.4byte gPartyState
.L_02008f18:
	.4byte IwramMulQ16
.L_02008f1c:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_02008f3e
.L_02008f22:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02008f4c
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_020021c4
	movs r0, #228
	bl Func_0200232c
	ldr r3, .L_02008f50
	str r5, [r3]
.L_02008f3e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f4c:
	.4byte Data_02002b3c
.L_02008f50:
	.4byte gOverlayArea + 0x2b94
	.section .text.x02008f54,"ax",%progbits
	.global Func_02000f54
	.thumb_func
Func_02000f54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_0200232c
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02009008
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
	bl Func_020021cc
	movs r1, #2
	adds r7, r0, #0
	bl Func_020021b4
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	ldr r2, .L_02009004
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
	ldr r3, .L_0200900c
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
	ldr r3, .L_02009010
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02009014
.L_02009004:
	.4byte 0x00000000
.L_02009008:
	.4byte IwramMulQ16
.L_0200900c:
	.4byte Func_02000d94
.L_02009010:
	.4byte 0xfffa0000
.L_02009014:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_0200165c
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200902c,"ax",%progbits
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020090c8
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_020090ba
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl Engine_MathDivide
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200165c
.L_020090ba:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020090c8:
	.4byte Data_0300122c
	.section .text.x020090cc,"ax",%progbits
	.global Func_020010cc
	.thumb_func
Func_020010cc:
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
	bl Func_0200227c
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_0200227c
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200227c
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_0200227c
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
	beq .L_020091a8
	ldr r3, [r6, #12]
	ldr r2, .L_02009240
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02009244
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009248
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
.L_020091a8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091f2
	ldr r3, [r7, #12]
	ldr r2, .L_02009240
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009244
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200924c
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02009250
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
.L_020091f2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009232
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009232
	ldr r3, [r6, #12]
	ldr r2, .L_02009254
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009258
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
.L_02009232:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009240:
	.4byte 0x00066640
.L_02009244:
	.4byte 0x0001eb80
.L_02009248:
	.4byte 0xfffd70c0
.L_0200924c:
	.4byte 0x00028f40
.L_02009250:
	.4byte 0xfffff800
.L_02009254:
	.4byte 0x00199900
.L_02009258:
	.4byte 0x001b8480
	.section .text.x0200925c,"ax",%progbits
	.global Func_0200125c
	.thumb_func
Func_0200125c:
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
	ldr r3, .L_020093f0
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_020093f4
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_020093f8
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_020093fc
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009400
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_020092ae
	b .L_020093e2
.L_020092ae:
	ldr r2, .L_02009404
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_020092bc
	b .L_020093d2
.L_020092bc:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_020092c4
	b .L_020093d2
.L_020092c4:
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
	ldr r3, .L_02009408
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200933a
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
	bhi .L_020093d2
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_020093d2
	cmp r4, #239
	bgt .L_020093d2
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
	ldr r3, .L_0200940c
	b .L_02009376
.L_0200933a:
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
	bhi .L_020093d2
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_020093d2
	cmp r4, #175
	bgt .L_020093d2
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
	ldr r3, .L_02009410
.L_02009376:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009414
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_020093b4
	adds r0, r5, #0
	bl Func_020022f4
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
	b .L_020093c8
.L_020093b4:
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
.L_020093c8:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0200218c
	adds r6, #12
.L_020093d2:
	ldr r3, .L_02009400
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_020093e2
	b .L_020092ae
.L_020093e2:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020093f0:
	.4byte 0xffff0000
.L_020093f4:
	.4byte gOverlayArea + 0x2bd8
.L_020093f8:
	.4byte ResourceTableEntries
.L_020093fc:
	.4byte gOverlayArea + 0x2c1c
.L_02009400:
	.4byte gOverlayArea + 0x2bda
.L_02009404:
	.4byte gOverlayArea + 0x2bdc
.L_02009408:
	.4byte gOverlayArea + 0x2cdc
.L_0200940c:
	.4byte 0x40002000
.L_02009410:
	.4byte 0xc000a000
.L_02009414:
	.4byte gOverlayArea + 0x2cde
	.section .text.x02009418,"ax",%progbits
	.global Func_02001418
	.thumb_func
Func_02001418:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02009478
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200947c
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009480
	bl Func_02002174
	ldr r5, .L_02009484
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009488
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200948c
	ldr r2, .L_02009470
	strh r2, [r3]
	ldr r3, .L_02009490
	strh r2, [r3]
	ldr r2, .L_02009494
	ldr r3, .L_02009474
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009470:
	.4byte 0x00000000
.L_02009474:
	.4byte 0xffffffff
.L_02009478:
	.4byte IwramClearWords
.L_0200947c:
	.4byte gOverlayArea + 0x2bdc
.L_02009480:
	.4byte Data_02002334
.L_02009484:
	.4byte gOverlayArea + 0x2bd8
.L_02009488:
	.4byte Func_0200125c
.L_0200948c:
	.4byte gOverlayArea + 0x2bda
.L_02009490:
	.4byte gOverlayArea + 0x2cdc
.L_02009494:
	.4byte gOverlayArea + 0x2cde
	.section .text.x02009498,"ax",%progbits
	.global Func_02001498
	.thumb_func
Func_02001498:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_020094f8
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_020094fc
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009500
	bl Func_02002174
	ldr r5, .L_02009504
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009508
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200950c
	ldr r2, .L_020094f0
	strh r2, [r3]
	ldr r3, .L_02009510
	strh r2, [r3]
	ldr r2, .L_02009514
	ldr r3, .L_020094f4
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020094f0:
	.4byte 0x00000000
.L_020094f4:
	.4byte 0xffffffff
.L_020094f8:
	.4byte IwramClearWords
.L_020094fc:
	.4byte gOverlayArea + 0x2bdc
.L_02009500:
	.4byte Data_02002496 + 0x1
.L_02009504:
	.4byte gOverlayArea + 0x2bd8
.L_02009508:
	.4byte Func_0200125c
.L_0200950c:
	.4byte gOverlayArea + 0x2bda
.L_02009510:
	.4byte gOverlayArea + 0x2cdc
.L_02009514:
	.4byte gOverlayArea + 0x2cde
	.section .text.x02009518,"ax",%progbits
	.global Func_02001518
	.thumb_func
Func_02001518:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200957c
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02009580
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009584
	bl Func_02002174
	ldr r5, .L_02009588
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200958c
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_02009590
	ldr r3, .L_02009570
	strh r3, [r2]
	ldr r2, .L_02009594
	ldr r3, .L_02009574
	strh r3, [r2]
	ldr r2, .L_02009598
	ldr r3, .L_02009578
	strh r3, [r2]
	b .L_0200959c
.L_02009570:
	.4byte 0x00000000
.L_02009574:
	.4byte 0x00000001
.L_02009578:
	.4byte 0xffffffff
.L_0200957c:
	.4byte IwramClearWords
.L_02009580:
	.4byte gOverlayArea + 0x2bdc
.L_02009584:
	.4byte Data_020026c6
.L_02009588:
	.4byte gOverlayArea + 0x2bd8
.L_0200958c:
	.4byte Func_0200125c
.L_02009590:
	.4byte gOverlayArea + 0x2bda
.L_02009594:
	.4byte gOverlayArea + 0x2cdc
.L_02009598:
	.4byte gOverlayArea + 0x2cde
.L_0200959c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020095a0,"ax",%progbits
	.global Func_020015a0
	.thumb_func
Func_020015a0:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_020095c6
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_020095c8
	ldr r0, .L_020095cc
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_020095c6:
	pop {r5, pc}
.L_020095c8:
	.4byte gOverlayArea + 0x2bda
.L_020095cc:
	.4byte gOverlayArea + 0x2bdc
	.section .text.x020095d0,"ax",%progbits
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	ldr r3, .L_020095d8
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_020095d8:
	.4byte gOverlayArea + 0x2cde
	.section .text.x02009622,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009624,"ax",%progbits
	.global Func_02001624
	.thumb_func
Func_02001624:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x0200965c,"ax",%progbits
	.global Func_0200165c
	.thumb_func
Func_0200165c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02009814
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020096a4
	cmp r7, #0
	beq .L_020096a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020096ac
.L_020096a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020096ac:
	mov r3, r10
	bl Func_020021cc
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020096ba
	b .L_02009806
.L_020096ba:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_020021b4
	ldr r2, .L_02009818
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_020021c4
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200981c
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02009820
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009806
	cmp r7, #0
	beq .L_02009806
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200973c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200973c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200975c
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200975c:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02009770
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02009770:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020097b6
	ldr r3, .L_02009818
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200979e
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020097b0
.L_0200979e:
	ldr r2, .L_02009820
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009820
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020097b0:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_020097b6:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020097d2
	adds r0, r6, #0
	movs r1, #1
	bl Func_020021b4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020021c4
.L_020097d2:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020097e4
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_020097e4:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020097f6
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020097f6:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009806
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02009806:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009814:
	.4byte gPartyState
.L_02009818:
	.4byte Data_02002b50
.L_0200981c:
	.4byte Func_02001624
.L_02009820:
	.4byte 0xffff0000
	.section .text.x02009824,"ax",%progbits
	.global Func_02001824
	.thumb_func
Func_02001824:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200993c
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_02009930
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_02009940
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_02009870
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_02009878
.L_02009870:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_02009878:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_02009944
	cmp r3, r2
	beq .L_02009930
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_02009930
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_02009948
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_02009930
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_02009930
	cmp r2, #239
	bgt .L_02009930
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200994c
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_0200218c
.L_02009930:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200993c:
	.4byte gOverlayArea + 0x2ce0
.L_02009940:
	.4byte gPartyState
.L_02009944:
	.4byte 0xffff0000
.L_02009948:
	.4byte ResourceTableEntries
.L_0200994c:
	.4byte 0x80008800
	.section .text.x02009950,"ax",%progbits
	.global Func_02001950
	.thumb_func
Func_02001950:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_02009b80
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
	ldr r3, .L_02009b84
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
	bge .L_02009ad4
.L_02009a04:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_02009ac8
.L_02009a18:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_02009ab8
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_02009ab8
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_02009ab8
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
	bne .L_02009a6c
	cmp r5, r10
	bne .L_02009aaa
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_02009aaa
.L_02009a6c:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009aaa
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
	bl Func_02002204
.L_02009aaa:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_02009ab8:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_02009a18
.L_02009ac8:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_02009a04
.L_02009ad4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b2c
	ldr r3, .L_02009b88
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
	bge .L_02009b2c
.L_02009b06:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_02009b1c
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_02009b1c
	mov r0, r8
	strh r2, [r0, #12]
.L_02009b1c:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_02009b06
.L_02009b2c:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_02009b3a:
	ldr r3, .L_02009b8c
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02009b3a
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009b90
	bl Scheduler_AddOrUpdateCallback
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
.L_02009b80:
	.4byte gOverlayArea + 0x2ce0
.L_02009b84:
	.4byte IwramClearWords
.L_02009b88:
	.4byte gPartyState
.L_02009b8c:
	.4byte 0x11111111
.L_02009b90:
	.4byte Func_02001824
	.section .text.x02009b94,"ax",%progbits
	.global Func_02001b94
	.thumb_func
Func_02001b94:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_02009c14
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_020022b4
	ldr r2, .L_02009c18
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009c14:
	.4byte gPartyState
.L_02009c18:
	.4byte 0xfff80000
	.section .text.x02009c1c,"ax",%progbits
	.global Func_02001c1c
	.thumb_func
Func_02001c1c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_02009c88
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02001b94
	movs r0, #161
	bl Func_0200232c
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002204
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009c88:
	.4byte gPartyState
	.section .text.x02009c8c,"ax",%progbits
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02009d3c
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
	bl Func_02001b94
	movs r0, #229
	bl Func_0200232c
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
	bl Func_02002204
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_02009d34
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	ldr r2, .L_02009d38
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_02009d40
	.2byte 0x0000
.L_02009d34:
	.4byte 0x00000000
.L_02009d38:
	.4byte 0x00008000
.L_02009d3c:
	.4byte gPartyState
.L_02009d40:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_02009d54:
	cmp r7, #5
	bne .L_02009d5e
	movs r0, #204
	bl Func_0200232c
.L_02009d5e:
	ldr r3, [r6, #24]
	ldr r1, .L_02009dbc
	ldr r2, .L_02009dc0
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_02009dc4
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_02009d54
	ldr r3, .L_02009dc8
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
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009dbc:
	.4byte 0xfffffc00
.L_02009dc0:
	.4byte 0xfffffd00
.L_02009dc4:
	.4byte 0xffff6667
.L_02009dc8:
	.4byte gPartyState
	.section .text.x02009dcc,"ax",%progbits
	.global Func_02001dcc
	.thumb_func
Func_02001dcc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02009dfc
	adds r3, #15
.L_02009dfc:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009e24,"ax",%progbits
	.global Func_02001e24
	.thumb_func
Func_02001e24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009fa8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_020021e4
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_0200232c
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009fac
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_02009ebe:
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
	ldr r3, .L_02009fb0
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009fb4
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
	ldr r4, .L_02009fb8
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200165c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_02009ebe
	movs r0, #188
	bl Func_0200232c
	ldr r5, .L_02009fa8
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_0200229c
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002224
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002224
	bl Func_0200222c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_0200229c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_02002254
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009fa8:
	.4byte gPartyState
.L_02009fac:
	.4byte Func_02001dcc
.L_02009fb0:
	.4byte 0xffffa000
.L_02009fb4:
	.4byte 0xffffd000
.L_02009fb8:
	.4byte 0x01090001
	.section .text.x02009fbc,"ax",%progbits
	.global Func_02001fbc
	.thumb_func
Func_02001fbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a064
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a068
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
	bge .L_0200a058
.L_02009ff0:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a04c
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a04c
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a020
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02001c1c
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a058
.L_0200a020:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a058
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02001c8c
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
	b .L_0200a05a
.L_0200a04c:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_02009ff0
.L_0200a058:
	movs r0, #0
.L_0200a05a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a064:
	.4byte gPartyState
.L_0200a068:
	.4byte gOverlayArea + 0x2ce0
	.section .text.x0200a06c,"ax",%progbits
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200a11c
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200a120
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200a0ba
	cmp r0, #0
	beq .L_0200a10e
.L_0200a0ba:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_020021e4
	bl Func_02001e24
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200a10e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a11c:
	.4byte gPartyState
.L_0200a120:
	.4byte gOverlayArea + 0x2ce0
	.section .rodata.x0200a334,"a",%progbits
	.global Data_02002334
Data_02002334:
	.4byte 0x06345d01
	.4byte Runtime_ReciprocalTable + 0x1259
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte Text_MessageContexts + 0x10b38
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte Data_02001024 + 0xbb
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte Data_0200752c + 0x2d4
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte Resource_DecodeHalfwordLzCode + 0x12
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte Text_MessageContexts + 0x15ce8
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.2byte 0x0002
	.global Data_02002496
Data_02002496:
	.2byte 0x0000
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte Tileset_Set112TilesA + 0x21e
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte Battle_PurpleCaveBackdrop + 0x3607
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.2byte 0x0000
	.global Data_020026c6
Data_020026c6:
	.2byte 0x0100
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
.L_0200a7a8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200a7e4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200a820:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
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
	.4byte 0x00000026
	.4byte 0x00000011
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
	.4byte 0x000000ff
	.4byte 0x001010fe
	.4byte 0x002030ff
	.4byte 0x003020ff
	.4byte 0x004050ff
	.4byte 0x005040ff
	.4byte 0x00606100
	.4byte 0x00707100
	.4byte 0x00808100
	.4byte 0x00909100
	.4byte 0x00a0a100
	.4byte 0x00b0b100
	.4byte 0x00c0b101
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01020000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x01022000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01022000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_02000454
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_020003f4
	.4byte 0x00000006
	.4byte 0xffff001e
	.4byte Func_02000230
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_02000410
	.4byte 0x00000006
	.4byte 0xffff001f
	.4byte Func_02000230
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte Func_0200042c
	.4byte 0x00000006
	.4byte 0xffff0020
	.4byte Func_020000d0
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte Func_02000440
	.4byte 0x00000006
	.4byte 0xffff0021
	.4byte Func_020000d0
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte Func_020004d0
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte Func_02000500
	.4byte 0x00000008
	.4byte 0xffff0023
	.4byte Func_020004d0
	.4byte 0x00000009
	.4byte 0xffff0023
	.4byte Func_02000500
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_02000674
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_02000680
	.4byte 0x00009985
	.4byte 0xffff0000
	.4byte Func_020006dc
	.4byte 0x20009985
	.4byte 0xffff0000
	.4byte Func_02000704
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002b34
Data_02002b34:
	.4byte 0xffffffff
	.global Data_02002b38
Data_02002b38:
	.4byte 0x00000001
	.global Data_02002b3c
Data_02002b3c:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_02002b50
Data_02002b50:
	.4byte .L_0200a7a8
	.4byte .L_0200a7e4
	.4byte .L_0200a820
