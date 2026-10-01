.syntax unified
	.thumb
	.global BattleEffectB
	.thumb_func
BattleEffectB:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r1, [sp, #72]
	ldr r3, .L_080d9248
	adds r6, r0, #0
	ldmia r3!, {r0}
	str r0, [sp, #68]
	ldr r1, .L_080d924c
	ldr r3, [r3]
	adds r5, r0, r1
	str r3, [sp, #64]
	movs r0, #0
	str r6, [r5]
	bl BattleFx_BeginCanvasLayer
	ldr r5, [r5]
	ldr r3, [r5, #28]
	cmp r3, #1
	bne .L_080d9272
	ldr r2, [sp, #72]
	cmp r2, #3
	bne .L_080d9226
	ldr r2, [r5, #4]
	eors r2, r3
	add r3, sp, #80
	str r3, [sp, #0]
	add r3, sp, #76
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r1, #0
	b .L_080d9240
.L_080d9226:
	ldr r4, [sp, #72]
	cmp r4, #2
	beq .L_080d9230
	cmp r4, #4
	bne .L_080d9250
.L_080d9230:
	ldr r2, [r5, #4]
	eors r2, r3
	add r3, sp, #80
	str r3, [sp, #0]
	add r3, sp, #76
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r1, #3
.L_080d9240:
	movs r3, #1
	bl BattleFx_PrepareCanvasEffect
	b .L_080d9266
.L_080d9248:
	.4byte gBattleFxWork
.L_080d924c:
	.4byte 0x00007828
.L_080d9250:
	ldr r2, [r5, #4]
	eors r2, r3
	add r3, sp, #80
	str r3, [sp, #0]
	add r3, sp, #76
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r1, #2
	movs r3, #1
	bl BattleFx_PrepareCanvasEffect
.L_080d9266:
	ldr r0, [sp, #80]
	movs r1, #5
	lsls r0, r0, #2
	bl __divsi3
	str r0, [sp, #80]
.L_080d9272:
	ldr r2, .L_080d92b0
	ldr r3, .L_080d92ac
	strh r3, [r2]
	ldr r1, [sp, #68]
	ldr r0, .L_080d92b4
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, .L_080d92b8
	ldr r5, [sp, #68]
	movs r3, #1
	adds r1, r5, r2
	ldr r0, .L_080d92bc
	movs r2, #1
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #72]
	cmp r3, #3
	beq .L_080d929e
	cmp r3, #5
	bne .L_080d92ee
.L_080d929e:
	ldr r4, [sp, #68]
	ldr r5, .L_080d92c0
	ldr r0, .L_080d92c4
	adds r1, r4, r5
	movs r2, #1
	b .L_080d92c8
	.2byte 0x0000
.L_080d92ac:
	.4byte 0x000000cc
.L_080d92b0:
	.4byte 0x04000020
.L_080d92b4:
	.4byte 0x00000076
.L_080d92b8:
	.4byte 0x0000060e
.L_080d92bc:
	.4byte 0x000000b7
.L_080d92c0:
	.4byte 0x00002b8e
.L_080d92c4:
	.4byte 0x000000b0
.L_080d92c8:
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, [sp, #72]
	cmp r0, #3
	bne .L_080d92d8
	ldr r0, .L_080d9650
	b .L_080d92da
.L_080d92d8:
	ldr r0, .L_080d9654
.L_080d92da:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d9658
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	b .L_080d9364
.L_080d92ee:
	ldr r1, [sp, #72]
	cmp r1, #4
	bne .L_080d9306
	ldr r2, [sp, #68]
	ldr r3, .L_080d965c
	ldr r0, .L_080d9660
	adds r1, r2, r3
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_080d9364
.L_080d9306:
	ldr r4, [sp, #72]
	cmp r4, #0
	bne .L_080d931e
	ldr r2, .L_080d965c
	ldr r5, [sp, #68]
	ldr r0, .L_080d9664
	adds r1, r5, r2
	movs r3, #0
	movs r2, #1
	bl Resource_LoadAndDecompress
	b .L_080d932e
.L_080d931e:
	ldr r3, [sp, #68]
	ldr r4, .L_080d965c
	ldr r0, .L_080d9668
	adds r1, r3, r4
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_080d932e:
	ldr r5, [sp, #72]
	cmp r5, #0
	bne .L_080d9338
	ldr r0, .L_080d9654
	b .L_080d9352
.L_080d9338:
	ldr r0, [sp, #72]
	cmp r0, #2
	beq .L_080d9342
	cmp r0, #4
	bne .L_080d9346
.L_080d9342:
	ldr r0, .L_080d966c
	b .L_080d9352
.L_080d9346:
	ldr r1, [sp, #72]
	cmp r1, #1
	bne .L_080d9350
	ldr r0, .L_080d9654
	b .L_080d9352
.L_080d9350:
	ldr r0, .L_080d9670
.L_080d9352:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d9658
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080d9364:
	ldr r2, [sp, #72]
	cmp r2, #3
	bne .L_080d9374
	ldr r3, [sp, #68]
	ldr r4, .L_080d9674
	ldr r0, .L_080d9650
	adds r1, r3, r4
	b .L_080d9386
.L_080d9374:
	ldr r5, [sp, #72]
	cmp r5, #2
	beq .L_080d937e
	cmp r5, #4
	bne .L_080d9390
.L_080d937e:
	ldr r2, [sp, #68]
	ldr r3, .L_080d9674
	ldr r0, .L_080d966c
	adds r1, r2, r3
.L_080d9386:
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_080d93a0
.L_080d9390:
	ldr r4, [sp, #68]
	ldr r5, .L_080d9674
	ldr r0, .L_080d9654
	adds r1, r4, r5
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_080d93a0:
	movs r0, #0
	ldr r5, .L_080d9678
	mov r10, r0
.L_080d93a6:
	bl Random16
	movs r1, #200
	bl __umodsi3
	subs r0, #100
	lsls r0, r0, #14
	str r0, [r5]
	bl Random16
	movs r1, #200
	bl __umodsi3
	subs r0, #100
	lsls r0, r0, #15
	str r0, [r5, #4]
	bl Random16
	movs r1, #200
	bl __umodsi3
	movs r1, #1
	subs r0, #100
	movs r2, #128
	lsls r0, r0, #14
	movs r3, #0
	add r10, r1
	lsls r2, r2, #2
	str r0, [r5, #8]
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r2
	bne .L_080d93a6
	ldr r4, [sp, #68]
	ldr r5, .L_080d967c
	adds r3, r4, r5
	ldr r2, [r3]
	ldr r3, [r2, #20]
	cmp r3, #1
	bne .L_080d9414
	add r5, sp, #120
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r0, [r5]
	negs r0, r0
	lsls r0, r0, #2
	movs r1, #5
	bl __divsi3
	adds r0, #64
	str r0, [sp, #48]
	b .L_080d9424
.L_080d9414:
	ldr r3, [r2, #4]
	movs r2, #64
	negs r2, r2
	str r2, [sp, #48]
	cmp r3, #1
	beq .L_080d9424
	movs r3, #0
	str r3, [sp, #48]
.L_080d9424:
	ldr r4, [sp, #48]
	ldr r2, .L_080d9680
	lsls r3, r4, #8
	str r3, [r2]
	ldr r5, [sp, #68]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, .L_080d9684
	adds r2, r5, r0
	movs r3, #2
	str r3, [r2]
	adds r2, r5, r1
	movs r3, #50
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080d9688
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080d967c
	adds r3, r5, r2
	ldr r3, [r3]
	ldr r3, [r3, #4]
	movs r4, #7
	str r4, [sp, #44]
	cmp r3, #1
	beq .L_080d945e
	movs r5, #3
	str r5, [sp, #44]
.L_080d945e:
	movs r0, #142
	bl AudioCommand_PlayFar
	ldr r2, .L_080d967c
	ldr r1, [sp, #68]
	adds r3, r1, r2
	ldr r3, [r3]
	ldr r3, [r3, #20]
	movs r4, #108
	movs r0, #0
	lsls r3, r3, #3
	negs r4, r4
	mov r11, r0
	cmp r3, r4
	bne .L_080d947e
	b .L_080d9a86
.L_080d947e:
	ldr r5, .L_080d968c
	str r5, [sp, #36]
.L_080d9482:
	ldr r0, [sp, #36]
	ldr r0, [r0]
	mov r1, r11
	str r0, [sp, #40]
	cmp r1, #80
	bne .L_080d9494
	movs r0, #0
	bl BattleEventRuntime_BeginPhaseFar
.L_080d9494:
	ldr r2, [sp, #68]
	ldr r4, .L_080d967c
	adds r3, r2, r4
	ldr r3, [r3]
	ldr r3, [r3, #28]
	cmp r3, #1
	bne .L_080d9554
	mov r0, r11
	lsls r5, r0, #11
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [sp, #80]
	lsls r3, r3, #2
	ldr r1, [sp, #48]
	asrs r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r1
	subs r3, #20
	adds r0, r5, #0
	mov r10, r3
	bl Trig_Cos
	ldr r3, [sp, #76]
	ldr r2, [sp, #44]
	lsls r0, r0, #2
	asrs r0, r0, #16
	movs r5, #4
	eors r5, r2
	adds r0, r0, r3
	movs r3, #2
	str r3, [sp, #0]
	adds r7, r0, #0
	adds r3, r5, #0
	movs r1, #7
	movs r2, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	ldr r3, [sp, #36]
	adds r3, #136
	ldr r3, [r3]
	str r3, [sp, #56]
	mov r8, r3
	movs r3, #3
	str r3, [sp, #0]
	movs r0, #47
	adds r3, r5, #0
	movs r1, #7
	movs r2, #7
	bl BattleEffect_LoadWork
	ldr r3, .L_080d9690
	ldr r4, [r3]
	mov r5, r11
	subs r7, #24
	str r4, [sp, #60]
	cmp r5, #32
	ble .L_080d9516
	lsls r3, r5, #1
	subs r3, r7, r3
	adds r7, r3, #0
	adds r7, #64
.L_080d9516:
	ldr r0, [sp, #68]
	ldr r1, .L_080d9674
	movs r5, #40
	adds r6, r0, r1
	mov r2, r10
	str r4, [sp, #8]
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #64]
	adds r1, r6, #0
	adds r3, r7, #0
	bl _call_via_r8
	mov r2, r11
	ldr r4, [sp, #8]
	cmp r2, #3
	bgt .L_080d9548
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #64]
	adds r1, r6, #0
	mov r2, r10
	adds r3, r7, #0
	bl _call_via_r4
.L_080d9548:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
.L_080d9554:
	movs r3, #0
	str r3, [sp, #52]
	ldr r2, .L_080d967c
	ldr r4, [sp, #68]
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080d9566
	b .L_080d9a5c
.L_080d9566:
	ldr r5, [sp, #40]
	mov r0, sp
	adds r5, #12
	mov r1, sp
	mov r3, sp
	str r5, [sp, #32]
	adds r0, #84
	adds r1, #96
	adds r3, #108
	movs r4, #0
	movs r5, #36
	str r0, [sp, #28]
	str r1, [sp, #24]
	str r3, [sp, #20]
	str r4, [sp, #16]
	str r5, [sp, #12]
	mov r9, r11
.L_080d9588:
	ldr r0, [sp, #68]
	adds r6, r0, r2
	ldr r1, [sp, #12]
	ldr r3, [r6]
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [r6]
	ldr r4, [sp, #12]
	ldr r5, [r0]
	ldrsh r0, [r3, r4]
	bl Battle_GetObjectTableValueFar
	movs r1, #3
	lsls r0, r0, #1
	bl __divsi3
	ldr r3, [sp, #16]
	adds r3, #80
	adds r7, r0, #0
	cmp r11, r3
	bne .L_080d95ba
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080d95ba:
	bl Render_ResetTransformState
	ldr r0, [sp, #40]
	ldr r1, [sp, #32]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	ldr r2, [sp, #28]
	str r3, [r2]
	str r7, [r2, #4]
	ldr r3, [r5, #16]
	str r3, [r2, #8]
	ldr r0, [sp, #28]
	bl SceneTransform_ApplyPosition
	ldr r3, [sp, #16]
	adds r3, #48
	cmp r11, r3
	bne .L_080d95f6
	ldr r3, [r6]
	ldr r4, [sp, #12]
	movs r2, #1
	ldrsh r0, [r3, r4]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	ldr r3, [sp, #52]
	bl ObjectGroup_UpdateMembers
.L_080d95f6:
	ldr r0, [sp, #16]
	cmp r11, r0
	bgt .L_080d95fe
	b .L_080d979c
.L_080d95fe:
	movs r1, #2
	str r1, [sp, #0]
	ldr r3, [sp, #44]
	movs r1, #7
	movs r2, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	ldr r2, .L_080d9694
	ldr r2, [r2]
	movs r3, #3
	str r2, [sp, #56]
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	ldr r3, [sp, #44]
	bl BattleEffect_LoadWork
	ldr r4, .L_080d9690
	ldr r5, [sp, #72]
	ldr r4, [r4]
	str r4, [sp, #60]
	cmp r5, #0
	bne .L_080d963c
	mov r1, r11
	negs r0, r1
	lsls r0, r0, #10
	bl SceneTransform_ApplyPitch
	b .L_080d96a8
.L_080d963c:
	ldr r2, [sp, #72]
	cmp r2, #1
	beq .L_080d96a8
	cmp r2, #2
	bne .L_080d9698
	mov r3, r11
	lsls r0, r3, #10
	bl SceneTransform_ApplyYaw
	b .L_080d96a8
.L_080d9650:
	.4byte 0x00000093
.L_080d9654:
	.4byte 0x0000008d
.L_080d9658:
	.4byte IwramCopyWords
.L_080d965c:
	.4byte 0x00002b8e
.L_080d9660:
	.4byte 0x000000a5
.L_080d9664:
	.4byte 0x0000009c
.L_080d9668:
	.4byte 0x0000009b
.L_080d966c:
	.4byte 0x0000008f
.L_080d9670:
	.4byte 0x000000bb
.L_080d9674:
	.4byte 0x000065c0
.L_080d9678:
	.4byte gMapCellBuffer
.L_080d967c:
	.4byte 0x00007828
.L_080d9680:
	.4byte 0x04000028
.L_080d9684:
	.4byte 0x00007784
.L_080d9688:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d968c:
	.4byte gCameraWork
.L_080d9690:
	.4byte gTransitionWork + 0xc
.L_080d9694:
	.4byte gTransitionWork + 0x8
.L_080d9698:
	mov r4, r11
	lsls r5, r4, #10
	adds r0, r5, #0
	bl SceneTransform_ApplyYaw
	adds r0, r5, #0
	bl SceneTransform_ApplyRoll
.L_080d96a8:
	ldr r0, [sp, #52]
	lsls r2, r0, #6
	lsls r3, r0, #9
	subs r3, r3, r2
	ldr r1, [sp, #24]
	ldr r2, .L_080d99a8
	movs r5, #0
	lsls r3, r3, #2
	mov r10, r5
	mov r8, r1
	adds r6, r3, r2
.L_080d96be:
	ldr r3, [sp, #16]
	add r3, r10
	cmp r11, r3
	ble .L_080d9784
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	asrs r3, r3, #8
	adds r4, r3, #0
	muls r4, r3
	adds r0, r0, r2
	adds r3, r4, #0
	adds r0, r0, r3
	ldr r3, .L_080d99ac
	bl _call_via_r3
	asrs r7, r0, #9
	cmp r7, #0
	beq .L_080d9784
	mov r5, r8
	mov r1, r8
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r0, [r5]
	movs r1, #5
	lsls r0, r0, #2
	bl __divsi3
	ldr r1, [sp, #48]
	ldr r2, [r5, #8]
	ldr r3, .L_080d99b0
	adds r0, r0, r1
	str r0, [r5]
	cmp r2, r3
	bgt .L_080d971a
	ldr r4, [sp, #24]
	adds r3, #1
	str r3, [r4, #8]
	adds r2, r3, #0
.L_080d971a:
	ldr r3, .L_080d99b4
	cmp r2, r3
	ble .L_080d9726
	ldr r5, [sp, #24]
	str r3, [r5, #8]
	adds r2, r3, #0
.L_080d9726:
	ldr r0, .L_080d99b8
	adds r3, r2, r0
	cmp r3, #0
	bge .L_080d9732
	adds r3, r2, #0
	subs r3, #251
.L_080d9732:
	asrs r3, r3, #6
	movs r0, #6
	subs r0, r0, r3
	lsls r4, r0, #1
	ldr r2, .L_080d99bc
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #68]
	ldr r3, [sp, #24]
	adds r1, r2, r1
	ldr r2, [r3]
	ldr r3, [r3, #4]
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #64]
	ldr r4, [sp, #60]
	bl _call_via_r4
	ldr r5, [r6]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #8]
.L_080d9784:
	movs r5, #1
	add r10, r5
	mov r0, r10
	adds r6, #28
	cmp r0, #32
	bne .L_080d96be
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
.L_080d979c:
	ldr r1, [sp, #20]
	movs r3, #0
	str r3, [r1]
	str r3, [r1, #4]
	str r3, [r1, #8]
	add r2, sp, #96
	adds r1, r2, #0
	ldr r0, [sp, #20]
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r0, [sp, #96]
	movs r1, #5
	lsls r0, r0, #2
	bl __divsi3
	ldr r3, [sp, #48]
	adds r0, r0, r3
	ldr r3, [sp, #16]
	adds r3, #52
	str r0, [sp, #96]
	cmp r11, r3
	blt .L_080d9828
	ldr r3, [sp, #16]
	adds r3, #76
	cmp r11, r3
	bge .L_080d9828
	mov r3, r9
	subs r3, #52
	cmp r3, #0
	bge .L_080d97da
	adds r3, #3
.L_080d97da:
	asrs r0, r3, #2
	movs r1, #6
	bl __modsi3
	movs r4, #2
	adds r5, r0, #0
	ldr r3, [sp, #44]
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r4, [sp, #0]
	bl BattleEffect_LoadWork
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r2, [sp, #68]
	adds r1, r1, r5
	ldr r0, .L_080d99c0
	ldr r5, [sp, #24]
	ldr r3, .L_080d99c4
	lsls r1, r1, #6
	adds r1, r2, r1
	ldr r4, [r0]
	ldr r2, [r5]
	adds r1, r1, r3
	ldr r3, [r5, #4]
	movs r0, #40
	str r0, [sp, #0]
	str r0, [sp, #4]
	str r4, [sp, #56]
	subs r2, #20
	subs r3, #20
	ldr r0, [sp, #64]
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
.L_080d9828:
	ldr r0, [sp, #72]
	cmp r0, #0
	bne .L_080d9892
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r11, r3
	bge .L_080d9838
	b .L_080d9a36
.L_080d9838:
	ldr r3, [sp, #16]
	adds r3, #108
	cmp r11, r3
	blt .L_080d9842
	b .L_080d9a36
.L_080d9842:
	mov r3, r9
	subs r3, #80
	cmp r3, #0
	bge .L_080d984c
	adds r3, #3
.L_080d984c:
	asrs r0, r3, #2
	movs r1, #7
	bl __modsi3
	movs r1, #2
	adds r5, r0, #0
	str r1, [sp, #0]
	ldr r3, [sp, #44]
	movs r1, #7
	movs r2, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	lsls r1, r5, #4
	ldr r2, .L_080d99c0
	ldr r3, [sp, #68]
	subs r1, r1, r5
	lsls r1, r1, #6
	ldr r4, [r2]
	adds r1, r3, r1
	ldr r2, [sp, #96]
	ldr r3, [sp, #100]
	movs r0, #24
	ldr r5, .L_080d99c8
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	str r4, [sp, #56]
	adds r1, r1, r5
	subs r2, #12
	subs r3, #20
	ldr r0, [sp, #64]
	bl _call_via_r4
	b .L_080d999e
.L_080d9892:
	ldr r0, [sp, #72]
	cmp r0, #3
	beq .L_080d989c
	cmp r0, #5
	bne .L_080d98fe
.L_080d989c:
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r11, r3
	bge .L_080d98a6
	b .L_080d9a36
.L_080d98a6:
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r11, r3
	blt .L_080d98b0
	b .L_080d9a36
.L_080d98b0:
	mov r3, r9
	subs r3, #80
	cmp r3, #0
	bge .L_080d98ba
	adds r3, #3
.L_080d98ba:
	asrs r0, r3, #2
	movs r1, #6
	bl __modsi3
	movs r1, #2
	str r1, [sp, #0]
	ldr r3, [sp, #44]
	adds r5, r0, #0
	movs r1, #7
	movs r2, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	ldr r2, .L_080d99c0
	ldr r3, [sp, #68]
	ldr r0, .L_080d99c8
	lsls r5, r5, #11
	ldr r4, [r2]
	adds r5, r3, r5
	ldr r2, [sp, #96]
	ldr r3, [sp, #100]
	movs r1, #32
	adds r5, r5, r0
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	str r4, [sp, #56]
	subs r2, #16
	subs r3, #32
	ldr r0, [sp, #64]
	adds r1, r5, #0
	bl _call_via_r4
	b .L_080d999e
.L_080d98fe:
	ldr r1, [sp, #72]
	cmp r1, #4
	bne .L_080d99d0
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r11, r3
	bge .L_080d990e
	b .L_080d9a36
.L_080d990e:
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r11, r3
	blt .L_080d9918
	b .L_080d9a36
.L_080d9918:
	mov r0, r9
	subs r0, #80
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r1, #6
	asrs r0, r0, #1
	bl __modsi3
	movs r2, #2
	str r2, [sp, #0]
	ldr r3, [sp, #44]
	adds r5, r0, #0
	movs r1, #7
	movs r2, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	ldr r3, .L_080d99c0
	movs r4, #2
	ldr r3, [r3]
	str r4, [sp, #0]
	ldr r4, [sp, #44]
	str r3, [sp, #56]
	mov r8, r3
	movs r3, #8
	orrs r3, r4
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl BattleEffect_LoadWork
	ldr r1, [sp, #68]
	ldr r0, .L_080d99cc
	ldr r2, .L_080d99c8
	lsls r5, r5, #11
	adds r5, r1, r5
	ldr r0, [r0]
	ldr r3, [sp, #100]
	adds r5, r5, r2
	ldr r2, [sp, #96]
	movs r4, #64
	movs r6, #32
	str r0, [sp, #60]
	str r4, [sp, #0]
	str r4, [sp, #8]
	str r6, [sp, #4]
	mov r10, r0
	subs r2, #32
	ldr r0, [sp, #64]
	subs r3, #24
	adds r1, r5, #0
	bl _call_via_r8
	ldr r2, [sp, #96]
	ldr r3, [sp, #100]
	ldr r4, [sp, #8]
	subs r2, #32
	adds r3, #8
	str r4, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #64]
	adds r1, r5, #0
	bl _call_via_sl
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080d999e:
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	b .L_080d9a36
	.2byte 0x0000
.L_080d99a8:
	.4byte gMapCellBuffer
.L_080d99ac:
	.4byte IwramSqrt
.L_080d99b0:
	.4byte 0x00000139
.L_080d99b4:
	.4byte 0x0000027a
.L_080d99b8:
	.4byte 0xfffffec6
.L_080d99bc:
	.4byte BattleFx6_FlareCells
.L_080d99c0:
	.4byte gTransitionWork + 0x8
.L_080d99c4:
	.4byte 0x0000060e
.L_080d99c8:
	.4byte 0x00002b8e
.L_080d99cc:
	.4byte gTransitionWork + 0xc
.L_080d99d0:
	ldr r3, [sp, #16]
	adds r3, #80
	cmp r11, r3
	blt .L_080d9a36
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r11, r3
	bge .L_080d9a36
	mov r3, r9
	subs r3, #80
	cmp r3, #0
	bge .L_080d99ea
	adds r3, #3
.L_080d99ea:
	asrs r0, r3, #2
	movs r1, #6
	bl __modsi3
	movs r3, #3
	adds r5, r0, #0
	str r3, [sp, #0]
	movs r1, #7
	ldr r3, [sp, #44]
	movs r2, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r2, [sp, #68]
	adds r1, r1, r5
	ldr r0, .L_080d9aa4
	ldr r3, .L_080d9aa8
	lsls r1, r1, #6
	adds r1, r2, r1
	ldr r4, [r0]
	adds r1, r1, r3
	ldr r2, [sp, #96]
	ldr r3, [sp, #100]
	movs r0, #40
	str r0, [sp, #0]
	str r0, [sp, #4]
	str r4, [sp, #56]
	subs r2, #20
	subs r3, #20
	ldr r0, [sp, #64]
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
.L_080d9a36:
	ldr r5, [sp, #16]
	ldr r0, [sp, #12]
	ldr r1, [sp, #52]
	movs r4, #8
	negs r4, r4
	adds r5, #8
	adds r0, #2
	adds r1, #1
	str r5, [sp, #16]
	str r0, [sp, #12]
	str r1, [sp, #52]
	add r9, r4
	ldr r2, .L_080d9aac
	ldr r4, [sp, #68]
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r1, r3
	beq .L_080d9a5c
	b .L_080d9588
.L_080d9a5c:
	bl ObjectGroup_TickMemberTimers
	ldr r0, .L_080d9ab0
	ldr r5, [sp, #68]
	movs r3, #1
	adds r2, r5, r0
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080d9aac
	adds r3, r5, r2
	ldr r3, [r3]
	ldr r3, [r3, #20]
	movs r1, #1
	lsls r3, r3, #3
	add r11, r1
	adds r3, #108
	cmp r11, r3
	beq .L_080d9a86
	b .L_080d9482
.L_080d9a86:
	ldr r0, .L_080d9ab4
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d9aa4:
	.4byte gTransitionWork + 0x8
.L_080d9aa8:
	.4byte 0x00002b8e
.L_080d9aac:
	.4byte 0x00007828
.L_080d9ab0:
	.4byte 0x00007824
.L_080d9ab4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
