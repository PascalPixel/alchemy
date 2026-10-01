.syntax unified
	.thumb
	.global Unnamed_080d3854
	.thumb_func
Unnamed_080d3854:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080d38c0
	adds r3, r6, #0
	ldmia r3!, {r1}
	sub sp, #52
	str r1, [sp, #40]
	ldr r2, .L_080d38c4
	ldr r3, [r3]
	str r3, [sp, #36]
	adds r3, r1, r2
	str r0, [r3]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d38c8
	ldr r3, .L_080d38bc
	ldr r0, .L_080d38cc
	strh r3, [r2]
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080d38d0
	adds r1, r5, #0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	adds r5, #128
	ldr r1, [sp, #40]
	adds r0, r5, #0
	bl Resource_DecodeType01
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r6, #28]
	movs r1, #7
	str r3, [sp, #44]
	movs r2, #7
	b .L_080d38d4
.L_080d38bc:
	.4byte 0x00001010
.L_080d38c0:
	.4byte gBattleFxWork
.L_080d38c4:
	.4byte 0x00007828
.L_080d38c8:
	.4byte 0x04000052
.L_080d38cc:
	.4byte 0x000000ce
.L_080d38d0:
	.4byte IwramCopyWords
.L_080d38d4:
	movs r3, #7
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r6, #32]
	mov r0, sp
	adds r0, #44
	str r0, [sp, #24]
	str r3, [r0, #4]
	ldr r2, [sp, #40]
	movs r3, #225
	movs r1, #0
	lsls r3, r3, #7
	mov r8, r1
	adds r5, r2, r3
.L_080d38f4:
	bl Random16
	movs r3, #31
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r0, #1
	add r8, r0
	negs r3, r3
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #16
	bne .L_080d38f4
	ldr r3, [sp, #40]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #40]
	ldr r3, .L_080d3980
	adds r2, r1, r3
	movs r3, #50
	movs r1, #144
	lsls r1, r1, #3
	str r3, [r2]
	ldr r0, .L_080d3984
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080d3988
	ldr r3, .L_080d397c
	movs r0, #1
	strh r3, [r2]
	bl WaitFrames
	movs r0, #141
	bl AudioCommand_PlayFar
	ldr r2, .L_080d398c
	ldr r1, [sp, #40]
	adds r2, r1, r2
	movs r0, #0
	str r2, [sp, #28]
	mov r11, r0
.L_080d3960:
	mov r3, r11
	lsls r0, r3, #10
	bl Trig_Sin
	lsls r0, r0, #4
	str r0, [sp, #32]
	mov r0, r11
	cmp r0, #32
	bne .L_080d3990
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080d3990
	.2byte 0x0000
.L_080d397c:
	.4byte 0x00001000
.L_080d3980:
	.4byte 0x00007784
.L_080d3984:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d3988:
	.4byte 0x04000052
.L_080d398c:
	.4byte 0x00007828
.L_080d3990:
	movs r1, #0
	ldr r6, .L_080d39cc
	mov r9, r1
	movs r5, #16
.L_080d3998:
	cmp r11, r5
	bne .L_080d39a8
	movs r1, #128
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	ldr r2, .L_080d39d0
	bl _call_via_r6
.L_080d39a8:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r5, #8
	cmp r3, #7
	bne .L_080d3998
	ldr r0, [sp, #28]
	ldr r3, [r0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d39d4
	ldr r1, [sp, #32]
	movs r2, #128
	lsls r2, r2, #14
	adds r1, r1, r2
	str r1, [sp, #32]
	b .L_080d39dc
	.2byte 0x0000
.L_080d39cc:
	.4byte IwramFillWords
.L_080d39d0:
	.4byte BattleAction_DefinitionTable + 0x19b0
.L_080d39d4:
	ldr r3, [sp, #32]
	ldr r0, .L_080d3a1c
	adds r3, r3, r0
	str r3, [sp, #32]
.L_080d39dc:
	mov r1, r11
	cmp r1, #16
	bgt .L_080d39ec
	ldr r2, .L_080d3a14
	ldr r1, .L_080d3a20
	mov r3, r11
	orrs r3, r2
	strh r3, [r1]
.L_080d39ec:
	mov r2, r11
	cmp r2, #63
	ble .L_080d3a00
	ldr r2, .L_080d3a18
	mov r0, r11
	ldr r1, .L_080d3a14
	ldr r3, .L_080d3a20
	subs r2, r2, r0
	orrs r2, r1
	strh r2, [r3]
.L_080d3a00:
	ldr r2, [sp, #28]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	ldr r0, .L_080d3a24
	lsls r3, r2, #1
	adds r3, r3, r2
	ldrb r3, [r0, r3]
	movs r1, #0
	mov r9, r1
	b .L_080d3a28
.L_080d3a14:
	.4byte 0x00001000
.L_080d3a18:
	.4byte 0x0000004f
.L_080d3a1c:
	.4byte 0xffe00000
.L_080d3a20:
	.4byte 0x04000052
.L_080d3a24:
	.4byte Data_080ee1b4 + 0x16
.L_080d3a28:
	cmp r3, #0
	bne .L_080d3a2e
	b .L_080d3b88
.L_080d3a2e:
	mov r1, r11
	mov r2, r11
	ldr r3, [sp, #40]
	asrs r1, r1, #31
	lsls r2, r2, #11
	str r1, [sp, #20]
	str r2, [sp, #16]
	str r3, [sp, #12]
.L_080d3a3e:
	ldr r0, [sp, #16]
	bl Trig_Sin
	ldr r1, [sp, #28]
	ldr r3, [r1]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_080d3c54
	adds r3, #1
	ldrb r3, [r2, r3]
	muls r3, r0
	ldr r0, [sp, #32]
	adds r3, r3, r0
	asrs r3, r3, #16
	ldr r0, [sp, #16]
	adds r3, #40
	mov r10, r3
	bl Trig_Cos
	ldr r1, [sp, #20]
	lsls r0, r0, #1
	asrs r7, r0, #16
	lsrs r0, r1, #31
	add r0, r11
	movs r1, #3
	asrs r0, r0, #1
	bl __modsi3
	lsls r5, r0, #2
	ldr r2, [sp, #40]
	adds r5, r5, r0
	lsls r6, r5, #9
	ldr r3, .L_080d3c58
	adds r6, r2, r6
	adds r1, r6, r3
	movs r0, #40
	movs r2, #32
	adds r3, r7, #0
	str r0, [sp, #0]
	str r2, [sp, #4]
	ldr r4, [sp, #44]
	ldr r0, [sp, #36]
	adds r3, #16
	mov r2, r10
	bl _call_via_r4
	ldr r3, [sp, #40]
	ldr r0, .L_080d3c5c
	lsls r5, r5, #8
	adds r5, r3, r5
	adds r5, r5, r0
	movs r1, #40
	movs r2, #32
	adds r3, r7, #0
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r4, [sp, #44]
	adds r3, #48
	ldr r0, [sp, #36]
	adds r1, r5, #0
	mov r2, r10
	bl _call_via_r4
	ldr r3, .L_080d3c60
	movs r0, #40
	adds r6, r6, r3
	movs r1, #32
	adds r3, r7, #0
	adds r3, #80
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r2, r10
	adds r1, r6, #0
	ldr r4, [sp, #44]
	ldr r0, [sp, #36]
	bl _call_via_r4
	movs r0, #225
	ldr r3, [sp, #12]
	movs r2, #0
	lsls r0, r0, #7
	mov r8, r2
	adds r6, r3, r0
.L_080d3ae6:
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_080d3b32
	mov r1, r8
	lsrs r2, r1, #31
	add r2, r8
	asrs r2, r2, #1
	lsrs r4, r3, #31
	adds r4, r3, r4
	lsls r3, r2, #1
	adds r3, r3, r2
	asrs r4, r4, #1
	movs r2, #1
	mov r5, r8
	adds r4, r4, r3
	ands r5, r2
	ldr r0, .L_080d3c64
	ldr r2, .L_080d3c68
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldrb r0, [r0, r4]
	ldr r3, [sp, #40]
	ldr r2, [r6]
	adds r1, r3, r1
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	ldr r0, .L_080d3c6c
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #24]
	lsls r5, r5, #2
	adds r3, r3, r7
	ldr r4, [r5, r0]
	add r2, r10
	ldr r0, [sp, #36]
	bl _call_via_r4
	ldr r3, [r6, #24]
.L_080d3b32:
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #6
	bne .L_080d3b54
	bl Random16
	movs r3, #31
	ands r3, r0
	str r3, [r6]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #16
	str r3, [r6, #4]
	movs r3, #0
	str r3, [r6, #24]
.L_080d3b54:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #28
	cmp r2, #4
	bne .L_080d3ae6
	ldr r3, [sp, #16]
	ldr r1, [sp, #12]
	movs r0, #128
	lsls r0, r0, #7
	adds r3, r3, r0
	adds r1, #112
	str r1, [sp, #12]
	str r3, [sp, #16]
	ldr r0, [sp, #28]
	ldr r3, [r0]
	movs r2, #1
	add r9, r2
	ldr r2, [r3, #24]
	ldr r1, .L_080d3c54
	lsls r3, r2, #1
	adds r3, r3, r2
	ldrb r3, [r1, r3]
	cmp r9, r3
	beq .L_080d3b88
	b .L_080d3a3e
.L_080d3b88:
	ldr r0, [sp, #28]
	ldr r3, [r0]
	ldr r3, [r3, #20]
	movs r2, #0
	mov r8, r2
	cmp r3, #0
	beq .L_080d3bec
	ldr r1, [sp, #40]
	ldr r2, .L_080d3c70
	movs r3, #0
	adds r7, r1, r2
	mov r10, r3
	movs r4, #36
.L_080d3ba2:
	movs r0, #0
	mov r5, r10
	mov r9, r0
	adds r6, r4, #0
	adds r5, #16
.L_080d3bac:
	cmp r11, r5
	bne .L_080d3bd0
	ldr r3, [r7]
	ldrsh r0, [r3, r6]
	movs r3, #4
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	mov r3, r8
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r3, [r7]
	movs r1, #6
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r4, [sp, #8]
.L_080d3bd0:
	movs r3, #1
	add r9, r3
	mov r0, r9
	adds r5, #8
	cmp r0, #7
	bne .L_080d3bac
	add r8, r3
	ldr r3, [r7]
	ldr r3, [r3, #20]
	movs r1, #3
	add r10, r1
	adds r4, #2
	cmp r8, r3
	bne .L_080d3ba2
.L_080d3bec:
	ldr r2, [sp, #40]
	ldr r0, .L_080d3c74
	movs r1, #1
	adds r3, r2, r0
	str r1, [r3]
	ldr r2, [sp, #28]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r1, .L_080d3c54
	adds r3, #2
	ldrb r0, [r1, r3]
	lsls r1, r0, #1
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r0, .L_080d3c78
	ldr r2, [sp, #40]
	movs r1, #1
	adds r3, r2, r0
	str r1, [r3]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r11, r2
	mov r3, r11
	cmp r3, #80
	beq .L_080d3c2c
	b .L_080d3960
.L_080d3c2c:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080d3c7c
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080d3c54:
	.4byte Data_080ee1b4 + 0x16
.L_080d3c58:
	.4byte 0x00000c56
.L_080d3c5c:
	.4byte 0x00002a56
.L_080d3c60:
	.4byte 0x00001156
.L_080d3c64:
	.4byte BattleFx_GlintCellWidths
.L_080d3c68:
	.4byte BattleFx_GlintCellOffsets
.L_080d3c6c:
	.4byte BattleFx_GlintCellHeights
.L_080d3c70:
	.4byte 0x00007828
.L_080d3c74:
	.4byte 0x000077a8
.L_080d3c78:
	.4byte 0x00007824
.L_080d3c7c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
