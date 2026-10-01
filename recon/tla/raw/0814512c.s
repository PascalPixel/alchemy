.syntax unified
	.thumb
	.global Func_0814512c
	.thumb_func
Func_0814512c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	str r1, [sp, #48]
	str r0, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r2, [sp, #48]
	str r0, [sp, #44]
	ldr r1, [r3, #96]
	str r1, [sp, #40]
	ldr r3, [r3, #48]
	str r3, [sp, #20]
	cmp r2, #7
	bne .L_08145180
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, [sp, #52]
	add r2, sp, #64
	ldr r1, [r3, #4]
	movs r3, #68
	lsls r1, r1, #4
	orrs r1, r3
	ldr r0, [sp, #52]
	add r3, sp, #76
	bl Func_0815585c
	movs r2, #128
	ldr r3, .L_0814517c
	lsls r2, r2, #19
	adds r2, #12
	b .L_0814518e
	.2byte 0x0000
.L_0814517c:
	.4byte 0x00000785
.L_08145180:
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r2, #128
	ldr r3, .L_081451c0
	lsls r2, r2, #19
	adds r2, #82
.L_0814518e:
	strh r3, [r2]
	ldr r4, [sp, #44]
	ldr r5, .L_081451c4
	movs r0, #224
	lsls r0, r0, #3
	adds r1, r4, r0
	movs r2, #1
	adds r0, r5, #0
	movs r3, #1
	bl Func_08157cf4
	ldr r2, [sp, #44]
	movs r3, #139
	lsls r3, r3, #7
	adds r1, r2, r3
	ldr r0, .L_081451c8
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r4, [sp, #48]
	cmp r4, #0
	bne .L_081451d2
	ldr r0, .L_081451cc
	b .L_081451d0
.L_081451c0:
	.4byte 0x00001010
.L_081451c4:
	.4byte 0x00000183
.L_081451c8:
	.4byte 0x00000161
.L_081451cc:
	.4byte 0x00000162
.L_081451d0:
	b .L_0814521a
.L_081451d2:
	ldr r0, [sp, #48]
	cmp r0, #1
	bne .L_081451dc
	ldr r0, .L_08145488
	b .L_0814521a
.L_081451dc:
	ldr r1, [sp, #48]
	cmp r1, #2
	bne .L_081451e6
	ldr r0, .L_0814548c
	b .L_0814521a
.L_081451e6:
	ldr r2, [sp, #48]
	cmp r2, #8
	bne .L_081451f0
	ldr r0, .L_0814548c
	b .L_0814521a
.L_081451f0:
	ldr r3, [sp, #48]
	cmp r3, #3
	beq .L_08145218
	ldr r4, [sp, #48]
	cmp r4, #4
	beq .L_08145200
	cmp r4, #7
	bne .L_08145204
.L_08145200:
	adds r0, r5, #0
	b .L_0814521a
.L_08145204:
	ldr r0, [sp, #48]
	cmp r0, #6
	bne .L_0814520e
	ldr r0, .L_08145490
	b .L_0814521a
.L_0814520e:
	ldr r1, [sp, #48]
	cmp r1, #9
	bne .L_08145218
	ldr r0, .L_08145494
	b .L_0814521a
.L_08145218:
	ldr r0, .L_08145498
.L_0814521a:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814549c
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	ldr r3, .L_081454a0
	str r2, [sp, #32]
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #3
.L_0814523a:
	str r1, [r3]
	ldr r4, [sp, #32]
	adds r3, #28
	adds r4, #1
	str r4, [sp, #32]
	cmp r4, r2
	bne .L_0814523a
	ldr r1, [sp, #52]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #48]
	movs r2, #160
	lsls r2, r2, #14
	ldr r7, [r0]
	mov r10, r2
	cmp r3, #7
	beq .L_08145270
	ldr r0, [sp, #48]
	movs r4, #160
	lsls r4, r4, #13
	mov r10, r4
	cmp r0, #8
	beq .L_08145270
	movs r1, #128
	lsls r1, r1, #11
	mov r10, r1
.L_08145270:
	movs r2, #0
	str r2, [sp, #28]
	ldr r3, [sp, #52]
	ldr r1, [r3, #20]
	cmp r1, #0
	beq .L_081452f4
	mov r8, r2
.L_0814527e:
	ldr r0, [sp, #28]
	bl Math_Mod
	ldr r4, [sp, #52]
	lsls r0, r0, #1
	adds r0, #36
	ldrsh r0, [r4, r0]
	bl GetBattleObjectSlotFar
	ldr r5, .L_081454a4
	movs r2, #0
	ldr r6, [r0]
	str r2, [sp, #32]
	add r5, r8
.L_0814529a:
	ldr r3, [r7, #8]
	str r3, [r5]
	mov r3, r10
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16
	movs r3, #31
	ldr r1, [r6, #16]
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #17
	ldr r2, [r7, #8]
	adds r1, r1, r3
	ldr r3, [r6, #8]
	subs r3, r3, r2
	asrs r3, r3, #4
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #16]
	ldr r3, [r7, #16]
	subs r1, r1, r3
	asrs r1, r1, #4
	movs r3, #0
	str r1, [r5, #20]
	str r3, [r5, #24]
	ldr r4, [sp, #32]
	adds r5, #28
	adds r4, #1
	str r4, [sp, #32]
	cmp r4, #16
	bne .L_0814529a
	ldr r1, [sp, #28]
	ldr r2, [sp, #52]
	adds r1, #1
	str r1, [sp, #28]
	ldr r3, [sp, #28]
	ldr r1, [r2, #20]
	movs r0, #224
	lsls r0, r0, #1
	add r8, r0
	cmp r3, r1
	bne .L_0814527e
.L_081452f4:
	movs r4, #0
	str r4, [sp, #32]
	ldr r3, .L_081454a8
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #1
.L_08145302:
	str r1, [r3]
	ldr r0, [sp, #32]
	adds r3, #28
	adds r0, #1
	str r0, [sp, #32]
	cmp r0, r2
	bne .L_08145302
	ldr r1, [sp, #52]
	mov r2, sp
	adds r2, #56
	ldr r0, [r1, #4]
	adds r1, r2, #0
	str r2, [sp, #16]
	bl Func_08144aac
	ldr r3, [sp, #48]
	cmp r3, #4
	beq .L_0814532a
	cmp r3, #7
	bne .L_0814534c
.L_0814532a:
	ldr r4, [sp, #44]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r4, r0
	movs r3, #1
	adds r1, #132
	str r3, [r2]
	adds r3, r4, r1
	movs r2, #0
	str r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	b .L_08145366
.L_0814534c:
	ldr r3, [sp, #44]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #44]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #75
	str r3, [r2]
.L_08145366:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_081454ac
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #48]
	ldr r2, .L_081454b0
	ldr r4, [sp, #52]
	lsls r3, r3, #2
	mov r11, r3
	adds r3, #3
	ldrsb r2, [r2, r3]
	ldr r3, [r4, #20]
	movs r0, #103
	lsls r3, r3, #3
	adds r2, r2, r3
	str r2, [sp, #24]
	bl Audio_PlayCue
	ldr r1, [sp, #24]
	movs r0, #0
	str r0, [sp, #36]
	cmp r1, #0
	bne .L_08145398
	b .L_081456fc
.L_08145398:
	ldr r2, [sp, #20]
	adds r2, #12
	str r2, [sp, #12]
.L_0814539e:
	bl Func_08014de4
	ldr r1, [sp, #12]
	ldr r0, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	movs r3, #0
	str r3, [sp, #28]
	ldr r4, [sp, #52]
	ldr r1, [r4, #20]
	cmp r1, #0
	bne .L_081453b8
	b .L_081455d0
.L_081453b8:
	ldr r2, .L_081454b0
.L_081453ba:
	movs r0, #0
	str r0, [sp, #32]
	mov r3, r11
	adds r3, #2
	ldrsb r3, [r2, r3]
	cmp r3, #0
	bne .L_081453ca
	b .L_081455c4
.L_081453ca:
	ldr r1, [sp, #28]
	lsls r1, r1, #3
	mov r9, r1
.L_081453d0:
	ldr r3, [sp, #36]
	cmp r3, r9
	bge .L_081453d8
	b .L_081455a6
.L_081453d8:
	ldr r4, [sp, #28]
	ldr r0, [sp, #32]
	lsls r2, r4, #4
	adds r2, r2, r0
	lsls r3, r2, #3
	ldr r1, .L_081454a4
	subs r3, r3, r2
	lsls r3, r3, #2
	ldr r2, [sp, #36]
	adds r7, r3, r1
	mov r3, r9
	adds r3, #17
	cmp r2, r3
	bne .L_0814541a
	ldr r3, [sp, #52]
	adds r0, r4, #0
	ldr r1, [r3, #20]
	bl Math_Mod
	ldr r4, [sp, #52]
	lsls r0, r0, #1
	movs r3, #16
	adds r0, #36
	ldrsh r0, [r4, r0]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	ldr r3, [sp, #28]
	bl Func_0814cd48
	movs r0, #133
	bl Func_081180e8
.L_0814541a:
	ldr r3, [r7, #24]
	cmp r3, #0
	bge .L_08145422
	b .L_081455a4
.L_08145422:
	ldr r2, [sp, #36]
	mov r3, r9
	subs r0, r2, r3
	movs r1, #3
	bl Math_Div
	adds r6, r0, #0
	cmp r6, #9
	ble .L_08145436
	movs r6, #9
.L_08145436:
	add r5, sp, #88
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	ldr r0, [r5, #4]
	asrs r2, r3, #1
	adds r3, r0, #0
	subs r3, #8
	str r2, [r5]
	str r3, [r5, #4]
	cmp r6, #4
	ble .L_081454b4
	ldr r1, .L_081454b0
	mov r3, r11
	adds r3, #1
	ldrsb r4, [r1, r3]
	ldr r3, [sp, #44]
	lsls r1, r6, #1
	adds r1, r1, r6
	lsls r1, r1, #8
	adds r1, r3, r1
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r1, r3
	adds r3, r0, #0
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	ldr r0, [sp, #16]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	subs r2, #16
	subs r3, #20
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	b .L_081454e6
	.2byte 0x0000
.L_08145488:
	.4byte 0x0000011b
.L_0814548c:
	.4byte 0x00000163
.L_08145490:
	.4byte 0x00000150
.L_08145494:
	.4byte 0x0000017f
.L_08145498:
	.4byte 0x00000138
.L_0814549c:
	.4byte IwramCopyWords
.L_081454a0:
	.4byte Data_02010018
.L_081454a4:
	.4byte gMapCellBuffer
.L_081454a8:
	.4byte Data_02011c18
.L_081454ac:
	.4byte Func_08143000
.L_081454b0:
	.4byte Data_08197880
.L_081454b4:
	ldr r1, .L_0814575c
	mov r3, r11
	adds r3, #1
	ldrsb r4, [r1, r3]
	ldr r3, [sp, #44]
	lsls r1, r6, #1
	adds r1, r1, r6
	lsls r1, r1, #8
	adds r1, r3, r1
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r1, r3
	adds r3, r0, #0
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #32
	str r0, [sp, #4]
	ldr r0, [sp, #16]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	subs r2, #12
	subs r3, #24
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
.L_081454e6:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_081454f6
	adds r0, r7, #0
	movs r1, #63
	ldr r2, .L_08145760
	bl BattleFxKernels_IntegrateVector3
.L_081454f6:
	ldr r3, [r7, #4]
	cmp r3, #0
	bge .L_081455a4
	ldr r2, .L_0814575c
	movs r3, #0
	str r3, [r7, #4]
	mov r1, r11
	movs r3, #1
	str r3, [r7, #24]
	ldrsb r3, [r2, r1]
	movs r4, #4
	mov r10, r4
	cmp r3, #0
	beq .L_08145516
	movs r0, #16
	mov r10, r0
.L_08145516:
	movs r1, #0
	mov r3, r10
	mov r8, r1
	cmp r3, #0
	beq .L_081455a6
	ldr r4, [sp, #28]
	ldr r0, [sp, #32]
	lsls r2, r4, #2
	adds r2, r2, r0
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_08145764
	lsls r3, r3, #5
	movs r1, #63
	adds r5, r3, r2
.L_08145534:
	ldr r3, [r7]
	mov r4, r11
	str r3, [r5]
	ldr r3, [r7, #4]
	str r3, [r5, #4]
	ldr r3, [r7, #8]
	str r3, [r5, #8]
	ldr r3, .L_0814575c
	ldrsb r6, [r3, r4]
	cmp r6, #0
	bne .L_0814556a
	str r1, [sp, #8]
	bl Random16
	ldr r1, [sp, #8]
	str r6, [r5, #16]
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #11
	b .L_08145594
.L_0814556a:
	str r1, [sp, #8]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #13
.L_08145594:
	str r0, [r5, #20]
	movs r0, #1
	movs r3, #0
	add r8, r0
	str r3, [r5, #24]
	adds r5, #28
	cmp r8, r10
	bne .L_08145534
.L_081455a4:
	ldr r2, .L_0814575c
.L_081455a6:
	ldr r3, [sp, #32]
	ldr r4, .L_0814575c
	adds r3, #1
	str r3, [sp, #32]
	mov r3, r11
	adds r3, #2
	ldrsb r3, [r4, r3]
	ldr r0, [sp, #32]
	movs r1, #4
	add r9, r1
	cmp r0, r3
	beq .L_081455c0
	b .L_081453d0
.L_081455c0:
	ldr r3, [sp, #52]
	ldr r1, [r3, #20]
.L_081455c4:
	ldr r4, [sp, #28]
	adds r4, #1
	str r4, [sp, #28]
	cmp r4, r1
	beq .L_081455d0
	b .L_081453ba
.L_081455d0:
	movs r0, #0
	ldr r1, .L_08145764
	ldr r2, .L_0814575c
	str r0, [sp, #32]
	mov r8, r1
	mov r10, r2
.L_081455dc:
	mov r4, r8
	ldr r3, [r4, #24]
	cmp r3, #44
	bhi .L_081456bc
	ldr r3, [r4, #4]
	cmp r3, #0
	blt .L_081456bc
	add r6, sp, #88
	adds r1, r6, #0
	mov r0, r8
	bl Func_0815e1ec
	ldr r3, [r6]
	mov r0, r10
	mov r1, r11
	asrs r2, r3, #1
	ldrsb r3, [r0, r1]
	str r2, [r6]
	cmp r3, #0
	bne .L_0814563a
	mov r4, r8
	ldr r3, [r4, #24]
	cmp r3, #0
	bge .L_0814560e
	adds r3, #7
.L_0814560e:
	asrs r3, r3, #3
	lsls r1, r3, #3
	ldr r0, [sp, #44]
	adds r1, r1, r3
	lsls r1, r1, #7
	movs r3, #139
	adds r1, r0, r1
	lsls r3, r3, #7
	movs r0, #24
	adds r1, r1, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r0, [sp, #16]
	subs r2, #12
	ldr r4, [r0, #4]
	subs r3, #24
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	b .L_08145692
.L_0814563a:
	mov r1, r8
	ldr r0, [r1, #24]
	movs r1, #5
	bl Math_Div
	ldr r3, [sp, #32]
	movs r1, #1
	ands r3, r1
	cmp r3, #0
	beq .L_08145650
	adds r0, #9
.L_08145650:
	ldr r3, [sp, #52]
	mov r4, r8
	ldr r2, [r3, #4]
	ldr r3, [r4, #12]
	cmp r3, #0
	ble .L_0814565e
	eors r2, r1
.L_0814565e:
	lsls r7, r2, #2
	ldr r2, .L_08145768
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #44]
	movs r3, #148
	adds r1, r2, r1
	lsls r3, r3, #6
	adds r1, r1, r3
	ldr r3, .L_0814576c
	ldr r2, [r6]
	ldrb r5, [r3, r0]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_08145770
	ldrb r4, [r3, r0]
	ldr r3, [r6, #4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	ldr r0, [sp, #16]
	str r5, [sp, #0]
	str r4, [sp, #4]
	ldr r4, [r7, r0]
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
.L_08145692:
	mov r1, r10
	mov r2, r11
	ldrsb r3, [r1, r2]
	cmp r3, #0
	bne .L_081456aa
	movs r2, #128
	mov r0, r8
	movs r1, #62
	lsls r2, r2, #4
	bl BattleFxKernels_IntegrateVector3
	b .L_081456b4
.L_081456aa:
	mov r0, r8
	movs r1, #62
	ldr r2, .L_08145760
	bl BattleFxKernels_IntegrateVector3
.L_081456b4:
	mov r4, r8
	ldr r3, [r4, #24]
	adds r3, #1
	str r3, [r4, #24]
.L_081456bc:
	ldr r1, [sp, #32]
	movs r2, #128
	movs r0, #28
	adds r1, #1
	lsls r2, r2, #1
	add r8, r0
	str r1, [sp, #32]
	cmp r1, r2
	bne .L_081455dc
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #44]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #36]
	ldr r1, [sp, #24]
	adds r0, #1
	str r0, [sp, #36]
	cmp r0, r1
	beq .L_081456fc
	b .L_0814539e
.L_081456fc:
	ldr r0, .L_08145774
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #48]
	cmp r2, #3
	beq .L_08145718
	cmp r2, #5
	bne .L_0814574a
.L_08145718:
	movs r1, #240
	ldr r5, .L_08145778
	lsls r1, r1, #6
	ldr r0, .L_0814577c
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #40]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_08145780
	bl Scheduler_RemoveCallback
	ldr r3, [sp, #48]
	cmp r3, #3
	bne .L_08145742
	ldr r0, [sp, #52]
	bl Func_081504c0
	b .L_0814574e
.L_08145742:
	ldr r0, [sp, #52]
	bl Func_081504b4
	b .L_0814574e
.L_0814574a:
	bl Func_08143bb8
.L_0814574e:
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0814575c:
	.4byte Data_08197880
.L_08145760:
	.4byte 0xffff8000
.L_08145764:
	.4byte Data_02011c00
.L_08145768:
	.4byte Data_081978cc
.L_0814576c:
	.4byte Data_081978a8
.L_08145770:
	.4byte Data_081978ba
.L_08145774:
	.4byte Func_08143000
.L_08145778:
	.4byte IwramClearWords
.L_0814577c:
	.4byte 0x06004000
.L_08145780:
	.4byte Func_08143488
