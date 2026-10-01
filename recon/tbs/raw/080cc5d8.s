.syntax unified
	.thumb
	.global Func_080cc5d8
	.thumb_func
Func_080cc5d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r0, [sp, #24]
	ldr r1, .L_080cc664
	movs r0, #39
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	mov r9, r0
	lsls r1, r1, #7
	movs r0, #40
	bl Runtime_AllocateHeapBlock
	ldr r1, .L_080cc668
	str r0, [sp, #20]
	movs r0, #41
	bl Runtime_AllocateHeapBlock
	ldr r5, .L_080cc66c
	str r0, [sp, #8]
	ldr r0, [sp, #24]
	add r5, r9
	str r0, [r5]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080cc670
	movs r3, #24
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080cc674
	movs r3, #0
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080cc678
	ldr r3, .L_080cc65c
	strh r3, [r2]
	ldr r3, .L_080cc660
	subs r2, #50
	strh r3, [r2]
	ldr r0, .L_080cc67c
	mov r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .L_080cc680
	ldr r1, [sp, #8]
	movs r2, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	ldr r3, [r3]
	cmp r3, #1
	beq .L_080cc694
	cmp r3, #1
	bgt .L_080cc68a
	b .L_080cc684
	.2byte 0x0000
.L_080cc65c:
	.4byte 0x0000100c
.L_080cc660:
	.4byte 0x00000100
.L_080cc664:
	.4byte 0x0000782c
.L_080cc668:
	.4byte 0x0000060e
.L_080cc66c:
	.4byte 0x00007828
.L_080cc670:
	.4byte 0x000077b4
.L_080cc674:
	.4byte 0x000077b8
.L_080cc678:
	.4byte 0x04000052
.L_080cc67c:
	.4byte 0x00000045
.L_080cc680:
	.4byte 0x00000076
.L_080cc684:
	cmp r3, #0
	beq .L_080cc690
	b .L_080cc6a8
.L_080cc68a:
	cmp r3, #2
	beq .L_080cc698
	b .L_080cc6a8
.L_080cc690:
	ldr r0, .L_080cc69c
	b .L_080cc6aa
.L_080cc694:
	ldr r0, .L_080cc6a0
	b .L_080cc6aa
.L_080cc698:
	ldr r0, .L_080cc6a4
	b .L_080cc6aa
.L_080cc69c:
	.4byte 0x00000048
.L_080cc6a0:
	.4byte 0x00000057
.L_080cc6a4:
	.4byte 0x00000047
.L_080cc6a8:
	ldr r0, .L_080cc7bc
.L_080cc6aa:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080cc7c0
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	ldr r5, .L_080cc7c4
	movs r7, #0
.L_080cc6c0:
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ldr r3, .L_080cc7c8
	ands r3, r0
	str r3, [r5]
	bl Random16
	ldr r3, .L_080cc7cc
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #3
	adds r3, r3, r1
	str r3, [r5, #8]
	negs r3, r7
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #128
	bne .L_080cc6c0
	movs r5, #225
	lsls r5, r5, #7
	movs r7, #0
	add r5, r9
.L_080cc6f4:
	bl Random16
	ldr r3, .L_080cc7c8
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #16
	str r3, [r5, #4]
	movs r3, #15
	ands r3, r7
	adds r3, #16
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne .L_080cc6f4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080cc7d0
	movs r3, #75
	add r2, r9
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080cc7d4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #3
	str r3, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #7
	movs r0, #46
	bl Unnamed_080ed408
	ldr r3, .L_080cc7d8
	adds r3, #184
	ldr r3, [r3]
	movs r0, #140
	str r3, [sp, #12]
	bl AudioCommand_PlayFar
	movs r3, #28
	movs r2, #0
	add r3, sp
	mov r10, r2
	mov r11, r3
.L_080cc75e:
	ldr r4, [sp, #24]
	mov r1, r11
	ldr r0, [r4, #8]
	mov r5, r11
	bl EffectPosition_ApplyStepAndYOffset
	ldr r2, [r5]
	movs r3, #64
	ldr r1, .L_080cc7dc
	subs r3, r3, r2
	lsls r3, r3, #8
	mov r0, r10
	str r3, [r1]
	cmp r0, #49
	ble .L_080cc78a
	ldr r3, .L_080cc7b4
	lsls r2, r0, #1
	subs r3, r3, r2
	ldr r2, .L_080cc7b8
	adds r1, #42
	orrs r3, r2
	strh r3, [r1]
.L_080cc78a:
	mov r1, r10
	cmp r1, #26
	bne .L_080cc7e4
	movs r0, #212
	bl AudioCommand_PlayFar
	ldr r3, .L_080cc7e0
	add r3, r9
	ldr r3, [r3]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #20
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	b .L_080cc7e4
	.2byte 0x0000
.L_080cc7b4:
	.4byte 0x00000070
.L_080cc7b8:
	.4byte 0x00001000
.L_080cc7bc:
	.4byte 0x00000046
.L_080cc7c0:
	.4byte IwramCopyWords
.L_080cc7c4:
	.4byte gMapCellBuffer
.L_080cc7c8:
	.4byte 0x0000ffff
.L_080cc7cc:
	.4byte 0x000001ff
.L_080cc7d0:
	.4byte 0x00007784
.L_080cc7d4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cc7d8:
	.4byte gWorkSlot
.L_080cc7dc:
	.4byte 0x04000028
.L_080cc7e0:
	.4byte 0x00007828
.L_080cc7e4:
	mov r0, r10
	subs r0, #28
	cmp r0, #20
	bhi .L_080cc816
	movs r1, #3
	bl __divsi3
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #8
	movs r3, #160
	lsls r3, r3, #5
	add r1, r9
	mov r4, r11
	adds r1, r1, r3
	ldr r3, [r4, #4]
	movs r2, #48
	str r2, [sp, #0]
	str r2, [sp, #4]
	subs r3, #24
	ldr r0, [sp, #20]
	movs r2, #40
	ldr r5, [sp, #12]
	bl _call_via_r5
.L_080cc816:
	mov r0, r10
	cmp r0, #14
	bhi .L_080cc878
	movs r1, #3
	bl __divsi3
	movs r1, #5
	bl __modsi3
	ldr r6, .L_080cc944
	lsls r0, r0, #10
	movs r7, #0
	mov r8, r0
.L_080cc830:
	ldr r3, .L_080cc948
	ldrb r2, [r3, r7]
	movs r3, #3
	orrs r3, r2
	movs r2, #2
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl Unnamed_080ed408
	ldr r3, .L_080cc94c
	ldrsb r2, [r3, r7]
	ldr r3, .L_080cc950
	mov r5, r11
	ldrsb r1, [r3, r7]
	ldr r3, [r5, #4]
	ldr r4, [r6]
	adds r3, r3, r1
	movs r1, #32
	str r1, [sp, #0]
	str r1, [sp, #4]
	mov r1, r9
	str r4, [sp, #16]
	adds r2, #32
	subs r3, #32
	ldr r0, [sp, #20]
	add r1, r8
	bl _call_via_r4
	adds r7, #1
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	cmp r7, #4
	bne .L_080cc830
.L_080cc878:
	mov r0, r10
	cmp r0, #0
	blt .L_080cc8f0
	movs r5, #225
	lsls r5, r5, #7
	movs r7, #0
	add r5, r9
.L_080cc886:
	ldr r2, [r5, #24]
	cmp r2, #0
	blt .L_080cc8e8
	ldr r3, [r5, #4]
	cmp r3, #0
	ble .L_080cc8e8
	asrs r3, r2, #3
	ldr r0, [r5]
	adds r6, r3, #1
	bl Trig_Sin
	ldr r3, [r5, #4]
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #64
	ldr r0, [r5]
	mov r8, r3
	bl Trig_Cos
	ldr r3, [r5, #4]
	muls r3, r0
	mov r1, r11
	ldr r2, [r1, #4]
	asrs r3, r3, #16
	adds r4, r3, r2
	cmp r6, #0
	bgt .L_080cc8be
	movs r6, #1
.L_080cc8be:
	lsls r0, r6, #1
	ldr r2, .L_080cc954
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #8]
	mov r3, r8
	adds r1, r2, r1
	str r0, [sp, #0]
	subs r2, r3, r6
	str r0, [sp, #4]
	subs r3, r4, r6
	ldr r0, [sp, #20]
	ldr r4, [sp, #12]
	bl _call_via_r4
	ldr r3, [r5, #4]
	subs r3, #2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080cc8e8:
	adds r7, #1
	adds r5, #28
	cmp r7, #64
	bne .L_080cc886
.L_080cc8f0:
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cc958
	movs r3, #1
	add r2, r9
	movs r5, #1
	movs r0, #1
	str r3, [r2]
	add r10, r5
	bl WaitFrames
	mov r0, r10
	cmp r0, #56
	beq .L_080cc90e
	b .L_080cc75e
.L_080cc90e:
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080cc95c
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	movs r0, #41
	bl Runtime_ReleaseHeapBlock
	movs r0, #40
	bl Runtime_ReleaseHeapBlock
	movs r0, #39
	bl Runtime_ReleaseHeapBlock
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080cc944:
	.4byte gTransitionWork + 0xc
.L_080cc948:
	.4byte Data_080ee060
.L_080cc94c:
	.4byte Data_080ee058
.L_080cc950:
	.4byte Data_080ee05c
.L_080cc954:
	.4byte BattleFx6_FlareCells
.L_080cc958:
	.4byte 0x00007824
.L_080cc95c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
