.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	ldr r0, .L_02008044
	bl Func_02000de8
	pop {pc}
	.2byte 0x0000
.L_02008044:
	.4byte Data_02000e10
	.section .text.x02008048,"ax",%progbits
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	bl Func_02000df0
	movs r0, #132
	bl Func_02000e08
	cmp r6, #20
	bne .L_0200814c
	cmp r5, #9
	bne .L_0200814c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080a0
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #12
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #12
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
.L_020080a0:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008142
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r0, #22
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02000da0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #22
	bl Func_02000d88
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02000d88
	movs r0, #60
	bl Battle_WaitMode0
	ldr r0, .L_02008194
	bl Func_02000d60
	movs r1, #0
	movs r0, #22
	bl Func_02000d70
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02000d88
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	bl Func_02000d70
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02000d08
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_SetBit
.L_02008142:
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008190
.L_0200814c:
	cmp r6, #19
	bne .L_02008190
	cmp r5, #9
	bne .L_02008190
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008190
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
.L_02008190:
	add sp, #8
	pop {r5, r6, pc}
.L_02008194:
	.4byte 0x000022b0
	.section .text.x020081ac,"ax",%progbits
	.global Func_020001ac
	.thumb_func
Func_020001ac:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081c0
	ldr r0, .L_020081c4
	b .L_020081c2
.L_020081c0:
	ldr r0, .L_020081c8
.L_020081c2:
	pop {pc}
.L_020081c4:
	.4byte Data_02001644
.L_020081c8:
	.4byte Data_02001494
	.section .text.x020081cc,"ax",%progbits
	.global Func_020001cc
	.thumb_func
Func_020001cc:
	push {lr}
	sub sp, #12
	movs r3, #23
	movs r2, #30
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #53
	movs r1, #5
	movs r2, #8
	movs r3, #9
	bl Func_02000df8
	add sp, #12
	pop {pc}
	.section .text.x020081ec,"ax",%progbits
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {r5, r6, lr}
	ldr r3, .L_02008278
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #23
	movs r2, #30
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #5
	movs r0, #53
	movs r2, #8
	movs r3, #9
	bl Func_02000df8
	movs r0, #234
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008274
	movs r1, #228
	movs r2, #146
	movs r0, #64
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02000d40
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #28
	bne .L_02008274
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #36
	bne .L_02008274
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02000d90
	movs r1, #16
	ldr r0, [r6]
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02000d08
.L_02008274:
	add sp, #12
	pop {r5, r6, pc}
.L_02008278:
	.4byte gPartyState
	.section .text.x0200827c,"ax",%progbits
	.global Func_0200027c
	.thumb_func
Func_0200027c:
	push {r5, r6, lr}
	ldr r3, .L_02008308
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #23
	movs r2, #30
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #5
	movs r0, #53
	movs r2, #8
	movs r3, #9
	bl Func_02000df8
	movs r0, #250
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008302
	movs r1, #236
	movs r2, #138
	movs r0, #65
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02000d40
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #29
	bne .L_02008302
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #34
	bne .L_02008302
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02000d90
	movs r2, #16
	ldr r0, [r6]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02000d08
