.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_0200054c
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	movs r0, #150
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008056
	ldr r0, .L_0200805c
	b .L_02008058
.L_02008056:
	ldr r0, .L_02008060
.L_02008058:
	pop {pc}
	.2byte 0x0000
.L_0200805c:
	.4byte Data_0200057c
.L_02008060:
	.4byte Data_0200059c
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	ldr r0, .L_02008068
	bx lr
.L_02008068:
	.4byte Data_020005bc
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, lr}
	ldr r5, .L_02008150
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200814c
	bl Func_0200046c
	movs r0, #0
	bl Func_02000524
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	bne .L_020080de
	movs r2, #16
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #16
	movs r3, #192
	lsls r3, r3, #6
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl Func_0200052c
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02008154
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #236
	movs r2, #196
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_020004f4
	b .L_02008148
.L_020080de:
	movs r0, #4
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #220
	movs r2, #220
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #8
	ldr r1, .L_02008154
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008138
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02008138:
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020004bc
.L_02008148:
	bl Func_02000474
.L_0200814c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008150:
	.4byte gPartyState
.L_02008154:
	.4byte 0x00013333
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {r5, lr}
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #108]
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200818c
	ldr r3, .L_0200824c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #30
	bl Func_02000504
.L_0200818c:
	ldr r0, .L_02008250
	bl Func_020004dc
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	bl Func_0200053c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200822c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #8
	bl Func_020004ec
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #8
	bl Func_020004f4
	movs r0, #5
	bl Battle_WaitMode0
	ldr r5, .L_0200824c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #196
	lsls r2, r2, #17
	ldr r0, [r5]
	cmp r3, r2
	bge .L_020081fc
	movs r1, #236
	movs r2, #188
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	b .L_02008208
.L_020081fc:
	movs r1, #236
	movs r2, #204
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
.L_02008208:
	movs r0, #4
	movs r1, #0
	bl Func_020004f4
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_0200824c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r2, #1
	strh r2, [r3]
	movs r0, #1
	bl Func_0200050c
	b .L_02008248
.L_0200822c:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020004ec
.L_02008248:
	pop {r5, pc}
	.2byte 0x0000
.L_0200824c:
	.4byte gPartyState
.L_02008250:
	.4byte 0x0000289a
	.section .text.x02008254,"ax",%progbits
	.global Func_02000254
	.thumb_func
Func_02000254:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	bl Func_0200046c
	movs r0, #0
	bl Func_02000524
	movs r0, #158
	bl Func_02000544
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	ldr r0, [r5]
	bl Func_02000454
	ldr r5, .L_020082d8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	cmp r6, #0
	bne .L_020082b2
	movs r2, #8
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
	b .L_020082bc
.L_020082b2:
	movs r2, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
.L_020082bc:
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_0200050c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000474
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082d8:
	.4byte gPartyState
	.section .text.x020082dc,"ax",%progbits
	.global Func_020002dc
	.thumb_func
Func_020002dc:
	push {lr}
	adds r1, r0, #0
	movs r2, #0
	ldr r0, .L_020082ec
	bl Func_02000254
	pop {pc}
	.2byte 0x0000
.L_020082ec:
	.4byte Data_02000610
	.section .text.x020082f0,"ax",%progbits
	.global Func_020002f0
	.thumb_func
Func_020002f0:
	ldr r0, .L_020082f4
	bx lr
.L_020082f4:
	.4byte Data_02000618
	.section .text.x020082f8,"ax",%progbits
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {lr}
	ldr r3, .L_0200834c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200834a
	ldr r3, .L_02008350
	movs r1, #9
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008310:
	ldr r4, .L_02008354
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #4
	bne .L_02008310
	ldr r3, .L_02008358
	movs r1, #15
	strh r0, [r3]
	adds r3, #22
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008332:
	ldr r4, .L_02008354
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #12
	bne .L_02008332
	ldr r3, .L_0200835c
	strh r0, [r3]
.L_0200834a:
	pop {pc}
.L_0200834c:
	.4byte Data_0300122c
.L_02008350:
	.4byte 0x05000172
.L_02008354:
	.4byte 0x05000160
.L_02008358:
	.4byte 0x05000168
.L_0200835c:
	.4byte 0x05000178
	.section .text.x02008360,"ax",%progbits
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r3, .L_020083c4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #3
	ldr r0, .L_020083c8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083b4
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	b .L_020083be
.L_020083b4:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020004bc
.L_020083be:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020083c4:
	.4byte gPartyState
.L_020083c8:
	.4byte Func_020002f8
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, r6, lr}
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008436
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #57
	movs r1, #52
	movs r2, #64
	movs r3, #15
	bl Func_0200045c
	movs r5, #3
	movs r6, #6
	movs r0, #60
	movs r1, #53
	movs r2, #67
	movs r3, #16
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200045c
	movs r0, #50
	movs r1, #52
	movs r2, #16
	movs r3, #16
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200045c
	movs r6, #10
	movs r0, #50
	movs r1, #61
	movs r2, #16
	movs r3, #28
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200045c
	movs r0, #61
	movs r1, #61
	movs r2, #16
	movs r3, #63
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200045c
.L_02008436:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.section .rodata.x0200854c,"a",%progbits
	.global Data_0200054c
Data_0200054c:
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x40000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200057c
Data_0200057c:
	.4byte 0x000000c8
	.4byte 0x101030c7
	.4byte 0xffffffff
	.4byte 0x102020c9
	.4byte 0xffffffff
	.4byte 0x10301028
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_0200059c
Data_0200059c:
	.4byte 0x000000c8
	.4byte 0x101050c7
	.4byte 0xffffffff
	.4byte 0x102050c9
	.4byte 0xffffffff
	.4byte 0x10301028
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020005bc
Data_020005bc:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff01a4
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_02008604:
	.4byte 0x00260033
	.4byte 0x00010001
	.4byte 0xffff0006
	.global Data_02000610
Data_02000610:
	.4byte .L_02008604
	.4byte 0x00160046
	.global Data_02000618
Data_02000618:
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020002dc
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_0200006c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000158
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte Func_02000158
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
