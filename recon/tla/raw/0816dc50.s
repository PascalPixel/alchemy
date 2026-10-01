.syntax unified
	.thumb
	.global Func_0816dc50
	.thumb_func
Func_0816dc50:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #160
	str r0, [sp, #64]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	str r0, [sp, #60]
	ldr r3, [r3, #92]
	str r3, [sp, #56]
	bl Func_0813ba50
	ldr r1, [sp, #64]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816dc84
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginTiledCanvasFilled
	b .L_0816dc8c
.L_0816dc84:
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginTiledCanvas
.L_0816dc8c:
	ldr r2, [sp, #64]
	movs r3, #2
	ldr r1, [r2, #4]
	adds r3, #255
	lsls r1, r1, #4
	add r2, sp, #136
	ldr r0, [sp, #64]
	orrs r1, r3
	add r3, sp, #148
	bl Func_0815585c
	movs r1, #240
	ldr r5, .L_0816dcfc
	lsls r1, r1, #6
	ldr r0, .L_0816dd00
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #60]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r3, .L_0816dcf4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	ldr r5, .L_0816dcf8
	movs r6, #128
	strh r3, [r2]
	lsls r6, r6, #19
	mov r3, sp
	adds r3, #80
	adds r6, #32
	strh r5, [r6]
	adds r1, r3, #0
	movs r0, #0
	str r3, [sp, #48]
	bl Func_08144aac
	ldr r4, [sp, #56]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r4, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r4, r1
	movs r3, #50
	movs r1, #200
	b .L_0816dd04
.L_0816dcf4:
	.4byte 0x00001010
.L_0816dcf8:
	.4byte 0x00000100
.L_0816dcfc:
	.4byte IwramClearWords
.L_0816dd00:
	.4byte 0x06004000
.L_0816dd04:
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816df34
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_0816df38
	ldr r1, .L_0816df3c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #56]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0816df40
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #56]
	movs r2, #176
	lsls r2, r2, #4
	adds r1, r4, r2
	ldr r0, .L_0816df44
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #56]
	movs r4, #142
	lsls r4, r4, #7
	adds r1, r3, r4
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0816df48
	bl Resource_LoadAndDecompress
	ldr r0, .L_0816df4c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_0816df50
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	str r3, [sp, #44]
	movs r1, #54
	ldrsh r0, [r3, r1]
	ldr r3, [sp, #64]
	str r0, [sp, #40]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlotFar
	movs r2, #128
	ldr r0, [r0]
	lsls r2, r2, #19
	movs r3, #0
	adds r2, #40
	str r0, [sp, #36]
	strh r5, [r6]
	str r3, [r2]
	ldr r4, [sp, #44]
	mov r5, sp
	mov r6, sp
	adds r4, #12
	adds r5, #112
	adds r6, #124
	str r3, [sp, #52]
	str r4, [sp, #16]
	str r5, [sp, #24]
	str r6, [sp, #20]
.L_0816dda0:
	ldr r0, [sp, #52]
	cmp r0, #0
	beq .L_0816dda8
	b .L_0816ded4
.L_0816dda8:
	add r1, sp, #40
	ldr r2, [sp, #44]
	ldrh r1, [r1]
	movs r3, #7
	strh r1, [r2, #54]
	movs r2, #0
	mov r9, r2
	mov r11, r3
.L_0816ddb8:
	mov r5, r9
	movs r4, #0
	lsls r5, r5, #3
	mov r8, r4
	mov r10, r5
.L_0816ddc2:
	mov r6, r9
	cmp r6, #0
	bne .L_0816ddd2
	ldr r3, .L_0816df54
	ldrsb r1, [r3, r6]
	ldr r3, .L_0816df58
	ldrsb r2, [r3, r6]
	b .L_0816ddf6
.L_0816ddd2:
	bl Random16
	ldr r3, .L_0816df54
	mov r1, r9
	ldrsb r3, [r3, r1]
	movs r2, #3
	ands r0, r2
	adds r1, r3, r0
	str r1, [sp, #12]
	bl Random16
	ldr r3, .L_0816df58
	mov r4, r9
	ldrsb r3, [r3, r4]
	movs r5, #3
	ldr r1, [sp, #12]
	ands r0, r5
	adds r2, r3, r0
.L_0816ddf6:
	mov r0, r8
	movs r3, #128
	lsls r3, r3, #5
	lsls r6, r0, #13
	adds r6, r6, r3
	adds r0, r6, #0
	str r1, [sp, #12]
	str r2, [sp, #8]
	bl Trig_Sin
	ldr r2, [sp, #8]
	ldr r4, .L_0816df5c
	adds r3, r2, #0
	muls r3, r0
	mov r7, r10
	ldr r1, [sp, #12]
	add r7, r8
	lsls r5, r7, #2
	adds r5, r5, r4
	asrs r3, r3, #16
	strb r3, [r5]
	negs r3, r1
	strb r3, [r5, #1]
	adds r0, r6, #0
	bl Trig_Cos
	ldr r2, [sp, #8]
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r5, #2]
	mov r5, r9
	cmp r5, #6
	bgt .L_0816deae
	ldr r6, .L_0816df60
	lsls r3, r7, #1
	mov r1, r8
	adds r3, r3, r7
	lsls r2, r1, #4
	lsls r3, r3, #3
	adds r0, r3, r6
	mov r5, r11
	adds r1, r2, #0
	adds r6, #12
	strb r5, [r0, #9]
	strb r5, [r0, #7]
	adds r1, #15
	adds r5, r3, r6
	mov r4, r10
	mov r6, r8
	strb r2, [r0, #8]
	strb r1, [r0, #4]
	strb r4, [r0, #5]
	strb r1, [r0, #6]
	adds r6, #1
	strb r2, [r5, #4]
	strb r2, [r5, #8]
	mov r2, r11
	strb r4, [r5, #5]
	strb r2, [r5, #9]
	strb r1, [r5, #6]
	strb r4, [r5, #7]
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0816de7c
	mov r3, r8
	adds r3, #8
.L_0816de7c:
	asrs r3, r3, #3
	lsls r3, r3, #3
	subs r3, r6, r3
	mov r1, r10
	adds r4, r3, r1
	mov r3, r8
	strb r4, [r0]
	cmp r3, #0
	bge .L_0816de90
	adds r3, #7
.L_0816de90:
	asrs r3, r3, #3
	mov r2, r8
	lsls r3, r3, #3
	subs r3, r2, r3
	add r3, r10
	adds r1, r3, #0
	adds r2, r4, #0
	adds r1, #8
	adds r2, #8
	strb r1, [r0, #1]
	strb r2, [r0, #2]
	strb r3, [r5]
	strb r1, [r5, #1]
	strb r4, [r5, #2]
	b .L_0816deb2
.L_0816deae:
	mov r6, r8
	adds r6, #1
.L_0816deb2:
	mov r8, r6
	cmp r6, #8
	beq .L_0816deba
	b .L_0816ddc2
.L_0816deba:
	movs r4, #1
	add r9, r4
	movs r3, #8
	mov r5, r9
	add r11, r3
	cmp r5, #8
	beq .L_0816deca
	b .L_0816ddb8
.L_0816deca:
	ldr r3, .L_0816df64
	movs r6, #0
	strb r6, [r3]
	strb r6, [r3, #1]
	strb r6, [r3, #2]
.L_0816ded4:
	ldr r3, [sp, #52]
	subs r3, #4
	cmp r3, #79
	bhi .L_0816df80
	ldr r0, [sp, #52]
	movs r2, #160
	lsls r2, r2, #1
	cmp r0, #63
	ble .L_0816def4
	ldr r1, [sp, #52]
	movs r2, #200
	lsls r3, r1, #2
	adds r3, r3, r1
	lsls r3, r3, #2
	lsls r2, r2, #3
	subs r2, r2, r3
.L_0816def4:
	ldr r4, [sp, #64]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0816df08
	ldr r5, [sp, #44]
	ldrh r3, [r5, #54]
	adds r6, r5, #0
	subs r3, r3, r2
	strh r3, [r6, #54]
	b .L_0816df12
.L_0816df08:
	ldr r0, [sp, #44]
	ldrh r3, [r0, #54]
	adds r1, r0, #0
	adds r3, r3, r2
	strh r3, [r1, #54]
.L_0816df12:
	ldr r2, [sp, #64]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0816df6c
	ldr r5, [sp, #44]
	ldr r6, .L_0816df68
	movs r4, #54
	ldrsh r3, [r5, r4]
	cmp r3, r6
	bge .L_0816df80
	movs r3, #201
	lsls r3, r3, #8
	adds r3, #80
	adds r0, r5, #0
	strh r3, [r0, #54]
	b .L_0816df80
	.2byte 0x0000
.L_0816df34:
	.4byte Func_08143000
.L_0816df38:
	.4byte 0x000000c1
.L_0816df3c:
	.4byte Data_02014000
.L_0816df40:
	.4byte 0x000000cd
.L_0816df44:
	.4byte 0x000000cf
.L_0816df48:
	.4byte 0x00000154
.L_0816df4c:
	.4byte 0x00000130
.L_0816df50:
	.4byte IwramCopyWords
.L_0816df54:
	.4byte Data_08198b7a
.L_0816df58:
	.4byte Data_08198b82
.L_0816df5c:
	.4byte gMapCellBuffer
.L_0816df60:
	.4byte Data_02012000
.L_0816df64:
	.4byte Data_02012540
.L_0816df68:
	.4byte 0xffffc950
.L_0816df6c:
	ldr r2, [sp, #44]
	movs r1, #54
	ldrsh r3, [r2, r1]
	movs r2, #216
	lsls r2, r2, #6
	adds r2, #176
	cmp r3, r2
	ble .L_0816df80
	ldr r3, [sp, #44]
	strh r2, [r3, #54]
.L_0816df80:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	ldr r5, .L_0816e334
	mov r9, r0
	ldr r4, [r5, #12]
	str r4, [sp, #32]
	ldr r6, [r5, #16]
	adds r3, r6, #0
	subs r3, #16
	str r6, [sp, #28]
	str r3, [r5, #16]
	bl Func_08014de4
	ldr r0, [sp, #44]
	ldr r1, [sp, #16]
	bl Graphics_PrepareTransferInIwramWork
	ldr r0, [sp, #36]
	ldr r1, [sp, #24]
	ldr r3, [r0, #8]
	movs r2, #0
	str r2, [r1, #4]
	str r3, [r1]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	bl Render_ProjectPoint
	ldr r3, [sp, #20]
	ldr r2, [r3]
	cmp r2, #63
	bgt .L_0816dfd6
	ldr r4, [sp, #20]
	movs r3, #64
	str r3, [r4]
	movs r2, #64
.L_0816dfd6:
	cmp r2, #176
	ble .L_0816dfe2
	ldr r6, [sp, #20]
	movs r3, #176
	str r3, [r6]
	movs r2, #176
.L_0816dfe2:
	ldr r1, .L_0816e338
	movs r3, #184
	subs r3, r3, r2
	str r3, [r5, #12]
	ldr r0, .L_0816e33c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_0816e020
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	ldr r3, [sp, #20]
	lsls r2, r2, #2
	adds r2, r2, r1
	ldr r1, [r3]
	movs r3, #64
	subs r3, r3, r1
	adds r2, #4
	lsls r3, r3, #8
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	stmia r2!, {r3}
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2]
.L_0816e020:
	strh r4, [r0]
	ldr r4, [sp, #52]
	cmp r4, #63
	bhi .L_0816e0f2
	cmp r4, #0
	bne .L_0816e094
	ldr r7, [sp, #56]
	movs r5, #0
	mov r8, r5
	mov r10, r5
.L_0816e034:
	bl Random16
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #255
	ands r5, r0
	bl Random16
	movs r6, #128
	lsls r6, r6, #3
	adds r5, r5, r6
	movs r3, #255
	adds r6, r0, #0
	ldr r0, [sp, #36]
	lsls r3, r3, #8
	adds r3, #255
	ands r6, r3
	ldr r3, [r0, #8]
	mov r1, r10
	str r1, [r7, #4]
	str r3, [r7]
	ldr r3, [r0, #16]
	adds r0, r6, #0
	str r3, [r7, #8]
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	mov r2, r10
	asrs r3, r3, #9
	str r3, [r7, #12]
	str r2, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r5, #1
	asrs r3, r3, #9
	mov r4, r8
	add r8, r5
	str r3, [r7, #20]
	mov r6, r8
	negs r3, r4
	str r3, [r7, #24]
	adds r7, #28
	cmp r6, #32
	bne .L_0816e034
.L_0816e094:
	ldr r6, [sp, #56]
	movs r0, #0
	mov r8, r0
	add r7, sp, #100
.L_0816e09c:
	ldr r3, [r6, #24]
	adds r0, r3, #1
	str r0, [r6, #24]
	cmp r0, #23
	bhi .L_0816e0e6
	movs r1, #6
	bl Math_Div
	adds r1, r7, #0
	adds r5, r0, #0
	adds r0, r6, #0
	bl Render_ProjectPoint
	movs r2, #128
	adds r0, r6, #0
	movs r1, #60
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector3
	ldr r1, [sp, #56]
	ldr r2, [r7]
	ldr r3, [r7, #4]
	lsls r5, r5, #10
	movs r4, #176
	adds r5, r1, r5
	lsls r4, r4, #4
	movs r1, #32
	adds r5, r5, r4
	str r1, [sp, #0]
	str r1, [sp, #4]
	subs r2, #16
	subs r3, #16
	ldr r4, [sp, #80]
	ldr r0, [sp, #60]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_0816e0e6:
	movs r5, #1
	add r8, r5
	mov r0, r8
	adds r6, #28
	cmp r0, #32
	bne .L_0816e09c
.L_0816e0f2:
	ldr r1, [sp, #52]
	cmp r1, #29
	bgt .L_0816e144
	ldr r2, [sp, #36]
	add r6, sp, #100
	ldr r3, [r2, #8]
	lsls r2, r1, #17
	str r3, [r6]
	movs r3, #240
	lsls r3, r3, #14
	subs r3, r3, r2
	str r3, [r6, #4]
	lsls r0, r1, #9
	bl Trig_Sin
	ldr r4, [sp, #36]
	lsls r0, r0, #5
	ldr r3, [r4, #16]
	add r5, sp, #88
	adds r3, r3, r0
	str r3, [r6, #8]
	adds r1, r5, #0
	adds r0, r6, #0
	bl Render_ProjectPoint
	movs r1, #40
	ldr r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	str r1, [sp, #4]
	ldr r5, [sp, #48]
	ldr r6, [sp, #56]
	ldr r4, [r5, #4]
	movs r5, #142
	lsls r5, r5, #7
	subs r2, #20
	subs r3, #30
	ldr r0, [sp, #60]
	adds r1, r6, r5
	mov lr, r4
	.2byte 0xf800
.L_0816e144:
	ldr r6, [sp, #52]
	cmp r6, #4
	bne .L_0816e150
	movs r0, #156
	bl Audio_PlayCue
.L_0816e150:
	ldr r0, [sp, #52]
	cmp r0, #30
	bne .L_0816e15c
	movs r0, #139
	bl Audio_PlayCue
.L_0816e15c:
	ldr r1, [sp, #52]
	cmp r1, #60
	bne .L_0816e168
	movs r0, #212
	bl Audio_PlayCue
.L_0816e168:
	ldr r2, [sp, #52]
	cmp r2, #64
	bne .L_0816e17c
	movs r0, #134
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
.L_0816e17c:
	ldr r3, [sp, #52]
	cmp r3, #30
	bne .L_0816e198
	ldr r5, [sp, #64]
	movs r3, #1
	movs r4, #36
	ldrsh r0, [r5, r4]
	movs r6, #0
	movs r1, #9
	movs r2, #5
	negs r3, r3
	str r6, [sp, #0]
	bl Func_0814cd48
.L_0816e198:
	ldr r0, [sp, #52]
	cmp r0, #26
	bne .L_0816e1ac
	ldr r1, [sp, #56]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #8
	str r3, [r2]
.L_0816e1ac:
	ldr r4, [sp, #52]
	cmp r4, #64
	bne .L_0816e1d2
	ldr r5, [sp, #56]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #168
	movs r2, #4
	adds r3, r5, r6
	str r2, [r3]
	ldr r3, [sp, #64]
	movs r1, #36
	ldrsh r0, [r3, r1]
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_0816e1d2:
	ldr r4, [sp, #52]
	cmp r4, #70
	bne .L_0816e1ec
	ldr r6, [sp, #64]
	movs r3, #4
	movs r5, #36
	ldrsh r0, [r6, r5]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_0816e1ec:
	ldr r0, [sp, #52]
	cmp r0, #56
	bne .L_0816e292
	ldr r1, [sp, #64]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #64]
	ldr r6, [r0]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	ldr r4, [r6, #8]
	ldr r3, [r2, #8]
	movs r1, #100
	subs r3, r3, r4
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #4
	mov r10, r4
	str r2, [sp, #8]
	bl Math_Div
	ldr r2, [sp, #8]
	adds r5, r0, #0
	ldr r3, [r2, #16]
	ldr r0, [r6, #16]
	movs r1, #100
	subs r3, r3, r0
	mov r8, r0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #4
	bl Math_Div
	add r10, r5
	add r8, r0
	asrs r5, r5, #8
	asrs r0, r0, #8
	adds r3, r0, #0
	muls r3, r0
	adds r2, r5, #0
	muls r2, r5
	adds r2, r2, r3
	adds r0, r2, #0
	ldr r3, .L_0816e340
	mov lr, r3
	.2byte 0xf800
	movs r1, #10
	lsls r0, r0, #8
	bl Math_Div
	adds r3, r6, #0
	movs r2, #1
	adds r3, #88
	str r0, [r6, #52]
	str r0, [r6, #48]
	strb r2, [r3]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	ldr r3, .L_0816e344
	movs r1, #0
	str r3, [r6, #72]
	adds r3, r6, #0
	adds r3, #90
	str r1, [r6, #68]
	adds r0, r6, #0
	strb r2, [r3]
	bl Object_ResetMotion
	adds r0, r6, #0
	mov r1, r10
	movs r2, #0
	mov r3, r8
	bl Object_SetPosition
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetMode
.L_0816e292:
	ldr r2, [sp, #52]
	cmp r2, #63
	bgt .L_0816e29a
	b .L_0816e412
.L_0816e29a:
	cmp r2, #64
	bne .L_0816e388
	movs r1, #128
	ldr r3, .L_0816e348
	ldr r0, [sp, #60]
	lsls r1, r1, #7
	ldr r2, .L_0816e34c
	mov lr, r3
	.2byte 0xf800
	movs r4, #255
	lsls r4, r4, #8
	ldr r6, [sp, #56]
	movs r3, #0
	adds r4, #255
	mov r8, r3
	mov r10, r4
.L_0816e2ba:
	movs r5, #7
	mov r3, r8
	ands r3, r5
	lsls r7, r3, #13
	adds r0, r7, #0
	mov r5, r8
	bl Trig_Sin
	cmp r5, #0
	bge .L_0816e2d0
	adds r5, #7
.L_0816e2d0:
	asrs r5, r5, #3
	adds r2, r5, #0
	muls r2, r0
	ldr r0, [sp, #36]
	lsls r2, r2, #2
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r6]
	bl Random16
	mov r1, r10
	lsls r3, r5, #2
	ands r0, r1
	adds r3, r3, r5
	lsls r3, r3, #17
	lsls r0, r0, #2
	movs r2, #128
	subs r0, r0, r3
	lsls r2, r2, #15
	adds r0, r0, r2
	str r0, [r6, #4]
	adds r0, r7, #0
	bl Trig_Cos
	ldr r4, [sp, #36]
	adds r2, r5, #0
	muls r2, r0
	ldr r3, [r4, #16]
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #8]
	bl Random16
	mov r5, r10
	ands r0, r5
	movs r1, #128
	lsls r1, r1, #9
	lsls r0, r0, #1
	adds r0, r0, r1
	str r0, [r6, #12]
	bl Random16
	ldr r2, .L_0816e350
	ands r0, r5
	adds r0, r0, r2
	lsls r3, r0, #1
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0816e354
	.2byte 0x0000
.L_0816e334:
	.4byte gCameraSceneParameters
.L_0816e338:
	.4byte gIoWriteQueue
.L_0816e33c:
	.4byte 0x04000208
.L_0816e340:
	.4byte IwramFillWords + 0x74
.L_0816e344:
	.4byte 0x00013333
.L_0816e348:
	.4byte IwramFillWords
.L_0816e34c:
	.4byte 0x3f3f3f3f
.L_0816e350:
	.4byte 0xffffce20
.L_0816e354:
	bl Random16
	ldr r3, .L_0816e600
	ands r0, r5
	adds r0, r0, r3
	ldr r3, [r6]
	lsls r0, r0, #1
	str r0, [r6, #20]
	cmp r3, #0
	bge .L_0816e36e
	ldr r3, [r6, #12]
	negs r3, r3
	str r3, [r6, #12]
.L_0816e36e:
	bl Random16
	movs r3, #31
	movs r4, #1
	ands r3, r0
	add r8, r4
	adds r3, #32
	mov r5, r8
	str r3, [r6, #24]
	adds r6, #28
	cmp r5, #64
	bne .L_0816e2ba
	b .L_0816e40a
.L_0816e388:
	mov r6, r9
	movs r3, #2
	str r3, [r6]
	ldr r3, .L_0816e604
	movs r0, #0
	mov r1, r11
	str r0, [r6, #4]
	str r3, [r6, #8]
	str r1, [r6, #12]
	ldr r2, [sp, #52]
	ldr r5, [sp, #56]
	mov r7, sp
	mov r8, r0
	adds r7, #79
	lsls r6, r2, #10
.L_0816e3a6:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0816e3f8
	strb r3, [r7]
	subs r3, #2
	str r3, [r5, #24]
	movs r2, #128
	mov r3, r9
	str r7, [r3, #20]
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #3
	bl BattleFxKernels_IntegrateVector3
	bl Func_08014e38
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r0, [r5]
	bl Func_08015160
	adds r0, r6, #0
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	bl Func_080150e4
	movs r0, #128
	lsls r0, r0, #9
	bl Func_0801521c
	ldr r0, .L_0816e608
	mov r1, r11
	movs r2, #3
	bl Func_081969ac
	mov r0, r9
	bl Func_08196a7c
	bl Func_08014ea8
.L_0816e3f8:
	movs r0, #1
	movs r4, #128
	add r8, r0
	lsls r4, r4, #3
	mov r1, r8
	adds r6, r6, r4
	adds r5, #28
	cmp r1, #64
	bne .L_0816e3a6
.L_0816e40a:
	ldr r2, [sp, #52]
	cmp r2, #63
	ble .L_0816e412
	b .L_0816e598
.L_0816e412:
	ldr r3, [sp, #52]
	cmp r3, #25
	bgt .L_0816e41a
	b .L_0816e598
.L_0816e41a:
	ldr r3, [sp, #68]
	ldr r2, .L_0816e60c
	ldr r4, [sp, #56]
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0816e610
	movs r5, #224
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	lsls r5, r5, #3
	str r3, [sp, #68]
	add r6, sp, #68
	adds r3, r4, r5
	mov r0, r9
	str r3, [r6, #4]
	ldr r1, .L_0816e614
	movs r3, #6
	str r3, [r0]
	movs r3, #3
	str r3, [r0, #4]
	mov r2, r11
	movs r3, #0
	str r2, [r0, #12]
	strb r3, [r0, #24]
	str r6, [r0, #16]
	str r1, [r0, #8]
	ldr r4, [sp, #52]
	movs r2, #31
	asrs r7, r4, #31
	lsrs r3, r7, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	ands r3, r2
	negs r3, r3
	strb r3, [r0, #25]
	bl Func_08014e38
	ldr r5, [sp, #52]
	movs r2, #0
	cmp r5, #31
	bgt .L_0816e480
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #16
	muls r3, r5
	ldr r0, .L_0816e618
	adds r5, r3, r0
	b .L_0816e486
.L_0816e480:
	movs r5, #234
	lsls r5, r5, #8
	adds r5, #96
.L_0816e486:
	mov r1, r9
	str r2, [r1, #20]
	ldr r2, [sp, #36]
	ldr r1, .L_0816e61c
	ldr r0, [r2, #8]
	ldr r2, [r2, #16]
	bl Func_08015160
	adds r0, r5, #0
	adds r1, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	mov r1, r11
	movs r2, #64
	ldr r0, .L_0816e620
	bl Func_081969ac
	mov r0, r9
	bl Func_08196a7c
	movs r2, #32
	mov r3, r9
	movs r4, #7
	negs r2, r2
	movs r5, #0
	str r4, [r3]
	strb r5, [r3, #24]
	str r2, [r3, #20]
	ldr r0, [sp, #52]
	lsrs r3, r7, #31
	adds r3, r0, r3
	movs r2, #31
	asrs r3, r3, #1
	ands r3, r2
	mov r1, r9
	strb r3, [r1, #25]
	movs r3, #2
	str r3, [r1, #4]
	mov r0, r9
	bl Func_08196a7c
	bl Func_08014ea8
	ldr r4, .L_0816e624
	movs r2, #7
	strb r2, [r6]
	add r3, sp, #68
	mov r6, r9
	strb r2, [r3, #1]
	str r4, [r3, #4]
	str r3, [r6, #16]
	ldr r3, .L_0816e628
	movs r0, #0
	movs r5, #7
	mov r1, r11
	movs r2, #0
	str r3, [r6, #8]
	str r5, [r6]
	str r0, [r6, #4]
	str r1, [r6, #12]
	strb r0, [r6, #24]
	strb r2, [r6, #25]
	ldr r7, [sp, #52]
	movs r3, #0
	mov r8, r3
.L_0816e50a:
	ldr r3, .L_0816e62c
	mov r4, r8
	ldrb r3, [r3, r4]
	ldr r5, [sp, #52]
	adds r1, r3, #0
	adds r1, #22
	cmp r5, r1
	ble .L_0816e58c
	ldr r3, .L_0816e630
	subs r2, r5, r1
	ldrb r3, [r3, r4]
	movs r0, #131
	muls r2, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	muls r3, r2
	lsls r0, r0, #7
	adds r6, r3, r0
	subs r3, r1, r5
	lsls r3, r3, #3
	adds r5, r3, #0
	adds r5, #56
	cmp r5, #0
	ble .L_0816e53e
	movs r5, #0
.L_0816e53e:
	movs r1, #64
	negs r1, r1
	cmp r5, r1
	ble .L_0816e58c
	bl Func_08014e38
	mov r2, r9
	str r5, [r2, #20]
	ldr r3, [sp, #36]
	mov r4, r8
	ldr r0, [r3, #8]
	ldr r3, .L_0816e634
	ldr r5, [sp, #36]
	ldrsb r1, [r3, r4]
	ldr r2, [r5, #16]
	lsls r1, r1, #16
	bl Func_08015160
	adds r0, r6, #0
	adds r1, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	movs r6, #7
	adds r3, r7, #0
	ands r3, r6
	lsls r3, r3, #4
	mov r0, r9
	strb r3, [r0, #24]
	mov r1, r11
	ldr r0, .L_0816e638
	movs r2, #32
	bl Func_081969ac
	mov r0, r9
	bl Func_08196a7c
	bl Func_08014ea8
.L_0816e58c:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r7, #5
	cmp r2, #4
	bne .L_0816e50a
.L_0816e598:
	mov r0, r9
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
	ldr r3, .L_0816e63c
	ldr r4, [sp, #32]
	movs r1, #8
	str r4, [r3, #12]
	ldr r5, [sp, #28]
	movs r0, #8
	str r5, [r3, #16]
	bl Func_08158ce0
	bl Func_081434f8
	movs r0, #240
	ldr r6, [sp, #56]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r6, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #52]
	adds r1, #1
	str r1, [sp, #52]
	cmp r1, #88
	beq .L_0816e5dc
	bl .L_0816dda0
.L_0816e5dc:
	ldr r0, .L_0816e640
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #160
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816e600:
	.4byte 0xffff8000
.L_0816e604:
	.4byte Data_0819919c
.L_0816e608:
	.4byte Data_081991a4
.L_0816e60c:
	.4byte 0xffffff00
.L_0816e610:
	.4byte 0xffff00ff
.L_0816e614:
	.4byte Data_02012000
.L_0816e618:
	.4byte 0xfffc0860
.L_0816e61c:
	.4byte 0xfffc0000
.L_0816e620:
	.4byte gMapCellBuffer
.L_0816e624:
	.4byte Data_02014000
.L_0816e628:
	.4byte Data_08198ec4
.L_0816e62c:
	.4byte Data_08198b8a
.L_0816e630:
	.4byte Data_08198b8e
.L_0816e634:
	.4byte Data_08198b92
.L_0816e638:
	.4byte Data_08198cac
.L_0816e63c:
	.4byte gCameraSceneParameters
.L_0816e640:
	.4byte Func_08143000
