.syntax unified
	.thumb
	.global Func_0815f16c
	.thumb_func
Func_0815f16c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #264
	str r1, [sp, #124]
	str r0, [sp, #128]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r5, [sp, #124]
	str r0, [sp, #120]
	ldr r1, [r3, #96]
	str r1, [sp, #116]
	ldr r2, [r3, #48]
	str r2, [sp, #108]
	ldr r3, [r3, #100]
	str r3, [sp, #104]
	movs r3, #0
	str r3, [sp, #100]
	str r3, [sp, #96]
	cmp r5, #41
	beq .L_0815f1b8
	cmp r5, #38
	beq .L_0815f1b8
	cmp r5, #62
	beq .L_0815f1b8
	cmp r5, #85
	beq .L_0815f1b8
	cmp r5, #86
	beq .L_0815f1b8
	cmp r5, #87
	beq .L_0815f1b8
	cmp r5, #88
	bne .L_0815f1c2
.L_0815f1b8:
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginTiledCanvas
	b .L_0815f1c8
.L_0815f1c2:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
.L_0815f1c8:
	ldr r3, .L_0815f208
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #104]
	ldr r0, .L_0815f20c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #120]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815f210
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0815f214
	ldr r1, .L_0815f218
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r5, [sp, #120]
	movs r0, #176
	lsls r0, r0, #7
	movs r3, #144
	b .L_0815f21c
	.2byte 0x0000
.L_0815f208:
	.4byte 0x00001010
.L_0815f20c:
	.4byte 0x00000134
.L_0815f210:
	.4byte 0x00000159
.L_0815f214:
	.4byte 0x0000015c
.L_0815f218:
	.4byte gMapCellBuffer
.L_0815f21c:
	adds r1, r5, r0
	lsls r3, r3, #1
	ldr r0, .L_0815f51c
	movs r2, #40
	bl Graphics_PackTileRows
	ldr r1, [sp, #124]
	cmp r1, #5
	beq .L_0815f236
	cmp r1, #53
	beq .L_0815f236
	cmp r1, #78
	bne .L_0815f23a
.L_0815f236:
	ldr r0, .L_0815f520
	b .L_0815f50c
.L_0815f23a:
	ldr r2, [sp, #124]
	cmp r2, #42
	beq .L_0815f244
	cmp r2, #15
	bne .L_0815f248
.L_0815f244:
	ldr r0, .L_0815f524
	b .L_0815f50c
.L_0815f248:
	ldr r3, [sp, #124]
	cmp r3, #16
	bne .L_0815f29a
	movs r5, #1
	mov r11, r5
	ldr r7, .L_0815f51c
	movs r5, #128
	lsls r5, r5, #3
	ldr r0, .L_0815f528
	ldr r1, .L_0815f51c
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	adds r6, r5, #0
	mov r12, r5
.L_0815f268:
	mov r1, r11
	lsls r4, r1, #3
	ldr r1, .L_0815f51c
	movs r0, #0
	adds r2, r5, r7
.L_0815f272:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, #0
	beq .L_0815f282
	subs r3, r3, r4
	cmp r3, #0
	bgt .L_0815f282
	movs r3, #1
.L_0815f282:
	strb r3, [r2]
	adds r0, #1
	adds r2, #1
	cmp r0, r12
	bne .L_0815f272
	movs r2, #1
	add r11, r2
	mov r3, r11
	adds r5, r5, r6
	cmp r3, #8
	bne .L_0815f268
	b .L_0815f5c6
.L_0815f29a:
	ldr r5, [sp, #124]
	cmp r5, #10
	bne .L_0815f2a4
	ldr r0, .L_0815f52c
	b .L_0815f50c
.L_0815f2a4:
	ldr r0, [sp, #124]
	cmp r0, #6
	beq .L_0815f2b6
	cmp r0, #57
	beq .L_0815f2b6
	cmp r0, #14
	beq .L_0815f2b6
	cmp r0, #17
	bne .L_0815f2c8
.L_0815f2b6:
	ldr r0, .L_0815f530
	ldr r1, .L_0815f51c
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0815f534
	ldr r1, .L_0815f538
	b .L_0815f50e
.L_0815f2c8:
	ldr r1, [sp, #124]
	cmp r1, #68
	bne .L_0815f2e0
	ldr r0, .L_0815f530
	ldr r1, .L_0815f51c
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0815f53c
	ldr r1, .L_0815f540
	b .L_0815f50e
.L_0815f2e0:
	ldr r2, [sp, #124]
	cmp r2, #61
	bne .L_0815f2ea
	ldr r0, .L_0815f544
	b .L_0815f5a4
.L_0815f2ea:
	ldr r3, [sp, #124]
	cmp r3, #38
	bne .L_0815f2f4
	ldr r0, .L_0815f548
	b .L_0815f5a4
.L_0815f2f4:
	ldr r5, [sp, #124]
	cmp r5, #44
	bne .L_0815f2fe
	ldr r0, .L_0815f54c
	b .L_0815f50c
.L_0815f2fe:
	ldr r0, [sp, #124]
	cmp r0, #60
	bne .L_0815f308
	ldr r0, .L_0815f530
	b .L_0815f50c
.L_0815f308:
	ldr r1, [sp, #124]
	cmp r1, #46
	bne .L_0815f312
	ldr r0, .L_0815f550
	b .L_0815f50c
.L_0815f312:
	ldr r2, [sp, #124]
	cmp r2, #50
	bne .L_0815f31c
	ldr r0, .L_0815f554
	b .L_0815f50c
.L_0815f31c:
	ldr r3, [sp, #124]
	cmp r3, #90
	bne .L_0815f32a
	ldr r0, .L_0815f558
	ldr r1, .L_0815f51c
	movs r2, #0
	b .L_0815f510
.L_0815f32a:
	ldr r5, [sp, #124]
	cmp r5, #91
	bne .L_0815f338
	ldr r0, .L_0815f55c
	ldr r1, .L_0815f560
	movs r2, #0
	b .L_0815f510
.L_0815f338:
	ldr r0, [sp, #124]
	cmp r0, #84
	bne .L_0815f3b6
	ldr r0, .L_0815f530
	ldr r1, .L_0815f51c
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #120]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815f534
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #128
	ldr r0, .L_0815f538
	ldr r3, .L_0815f564
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r5, #0
	mov r11, r5
	movs r7, #0
.L_0815f36e:
	mov r1, r11
	movs r6, #0
	lsls r5, r7, #6
	lsls r0, r1, #12
.L_0815f376:
	ldr r2, [sp, #120]
	movs r4, #0
	adds r3, r5, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r3, .L_0815f568
	adds r2, r0, r3
.L_0815f386:
	ldrb r3, [r1]
	adds r4, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r4, #24
	bne .L_0815f386
	adds r6, #1
	adds r5, #24
	adds r0, #32
	cmp r6, #120
	bne .L_0815f376
	movs r5, #1
	add r11, r5
	mov r0, r11
	adds r7, #45
	cmp r0, #4
	bne .L_0815f36e
	ldr r2, [sp, #120]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815f56c
	b .L_0815f50e
.L_0815f3b6:
	ldr r3, [sp, #124]
	subs r3, #63
	cmp r3, #1
	bhi .L_0815f3c2
	ldr r0, .L_0815f570
	b .L_0815f50c
.L_0815f3c2:
	ldr r5, [sp, #124]
	cmp r5, #71
	beq .L_0815f3cc
	cmp r5, #82
	bne .L_0815f3d4
.L_0815f3cc:
	ldr r0, .L_0815f574
	ldr r1, .L_0815f51c
	movs r2, #0
	b .L_0815f510
.L_0815f3d4:
	ldr r3, [sp, #124]
	subs r3, #96
	cmp r3, #2
	bhi .L_0815f3ee
	ldr r0, .L_0815f578
	ldr r1, .L_0815f51c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0815f520
	ldr r1, .L_0815f57c
	b .L_0815f50e
.L_0815f3ee:
	ldr r0, [sp, #124]
	cmp r0, #80
	bne .L_0815f458
	ldr r2, .L_0815f51c
	movs r5, #128
	lsls r5, r5, #5
	adds r1, r5, r2
	ldr r0, .L_0815f544
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, .L_0815f564
	adds r1, r5, #0
	ldr r0, .L_0815f51c
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0815f51c
	movs r6, #0
	movs r7, #0
	movs r5, #0
	mov r12, r3
.L_0815f41a:
	ldr r1, .L_0815f580
	mov r0, r12
	adds r3, r7, r6
	adds r2, r5, r0
	adds r0, r5, r1
	lsls r1, r3, #3
	movs r3, #128
	lsls r3, r3, #5
	add r1, r12
	adds r1, r1, r3
	movs r3, #156
	lsls r3, r3, #1
	adds r3, #255
	movs r4, #0
	adds r2, r2, r3
.L_0815f438:
	ldrb r3, [r1]
	adds r4, #1
	strb r3, [r0]
	adds r0, #1
	ldrb r3, [r1]
	adds r1, #1
	strb r3, [r2]
	subs r2, #1
	cmp r4, #24
	bne .L_0815f438
	adds r6, #1
	adds r7, #2
	adds r5, #64
	cmp r6, #48
	bne .L_0815f41a
	b .L_0815f5c6
.L_0815f458:
	ldr r5, [sp, #124]
	cmp r5, #79
	bne .L_0815f4da
	movs r2, #128
	lsls r2, r2, #9
	ldr r0, .L_0815f51c
	movs r1, #48
	bl Func_0815b434
	movs r2, #230
	movs r1, #128
	lsls r2, r2, #3
	movs r7, #136
	movs r0, #0
	lsls r1, r1, #4
	adds r2, #255
	lsls r7, r7, #5
	mov r12, r0
	mov r10, r1
	mov r8, r2
	adds r7, #208
	mov lr, r0
.L_0815f484:
	ldr r1, .L_0815f584
	mov r2, lr
	add r2, r12
	ldr r0, .L_0815f588
	lsls r3, r2, #4
	adds r4, r3, r1
	ldr r1, .L_0815f51c
	adds r5, r3, r0
	mov r0, r8
	adds r3, r7, r0
	adds r0, r3, r1
	adds r3, r7, #0
	add r3, r10
	lsls r2, r2, #3
	mov r9, r3
	adds r2, r2, r1
	movs r6, #0
	add r1, r9
.L_0815f4a8:
	ldrb r3, [r2]
	adds r6, #1
	strb r3, [r4]
	adds r4, #1
	ldrb r3, [r2]
	strb r3, [r5]
	subs r5, #1
	ldrb r3, [r2]
	strb r3, [r1]
	adds r1, #1
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r0]
	subs r0, #1
	cmp r6, #24
	bne .L_0815f4a8
	movs r0, #1
	add r12, r0
	movs r5, #2
	mov r1, r12
	subs r7, #48
	add lr, r5
	cmp r1, #48
	bne .L_0815f484
	b .L_0815f5c6
.L_0815f4da:
	ldr r2, [sp, #124]
	cmp r2, #94
	bne .L_0815f4f2
	ldr r0, .L_0815f58c
	ldr r1, .L_0815f51c
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_0815f590
	ldr r1, .L_0815f594
	b .L_0815f5a6
.L_0815f4f2:
	ldr r3, [sp, #124]
	cmp r3, #73
	bne .L_0815f504
	movs r2, #128
	ldr r3, .L_0815f518
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	b .L_0815f5c6
.L_0815f504:
	ldr r5, [sp, #124]
	cmp r5, #70
	bne .L_0815f59c
	ldr r0, .L_0815f598
.L_0815f50c:
	ldr r1, .L_0815f51c
.L_0815f50e:
	movs r2, #1
.L_0815f510:
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_0815f5c6
.L_0815f518:
	.4byte 0x00000c10
.L_0815f51c:
	.4byte gMapCellBuffer
.L_0815f520:
	.4byte 0x0000013e
.L_0815f524:
	.4byte 0x0000016d
.L_0815f528:
	.4byte 0x000000d9
.L_0815f52c:
	.4byte 0x00000105
.L_0815f530:
	.4byte 0x00000192
.L_0815f534:
	.4byte 0x00000188
.L_0815f538:
	.4byte Data_02010c56
.L_0815f53c:
	.4byte 0x00000161
.L_0815f540:
	.4byte Data_02011809
.L_0815f544:
	.4byte 0x0000013a
.L_0815f548:
	.4byte 0x00000187
.L_0815f54c:
	.4byte 0x00000130
.L_0815f550:
	.4byte 0x0000017c
.L_0815f554:
	.4byte 0x00000178
.L_0815f558:
	.4byte 0x000000da
.L_0815f55c:
	.4byte 0x000000c1
.L_0815f560:
	.4byte Data_02014000
.L_0815f564:
	.4byte IwramClearWords
.L_0815f568:
	.4byte Data_02010c5a
.L_0815f56c:
	.4byte 0x00000159
.L_0815f570:
	.4byte 0x00000115
.L_0815f574:
	.4byte 0x000000b4
.L_0815f578:
	.4byte 0x0000018a
.L_0815f57c:
	.4byte Data_02010d80
.L_0815f580:
	.4byte Data_02010208
.L_0815f584:
	.4byte Data_02010800
.L_0815f588:
	.4byte Data_0201082f
.L_0815f58c:
	.4byte 0x0000014d
.L_0815f590:
	.4byte 0x000000e6
.L_0815f594:
	.4byte Data_02012000
.L_0815f598:
	.4byte 0x00000193
.L_0815f59c:
	ldr r0, [sp, #124]
	cmp r0, #66
	bne .L_0815f5b0
	ldr r0, .L_0815f840
.L_0815f5a4:
	ldr r1, .L_0815f844
.L_0815f5a6:
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_0815f5c6
.L_0815f5b0:
	ldr r1, [sp, #124]
	cmp r1, #41
	beq .L_0815f5c6
	cmp r1, #62
	beq .L_0815f5c6
	ldr r0, .L_0815f848
	ldr r1, .L_0815f844
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_0815f5c6:
	ldr r2, [sp, #124]
	cmp r2, #100
	bls .L_0815f5ce
	b .L_0815f7fe
.L_0815f5ce:
	lsls r3, r2, #2
	ldr r2, .L_0815f84c
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0815f5d8:
	.4byte .L_0815f76c
	.4byte .L_0815f7bc
	.4byte .L_0815f79c
	.4byte .L_0815f7ac
	.4byte .L_0815f76c
	.4byte .L_0815f7ac
	.4byte .L_0815f7bc
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f76c
	.4byte .L_0815f79c
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f7fe
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f76c
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f79c
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f7ac
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7ac
	.4byte .L_0815f76c
	.4byte .L_0815f79c
	.4byte .L_0815f7ac
	.4byte .L_0815f79c
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f76c
	.4byte .L_0815f7bc
	.4byte .L_0815f7fe
	.4byte .L_0815f7bc
	.4byte .L_0815f7bc
	.4byte .L_0815f7ac
	.4byte .L_0815f7ac
	.4byte .L_0815f7bc
	.4byte .L_0815f7d6
	.4byte .L_0815f76c
	.4byte .L_0815f7d6
	.4byte .L_0815f7d6
	.4byte .L_0815f76c
	.4byte .L_0815f76c
	.4byte .L_0815f79c
	.4byte .L_0815f7f2
.L_0815f76c:
	ldr r0, .L_0815f850
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #128]
	ldr r3, [r5, #28]
	cmp r3, #1
	bne .L_0815f7fe
	ldr r0, .L_0815f854
	ldr r1, .L_0815f858
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_0815f7fe
.L_0815f79c:
	ldr r0, [sp, #128]
	ldr r3, [r0, #28]
	cmp r3, #1
	bne .L_0815f7a8
	ldr r0, .L_0815f85c
	b .L_0815f7c6
.L_0815f7a8:
	ldr r0, .L_0815f85c
	b .L_0815f7d8
.L_0815f7ac:
	ldr r1, [sp, #128]
	ldr r3, [r1, #28]
	cmp r3, #1
	bne .L_0815f7b8
	ldr r0, .L_0815f860
	b .L_0815f7c6
.L_0815f7b8:
	ldr r0, .L_0815f860
	b .L_0815f7d8
.L_0815f7bc:
	ldr r2, [sp, #128]
	ldr r3, [r2, #28]
	cmp r3, #1
	bne .L_0815f7d2
	ldr r0, .L_0815f864
.L_0815f7c6:
	ldr r1, .L_0815f858
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_0815f7fe
.L_0815f7d2:
	ldr r0, .L_0815f864
	b .L_0815f7d8
.L_0815f7d6:
	ldr r0, .L_0815f868
.L_0815f7d8:
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_0815f7fe
.L_0815f7f2:
	ldr r0, .L_0815f85c
	ldr r1, .L_0815f858
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_0815f7fe:
	ldr r3, [sp, #124]
	cmp r3, #81
	beq .L_0815f814
	cmp r3, #83
	beq .L_0815f814
	cmp r3, #91
	beq .L_0815f814
	cmp r3, #92
	beq .L_0815f814
	cmp r3, #93
	bne .L_0815f820
.L_0815f814:
	ldr r0, .L_0815f86c
	ldr r1, .L_0815f844
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_0815f820:
	ldr r5, [sp, #120]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r5, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #124]
	cmp r1, #42
	bne .L_0815f870
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r5, r3
	movs r3, #75
	b .L_0815f87c
	.2byte 0x0000
.L_0815f840:
	.4byte 0x00000129
.L_0815f844:
	.4byte gMapCellBuffer
.L_0815f848:
	.4byte 0x00000161
.L_0815f84c:
	.4byte .L_0815f5d8
.L_0815f850:
	.4byte 0x00000184
.L_0815f854:
	.4byte 0x00000157
.L_0815f858:
	.4byte Data_02013c56
.L_0815f85c:
	.4byte 0x00000155
.L_0815f860:
	.4byte 0x00000151
.L_0815f864:
	.4byte 0x00000153
.L_0815f868:
	.4byte 0x00000182
.L_0815f86c:
	.4byte 0x000000c2
.L_0815f870:
	ldr r5, [sp, #120]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r5, r0
	movs r3, #50
.L_0815f87c:
	str r3, [r2]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0815fafc
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #128]
	mov r3, sp
	adds r3, #240
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r3, #0
	str r3, [sp, #12]
	bl Func_0815e20c
	ldr r5, [sp, #128]
	mov r1, sp
	ldr r0, [r5, #8]
	adds r1, #252
	str r1, [sp, #88]
	bl Func_0815e20c
	mov r2, sp
	adds r2, #172
	ldr r0, [r5, #4]
	adds r1, r2, #0
	str r2, [sp, #84]
	bl Func_08144aac
	movs r5, #238
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #180
	adds r2, r3, r5
	movs r3, #24
	str r3, [r2]
	ldr r0, [sp, #120]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #184
	adds r2, r0, r1
	movs r3, #0
	str r3, [r2]
	ldr r2, [sp, #128]
	movs r6, #255
	ldr r0, [r2, #8]
	bl GetBattleObjectSlotFar
	ldr r7, [r0]
	ldr r5, [sp, #120]
	movs r3, #0
	lsls r6, r6, #8
	mov r11, r3
	mov r8, r3
	adds r6, #255
.L_0815f8ea:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	mov r0, r8
	str r3, [r5]
	str r0, [r5, #4]
	str r0, [r5, #8]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	add r11, r1
	ands r0, r6
	mov r2, r11
	str r0, [r5, #20]
	adds r5, #28
	cmp r2, #64
	bne .L_0815f8ea
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r3, sp
	adds r3, #228
	str r3, [sp, #16]
	ldr r5, [sp, #16]
	ldr r3, [r7, #8]
	movs r0, #160
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r1, [r7, #36]
	str r1, [sp, #80]
	ldr r2, [r7, #40]
	str r2, [sp, #76]
	ldr r3, [r7, #44]
	str r3, [sp, #72]
	ldr r5, [r7, #52]
	movs r3, #0
	str r5, [sp, #68]
	ldr r0, [r7, #72]
	str r0, [sp, #64]
	str r3, [r7, #36]
	str r3, [r7, #40]
	str r3, [r7, #44]
	str r3, [r7, #52]
	str r3, [r7, #72]
	ldr r1, [sp, #128]
	ldr r0, [r1, #8]
	ldr r1, [sp, #88]
	bl Func_0815e20c
	ldr r2, [sp, #88]
	ldr r5, [sp, #88]
	ldr r3, [r2]
	movs r0, #212
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
	bl Audio_PlayCue
	movs r0, #0
	movs r1, #204
	str r0, [sp, #112]
	add r1, sp
	mov r8, r1
.L_0815f98a:
	ldr r6, [sp, #120]
	movs r2, #0
	mov r10, r2
	mov r11, r2
.L_0815f992:
	ldr r3, [r6]
	cmp r3, #0
	blt .L_0815fa54
	mov r3, r11
	cmp r3, #0
	bge .L_0815f9a0
	adds r3, #3
.L_0815f9a0:
	ldr r5, [sp, #112]
	asrs r3, r3, #2
	cmp r5, r3
	blt .L_0815fa50
	movs r0, #5
	mov r9, r0
	bl Func_08014de4
	ldr r0, [r6, #20]
	bl Func_080150e4
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #16]
	bl Func_08015068
	add r5, sp, #204
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	ldr r1, [sp, #88]
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [r1]
	asrs r3, r3, #1
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #124]
	cmp r2, #37
	bgt .L_0815f9ec
	ldr r3, [r5, #4]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	subs r3, #8
	b .L_0815fa08
.L_0815f9ec:
	ldr r3, [sp, #124]
	cmp r3, #65
	bne .L_0815f9fe
	ldr r0, [sp, #88]
	ldr r3, [r5, #4]
	ldr r2, [r0, #4]
	adds r3, r3, r2
	adds r3, #44
	b .L_0815fa08
.L_0815f9fe:
	ldr r1, [sp, #88]
	ldr r3, [r5, #4]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	adds r3, #12
.L_0815fa08:
	str r3, [r5, #4]
	ldr r3, [r5, #8]
	movs r2, #60
	negs r2, r2
	cmp r3, r2
	bge .L_0815fa18
	str r2, [r5, #8]
	adds r3, r2, #0
.L_0815fa18:
	cmp r3, #60
	ble .L_0815fa20
	movs r3, #60
	str r3, [r5, #8]
.L_0815fa20:
	ldr r2, .L_0815fb00
	adds r3, #60
	str r3, [r5, #8]
	movs r3, #10
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #104]
	ldr r3, [r5, #4]
	adds r1, r2, r1
	movs r0, #10
	ldr r2, [r5]
	mov r5, r9
	str r5, [sp, #0]
	str r0, [sp, #4]
	ldr r5, [sp, #84]
	subs r3, #5
	subs r2, #2
	ldr r4, [r5, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6]
	subs r3, #4
	str r3, [r6]
.L_0815fa50:
	movs r0, #1
	add r10, r0
.L_0815fa54:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r6, #28
	cmp r2, #64
	bne .L_0815f992
	ldr r3, [sp, #124]
	cmp r3, #37
	bgt .L_0815faaa
	mov r5, r10
	cmp r5, #63
	bgt .L_0815faaa
	bl Func_08014de4
	ldr r0, [sp, #108]
	mov r5, r8
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	add r1, sp, #228
	adds r0, r1, #0
	mov r1, r8
	bl Func_0815e1ec
	mov r3, r8
	ldr r2, [r3]
	movs r1, #20
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5, #4]
	asrs r2, r2, #1
	str r2, [r5]
	str r1, [sp, #0]
	movs r1, #40
	str r1, [sp, #4]
	subs r2, #10
	subs r3, #4
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_0815fb04
	mov lr, r4
	.2byte 0xf800
.L_0815faaa:
	ldr r0, [sp, #120]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #112]
	adds r2, #1
	str r2, [sp, #112]
	cmp r2, #32
	beq .L_0815faca
	b .L_0815f98a
.L_0815faca:
	ldr r3, [sp, #124]
	cmp r3, #41
	bne .L_0815fb08
	movs r3, #128
	ldr r2, .L_0815faf8
	lsls r3, r3, #19
	adds r3, #32
	strh r2, [r3]
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0815faee
	ldr r3, [sp, #240]
	ldr r0, [sp, #112]
	movs r2, #128
	lsls r2, r2, #19
	subs r3, r0, r3
	b .L_0815fbbc
.L_0815faee:
	ldr r2, [sp, #240]
	movs r1, #128
	movs r3, #96
	b .L_0815fb22
	.2byte 0x0000
.L_0815faf8:
	.4byte 0x00000100
.L_0815fafc:
	.4byte Func_08143000
.L_0815fb00:
	.4byte Data_08197410
.L_0815fb04:
	.4byte Data_02013c56
.L_0815fb08:
	ldr r1, [sp, #124]
	cmp r1, #85
	bne .L_0815fb34
	ldr r2, .L_0815fb30
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #32
	strh r2, [r3]
	ldr r2, [sp, #128]
	ldr r3, [r2, #4]
	ldr r2, [sp, #240]
	movs r1, #128
	movs r3, #64
.L_0815fb22:
	lsls r1, r1, #19
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	b .L_0815fbc2
	.2byte 0x0000
.L_0815fb30:
	.4byte 0x00000100
.L_0815fb34:
	ldr r3, [sp, #124]
	cmp r3, #88
	bne .L_0815fb7c
	movs r3, #128
	ldr r2, .L_0815fb6c
	lsls r3, r3, #19
	adds r3, #32
	strh r2, [r3]
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0815fb58
	ldr r0, .L_0815fb70
	movs r1, #128
	lsls r1, r1, #10
	str r0, [sp, #100]
	str r1, [sp, #96]
	b .L_0815fb60
.L_0815fb58:
	ldr r2, .L_0815fb74
	ldr r3, .L_0815fb78
	str r2, [sp, #100]
	str r3, [sp, #96]
.L_0815fb60:
	ldr r5, [sp, #100]
	movs r2, #128
	lsls r2, r2, #19
	asrs r3, r5, #16
	b .L_0815fbbc
	.2byte 0x0000
.L_0815fb6c:
	.4byte 0x00000100
.L_0815fb70:
	.4byte 0xffc00000
.L_0815fb74:
	.4byte 0xffc80000
.L_0815fb78:
	.4byte 0xfffe0000
.L_0815fb7c:
	ldr r0, [sp, #124]
	cmp r0, #62
	beq .L_0815fb86
	cmp r0, #86
	bne .L_0815fbc2
.L_0815fb86:
	movs r3, #128
	ldr r2, .L_0815fba4
	lsls r3, r3, #19
	adds r3, #32
	strh r2, [r3]
	ldr r1, [sp, #128]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0815fbac
	ldr r2, .L_0815fba8
	movs r3, #192
	lsls r3, r3, #12
	str r2, [sp, #100]
	str r3, [sp, #96]
	b .L_0815fbb4
.L_0815fba4:
	.4byte 0x00000100
.L_0815fba8:
	.4byte 0xff800000
.L_0815fbac:
	ldr r0, .L_0815fc08
	movs r5, #0
	str r5, [sp, #100]
	str r0, [sp, #96]
.L_0815fbb4:
	ldr r1, [sp, #100]
	movs r2, #128
	lsls r2, r2, #19
	asrs r3, r1, #16
.L_0815fbbc:
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
.L_0815fbc2:
	ldr r2, [sp, #124]
	cmp r2, #38
	bne .L_0815fc2a
	ldr r3, .L_0815fc04
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [sp, #240]
	movs r1, #128
	movs r3, #64
	subs r3, r3, r2
	lsls r1, r1, #19
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	ldr r3, [sp, #120]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	ldr r0, [sp, #120]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r3, r0, r1
	movs r6, #0
	movs r1, #240
	str r6, [r3]
	ldr r5, .L_0815fc0c
	b .L_0815fc10
	.2byte 0x0000
.L_0815fc04:
	.4byte 0x00000100
.L_0815fc08:
	.4byte 0xfff40000
.L_0815fc0c:
	.4byte IwramClearWords
.L_0815fc10:
	lsls r1, r1, #6
	ldr r0, .L_0815fc50
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #116]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r6, [r3]
.L_0815fc2a:
	ldr r2, [sp, #124]
	cmp r2, #61
	bne .L_0815fc64
	movs r3, #128
	ldr r2, .L_0815fc4c
	lsls r3, r3, #19
	adds r3, #32
	strh r2, [r3]
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0815fc54
	ldr r2, [sp, #240]
	movs r1, #128
	movs r3, #32
	b .L_0815fc5a
	.2byte 0x0000
.L_0815fc4c:
	.4byte 0x00000100
.L_0815fc50:
	.4byte 0x06004000
.L_0815fc54:
	ldr r2, [sp, #240]
	movs r1, #128
	movs r3, #96
.L_0815fc5a:
	lsls r1, r1, #19
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
.L_0815fc64:
	ldr r0, [sp, #124]
	cmp r0, #45
	beq .L_0815fc76
	cmp r0, #47
	beq .L_0815fc76
	cmp r0, #54
	beq .L_0815fc76
	cmp r0, #56
	bne .L_0815fce2
.L_0815fc76:
	movs r1, #240
	ldr r5, .L_0815fe48
	lsls r1, r1, #6
	ldr r0, .L_0815fe4c
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #116]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r1, [sp, #128]
	movs r3, #0
	str r3, [r1, #28]
	ldr r0, .L_0815fe50
	bl Scheduler_RemoveCallback
	ldr r0, .L_0815fe54
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	adds r0, r7, #0
	movs r1, #3
	bl Object_SetMode
	ldr r2, [sp, #124]
	cmp r2, #45
	bne .L_0815fcc0
	ldr r0, [sp, #128]
	movs r1, #9
	bl BattleFx_RunProjectileVolley
.L_0815fcc0:
	ldr r3, [sp, #124]
	cmp r3, #54
	bne .L_0815fccc
	ldr r0, [sp, #128]
	bl Func_0814a7f0
.L_0815fccc:
	ldr r5, [sp, #124]
	cmp r5, #56
	beq .L_0815fcd6
	bl .L_081638ac
.L_0815fcd6:
	ldr r0, [sp, #128]
	movs r1, #8
	bl BattleFx_RunProjectileVolley
	bl .L_081638ac
.L_0815fce2:
	adds r0, r7, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r0, [sp, #80]
	str r0, [r7, #36]
	ldr r1, [sp, #76]
	str r1, [r7, #40]
	ldr r2, [sp, #72]
	str r2, [r7, #44]
	ldr r3, [sp, #68]
	str r3, [r7, #52]
	ldr r5, [sp, #64]
	str r5, [r7, #72]
	ldr r0, [sp, #124]
	cmp r0, #65
	bne .L_0815fd4a
	movs r1, #240
	ldr r5, .L_0815fe48
	lsls r1, r1, #6
	ldr r0, .L_0815fe4c
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #116]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r1, [sp, #128]
	movs r3, #0
	str r3, [r1, #28]
	ldr r0, .L_0815fe50
	bl Scheduler_RemoveCallback
	ldr r0, .L_0815fe54
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #128]
	movs r3, #3
	str r3, [r2, #24]
	ldr r0, [sp, #128]
	movs r1, #2
	bl BattleFx_RunSparkGroups
	bl .L_081638ac
.L_0815fd4a:
	movs r1, #240
	ldr r5, .L_0815fe48
	ldr r0, [sp, #116]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, .L_0815fe4c
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r3, [sp, #124]
	cmp r3, #79
	bne .L_0815fd96
	movs r5, #1
	mov r11, r5
	movs r5, #160
	lsls r5, r5, #19
	adds r5, #2
.L_0815fd70:
	mov r1, r11
	lsls r0, r1, #8
	bl Trig_Cos
	lsls r0, r0, #5
	asrs r0, r0, #16
	movs r1, #31
	subs r1, r1, r0
	lsls r2, r1, #5
	lsls r3, r1, #10
	orrs r3, r2
	movs r2, #1
	orrs r3, r1
	add r11, r2
	strh r3, [r5]
	mov r3, r11
	adds r5, #2
	cmp r3, #64
	bne .L_0815fd70
.L_0815fd96:
	ldr r1, [sp, #128]
	movs r5, #36
	ldrsh r0, [r1, r5]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r2, sp
	adds r2, #216
	ldr r5, [sp, #16]
	str r2, [sp, #56]
	str r0, [sp, #60]
	movs r1, #6
	ldr r3, [r5]
	ldr r0, [r0, #8]
	subs r0, r0, r3
	bl __divsi3
	ldr r1, [sp, #56]
	str r0, [r1]
	ldr r2, [sp, #60]
	ldr r3, [r5, #4]
	ldr r0, [r2, #12]
	movs r1, #6
	subs r0, r0, r3
	movs r3, #240
	lsls r3, r3, #13
	adds r0, r0, r3
	bl __divsi3
	ldr r5, [sp, #56]
	str r0, [r5, #4]
	ldr r1, [sp, #60]
	ldr r2, [sp, #16]
	ldr r0, [r1, #16]
	ldr r3, [r2, #8]
	movs r1, #6
	subs r0, r0, r3
	bl __divsi3
	movs r3, #0
	str r0, [r5, #8]
	mov r11, r3
	ldr r3, [sp, #120]
	movs r2, #0
	adds r3, #24
.L_0815fdf0:
	movs r5, #1
	add r11, r5
	mov r0, r11
	str r2, [r3]
	adds r3, #28
	cmp r0, #64
	bne .L_0815fdf0
	ldr r1, [sp, #124]
	cmp r1, #44
	beq .L_0815feb2
	ldr r3, [sp, #128]
	movs r5, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Battle_GetObjectTableValueFar
	mov r11, r5
	lsrs r3, r0, #31
	ldr r5, [sp, #120]
	adds r0, r0, r3
	asrs r7, r0, #1
	movs r6, #255
.L_0815fe1c:
	ldr r0, [sp, #60]
	ldr r3, [r0, #8]
	str r7, [r5, #4]
	str r3, [r5]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #124]
	cmp r1, #61
	bne .L_0815fe58
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #10
	b .L_0815fe6e
	.2byte 0x0000
.L_0815fe48:
	.4byte IwramClearWords
.L_0815fe4c:
	.4byte 0x06004000
.L_0815fe50:
	.4byte Func_08143488
.L_0815fe54:
	.4byte Func_08143000
.L_0815fe58:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #12
.L_0815fe6e:
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	ldr r3, [r5, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	ldr r3, [r5, #20]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r11
	str r3, [r5, #20]
	lsrs r3, r2, #31
	add r3, r11
	asrs r3, r3, #1
	adds r3, #32
	str r3, [r5, #24]
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r5, #28
	cmp r0, #32
	bne .L_0815fe1c
.L_0815feb2:
	ldr r1, [sp, #124]
	cmp r1, #41
	bne .L_0815fee0
	ldr r2, [sp, #120]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815ff14
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0815ff18
	ldr r1, .L_0815ff1c
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_0815ff0c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0815fee0:
	ldr r5, [sp, #124]
	cmp r5, #86
	bne .L_0815fefc
	movs r2, #1
	movs r3, #1
	ldr r0, .L_0815ff20
	ldr r1, .L_0815ff1c
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_0815ff10
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0815fefc:
	ldr r0, [sp, #124]
	cmp r0, #88
	bne .L_0815ff38
	movs r2, #1
	movs r3, #1
	ldr r0, .L_0815ff24
	b .L_0815ff28
	.2byte 0x0000
.L_0815ff0c:
	.4byte 0x00000e10
.L_0815ff10:
	.4byte 0x00000810
.L_0815ff14:
	.4byte 0x0000016f
.L_0815ff18:
	.4byte 0x00000170
.L_0815ff1c:
	.4byte gMapCellBuffer
.L_0815ff20:
	.4byte 0x000000f1
.L_0815ff24:
	.4byte 0x000000dd
.L_0815ff28:
	ldr r1, .L_0815ff70
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_0815ff68
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0815ff38:
	ldr r1, [sp, #124]
	cmp r1, #62
	bne .L_0815ff7c
	ldr r2, [sp, #120]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815ff74
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0815ff78
	ldr r1, .L_0815ff70
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_0815ff6c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	b .L_0815ff7c
.L_0815ff68:
	.4byte 0x00000810
.L_0815ff6c:
	.4byte 0x00000e10
.L_0815ff70:
	.4byte gMapCellBuffer
.L_0815ff74:
	.4byte 0x00000171
.L_0815ff78:
	.4byte 0x00000172
.L_0815ff7c:
	ldr r5, [sp, #124]
	cmp r5, #85
	bne .L_0815ffa8
	movs r2, #1
	movs r3, #1
	ldr r0, .L_0815ffa0
	ldr r1, .L_0815ffa4
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_0815ff9c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	b .L_0815ffa8
	.2byte 0x0000
.L_0815ff9c:
	.4byte 0x00000e10
.L_0815ffa0:
	.4byte 0x000000f0
.L_0815ffa4:
	.4byte gMapCellBuffer
.L_0815ffa8:
	ldr r0, [sp, #124]
	cmp r0, #7
	bne .L_0815ffb0
	b .L_081601a0
.L_0815ffb0:
	cmp r0, #43
	bne .L_0815ffb6
	b .L_081601a0
.L_0815ffb6:
	cmp r0, #48
	bne .L_0815ffbc
	b .L_081601a0
.L_0815ffbc:
	cmp r0, #73
	bne .L_0815ffc2
	b .L_081601a0
.L_0815ffc2:
	cmp r0, #77
	bne .L_0815ffc8
	b .L_081601a0
.L_0815ffc8:
	cmp r0, #78
	bne .L_0815ffce
	b .L_081601a0
.L_0815ffce:
	cmp r0, #41
	bne .L_0815ffd4
	b .L_081601a0
.L_0815ffd4:
	cmp r0, #62
	bne .L_0815ffda
	b .L_081601a0
.L_0815ffda:
	cmp r0, #49
	bne .L_0815ffe0
	b .L_081601a0
.L_0815ffe0:
	cmp r0, #12
	bne .L_0815ffe6
	b .L_081601a0
.L_0815ffe6:
	cmp r0, #83
	bne .L_0815ffec
	b .L_081601a0
.L_0815ffec:
	cmp r0, #84
	bne .L_0815fff2
	b .L_081601a0
.L_0815fff2:
	cmp r0, #85
	bne .L_0815fff8
	b .L_081601a0
.L_0815fff8:
	cmp r0, #86
	bne .L_0815fffe
	b .L_081601a0
.L_0815fffe:
	cmp r0, #88
	bne .L_08160004
	b .L_081601a0
.L_08160004:
	cmp r0, #66
	bne .L_08160060
	ldr r5, .L_081602a4
	movs r1, #0
	mov r11, r1
	movs r7, #255
.L_08160010:
	ldr r2, [sp, #60]
	adds r6, r5, #0
	ldr r3, [r2, #8]
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	ldr r3, [r2, #8]
	cmp r3, #0
	bge .L_08160034
	bl Random16
	ands r0, r7
	adds r0, #128
	negs r0, r0
	b .L_0816003c
.L_08160034:
	bl Random16
	ands r0, r7
	adds r0, #128
.L_0816003c:
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	movs r3, #0
	ands r0, r7
	subs r0, #127
	str r3, [r6, #20]
	str r3, [r6, #24]
	movs r3, #1
	lsls r0, r0, #10
	add r11, r3
	str r0, [r6, #16]
	mov r0, r11
	adds r5, #28
	cmp r0, #64
	bne .L_08160010
	b .L_081601a0
.L_08160060:
	ldr r1, [sp, #124]
	cmp r1, #42
	beq .L_0816006e
	movs r7, #160
	lsls r7, r7, #13
	cmp r1, #15
	bne .L_08160070
.L_0816006e:
	movs r7, #0
.L_08160070:
	ldr r5, .L_081602a4
	movs r2, #0
	mov r11, r2
	movs r6, #255
.L_08160078:
	ldr r0, [sp, #60]
	ldr r3, [r0, #8]
	str r7, [r5, #4]
	str r3, [r5]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #124]
	cmp r1, #5
	beq .L_0816008e
	cmp r1, #53
	bne .L_081600b0
.L_0816008e:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	b .L_08160168
.L_081600b0:
	ldr r3, [sp, #124]
	subs r3, #68
	cmp r3, #1
	bls .L_081600ce
	ldr r2, [sp, #124]
	cmp r2, #74
	beq .L_081600ce
	cmp r2, #75
	beq .L_081600ce
	cmp r2, #16
	beq .L_081600ce
	cmp r2, #97
	beq .L_081600ce
	cmp r2, #10
	bne .L_081600f2
.L_081600ce:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #13
	b .L_08160168
.L_081600f2:
	ldr r3, [sp, #124]
	cmp r3, #98
	bne .L_0816011c
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #14
	b .L_08160168
.L_0816011c:
	ldr r0, [sp, #124]
	cmp r0, #55
	bne .L_08160146
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	b .L_08160168
.L_08160146:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #10
.L_08160168:
	str r0, [r5, #20]
	ldr r3, [r5, #12]
	cmp r3, #0
	bge .L_08160172
	adds r3, #3
.L_08160172:
	asrs r3, r3, #2
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	cmp r3, #0
	bge .L_0816017e
	adds r3, #3
.L_0816017e:
	asrs r3, r3, #2
	str r3, [r5, #16]
	ldr r3, [r5, #20]
	cmp r3, #0
	bge .L_0816018a
	adds r3, #3
.L_0816018a:
	movs r1, #1
	asrs r3, r3, #2
	add r11, r1
	str r3, [r5, #20]
	mov r2, r11
	movs r3, #0
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	beq .L_081601a0
	b .L_08160078
.L_081601a0:
	ldr r3, [sp, #124]
	subs r3, #2
	str r3, [sp, #52]
	cmp r3, #1
	bls .L_081601cc
	ldr r5, [sp, #124]
	cmp r5, #42
	beq .L_081601cc
	cmp r5, #15
	beq .L_081601cc
	cmp r5, #52
	beq .L_081601cc
	cmp r5, #72
	beq .L_081601cc
	cmp r5, #59
	beq .L_081601cc
	cmp r5, #58
	beq .L_081601cc
	cmp r5, #69
	beq .L_081601cc
	cmp r5, #89
	bne .L_081601d6
.L_081601cc:
	movs r1, #200
	ldr r0, .L_081602a8
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_081601d6:
	ldr r0, [sp, #124]
	subs r0, #4
	str r0, [sp, #48]
	cmp r0, #2
	bls .L_08160226
	ldr r1, [sp, #124]
	cmp r1, #17
	beq .L_08160226
	cmp r1, #53
	beq .L_08160226
	cmp r1, #60
	beq .L_08160226
	cmp r1, #57
	beq .L_08160226
	cmp r1, #14
	beq .L_08160226
	cmp r1, #63
	beq .L_08160226
	cmp r1, #64
	beq .L_08160226
	cmp r1, #100
	beq .L_08160226
	cmp r1, #83
	beq .L_08160226
	cmp r1, #90
	beq .L_08160226
	cmp r1, #91
	beq .L_08160226
	cmp r1, #76
	beq .L_08160226
	cmp r1, #66
	beq .L_08160226
	cmp r1, #92
	beq .L_08160226
	cmp r1, #93
	beq .L_08160226
	cmp r1, #96
	beq .L_08160226
	cmp r1, #97
	bne .L_0816022c
.L_08160226:
	movs r2, #32
	str r2, [sp, #92]
	b .L_08160310
.L_0816022c:
	ldr r3, [sp, #124]
	cmp r3, #3
	bls .L_0816027e
	cmp r3, #13
	beq .L_0816027e
	cmp r3, #38
	beq .L_0816027e
	cmp r3, #39
	beq .L_0816027e
	cmp r3, #74
	beq .L_0816027e
	cmp r3, #75
	beq .L_0816027e
	cmp r3, #40
	beq .L_0816027e
	cmp r3, #52
	beq .L_0816027e
	cmp r3, #72
	beq .L_0816027e
	cmp r3, #69
	beq .L_0816027e
	cmp r3, #55
	beq .L_0816027e
	cmp r3, #59
	beq .L_0816027e
	cmp r3, #61
	beq .L_0816027e
	cmp r3, #44
	beq .L_0816027e
	cmp r3, #70
	beq .L_0816027e
	cmp r3, #86
	beq .L_0816027e
	cmp r3, #88
	beq .L_0816027e
	cmp r3, #89
	beq .L_0816027e
	cmp r3, #16
	beq .L_0816027e
	cmp r3, #10
	bne .L_08160284
.L_0816027e:
	movs r5, #48
	str r5, [sp, #92]
	b .L_08160310
.L_08160284:
	ldr r0, [sp, #124]
	cmp r0, #51
	beq .L_0816029e
	cmp r0, #67
	beq .L_0816029e
	cmp r0, #9
	beq .L_0816029e
	cmp r0, #11
	beq .L_0816029e
	cmp r0, #95
	beq .L_0816029e
	cmp r0, #99
	bne .L_081602ac
.L_0816029e:
	movs r1, #20
	str r1, [sp, #92]
	b .L_08160310
.L_081602a4:
	.4byte Data_02016000
.L_081602a8:
	.4byte Func_08152474
.L_081602ac:
	ldr r2, [sp, #124]
	cmp r2, #41
	beq .L_081602ce
	cmp r2, #62
	beq .L_081602ce
	cmp r2, #50
	beq .L_081602ce
	cmp r2, #81
	beq .L_081602ce
	cmp r2, #84
	beq .L_081602ce
	cmp r2, #91
	beq .L_081602ce
	cmp r2, #79
	beq .L_081602ce
	cmp r2, #98
	bne .L_081602d4
.L_081602ce:
	movs r3, #40
	str r3, [sp, #92]
	b .L_08160310
.L_081602d4:
	ldr r0, [sp, #124]
	movs r5, #72
	str r5, [sp, #92]
	cmp r0, #85
	beq .L_08160310
	ldr r1, [sp, #124]
	cmp r1, #58
	beq .L_081602f0
	cmp r1, #42
	beq .L_081602f0
	cmp r1, #15
	beq .L_081602f0
	cmp r1, #68
	bne .L_081602f6
.L_081602f0:
	movs r2, #64
	str r2, [sp, #92]
	b .L_08160310
.L_081602f6:
	ldr r3, [sp, #124]
	cmp r3, #8
	beq .L_0816030c
	cmp r3, #71
	beq .L_0816030c
	cmp r3, #80
	beq .L_0816030c
	movs r5, #80
	str r5, [sp, #92]
	cmp r3, #94
	bne .L_08160310
.L_0816030c:
	movs r0, #54
	str r0, [sp, #92]
.L_08160310:
	ldr r2, [sp, #92]
	movs r1, #0
	str r1, [sp, #112]
	cmp r2, #0
	bne .L_0816031e
	bl .L_081637d4
.L_0816031e:
	ldr r3, [sp, #124]
	cmp r3, #41
	beq .L_0816036a
	cmp r3, #62
	beq .L_0816036a
	cmp r3, #83
	beq .L_0816036a
	cmp r3, #85
	beq .L_0816036a
	cmp r3, #86
	beq .L_0816036a
	cmp r3, #88
	beq .L_0816036a
	ldr r5, [sp, #120]
	ldr r2, [sp, #112]
	movs r0, #225
	lsls r0, r0, #7
	movs r1, #0
	movs r7, #128
	adds r6, r5, r0
	mov r11, r1
	lsls r7, r7, #11
	lsls r5, r2, #12
.L_0816034c:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #2
	subs r0, r7, r0
	asrs r0, r0, #10
	stmia r6!, {r0}
	movs r0, #1
	movs r3, #128
	add r11, r0
	lsls r3, r3, #4
	mov r1, r11
	adds r5, r5, r3
	cmp r1, #160
	bne .L_0816034c
.L_0816036a:
	ldr r2, [sp, #124]
	cmp r2, #98
	bls .L_08160372
	b .L_0816059e
.L_08160372:
	lsls r3, r2, #2
	ldr r2, .L_08160644
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0816037c:
	.4byte .L_08160540
	.4byte .L_08160568
	.4byte .L_08160560
	.4byte .L_08160570
	.4byte .L_0816059e
	.4byte .L_08160580
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160508
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160568
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160538
	.4byte .L_08160578
	.4byte .L_08160540
	.4byte .L_0816059e
	.4byte .L_08160548
	.4byte .L_08160548
	.4byte .L_08160528
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160550
	.4byte .L_08160558
	.4byte .L_08160570
	.4byte .L_0816059e
	.4byte .L_08160570
	.4byte .L_08160580
	.4byte .L_0816059e
	.4byte .L_08160548
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160568
	.4byte .L_08160560
	.4byte .L_0816059e
	.4byte .L_08160530
	.4byte .L_0816059e
	.4byte .L_08160520
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160510
	.4byte .L_0816059e
	.4byte .L_08160528
	.4byte .L_08160518
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160590
	.4byte .L_08160540
	.4byte .L_08160560
	.4byte .L_0816059e
	.4byte .L_08160550
	.4byte .L_08160570
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160588
	.4byte .L_08160560
	.4byte .L_08160578
	.4byte .L_08160598
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160568
	.4byte .L_08160588
	.4byte .L_08160560
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_0816059e
	.4byte .L_08160588
.L_08160508:
	ldr r0, .L_08160648
	bl Func_0815f0a0
	b .L_0816059e
.L_08160510:
	ldr r0, .L_0816064c
	bl Func_0815f0a0
	b .L_0816059e
.L_08160518:
	ldr r0, .L_08160650
	bl Func_0815f0a0
	b .L_0816059e
.L_08160520:
	ldr r0, .L_08160654
	bl Func_0815f0a0
	b .L_0816059e
.L_08160528:
	ldr r0, .L_08160658
	bl Func_0815f0a0
	b .L_0816059e
.L_08160530:
	ldr r0, .L_0816065c
	bl Func_0815f0a0
	b .L_0816059e
.L_08160538:
	ldr r0, .L_08160660
	bl Func_0815f0a0
	b .L_0816059e
.L_08160540:
	ldr r0, .L_08160664
	bl Func_0815f0a0
	b .L_0816059e
.L_08160548:
	ldr r0, .L_08160668
	bl Func_0815f0a0
	b .L_0816059e
.L_08160550:
	ldr r0, .L_0816066c
	bl Func_0815f0a0
	b .L_0816059e
.L_08160558:
	ldr r0, .L_08160670
	bl Func_0815f0a0
	b .L_0816059e
.L_08160560:
	ldr r0, .L_08160674
	bl Func_0815f0a0
	b .L_0816059e
.L_08160568:
	ldr r0, .L_08160678
	bl Func_0815f0a0
	b .L_0816059e
.L_08160570:
	ldr r0, .L_0816067c
	bl Func_0815f0a0
	b .L_0816059e
.L_08160578:
	ldr r0, .L_08160680
	bl Func_0815f0a0
	b .L_0816059e
.L_08160580:
	ldr r0, .L_08160684
	bl Func_0815f0a0
	b .L_0816059e
.L_08160588:
	ldr r0, .L_08160688
	bl Func_0815f0a0
	b .L_0816059e
.L_08160590:
	ldr r0, .L_0816068c
	bl Func_0815f0a0
	b .L_0816059e
.L_08160598:
	ldr r0, .L_08160690
	bl Func_0815f0a0
.L_0816059e:
	ldr r3, [sp, #112]
	cmp r3, #5
	bgt .L_081605c0
	ldr r5, [sp, #128]
	ldr r1, [sp, #88]
	ldr r0, [r5, #8]
	bl Func_0815e20c
	ldr r0, [sp, #88]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r0]
	ldr r3, [r0, #4]
	adds r3, #8
	str r3, [r0, #4]
.L_081605c0:
	ldr r1, [sp, #124]
	cmp r1, #41
	bne .L_081605c8
	b .L_08160862
.L_081605c8:
	cmp r1, #38
	bne .L_081605ce
	b .L_081606ca
.L_081605ce:
	cmp r1, #62
	beq .L_081606ca
	cmp r1, #63
	beq .L_081606ca
	cmp r1, #64
	beq .L_081606ca
	cmp r1, #79
	beq .L_081606ca
	cmp r1, #70
	beq .L_081606ca
	cmp r1, #82
	beq .L_081606ca
	cmp r1, #85
	beq .L_081606ca
	cmp r1, #86
	beq .L_081606ca
	cmp r1, #88
	beq .L_081606ca
	cmp r1, #89
	beq .L_081606ca
	cmp r1, #94
	beq .L_081606ca
	cmp r1, #96
	beq .L_081606ca
	ldr r2, [sp, #112]
	cmp r2, #11
	bgt .L_081606ca
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_08160694
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	ldr r0, [sp, #120]
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #88]
	lsls r1, r1, #7
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #3
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r3, [r3, #4]
	movs r0, #48
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	subs r2, #28
	subs r3, #40
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	b .L_081606ca
	.2byte 0x0000
.L_08160644:
	.4byte .L_0816037c
.L_08160648:
	.4byte 0x00000105
.L_0816064c:
	.4byte 0x00000129
.L_08160650:
	.4byte 0x000000b9
.L_08160654:
	.4byte 0x00000115
.L_08160658:
	.4byte 0x00000130
.L_0816065c:
	.4byte 0x0000013a
.L_08160660:
	.4byte 0x00000187
.L_08160664:
	.4byte 0x00000150
.L_08160668:
	.4byte 0x0000017f
.L_0816066c:
	.4byte 0x0000017d
.L_08160670:
	.4byte 0x00000184
.L_08160674:
	.4byte 0x00000167
.L_08160678:
	.4byte 0x00000166
.L_0816067c:
	.4byte 0x00000178
.L_08160680:
	.4byte 0x00000163
.L_08160684:
	.4byte 0x0000013e
.L_08160688:
	.4byte 0x00000148
.L_0816068c:
	.4byte 0x00000147
.L_08160690:
	.4byte 0x00000169
.L_08160694:
	ldr r5, [sp, #112]
	ldr r0, [sp, #120]
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #88]
	lsls r1, r1, #7
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #3
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r3, [r3, #4]
	movs r0, #48
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	subs r2, #20
	subs r3, #40
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_081606ca:
	ldr r5, [sp, #124]
	cmp r5, #41
	bne .L_081606d2
	b .L_08160862
.L_081606d2:
	cmp r5, #38
	bne .L_081606d8
	b .L_08160862
.L_081606d8:
	cmp r5, #62
	bne .L_081606de
	b .L_08160862
.L_081606de:
	cmp r5, #83
	bne .L_081606e4
	b .L_08160862
.L_081606e4:
	cmp r5, #85
	bne .L_081606ea
	b .L_08160862
.L_081606ea:
	cmp r5, #86
	bne .L_081606f0
	b .L_08160862
.L_081606f0:
	cmp r5, #88
	bne .L_081606f6
	b .L_08160862
.L_081606f6:
	cmp r5, #94
	bne .L_081606fc
	b .L_08160862
.L_081606fc:
	ldr r2, [sp, #112]
	subs r2, #4
	cmp r2, #11
	bhi .L_0816073e
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #4
	ldr r0, [sp, #120]
	subs r1, r1, r3
	ldr r3, [sp, #12]
	lsls r1, r1, #6
	movs r2, #176
	adds r1, r0, r1
	lsls r2, r2, #7
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r5, [sp, #88]
	lsrs r3, r2, #31
	movs r0, #20
	adds r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	asrs r2, r2, #1
	ldr r4, [r0, #4]
	subs r2, #8
	subs r3, #24
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_0816073e:
	bl Func_08014de4
	ldr r1, [sp, #108]
	ldr r0, [sp, #108]
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r1, [sp, #112]
	cmp r1, #3
	bgt .L_08160754
	b .L_08160862
.L_08160754:
	ldr r2, [sp, #124]
	cmp r2, #79
	bne .L_081607d8
	movs r5, #192
	ldr r7, [sp, #120]
	movs r3, #0
	add r5, sp
	mov r11, r3
	mov r10, r5
.L_08160766:
	ldr r3, [r7, #24]
	mov r8, r7
	cmp r3, #0
	ble .L_081607ca
	asrs r3, r3, #2
	adds r5, r3, #1
	cmp r5, #10
	ble .L_08160778
	movs r5, #10
.L_08160778:
	mov r6, r10
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	ldr r1, .L_08160ad0
	lsrs r3, r2, #31
	adds r2, r2, r3
	lsls r0, r5, #1
	asrs r2, r2, #1
	str r2, [r6]
	subs r3, r0, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #104]
	ldr r4, [sp, #172]
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	subs r3, r3, r5
	str r0, [sp, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [sp, #124]
	movs r2, #0
	cmp r5, #89
	beq .L_081607ba
	ldr r2, .L_08160ad4
.L_081607ba:
	mov r0, r8
	movs r1, #62
	bl BattleFxKernels_IntegrateVector3
	mov r0, r8
	ldr r3, [r0, #24]
	subs r3, #2
	str r3, [r0, #24]
.L_081607ca:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r7, #28
	cmp r2, #64
	bne .L_08160766
	b .L_08160862
.L_081607d8:
	movs r5, #192
	movs r3, #0
	add r5, sp
	mov r11, r3
	mov r8, r5
.L_081607e2:
	mov r0, r11
	lsrs r3, r0, #31
	add r3, r11
	asrs r4, r3, #1
	lsls r3, r4, #3
	ldr r1, [sp, #120]
	subs r3, r3, r4
	lsls r3, r3, #2
	adds r7, r1, r3
	ldr r0, [r7, #24]
	cmp r0, #0
	ble .L_08160858
	movs r1, #20
	str r4, [sp, #8]
	bl __divsi3
	mov r6, r8
	adds r5, r0, #0
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	ldr r4, [sp, #8]
	lsrs r3, r2, #31
	adds r5, #2
	adds r2, r2, r3
	ldr r1, .L_08160ad0
	movs r0, #1
	asrs r2, r2, #1
	ands r0, r4
	lsls r4, r5, #1
	str r2, [r6]
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #104]
	lsls r0, r0, #2
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	subs r3, r3, r5
	str r4, [sp, #4]
	ldr r5, [sp, #84]
	ldr r4, [r0, r5]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #62
	ldr r2, .L_08160ad4
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_08160858:
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #64
	bne .L_081607e2
.L_08160862:
	ldr r2, [sp, #124]
	cmp r2, #7
	beq .L_0816088a
	cmp r2, #43
	beq .L_0816088a
	cmp r2, #48
	beq .L_0816088a
	cmp r2, #73
	beq .L_0816088a
	cmp r2, #77
	beq .L_0816088a
	cmp r2, #78
	beq .L_0816088a
	cmp r2, #49
	beq .L_0816088a
	cmp r2, #12
	beq .L_0816088a
	cmp r2, #82
	beq .L_0816088a
	b .L_08160a2e
.L_0816088a:
	ldr r3, [sp, #112]
	movs r7, #12
	cmp r3, #12
	bne .L_08160898
	movs r0, #142
	bl Audio_PlayCue
.L_08160898:
	ldr r5, [sp, #124]
	cmp r5, #78
	bne .L_081608a0
	movs r7, #0
.L_081608a0:
	ldr r0, [sp, #112]
	cmp r0, #50
	bne .L_081608ba
	ldr r1, [sp, #128]
	movs r3, #1
	movs r2, #0
	negs r3, r3
	ldr r0, [r1, #8]
	str r2, [sp, #0]
	movs r1, #7
	adds r2, r3, #0
	bl Func_0814cd48
.L_081608ba:
	ldr r2, [sp, #112]
	cmp r2, #79
	bne .L_081608d4
	ldr r3, [sp, #128]
	movs r2, #0
	ldr r0, [r3, #8]
	movs r3, #1
	negs r3, r3
	str r2, [sp, #0]
	movs r1, #0
	adds r2, r3, #0
	bl Func_0814cd48
.L_081608d4:
	ldr r5, [sp, #112]
	cmp r5, r7
	bne .L_0816093c
	ldr r5, .L_08160ad8
	movs r0, #0
	mov r11, r0
	movs r6, #255
.L_081608e2:
	ldr r1, [sp, #60]
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
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r3, r0, #10
	str r3, [r5, #20]
	ldr r2, [sp, #124]
	cmp r2, #78
	bne .L_0816092c
	ldr r3, [r5, #12]
	lsls r3, r3, #1
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	lsls r3, r3, #1
	str r3, [r5, #16]
	lsls r3, r0, #11
	str r3, [r5, #20]
.L_0816092c:
	movs r3, #0
	str r3, [r5, #24]
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r5, #28
	cmp r0, #64
	bne .L_081608e2
.L_0816093c:
	ldr r1, [sp, #112]
	cmp r1, r7
	blt .L_08160a2e
	ldr r2, [sp, #128]
	ldr r7, .L_08160ad8
	ldr r0, [r2, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r3, [sp, #128]
	mov r8, r0
	ldr r0, [r3, #8]
	bl Battle_GetObjectTableValueFar
	ldr r5, [sp, #124]
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r3, #78
	eors r3, r5
	negs r2, r3
	orrs r2, r3
	lsrs r2, r2, #31
	movs r3, #1
	asrs r0, r0, #1
	subs r2, r3, r2
	movs r1, #192
	str r0, [sp, #44]
	add r1, sp
	movs r0, #0
	lsls r2, r2, #2
	mov r11, r0
	mov r9, r1
	mov r10, r2
.L_0816097e:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_08160a22
	mov r6, r9
	mov r2, r11
	adds r1, r6, #0
	movs r5, #1
	adds r0, r7, #0
	ands r5, r2
	bl Func_0815e1ec
	ldr r2, [r6]
	adds r5, #6
	ldr r1, .L_08160ad0
	lsls r0, r5, #1
	asrs r2, r2, #1
	str r2, [r6]
	subs r3, r0, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #104]
	adds r1, r3, r1
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	subs r3, r3, r5
	mov r5, r10
	ldr r4, [r5, r0]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r1, #62
	adds r0, r7, #0
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	ldr r1, [sp, #112]
	mov r3, r11
	adds r3, #22
	cmp r1, r3
	ble .L_08160a22
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r3, [r7]
	ldr r1, [r2, #12]
	subs r0, r0, r3
	ldr r3, [sp, #44]
	asrs r0, r0, #8
	adds r1, r1, r3
	ldr r3, [r7, #4]
	movs r5, #240
	subs r1, r1, r3
	ldr r3, [r2, #16]
	ldr r2, [r7, #8]
	asrs r1, r1, #8
	subs r3, r3, r2
	asrs r4, r3, #8
	ldr r3, [r7, #12]
	lsls r5, r5, #4
	adds r3, r3, r0
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	movs r2, #248
	adds r3, r3, r1
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	adds r5, #255
	lsls r2, r2, #5
	adds r3, r3, r4
	adds r0, r0, r5
	adds r2, #254
	str r3, [r7, #20]
	cmp r0, r2
	bhi .L_08160a22
	adds r3, r4, r5
	cmp r3, r2
	bhi .L_08160a22
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_08160a22:
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #32
	bne .L_0816097e
.L_08160a2e:
	ldr r2, [sp, #124]
	cmp r2, #7
	bne .L_08160a38
	bl .L_0816342a
.L_08160a38:
	cmp r2, #43
	bne .L_08160a40
	bl .L_0816342a
.L_08160a40:
	cmp r2, #48
	bne .L_08160a48
	bl .L_0816342a
.L_08160a48:
	cmp r2, #73
	bne .L_08160a50
	bl .L_0816342a
.L_08160a50:
	cmp r2, #77
	bne .L_08160a58
	bl .L_0816342a
.L_08160a58:
	cmp r2, #49
	bne .L_08160a60
	bl .L_0816342a
.L_08160a60:
	cmp r2, #12
	bne .L_08160a68
	bl .L_0816342a
.L_08160a68:
	cmp r2, #66
	bne .L_08160ae0
	ldr r5, .L_08160ad8
	movs r3, #0
	mov r11, r3
	add r7, sp, #192
.L_08160a74:
	ldr r0, [sp, #112]
	mov r3, r11
	adds r3, #4
	cmp r0, r3
	blt .L_08160ac0
	ldr r3, [r5, #24]
	cmp r3, #23
	bgt .L_08160ac0
	adds r6, r7, #0
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	movs r1, #40
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	asrs r2, r2, #1
	str r2, [r6]
	str r1, [sp, #0]
	movs r1, #64
	subs r3, #32
	str r1, [sp, #4]
	subs r2, #20
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_08160adc
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08160ac0:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r5, #28
	cmp r2, #16
	bne .L_08160a74
	bl .L_0816342a
.L_08160ad0:
	.4byte Data_08197410
.L_08160ad4:
	.4byte 0xfffff800
.L_08160ad8:
	.4byte Data_02016000
.L_08160adc:
	.4byte gMapCellBuffer
.L_08160ae0:
	ldr r3, [sp, #124]
	cmp r3, #51
	bne .L_08160aea
	bl .L_0816342a
.L_08160aea:
	cmp r3, #67
	bne .L_08160af2
	bl .L_0816342a
.L_08160af2:
	cmp r3, #9
	bne .L_08160afa
	bl .L_0816342a
.L_08160afa:
	cmp r3, #95
	bne .L_08160b02
	bl .L_0816342a
.L_08160b02:
	cmp r3, #99
	bne .L_08160b0a
	bl .L_0816342a
.L_08160b0a:
	cmp r3, #6
	beq .L_08160b1c
	cmp r3, #57
	beq .L_08160b1c
	cmp r3, #14
	beq .L_08160b1c
	cmp r3, #17
	beq .L_08160b1c
	b .L_08160d10
.L_08160b1c:
	ldr r5, [sp, #124]
	cmp r5, #17
	bne .L_08160b42
	ldr r0, [sp, #112]
	cmp r0, #4
	bne .L_08160b36
	ldr r1, [sp, #120]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #12
	str r3, [r2]
.L_08160b36:
	ldr r5, [sp, #112]
	cmp r5, #8
	bne .L_08160b42
	movs r0, #221
	bl Audio_PlayCue
.L_08160b42:
	ldr r0, [sp, #124]
	cmp r0, #6
	beq .L_08160b4c
	cmp r0, #57
	bne .L_08160b9e
.L_08160b4c:
	ldr r3, [sp, #112]
	subs r3, #6
	cmp r3, #13
	bhi .L_08160c1e
	ldr r5, [sp, #112]
	movs r1, #0
	mov r11, r1
.L_08160b5a:
	lsrs r3, r5, #31
	adds r3, r5, r3
	movs r2, #3
	asrs r3, r3, #1
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r1, r2, #4
	ldr r3, [sp, #12]
	subs r1, r1, r2
	ldr r2, .L_08160e74
	lsls r1, r1, #6
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r4, [sp, #172]
	lsrs r3, r2, #31
	adds r2, r2, r3
	movs r3, #24
	str r3, [sp, #0]
	asrs r2, r2, #1
	movs r3, #104
	str r3, [sp, #4]
	subs r2, #8
	ldr r0, [sp, #116]
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r5, #3
	cmp r1, #2
	bne .L_08160b5a
	b .L_08160c1e
.L_08160b9e:
	ldr r2, [sp, #124]
	cmp r2, #17
	bne .L_08160c1e
	ldr r3, [sp, #112]
	subs r3, #6
	cmp r3, #13
	bhi .L_08160c1e
	ldr r5, [sp, #112]
	movs r3, #0
	mov r11, r3
	lsls r3, r5, #3
	subs r3, r3, r5
	lsls r2, r3, #5
	adds r3, r3, r2
	mov r8, r5
	lsls r7, r3, #3
.L_08160bbe:
	mov r0, r8
	lsrs r6, r0, #31
	add r6, r8
	movs r3, #3
	adds r0, r7, #0
	asrs r6, r6, #1
	ands r6, r3
	bl Trig_Sin
	ldr r1, [sp, #12]
	lsls r0, r0, #5
	ldr r5, [r1]
	asrs r0, r0, #16
	lsrs r3, r5, #31
	adds r5, r5, r3
	asrs r5, r5, #1
	adds r5, r5, r0
	adds r0, r7, #0
	bl Trig_Cos
	lsls r3, r6, #1
	adds r3, r3, r6
	lsls r1, r3, #4
	ldr r2, .L_08160e74
	subs r1, r1, r3
	movs r3, #24
	subs r5, #8
	str r3, [sp, #0]
	lsls r1, r1, #6
	movs r3, #104
	adds r1, r1, r2
	str r3, [sp, #4]
	adds r2, r5, #0
	movs r3, #0
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	movs r5, #128
	add r11, r0
	movs r3, #3
	lsls r5, r5, #7
	mov r1, r11
	add r8, r3
	adds r7, r7, r5
	cmp r1, #4
	bne .L_08160bbe
.L_08160c1e:
	ldr r3, [sp, #112]
	subs r3, #8
	cmp r3, #15
	bls .L_08160c2a
	bl .L_0816342a
.L_08160c2a:
	ldr r2, [sp, #124]
	cmp r2, #14
	bne .L_08160c3a
	movs r3, #12
	movs r5, #24
	str r3, [sp, #36]
	str r5, [sp, #40]
	b .L_08160c42
.L_08160c3a:
	movs r0, #0
	movs r1, #32
	str r0, [sp, #40]
	str r1, [sp, #36]
.L_08160c42:
	movs r2, #0
	mov r11, r2
	movs r7, #3
.L_08160c48:
	mov r8, r11
	mov r3, r8
	ands r3, r7
	mov r8, r3
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, r0, #0
	ldr r0, [sp, #12]
	ldr r1, .L_08160e78
	ldr r3, [r0]
	lsls r6, r6, #3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r8
	asrs r6, r6, #16
	adds r6, r6, r3
	ldrb r3, [r1, r2]
	adds r0, r5, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Trig_Cos
	ldr r3, [sp, #36]
	ldr r1, .L_08160e7c
	mov r2, r8
	adds r5, r3, #0
	muls r5, r0
	ldr r0, [sp, #40]
	ldrb r3, [r1, r2]
	asrs r5, r5, #16
	lsrs r3, r3, #1
	adds r5, r5, r0
	movs r0, #188
	mov r10, r1
	subs r5, r5, r3
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Random16
	ldr r3, .L_08160e80
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #0
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r2, .L_08160e84
	mov r0, r8
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_08160e88
	adds r5, #56
	adds r1, r1, r2
	ldr r2, .L_08160e78
	ldrb r3, [r2, r0]
	mov r2, r10
	str r3, [sp, #0]
	ldrb r3, [r2, r0]
	ldr r0, [sp, #116]
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	adds r2, r6, #0
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #128]
	ldr r1, [sp, #84]
	ldr r0, [r5, #4]
	bl Func_08144aac
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #3
	bne .L_08160c48
	bl .L_0816342a
.L_08160d10:
	ldr r2, [sp, #124]
	cmp r2, #68
	beq .L_08160d18
	b .L_08160e98
.L_08160d18:
	ldr r3, [sp, #112]
	subs r3, #8
	cmp r3, #15
	bhi .L_08160de6
	movs r3, #0
	mov r11, r3
	movs r7, #3
.L_08160d26:
	bl Random16
	mov r8, r11
	mov r5, r8
	movs r3, #255
	ands r5, r7
	lsls r3, r3, #8
	mov r8, r5
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, r0, #0
	ldr r0, [sp, #12]
	ldr r1, .L_08160e78
	ldr r3, [r0]
	lsls r6, r6, #4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r8
	asrs r6, r6, #16
	adds r6, r6, r3
	ldrb r3, [r1, r2]
	adds r0, r5, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Trig_Cos
	ldr r3, .L_08160e7c
	adds r5, r0, #0
	mov r0, r8
	mov r10, r3
	ldrb r3, [r3, r0]
	lsls r5, r5, #4
	lsrs r3, r3, #1
	movs r0, #188
	asrs r5, r5, #16
	subs r5, r5, r3
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Random16
	ldr r3, .L_08160e8c
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #1
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r2, .L_08160e84
	mov r1, r8
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_08160e88
	ldr r0, .L_08160e78
	adds r1, r1, r2
	mov r2, r8
	ldrb r3, [r0, r2]
	mov r0, r10
	str r3, [sp, #0]
	adds r5, #80
	ldrb r3, [r0, r2]
	movs r2, #192
	str r3, [sp, #4]
	lsls r2, r2, #18
	adds r2, #188
	ldr r4, [r2]
	adds r3, r5, #0
	ldr r0, [sp, #116]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #128]
	movs r5, #1
	ldr r0, [r3, #4]
	ldr r1, [sp, #84]
	add r11, r5
	bl Func_08144aac
	mov r0, r11
	cmp r0, #3
	bne .L_08160d26
.L_08160de6:
	movs r2, #192
	ldr r7, .L_08160e90
	movs r1, #0
	add r2, sp
	mov r11, r1
	mov r8, r2
.L_08160df2:
	ldr r5, [sp, #112]
	mov r3, r11
	adds r3, #4
	cmp r5, r3
	blt .L_08160e52
	ldr r5, [r7, #24]
	cmp r5, #23
	bgt .L_08160e52
	cmp r5, #0
	bge .L_08160e08
	adds r5, #3
.L_08160e08:
	mov r6, r8
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	asrs r5, r5, #2
	lsls r1, r5, #3
	ldr r0, .L_08160e94
	lsrs r3, r2, #31
	adds r1, r1, r5
	adds r2, r2, r3
	lsls r1, r1, #7
	adds r1, r1, r0
	asrs r2, r2, #1
	movs r0, #24
	str r2, [r6]
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r5, [sp, #84]
	subs r3, #24
	subs r2, #12
	ldr r4, [r5, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_08160e52:
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #16
	bne .L_08160df2
	ldr r2, [sp, #112]
	cmp r2, #6
	beq .L_08160e68
	bl .L_0816342a
.L_08160e68:
	movs r0, #221
	bl Audio_PlayCue
	bl .L_0816342a
	.2byte 0x0000
.L_08160e74:
	.4byte Data_02010c56
.L_08160e78:
	.4byte Data_08197492
.L_08160e7c:
	.4byte Data_08197498
.L_08160e80:
	.4byte Data_0819887e
.L_08160e84:
	.4byte Data_08197486
.L_08160e88:
	.4byte gMapCellBuffer
.L_08160e8c:
	.4byte Data_08198882
.L_08160e90:
	.4byte Data_02016000
.L_08160e94:
	.4byte Data_02011809
.L_08160e98:
	ldr r3, [sp, #124]
	cmp r3, #44
	beq .L_08160ea0
	b .L_08160fd8
.L_08160ea0:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #112]
	cmp r5, #23
	bls .L_08160eb6
	bl .L_0816186e
.L_08160eb6:
	ldr r0, [sp, #12]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r9, r3
	lsls r3, r5, #5
	subs r3, #232
	mov r11, r3
	lsls r3, r5, #4
	adds r7, r3, #0
	mov r1, r11
	subs r7, #48
	cmp r1, #0
	ble .L_08160ed8
	movs r2, #0
	mov r11, r2
.L_08160ed8:
	cmp r7, #104
	ble .L_08160ee2
.L_08160edc:
	subs r7, #104
	cmp r7, #104
	bgt .L_08160edc
.L_08160ee2:
	movs r1, #19
	movs r0, #188
	bl Func_081963ec
	movs r5, #8
	negs r5, r5
	add r5, r9
	movs r3, #192
	mov r0, r11
	mov r10, r5
	adds r0, r0, r7
	movs r6, #17
	movs r5, #104
	lsls r3, r3, #18
	str r0, [sp, #32]
	str r5, [sp, #4]
	str r6, [sp, #0]
	adds r3, #188
	mov r8, r3
	mov r1, r8
	adds r3, r0, #0
	ldr r4, [r1]
	ldr r0, [sp, #116]
	subs r3, #104
	ldr r1, .L_0816105c
	mov r2, r10
	mov lr, r4
	.2byte 0xf800
	subs r5, r5, r7
	str r5, [sp, #4]
	str r6, [sp, #0]
	mov r2, r8
	ldr r3, [sp, #32]
	ldr r4, [r2]
	ldr r0, [sp, #116]
	ldr r1, .L_0816105c
	mov r2, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #34
	str r0, [sp, #0]
	movs r0, #65
	str r0, [sp, #4]
	mov r2, r9
	mov r3, r11
	mov r5, r8
	ldr r1, .L_08161060
	subs r2, #17
	adds r3, #47
	ldr r4, [r5]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #112]
	cmp r0, #8
	bne .L_08160f64
	ldr r1, [sp, #120]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	adds r3, r1, r2
	str r0, [r3]
.L_08160f64:
	ldr r3, [sp, #112]
	cmp r3, #1
	bgt .L_08160f6e
	bl .L_0816186e
.L_08160f6e:
	movs r5, #0
	mov r11, r5
	ldr r5, [sp, #120]
	movs r7, #0
	movs r6, #255
.L_08160f78:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_08160fc8
	ldr r0, [sp, #60]
	adds r7, #1
	ldr r3, [r0, #8]
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	mov r1, r11
	lsrs r3, r1, #31
	ands r0, r6
	add r3, r11
	subs r0, #127
	asrs r3, r3, #1
	lsls r0, r0, #12
	adds r3, #32
	str r0, [r5, #20]
	str r3, [r5, #24]
	cmp r7, #4
	bne .L_08160fc8
	bl .L_0816186e
.L_08160fc8:
	movs r2, #1
	add r11, r2
	mov r3, r11
	adds r5, #28
	cmp r3, #64
	bne .L_08160f78
	bl .L_0816186e
.L_08160fd8:
	ldr r0, [sp, #124]
	cmp r0, #61
	bne .L_08161064
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #112]
	subs r3, #4
	cmp r3, #19
	bls .L_08160ff6
	bl .L_08161ab4
.L_08160ff6:
	ldr r1, [sp, #12]
	movs r2, #48
	ldr r5, [r1]
	movs r0, #188
	lsrs r3, r5, #31
	movs r1, #19
	mov r10, r2
	adds r5, r5, r3
	bl Func_081963ec
	movs r6, #24
	mov r0, r10
	str r0, [sp, #4]
	str r6, [sp, #0]
	movs r3, #192
	asrs r5, r5, #1
	lsls r3, r3, #18
	adds r3, #188
	adds r2, r5, #0
	ldr r4, [r3]
	subs r2, #24
	ldr r0, [sp, #116]
	ldr r1, .L_0816105c
	mov r8, r3
	movs r3, #48
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	mov r1, r10
	str r1, [sp, #4]
	str r6, [sp, #0]
	mov r2, r8
	ldr r4, [r2]
	ldr r0, [sp, #116]
	ldr r1, .L_0816105c
	adds r2, r5, #0
	movs r3, #48
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	bl .L_08161ab4
	.2byte 0x0000
.L_0816105c:
	.4byte gMapCellBuffer
.L_08161060:
	.4byte Data_020106e8
.L_08161064:
	ldr r5, [sp, #124]
	cmp r5, #60
	bne .L_08161112
	ldr r0, [sp, #112]
	cmp r0, #15
	ble .L_08161080
	ldr r2, .L_081610ac
	ldr r1, .L_081610b0
	movs r3, #128
	lsls r3, r3, #19
	subs r2, r2, r0
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08161080:
	ldr r1, [sp, #112]
	cmp r1, #5
	bgt .L_0816108a
	bl .L_0816342a
.L_0816108a:
	ldr r2, [sp, #12]
	lsrs r0, r1, #31
	ldr r6, [r2]
	adds r0, r1, r0
	lsrs r3, r6, #31
	movs r1, #3
	asrs r0, r0, #1
	adds r6, r6, r3
	bl __modsi3
	lsls r5, r0, #2
	adds r5, r5, r0
	ldr r0, .L_081610b4
	lsls r3, r5, #9
	asrs r6, r6, #1
	adds r1, r3, r0
	b .L_081610b8
.L_081610ac:
	.4byte 0x00000020
.L_081610b0:
	.4byte 0x00001000
.L_081610b4:
	.4byte Data_02010c56
.L_081610b8:
	subs r6, #20
	str r3, [sp, #28]
	movs r2, #40
	movs r3, #32
	mov r10, r2
	str r2, [sp, #0]
	mov r8, r3
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	adds r2, r6, #0
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
	ldr r0, .L_08161358
	lsls r5, r5, #8
	adds r5, r5, r0
	mov r1, r10
	mov r2, r8
	str r1, [sp, #0]
	str r2, [sp, #4]
	adds r1, r5, #0
	ldr r4, [sp, #172]
	adds r2, r6, #0
	movs r3, #48
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r5, .L_0816135c
	ldr r3, [sp, #28]
	mov r0, r10
	adds r5, r3, r5
	mov r1, r8
	str r0, [sp, #0]
	str r1, [sp, #4]
	str r5, [sp, #24]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	adds r1, r5, #0
	adds r2, r6, #0
	movs r3, #80
	mov lr, r4
	.2byte 0xf800
	bl .L_0816342a
.L_08161112:
	ldr r2, [sp, #124]
	cmp r2, #5
	beq .L_08161120
	cmp r2, #53
	beq .L_08161120
	cmp r2, #78
	bne .L_081611a6
.L_08161120:
	ldr r3, [sp, #112]
	cmp r3, #23
	ble .L_0816112a
	bl .L_0816342a
.L_0816112a:
	movs r0, #192
	ldr r7, .L_08161360
	movs r5, #0
	add r0, sp
	mov r11, r5
	mov r8, r0
.L_08161136:
	mov r1, r11
	lsrs r3, r1, #31
	add r3, r11
	ldr r2, [sp, #112]
	asrs r3, r3, #1
	adds r3, #4
	cmp r2, r3
	blt .L_08161196
	ldr r3, [r7, #24]
	cmp r3, #11
	bgt .L_08161196
	mov r6, r8
	lsrs r5, r3, #31
	adds r1, r6, #0
	adds r0, r7, #0
	adds r5, r3, r5
	bl Func_0815e1ec
	ldr r2, [r6]
	asrs r5, r5, #1
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_08161364
	lsls r5, r5, #11
	adds r5, r5, r3
	ldr r3, [r6, #4]
	asrs r2, r2, #1
	movs r1, #32
	str r2, [r6]
	str r1, [sp, #0]
	movs r1, #64
	subs r3, #32
	str r1, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_08161196:
	movs r5, #1
	add r11, r5
	mov r0, r11
	adds r7, #28
	cmp r0, #16
	bne .L_08161136
	bl .L_0816342a
.L_081611a6:
	ldr r1, [sp, #124]
	cmp r1, #4
	bne .L_081611b0
	bl .L_0816342a
.L_081611b0:
	cmp r1, #8
	bne .L_081611b8
	bl .L_08163432
.L_081611b8:
	cmp r1, #41
	beq .L_081611be
	b .L_08161384
.L_081611be:
	ldr r2, [sp, #112]
	lsls r0, r2, #9
	bl Trig_Cos
	ldr r1, [sp, #12]
	lsls r0, r0, #2
	movs r5, #6
	ldrsh r3, [r1, r5]
	asrs r0, r0, #16
	ldr r2, [sp, #112]
	adds r3, r3, r0
	adds r5, r3, #0
	adds r5, #16
	cmp r2, #3
	bgt .L_08161202
	ldr r3, [sp, #120]
	movs r0, #224
	lsls r0, r0, #3
	adds r1, r3, r0
	ldr r3, [sp, #128]
	ldr r0, .L_08161368
	ldr r2, [r3, #4]
	ldr r4, [sp, #172]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r0, r3]
	ldr r3, .L_0816136c
	movs r0, #57
	ldrb r3, [r3]
	str r0, [sp, #0]
	movs r0, #98
	str r0, [sp, #4]
	adds r3, r5, r3
	b .L_0816134e
.L_08161202:
	ldr r0, [sp, #112]
	cmp r0, #7
	bgt .L_08161232
	ldr r2, [sp, #120]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r3, [sp, #128]
	ldr r0, .L_08161368
	ldr r2, [r3, #4]
	ldr r4, [sp, #172]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r0, r3]
	ldr r3, .L_0816136c
	movs r0, #57
	ldrb r3, [r3]
	str r0, [sp, #0]
	movs r0, #98
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_08161232:
	ldr r0, [sp, #120]
	ldr r3, [sp, #128]
	movs r2, #224
	lsls r2, r2, #5
	adds r2, #210
	adds r1, r0, r2
	ldr r2, [r3, #4]
	ldr r7, .L_08161368
	lsls r3, r2, #3
	ldr r6, .L_0816136c
	subs r3, r3, r2
	adds r3, #1
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #1]
	movs r0, #99
	str r0, [sp, #0]
	movs r0, #69
	adds r3, r5, r3
	str r0, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [sp, #112]
	subs r3, #4
	cmp r3, #1
	bhi .L_08161276
	movs r1, #128
	ldr r3, .L_08161370
	ldr r0, [sp, #116]
	lsls r1, r1, #7
	ldr r2, .L_08161374
	mov lr, r3
	.2byte 0xf800
.L_08161276:
	ldr r3, [sp, #112]
	subs r3, #6
	cmp r3, #1
	bhi .L_081612a8
	ldr r0, [sp, #120]
	ldr r3, [sp, #128]
	movs r2, #220
	lsls r2, r2, #6
	adds r2, #129
	adds r1, r0, r2
	ldr r2, [r3, #4]
	movs r0, #128
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #2
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #2]
	str r0, [sp, #0]
	movs r0, #91
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_081612a8:
	ldr r3, [sp, #112]
	subs r3, #8
	cmp r3, #1
	bhi .L_081612d2
	ldr r0, [sp, #128]
	movs r1, #128
	ldr r2, [r0, #4]
	ldr r4, [sp, #172]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #3
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #3]
	str r1, [sp, #0]
	movs r1, #91
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r0, [sp, #116]
	ldr r1, .L_08161364
	mov lr, r4
	.2byte 0xf800
.L_081612d2:
	ldr r3, [sp, #112]
	subs r3, #10
	cmp r3, #1
	bhi .L_081612fc
	ldr r3, [sp, #128]
	movs r0, #128
	ldr r2, [r3, #4]
	ldr r1, .L_08161378
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #4
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #59
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_081612fc:
	ldr r3, [sp, #112]
	subs r3, #12
	cmp r3, #1
	bhi .L_08161326
	ldr r0, [sp, #128]
	ldr r1, .L_0816137c
	ldr r2, [r0, #4]
	movs r0, #122
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #5
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #5]
	str r0, [sp, #0]
	movs r0, #29
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_08161326:
	ldr r3, [sp, #112]
	subs r3, #14
	cmp r3, #1
	bls .L_08161332
	bl .L_0816342a
.L_08161332:
	ldr r3, [sp, #128]
	movs r0, #76
	ldr r2, [r3, #4]
	ldr r1, .L_08161380
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #6
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #6]
	str r0, [sp, #0]
	movs r0, #25
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #172]
.L_0816134e:
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	bl .L_0816342a
.L_08161358:
	.4byte Data_02012a56
.L_0816135c:
	.4byte Data_02011156
.L_08161360:
	.4byte Data_02016000
.L_08161364:
	.4byte gMapCellBuffer
.L_08161368:
	.4byte Data_0819750c
.L_0816136c:
	.4byte Data_0819751a
.L_08161370:
	.4byte IwramFillWords
.L_08161374:
	.4byte 0x3f3f3f3f
.L_08161378:
	.4byte Data_02012d80
.L_0816137c:
	.4byte Data_02014b00
.L_08161380:
	.4byte Data_020158d2
.L_08161384:
	ldr r5, [sp, #124]
	cmp r5, #85
	beq .L_0816138c
	b .L_0816157e
.L_0816138c:
	ldr r3, .L_081613cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r0, [sp, #120]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r3, #0
	str r3, [r2]
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	movs r1, #35
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	b .L_081613d0
	.2byte 0x0000
.L_081613cc:
	.4byte 0x00000000
.L_081613d0:
	ldr r5, [sp, #112]
	str r3, [sp, #172]
	cmp r5, #0
	bne .L_081613fc
	ldr r1, [sp, #120]
	movs r2, #224
	movs r0, #0
	lsls r2, r2, #3
	mov r11, r0
	movs r6, #31
	adds r5, r1, r2
.L_081613e6:
	bl Random16
	ands r0, r6
	strb r0, [r5]
	movs r3, #1
	movs r0, #138
	add r11, r3
	lsls r0, r0, #3
	adds r5, #1
	cmp r11, r0
	bne .L_081613e6
.L_081613fc:
	ldr r1, [sp, #112]
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r3, r3, #2
	adds r7, r3, #0
	subs r7, #96
	cmp r7, #16
	ble .L_0816140e
	movs r7, #16
.L_0816140e:
	ldr r5, [sp, #112]
	movs r3, #96
	mov r8, r3
	cmp r5, #7
	ble .L_08161474
	movs r0, #0
	movs r3, #112
	lsls r2, r5, #1
	mov r11, r0
	mov lr, r0
	subs r0, r3, r2
	lsls r3, r0, #4
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r4, r3, r0
.L_0816142c:
	cmp r0, #95
	bhi .L_08161464
	movs r2, #15
	ands r2, r0
	lsls r3, r2, #4
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r2, .L_08161714
	movs r5, #0
	adds r1, r4, r2
	ldr r2, [sp, #120]
	mov r12, lr
	adds r3, r3, r2
	movs r2, #224
	mov r10, r3
	lsls r2, r2, #3
	movs r6, #0
	add r2, r10
.L_08161452:
	ldrb r3, [r2]
	adds r2, #1
	cmp r3, r12
	bge .L_0816145c
	strb r6, [r1]
.L_0816145c:
	adds r5, #1
	adds r1, #1
	cmp r5, #69
	bne .L_08161452
.L_08161464:
	movs r3, #1
	add r11, r3
	mov r5, r11
	add lr, r3
	adds r4, #69
	adds r0, #1
	cmp r5, #34
	bne .L_0816142c
.L_08161474:
	movs r3, #69
	mov r0, r8
	str r3, [sp, #0]
	str r0, [sp, #4]
	ldr r1, .L_08161714
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #29
	adds r3, r7, #0
	mov lr, r4
	.2byte 0xf800
	ldr r1, [sp, #112]
	cmp r1, #5
	ble .L_0816156a
	adds r3, r1, #0
	subs r3, #6
	cmp r3, #0
	bge .L_0816149a
	adds r3, #3
.L_0816149a:
	asrs r3, r3, #2
	cmp r3, #6
	bhi .L_0816156a
	ldr r2, .L_08161718
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_081614a8:
	.4byte .L_081614c4
	.4byte .L_081614dc
	.4byte .L_081614f4
	.4byte .L_0816150c
	.4byte .L_08161524
	.4byte .L_0816153c
	.4byte .L_08161554
.L_081614c4:
	movs r3, #59
	str r3, [sp, #0]
	movs r3, #24
	str r3, [sp, #4]
	ldr r1, .L_0816171c
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #32
	movs r3, #92
	mov lr, r4
	.2byte 0xf800
	b .L_0816156a
.L_081614dc:
	movs r3, #70
	str r3, [sp, #0]
	movs r3, #27
	str r3, [sp, #4]
	ldr r1, .L_08161720
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #30
	movs r3, #87
	mov lr, r4
	.2byte 0xf800
	b .L_0816156a
.L_081614f4:
	movs r3, #84
	str r3, [sp, #0]
	movs r3, #55
	str r3, [sp, #4]
	ldr r1, .L_08161724
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #23
	movs r3, #57
	mov lr, r4
	.2byte 0xf800
	b .L_0816156a
.L_0816150c:
	movs r3, #75
	str r3, [sp, #0]
	movs r3, #57
	str r3, [sp, #4]
	ldr r1, .L_08161728
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #27
	movs r3, #55
	mov lr, r4
	.2byte 0xf800
	b .L_0816156a
.L_08161524:
	movs r3, #67
	str r3, [sp, #0]
	movs r3, #86
	str r3, [sp, #4]
	ldr r1, .L_0816172c
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #28
	movs r3, #19
	mov lr, r4
	.2byte 0xf800
	b .L_0816156a
.L_0816153c:
	movs r3, #50
	str r3, [sp, #0]
	movs r3, #65
	str r3, [sp, #4]
	ldr r1, .L_08161730
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #33
	movs r3, #22
	mov lr, r4
	.2byte 0xf800
	b .L_0816156a
.L_08161554:
	movs r3, #48
	str r3, [sp, #0]
	movs r3, #29
	str r3, [sp, #4]
	ldr r1, .L_08161734
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #41
	movs r3, #21
	mov lr, r4
	.2byte 0xf800
.L_0816156a:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #128]
	ldr r1, [sp, #84]
	ldr r0, [r2, #4]
	bl Func_08144aac
	bl .L_0816342a
.L_0816157e:
	ldr r3, [sp, #124]
	subs r3, #96
	cmp r3, #2
	bls .L_08161588
	b .L_081616d6
.L_08161588:
	ldr r3, [sp, #112]
	cmp r3, #19
	bgt .L_0816162e
	cmp r3, #7
	bgt .L_081615a0
	ldr r5, [sp, #112]
	ldr r2, .L_08161738
	lsrs r3, r3, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	ldrb r6, [r2, r3]
	b .L_081615a2
.L_081615a0:
	movs r6, #3
.L_081615a2:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [sp, #12]
	lsls r5, r6, #3
	ldr r2, [r3]
	subs r5, r5, r6
	movs r0, #192
	lsls r0, r0, #18
	lsls r5, r5, #2
	mov r10, r0
	subs r5, r5, r6
	movs r0, #18
	ldr r1, .L_08161714
	movs r6, #48
	str r0, [sp, #0]
	str r6, [sp, #4]
	lsrs r3, r2, #31
	adds r2, r2, r3
	lsls r5, r5, #5
	adds r5, r5, r1
	asrs r2, r2, #1
	mov r1, r10
	ldr r4, [r1, #104]
	subs r2, #18
	movs r3, #56
	adds r1, r5, #0
	mov r8, r0
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #7
	movs r0, #104
	bl Func_081963ec
	mov r2, sp
	adds r2, #240
	str r2, [sp, #12]
	mov r0, r10
	ldr r2, [r2]
	str r6, [sp, #4]
	lsrs r3, r2, #31
	adds r2, r2, r3
	mov r3, r8
	str r3, [sp, #0]
	adds r1, r5, #0
	ldr r4, [r0, #104]
	asrs r2, r2, #1
	ldr r0, [sp, #116]
	movs r3, #56
	mov lr, r4
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #128]
	ldr r0, [r1, #4]
	ldr r1, [sp, #84]
	bl Func_08144aac
.L_0816162e:
	ldr r3, [sp, #124]
	subs r3, #97
	cmp r3, #1
	bls .L_0816163a
	bl .L_0816342a
.L_0816163a:
	ldr r2, [sp, #112]
	cmp r2, #23
	ble .L_08161644
	bl .L_0816342a
.L_08161644:
	ldr r5, [sp, #124]
	movs r3, #16
	mov r8, r3
	cmp r5, #97
	beq .L_08161652
	movs r0, #24
	mov r8, r0
.L_08161652:
	movs r1, #0
	mov r2, r8
	mov r11, r1
	cmp r2, #0
	bne .L_08161660
	bl .L_0816342a
.L_08161660:
	movs r3, #192
	ldr r7, .L_0816173c
	add r3, sp
	mov r10, r3
.L_08161668:
	mov r5, r11
	lsrs r3, r5, #31
	add r3, r11
	ldr r0, [sp, #112]
	asrs r3, r3, #1
	adds r3, #4
	cmp r0, r3
	blt .L_081616c8
	ldr r3, [r7, #24]
	cmp r3, #9
	bgt .L_081616c8
	mov r6, r10
	lsrs r5, r3, #31
	adds r1, r6, #0
	adds r0, r7, #0
	adds r5, r3, r5
	bl Func_0815e1ec
	ldr r2, [r6]
	ldr r1, .L_08161740
	lsrs r3, r2, #31
	asrs r5, r5, #1
	adds r2, r2, r3
	lsls r5, r5, #11
	ldr r3, [r6, #4]
	asrs r2, r2, #1
	adds r5, r5, r1
	movs r1, #32
	str r2, [r6]
	str r1, [sp, #0]
	movs r1, #64
	subs r3, #32
	str r1, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r7, #0
	movs r1, #64
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_081616c8:
	movs r2, #1
	add r11, r2
	adds r7, #28
	cmp r11, r8
	bne .L_08161668
	bl .L_0816342a
.L_081616d6:
	ldr r3, [sp, #124]
	cmp r3, #86
	beq .L_081616de
	b .L_0816187c
.L_081616de:
	ldr r5, [sp, #120]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r5, r0
	adds r1, #132
	movs r3, #1
	str r3, [r2]
	adds r2, r5, r1
	movs r3, #0
	str r3, [r2]
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #128]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08161744
	movs r0, #104
	movs r1, #35
	bl Func_081963ec
	b .L_0816174c
.L_08161714:
	.4byte gMapCellBuffer
.L_08161718:
	.4byte .L_081614a8
.L_0816171c:
	.4byte Data_020119e0
.L_08161720:
	.4byte Data_02011f68
.L_08161724:
	.4byte Data_020126ca
.L_08161728:
	.4byte Data_020138d6
.L_0816172c:
	.4byte Data_02014989
.L_08161730:
	.4byte Data_0201600b
.L_08161734:
	.4byte Data_02016cbd
.L_08161738:
	.4byte Data_08198886
.L_0816173c:
	.4byte Data_02016000
.L_08161740:
	.4byte Data_02010d80
.L_08161744:
	movs r0, #104
	movs r1, #39
	bl Func_081963ec
.L_0816174c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r5, [sp, #96]
	str r3, [sp, #172]
	ldr r3, [sp, #100]
	ldr r0, [sp, #112]
	adds r3, r3, r5
	str r3, [sp, #100]
	cmp r0, #6
	ble .L_08161772
	lsls r3, r5, #1
	adds r3, r3, r5
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_0816176e
	adds r3, #63
.L_0816176e:
	asrs r3, r3, #6
	str r3, [sp, #96]
.L_08161772:
	ldr r1, [sp, #100]
	movs r2, #128
	lsls r2, r2, #19
	asrs r3, r1, #16
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	ldr r0, [sp, #112]
	subs r0, #28
	cmp r0, #15
	bhi .L_081617a0
	lsrs r2, r0, #31
	adds r2, r0, r2
	ldr r1, .L_081617b8
	asrs r2, r2, #1
	movs r3, #128
	adds r2, #8
	lsls r3, r3, #19
	lsls r2, r2, #8
	subs r1, r1, r0
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_081617a0:
	ldr r3, [sp, #112]
	subs r3, #4
	cmp r3, #3
	bhi .L_081617c4
	movs r1, #128
	ldr r3, .L_081617bc
	ldr r0, [sp, #116]
	lsls r1, r1, #7
	ldr r2, .L_081617c0
	mov lr, r3
	.2byte 0xf800
	b .L_081617c4
.L_081617b8:
	.4byte 0x00000010
.L_081617bc:
	.4byte IwramFillWords
.L_081617c0:
	.4byte 0x01010101
.L_081617c4:
	ldr r2, [sp, #112]
	cmp r2, #3
	ble .L_08161800
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_081617ea
	movs r3, #96
	str r3, [sp, #0]
	movs r3, #88
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_081618ec
	movs r2, #32
	movs r3, #21
	mov lr, r4
	.2byte 0xf800
	b .L_08161800
.L_081617ea:
	movs r3, #96
	str r3, [sp, #0]
	movs r3, #88
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_081618ec
	movs r2, #0
	movs r3, #21
	mov lr, r4
	.2byte 0xf800
.L_08161800:
	ldr r0, [sp, #112]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r4, r3, #1
	cmp r4, #9
	bgt .L_08161868
	ldr r1, [sp, #128]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816183c
	ldr r3, .L_081618f0
	lsls r4, r4, #1
	ldrh r1, [r3, r4]
	ldr r2, .L_081618ec
	ldr r3, .L_081618f4
	ldr r0, .L_081618f8
	adds r1, r1, r2
	ldrh r0, [r0, r4]
	ldrh r2, [r3, r4]
	ldr r3, .L_081618fc
	ldrh r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_08161900
	ldrh r0, [r0, r4]
	ldr r4, [sp, #172]
	str r0, [sp, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	b .L_08161868
.L_0816183c:
	ldr r3, .L_081618f0
	lsls r4, r4, #1
	ldrh r1, [r3, r4]
	ldr r3, .L_081618ec
	adds r1, r1, r3
	ldr r3, .L_081618f4
	ldrh r2, [r3, r4]
	ldr r3, .L_081618f8
	negs r2, r2
	ldrh r0, [r3, r4]
	ldr r3, .L_081618fc
	subs r2, r2, r0
	ldrh r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_08161900
	adds r2, #128
	ldrh r0, [r0, r4]
	ldr r4, [sp, #172]
	str r0, [sp, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_08161868:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_0816186e:
	ldr r5, [sp, #128]
	ldr r1, [sp, #84]
	ldr r0, [r5, #4]
	bl Func_08144aac
	bl .L_0816342a
.L_0816187c:
	ldr r0, [sp, #124]
	cmp r0, #88
	beq .L_08161884
	b .L_08161af4
.L_08161884:
	ldr r1, [sp, #120]
	movs r3, #239
	movs r5, #238
	lsls r3, r3, #7
	lsls r5, r5, #7
	adds r2, r1, r3
	adds r5, #132
	movs r3, #1
	str r3, [r2]
	adds r2, r1, r5
	movs r3, #0
	str r3, [r2]
	ldr r0, [sp, #112]
	cmp r0, #7
	bne .L_081618b4
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r0, #134
	bl Audio_PlayCue
.L_081618b4:
	ldr r5, [sp, #112]
	cmp r5, #11
	bne .L_081618c0
	movs r0, #134
	bl Audio_PlayCue
.L_081618c0:
	ldr r0, [sp, #112]
	cmp r0, #15
	bne .L_081618cc
	movs r0, #134
	bl Audio_PlayCue
.L_081618cc:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #128]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08161904
	movs r0, #104
	movs r1, #35
	bl Func_081963ec
	b .L_0816190c
	.2byte 0x0000
.L_081618ec:
	.4byte gMapCellBuffer
.L_081618f0:
	.4byte Data_0819888a
.L_081618f4:
	.4byte Data_081988c6
.L_081618f8:
	.4byte Data_0819889e
.L_081618fc:
	.4byte Data_081988da
.L_08161900:
	.4byte Data_081988b2
.L_08161904:
	movs r0, #104
	movs r1, #39
	bl Func_081963ec
.L_0816190c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r2, [sp, #100]
	str r3, [sp, #172]
	ldr r3, [sp, #96]
	ldr r5, [sp, #112]
	adds r2, r2, r3
	str r2, [sp, #100]
	cmp r5, #5
	bne .L_0816192a
	movs r0, #128
	lsls r0, r0, #14
	adds r2, r2, r0
	str r2, [sp, #100]
.L_0816192a:
	ldr r1, [sp, #112]
	cmp r1, #6
	ble .L_08161942
	ldr r2, [sp, #96]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_0816193e
	adds r3, #63
.L_0816193e:
	asrs r3, r3, #6
	str r3, [sp, #96]
.L_08161942:
	ldr r5, [sp, #100]
	movs r2, #128
	lsls r2, r2, #19
	asrs r3, r5, #16
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	ldr r0, [sp, #112]
	subs r0, #29
	cmp r0, #15
	bhi .L_08161970
	lsrs r2, r0, #31
	adds r2, r0, r2
	ldr r1, .L_08161990
	asrs r2, r2, #1
	movs r3, #128
	adds r2, #8
	lsls r3, r3, #19
	lsls r2, r2, #8
	subs r1, r1, r0
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08161970:
	ldr r0, [sp, #112]
	cmp r0, #4
	ble .L_08161998
	movs r3, #60
	str r3, [sp, #0]
	movs r3, #95
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_08161994
	movs r2, #35
	movs r3, #17
	mov lr, r4
	.2byte 0xf800
	b .L_08161998
	.2byte 0x0000
.L_08161990:
	.4byte 0x00000010
.L_08161994:
	.4byte gMapCellBuffer
.L_08161998:
	ldr r1, [sp, #112]
	cmp r1, #5
	bne .L_081619ac
	movs r1, #128
	ldr r3, .L_08161ac4
	ldr r0, [sp, #116]
	lsls r1, r1, #7
	ldr r2, .L_08161ac8
	mov lr, r3
	.2byte 0xf800
.L_081619ac:
	ldr r2, [sp, #112]
	cmp r2, #0
	blt .L_08161a1e
	adds r0, r2, #0
	movs r1, #3
	bl __divsi3
	cmp r0, #6
	bgt .L_08161a1e
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_081619f0
	ldr r3, .L_08161acc
	lsls r4, r0, #1
	ldrh r1, [r3, r4]
	ldr r0, .L_08161ad0
	ldr r3, .L_08161ad4
	adds r1, r1, r0
	ldr r0, .L_08161ad8
	ldrh r2, [r3, r4]
	ldrh r0, [r0, r4]
	ldr r3, .L_08161adc
	ldrh r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_08161ae0
	subs r3, #16
	ldrh r0, [r0, r4]
	ldr r4, [sp, #172]
	str r0, [sp, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	b .L_08161a1e
.L_081619f0:
	ldr r3, .L_08161acc
	lsls r4, r0, #1
	ldrh r1, [r3, r4]
	ldr r2, .L_08161ad0
	ldr r3, .L_08161ad4
	adds r1, r1, r2
	ldrh r2, [r3, r4]
	ldr r3, .L_08161ad8
	negs r2, r2
	ldrh r0, [r3, r4]
	ldr r3, .L_08161adc
	subs r2, r2, r0
	ldrh r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_08161ae0
	adds r2, #128
	ldrh r0, [r0, r4]
	subs r3, #16
	str r0, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_08161a1e:
	ldr r5, .L_08161ae4
	ldr r6, [sp, #112]
	ldr r7, .L_08161ae8
	movs r3, #0
	mov r11, r3
	mov r8, r5
	subs r6, #8
.L_08161a2c:
	cmp r6, #8
	bhi .L_08161aa0
	adds r0, r6, #0
	movs r1, #3
	bl __divsi3
	adds r4, r0, #0
	ldr r0, [sp, #128]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_08161a70
	lsls r3, r4, #1
	mov r2, r8
	ldrh r1, [r2, r3]
	ldr r3, .L_08161ad0
	ldr r0, .L_08161aec
	adds r1, r1, r3
	ldr r3, .L_08161af0
	ldrb r2, [r7]
	ldrb r5, [r3, r4]
	ldrb r4, [r0, r4]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldrb r3, [r7, #1]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	subs r3, #16
	str r5, [sp, #0]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	b .L_08161aa0
.L_08161a70:
	lsls r3, r4, #1
	mov r5, r8
	ldrh r1, [r5, r3]
	ldr r0, .L_08161ad0
	ldr r3, .L_08161af0
	ldrb r2, [r7]
	ldrb r5, [r3, r4]
	adds r1, r1, r0
	ldr r0, .L_08161aec
	lsrs r3, r5, #1
	ldrb r4, [r0, r4]
	negs r2, r2
	subs r2, r2, r3
	ldrb r3, [r7, #1]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	adds r2, #128
	subs r3, #16
	str r5, [sp, #0]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_08161aa0:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r7, #2
	subs r6, #2
	cmp r2, #3
	bne .L_08161a2c
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_08161ab4:
	ldr r3, [sp, #128]
	ldr r1, [sp, #84]
	ldr r0, [r3, #4]
	bl Func_08144aac
	bl .L_0816342a
	.2byte 0x0000
.L_08161ac4:
	.4byte IwramFillWords
.L_08161ac8:
	.4byte 0x3f3f3f3f
.L_08161acc:
	.4byte Data_081988ee
.L_08161ad0:
	.4byte gMapCellBuffer
.L_08161ad4:
	.4byte Data_08198918
.L_08161ad8:
	.4byte Data_081988fc
.L_08161adc:
	.4byte Data_08198926
.L_08161ae0:
	.4byte Data_0819890a
.L_08161ae4:
	.4byte Data_08198934
.L_08161ae8:
	.4byte Data_08198940
.L_08161aec:
	.4byte Data_0819893d
.L_08161af0:
	.4byte Data_0819893a
.L_08161af4:
	ldr r5, [sp, #124]
	cmp r5, #62
	beq .L_08161afc
	b .L_08161ce8
.L_08161afc:
	ldr r0, [sp, #100]
	ldr r1, [sp, #96]
	ldr r2, [sp, #112]
	adds r0, r0, r1
	str r0, [sp, #100]
	cmp r2, #6
	ble .L_08161b1a
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_08161b16
	adds r3, #63
.L_08161b16:
	asrs r3, r3, #6
	str r3, [sp, #96]
.L_08161b1a:
	ldr r5, [sp, #100]
	movs r2, #128
	lsls r2, r2, #19
	asrs r3, r5, #16
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	ldr r0, [sp, #112]
	cmp r0, #0
	bne .L_08161b34
	ldr r3, .L_08161b5c
	adds r2, #42
	strh r3, [r2]
.L_08161b34:
	ldr r1, [sp, #112]
	subs r1, #24
	cmp r1, #15
	bhi .L_08161b4c
	ldr r2, .L_08161b60
	movs r3, #128
	subs r2, r2, r1
	ldr r1, .L_08161b64
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08161b4c:
	ldr r3, [sp, #112]
	subs r3, #4
	cmp r3, #1
	bhi .L_08161b74
	movs r1, #128
	ldr r3, .L_08161b68
	ldr r0, [sp, #116]
	b .L_08161b6c
.L_08161b5c:
	.4byte 0x00000810
.L_08161b60:
	.4byte 0x00000010
.L_08161b64:
	.4byte 0x00001000
.L_08161b68:
	.4byte IwramFillWords
.L_08161b6c:
	lsls r1, r1, #7
	ldr r2, .L_08161ed8
	mov lr, r3
	.2byte 0xf800
.L_08161b74:
	ldr r1, [sp, #112]
	cmp r1, #3
	bgt .L_08161bba
	ldr r2, [sp, #128]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08161b9a
	ldr r3, [sp, #120]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r3, r5
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #0
	b .L_08161bb0
.L_08161b9a:
	ldr r0, [sp, #120]
	movs r3, #80
	movs r2, #224
	lsls r2, r2, #3
	str r3, [sp, #0]
	movs r3, #104
	adds r1, r0, r2
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #48
.L_08161bb0:
	movs r3, #24
	mov lr, r4
	.2byte 0xf800
	bl .L_0816342a
.L_08161bba:
	ldr r3, [sp, #112]
	cmp r3, #7
	bgt .L_08161c02
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #1
	bne .L_08161be6
	ldr r0, [sp, #120]
	movs r3, #80
	movs r2, #224
	lsls r2, r2, #3
	str r3, [sp, #0]
	movs r3, #104
	adds r1, r0, r2
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #0
	movs r3, #24
	mov lr, r4
	.2byte 0xf800
	b .L_08161c02
.L_08161be6:
	ldr r3, [sp, #120]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r3, r5
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #48
	movs r3, #24
	mov lr, r4
	.2byte 0xf800
.L_08161c02:
	ldr r0, [sp, #128]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_08161c28
	ldr r2, [sp, #120]
	movs r3, #148
	lsls r3, r3, #6
	adds r1, r2, r3
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #16
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
	b .L_08161c44
.L_08161c28:
	ldr r5, [sp, #120]
	movs r3, #80
	movs r0, #148
	lsls r0, r0, #6
	str r3, [sp, #0]
	movs r3, #104
	adds r1, r5, r0
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #32
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
.L_08161c44:
	ldr r3, [sp, #112]
	subs r3, #6
	cmp r3, #1
	bhi .L_08161c68
	ldr r2, [sp, #120]
	movs r3, #139
	lsls r3, r3, #7
	adds r1, r2, r3
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #91
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #0
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
.L_08161c68:
	ldr r3, [sp, #112]
	subs r3, #8
	cmp r3, #1
	bhi .L_08161c86
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #91
	str r3, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_08161edc
	movs r2, #0
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
.L_08161c86:
	ldr r3, [sp, #112]
	subs r3, #10
	cmp r3, #1
	bhi .L_08161ca4
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #59
	str r3, [sp, #4]
	ldr r1, .L_08161ee0
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #0
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
.L_08161ca4:
	ldr r3, [sp, #112]
	subs r3, #12
	cmp r3, #1
	bhi .L_08161cc2
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #29
	str r3, [sp, #4]
	ldr r1, .L_08161ee4
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #0
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
.L_08161cc2:
	ldr r3, [sp, #112]
	subs r3, #14
	cmp r3, #1
	bls .L_08161cce
	bl .L_0816342a
.L_08161cce:
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #26
	str r3, [sp, #4]
	ldr r1, .L_08161ee8
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #0
	movs r3, #16
	mov lr, r4
	.2byte 0xf800
	bl .L_0816342a
.L_08161ce8:
	ldr r5, [sp, #124]
	cmp r5, #50
	bne .L_08161d9e
	movs r0, #0
	mov r11, r0
.L_08161cf2:
	ldr r1, [sp, #112]
	mov r3, r11
	mov r5, r11
	adds r3, #6
	adds r5, #1
	cmp r1, r3
	blt .L_08161d94
	adds r3, #12
	cmp r1, r3
	bge .L_08161d90
	mov r2, r11
	subs r3, r1, r2
	subs r3, #6
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r4, r3, #1
	ldr r3, [sp, #12]
	ldr r7, .L_08161eec
	ldr r2, [r3]
	mov r5, r11
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldrb r3, [r7, r4]
	asrs r2, r2, #1
	lsrs r3, r3, #1
	subs r6, r2, r3
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_08161d3e
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r6, r6, r2
	b .L_08161d4e
.L_08161d3e:
	mov r5, r11
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	subs r6, r6, r2
.L_08161d4e:
	mov r1, r11
	movs r0, #1
	cmp r1, #0
	beq .L_08161d66
	mov r3, r11
	subs r3, #1
	movs r2, #3
	ands r3, r2
	movs r0, #0
	cmp r3, #1
	ble .L_08161d66
	movs r0, #1
.L_08161d66:
	ldr r2, .L_08161ef0
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_08161edc
	ldr r3, .L_08161ef4
	adds r1, r1, r2
	ldrb r2, [r7, r4]
	ldrb r3, [r3, r4]
	str r2, [sp, #0]
	ldr r2, .L_08161ef8
	lsls r0, r0, #2
	ldrb r2, [r2, r4]
	adds r3, #48
	str r2, [sp, #4]
	ldr r2, [sp, #84]
	ldr r4, [r0, r2]
	ldr r0, [sp, #116]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	b .L_08161d94
.L_08161d90:
	mov r5, r11
	adds r5, #1
.L_08161d94:
	mov r11, r5
	cmp r5, #12
	bne .L_08161cf2
	bl .L_0816342a
.L_08161d9e:
	ldr r3, [sp, #124]
	cmp r3, #46
	beq .L_08161da6
	b .L_08161eb8
.L_08161da6:
	ldr r5, [sp, #112]
	cmp r5, #0
	bne .L_08161df4
	movs r6, #255
	ldr r5, .L_08161efc
	movs r0, #0
	lsls r6, r6, #8
	mov r11, r0
	movs r7, #0
	adds r6, #255
.L_08161dba:
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #32
	str r3, [r5]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	add r11, r1
	ands r0, r6
	mov r2, r11
	str r0, [r5, #20]
	adds r5, #28
	cmp r2, #64
	bne .L_08161dba
	ldr r2, .L_08161f00
	movs r3, #159
	str r3, [r2, #4]
.L_08161df4:
	ldr r0, [sp, #12]
	movs r5, #192
	ldr r7, .L_08161efc
	movs r3, #0
	add r5, sp
	mov r11, r3
	mov r10, r5
	mov r8, r0
.L_08161e04:
	ldr r3, [r7]
	cmp r3, #0
	blt .L_08161ea8
	mov r1, r11
	lsrs r3, r1, #31
	ldr r2, [sp, #112]
	add r3, r11
	asrs r3, r3, #1
	cmp r2, r3
	blt .L_08161ea8
	movs r5, #3
	ands r5, r1
	bl Func_08014de4
	ldr r0, [r7, #12]
	bl SceneTransform_ApplyPitch
	mov r6, r10
	ldr r0, [r7, #16]
	bl Func_08015068
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	mov r0, r8
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0]
	asrs r2, r2, #1
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	adds r2, r2, r3
	str r2, [r6]
	ldr r3, [r6, #4]
	ldr r1, [r0, #4]
	lsls r5, r5, #1
	adds r3, r3, r1
	adds r1, r3, #0
	adds r1, #32
	str r1, [r6, #4]
	ldr r1, .L_08161f04
	movs r0, #8
	ldrh r1, [r1, r5]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r5, .L_08161edc
	ldr r0, [sp, #84]
	adds r3, #28
	ldr r4, [r0, #4]
	adds r1, r1, r5
	subs r2, #4
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7]
	subs r3, #6
	str r3, [r7]
	cmp r3, #0
	bge .L_08161ea8
	movs r3, #7
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_08161e8e
	cmp r1, #63
	bne .L_08161ea8
.L_08161e8e:
	movs r0, #133
	bl Audio_PlayCue
	ldr r3, [sp, #128]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #4
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_08161ea8:
	movs r5, #1
	add r11, r5
	mov r0, r11
	adds r7, #28
	cmp r0, #64
	bne .L_08161e04
	bl .L_0816342a
.L_08161eb8:
	ldr r1, [sp, #124]
	cmp r1, #38
	bne .L_08161f30
	ldr r3, [sp, #112]
	subs r3, #5
	cmp r3, #44
	bls .L_08161eca
	bl .L_0816342a
.L_08161eca:
	ldr r2, [sp, #112]
	cmp r2, #25
	ble .L_08161f08
	lsls r2, r2, #2
	movs r3, #196
	subs r1, r3, r2
	b .L_08161f10
.L_08161ed8:
	.4byte 0x3f3f3f3f
.L_08161edc:
	.4byte gMapCellBuffer
.L_08161ee0:
	.4byte Data_02012d80
.L_08161ee4:
	.4byte Data_02014b00
.L_08161ee8:
	.4byte Data_02015980
.L_08161eec:
	.4byte Data_08197467
.L_08161ef0:
	.4byte Data_0819747a
.L_08161ef4:
	.4byte Data_08197473
.L_08161ef8:
	.4byte Data_0819746d
.L_08161efc:
	.4byte Data_02016000
.L_08161f00:
	.4byte Data_020166e4
.L_08161f04:
	.4byte Data_08198946
.L_08161f08:
	ldr r5, [sp, #112]
	lsls r3, r5, #4
	adds r1, r3, #0
	subs r1, #64
.L_08161f10:
	cmp r1, #96
	ble .L_08161f16
	movs r1, #96
.L_08161f16:
	movs r2, #32
	movs r3, #104
	subs r3, r3, r1
	str r2, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_08161fc0
	movs r2, #48
	mov lr, r4
	.2byte 0xf800
	bl .L_0816342a
.L_08161f30:
	ldr r0, [sp, #124]
	cmp r0, #79
	beq .L_08161f38
	b .L_0816207a
.L_08161f38:
	ldr r1, [sp, #112]
	ldr r5, .L_08161fc4
	cmp r1, #0
	bne .L_08161f92
	ldr r2, [sp, #12]
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r5]
	ldr r0, [sp, #12]
	ldr r3, [r0, #4]
	adds r3, #48
	lsls r3, r3, #16
	str r3, [r5, #4]
	ldr r1, [sp, #128]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08161f66
	ldr r3, .L_08161fc8
	str r3, [r5, #12]
	b .L_08161f6e
.L_08161f66:
	ldr r2, .L_08161fc4
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2, #12]
.L_08161f6e:
	ldr r3, [r5, #12]
	movs r1, #128
	negs r3, r3
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r5]
	lsls r2, r2, #2
	lsls r1, r1, #12
	adds r3, r3, r2
	str r3, [r5]
	negs r3, r1
	ldr r2, [r5, #4]
	lsls r3, r3, #1
	subs r3, r3, r1
	lsls r3, r3, #2
	adds r2, r2, r3
	str r1, [r5, #16]
	str r2, [r5, #4]
.L_08161f92:
	ldr r3, [sp, #112]
	cmp r3, #150
	bgt .L_08161fcc
	ldr r3, .L_08161fbc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r0, [sp, #120]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r3, #0
	str r3, [r2]
	b .L_08161fee
.L_08161fbc:
	.4byte 0x00000000
.L_08161fc0:
	.4byte gMapCellBuffer
.L_08161fc4:
	.4byte Data_02016000
.L_08161fc8:
	.4byte 0xfffd0000
.L_08161fcc:
	ldr r0, [sp, #120]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r3, #50
	str r3, [r2]
	movs r2, #128
	ldr r3, .L_08162018
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
.L_08161fee:
	ldr r0, [sp, #112]
	cmp r0, #31
	bgt .L_08162058
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #48
	str r0, [sp, #0]
	movs r0, #96
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	ldr r1, .L_0816201c
	ldr r4, [r0, #4]
	subs r2, #24
	subs r3, #96
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r1, [sp, #112]
	b .L_08162020
.L_08162018:
	.4byte 0x00003f44
.L_0816201c:
	.4byte Data_02010800
.L_08162020:
	cmp r1, #11
	bgt .L_08162030
	ldr r0, .L_081620f0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	b .L_08162058
.L_08162030:
	ldr r2, [sp, #112]
	cmp r2, #12
	bne .L_08162044
	ldr r0, [r5, #16]
	movs r1, #3
	negs r0, r0
	bl __divsi3
	str r0, [r5, #16]
	b .L_08162058
.L_08162044:
	ldr r3, [sp, #112]
	subs r3, #13
	cmp r3, #5
	bls .L_08162058
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, .L_081620f0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector2
.L_08162058:
	ldr r3, [sp, #112]
	cmp r3, #11
	beq .L_08162062
	bl .L_0816342a
.L_08162062:
	movs r0, #144
	bl Audio_PlayCue
	movs r0, #238
	ldr r5, [sp, #120]
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r5, r0
	movs r3, #16
	str r3, [r2]
	bl .L_0816342a
.L_0816207a:
	ldr r3, [sp, #124]
	subs r3, #63
	cmp r3, #1
	bhi .L_081620f8
	ldr r1, [sp, #112]
	cmp r1, #5
	ble .L_0816208c
	bl .L_0816342a
.L_0816208c:
	ldr r2, [sp, #128]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_081620b0
	ldr r3, [sp, #12]
	ldr r5, [sp, #112]
	ldr r1, [r3]
	ldr r0, [sp, #12]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #6
	subs r3, r3, r5
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r3, r2, #1
	asrs r1, r1, #1
	adds r1, r1, r3
	b .L_081620ca
.L_081620b0:
	ldr r2, [sp, #12]
	ldr r5, [sp, #112]
	ldr r1, [r2]
	ldr r0, [sp, #12]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #6
	subs r3, r3, r5
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r3, r2, #1
	asrs r1, r1, #1
	subs r1, r1, r3
.L_081620ca:
	ldr r3, [r0, #4]
	lsls r2, r2, #2
	subs r3, r3, r2
	adds r3, #24
	adds r2, r1, #0
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	ldr r1, [sp, #84]
	subs r2, #16
	ldr r4, [r1, #4]
	subs r3, #32
	ldr r0, [sp, #116]
	ldr r1, .L_081620f4
	mov lr, r4
	.2byte 0xf800
	bl .L_0816342a
.L_081620f0:
	.4byte Data_02016000
.L_081620f4:
	.4byte gMapCellBuffer
.L_081620f8:
	ldr r2, [sp, #124]
	cmp r2, #70
	beq .L_08162100
	b .L_081622c8
.L_08162100:
	ldr r3, [sp, #112]
	cmp r3, #31
	ble .L_08162118
	ldr r5, [sp, #112]
	ldr r2, .L_08162130
	ldr r1, .L_08162134
	movs r3, #128
	lsls r3, r3, #19
	subs r2, r2, r5
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08162118:
	ldr r1, .L_08162138
	ldr r2, [sp, #112]
	movs r0, #0
	mov r11, r0
	mov r8, r1
	cmp r2, #13
	bgt .L_0816221a
	ldr r5, [sp, #128]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_08162154
	b .L_0816213c
.L_08162130:
	.4byte 0x00000030
.L_08162134:
	.4byte 0x00001000
.L_08162138:
	.4byte Data_02016000
.L_0816213c:
	ldr r0, [sp, #12]
	ldr r1, [r0]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #14
	subs r3, r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r3, r2, #1
	asrs r1, r1, #1
	adds r1, r1, r3
	b .L_0816216e
.L_08162154:
	ldr r2, [sp, #12]
	ldr r5, [sp, #112]
	ldr r1, [r2]
	ldr r0, [sp, #12]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #14
	subs r3, r3, r5
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r3, r2, #1
	asrs r1, r1, #1
	subs r1, r1, r3
.L_0816216e:
	ldr r3, [r0, #4]
	lsls r2, r2, #2
	subs r3, r3, r2
	mov r8, r1
	adds r3, #24
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	ldr r1, [sp, #84]
	mov r2, r8
	subs r2, #16
	ldr r4, [r1, #4]
	subs r3, #32
	ldr r0, [sp, #116]
	ldr r1, .L_081622ac
	mov lr, r4
	.2byte 0xf800
	ldr r2, [sp, #112]
	cmp r2, #13
	beq .L_0816219c
	bl .L_0816342a
.L_0816219c:
	movs r0, #134
	bl Audio_PlayCue
	movs r5, #238
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	str r3, [r2]
	ldr r2, [sp, #128]
	movs r3, #128
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #192
	lsls r1, r1, #10
	str r1, [sp, #0]
	movs r2, #128
	movs r1, #150
	lsls r3, r3, #12
	str r1, [sp, #4]
	lsls r2, r2, #10
	movs r1, #1
	bl Func_0815f000
	ldr r5, .L_081622b0
	ldr r6, .L_081622b4
	movs r3, #0
	mov r11, r3
	movs r7, #127
.L_081621d8:
	ldrb r3, [r6]
	add r3, r8
	subs r3, #40
	lsls r3, r3, #16
	str r3, [r5]
	ldrb r3, [r6, #1]
	adds r6, #2
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r7
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	negs r0, r0
	lsls r0, r0, #11
	str r0, [r5, #16]
	movs r0, #1
	movs r3, #32
	add r11, r0
	str r3, [r5, #8]
	mov r1, r11
	movs r3, #0
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #16
	bne .L_081621d8
	bl .L_0816342a
.L_0816221a:
	movs r1, #5
	mov r0, r11
	bl __modsi3
	mov r6, r8
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r6, #24]
	bl __divsi3
	movs r1, #3
	bl __modsi3
	mov r2, r11
	adds r4, r5, r0
	movs r7, #4
	cmp r2, #2
	ble .L_08162242
	movs r7, #0
.L_08162242:
	ldr r2, .L_081622b8
	lsls r3, r4, #2
	ldr r1, [r2, r3]
	ldr r3, .L_081622bc
	movs r5, #2
	ldrsh r2, [r6, r5]
	adds r1, r1, r3
	ldr r3, .L_081622c0
	ldrb r5, [r3, r4]
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r0, .L_081622c4
	ldrb r4, [r0, r4]
	str r5, [sp, #0]
	ldr r5, [sp, #84]
	lsrs r0, r4, #1
	str r4, [sp, #4]
	subs r3, r3, r0
	ldr r4, [r7, r5]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #6
	adds r0, r6, #0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	ldr r2, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #24]
	cmp r2, #1
	ble .L_08162298
	ldr r0, [sp, #112]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_08162298
	subs r3, r2, #1
	str r3, [r6, #8]
.L_08162298:
	movs r2, #1
	add r11, r2
	movs r1, #28
	mov r3, r11
	add r8, r1
	cmp r3, #16
	bne .L_0816221a
	bl .L_0816342a
	.2byte 0x0000
.L_081622ac:
	.4byte gMapCellBuffer
.L_081622b0:
	.4byte Data_02016000
.L_081622b4:
	.4byte Data_081977f8
.L_081622b8:
	.4byte Data_08197834
.L_081622bc:
	.4byte Data_02010800
.L_081622c0:
	.4byte Data_0819781a
.L_081622c4:
	.4byte Data_08197826
.L_081622c8:
	ldr r5, [sp, #124]
	cmp r5, #94
	beq .L_081622d0
	b .L_0816249c
.L_081622d0:
	ldr r0, [sp, #112]
	cmp r0, #0
	bne .L_08162380
	ldr r0, .L_08162320
	ldr r1, .L_08162324
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r3, .L_08162318
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #120]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r5, #238
	ldr r0, [sp, #112]
	lsls r5, r5, #7
	adds r5, #132
	adds r3, r1, r5
	str r0, [r3]
	movs r2, #128
	ldr r3, .L_0816231c
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r5, [sp, #120]
	movs r1, #0
	mov r11, r1
	movs r6, #31
	b .L_08162328
.L_08162318:
	.4byte 0x00000410
.L_0816231c:
	.4byte 0x00000100
.L_08162320:
	.4byte 0x000000e6
.L_08162324:
	.4byte Data_02012000
.L_08162328:
	bl Random16
	ands r0, r6
	adds r0, #48
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r6
	adds r0, #64
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	str r0, [r5, #8]
	bl Random16
	movs r3, #63
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #16
	negs r3, r3
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	movs r2, #1
	str r3, [r5, #20]
	add r11, r2
	movs r3, #0
	str r3, [r5, #24]
	mov r3, r11
	adds r5, #28
	cmp r3, #32
	bne .L_08162328
.L_08162380:
	ldr r5, [sp, #112]
	cmp r5, #14
	bne .L_081623b8
	ldr r2, [sp, #128]
	movs r3, #128
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #192
	lsls r1, r1, #10
	str r1, [sp, #0]
	movs r2, #128
	movs r1, #150
	lsls r2, r2, #10
	lsls r3, r3, #12
	str r1, [sp, #4]
	movs r1, #1
	bl Func_0815f000
	movs r0, #134
	bl Audio_PlayCue
	movs r5, #238
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	str r3, [r2]
.L_081623b8:
	ldr r1, [sp, #112]
	movs r0, #0
	mov r11, r0
	ldr r6, [sp, #120]
	cmp r1, #13
	bgt .L_0816241c
	ldr r2, [sp, #128]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_081623de
	ldr r5, [sp, #12]
	movs r3, #14
	subs r3, r3, r1
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r5]
	lsls r1, r2, #1
	adds r6, r3, r1
	b .L_081623f0
.L_081623de:
	ldr r0, [sp, #112]
	ldr r5, [sp, #12]
	movs r3, #14
	subs r3, r3, r0
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r5]
	lsls r1, r2, #1
	subs r6, r3, r1
.L_081623f0:
	ldr r3, [r5, #4]
	lsls r2, r2, #2
	subs r3, r3, r2
	adds r3, #24
	movs r5, #64
	subs r3, #32
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r1, .L_08162710
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	movs r2, #32
	mov lr, r4
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #19
	subs r5, r5, r6
	adds r3, #40
	lsls r5, r5, #8
	str r5, [r3]
	bl .L_0816342a
.L_0816241c:
	movs r1, #5
	mov r0, r11
	bl __modsi3
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r6, #8]
	bl __divsi3
	movs r1, #3
	bl __modsi3
	ldr r3, .L_08162714
	adds r5, r5, r0
	ldrb r4, [r3, r5]
	movs r0, #2
	ldrsh r2, [r6, r0]
	lsrs r3, r4, #1
	subs r2, r2, r3
	movs r1, #6
	ldrsh r3, [r6, r1]
	ldr r1, .L_08162718
	ldrb r0, [r1, r5]
	lsls r5, r5, #1
	lsrs r1, r0, #1
	subs r3, r3, r1
	ldr r1, .L_0816271c
	ldrh r1, [r1, r5]
	ldr r5, .L_08162720
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r1, r1, r5
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r6, #0
	lsls r2, r2, #8
	movs r1, #63
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #8]
	ldr r2, [r6, #20]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r0, [sp, #112]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0816248c
	cmp r2, #0
	ble .L_0816248c
	subs r3, r2, #1
	str r3, [r6, #20]
.L_0816248c:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r6, #28
	cmp r2, #32
	bne .L_0816241c
	bl .L_0816342a
.L_0816249c:
	ldr r3, [sp, #124]
	cmp r3, #84
	beq .L_081624a4
	b .L_081626de
.L_081624a4:
	ldr r5, [sp, #112]
	cmp r5, #0
	bne .L_081624ea
	ldr r5, .L_08162724
	movs r0, #0
	mov r11, r0
.L_081624b0:
	bl Random16
	str r0, [r5, #8]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	str r3, [r5, #20]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_081624d4
	ldr r3, [r5, #20]
	negs r3, r3
	str r3, [r5, #20]
.L_081624d4:
	bl Random16
	movs r1, #1
	movs r3, #3
	add r11, r1
	ands r3, r0
	mov r2, r11
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #32
	bne .L_081624b0
.L_081624ea:
	ldr r3, [sp, #112]
	cmp r3, #4
	bne .L_081624fe
	ldr r5, [sp, #120]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r5, r0
	movs r3, #8
	str r3, [r2]
.L_081624fe:
	ldr r1, [sp, #112]
	cmp r1, #6
	bne .L_0816250a
	movs r0, #221
	bl Audio_PlayCue
.L_0816250a:
	ldr r2, [sp, #112]
	cmp r2, #24
	bne .L_08162518
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
.L_08162518:
	ldr r3, [sp, #112]
	subs r3, #8
	cmp r3, #15
	bhi .L_081625e6
	movs r3, #0
	mov r11, r3
	movs r7, #3
.L_08162526:
	bl Random16
	mov r8, r11
	mov r5, r8
	movs r3, #255
	ands r5, r7
	lsls r3, r3, #8
	mov r8, r5
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, r0, #0
	ldr r0, [sp, #12]
	ldr r1, .L_08162728
	ldr r3, [r0]
	lsls r6, r6, #4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r8
	asrs r6, r6, #16
	adds r6, r6, r3
	ldrb r3, [r1, r2]
	adds r0, r5, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Trig_Cos
	ldr r3, .L_0816272c
	adds r5, r0, #0
	mov r0, r8
	mov r10, r3
	ldrb r3, [r3, r0]
	lsls r5, r5, #4
	lsrs r3, r3, #1
	movs r0, #188
	asrs r5, r5, #16
	subs r5, r5, r3
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Random16
	ldr r3, .L_08162730
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #1
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r2, .L_08162734
	mov r1, r8
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_08162738
	ldr r0, .L_08162728
	adds r1, r1, r2
	mov r2, r8
	ldrb r3, [r0, r2]
	mov r0, r10
	str r3, [sp, #0]
	adds r5, #80
	ldrb r3, [r0, r2]
	movs r2, #192
	str r3, [sp, #4]
	lsls r2, r2, #18
	adds r2, #188
	ldr r4, [r2]
	adds r3, r5, #0
	ldr r0, [sp, #116]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #128]
	movs r5, #1
	ldr r0, [r3, #4]
	ldr r1, [sp, #84]
	add r11, r5
	bl Func_08144aac
	mov r0, r11
	cmp r0, #3
	bne .L_08162526
.L_081625e6:
	ldr r1, [sp, #112]
	cmp r1, #3
	bgt .L_081625f0
	bl .L_0816342a
.L_081625f0:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0816273c
	ldr r3, [sp, #164]
	adds r5, r0, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08162740
	ldr r6, .L_08162724
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #164]
	movs r3, #6
	str r3, [r5]
	ldr r3, .L_08162744
	add r2, sp, #164
	str r3, [r5, #8]
	mov r3, r10
	str r3, [r5, #12]
	str r2, [r5, #16]
	mov r9, r2
	ldr r2, [sp, #112]
	ldr r1, [sp, #12]
	lsls r3, r2, #4
	movs r0, #0
	adds r7, r3, #0
	mov r11, r0
	mov r8, r1
	subs r7, #64
.L_0816263a:
	ldr r3, [sp, #112]
	cmp r3, r11
	blt .L_081626c0
	ldr r0, [sp, #112]
	mov r3, r11
	adds r3, #8
	cmp r0, r3
	bge .L_081626c0
	movs r2, #0
	cmp r0, r11
	bne .L_08162654
	subs r2, #16
	b .L_08162660
.L_08162654:
	ldr r1, [sp, #112]
	mov r3, r11
	adds r3, #4
	cmp r1, r3
	blt .L_08162660
	negs r2, r7
.L_08162660:
	str r2, [r5, #20]
	ldr r2, [r6, #24]
	ldr r0, .L_08162748
	lsls r3, r2, #12
	adds r3, r3, r0
	mov r1, r9
	str r3, [r1, #4]
	adds r2, #1
	movs r3, #3
	ands r2, r3
	str r2, [r6, #24]
	bl Func_08014de4
	mov r2, r8
	ldr r0, [r2]
	ldr r1, [r2, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r1, r1, #16
	movs r2, #0
	lsls r0, r0, #16
	bl Func_08015160
	ldr r0, [r6, #8]
	bl Func_080150e4
	ldr r3, [r6, #8]
	ldr r2, [r6, #20]
	movs r0, #128
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #9
	adds r1, r2, #0
	str r3, [r6, #8]
	lsls r0, r0, #11
	bl Func_080151e4
	ldr r0, .L_0816274c
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
.L_081626c0:
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r6, #28
	subs r7, #16
	cmp r0, #20
	bne .L_0816263a
	adds r0, r5, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
	bl .L_0816342a
.L_081626de:
	ldr r1, [sp, #124]
	cmp r1, #71
	beq .L_081626ee
	cmp r1, #80
	beq .L_081626ee
	cmp r1, #82
	beq .L_081626ee
	b .L_081628ba
.L_081626ee:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, [sp, #112]
	adds r6, r0, #0
	movs r7, #0
	cmp r2, #39
	ble .L_08162750
	movs r3, #40
	subs r3, r3, r2
	lsls r7, r3, #2
	b .L_0816275e
.L_08162710:
	.4byte Data_02012000
.L_08162714:
	.4byte Data_0819749e
.L_08162718:
	.4byte Data_081974ad
.L_0816271c:
	.4byte Data_081974bc
.L_08162720:
	.4byte Data_0201083c
.L_08162724:
	.4byte Data_02016e00
.L_08162728:
	.4byte Data_08197492
.L_0816272c:
	.4byte Data_08197498
.L_08162730:
	.4byte Data_0819894e
.L_08162734:
	.4byte Data_08197486
.L_08162738:
	.4byte gMapCellBuffer
.L_0816273c:
	.4byte 0xffffff00
.L_08162740:
	.4byte 0xffff00ff
.L_08162744:
	.4byte Data_08199268
.L_08162748:
	.4byte Data_02010c56
.L_0816274c:
	.4byte Data_081991b0
.L_08162750:
	ldr r3, [sp, #120]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
.L_0816275e:
	ldr r3, [sp, #156]
	ldr r2, .L_08162a30
	ldr r0, .L_08162a34
	ands r3, r2
	movs r2, #6
	orrs r3, r2
	ldr r2, .L_08162a38
	mov r1, r8
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #156]
	movs r3, #7
	str r3, [r6]
	ldr r3, .L_08162a3c
	add r2, sp, #156
	str r0, [r2, #4]
	str r2, [r6, #16]
	str r3, [r6, #8]
	str r1, [r6, #12]
	str r7, [r6, #20]
	bl Func_08014de4
	ldr r2, [sp, #12]
	ldr r0, [r2]
	ldr r1, [r2, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r0, r2, #0
	bl Func_080151e4
	ldr r3, [sp, #112]
	cmp r3, #8
	bne .L_081627c2
	movs r0, #104
	bl Audio_PlayCue
.L_081627c2:
	movs r5, #63
	negs r5, r5
	cmp r7, r5
	blt .L_0816283c
	ldr r0, [sp, #112]
	ldr r1, [sp, #124]
	lsls r7, r0, #12
	cmp r1, #82
	bne .L_081627ee
	ldr r0, .L_08162a40
	bl SceneTransform_ApplyPitch
	ldr r2, [sp, #112]
	negs r5, r2
	lsls r0, r5, #11
	lsls r5, r5, #4
	bl Func_08015068
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
	b .L_081627fc
.L_081627ee:
	ldr r0, .L_08162a44
	bl SceneTransform_ApplyPitch
	ldr r3, [sp, #112]
	lsls r0, r3, #11
	bl Func_08015068
.L_081627fc:
	adds r0, r7, #0
	bl Func_0801521c
	ldr r5, .L_08162a48
	mov r1, r8
	adds r0, r5, #0
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	ldr r0, [sp, #124]
	cmp r0, #82
	beq .L_0816283c
	ldr r1, [sp, #112]
	negs r0, r1
	lsls r0, r0, #12
	bl Func_08015068
	movs r0, #128
	lsls r0, r0, #8
	bl Func_0801521c
	adds r0, r5, #0
	mov r1, r8
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0816283c:
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	ldr r2, [sp, #124]
	cmp r2, #82
	beq .L_08162852
	bl .L_0816342a
.L_08162852:
	ldr r6, [sp, #112]
	cmp r6, #0
	bge .L_0816285a
	adds r6, #3
.L_0816285a:
	ldr r3, [sp, #112]
	asrs r6, r6, #2
	movs r7, #128
	lsls r1, r6, #1
	lsls r7, r7, #9
	cmp r3, #39
	ble .L_08162870
	lsls r2, r3, #12
	movs r3, #224
	lsls r3, r3, #10
	subs r7, r3, r2
.L_08162870:
	cmp r1, #1
	bgt .L_08162878
	bl .L_0816342a
.L_08162878:
	cmp r7, #0
	bgt .L_08162880
	bl .L_0816342a
.L_08162880:
	ldr r0, [sp, #120]
	movs r1, #224
	lsls r1, r1, #3
	adds r5, r0, r1
	lsls r6, r6, #2
	adds r1, r6, #0
	adds r2, r7, #0
	adds r0, r5, #0
	bl Func_0815b434
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #12]
	adds r0, r5, #0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	adds r3, r6, #0
	bl Func_0818caa8
	movs r0, #188
	movs r1, #15
	bl Func_081963ec
	bl .L_0816342a
.L_081628ba:
	ldr r3, [sp, #124]
	cmp r3, #81
	beq .L_081628ce
	cmp r3, #83
	beq .L_081628ce
	cmp r3, #92
	beq .L_081628ce
	cmp r3, #93
	beq .L_081628ce
	b .L_08162bca
.L_081628ce:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08162a30
	ldr r3, [sp, #148]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08162a38
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	ldr r5, .L_08162a34
	orrs r3, r2
	adds r7, r0, #0
	str r3, [sp, #148]
	add r3, sp, #148
	str r5, [r3, #4]
	str r3, [r7, #16]
	ldr r3, .L_08162a4c
	mov r0, r9
	str r1, [r7]
	str r3, [r7, #8]
	str r0, [r7, #12]
	ldr r1, [sp, #124]
	subs r1, #92
	str r1, [sp, #20]
	cmp r1, #1
	bhi .L_081629a4
	movs r3, #128
	ldr r5, [sp, #12]
	movs r2, #0
	lsls r3, r3, #8
	mov r11, r2
	mov r8, r3
.L_0816291e:
	ldr r1, [sp, #112]
	mov r0, r11
	lsls r3, r0, #1
	subs r2, r1, r3
	cmp r2, #0
	blt .L_08162998
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r6, r3, #12
	movs r3, #0
	cmp r2, #7
	ble .L_0816293c
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #3
.L_0816293c:
	movs r2, #64
	negs r2, r2
	str r3, [r7, #20]
	cmp r3, r2
	ble .L_08162998
	bl Func_08014de4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	mov r2, r8
	mov r0, r8
	lsls r1, r1, #9
	bl Func_080151e4
	mov r3, r11
	movs r1, #128
	lsls r0, r3, #14
	lsls r1, r1, #6
	adds r0, r0, r1
	bl Func_080150e4
	ldr r0, .L_08162a44
	bl SceneTransform_ApplyPitch
	asrs r0, r6, #1
	bl Func_0801521c
	ldr r0, .L_08162a48
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08162998:
	movs r2, #1
	add r11, r2
	mov r3, r11
	cmp r3, #3
	bne .L_0816291e
	b .L_08162ad0
.L_081629a4:
	ldr r1, [sp, #112]
	ldr r0, [sp, #12]
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r6, r3, #12
	ldr r3, [sp, #124]
	movs r2, #128
	movs r5, #0
	lsls r2, r2, #8
	mov r11, r5
	mov r8, r0
	adds r5, r1, #0
	mov r10, r2
	cmp r3, #83
	bne .L_08162a50
	adds r2, r1, #0
	subs r2, #4
	cmp r2, #0
	bge .L_081629cc
	b .L_08162ad0
.L_081629cc:
	lsls r5, r2, #13
	movs r3, #0
	cmp r2, #7
	ble .L_081629da
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #3
.L_081629da:
	movs r0, #63
	negs r0, r0
	str r3, [r7, #20]
	cmp r3, r0
	blt .L_08162ad0
	bl Func_08014de4
	ldr r1, [sp, #12]
	movs r2, #0
	ldr r0, [r1]
	ldr r1, [r1, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	adds r0, r5, #0
	bl Func_0801521c
	ldr r0, .L_08162a48
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	b .L_08162ad0
.L_08162a30:
	.4byte 0xffffff00
.L_08162a34:
	.4byte gMapCellBuffer
.L_08162a38:
	.4byte 0xffff00ff
.L_08162a3c:
	.4byte Data_08199340
.L_08162a40:
	.4byte 0xfffff800
.L_08162a44:
	.4byte 0xfffff000
.L_08162a48:
	.4byte Data_08199210
.L_08162a4c:
	.4byte Data_08199364
.L_08162a50:
	cmp r5, #0
	blt .L_08162ac0
	movs r3, #0
	cmp r5, #7
	ble .L_08162a60
	movs r3, #8
	subs r3, r3, r5
	lsls r3, r3, #3
.L_08162a60:
	movs r2, #64
	negs r2, r2
	str r3, [r7, #20]
	cmp r3, r2
	ble .L_08162ac0
	bl Func_08014de4
	mov r3, r8
	ldr r0, [r3]
	mov r2, r8
	lsrs r3, r0, #31
	ldr r1, [r2, #4]
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	mov r2, r10
	mov r0, r10
	lsls r1, r1, #9
	bl Func_080151e4
	mov r3, r11
	movs r1, #128
	lsls r0, r3, #14
	lsls r1, r1, #6
	adds r0, r0, r1
	bl Func_080150e4
	ldr r0, .L_08162d70
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	bl Func_0801521c
	ldr r0, .L_08162d74
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08162ac0:
	ldr r2, .L_08162d78
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r6, r6, r2
	subs r5, #4
	cmp r0, #3
	bne .L_08162a50
.L_08162ad0:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r9
	bl Sys_Free
	ldr r1, [sp, #20]
	cmp r1, #1
	bhi .L_08162ae6
	bl .L_0816342a
.L_08162ae6:
	ldr r2, [sp, #112]
	cmp r2, #4
	bne .L_08162b16
	ldr r5, .L_08162d7c
	movs r3, #0
	mov r11, r3
.L_08162af2:
	bl Random16
	movs r3, #0
	str r3, [r5, #4]
	str r0, [r5]
	bl Random16
	movs r3, #3
	ands r3, r0
	movs r0, #1
	adds r3, #3
	add r11, r0
	lsls r3, r3, #17
	mov r1, r11
	str r3, [r5, #16]
	adds r5, #28
	cmp r1, #64
	bne .L_08162af2
.L_08162b16:
	ldr r2, [sp, #112]
	cmp r2, #4
	bgt .L_08162b20
	bl .L_0816342a
.L_08162b20:
	ldr r5, [sp, #12]
	ldr r0, .L_08162d7c
	movs r3, #0
	mov r11, r3
	mov r10, r5
	mov r9, r0
.L_08162b2c:
	mov r6, r9
	ldr r1, [r6, #4]
	movs r7, #0
.L_08162b32:
	asrs r2, r1, #16
	lsls r3, r7, #1
	subs r5, r2, r3
	cmp r5, #0
	blt .L_08162b9c
	ldr r0, [r6]
	bl Trig_Sin
	mov r1, r10
	ldr r2, [r1]
	lsrs r3, r2, #31
	adds r2, r2, r3
	adds r3, r5, #0
	muls r3, r0
	asrs r2, r2, #1
	asrs r3, r3, #16
	adds r2, r2, r3
	ldr r0, [r6]
	mov r8, r2
	bl Trig_Cos
	adds r2, r5, #0
	muls r2, r0
	mov r5, r10
	ldr r3, [r5, #4]
	asrs r2, r2, #16
	adds r5, r3, r2
	adds r3, r7, #0
	cmp r7, #0
	bge .L_08162b70
	adds r3, r7, #7
.L_08162b70:
	asrs r3, r3, #3
	movs r0, #2
	subs r0, r0, r3
	ldr r2, .L_08162d80
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #104]
	mov r3, r8
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r2, r3, r2
	str r0, [sp, #0]
	subs r3, r5, r0
	str r4, [sp, #4]
	ldr r0, [sp, #116]
	ldr r4, [sp, #172]
	mov lr, r4
	.2byte 0xf800
	ldr r1, [r6, #4]
.L_08162b9c:
	adds r7, #1
	cmp r7, #12
	bne .L_08162b32
	ldr r2, [r6, #16]
	adds r3, r1, r2
	str r3, [r6, #4]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_08162bb4
	adds r3, #63
.L_08162bb4:
	movs r0, #1
	add r11, r0
	asrs r3, r3, #6
	movs r5, #28
	mov r1, r11
	str r3, [r6, #16]
	add r9, r5
	cmp r1, #64
	bne .L_08162b2c
	bl .L_0816342a
.L_08162bca:
	ldr r2, [sp, #124]
	cmp r2, #91
	beq .L_08162bd2
	b .L_08162dac
.L_08162bd2:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08162d84
	ldr r3, [sp, #140]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08162d88
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #140]
	ldr r3, .L_08162d8c
	add r5, sp, #140
	str r3, [r5, #4]
	ldr r3, .L_08162d90
	adds r7, r0, #0
	mov r0, r8
	str r1, [r7]
	str r5, [r7, #16]
	str r3, [r7, #8]
	str r0, [r7, #12]
	ldr r2, [sp, #112]
	subs r2, #4
	cmp r2, #0
	blt .L_08162c74
	lsls r6, r2, #14
	movs r3, #0
	cmp r2, #7
	ble .L_08162c22
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #3
.L_08162c22:
	movs r1, #63
	negs r1, r1
	str r3, [r7, #20]
	cmp r3, r1
	blt .L_08162c74
	bl Func_08014de4
	ldr r2, [sp, #12]
	movs r1, #160
	ldr r0, [r2]
	lsls r1, r1, #14
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	bl Func_0801521c
	ldr r0, .L_08162d74
	mov r1, r8
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08162c74:
	movs r3, #7
	strb r3, [r5]
	add r2, sp, #140
	movs r3, #4
	strb r3, [r2, #1]
	ldr r3, .L_08162d94
	str r3, [r2, #4]
	ldr r3, .L_08162d98
	str r3, [r7, #8]
	movs r3, #0
	mov r11, r3
.L_08162c8a:
	ldr r3, .L_08162d9c
	mov r5, r11
	ldrb r3, [r3, r5]
	ldr r0, [sp, #112]
	adds r2, r3, #4
	cmp r0, r2
	ble .L_08162d42
	subs r3, r2, r0
	lsls r1, r3, #3
	movs r3, #16
	negs r3, r3
	cmp r1, r3
	ble .L_08162ca8
	movs r1, #16
	negs r1, r1
.L_08162ca8:
	movs r5, #64
	negs r5, r5
	cmp r1, r5
	ble .L_08162d42
	ldr r3, .L_08162da0
	ldr r0, [sp, #112]
	mov r5, r11
	ldrb r3, [r3, r5]
	subs r2, r0, r2
	muls r2, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	adds r6, r2, #0
	muls r6, r3
	movs r0, #131
	lsls r0, r0, #7
	str r1, [r7, #20]
	adds r5, r6, r0
	bl Func_08014de4
	ldr r1, [sp, #12]
	mov r2, r11
	ldr r0, [r1]
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, .L_08162da4
	asrs r0, r0, #1
	ldrsb r1, [r3, r2]
	subs r0, #60
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	lsls r0, r5, #1
	lsls r1, r5, #2
	cmp r5, #0
	bge .L_08162cfe
	movs r3, #130
	lsls r3, r3, #7
	adds r3, #131
	adds r5, r6, r3
.L_08162cfe:
	asrs r2, r5, #2
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl SceneTransform_ApplyPitch
	mov r5, r11
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_08162d24
	ldr r1, [sp, #112]
	lsls r0, r5, #1
	adds r0, r0, r1
	lsls r0, r0, #12
	bl Func_08015068
	b .L_08162d32
.L_08162d24:
	ldr r3, [sp, #112]
	mov r2, r11
	lsls r0, r2, #1
	subs r0, r0, r3
	lsls r0, r0, #12
	bl Func_08015068
.L_08162d32:
	ldr r0, .L_08162da8
	mov r1, r8
	movs r2, #16
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08162d42:
	movs r5, #1
	add r11, r5
	mov r0, r11
	cmp r0, #2
	bne .L_08162c8a
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	ldr r1, [sp, #112]
	cmp r1, #4
	beq .L_08162d60
	b .L_0816342a
.L_08162d60:
	ldr r3, [sp, #120]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	str r3, [r2]
	b .L_0816342a
.L_08162d70:
	.4byte 0xfffff000
.L_08162d74:
	.4byte Data_08199210
.L_08162d78:
	.4byte 0xffff4000
.L_08162d7c:
	.4byte Data_02016000
.L_08162d80:
	.4byte Data_08197410
.L_08162d84:
	.4byte 0xffffff00
.L_08162d88:
	.4byte 0xffff00ff
.L_08162d8c:
	.4byte gMapCellBuffer
.L_08162d90:
	.4byte Data_08199364
.L_08162d94:
	.4byte Data_02014000
.L_08162d98:
	.4byte Data_08198d2c
.L_08162d9c:
	.4byte Data_08198952
.L_08162da0:
	.4byte Data_08198956
.L_08162da4:
	.4byte Data_0819895a
.L_08162da8:
	.4byte Data_08198c6c
.L_08162dac:
	ldr r0, [sp, #124]
	cmp r0, #90
	beq .L_08162db4
	b .L_08162ef4
.L_08162db4:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	movs r3, #6
	add r2, sp, #132
	strb r3, [r2]
	strb r3, [r2, #1]
	ldr r3, .L_081630c4
	adds r7, r0, #0
	str r3, [r2, #4]
	movs r3, #7
	str r3, [r7]
	ldr r3, .L_081630c8
	mov r1, r10
	str r2, [r7, #16]
	str r1, [r7, #12]
	str r3, [r7, #8]
	ldr r0, [sp, #112]
	movs r5, #0
	lsls r0, r0, #3
	mov r11, r5
	mov r8, r0
.L_08162dea:
	ldr r3, .L_081630cc
	mov r1, r11
	ldrb r3, [r3, r1]
	movs r2, #127
	adds r5, r3, #4
	mov r3, r8
	ands r3, r2
	strb r3, [r7, #25]
	ldr r2, [sp, #112]
	adds r3, r5, #4
	cmp r2, r3
	bne .L_08162e08
	movs r0, #104
	bl Audio_PlayCue
.L_08162e08:
	ldr r3, [sp, #112]
	cmp r3, r5
	ble .L_08162ec2
	subs r3, r5, r3
	lsls r3, r3, #3
	adds r1, r3, #0
	movs r0, #16
	adds r1, #56
	negs r0, r0
	cmp r1, r0
	ble .L_08162e22
	movs r1, #16
	negs r1, r1
.L_08162e22:
	movs r2, #64
	negs r2, r2
	cmp r1, r2
	ble .L_08162ec2
	ldr r3, [sp, #112]
	movs r0, #128
	subs r2, r3, r5
	ldr r3, .L_081630d0
	mov r5, r11
	ldrb r3, [r3, r5]
	lsls r0, r0, #7
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	lsls r6, r3, #4
	str r1, [r7, #20]
	adds r5, r6, r0
	bl Func_08014de4
	ldr r1, [sp, #12]
	movs r2, #0
	ldr r0, [r1]
	movs r1, #160
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #60
	lsls r1, r1, #14
	lsls r0, r0, #16
	bl Func_08015160
	ldr r3, .L_081630d4
	mov r2, r11
	ldrb r3, [r3, r2]
	adds r2, r5, #0
	muls r3, r5
	lsls r1, r3, #1
	cmp r5, #0
	bge .L_08162e7c
	movs r3, #128
	lsls r3, r3, #7
	adds r3, #3
	adds r2, r6, r3
.L_08162e7c:
	adds r0, r5, #0
	asrs r2, r2, #2
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl SceneTransform_ApplyPitch
	mov r5, r11
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_08162ea4
	ldr r1, [sp, #112]
	lsls r0, r5, #3
	adds r0, r0, r1
	lsls r0, r0, #10
	bl Func_08015068
	b .L_08162eb2
.L_08162ea4:
	ldr r3, [sp, #112]
	mov r2, r11
	lsls r0, r2, #3
	subs r0, r0, r3
	lsls r0, r0, #10
	bl Func_08015068
.L_08162eb2:
	ldr r0, .L_081630d8
	mov r1, r10
	movs r2, #32
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08162ec2:
	movs r0, #1
	add r11, r0
	movs r5, #32
	mov r1, r11
	add r8, r5
	cmp r1, #4
	bne .L_08162dea
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
	ldr r2, [sp, #112]
	cmp r2, #4
	beq .L_08162ee4
	b .L_0816342a
.L_08162ee4:
	ldr r3, [sp, #120]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #20
	str r3, [r2]
	b .L_0816342a
.L_08162ef4:
	ldr r0, [sp, #124]
	cmp r0, #15
	beq .L_08162efc
	b .L_08163026
.L_08162efc:
	ldr r1, [sp, #112]
	cmp r1, #0
	bne .L_08162f3a
	ldr r5, .L_081630dc
	movs r2, #0
	mov r11, r2
	movs r7, #80
	movs r6, #0
.L_08162f0c:
	str r6, [r5, #24]
	bl Random16
	str r0, [r5]
	bl Random16
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r0
	movs r1, #1
	movs r0, #128
	lsls r0, r0, #2
	add r11, r1
	adds r3, r3, r0
	mov r2, r11
	str r7, [r5, #8]
	str r3, [r5, #12]
	adds r7, #2
	subs r6, #2
	adds r5, #28
	cmp r2, #16
	bne .L_08162f0c
.L_08162f3a:
	ldr r0, [sp, #112]
	movs r3, #0
	mov r11, r3
	lsrs r3, r0, #31
	adds r3, r0, r3
	ldr r1, .L_081630c4
	ldr r0, .L_081630e0
	ldr r2, .L_081630e4
	movs r5, #128
	lsls r5, r5, #3
	asrs r4, r3, #1
.L_08162f50:
	ldrb r3, [r0]
	adds r0, #1
	lsrs r3, r3, #1
	cmp r3, r4
	bne .L_08162f5e
	ldrb r3, [r1]
	strb r3, [r2]
.L_08162f5e:
	movs r3, #1
	add r11, r3
	adds r2, #1
	adds r1, #1
	cmp r11, r5
	bne .L_08162f50
	ldr r1, [sp, #128]
	ldr r6, .L_081630dc
	movs r5, #36
	ldrsh r0, [r1, r5]
	add r5, sp, #180
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r3, [r5]
	mov r10, r5
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
	movs r2, #0
	mov r11, r2
.L_08162f8a:
	ldr r2, [r6, #24]
	mov r8, r6
	cmp r2, #0
	blt .L_08163012
	ldr r3, [r6, #8]
	cmp r3, #0
	ble .L_08163012
	movs r1, #3
	mov r0, r11
	bl __modsi3
	adds r5, r0, #0
	ldr r0, [r6]
	bl Trig_Sin
	ldr r3, [r6, #8]
	muls r3, r0
	mov r0, r10
	ldr r2, [r0]
	asrs r3, r3, #16
	ldr r0, [r6]
	adds r7, r2, r3
	bl Trig_Cos
	ldr r1, [r6, #8]
	adds r3, r1, #0
	muls r3, r0
	mov r0, r10
	ldr r2, [r0, #4]
	asrs r3, r3, #16
	adds r0, r2, r3
	ldr r3, [r6]
	ldr r2, [r6, #12]
	subs r1, #4
	adds r3, r3, r2
	str r1, [r6, #8]
	str r3, [r6]
	ldr r1, [sp, #124]
	cmp r1, #16
	bne .L_08162ff0
	movs r1, #32
	adds r3, r0, #0
	adds r2, r7, #0
	subs r2, #16
	str r1, [sp, #0]
	str r1, [sp, #4]
	subs r3, #16
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	ldr r1, .L_081630c4
	b .L_0816300c
.L_08162ff0:
	ldr r2, .L_081630c4
	lsls r1, r5, #3
	adds r1, r1, r5
	lsls r1, r1, #6
	adds r1, r1, r2
	adds r3, r0, #0
	adds r2, r7, #0
	movs r0, #24
	subs r2, #12
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r3, #12
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
.L_0816300c:
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r6, #24]
.L_08163012:
	movs r0, #1
	add r11, r0
	adds r3, r2, #1
	mov r5, r8
	mov r1, r11
	str r3, [r5, #24]
	adds r6, #28
	cmp r1, #16
	bne .L_08162f8a
	b .L_0816342a
.L_08163026:
	ldr r2, [sp, #124]
	cmp r2, #16
	bne .L_081630e8
	movs r5, #192
	movs r0, #128
	ldr r7, .L_081630dc
	movs r3, #0
	add r5, sp
	lsls r0, r0, #4
	mov r11, r3
	mov r10, r5
	mov r8, r0
.L_0816303e:
	mov r1, r11
	lsrs r3, r1, #31
	add r3, r11
	ldr r2, [sp, #112]
	asrs r3, r3, #1
	adds r3, #4
	cmp r2, r3
	blt .L_081630ac
	ldr r3, [r7, #24]
	cmp r3, #23
	bgt .L_081630ac
	movs r5, #0
	cmp r3, #11
	ble .L_08163062
	subs r3, #12
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r5, r3, #1
.L_08163062:
	mov r6, r10
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	movs r0, #32
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_081630c4
	asrs r2, r2, #1
	str r2, [r6]
	lsls r1, r5, #10
	adds r1, r1, r3
	ldr r5, [sp, #84]
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r3, #16
	ldr r4, [r5, #4]
	ldr r0, [sp, #116]
	subs r2, #16
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #62
	mov r2, r8
	bl BattleFxKernels_IntegrateVector3
	adds r0, r7, #0
	movs r1, #62
	mov r2, r8
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_081630ac:
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #32
	bne .L_0816303e
	ldr r2, [sp, #112]
	cmp r2, #4
	beq .L_081630c0
	b .L_081631b8
.L_081630c0:
	b .L_081631aa
	.2byte 0x0000
.L_081630c4:
	.4byte gMapCellBuffer
.L_081630c8:
	.4byte Data_081990d0
.L_081630cc:
	.4byte Data_0819895e
.L_081630d0:
	.4byte Data_08198962
.L_081630d4:
	.4byte Data_08198966
.L_081630d8:
	.4byte Data_08199090
.L_081630dc:
	.4byte Data_02016000
.L_081630e0:
	.4byte Data_02010400
.L_081630e4:
	.4byte Data_02010800
.L_081630e8:
	ldr r1, [sp, #124]
	cmp r1, #10
	bne .L_081631cc
	ldr r2, [sp, #112]
	cmp r2, #0
	bne .L_08163118
	ldr r3, [sp, #120]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	ldr r0, [sp, #120]
	movs r1, #238
	ldr r2, [sp, #112]
	lsls r1, r1, #7
	adds r1, #132
	adds r3, r0, r1
	str r2, [r3]
	movs r2, #128
	ldr r3, .L_08163128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_08163118:
	movs r5, #192
	ldr r7, .L_0816312c
	movs r3, #0
	add r5, sp
	mov r11, r3
	mov r8, r5
	b .L_08163130
	.2byte 0x0000
.L_08163128:
	.4byte 0x00000610
.L_0816312c:
	.4byte Data_02016000
.L_08163130:
	mov r0, r11
	lsrs r3, r0, #31
	add r3, r11
	ldr r1, [sp, #112]
	asrs r3, r3, #1
	adds r3, #4
	cmp r1, r3
	blt .L_08163198
	ldr r0, [r7, #24]
	cmp r0, #23
	bgt .L_08163198
	movs r5, #0
	cmp r0, #11
	ble .L_08163156
	subs r0, #12
	movs r1, #3
	bl __divsi3
	adds r5, r0, #0
.L_08163156:
	mov r6, r8
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	lsls r1, r5, #11
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, .L_081631c8
	asrs r2, r2, #1
	adds r1, r1, r3
	ldr r3, [r6, #4]
	movs r0, #32
	str r2, [r6]
	str r0, [sp, #0]
	movs r0, #64
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #172]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r7, #0
	movs r1, #58
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_08163198:
	movs r5, #1
	add r11, r5
	mov r0, r11
	adds r7, #28
	cmp r0, #24
	bne .L_08163130
	ldr r1, [sp, #112]
	cmp r1, #4
	bne .L_081631b8
.L_081631aa:
	ldr r3, [sp, #120]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #12
	str r3, [r2]
.L_081631b8:
	ldr r0, [sp, #112]
	cmp r0, #8
	beq .L_081631c0
	b .L_0816342a
.L_081631c0:
	movs r0, #103
	bl Audio_PlayCue
	b .L_0816342a
.L_081631c8:
	.4byte gMapCellBuffer
.L_081631cc:
	ldr r1, [sp, #124]
	cmp r1, #42
	bne .L_08163274
	ldr r2, [sp, #112]
	cmp r2, #47
	ble .L_081631ea
	ldr r5, [sp, #112]
	ldr r2, .L_081631fc
	ldr r1, .L_08163200
	movs r3, #128
	lsls r3, r3, #19
	subs r2, r2, r5
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_081631ea:
	ldr r7, .L_08163204
	movs r0, #0
	movs r1, #3
	movs r2, #1
	mov r11, r0
	mov r10, r1
	add r6, sp, #192
	mov r8, r2
	b .L_08163208
.L_081631fc:
	.4byte 0x00000040
.L_08163200:
	.4byte 0x00001000
.L_08163204:
	.4byte Data_02016000
.L_08163208:
	mov r0, r11
	movs r1, #3
	bl __modsi3
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	movs r0, #24
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r2, [r6]
	mov r4, r11
	mov r3, r8
	lsls r1, r5, #3
	ands r4, r3
	adds r1, r1, r5
	ldr r3, [r6, #4]
	ldr r5, .L_081633fc
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	lsls r4, r4, #2
	lsls r1, r1, #6
	ldr r4, [r4, r0]
	adds r1, r1, r5
	subs r2, #12
	subs r3, #12
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	mov r3, r11
	mov r1, r10
	ands r3, r1
	adds r3, #11
	mov r2, r8
	lsls r2, r3
	adds r0, r7, #0
	movs r1, #60
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	movs r2, #1
	adds r3, #1
	add r11, r2
	str r3, [r7, #24]
	mov r3, r11
	adds r7, #28
	cmp r3, #16
	bne .L_08163208
	b .L_0816342a
.L_08163274:
	ldr r5, [sp, #124]
	cmp r5, #76
	bne .L_081632b6
	ldr r0, [sp, #112]
	cmp r0, #18
	bne .L_08163288
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
.L_08163288:
	ldr r6, .L_08163400
	movs r1, #0
	mov r11, r1
	movs r5, #3
.L_08163290:
	ldr r2, [sp, #112]
	cmp r2, r5
	bne .L_081632a8
	movs r0, #212
	bl Audio_PlayCue
	movs r1, #128
	ldr r0, [sp, #116]
	lsls r1, r1, #7
	ldr r2, .L_08163404
	mov lr, r6
	.2byte 0xf800
.L_081632a8:
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r5, #3
	cmp r0, #3
	bne .L_08163290
	b .L_0816342a
.L_081632b6:
	ldr r1, [sp, #124]
	cmp r1, #74
	bne .L_08163356
	ldr r2, [sp, #112]
	cmp r2, #8
	bne .L_081632c8
	movs r0, #103
	bl Audio_PlayCue
.L_081632c8:
	movs r5, #192
	movs r0, #128
	ldr r7, .L_08163408
	movs r3, #0
	add r5, sp
	lsls r0, r0, #4
	mov r11, r3
	mov r10, r5
	mov r8, r0
.L_081632da:
	mov r1, r11
	lsrs r3, r1, #31
	add r3, r11
	ldr r2, [sp, #112]
	asrs r3, r3, #1
	adds r3, #4
	cmp r2, r3
	blt .L_08163348
	ldr r5, [r7, #24]
	cmp r5, #23
	bgt .L_08163348
	cmp r5, #0
	bge .L_081632f6
	adds r5, #3
.L_081632f6:
	mov r6, r10
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r2, [r6]
	asrs r5, r5, #2
	lsrs r3, r2, #31
	adds r2, r2, r3
	lsls r1, r5, #3
	ldr r3, .L_081633fc
	adds r1, r1, r5
	asrs r2, r2, #1
	movs r0, #24
	lsls r1, r1, #7
	str r2, [r6]
	adds r1, r1, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r5, [sp, #84]
	subs r3, #24
	ldr r4, [r5, #4]
	ldr r0, [sp, #116]
	subs r2, #12
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #62
	mov r2, r8
	bl BattleFxKernels_IntegrateVector3
	adds r0, r7, #0
	movs r1, #62
	mov r2, r8
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_08163348:
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #32
	bne .L_081632da
	b .L_0816342a
.L_08163356:
	ldr r2, [sp, #124]
	cmp r2, #100
	beq .L_0816342a
	ldr r7, .L_08163408
	movs r3, #0
	mov r11, r3
.L_08163362:
	ldr r5, [sp, #112]
	mov r3, r11
	adds r3, #4
	cmp r5, r3
	blt .L_0816341e
	ldr r3, [r7, #24]
	cmp r3, #23
	bgt .L_0816341e
	adds r1, r3, #0
	cmp r1, #0
	bge .L_0816337a
	adds r1, #3
.L_0816337a:
	add r6, sp, #192
	asrs r5, r1, #2
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0815e1ec
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r2, r3, #1
	str r2, [r6]
	ldr r0, [sp, #124]
	cmp r0, #69
	beq .L_0816339a
	cmp r0, #74
	bne .L_081633be
.L_0816339a:
	lsls r1, r5, #3
	ldr r3, .L_081633fc
	adds r1, r1, r5
	movs r0, #24
	lsls r1, r1, #7
	adds r1, r1, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r5, [sp, #84]
	subs r2, #12
	subs r3, #24
	ldr r4, [r5, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	b .L_081633e8
.L_081633be:
	lsls r1, r5, #3
	ldr r3, .L_081633fc
	mov r0, r11
	adds r1, r1, r5
	movs r4, #1
	ands r4, r0
	lsls r1, r1, #7
	movs r0, #24
	adds r1, r1, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r5, [sp, #84]
	lsls r4, r4, #2
	subs r2, #12
	subs r3, #24
	ldr r4, [r4, r5]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
.L_081633e8:
	ldr r0, [sp, #124]
	cmp r0, #55
	bne .L_0816340c
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #3
	bl BattleFxKernels_IntegrateVector3
	b .L_08163418
.L_081633fc:
	.4byte gMapCellBuffer
.L_08163400:
	.4byte IwramFillWords
.L_08163404:
	.4byte 0x2f2f2f2f
.L_08163408:
	.4byte Data_02016000
.L_0816340c:
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector3
.L_08163418:
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_0816341e:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r7, #28
	cmp r2, #16
	bne .L_08163362
.L_0816342a:
	ldr r3, [sp, #124]
	cmp r3, #8
	beq .L_08163432
	b .L_08163580
.L_08163432:
	ldr r5, [sp, #112]
	cmp r5, #43
	ble .L_0816343a
	b .L_08163580
.L_0816343a:
	add r6, sp, #192
	adds r1, r6, #0
	ldr r0, [sp, #16]
	bl Func_0815e1ec
	ldr r2, [r6]
	movs r0, #20
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r2, [r6]
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	ldr r1, .L_08163730
	ldr r4, [r0, #4]
	subs r2, #10
	subs r3, #4
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	cmp r5, #23
	bne .L_0816347c
	ldr r1, [sp, #128]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_0816347c:
	ldr r2, [sp, #112]
	cmp r2, #19
	ble .L_0816348e
	ldr r5, [sp, #16]
	movs r0, #128
	ldr r3, [r5, #4]
	lsls r0, r0, #11
	adds r3, r3, r0
	str r3, [r5, #4]
.L_0816348e:
	ldr r1, [sp, #112]
	cmp r1, #23
	bne .L_081634fa
	ldr r3, [sp, #128]
	movs r5, #240
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	lsls r5, r5, #13
	adds r7, r0, r5
	ldr r5, [sp, #120]
	movs r0, #0
	mov r11, r0
	movs r6, #255
.L_081634b2:
	ldr r1, [sp, #60]
	ldr r3, [r1, #8]
	str r7, [r5, #4]
	str r3, [r5]
	ldr r3, [r1, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	mov r2, r11
	lsrs r3, r2, #31
	add r3, r11
	asrs r3, r3, #1
	adds r3, #32
	ands r0, r6
	str r3, [r5, #24]
	subs r0, #127
	movs r3, #1
	lsls r0, r0, #11
	add r11, r3
	str r0, [r5, #20]
	mov r0, r11
	adds r5, #28
	cmp r0, #64
	bne .L_081634b2
.L_081634fa:
	ldr r1, [sp, #112]
	cmp r1, #24
	bne .L_08163542
	ldr r3, [sp, #128]
	movs r1, #1
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #144
	lsls r3, r3, #12
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r2, #0
	movs r3, #0
	bl Func_0815f000
	ldr r1, [sp, #128]
	movs r3, #4
	movs r5, #36
	ldrsh r0, [r1, r5]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	movs r0, #134
	bl Func_081180e8
	movs r5, #238
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	str r3, [r2]
.L_08163542:
	ldr r2, [sp, #112]
	subs r2, #24
	cmp r2, #11
	bhi .L_08163580
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #4
	ldr r0, [sp, #120]
	subs r1, r1, r3
	ldr r3, [sp, #12]
	lsls r1, r1, #6
	movs r2, #176
	adds r1, r0, r1
	lsls r2, r2, #7
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r5, [sp, #84]
	lsrs r3, r2, #31
	adds r2, r2, r3
	movs r3, #20
	str r3, [sp, #0]
	movs r3, #48
	str r3, [sp, #4]
	asrs r2, r2, #1
	subs r2, #8
	ldr r4, [r5, #4]
	ldr r0, [sp, #116]
	movs r3, #28
	mov lr, r4
	.2byte 0xf800
.L_08163580:
	ldr r0, [sp, #124]
	cmp r0, #66
	bne .L_081635c4
	movs r1, #0
	mov r11, r1
	movs r6, #2
	movs r5, #8
.L_0816358e:
	ldr r2, [sp, #112]
	cmp r2, r5
	bne .L_081635b8
	ldr r1, [sp, #128]
	movs r2, #5
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #0
	movs r1, #7
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r0, #238
	ldr r2, [sp, #120]
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r2, r0
	str r6, [r3]
	movs r0, #133
	bl Audio_PlayCue
.L_081635b8:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r5, #4
	cmp r2, #4
	bne .L_0816358e
.L_081635c4:
	ldr r3, [sp, #124]
	cmp r3, #37
	bgt .L_0816361a
	ldr r5, [sp, #112]
	cmp r5, #5
	bgt .L_0816361a
	add r5, sp, #192
	adds r1, r5, #0
	ldr r0, [sp, #16]
	bl Func_0815e1ec
	ldr r2, [r5]
	movs r0, #20
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r2, [r5]
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	ldr r1, .L_08163730
	subs r2, #10
	subs r3, #4
	ldr r4, [r0, #4]
	ldr r0, [sp, #116]
	mov lr, r4
	.2byte 0xf800
	ldr r1, [sp, #16]
	ldr r5, [sp, #56]
	ldr r3, [r1]
	ldr r2, [r5]
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r1, #4]
	ldr r2, [r5, #4]
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r3, [r1, #8]
	ldr r2, [r5, #8]
	adds r3, r3, r2
	str r3, [r1, #8]
.L_0816361a:
	ldr r0, [sp, #124]
	cmp r0, #8
	beq .L_0816362e
	ldr r1, [sp, #112]
	cmp r1, #3
	bne .L_0816362e
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
.L_0816362e:
	ldr r2, [sp, #112]
	cmp r2, #4
	bne .L_0816365e
	ldr r3, [sp, #124]
	cmp r3, #91
	bne .L_08163642
	movs r0, #145
	bl Audio_PlayCue
	b .L_0816365e
.L_08163642:
	ldr r5, [sp, #124]
	cmp r5, #78
	beq .L_08163650
	cmp r5, #81
	beq .L_08163650
	cmp r5, #98
	bne .L_08163658
.L_08163650:
	movs r0, #144
	bl Audio_PlayCue
	b .L_0816365e
.L_08163658:
	movs r0, #134
	bl Audio_PlayCue
.L_0816365e:
	ldr r0, [sp, #112]
	cmp r0, #6
	beq .L_08163666
	b .L_0816378a
.L_08163666:
	ldr r1, [sp, #48]
	cmp r1, #1
	bls .L_081636ae
	ldr r2, [sp, #124]
	cmp r2, #7
	beq .L_081636ae
	cmp r2, #43
	beq .L_081636ae
	cmp r2, #48
	beq .L_081636ae
	cmp r2, #73
	beq .L_081636ae
	cmp r2, #77
	beq .L_081636ae
	cmp r2, #78
	beq .L_081636ae
	cmp r2, #49
	beq .L_081636ae
	cmp r2, #12
	beq .L_081636ae
	cmp r2, #53
	beq .L_081636ae
	cmp r2, #64
	beq .L_081636ae
	cmp r2, #100
	beq .L_081636ae
	cmp r2, #66
	beq .L_081636ae
	cmp r2, #81
	beq .L_081636ae
	cmp r2, #83
	beq .L_081636ae
	cmp r2, #76
	beq .L_081636ae
	cmp r2, #98
	bne .L_081636c6
.L_081636ae:
	ldr r5, [sp, #128]
	movs r1, #4
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Func_08118088
	movs r1, #238
	ldr r0, [sp, #120]
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	b .L_0816376c
.L_081636c6:
	ldr r2, [sp, #124]
	cmp r2, #8
	beq .L_081636d4
	cmp r2, #79
	beq .L_081636d4
	cmp r2, #85
	bne .L_081636e2
.L_081636d4:
	ldr r3, [sp, #120]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	b .L_0816376e
.L_081636e2:
	ldr r0, [sp, #124]
	cmp r0, #50
	beq .L_081636f0
	cmp r0, #44
	beq .L_081636f0
	cmp r0, #63
	bne .L_0816370a
.L_081636f0:
	ldr r2, [sp, #128]
	movs r5, #238
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #1
	bl Func_08118088
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #2
	b .L_0816376e
.L_0816370a:
	ldr r0, [sp, #124]
	cmp r0, #60
	beq .L_08163714
	cmp r0, #38
	bne .L_08163734
.L_08163714:
	ldr r2, [sp, #128]
	movs r5, #238
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #3
	bl Func_08118088
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #16
	b .L_0816376e
	.2byte 0x0000
.L_08163730:
	.4byte Data_02013c56
.L_08163734:
	ldr r0, [sp, #124]
	cmp r0, #95
	beq .L_08163756
	cmp r0, #99
	beq .L_08163756
	cmp r0, #96
	beq .L_08163756
	cmp r0, #97
	beq .L_08163756
	cmp r0, #92
	beq .L_08163756
	cmp r0, #93
	beq .L_08163756
	cmp r0, #86
	beq .L_08163756
	cmp r0, #74
	bne .L_08163770
.L_08163756:
	ldr r2, [sp, #128]
	movs r5, #238
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #0
	bl Func_08118088
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
.L_0816376c:
	movs r3, #12
.L_0816376e:
	str r3, [r2]
.L_08163770:
	ldr r0, [sp, #112]
	cmp r0, #6
	bne .L_0816378a
	ldr r2, [sp, #128]
	movs r3, #4
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_0816378a:
	ldr r3, [sp, #112]
	cmp r3, #14
	bne .L_081637a4
	ldr r1, [sp, #128]
	movs r3, #4
	movs r5, #36
	ldrsh r0, [r1, r5]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
.L_081637a4:
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r5, #240
	ldr r3, [sp, #120]
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #112]
	ldr r1, [sp, #92]
	adds r0, #1
	str r0, [sp, #112]
	cmp r0, r1
	beq .L_081637d4
	bl .L_0816031e
.L_081637d4:
	ldr r2, [sp, #124]
	cmp r2, #51
	beq .L_081637f6
	cmp r2, #67
	beq .L_081637f6
	cmp r2, #9
	beq .L_081637f6
	cmp r2, #10
	beq .L_081637f6
	cmp r2, #11
	beq .L_081637f6
	cmp r2, #70
	beq .L_081637f6
	cmp r2, #79
	beq .L_081637f6
	cmp r2, #90
	bne .L_08163868
.L_081637f6:
	movs r1, #240
	ldr r5, .L_081638bc
	lsls r1, r1, #6
	ldr r0, .L_081638c0
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #116]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r5, [sp, #128]
	movs r3, #0
	str r3, [r5, #28]
	ldr r0, .L_081638c4
	bl Scheduler_RemoveCallback
	ldr r0, .L_081638c8
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #124]
	cmp r0, #51
	beq .L_08163840
	cmp r0, #10
	beq .L_08163840
	cmp r0, #70
	beq .L_08163840
	cmp r0, #79
	beq .L_08163840
	cmp r0, #90
	bne .L_08163848
.L_08163840:
	ldr r0, [sp, #128]
	bl Func_081504c0
	b .L_081638ac
.L_08163848:
	ldr r1, [sp, #124]
	cmp r1, #9
	beq .L_08163852
	cmp r1, #67
	bne .L_0816385a
.L_08163852:
	ldr r0, [sp, #128]
	bl Func_081504cc
	b .L_081638ac
.L_0816385a:
	ldr r2, [sp, #124]
	cmp r2, #11
	bne .L_081638ac
	ldr r0, [sp, #128]
	bl Func_081504b4
	b .L_081638ac
.L_08163868:
	ldr r3, [sp, #52]
	cmp r3, #1
	bls .L_08163890
	ldr r5, [sp, #124]
	cmp r5, #42
	beq .L_08163890
	cmp r5, #15
	beq .L_08163890
	cmp r5, #52
	beq .L_08163890
	cmp r5, #72
	beq .L_08163890
	cmp r5, #58
	beq .L_08163890
	cmp r5, #59
	beq .L_08163890
	cmp r5, #69
	beq .L_08163890
	cmp r5, #89
	bne .L_08163896
.L_08163890:
	ldr r0, .L_081638cc
	bl Scheduler_RemoveCallback
.L_08163896:
	ldr r0, .L_081638c8
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
.L_081638ac:
	add sp, #264
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081638bc:
	.4byte IwramClearWords
.L_081638c0:
	.4byte 0x06004000
.L_081638c4:
	.4byte Func_08143488
.L_081638c8:
	.4byte Func_08143000
.L_081638cc:
	.4byte Func_08152474
