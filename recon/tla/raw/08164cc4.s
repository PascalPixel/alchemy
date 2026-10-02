.syntax unified
	.thumb
	.global Func_08164cc4
	.thumb_func
Func_08164cc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #280
	str r1, [sp, #76]
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r0, #0
	ldr r0, [r3, #96]
	mov r1, sp
	adds r1, #148
	str r0, [sp, #72]
	str r1, [sp, #60]
	movs r0, #128
	ldr r3, [r3, #92]
	lsls r0, r0, #6
	str r3, [r1]
	bl BattleFx_BeginCanvasLayer
	movs r2, #128
	ldr r3, .L_08164d04
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [sp, #76]
	cmp r2, #1
	bne .L_08164d46
	b .L_08164d08
.L_08164d04:
	.4byte 0x00000100
.L_08164d08:
	ldr r0, [r6, #8]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r2, #40]
	movs r3, #145
	lsls r3, r3, #8
	adds r3, #235
	str r3, [r2, #72]
	movs r5, #1
	negs r5, r5
	movs r3, #0
	ldr r0, [r6, #8]
	adds r1, r5, #0
	str r3, [sp, #0]
	movs r2, #2
	adds r3, r5, #0
	bl Func_0814cd48
	movs r0, #145
	bl Audio_PlayCue
	ldr r3, [r6, #4]
	ldr r4, [sp, #76]
	str r4, [sp, #64]
	cmp r3, #1
	beq .L_08164d4c
	str r5, [sp, #64]
	b .L_08164d4c
.L_08164d46:
	movs r0, #1
	negs r0, r0
	str r0, [sp, #64]
.L_08164d4c:
	bl Func_0813ba50
	ldr r2, .L_08164d88
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r1, [sp, #60]
	ldr r5, .L_08164d8c
	ldr r3, [r1]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r3, r2
	movs r1, #200
	movs r2, #0
	str r2, [r3]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	movs r1, #0
	bl Func_08163c2c
	adds r0, r5, #0
	bl Scheduler_RemoveCallback
	ldr r3, [sp, #76]
	b .L_08164d90
.L_08164d88:
	.4byte 0x00000000
.L_08164d8c:
	.4byte Func_08143000
.L_08164d90:
	cmp r3, #1
	bne .L_08164de2
	movs r5, #238
	movs r6, #128
	movs r4, #0
	lsls r5, r5, #7
	lsls r6, r6, #2
	mov r8, r4
	adds r5, #220
	adds r6, #126
.L_08164da4:
	adds r0, r6, #0
	bl ResourceObject_CreateFar
	ldr r1, [sp, #60]
	ldr r3, [r1]
	str r0, [r3, r5]
	cmp r0, #0
	beq .L_08164dcc
	movs r3, #0
	strb r3, [r0, #26]
	movs r1, #2
	bl Animation_ApplyChildArgumentFar
	ldr r2, [sp, #60]
	ldr r3, [r2]
	movs r2, #12
	ldr r1, [r3, r5]
	ldrb r3, [r1, #9]
	orrs r3, r2
	strb r3, [r1, #9]
.L_08164dcc:
	movs r3, #128
	movs r4, #1
	lsls r3, r3, #6
	add r8, r4
	adds r3, #1
	mov r0, r8
	adds r5, #4
	adds r6, r6, r3
	cmp r0, #2
	bne .L_08164da4
	b .L_08164df0
.L_08164de2:
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #138
	movs r0, #1
	movs r2, #3
	bl Func_08152404
.L_08164df0:
	ldr r2, [sp, #60]
	movs r3, #224
	ldr r1, [r2]
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r0, .L_08164e78
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #76]
	cmp r4, #1
	bne .L_08164e1e
	ldr r0, .L_08164e7c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08164e80
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08164e1e:
	movs r1, #128
	ldr r3, .L_08164e84
	lsls r1, r1, #8
	ldr r2, .L_08164e88
	ldr r0, .L_08164e8c
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_08164e80
	movs r2, #240
	ldr r1, .L_08164e8c
	lsls r2, r2, #7
	ldr r0, .L_08164e90
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_08164e68
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08164e6c
	subs r2, #48
	strh r3, [r2]
	ldr r3, .L_08164e70
	subs r2, #22
	strh r3, [r2]
	ldr r3, .L_08164e74
	adds r2, #2
	ldr r1, .L_08164e94
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	movs r7, #15
	mov r10, r1
	b .L_08164e98
.L_08164e68:
	.4byte 0x00000000
.L_08164e6c:
	.4byte 0x00000100
.L_08164e70:
	.4byte 0x00001f80
.L_08164e74:
	.4byte 0x00002787
.L_08164e78:
	.4byte 0x00000185
.L_08164e7c:
	.4byte 0x00000188
.L_08164e80:
	.4byte IwramCopyWords
.L_08164e84:
	.4byte IwramFillWords
.L_08164e88:
	.4byte 0x01010101
.L_08164e8c:
	.4byte gMapCellBuffer
.L_08164e90:
	.4byte 0x06008000
.L_08164e94:
	.4byte 0x05000100
.L_08164e98:
	bl Random16
	adds r6, r0, #0
	bl Random16
	adds r5, r0, #0
	bl Random16
	ands r5, r7
	ands r0, r7
	adds r5, #16
	adds r0, #16
	ands r6, r7
	lsls r0, r0, #10
	lsls r5, r5, #5
	adds r6, #16
	orrs r0, r5
	movs r4, #1
	orrs r0, r6
	mov r2, r10
	add r8, r4
	strh r0, [r2]
	movs r3, #2
	mov r0, r8
	add r10, r3
	cmp r0, #63
	bne .L_08164e98
	movs r1, #128
	ldr r3, .L_081650a0
	ldr r0, [sp, #72]
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #0
	movs r2, #127
	mov r8, r1
	mov r10, r2
	movs r7, #7
.L_08164ee6:
	bl Random16
	mov r3, r10
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	mov r4, r10
	adds r5, r0, #0
	ands r5, r4
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r1, r3, #0
	adds r1, #64
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08164f0e
	adds r3, r5, #7
.L_08164f0e:
	asrs r3, r3, #3
	adds r2, r6, #0
	cmp r6, #0
	bge .L_08164f18
	adds r2, r6, #7
.L_08164f18:
	asrs r2, r2, #3
	lsls r3, r3, #4
	adds r3, r3, r2
	ands r5, r7
	lsls r3, r3, #3
	adds r3, r3, r5
	ldr r0, [sp, #72]
	ands r6, r7
	lsls r3, r3, #3
	adds r3, r3, r6
	strb r1, [r0, r3]
	movs r2, #128
	movs r1, #1
	add r8, r1
	lsls r2, r2, #1
	cmp r8, r2
	bne .L_08164ee6
	movs r2, #128
	ldr r1, [sp, #72]
	ldr r3, .L_081650a4
	lsls r2, r2, #7
	ldr r0, .L_081650a8
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_081650ac
	movs r3, #240
	str r3, [r2, #16]
	ldr r4, [sp, #60]
	movs r0, #240
	ldr r3, [r4]
	lsls r0, r0, #7
	adds r0, #240
	adds r3, r3, r0
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r1, [sp, #60]
	movs r4, #238
	ldr r2, [r1]
	lsls r4, r4, #7
	movs r0, #238
	adds r4, #208
	lsls r0, r0, #7
	adds r3, r2, r4
	movs r1, #0
	adds r0, #212
	str r1, [r3]
	subs r4, #64
	adds r3, r2, r0
	str r1, [r3]
	subs r0, #64
	adds r3, r2, r4
	str r1, [r3]
	adds r1, r2, r0
	movs r3, #2
	str r3, [r1]
	ldr r4, [sp, #64]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #152
	adds r1, r2, r3
	adds r0, #8
	lsls r3, r4, #7
	str r3, [r1]
	adds r2, r2, r0
	mov r1, r8
	str r1, [r2]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #255
	ldr r0, .L_081650b0
	bl Scheduler_AddOrUpdateCallback
	movs r1, #200
	ldr r0, .L_081650b4
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	add r2, sp, #152
	mov r10, r2
	add r3, sp, #280
	movs r6, #63
	mov r5, r10
	mov r8, r3
.L_08164fc0:
	bl Random16
	ands r0, r6
	strb r0, [r5]
	adds r5, #1
	cmp r5, r8
	bne .L_08164fc0
	movs r4, #1
	movs r6, #0
	mov r8, r4
	movs r5, #0
.L_08164fd6:
	mov r0, r8
	lsrs r3, r0, #31
	add r3, r8
	asrs r3, r3, #1
	movs r1, #4
	adds r6, r6, r3
	add r8, r1
	cmp r5, r6
	beq .L_08165046
	movs r2, #127
	movs r3, #0
	mov r7, r10
	movs r4, #7
	mov lr, r2
	mov r12, r3
.L_08164ff4:
	movs r0, #0
.L_08164ff6:
	mov r1, lr
	adds r3, r0, #0
	ands r3, r1
	ldrb r3, [r7, r3]
	subs r1, r5, r3
	cmp r1, #0
	blt .L_08165036
	cmp r1, #127
	bgt .L_08165036
	adds r2, r1, #0
	cmp r1, #0
	bge .L_08165010
	adds r2, r1, #7
.L_08165010:
	asrs r2, r2, #3
	adds r3, r0, #0
	cmp r0, #0
	bge .L_0816501a
	adds r3, r0, #7
.L_0816501a:
	asrs r3, r3, #3
	lsls r2, r2, #5
	adds r2, r2, r3
	ands r1, r4
	lsls r2, r2, #3
	adds r2, r2, r1
	adds r3, r0, #0
	ands r3, r4
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, .L_081650b8
	mov r1, r12
	adds r2, r2, r3
	strb r1, [r2]
.L_08165036:
	movs r2, #128
	adds r0, #1
	lsls r2, r2, #1
	cmp r0, r2
	bne .L_08164ff6
	adds r5, #1
	cmp r5, r6
	bne .L_08164ff4
.L_08165046:
	ldr r4, [sp, #60]
	movs r0, #240
	ldr r3, [r4]
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r3, r0
	movs r7, #1
	str r7, [r3]
	movs r0, #1
	bl WaitFrames
	cmp r6, #191
	ble .L_08164fd6
	ldr r3, .L_08165098
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0816509c
	adds r2, #2
	strh r3, [r2]
	ldr r2, .L_081650bc
	movs r5, #192
	ldrh r1, [r2, #4]
	lsls r5, r5, #18
	str r1, [sp, #56]
	movs r0, #104
	ldrh r3, [r2, #6]
	movs r1, #8
	str r3, [sp, #52]
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	movs r6, #3
	str r3, [sp, #48]
	movs r3, #0
	strh r3, [r2, #4]
	movs r3, #32
	strh r3, [r2, #6]
	movs r2, #7
	b .L_081650c0
.L_08165098:
	.4byte 0x00003f42
.L_0816509c:
	.4byte 0x00001010
.L_081650a0:
	.4byte IwramFillWords
.L_081650a4:
	.4byte IwramCopyWords
.L_081650a8:
	.4byte 0x06004000
.L_081650ac:
	.4byte gCameraSceneParameters
.L_081650b0:
	.4byte Func_0813bb38
.L_081650b4:
	.4byte Func_08143174
.L_081650b8:
	.4byte gMapCellBuffer
.L_081650bc:
	.4byte Data_03001120
.L_081650c0:
	movs r3, #3
	str r7, [sp, #0]
	bl Func_08196404
	ldr r5, [r5, #104]
	ldr r4, [sp, #60]
	str r5, [sp, #68]
	movs r0, #239
	ldr r3, [r4]
	lsls r0, r0, #7
	movs r1, #238
	adds r2, r3, r0
	lsls r1, r1, #7
	str r6, [r2]
	adds r1, #132
	ldr r2, .L_08165214
	adds r3, r3, r1
	movs r1, #192
	lsls r1, r1, #4
	str r2, [r3]
	ldr r0, .L_08165218
	adds r1, #254
	bl Scheduler_AddOrUpdateCallback
	ldr r4, [sp, #60]
	movs r2, #0
	ldr r3, [r4]
	mov r8, r2
	adds r3, #24
	subs r2, #1
.L_081650fc:
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #64
	bne .L_081650fc
	ldr r2, [sp, #48]
	movs r3, #1
	str r3, [r2, #16]
	ldr r4, [sp, #60]
	movs r0, #238
	ldr r3, [r4]
	lsls r0, r0, #7
	adds r0, #140
	movs r2, #0
	adds r3, r3, r0
	str r2, [r3]
	mov r11, r2
	mov r1, sp
	mov r2, sp
	adds r1, #132
	adds r2, #88
	movs r3, #0
	str r1, [sp, #24]
	str r2, [sp, #44]
	str r4, [sp, #40]
	str r3, [sp, #16]
.L_08165134:
	ldr r4, [sp, #60]
	movs r1, #238
	ldr r0, [r4]
	lsls r1, r1, #7
	adds r1, #140
	adds r3, r0, r1
	ldr r2, [r3]
	adds r3, r2, #0
	cmp r3, #0
	bge .L_0816514a
	adds r3, #3
.L_0816514a:
	asrs r4, r3, #2
	movs r2, #236
	ldr r3, [sp, #76]
	lsls r2, r2, #7
	adds r2, #64
	adds r5, r0, r2
	cmp r3, #1
	bne .L_0816516e
	ldr r3, .L_0816521c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08165182
	mov r0, r11
	cmp r0, #16
	ble .L_08165182
	b .L_0816553e
.L_0816516e:
	ldr r3, .L_0816521c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08165182
	mov r1, r11
	cmp r1, #4
	ble .L_08165182
	b .L_0816553e
.L_08165182:
	mov r2, r11
	cmp r2, #0
	bne .L_08165192
	movs r0, #141
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r4, [sp, #8]
.L_08165192:
	movs r3, #0
	mov r8, r3
.L_08165196:
	movs r0, #1
	add r8, r0
	mov r1, r8
	strh r3, [r5]
	adds r5, #2
	cmp r1, #15
	bne .L_08165196
.L_081651a4:
	mov r1, r8
	subs r1, #16
	adds r3, r1, #0
	cmp r1, #0
	bge .L_081651b2
	mov r3, r8
	subs r3, #13
.L_081651b2:
	asrs r3, r3, #2
	adds r2, r3, r4
	adds r3, r2, #0
	adds r1, r2, #0
	subs r3, #32
	subs r1, #80
	cmp r3, #0
	bge .L_081651c4
	movs r3, #0
.L_081651c4:
	cmp r3, #31
	ble .L_081651ca
	movs r3, #31
.L_081651ca:
	cmp r1, #0
	bge .L_081651d0
	movs r1, #0
.L_081651d0:
	cmp r1, #31
	ble .L_081651d6
	movs r1, #31
.L_081651d6:
	lsls r2, r1, #5
	lsls r3, r3, #10
	orrs r3, r2
	asrs r2, r1, #1
	orrs r3, r2
	movs r2, #1
	add r8, r2
	strh r3, [r5]
	mov r3, r8
	adds r5, #2
	cmp r3, #135
	bne .L_081651a4
	ldr r3, .L_08165210
.L_081651f0:
	movs r4, #1
	add r8, r4
	mov r0, r8
	strh r3, [r5]
	adds r5, #2
	cmp r0, #160
	bne .L_081651f0
	ldr r1, [sp, #64]
	cmp r1, #1
	bne .L_08165220
	mov r3, r11
	cmp r3, #0
	bge .L_0816520c
	adds r3, #3
.L_0816520c:
	asrs r7, r3, #2
	b .L_0816522e
.L_08165210:
	.4byte 0x00000000
.L_08165214:
	.4byte Data_02020202
.L_08165218:
	.4byte Func_08164bb4
.L_0816521c:
	.4byte gInput
.L_08165220:
	mov r2, r11
	cmp r2, #0
	bge .L_08165228
	adds r2, #3
.L_08165228:
	asrs r2, r2, #2
	movs r3, #64
	subs r7, r3, r2
.L_0816522e:
	ldr r4, [sp, #24]
	movs r2, #96
	mov r3, r11
	subs r3, r2, r3
	mov r10, r3
	movs r3, #0
	str r3, [r4, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r4, #4]
	ldr r0, [sp, #76]
	cmp r0, #1
	bne .L_0816529e
	ldr r1, [sp, #16]
	ldr r3, [sp, #44]
	movs r2, #160
	lsls r2, r2, #8
	adds r6, r1, r2
	str r6, [sp, #88]
	str r6, [r3, #4]
	ldr r0, [sp, #24]
	movs r4, #160
	lsls r4, r4, #15
	lsls r3, r7, #16
	adds r3, r3, r4
	str r3, [r0]
	mov r1, r10
	movs r3, #64
	subs r3, r3, r1
	lsls r3, r3, #16
	str r3, [r0, #8]
	ldr r2, [sp, #40]
	movs r4, #238
	ldr r3, [r2]
	lsls r4, r4, #7
	adds r4, #220
	adds r3, r3, r4
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #44]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	ldr r0, [sp, #40]
	movs r1, #238
	ldr r3, [r0]
	lsls r1, r1, #7
	adds r1, #224
	adds r3, r3, r1
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #44]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	b .L_081652d8
.L_0816529e:
	ldr r3, [sp, #16]
	ldr r0, [sp, #44]
	movs r4, #128
	lsls r4, r4, #9
	adds r6, r3, r4
	str r6, [sp, #88]
	str r6, [r0, #4]
	ldr r4, [sp, #24]
	movs r1, #192
	lsls r1, r1, #15
	lsls r3, r7, #16
	adds r3, r3, r1
	mov r0, r10
	str r3, [r4]
	subs r3, r2, r0
	lsls r3, r3, #16
	str r3, [r4, #8]
	ldr r1, [sp, #60]
	movs r2, #238
	ldr r3, [r1]
	lsls r2, r2, #7
	adds r2, #220
	adds r3, r3, r2
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #44]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_081652d8:
	movs r3, #0
	mov r4, r10
	mov r8, r3
	movs r3, #32
	subs r4, r3, r4
	mov r10, r4
	movs r2, #0
.L_081652e6:
	ldr r0, [sp, #60]
	movs r1, #1
	ldr r3, [r0]
	negs r1, r1
	adds r5, r3, r2
	ldr r3, [r5, #24]
	cmp r3, r1
	bne .L_0816535c
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	movs r2, #128
	ands r3, r0
	lsls r2, r2, #7
	adds r1, r3, r2
	movs r3, #0
	str r3, [r5, #24]
	adds r0, r1, #0
	str r1, [sp, #12]
	bl Trig_Sin
	adds r3, r7, #0
	adds r3, #96
	lsls r2, r3, #16
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #1
	ldr r1, [sp, #12]
	cmp r3, #0
	bge .L_0816532e
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
.L_0816532e:
	asrs r3, r3, #16
	muls r3, r6
	adds r3, r2, r3
	str r3, [r5]
	adds r0, r1, #0
	bl Trig_Cos
	lsls r3, r0, #4
	subs r3, r3, r0
	mov r1, r10
	lsls r3, r3, #1
	lsls r2, r1, #16
	cmp r3, #0
	bge .L_08165352
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
.L_08165352:
	asrs r3, r3, #16
	muls r3, r6
	subs r3, r2, r3
	str r3, [r5, #4]
	b .L_08165368
.L_0816535c:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r2, #28
	cmp r1, #32
	bne .L_081652e6
.L_08165368:
	add r5, sp, #96
	movs r3, #0
	str r3, [r5]
	str r3, [r5, #4]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r5, #8]
	bl Func_08014de4
	adds r0, r5, #0
	bl SceneTransform_ApplyPosition
	movs r0, #128
	lsls r0, r0, #4
	bl Func_080150e4
	ldr r0, [sp, #16]
	bl Func_08015068
	ldr r7, .L_08165468
	movs r2, #0
	mov r8, r2
	add r6, sp, #120
	add r5, sp, #108
.L_08165398:
	ldrh r3, [r7]
	adds r1, r5, #0
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	movs r4, #2
	ldrsh r3, [r7, r4]
	asrs r2, r2, #1
	add r3, r11
	lsls r3, r3, #16
	str r3, [r6, #4]
	ldrh r3, [r7, #4]
	lsls r2, r2, #16
	lsls r3, r3, #16
	str r2, [r6]
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	lsls r2, r2, #16
	str r2, [r6, #8]
	adds r0, r6, #0
	bl Func_0815e1ec
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r0, #153
	adds r3, r2, #0
	adds r3, #128
	str r3, [r5]
	movs r1, #6
	ldrsh r3, [r5, r1]
	lsls r0, r0, #6
	adds r1, r3, #0
	adds r1, #60
	str r1, [r5, #4]
	ldr r4, [sp, #60]
	adds r2, #124
	ldr r1, [r4]
	adds r3, #56
	adds r1, r1, r0
	movs r0, #8
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, .L_0816546c
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r7, #6
	cmp r1, #7
	bne .L_08165398
	ldr r2, [sp, #64]
	cmp r2, #1
	bne .L_0816541c
	mov r3, r11
	cmp r3, #0
	bge .L_08165414
	adds r3, #3
.L_08165414:
	asrs r3, r3, #2
	adds r7, r3, #0
	subs r7, #16
	b .L_0816542a
.L_0816541c:
	mov r2, r11
	cmp r2, #0
	bge .L_08165424
	adds r2, #3
.L_08165424:
	asrs r2, r2, #2
	movs r3, #16
	subs r7, r3, r2
.L_0816542a:
	movs r3, #96
	negs r3, r3
	ldr r5, .L_08165470
	add r3, r11
	movs r4, #0
	mov r10, r3
	mov r8, r4
.L_08165438:
	movs r0, #2
	ldrsh r3, [r5, r0]
	add r3, r10
	cmp r3, #93
	bgt .L_08165474
	ldr r2, [sp, #60]
	movs r4, #142
	ldr r1, [r2]
	movs r0, #0
	ldrsh r2, [r5, r0]
	lsls r4, r4, #6
	movs r0, #24
	adds r2, r2, r7
	adds r1, r1, r4
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #12
	ldr r0, .L_0816546c
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	b .L_0816548c
	.2byte 0x0000
.L_08165468:
	.4byte Data_08198a02
.L_0816546c:
	.4byte gMapCellBuffer
.L_08165470:
	.4byte Data_08198a2c
.L_08165474:
	cmp r3, #95
	bgt .L_0816548c
	movs r1, #0
	ldrsh r0, [r5, r1]
	add r2, sp, #280
	adds r0, r0, r7
	mov r9, r2
	lsls r0, r0, #16
	lsls r1, r3, #16
	movs r2, #1
	bl Func_08164c0c
.L_0816548c:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #4
	cmp r4, #7
	bne .L_08165438
	ldr r1, [sp, #64]
	movs r0, #0
	lsls r3, r1, #2
	adds r3, r3, r1
	mov r8, r0
	lsls r7, r3, #14
	movs r6, #0
.L_081654a6:
	ldr r2, [sp, #60]
	ldr r3, [r2]
	adds r5, r3, r6
	ldr r1, [r5, #24]
	cmp r1, #0
	blt .L_081654f4
	lsls r1, r1, #10
	adds r1, r3, r1
	movs r3, #224
	lsls r3, r3, #3
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #32
	subs r3, #16
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, .L_081655bc
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r0, .L_081655c0
	subs r3, r3, r7
	str r3, [r5]
	ldr r3, [r5, #4]
	adds r3, r3, r0
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #6
	bne .L_081654f4
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_081654f4:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #28
	cmp r2, #32
	bne .L_081654a6
	ldr r4, [sp, #40]
	movs r0, #240
	ldr r3, [r4]
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r3, r0
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #16]
	movs r2, #128
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [sp, #16]
	ldr r4, [sp, #40]
	movs r0, #238
	ldr r2, [r4]
	lsls r0, r0, #7
	adds r0, #140
	movs r3, #1
	adds r2, r2, r0
	add r11, r3
	ldr r3, [r2]
	mov r1, r11
	adds r3, #1
	str r3, [r2]
	cmp r1, #192
	beq .L_0816553e
	b .L_08165134
.L_0816553e:
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #48]
	movs r5, #0
	str r5, [r2, #16]
	ldr r0, .L_081655c4
	bl Scheduler_RemoveCallback
	ldr r0, .L_081655c8
	bl Scheduler_RemoveCallback
	ldr r0, .L_081655cc
	bl Scheduler_RemoveCallback
	add r4, sp, #56
	add r0, sp, #52
	ldrh r4, [r4]
	ldr r3, .L_081655d0
	ldrh r0, [r0]
	strh r4, [r3, #4]
	strh r0, [r3, #6]
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_0814cca8
	ldr r3, .L_081655b0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	str r5, [r3]
	ldr r3, .L_081655d4
	adds r2, #12
	str r3, [r2]
	ldr r3, .L_081655b4
	adds r2, #38
	strh r3, [r2]
	ldr r3, .L_081655b8
	subs r2, #70
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r2, [sp, #60]
	str r3, [sp, #68]
	movs r3, #224
	b .L_081655d8
	.2byte 0x0000
.L_081655b0:
	.4byte 0x00000080
.L_081655b4:
	.4byte 0x00001010
.L_081655b8:
	.4byte 0x00002784
.L_081655bc:
	.4byte gMapCellBuffer
.L_081655c0:
	.4byte 0xfffb0000
.L_081655c4:
	.4byte Func_0813bb38
.L_081655c8:
	.4byte Func_08164bb4
.L_081655cc:
	.4byte Func_08143174
.L_081655d0:
	.4byte Data_03001120
.L_081655d4:
	.4byte 0xfffff000
.L_081655d8:
	ldr r1, [r2]
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r0, .L_08165960
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r4, #0
	mov r8, r4
	movs r7, #127
	movs r6, #0
.L_081655f0:
	ldr r0, [sp, #60]
	ldr r5, [r0]
	bl Random16
	adds r5, r5, r6
	ands r0, r7
	str r0, [r5]
	bl Random16
	movs r1, #1
	ands r0, r7
	add r8, r1
	adds r0, #127
	mov r2, r8
	str r0, [r5, #4]
	adds r6, #28
	cmp r2, #32
	bne .L_081655f0
	ldr r5, .L_08165964
	movs r3, #0
	mov r8, r3
	movs r6, #0
	movs r7, #255
.L_0816561e:
	str r6, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #127
	movs r4, #1
	lsls r0, r0, #12
	add r8, r4
	str r0, [r5, #20]
	mov r0, r8
	str r6, [r5, #24]
	adds r5, #28
	cmp r0, #128
	bne .L_0816561e
	ldr r5, .L_08165968
	movs r1, #0
	mov r8, r1
	movs r6, #0
	movs r7, #255
.L_0816565e:
	str r6, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #128
	movs r2, #1
	movs r3, #128
	lsls r0, r0, #13
	add r8, r2
	lsls r3, r3, #2
	str r0, [r5, #20]
	str r6, [r5, #24]
	adds r5, #28
	cmp r8, r3
	bne .L_0816565e
	ldr r4, [sp, #60]
	movs r0, #239
	ldr r2, [r4]
	lsls r0, r0, #7
	adds r1, r2, r0
	movs r3, #1
	str r3, [r1]
	movs r1, #238
	ldr r3, .L_0816596c
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r2, r1
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08165970
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #60]
	mov r4, sp
	movs r0, #232
	adds r4, #80
	lsls r0, r0, #9
	str r3, [sp, #32]
	str r4, [sp, #28]
	str r0, [sp, #20]
	movs r2, #0
	mov r11, r2
.L_081656ce:
	movs r3, #192
	mov r1, r11
	lsls r3, r3, #18
	subs r1, #16
	ldr r5, [r3, #48]
	str r1, [sp, #36]
	cmp r1, #19
	ble .L_081656e8
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
.L_081656e8:
	mov r2, r11
	cmp r2, #0
	bne .L_081656f4
	movs r0, #156
	bl Audio_PlayCue
.L_081656f4:
	mov r3, r11
	cmp r3, #40
	bne .L_08165700
	movs r0, #145
	bl Audio_PlayCue
.L_08165700:
	mov r4, r11
	cmp r4, #48
	bne .L_0816573a
	ldr r0, [sp, #76]
	cmp r0, #1
	bne .L_08165734
	ldr r1, [sp, #32]
	movs r2, #238
	ldr r3, [r1]
	lsls r2, r2, #7
	adds r2, #220
	adds r3, r3, r2
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	ldr r4, [sp, #32]
	movs r0, #238
	ldr r3, [r4]
	lsls r0, r0, #7
	adds r0, #224
	adds r3, r3, r0
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	bl BattleActor_CommitPlacementFar
.L_08165734:
	movs r0, #134
	bl Func_081180e8
.L_0816573a:
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	ldr r7, .L_08165968
	bl Graphics_PrepareTransferInIwramWork
	movs r1, #0
	movs r2, #63
	mov r8, r1
	mov r10, r2
.L_08165752:
	ldr r3, [r7, #4]
	cmp r3, #0
	blt .L_08165816
	add r6, sp, #96
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0815e1ec
	ldr r3, [r6]
	ldr r2, [r6, #8]
	asrs r3, r3, #1
	str r3, [r6]
	cmp r2, #159
	bgt .L_08165774
	movs r3, #160
	str r3, [r6, #8]
	movs r2, #160
.L_08165774:
	movs r3, #136
	lsls r3, r3, #2
	adds r3, #255
	cmp r2, r3
	ble .L_08165782
	str r3, [r6, #8]
	adds r2, r3, #0
.L_08165782:
	adds r3, r2, #0
	subs r3, #160
	cmp r3, #0
	bge .L_0816578c
	adds r3, #63
.L_0816578c:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	ldr r2, .L_08165974
	lsls r5, r0, #1
	subs r3, r5, #2
	ldrh r4, [r2, r3]
	movs r3, #1
	mov r2, r8
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	ldr r2, [sp, #60]
	lsls r3, r3, #1
	ldr r1, [r2]
	adds r4, r4, r3
	movs r3, #228
	adds r1, r1, r4
	lsls r3, r3, #6
	ldr r2, [r6]
	adds r1, r1, r3
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	ldr r4, [sp, #68]
	subs r3, r3, r0
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	ldr r2, .L_08165978
	adds r0, r7, #0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #4]
	movs r2, #160
	lsls r2, r2, #13
	cmp r3, r2
	bgt .L_08165816
	movs r3, #0
	str r3, [r7]
	str r3, [r7, #8]
	str r2, [r7, #4]
	bl Random16
	mov r1, r10
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #15
	str r0, [r7, #12]
	bl Random16
	mov r2, r10
	ands r0, r2
	lsls r0, r0, #13
	str r0, [r7, #16]
	bl Random16
	mov r3, r10
	ands r0, r3
	subs r0, #32
	lsls r0, r0, #15
	str r0, [r7, #20]
.L_08165816:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r7, #28
	cmp r0, #64
	bne .L_08165752
	ldr r2, .L_08165974
	movs r1, #0
	mov r8, r1
	mov r10, r1
	mov r9, r2
.L_0816582c:
	ldr r3, [sp, #60]
	mov r0, r8
	ldr r1, [r3]
	movs r5, #7
	ands r5, r0
	mov r4, r10
	adds r7, r1, r4
	adds r4, r5, #3
	lsls r6, r4, #1
	subs r3, r6, #2
	mov r2, r9
	ldrh r0, [r2, r3]
	movs r3, #1
	mov r2, r8
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	ldr r2, [r7]
	lsls r3, r3, #1
	adds r0, r0, r3
	lsrs r3, r4, #1
	subs r2, r2, r3
	ldr r3, [r7, #4]
	adds r1, r1, r0
	movs r0, #228
	lsls r0, r0, #6
	subs r3, r3, r4
	adds r1, r1, r0
	str r4, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #4]
	movs r0, #10
	subs r3, r3, r5
	subs r3, #8
	negs r0, r0
	str r3, [r7, #4]
	cmp r3, r0
	bge .L_08165888
	movs r3, #128
	str r3, [r7, #4]
.L_08165888:
	movs r2, #1
	add r8, r2
	movs r1, #28
	mov r3, r8
	add r10, r1
	cmp r3, #64
	bne .L_0816582c
	ldr r6, .L_08165964
	movs r4, #0
	movs r0, #255
	mov r8, r4
	mov r10, r4
	mov r9, r0
.L_081658a2:
	movs r1, #3
	mov r0, r8
	bl __divsi3
	ldr r1, [sp, #36]
	cmp r0, r1
	bge .L_08165940
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L_08165940
	add r5, sp, #96
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r7, r3, #1
	str r7, [r5]
	ldr r3, [r6, #24]
	cmp r3, #13
	bhi .L_081658fe
	lsrs r2, r3, #31
	ldr r4, [sp, #60]
	adds r2, r3, r2
	ldr r3, .L_0816597c
	asrs r2, r2, #1
	lsls r2, r2, #1
	ldrh r3, [r3, r2]
	ldr r1, [r4]
	movs r0, #224
	adds r1, r1, r3
	ldr r3, .L_08165980
	lsls r0, r0, #3
	ldrh r4, [r3, r2]
	ldr r3, [r5, #4]
	adds r1, r1, r0
	lsrs r0, r4, #1
	subs r3, r3, r0
	subs r2, r7, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
.L_081658fe:
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #14
	bne .L_08165936
	movs r3, #160
	lsls r3, r3, #13
	mov r0, r10
	str r3, [r6, #4]
	str r0, [r6]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #127
	lsls r0, r0, #16
	mov r2, r10
	str r0, [r6, #8]
	str r2, [r6, #12]
	bl Random16
	mov r3, r9
	ands r0, r3
	mov r4, r10
	lsls r0, r0, #11
	str r0, [r6, #16]
	str r4, [r6, #20]
	str r4, [r6, #24]
	b .L_08165940
.L_08165936:
	adds r0, r6, #0
	movs r1, #64
	movs r2, #1
	bl BattleFxKernels_IntegrateVector3
.L_08165940:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #64
	bne .L_081658a2
	ldr r2, [sp, #64]
	cmp r2, #1
	bne .L_08165984
	mov r4, r11
	lsrs r3, r4, #31
	add r3, r11
	asrs r3, r3, #1
	adds r1, r3, #0
	adds r1, #24
	b .L_08165990
.L_08165960:
	.4byte 0x00000184
.L_08165964:
	.4byte gMapCellBuffer
.L_08165968:
	.4byte Data_02010e00
.L_0816596c:
	.4byte 0x10101010
.L_08165970:
	.4byte Func_08143000
.L_08165974:
	.4byte Data_08197410
.L_08165978:
	.4byte 0xffffe000
.L_0816597c:
	.4byte Data_08198a48
.L_08165980:
	.4byte Data_08198a56
.L_08165984:
	mov r0, r11
	lsrs r3, r0, #31
	add r3, r11
	asrs r3, r3, #1
	movs r2, #56
	subs r1, r2, r3
.L_08165990:
	mov r3, r11
	lsls r2, r3, #1
	mov r4, r11
	movs r3, #64
	subs r0, r3, r2
	lsls r3, r4, #8
	movs r4, #128
	lsls r4, r4, #10
	adds r2, r3, r4
	ldr r4, [sp, #24]
	movs r3, #0
	str r3, [r4, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r4, #4]
	ldr r3, [sp, #76]
	cmp r3, #1
	bne .L_08165a02
	ldr r4, [sp, #20]
	ldr r2, [sp, #28]
	str r4, [sp, #80]
	str r4, [r2, #4]
	lsls r3, r1, #16
	movs r4, #192
	ldr r1, [sp, #24]
	lsls r4, r4, #15
	adds r3, r3, r4
	str r3, [r1]
	movs r3, #96
	subs r3, r3, r0
	lsls r3, r3, #16
	str r3, [r1, #8]
	ldr r2, [sp, #32]
	movs r4, #238
	ldr r3, [r2]
	lsls r4, r4, #7
	adds r4, #220
	adds r3, r3, r4
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	ldr r0, [sp, #32]
	movs r1, #238
	ldr r3, [r0]
	lsls r1, r1, #7
	adds r1, #224
	adds r3, r3, r1
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	b .L_08165a34
.L_08165a02:
	ldr r3, [sp, #28]
	str r2, [sp, #80]
	str r2, [r3, #4]
	movs r4, #192
	lsls r3, r1, #16
	ldr r1, [sp, #24]
	lsls r4, r4, #15
	adds r3, r3, r4
	str r3, [r1]
	movs r3, #96
	subs r3, r3, r0
	lsls r3, r3, #16
	str r3, [r1, #8]
	ldr r2, [sp, #60]
	movs r4, #238
	ldr r3, [r2]
	lsls r4, r4, #7
	adds r4, #220
	adds r3, r3, r4
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08165a34:
	ldr r0, [sp, #32]
	movs r1, #238
	ldr r3, [r0]
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r3, r1
	movs r2, #1
	str r2, [r3]
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	ldr r4, [sp, #32]
	movs r0, #240
	ldr r3, [r4]
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r3, r0
	movs r1, #1
	str r1, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #20]
	movs r3, #128
	movs r4, #1
	lsls r3, r3, #1
	add r11, r4
	adds r2, r2, r3
	mov r0, r11
	str r2, [sp, #20]
	cmp r0, #54
	beq .L_08165a78
	b .L_081656ce
.L_08165a78:
	ldr r0, .L_08165ab0
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #76]
	cmp r1, #0
	bne .L_08165a9c
	ldr r2, [sp, #60]
	movs r4, #238
	ldr r3, [r2]
	lsls r4, r4, #7
	adds r4, #220
	adds r3, r3, r4
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
.L_08165a9c:
	bl Func_08143bb8
	add sp, #280
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08165ab0:
	.4byte Func_08143000
