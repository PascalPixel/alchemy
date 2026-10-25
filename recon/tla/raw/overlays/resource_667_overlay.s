.syntax unified
	.thumb
	.section .text.x02008514,"ax",%progbits
	.balign 4
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_02008640
	ldr r5, .L_02008644
	str r3, [r0, #108]
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r5, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #80
	bne .L_0200857c
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r1, #160
	movs r3, #14
	strh r3, [r2]
	lsls r1, r1, #7
	strh r3, [r6]
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	ldr r0, .L_02008648
	bl Func_02001538
	movs r0, #14
	movs r1, #0
	bl Func_02001550
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200857c:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #81
	bne .L_02008594
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #8
	strh r3, [r2]
	strh r3, [r6]
	bl Func_02000658
.L_02008594:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085c4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085c4
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001500
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02001500
.L_020085c4:
	ldr r3, .L_02008644
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200863a
	movs r0, #152
	movs r1, #1
	movs r2, #160
	negs r1, r1
	lsls r0, r0, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02001578
	movs r0, #144
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	b .L_0200862a
.L_02008624:
	movs r0, #1
	bl WaitFrames
.L_0200862a:
	ldr r3, .L_0200864c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008624
	ldr r0, .L_02008650
	movs r1, #80
	bl Func_02001588
.L_0200863a:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008640:
	.4byte Func_02001488
.L_02008644:
	.4byte gPartyState
.L_02008648:
	.4byte MsgAlhafraManor10
.L_0200864c:
	.4byte gInput
.L_02008650:
	.4byte 0x00000061
