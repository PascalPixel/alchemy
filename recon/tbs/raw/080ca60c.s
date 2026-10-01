.syntax unified
	.thumb
	.global BattleFx_RunTwelveMode
	.thumb_func
BattleFx_RunTwelveMode:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #156
	ldr r2, .L_080ca648
	str r1, [sp, #96]
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	str r3, [sp, #92]
	ldr r3, [r2, #8]
	str r3, [sp, #80]
	subs r2, #108
	ldr r3, .L_080ca64c
	mov r11, r1
	ldr r2, [r2]
	add r3, r11
	str r2, [sp, #76]
	str r0, [r3]
	ldr r5, [sp, #96]
	cmp r5, #8
	bne .L_080ca650
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	b .L_080ca656
.L_080ca648:
	.4byte gBattleFxWork
.L_080ca64c:
	.4byte 0x00007828
.L_080ca650:
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
.L_080ca656:
	ldr r2, .L_080ca698
	ldr r3, .L_080ca694
	strh r3, [r2]
	ldr r1, [sp, #80]
	ldr r0, .L_080ca69c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080ca6a0
	mov r1, r11
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, [sp, #96]
	ldr r1, [sp, #96]
	lsls r0, r0, #3
	ldr r2, .L_080ca6a4
	str r0, [sp, #72]
	subs r3, r0, r1
	ldrb r3, [r2, r3]
	cmp r3, #0
	bne .L_080ca6b4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_080ca6a8
	add r1, r11
	movs r2, #0
	b .L_080ca6ac
	.2byte 0x0000
.L_080ca694:
	.4byte 0x00001010
.L_080ca698:
	.4byte 0x04000052
.L_080ca69c:
	.4byte 0x00000073
.L_080ca6a0:
	.4byte 0x000000ce
.L_080ca6a4:
	.4byte Data_080edf04
.L_080ca6a8:
	.4byte 0x000000c5
.L_080ca6ac:
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_080ca6c4
.L_080ca6b4:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_080caa08
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_080ca6c4:
	ldr r5, [sp, #72]
	ldr r0, [sp, #96]
	ldr r2, .L_080caa0c
	subs r3, r5, r0
	adds r3, #3
	ldrb r3, [r2, r3]
	cmp r3, #5
	bhi .L_080ca708
	ldr r2, .L_080caa10
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080ca6dc:
	.4byte .L_080ca6f4
	.4byte .L_080ca6f8
	.4byte .L_080ca6fc
	.4byte .L_080ca700
	.4byte .L_080ca704
	.4byte .L_080ca708
.L_080ca6f4:
	ldr r0, .L_080caa14
	b .L_080ca70a
.L_080ca6f8:
	ldr r0, .L_080caa18
	b .L_080ca70a
.L_080ca6fc:
	ldr r0, .L_080caa1c
	b .L_080ca70a
.L_080ca700:
	ldr r0, .L_080caa20
	b .L_080ca70a
.L_080ca704:
	ldr r0, .L_080caa24
	b .L_080ca70a
.L_080ca708:
	ldr r0, .L_080caa28
.L_080ca70a:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080caa2c
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r1, #200
	lsls r1, r1, #6
	add r1, r11
	movs r2, #1
	movs r3, #0
	ldr r0, .L_080caa30
	bl Resource_LoadAndDecompress
	mov r2, sp
	movs r1, #0
	adds r2, #144
	movs r3, #36
	str r1, [sp, #84]
	str r2, [sp, #68]
	str r1, [sp, #16]
	str r3, [sp, #12]
.L_080ca73c:
	ldr r3, .L_080caa34
	add r3, r11
	ldr r5, [sp, #12]
	ldr r3, [r3]
	ldrsh r0, [r3, r5]
	bl GetBattleObjectSlotFar
	movs r5, #225
	ldr r0, [r0]
	movs r2, #0
	lsls r5, r5, #7
	mov r10, r0
	mov r8, r2
	add r5, r11
.L_080ca758:
	ldr r3, .L_080caa34
	add r3, r11
	ldr r1, [sp, #12]
	ldr r3, [r3]
	ldrsh r0, [r3, r1]
	ldr r1, [sp, #68]
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r0, [sp, #68]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #15
	movs r1, #0
	str r3, [r5, #4]
	str r1, [r5, #8]
	bl Random16
	movs r2, #255
	ands r0, r2
	subs r0, #128
	lsls r0, r0, #9
	str r0, [r5, #12]
	bl Random16
	movs r3, #255
	ands r0, r3
	subs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	movs r3, #1
	add r8, r1
	str r0, [r5, #16]
	negs r3, r3
	movs r0, #0
	mov r2, r8
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	bne .L_080ca758
	mov r8, r0
	ldr r3, [sp, #16]
	ldr r0, .L_080caa38
	movs r6, #255
	adds r5, r3, r0
.L_080ca7bc:
	mov r1, r10
	ldr r3, [r1, #8]
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r1, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	movs r3, #1
	ands r0, r6
	movs r2, #1
	negs r3, r3
	subs r0, #128
	add r8, r2
	str r3, [r5, #24]
	lsls r0, r0, #11
	mov r3, r8
	str r0, [r5, #20]
	adds r5, #28
	cmp r3, #128
	bne .L_080ca7bc
	ldr r0, [sp, #72]
	ldr r1, [sp, #96]
	subs r0, r0, r1
	str r0, [sp, #64]
	ldr r2, .L_080caa3c
	adds r0, #2
	movs r5, #0
	str r0, [sp, #60]
	ldr r6, .L_080caa40
	mov r8, r5
	movs r7, #255
	mov r9, r2
.L_080ca81a:
	mov r5, r10
	ldr r3, [r5, #8]
	str r3, [r6]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	ldr r0, .L_080caa0c
	ldr r1, [sp, #64]
	ldrb r3, [r0, r1]
	cmp r3, #1
	bne .L_080ca85a
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r6, #12]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r6, #16]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r6, #20]
	b .L_080ca8be
.L_080ca85a:
	ldr r3, .L_080caa0c
	ldr r5, [sp, #60]
	ldrb r2, [r3, r5]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080ca890
	bl Random16
	mov r1, r9
	ands r0, r1
	lsls r0, r0, #11
	str r0, [r6, #12]
	bl Random16
	ldr r5, .L_080caa44
	mov r2, r9
	ands r0, r2
	adds r0, r0, r5
	lsls r0, r0, #11
	str r0, [r6, #16]
	bl Random16
	mov r3, r9
	ands r0, r3
	adds r0, r0, r5
	b .L_080ca8ae
.L_080ca890:
	bl Random16
	ands r0, r7
	lsls r0, r0, #11
	str r0, [r6, #12]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r6, #16]
	bl Random16
	ands r0, r7
	subs r0, #128
.L_080ca8ae:
	lsls r0, r0, #11
	str r0, [r6, #20]
	ldr r3, [r6]
	cmp r3, #0
	ble .L_080ca8be
	ldr r3, [r6, #12]
	negs r3, r3
	str r3, [r6, #12]
.L_080ca8be:
	movs r3, #1
	movs r5, #1
	movs r0, #128
	negs r3, r3
	add r8, r5
	lsls r0, r0, #2
	str r3, [r6, #24]
	adds r6, #28
	cmp r8, r0
	bne .L_080ca81a
	ldr r1, [sp, #16]
	ldr r3, [sp, #12]
	ldr r5, [sp, #84]
	movs r2, #224
	lsls r2, r2, #4
	adds r1, r1, r2
	adds r3, #2
	adds r5, #1
	str r1, [sp, #16]
	str r3, [sp, #12]
	str r5, [sp, #84]
	cmp r5, #1
	beq .L_080ca8ee
	b .L_080ca73c
.L_080ca8ee:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080caa48
	movs r3, #75
	add r2, r11
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080caa4c
	bl Scheduler_AddOrUpdateCallback
	ldr r5, .L_080caa34
	add r5, r11
	ldr r3, [r5]
	mov r2, sp
	adds r2, #132
	movs r1, #36
	ldrsh r0, [r3, r1]
	adds r1, r2, #0
	str r2, [sp, #56]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r5]
	movs r5, #36
	ldrsh r0, [r3, r5]
	add r5, sp, #120
	adds r1, r5, #0
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r0, [sp, #56]
	ldr r3, [r5, #4]
	ldr r1, [r0, #4]
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [r0, #4]
	ldr r3, [sp, #96]
	ldr r2, [sp, #72]
	movs r1, #0
	ldr r7, .L_080caa0c
	str r1, [sp, #88]
	subs r1, r2, r3
	adds r3, r1, #6
	adds r5, r7, #0
	ldrb r3, [r5, r3]
	cmp r3, #0
	bne .L_080ca95a
	bl .L_080cb17a
.L_080ca95a:
	ldr r0, [sp, #76]
	ldr r5, .L_080caa34
	adds r0, #12
	str r0, [sp, #20]
	mov r3, sp
	adds r0, r1, #0
	adds r3, #100
	add r5, r11
	str r1, [sp, #44]
	adds r0, #4
	adds r1, #2
	str r3, [sp, #24]
	str r5, [sp, #48]
	str r0, [sp, #40]
	str r1, [sp, #36]
.L_080ca978:
	ldr r3, [sp, #96]
	ldr r5, .L_080caa0c
	subs r2, r2, r3
	adds r3, r2, #1
	ldrb r1, [r5, r3]
	adds r3, r2, #4
	ldrb r4, [r5, r3]
	muls r1, r4
	movs r3, #0
	str r3, [sp, #52]
	ldrb r3, [r5, r2]
	lsls r0, r1, #2
	cmp r3, #0
	beq .L_080ca996
	b .L_080cabe4
.L_080ca996:
	lsls r3, r1, #1
	adds r3, r3, r1
	ldr r5, [sp, #88]
	lsls r3, r3, #1
	cmp r5, r3
	blt .L_080ca9a4
	b .L_080cabd4
.L_080ca9a4:
	adds r1, r4, #0
	adds r0, r5, #0
	bl __divsi3
	movs r1, #6
	bl __modsi3
	adds r6, r0, #0
	ldr r0, [sp, #48]
	ldr r3, [r0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080caa64
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #46
	bl Unnamed_080ed408
	ldr r2, .L_080caa50
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #132]
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_080caa54
	ldrb r3, [r3, r6]
	asrs r2, r2, #1
	lsrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, .L_080caa58
	ldrb r5, [r3, r6]
	ldr r3, .L_080caa5c
	ldrb r4, [r3, r6]
	subs r2, r2, r5
	str r5, [sp, #0]
	ldr r3, [sp, #136]
	str r4, [sp, #4]
	ldr r5, .L_080caa60
	lsrs r0, r4, #1
	subs r3, r3, r0
	add r1, r11
	adds r2, #8
	ldr r4, [r5]
	ldr r0, [sp, #92]
	bl _call_via_r4
	b .L_080caaaa
.L_080caa08:
	.4byte 0x000000c6
.L_080caa0c:
	.4byte Data_080edf04
.L_080caa10:
	.4byte .L_080ca6dc
.L_080caa14:
	.4byte 0x000000cc
.L_080caa18:
	.4byte 0x000000a0
.L_080caa1c:
	.4byte 0x000000a1
.L_080caa20:
	.4byte 0x000000b4
.L_080caa24:
	.4byte 0x0000008d
.L_080caa28:
	.4byte 0x000000c4
.L_080caa2c:
	.4byte IwramCopyWords
.L_080caa30:
	.4byte 0x0000009e
.L_080caa34:
	.4byte 0x00007828
.L_080caa38:
	.4byte gMapCellBuffer
.L_080caa3c:
	.4byte 0x000001ff
.L_080caa40:
	.4byte gMapCellBuffer + 0x3800
.L_080caa44:
	.4byte 0xffffff00
.L_080caa48:
	.4byte 0x00007784
.L_080caa4c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080caa50:
	.4byte Data_080edf64
.L_080caa54:
	.4byte Data_080edf70
.L_080caa58:
	.4byte Data_080edf58
.L_080caa5c:
	.4byte Data_080edf5e
.L_080caa60:
	.4byte gTransitionWork + 0x8
.L_080caa64:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r2, .L_080cacf8
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #132]
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_080cacfc
	ldrb r3, [r3, r6]
	asrs r2, r2, #1
	lsrs r3, r3, #1
	adds r2, r2, r3
	ldr r3, .L_080cad00
	ldrb r4, [r3, r6]
	ldr r3, [sp, #136]
	lsrs r0, r4, #1
	subs r3, r3, r0
	ldr r0, .L_080cad04
	ldrb r0, [r0, r6]
	str r4, [sp, #4]
	str r0, [sp, #0]
	ldr r0, .L_080cad08
	add r1, r11
	ldr r4, [r0]
	subs r2, #8
	ldr r0, [sp, #92]
	bl _call_via_r4
.L_080caaaa:
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r7, .L_080cad0c
	ldr r1, [sp, #40]
	ldrb r5, [r7, r1]
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #1
	ldr r0, [sp, #88]
	bl __modsi3
	lsls r5, r5, #2
	cmp r0, r5
	beq .L_080caaca
	b .L_080cabd4
.L_080caaca:
	ldr r2, [sp, #96]
	cmp r2, #8
	bne .L_080caad8
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080caae4
.L_080caad8:
	movs r0, #133
	bl AudioCommand_PlayFar
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080caae4:
	ldr r3, .L_080cad0c
	ldr r5, [sp, #36]
	ldrb r2, [r3, r5]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080cab40
	ldr r2, .L_080cad10
	movs r3, #8
	add r2, r11
	str r3, [r2]
	ldr r0, [sp, #48]
	ldr r3, [r0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #12
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	ldr r2, [sp, #48]
	ldr r3, [r2]
	movs r1, #4
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r5, .L_080cad14
	movs r0, #0
	mov r8, r0
	movs r6, #15
.L_080cab26:
	bl Random16
	movs r1, #1
	ands r0, r6
	movs r2, #128
	adds r0, #15
	add r8, r1
	lsls r2, r2, #2
	str r0, [r5]
	adds r5, #28
	cmp r8, r2
	bne .L_080cab26
	b .L_080cab9a
.L_080cab40:
	ldr r2, .L_080cad10
	movs r3, #4
	add r2, r11
	str r3, [r2]
	ldr r5, [sp, #48]
	ldr r3, [r5]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	ldr r6, [sp, #44]
	movs r2, #0
	ldr r7, .L_080cad0c
	mov r8, r2
	adds r6, #4
.L_080cab68:
	ldrb r3, [r7, r6]
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #1
	ldr r0, [sp, #88]
	bl __divsi3
	lsls r0, r0, #5
	add r0, r8
	lsls r5, r0, #3
	ldr r3, .L_080cad18
	subs r5, r5, r0
	lsls r5, r5, #2
	adds r5, r5, r3
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #7
	str r3, [r5, #24]
	movs r5, #1
	add r8, r5
	mov r0, r8
	cmp r0, #32
	bne .L_080cab68
.L_080cab9a:
	ldr r3, .L_080cad0c
	ldr r5, [sp, #40]
	ldrb r2, [r3, r5]
	lsls r3, r2, #1
	movs r1, #0
	adds r3, r3, r2
	mov r8, r1
	lsls r5, r3, #1
.L_080cabaa:
	ldr r0, [sp, #88]
	adds r1, r5, #0
	bl __divsi3
	lsls r0, r0, #4
	add r0, r8
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	movs r0, #225
	add r3, r11
	lsls r0, r0, #7
	movs r1, #1
	movs r2, #0
	adds r3, r3, r0
	add r8, r1
	str r2, [r3, #24]
	mov r2, r8
	cmp r2, #8
	bne .L_080cabaa
	ldr r7, .L_080cad0c
.L_080cabd4:
	ldr r3, [sp, #88]
	subs r3, #12
	cmp r3, #19
	bls .L_080cabde
	b .L_080cadd8
.L_080cabde:
	movs r3, #1
	str r3, [sp, #52]
	b .L_080cadd8
.L_080cabe4:
	ldr r5, [sp, #88]
	adds r3, r0, #4
	cmp r5, r3
	blt .L_080cabee
	b .L_080cadd8
.L_080cabee:
	cmp r5, r0
	bge .L_080cac0c
	adds r0, r5, #0
	adds r1, r4, #0
	bl __divsi3
	cmp r0, #4
	ble .L_080cac04
.L_080cabfe:
	subs r0, #4
	cmp r0, #4
	bgt .L_080cabfe
.L_080cac04:
	ldr r3, .L_080cad1c
	ldrb r3, [r3, r0]
	str r3, [sp, #32]
	b .L_080cac10
.L_080cac0c:
	movs r0, #3
	str r0, [sp, #32]
.L_080cac10:
	ldr r2, [sp, #96]
	ldr r1, [sp, #72]
	subs r1, r1, r2
	mov r9, r1
	ldr r5, .L_080cad0c
	movs r3, #5
	add r3, r9
	mov r10, r3
	ldrb r3, [r5, r3]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r0, [sp, #32]
	lsls r5, r0, #3
	ldr r2, [sp, #132]
	subs r5, r5, r0
	lsls r5, r5, #2
	subs r5, r5, r0
	lsrs r3, r2, #31
	movs r6, #48
	adds r2, r2, r3
	lsls r5, r5, #5
	movs r3, #18
	movs r1, #200
	str r3, [sp, #0]
	str r6, [sp, #4]
	ldr r0, .L_080cad08
	lsls r1, r1, #4
	add r5, r11
	adds r5, r5, r1
	asrs r2, r2, #1
	ldr r4, [r0]
	adds r1, r5, #0
	ldr r0, [sp, #92]
	subs r2, #18
	mov r8, r3
	movs r3, #56
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r1, .L_080cad0c
	mov r2, r10
	ldrb r3, [r1, r2]
	movs r0, #46
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl Unnamed_080ed408
	ldr r2, [sp, #132]
	lsrs r3, r2, #31
	adds r2, r2, r3
	mov r3, r8
	str r3, [sp, #0]
	str r6, [sp, #4]
	ldr r0, .L_080cad08
	adds r1, r5, #0
	ldr r4, [r0]
	movs r3, #56
	asrs r2, r2, #1
	ldr r0, [sp, #92]
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	mov r6, r9
	ldr r1, .L_080cad0c
	adds r6, #4
	ldrb r5, [r1, r6]
	ldr r0, [sp, #88]
	lsls r1, r5, #2
	bl __modsi3
	lsls r3, r5, #1
	adds r3, r3, r5
	cmp r0, r3
	beq .L_080cacbc
	b .L_080cadcc
.L_080cacbc:
	ldr r2, [sp, #48]
	ldr r3, [r2]
	movs r5, #36
	ldrsh r0, [r3, r5]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	movs r1, #7
	bl ObjectGroup_UpdateMembers
	ldr r2, .L_080cad10
	movs r3, #4
	add r2, r11
	str r3, [r2]
	mov r3, r9
	adds r3, #1
	ldrb r3, [r7, r3]
	ldrb r2, [r7, r6]
	lsls r3, r3, #2
	subs r3, #4
	muls r3, r2
	ldr r0, [sp, #88]
	cmp r0, r3
	ble .L_080cad20
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080cad26
	.2byte 0x0000
.L_080cacf8:
	.4byte Data_080edf64
.L_080cacfc:
	.4byte Data_080edf70
.L_080cad00:
	.4byte Data_080edf5e
.L_080cad04:
	.4byte Data_080edf58
.L_080cad08:
	.4byte gTransitionWork + 0x8
.L_080cad0c:
	.4byte Data_080edf04
.L_080cad10:
	.4byte 0x000077a8
.L_080cad14:
	.4byte gMapCellBuffer + 0x3818
.L_080cad18:
	.4byte gMapCellBuffer + 0x3800
.L_080cad1c:
	.4byte Data_080edf76
.L_080cad20:
	movs r0, #133
	bl AudioCommand_PlayFar
.L_080cad26:
	ldr r6, [sp, #44]
	movs r1, #0
	ldr r7, .L_080cb080
	mov r8, r1
	adds r6, #4
.L_080cad30:
	ldrb r3, [r7, r6]
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #1
	ldr r0, [sp, #88]
	bl __divsi3
	lsls r0, r0, #6
	add r0, r8
	lsls r5, r0, #3
	subs r5, r5, r0
	ldr r2, .L_080cb084
	lsls r5, r5, #2
	adds r5, r5, r2
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #7
	str r3, [r5, #24]
	movs r3, #1
	add r8, r3
	mov r5, r8
	cmp r5, #64
	bne .L_080cad30
	ldr r3, .L_080cb080
	ldr r1, [sp, #40]
	ldrb r2, [r3, r1]
	lsls r3, r2, #1
	movs r0, #0
	adds r3, r3, r2
	mov r8, r0
	lsls r5, r3, #1
.L_080cad72:
	ldr r0, [sp, #88]
	adds r1, r5, #0
	bl __divsi3
	lsls r0, r0, #4
	add r0, r8
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	movs r2, #225
	lsls r2, r2, #7
	add r3, r11
	adds r3, r3, r2
	movs r2, #0
	str r2, [r3, #24]
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #8
	bne .L_080cad72
	ldr r3, .L_080cb080
	ldr r1, [sp, #40]
	mov r8, r2
	ldrb r2, [r3, r1]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r5, r3, #1
.L_080cada8:
	ldr r0, [sp, #88]
	adds r1, r5, #0
	bl __divsi3
	lsls r0, r0, #4
	add r0, r8
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, .L_080cb088
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r2, #0
	str r2, [r3, #24]
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	bne .L_080cada8
.L_080cadcc:
	ldr r1, [sp, #32]
	cmp r1, #3
	bne .L_080cadd6
	movs r2, #1
	str r2, [sp, #52]
.L_080cadd6:
	ldr r7, .L_080cb080
.L_080cadd8:
	ldr r3, [sp, #36]
	ldrb r2, [r7, r3]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080cae9c
	ldr r5, [sp, #52]
	cmp r5, #0
	beq .L_080cae9c
	ldr r7, [sp, #88]
	ldr r2, [sp, #56]
	movs r1, #3
	movs r0, #0
	mov r8, r0
	mov r9, r1
	ands r7, r1
	mov r10, r2
.L_080cadfa:
	bl Random16
	ldr r3, .L_080cb08c
	adds r2, r0, #0
	ands r2, r3
	str r2, [sp, #8]
	bl Random16
	ldr r2, [sp, #8]
	movs r5, #31
	ands r5, r0
	adds r0, r2, #0
	bl Trig_Sin
	mov r3, r10
	ldr r6, [r3]
	adds r5, #4
	lsrs r3, r6, #31
	adds r6, r6, r3
	adds r3, r5, #0
	muls r3, r0
	ldr r0, .L_080cb090
	asrs r3, r3, #17
	asrs r6, r6, #1
	ldr r2, [sp, #8]
	adds r6, r6, r3
	ldrb r3, [r0, r7]
	adds r0, r2, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	mov r1, r10
	ldr r5, [r1, #4]
	ldr r2, .L_080cb094
	asrs r3, r3, #17
	subs r5, r5, r3
	ldrb r3, [r2, r7]
	lsrs r3, r3, #1
	subs r5, r5, r3
	bl Random16
	ldr r3, .L_080cb098
	mov r1, r9
	ands r0, r1
	ldrb r2, [r3, r0]
	mov r3, r9
	str r1, [sp, #0]
	orrs r3, r2
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl Unnamed_080ed408
	ldr r2, .L_080cb09c
	lsls r3, r7, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_080cb090
	ldrb r3, [r2, r7]
	ldr r0, .L_080cb094
	str r3, [sp, #0]
	ldrb r3, [r0, r7]
	ldr r2, .L_080cb0a0
	str r3, [sp, #4]
	add r1, r11
	adds r3, r5, #0
	ldr r4, [r2]
	ldr r0, [sp, #92]
	adds r2, r6, #0
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	add r8, r3
	mov r5, r8
	cmp r5, #3
	bne .L_080cadfa
.L_080cae9c:
	bl Render_ResetTransformState
	ldr r0, [sp, #76]
	ldr r1, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r0, .L_080cb0a4
	ldr r3, [r0]
	movs r1, #7
	str r3, [sp, #100]
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r1, .L_080cb0a0
	ldr r2, [sp, #24]
	ldr r3, [r1]
	str r3, [r2, #4]
	ldr r5, [sp, #44]
	movs r3, #0
	adds r5, #2
	str r3, [sp, #84]
	str r5, [sp, #28]
.L_080caede:
	ldr r0, [sp, #48]
	ldr r3, [r0]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r7, .L_080cb080
	ldr r1, [sp, #28]
	ldrb r2, [r7, r1]
	ldr r0, [r0]
	movs r3, #1
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_080caf64
	movs r5, #225
	movs r2, #0
	lsls r5, r5, #7
	mov r8, r2
	add r5, r11
.L_080caf04:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_080caf56
	cmp r3, #0
	bge .L_080caf10
	adds r3, #3
.L_080caf10:
	asrs r3, r3, #2
	lsls r1, r3, #3
	adds r1, r1, r3
	lsls r1, r1, #7
	movs r2, #200
	mov r0, r8
	lsls r2, r2, #6
	movs r4, #1
	add r1, r11
	ands r4, r0
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r0, [sp, #24]
	lsls r4, r4, #2
	subs r3, #24
	ldr r4, [r4, r0]
	subs r2, #12
	ldr r0, [sp, #92]
	bl _call_via_r4
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_080cb0a8
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_080caf56:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #64
	bne .L_080caf04
	ldr r7, .L_080cb080
.L_080caf64:
	ldr r3, [sp, #28]
	ldrb r2, [r7, r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080cb066
	ldr r0, [sp, #96]
	movs r5, #3
	mov r10, r5
	cmp r0, #11
	bne .L_080caf7e
	movs r1, #8
	mov r10, r1
.L_080caf7e:
	ldr r2, [sp, #88]
	cmp r2, #55
	bne .L_080caf9a
	ldr r5, [sp, #48]
	ldr r3, [r5]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080caf9a:
	ldr r0, [sp, #88]
	cmp r0, #90
	bne .L_080cafb6
	ldr r1, [sp, #48]
	ldr r3, [r1]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080cafb6:
	movs r2, #0
	mov r3, r10
	ldr r5, .L_080cb088
	mov r8, r2
	add r6, sp, #108
	lsls r7, r3, #1
.L_080cafc2:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080cb058
	adds r1, r6, #0
	adds r0, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	ldr r1, .L_080cb0ac
	asrs r2, r2, #1
	str r2, [r6]
	subs r3, r7, #2
	ldrh r1, [r1, r3]
	ldr r0, [sp, #80]
	adds r1, r0, r1
	mov r0, r10
	lsrs r3, r0, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r7, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [r0, #4]
	ldr r0, [sp, #92]
	bl _call_via_r4
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #10
	ble .L_080cb058
	mov r1, r9
	ldr r0, [r1, #8]
	ldr r3, [r5]
	ldr r1, [r1, #12]
	subs r0, r0, r3
	ldr r3, [r5, #4]
	movs r2, #160
	subs r1, r1, r3
	lsls r2, r2, #13
	mov r3, r9
	adds r1, r1, r2
	ldr r2, [r3, #16]
	ldr r3, [r5, #8]
	subs r2, r2, r3
	ldr r3, [r5, #12]
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	asrs r1, r1, #8
	adds r3, r3, r1
	ldr r1, .L_080cb0b0
	str r3, [r5, #16]
	ldr r3, [r5, #20]
	asrs r2, r2, #8
	adds r0, r0, r1
	ldr r1, .L_080cb0b4
	adds r3, r3, r2
	str r3, [r5, #20]
	cmp r0, r1
	bhi .L_080cb058
	ldr r0, .L_080cb0b0
	adds r3, r2, r0
	cmp r3, r1
	bhi .L_080cb058
	movs r1, #1
	negs r1, r1
	str r1, [r5, #24]
.L_080cb058:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #64
	bne .L_080cafc2
	ldr r7, .L_080cb080
.L_080cb066:
	ldr r5, [sp, #28]
	ldrb r2, [r7, r5]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080cb114
	ldr r1, .L_080cb0ac
	movs r0, #0
	ldr r7, .L_080cb084
	mov r8, r0
	add r6, sp, #108
	mov r10, r1
	b .L_080cb0b8
.L_080cb080:
	.4byte Data_080edf04
.L_080cb084:
	.4byte gMapCellBuffer + 0x3800
.L_080cb088:
	.4byte gMapCellBuffer
.L_080cb08c:
	.4byte 0x0000ffff
.L_080cb090:
	.4byte BattleFx_GlintCellWidths
.L_080cb094:
	.4byte BattleFx_GlintCellHeights
.L_080cb098:
	.4byte Data_080edf7b
.L_080cb09c:
	.4byte BattleFx_GlintCellOffsets
.L_080cb0a0:
	.4byte gTransitionWork + 0xc
.L_080cb0a4:
	.4byte gTransitionWork + 0x8
.L_080cb0a8:
	.4byte 0xfffffc00
.L_080cb0ac:
	.4byte ParticleStreams_CellOffsets
.L_080cb0b0:
	.4byte 0x00000fff
.L_080cb0b4:
	.4byte 0x00001ffe
.L_080cb0b8:
	ldr r5, [r7, #24]
	cmp r5, #0
	blt .L_080cb108
	adds r1, r6, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	asrs r3, r3, #1
	str r3, [r6]
	adds r0, r7, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	asrs r5, r5, #3
	adds r5, #1
	lsls r0, r5, #1
	subs r3, r0, #2
	mov r2, r10
	ldrh r1, [r2, r3]
	ldr r3, [sp, #80]
	adds r1, r3, r1
	lsrs r3, r5, #31
	ldr r2, [r6]
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	subs r3, r3, r5
	str r0, [sp, #4]
	ldr r5, [sp, #24]
	ldr r0, [sp, #92]
	ldr r4, [r5, #4]
	bl _call_via_r4
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_080cb108:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r7, #28
	cmp r1, #128
	bne .L_080cb0b8
.L_080cb114:
	ldr r2, [sp, #84]
	adds r2, #1
	str r2, [sp, #84]
	cmp r2, #1
	beq .L_080cb120
	b .L_080caede
.L_080cb120:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080cb198
	ldr r5, [sp, #36]
	ldrb r2, [r3, r5]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080cb144
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	b .L_080cb14c
.L_080cb144:
	movs r0, #2
	movs r1, #4
	bl Camera_ApplyShake
.L_080cb14c:
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cb19c
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #88]
	ldr r2, [sp, #72]
	ldr r1, [sp, #96]
	adds r0, #1
	ldr r7, .L_080cb198
	str r0, [sp, #88]
	subs r3, r2, r1
	adds r3, #6
	adds r5, r7, #0
	ldrb r3, [r5, r3]
	cmp r0, r3
	beq .L_080cb17a
	bl .L_080ca978
.L_080cb17a:
	ldr r0, .L_080cb1a0
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #156
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080cb198:
	.4byte Data_080edf04
.L_080cb19c:
	.4byte 0x00007824
.L_080cb1a0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
