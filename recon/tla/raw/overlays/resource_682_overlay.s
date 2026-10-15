.syntax unified
	.thumb
	.ifdef TLA_EDITION_JA
	.section .text.x02008638,"ax",%progbits
	.global Func_02000638
	.thumb_func
Func_02000638:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200864a
	b .L_ja_02008a4e
.L_ja_0200864a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #117
	bl GameFlag_SetBit
	bl Func_02002a44
	movs r0, #0
	bl Func_02002b64
	ldr r0, .L_ja_020086fc
	bl Func_02002adc
	movs r3, #192
	movs r1, #157
	lsls r3, r3, #8
	movs r0, #29
	lsls r1, r1, #17
	ldr r2, .L_ja_02008700
	bl Func_02002aa4
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #157
	movs r2, #145
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #29
	bl Object_AttachWorkTargetToObject
	bl Func_02002b3c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #157
	movs r2, #156
	movs r0, #29
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r1, #0
	movs r0, #29
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_ja_02008704
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2, #0]
	adds r3, #1
	strh r3, [r2, #0]
	b .L_ja_02008720
.L_ja_020086fc:
	.4byte 0x000030b7
.L_ja_02008700:
	.4byte 0x02a70000
.L_ja_02008704:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2, #0]
	movs r0, #29
	adds r3, #1
	strh r3, [r2, #0]
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
.L_ja_02008720:
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #16
	movs r2, #0
	movs r0, #30
	bl Func_02002b74
	movs r0, #30
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #29
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #4
	movs r2, #0
	movs r0, #30
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r0, #29
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r0, #30
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #29
	bl Func_02002b1c
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #29
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	bl Func_0200060c
	movs r1, #4
	movs r2, #0
	movs r0, #30
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #29
	ldr r1, .L_ja_02008a50
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #100
	movs r0, #29
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #30
	movs r0, #4
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_ja_02008a30
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #30
	bl ObjectMotion_ResetAndSetPosition
.L_ja_02008a30:
	movs r0, #30
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	bl Func_02002a4c
.L_ja_02008a4e:
	pop {pc}
.L_ja_02008a50:
	.4byte 0x00013333
	.endif
	.section .text.x02008d4c,"ax",%progbits
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [sp, #32]
	mov r8, r1
	mov r10, r2
	ldr r7, [r3, #108]
	bl ObjectTable_ReadActiveValue
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_ReadActiveValue
	adds r5, r0, #0
	adds r0, r6, #0
	bl BattleFx_GetResourceId
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r1]
	asrs r0, r0, #16
	mov r1, r8
	mov r2, r10
	bl UiText_OpenMessageWindow
	adds r0, r6, #0
	ldr r3, [sp, #28]
	movs r1, #0
	mov r2, r9
	bl Func_02002a2c
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_02008db6
	b .L_02008dae
.L_02008da8:
	ldr r0, [sp, #52]
	bl WaitFrames
.L_02008dae:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_02008da8
.L_02008db6:
	adds r0, r5, #0
	bl BattleFx_GetResourceId
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	strh r2, [r1]
	lsls r0, r0, #16
	ldr r1, [sp, #36]
	ldr r2, [sp, #40]
	asrs r0, r0, #16
	bl UiText_OpenMessageWindow
	adds r0, r5, #0
	movs r1, #0
	ldr r2, [sp, #44]
	ldr r3, [sp, #48]
	bl Func_02002a2c
	b .L_02008dec
.L_02008de6:
	movs r0, #1
	bl WaitFrames
.L_02008dec:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_02008de6
	movs r0, #1
	bl WaitFrames
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
