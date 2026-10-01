.syntax unified
	.thumb
	.global Func_0813fd84
	.thumb_func
Func_0813fd84:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #196
	str r0, [sp, #132]
	movs r5, #192
	lsls r5, r5, #18
	ldr r1, [r5, #96]
	ldr r0, [r5, #92]
	str r1, [sp, #128]
	adds r3, r5, #0
	ldr r2, [r5, #100]
	adds r3, #176
	str r2, [sp, #112]
	mov r11, r0
	ldr r3, [r3]
	str r3, [sp, #108]
	bl Func_0813ba50
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0813fdf4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r5, [r5, #104]
	adds r2, #132
	add r2, r11
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0813fdf8
	str r5, [sp, #120]
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0813fdfc
	ldr r4, [sp, #132]
	b .L_0813fe00
.L_0813fdf4:
	.4byte 0x00001010
.L_0813fdf8:
	.4byte Func_08143000
.L_0813fdfc:
	.4byte Data_03001120
.L_0813fe00:
	ldrh r3, [r3, #4]
	str r3, [sp, #104]
	movs r3, #0
	str r3, [sp, #100]
	str r3, [sp, #96]
	ldr r0, [r4, #8]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	ldr r5, [sp, #96]
	str r5, [r6, #72]
	bl Func_0815b410
	movs r0, #140
	bl Audio_PlayCue
	movs r0, #0
	movs r1, #224
	lsls r1, r1, #3
	str r0, [sp, #116]
	add r1, r11
	add r7, sp, #184
	mov r8, r1
	mov r9, r0
.L_0813fe30:
	ldr r2, [sp, #116]
	cmp r2, #24
	bne .L_0813fe4c
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #4
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r3, .L_0813ff34
	adds r2, #132
	add r2, r11
	str r3, [r2]
.L_0813fe4c:
	ldr r3, [sp, #132]
	mov r5, r9
	ldr r0, [r3, #8]
	adds r1, r7, #0
	adds r5, #2
	bl Func_0815e21c
	cmp r5, #96
	ble .L_0813fe60
	movs r5, #96
.L_0813fe60:
	movs r2, #128
	mov r0, r8
	adds r1, r5, #0
	lsls r2, r2, #9
	bl Func_0815b434
	ldr r1, [r7]
	ldr r2, [r7, #4]
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	subs r2, #16
	mov r0, r8
	adds r3, r5, #0
	bl Func_0818caa8
	ldr r4, [sp, #116]
	cmp r4, #15
	ble .L_0813fe90
	ldr r3, [r6, #12]
	movs r5, #128
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r6, #12]
.L_0813fe90:
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	movs r0, #1
	add r3, r11
	str r0, [r3]
	mov r10, r0
	bl WaitFrames
	ldr r2, [sp, #116]
	movs r1, #6
	adds r2, #1
	add r9, r1
	str r2, [sp, #116]
	cmp r2, #36
	bne .L_0813fe30
	movs r3, #171
	lsls r3, r3, #8
	movs r5, #0
	adds r3, #133
	movs r1, #128
	str r3, [r6, #72]
	str r5, [r6, #12]
	lsls r1, r1, #7
	ldr r3, .L_0813ff38
	ldr r2, .L_0813ff3c
	ldr r0, .L_0813ff40
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_0813ff44
	movs r3, #240
	str r3, [r2, #16]
	lsls r3, r3, #7
	adds r3, #240
	add r3, r11
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r1, .L_0813ff48
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	ldr r4, [sp, #108]
	mov r3, r10
	str r3, [r4, #16]
	ldr r3, .L_0813ff4c
	movs r2, #128
	strh r5, [r3, #4]
	ldr r3, .L_0813ff30
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	str r5, [r3]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r5, #224
	str r3, [sp, #120]
	ldr r1, [sp, #112]
	ldr r0, .L_0813ff50
	movs r2, #0
	movs r3, #0
	lsls r5, r5, #3
	bl Resource_LoadAndDecompress
	add r5, r11
	ldr r0, .L_0813ff54
	ldr r1, .L_0813ff58
	movs r2, #0
	movs r3, #0
	b .L_0813ff5c
	.2byte 0x0000
.L_0813ff30:
	.4byte 0x00000080
.L_0813ff34:
	.4byte 0x06060606
.L_0813ff38:
	.4byte IwramFillWords
.L_0813ff3c:
	.4byte 0x3f3f3f3f
.L_0813ff40:
	.4byte 0x06004000
.L_0813ff44:
	.4byte gCameraSceneParameters
.L_0813ff48:
	.4byte 0x00000071
.L_0813ff4c:
	.4byte Data_03001120
.L_0813ff50:
	.4byte 0x00000134
.L_0813ff54:
	.4byte 0x000000c2
.L_0813ff58:
	.4byte gMapCellBuffer
.L_0813ff5c:
	bl Resource_LoadAndDecompress
	adds r1, r5, #0
	ldr r0, .L_08140018
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	adds r0, r5, #0
	ldr r1, .L_0814001c
	movs r2, #64
	movs r3, #64
	bl Func_0816ae40
	movs r1, #180
	lsls r1, r1, #6
	adds r1, #72
	ldr r0, .L_08140020
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #244
	lsls r1, r1, #6
	adds r1, #72
	ldr r0, .L_08140024
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #244
	lsls r1, r1, #6
	adds r1, #136
	ldr r0, .L_08140028
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r7, .L_0814002c
	movs r5, #0
	mov r10, r5
	movs r6, #2
.L_0813ffb6:
	ldrh r0, [r5, r7]
	movs r1, #130
	lsls r1, r1, #7
	adds r1, #72
	add r0, r11
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #9
	adds r1, r6, #0
	bl Func_0815b510
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r6, #2
	adds r5, #2
	cmp r3, #10
	bne .L_0813ffb6
	bl Func_0815b410
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	str r3, [r2]
	movs r2, #128
	ldr r3, .L_08140014
	lsls r2, r2, #19
	movs r4, #0
	adds r2, #82
	strh r3, [r2]
	str r4, [sp, #92]
	str r4, [sp, #88]
	str r4, [sp, #84]
	str r4, [sp, #80]
	str r4, [sp, #76]
	str r4, [sp, #72]
	str r4, [sp, #68]
	mov r10, r4
	mov r5, r11
	b .L_08140030
.L_08140014:
	.4byte 0x00001010
.L_08140018:
	.4byte 0x000000ec
.L_0814001c:
	.4byte Data_02014000
.L_08140020:
	.4byte 0x000000b4
.L_08140024:
	.4byte 0x000000c9
.L_08140028:
	.4byte 0x000000ca
.L_0814002c:
	.4byte Data_08197438
.L_08140030:
	bl Random16
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #170
	mov r2, r10
	muls r2, r3
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r3, r0
	adds r2, r2, r3
	str r2, [r5, #8]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r0, #1
	add r10, r0
	negs r3, r3
	mov r1, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #24
	bne .L_08140030
	movs r5, #224
	movs r2, #0
	lsls r5, r5, #2
	mov r10, r2
	movs r6, #0
	add r5, r11
.L_0814006e:
	bl Random16
	movs r3, #127
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #88
	str r3, [r5, #4]
	bl Random16
	movs r3, #3
	ands r3, r0
	adds r3, #5
	str r3, [r5, #8]
	movs r3, #1
	add r10, r3
	mov r4, r10
	str r6, [r5, #24]
	subs r6, #2
	adds r5, #28
	cmp r4, #31
	bne .L_0814006e
	mov r0, sp
	movs r5, #0
	adds r0, #160
	str r5, [sp, #116]
	str r0, [sp, #44]
.L_081400aa:
	ldr r3, .L_081402dc
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081400c0
	ldr r1, [sp, #116]
	cmp r1, #168
	bgt .L_081400c0
	movs r2, #169
	str r2, [sp, #116]
.L_081400c0:
	ldr r3, [sp, #116]
	cmp r3, #4
	bne .L_081400cc
	movs r0, #220
	bl Audio_PlayCue
.L_081400cc:
	ldr r4, [sp, #116]
	cmp r4, #40
	bne .L_081400d8
	movs r0, #154
	bl Audio_PlayCue
.L_081400d8:
	ldr r5, [sp, #116]
	cmp r5, #64
	bne .L_081400e4
	movs r0, #139
	bl Audio_PlayCue
.L_081400e4:
	ldr r0, [sp, #116]
	cmp r0, #74
	bne .L_081400f0
	movs r0, #139
	bl Audio_PlayCue
.L_081400f0:
	ldr r1, [sp, #116]
	cmp r1, #84
	bne .L_081400fc
	movs r0, #139
	bl Audio_PlayCue
.L_081400fc:
	ldr r2, [sp, #116]
	cmp r2, #94
	bne .L_08140108
	movs r0, #139
	bl Audio_PlayCue
.L_08140108:
	ldr r3, [sp, #116]
	cmp r3, #124
	bne .L_08140114
	movs r0, #144
	bl Audio_PlayCue
.L_08140114:
	movs r4, #0
	mov r10, r4
	movs r5, #140
.L_0814011a:
	ldr r0, [sp, #116]
	cmp r0, r5
	bne .L_08140126
	movs r0, #104
	bl Audio_PlayCue
.L_08140126:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #4
	cmp r2, #6
	bne .L_0814011a
	ldr r3, [sp, #116]
	cmp r3, #169
	bne .L_0814014a
	movs r0, #145
	bl Audio_PlayCue
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #16
	str r3, [r2]
.L_0814014a:
	ldr r4, [sp, #116]
	cmp r4, #0
	bne .L_0814015a
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_08164b2c
.L_0814015a:
	ldr r5, [sp, #116]
	cmp r5, #32
	bne .L_08140176
	ldr r1, .L_081402e0
	ldr r2, .L_081402e4
	movs r0, #0
	movs r3, #28
	str r0, [sp, #92]
	str r0, [sp, #88]
	str r0, [sp, #84]
	str r1, [sp, #80]
	str r2, [sp, #76]
	str r1, [sp, #72]
	str r3, [sp, #68]
.L_08140176:
	ldr r4, [sp, #116]
	cmp r4, #124
	bne .L_08140198
	ldr r1, .L_081402e8
	movs r5, #248
	movs r0, #128
	movs r2, #0
	lsls r5, r5, #13
	lsls r0, r0, #12
	movs r3, #1
	str r5, [sp, #92]
	str r0, [sp, #88]
	str r1, [sp, #84]
	str r2, [sp, #80]
	str r2, [sp, #76]
	str r2, [sp, #72]
	str r3, [sp, #68]
.L_08140198:
	ldr r4, [sp, #116]
	cmp r4, #128
	bne .L_081401d2
	ldr r3, .L_081402ec
	ldr r5, [sp, #92]
	subs r3, r3, r5
	cmp r3, #0
	bge .L_081401aa
	adds r3, #15
.L_081401aa:
	asrs r3, r3, #4
	ldr r0, [sp, #88]
	str r3, [sp, #80]
	movs r3, #192
	lsls r3, r3, #11
	subs r3, r3, r0
	cmp r3, #0
	bge .L_081401bc
	adds r3, #15
.L_081401bc:
	ldr r1, [sp, #84]
	asrs r3, r3, #4
	str r3, [sp, #76]
	negs r3, r1
	cmp r3, #0
	bge .L_081401ca
	adds r3, #15
.L_081401ca:
	asrs r3, r3, #4
	movs r2, #16
	str r3, [sp, #72]
	str r2, [sp, #68]
.L_081401d2:
	ldr r3, [sp, #116]
	cmp r3, #144
	bne .L_0814020e
	ldr r3, .L_081402f0
	ldr r4, [sp, #92]
	subs r3, r3, r4
	cmp r3, #0
	bge .L_081401e4
	adds r3, #7
.L_081401e4:
	asrs r3, r3, #3
	ldr r5, [sp, #88]
	str r3, [sp, #80]
	movs r3, #208
	lsls r3, r3, #12
	subs r3, r3, r5
	cmp r3, #0
	bge .L_081401f6
	adds r3, #7
.L_081401f6:
	asrs r3, r3, #3
	str r3, [sp, #76]
	ldr r0, [sp, #84]
	ldr r3, .L_081402f4
	subs r3, r3, r0
	cmp r3, #0
	bge .L_08140206
	adds r3, #7
.L_08140206:
	asrs r3, r3, #3
	movs r1, #8
	str r3, [sp, #72]
	str r1, [sp, #68]
.L_0814020e:
	ldr r2, [sp, #116]
	cmp r2, #156
	bne .L_0814024c
	ldr r4, [sp, #92]
	movs r3, #248
	lsls r3, r3, #13
	subs r3, r3, r4
	cmp r3, #0
	bge .L_08140222
	adds r3, #15
.L_08140222:
	asrs r3, r3, #4
	ldr r5, [sp, #88]
	str r3, [sp, #80]
	movs r3, #128
	lsls r3, r3, #12
	subs r3, r3, r5
	cmp r3, #0
	bge .L_08140234
	adds r3, #15
.L_08140234:
	asrs r3, r3, #4
	str r3, [sp, #76]
	ldr r0, [sp, #84]
	ldr r3, .L_081402e8
	subs r3, r3, r0
	cmp r3, #0
	bge .L_08140244
	adds r3, #15
.L_08140244:
	asrs r3, r3, #4
	movs r1, #16
	str r3, [sp, #72]
	str r1, [sp, #68]
.L_0814024c:
	ldr r2, [sp, #68]
	cmp r2, #0
	ble .L_0814027a
	subs r2, #1
	ldr r1, [sp, #84]
	str r2, [sp, #68]
	ldr r2, [sp, #72]
	ldr r3, [sp, #92]
	ldr r4, [sp, #80]
	ldr r5, [sp, #88]
	ldr r0, [sp, #76]
	adds r1, r1, r2
	adds r3, r3, r4
	str r1, [sp, #84]
	adds r5, r5, r0
	str r3, [sp, #92]
	asrs r0, r3, #16
	ldr r3, [sp, #84]
	asrs r1, r5, #16
	asrs r2, r3, #16
	str r5, [sp, #88]
	bl Func_08164b2c
.L_0814027a:
	ldr r4, [sp, #116]
	cmp r4, #124
	bne .L_0814028e
	movs r1, #128
	ldr r3, .L_081402f8
	ldr r0, [sp, #128]
	lsls r1, r1, #7
	ldr r2, .L_081402fc
	mov lr, r3
	.2byte 0xf800
.L_0814028e:
	ldr r5, [sp, #116]
	cmp r5, #123
	bgt .L_0814034e
	movs r0, #58
	movs r7, #128
	movs r6, #140
	mov r8, r0
	lsls r7, r7, #10
	cmp r5, #29
	bgt .L_081402a8
	lsls r3, r5, #2
	subs r6, r6, r3
	b .L_081402b2
.L_081402a8:
	ldr r1, [sp, #116]
	movs r2, #114
	lsls r3, r1, #1
	adds r3, r3, r1
	subs r6, r2, r3
.L_081402b2:
	ldr r2, [sp, #116]
	cmp r2, #7
	ble .L_081402c0
	movs r3, #144
	lsls r2, r2, #11
	lsls r3, r3, #10
	subs r7, r3, r2
.L_081402c0:
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	cmp r7, r3
	bgt .L_081402ce
	movs r7, #128
	lsls r7, r7, #8
.L_081402ce:
	ldr r4, [sp, #116]
	cmp r4, #35
	ble .L_08140300
	movs r5, #20
	mov r8, r5
	b .L_0814031a
	.2byte 0x0000
.L_081402dc:
	.4byte gInput
.L_081402e0:
	.4byte 0xffffdb6e
.L_081402e4:
	.4byte 0xffff924a
.L_081402e8:
	.4byte 0xfffc0000
.L_081402ec:
	.4byte 0xffff0000
.L_081402f0:
	.4byte 0xfff70000
.L_081402f4:
	.4byte 0xfffb0000
.L_081402f8:
	.4byte IwramFillWords
.L_081402fc:
	.4byte 0x3f3f3f3f
.L_08140300:
	ldr r0, [sp, #116]
	cmp r0, #19
	ble .L_0814031a
	ldr r1, .L_08140698
	lsls r0, r0, #10
	adds r0, r0, r1
	bl Trig_Cos
	movs r3, #44
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #14
	mov r8, r3
.L_0814031a:
	cmp r6, #7
	bgt .L_08140320
	movs r6, #8
.L_08140320:
	ldr r2, [sp, #116]
	cmp r2, #67
	ble .L_08140332
	adds r0, r2, #0
	subs r0, #68
	movs r1, #6
	bl Math_Div
	adds r6, r6, r0
.L_08140332:
	movs r5, #224
	lsls r5, r5, #3
	add r5, r11
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_0815b434
	adds r0, r5, #0
	movs r1, #60
	mov r2, r8
	adds r3, r6, #0
	bl Func_0818caa8
.L_0814034e:
	ldr r3, [sp, #116]
	cmp r3, #127
	ble .L_081403aa
	movs r5, #224
	ldr r6, .L_0814069c
	movs r4, #0
	lsls r5, r5, #2
	mov r10, r4
	add r5, r11
.L_08140360:
	ldr r0, [r5, #24]
	cmp r0, #15
	bhi .L_0814039a
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r3, [r5, #8]
	movs r2, #130
	muls r0, r3
	asrs r0, r0, #16
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	lsls r2, r2, #7
	adds r2, #72
	add r1, r11
	adds r1, r1, r2
	ldr r2, [r5]
	ldr r3, [r5, #4]
	subs r2, r2, r0
	lsls r0, r0, #2
	subs r3, r3, r4
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #120]
	ldr r0, [sp, #128]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_0814039a:
	adds r3, r0, #1
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #31
	bne .L_08140360
.L_081403aa:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #64]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_081406a0
	ldr r3, [sp, #160]
	ldr r4, .L_081406a4
	ands r3, r2
	movs r2, #7
	orrs r3, r2
	ldr r2, .L_081406a8
	movs r5, #0
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #160]
	movs r3, #6
	str r3, [r0]
	ldr r2, [sp, #44]
	ldr r3, .L_081406ac
	str r2, [r0, #16]
	str r3, [r0, #8]
	ldr r3, [sp, #64]
	str r5, [r0, #20]
	str r3, [r0, #12]
	str r4, [r2, #4]
	mov r10, r5
	ldr r5, [sp, #116]
	mov r8, r0
	ldr r0, .L_081406b0
	lsls r3, r5, #15
	adds r6, r5, #0
	adds r7, r3, r0
	subs r6, #64
.L_081403f6:
	cmp r6, #15
	bhi .L_08140440
	movs r5, #128
	lsls r5, r5, #12
	bl Func_08014de4
	subs r5, r5, r7
	ldr r0, .L_081406b4
	ldr r1, .L_081406b8
	movs r2, #0
	bl Func_08015160
	lsrs r0, r5, #31
	adds r0, r5, r0
	adds r2, r5, #0
	adds r1, r5, #0
	asrs r0, r0, #1
	bl Func_080151e4
	ldr r3, .L_081406bc
	mov r1, r10
	lsls r5, r1, #1
	ldrh r0, [r3, r5]
	bl Func_08015068
	ldr r3, .L_081406c0
	ldrh r0, [r3, r5]
	bl SceneTransform_ApplyPitch
	ldr r0, .L_081406c4
	ldr r1, [sp, #64]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08140440:
	ldr r2, .L_081406c8
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r7, r7, r2
	subs r6, #10
	cmp r4, #4
	bne .L_081403f6
	ldr r2, [sp, #116]
	subs r2, #124
	cmp r2, #15
	bhi .L_08140492
	lsls r6, r2, #15
	movs r3, #64
	lsls r2, r2, #3
	subs r5, r3, r2
	cmp r5, #0
	ble .L_08140466
	movs r5, #0
.L_08140466:
	mov r0, r8
	str r5, [r0, #20]
	bl Func_08014de4
	ldr r0, .L_081406b4
	ldr r1, .L_081406b8
	movs r2, #0
	bl Func_08015160
	asrs r0, r6, #1
	adds r1, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	ldr r0, .L_081406c4
	ldr r1, [sp, #64]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08140492:
	ldr r6, [sp, #116]
	subs r6, #24
	cmp r6, #99
	bhi .L_08140574
	movs r3, #192
	movs r1, #255
	lsls r2, r6, #11
	lsls r3, r3, #10
	lsls r1, r1, #8
	subs r7, r3, r2
	adds r1, #255
	cmp r7, r1
	bgt .L_081404b0
	movs r7, #128
	lsls r7, r7, #9
.L_081404b0:
	movs r5, #32
	negs r5, r5
	cmp r6, #7
	bgt .L_081404be
	lsls r3, r6, #2
	adds r5, r3, #0
	subs r5, #64
.L_081404be:
	ldr r2, [sp, #116]
	cmp r2, #63
	ble .L_081404ca
	lsls r2, r2, #2
	movs r3, #224
	subs r5, r3, r2
.L_081404ca:
	mov r3, r8
	str r5, [r3, #20]
	ldr r4, [sp, #44]
	ldr r3, .L_081406cc
	lsls r6, r6, #9
	str r3, [r4, #4]
	bl Func_08014de4
	movs r2, #0
	ldr r0, .L_081406b4
	ldr r1, .L_081406b8
	bl Func_08015160
	movs r1, #3
	adds r0, r7, #0
	bl Math_Div
	adds r1, r0, #0
	asrs r0, r7, #1
	mov r10, r0
	mov r2, r10
	adds r0, r7, #0
	bl Func_080151e4
	adds r0, r6, #0
	bl Func_080150e4
	movs r1, #64
	negs r1, r1
	cmp r5, r1
	ble .L_08140518
	ldr r0, .L_081406c4
	ldr r1, [sp, #64]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08140518:
	movs r3, #180
	ldr r2, [sp, #44]
	lsls r3, r3, #6
	adds r3, #72
	add r3, r11
	str r3, [r2, #4]
	movs r3, #6
	strb r3, [r2]
	strb r3, [r2, #1]
	ldr r3, .L_081406d0
	mov r4, r8
	str r3, [r4, #8]
	ldr r5, [sp, #116]
	cmp r5, #63
	ble .L_08140574
	ldr r0, .L_081406d4
	lsls r3, r5, #2
	adds r5, r3, r0
	cmp r5, #0
	ble .L_08140542
	movs r5, #0
.L_08140542:
	mov r1, r8
	str r5, [r1, #20]
	bl Func_08014de4
	ldr r0, .L_081406b4
	ldr r1, .L_081406b8
	movs r2, #0
	bl Func_08015160
	mov r1, r10
	mov r2, r10
	adds r0, r7, #0
	bl Func_080151e4
	adds r0, r6, #0
	bl Func_080150e4
	ldr r0, .L_081406c4
	ldr r1, [sp, #64]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08140574:
	ldr r2, [sp, #116]
	cmp r2, #123
	bgt .L_0814057c
	b .L_08140760
.L_0814057c:
	ldr r4, [sp, #44]
	movs r3, #5
	add r5, sp, #160
	strb r3, [r4]
	movs r3, #2
	str r5, [sp, #44]
	strb r3, [r5, #1]
	movs r3, #244
	lsls r3, r3, #6
	adds r3, #72
	add r3, r11
	str r3, [r5, #4]
	ldr r3, .L_081406d8
	mov r0, r8
	str r3, [r0, #8]
	movs r3, #6
	mov r9, r5
	str r3, [r0]
	movs r5, #16
	negs r5, r5
	movs r1, #0
	movs r7, #128
	str r5, [r0, #20]
	mov r10, r1
	lsls r7, r7, #8
	mov r5, r11
.L_081405b0:
	ldr r1, [r5, #24]
	cmp r1, #0
	blt .L_08140608
	movs r2, #156
	lsls r2, r2, #9
	lsls r6, r1, #13
	adds r2, #128
	cmp r6, r2
	ble .L_081405c8
	movs r6, #156
	lsls r6, r6, #9
	adds r6, #128
.L_081405c8:
	bl Func_08014de4
	ldr r0, .L_081406b4
	ldr r1, .L_081406b8
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	lsls r1, r1, #9
	adds r2, r7, #0
	adds r0, r7, #0
	bl Func_080151e4
	ldr r0, [r5, #8]
	bl Func_080150e4
	movs r0, #236
	lsls r0, r0, #7
	adds r0, #176
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080151e4
	ldr r0, .L_081406dc
	ldr r1, [sp, #64]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
	ldr r1, [r5, #24]
.L_08140608:
	adds r3, r1, #1
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #28
	cmp r4, #24
	bne .L_081405b0
	ldr r5, [sp, #116]
	cmp r5, #163
	ble .L_0814063c
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r3, .L_081406e0
	adds r2, #132
	add r2, r11
	str r3, [r2]
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
.L_0814063c:
	ldr r0, [sp, #116]
	cmp r0, #159
	bgt .L_08140644
	b .L_08140760
.L_08140644:
	ldr r1, .L_081406e4
	movs r6, #192
	lsls r6, r6, #3
	lsls r3, r0, #1
	adds r6, #228
	adds r7, r3, r1
	add r6, r11
	cmp r7, #64
	ble .L_08140658
	movs r7, #64
.L_08140658:
	ldr r2, [sp, #116]
	cmp r2, #160
	bne .L_08140668
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, .L_081406b0
	str r3, [r6, #4]
.L_08140668:
	ldr r3, [sp, #116]
	cmp r3, #171
	bgt .L_081406f8
	ldr r3, [r6]
	ldr r4, .L_081406e8
	movs r5, #128
	adds r3, r3, r4
	str r3, [r6]
	ldr r3, [r6, #4]
	lsls r5, r5, #13
	adds r3, r3, r5
	movs r5, #224
	lsls r5, r5, #3
	add r5, r11
	movs r2, #128
	str r3, [r6, #4]
	adds r0, r5, #0
	adds r1, r7, #0
	lsls r2, r2, #9
	bl Func_0815b434
	movs r0, #2
	ldrsh r1, [r6, r0]
	b .L_081406ec
.L_08140698:
	.4byte 0xffffb000
.L_0814069c:
	.4byte Data_08197438
.L_081406a0:
	.4byte 0xffffff00
.L_081406a4:
	.4byte gMapCellBuffer
.L_081406a8:
	.4byte 0xffff00ff
.L_081406ac:
	.4byte Data_08199364
.L_081406b0:
	.4byte 0xffe00000
.L_081406b4:
	.4byte 0xfffc0000
.L_081406b8:
	.4byte 0xffd40000
.L_081406bc:
	.4byte Data_081976f8
.L_081406c0:
	.4byte Data_081976f0
.L_081406c4:
	.4byte Data_081991e0
.L_081406c8:
	.4byte 0xfffb0000
.L_081406cc:
	.4byte Data_02014000
.L_081406d0:
	.4byte Data_08199340
.L_081406d4:
	.4byte 0xfffffec0
.L_081406d8:
	.4byte Data_08199220
.L_081406dc:
	.4byte Data_081991c0
.L_081406e0:
	.4byte 0x01010101
.L_081406e4:
	.4byte 0xfffffee0
.L_081406e8:
	.4byte 0xfff80000
.L_081406ec:
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r0, r5, #0
	adds r3, r7, #0
	bl Func_0818caa8
.L_081406f8:
	mov r4, r8
	movs r3, #6
	str r3, [r4]
	movs r3, #244
	lsls r3, r3, #6
	movs r5, #0
	adds r3, #136
	str r5, [r4, #20]
	add r3, r11
	mov r5, r9
	str r3, [r5, #4]
	bl Func_08014de4
	ldr r0, [r6]
	ldr r1, .L_08140790
	ldr r2, .L_08140794
	adds r0, r0, r1
	ldr r1, [r6, #4]
	movs r5, #128
	adds r1, r1, r2
	movs r2, #0
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #160
	lsls r0, r0, #8
	bl Func_080150e4
	movs r3, #144
	lsls r3, r3, #6
	adds r3, #184
	movs r1, #128
	adds r0, r7, #0
	muls r0, r3
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_08140798
	ldr r1, [sp, #64]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08140760:
	mov r0, r8
	bl Sys_Free
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	ldr r0, [sp, #64]
	add r5, r11
	bl Sys_Free
	ldr r3, [r5]
	cmp r3, #0
	ble .L_081407a0
	bl Random16
	movs r3, #7
	ldr r2, .L_0814079c
	ands r3, r0
	adds r3, #28
	strh r3, [r2, #6]
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
	b .L_081407ae
.L_08140790:
	.4byte 0xffc10000
.L_08140794:
	.4byte 0xffc00000
.L_08140798:
	.4byte Data_081976e0
.L_0814079c:
	.4byte Data_03001120
.L_081407a0:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #164
	add r2, r11
	ldr r3, .L_081408fc
	ldr r2, [r2]
	strh r2, [r3, #6]
.L_081407ae:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	add r2, r11
.L_081407ba:
	ldr r3, [r2]
	ldr r3, [r2]
	cmp r3, #1
	bls .L_081407ba
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #116]
	adds r3, #1
	str r3, [sp, #116]
	cmp r3, #180
	beq .L_081407e0
	b .L_081400aa
.L_081407e0:
	movs r4, #1
	negs r4, r4
	movs r2, #16
	ldr r0, .L_08140900
	movs r1, #8
	movs r3, #32
	str r4, [sp, #60]
	bl Func_08178680
	movs r2, #0
.L_081407f4:
	lsls r5, r2, #5
	str r5, [sp, #36]
	movs r0, #0
	movs r7, #0
	mov r9, r0
.L_081407fe:
	ldr r3, .L_08140904
	ldrb r6, [r3, r2]
	cmp r7, #15
	ble .L_0814080e
	lsls r3, r7, #1
	adds r3, r6, r3
	adds r6, r3, #0
	subs r6, #32
.L_0814080e:
	ldr r4, [sp, #36]
	ldr r0, .L_08140908
	adds r3, r4, r2
	adds r3, r3, r7
	lsls r3, r3, #3
	adds r5, r3, r0
	mov r4, r9
	movs r3, #126
	subs r4, r3, r4
	lsrs r3, r6, #31
	movs r1, #0
	adds r3, r6, r3
	mov r8, r4
	mov r10, r1
	asrs r4, r3, #1
.L_0814082c:
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #44
	mov r0, r8
	strb r0, [r5]
	adds r0, r7, #0
	muls r0, r3
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Sin
	ldr r3, .L_0814090c
	ldr r2, [sp, #12]
	ldr r1, [sp, #16]
	ldrb r3, [r3, r2]
	ldr r4, [sp, #8]
	muls r3, r0
	asrs r3, r3, #16
	adds r3, r3, r1
	subs r3, r3, r4
	movs r0, #1
	strb r3, [r5, #1]
	add r10, r0
	movs r3, #0
	strb r3, [r5, #2]
	mov r3, r10
	adds r1, r1, r6
	adds r5, #4
	cmp r3, #2
	bne .L_0814082c
	movs r4, #7
	adds r7, #1
	add r9, r4
	cmp r7, #33
	bne .L_081407fe
	adds r2, #1
	cmp r2, #2
	bne .L_081407f4
	movs r3, #239
	movs r2, #238
	lsls r3, r3, #7
	lsls r2, r2, #7
	add r3, r11
	mov r5, r10
	adds r2, #132
	str r5, [r3]
	add r2, r11
	movs r3, #50
	str r3, [r2]
	movs r1, #128
	ldr r0, [sp, #128]
	ldr r6, .L_08140910
	lsls r1, r1, #7
	ldr r2, .L_08140914
	mov lr, r6
	.2byte 0xf800
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	ldr r0, [sp, #128]
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r6
	.2byte 0xf800
	ldr r3, .L_081408f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r1, .L_08140918
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	movs r0, #31
	movs r1, #31
	movs r2, #31
	bl Func_08164abc
	movs r1, #244
	lsls r1, r1, #6
	adds r1, #72
	ldr r0, .L_0814091c
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #244
	lsls r1, r1, #6
	adds r1, #200
	movs r3, #0
	add r1, r11
	b .L_08140920
.L_081408f8:
	.4byte 0x00000786
.L_081408fc:
	.4byte Data_03001120
.L_08140900:
	.4byte Data_02012000
.L_08140904:
	.4byte Data_08197702
.L_08140908:
	.4byte gMapCellBuffer
.L_0814090c:
	.4byte Data_08197700
.L_08140910:
	.4byte IwramFillWords
.L_08140914:
	.4byte 0x3f3f3f3f
.L_08140918:
	.4byte 0x00000045
.L_0814091c:
	.4byte 0x000000ca
.L_08140920:
	movs r2, #0
	ldr r0, .L_08140aac
	bl Resource_LoadAndDecompress
	bl Func_0815b410
	ldr r0, .L_08140ab0
	bl Resource_GetTableEntry
	movs r1, #32
	adds r5, r0, #0
	ldr r2, .L_08140ab4
	ldr r0, .L_08140ab8
	mov lr, r6
	.2byte 0xf800
	movs r1, #224
	adds r5, #32
	lsls r1, r1, #3
	movs r6, #238
	add r1, r11
	adds r0, r5, #0
	lsls r6, r6, #7
	bl Resource_DecodeType01
	adds r6, #220
	movs r0, #0
	movs r7, #0
	mov r9, r0
	add r6, r11
	mov r8, r0
.L_0814095c:
	lsls r3, r7, #12
	movs r2, #224
	movs r1, #0
	add r3, r11
	lsls r2, r2, #3
	mov r10, r1
	adds r5, r3, r2
.L_0814096a:
	movs r2, #128
	movs r3, #240
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b290
	mov r3, r8
	movs r4, #238
	add r3, r10
	lsls r4, r4, #7
	adds r4, #220
	lsls r3, r3, #2
	adds r3, r3, r4
	mov r1, r11
	str r0, [r1, r3]
	movs r4, #13
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	ldr r0, .L_08140abc
	lsls r3, r3, #2
	adds r3, r3, r0
	ldrh r0, [r3, #2]
	ldr r1, .L_08140ac0
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #2
	adds r1, r5, #0
	ldr r3, .L_08140ac4
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	movs r4, #128
	add r10, r0
	lsls r4, r4, #2
	mov r1, r10
	adds r5, r5, r4
	cmp r1, #8
	bne .L_0814096a
	movs r2, #128
	movs r3, #240
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b3b0
	mov r5, r9
	ldr r1, [r6]
	movs r2, #24
	ldr r3, .L_08140ac4
	str r0, [r6, #32]
	mov lr, r3
	.2byte 0xf800
	adds r2, r5, r7
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r0, #238
	add r3, r11
	lsls r2, r2, #2
	lsls r0, r0, #7
	movs r4, #0
	adds r1, r3, #0
	add r2, r11
	adds r0, #220
	mov r10, r4
	adds r1, #24
	adds r2, r2, r0
.L_08140a02:
	ldmia r2!, {r3}
	ldrh r3, [r3, #8]
	lsls r3, r3, #22
	lsrs r3, r3, #22
	str r3, [r1]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r1, #28
	cmp r4, #8
	bne .L_08140a02
	movs r5, #8
	movs r0, #9
	adds r7, #1
	add r9, r5
	adds r6, #36
	add r8, r0
	cmp r7, #2
	bne .L_0814095c
	movs r0, #1
	bl WaitFrames
	mov r2, sp
	mov r3, sp
	mov r4, sp
	movs r1, #0
	adds r2, #152
	adds r3, #168
	adds r4, #144
	str r1, [sp, #116]
	str r2, [sp, #48]
	str r3, [sp, #40]
	str r4, [sp, #52]
	str r1, [sp, #24]
.L_08140a46:
	ldr r3, .L_08140ac8
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08140a86
	ldr r5, [sp, #116]
	cmp r5, #84
	bgt .L_08140a86
	ldr r0, [sp, #60]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_08140a86
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #4
	str r3, [r2]
	movs r2, #238
	ldr r3, .L_08140acc
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	str r3, [r2]
	movs r2, #17
	str r2, [sp, #60]
	ldr r3, .L_08140aa8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
.L_08140a86:
	ldr r3, [sp, #116]
	cmp r3, #17
	bgt .L_08140b4a
	ldr r4, [sp, #24]
	movs r2, #31
	subs r1, r2, r4
	adds r3, r4, r3
	adds r0, r1, #0
	subs r2, r2, r3
	cmp r1, #0
	bge .L_08140a9e
	movs r0, #0
.L_08140a9e:
	cmp r1, #5
	bgt .L_08140ad0
	movs r1, #6
	b .L_08140ad0
	.2byte 0x0000
.L_08140aa8:
	.4byte 0x00000784
.L_08140aac:
	.4byte 0x00000104
.L_08140ab0:
	.4byte 0x000000a0
.L_08140ab4:
	.4byte 0x7fff7fff
.L_08140ab8:
	.4byte 0x050003e0
.L_08140abc:
	.4byte ResourceTableEntries
.L_08140ac0:
	.4byte 0x06010000
.L_08140ac4:
	.4byte IwramCopyWords
.L_08140ac8:
	.4byte gInput
.L_08140acc:
	.4byte 0x04040404
.L_08140ad0:
	movs r5, #18
	negs r5, r5
	cmp r2, r5
	bge .L_08140adc
	movs r2, #18
	negs r2, r2
.L_08140adc:
	bl Func_08164b2c
	ldr r0, [sp, #116]
	cmp r0, #16
	bgt .L_08140b4a
	ldr r0, .L_08140b24
	bl Resource_GetTableEntry
	ldr r2, [sp, #24]
	ldr r5, .L_08140b28
	ldr r6, .L_08140b20
	movs r1, #0
	movs r3, #32
	mov r10, r1
	subs r4, r3, r2
.L_08140afa:
	ldrh r3, [r0]
	movs r2, #31
	ands r2, r3
	lsls r3, r3, #16
	adds r1, r2, r4
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r6
	ands r3, r6
	adds r2, r2, r4
	adds r3, r3, r4
	cmp r1, #31
	ble .L_08140b16
	movs r1, #31
.L_08140b16:
	cmp r2, #31
	ble .L_08140b2c
	movs r2, #31
	b .L_08140b2c
	.2byte 0x0000
.L_08140b20:
	.4byte 0x0000001f
.L_08140b24:
	.4byte 0x000000a0
.L_08140b28:
	.4byte 0x050003e0
.L_08140b2c:
	cmp r3, #31
	ble .L_08140b32
	movs r3, #31
.L_08140b32:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	strh r3, [r5]
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r0, #2
	adds r5, #2
	cmp r1, #16
	bne .L_08140afa
.L_08140b4a:
	ldr r2, [sp, #116]
	cmp r2, #0
	bne .L_08140b5a
	movs r2, #128
	ldr r3, .L_08140b88
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
.L_08140b5a:
	ldr r3, [sp, #116]
	cmp r3, #75
	bne .L_08140b6a
	movs r2, #128
	ldr r3, .L_08140b8c
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
.L_08140b6a:
	ldr r4, [sp, #116]
	cmp r4, #85
	bne .L_08140b96
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #4
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r3, .L_08140b90
	adds r2, #132
	add r2, r11
	b .L_08140b94
	.2byte 0x0000
.L_08140b88:
	.4byte 0x00000786
.L_08140b8c:
	.4byte 0x00000784
.L_08140b90:
	.4byte 0x04040404
.L_08140b94:
	str r3, [r2]
.L_08140b96:
	movs r0, #132
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #56]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08140ea0
	ldr r3, [sp, #152]
	ldr r5, [sp, #48]
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08140ea4
	movs r1, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	str r3, [sp, #152]
	movs r3, #244
	lsls r3, r3, #6
	adds r3, #72
	add r3, r11
	str r3, [r5, #4]
	ldr r3, .L_08140ea8
	str r1, [r0, #20]
	str r3, [r0, #8]
	str r5, [r0, #16]
	ldr r2, [sp, #56]
	movs r3, #6
	str r2, [r0, #12]
	str r3, [r0]
	mov r9, r0
	mov r10, r1
.L_08140bde:
	ldr r3, .L_08140eac
	mov r4, r10
	ldrb r3, [r3, r4]
	ldr r0, [sp, #116]
	subs r5, r0, r3
	cmp r5, #0
	bne .L_08140c18
	cmp r4, #5
	bne .L_08140bf8
	movs r0, #145
	bl Audio_PlayCue
	b .L_08140bfe
.L_08140bf8:
	movs r0, #144
	bl Audio_PlayCue
.L_08140bfe:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	movs r2, #8
	str r2, [r3]
	movs r1, #128
	ldr r3, .L_08140eb0
	ldr r0, [sp, #128]
	lsls r1, r1, #7
	ldr r2, .L_08140eb4
	mov lr, r3
	.2byte 0xf800
.L_08140c18:
	cmp r5, #15
	bhi .L_08140cb0
	lsls r0, r5, #11
	bl Trig_Sin
	ldr r3, .L_08140eb8
	mov r1, r10
	ldrb r3, [r3, r1]
	muls r3, r0
	asrs r3, r3, #16
	adds r7, r3, #2
	cmp r7, #1
	ble .L_08140cb0
	ldr r3, .L_08140ebc
	movs r0, #224
	ldrb r3, [r3, r1]
	lsls r0, r0, #3
	mov r8, r3
	lsls r3, r5, #3
	add r8, r3
	ldr r3, .L_08140ec0
	movs r2, #128
	ldrb r6, [r3, r1]
	lsls r2, r2, #9
	add r0, r11
	adds r1, r7, #0
	bl Func_0815b434
	movs r1, #19
	movs r0, #188
	bl Func_081963ec
	lsrs r5, r7, #31
	adds r5, r7, r5
	asrs r5, r5, #1
	str r5, [sp, #0]
	str r7, [sp, #4]
	movs r0, #192
	subs r6, r6, r7
	lsls r0, r0, #18
	movs r1, #224
	mov r3, r8
	adds r6, #8
	adds r0, #188
	lsls r1, r1, #3
	subs r2, r3, r5
	ldr r4, [r0]
	adds r3, r6, #0
	ldr r0, [sp, #128]
	add r1, r11
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	movs r1, #192
	str r5, [sp, #0]
	str r7, [sp, #4]
	lsls r1, r1, #18
	adds r1, #188
	ldr r4, [r1]
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, [sp, #128]
	add r1, r11
	mov r2, r8
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_08140cb0:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #6
	bne .L_08140bde
	movs r5, #128
	lsls r5, r5, #8
	mov r8, r5
	movs r7, #224
	movs r5, #252
	movs r4, #0
	lsls r5, r5, #1
	lsls r7, r7, #3
	mov r10, r4
	add r5, r11
	add r7, r11
.L_08140cd0:
	ldr r3, .L_08140ec4
	mov r0, r10
	ldrb r3, [r3, r0]
	ldr r1, [sp, #116]
	lsrs r2, r3, #1
	cmp r1, #0
	bne .L_08140cea
	ldr r3, .L_08140ec8
	str r1, [r5, #4]
	ldrb r3, [r3, r0]
	str r1, [r5, #24]
	lsls r3, r3, #16
	str r3, [r5]
.L_08140cea:
	ldr r3, [sp, #116]
	cmp r3, r2
	blt .L_08140da2
	ldr r4, [sp, #116]
	adds r3, r2, #0
	adds r3, #32
	cmp r4, r3
	bge .L_08140da2
	ldr r3, .L_08140ecc
	mov r0, r10
	ldrb r6, [r3, r0]
	ldr r3, .L_08140ed0
	ldr r1, [r5, #4]
	ldrb r3, [r3, r0]
	lsls r3, r3, #16
	cmp r1, r3
	bge .L_08140d24
	ldr r3, .L_08140ed4
	ldrb r2, [r3, r0]
	ldr r3, [r5]
	lsls r2, r2, #16
	subs r3, r3, r2
	str r3, [r5]
	ldr r3, .L_08140ed8
	ldrb r3, [r3, r0]
	lsls r3, r3, #16
	adds r3, r1, r3
	str r3, [r5, #4]
	b .L_08140d2a
.L_08140d24:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08140d2a:
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r5]
	bl Func_08014de4
	ldr r3, .L_08140edc
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	lsls r1, r1, #9
	mov r0, r8
	mov r2, r8
	bl Func_080151e4
	ldr r2, .L_08140ee0
	mov r4, r10
	lsls r3, r4, #2
	ldr r0, [r2, r3]
	bl Func_080150e4
	movs r3, #144
	lsls r3, r3, #6
	adds r3, #184
	movs r1, #128
	adds r0, r6, #0
	muls r0, r3
	lsls r1, r1, #10
	mov r2, r8
	bl Func_080151e4
	ldr r0, .L_08140ee4
	ldr r1, [sp, #56]
	movs r2, #4
	bl Func_08196958
	movs r2, #128
	adds r0, r7, #0
	adds r1, r6, #0
	lsls r2, r2, #9
	bl Func_0815b434
	movs r0, #2
	ldrsh r1, [r5, r0]
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r0, r7, #0
	subs r1, #1
	adds r3, r6, #0
	bl Func_0818caa8
	mov r0, r9
	bl Func_08196a7c
.L_08140da2:
	movs r4, #1
	add r10, r4
	mov r0, r10
	adds r5, #28
	cmp r0, #4
	bne .L_08140cd0
	ldr r1, [sp, #48]
	movs r5, #6
	add r3, sp, #152
	strb r5, [r1]
	str r3, [sp, #48]
	ldr r2, [sp, #48]
	movs r3, #5
	strb r3, [r2, #1]
	movs r3, #244
	lsls r3, r3, #6
	adds r3, #200
	add r3, r11
	str r3, [r2, #4]
	ldr r0, .L_08140ee8
	movs r3, #32
	movs r1, #2
	movs r2, #32
	bl Func_08178680
	ldr r3, .L_08140ee8
	mov r4, r9
	str r3, [r4, #8]
	movs r3, #64
	strb r3, [r4, #24]
	ldr r6, .L_08140eec
	str r5, [r4]
	movs r5, #0
	mov r10, r5
	movs r7, #127
.L_08140de8:
	movs r3, #46
	mov r0, r10
	muls r0, r3
	ldr r1, [sp, #116]
	adds r3, r0, #0
	subs r3, r1, r3
	adds r5, r3, #0
	subs r5, #12
	cmp r5, #45
	bls .L_08140dfe
	b .L_08140f12
.L_08140dfe:
	mov r2, r10
	cmp r2, #0
	bne .L_08140e24
	lsls r2, r5, #1
	adds r3, r2, #0
	adds r3, #63
	adds r4, r7, #0
	bics r4, r3
	adds r3, r4, #0
	mov r0, r9
	strb r3, [r0, #24]
	cmp r5, #32
	bgt .L_08140e48
	ldr r1, .L_08140ee8
	adds r3, r2, r5
	lsls r3, r3, #3
	adds r3, r3, r1
	mov r2, r10
	b .L_08140e42
.L_08140e24:
	lsls r3, r5, #2
	adds r3, #63
	adds r4, r7, #0
	bics r4, r3
	adds r3, r4, #0
	mov r0, r9
	movs r2, #0
	strb r3, [r0, #24]
	cmp r5, #15
	bgt .L_08140e48
	lsls r3, r5, #1
	ldr r1, .L_08140ee8
	adds r3, r3, r5
	lsls r3, r3, #4
	adds r3, r3, r1
.L_08140e42:
	strb r2, [r3]
	strb r2, [r3, #1]
	strb r2, [r3, #2]
.L_08140e48:
	bl Func_08014de4
	ldr r0, .L_08140ef0
	cmp r5, #0
	blt .L_08140e56
	lsls r3, r5, #16
	adds r0, r3, r0
.L_08140e56:
	mov r2, r10
	cmp r2, #0
	bne .L_08140e68
	cmp r5, #31
	ble .L_08140e76
	lsls r3, r5, #3
	subs r3, r3, r5
	subs r3, #224
	b .L_08140e72
.L_08140e68:
	cmp r5, #15
	ble .L_08140e76
	lsls r3, r5, #3
	subs r3, r3, r5
	subs r3, #112
.L_08140e72:
	lsls r3, r3, #16
	adds r0, r0, r3
.L_08140e76:
	ldr r1, .L_08140ef4
	movs r2, #0
	bl Func_08015160
	movs r0, #177
	lsls r0, r0, #8
	movs r1, #128
	movs r2, #128
	adds r0, #224
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Func_080151e4
	mov r3, r10
	cmp r3, #0
	bne .L_08140efc
	ldr r0, .L_08140ef8
	bl Func_080150e4
	b .L_08140f02
	.2byte 0x0000
.L_08140ea0:
	.4byte 0xffffff00
.L_08140ea4:
	.4byte 0xffff00ff
.L_08140ea8:
	.4byte Data_08199220
.L_08140eac:
	.4byte Data_08197704
.L_08140eb0:
	.4byte IwramFillWords
.L_08140eb4:
	.4byte 0x3f3f3f3f
.L_08140eb8:
	.4byte Data_0819770a
.L_08140ebc:
	.4byte Data_08197710
.L_08140ec0:
	.4byte Data_08197716
.L_08140ec4:
	.4byte Data_0819771c
.L_08140ec8:
	.4byte Data_08197724
.L_08140ecc:
	.4byte Data_08197720
.L_08140ed0:
	.4byte Data_08197730
.L_08140ed4:
	.4byte Data_08197728
.L_08140ed8:
	.4byte Data_0819772c
.L_08140edc:
	.4byte 0xffc00000
.L_08140ee0:
	.4byte Data_08197734
.L_08140ee4:
	.4byte Data_081976e0
.L_08140ee8:
	.4byte Data_02012000
.L_08140eec:
	.4byte gMapCellBuffer
.L_08140ef0:
	.4byte 0xffe80000
.L_08140ef4:
	.4byte 0xfff00000
.L_08140ef8:
	.4byte 0xfffff418
.L_08140efc:
	ldr r0, .L_08141098
	bl Func_080150e4
.L_08140f02:
	adds r0, r6, #0
	ldr r1, [sp, #56]
	movs r2, #66
	bl Func_08196958
	mov r0, r9
	bl Func_08196a7c
.L_08140f12:
	movs r5, #1
	movs r4, #132
	add r10, r5
	lsls r4, r4, #1
	mov r0, r10
	adds r6, r6, r4
	cmp r0, #2
	beq .L_08140f24
	b .L_08140de8
.L_08140f24:
	mov r0, r9
	bl Sys_Free
	ldr r0, [sp, #56]
	bl Sys_Free
	ldr r3, .L_0814109c
	ldr r2, [sp, #40]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r1, #0
	str r3, [sp, #144]
	str r4, [sp, #148]
	movs r3, #255
	lsls r3, r3, #16
	str r1, [r2, #12]
	str r3, [r2, #4]
	ldr r3, [sp, #100]
	movs r4, #192
	ldr r5, .L_081410a0
	lsls r4, r4, #13
	adds r3, r3, r4
	str r3, [sp, #100]
	cmp r3, r5
	ble .L_08140f76
	ldr r2, [sp, #96]
	adds r2, #7
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08140f64
	ldr r3, [sp, #96]
	adds r3, #14
.L_08140f64:
	ldr r0, [sp, #100]
	ldr r1, .L_081410a4
	asrs r3, r3, #3
	str r3, [sp, #96]
	lsls r3, r3, #3
	subs r2, r2, r3
	adds r0, r0, r1
	str r2, [sp, #96]
	str r0, [sp, #100]
.L_08140f76:
	ldr r2, .L_081410a8
	movs r4, #255
	ldrh r3, [r2, #4]
	lsls r4, r4, #8
	adds r4, #248
	adds r3, r3, r4
	strh r3, [r2, #4]
	ldr r5, [sp, #40]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #252
	movs r7, #0
	mov r8, r5
	movs r4, #0
	add r6, r11
.L_08140f94:
	movs r2, #238
	lsls r3, r4, #2
	lsls r2, r2, #7
	add r3, r11
	adds r2, #220
	ldr r5, [sp, #96]
	adds r2, r3, r2
	str r2, [sp, #20]
	movs r0, #0
	lsls r1, r7, #21
	mov r10, r0
	mov r9, r1
	adds r5, #64
.L_08140fae:
	ldr r1, [sp, #100]
	mov r0, r10
	lsls r3, r0, #21
	adds r3, r3, r1
	mov r2, r8
	str r3, [r2]
	movs r3, #242
	lsls r3, r3, #15
	add r3, r9
	str r3, [r2, #8]
	cmp r0, #8
	bne .L_08140fde
	ldr r3, [sp, #96]
	ldr r1, [r6]
	adds r3, #71
	adds r2, r3, #0
	cmp r3, #0
	bge .L_08140fd6
	ldr r2, [sp, #96]
	adds r2, #78
.L_08140fd6:
	asrs r2, r2, #3
	lsls r2, r2, #3
	subs r2, r3, r2
	b .L_08141004
.L_08140fde:
	mov r1, r10
	movs r2, #238
	adds r3, r4, r1
	lsls r2, r2, #7
	adds r2, #220
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r0, r11
	ldr r1, [r0, r3]
	ldr r3, [sp, #96]
	adds r2, r5, #0
	add r3, r10
	cmp r5, #0
	bge .L_08140ffe
	adds r2, r3, #0
	adds r2, #71
.L_08140ffe:
	asrs r2, r2, #3
	lsls r2, r2, #3
	subs r2, r5, r2
.L_08141004:
	adds r2, r4, r2
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #24
	mov r0, r11
	ldr r2, [r0, r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r2, r3
	ldr r0, .L_08141098
	ldrh r3, [r1, #8]
	ands r3, r0
	orrs r3, r2
	strh r3, [r1, #8]
	ldr r1, [sp, #40]
	ldr r2, .L_081410ac
	ldr r3, [r1]
	cmp r3, r2
	ble .L_08141034
	ldr r0, .L_081410b0
	adds r3, r3, r0
	str r3, [r1]
.L_08141034:
	ldr r2, [sp, #20]
	movs r3, #0
	ldmia r2!, {r0}
	str r4, [sp, #8]
	adds r1, r2, #0
	str r1, [sp, #20]
	ldr r2, [sp, #52]
	ldr r1, [sp, #40]
	bl Render_ApplyProjectedPlacementFar
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #1
	ldr r4, [sp, #8]
	cmp r0, #9
	bne .L_08140fae
	adds r7, #1
	adds r4, #9
	adds r6, #36
	cmp r7, #2
	bne .L_08140f94
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	add r2, r11
.L_0814106c:
	ldr r3, [r2]
	ldr r3, [r2]
	cmp r3, #1
	bls .L_0814106c
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	add r5, r11
	ldr r3, [r5]
	cmp r3, #0
	ble .L_081410b4
	bl Random16
	movs r3, #7
	ldr r2, .L_081410a8
	ands r3, r0
	adds r3, #28
	strh r3, [r2, #6]
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
	b .L_081410c2
.L_08141098:
	.4byte 0xfffffc00
.L_0814109c:
	.4byte Data_08196dd8
.L_081410a0:
	.4byte 0x001fffff
.L_081410a4:
	.4byte 0xffe00000
.L_081410a8:
	.4byte Data_03001120
.L_081410ac:
	.4byte 0x00ffffff
.L_081410b0:
	.4byte 0xfee00000
.L_081410b4:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #164
	add r2, r11
	ldr r3, .L_081411e0
	ldr r2, [r2]
	strh r2, [r3, #6]
.L_081410c2:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #24]
	ldr r2, [sp, #116]
	adds r1, #2
	adds r2, #1
	str r1, [sp, #24]
	str r2, [sp, #116]
	cmp r2, #102
	beq .L_081410f6
	ldr r3, [sp, #60]
	cmp r3, #0
	bgt .L_081410ec
	b .L_08140a46
.L_081410ec:
	subs r3, #1
	str r3, [sp, #60]
	cmp r3, #0
	beq .L_081410f6
	b .L_08140a46
.L_081410f6:
	add r4, sp, #104
	ldr r1, .L_081411e0
	ldrh r4, [r4]
	movs r3, #0
	strh r4, [r1, #4]
	ldr r5, [sp, #108]
	movs r0, #206
	str r3, [r5, #16]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	lsls r0, r0, #3
	adds r3, r3, r0
	movs r2, #32
	strh r2, [r1, #6]
	movs r0, #1
	ldrh r1, [r3]
	movs r2, #24
	bl Func_08118040
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_081411e4
	movs r3, #120
	movs r0, #195
	movs r5, #238
	str r3, [r2, #16]
	lsls r0, r0, #1
	lsls r5, r5, #7
	bl Audio_PlayCue
	adds r5, #220
	movs r1, #0
	mov r10, r1
	add r5, r11
.L_0814113e:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #18
	bne .L_0814113e
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081411e8
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081411ec
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #3
	str r3, [r2]
	movs r2, #238
	ldr r3, .L_081411f0
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	str r3, [r2]
	movs r1, #128
	ldr r0, [sp, #128]
	ldr r3, .L_081411f4
	lsls r1, r1, #7
	ldr r2, .L_081411f8
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_081411d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r4, [sp, #132]
	ldr r2, .L_081411d8
	ldr r3, [r4, #20]
	adds r5, r4, #0
	lsls r3, r3, #1
	adds r3, #36
	adds r0, r5, #0
	strh r2, [r5, r3]
	adds r0, #36
	movs r1, #0
	bl BattleActor_SpawnObjectsForListFar
	movs r1, #253
	lsls r1, r1, #6
	ldr r0, .L_081411fc
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_081411dc
	lsls r2, r2, #19
	b .L_08141200
.L_081411d4:
	.4byte 0x00000784
.L_081411d8:
	.4byte 0x000000ff
.L_081411dc:
	.4byte 0x00001010
.L_081411e0:
	.4byte Data_03001120
.L_081411e4:
	.4byte gCameraSceneParameters
.L_081411e8:
	.4byte 0x05000200
.L_081411ec:
	.4byte 0x050001e8
.L_081411f0:
	.4byte Data_02020202
.L_081411f4:
	.4byte IwramFillWords
.L_081411f8:
	.4byte 0x3f3f3f3f
.L_081411fc:
	.4byte 0x000000ca
.L_08141200:
	adds r2, #82
	strh r3, [r2]
	movs r0, #0
	movs r1, #224
	mov r10, r0
	lsls r1, r1, #3
.L_0814120c:
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r2, r3, #1
	cmp r2, #63
	ble .L_0814121a
	movs r2, #63
.L_0814121a:
	mov r3, r10
	add r3, r11
	movs r7, #0
	adds r3, r3, r1
.L_08141222:
	adds r7, #1
	strb r2, [r3]
	adds r3, #120
	cmp r7, #120
	bne .L_08141222
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #120
	bne .L_0814120c
	ldr r0, .L_081414fc
	ldr r1, .L_08141500
	movs r5, #0
	movs r2, #80
	str r5, [sp, #116]
	str r0, [sp, #32]
	str r1, [sp, #28]
	negs r2, r2
	mov r9, r2
.L_08141248:
	ldr r3, [sp, #116]
	cmp r3, #0
	bne .L_0814127e
	ldr r5, [sp, #132]
	movs r4, #0
	ldr r3, [r5, #20]
	mov r10, r4
	cmp r3, #0
	beq .L_0814127e
	movs r5, #36
	movs r6, #16
.L_0814125e:
	ldr r1, [sp, #132]
	mov r3, r10
	ldrsh r0, [r5, r1]
	movs r2, #1
	movs r1, #0
	negs r2, r2
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r4, [sp, #132]
	movs r3, #1
	add r10, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r10, r3
	bne .L_0814125e
.L_0814127e:
	ldr r5, [sp, #116]
	cmp r5, #127
	bne .L_0814128a
	movs r0, #134
	bl Func_081180e8
.L_0814128a:
	ldr r0, [sp, #116]
	cmp r0, #107
	bne .L_0814129e
	movs r1, #128
	ldr r3, .L_08141504
	ldr r0, [sp, #128]
	lsls r1, r1, #7
	ldr r2, .L_08141508
	mov lr, r3
	.2byte 0xf800
.L_0814129e:
	ldr r1, [sp, #116]
	cmp r1, #107
	ble .L_081412c4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #3
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r3, .L_0814150c
	adds r2, #132
	add r2, r11
	str r3, [r2]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_08164b2c
.L_081412c4:
	ldr r2, [sp, #116]
	cmp r2, #32
	bne .L_081412d0
	movs r0, #140
	bl Audio_PlayCue
.L_081412d0:
	ldr r3, [sp, #116]
	cmp r3, #48
	bne .L_081412e8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #90
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
.L_081412e8:
	ldr r4, [sp, #116]
	cmp r4, #72
	bne .L_081412f4
	movs r0, #208
	bl Audio_PlayCue
.L_081412f4:
	ldr r5, [sp, #116]
	cmp r5, #109
	ble .L_081412fc
	b .L_08141484
.L_081412fc:
	cmp r5, #31
	ble .L_08141326
	adds r1, r5, #0
	subs r1, #32
	cmp r1, #0
	bge .L_0814130a
	adds r1, #3
.L_0814130a:
	ldr r3, [sp, #116]
	asrs r0, r1, #2
	movs r2, #0
	cmp r3, #47
	ble .L_0814131c
	subs r3, #48
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r2, r3, #1
.L_0814131c:
	lsrs r1, r1, #31
	adds r1, r0, r1
	asrs r1, r1, #1
	bl Func_08164abc
.L_08141326:
	mov r4, r9
	cmp r4, #31
	bhi .L_0814134a
	ldr r3, [sp, #28]
	ldr r0, [sp, #32]
	ldr r2, .L_08141510
	movs r5, #0
	orrs r3, r0
	mov r10, r5
	orrs r3, r4
.L_0814133a:
	movs r1, #1
	add r10, r1
	mov r4, r10
	strh r3, [r2]
	adds r2, #2
	cmp r4, #223
	bne .L_0814133a
	b .L_0814135e
.L_0814134a:
	ldr r5, [sp, #116]
	cmp r5, #47
	ble .L_08141380
	ldr r1, .L_08141514
	movs r2, #240
	adds r0, r1, #0
	lsls r2, r2, #8
	movs r3, #224
	bl ColorBuffer_ScaleFar
.L_0814135e:
	ldr r0, [sp, #116]
	cmp r0, #47
	ble .L_08141380
	lsls r3, r0, #1
	movs r2, #216
	movs r1, #224
	subs r2, r2, r3
	lsls r1, r1, #3
	movs r3, #120
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #128]
	add r1, r11
	movs r3, #0
	ldr r4, [sp, #120]
	mov lr, r4
	.2byte 0xf800
.L_08141380:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08141518
	ldr r3, [sp, #136]
	movs r5, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0814151c
	mov r1, r10
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	str r3, [sp, #136]
	movs r3, #253
	lsls r3, r3, #6
	add r2, sp, #136
	add r3, r11
	str r3, [r2, #4]
	ldr r3, .L_08141520
	str r5, [r0, #20]
	str r3, [r0, #8]
	movs r3, #6
	str r2, [r0, #16]
	str r1, [r0, #12]
	str r3, [r0]
	ldr r2, [sp, #116]
	mov r8, r0
	cmp r2, #31
	ble .L_08141478
	movs r6, #192
	lsls r3, r2, #1
	lsls r6, r6, #3
	adds r7, r3, #0
	adds r6, #228
	subs r7, #32
	add r6, r11
	cmp r7, #64
	ble .L_081413dc
	movs r7, #64
.L_081413dc:
	ldr r3, [sp, #116]
	cmp r3, #32
	bne .L_081413ec
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, .L_08141524
	str r3, [r6, #4]
.L_081413ec:
	ldr r4, [sp, #116]
	cmp r4, #43
	bgt .L_08141426
	ldr r3, [r6]
	ldr r5, .L_08141528
	movs r0, #128
	adds r3, r3, r5
	str r3, [r6]
	ldr r3, [r6, #4]
	movs r5, #255
	lsls r0, r0, #13
	lsls r5, r5, #6
	adds r3, r3, r0
	add r5, r11
	movs r2, #128
	str r3, [r6, #4]
	lsls r2, r2, #9
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_0815b434
	movs r2, #2
	ldrsh r1, [r6, r2]
	adds r0, r5, #0
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r3, r7, #0
	bl Func_0818caa8
.L_08141426:
	bl Func_08014de4
	ldr r5, .L_0814152c
	ldr r4, .L_08141530
	ldr r0, [r6]
	ldr r1, [r6, #4]
	adds r0, r0, r4
	adds r1, r1, r5
	movs r2, #0
	movs r5, #128
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #160
	lsls r0, r0, #8
	bl Func_080150e4
	movs r3, #144
	lsls r3, r3, #6
	adds r3, #184
	movs r1, #128
	adds r0, r7, #0
	muls r0, r3
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_08141534
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08141478:
	mov r0, r8
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
.L_08141484:
	ldr r0, [sp, #116]
	movs r1, #8
	cmp r0, #130
	ble .L_08141490
	movs r1, #2
	b .L_081414a2
.L_08141490:
	ldr r2, [sp, #116]
	cmp r2, #120
	ble .L_0814149a
	movs r1, #4
	b .L_081414a2
.L_0814149a:
	ldr r3, [sp, #116]
	cmp r3, #110
	ble .L_081414a2
	movs r1, #6
.L_081414a2:
	adds r0, r1, #0
	bl Func_08158ce0
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #32]
	ldr r5, [sp, #28]
	ldr r2, [sp, #116]
	movs r0, #128
	lsls r0, r0, #3
	adds r4, #32
	adds r5, r5, r0
	movs r1, #1
	adds r2, #1
	str r4, [sp, #32]
	str r5, [sp, #28]
	add r9, r1
	str r2, [sp, #116]
	cmp r2, #140
	beq .L_081414da
	b .L_08141248
.L_081414da:
	bl Func_08014c4c
	ldr r0, .L_08141538
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #196
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081414fc:
	.4byte 0xfffff600
.L_08141500:
	.4byte 0xfffec000
.L_08141504:
	.4byte IwramFillWords
.L_08141508:
	.4byte 0x3f3f3f3f
.L_0814150c:
	.4byte Data_02020202
.L_08141510:
	.4byte 0x05000202
.L_08141514:
	.4byte 0x05000200
.L_08141518:
	.4byte 0xffffff00
.L_0814151c:
	.4byte 0xffff00ff
.L_08141520:
	.4byte Data_08199220
.L_08141524:
	.4byte 0xffe00000
.L_08141528:
	.4byte 0xfff80000
.L_0814152c:
	.4byte 0xffc00000
.L_08141530:
	.4byte 0xffc10000
.L_08141534:
	.4byte Data_081976e0
.L_08141538:
	.4byte Func_08143000
