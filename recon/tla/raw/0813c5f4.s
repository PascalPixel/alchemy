.syntax unified
	.thumb
	.global Func_0813c5f4
	.thumb_func
Func_0813c5f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r0, [sp, #72]
	str r1, [sp, #68]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	str r0, [sp, #64]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #60]
	ldr r2, [r5, #48]
	str r2, [sp, #44]
	bl BattleFx_BeginCanvasLayer
	ldr r4, [sp, #72]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0813c638
	movs r1, #27
	movs r0, #104
	bl Func_081963ec
	ldr r0, [r5, #104]
	movs r1, #19
	str r0, [sp, #48]
	movs r0, #188
	b .L_0813c648
.L_0813c638:
	movs r1, #31
	movs r0, #104
	bl Func_081963ec
	ldr r1, [r5, #104]
	movs r0, #188
	str r1, [sp, #48]
	movs r1, #23
.L_0813c648:
	bl Func_081963ec
	adds r3, r5, #0
	adds r3, #188
	ldr r3, [r3]
	str r3, [sp, #52]
	ldr r2, [sp, #64]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0813c698
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #68]
	cmp r4, #4
	bhi .L_0813c6b0
	ldr r3, .L_0813c69c
	lsls r0, r4, #2
	ldr r3, [r0, r3]
	mov pc, r3
.L_0813c674:
	.4byte .L_0813c688
	.4byte .L_0813c68c
	.4byte .L_0813c690
	.4byte .L_0813c694
	.4byte .L_0813c6b0
.L_0813c688:
	ldr r0, .L_0813c6a0
	b .L_0813c6b2
.L_0813c68c:
	ldr r0, .L_0813c6a4
	b .L_0813c6b2
.L_0813c690:
	ldr r0, .L_0813c6a8
	b .L_0813c6b2
.L_0813c694:
	ldr r0, .L_0813c6ac
	b .L_0813c6b2
.L_0813c698:
	.4byte 0x0000011a
.L_0813c69c:
	.4byte .L_0813c674
.L_0813c6a0:
	.4byte 0x00000178
.L_0813c6a4:
	.4byte 0x00000163
.L_0813c6a8:
	.4byte 0x0000018f
.L_0813c6ac:
	.4byte 0x00000147
.L_0813c6b0:
	ldr r0, .L_0813c82c
.L_0813c6b2:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0813c830
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #64]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0813c834
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #72]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r2, [sp, #72]
	movs r3, #3
	ldr r1, [r2, #20]
	ldr r0, [r0]
	adds r2, r1, #0
	muls r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #48
	movs r4, #0
	mov r10, r0
	str r3, [sp, #40]
	str r4, [sp, #56]
	cmp r1, #0
	beq .L_0813c79e
	movs r0, #36
	str r0, [sp, #12]
	mov r9, r4
.L_0813c714:
	ldr r1, [sp, #12]
	ldr r3, [sp, #72]
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r1, [sp, #64]
	mov r8, r0
	mov r0, r9
	movs r4, #0
	lsls r3, r0, #2
	mov r11, r4
	adds r7, r3, r1
.L_0813c72e:
	mov r2, r10
	ldr r3, [r2, #8]
	movs r4, #160
	str r3, [r7]
	lsls r4, r4, #13
	ldr r5, [r2, #12]
	mov r1, r8
	adds r5, r5, r4
	str r5, [r7, #4]
	ldr r6, [r2, #16]
	str r6, [r7, #8]
	ldr r0, [r1, #8]
	movs r1, #24
	subs r0, r0, r3
	bl Math_Div
	str r0, [r7, #12]
	mov r2, r8
	ldr r0, [r2, #12]
	movs r3, #160
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r1, #24
	subs r0, r0, r5
	bl Math_Div
	str r0, [r7, #16]
	mov r4, r8
	ldr r0, [r4, #16]
	movs r1, #24
	subs r0, r0, r6
	bl Math_Div
	str r0, [r7, #20]
	movs r0, #1
	add r11, r0
	movs r3, #0
	mov r1, r11
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #3
	bne .L_0813c72e
	movs r2, #3
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r4, [sp, #56]
	add r9, r3
	ldr r3, [sp, #12]
	adds r4, #1
	adds r3, #2
	str r3, [sp, #12]
	str r4, [sp, #56]
	ldr r0, [sp, #72]
	ldr r3, [r0, #20]
	cmp r4, r3
	bne .L_0813c714
.L_0813c79e:
	ldr r2, [sp, #40]
	movs r1, #0
	mov r10, r1
	cmp r2, #0
	bne .L_0813c7aa
	b .L_0813cadc
.L_0813c7aa:
	ldr r3, [sp, #44]
	subs r2, #16
	adds r3, #12
	str r2, [sp, #28]
	str r3, [sp, #32]
.L_0813c7b4:
	ldr r4, [sp, #64]
	movs r1, #0
	movs r0, #225
	str r1, [sp, #56]
	lsls r0, r0, #7
	movs r7, #128
	mov r2, r10
	adds r6, r4, r0
	lsls r7, r7, #12
	lsls r5, r2, #12
.L_0813c7c8:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #1
	subs r0, r7, r0
	asrs r0, r0, #10
	stmia r6!, {r0}
	ldr r4, [sp, #56]
	movs r3, #128
	lsls r3, r3, #5
	adds r4, #1
	adds r5, r5, r3
	str r4, [sp, #56]
	cmp r4, #160
	bne .L_0813c7c8
	ldr r0, [sp, #28]
	cmp r10, r0
	ble .L_0813c7fe
	ldr r1, [sp, #40]
	mov r4, r10
	subs r2, r1, r4
	ldr r1, .L_0813c828
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_0813c7fe:
	bl Func_08014de4
	ldr r0, [sp, #44]
	ldr r1, [sp, #32]
	bl Graphics_PrepareTransferInIwramWork
	movs r0, #0
	str r0, [sp, #56]
	ldr r1, [sp, #72]
	ldr r3, [r1, #20]
	cmp r3, #0
	bne .L_0813c818
	b .L_0813cab0
.L_0813c818:
	mov r3, r10
	movs r2, #36
	subs r3, #30
	str r2, [sp, #24]
	str r3, [sp, #20]
	str r0, [sp, #16]
	str r0, [sp, #8]
	b .L_0813c838
.L_0813c828:
	.4byte 0x00001000
.L_0813c82c:
	.4byte 0x00000166
.L_0813c830:
	.4byte IwramCopyWords
.L_0813c834:
	.4byte Func_08143000
.L_0813c838:
	ldr r4, [sp, #16]
	cmp r10, r4
	bne .L_0813c86e
	ldr r0, [sp, #68]
	cmp r0, #1
	bne .L_0813c84a
	movs r0, #192
	bl Audio_PlayCue
.L_0813c84a:
	ldr r1, [sp, #68]
	cmp r1, #2
	bne .L_0813c856
	movs r0, #189
	bl Audio_PlayCue
.L_0813c856:
	ldr r2, [sp, #68]
	cmp r2, #3
	bne .L_0813c862
	movs r0, #148
	bl Audio_PlayCue
.L_0813c862:
	ldr r3, [sp, #68]
	cmp r3, #4
	bne .L_0813c86e
	movs r0, #138
	bl Audio_PlayCue
.L_0813c86e:
	ldr r4, [sp, #16]
	cmp r10, r4
	bge .L_0813c876
	b .L_0813ca88
.L_0813c876:
	ldr r1, [sp, #68]
	movs r0, #0
	lsls r1, r1, #2
	str r1, [sp, #36]
	mov r11, r0
	mov r9, r4
.L_0813c882:
	cmp r10, r9
	blt .L_0813c97c
	ldr r3, [sp, #8]
	add r6, sp, #76
	add r3, r11
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r3, [sp, #64]
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	mov r0, r8
	adds r1, r6, #0
	bl Func_0815e1ec
	ldr r3, [r6]
	mov r4, r8
	asrs r7, r3, #1
	str r7, [r6]
	ldr r5, [r4, #24]
	cmp r5, #0
	bge .L_0813c8b0
	adds r5, #7
.L_0813c8b0:
	asrs r2, r5, #3
	cmp r2, #5
	ble .L_0813c8b8
	movs r2, #5
.L_0813c8b8:
	ldr r3, .L_0813cb00
	ldr r0, [sp, #36]
	ldrsb r3, [r3, r0]
	cmp r3, #0
	beq .L_0813c924
	mov r1, r10
	lsls r5, r2, #1
	lsrs r0, r1, #31
	adds r5, r5, r2
	add r0, r10
	movs r1, #3
	lsls r5, r5, #3
	asrs r0, r0, #1
	adds r5, r5, r2
	bl Math_Mod
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r3, r2, #4
	subs r3, r3, r2
	ldr r2, [sp, #64]
	lsls r3, r3, #6
	lsls r5, r5, #5
	adds r5, r5, r3
	movs r3, #224
	adds r5, r2, r5
	lsls r3, r3, #3
	adds r5, r5, r3
	ldr r3, [r6, #4]
	movs r4, #20
	movs r0, #40
	adds r2, r7, #0
	subs r2, #10
	subs r3, #40
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r1, r5, #0
	ldr r4, [sp, #48]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r6]
	movs r0, #20
	movs r1, #40
	ldr r3, [r6, #4]
	subs r2, #10
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #60]
	adds r1, r5, #0
	ldr r4, [sp, #52]
	mov lr, r4
	.2byte 0xf800
	b .L_0813c96a
.L_0813c924:
	lsls r5, r2, #1
	adds r5, r5, r2
	ldr r0, [sp, #64]
	lsls r5, r5, #3
	adds r5, r5, r2
	ldr r3, [r6, #4]
	lsls r5, r5, #5
	movs r1, #178
	adds r5, r0, r5
	lsls r1, r1, #6
	adds r5, r5, r1
	movs r4, #20
	movs r0, #40
	adds r2, r7, #0
	subs r2, #10
	subs r3, #40
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r1, r5, #0
	ldr r4, [sp, #48]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r6]
	movs r0, #20
	movs r1, #40
	ldr r3, [r6, #4]
	subs r2, #10
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #60]
	adds r1, r5, #0
	ldr r4, [sp, #52]
	mov lr, r4
	.2byte 0xf800
.L_0813c96a:
	mov r0, r8
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	mov r0, r8
	ldr r3, [r0, #24]
	adds r3, #1
	str r3, [r0, #24]
.L_0813c97c:
	movs r2, #1
	add r11, r2
	movs r1, #6
	mov r3, r11
	add r9, r1
	cmp r3, #3
	beq .L_0813c98c
	b .L_0813c882
.L_0813c98c:
	ldr r3, [sp, #36]
	ldr r1, .L_0813cb00
	adds r3, #3
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_0813ca02
	ldr r3, [sp, #16]
	adds r3, #30
	cmp r10, r3
	blt .L_0813ca02
	ldr r3, [sp, #16]
	adds r3, #62
	cmp r10, r3
	bge .L_0813ca02
	ldr r4, [sp, #24]
	ldr r2, [sp, #72]
	ldrsh r0, [r4, r2]
	bl GetBattleObjectSlotFar
	ldr r4, [sp, #16]
	mov r3, r10
	subs r2, r3, r4
	ldr r3, [sp, #20]
	ldr r0, [r0]
	ldr r1, .L_0813cb04
	cmp r3, #0
	bge .L_0813c9c6
	adds r3, r2, #0
	subs r3, #23
.L_0813c9c6:
	ldr r2, [sp, #20]
	asrs r3, r3, #3
	lsls r3, r3, #3
	subs r3, r2, r3
	ldrsb r3, [r1, r3]
	ldr r2, [r0, #8]
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r0, #8]
	cmp r2, #0
	ble .L_0813c9e4
	movs r4, #128
	lsls r4, r4, #8
	adds r3, r2, r4
	b .L_0813c9e8
.L_0813c9e4:
	ldr r1, .L_0813cb08
	adds r3, r2, r1
.L_0813c9e8:
	str r3, [r0, #8]
	ldr r2, [sp, #24]
	ldr r4, [sp, #72]
	movs r1, #1
	ldrsh r0, [r2, r4]
	movs r3, #0
	str r3, [sp, #0]
	negs r1, r1
	movs r2, #5
	subs r3, #1
	bl Func_0814cd48
	ldr r1, .L_0813cb00
.L_0813ca02:
	ldr r3, [sp, #36]
	adds r3, #1
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_0813ca5a
	ldr r3, [sp, #16]
	adds r3, #24
	cmp r10, r3
	bne .L_0813ca3c
	movs r0, #133
	bl Audio_PlayCue
	ldr r0, [sp, #56]
	cmp r0, #0
	bne .L_0813ca28
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
.L_0813ca28:
	ldr r1, [sp, #24]
	ldr r3, [sp, #72]
	ldrsh r0, [r1, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #56]
	bl Func_0814cd48
.L_0813ca3c:
	ldr r3, [sp, #16]
	adds r3, #40
	cmp r10, r3
	bne .L_0813ca58
	ldr r2, [sp, #72]
	ldr r4, [sp, #24]
	movs r3, #8
	ldrsh r0, [r4, r2]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #56]
	bl Func_0814cd48
.L_0813ca58:
	ldr r1, .L_0813cb00
.L_0813ca5a:
	ldr r3, [sp, #36]
	adds r3, #2
	ldrsb r1, [r1, r3]
	movs r3, #1
	negs r3, r3
	cmp r1, r3
	beq .L_0813ca88
	ldr r3, [sp, #16]
	adds r3, #24
	cmp r10, r3
	bne .L_0813ca88
	ldr r4, [sp, #64]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r4, r0
	movs r2, #4
	str r2, [r3]
	ldr r2, [sp, #24]
	ldr r4, [sp, #72]
	ldrsh r0, [r2, r4]
	bl Func_08118088
.L_0813ca88:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	ldr r3, [sp, #8]
	ldr r4, [sp, #56]
	adds r0, #2
	adds r3, #3
	subs r1, #32
	adds r2, #32
	adds r4, #1
	str r0, [sp, #24]
	str r1, [sp, #20]
	str r2, [sp, #16]
	str r3, [sp, #8]
	str r4, [sp, #56]
	ldr r0, [sp, #72]
	ldr r3, [r0, #20]
	cmp r4, r3
	beq .L_0813cab0
	b .L_0813c838
.L_0813cab0:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #64]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #40]
	movs r4, #1
	add r10, r4
	cmp r10, r0
	beq .L_0813cadc
	b .L_0813c7b4
.L_0813cadc:
	ldr r0, .L_0813cb0c
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0813cb00:
	.4byte Data_08197533
.L_0813cb04:
	.4byte Data_08197547
.L_0813cb08:
	.4byte 0xffff8000
.L_0813cb0c:
	.4byte Func_08143000
