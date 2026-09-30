.syntax unified
	.thumb
	.global Func_081843fc
	.thumb_func
Func_081843fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #272
	str r0, [sp, #172]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	ldr r2, .L_0818448c
	str r0, [sp, #168]
	mov r8, r2
	ldr r1, [r5, #92]
	str r1, [sp, #164]
	ldrh r3, [r2, #4]
	str r3, [sp, #152]
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	str r3, [sp, #148]
	ldr r4, [r5, #100]
	str r4, [sp, #144]
	ldr r6, [r5, #36]
	str r6, [sp, #140]
	ldr r0, [r6, #84]
	bl Resource_ResetEntry
	bl Func_08020380
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginCanvasLayer
	movs r6, #128
	ldr r3, .L_08184484
	lsls r6, r6, #19
	adds r6, #32
	strh r3, [r6]
	bl Func_0813ba50
	ldr r2, .L_08184488
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r7, [sp, #164]
	movs r0, #239
	lsls r0, r0, #7
	str r5, [sp, #156]
	adds r3, r7, r0
	movs r5, #0
	movs r1, #200
	str r5, [r3]
	lsls r1, r1, #4
	ldr r0, .L_08184490
	bl Scheduler_AddOrUpdateCallback
	b .L_08184494
	.2byte 0x0000
.L_08184484:
	.4byte 0x00000100
.L_08184488:
	.4byte 0x00000000
.L_0818448c:
	.4byte Data_03001120
.L_08184490:
	.4byte Func_08143000
.L_08184494:
	movs r0, #0
	movs r1, #0
	bl Func_08163c2c
	movs r3, #255
	add r0, sp, #244
	strh r3, [r0]
	movs r1, #0
	bl BattleActor_SpawnObjectsForListFar
	ldr r2, .L_0818450c
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, .L_08184510
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	movs r2, #31
	negs r2, r2
	adds r1, r2, #0
	adds r0, r2, #0
	bl Func_08164b2c
	ldr r1, [sp, #148]
	movs r3, #1
	str r3, [r1, #16]
	ldr r3, .L_081844fc
	mov r2, r8
	strh r5, [r2, #4]
	strh r3, [r6]
	movs r2, #128
	ldr r3, .L_08184500
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_08184504
	subs r2, #2
	strh r3, [r2]
	ldr r1, .L_08184514
	movs r7, #0
	movs r6, #0
.L_081844ee:
	cmp r7, #14
	ble .L_0818452c
	ldr r4, .L_08184508
	movs r2, #0
	adds r0, r6, #0
	b .L_08184518
	.2byte 0x0000
.L_081844fc:
	.4byte 0x00000080
.L_08184500:
	.4byte 0x00001010
.L_08184504:
	.4byte 0x00003f44
.L_08184508:
	.4byte 0x0000000f
.L_0818450c:
	.4byte gCameraSceneParameters
.L_08184510:
	.4byte 0x00000072
.L_08184514:
	.4byte 0x0600f800
.L_08184518:
	adds r3, r2, #0
	ands r3, r4
	adds r3, r0, r3
	adds r3, #48
	adds r2, #1
	strh r3, [r1]
	adds r1, #2
	cmp r2, #32
	bne .L_08184518
	b .L_08184546
.L_0818452c:
	ldr r5, .L_08184560
	ldr r4, .L_08184564
	movs r2, #0
	adds r0, r6, #0
.L_08184534:
	adds r3, r2, #0
	ands r3, r5
	adds r3, r0, r3
	adds r3, r3, r4
	adds r2, #1
	strh r3, [r1]
	adds r1, #2
	cmp r2, #32
	bne .L_08184534
.L_08184546:
	adds r7, #1
	adds r6, #32
	cmp r7, #30
	bne .L_081844ee
	ldr r0, .L_08184568
	bl Resource_GetTableEntry
	movs r2, #192
	adds r7, r0, #0
	ldr r5, .L_0818456c
	adds r1, r7, #0
	b .L_08184570
	.2byte 0x0000
.L_08184560:
	.4byte 0x0000000f
.L_08184564:
	.4byte 0x00000200
.L_08184568:
	.4byte 0x000000ab
.L_0818456c:
	.4byte IwramCopyWords
.L_08184570:
	lsls r2, r2, #1
	ldr r0, .L_0818489c
	mov lr, r5
	.2byte 0xf800
	movs r3, #192
	lsls r3, r3, #1
	adds r7, r7, r3
	adds r0, r7, #0
	ldr r1, .L_081848a0
	bl Func_0801587c
	ldr r6, .L_081848a4
	ldr r0, [sp, #164]
	movs r1, #238
	lsls r1, r1, #7
	movs r2, #13
	ldr r7, .L_081848a0
	movs r4, #0
	adds r1, #220
	negs r2, r2
	mov r10, r6
	mov r8, r5
	mov r9, r4
	adds r6, r0, r1
	adds r5, r2, #0
.L_081845a2:
	movs r1, #32
	ldr r2, .L_081848a8
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #8
	ands r3, r5
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r6!, {r0}
	lsls r3, r3, #2
	add r3, r10
	ldrh r0, [r3, #2]
	ldr r3, .L_081848ac
	movs r2, #128
	adds r1, r7, #0
	adds r0, r0, r3
	lsls r2, r2, #3
	mov lr, r8
	.2byte 0xf800
	movs r0, #1
	movs r4, #128
	add r9, r0
	lsls r4, r4, #3
	mov r1, r9
	adds r7, r7, r4
	cmp r1, #24
	bne .L_081845a2
	ldr r3, .L_081848a4
	ldr r4, [sp, #164]
	movs r0, #238
	lsls r0, r0, #7
	ldr r7, .L_081848b0
	ldr r6, .L_081848b4
	movs r2, #0
	adds r0, #220
	mov r9, r2
	mov r8, r3
	adds r5, r4, r0
.L_081845f6:
	movs r3, #0
	movs r1, #32
	ldr r2, .L_081848a8
	movs r0, #32
	bl Func_0815b3b0
	ldr r1, [r5]
	str r0, [r5, #96]
	movs r2, #24
	mov lr, r7
	.2byte 0xf800
	ldr r3, [r5, #96]
	ldr r1, .L_081848ac
	ldrb r3, [r3, #16]
	movs r2, #128
	lsls r3, r3, #2
	add r3, r8
	ldrh r0, [r3, #2]
	lsls r2, r2, #3
	adds r0, r0, r1
	adds r1, r6, #0
	mov lr, r7
	.2byte 0xf800
	movs r3, #1
	movs r2, #128
	add r9, r3
	lsls r2, r2, #3
	mov r4, r9
	adds r6, r6, r2
	adds r5, #4
	cmp r4, #4
	bne .L_081845f6
	ldr r5, .L_081848a0
	movs r2, #128
	lsls r2, r2, #5
	adds r0, r2, r5
	ldr r1, .L_081848b4
	ldr r5, .L_081848b0
	mov lr, r5
	.2byte 0xf800
	movs r0, #238
	ldr r7, [sp, #164]
	lsls r0, r0, #7
	movs r6, #0
	adds r0, #220
	mov r9, r6
	adds r6, r7, r0
.L_08184654:
	movs r1, #32
	ldr r2, .L_081848a8
	movs r3, #0
	movs r0, #32
	bl Func_0815b3b0
	movs r2, #24
	str r0, [r6, #112]
	ldmia r6!, {r1}
	mov lr, r5
	.2byte 0xf800
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #6
	bne .L_08184654
	ldr r0, .L_081848b8
	bl Resource_GetTableEntry
	adds r7, r0, #0
	ldr r6, .L_081848b0
	adds r1, r7, #0
	movs r2, #32
	ldr r0, .L_081848bc
	mov lr, r6
	.2byte 0xf800
	ldr r3, [sp, #164]
	movs r4, #224
	lsls r4, r4, #3
	adds r5, r3, r4
	adds r7, #32
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0801587c
	ldr r0, .L_081848a4
	ldr r1, [sp, #164]
	movs r2, #240
	lsls r2, r2, #7
	movs r3, #13
	movs r7, #0
	adds r2, #100
	negs r3, r3
	mov r9, r7
	mov r8, r6
	mov r10, r0
	adds r7, r1, r2
	adds r6, r3, #0
.L_081846b4:
	movs r2, #128
	movs r3, #192
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #4
	ands r3, r6
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r7!, {r0}
	lsls r3, r3, #2
	add r3, r10
	ldrh r0, [r3, #2]
	ldr r4, .L_081848ac
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #2
	adds r0, r0, r4
	mov lr, r8
	.2byte 0xf800
	movs r1, #1
	movs r0, #128
	add r9, r1
	lsls r0, r0, #2
	mov r2, r9
	adds r5, r5, r0
	cmp r2, #5
	bne .L_081846b4
	ldr r0, .L_081848c0
	bl Resource_GetTableEntry
	adds r7, r0, #0
	ldr r6, .L_081848b0
	adds r1, r7, #0
	movs r2, #32
	ldr r0, .L_081848c4
	mov lr, r6
	.2byte 0xf800
	ldr r3, [sp, #164]
	movs r4, #224
	lsls r4, r4, #3
	adds r5, r3, r4
	adds r7, #32
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0801587c
	ldr r0, .L_081848a4
	ldr r1, [sp, #164]
	movs r2, #240
	lsls r2, r2, #7
	movs r3, #13
	movs r7, #0
	adds r2, #120
	negs r3, r3
	mov r9, r7
	mov r8, r6
	mov r10, r0
	adds r7, r1, r2
	adds r6, r3, #0
.L_08184736:
	movs r2, #128
	movs r3, #208
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #4
	ands r3, r6
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r7!, {r0}
	lsls r3, r3, #2
	add r3, r10
	ldrh r0, [r3, #2]
	ldr r4, .L_081848ac
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #2
	adds r0, r0, r4
	mov lr, r8
	.2byte 0xf800
	movs r1, #1
	movs r0, #128
	add r9, r1
	lsls r0, r0, #2
	mov r2, r9
	adds r5, r5, r0
	cmp r2, #5
	bne .L_08184736
	ldr r0, .L_081848c8
	bl Resource_GetTableEntry
	adds r7, r0, #0
	ldr r6, .L_081848b0
	adds r1, r7, #0
	movs r2, #32
	ldr r0, .L_081848cc
	mov lr, r6
	.2byte 0xf800
	ldr r3, [sp, #164]
	movs r4, #224
	lsls r4, r4, #3
	adds r5, r3, r4
	adds r7, #32
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0801587c
	ldr r0, .L_081848a4
	ldr r1, [sp, #164]
	movs r2, #240
	lsls r2, r2, #7
	movs r3, #13
	movs r7, #0
	adds r2, #140
	negs r3, r3
	mov r9, r7
	mov r8, r6
	mov r10, r0
	adds r7, r1, r2
	adds r6, r3, #0
.L_081847b8:
	movs r2, #128
	movs r3, #224
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #4
	ands r3, r6
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r7!, {r0}
	lsls r3, r3, #2
	add r3, r10
	ldrh r0, [r3, #2]
	ldr r4, .L_081848ac
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #2
	adds r0, r0, r4
	mov lr, r8
	.2byte 0xf800
	movs r1, #1
	movs r0, #128
	add r9, r1
	lsls r0, r0, #2
	mov r2, r9
	adds r5, r5, r0
	cmp r2, #6
	bne .L_081847b8
	ldr r3, [sp, #164]
	movs r4, #174
	lsls r4, r4, #7
	movs r2, #240
	adds r0, r3, r4
	ldr r5, .L_081848b0
	ldr r1, .L_0818489c
	lsls r2, r2, #1
	mov lr, r5
	.2byte 0xf800
	ldr r6, [sp, #164]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r6, r7
	ldr r0, .L_081848d0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r2, #156
	lsls r2, r2, #6
	adds r1, r6, r2
	movs r3, #0
	movs r2, #0
	ldr r0, .L_081848d4
	bl Func_08157cf4
	ldr r0, .L_081848d8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_081848dc
	ldr r1, [sp, #144]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_081848e0
	ldr r1, .L_081848e4
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r5, #238
	movs r4, #239
	movs r3, #0
	lsls r4, r4, #7
	lsls r5, r5, #7
	str r3, [sp, #136]
	str r3, [sp, #132]
	str r3, [sp, #128]
	str r3, [sp, #124]
	str r3, [sp, #120]
	str r3, [sp, #116]
	adds r2, r6, r4
	movs r3, #2
	adds r5, #132
	str r3, [r2]
	adds r2, r6, r5
	movs r3, #50
	str r3, [r2]
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r7, .L_081848e8
	movs r6, #0
	mov r11, r6
	str r3, [sp, #156]
	str r7, [sp, #32]
	str r6, [sp, #28]
	bl Func_08185b9c
.L_0818489c:
	.4byte 0x05000200
.L_081848a0:
	.4byte gMapCellBuffer
.L_081848a4:
	.4byte ResourceTableEntries
.L_081848a8:
	.4byte 0x80002000
.L_081848ac:
	.4byte 0x06010000
.L_081848b0:
	.4byte IwramCopyWords
.L_081848b4:
	.4byte Data_02016000
.L_081848b8:
	.4byte 0x000000ae
.L_081848bc:
	.4byte 0x05000380
.L_081848c0:
	.4byte 0x000000ac
.L_081848c4:
	.4byte 0x050003a0
.L_081848c8:
	.4byte 0x000000ad
.L_081848cc:
	.4byte 0x050003c0
.L_081848d0:
	.4byte 0x000000f9
.L_081848d4:
	.4byte 0x000000f8
.L_081848d8:
	.4byte 0x00000188
.L_081848dc:
	.4byte 0x00000134
.L_081848e0:
	.4byte 0x000000b4
.L_081848e4:
	.4byte Data_02012000
.L_081848e8:
	.4byte 0xff9021c0
