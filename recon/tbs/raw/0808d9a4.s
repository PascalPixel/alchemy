.syntax unified
	.thumb
	.global Func_0808d9a4
	.thumb_func
Func_0808d9a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r0
	ldr r3, .L_0808db74
	movs r0, #250
	lsls r0, r0, #1
	mov r5, r8
	adds r3, r3, r0
	subs r5, #242
	ldr r6, [r3]
	cmp r5, #5
	bhi .L_0808d9e6
	bl Battle_InitializeRenderObject
	ldr r3, .L_0808db78
	ldrb r3, [r3, r5]
	ldr r0, .L_0808db7c
	mov r8, r3
	add r0, r8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r0, .L_0808db80
	movs r1, #1
	add r0, r8
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_0808dd72
.L_0808d9e6:
	movs r0, #3
	mov r1, r8
	bl BattleFx_FindDescriptor
	adds r7, r0, #0
	cmp r7, #0
	bne .L_0808d9f6
	b .L_0808dd5a
.L_0808d9f6:
	ldr r3, [r7]
	asrs r5, r3, #4
	movs r3, #31
	movs r2, #6
	ldrsh r1, [r7, r2]
	ands r5, r3
	ldrh r2, [r7, #4]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	mov r10, r1
	cmp r3, #0
	bne .L_0808da2c
	cmp r5, #0
	beq .L_0808da2c
	bl Battle_InitializeRenderObject
	ldr r0, .L_0808db7c
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
	b .L_0808da34
.L_0808da2c:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_ClearBitFar
.L_0808da34:
	ldr r2, [r7, #8]
	movs r3, #240
	lsls r3, r3, #20
	ands r3, r2
	cmp r3, #0
	bne .L_0808da4e
	ldr r3, .L_0808db84
	movs r0, #128
	ands r3, r2
	lsls r0, r0, #15
	cmp r3, r0
	bne .L_0808da9a
	b .L_0808da82
.L_0808da4e:
	mov r0, r10
	bl GameFlag_IsConditionActive
	cmp r0, #0
	beq .L_0808da68
	ldr r3, .L_0808db74
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	ldr r3, [r7, #8]
	bl _call_via_r3
.L_0808da68:
	movs r0, #161
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808da76
	b .L_0808dd6a
.L_0808da76:
	ldr r0, .L_0808db80
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_0808dd6a
.L_0808da82:
	mov r0, r10
	bl GameFlag_IsConditionActive
	cmp r0, #0
	beq .L_0808da90
	ldrh r0, [r7, #8]
	b .L_0808da92
.L_0808da90:
	ldr r0, .L_0808db88
.L_0808da92:
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_0808dd6a
.L_0808da9a:
	bl Battle_Reset
	mov r0, r10
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_0808daaa
	b .L_0808dd46
.L_0808daaa:
	ldr r1, [r7, #8]
	movs r3, #240
	lsls r3, r3, #12
	movs r0, #128
	ands r3, r1
	lsls r0, r0, #9
	movs r2, #1
	cmp r3, r0
	bne .L_0808dac2
	cmp r6, #7
	bgt .L_0808dac2
	movs r2, #0
.L_0808dac2:
	cmp r2, #0
	bne .L_0808dac8
	b .L_0808dd3c
.L_0808dac8:
	ldr r2, .L_0808db8c
	ldr r3, [r7]
	ands r3, r2
	mov r11, r2
	cmp r3, #19
	bne .L_0808dadc
	mov r0, r8
	bl EffectRuntime_SetMode4AndPlayCue
	ldr r1, [r7, #8]
.L_0808dadc:
	ldr r3, .L_0808db84
	movs r0, #192
	ands r3, r1
	lsls r0, r0, #14
	cmp r3, r0
	bne .L_0808db98
	ldr r3, [r7]
	mov r1, r11
	ands r3, r1
	cmp r3, #19
	bne .L_0808daf8
	mov r0, r8
	bl EffectRuntime_SetMode2
.L_0808daf8:
	mov r0, r8
	bl EffectRuntime_GetCurrentObject
	adds r6, r0, #0
	bl EffectRuntime_PrepareRisingObject
	movs r0, #83
	bl Func_080f9010
	ldrh r0, [r7, #8]
	movs r1, #5
	bl Func_08015120
	ldr r5, .L_0808db90
	movs r1, #3
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r1, #0
	ldr r0, .L_0808db94
	bl BattleParty_ApplyDrain
	movs r0, #1
	bl UiWindow_CreateWithLayoutBoundsFar
	movs r0, #126
	bl Func_080f9010
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	bl UiWork_FinalizeAndReleaseBlock16Far
	movs r1, #2
	adds r0, r6, #0
	bl Func_08009080
	movs r0, #246
	bl Func_080f9010
	adds r5, #2
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	mov r0, r8
	bl EffectRuntime_ClearCurrentFlags
	movs r2, #1
	negs r2, r2
	cmp r10, r2
	bne .L_0808db6a
	b .L_0808dd50
.L_0808db6a:
	mov r0, r10
	bl GameFlag_SetBitFar
	b .L_0808dd50
	.2byte 0x0000
.L_0808db74:
	.4byte gCell
.L_0808db78:
	.4byte Data_0809e680
.L_0808db7c:
	.4byte 0x00000928
.L_0808db80:
	.4byte 0x00000948
.L_0808db84:
	.4byte 0xfff00000
.L_0808db88:
	.4byte 0x00000976
.L_0808db8c:
	.4byte 0x000001ff
.L_0808db90:
	.4byte 0x00000970
.L_0808db94:
	.4byte 0x000003e7
.L_0808db98:
	movs r0, #160
	lsls r0, r0, #15
	cmp r3, r0
	bne .L_0808dc18
	ldr r3, .L_0808dbfc
	ldr r5, [r3]
	ldr r3, [r7]
	mov r1, r11
	ands r3, r1
	cmp r3, #19
	bne .L_0808dbb4
	mov r0, r8
	bl EffectRuntime_SetMode7AndLaunch
.L_0808dbb4:
	movs r2, #1
	negs r2, r2
	cmp r10, r2
	beq .L_0808dbd0
	ldr r2, .L_0808dbf8
	mov r1, r10
	orrs r1, r2
	ldr r3, .L_0808dc00
	movs r0, #141
	lsls r0, r0, #2
	mov r10, r1
	adds r3, r3, r0
	mov r2, r10
	strh r2, [r3]
.L_0808dbd0:
	ldrh r1, [r7, #8]
	movs r0, #99
	bl BattleFx_GetWeightedResult
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r5, r1
	strh r0, [r3]
	ldr r5, .L_0808dc00
	ldr r3, .L_0808dc04
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	ldrh r1, [r7, #8]
	movs r0, #99
	bl BattleFx_SelectBattleCue
	movs r0, #247
	b .L_0808dc08
	.2byte 0x0000
.L_0808dbf8:
	.4byte 0x00001000
.L_0808dbfc:
	.4byte Data_03001ebc
.L_0808dc00:
	.4byte gCell
.L_0808dc04:
	.4byte 0x0000022b
.L_0808dc08:
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_080f9010
	ldr r0, .L_0808dd84
	b .L_0808dd3e
.L_0808dc18:
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	bne .L_0808dc80
	ldr r3, .L_0808dd88
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #0
	bl BattleFx_StartRandomParticleEmitter
	adds r5, r0, #0
	movs r0, #30
	bl WaitFrames
	ldr r3, [r7]
	mov r1, r11
	ands r3, r1
	cmp r3, #19
	bne .L_0808dc48
	mov r0, r8
	bl EffectRuntime_SetMode2
.L_0808dc48:
	adds r0, r5, #0
	bl EffectRuntime_PrepareRisingObject
	movs r0, #83
	bl Func_080f9010
	ldrh r0, [r7, #8]
	movs r1, #5
	bl Func_08015120
	ldr r0, .L_0808dd8c
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
	ldrh r0, [r7, #8]
	bl Party_AdjustSixDigitCounterAFar
	movs r2, #1
	negs r2, r2
	cmp r10, r2
	beq .L_0808dc78
	mov r0, r10
	bl GameFlag_SetBitFar
.L_0808dc78:
	adds r0, r5, #0
	bl ObjectDispatch_ReleaseFar
	b .L_0808dd50
.L_0808dc80:
	ldr r3, .L_0808dd90
	ldr r2, .L_0808dd94
	ldr r0, [r3]
	ands r1, r2
	bl BattleFx_StartRandomParticleEmitter
	mov r9, r0
	movs r0, #30
	bl WaitFrames
	ldrh r0, [r7, #8]
	bl Func_08077030
	movs r3, #1
	adds r6, r0, #0
	negs r3, r3
	ldr r5, .L_0808dd98
	cmp r6, r3
	bne .L_0808dcde
	ldr r0, [r7, #8]
	ldr r1, .L_0808dd94
	ands r0, r1
	movs r1, #2
	bl Func_08015120
	ldr r5, .L_0808dd9c
	movs r1, #1
	adds r0, r5, #0
	adds r5, #4
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	mov r0, r9
	bl Object_DestroyIfPresent
	ldr r3, [r7]
	mov r2, r11
	ands r3, r2
	cmp r3, #19
	bne .L_0808dd50
	mov r0, r8
	bl EffectRuntime_SetMode5AndPlayCue
	b .L_0808dd50
.L_0808dcde:
	ldr r3, [r7]
	mov r0, r11
	ands r3, r0
	cmp r3, #19
	bne .L_0808dcee
	mov r0, r8
	bl EffectRuntime_SetMode2
.L_0808dcee:
	mov r0, r9
	bl EffectRuntime_PrepareRisingObject
	movs r0, #83
	bl Func_080f9010
	ldr r0, [r7, #8]
	movs r1, #2
	ands r0, r5
	bl Func_08015120
	ldr r1, .L_0808dd90
	ldr r3, [r1]
	cmp r6, r3
	bne .L_0808dd16
	ldr r0, .L_0808dda0
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_0808dd26
.L_0808dd16:
	adds r0, r6, #0
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_0808dda4
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWaitFar
.L_0808dd26:
	movs r2, #1
	negs r2, r2
	cmp r10, r2
	beq .L_0808dd34
	mov r0, r10
	bl GameFlag_SetBitFar
.L_0808dd34:
	mov r0, r9
	bl ObjectDispatch_ReleaseFar
	b .L_0808dd50
.L_0808dd3c:
	ldr r0, .L_0808dda8
.L_0808dd3e:
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_0808dd50
.L_0808dd46:
	ldr r0, .L_0808ddac
	movs r1, #1
	adds r0, r5, r0
	bl UiText_ShowPositionedMessageAndWaitFar
.L_0808dd50:
	bl BattleFx_FinishAction
	bl BattleFx_PlayQueuedSound
	b .L_0808dd6a
.L_0808dd5a:
	ldr r0, .L_0808ddb0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r0, .L_0808ddb4
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_0808dd6a:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_ClearBitFar
.L_0808dd72:
	movs r0, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0808dd84:
	.4byte 0x00000973
.L_0808dd88:
	.4byte gCell
.L_0808dd8c:
	.4byte 0x00000969
.L_0808dd90:
	.4byte Data_02000434
.L_0808dd94:
	.4byte 0x00000fff
.L_0808dd98:
	.4byte 0x0000ffff
.L_0808dd9c:
	.4byte 0x00000968
.L_0808dda0:
	.4byte 0x0000096a
.L_0808dda4:
	.4byte 0x0000096b
.L_0808dda8:
	.4byte 0x0000096f
.L_0808ddac:
	.4byte 0x00000948
.L_0808ddb0:
	.4byte 0x0000092d
.L_0808ddb4:
	.4byte 0x0000094d
