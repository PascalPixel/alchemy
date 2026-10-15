.syntax unified
	.thumb
	.section .text.x02008a5c,"ax",%progbits
	.balign 4
	.global Func_02000a5c
	.thumb_func
Func_02000a5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #11
	sub sp, #48
	bl Object_GetById
	mov r11, r0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a86
	bl .L_0200927c
.L_02008a86:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_SetBit
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_0200282c
	movs r0, #0
	bl Func_020028fc
	ldr r0, .L_02008d54
	bl Func_020028ac
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #4
	movs r1, #136
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #8
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020028dc
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #8
	movs r2, #16
	negs r2, r2
	negs r1, r1
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #12
	movs r2, #12
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008c64
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	movs r2, #10
	adds r0, #10
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008c8a
.L_02008c64:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_02008c8a:
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008d58
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	movs r2, #10
	adds r0, #10
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008d7e
.L_02008d54:
	.4byte MsgLemuriaInteriors10
.L_02008d58:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_02008d7e:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #20
	bl Func_020028bc
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #89
	movs r2, #92
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020028dc
	movs r1, #3
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #182
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002924
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_020028e4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	mov r1, r11
	adds r1, #85
	str r1, [sp, #0]
	movs r2, #0
	movs r3, #36
	strb r2, [r1]
	add r3, sp
	mov r1, r11
	mov r10, r3
	ldr r3, [r1, #8]
	mov r1, r10
	str r3, [r1]
	mov r1, r11
	ldr r3, [r1, #12]
	mov r1, r10
	str r3, [r1, #4]
	mov r1, r11
	ldr r3, [r1, #16]
	mov r1, r10
	str r3, [r1, #8]
	add r3, sp, #24
	mov r8, r3
	movs r3, #232
	mov r1, r8
	lsls r3, r3, #15
	str r3, [r1]
	str r2, [r1, #4]
	movs r3, #220
	movs r2, #176
	lsls r3, r3, #15
	lsls r2, r2, #8
	str r3, [r1, #8]
	str r2, [sp, #4]
	movs r7, #0
.L_02008ede:
	movs r1, #192
	lsls r0, r7, #15
	bl __divsi3
	adds r6, r0, #0
	bl Math_Sine
	ldr r3, [sp, #4]
	movs r1, #128
	lsls r1, r1, #2
	adds r3, r3, r1
	lsls r0, r0, #5
	str r0, [sp, #8]
	str r3, [sp, #4]
	add r2, sp, #12
	mov r9, r2
	mov r1, r8
	mov r2, r10
	ldr r5, [r2]
	ldr r3, [r1]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	adds r0, r6, #0
	bl Math_Sine
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	adds r6, r0, #0
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #192
	bl __divsi3
	lsls r3, r6, #3
	subs r3, r3, r6
	adds r5, r5, r0
	lsls r3, r3, #3
	adds r5, r5, r3
	mov r3, r9
	str r5, [r3, #4]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3, #8]
	ldr r1, [sp, #4]
	mov r2, r9
	ldr r0, [sp, #8]
	bl Vector_AddPolarOffsetFar
	mov r1, r9
	ldr r3, [r1]
	mov r2, r11
	str r3, [r2, #8]
	ldr r3, [r1, #4]
	str r3, [r2, #12]
	ldr r3, [r1, #8]
	str r3, [r2, #16]
	ldr r1, [sp, #4]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r1, r2
	mov r1, r11
	strh r3, [r1, #6]
	cmp r7, #128
	bne .L_02008f92
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008f92:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #191
	ble .L_02008ede
	movs r3, #128
	lsls r3, r3, #8
	mov r2, r11
	strh r3, [r2, #6]
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #11
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #100
	movs r2, #112
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #10
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #11
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #8
	strb r3, [r0]
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020028dc
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	ldr r5, .L_020090dc
	ldr r3, [sp, #0]
	mov r1, r11
	strb r5, [r3]
	mov r2, r10
	ldr r3, [r1, #8]
	movs r7, #0
	str r3, [r2]
	ldr r3, [r1, #12]
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	mov r1, r8
	str r3, [r2, #8]
	movs r3, #132
	lsls r3, r3, #16
	str r3, [r1]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r1, #4]
	movs r2, #132
	movs r3, #200
	lsls r3, r3, #15
	lsls r2, r2, #7
	str r3, [r1, #8]
	str r2, [sp, #4]
	b .L_020090e0
	.2byte 0x0000
.L_020090dc:
	.4byte 0x00000000
.L_020090e0:
	movs r1, #192
	lsls r0, r7, #14
	bl __divsi3
	bl Math_Sine
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	str r3, [sp, #8]
	ldr r3, [sp, #4]
	movs r1, #192
	lsls r1, r1, #2
	adds r3, r3, r1
	str r3, [sp, #4]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2]
	ldr r5, [r1]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	mov r2, r9
	adds r5, r5, r0
	str r5, [r2]
	movs r1, #192
	lsls r0, r7, #15
	bl __divsi3
	bl Math_Sine
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	adds r6, r0, #0
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #192
	bl __divsi3
	lsls r6, r6, #5
	adds r5, r5, r0
	adds r5, r5, r6
	mov r3, r9
	str r5, [r3, #4]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3, #8]
	ldr r1, [sp, #4]
	mov r2, r9
	ldr r0, [sp, #8]
	bl Vector_AddPolarOffsetFar
	mov r1, r9
	ldr r3, [r1]
	mov r2, r11
	str r3, [r2, #8]
	ldr r3, [r1, #4]
	str r3, [r2, #12]
	ldr r3, [r1, #8]
	str r3, [r2, #16]
	ldr r1, [sp, #4]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r1, r2
	mov r1, r11
	strh r3, [r1, #6]
	cmp r7, #153
	bne .L_02009194
	movs r1, #224
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02009194:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #191
	ble .L_020090e0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r3, #208
	mov r2, r11
	lsls r3, r3, #8
	strh r3, [r2, #6]
	ldr r3, [r2, #8]
	mov r1, r10
	str r3, [r1]
	movs r7, #0
	ldr r3, [r2, #12]
	str r3, [r1, #4]
	ldr r3, [r2, #16]
	mov r2, r8
	str r3, [r1, #8]
	movs r3, #180
	lsls r3, r3, #15
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r2, #4]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r2, #8]
.L_020091d8:
	mov r1, r8
	ldr r3, [r1]
	mov r1, r10
	ldr r2, [r1]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_020091ea
	adds r3, #15
.L_020091ea:
	asrs r3, r3, #4
	adds r3, r2, r3
	mov r2, r11
	str r3, [r2, #8]
	mov r1, r8
	ldr r3, [r1, #4]
	mov r1, r10
	ldr r2, [r1, #4]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_02009204
	adds r3, #15
.L_02009204:
	asrs r3, r3, #4
	adds r3, r2, r3
	mov r2, r11
	str r3, [r2, #12]
	mov r1, r8
	ldr r3, [r1, #8]
	mov r1, r10
	ldr r2, [r1, #8]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_0200921e
	adds r3, #15
.L_0200921e:
	asrs r3, r3, #4
	adds r3, r2, r3
	mov r2, r11
	str r3, [r2, #16]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #15
	ble .L_020091d8
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02002874
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	bl Func_02002834
.L_0200927c:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020092dc,"ax",%progbits
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	bl Object_GetById
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #12
	ldr r0, [r5, #8]
	mov r10, r3
	ldr r1, [r5, #12]
	movs r3, #224
	lsls r3, r3, #13
	mov r8, r3
	movs r3, #128
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #5
	movs r6, #15
	add r0, r10
	str r6, [sp, #0]
	mov r11, r3
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r3, .L_02009394
	ldr r1, [r5, #12]
	adds r0, r0, r3
	movs r3, #240
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #8
	str r6, [sp, #0]
	mov r9, r3
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	add r1, r8
	mov r3, r11
	add r0, r10
	str r6, [sp, #0]
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r3, .L_02009394
	ldr r2, [r5, #16]
	add r1, r8
	adds r0, r0, r3
	mov r3, r9
	str r6, [sp, #0]
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
.L_02009394:
	.4byte 0xfff80000
