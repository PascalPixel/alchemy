.syntax unified
	.thumb
	.global Func_0818df5c
	.thumb_func
Func_0818df5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #208
	str r0, [sp, #104]
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #96]
	movs r6, #240
	str r1, [sp, #100]
	mov r8, r0
	ldr r3, [r0, #92]
	lsls r6, r6, #7
	str r3, [sp, #96]
	adds r6, #240
	ldr r4, [r0, #100]
	ldr r0, [sp, #104]
	adds r7, r3, r6
	ldr r2, .L_0818dfec
	str r4, [sp, #84]
	str r0, [r7]
	movs r0, #128
	lsls r0, r0, #6
	mov r11, r2
	bl Func_081435e0
	movs r2, #128
	movs r1, #128
	lsls r2, r2, #19
	lsls r1, r1, #1
	adds r2, #32
	strh r1, [r2]
	bl Func_0813ba50
	ldr r2, .L_0818dfe8
	movs r3, #160
	movs r6, #160
	lsls r3, r3, #19
	lsls r6, r6, #19
	adds r3, #2
	strh r2, [r6]
	strh r2, [r3]
	ldr r4, [sp, #96]
	movs r0, #239
	ldr r5, .L_0818dff0
	lsls r0, r0, #7
	adds r3, r4, r0
	movs r1, #0
	str r1, [r3]
	mov r9, r1
	movs r1, #200
	adds r0, r5, #0
	lsls r1, r1, #4
	bl Func_080145a8
	movs r1, #0
	movs r0, #0
	bl Func_08163c2c
	adds r0, r5, #0
	bl Func_08014644
	ldr r2, [sp, #96]
	movs r3, #224
	b .L_0818dff4
	.2byte 0x0000
.L_0818dfe8:
	.4byte 0x00000000
.L_0818dfec:
	.4byte gMapCellBuffer
.L_0818dff0:
	.4byte Func_08143000
.L_0818dff4:
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0818e0c0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r4, [sp, #96]
	movs r2, #156
	lsls r2, r2, #6
	adds r1, r4, r2
	ldr r0, .L_0818e0c4
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r4, #128
	ldr r3, [sp, #96]
	lsls r4, r4, #4
	adds r4, #194
	adds r1, r3, r4
	ldr r0, [sp, #84]
	ldr r5, .L_0818e0c8
	movs r2, #30
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0818e0cc
	bl Resource_GetTableEntry
	movs r2, #128
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	bl Func_0815b410
	ldr r2, .L_0818e0d0
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_0818e0d4
	movs r1, #128
	mov r10, r6
	mov r0, r11
	lsls r1, r1, #8
	ldr r2, .L_0818e0d8
	mov lr, r10
	.2byte 0xf800
	ldr r0, .L_0818e0dc
	movs r2, #240
	mov r1, r11
	lsls r2, r2, #7
	mov lr, r5
	.2byte 0xf800
	ldr r0, [r7]
	bl Func_0814cc4c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #138
	movs r0, #1
	movs r2, #1
	bl Func_08152404
	movs r6, #128
	movs r2, #128
	ldr r3, .L_0818e0b8
	lsls r6, r6, #19
	movs r1, #128
	lsls r2, r2, #19
	adds r6, #80
	mov r0, r9
	lsls r1, r1, #1
	adds r2, #32
	strh r0, [r6]
	strh r1, [r2]
	subs r2, #22
	strh r3, [r2]
	ldr r3, .L_0818e0bc
	adds r2, #2
	strh r3, [r2]
	ldr r0, [sp, #100]
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r10
	.2byte 0xf800
	ldr r0, .L_0818e0e0
	movs r2, #128
	ldr r1, [sp, #100]
	lsls r2, r2, #7
	mov lr, r5
	.2byte 0xf800
	movs r1, #200
	b .L_0818e0e4
	.2byte 0x0000
.L_0818e0b8:
	.4byte 0x00001f81
.L_0818e0bc:
	.4byte 0x00002787
.L_0818e0c0:
	.4byte 0x000000af
.L_0818e0c4:
	.4byte 0x000000cd
.L_0818e0c8:
	.4byte IwramCopyWords
.L_0818e0cc:
	.4byte 0x00000148
.L_0818e0d0:
	.4byte gCameraSceneParameters
.L_0818e0d4:
	.4byte IwramFillWords
.L_0818e0d8:
	.4byte 0x01010101
.L_0818e0dc:
	.4byte 0x06008000
.L_0818e0e0:
	.4byte 0x06004000
.L_0818e0e4:
	lsls r1, r1, #4
	ldr r0, .L_0818e130
	bl Func_080145a8
	ldr r3, .L_0818e128
	movs r2, #128
	strh r3, [r6]
	ldr r3, .L_0818e12c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #96]
	movs r4, #188
	movs r6, #204
	movs r0, #212
	lsls r4, r4, #7
	lsls r6, r6, #7
	lsls r0, r0, #7
	adds r4, #64
	adds r6, #64
	adds r0, #64
	adds r4, r3, r4
	adds r6, r3, r6
	adds r0, r3, r0
	str r0, [sp, #72]
	str r4, [sp, #80]
	str r6, [sp, #76]
	movs r1, #176
	add r8, r1
	mov r2, r8
	movs r4, #224
	ldr r2, [r2]
	lsls r4, r4, #4
	b .L_0818e134
.L_0818e128:
	.4byte 0x00003f46
.L_0818e12c:
	.4byte 0x00001010
.L_0818e130:
	.4byte Func_08143174
.L_0818e134:
	adds r4, #164
	movs r0, #128
	adds r4, r3, r4
	lsls r0, r0, #9
	movs r3, #2
	movs r6, #0
	str r2, [sp, #68]
	str r0, [sp, #40]
	str r3, [sp, #0]
	ldr r0, [sp, #72]
	movs r1, #2
	movs r2, #16
	movs r3, #32
	str r4, [sp, #64]
	str r6, [sp, #60]
	str r6, [sp, #56]
	str r6, [sp, #52]
	str r6, [sp, #48]
	str r6, [sp, #44]
	bl Func_0818de3c
	mov r11, r6
	mov r10, r6
.L_0818e162:
	mov r0, r10
	bl Trig_Cos
	negs r0, r0
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r3, r3, #16
	mov r0, r10
	mov r8, r3
	bl Trig_Sin
	lsls r3, r0, #1
	ldr r6, [sp, #76]
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r7, r3, #16
	movs r4, #0
	add r6, r9
.L_0818e188:
	lsls r5, r4, #13
	adds r0, r5, #0
	str r4, [sp, #8]
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	mov r1, r8
	asrs r3, r3, #16
	strb r3, [r6]
	strb r1, [r6, #1]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r4, [sp, #8]
	asrs r3, r3, #16
	adds r4, #1
	strb r3, [r6, #2]
	adds r6, #4
	cmp r4, #2
	bne .L_0818e188
	movs r2, #128
	movs r6, #1
	lsls r2, r2, #3
	add r11, r6
	adds r2, #68
	movs r3, #8
	mov r0, r11
	add r10, r2
	add r9, r3
	cmp r0, #16
	bne .L_0818e162
	movs r1, #8
	movs r2, #7
	movs r3, #3
	movs r0, #104
	str r6, [sp, #0]
	str r4, [sp, #8]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r1, [sp, #96]
	str r3, [sp, #88]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r5, #3
	str r5, [r2]
	ldr r0, [sp, #96]
	adds r3, #4
	adds r1, r0, r3
	ldr r3, .L_0818e290
	ldr r4, [sp, #8]
	str r3, [r1]
	movs r3, #50
	str r4, [r2]
	str r3, [r1]
	movs r1, #192
	lsls r1, r1, #4
	ldr r0, .L_0818e294
	adds r1, #254
	bl Func_080145a8
	ldr r2, .L_0818e298
	movs r3, #0
	strh r3, [r2, #4]
	ldr r4, [sp, #68]
	movs r7, #0
	str r6, [r4, #16]
	ldr r6, [sp, #64]
	ldr r0, [sp, #64]
	adds r6, #12
	mov r10, r6
	mov r8, r0
.L_0818e224:
	movs r5, #255
	ands r5, r7
	lsls r5, r5, #7
	adds r0, r5, #0
	bl Trig_Sin
	movs r6, #160
	adds r3, r0, #0
	muls r3, r6
	mov r1, r8
	str r3, [r1, #4]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r0, #0
	muls r3, r6
	asrs r6, r3, #16
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	mov r2, r8
	str r3, [r2]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	mov r4, r8
	str r3, [r4, #8]
	movs r3, #31
	ands r3, r7
	cmp r3, #0
	bne .L_0818e29c
	adds r0, r7, #0
	cmp r7, #0
	bge .L_0818e280
	adds r0, #31
.L_0818e280:
	asrs r0, r0, #5
	movs r1, #7
	bl Math_Mod
	mov r6, r10
	str r0, [r6]
	b .L_0818e2a2
	.2byte 0x0000
.L_0818e290:
	.4byte 0x04040404
.L_0818e294:
	.4byte Func_0818ddfc
.L_0818e298:
	.4byte Data_03001120
.L_0818e29c:
	movs r3, #7
	mov r0, r10
	str r3, [r0]
.L_0818e2a2:
	movs r2, #128
	movs r1, #16
	adds r7, #1
	lsls r2, r2, #1
	add r10, r1
	add r8, r1
	cmp r7, r2
	bne .L_0818e224
	ldr r1, .L_0818e364
	movs r3, #0
	mov r11, r3
	ldr r3, [r1, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0818e2c4
	b .L_0818e7fe
.L_0818e2c4:
	ldr r3, [r1, #12]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0818e2d0
	b .L_0818e7fe
.L_0818e2d0:
	mov r4, sp
	mov r6, sp
	adds r4, #124
	adds r6, #156
	str r4, [sp, #20]
	str r6, [sp, #16]
.L_0818e2dc:
	mov r0, r11
	cmp r0, #0
	bne .L_0818e314
	movs r1, #136
	lsls r1, r1, #17
	ldr r0, [sp, #96]
	str r1, [sp, #52]
	movs r1, #238
	movs r2, #176
	movs r3, #128
	lsls r1, r1, #7
	lsls r2, r2, #15
	lsls r3, r3, #3
	movs r4, #208
	movs r6, #148
	adds r1, #140
	str r2, [sp, #48]
	str r3, [sp, #44]
	adds r2, r0, r1
	lsls r4, r4, #4
	lsls r6, r6, #6
	movs r3, #8
	str r4, [sp, #60]
	str r6, [sp, #56]
	movs r0, #162
	str r3, [r2]
	bl Audio_PlayCue
.L_0818e314:
	mov r2, r11
	cmp r2, #120
	bne .L_0818e320
	movs r0, #163
	bl Audio_PlayCue
.L_0818e320:
	mov r1, r11
	subs r1, #96
	cmp r1, #16
	bhi .L_0818e338
	ldr r2, .L_0818e358
	movs r3, #128
	subs r2, r2, r1
	ldr r1, .L_0818e35c
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_0818e338:
	mov r3, r11
	cmp r3, #115
	bne .L_0818e348
	movs r2, #128
	ldr r3, .L_0818e360
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0818e348:
	mov r4, r11
	cmp r4, #116
	bne .L_0818e3e6
	ldr r0, .L_0818e368
	bl Func_08014644
	movs r1, #200
	b .L_0818e36c
.L_0818e358:
	.4byte 0x00000010
.L_0818e35c:
	.4byte 0x00001000
.L_0818e360:
	.4byte 0x00001010
.L_0818e364:
	.4byte gInput
.L_0818e368:
	.4byte Func_08143174
.L_0818e36c:
	lsls r1, r1, #4
	ldr r0, .L_0818e470
	bl Func_080145a8
	ldr r1, .L_0818e474
	ldr r6, .L_0818e478
	ldrh r3, [r6]
	adds r0, r3, #0
	movs r2, #130
	ldr r3, .L_0818e478
	lsls r2, r2, #2
	strh r2, [r3]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0818e3a8
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #128
	stmia r3!, {r2}
	lsls r2, r2, #19
	adds r2, #32
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0818e3a8:
	ldr r4, .L_0818e478
	strh r0, [r4]
	ldrh r3, [r4]
	adds r0, r3, #0
	movs r6, #130
	ldr r2, .L_0818e478
	lsls r6, r6, #2
	strh r6, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0818e3e2
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #224
	adds r3, r3, r1
	lsls r2, r2, #3
	adds r3, #4
	adds r2, #132
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0818e3e2:
	ldr r3, .L_0818e478
	strh r0, [r3]
.L_0818e3e6:
	mov r4, r11
	cmp r4, #128
	ble .L_0818e3f2
	ldr r0, .L_0818e47c
	bl Func_0815f0a0
.L_0818e3f2:
	mov r6, r11
	cmp r6, #117
	bne .L_0818e420
	ldr r0, [sp, #96]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #140
	adds r2, r0, r1
	movs r3, #45
	str r3, [r2]
	movs r2, #1
	adds r3, r0, #0
	movs r7, #0
	negs r2, r2
	adds r3, #24
.L_0818e410:
	adds r7, #1
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_0818e410
	movs r2, #128
	lsls r2, r2, #9
	str r2, [sp, #40]
.L_0818e420:
	movs r3, #168
	lsls r3, r3, #3
	adds r3, #255
	cmp r11, r3
	bgt .L_0818e4d2
	ldr r4, [sp, #96]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #140
	adds r1, r4, r2
	ldr r2, [r1]
	movs r6, #236
	lsrs r3, r2, #31
	lsls r6, r6, #7
	adds r3, r2, r3
	asrs r3, r3, #1
	adds r6, #64
	adds r0, r4, r6
	adds r4, r3, #0
	mov r3, r11
	adds r4, #16
	cmp r3, #79
	ble .L_0818e45a
	adds r3, r2, #1
	str r3, [r1]
	cmp r3, #236
	ble .L_0818e45a
	movs r3, #236
	str r3, [r1]
.L_0818e45a:
	ldr r3, .L_0818e46c
	movs r7, #0
.L_0818e45e:
	adds r7, #1
	strh r3, [r0]
	adds r0, #2
	cmp r7, #15
	bne .L_0818e45e
	b .L_0818e480
	.2byte 0x0000
.L_0818e46c:
	.4byte 0x00000000
.L_0818e470:
	.4byte Func_08143000
.L_0818e474:
	.4byte Data_020038e0
.L_0818e478:
	.4byte 0x04000208
.L_0818e47c:
	.4byte 0x00000148
.L_0818e480:
	adds r1, r7, #0
	subs r1, #16
	adds r3, r1, #0
	cmp r1, #0
	bge .L_0818e48e
	adds r3, r7, #0
	subs r3, #13
.L_0818e48e:
	asrs r3, r3, #2
	adds r2, r3, r4
	adds r3, r2, #0
	adds r1, r2, #0
	subs r3, #32
	subs r1, #80
	cmp r3, #0
	bge .L_0818e4a0
	movs r3, #0
.L_0818e4a0:
	cmp r3, #31
	ble .L_0818e4a6
	movs r3, #31
.L_0818e4a6:
	cmp r1, #0
	bge .L_0818e4ac
	movs r1, #0
.L_0818e4ac:
	cmp r1, #31
	ble .L_0818e4b2
	movs r1, #31
.L_0818e4b2:
	lsls r2, r1, #5
	lsls r3, r3, #10
	orrs r3, r2
	asrs r2, r1, #1
	orrs r3, r2
	adds r7, #1
	strh r3, [r0]
	adds r0, #2
	cmp r7, #135
	bne .L_0818e480
	ldr r3, .L_0818e4f8
.L_0818e4c8:
	adds r7, #1
	strh r3, [r0]
	adds r0, #2
	cmp r7, #160
	bne .L_0818e4c8
.L_0818e4d2:
	mov r4, r11
	cmp r4, #115
	ble .L_0818e5c8
	movs r0, #1
	bl Func_081969f8
	adds r5, r0, #0
	ldr r0, .L_0818e4fc
	mov r6, r11
	lsls r3, r6, #2
	adds r1, r3, r0
	cmp r1, #0
	ble .L_0818e4ee
	movs r1, #0
.L_0818e4ee:
	ldr r3, [sp, #132]
	ldr r2, .L_0818e500
	movs r4, #156
	ands r3, r2
	b .L_0818e504
.L_0818e4f8:
	.4byte 0x00000000
.L_0818e4fc:
	.4byte 0xfffffdf0
.L_0818e500:
	.4byte 0xffffff00
.L_0818e504:
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0818e820
	lsls r4, r4, #6
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, [sp, #96]
	str r3, [sp, #132]
	adds r3, r2, r4
	add r2, sp, #132
	str r3, [r2, #4]
	movs r3, #6
	str r3, [r5]
	str r2, [r5, #16]
	ldr r6, [sp, #72]
	movs r3, #0
	str r6, [r5, #8]
	ldr r0, [sp, #80]
	str r1, [r5, #20]
	mov r1, r11
	str r0, [r5, #12]
	strb r3, [r5, #24]
	cmp r1, #127
	ble .L_0818e55c
	ldr r2, .L_0818e824
	movs r4, #192
	lsls r3, r1, #6
	lsls r4, r4, #2
	adds r3, r3, r2
	adds r4, #255
	cmp r3, r4
	ble .L_0818e54c
	movs r3, #128
	lsls r3, r3, #3
.L_0818e54c:
	ldr r6, [sp, #40]
	adds r6, r6, r3
	adds r0, r6, #0
	adds r1, r0, #0
	adds r2, r1, #0
	str r6, [sp, #40]
	bl Func_080151e4
.L_0818e55c:
	ldr r0, .L_0818e828
	mov r1, r11
	movs r7, #0
	mov r8, r0
	lsls r6, r1, #1
.L_0818e566:
	movs r3, #127
	bics r3, r6
	strb r3, [r5, #25]
	bl Func_08014de4
	ldr r0, .L_0818e82c
	ldr r1, .L_0818e830
	movs r2, #0
	bl Func_08015160
	movs r0, #164
	lsls r0, r0, #8
	movs r2, #128
	adds r0, #16
	ldr r1, .L_0818e834
	lsls r2, r2, #9
	bl Func_080151e4
	ldr r0, [sp, #40]
	adds r7, #1
	adds r1, r0, #0
	adds r2, r1, #0
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #8
	bl Func_080150e4
	mov r0, r8
	bl Func_08015068
	movs r2, #32
	ldr r0, [sp, #76]
	ldr r1, [sp, #80]
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	movs r2, #192
	lsls r2, r2, #5
	add r8, r2
	adds r6, #20
	cmp r7, #5
	bne .L_0818e566
	adds r0, r5, #0
	bl Sys_Free
	b .L_0818e6f8
.L_0818e5c8:
	mov r3, r11
	cmp r3, #159
	bgt .L_0818e5da
	ldr r4, [sp, #60]
	movs r6, #128
	lsls r3, r4, #16
	lsls r6, r6, #17
	adds r3, r3, r6
	b .L_0818e5f2
.L_0818e5da:
	mov r0, r11
	cmp r0, #191
	bgt .L_0818e5f6
	ldr r1, [sp, #60]
	lsls r2, r0, #3
	lsls r3, r1, #16
	lsrs r3, r3, #16
	subs r3, r3, r2
	movs r2, #152
	lsls r2, r2, #3
	adds r3, r3, r2
	lsls r3, r3, #16
.L_0818e5f2:
	asrs r3, r3, #16
	str r3, [sp, #60]
.L_0818e5f6:
	mov r3, r11
	cmp r3, #47
	bgt .L_0818e608
	ldr r4, [sp, #56]
	movs r6, #128
	lsls r3, r4, #16
	lsls r6, r6, #17
	adds r3, r3, r6
	b .L_0818e620
.L_0818e608:
	mov r0, r11
	cmp r0, #111
	bgt .L_0818e624
	ldr r1, [sp, #56]
	lsls r2, r0, #2
	lsls r3, r1, #16
	lsrs r3, r3, #16
	subs r3, r3, r2
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #16
.L_0818e620:
	asrs r3, r3, #16
	str r3, [sp, #56]
.L_0818e624:
	add r5, sp, #196
	movs r3, #120
	str r3, [r5]
	movs r3, #64
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	bl Func_08014de4
	adds r0, r5, #0
	bl Func_08015128
	ldr r3, [sp, #60]
	mov r6, r11
	lsls r0, r3, #16
	lsrs r0, r0, #16
	bl Func_08015024
	ldr r4, [sp, #56]
	lsls r0, r4, #16
	lsrs r0, r0, #16
	bl Func_08015068
	cmp r6, #112
	bgt .L_0818e6ec
	add r0, sp, #184
	movs r1, #172
	ldr r6, [sp, #64]
	mov r8, r0
	add r1, sp
	movs r7, #0
	mov r10, r1
	mov r9, r8
.L_0818e666:
	movs r2, #2
	ldrsh r3, [r6, r2]
	mov r4, r10
	ldr r5, [r6, #12]
	str r3, [r4]
	movs r0, #6
	ldrsh r3, [r6, r0]
	mov r0, r10
	str r3, [r4, #4]
	movs r1, #10
	ldrsh r3, [r6, r1]
	mov r1, r9
	str r3, [r4, #8]
	bl Func_08015778
	mov r2, r9
	ldr r3, [r2, #8]
	cmp r3, #0
	ble .L_0818e6de
	cmp r5, #7
	bne .L_0818e6b4
	movs r1, #30
	adds r0, r7, #0
	bl Math_Mod
	ldr r3, [sp, #84]
	mov r4, r8
	adds r1, r0, #0
	movs r0, #1
	ldr r2, [r4]
	adds r1, r3, r1
	ldr r3, [r4, #4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #88]
	ldr r0, .L_0818e838
	mov lr, r4
	.2byte 0xf800
	b .L_0818e6de
.L_0818e6b4:
	ldr r2, .L_0818e83c
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r0, [sp, #96]
	movs r2, #224
	adds r1, r0, r1
	ldr r0, .L_0818e840
	lsls r2, r2, #3
	ldrb r0, [r0, r5]
	mov r3, r8
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r3, [r3, #4]
	str r0, [sp, #0]
	ldr r0, .L_0818e844
	ldr r4, [sp, #88]
	ldrb r0, [r0, r5]
	str r0, [sp, #4]
	ldr r0, .L_0818e838
	mov lr, r4
	.2byte 0xf800
.L_0818e6de:
	movs r0, #128
	adds r7, #1
	lsls r0, r0, #1
	adds r6, #16
	cmp r7, r0
	bne .L_0818e666
	b .L_0818e6f8
.L_0818e6ec:
	movs r1, #240
	ldr r3, .L_0818e848
	ldr r0, .L_0818e838
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
.L_0818e6f8:
	ldr r1, [sp, #44]
	ldr r2, [sp, #20]
	movs r3, #0
	str r1, [r2, #4]
	ldr r4, [sp, #16]
	str r1, [sp, #124]
	str r3, [r4, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r4, #4]
	ldr r6, [sp, #52]
	movs r2, #238
	str r6, [r4]
	ldr r0, [sp, #48]
	lsls r2, r2, #7
	str r0, [r4, #8]
	ldr r1, [sp, #96]
	adds r2, #220
	adds r3, r1, r2
	ldr r0, [r3]
	ldr r1, [sp, #16]
	movs r3, #0
	ldr r2, [sp, #20]
	bl Func_08020010
	movs r5, #128
	movs r6, #192
	mov r3, r11
	lsls r5, r5, #4
	lsls r6, r6, #7
	cmp r3, #23
	ble .L_0818e752
	lsls r3, r3, #3
	add r3, r11
	lsls r3, r3, #4
	mov r4, r11
	subs r3, r3, r4
	lsls r3, r3, #1
	movs r0, #143
	subs r3, r6, r3
	lsls r0, r0, #5
	adds r6, r3, r0
	cmp r6, #0
	bge .L_0818e752
	movs r6, #0
.L_0818e752:
	mov r1, r11
	cmp r1, #87
	ble .L_0818e768
	lsls r3, r1, #6
	movs r2, #176
	subs r3, r5, r3
	lsls r2, r2, #5
	adds r5, r3, r2
	cmp r5, #0
	bge .L_0818e768
	movs r5, #0
.L_0818e768:
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	ldr r4, [sp, #52]
	asrs r3, r3, #10
	subs r4, r4, r3
	adds r0, r6, #0
	str r4, [sp, #52]
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	ldr r6, [sp, #48]
	ldr r0, [sp, #44]
	movs r1, #128
	asrs r3, r3, #10
	lsls r1, r1, #2
	adds r6, r6, r3
	adds r0, r0, r1
	mov r2, r11
	str r6, [sp, #48]
	str r0, [sp, #44]
	cmp r2, #127
	ble .L_0818e7be
	ldr r4, .L_0818e84c
	ldr r6, .L_0818e850
	lsls r3, r2, #13
	adds r5, r3, r4
	cmp r5, r6
	ble .L_0818e7ac
	movs r5, #128
	lsls r5, r5, #10
.L_0818e7ac:
	ldr r0, [sp, #48]
	ldr r1, [sp, #44]
	movs r2, #128
	lsls r2, r2, #2
	adds r0, r0, r5
	adds r1, r1, r2
	str r0, [sp, #48]
	str r1, [sp, #44]
	b .L_0818e7ca
.L_0818e7be:
	ldr r3, [sp, #44]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	ble .L_0818e7ca
	str r4, [sp, #44]
.L_0818e7ca:
	ldr r6, [sp, #96]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r6, r0
	movs r5, #1
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r11, r1
	mov r2, r11
	cmp r2, #160
	beq .L_0818e7fe
	ldr r2, .L_0818e854
	ldr r3, [r2, #12]
	ands r3, r5
	cmp r3, #0
	bne .L_0818e7fe
	ldr r3, [r2, #12]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0818e7fe
	b .L_0818e2dc
.L_0818e7fe:
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #68]
	movs r3, #0
	str r3, [r4, #16]
	ldr r0, .L_0818e858
	mov r6, r11
	bl Func_08014644
	cmp r6, #115
	bgt .L_0818e860
	ldr r0, .L_0818e85c
	bl Func_08014644
	b .L_0818e866
	.2byte 0x0000
.L_0818e820:
	.4byte 0xffff00ff
.L_0818e824:
	.4byte 0xffffe000
.L_0818e828:
	.4byte 0xffffc000
.L_0818e82c:
	.4byte 0xfffe0000
.L_0818e830:
	.4byte 0xffc00000
.L_0818e834:
	.4byte 0x00024650
.L_0818e838:
	.4byte gMapCellBuffer
.L_0818e83c:
	.4byte Data_08199e7e
.L_0818e840:
	.4byte Data_08199e70
.L_0818e844:
	.4byte Data_08199e77
.L_0818e848:
	.4byte IwramClearWords
.L_0818e84c:
	.4byte 0xfff00000
.L_0818e850:
	.4byte 0x0001ffff
.L_0818e854:
	.4byte gInput
.L_0818e858:
	.4byte Func_0818ddfc
.L_0818e85c:
	.4byte Func_08143174
.L_0818e860:
	ldr r0, .L_0818e8bc
	bl Func_08014644
.L_0818e866:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_0814cca8
	ldr r3, .L_0818e8a8
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #0
	movs r0, #200
	adds r2, #32
	str r0, [sp, #36]
	str r1, [sp, #32]
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	str r1, [r3]
	ldr r3, .L_0818e8c0
	adds r2, #12
	str r3, [r2]
	ldr r3, .L_0818e8ac
	adds r2, #38
	strh r3, [r2]
	ldr r3, .L_0818e8b0
	subs r2, #70
	strh r3, [r2]
	ldr r3, .L_0818e8b4
	adds r2, #60
	strh r3, [r2]
	ldr r3, .L_0818e8b8
	b .L_0818e8c4
	.2byte 0x0000
.L_0818e8a8:
	.4byte 0x00000080
.L_0818e8ac:
	.4byte 0x00001010
.L_0818e8b0:
	.4byte 0x00000784
.L_0818e8b4:
	.4byte 0x00002737
.L_0818e8b8:
	.4byte 0x00000721
.L_0818e8bc:
	.4byte Func_08143000
.L_0818e8c0:
	.4byte 0xfffff000
.L_0818e8c4:
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_0818e904
	subs r2, #6
	strh r3, [r2]
	ldr r3, .L_0818e908
	adds r2, #2
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r4, #239
	str r3, [sp, #88]
	ldr r3, [sp, #96]
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r6, [sp, #96]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r6, r0
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	b .L_0818e90c
	.2byte 0x0000
.L_0818e904:
	.4byte 0x0000107c
.L_0818e908:
	.4byte 0x00000088
.L_0818e90c:
	lsls r1, r1, #4
	ldr r0, .L_0818ebac
	bl Func_080145a8
	ldr r0, .L_0818ebb0
	ldr r1, [sp, #84]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r6, r2
	ldr r0, .L_0818ebb4
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r3, #142
	lsls r3, r3, #7
	adds r1, r6, r3
	ldr r0, .L_0818ebb8
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r4, #174
	lsls r4, r4, #7
	adds r1, r6, r4
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0818ebbc
	bl Func_08157cf4
	ldr r0, .L_0818ebc0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818ebc4
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0818ebc8
	movs r2, #128
	movs r7, #0
	movs r1, #0
	lsls r2, r2, #1
.L_0818e96e:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_0818e96e
	movs r1, #4
	movs r2, #12
	movs r0, #5
	negs r1, r1
	negs r2, r2
	bl Func_08164abc
	movs r0, #108
	movs r6, #0
	add r0, sp
	mov r11, r6
	mov r9, r0
.L_0818e990:
	mov r1, r11
	cmp r1, #0
	bne .L_0818e99e
	movs r2, #144
	movs r3, #0
	str r2, [sp, #36]
	str r3, [sp, #32]
.L_0818e99e:
	mov r4, r11
	cmp r4, #33
	bgt .L_0818e9f0
	ldr r3, .L_0818ebcc
	add r1, sp, #140
	ldr r4, [r3, #4]
	ldr r3, [r3]
	mov r2, r11
	str r3, [sp, #116]
	str r4, [sp, #120]
	movs r3, #0
	str r3, [r1, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r1, #4]
	ldr r6, [sp, #36]
	lsls r3, r6, #16
	str r3, [r1]
	ldr r0, [sp, #32]
	lsls r3, r0, #16
	str r3, [r1, #8]
	cmp r2, #15
	bgt .L_0818e9d6
	subs r6, #1
	adds r0, #9
	str r6, [sp, #36]
	str r0, [sp, #32]
	b .L_0818e9dc
.L_0818e9d6:
	ldr r3, [sp, #32]
	adds r3, #2
	str r3, [sp, #32]
.L_0818e9dc:
	ldr r4, [sp, #96]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #220
	adds r3, r4, r6
	ldr r0, [r3]
	add r2, sp, #116
	movs r3, #0
	bl Func_08020010
.L_0818e9f0:
	mov r0, r11
	cmp r0, #34
	bne .L_0818e9fc
	movs r0, #134
	bl Func_08118088 + 0x60
.L_0818e9fc:
	mov r1, r11
	cmp r1, #16
	bne .L_0818eac6
	ldr r4, [sp, #36]
	movs r2, #128
	lsrs r3, r4, #31
	lsls r2, r2, #1
	adds r3, r4, r3
	mov r8, r2
	asrs r3, r3, #1
	ldr r2, .L_0818ebd0
	lsls r3, r3, #16
	movs r7, #0
	mov r10, r3
.L_0818ea18:
	str r2, [sp, #12]
	bl Random16
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	ldr r2, [sp, #12]
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #7
	movs r3, #228
	mov r1, r10
	lsls r3, r3, #15
	adds r5, r5, r0
	str r1, [r2]
	str r3, [r2, #4]
	adds r0, r5, #0
	bl Trig_Sin
	add r6, r8
	adds r3, r6, #0
	muls r3, r0
	ldr r2, [sp, #12]
	asrs r3, r3, #6
	str r3, [r2, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	ldr r2, [sp, #12]
	lsls r3, r3, #1
	asrs r3, r3, #6
	str r3, [r2, #16]
	bl Random16
	movs r3, #7
	ldr r2, [sp, #12]
	ands r0, r3
	adds r0, #24
	adds r7, #1
	str r0, [r2, #24]
	adds r2, #28
	cmp r7, r8
	bne .L_0818ea18
	ldr r4, [sp, #96]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #168
	adds r3, r4, r6
	movs r2, #4
	str r2, [r3]
	movs r0, #145
	bl Audio_PlayCue
	ldr r0, [sp, #104]
	movs r7, #0
	ldr r3, [r0, #20]
	cmp r3, #0
	beq .L_0818eac6
	movs r5, #36
.L_0818ea9e:
	ldr r1, [sp, #104]
	ldrsh r0, [r5, r1]
	movs r1, #4
	bl Func_08118088
	ldr r3, [sp, #104]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r7, #0
	bl Func_0814cd48
	ldr r6, [sp, #104]
	adds r7, #1
	ldr r3, [r6, #20]
	adds r5, #2
	cmp r7, r3
	bne .L_0818ea9e
.L_0818eac6:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #28]
	movs r0, #1
	bl Func_081969f8
	mov r2, r9
	str r2, [r0, #16]
	ldr r3, [sp, #28]
	movs r1, #0
	str r3, [r0, #12]
	add r4, sp, #108
	movs r3, #6
	str r1, [r0, #20]
	strb r3, [r4]
	strb r3, [r2, #1]
	ldr r6, [sp, #96]
	mov r8, r0
	movs r0, #142
	lsls r0, r0, #7
	adds r3, r6, r0
	str r3, [r2, #4]
	ldr r3, .L_0818ebd4
	mov r2, r8
	movs r1, #7
	str r3, [r2, #8]
	str r1, [r2]
	ldr r4, [sp, #36]
	mov r6, r11
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r3, #60
	str r3, [sp, #24]
	lsls r3, r6, #11
	negs r0, r3
	movs r7, #0
	mov r9, r0
	mov r10, r3
.L_0818eb18:
	ldr r3, .L_0818ebd8
	ldrb r3, [r3, r7]
	adds r2, r3, #0
	adds r2, #16
	cmp r11, r2
	ble .L_0818ebfa
	mov r1, r11
	subs r3, r2, r1
	lsls r3, r3, #3
	adds r1, r3, #0
	movs r3, #16
	adds r1, #56
	negs r3, r3
	cmp r1, r3
	ble .L_0818eb3a
	movs r1, #16
	negs r1, r1
.L_0818eb3a:
	movs r4, #64
	negs r4, r4
	cmp r1, r4
	ble .L_0818ebfa
	ldr r3, .L_0818ebdc
	mov r6, r11
	ldrb r3, [r3, r7]
	subs r2, r6, r2
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r0, #167
	adds r3, r3, r2
	lsls r0, r0, #8
	mov r2, r8
	lsls r6, r3, #4
	adds r0, #16
	str r1, [r2, #20]
	adds r5, r6, r0
	bl Func_08014de4
	ldr r3, [sp, #24]
	movs r1, #160
	lsls r0, r3, #16
	lsls r1, r1, #14
	movs r2, #0
	bl Func_08015160
	ldr r3, .L_0818ebe0
	adds r2, r5, #0
	ldrb r3, [r3, r7]
	adds r1, r3, #0
	muls r1, r5
	cmp r5, #0
	bge .L_0818eb8a
	movs r4, #167
	lsls r4, r4, #8
	adds r4, #19
	adds r2, r6, r4
.L_0818eb8a:
	asrs r2, r2, #2
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl Func_08015024
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_0818ebe4
	mov r0, r10
	bl Func_08015068
	b .L_0818ebea
	.2byte 0x0000
.L_0818ebac:
	.4byte Func_08143000
.L_0818ebb0:
	.4byte 0x00000134
.L_0818ebb4:
	.4byte 0x0000013e
.L_0818ebb8:
	.4byte 0x000000da
.L_0818ebbc:
	.4byte 0x000000c1
.L_0818ebc0:
	.4byte 0x00000148
.L_0818ebc4:
	.4byte IwramCopyWords
.L_0818ebc8:
	.4byte Data_02010018
.L_0818ebcc:
	.4byte Data_08196f20
.L_0818ebd0:
	.4byte gMapCellBuffer
.L_0818ebd4:
	.4byte Data_081990d0
.L_0818ebd8:
	.4byte Data_08199e8c
.L_0818ebdc:
	.4byte Data_08199e90
.L_0818ebe0:
	.4byte Data_08199e94
.L_0818ebe4:
	mov r0, r9
	bl Func_08015068
.L_0818ebea:
	ldr r0, .L_0818ed80
	ldr r1, [sp, #28]
	movs r2, #32
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_0818ebfa:
	movs r3, #128
	lsls r3, r3, #6
	adds r7, #1
	add r9, r3
	add r10, r3
	cmp r7, #4
	bne .L_0818eb18
	ldr r3, .L_0818ed84
	mov r6, r8
	str r3, [r6, #8]
	ldr r0, [sp, #96]
	movs r1, #174
	lsls r1, r1, #7
	adds r3, r0, r1
	str r3, [sp, #112]
	movs r2, #7
	add r3, sp, #108
	strb r2, [r3]
	mov r9, r3
	movs r3, #109
	add r3, sp
	strb r2, [r3]
	ldr r4, [sp, #36]
	movs r7, #0
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r3, #64
	mov r10, r3
	mov r6, r11
.L_0818ec36:
	ldr r3, .L_0818ed88
	ldrb r3, [r3, r7]
	adds r2, r3, #0
	adds r2, #8
	cmp r11, r2
	ble .L_0818ecb8
	mov r0, r11
	subs r3, r2, r0
	lsls r3, r3, #3
	adds r1, r3, #0
	adds r1, #40
	cmp r1, #0
	ble .L_0818ec52
	movs r1, #0
.L_0818ec52:
	movs r3, #64
	negs r3, r3
	cmp r1, r3
	ble .L_0818ecb8
	ldr r3, .L_0818ed8c
	mov r4, r11
	ldrb r3, [r3, r7]
	subs r2, r4, r2
	muls r2, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	movs r4, #7
	adds r5, r2, #0
	muls r5, r3
	adds r3, r6, #0
	ands r3, r4
	mov r2, r8
	lsls r3, r3, #4
	movs r0, #131
	lsls r0, r0, #7
	str r1, [r2, #20]
	strb r3, [r2, #24]
	adds r5, r5, r0
	bl Func_08014de4
	ldr r3, .L_0818ed90
	mov r2, r10
	ldrsb r1, [r3, r7]
	lsls r0, r2, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #3
	bl Func_08015024
	ldr r0, .L_0818ed94
	ldr r1, [sp, #28]
	movs r2, #32
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_0818ecb8:
	adds r7, #1
	adds r6, #5
	cmp r7, #3
	bne .L_0818ec36
	mov r0, r8
	bl Sys_Free
	ldr r0, [sp, #28]
	bl Sys_Free
	ldr r3, .L_0818ed98
	ldr r5, .L_0818ed9c
	movs r7, #0
	mov r8, r3
.L_0818ecd4:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0818ed1a
	asrs r0, r0, #2
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	mov r6, r8
	ldrh r1, [r6, r3]
	ldr r2, [sp, #84]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #100]
	ldr r4, [sp, #88]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
	movs r2, #128
	subs r3, #1
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #56
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
.L_0818ed1a:
	movs r6, #128
	adds r7, #1
	lsls r6, r6, #1
	adds r5, #28
	cmp r7, r6
	bne .L_0818ecd4
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #96]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r11, r2
	mov r3, r11
	cmp r3, #50
	beq .L_0818ed52
	b .L_0818e990
.L_0818ed52:
	ldr r0, .L_0818eda0
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r6, #238
	ldr r4, [sp, #96]
	lsls r6, r6, #7
	adds r6, #220
	adds r3, r4, r6
	ldr r0, [r3]
	bl Func_08020040 + 0x8
	bl Func_08143bb8
	add sp, #208
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0818ed80:
	.4byte Data_08199090
.L_0818ed84:
	.4byte Data_08198ec4
.L_0818ed88:
	.4byte Data_08199e98
.L_0818ed8c:
	.4byte Data_08199e9c
.L_0818ed90:
	.4byte Data_08199ea0
.L_0818ed94:
	.4byte Data_08198cac
.L_0818ed98:
	.4byte Data_08197410
.L_0818ed9c:
	.4byte gMapCellBuffer
.L_0818eda0:
	.4byte Func_08143000