.L_02008302:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008308:
	.4byte gPartyState
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {lr}
	sub sp, #12
	movs r3, #7
	movs r2, #32
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #44
	movs r1, #5
	movs r2, #8
	movs r3, #5
	bl Func_02000df8
	add sp, #12
	pop {pc}
	.section .text.x0200832c,"ax",%progbits
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	push {lr}
	sub sp, #12
	movs r3, #16
	movs r2, #23
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #55
	movs r2, #4
	movs r3, #2
	str r1, [sp, #8]
	bl Func_02000df8
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x0200834c,"ax",%progbits
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {lr}
	sub sp, #12
	movs r3, #9
	movs r2, #18
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r2, #6
	movs r3, #2
	str r1, [sp, #8]
	bl Func_02000df8
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x0200836c,"ax",%progbits
	.global Func_0200036c
	.thumb_func
Func_0200036c:
	push {r5, r6, lr}
	ldr r3, .L_020083f4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #9
	movs r2, #18
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, r0, #0
	movs r2, #6
	movs r0, #42
	movs r3, #2
	str r1, [sp, #8]
	bl Func_02000df8
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #161
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083f0
	movs r1, #168
	movs r2, #164
	movs r0, #66
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02000d40
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #10
	bne .L_020083f0
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_020083f0
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02000d90
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02000d08
.L_020083f0:
	add sp, #12
	pop {r5, r6, pc}
.L_020083f4:
	.4byte gPartyState
	.section .text.x020083f8,"ax",%progbits
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {lr}
	sub sp, #12
	movs r3, #24
	movs r2, #16
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r2, #5
	movs r3, #4
	str r1, [sp, #8]
	bl Func_02000df8
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x02008418,"ax",%progbits
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {lr}
	sub sp, #12
	movs r3, #16
	movs r2, #9
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r2, #5
	movs r3, #3
	str r1, [sp, #8]
	bl Func_02000df8
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x02008438,"ax",%progbits
	.global Func_02000438
	.thumb_func
Func_02000438:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	ldr r5, .L_02008498
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #1
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #123
	bl Func_02000e08
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_02000db0
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000d08
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008498:
	.4byte gPartyState
	.section .text.x0200849c,"ax",%progbits
	.global Func_0200049c
	.thumb_func
Func_0200049c:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084b0
	ldr r0, .L_020084b4
	b .L_020084b2
.L_020084b0:
	ldr r0, .L_020084b8
.L_020084b2:
	pop {pc}
.L_020084b4:
	.4byte Data_02001b00
.L_020084b8:
	.4byte Data_020018b4
	.section .text.x020084bc,"ax",%progbits
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084e8
	ldr r0, .L_02008530
	bl Func_02000d60
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000d70
	b .L_02008528
.L_020084e8:
	ldr r6, .L_02008534
	adds r0, r6, #0
	bl Func_02000d60
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000e00
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008514
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #1
	bl Func_02000d60
	b .L_02008520
.L_02008514:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	bl Func_02000d60
.L_02008520:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000d70
.L_02008528:
	bl Func_02000d08
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008530:
	.4byte 0x000022b2
.L_02008534:
	.4byte 0x000022ad
	.section .text.x02008538,"ax",%progbits
	.global Func_02000538
	.thumb_func
Func_02000538:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008564
	ldr r0, .L_020085ac
	bl Func_02000d60
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000d70
	b .L_020085a4
.L_02008564:
	ldr r6, .L_020085b0
	adds r0, r6, #0
	bl Func_02000d60
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000e00
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008590
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #1
	bl Func_02000d60
	b .L_0200859c
.L_02008590:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	bl Func_02000d60
.L_0200859c:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000d70
.L_020085a4:
	bl Func_02000d08
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085ac:
	.4byte 0x00002328
.L_020085b0:
	.4byte 0x00002323
	.section .text.x020085b4,"ax",%progbits
	.global Func_020005b4
	.thumb_func
Func_020005b4:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	ldr r5, .L_02008608
	adds r0, r5, #0
	bl Func_02000d60
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000e00
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020085ee
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000d60
	b .L_020085fa
.L_020085ee:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000d60
.L_020085fa:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02000d70
	bl Func_02000d08
	pop {r5, r6, pc}
.L_02008608:
	.4byte 0x00002338
	.section .text.x0200860c,"ax",%progbits
	.global Func_0200060c
	.thumb_func
Func_0200060c:
	push {lr}
	ldr r3, .L_020086a0
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200869c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #124
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008628:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #96
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008628
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #98
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008652:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #128
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008652
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #130
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_0200867c:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #160
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_0200867c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #162
	strh r0, [r3]
.L_0200869c:
	pop {pc}
	.2byte 0x0000
.L_020086a0:
	.4byte gFrameCount
	.section .text.x020086a4,"ax",%progbits
	.global Func_020006a4
	.thumb_func
Func_020006a4:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_SetBit
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	ldr r0, .L_0200880c
	bl Func_02000d60
	ldr r5, .L_02008810
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #132
	lsls r2, r2, #1
	movs r1, #200
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r0, #32
	bl Func_02000d70
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	ldr r0, [r5]
	bl Func_02000d88
	movs r1, #200
	movs r2, #216
	movs r0, #32
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02000d40
	movs r2, #248
	movs r1, #200
	movs r0, #32
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r2, #0
	movs r1, #32
	ldr r0, [r5]
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #32
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r1, #6
	adds r1, #255
	movs r2, #60
	ldr r0, [r5]
	bl Func_02000d88
	movs r1, #131
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02000d88
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r1, #3
	movs r0, #32
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #32
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02008802
	movs r0, #90
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #32
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #216
	movs r1, #200
	movs r0, #32
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	movs r0, #32
	bl Func_02000da8
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #32
	bl Func_02000d88
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r0, #32
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	bl Object_GetById
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	b .L_02008806
.L_02008802:
	bl Func_02000814
.L_02008806:
	bl Func_02000d08
	pop {r5, pc}
.L_0200880c:
	.4byte 0x000023b1
.L_02008810:
	.4byte gPartyState
	.section .text.x02008814,"ax",%progbits
	.global Func_02000814
	.thumb_func
Func_02000814:
	push {r5, lr}
	ldr r0, .L_0200894c
	bl Func_02000d60
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02000d88
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	ldr r5, .L_02008950
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	ldr r1, [r5]
	movs r0, #32
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #32
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r1, #1
	movs r0, #32
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r1, #4
	movs r0, #32
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #32
	bl Func_02000d88
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r2, #0
	ldr r1, [r5]
	movs r0, #32
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
	movs r1, #3
	movs r0, #32
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	adds r2, #153
	adds r1, #51
	movs r0, #32
	bl ObjectMotion_SetSpeedParameters
	movs r0, #32
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #3
	movs r0, #32
	bl ObjectMotion_SetActionVariant
	movs r1, #200
	movs r2, #208
	movs r0, #32
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #32
	bl Func_02000d40
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	pop {r5, pc}
	.2byte 0x0000
.L_0200894c:
	.4byte 0x000023b8
.L_02008950:
	.4byte gPartyState
	.section .text.x02008954,"ax",%progbits
	.global Func_02000954
	.thumb_func
Func_02000954:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020089b8
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02000d88
	ldr r5, .L_020089c4
	adds r0, r5, #0
	bl Func_02000d60
	movs r1, #0
	movs r0, #32
	bl UiText_OpenMessageAtObject
	ldr r3, .L_020089c8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020089a4
	bl Func_02000814
	b .L_020089b2
.L_020089a4:
	subs r0, r5, #1
	bl Func_02000d60
	movs r0, #32
	movs r1, #0
	bl Func_02000d70
.L_020089b2:
	bl Func_02000d08
	b .L_020089c0
.L_020089b8:
	ldr r0, .L_020089cc
	movs r1, #0
	bl Func_02000d70
.L_020089c0:
	pop {r5, pc}
	.2byte 0x0000
.L_020089c4:
	.4byte 0x000023b7
.L_020089c8:
	.4byte gPartyState
.L_020089cc:
	.4byte 0x000023bf
	.section .text.x020089d0,"ax",%progbits
	.global Func_020009d0
	.thumb_func
Func_020009d0:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #200
	subs r2, #172
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_02008af8
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_02008afc
	bl Func_02000de0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008adc
	movs r0, #15
	bl Object_GetById
	movs r3, #2
	adds r0, #90
	strb r3, [r0]
	movs r1, #5
	movs r0, #25
	bl Object_SetModeById
	movs r0, #26
	movs r1, #5
	bl Object_SetModeById
	movs r0, #27
	movs r1, #5
	bl Object_SetModeById
	movs r0, #28
	movs r1, #5
	bl Object_SetModeById
	movs r0, #29
	movs r1, #6
	bl Object_SetModeById
	movs r0, #30
	movs r1, #6
	bl Object_SetModeById
	movs r0, #31
	movs r1, #5
	bl Object_SetModeById
	ldr r3, .L_02008b00
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	bne .L_02008a90
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a90
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a90
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a90
	bl Event_SetStatus1c6
	bl Func_020006a4
.L_02008a90:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008adc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008adc
	movs r1, #200
	movs r2, #216
	lsls r2, r2, #16
	movs r0, #32
	lsls r1, r1, #16
	bl Func_02000d40
	movs r0, #32
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	bl Object_GetById
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_02008adc:
	movs r1, #3
	movs r0, #24
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	bl Object_GetById
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	pop {pc}
.L_02008af8:
	.4byte Func_0200060c
.L_02008afc:
	.4byte Data_02000e10
.L_02008b00:
	.4byte gPartyState
	.section .text.x02008b08,"ax",%progbits
	.global Func_02000b08
	.thumb_func
Func_02000b08:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008ba0
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	asrs r5, r3, #20
	bl GameFlag_SetBit
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b76
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #12
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #12
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
	cmp r6, #23
	bne .L_02008b9c
	cmp r5, #9
	bne .L_02008b9c
	movs r1, #188
	movs r2, #136
	ldr r0, [r7]
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000d40
	b .L_02008b9c
.L_02008b76:
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
.L_02008b9c:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02008ba0:
	.4byte gPartyState
	.section .text.x02008ba4,"ax",%progbits
	.global Func_02000ba4
	.thumb_func
Func_02000ba4:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	sub sp, #8
	bl GameFlag_ClearBit
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #40
	movs r1, #8
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
	add sp, #8
	pop {pc}
	.section .text.x02008bdc,"ax",%progbits
	.global Func_02000bdc
	.thumb_func
Func_02000bdc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #192
	movs r2, #0
	ldrsh r5, [r3, r2]
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #4
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #129
	bl Func_02000e08
	movs r2, #16
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #13
	movs r0, #4
	bl Object_SetModeById
	movs r0, #14
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_02000db0
	pop {r5, pc}
	.section .text.x02008c58,"ax",%progbits
	.global Func_02000c58
	.thumb_func
Func_02000c58:
	push {lr}
	movs r2, #14
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000bdc
	pop {pc}
	.section .text.x02008c7c,"ax",%progbits
	.global Func_02000c7c
	.thumb_func
Func_02000c7c:
	push {lr}
	movs r2, #14
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000bdc
	pop {pc}
	.2byte 0x0000
	.section .text.x02008ca4,"ax",%progbits
	.global Func_02000ca4
	.thumb_func
Func_02000ca4:
	push {lr}
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000bdc
	pop {pc}
	.2byte 0x0000
	.section .rodata.x02008e10,"a",%progbits
	.global Data_02000e10
Data_02000e10:
	.4byte 0xffff000c
.L_02008e14:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02008f14:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02009014:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02009114:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02009214:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02009314:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00004ccc
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
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
	.4byte 0x000000ab
	.4byte 0x10126002
	.4byte 0xffffffff
	.4byte 0x102010aa
	.4byte 0xffffffff
	.4byte 0x103020aa
	.4byte 0xffffffff
	.4byte 0x104030aa
	.4byte 0xffffffff
	.4byte 0x105040aa
	.4byte 0xffffffff
	.4byte 0x106050aa
	.4byte 0xffffffff
	.4byte 0x107060aa
	.4byte 0xffffffff
	.4byte 0x108070aa
	.4byte 0xffffffff
	.4byte 0x10a010ac
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001494
Data_02001494:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0075
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00018000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00004000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0001c000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0xffff007e
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001e000
	.4byte 0xffff007f
	.4byte 0x00000003
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001644
Data_02001644:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0075
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00018000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff007e
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001e000
	.4byte 0xffff007f
	.4byte 0x00000003
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte .L_02008e14
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte .L_02008f14
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte .L_02009014
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001e000
	.4byte 0xffff007c
	.4byte .L_02009114
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte .L_02009214
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff007d
	.4byte .L_02009314
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020018b4
Data_020018b4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_02000438
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte Func_02000ca4
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte Func_02000c7c
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte Func_02000c58
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000022a5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000022a6
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000022a7
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000022a8
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000022a9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000022aa
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000022ab
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000022ac
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020004bc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000022ce
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x000022cf
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000022b3
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000022b4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000022b5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000022b6
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000022b7
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000022b8
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000022b9
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000022ba
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000022bb
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000022d0
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000022d1
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000038
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000048
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_02000038
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_02000048
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte Func_020001cc
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte Func_020001ec
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte Func_0200027c
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte Func_0200030c
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte Func_0200032c
	.4byte 0x50008905
	.4byte 0xffff001a
	.4byte Func_0200034c
	.4byte 0x50008905
	.4byte 0xffff001b
	.4byte Func_0200036c
	.4byte 0x50008905
	.4byte 0xffff001c
	.4byte Func_020003f8
	.4byte 0x50008905
	.4byte 0xffff001d
	.4byte Func_02000418
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000b08
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000ba4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001b00
Data_02001b00:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_02000438
	.4byte 0x0000c602
	.4byte 0x03020008
	.4byte Func_02000438
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte Func_02000ca4
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte Func_02000c7c
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte Func_02000c58
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000231b
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000231c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000231d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000231e
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000231f
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002320
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002321
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002322
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000538
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002342
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002343
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002332
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002333
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002334
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002335
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00002336
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00002337
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte Func_02000954
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002329
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000232a
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000232b
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000232c
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000232d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000232e
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000232f
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002330
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002331
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002344
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002345
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000233b
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x0000233c
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x0000233d
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x0000233e
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x0000233f
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002340
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00002341
	.4byte 0x00008d15
	.4byte 0xffff0420
	.4byte Func_02000954
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_02000038
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_02000048
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000038
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000048
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000b08
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000ba4
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte Func_020001cc
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte Func_020001ec
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte Func_0200027c
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte Func_0200030c
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte Func_0200032c
	.4byte 0x50008905
	.4byte 0xffff001a
	.4byte Func_0200034c
	.4byte 0x50008905
	.4byte 0xffff001b
	.4byte Func_0200036c
	.4byte 0x50008905
	.4byte 0xffff001c
	.4byte Func_020003f8
	.4byte 0x50008905
	.4byte 0xffff001d
	.4byte Func_02000418
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.2byte 0x0001
