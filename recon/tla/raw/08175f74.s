.syntax unified
	.thumb
	.global Func_08175f74
	.thumb_func
Func_08175f74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #324
	str r0, [sp, #100]
	movs r2, #192
	lsls r2, r2, #18
	ldr r0, [r2, #96]
	adds r3, r2, #0
	str r0, [sp, #96]
	adds r3, #176
	ldr r1, [r2, #92]
	movs r0, #0
	str r1, [sp, #92]
	ldr r6, .L_08176000
	ldr r3, [r3]
	mov r11, r6
	str r3, [sp, #88]
	ldr r3, .L_08176004
	movs r6, #1
	ldrh r4, [r3, #4]
	mov r10, r3
	str r4, [sp, #76]
	ldr r5, [r2, #100]
	str r5, [sp, #72]
	movs r5, #240
	ldr r2, [r2, #36]
	str r2, [sp, #68]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	ldr r3, .L_08175ff8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r2, .L_08175ffc
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r7, [sp, #92]
	movs r0, #239
	lsls r0, r0, #7
	adds r3, r7, r0
	movs r1, #0
	str r1, [r3]
	mov r8, r1
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08176008
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	movs r1, #0
	bl Func_08163c2c
	ldr r3, .L_0817600c
	mov r4, r10
	b .L_08176010
.L_08175ff8:
	.4byte 0x00000784
.L_08175ffc:
	.4byte 0x00000000
.L_08176000:
	.4byte gMapCellBuffer
.L_08176004:
	.4byte Data_03001120
.L_08176008:
	.4byte Func_08143000
.L_0817600c:
	.4byte gCameraSceneParameters
.L_08176010:
	str r5, [r3, #16]
	ldr r2, [sp, #88]
	mov r3, r8
	add r0, sp, #264
	strh r3, [r4, #4]
	movs r3, #255
	movs r1, #0
	str r6, [r2, #16]
	strh r3, [r0]
	bl Func_08118010
	ldr r3, .L_08176064
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #64
	strh r5, [r3]
	movs r0, #1
	bl WaitFrames
	bl Func_08014c4c
	movs r7, #237
	ldr r5, [sp, #68]
	lsls r7, r7, #3
	adds r7, #255
	adds r3, r5, r7
	strb r6, [r3]
	ldr r1, .L_08176068
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	movs r2, #20
	negs r2, r2
	adds r1, r2, #0
	movs r0, #0
	b .L_0817606c
	.2byte 0x0000
.L_08176064:
	.4byte 0x00002737
.L_08176068:
	.4byte 0x00000075
.L_0817606c:
	bl Func_08164b2c
	movs r0, #1
	movs r1, #1
	bl Func_08163c2c
	ldr r3, .L_081760b0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_081760b4
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_081760b8
	subs r2, #48
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	mov r1, sp
	adds r3, #40
	mov r0, r8
	str r0, [r3]
	adds r1, #176
	str r1, [sp, #64]
	bl Func_08144aac
	ldr r0, .L_081760bc
	bl Resource_GetTableEntry
	movs r2, #160
	adds r7, r0, #0
	b .L_081760c0
	.2byte 0x0000
.L_081760b0:
	.4byte 0x00001010
.L_081760b4:
	.4byte 0x00003f44
.L_081760b8:
	.4byte 0x00000080
.L_081760bc:
	.4byte 0x0000009c
.L_081760c0:
	adds r1, r7, #0
	ldr r3, .L_081762e0
	lsls r2, r2, #1
	ldr r0, .L_081762e4
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #92]
	movs r2, #160
	movs r4, #224
	lsls r2, r2, #1
	lsls r4, r4, #3
	adds r7, r7, r2
	adds r5, r3, r4
	adds r1, r5, #0
	adds r0, r7, #0
	bl Func_0801587c
	movs r6, #0
	movs r1, #128
	mov r9, r6
	lsls r1, r1, #5
.L_081760ea:
	ldrb r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_081760f6
	adds r3, #32
	strb r3, [r5]
.L_081760f6:
	movs r7, #1
	add r9, r7
	adds r5, #1
	cmp r9, r1
	bne .L_081760ea
	ldr r2, .L_081762e8
	movs r1, #64
	movs r3, #0
	movs r0, #64
	bl Func_0815b290
	movs r2, #238
	ldr r1, [sp, #92]
	ldrb r3, [r0, #9]
	lsls r2, r2, #7
	adds r2, #220
	adds r1, r1, r2
	movs r2, #12
	orrs r3, r2
	strb r3, [r0, #9]
	ldr r3, .L_081762ec
	str r0, [r1]
	mov r10, r3
	ldrb r3, [r0, #16]
	ldr r5, [sp, #92]
	lsls r3, r3, #2
	add r3, r10
	ldr r4, .L_081762f0
	ldrh r0, [r3, #2]
	movs r7, #224
	lsls r7, r7, #3
	adds r6, r5, r7
	movs r2, #128
	adds r0, r0, r4
	ldr r5, .L_081762e0
	mov r8, r1
	lsls r2, r2, #5
	adds r1, r6, #0
	mov lr, r5
	.2byte 0xf800
	mov r0, r8
	ldr r2, [r0]
	movs r3, #13
	ldrb r1, [r2, #5]
	negs r3, r3
	ands r3, r1
	strb r3, [r2, #5]
	movs r3, #32
	strb r3, [r2, #23]
	ldr r0, .L_081762f4
	bl Resource_GetTableEntry
	adds r7, r0, #0
	adds r1, r7, #0
	movs r2, #32
	adds r7, #32
	ldr r0, .L_081762f8
	mov lr, r5
	.2byte 0xf800
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0801587c
	movs r2, #146
	lsls r2, r2, #1
	add r2, sp
	str r2, [sp, #24]
	ldr r7, .L_081762fc
	movs r1, #0
	mov r8, r5
	mov r9, r1
	adds r5, r2, #0
.L_08176186:
	movs r3, #240
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	movs r0, #16
	bl Func_0815b290
	movs r4, #13
	ldrb r2, [r0, #9]
	negs r4, r4
	adds r3, r4, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldrb r3, [r0, #16]
	str r0, [r7]
	lsls r3, r3, #2
	add r3, r10
	strb r2, [r0, #9]
	ldr r1, .L_081762f0
	ldrh r0, [r3, #2]
	movs r2, #32
	adds r0, r0, r1
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldmia r7!, {r3}
	movs r2, #1
	ldrh r3, [r3, #8]
	add r9, r2
	lsls r3, r3, #22
	lsrs r3, r3, #22
	strh r3, [r5]
	mov r3, r9
	adds r5, #2
	adds r6, #32
	cmp r3, #16
	bne .L_08176186
	ldr r6, .L_081762e0
	ldr r5, .L_08176300
	movs r4, #0
	mov r9, r4
.L_081761da:
	movs r3, #240
	movs r1, #8
	movs r2, #0
	lsls r3, r3, #8
	movs r0, #8
	bl Func_0815b3b0
	mov r7, r11
	stmia r5!, {r0}
	ldr r1, [r7]
	movs r2, #24
	mov lr, r6
	.2byte 0xf800
	movs r0, #1
	add r9, r0
	mov r1, r9
	cmp r1, #32
	bne .L_081761da
	ldr r5, .L_08176304
	ldr r1, [sp, #72]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #92]
	movs r6, #224
	lsls r6, r6, #3
	adds r0, r5, #0
	adds r1, r2, r6
	movs r3, #0
	movs r2, #0
	bl Func_08157cf4
	ldr r4, .L_08176308
	movs r3, #0
	str r3, [sp, #84]
	movs r5, #32
	mov r12, r4
	movs r7, #6
	mov lr, r5
	movs r4, #0
	movs r0, #0
.L_08176230:
	ldr r3, [sp, #72]
	movs r1, #0
	lsls r2, r4, #1
	mov r9, r1
	adds r2, r2, r3
	adds r1, r0, #0
.L_0817623c:
	mov r5, r12
	ldrh r3, [r5, r7]
	ldr r5, [sp, #92]
	add r3, r9
	adds r3, r3, r6
	ldrb r3, [r5, r3]
	cmp r3, #0
	beq .L_08176254
	subs r3, r3, r1
	cmp r3, #0
	bgt .L_08176254
	movs r3, #1
.L_08176254:
	strb r3, [r2]
	movs r3, #1
	add r9, r3
	adds r2, #1
	cmp r9, lr
	bne .L_0817623c
	ldr r5, [sp, #84]
	adds r4, #16
	adds r5, #1
	adds r0, #7
	str r5, [sp, #84]
	cmp r5, #10
	bne .L_08176230
	ldr r6, [sp, #92]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r6, r7
	ldr r0, .L_0817630c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r2, #184
	lsls r2, r2, #5
	adds r1, r6, r2
	movs r3, #1
	movs r2, #1
	ldr r0, .L_08176310
	bl Func_08157cf4
	ldr r0, .L_08176314
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r5, .L_081762e0
	lsls r0, r0, #19
	adds r1, r7, #0
	mov lr, r5
	.2byte 0xf800
	ldr r3, .L_081762dc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	movs r3, #128
	adds r2, #28
	lsls r3, r3, #4
	str r3, [r2]
	movs r3, #0
	str r3, [sp, #60]
	str r3, [sp, #56]
	str r3, [sp, #80]
	ldr r3, .L_08176318
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081762d0
	bl .L_08177932
.L_081762d0:
	ldr r5, .L_0817631c
	movs r4, #0
	str r4, [sp, #20]
	str r5, [sp, #16]
	b .L_08176320
	.2byte 0x0000
.L_081762dc:
	.4byte 0x00000784
.L_081762e0:
	.4byte IwramCopyWords
.L_081762e4:
	.4byte 0x05000240
.L_081762e8:
	.4byte 0xc0002000
.L_081762ec:
	.4byte ResourceTableEntries
.L_081762f0:
	.4byte 0x06010000
.L_081762f4:
	.4byte 0x0000009e
.L_081762f8:
	.4byte 0x050003e0
.L_081762fc:
	.4byte gMapCellBuffer
.L_08176300:
	.4byte Data_02010040
.L_08176304:
	.4byte 0x00000134
.L_08176308:
	.4byte Data_08197410
.L_0817630c:
	.4byte 0x000000b4
.L_08176310:
	.4byte 0x000000b6
.L_08176314:
	.4byte 0x000000bd
.L_08176318:
	.4byte gInput
.L_0817631c:
	.4byte Data_02013e18
.L_08176320:
	ldr r0, [sp, #80]
	subs r0, #32
	cmp r0, #21
	bhi .L_08176336
	ldr r2, [sp, #80]
	movs r1, #20
	negs r0, r0
	subs r2, #52
	negs r1, r1
	bl Func_08164b2c
.L_08176336:
	ldr r6, [sp, #80]
	cmp r6, #55
	bne .L_0817634a
	movs r1, #10
	movs r2, #20
	movs r0, #0
	negs r1, r1
	negs r2, r2
	bl Func_08164b2c
.L_0817634a:
	ldr r7, [sp, #80]
	cmp r7, #4
	bne .L_08176356
	movs r0, #107
	bl Audio_PlayCue
.L_08176356:
	ldr r0, [sp, #80]
	cmp r0, #55
	bne .L_08176362
	movs r0, #208
	bl Audio_PlayCue
.L_08176362:
	ldr r1, [sp, #80]
	cmp r1, #78
	bne .L_0817636e
	movs r0, #219
	bl Audio_PlayCue
.L_0817636e:
	ldr r2, [sp, #80]
	cmp r2, #127
	bne .L_0817637a
	movs r0, #212
	bl Audio_PlayCue
.L_0817637a:
	ldr r3, [sp, #80]
	cmp r3, #131
	bne .L_08176386
	movs r0, #149
	bl Audio_PlayCue
.L_08176386:
	ldr r4, [sp, #80]
	cmp r4, #206
	bne .L_08176392
	movs r0, #142
	bl Audio_PlayCue
.L_08176392:
	ldr r5, [sp, #80]
	cmp r5, #244
	bne .L_0817639e
	movs r0, #212
	bl Audio_PlayCue
.L_0817639e:
	ldr r6, [sp, #80]
	movs r7, #151
	lsls r7, r7, #1
	cmp r6, r7
	bne .L_081763ae
	movs r0, #212
	bl Audio_PlayCue
.L_081763ae:
	ldr r0, [sp, #80]
	movs r1, #154
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_081763be
	movs r0, #212
	bl Audio_PlayCue
.L_081763be:
	ldr r2, [sp, #80]
	movs r3, #156
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_081763ce
	movs r0, #212
	bl Audio_PlayCue
.L_081763ce:
	ldr r4, [sp, #80]
	movs r5, #175
	lsls r5, r5, #1
	cmp r4, r5
	bne .L_081763de
	movs r0, #104
	bl Audio_PlayCue
.L_081763de:
	ldr r6, [sp, #80]
	cmp r6, #0
	bne .L_08176456
	ldr r7, [sp, #92]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #75
	str r3, [r2]
	movs r2, #128
	ldr r3, .L_08176428
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r2, #0
	mov r9, r2
	movs r5, #240
	ldr r4, .L_0817642c
	ldr r0, .L_08176430
	ldr r1, .L_08176434
	ldr r2, .L_08176438
	lsls r5, r5, #16
	adds r5, #160
.L_08176418:
	str r5, [r2]
	ldr r3, [r0]
	adds r0, #28
	cmp r3, #0
	bge .L_0817643c
	adds r3, #15
	b .L_0817643c
	.2byte 0x0000
.L_08176428:
	.4byte 0x00001010
.L_0817642c:
	.4byte 0x0000f000
.L_08176430:
	.4byte Data_02014218
.L_08176434:
	.4byte Data_02014004
.L_08176438:
	.4byte Data_02014000
.L_0817643c:
	ldr r6, [sp, #24]
	asrs r3, r3, #4
	lsls r3, r3, #1
	ldrh r3, [r6, r3]
	movs r7, #1
	orrs r3, r4
	add r9, r7
	str r3, [r1]
	mov r3, r9
	adds r1, #8
	adds r2, #8
	cmp r3, #64
	bne .L_08176418
.L_08176456:
	ldr r4, [sp, #80]
	subs r4, #132
	mov r11, r4
	cmp r4, #107
	bls .L_08176462
	b .L_081765b2
.L_08176462:
	ldr r3, .L_08176558
	movs r5, #168
	ldr r4, [r3, #4]
	ldr r3, [r3]
	add r5, sp
	str r3, [sp, #168]
	str r4, [sp, #172]
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #4]
	ldr r6, [sp, #80]
	mov r8, r5
	str r3, [sp, #168]
	cmp r6, #132
	bne .L_081764d4
	ldr r5, .L_0817655c
	movs r7, #0
	mov r9, r7
	movs r6, #255
.L_08176488:
	bl Random16
	movs r1, #160
	bl Math_ModU
	adds r0, #40
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	str r0, [r5, #24]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #9
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	negs r0, r0
	subs r0, #128
	lsls r0, r0, #8
	str r0, [r5, #16]
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r5, #28
	cmp r1, #128
	bne .L_08176488
.L_081764d4:
	ldr r7, .L_08176560
	ldr r6, .L_08176564
	movs r2, #0
	movs r4, #128
	mov r9, r2
	add r5, sp, #248
	lsls r4, r4, #9
.L_081764e2:
	mov r3, r9
	cmp r3, #10
	bne .L_081764ee
	mov r0, r8
	str r4, [r0, #4]
	str r4, [sp, #168]
.L_081764ee:
	movs r3, #0
	str r3, [r5, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r5, #4]
	movs r1, #128
	ldr r3, [r6]
	lsls r1, r1, #14
	str r3, [r5]
	ldr r0, [r7]
	ldr r3, [r6, #4]
	mov r2, r8
	adds r3, r3, r1
	str r3, [r5, #8]
	adds r1, r5, #0
	movs r3, #0
	str r4, [sp, #8]
	bl Func_08020010
	ldr r3, [r6, #24]
	ldr r0, [r7]
	ldr r4, [sp, #8]
	cmp r3, #0
	bge .L_08176520
	adds r3, #15
.L_08176520:
	ldr r2, [sp, #24]
	asrs r3, r3, #4
	lsls r3, r3, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_08176550
	ldr r2, .L_08176554
	ands r1, r3
	ldrh r3, [r0, #8]
	adds r7, #4
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	movs r3, #1
	add r9, r3
	mov r0, r9
	adds r6, #28
	cmp r0, #48
	bne .L_081764e2
	movs r1, #0
	mov r9, r1
	ldr r4, .L_08176568
	ldr r1, .L_0817655c
	b .L_0817656c
	.2byte 0x0000
.L_08176550:
	.4byte 0x000003ff
.L_08176554:
	.4byte 0xfffffc00
.L_08176558:
	.4byte Data_08196ea8
.L_0817655c:
	.4byte Data_02014200
.L_08176560:
	.4byte gMapCellBuffer
.L_08176564:
	.4byte Data_02014900
.L_08176568:
	.4byte 0xfff00000
.L_0817656c:
	ldr r0, [r1, #4]
	cmp r0, r4
	ble .L_0817658a
	ldr r2, [r1]
	cmp r2, r4
	ble .L_0817658a
	ldr r3, .L_081765f4
	cmp r2, r3
	bgt .L_0817658a
	ldr r3, [r1, #12]
	adds r3, r2, r3
	str r3, [r1]
	ldr r3, [r1, #16]
	adds r3, r0, r3
	str r3, [r1, #4]
.L_0817658a:
	mov r2, r9
	cmp r2, #0
	bge .L_08176592
	adds r2, #15
.L_08176592:
	ldr r3, [r1, #24]
	asrs r2, r2, #4
	adds r2, r3, r2
	adds r3, r2, #2
	str r3, [r1, #24]
	cmp r3, #255
	ble .L_081765a6
	adds r3, r2, #0
	subs r3, #254
	str r3, [r1, #24]
.L_081765a6:
	movs r5, #1
	add r9, r5
	mov r6, r9
	adds r1, #28
	cmp r6, #128
	bne .L_0817656c
.L_081765b2:
	ldr r7, [sp, #80]
	subs r7, #56
	str r7, [sp, #52]
	cmp r7, #75
	bhi .L_08176614
	ldr r3, .L_081765f8
	lsls r0, r7, #7
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #160]
	str r4, [sp, #164]
	bl Trig_Sin
	movs r1, #128
	lsls r1, r1, #7
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r3, #0
	add r1, sp, #232
	add r2, sp, #160
	str r0, [r2, #4]
	str r0, [sp, #160]
	str r3, [r1, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r1, #4]
	movs r3, #228
	lsls r3, r3, #15
	str r3, [r1]
	cmp r7, #0
	blt .L_081765fc
	movs r3, #156
	b .L_081765fe
.L_081765f4:
	.4byte 0x00ffffff
.L_081765f8:
	.4byte Data_08196eb0
.L_081765fc:
	movs r3, #200
.L_081765fe:
	lsls r3, r3, #15
	str r3, [r1, #8]
	ldr r4, [sp, #92]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #220
	adds r3, r4, r5
	ldr r0, [r3]
	movs r3, #0
	bl Func_08020010
.L_08176614:
	ldr r6, [sp, #80]
	cmp r6, #131
	bne .L_08176642
	movs r0, #1
	ldr r1, .L_0817669c
	movs r2, #0
	bl Func_08118040
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, .L_081766a0
	adds r0, #192
	ldr r7, .L_081766a4
	mov lr, r7
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #96]
	lsls r1, r1, #6
	ldr r2, .L_081766a8
	mov lr, r7
	.2byte 0xf800
.L_08176642:
	mov r0, r11
	cmp r0, #14
	bhi .L_08176664
	ldr r1, [sp, #80]
	ldr r0, [sp, #96]
	lsls r3, r1, #8
	adds r3, r3, r1
	lsls r2, r3, #16
	adds r3, r3, r2
	ldr r2, .L_081766ac
	lsls r3, r3, #2
	movs r1, #240
	subs r2, r2, r3
	lsls r1, r1, #6
	ldr r3, .L_081766a4
	mov lr, r3
	.2byte 0xf800
.L_08176664:
	ldr r4, [sp, #80]
	movs r3, #3
	ands r3, r4
	cmp r3, #0
	beq .L_08176670
	b .L_08176794
.L_08176670:
	mov r5, r11
	cmp r5, #107
	bls .L_08176678
	b .L_08176816
.L_08176678:
	ldr r0, .L_0817669c
	bl Resource_GetTableEntry
	movs r6, #160
	mov lr, r0
	ldr r0, .L_08176698
	lsls r6, r6, #19
	adds r6, #192
	movs r7, #0
	movs r1, #31
	mov r8, r6
	mov r9, r7
	mov r12, r0
	mov r10, r1
	b .L_081766b0
	.2byte 0x0000
.L_08176698:
	.4byte 0x0000001f
.L_0817669c:
	.4byte 0x00000075
.L_081766a0:
	.4byte 0x7fff7fff
.L_081766a4:
	.4byte IwramFillWords
.L_081766a8:
	.4byte 0x3f3f3f3f
.L_081766ac:
	.4byte 0x5151514f
.L_081766b0:
	mov r3, r8
	ldrh r2, [r3]
	ldr r5, [sp, #80]
	lsls r3, r2, #16
	mov r4, r12
	lsrs r0, r3, #26
	lsrs r6, r3, #21
	mov r7, r10
	ands r0, r4
	ands r6, r4
	ands r7, r2
	cmp r5, #167
	bgt .L_081766e0
	mov r1, lr
	ldrh r2, [r1]
	lsls r3, r2, #16
	lsrs r5, r3, #26
	ands r5, r4
	lsrs r4, r3, #21
	mov r3, r12
	ands r4, r3
	mov r3, r10
	ands r3, r2
	b .L_08176700
.L_081766e0:
	mov r4, lr
	ldrh r1, [r4]
	mov r5, r12
	lsls r2, r1, #16
	lsrs r3, r2, #26
	ands r3, r5
	adds r5, r3, #0
	lsrs r2, r2, #21
	mov r3, r12
	ands r2, r3
	mov r3, r10
	adds r4, r2, #0
	ands r3, r1
	subs r5, #16
	subs r4, #16
	adds r3, #8
.L_08176700:
	cmp r5, #0
	bge .L_08176706
	movs r5, #0
.L_08176706:
	cmp r4, #0
	bge .L_0817670c
	movs r4, #0
.L_0817670c:
	cmp r3, #31
	ble .L_08176712
	movs r3, #31
.L_08176712:
	cmp r0, r5
	ble .L_08176718
	subs r0, #1
.L_08176718:
	cmp r6, r4
	ble .L_0817671e
	subs r6, #1
.L_0817671e:
	cmp r7, r3
	ble .L_08176724
	subs r7, #1
.L_08176724:
	lsls r2, r6, #5
	lsls r3, r0, #10
	movs r5, #1
	orrs r3, r2
	add r9, r5
	orrs r3, r7
	mov r4, r8
	movs r6, #2
	mov r7, r9
	strh r3, [r4]
	add lr, r6
	add r8, r6
	cmp r7, #128
	bne .L_081766b0
	movs r0, #0
	mov r9, r0
	ldr r0, .L_081767b8
	movs r4, #31
.L_08176748:
	mov r3, r11
	cmp r3, #0
	bge .L_08176752
	ldr r3, [sp, #80]
	subs r3, #129
.L_08176752:
	asrs r3, r3, #2
	subs r1, r4, r3
	adds r2, r1, #0
	adds r3, r1, #0
	cmp r1, #31
	ble .L_08176760
	movs r1, #31
.L_08176760:
	cmp r1, #11
	bgt .L_08176766
	movs r1, #12
.L_08176766:
	cmp r2, #31
	ble .L_0817676c
	movs r2, #31
.L_0817676c:
	cmp r2, #3
	bgt .L_08176772
	movs r2, #4
.L_08176772:
	cmp r3, #31
	ble .L_08176778
	movs r3, #31
.L_08176778:
	cmp r3, #3
	bgt .L_0817677e
	movs r3, #4
.L_0817677e:
	lsls r2, r2, #5
	lsls r3, r3, #10
	orrs r3, r2
	orrs r3, r1
	movs r1, #1
	add r9, r1
	mov r2, r9
	strh r3, [r0]
	adds r0, #2
	cmp r2, #15
	bne .L_08176748
.L_08176794:
	mov r3, r11
	cmp r3, #107
	bhi .L_08176816
	ldr r0, .L_081767bc
	bl Resource_GetTableEntry
	ldr r6, .L_081767b4
	ldr r4, .L_081767c0
	movs r5, #0
	movs r7, #31
	mov lr, r4
	mov r9, r5
	mov r12, r6
	mov r8, r7
	b .L_081767c4
	.2byte 0x0000
.L_081767b4:
	.4byte 0x0000001f
.L_081767b8:
	.4byte 0x050003c2
.L_081767bc:
	.4byte 0x0000009b
.L_081767c0:
	.4byte 0x05000200
.L_081767c4:
	mov r1, lr
	ldrh r2, [r1]
	ldrh r1, [r0]
	lsls r3, r2, #16
	lsrs r7, r3, #26
	lsrs r5, r3, #21
	mov r6, r8
	lsls r3, r1, #16
	mov r4, r12
	ands r6, r2
	lsrs r2, r3, #26
	lsrs r3, r3, #21
	ands r7, r4
	ands r5, r4
	ands r2, r4
	ands r3, r4
	mov r4, r8
	ands r4, r1
	cmp r7, r2
	ble .L_081767ee
	subs r7, #1
.L_081767ee:
	cmp r5, r3
	ble .L_081767f4
	subs r5, #1
.L_081767f4:
	cmp r6, r4
	ble .L_081767fa
	subs r6, #1
.L_081767fa:
	lsls r3, r7, #10
	lsls r2, r5, #5
	orrs r3, r2
	orrs r3, r6
	movs r6, #1
	add r9, r6
	mov r5, lr
	movs r7, #2
	mov r1, r9
	strh r3, [r5]
	adds r0, #2
	add lr, r7
	cmp r1, #192
	bne .L_081767c4
.L_08176816:
	ldr r2, [sp, #80]
	movs r1, #0
	subs r2, #214
	cmp r2, #31
	bhi .L_0817683c
	adds r3, r2, #0
	cmp r3, #0
	bge .L_0817682a
	ldr r3, [sp, #80]
	subs r3, #211
.L_0817682a:
	asrs r2, r3, #2
	lsrs r3, r3, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	str r3, [sp, #48]
	movs r3, #0
	str r3, [sp, #44]
	str r2, [sp, #40]
	movs r1, #1
.L_0817683c:
	ldr r2, [sp, #80]
	subs r2, #251
	cmp r2, #31
	bhi .L_0817686c
	lsrs r3, r2, #31
	adds r0, r2, r3
	asrs r1, r0, #1
	adds r2, r1, #0
	cmp r1, #0
	bge .L_08176852
	adds r2, r1, #3
.L_08176852:
	asrs r2, r2, #2
	movs r3, #4
	subs r3, r3, r2
	str r3, [sp, #48]
	lsrs r3, r0, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	movs r2, #8
	negs r4, r3
	subs r2, r2, r3
	str r4, [sp, #44]
	str r2, [sp, #40]
	movs r1, #1
.L_0817686c:
	ldr r5, [sp, #80]
	ldr r6, .L_081768dc
	adds r3, r5, r6
	cmp r3, #63
	bhi .L_0817689c
	cmp r3, #0
	bge .L_0817687e
	ldr r7, .L_081768e0
	adds r3, r5, r7
.L_0817687e:
	asrs r0, r3, #2
	lsrs r3, r3, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r3, #8
	str r3, [sp, #44]
	str r0, [sp, #48]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_08176894
	adds r3, #7
.L_08176894:
	asrs r3, r3, #3
	subs r3, #4
	str r3, [sp, #40]
	movs r1, #1
.L_0817689c:
	cmp r1, #1
	bne .L_0817691c
	ldr r0, .L_081768e4
	bl Resource_GetTableEntry
	ldr r5, .L_081768d8
	ldr r4, .L_081768e8
	movs r1, #0
	mov r9, r1
.L_081768ae:
	ldrh r3, [r0]
	ldr r6, [sp, #48]
	movs r2, #31
	ands r2, r3
	adds r1, r2, r6
	lsls r3, r3, #16
	ldr r7, [sp, #44]
	ldr r6, [sp, #40]
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r5
	ands r3, r5
	adds r2, r2, r7
	adds r3, r3, r6
	cmp r1, #31
	ble .L_081768d0
	movs r1, #31
.L_081768d0:
	cmp r2, #31
	ble .L_081768ec
	movs r2, #31
	b .L_081768ec
.L_081768d8:
	.4byte 0x0000001f
.L_081768dc:
	.4byte 0xfffffea4
.L_081768e0:
	.4byte 0xfffffea7
.L_081768e4:
	.4byte 0x0000009b
.L_081768e8:
	.4byte 0x05000200
.L_081768ec:
	cmp r3, #31
	ble .L_081768f2
	movs r3, #31
.L_081768f2:
	cmp r1, #0
	bge .L_081768f8
	movs r1, #0
.L_081768f8:
	cmp r2, #0
	bge .L_081768fe
	movs r2, #0
.L_081768fe:
	cmp r3, #0
	bge .L_08176904
	movs r3, #0
.L_08176904:
	lsls r3, r3, #10
	lsls r2, r2, #5
	movs r7, #1
	orrs r3, r2
	add r9, r7
	orrs r3, r1
	mov r1, r9
	strh r3, [r4]
	adds r0, #2
	adds r4, #2
	cmp r1, #192
	bne .L_081768ae
.L_0817691c:
	movs r2, #236
	lsls r2, r2, #1
	adds r2, #255
	cmp r11, r2
	bls .L_0817692a
	bl .L_0817749a
.L_0817692a:
	ldr r3, [sp, #80]
	cmp r3, #132
	beq .L_08176932
	b .L_08176ac0
.L_08176932:
	ldr r4, [sp, #92]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #220
	adds r5, r4, r6
	ldr r0, [r5]
	bl ResourceObject_ReleaseFar
	ldr r0, .L_08176b8c
	bl Resource_GetTableEntry
	movs r1, #192
	ldr r2, .L_08176b90
	adds r7, r0, #0
	lsls r1, r1, #1
	ldr r3, .L_08176b94
	ldr r0, .L_08176b98
	mov lr, r3
	.2byte 0xf800
	movs r4, #192
	lsls r4, r4, #1
	adds r7, r7, r4
	ldr r6, [sp, #92]
	adds r0, r7, #0
	movs r7, #172
	lsls r7, r7, #6
	adds r1, r6, r7
	bl Func_0801587c
	ldr r1, .L_08176b9c
	ldr r6, [sp, #92]
	movs r2, #172
	ldr r7, .L_08176ba0
	movs r0, #0
	lsls r2, r2, #6
	mov r9, r0
	mov r10, r1
	adds r6, r6, r2
.L_0817697e:
	movs r1, #32
	ldr r2, .L_08176ba4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	movs r4, #12
	ldrb r3, [r0, #9]
	mov r8, r4
	mov r1, r8
	orrs r3, r1
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r10
	ldrh r0, [r3, #2]
	ldr r2, .L_08176ba8
	adds r1, r6, #0
	adds r0, r0, r2
	movs r2, #128
	lsls r2, r2, #3
	mov lr, r7
	.2byte 0xf800
	movs r4, #1
	movs r3, #128
	add r9, r4
	lsls r3, r3, #3
	mov r0, r9
	adds r6, r6, r3
	cmp r0, #12
	bne .L_0817697e
	ldr r0, .L_08176bac
	bl Resource_GetTableEntry
	adds r7, r0, #0
	ldr r3, .L_08176ba0
	adds r1, r7, #0
	movs r2, #32
	ldr r0, .L_08176bb0
	mov lr, r3
	.2byte 0xf800
	ldr r4, .L_08176b94
	ldr r2, .L_08176b90
	movs r1, #32
	ldr r0, .L_08176bb0
	mov lr, r4
	.2byte 0xf800
	ldr r6, [sp, #92]
	movs r0, #172
	lsls r0, r0, #6
	adds r5, r6, r0
	adds r7, #32
	adds r1, r5, #0
	adds r0, r7, #0
	bl Func_0801587c
	movs r2, #192
	movs r3, #224
	movs r1, #64
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #64
	bl Func_0815b290
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #12
	adds r3, r6, r1
	str r0, [r3]
	ldrb r3, [r0, #9]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	ldr r6, .L_08176b9c
	lsls r3, r3, #2
	adds r3, r3, r6
	ldrh r0, [r3, #2]
	ldr r3, .L_08176ba8
	movs r2, #128
	adds r1, r5, #0
	ldr r4, .L_08176ba0
	adds r0, r0, r3
	lsls r2, r2, #4
	mov lr, r4
	.2byte 0xf800
	movs r1, #64
	ldr r2, .L_08176bb4
	movs r3, #0
	movs r0, #64
	bl Func_0815b290
	movs r1, #240
	ldr r7, [sp, #92]
	lsls r1, r1, #7
	ldrb r2, [r0, #9]
	adds r1, #16
	movs r4, #13
	negs r4, r4
	adds r3, r7, r1
	str r0, [r3]
	adds r3, r4, #0
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	movs r3, #32
	strb r3, [r0, #23]
	ldrb r3, [r0, #16]
	strb r2, [r0, #9]
	lsls r3, r3, #2
	adds r3, r3, r6
	ldrh r0, [r3, #2]
	ldr r6, .L_08176ba8
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	adds r0, r0, r6
	ldr r7, .L_08176b94
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_08176bb8
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	adds r1, r7, #0
	movs r2, #128
	ldr r3, .L_08176ba0
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	adds r1, r5, #0
	movs r3, #0
	ldr r0, .L_08176bbc
	movs r2, #0
	bl Func_08157cf4
	movs r2, #192
	lsls r2, r2, #2
	adds r1, r5, #0
	ldr r0, [sp, #72]
	adds r2, #2
	ldr r4, .L_08176ba0
	mov lr, r4
	.2byte 0xf800
	ldr r5, [sp, #92]
	movs r6, #184
	lsls r6, r6, #5
	ldr r0, .L_08176bc0
	adds r1, r5, r6
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r7, #144
	movs r0, #224
	lsls r7, r7, #15
	lsls r0, r0, #14
	str r7, [sp, #60]
	str r0, [sp, #56]
.L_08176ac0:
	ldr r1, [sp, #80]
	cmp r1, #243
	bne .L_08176b46
	ldr r0, .L_08176bc4
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r1, #64
	movs r2, #0
	ldr r3, .L_08176b94
	ldr r0, .L_08176bc8
	adds r7, #64
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_08176bcc
	adds r0, r7, #0
	bl Func_0801587c
	ldr r1, .L_08176bcc
	movs r4, #0
	movs r0, #128
	mov r9, r4
	lsls r0, r0, #5
.L_08176aee:
	ldrb r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08176afa
	adds r3, #192
	strb r3, [r1]
.L_08176afa:
	movs r5, #1
	add r9, r5
	adds r1, #1
	cmp r9, r0
	bne .L_08176aee
	ldr r3, .L_08176bd0
	movs r6, #0
	mov r9, r6
.L_08176b0a:
	movs r0, #1
	mov r7, r9
	add r9, r0
	mov r1, r9
	strb r7, [r3]
	adds r3, #1
	cmp r1, #32
	bne .L_08176b0a
	ldr r5, .L_08176bd0
	movs r2, #0
	movs r6, #128
	mov r9, r2
	movs r7, #31
	lsls r6, r6, #5
.L_08176b26:
	bl Random16
	ldr r3, .L_08176bcc
	ands r0, r7
	adds r0, r0, r3
	adds r0, r0, r6
	ldrb r2, [r5]
	ldrb r3, [r0]
	movs r4, #1
	add r9, r4
	strb r3, [r5]
	strb r2, [r0]
	mov r0, r9
	adds r5, #1
	cmp r0, #32
	bne .L_08176b26
.L_08176b46:
	add r5, sp, #216
	movs r3, #0
	str r3, [r5, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r5, #4]
	ldr r1, [sp, #80]
	cmp r1, #159
	bgt .L_08176b6c
	ldr r2, [sp, #60]
	ldr r4, [sp, #56]
	ldr r6, .L_08176bd4
	movs r3, #128
	lsls r3, r3, #6
	adds r2, r2, r3
	adds r4, r4, r6
	str r2, [sp, #60]
	str r4, [sp, #56]
	b .L_08176be2
.L_08176b6c:
	ldr r7, [sp, #80]
	movs r0, #76
	adds r0, #255
	cmp r7, r0
	bgt .L_08176bd8
	ldr r1, [sp, #60]
	ldr r3, [sp, #56]
	movs r2, #128
	movs r4, #128
	lsls r2, r2, #6
	lsls r4, r4, #5
	adds r1, r1, r2
	adds r3, r3, r4
	str r1, [sp, #60]
	str r3, [sp, #56]
	b .L_08176be2
.L_08176b8c:
	.4byte 0x0000009b
.L_08176b90:
	.4byte 0x7fff7fff
.L_08176b94:
	.4byte IwramFillWords
.L_08176b98:
	.4byte 0x05000200
.L_08176b9c:
	.4byte ResourceTableEntries
.L_08176ba0:
	.4byte IwramCopyWords
.L_08176ba4:
	.4byte 0x80002000
.L_08176ba8:
	.4byte 0x06010000
.L_08176bac:
	.4byte 0x0000009f
.L_08176bb0:
	.4byte 0x050003c0
.L_08176bb4:
	.4byte 0xc0002000
.L_08176bb8:
	.4byte 0x000000b8
.L_08176bbc:
	.4byte 0x00000137
.L_08176bc0:
	.4byte 0x000000be
.L_08176bc4:
	.4byte 0x0000009d
.L_08176bc8:
	.4byte 0x05000380
.L_08176bcc:
	.4byte Data_02014000
.L_08176bd0:
	.4byte Data_02015000
.L_08176bd4:
	.4byte 0xffffc000
.L_08176bd8:
	ldr r6, [sp, #60]
	movs r7, #128
	lsls r7, r7, #6
	adds r6, r6, r7
	str r6, [sp, #60]
.L_08176be2:
	movs r3, #160
	add r7, sp, #152
	lsls r3, r3, #9
	str r3, [r7, #4]
	ldr r0, [sp, #56]
	movs r1, #48
	str r3, [sp, #152]
	bl Math_Div
	movs r1, #128
	lsls r1, r1, #7
	adds r0, r0, r1
	str r0, [r7, #4]
	ldr r2, [sp, #60]
	movs r4, #128
	lsls r4, r4, #15
	adds r3, r2, r4
	str r0, [sp, #152]
	str r3, [r5]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r5, #8]
	ldr r6, [sp, #92]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #12
	adds r3, r6, r0
	ldr r0, [r3]
	adds r1, r5, #0
	adds r2, r7, #0
	movs r3, #0
	bl Func_08020010
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #4]
	ldr r2, [sp, #92]
	str r3, [sp, #152]
	movs r3, #238
	lsls r3, r3, #7
	movs r1, #0
	adds r3, #220
	mov r9, r1
	adds r6, r2, r3
.L_08176c3a:
	mov r3, r9
	cmp r3, #0
	bge .L_08176c42
	adds r3, #3
.L_08176c42:
	asrs r3, r3, #2
	ldr r0, [sp, #60]
	mov r4, r9
	lsls r2, r3, #2
	subs r2, r4, r2
	lsls r2, r2, #21
	adds r2, r2, r0
	str r2, [r5]
	ldr r1, [sp, #56]
	lsls r3, r3, #21
	adds r3, r3, r1
	str r3, [r5, #8]
	adds r2, r7, #0
	movs r3, #0
	ldmia r6!, {r0}
	adds r1, r5, #0
	bl Func_08020010
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #12
	bne .L_08176c3a
	ldr r4, [sp, #80]
	cmp r4, #200
	bne .L_08176c8c
	ldr r0, .L_08176f0c
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	lsls r0, r0, #19
	adds r1, r7, #0
	movs r2, #128
	ldr r5, .L_08176f10
	mov lr, r5
	.2byte 0xf800
.L_08176c8c:
	ldr r5, [sp, #80]
	subs r5, #200
	cmp r5, #101
	bls .L_08176c96
	b .L_08176e16
.L_08176c96:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	adds r7, r5, #0
	lsls r3, r7, #2
	adds r4, r3, #0
	lsls r3, r7, #1
	adds r3, r3, r7
	adds r6, r0, #0
	lsls r2, r3, #7
	movs r0, #195
	ldr r1, [sp, #80]
	subs r2, r2, r3
	lsls r0, r0, #9
	adds r0, #160
	lsls r2, r2, #2
	subs r4, #64
	mov r11, r0
	subs r5, r0, r2
	cmp r1, #253
	ble .L_08176cce
	movs r3, #254
	subs r3, r3, r1
	lsls r4, r3, #3
.L_08176cce:
	movs r2, #212
	lsls r2, r2, #6
	adds r2, #231
	cmp r5, r2
	bgt .L_08176cde
	movs r5, #212
	lsls r5, r5, #6
	adds r5, #232
.L_08176cde:
	cmp r4, #0
	ble .L_08176ce4
	movs r4, #0
.L_08176ce4:
	str r4, [r6, #20]
	ldr r3, [sp, #92]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #92]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #75
	str r3, [r2]
	ldr r3, [sp, #144]
	ldr r2, .L_08176f14
	movs r4, #184
	ands r3, r2
	movs r2, #7
	mov r9, r2
	orrs r3, r2
	ldr r2, .L_08176f18
	lsls r4, r4, #5
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #144]
	adds r3, r0, r4
	add r0, sp, #144
	str r3, [r0, #4]
	ldr r3, .L_08176f1c
	mov r1, r9
	mov r2, r10
	str r1, [r6]
	str r0, [r6, #16]
	str r3, [r6, #8]
	str r2, [r6, #12]
	ldr r3, [sp, #80]
	movs r4, #141
	lsls r4, r4, #1
	mov r8, r0
	cmp r3, r4
	bgt .L_08176d80
	bl Func_08014de4
	lsls r2, r5, #1
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_080151e4
	ldr r1, .L_08176f20
	movs r2, #0
	ldr r0, .L_08176f24
	bl Func_08015160
	movs r0, #224
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	movs r0, #128
	lsls r3, r7, #8
	lsls r0, r0, #7
	subs r0, r0, r3
	bl Func_080150e4
	negs r0, r7
	lsls r0, r0, #10
	bl Func_08015068
	ldr r0, .L_08176f28
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_08176d80:
	ldr r5, [sp, #80]
	cmp r5, #231
	ble .L_08176e0a
	adds r7, r5, #0
	subs r7, #232
	lsls r2, r7, #2
	lsls r3, r7, #1
	adds r2, r2, r7
	adds r4, r3, #0
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r1, #64
	lsls r3, r3, #4
	mov r0, r11
	subs r4, #64
	negs r1, r1
	subs r5, r0, r3
	cmp r4, r1
	bge .L_08176daa
	movs r4, #64
	negs r4, r4
.L_08176daa:
	cmp r4, #0
	ble .L_08176db0
	movs r4, #0
.L_08176db0:
	mov r2, r8
	movs r3, #6
	strb r3, [r2]
	strb r3, [r2, #1]
	str r4, [r6, #20]
	ldr r4, [sp, #92]
	movs r0, #224
	lsls r0, r0, #3
	adds r3, r4, r0
	str r3, [r2, #4]
	ldr r3, .L_08176f2c
	mov r1, r9
	str r2, [r6, #16]
	mov r2, r10
	str r3, [r6, #8]
	str r1, [r6]
	str r2, [r6, #12]
	bl Func_08014de4
	ldr r1, .L_08176f20
	movs r2, #0
	adds r0, r1, #0
	bl Func_08015160
	adds r1, r5, #0
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #192
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	negs r0, r7
	lsls r0, r0, #10
	bl Func_08015068
	ldr r0, .L_08176f28
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_08176e0a:
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
.L_08176e16:
	ldr r3, [sp, #80]
	subs r3, #244
	cmp r3, #65
	bls .L_08176e20
	b .L_08176f9a
.L_08176e20:
	ldr r3, .L_08176f30
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #136]
	str r4, [sp, #140]
	ldr r3, [sp, #80]
	movs r4, #4
	adds r4, #255
	cmp r3, r4
	bgt .L_08176eb4
	ldr r6, [sp, #16]
	ldr r0, [sp, #92]
	movs r1, #240
	movs r7, #128
	lsls r1, r1, #7
	lsls r7, r7, #5
	adds r1, #16
	movs r5, #0
	mov r12, r6
	adds r3, r0, r1
	adds r6, r6, r7
	mov r9, r5
	mov lr, r6
	ldr r4, [r3]
	ldr r6, .L_08176f34
	ldr r5, .L_08176f38
.L_08176e54:
	mov r2, lr
	ldrb r0, [r2]
	mov r3, r9
	ldr r7, .L_08176f3c
	lsls r1, r3, #6
	lsls r0, r0, #1
	adds r3, r1, r0
	adds r2, r3, r7
	adds r7, #1
	adds r3, r3, r7
	ldrb r3, [r3]
	ldrb r2, [r2]
	lsls r3, r3, #8
	orrs r2, r3
	ldrb r3, [r4, #16]
	subs r7, #1
	lsls r3, r3, #2
	adds r3, r3, r6
	ldrh r3, [r3, #2]
	adds r3, r1, r3
	adds r3, r3, r0
	strh r2, [r3, r5]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #1
	add r3, r12
	ldrb r0, [r3]
	lsls r0, r0, #1
	adds r3, r1, r0
	adds r2, r3, r7
	adds r7, #1
	adds r3, r3, r7
	ldrb r3, [r3]
	ldrb r2, [r2]
	lsls r3, r3, #8
	orrs r2, r3
	ldrb r3, [r4, #16]
	lsls r3, r3, #2
	adds r3, r3, r6
	ldrh r3, [r3, #2]
	adds r1, r1, r3
	adds r1, r1, r0
	movs r0, #1
	add r9, r0
	strh r2, [r1, r5]
	mov r1, r9
	cmp r1, #64
	bne .L_08176e54
.L_08176eb4:
	ldr r3, [sp, #80]
	add r1, sp, #200
	lsls r2, r3, #11
	movs r3, #155
	lsls r3, r3, #12
	subs r3, r3, r2
	add r2, sp, #136
	str r3, [sp, #136]
	str r3, [r2, #4]
	movs r3, #0
	str r3, [r1, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r1, #4]
	movs r3, #184
	lsls r3, r3, #15
	str r3, [r1]
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r1, #8]
	ldr r4, [sp, #92]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #16
	adds r3, r4, r5
	ldr r0, [r3]
	movs r3, #0
	bl Func_08020010
	ldr r0, .L_08176f40
	bl Resource_GetTableEntry
	ldr r6, .L_08176f44
	ldr r1, .L_08176f08
	movs r7, #0
	movs r2, #31
	mov lr, r6
	mov r9, r7
	mov r12, r1
	mov r8, r2
	b .L_08176f48
	.2byte 0x0000
.L_08176f08:
	.4byte 0x0000001f
.L_08176f0c:
	.4byte 0x000000bd
.L_08176f10:
	.4byte IwramCopyWords
.L_08176f14:
	.4byte 0xffffff00
.L_08176f18:
	.4byte 0xffff00ff
.L_08176f1c:
	.4byte Data_08199364
.L_08176f20:
	.4byte 0xfff80000
.L_08176f24:
	.4byte 0xfff00000
.L_08176f28:
	.4byte Data_08199210
.L_08176f2c:
	.4byte Data_08199340
.L_08176f30:
	.4byte Data_08196eb8
.L_08176f34:
	.4byte ResourceTableEntries
.L_08176f38:
	.4byte 0x06010000
.L_08176f3c:
	.4byte Data_02014000
.L_08176f40:
	.4byte 0x0000009d
.L_08176f44:
	.4byte 0x05000380
.L_08176f48:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r1, [r0]
	lsls r3, r2, #16
	lsrs r7, r3, #26
	lsrs r5, r3, #21
	mov r6, r8
	lsls r3, r1, #16
	mov r4, r12
	ands r6, r2
	lsrs r2, r3, #26
	lsrs r3, r3, #21
	ands r7, r4
	ands r5, r4
	ands r2, r4
	ands r3, r4
	mov r4, r8
	ands r4, r1
	cmp r7, r2
	bge .L_08176f72
	adds r7, #1
.L_08176f72:
	cmp r5, r3
	bge .L_08176f78
	adds r5, #1
.L_08176f78:
	cmp r6, r4
	bge .L_08176f7e
	adds r6, #1
.L_08176f7e:
	lsls r3, r7, #10
	lsls r2, r5, #5
	orrs r3, r2
	orrs r3, r6
	movs r6, #1
	add r9, r6
	mov r5, lr
	movs r7, #2
	mov r1, r9
	strh r3, [r5]
	adds r0, #2
	add lr, r7
	cmp r1, #32
	bne .L_08176f48
.L_08176f9a:
	ldr r2, [sp, #80]
	movs r3, #46
	adds r3, #255
	cmp r2, r3
	bgt .L_08176fa6
	b .L_081771ca
.L_08176fa6:
	movs r4, #151
	lsls r4, r4, #1
	cmp r2, r4
	bne .L_08177008
	movs r1, #240
	ldr r0, [sp, #96]
	lsls r1, r1, #6
	ldr r2, .L_08177034
	ldr r5, .L_08177038
	mov lr, r5
	.2byte 0xf800
	movs r1, #64
	ldr r2, .L_0817703c
	ldr r0, .L_08177040
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_08177044
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	lsls r0, r0, #19
	movs r2, #128
	lsls r2, r2, #1
	ldr r6, .L_08177048
	adds r0, #192
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_0817704c
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	adds r1, r7, #0
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r6
	.2byte 0xf800
	ldr r7, [sp, #92]
	movs r2, #184
	lsls r2, r2, #5
	adds r1, r7, r2
	ldr r0, .L_08177050
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	bl Func_0815b410
.L_08177008:
	ldr r3, [sp, #80]
	movs r4, #48
	adds r4, #255
	cmp r3, r4
	ble .L_081770c6
	movs r5, #160
	ldr r7, .L_08177030
	ldr r0, .L_08177044
	lsls r5, r5, #19
	bl Resource_GetTableEntry
	adds r5, #192
	movs r6, #0
	movs r1, #31
	mov lr, r5
	mov r9, r6
	mov r12, r7
	mov r8, r1
	b .L_08177054
	.2byte 0x0000
.L_08177030:
	.4byte 0x0000001f
.L_08177034:
	.4byte 0x3f3f3f3f
.L_08177038:
	.4byte IwramFillWords
.L_0817703c:
	.4byte 0x7fff7fff
.L_08177040:
	.4byte 0x05000380
.L_08177044:
	.4byte 0x00000075
.L_08177048:
	.4byte IwramCopyWords
.L_0817704c:
	.4byte 0x000000bd
.L_08177050:
	.4byte 0x000000c2
.L_08177054:
	mov r3, lr
	ldrh r2, [r3]
	mov r4, r12
	lsls r3, r2, #16
	lsrs r6, r3, #26
	ldrh r1, [r0]
	mov r5, r12
	ands r6, r4
	lsrs r4, r3, #21
	ands r4, r5
	mov r5, r8
	ands r5, r2
	lsls r2, r1, #16
	lsrs r3, r2, #26
	mov r7, r12
	ands r3, r7
	adds r7, r3, #0
	lsrs r2, r2, #21
	mov r3, r12
	ands r2, r3
	mov r3, r8
	ands r3, r1
	subs r7, #20
	subs r2, #20
	adds r3, #8
	cmp r7, #0
	bge .L_0817708c
	movs r7, #0
.L_0817708c:
	cmp r2, #0
	bge .L_08177092
	movs r2, #0
.L_08177092:
	cmp r3, #31
	ble .L_08177098
	movs r3, #31
.L_08177098:
	cmp r6, r7
	ble .L_0817709e
	subs r6, #1
.L_0817709e:
	cmp r4, r2
	ble .L_081770a4
	subs r4, #1
.L_081770a4:
	cmp r5, r3
	ble .L_081770aa
	subs r5, #1
.L_081770aa:
	lsls r3, r6, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r5
	movs r5, #1
	add r9, r5
	mov r4, lr
	movs r6, #2
	mov r7, r9
	strh r3, [r4]
	adds r0, #2
	add lr, r6
	cmp r7, #128
	bne .L_08177054
.L_081770c6:
	ldr r0, [sp, #80]
	movs r1, #46
	adds r1, #255
	cmp r0, r1
	ble .L_081771ca
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08177314
	ldr r3, [sp, #128]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08177318
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, [sp, #92]
	movs r4, #184
	lsls r4, r4, #5
	str r3, [sp, #128]
	adds r3, r2, r4
	add r2, sp, #128
	str r3, [r2, #4]
	ldr r3, .L_0817731c
	adds r6, r0, #0
	mov r5, r11
	str r1, [r6]
	str r2, [r6, #16]
	str r3, [r6, #8]
	str r5, [r6, #12]
	ldr r0, [sp, #80]
	ldr r1, .L_08177320
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r2, r3, #7
	adds r3, r3, r2
	ldr r2, .L_08177324
	lsls r3, r3, #4
	adds r1, r1, r3
	lsls r3, r0, #2
	adds r2, r2, r3
	ldr r3, .L_08177328
	movs r7, #0
	mov r9, r7
	mov r10, r1
	mov r8, r2
	adds r7, r0, r3
.L_08177130:
	cmp r7, #0
	blt .L_081771a8
	ldr r3, .L_0817732c
	mov r4, r10
	mov r0, r8
	subs r5, r3, r4
	subs r0, #16
	cmp r5, #0
	bge .L_08177144
	movs r5, #0
.L_08177144:
	cmp r0, #0
	ble .L_0817714a
	movs r0, #0
.L_0817714a:
	str r0, [r6, #20]
	bl Func_08014de4
	ldr r1, .L_08177330
	movs r2, #0
	adds r0, r1, #0
	bl Func_08015160
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	adds r1, r5, #0
	adds r0, r2, #0
	mov r5, r9
	bl Func_080151e4
	cmp r5, #0
	bne .L_08177178
	movs r0, #240
	lsls r0, r0, #8
	bl Func_080150e4
	b .L_08177190
.L_08177178:
	mov r0, r9
	cmp r0, #1
	bne .L_08177188
	movs r0, #128
	lsls r0, r0, #6
	bl Func_080150e4
	b .L_08177190
.L_08177188:
	movs r0, #224
	lsls r0, r0, #8
	bl Func_080150e4
.L_08177190:
	movs r0, #128
	lsls r0, r0, #4
	bl SceneTransform_ApplyPitch
	ldr r0, .L_08177334
	mov r1, r11
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_081771a8:
	ldr r1, .L_08177338
	movs r3, #1
	movs r2, #32
	add r9, r3
	negs r2, r2
	mov r4, r9
	add r10, r1
	add r8, r2
	subs r7, #8
	cmp r4, #3
	bne .L_08177130
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
.L_081771ca:
	ldr r5, [sp, #80]
	movs r6, #165
	lsls r6, r6, #1
	cmp r5, r6
	bne .L_081771e0
	ldr r0, .L_0817733c
	movs r1, #64
	ldr r2, .L_08177340
	ldr r7, .L_08177344
	mov lr, r7
	.2byte 0xf800
.L_081771e0:
	ldr r0, [sp, #80]
	ldr r1, .L_08177348
	adds r3, r0, r1
	cmp r3, #20
	bhi .L_08177254
	ldr r3, .L_0817734c
	ldr r2, [sp, #80]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r0, #0
	str r3, [sp, #120]
	str r4, [sp, #124]
	movs r3, #173
	lsls r3, r3, #1
	movs r4, #0
	cmp r2, r3
	ble .L_08177214
	ldr r5, .L_08177350
	adds r3, #1
	subs r4, r3, r2
	adds r3, r2, r5
	cmp r3, #0
	bge .L_08177212
	ldr r6, .L_08177354
	adds r3, r2, r6
.L_08177212:
	asrs r0, r3, #2
.L_08177214:
	ldr r7, [sp, #80]
	ldr r1, .L_08177358
	lsls r3, r7, #11
	adds r3, r3, r1
	add r2, sp, #120
	str r3, [r2, #4]
	add r1, sp, #184
	str r3, [sp, #120]
	movs r3, #0
	str r3, [r1, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r1, #4]
	lsls r3, r4, #16
	movs r4, #184
	lsls r4, r4, #15
	adds r3, r3, r4
	movs r5, #144
	str r3, [r1]
	lsls r5, r5, #15
	lsls r3, r0, #16
	adds r3, r3, r5
	str r3, [r1, #8]
	ldr r6, [sp, #92]
	movs r7, #240
	lsls r7, r7, #7
	adds r7, #16
	adds r3, r6, r7
	ldr r0, [r3]
	movs r3, #0
	bl Func_08020010
.L_08177254:
	ldr r0, [sp, #80]
	movs r1, #92
	adds r1, #255
	cmp r0, r1
	bne .L_081772ae
	ldr r3, [sp, #92]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r5, [sp, #92]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #132
	movs r2, #0
	adds r3, r5, r6
	str r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	movs r1, #160
	strh r2, [r3]
	lsls r1, r1, #19
	adds r1, #2
	mov r9, r2
	movs r0, #31
.L_0817728a:
	mov r7, r9
	subs r2, r0, r7
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08177296
	movs r2, #0
.L_08177296:
	cmp r3, #0
	bge .L_0817729c
	movs r3, #0
.L_0817729c:
	lsls r3, r3, #10
	orrs r3, r2
	movs r2, #1
	add r9, r2
	strh r3, [r1]
	mov r3, r9
	adds r1, #2
	cmp r3, #63
	bne .L_0817728a
.L_081772ae:
	ldr r4, [sp, #80]
	movs r5, #173
	lsls r5, r5, #1
	cmp r4, r5
	ble .L_08177392
	ldr r6, .L_0817735c
	movs r7, #94
	lsls r3, r4, #2
	adds r7, #255
	adds r5, r3, r6
	movs r6, #54
	cmp r4, r7
	ble .L_081772de
	ldr r1, .L_08177360
	lsls r0, r4, #10
	adds r0, r0, r1
	bl Trig_Sin
	adds r3, r0, #0
	muls r3, r6
	negs r3, r3
	asrs r3, r3, #16
	adds r6, r3, #0
	adds r6, #54
.L_081772de:
	ldr r2, [sp, #80]
	movs r3, #180
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_081772f4
	ldr r4, [sp, #92]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r4, r7
	movs r3, #0
	str r3, [r2]
.L_081772f4:
	ldr r0, [sp, #80]
	movs r1, #116
	adds r1, #255
	cmp r0, r1
	bgt .L_08177364
	ldr r2, [sp, #92]
	movs r3, #184
	lsls r3, r3, #5
	adds r0, r2, r3
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #9
	bl Func_0815b434
	b .L_08177366
	.2byte 0x0000
.L_08177314:
	.4byte 0xffffff00
.L_08177318:
	.4byte 0xffff00ff
.L_0817731c:
	.4byte Data_08199364
.L_08177320:
	.4byte 0xffe37760
.L_08177324:
	.4byte 0xfffffb48
.L_08177328:
	.4byte 0xfffffed2
.L_0817732c:
	.4byte 0x000222e0
.L_08177330:
	.4byte 0xfff80000
.L_08177334:
	.4byte Data_08199210
.L_08177338:
	.4byte 0xffff3e80
.L_0817733c:
	.4byte 0x05000380
.L_08177340:
	.4byte 0x7c1f7c1f
.L_08177344:
	.4byte IwramFillWords
.L_08177348:
	.4byte 0xfffffeb6
.L_0817734c:
	.4byte Data_08196ec0
.L_08177350:
	.4byte 0xfffffea5
.L_08177354:
	.4byte 0xfffffea8
.L_08177358:
	.4byte 0xfff5b000
.L_0817735c:
	.4byte 0xfffffa9c
.L_08177360:
	.4byte 0xfffa8800
.L_08177364:
	movs r5, #104
.L_08177366:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r4, [sp, #92]
	movs r7, #184
	lsls r7, r7, #5
	adds r0, r4, r7
	adds r3, r5, #0
	adds r1, r6, #0
	movs r2, #58
	bl Func_0818caa8
	movs r0, #188
	movs r1, #3
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r0, [sp, #64]
	str r3, [r0, #4]
.L_08177392:
	ldr r2, [sp, #80]
	ldr r3, .L_081773e0
	movs r1, #0
	str r1, [sp, #84]
	adds r2, r2, r3
	mov r8, r2
.L_0817739e:
	mov r4, r8
	cmp r4, #0
	bne .L_081773b6
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	adds r0, #192
	lsls r1, r1, #1
	ldr r2, .L_081773e4
	ldr r5, .L_081773e8
	mov lr, r5
	.2byte 0xf800
.L_081773b6:
	mov r6, r8
	cmp r6, #159
	bhi .L_08177488
	ldr r0, .L_081773ec
	bl Resource_GetTableEntry
	movs r7, #160
	ldr r1, .L_081773dc
	lsls r7, r7, #19
	mov r12, r0
	adds r7, #192
	movs r0, #0
	movs r2, #31
	mov r10, r7
	mov r9, r0
	mov lr, r1
	mov r11, r2
	b .L_081773f0
	.2byte 0x0000
.L_081773dc:
	.4byte 0x0000001f
.L_081773e0:
	.4byte 0xfffffe9b
.L_081773e4:
	.4byte 0x7fff7fff
.L_081773e8:
	.4byte IwramFillWords
.L_081773ec:
	.4byte 0x00000075
.L_081773f0:
	mov r3, r10
	ldrh r2, [r3]
	mov r4, lr
	lsls r3, r2, #16
	lsrs r0, r3, #26
	lsrs r6, r3, #21
	mov r7, r11
	mov r5, r8
	ands r0, r4
	ands r6, r4
	ands r7, r2
	cmp r5, #7
	bgt .L_08177420
	mov r1, r12
	ldrh r2, [r1]
	mov r1, r11
	lsls r3, r2, #16
	lsrs r5, r3, #26
	ands r5, r4
	lsrs r4, r3, #21
	mov r3, lr
	ands r4, r3
	ands r1, r2
	b .L_08177442
.L_08177420:
	mov r4, r12
	ldrh r1, [r4]
	mov r5, lr
	lsls r2, r1, #16
	lsrs r3, r2, #26
	ands r3, r5
	adds r5, r3, #0
	lsrs r2, r2, #21
	mov r3, lr
	ands r2, r3
	mov r3, r11
	ands r3, r1
	adds r4, r2, #0
	adds r1, r3, #0
	subs r5, #16
	subs r4, #16
	adds r1, #8
.L_08177442:
	cmp r5, #0
	bge .L_08177448
	movs r5, #0
.L_08177448:
	cmp r4, #0
	bge .L_0817744e
	movs r4, #0
.L_0817744e:
	cmp r1, #31
	ble .L_08177454
	movs r1, #31
.L_08177454:
	adds r3, r5, #1
	cmp r0, r3
	ble .L_0817745c
	subs r0, #2
.L_0817745c:
	adds r3, r4, #1
	cmp r6, r3
	ble .L_08177464
	subs r6, #2
.L_08177464:
	adds r3, r1, #1
	cmp r7, r3
	ble .L_0817746c
	subs r7, #2
.L_0817746c:
	lsls r2, r6, #5
	lsls r3, r0, #10
	movs r5, #1
	orrs r3, r2
	add r9, r5
	orrs r3, r7
	mov r4, r10
	movs r6, #2
	mov r7, r9
	strh r3, [r4]
	add r12, r6
	add r10, r6
	cmp r7, #128
	bne .L_081773f0
.L_08177488:
	ldr r1, [sp, #84]
	movs r0, #19
	negs r0, r0
	adds r1, #1
	add r8, r0
	str r1, [sp, #84]
	cmp r1, #2
	beq .L_0817749a
	b .L_0817739e
.L_0817749a:
	ldr r2, [sp, #52]
	cmp r2, #23
	bhi .L_0817750c
	ldr r0, [sp, #20]
	ldr r7, [sp, #20]
	subs r0, #114
	movs r1, #6
	subs r7, #104
	bl Math_Div
	cmp r0, #0
	bge .L_081774b4
	movs r0, #0
.L_081774b4:
	movs r3, #0
	movs r4, #8
	lsls r0, r0, #5
	movs r5, #2
	mov r9, r3
	mov r11, r4
	mov r8, r0
	mov r10, r5
.L_081774c4:
	mov r0, r9
	lsls r6, r0, #8
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, r7, #0
	muls r5, r0
	adds r0, r6, #0
	bl Trig_Cos
	lsls r5, r5, #1
	asrs r5, r5, #16
	adds r3, r7, #0
	muls r3, r0
	mov r1, r10
	adds r5, #66
	subs r5, r5, r1
	ldr r1, [sp, #72]
	movs r2, #4
	str r2, [sp, #0]
	mov r4, r11
	adds r2, r5, #0
	asrs r3, r3, #16
	movs r5, #1
	movs r6, #128
	str r4, [sp, #4]
	adds r3, #68
	ldr r4, [sp, #176]
	ldr r0, [sp, #96]
	add r1, r8
	add r9, r5
	lsls r6, r6, #1
	mov lr, r4
	.2byte 0xf800
	cmp r9, r6
	bne .L_081774c4
.L_0817750c:
	ldr r3, [sp, #80]
	subs r3, #108
	cmp r3, #23
	bhi .L_0817757c
	movs r0, #160
	ldr r4, .L_08177540
	movs r7, #0
	lsls r0, r0, #19
	mov r9, r7
	adds r0, #192
.L_08177520:
	ldrh r3, [r0]
	movs r1, #31
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	ands r2, r4
	lsrs r3, r3, #26
	subs r2, #1
	ands r3, r4
	cmp r2, #0
	bge .L_08177538
	subs r1, #1
.L_08177538:
	cmp r1, #31
	ble .L_08177544
	movs r1, #31
	b .L_0817754a
.L_08177540:
	.4byte 0x0000001f
.L_08177544:
	cmp r1, #0
	bge .L_0817754a
	movs r1, #0
.L_0817754a:
	cmp r2, #31
	ble .L_08177552
	movs r2, #31
	b .L_08177558
.L_08177552:
	cmp r2, #0
	bge .L_08177558
	movs r2, #0
.L_08177558:
	cmp r3, #31
	ble .L_08177560
	movs r3, #31
	b .L_08177566
.L_08177560:
	cmp r3, #0
	bge .L_08177566
	movs r3, #0
.L_08177566:
	lsls r2, r2, #5
	lsls r3, r3, #10
	orrs r3, r2
	orrs r3, r1
	movs r1, #1
	add r9, r1
	mov r2, r9
	strh r3, [r0]
	adds r0, #2
	cmp r2, #128
	bne .L_08177520
.L_0817757c:
	ldr r3, [sp, #52]
	cmp r3, #35
	bls .L_08177584
	b .L_081776d0
.L_08177584:
	ldr r4, [sp, #80]
	cmp r4, #56
	bne .L_0817764e
	ldr r5, [sp, #92]
	movs r6, #239
	lsls r6, r6, #7
	adds r2, r5, r6
	movs r3, #3
	movs r7, #238
	str r3, [r2]
	lsls r7, r7, #7
	ldr r3, .L_0817769c
	adds r7, #132
	adds r2, r5, r7
	str r3, [r2]
	ldr r0, .L_081776a0
	bl Resource_GetTableEntry
	movs r2, #172
	adds r7, r0, #0
	adds r7, #32
	lsls r2, r2, #6
	adds r0, r7, #0
	adds r1, r5, r2
	bl Func_0801587c
	movs r3, #7
	movs r4, #0
	movs r7, #0
	mov r10, r3
	mov lr, r4
.L_081775c2:
	ldr r3, [sp, #92]
	movs r5, #172
	lsls r5, r5, #6
	add r3, lr
	adds r5, #1
	adds r5, r5, r3
	movs r6, #0
	mov r8, lr
	mov r12, r5
.L_081775d4:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_081775dc
	adds r3, r7, #7
.L_081775dc:
	asrs r3, r3, #3
	lsls r3, r3, #5
	adds r2, r6, #0
	cmp r6, #0
	bge .L_081775e8
	adds r2, r6, #7
.L_081775e8:
	asrs r2, r2, #3
	mov r0, r10
	adds r2, r3, r2
	adds r3, r7, #0
	ands r3, r0
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r1, .L_081776a4
	adds r3, r6, #0
	ands r3, r0
	lsls r2, r2, #3
	adds r2, r2, r3
	adds r2, r2, r1
	ldrh r3, [r2]
	lsrs r5, r3, #8
	lsls r3, r3, #24
	lsrs r4, r3, #24
	mov r3, r8
	adds r0, r3, r6
	mov r3, r12
	ldrb r1, [r3]
	adds r3, r1, #0
	cmp r3, #0
	beq .L_0817761e
	adds r3, #224
	lsls r3, r3, #24
	lsrs r5, r3, #24
.L_0817761e:
	movs r1, #172
	lsls r1, r1, #6
	adds r3, r0, r1
	ldr r0, [sp, #92]
	ldrb r1, [r0, r3]
	adds r3, r1, #0
	cmp r3, #0
	beq .L_08177634
	adds r3, #224
	lsls r3, r3, #24
	lsrs r4, r3, #24
.L_08177634:
	lsls r3, r5, #8
	orrs r3, r4
	movs r1, #2
	adds r6, #2
	strh r3, [r2]
	add r12, r1
	cmp r6, #184
	bne .L_081775d4
	movs r2, #184
	adds r7, #1
	add lr, r2
	cmp r7, #80
	bne .L_081775c2
.L_0817764e:
	ldr r3, [sp, #80]
	cmp r3, #87
	bgt .L_081776d0
	ldr r0, .L_081776a0
	bl Resource_GetTableEntry
	ldr r7, [sp, #52]
	ldr r6, .L_081776a8
	lsls r7, r7, #9
	mov r8, r7
	ldr r7, .L_08177698
	movs r4, #0
	adds r5, r0, #0
	mov r9, r4
.L_0817766a:
	mov r0, r8
	bl Trig_Cos
	lsls r2, r0, #1
	adds r2, r2, r0
	ldrh r1, [r5]
	lsls r2, r2, #2
	asrs r2, r2, #16
	movs r3, #31
	adds r2, #16
	ands r3, r1
	lsls r1, r1, #16
	adds r4, r3, r2
	lsrs r3, r1, #21
	lsrs r1, r1, #26
	ands r3, r7
	ands r1, r7
	adds r0, r3, r2
	adds r1, r1, r2
	cmp r4, #31
	ble .L_081776ac
	movs r4, #31
	b .L_081776ac
.L_08177698:
	.4byte 0x0000001f
.L_0817769c:
	.4byte 0x04040404
.L_081776a0:
	.4byte 0x000000b5
.L_081776a4:
	.4byte 0x0600a900
.L_081776a8:
	.4byte 0x050001c0
.L_081776ac:
	cmp r0, #31
	ble .L_081776b2
	movs r0, #31
.L_081776b2:
	cmp r1, #31
	ble .L_081776b8
	movs r1, #31
.L_081776b8:
	lsls r3, r1, #10
	lsls r2, r0, #5
	movs r0, #1
	orrs r3, r2
	add r9, r0
	orrs r3, r4
	mov r1, r9
	strh r3, [r6]
	adds r5, #2
	adds r6, #2
	cmp r1, #16
	bne .L_0817766a
.L_081776d0:
	ldr r2, [sp, #80]
	cmp r2, #54
	bne .L_081776e4
	movs r1, #240
	ldr r0, [sp, #96]
	lsls r1, r1, #6
	ldr r2, .L_08177950
	ldr r3, .L_08177954
	mov lr, r3
	.2byte 0xf800
.L_081776e4:
	ldr r4, [sp, #80]
	cmp r4, #55
	bne .L_081776f8
	movs r1, #240
	ldr r0, [sp, #96]
	lsls r1, r1, #6
	ldr r2, .L_08177958
	ldr r5, .L_08177954
	mov lr, r5
	.2byte 0xf800
.L_081776f8:
	ldr r6, [sp, #80]
	cmp r6, #131
	ble .L_08177700
	b .L_081778f4
.L_08177700:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	adds r7, r0, #0
	ldr r0, [sp, #92]
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
	ldr r4, [sp, #20]
	ldr r0, [sp, #80]
	adds r2, r4, r6
	lsls r3, r2, #7
	subs r3, r3, r2
	adds r5, r4, #0
	lsls r4, r3, #2
	ldr r3, .L_0817795c
	subs r5, #64
	subs r6, r3, r4
	cmp r0, #55
	ble .L_08177744
	ldr r1, .L_08177960
	adds r6, r4, r1
.L_08177744:
	movs r2, #8
	negs r2, r2
	cmp r5, r2
	blt .L_08177750
	movs r5, #8
	negs r5, r5
.L_08177750:
	ldr r3, [sp, #80]
	cmp r3, #95
	ble .L_0817775e
	ldr r4, [sp, #20]
	movs r3, #184
	subs r5, r3, r4
	str r5, [r7, #20]
.L_0817775e:
	ldr r3, [sp, #112]
	ldr r2, .L_08177964
	ldr r0, [sp, #92]
	ands r3, r2
	movs r2, #6
	orrs r3, r2
	ldr r2, .L_08177968
	movs r1, #224
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	lsls r1, r1, #3
	add r2, sp, #112
	str r3, [sp, #112]
	adds r3, r0, r1
	str r3, [r2, #4]
	movs r3, #7
	str r3, [r7]
	ldr r3, .L_0817796c
	movs r4, #48
	str r3, [r7, #8]
	negs r4, r4
	mov r3, r10
	mov r8, r2
	str r2, [r7, #16]
	str r3, [r7, #12]
	cmp r5, r4
	ble .L_081777e4
	str r5, [r7, #20]
	bl Func_08014de4
	movs r0, #128
	lsls r0, r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_08015160
	adds r0, r6, #0
	adds r1, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #6
	bl SceneTransform_ApplyPitch
	ldr r6, [sp, #80]
	cmp r6, #55
	ble .L_081777ca
	lsls r0, r6, #10
	bl Func_08015068
	b .L_081777d4
.L_081777ca:
	ldr r1, [sp, #80]
	negs r0, r1
	lsls r0, r0, #10
	bl Func_08015068
.L_081777d4:
	ldr r0, .L_08177970
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081777e4:
	ldr r2, [sp, #80]
	cmp r2, #95
	ble .L_081777f8
	ldr r4, [sp, #20]
	adds r3, r4, r2
	movs r2, #142
	lsls r3, r3, #1
	lsls r2, r2, #2
	subs r5, r2, r3
	str r5, [r7, #20]
.L_081777f8:
	movs r6, #48
	negs r6, r6
	cmp r5, r6
	ble .L_08177838
	bl Func_08014de4
	movs r0, #128
	ldr r1, .L_08177974
	movs r2, #0
	lsls r0, r0, #11
	bl Func_08015160
	ldr r0, .L_08177978
	bl Func_0801521c
	movs r0, #128
	lsls r0, r0, #6
	bl SceneTransform_ApplyPitch
	ldr r5, [sp, #80]
	negs r0, r5
	lsls r0, r0, #9
	bl Func_08015068
	ldr r0, .L_08177970
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08177838:
	ldr r0, [sp, #80]
	cmp r0, #55
	ble .L_081778d6
	ldr r2, [sp, #20]
	ldr r1, [sp, #92]
	movs r4, #184
	lsls r4, r4, #5
	movs r0, #32
	adds r3, r1, r4
	mov r5, r8
	subs r2, #208
	negs r0, r0
	str r3, [r5, #4]
	cmp r2, r0
	blt .L_0817785a
	movs r2, #32
	negs r2, r2
.L_0817785a:
	str r2, [r7, #20]
	bl Func_08014de4
	movs r0, #128
	movs r2, #0
	ldr r1, .L_0817797c
	lsls r0, r0, #10
	bl Func_08015160
	movs r0, #176
	lsls r0, r0, #7
	adds r0, #240
	bl Func_0801521c
	movs r0, #128
	lsls r0, r0, #6
	bl SceneTransform_ApplyPitch
	ldr r1, [sp, #80]
	lsls r0, r1, #10
	bl Func_08015068
	ldr r5, .L_08177970
	mov r1, r10
	adds r0, r5, #0
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	ldr r4, [sp, #80]
	cmp r4, r6
	ble .L_081778d6
	bl Func_08014de4
	movs r0, #128
	ldr r1, .L_08177980
	movs r2, #0
	lsls r0, r0, #10
	bl Func_08015160
	ldr r0, .L_08177984
	bl Func_0801521c
	movs r0, #128
	lsls r0, r0, #6
	bl SceneTransform_ApplyPitch
	ldr r6, [sp, #80]
	negs r0, r6
	lsls r0, r0, #10
	bl Func_08015068
	adds r0, r5, #0
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081778d6:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
	movs r0, #240
	ldr r7, [sp, #92]
	lsls r0, r0, #7
	adds r0, #228
	adds r2, r7, r0
.L_081778ec:
	ldr r3, [r2]
	ldr r3, [r2]
	cmp r3, #1
	bls .L_081778ec
.L_081778f4:
	ldr r1, [sp, #92]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #20]
	ldr r5, [sp, #16]
	ldr r6, [sp, #80]
	movs r7, #197
	adds r4, #2
	adds r5, #2
	adds r6, #1
	lsls r7, r7, #1
	str r4, [sp, #20]
	str r5, [sp, #16]
	str r6, [sp, #80]
	cmp r6, r7
	beq .L_08177932
	ldr r3, .L_08177988
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08177932
	bl .L_08176320
.L_08177932:
	movs r0, #162
	bl Audio_PlayCue
	ldr r0, [sp, #80]
	cmp r0, #131
	bgt .L_0817798c
	ldr r1, [sp, #92]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	adds r3, r1, r2
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	b .L_081779aa
.L_08177950:
	.4byte 0x20202020
.L_08177954:
	.4byte IwramFillWords
.L_08177958:
	.4byte 0x3f3f3f3f
.L_0817795c:
	.4byte 0x0001c350
.L_08177960:
	.4byte 0xffff32a0
.L_08177964:
	.4byte 0xffffff00
.L_08177968:
	.4byte 0xffff00ff
.L_0817796c:
	.4byte Data_08199340
.L_08177970:
	.4byte Data_08199210
.L_08177974:
	.4byte 0xfff00000
.L_08177978:
	.4byte 0xffff58f0
.L_0817797c:
	.4byte 0xffe00000
.L_08177980:
	.4byte 0xffd00000
.L_08177984:
	.4byte 0xffffce20
.L_08177988:
	.4byte gInput
.L_0817798c:
	ldr r4, [sp, #92]
	movs r6, #238
	lsls r6, r6, #7
	movs r3, #0
	adds r6, #220
	mov r9, r3
	adds r5, r4, r6
.L_0817799a:
	movs r7, #1
	ldmia r5!, {r0}
	add r9, r7
	bl ResourceObject_ReleaseFar
	mov r0, r9
	cmp r0, #14
	bne .L_0817799a
.L_081779aa:
	ldr r5, .L_08177a0c
	movs r1, #0
	mov r9, r1
.L_081779b0:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #48
	bne .L_081779b0
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08014c4c
	ldr r3, .L_08177a08
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r5, #237
	ldr r4, [sp, #68]
	lsls r5, r5, #3
	adds r5, #255
	adds r2, r4, r5
	movs r3, #0
	strb r3, [r2]
	movs r6, #206
	lsls r6, r6, #3
	adds r3, r4, r6
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #24
	bl Func_08118040
	ldr r5, .L_08177a10
	movs r1, #128
	ldr r0, [sp, #96]
	lsls r1, r1, #7
	movs r2, #0
	b .L_08177a14
.L_08177a08:
	.4byte 0x00000141
.L_08177a0c:
	.4byte gMapCellBuffer
.L_08177a10:
	.4byte IwramFillWords
.L_08177a14:
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_08177a90
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	ldr r2, .L_08177a94
	movs r3, #120
	str r3, [r2, #16]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	movs r7, #0
	str r3, [sp, #36]
	str r7, [sp, #32]
	add r2, sp, #32
	movs r1, #54
	ldrsh r0, [r3, r1]
	movs r4, #239
	str r0, [sp, #28]
	lsls r4, r4, #7
	ldrh r2, [r2]
	movs r6, #238
	strh r2, [r3, #54]
	ldr r3, [sp, #92]
	lsls r6, r6, #7
	adds r1, r3, r4
	movs r3, #1
	str r3, [r1]
	ldr r5, [sp, #92]
	adds r6, #132
	adds r2, r5, r6
	movs r3, #2
	str r7, [r2]
	str r3, [r1]
	movs r3, #50
	str r3, [r2]
	ldr r3, .L_08177a88
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	str r7, [r3]
	ldr r3, .L_08177a8c
	subs r2, #70
	movs r5, #160
	strh r3, [r2]
	movs r7, #1
	lsls r5, r5, #19
	mov r9, r7
	adds r5, #2
	b .L_08177a98
	.2byte 0x0000
.L_08177a88:
	.4byte 0x00000410
.L_08177a8c:
	.4byte 0x00000786
.L_08177a90:
	.4byte 0x06004000
.L_08177a94:
	.4byte gCameraSceneParameters
.L_08177a98:
	mov r1, r9
	lsls r0, r1, #1
	movs r1, #3
	bl Math_Div
	movs r3, #31
	mov r2, r9
	subs r1, r3, r0
	lsrs r3, r2, #31
	add r3, r9
	asrs r3, r3, #1
	movs r2, #8
	subs r2, r2, r3
	movs r3, #27
	subs r3, r3, r0
	cmp r1, #0
	bge .L_08177abc
	movs r1, #0
.L_08177abc:
	cmp r2, #0
	bge .L_08177ac2
	movs r2, #0
.L_08177ac2:
	cmp r3, #0
	bge .L_08177ac8
	movs r3, #0
.L_08177ac8:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	strh r3, [r5]
	movs r3, #1
	add r9, r3
	mov r4, r9
	adds r5, #2
	cmp r4, #64
	bne .L_08177a98
	ldr r5, [sp, #100]
	ldr r2, .L_08177b20
	ldr r3, [r5, #20]
	adds r6, r5, #0
	lsls r3, r3, #1
	adds r3, #36
	adds r0, r6, #0
	strh r2, [r6, r3]
	adds r0, #36
	movs r1, #0
	bl Func_08118010
	ldr r7, [sp, #92]
	movs r1, #224
	movs r5, #128
	lsls r5, r5, #9
	lsls r1, r1, #3
	adds r0, r7, r1
	adds r2, r5, #0
	movs r1, #56
	bl Func_0815b434
	movs r2, #184
	lsls r2, r2, #5
	adds r0, r7, r2
	movs r1, #16
	adds r2, r5, #0
	bl Func_0815b434
	movs r3, #156
	lsls r3, r3, #6
	b .L_08177b24
	.2byte 0x0000
.L_08177b20:
	.4byte 0x000000ff
.L_08177b24:
	movs r1, #128
	adds r0, r7, r3
	lsls r1, r1, #5
	ldr r3, .L_08177e54
	ldr r2, .L_08177e58
	mov lr, r3
	.2byte 0xf800
	movs r4, #220
	lsls r4, r4, #6
	adds r1, r7, r4
	ldr r0, .L_08177e5c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r3, #0
	ldr r0, .L_08177e60
	ldr r1, [sp, #72]
	movs r2, #0
	bl Func_08157cf4
	movs r3, #192
	movs r5, #0
	lsls r3, r3, #2
	mov r9, r5
	adds r3, #2
.L_08177b58:
	movs r6, #35
	add r9, r6
	cmp r9, r3
	bne .L_08177b58
	ldr r3, [sp, #92]
	movs r4, #220
	movs r7, #0
	movs r1, #128
	lsls r4, r4, #6
	mov r9, r7
	movs r0, #63
	lsls r1, r1, #5
	adds r2, r3, r4
.L_08177b72:
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08177b7a
	strb r0, [r2]
.L_08177b7a:
	movs r5, #1
	add r9, r5
	adds r2, #1
	cmp r9, r1
	bne .L_08177b72
	ldr r1, .L_08177e64
	ldr r0, .L_08177e68
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08177bb6
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08177bb6:
	strh r4, [r0]
	movs r6, #0
	ldr r2, [sp, #92]
	mov r9, r6
	adds r2, #24
.L_08177bc0:
	movs r0, #1
	mov r7, r9
	add r9, r0
	negs r3, r7
	mov r1, r9
	str r3, [r2]
	adds r2, #28
	cmp r1, #16
	bne .L_08177bc0
	ldr r3, .L_08177e6c
	movs r2, #0
	mov r9, r2
	subs r2, #1
.L_08177bda:
	movs r4, #1
	add r9, r4
	mov r5, r9
	str r2, [r3]
	adds r3, #28
	cmp r5, #128
	bne .L_08177bda
	ldr r7, [sp, #100]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r9, r6
	cmp r3, #0
	beq .L_08177c24
	ldr r0, [sp, #92]
	movs r1, #224
	lsls r1, r1, #2
	adds r5, r0, r1
	movs r6, #36
.L_08177bfe:
	ldr r2, [sp, #100]
	ldrsh r0, [r6, r2]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	movs r4, #1
	ldr r3, [r2, #8]
	add r9, r4
	str r3, [r5]
	adds r6, #2
	ldr r3, [r2, #12]
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	ldr r7, [sp, #100]
	adds r5, #28
	ldr r3, [r7, #20]
	cmp r9, r3
	bne .L_08177bfe
.L_08177c24:
	movs r0, #0
	str r0, [sp, #80]
.L_08177c28:
	ldr r3, .L_08177e70
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08177c3e
	ldr r1, [sp, #80]
	cmp r1, #119
	bgt .L_08177c3e
	movs r2, #120
	str r2, [sp, #80]
.L_08177c3e:
	ldr r3, [sp, #80]
	cmp r3, #0
	bne .L_08177cd2
	movs r0, #162
	bl Audio_PlayCue
	movs r5, #192
	ldr r4, [sp, #92]
	lsls r5, r5, #3
	adds r5, #252
	movs r3, #160
	adds r2, r4, r5
	str r3, [r2]
	ldr r7, [sp, #100]
	movs r6, #0
	str r6, [sp, #32]
	ldr r3, [r7, #20]
	mov r9, r6
	cmp r3, #0
	beq .L_08177c80
	movs r0, #236
	movs r1, #1
	lsls r0, r0, #1
	negs r1, r1
	adds r2, r4, r0
.L_08177c70:
	str r1, [r2]
	ldr r4, [sp, #100]
	movs r3, #1
	add r9, r3
	ldr r3, [r4, #20]
	adds r2, #28
	cmp r9, r3
	bne .L_08177c70
.L_08177c80:
	movs r5, #0
	mov r9, r5
	cmp r3, #0
	beq .L_08177cd2
	movs r6, #1
	negs r6, r6
	movs r7, #254
	mov r10, r6
	mov r8, r5
	lsls r7, r7, #24
	movs r6, #36
.L_08177c96:
	ldr r1, [sp, #100]
	ldrsh r0, [r6, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #100]
	mov r1, r10
	ldr r5, [r0]
	movs r2, #5
	ldrsh r0, [r6, r3]
	str r1, [sp, #0]
	mov r3, r10
	movs r1, #0
	bl Func_0814cd48
	movs r3, #160
	mov r2, r8
	lsls r3, r3, #13
	str r7, [r5, #8]
	str r3, [r5, #12]
	str r2, [r5, #16]
	str r2, [r5, #72]
	ldr r3, .L_08177e74
	ldr r5, [sp, #100]
	adds r7, r7, r3
	ldr r3, [r5, #20]
	movs r4, #1
	add r9, r4
	adds r6, #2
	cmp r9, r3
	bne .L_08177c96
.L_08177cd2:
	ldr r6, [sp, #80]
	cmp r6, #120
	bne .L_08177d3c
	ldr r7, [sp, #92]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #1
	adds r1, #132
	str r3, [r2]
	adds r3, r7, r1
	movs r2, #0
	str r2, [r3]
	ldr r3, [sp, #68]
	movs r4, #160
	lsls r4, r4, #3
	movs r1, #160
	movs r2, #153
	adds r4, #108
	lsls r1, r1, #19
	lsls r2, r2, #8
	adds r0, r3, r4
	adds r1, #192
	movs r3, #128
	adds r2, #160
	bl ColorBuffer_ScaleFar
	ldr r6, [sp, #100]
	movs r5, #0
	ldr r3, [r6, #20]
	mov r9, r5
	cmp r3, #0
	beq .L_08177d3c
	movs r6, #36
.L_08177d1a:
	ldr r7, [sp, #100]
	ldrsh r0, [r6, r7]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	movs r3, #248
	lsls r3, r3, #15
	str r5, [r2, #8]
	str r3, [r2, #12]
	str r5, [r2, #16]
	str r5, [r2, #72]
	ldr r3, [r7, #20]
	movs r2, #1
	add r9, r2
	adds r6, #2
	cmp r9, r3
	bne .L_08177d1a
.L_08177d3c:
	ldr r3, [sp, #80]
	cmp r3, #160
	bne .L_08177d48
	movs r0, #134
	bl Func_081180e8
.L_08177d48:
	ldr r4, [sp, #80]
	cmp r4, #139
	bgt .L_08177d50
	b .L_08177ffc
.L_08177d50:
	cmp r4, #140
	beq .L_08177d56
	b .L_08177f16
.L_08177d56:
	movs r6, #240
	ldr r7, .L_08177e78
	movs r5, #0
	lsls r6, r6, #14
	mov r9, r5
	mov r8, r6
.L_08177d62:
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	bl Random16
	adds r6, r0, #0
	mov r0, r8
	str r0, [r7]
	str r0, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #1
	asrs r3, r3, #5
	add r9, r2
	str r3, [r7, #16]
	movs r1, #0
	mov r3, r9
	str r1, [r7, #24]
	adds r7, #28
	cmp r3, #32
	bne .L_08177d62
	ldr r7, .L_08177e7c
	mov r9, r1
.L_08177dac:
	bl Random16
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r7]
	str r3, [r7, #4]
	adds r6, r0, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	movs r4, #1
	ands r3, r0
	add r9, r4
	adds r3, #16
	mov r5, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r5, #128
	bne .L_08177dac
	ldr r1, .L_08177e80
	movs r2, #1
	movs r3, #0
	ldr r0, .L_08177e84
	bl Func_08157cf4
	movs r0, #144
	bl Audio_PlayCue
	movs r1, #240
	ldr r3, .L_08177e54
	lsls r1, r1, #6
	ldr r2, .L_08177e58
	ldr r0, [sp, #96]
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_08177e50
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r6, [sp, #92]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r6, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	adds r2, r6, r0
	movs r3, #50
	str r3, [r2]
	ldr r2, [sp, #100]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r9, r1
	cmp r3, #0
	beq .L_08177eb0
	movs r3, #226
	lsls r3, r3, #2
	adds r5, r6, r3
	movs r6, #36
	b .L_08177e88
	.2byte 0x0000
.L_08177e50:
	.4byte 0x00000784
.L_08177e54:
	.4byte IwramFillWords
.L_08177e58:
	.4byte 0x3f3f3f3f
.L_08177e5c:
	.4byte 0x000000c3
.L_08177e60:
	.4byte 0x00000134
.L_08177e64:
	.4byte Data_020038e0
.L_08177e68:
	.4byte 0x04000208
.L_08177e6c:
	.4byte Data_02014218
.L_08177e70:
	.4byte gInput
.L_08177e74:
	.4byte 0xff800000
.L_08177e78:
	.4byte Data_02014c80
.L_08177e7c:
	.4byte Data_02015000
.L_08177e80:
	.4byte gMapCellBuffer
.L_08177e84:
	.4byte 0x0000013e
.L_08177e88:
	ldr r4, [sp, #100]
	ldrsh r0, [r6, r4]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	movs r3, #128
	lsls r3, r3, #14
	movs r0, #0
	str r0, [r2, #8]
	str r3, [r2, #12]
	movs r1, #1
	ldr r3, [r5]
	add r9, r1
	str r3, [r2, #16]
	ldr r2, [sp, #100]
	adds r5, #28
	ldr r3, [r2, #20]
	adds r6, #2
	cmp r9, r3
	bne .L_08177e88
.L_08177eb0:
	movs r4, #0
	mov r9, r4
	cmp r3, #0
	beq .L_08177f08
	movs r6, #36
.L_08177eba:
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r3, #31
	ldr r1, [sp, #100]
	ands r3, r0
	adds r3, #120
	movs r7, #128
	ldrsh r0, [r6, r1]
	lsls r7, r7, #10
	str r3, [sp, #4]
	lsls r5, r5, #1
	movs r2, #128
	movs r3, #128
	adds r5, r5, r7
	movs r1, #1
	lsls r2, r2, #11
	lsls r3, r3, #12
	str r5, [sp, #0]
	bl Func_0815f000
	ldr r3, [sp, #100]
	movs r1, #7
	ldrsh r0, [r6, r3]
	movs r3, #32
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r9
	bl Func_0814cd48
	ldr r7, [sp, #100]
	movs r5, #1
	ldr r3, [r7, #20]
	add r9, r5
	adds r6, #2
	cmp r9, r3
	bne .L_08177eba
.L_08177f08:
	ldr r0, [sp, #92]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #3
	str r3, [r2]
.L_08177f16:
	movs r0, #104
	movs r1, #31
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r5, .L_08178064
	str r3, [sp, #176]
	movs r2, #0
	mov r9, r2
.L_08177f2c:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_08177f6a
	adds r1, r3, #0
	cmp r3, #0
	bge .L_08177f3a
	adds r1, r3, #3
.L_08177f3a:
	ldr r3, .L_08178068
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #176]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_0817806c
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_08177f6a:
	movs r7, #1
	add r9, r7
	adds r3, #1
	mov r0, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #16
	bne .L_08177f2c
	ldr r6, .L_08178070
	movs r1, #0
	mov r9, r1
.L_08177f80:
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_08177fea
	asrs r3, r3, #3
	adds r5, r3, #2
	ldr r3, .L_08178074
	movs r2, #0
	mov r11, r3
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	mov r8, r2
	lsls r7, r5, #1
	mov r10, r3
.L_08177f9c:
	subs r3, r7, #2
	mov r4, r11
	ldrh r1, [r4, r3]
	ldr r0, [sp, #72]
	movs r3, #2
	ldrsh r2, [r6, r3]
	adds r1, r0, r1
	movs r0, #6
	ldrsh r3, [r6, r0]
	mov r4, r10
	subs r2, r2, r4
	subs r3, r3, r5
	str r5, [sp, #0]
	str r7, [sp, #4]
	ldr r4, [sp, #176]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	movs r1, #63
	adds r0, r6, #0
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #4]
	movs r1, #224
	lsls r1, r1, #15
	cmp r3, r1
	ble .L_08177fda
	ldr r3, [r6, #16]
	negs r3, r3
	str r3, [r6, #16]
.L_08177fda:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #4
	bne .L_08177f9c
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_08177fea:
	movs r4, #1
	add r9, r4
	mov r5, r9
	adds r6, #28
	cmp r5, #128
	bne .L_08177f80
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_08177ffc:
	ldr r6, [sp, #80]
	cmp r6, #119
	ble .L_08178004
	b .L_0817810e
.L_08178004:
	movs r7, #0
	mov r9, r7
.L_08178008:
	ldr r0, .L_08178078
	mov r1, r9
	ldrb r2, [r0, r1]
	ldr r3, [sp, #80]
	cmp r3, r2
	bne .L_0817802c
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	ldr r2, .L_0817807c
	ldr r3, .L_08178080
	adds r0, #192
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
	ldr r4, .L_08178078
	mov r5, r9
	ldrb r2, [r4, r5]
.L_0817802c:
	ldr r6, [sp, #80]
	adds r3, r2, #1
	cmp r6, r3
	blt .L_08178102
	adds r3, #15
	cmp r6, r3
	bge .L_08178102
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r7, #160
	movs r0, #0
	lsls r7, r7, #3
	ldr r1, .L_08178060
	adds r7, #108
	str r0, [sp, #84]
	adds r7, r7, r3
	movs r2, #31
	movs r3, #10
	mov r11, r7
	mov lr, r1
	mov r10, r2
	mov r8, r3
	mov r12, r0
	b .L_08178084
	.2byte 0x0000
.L_08178060:
	.4byte 0x0000001f
.L_08178064:
	.4byte Data_02014c80
.L_08178068:
	.4byte gMapCellBuffer
.L_0817806c:
	.4byte 0xffffe000
.L_08178070:
	.4byte Data_02015000
.L_08178074:
	.4byte Data_08197410
.L_08178078:
	.4byte Data_08198c67
.L_0817807c:
	.4byte 0x7fff7fff
.L_08178080:
	.4byte IwramFillWords
.L_08178084:
	mov r4, r12
	mov r5, r11
	ldrh r2, [r4, r5]
	mov r3, r10
	ands r3, r2
	mov r7, r8
	lsls r2, r2, #16
	mov r0, lr
	subs r6, r3, r7
	lsrs r3, r2, #21
	lsrs r2, r2, #26
	ands r2, r0
	subs r5, r2, r7
	movs r7, #160
	lsls r7, r7, #19
	adds r7, #192
	ands r3, r0
	add r7, r12
	adds r4, r3, #0
	ldrh r3, [r7]
	mov r0, r10
	ands r0, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	mov r1, lr
	ands r2, r1
	lsrs r1, r3, #26
	mov r3, lr
	subs r4, #20
	ands r1, r3
	cmp r6, #0
	bge .L_081780c6
	movs r6, #0
.L_081780c6:
	cmp r4, #0
	bge .L_081780cc
	movs r4, #0
.L_081780cc:
	cmp r5, #0
	bge .L_081780d2
	movs r5, #0
.L_081780d2:
	subs r3, r0, #1
	cmp r6, r3
	bge .L_081780da
	subs r0, #2
.L_081780da:
	subs r3, r2, #1
	cmp r4, r3
	bge .L_081780e2
	subs r2, #2
.L_081780e2:
	subs r3, r1, #1
	cmp r5, r3
	bge .L_081780ea
	subs r1, #2
.L_081780ea:
	lsls r3, r1, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r7]
	ldr r5, [sp, #84]
	movs r4, #2
	adds r5, #1
	add r12, r4
	str r5, [sp, #84]
	cmp r5, #128
	bne .L_08178084
.L_08178102:
	movs r6, #1
	add r9, r6
	mov r7, r9
	cmp r7, #3
	beq .L_0817810e
	b .L_08178008
.L_0817810e:
	ldr r0, [sp, #80]
	cmp r0, #120
	ble .L_0817817c
	cmp r0, #121
	bne .L_0817812e
	add r1, sp, #76
	ldr r3, .L_081782a0
	ldrh r1, [r1]
	movs r2, #0
	strh r1, [r3, #4]
	ldr r3, [sp, #88]
	add r4, sp, #28
	str r2, [r3, #16]
	ldrh r4, [r4]
	ldr r5, [sp, #36]
	strh r4, [r5, #54]
.L_0817812e:
	ldr r5, [sp, #80]
	cmp r5, #147
	ble .L_08178136
	b .L_081785f6
.L_08178136:
	adds r0, r5, #0
	subs r0, #120
	movs r1, #6
	bl Math_Div
	movs r7, #192
	ldr r6, [sp, #92]
	lsls r7, r7, #3
	adds r7, #252
	adds r2, r6, r7
	ldr r6, [r2]
	movs r3, #8
	subs r3, r3, r0
	subs r3, r6, r3
	str r3, [r2]
	cmp r6, #0
	bgt .L_0817815a
	b .L_081785f6
.L_0817815a:
	ldr r0, [sp, #92]
	movs r1, #224
	lsls r1, r1, #3
	adds r5, r0, r1
	movs r2, #128
	adds r0, r5, #0
	adds r1, r6, #0
	lsls r2, r2, #9
	bl Func_0815b434
	adds r0, r5, #0
	movs r1, #60
	movs r2, #60
	adds r3, r6, #0
	bl Func_0818caa8
	b .L_081785f6
.L_0817817c:
	ldr r3, [sp, #80]
	subs r3, #4
	cmp r3, #21
	bhi .L_0817818a
	ldr r2, [sp, #32]
	adds r2, #4
	str r2, [sp, #32]
.L_0817818a:
	ldr r3, [sp, #80]
	cmp r3, #107
	ble .L_0817819e
	ldr r4, [sp, #32]
	subs r4, #8
	str r4, [sp, #32]
	cmp r4, #0
	bge .L_0817819e
	movs r5, #0
	str r5, [sp, #32]
.L_0817819e:
	ldr r7, [sp, #100]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r9, r6
	cmp r3, #0
	bne .L_081781ac
	b .L_08178320
.L_081781ac:
	ldr r0, [sp, #92]
	movs r1, #224
	lsls r1, r1, #1
	adds r2, r7, #0
	str r6, [sp, #12]
	adds r0, r0, r1
	adds r2, #36
	mov r8, r0
	mov r11, r2
.L_081781be:
	mov r4, r11
	movs r3, #0
	ldrsh r0, [r4, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r5, .L_081782a4
	ldr r3, [r0, #12]
	mov r10, r0
	movs r1, #0
	cmp r3, r5
	bgt .L_081782ac
	mov r6, r8
	ldr r3, [r6, #24]
	movs r7, #1
	negs r7, r7
	cmp r3, r7
	bne .L_081781f0
	ldr r3, [r0, #8]
	movs r0, #160
	lsls r0, r0, #12
	adds r3, r3, r0
	mov r2, r10
	str r3, [r2, #8]
	b .L_081781fc
.L_081781f0:
	mov r4, r10
	ldr r3, [r4, #8]
	movs r5, #128
	lsls r5, r5, #10
	adds r3, r3, r5
	str r3, [r4, #8]
.L_081781fc:
	mov r6, r8
	ldr r2, [r6, #24]
	movs r7, #1
	negs r7, r7
	cmp r2, r7
	bne .L_081782b0
	mov r0, r10
	ldr r3, [r0, #8]
	movs r4, #192
	lsls r4, r4, #14
	cmp r3, r4
	ble .L_08178216
	movs r1, #1
.L_08178216:
	cmp r1, #1
	bne .L_081782b0
	movs r5, #0
	mov r6, r8
	str r5, [r6, #24]
	mov r1, r11
	movs r7, #0
	ldrsh r0, [r1, r7]
	mov r1, r8
	bl Func_0815e20c
	movs r0, #103
	bl Audio_PlayCue
	ldr r3, .L_081782a8
	ldr r2, [sp, #12]
	str r5, [sp, #84]
	adds r7, r2, r3
.L_0817823a:
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r5, #255
	ands r5, r0
	mov r0, r8
	ldr r3, [r0]
	movs r4, #128
	lsls r3, r3, #15
	str r3, [r7]
	lsls r4, r4, #1
	ldr r3, [r0, #4]
	adds r0, r6, #0
	lsls r3, r3, #16
	str r3, [r7, #4]
	adds r5, r5, r4
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	ldr r1, [sp, #84]
	adds r7, #28
	adds r1, #1
	str r1, [sp, #84]
	cmp r1, #16
	bne .L_0817823a
	mov r3, r8
	ldr r2, [r3, #24]
	b .L_081782b0
.L_081782a0:
	.4byte Data_03001120
.L_081782a4:
	.4byte 0x0063ffff
.L_081782a8:
	.4byte Data_02014200
.L_081782ac:
	mov r4, r8
	ldr r2, [r4, #24]
.L_081782b0:
	cmp r2, #2
	beq .L_081782b8
	cmp r2, #7
	bne .L_081782d0
.L_081782b8:
	movs r3, #32
	mov r6, r11
	movs r5, #0
	ldrsh r0, [r6, r5]
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	mov r3, r9
	mov r7, r8
	bl Func_0814cd48
	ldr r2, [r7, #24]
.L_081782d0:
	cmp r2, #6
	bne .L_081782ee
	mov r2, r11
	movs r1, #0
	ldrsh r0, [r2, r1]
	movs r3, #32
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	mov r3, r9
	movs r1, #15
	bl Func_0814cd48
	mov r3, r8
	ldr r2, [r3, #24]
.L_081782ee:
	cmp r2, #8
	bne .L_08178300
	movs r3, #148
	mov r5, r10
	movs r4, #0
	lsls r3, r3, #16
	str r4, [r5, #8]
	str r3, [r5, #12]
	str r4, [r5, #16]
.L_08178300:
	ldr r0, [sp, #12]
	movs r1, #224
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [sp, #12]
	ldr r4, [sp, #100]
	movs r2, #1
	ldr r3, [r4, #20]
	movs r6, #2
	movs r7, #28
	add r9, r2
	add r11, r6
	add r8, r7
	cmp r9, r3
	beq .L_08178320
	b .L_081781be
.L_08178320:
	ldr r5, [sp, #80]
	cmp r5, #0
	bge .L_08178328
	b .L_08178524
.L_08178328:
	ldr r2, .L_08178658
	movs r6, #255
	ldrh r3, [r2, #4]
	lsls r6, r6, #8
	adds r6, #244
	adds r3, r3, r6
	strh r3, [r2, #4]
	movs r1, #27
	movs r0, #104
	bl Func_081963ec
	movs r7, #0
	ldr r2, [sp, #92]
	mov r9, r7
	movs r0, #26
	movs r3, #168
	ldr r7, [sp, #32]
	negs r0, r0
	movs r1, #0
	lsls r3, r3, #3
	movs r4, #28
	lsls r6, r5, #10
	mov r10, r0
	mov r8, r1
	adds r5, r2, r3
	mov r11, r4
.L_0817835c:
	adds r0, r6, #0
	bl Trig_Sin
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r3, r7, r0
	mov r1, r9
	mov r0, r8
	str r3, [r5]
	str r0, [r5, #4]
	cmp r1, #3
	bne .L_0817838a
	subs r3, #8
	mov r2, r10
	movs r0, #104
	str r3, [r5]
	str r2, [r5, #4]
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
.L_0817838a:
	ldr r2, [r5]
	mov r0, r11
	movs r1, #56
	ldr r4, [r5, #4]
	mov r3, r11
	str r0, [sp, #0]
	str r1, [sp, #4]
	subs r3, r2, r3
	movs r2, #192
	mov r12, r3
	lsls r2, r2, #18
	ldr r3, [sp, #92]
	mov lr, r4
	ldr r4, [r2, #104]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r0, [sp, #96]
	mov r3, lr
	mov r2, r12
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	movs r4, #128
	add r9, r0
	movs r3, #30
	lsls r4, r4, #6
	mov r1, r9
	add r10, r3
	add r8, r3
	adds r7, #8
	adds r6, r6, r4
	adds r5, #28
	cmp r1, #4
	bne .L_0817835c
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #92]
	movs r2, #0
	movs r6, #160
	mov r9, r2
	lsls r6, r6, #15
.L_081783e0:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #4
	bne .L_081783ee
	movs r3, #0
	str r3, [r5, #24]
.L_081783ee:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_08178428
	bl Random16
	movs r1, #24
	bl Math_ModU
	ldr r4, [sp, #32]
	mov r7, r9
	lsls r3, r7, #1
	adds r0, r0, r4
	subs r0, r0, r3
	lsls r0, r0, #16
	str r0, [r5]
	str r6, [r5, #4]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	ldr r0, .L_0817865c
	negs r3, r3
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, .L_08178660
	str r3, [r5, #16]
	ldr r3, [r5, #24]
.L_08178428:
	cmp r3, #0
	blt .L_08178452
	ldr r4, [sp, #92]
	movs r7, #184
	lsls r7, r7, #5
	movs r2, #2
	ldrsh r1, [r5, r2]
	adds r0, r4, r7
	movs r3, #6
	ldrsh r2, [r5, r3]
	movs r3, #16
	bl Func_0818caa8
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
.L_08178452:
	movs r1, #1
	movs r0, #192
	add r9, r1
	lsls r0, r0, #10
	mov r2, r9
	adds r6, r6, r0
	adds r5, #28
	cmp r2, #12
	bne .L_081783e0
	movs r0, #104
	movs r1, #31
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r4, [sp, #92]
	movs r7, #168
	str r3, [sp, #176]
	lsls r7, r7, #3
	movs r3, #0
	mov r9, r3
	movs r6, #0
	adds r5, r4, r7
.L_08178482:
	ldr r2, [r5]
	movs r3, #120
	subs r3, r3, r2
	adds r5, #28
	cmp r3, #0
	ble .L_081784a6
	str r3, [sp, #0]
	movs r3, #30
	str r3, [sp, #4]
	ldr r3, [sp, #92]
	movs r7, #156
	lsls r7, r7, #6
	adds r1, r3, r7
	ldr r4, [sp, #176]
	ldr r0, [sp, #96]
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
.L_081784a6:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r6, #30
	cmp r1, #4
	bne .L_08178482
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	movs r1, #47
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r6, .L_08178664
	str r3, [sp, #176]
	ldr r5, .L_08178668
	movs r2, #0
	mov r9, r2
.L_081784d0:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08178512
	asrs r0, r0, #3
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r3, [sp, #72]
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r3, r1
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #96]
	ldr r4, [sp, #176]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08178512:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r5, #28
	cmp r1, #96
	bne .L_081784d0
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_08178524:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0817866c
	ldr r3, [sp, #104]
	movs r4, #220
	ands r3, r2
	movs r2, #6
	orrs r3, r2
	ldr r2, .L_08178670
	lsls r4, r4, #6
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, [sp, #92]
	str r3, [sp, #104]
	adds r3, r2, r4
	add r2, sp, #104
	adds r7, r0, #0
	str r3, [r2, #4]
	movs r3, #9
	str r3, [r7]
	ldr r3, .L_08178674
	mov r5, r8
	str r3, [r7, #8]
	str r2, [r7, #16]
	str r5, [r7, #12]
	ldr r0, [sp, #100]
	movs r6, #0
	ldr r3, [r0, #20]
	mov r9, r6
	cmp r3, #0
	beq .L_081785f6
	ldr r1, [sp, #92]
	movs r2, #224
	lsls r2, r2, #1
	adds r6, r1, r2
.L_08178578:
	ldr r1, [r6, #24]
	cmp r1, #17
	bhi .L_081785dc
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r5, r3, #7
	adds r2, r3, #0
	subs r5, r5, r3
	movs r3, #128
	lsls r3, r3, #4
	lsls r5, r5, #3
	adds r5, r5, r3
	subs r2, #64
	adds r3, r1, #1
	str r3, [r6, #24]
	str r2, [r7, #20]
	bl Func_08014de4
	ldr r0, [r6]
	ldr r1, [r6, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #56
	subs r1, #56
	movs r2, #0
	lsls r0, r0, #16
	lsls r1, r1, #16
	bl Func_08015160
	movs r1, #3
	adds r0, r5, #0
	bl Math_Div
	adds r1, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	movs r0, #192
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	ldr r0, .L_08178678
	mov r1, r8
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081785dc:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	ldr r5, [sp, #100]
	movs r4, #1
	ldr r3, [r5, #20]
	add r9, r4
	adds r6, #28
	cmp r9, r3
	bne .L_08178578
.L_081785f6:
	movs r0, #8
	bl Func_08158d68
	bl Func_081434f8
	movs r7, #240
	ldr r6, [sp, #92]
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r6, r7
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #80]
	adds r0, #1
	str r0, [sp, #80]
	cmp r0, #180
	beq .L_08178622
	bl .L_08177c28
.L_08178622:
	ldr r2, [sp, #100]
	movs r1, #0
	ldr r0, [r2, #20]
	mov r9, r1
	cmp r0, #0
	beq .L_08178638
	mov r12, r0
.L_08178630:
	movs r3, #1
	add r9, r3
	cmp r9, r12
	bne .L_08178630
.L_08178638:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0817867c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #324
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08178658:
	.4byte Data_03001120
.L_0817865c:
	.4byte 0xffffc000
.L_08178660:
	.4byte 0xffff0000
.L_08178664:
	.4byte Data_08197410
.L_08178668:
	.4byte Data_02014200
.L_0817866c:
	.4byte 0xffffff00
.L_08178670:
	.4byte 0xffff00ff
.L_08178674:
	.4byte Data_08199340
.L_08178678:
	.4byte Data_08199210
.L_0817867c:
	.4byte Func_08143000
