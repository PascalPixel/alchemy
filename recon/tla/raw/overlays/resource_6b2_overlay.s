.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	ldr r3, .L_0200805c
	movs r2, #1
	ldr r3, [r3]
	lsrs r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02008050
	movs r1, #15
	bl Object_SetPartAttribute
	b .L_02008056
.L_02008050:
	movs r1, #7
	bl Object_SetPartAttribute
.L_02008056:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_0200805c:
	.4byte gOverlayArea + 0x7e4
	.section .text.x02008060,"ax",%progbits
	.global Func_02000060
	.thumb_func
Func_02000060:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #98
	ldrb r3, [r5]
	adds r7, r3, #0
	cmp r7, #0
	beq .L_02008076
	adds r3, #255
	strb r3, [r5]
	b .L_020080ce
.L_02008076:
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #40
	strb r3, [r5]
	adds r1, r6, #0
	adds r1, #99
	ldrb r3, [r1]
	cmp r3, #1
	beq .L_020080ae
	cmp r3, #1
	bgt .L_0200809a
	cmp r3, #0
	beq .L_020080a4
	b .L_020080ce
.L_0200809a:
	cmp r3, #2
	beq .L_020080ba
	cmp r3, #3
	beq .L_020080c4
	b .L_020080ce
.L_020080a4:
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r6, #6]
	movs r3, #1
	b .L_020080cc
.L_020080ae:
	ldr r2, .L_020080c0
	movs r3, #176
	lsls r3, r3, #8
	strh r3, [r6, #6]
	strb r2, [r1]
	b .L_020080ce
.L_020080ba:
	movs r3, #3
	strh r7, [r6, #6]
	b .L_020080cc
.L_020080c0:
	.4byte 0x00000000
.L_020080c4:
	movs r3, #208
	lsls r3, r3, #8
	strh r3, [r6, #6]
	movs r3, #2
.L_020080cc:
	strb r3, [r1]
.L_020080ce:
	pop {r5, r6, r7, pc}
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, r6, r7, lr}
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_020081a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_020081a4
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02008194
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #162
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_02000550
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008194
	ldr r5, [r6, #80]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r2, #203
	lsls r2, r2, #18
	lsls r3, r3, #3
	adds r1, r6, #0
	adds r3, r3, r2
	adds r1, #85
	strb r7, [r1]
	movs r2, #174
	str r3, [r6, #8]
	movs r3, #160
	lsls r2, r2, #18
	lsls r3, r3, #16
	str r3, [r6, #12]
	str r2, [r6, #16]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r1]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldr r1, .L_020081a8
	adds r0, r6, #0
	strb r3, [r5, #9]
	bl Func_02000548
	adds r0, r6, #0
	movs r1, #1
	bl Func_02000540
.L_02008194:
	ldr r2, .L_020081ac
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081a0:
	.4byte gPartyState
.L_020081a4:
	.4byte Data_0300122c
.L_020081a8:
	.4byte Data_02000618
.L_020081ac:
	.4byte gOverlayArea + 0x7e4
	.section .text.x020081b0,"ax",%progbits
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_02000568
	movs r0, #0
	bl Func_020005f8
	ldr r3, .L_020084b0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020084b4
	movs r6, #0
	movs r1, #144
	str r6, [r3]
	lsls r1, r1, #3
	ldr r0, .L_020084b8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #192
	lsls r2, r2, #18
	movs r3, #218
	mov r8, r2
	lsls r3, r3, #1
	ldr r2, [r2, #108]
	mov r10, r3
	mov r1, r10
	movs r3, #40
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #14
	movs r0, #13
	bl Func_02000600
	ldr r0, .L_020084bc
	bl Func_02000598
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_020005c0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #8
	bl Func_020005c0
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #11
	bl Func_020005c0
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #12
	bl Func_020005c0
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #12
	movs r1, #0
	bl Func_020005b0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_020005a0
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_020005b0
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #11
	bl Func_020005a0
	movs r0, #11
	bl Object_GetById
	adds r7, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r2, r7, #0
	lsrs r3, r3, #16
	adds r2, #98
	ldr r5, .L_020084c0
	adds r3, #40
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #99
	strb r6, [r3]
	movs r0, #12
	str r5, [r7, #108]
	bl Object_GetById
	adds r7, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r2, r7, #0
	lsrs r3, r3, #16
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	movs r3, #2
	adds r2, #1
	strb r3, [r2]
	movs r0, #13
	str r5, [r7, #108]
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #1
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #9
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_020005c0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r1, #0
	movs r0, #6
	bl Func_020005a0
	bl Func_02000608
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #160
	lsls r3, r3, #19
	movs r0, #1
	strh r6, [r3]
	bl WaitFrames
	mov r2, r8
	ldr r3, [r2, #108]
	mov r1, r10
	movs r2, #16
	str r2, [r3, r1]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #60
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_020005c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020084b0:
	.4byte gPartyState
.L_020084b4:
	.4byte gOverlayArea + 0x7e4
.L_020084b8:
	.4byte Func_020000f4
.L_020084bc:
	.4byte 0x00002e2f
.L_020084c0:
	.4byte Func_02000060
	.section .text.x020084c4,"ax",%progbits
	.global Func_020004c4
	.thumb_func
Func_020004c4:
	push {lr}
	ldr r3, .L_02008518
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_0200850e
	bl Func_02000568
	movs r0, #0
	bl Func_020005f8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #201
	bl Func_02000610
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_020005d0
	movs r0, #20
	bl Func_020005d8
	movs r0, #20
	bl WaitFrames
	movs r0, #9
	bl Func_020005c8
	b .L_02008512
.L_0200850e:
	bl Func_020001b0
.L_02008512:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008518:
	.4byte gPartyState
	.section .rodata.x02008618,"a",%progbits
	.global Data_02000618
Data_02000618:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0xc0010000
	.4byte 0x00000026
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
	.4byte 0x00000135
	.4byte 0x00202135
	.4byte 0x00305129
	.4byte 0x00902135
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00025000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00023000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x038a0000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00028000
	.4byte 0xffff00c2
	.4byte 0x00000001
	.4byte 0x030e0000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03b20000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03b20000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x03b20000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
