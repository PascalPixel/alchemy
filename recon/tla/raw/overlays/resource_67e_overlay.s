.syntax unified
	.thumb
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
	.4byte MsgTheMisalignedStone
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
	.4byte MsgCanYouImagine
.L_02008534:
	.4byte MsgSeeThisRing
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
	.4byte MsgButItS
.L_020085b0:
	.4byte MsgSeeThisRingOf
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
	.4byte MsgWait2
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
	.4byte MsgThisTokenIsn
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
	.4byte MsgWouldYouBe
.L_020089c8:
	.4byte gPartyState
.L_020089cc:
	.4byte MsgTheLegendSays
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
	bl Event_StartScreenTransition
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
