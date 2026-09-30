.syntax unified
	.thumb
	.global Func_080c972c
	.thumb_func
Func_080c972c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080c979c
	adds r3, r5, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #32
	str r3, [sp, #28]
	ldr r3, .L_080c97a0
	mov r10, r1
	add r3, r10
	str r0, [r3]
	ldr r0, .L_080c97a4
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080c97a8
	ldr r3, .L_080c9790
	ldr r1, .L_080c97ac
	strh r3, [r2]
	ldr r0, .L_080c97b0
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_080c97b4
	mov r1, r10
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080c97b8
	ldr r3, .L_080c9794
	strh r3, [r2]
	ldr r3, .L_080c9798
	subs r2, #8
	strh r3, [r2]
	movs r1, #7
	movs r3, #2
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	b .L_080c97bc
.L_080c9790:
	.4byte 0x00000100
.L_080c9794:
	.4byte 0x00003f44
.L_080c9798:
	.4byte 0x00003337
.L_080c979c:
	.4byte gBattleFxWork
.L_080c97a0:
	.4byte 0x00007828
.L_080c97a4:
	.4byte 0x00002001
.L_080c97a8:
	.4byte 0x04000020
.L_080c97ac:
	.4byte 0x00000604
.L_080c97b0:
	.4byte 0x000000cc
.L_080c97b4:
	.4byte 0x00000076
.L_080c97b8:
	.4byte 0x04000050
.L_080c97bc:
	bl Unnamed_080ed408
	ldr r2, [r5, #28]
	movs r3, #3
	str r2, [sp, #16]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #2
	movs r0, #47
	bl Unnamed_080ed408
	ldr r5, [r5, #32]
	movs r3, #0
	mov r8, r3
	movs r1, #1
	movs r2, #128
	str r5, [sp, #20]
	ldr r3, .L_080c9898
	negs r1, r1
	lsls r2, r2, #2
.L_080c97e6:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080c97e6
	movs r5, #225
	movs r7, #0
	lsls r5, r5, #7
	ldr r6, .L_080c989c
	mov r8, r7
	add r5, r10
.L_080c97fe:
	bl Random16
	movs r3, #63
	ands r0, r3
	ldr r3, .L_080c98a0
	add r3, r10
	ldr r2, [r3]
	ldr r3, [r2, #24]
	lsls r3, r3, #2
	adds r3, #2
	ldrb r3, [r6, r3]
	mov r1, r8
	muls r1, r3
	adds r3, r1, #0
	adds r3, #16
	negs r1, r3
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080c9832
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	adds r3, r0, r3
	adds r0, r3, #0
	subs r0, #48
	b .L_080c983e
.L_080c9832:
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	subs r3, r0, r3
	adds r0, r3, #0
	adds r0, #72
.L_080c983e:
	lsls r3, r0, #3
	str r3, [r5]
	lsls r3, r1, #3
	str r3, [r5, #4]
	movs r2, #1
	movs r3, #1
	negs r3, r3
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #64
	bne .L_080c97fe
	ldr r3, .L_080c98a0
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080c98c8
	movs r4, #0
	mov r8, r4
	ldr r5, .L_080c988c
	ldr r1, .L_080c98a4
	ldr r4, .L_080c9890
	ldr r0, .L_080c9894
.L_080c9870:
	mov r3, r8
	subs r3, #8
	cmp r3, #95
	bhi .L_080c98a8
	mov r7, r8
	lsrs r3, r7, #31
	add r3, r8
	asrs r3, r3, #1
	subs r2, r5, r3
	lsls r2, r2, #8
	subs r3, r4, r3
	orrs r2, r3
	strh r2, [r1]
	b .L_080c98b6
.L_080c988c:
	.4byte 0x00000034
.L_080c9890:
	.4byte 0x000000b4
.L_080c9894:
	.4byte 0x00000080
.L_080c9898:
	.4byte gMapCellBuffer + 0x158
.L_080c989c:
	.4byte Data_080ededc
.L_080c98a0:
	.4byte 0x00007828
.L_080c98a4:
	.4byte gMapCellBuffer
.L_080c98a8:
	mov r2, r8
	cmp r2, #135
	bgt .L_080c98b2
	strh r0, [r1]
	b .L_080c98b6
.L_080c98b2:
	ldr r3, .L_080c98c4
	strh r3, [r1]
.L_080c98b6:
	movs r3, #1
	add r8, r3
	mov r7, r8
	adds r1, #2
	cmp r7, #160
	bne .L_080c9870
	b .L_080c9916
.L_080c98c4:
	.4byte 0x00000100
.L_080c98c8:
	movs r0, #0
	mov r8, r0
	ldr r4, .L_080c98fc
	ldr r0, .L_080c9900
	ldr r1, .L_080c9904
.L_080c98d2:
	mov r3, r8
	subs r3, #8
	cmp r3, #95
	bhi .L_080c98f0
	mov r2, r8
	lsrs r3, r2, #31
	add r3, r8
	asrs r3, r3, #1
	adds r2, r3, #0
	adds r2, #60
	lsls r2, r2, #8
	adds r3, #188
	orrs r2, r3
	strh r2, [r1]
	b .L_080c990a
.L_080c98f0:
	mov r3, r8
	cmp r3, #135
	bgt .L_080c9908
	strh r4, [r1]
	b .L_080c990a
	.2byte 0x0000
.L_080c98fc:
	.4byte 0x000070f0
.L_080c9900:
	.4byte 0x00000100
.L_080c9904:
	.4byte gMapCellBuffer
.L_080c9908:
	strh r0, [r1]
.L_080c990a:
	movs r7, #1
	add r8, r7
	mov r2, r8
	adds r1, #2
	cmp r2, #160
	bne .L_080c98d2
.L_080c9916:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080c9c28
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_080c9c2c
	add r3, r10
	ldr r3, [r3]
	ldr r1, [r3, #24]
	cmp r1, #0
	bne .L_080c993e
	movs r3, #239
	lsls r3, r3, #7
	add r3, r10
	movs r2, #1
	str r2, [r3]
	ldr r3, .L_080c9c30
	add r3, r10
	str r1, [r3]
	b .L_080c9964
.L_080c993e:
	cmp r1, #1
	bne .L_080c9952
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080c9c30
	movs r3, #50
	b .L_080c9960
.L_080c9952:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080c9c30
	movs r3, #75
.L_080c9960:
	add r2, r10
	str r3, [r2]
.L_080c9964:
	movs r1, #144
	ldr r0, .L_080c9c34
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r3, #0
	str r3, [sp, #24]
	ldr r2, .L_080c9c2c
	mov r4, r10
	ldr r3, [r4, r2]
	ldr r3, [r3, #24]
	ldr r6, .L_080c9c38
	lsls r3, r3, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	cmp r3, #0
	bne .L_080c9988
	b .L_080c9bf4
.L_080c9988:
	mov r7, r10
	adds r5, r7, r2
	ldr r2, [r5]
	ldr r3, [r2, #24]
	lsls r3, r3, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	ldr r0, [sp, #24]
	subs r3, #64
	cmp r0, r3
	bne .L_080c99a6
	movs r0, #132
	bl BattleEventRuntime_BeginPhaseFar
	ldr r2, [r5]
.L_080c99a6:
	ldr r3, [r2, #24]
	lsls r3, r3, #2
	ldrb r3, [r6, r3]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	bne .L_080c99b6
	b .L_080c9b6c
.L_080c99b6:
	movs r7, #225
	lsls r7, r7, #7
	add r7, r10
.L_080c99bc:
	ldr r2, [r7]
	cmp r2, #0
	bge .L_080c99c4
	adds r2, #7
.L_080c99c4:
	ldr r3, [r7, #4]
	asrs r6, r2, #3
	cmp r3, #0
	bge .L_080c99ce
	adds r3, #7
.L_080c99ce:
	asrs r5, r3, #3
	movs r4, #1
	ldr r3, [r7, #24]
	negs r4, r4
	adds r2, r3, #0
	cmp r3, r4
	beq .L_080c99de
	b .L_080c9b14
.L_080c99de:
	ldr r1, .L_080c9c3c
	movs r3, #24
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #28]
	adds r3, r5, #0
	add r1, r10
	adds r2, r6, #0
	ldr r4, [sp, #16]
	bl _call_via_r4
	ldr r3, [r7, #4]
	ldr r0, .L_080c9c40
	cmp r3, r0
	bgt .L_080c9a1c
	ldr r3, .L_080c9c2c
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080c9a0e
	ldr r3, [r7]
	subs r3, #32
	b .L_080c9a12
.L_080c9a0e:
	ldr r3, [r7]
	adds r3, #32
.L_080c9a12:
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #64
	str r3, [r7, #4]
	b .L_080c9b52
.L_080c9a1c:
	ldr r1, .L_080c9c2c
	movs r3, #0
	mov r0, r10
	str r3, [r7, #24]
	ldr r3, [r0, r1]
	ldr r3, [r3, #24]
	ldr r2, .L_080c9c38
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r2, r3]
	movs r4, #0
	cmp r3, #0
	beq .L_080c9ace
	adds r6, #12
	lsls r5, r5, #16
	movs r3, #255
	str r6, [sp, #12]
	mov r11, r5
	mov r9, r3
.L_080c9a42:
	mov r0, r10
	adds r5, r0, r1
	ldr r3, [r5]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r2, r3]
	mov r2, r8
	muls r2, r3
	adds r2, r2, r4
	lsls r3, r2, #3
	ldr r1, .L_080c9c44
	subs r3, r3, r2
	ldr r2, [sp, #12]
	lsls r3, r3, #2
	adds r6, r3, r1
	lsls r3, r2, #16
	str r3, [r6]
	mov r3, r11
	str r3, [r6, #4]
	str r4, [sp, #8]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #128
	lsls r0, r0, #9
	str r0, [r6, #12]
	ldr r3, [r5]
	ldr r3, [r3, #24]
	ldr r4, [sp, #8]
	cmp r3, #2
	bne .L_080c9a96
	bl Random16
	ldr r3, .L_080c9c48
	ldr r2, .L_080c9c4c
	ands r3, r0
	adds r3, r3, r2
	lsls r3, r3, #10
	str r3, [r6, #16]
	b .L_080c9aa6
.L_080c9a96:
	str r4, [sp, #8]
	bl Random16
	mov r3, r9
	ands r0, r3
	subs r0, #255
	lsls r0, r0, #10
	str r0, [r6, #16]
.L_080c9aa6:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl Random16
	movs r3, #15
	ands r3, r0
	ldr r1, .L_080c9c2c
	adds r3, #16
	mov r0, r10
	str r3, [r6, #24]
	ldr r3, [r0, r1]
	ldr r3, [r3, #24]
	ldr r2, .L_080c9c38
	lsls r3, r3, #2
	ldr r4, [sp, #8]
	adds r3, #1
	ldrb r3, [r2, r3]
	adds r4, #1
	cmp r4, r3
	bne .L_080c9a42
.L_080c9ace:
	movs r3, #3
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	bne .L_080c9ade
	movs r0, #132
	bl AudioCommand_PlayFar
.L_080c9ade:
	ldr r2, .L_080c9c2c
	add r2, r10
	ldr r3, [r2]
	ldr r3, [r3, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_080c9b52
	adds r5, r2, #0
	movs r6, #36
.L_080c9af0:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldr r4, [sp, #8]
	ldr r3, [r3, #20]
	adds r4, #1
	adds r6, #2
	cmp r4, r3
	bne .L_080c9af0
	b .L_080c9b52
.L_080c9b14:
	cmp r2, #3
	bhi .L_080c9b2a
	ldr r1, .L_080c9c50
	movs r3, #24
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #28]
	adds r3, r5, #0
	add r1, r10
	adds r2, r6, #0
	b .L_080c9b42
.L_080c9b2a:
	cmp r2, #7
	bgt .L_080c9b4a
	movs r1, #42
	str r1, [sp, #0]
	str r1, [sp, #4]
	ldr r1, .L_080c9c54
	adds r2, r6, #0
	adds r3, r5, #0
	subs r3, #9
	subs r2, #9
	ldr r0, [sp, #28]
	add r1, r10
.L_080c9b42:
	ldr r4, [sp, #16]
	bl _call_via_r4
	ldr r3, [r7, #24]
.L_080c9b4a:
	cmp r3, #14
	bgt .L_080c9b52
	adds r3, #1
	str r3, [r7, #24]
.L_080c9b52:
	ldr r3, .L_080c9c2c
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #24]
	ldr r1, .L_080c9c38
	lsls r3, r3, #2
	movs r0, #1
	ldrb r3, [r1, r3]
	add r8, r0
	adds r7, #28
	cmp r8, r3
	beq .L_080c9b6c
	b .L_080c99bc
.L_080c9b6c:
	movs r2, #0
	ldr r6, .L_080c9c58
	ldr r5, .L_080c9c44
	mov r8, r2
.L_080c9b74:
	movs r3, #1
	ldr r0, [r5, #24]
	negs r3, r3
	cmp r0, r3
	beq .L_080c9bb8
	adds r4, r0, #1
	cmp r4, #6
	ble .L_080c9b86
	movs r4, #6
.L_080c9b86:
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r6, r3]
	movs r7, #2
	ldrsh r2, [r5, r7]
	movs r7, #6
	ldrsh r3, [r5, r7]
	subs r2, r2, r4
	subs r3, r3, r4
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r10
	ldr r0, [sp, #28]
	ldr r4, [sp, #20]
	bl _call_via_r4
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #6
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080c9bb8:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r8, r0
	bne .L_080c9b74
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080c9c5c
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #24]
	adds r1, #1
	str r1, [sp, #24]
	ldr r2, .L_080c9c2c
	mov r4, r10
	ldr r3, [r4, r2]
	ldr r3, [r3, #24]
	ldr r6, .L_080c9c38
	lsls r3, r3, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	cmp r1, r3
	beq .L_080c9bf4
	b .L_080c9988
.L_080c9bf4:
	ldr r0, .L_080c9c28
	bl Scheduler_RemoveCallback
	ldr r0, .L_080c9c34
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	bl BattlePres_ConfigureEffectDisplay
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080c9c28:
	.4byte BattleFx_ArmWin0HBlankDma
.L_080c9c2c:
	.4byte 0x00007828
.L_080c9c30:
	.4byte 0x00007784
.L_080c9c34:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080c9c38:
	.4byte Data_080ededc
.L_080c9c3c:
	.4byte 0x00000604
.L_080c9c40:
	.4byte 0x0000027f
.L_080c9c44:
	.4byte gMapCellBuffer + 0x140
.L_080c9c48:
	.4byte 0x000001ff
.L_080c9c4c:
	.4byte 0xfffffe80
.L_080c9c50:
	.4byte 0x00000844
.L_080c9c54:
	.4byte 0x00000a84
.L_080c9c58:
	.4byte BattleFx6_FlareCells
.L_080c9c5c:
	.4byte 0x00007824
