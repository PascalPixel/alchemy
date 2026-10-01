.syntax unified
	.thumb
	.global Unnamed_080cb4ec
	.thumb_func
Unnamed_080cb4ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080cb55c
	adds r3, r6, #0
	ldmia r3!, {r2}
	ldr r3, [r3]
	sub sp, #36
	str r3, [sp, #12]
	ldr r3, .L_080cb560
	mov r11, r2
	add r3, r11
	str r0, [r3]
	movs r0, #1
	mov r8, r3
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080cb564
	ldr r3, .L_080cb554
	strh r3, [r2]
	ldr r3, .L_080cb558
	adds r2, #50
	strh r3, [r2]
	movs r5, #1
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r3, [r6, #28]
	movs r1, #7
	str r3, [sp, #16]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r3, [r6, #32]
	mov r0, sp
	adds r0, #16
	str r0, [sp, #8]
	mov r1, r11
	str r3, [r0, #4]
	b .L_080cb568
	.2byte 0x0000
.L_080cb554:
	.4byte 0x00000100
.L_080cb558:
	.4byte 0x00001000
.L_080cb55c:
	.4byte gBattleFxWork
.L_080cb560:
	.4byte 0x00007828
.L_080cb564:
	.4byte 0x04000020
.L_080cb568:
	movs r2, #1
	ldr r0, .L_080cb604
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080cb608
	add r3, r11
	str r5, [r3]
	add r2, r11
	movs r3, #0
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080cb60c
	bl Scheduler_AddOrUpdateCallback
	mov r2, r8
	ldr r3, [r2]
	add r5, sp, #24
	movs r2, #36
	ldrsh r0, [r3, r2]
	adds r1, r5, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r2, [r5]
	movs r3, #64
	ldr r1, .L_080cb610
	subs r3, r3, r2
	lsls r3, r3, #8
	movs r5, #225
	str r3, [r1]
	lsls r5, r5, #7
	movs r3, #0
	mov r9, r3
	add r5, r11
.L_080cb5b2:
	bl Random16
	movs r1, #96
	bl __umodsi3
	mov r2, r9
	adds r0, #16
	str r0, [r5]
	cmp r2, #0
	bge .L_080cb5c8
	adds r2, #3
.L_080cb5c8:
	asrs r2, r2, #2
	movs r3, #24
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [r5, #4]
	cmp r0, #43
	bgt .L_080cb5da
	movs r3, #3
	b .L_080cb618
.L_080cb5da:
	cmp r0, #51
	bgt .L_080cb5e2
	movs r3, #2
	b .L_080cb618
.L_080cb5e2:
	cmp r0, #59
	bgt .L_080cb5ea
	movs r3, #1
	b .L_080cb618
.L_080cb5ea:
	cmp r0, #67
	bgt .L_080cb5f2
	movs r3, #0
	b .L_080cb618
.L_080cb5f2:
	cmp r0, #75
	bgt .L_080cb5fa
	movs r3, #1
	b .L_080cb616
.L_080cb5fa:
	cmp r0, #83
	bgt .L_080cb614
	movs r3, #2
	b .L_080cb616
	.2byte 0x0000
.L_080cb604:
	.4byte 0x00000078
.L_080cb608:
	.4byte 0x00007784
.L_080cb60c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cb610:
	.4byte 0x04000028
.L_080cb614:
	movs r3, #3
.L_080cb616:
	negs r3, r3
.L_080cb618:
	str r3, [r5, #12]
	ldr r3, [r5, #12]
	lsls r3, r3, #17
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #16]
	movs r0, #1
	ldr r3, [r5]
	add r9, r0
	lsls r3, r3, #16
	mov r2, r9
	str r3, [r5]
	adds r5, #28
	cmp r2, #64
	bne .L_080cb5b2
	movs r0, #212
	bl AudioCommand_PlayFar
	movs r3, #0
	mov r10, r3
.L_080cb642:
	mov r0, r10
	cmp r0, #16
	bgt .L_080cb65c
	ldr r2, .L_080cb674
	mov r3, r10
	orrs r3, r2
	ldr r2, .L_080cb680
	strh r3, [r2]
	cmp r0, #16
	bne .L_080cb65c
	ldr r3, .L_080cb678
	subs r2, #2
	strh r3, [r2]
.L_080cb65c:
	mov r3, r10
	cmp r3, #103
	ble .L_080cb68e
	ldr r3, .L_080cb67c
	ldr r2, .L_080cb674
	mov r0, r10
	subs r3, r3, r0
	orrs r3, r2
	ldr r2, .L_080cb680
	strh r3, [r2]
	b .L_080cb684
	.2byte 0x0000
.L_080cb674:
	.4byte 0x00001000
.L_080cb678:
	.4byte 0x00000000
.L_080cb67c:
	.4byte 0x00000078
.L_080cb680:
	.4byte 0x04000052
.L_080cb684:
	cmp r0, #104
	bne .L_080cb68e
	ldr r3, .L_080cb6b8
	subs r2, #2
	strh r3, [r2]
.L_080cb68e:
	ldr r7, .L_080cb6bc
	movs r3, #15
	mov r9, r3
	add r7, r11
.L_080cb696:
	ldr r6, [r7, #12]
	adds r3, r6, #0
	cmp r6, #0
	bge .L_080cb6a0
	negs r3, r6
.L_080cb6a0:
	mov r0, r9
	lsls r0, r0, #2
	mov r8, r0
	asrs r4, r3, #17
	mov r3, r8
	adds r3, #25
	cmp r10, r3
	bge .L_080cb70a
	ldr r2, .L_080cb6c0
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	b .L_080cb6c4
.L_080cb6b8:
	.4byte 0x00003f44
.L_080cb6bc:
	.4byte 0x00007224
.L_080cb6c0:
	.4byte Data_080edf88
.L_080cb6c4:
	movs r3, #2
	ldrsh r2, [r7, r3]
	ldr r3, .L_080cb7dc
	ldrb r5, [r3, r4]
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r7, r0]
	ldr r0, .L_080cb7e0
	ldrb r4, [r0, r4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r5, [sp, #0]
	ldr r0, [sp, #8]
	str r4, [sp, #4]
	lsrs r6, r6, #31
	lsls r6, r6, #2
	ldr r4, [r6, r0]
	add r1, r11
	ldr r0, [sp, #12]
	bl _call_via_r4
	mov r3, r8
	adds r3, #16
	cmp r10, r3
	blt .L_080cb73e
	ldr r3, [r7]
	ldr r2, [r7, #12]
	adds r3, r3, r2
	str r3, [r7]
	ldr r2, [r7, #16]
	ldr r3, [r7, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	b .L_080cb73e
.L_080cb70a:
	ldr r2, .L_080cb7e4
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	movs r3, #2
	ldrsh r2, [r7, r3]
	ldr r3, .L_080cb7dc
	ldrb r5, [r3, r4]
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r7, r0]
	ldr r0, .L_080cb7e0
	ldrb r4, [r0, r4]
	lsrs r0, r4, #1
	subs r4, #4
	lsrs r6, r6, #31
	subs r3, r3, r0
	str r5, [sp, #0]
	ldr r0, [sp, #8]
	str r4, [sp, #4]
	lsls r6, r6, #2
	ldr r4, [r6, r0]
	add r1, r11
	ldr r0, [sp, #12]
	bl _call_via_r4
.L_080cb73e:
	movs r2, #1
	negs r2, r2
	add r9, r2
	subs r7, #28
	cmp r9, r2
	bne .L_080cb696
	mov r3, r10
	subs r3, #23
	cmp r3, #64
	bhi .L_080cb78c
	movs r3, #3
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	bne .L_080cb78c
	ldr r3, .L_080cb7e8
	add r3, r11
	ldr r3, [r3]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #2
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	movs r1, #7
	bl ObjectGroup_UpdateMembers
	ldr r2, .L_080cb7ec
	movs r3, #1
	add r2, r11
	str r3, [r2]
	mov r0, r10
	movs r3, #7
	ands r3, r0
	cmp r3, #0
	bne .L_080cb78c
	movs r0, #133
	bl AudioCommand_PlayFar
.L_080cb78c:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cb7f0
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #120
	beq .L_080cb7b2
	b .L_080cb642
.L_080cb7b2:
	ldr r0, .L_080cb7f4
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080cb7dc:
	.4byte Data_080edf7f
.L_080cb7e0:
	.4byte Data_080edf83
.L_080cb7e4:
	.4byte Data_080edf88
.L_080cb7e8:
	.4byte 0x00007828
.L_080cb7ec:
	.4byte 0x000077a8
.L_080cb7f0:
	.4byte 0x00007824
.L_080cb7f4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
