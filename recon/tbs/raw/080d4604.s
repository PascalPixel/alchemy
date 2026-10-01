.syntax unified
	.thumb
	.global BattleFx_RunSparkGroups
	.thumb_func
BattleFx_RunSparkGroups:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #80
	ldr r2, .L_080d4658
	str r1, [sp, #56]
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	str r3, [sp, #52]
	ldr r5, .L_080d465c
	ldr r2, [r2, #8]
	mov r9, r1
	add r5, r9
	str r2, [sp, #40]
	str r0, [r5]
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_080d4642
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r3, #60
	movs r0, #48
	str r3, [sp, #36]
	str r0, [sp, #32]
	b .L_080d4682
.L_080d4642:
	ldr r1, [sp, #56]
	cmp r1, #1
	bne .L_080d4660
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	movs r2, #60
	movs r3, #64
	str r2, [sp, #36]
	b .L_080d4680
	.2byte 0x0000
.L_080d4658:
	.4byte gBattleFxWork
.L_080d465c:
	.4byte 0x00007828
.L_080d4660:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, [r5]
	add r5, sp, #68
	ldr r0, [r3, #8]
	adds r1, r5, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #36]
	ldr r3, [r5, #4]
	adds r3, #48
.L_080d4680:
	str r3, [sp, #32]
.L_080d4682:
	ldr r2, .L_080d46c4
	ldr r3, .L_080d46c0
	strh r3, [r2]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl BattleEffect_LoadWork
	ldr r5, .L_080d46c8
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #60]
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	adds r5, #188
	ldr r3, [r5]
	mov r0, sp
	adds r0, #60
	str r0, [sp, #24]
	mov r1, r9
	str r3, [r0, #4]
	b .L_080d46cc
.L_080d46c0:
	.4byte 0x00001010
.L_080d46c4:
	.4byte 0x04000052
.L_080d46c8:
	.4byte gWorkSlot
.L_080d46cc:
	movs r2, #1
	ldr r0, .L_080d49d0
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_080d49d4
	ldr r1, [sp, #40]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r1, [sp, #56]
	cmp r1, #1
	bne .L_080d46fe
	ldr r0, .L_080d49d8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d49dc
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	b .L_080d4718
.L_080d46fe:
	ldr r2, [sp, #56]
	cmp r2, #2
	bne .L_080d4718
	ldr r0, .L_080d49e0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d49dc
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080d4718:
	movs r3, #0
	str r3, [sp, #48]
	ldr r3, .L_080d49e4
	add r3, r9
	ldr r3, [r3]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r1, .L_080d49e8
	lsls r3, r3, #1
	adds r3, #2
	ldrh r3, [r1, r3]
	cmp r3, #0
	bne .L_080d4736
	b .L_080d4894
.L_080d4736:
	mov r0, r9
	str r0, [sp, #12]
.L_080d473a:
	ldr r2, [sp, #12]
	movs r3, #225
	movs r1, #0
	lsls r3, r3, #7
	mov r10, r1
	adds r7, r2, r3
.L_080d4746:
	mov r0, r10
	lsls r6, r0, #1
	bl Random16
	ldr r3, .L_080d49ec
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r7]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	mov r1, r10
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r1, #31
	add r3, r10
	asrs r3, r3, #1
	movs r2, #1
	adds r3, #25
	add r10, r2
	str r3, [r7, #24]
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080d4746
	movs r0, #0
	mov r10, r0
	ldr r0, .L_080d49e4
	mov r2, r9
	ldr r3, [r2, r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	ldr r1, .L_080d49e8
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r1, r3]
	adds r4, r1, #0
	cmp r3, #0
	beq .L_080d486a
	ldr r3, [sp, #32]
	lsls r3, r3, #16
	mov r11, r3
.L_080d47a8:
	mov r1, r9
	adds r5, r1, r0
	ldr r3, [r5]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r4, r3]
	ldr r0, [sp, #48]
	adds r2, r3, #0
	muls r2, r0
	add r2, r10
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r1, .L_080d49f0
	lsls r3, r3, #2
	adds r6, r3, r1
	str r4, [sp, #8]
	bl Random16
	ldr r3, .L_080d49f4
	ands r3, r0
	adds r3, #32
	mov r8, r3
	bl Random16
	ldr r3, .L_080d49ec
	ldr r5, [r5]
	adds r7, r0, #0
	ands r7, r3
	ldr r3, [r5, #4]
	ldr r4, [sp, #8]
	cmp r3, #1
	bne .L_080d4804
	ldr r3, [r5, #24]
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, [sp, #48]
	adds r2, r2, r3
	lsls r2, r2, #1
	adds r2, #4
	ldr r0, [sp, #36]
	ldrh r3, [r4, r2]
	subs r3, r0, r3
	adds r3, #28
	b .L_080d481a
.L_080d4804:
	ldr r3, [r5, #24]
	ldr r1, [sp, #48]
	lsls r2, r3, #2
	adds r2, r2, r3
	adds r2, r2, r1
	lsls r2, r2, #1
	adds r2, #4
	ldrh r3, [r4, r2]
	ldr r2, [sp, #36]
	adds r3, r2, r3
	subs r3, #28
.L_080d481a:
	lsls r3, r3, #16
	str r3, [r6]
	mov r3, r11
	str r3, [r6, #4]
	adds r0, r7, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r6, #12]
	adds r0, r7, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r6, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r0, #1
	add r10, r0
	ldr r0, .L_080d49e4
	adds r3, #32
	mov r2, r9
	str r3, [r6, #24]
	ldr r3, [r2, r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	ldr r1, .L_080d49e8
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r1, r3]
	adds r4, r1, #0
	cmp r10, r3
	bne .L_080d47a8
.L_080d486a:
	ldr r3, [sp, #12]
	movs r0, #224
	lsls r0, r0, #1
	ldr r2, [sp, #48]
	adds r3, r3, r0
	str r3, [sp, #12]
	adds r2, #1
	ldr r3, .L_080d49e4
	str r2, [sp, #48]
	add r3, r9
	ldr r3, [r3]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #2
	ldrh r3, [r1, r3]
	ldr r0, [sp, #48]
	cmp r0, r3
	beq .L_080d4894
	b .L_080d473a
.L_080d4894:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080d49f8
	movs r3, #75
	add r2, r9
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d49fc
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	str r1, [sp, #44]
	ldr r0, .L_080d49e4
	mov r2, r9
	ldr r3, [r2, r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r1, .L_080d49e8
	lsls r3, r3, #1
	adds r3, #2
	ldrh r3, [r1, r3]
	ldr r2, .L_080d4a00
	cmp r3, r2
	bne .L_080d48d0
	b .L_080d4c9c
.L_080d48d0:
	ldr r3, [sp, #56]
	ldr r1, .L_080d49e4
	subs r3, #1
	add r1, r9
	str r3, [sp, #20]
	str r1, [sp, #28]
.L_080d48dc:
	ldr r3, .L_080d4a04
	ldr r1, [r3]
	mov r3, r9
	ldr r2, [r3, r0]
	ldr r3, [r2, #24]
	cmp r3, #2
	bne .L_080d4908
	ldr r0, [sp, #44]
	cmp r0, #51
	bgt .L_080d4908
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_080d4900
	ldrh r3, [r1, #54]
	movs r2, #128
	lsls r2, r2, #1
	adds r3, r3, r2
	b .L_080d4906
.L_080d4900:
	ldrh r3, [r1, #54]
	ldr r0, .L_080d4a08
	adds r3, r3, r0
.L_080d4906:
	strh r3, [r1, #54]
.L_080d4908:
	ldr r1, [sp, #28]
	ldr r3, [r1]
	ldr r3, [r3, #24]
	cmp r3, #3
	bne .L_080d4926
	ldr r2, [sp, #44]
	cmp r2, #4
	bne .L_080d4926
	movs r1, #128
	ldr r3, .L_080d4a0c
	ldr r0, [sp, #52]
	lsls r1, r1, #7
	ldr r2, .L_080d4a10
	bl _call_via_r3
.L_080d4926:
	ldr r3, [sp, #20]
	cmp r3, #1
	bhi .L_080d493a
	ldr r0, [sp, #44]
	cmp r0, #2
	bne .L_080d4952
	movs r0, #145
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080d4952
.L_080d493a:
	ldr r1, [sp, #44]
	cmp r1, #2
	bne .L_080d4946
	movs r0, #145
	bl AudioCommand_PlayFar
.L_080d4946:
	ldr r2, [sp, #44]
	cmp r2, #24
	bne .L_080d4952
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080d4952:
	movs r3, #0
	str r3, [sp, #48]
	ldr r0, [sp, #28]
	ldr r3, [r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldr r1, .L_080d49e8
	adds r3, #2
	ldrh r3, [r1, r3]
	cmp r3, #0
	bne .L_080d496e
	b .L_080d4c5c
.L_080d496e:
	mov r2, r9
	str r2, [sp, #16]
.L_080d4972:
	ldr r3, [sp, #48]
	ldr r0, [sp, #44]
	lsls r3, r3, #3
	mov r11, r3
	cmp r0, r11
	bne .L_080d4986
	ldr r2, .L_080d4a14
	movs r3, #12
	add r2, r9
	str r3, [r2]
.L_080d4986:
	ldr r1, [sp, #44]
	cmp r1, r11
	bge .L_080d498e
	b .L_080d4af8
.L_080d498e:
	mov r3, r11
	adds r3, #2
	cmp r1, r3
	bge .L_080d4a46
	ldr r3, [sp, #28]
	ldr r2, [r3]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080d4a18
	ldr r2, [r2, #24]
	ldr r0, [sp, #48]
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r3, r3, r0
	ldr r1, .L_080d49e8
	lsls r3, r3, #1
	adds r3, #4
	ldrh r2, [r1, r3]
	ldr r3, [sp, #36]
	subs r2, r3, r2
	movs r3, #32
	str r3, [sp, #0]
	movs r3, #64
	str r3, [sp, #4]
	ldr r3, [sp, #32]
	adds r2, #12
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	mov r1, r9
	subs r3, #32
	bl _call_via_r4
	b .L_080d4a46
.L_080d49d0:
	.4byte 0x0000007d
.L_080d49d4:
	.4byte 0x00000073
.L_080d49d8:
	.4byte 0x00000087
.L_080d49dc:
	.4byte IwramCopyWords
.L_080d49e0:
	.4byte 0x000000c4
.L_080d49e4:
	.4byte 0x00007828
.L_080d49e8:
	.4byte Data_080ee262
.L_080d49ec:
	.4byte 0x0000ffff
.L_080d49f0:
	.4byte gMapCellBuffer
.L_080d49f4:
	.4byte 0x000003ff
.L_080d49f8:
	.4byte 0x00007784
.L_080d49fc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d4a00:
	.4byte 0x1ffffff9
.L_080d4a04:
	.4byte gCameraWork
.L_080d4a08:
	.4byte 0xffffff00
.L_080d4a0c:
	.4byte IwramFillWords
.L_080d4a10:
	.4byte 0x3f3f3f3f
.L_080d4a14:
	.4byte 0x000077a8
.L_080d4a18:
	ldr r2, [r2, #24]
	ldr r0, [sp, #48]
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r3, r3, r0
	ldr r1, .L_080d4cc4
	lsls r3, r3, #1
	adds r3, #4
	ldrh r2, [r1, r3]
	ldr r3, [sp, #36]
	adds r2, r3, r2
	movs r3, #32
	str r3, [sp, #0]
	movs r3, #64
	str r3, [sp, #4]
	ldr r3, [sp, #32]
	subs r2, #44
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	mov r1, r9
	subs r3, #32
	bl _call_via_r4
.L_080d4a46:
	ldr r0, [sp, #44]
	cmp r0, r11
	blt .L_080d4af8
	ldr r2, .L_080d4cc4
	ldr r3, [sp, #16]
	movs r0, #225
	movs r1, #0
	lsls r0, r0, #7
	mov r10, r1
	mov r8, r2
	adds r5, r3, r0
.L_080d4a5c:
	movs r1, #6
	ldrsh r3, [r5, r1]
	ldr r2, [sp, #32]
	adds r7, r3, r2
	ldr r3, [sp, #28]
	ldr r2, [r3]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080d4a90
	ldr r2, [r2, #24]
	movs r0, #2
	ldrsh r1, [r5, r0]
	ldr r3, [sp, #36]
	ldr r0, [sp, #48]
	adds r1, r1, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, #4
	mov r2, r8
	ldrh r3, [r2, r3]
	subs r1, r1, r3
	adds r6, r1, #0
	adds r6, #28
	b .L_080d4ab0
.L_080d4a90:
	ldr r2, [r2, #24]
	movs r3, #2
	ldrsh r1, [r5, r3]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #48]
	ldr r0, [sp, #36]
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r1, r1, r0
	adds r3, #4
	mov r0, r8
	ldrh r3, [r0, r3]
	adds r1, r1, r3
	adds r6, r1, #0
	subs r6, #28
.L_080d4ab0:
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_080d4ade
	movs r1, #3
	bl __divsi3
	ldr r2, .L_080d4cc8
	ldrb r1, [r2, r0]
	movs r0, #32
	str r0, [sp, #0]
	lsls r1, r1, #11
	movs r0, #64
	adds r2, r6, #0
	adds r3, r7, #0
	str r0, [sp, #4]
	add r1, r9
	subs r2, #16
	subs r3, #32
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r0, [r5, #24]
.L_080d4ade:
	cmp r0, #0
	ble .L_080d4ae6
	subs r3, r0, #1
	b .L_080d4aea
.L_080d4ae6:
	movs r3, #1
	negs r3, r3
.L_080d4aea:
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #28
	cmp r0, #12
	bne .L_080d4a5c
.L_080d4af8:
	mov r3, r11
	ldr r1, [sp, #44]
	adds r3, #5
	cmp r1, r3
	ble .L_080d4be4
	ldr r2, [sp, #56]
	ldr r7, .L_080d4ccc
	cmp r2, #2
	beq .L_080d4b0e
	movs r7, #128
	lsls r7, r7, #5
.L_080d4b0e:
	ldr r4, .L_080d4cd0
	movs r3, #0
	mov r2, r9
	mov r10, r3
	ldr r3, [r2, r4]
	ldr r2, [r3, #24]
	ldr r1, .L_080d4cc4
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r0, r1, #0
	lsls r3, r3, #1
	ldrh r3, [r0, r3]
	cmp r3, #0
	beq .L_080d4be4
.L_080d4b2a:
	mov r2, r9
	ldr r3, [r2, r4]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r0, r3]
	ldr r0, [sp, #48]
	adds r2, r3, #0
	muls r2, r0
	add r2, r10
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_080d4cd4
	lsls r3, r3, #2
	adds r6, r3, r2
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080d4bca
	adds r0, r6, #0
	adds r2, r7, #0
	movs r1, #60
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r6, #24]
	movs r0, #216
	ldr r2, [r6, #4]
	subs r3, #1
	lsls r0, r0, #15
	str r3, [r6, #24]
	cmp r2, r0
	ble .L_080d4b78
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080d4bc8
.L_080d4b78:
	ldr r0, [r6]
	ldr r1, .L_080d4cd8
	cmp r0, r1
	bhi .L_080d4bc8
	cmp r2, #0
	blt .L_080d4bc8
	asrs r2, r2, #16
	asrs r6, r0, #16
	movs r1, #5
	adds r0, r3, #0
	mov r8, r2
	bl __divsi3
	adds r0, #1
	mov r2, r10
	movs r4, #1
	lsls r5, r0, #1
	ands r4, r2
	ldr r2, .L_080d4cdc
	subs r3, r5, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #40]
	adds r1, r3, r1
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	mov r2, r8
	subs r2, r2, r0
	str r0, [sp, #0]
	subs r6, r6, r3
	str r5, [sp, #4]
	ldr r3, [sp, #24]
	mov r8, r2
	lsls r4, r4, #2
	ldr r4, [r4, r3]
	ldr r0, [sp, #52]
	adds r2, r6, #0
	mov r3, r8
	bl _call_via_r4
.L_080d4bc8:
	ldr r1, .L_080d4cc4
.L_080d4bca:
	ldr r4, .L_080d4cd0
	mov r2, r9
	ldr r3, [r2, r4]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	movs r0, #1
	adds r3, r3, r2
	add r10, r0
	lsls r3, r3, #1
	adds r0, r1, #0
	ldrh r3, [r0, r3]
	cmp r10, r3
	bne .L_080d4b2a
.L_080d4be4:
	ldr r2, .L_080d4cd0
	movs r3, #0
	mov r0, r9
	mov r10, r3
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080d4c32
	mov r7, r11
	adds r7, #6
	movs r6, #36
.L_080d4bfa:
	ldr r1, [sp, #44]
	cmp r1, r7
	bne .L_080d4c20
	mov r3, r9
	adds r5, r3, r2
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r10
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
.L_080d4c20:
	ldr r2, .L_080d4cd0
	movs r3, #1
	mov r0, r9
	add r10, r3
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r10, r3
	bne .L_080d4bfa
.L_080d4c32:
	ldr r1, [sp, #16]
	ldr r3, [sp, #48]
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r1, r2
	adds r3, #1
	str r1, [sp, #16]
	str r3, [sp, #48]
	ldr r0, [sp, #28]
	ldr r3, [r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldr r1, .L_080d4cc4
	adds r3, #2
	ldrh r3, [r1, r3]
	ldr r2, [sp, #48]
	cmp r2, r3
	beq .L_080d4c5c
	b .L_080d4972
.L_080d4c5c:
	movs r1, #16
	movs r0, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080d4ce0
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #44]
	adds r3, #1
	str r3, [sp, #44]
	ldr r0, .L_080d4cd0
	mov r1, r9
	ldr r3, [r1, r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldr r2, .L_080d4cc4
	adds r3, #2
	ldrh r3, [r2, r3]
	ldr r1, [sp, #44]
	lsls r3, r3, #3
	adds r3, #56
	cmp r1, r3
	beq .L_080d4c9c
	b .L_080d48dc
.L_080d4c9c:
	ldr r0, .L_080d4ce4
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080d4cc4:
	.4byte Data_080ee262
.L_080d4cc8:
	.4byte Data_080ee294
.L_080d4ccc:
	.4byte 0xfffff000
.L_080d4cd0:
	.4byte 0x00007828
.L_080d4cd4:
	.4byte gMapCellBuffer
.L_080d4cd8:
	.4byte 0x007effff
.L_080d4cdc:
	.4byte ParticleStreams_CellOffsets
.L_080d4ce0:
	.4byte 0x00007824
.L_080d4ce4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
