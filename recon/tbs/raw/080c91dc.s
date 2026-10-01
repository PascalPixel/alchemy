.syntax unified
	.thumb
	.global BattleEffect_RunFallingParticles
	.thumb_func
BattleEffect_RunFallingParticles:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080c9250
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #36
	str r3, [sp, #24]
	ldr r3, .L_080c9254
	mov r10, r1
	ldr r2, [r2, #8]
	add r3, r10
	str r2, [sp, #16]
	str r0, [r3]
	ldr r0, .L_080c9258
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080c925c
	ldr r3, .L_080c9244
	ldr r0, .L_080c9260
	strh r3, [r2]
	mov r1, r10
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #0
	movs r3, #0
	ldr r0, .L_080c9264
	ldr r1, [sp, #16]
	bl Resource_LoadAndDecompress
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080c9268
	ldr r3, .L_080c9248
	strh r3, [r2]
	ldr r3, .L_080c924c
	subs r2, #8
	strh r3, [r2]
	movs r6, #225
	movs r3, #128
	movs r2, #0
	negs r3, r3
	movs r7, #16
	lsls r6, r6, #7
	b .L_080c926c
.L_080c9244:
	.4byte 0x00000100
.L_080c9248:
	.4byte 0x00003f44
.L_080c924c:
	.4byte 0x00003337
.L_080c9250:
	.4byte gBattleFxWork
.L_080c9254:
	.4byte 0x00007828
.L_080c9258:
	.4byte 0x00002001
.L_080c925c:
	.4byte 0x04000020
.L_080c9260:
	.4byte 0x000000b3
.L_080c9264:
	.4byte 0x000000ba
.L_080c9268:
	.4byte 0x04000050
.L_080c926c:
	mov r8, r2
	mov r9, r3
	negs r7, r7
	add r6, r10
.L_080c9274:
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r2, #63
	movs r3, #7
	ands r3, r0
	ands r2, r5
	adds r2, r2, r3
	ldr r3, .L_080c9384
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	adds r2, #24
	cmp r3, #1
	bne .L_080c929e
	adds r3, r2, r7
	adds r2, r3, #0
	adds r2, #24
	b .L_080c92a4
.L_080c929e:
	subs r3, r2, r7
	adds r2, r3, #0
	adds r2, #80
.L_080c92a4:
	mov r1, r9
	lsls r3, r2, #3
	str r1, [r6, #4]
	movs r2, #64
	movs r1, #1
	str r3, [r6]
	negs r2, r2
	movs r3, #1
	add r8, r1
	negs r3, r3
	add r9, r2
	mov r2, r8
	str r3, [r6, #24]
	subs r7, #8
	adds r6, #28
	cmp r2, #32
	bne .L_080c9274
	adds r2, r3, #0
	ldr r3, .L_080c9388
	movs r7, #0
	mov r8, r7
	add r3, r10
.L_080c92d0:
	movs r1, #1
	add r8, r1
	mov r7, r8
	str r2, [r3]
	adds r3, #28
	cmp r7, #32
	bne .L_080c92d0
	ldr r3, .L_080c9384
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080c930a
	movs r3, #2
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl Unnamed_080ed408
	movs r3, #3
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #2
	bl Unnamed_080ed408
	b .L_080c932a
.L_080c930a:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #6
	movs r0, #46
	bl Unnamed_080ed408
	movs r3, #3
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #6
	bl Unnamed_080ed408
.L_080c932a:
	ldr r3, .L_080c938c
	adds r2, r3, #0
	adds r2, #184
	ldr r2, [r2]
	str r2, [sp, #28]
	adds r3, #188
	ldr r3, [r3]
	add r1, sp, #28
	str r3, [r1, #4]
	ldr r3, .L_080c9384
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	mov r11, r1
	cmp r3, #0
	bne .L_080c93aa
	movs r2, #0
	mov r8, r2
	movs r1, #224
	ldr r6, .L_080c9378
	ldr r2, .L_080c9390
	ldr r5, .L_080c937c
	ldr r4, .L_080c9380
	ldr r0, .L_080c9394
	lsls r1, r1, #7
.L_080c935c:
	mov r3, r8
	subs r3, #8
	cmp r3, #95
	bhi .L_080c936e
	mov r7, r8
	subs r3, r6, r7
	orrs r3, r1
	strh r3, [r2]
	b .L_080c939a
.L_080c936e:
	mov r3, r8
	cmp r3, #135
	bgt .L_080c9398
	strh r5, [r2]
	b .L_080c939a
.L_080c9378:
	.4byte 0x000000f0
.L_080c937c:
	.4byte 0x00000888
.L_080c9380:
	.4byte 0x00000100
.L_080c9384:
	.4byte 0x00007828
.L_080c9388:
	.4byte 0x00007418
.L_080c938c:
	.4byte gWorkSlot
.L_080c9390:
	.4byte gMapCellBuffer
.L_080c9394:
	.4byte 0xffffff00
.L_080c9398:
	strh r4, [r2]
.L_080c939a:
	movs r7, #1
	add r8, r7
	mov r3, r8
	adds r2, #2
	adds r1, r1, r0
	cmp r3, #160
	bne .L_080c935c
	b .L_080c93f4
.L_080c93aa:
	movs r7, #0
	movs r1, #192
	movs r0, #128
	ldr r5, .L_080c93d8
	ldr r4, .L_080c93dc
	ldr r2, .L_080c93e0
	mov r8, r7
	lsls r1, r1, #5
	lsls r0, r0, #1
.L_080c93bc:
	mov r3, r8
	subs r3, #8
	cmp r3, #87
	bhi .L_080c93cc
	adds r3, #160
	orrs r3, r1
	strh r3, [r2]
	b .L_080c93e6
.L_080c93cc:
	mov r3, r8
	cmp r3, #135
	bgt .L_080c93e4
	strh r5, [r2]
	b .L_080c93e6
	.2byte 0x0000
.L_080c93d8:
	.4byte 0x000078f8
.L_080c93dc:
	.4byte 0x00000100
.L_080c93e0:
	.4byte gMapCellBuffer
.L_080c93e4:
	strh r4, [r2]
.L_080c93e6:
	movs r7, #1
	add r8, r7
	mov r3, r8
	adds r2, #2
	adds r1, r1, r0
	cmp r3, #160
	bne .L_080c93bc
.L_080c93f4:
	movs r1, #144
	ldr r0, .L_080c9704
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #239
	lsls r2, r2, #7
	movs r3, #2
	add r2, r10
	str r3, [r2]
	ldr r3, .L_080c9708
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #24]
	cmp r3, #1
	bne .L_080c941a
	ldr r2, .L_080c970c
	movs r3, #75
	b .L_080c941e
.L_080c941a:
	ldr r2, .L_080c970c
	movs r3, #50
.L_080c941e:
	add r2, r10
	str r3, [r2]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080c9710
	bl Scheduler_AddOrUpdateCallback
	movs r7, #0
	str r7, [sp, #20]
	ldr r2, .L_080c9708
	mov r1, r10
	ldr r3, [r1, r2]
	ldr r3, [r3, #24]
	ldr r6, .L_080c9714
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r6, r3]
	cmp r3, #0
	bne .L_080c9446
	b .L_080c96d0
.L_080c9446:
	mov r3, r10
	adds r5, r3, r2
	ldr r2, [r5]
	ldr r3, [r2, #24]
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r6, r3]
	ldr r7, [sp, #20]
	subs r3, #16
	cmp r7, r3
	bne .L_080c9464
	movs r0, #132
	bl BattleEventRuntime_BeginPhaseFar
	ldr r2, [r5]
.L_080c9464:
	ldr r3, [r2, #24]
	lsls r3, r3, #1
	ldrb r3, [r6, r3]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	bne .L_080c9474
	b .L_080c9612
.L_080c9474:
	ldr r2, .L_080c9708
	movs r7, #225
	add r2, r10
	lsls r7, r7, #7
	mov r9, r2
	add r7, r10
.L_080c9480:
	movs r3, #1
	ldr r0, [r7, #24]
	negs r3, r3
	cmp r0, r3
	bne .L_080c9542
	ldr r2, [r7]
	cmp r2, #0
	bge .L_080c9492
	adds r2, #7
.L_080c9492:
	ldr r3, [r7, #4]
	asrs r2, r2, #3
	cmp r3, #0
	bge .L_080c949c
	adds r3, #7
.L_080c949c:
	mov r1, r9
	asrs r5, r3, #3
	ldr r3, [r1]
	ldr r3, [r3, #24]
	movs r1, #4
	cmp r3, #2
	beq .L_080c94ac
	movs r1, #0
.L_080c94ac:
	movs r3, #32
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r3, r11
	ldr r4, [r1, r3]
	ldr r0, [sp, #24]
	mov r1, r10
	adds r3, r5, #0
	bl _call_via_r4
	ldr r3, [r7, #4]
	ldr r1, .L_080c9718
	cmp r3, r1
	bgt .L_080c94e6
	mov r2, r9
	ldr r3, [r2]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080c94d8
	ldr r3, [r7]
	subs r3, #64
	b .L_080c94dc
.L_080c94d8:
	ldr r3, [r7]
	adds r3, #64
.L_080c94dc:
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #64
	str r3, [r7, #4]
	b .L_080c9538
.L_080c94e6:
	movs r3, #3
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	bne .L_080c94f6
	movs r0, #115
	bl AudioCommand_PlayFar
.L_080c94f6:
	ldr r2, .L_080c971c
	movs r3, #2
	add r2, r10
	str r3, [r2]
	movs r3, #0
	str r3, [r7, #24]
	ldr r3, .L_080c9708
	mov r2, r10
	ldr r3, [r2, r3]
	ldr r3, [r3, #20]
	movs r6, #0
	cmp r3, #0
	beq .L_080c9538
	ldr r5, .L_080c9708
	movs r4, #36
	add r5, r10
.L_080c9516:
	ldr r3, [r5]
	ldrsh r0, [r3, r4]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #9
	adds r3, r6, #0
	movs r2, #5
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldr r4, [sp, #8]
	ldr r3, [r3, #20]
	adds r6, #1
	adds r4, #2
	cmp r6, r3
	bne .L_080c9516
.L_080c9538:
	movs r2, #1
	ldr r0, [r7, #24]
	negs r2, r2
	cmp r0, r2
	beq .L_080c95fa
.L_080c9542:
	ldr r2, [r7]
	cmp r2, #0
	bge .L_080c954a
	adds r2, #7
.L_080c954a:
	ldr r3, [r7, #4]
	asrs r2, r2, #3
	cmp r3, #0
	bge .L_080c9554
	adds r3, #7
.L_080c9554:
	asrs r6, r3, #3
	subs r3, r0, #1
	cmp r3, #13
	bhi .L_080c9594
	mov r1, r9
	ldr r3, [r1]
	ldr r3, [r3, #24]
	movs r5, #4
	cmp r3, #2
	beq .L_080c956a
	movs r5, #0
.L_080c956a:
	movs r1, #3
	str r2, [sp, #12]
	bl __divsi3
	adds r1, r0, #0
	lsls r1, r1, #10
	movs r3, #128
	lsls r3, r3, #3
	add r1, r10
	adds r1, r1, r3
	movs r3, #32
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r5, r11
	ldr r4, [r5]
	ldr r0, [sp, #24]
	ldr r2, [sp, #12]
	adds r3, r6, #0
	bl _call_via_r4
	ldr r0, [r7, #24]
.L_080c9594:
	adds r3, r0, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_080c95f2
	movs r5, #232
	lsls r5, r5, #7
	movs r6, #0
	add r5, r10
.L_080c95a4:
	movs r1, #1
	ldr r3, [r5, #24]
	negs r1, r1
	cmp r3, r1
	bne .L_080c95ea
	movs r3, #18
	str r3, [r5, #24]
	bl Random16
	movs r3, #31
	ands r0, r3
	ldr r3, [r7]
	cmp r3, #0
	bge .L_080c95c2
	adds r3, #7
.L_080c95c2:
	asrs r3, r3, #3
	adds r3, r0, r3
	lsls r3, r3, #3
	adds r3, #8
	str r3, [r5]
	bl Random16
	movs r3, #15
	ands r0, r3
	ldr r3, [r7, #4]
	cmp r3, #0
	bge .L_080c95dc
	adds r3, #7
.L_080c95dc:
	asrs r3, r3, #3
	adds r3, r0, r3
	subs r3, #15
	lsls r3, r3, #3
	str r3, [r5, #4]
	ldr r0, [r7, #24]
	b .L_080c95f2
.L_080c95ea:
	adds r6, #1
	adds r5, #28
	cmp r6, #32
	bne .L_080c95a4
.L_080c95f2:
	cmp r0, #14
	bgt .L_080c95fa
	adds r3, r0, #1
	str r3, [r7, #24]
.L_080c95fa:
	mov r1, r9
	ldr r3, [r1]
	movs r2, #1
	ldr r3, [r3, #24]
	add r8, r2
	ldr r2, .L_080c9714
	lsls r3, r3, #1
	ldrb r3, [r2, r3]
	adds r7, #28
	cmp r8, r3
	beq .L_080c9612
	b .L_080c9480
.L_080c9612:
	movs r5, #232
	movs r2, #0
	lsls r5, r5, #7
	mov r8, r2
	add r5, r10
.L_080c961c:
	movs r3, #1
	ldr r2, [r5, #24]
	negs r3, r3
	cmp r2, r3
	beq .L_080c968c
	cmp r2, #17
	bgt .L_080c9680
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r2, [r5]
	asrs r0, r3, #1
	cmp r2, #0
	bge .L_080c9638
	adds r2, #7
.L_080c9638:
	ldr r6, .L_080c9720
	ldrb r3, [r6, r0]
	asrs r2, r2, #3
	lsrs r1, r3, #1
	ldr r3, [r5, #4]
	subs r2, r2, r1
	mov r12, r2
	cmp r3, #0
	bge .L_080c964c
	adds r3, #7
.L_080c964c:
	asrs r3, r3, #3
	subs r7, r3, r1
	ldr r3, .L_080c9708
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #24]
	movs r4, #4
	cmp r3, #2
	beq .L_080c9660
	movs r4, #0
.L_080c9660:
	ldr r2, .L_080c9724
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldrb r3, [r6, r0]
	ldr r2, [sp, #16]
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r3, r11
	adds r1, r2, r1
	ldr r4, [r4, r3]
	mov r2, r12
	ldr r0, [sp, #24]
	adds r3, r7, #0
	bl _call_via_r4
	ldr r2, [r5, #24]
.L_080c9680:
	movs r7, #1
	negs r7, r7
	cmp r2, r7
	ble .L_080c968c
	subs r3, r2, #1
	str r3, [r5, #24]
.L_080c968c:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #32
	bne .L_080c961c
	bl ObjectGroup_TickMemberTimers
	movs r1, #4
	movs r0, #4
	bl Camera_ApplyShake
	ldr r2, .L_080c9728
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #20]
	ldr r2, .L_080c9708
	adds r3, #1
	str r3, [sp, #20]
	mov r7, r10
	ldr r3, [r7, r2]
	ldr r3, [r3, #24]
	ldr r6, .L_080c9714
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r6, r3]
	ldr r1, [sp, #20]
	cmp r1, r3
	beq .L_080c96d0
	b .L_080c9446
.L_080c96d0:
	ldr r0, .L_080c9710
	bl Scheduler_RemoveCallback
	ldr r0, .L_080c9704
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	bl BattlePres_ConfigureEffectDisplay
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
.L_080c9704:
	.4byte BattleFx_ArmWin0HBlankDma
.L_080c9708:
	.4byte 0x00007828
.L_080c970c:
	.4byte 0x00007784
.L_080c9710:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080c9714:
	.4byte Data_080eded6
.L_080c9718:
	.4byte 0x0000027f
.L_080c971c:
	.4byte 0x000077a8
.L_080c9720:
	.4byte BattleFx_PuffSizes
.L_080c9724:
	.4byte BattleFx_PuffCells
.L_080c9728:
	.4byte 0x00007824
