.syntax unified
	.thumb
	.global Func_08186dfc
	.thumb_func
Func_08186dfc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #192
	str r0, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	movs r7, #239
	str r0, [sp, #96]
	movs r0, #0
	ldr r1, [r3, #96]
	lsls r7, r7, #7
	str r1, [sp, #92]
	ldr r2, [r3, #92]
	str r2, [sp, #88]
	ldr r4, [r3, #100]
	adds r3, #176
	str r4, [sp, #72]
	ldr r3, [r3]
	str r3, [sp, #68]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	ldr r2, .L_08186e70
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r5, [sp, #88]
	movs r1, #200
	adds r6, r5, r7
	movs r5, #0
	str r5, [r6]
	lsls r1, r1, #4
	ldr r0, .L_08186e78
	mov r8, r1
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #1
	bl Func_08163c2c
	ldr r3, .L_08186e74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r3, #128
	subs r2, #40
	lsls r3, r3, #3
	b .L_08186e7c
.L_08186e70:
	.4byte 0x00000000
.L_08186e74:
	.4byte 0x00003f40
.L_08186e78:
	.4byte Func_08143000
.L_08186e7c:
	str r3, [r2]
	ldr r2, .L_08187070
	movs r3, #240
	str r3, [r2, #16]
	ldr r2, [sp, #88]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #240
	adds r3, r2, r4
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r3, .L_08187074
	ldr r7, [sp, #68]
	movs r2, #1
	str r2, [r7, #16]
	strh r5, [r3, #4]
	str r2, [r6]
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r3, r0, r1
	str r5, [r3]
	mov r1, r8
	ldr r0, .L_08187078
	bl Scheduler_AddOrUpdateCallback
	movs r1, #202
	movs r2, #0
	lsls r1, r1, #1
	str r2, [sp, #48]
	str r2, [sp, #44]
	str r2, [sp, #40]
	adds r1, #255
	movs r0, #2
	movs r2, #2
	movs r5, #240
	bl Func_08152404
	movs r7, #13
	movs r3, #0
	lsls r5, r5, #7
	mov r8, r3
	negs r7, r7
	movs r6, #4
	adds r5, #12
.L_08186eda:
	movs r0, #165
	lsls r0, r0, #2
	bl Func_08020040
	ldr r4, [sp, #88]
	str r0, [r5, r4]
	cmp r0, #0
	beq .L_08186f0c
	movs r3, #0
	strb r3, [r0, #26]
	movs r1, #0
	bl Animation_ApplyChildArgumentFar
	ldr r0, [sp, #88]
	adds r3, r7, #0
	ldr r1, [r5, r0]
	ldrb r2, [r1, #9]
	ands r3, r2
	ldrb r2, [r1, #5]
	orrs r3, r6
	strb r3, [r1, #9]
	adds r3, r7, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #5]
.L_08186f0c:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #4
	cmp r2, #4
	bne .L_08186eda
	movs r5, #240
	movs r4, #13
	movs r3, #0
	lsls r5, r5, #7
	negs r4, r4
	mov r8, r3
	adds r5, #28
	adds r6, r4, #0
.L_08186f28:
	movs r0, #203
	lsls r0, r0, #1
	adds r0, #255
	bl Func_08020040
	ldr r7, [sp, #88]
	str r0, [r5, r7]
	cmp r0, #0
	beq .L_08186f50
	movs r3, #0
	strb r3, [r0, #26]
	movs r1, #0
	bl Animation_ApplyChildArgumentFar
	ldr r1, [r5, r7]
	movs r2, #4
	ldrb r3, [r1, #9]
	ands r3, r6
	orrs r3, r2
	strb r3, [r1, #9]
.L_08186f50:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #4
	cmp r1, #2
	bne .L_08186f28
	ldr r2, .L_0818707c
	movs r1, #64
	movs r3, #0
	movs r0, #64
	bl Func_0815b290
	movs r4, #240
	ldr r2, [sp, #88]
	lsls r4, r4, #7
	adds r4, #36
	adds r3, r2, r4
	ldrb r2, [r0, #5]
	str r0, [r3]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r0, #5]
	ldrb r3, [r0, #16]
	ldr r2, .L_08187080
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r5, [sp, #88]
	ldrh r3, [r3, #2]
	ldr r0, .L_08187084
	movs r7, #246
	lsls r7, r7, #7
	adds r7, #116
	adds r1, r5, r7
	adds r3, r3, r0
	str r3, [r1]
	ldr r1, [sp, #88]
	movs r2, #224
	lsls r2, r2, #3
	adds r5, r1, r2
	ldr r0, .L_08187088
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r1, .L_0818708c
	adds r0, r5, #0
	movs r2, #64
	movs r3, #64
	bl Func_0816ae40
	movs r2, #1
	movs r3, #0
	ldr r0, .L_08187090
	adds r1, r5, #0
	bl Func_08157cf4
	movs r2, #184
	movs r3, #0
	lsls r2, r2, #5
	mov r8, r3
	adds r2, #129
.L_08186fd2:
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_08186fde
	lsrs r3, r3, #2
	adds r3, #224
	strb r3, [r5]
.L_08186fde:
	movs r4, #1
	add r8, r4
	adds r5, #1
	cmp r8, r2
	bne .L_08186fd2
	ldr r7, [sp, #88]
	movs r1, #142
	lsls r1, r1, #7
	adds r5, r7, r1
	adds r1, r5, #0
	movs r2, #0
	ldr r0, .L_08187094
	movs r3, #0
	bl Func_08157cf4
	movs r2, #0
	ldr r1, .L_08187098
	mov r8, r2
	movs r2, #128
	lsls r2, r2, #3
.L_08187006:
	ldr r3, [r5]
	lsrs r3, r3, #1
	ands r3, r1
	stmia r5!, {r3}
	movs r3, #1
	add r8, r3
	cmp r8, r2
	bne .L_08187006
	ldr r2, .L_0818709c
	movs r4, #0
	mov r8, r4
	movs r0, #2
	movs r4, #3
	movs r1, #1
.L_08187022:
	mov r5, r8
	movs r7, #1
	lsls r3, r5, #1
	add r8, r7
	strb r3, [r2]
	mov r3, r8
	strb r1, [r2, #1]
	strb r0, [r2, #2]
	strb r1, [r2, #4]
	strb r4, [r2, #5]
	strb r0, [r2, #6]
	adds r4, #2
	adds r0, #2
	adds r1, #2
	adds r2, #8
	cmp r3, #31
	bne .L_08187022
	ldr r2, .L_081870a0
	movs r3, #0
	movs r4, #160
	strb r3, [r2]
	strb r3, [r2, #1]
	strb r3, [r2, #2]
	lsls r4, r4, #19
	movs r5, #1
	adds r4, #2
	mov r8, r5
.L_08187058:
	mov r7, r8
	cmp r7, #31
	bgt .L_081870a4
	movs r0, #0
	movs r2, #0
	mov r1, r8
	cmp r7, #0
	bge .L_0818706a
	movs r1, #0
.L_0818706a:
	strh r1, [r4]
	b .L_081870ba
	.2byte 0x0000
.L_08187070:
	.4byte gCameraSceneParameters
.L_08187074:
	.4byte Data_03001120
.L_08187078:
	.4byte Func_08143114
.L_0818707c:
	.4byte 0xc0002000
.L_08187080:
	.4byte ResourceTableEntries
.L_08187084:
	.4byte 0x06010000
.L_08187088:
	.4byte 0x000000f7
.L_0818708c:
	.4byte Data_02014000
.L_08187090:
	.4byte 0x00000178
.L_08187094:
	.4byte 0x000000b4
.L_08187098:
	.4byte 0x7f7f7f7f
.L_0818709c:
	.4byte Data_02010ae8
.L_081870a0:
	.4byte Data_02010be0
.L_081870a4:
	mov r3, r8
	subs r3, #32
	lsls r0, r3, #2
	adds r1, r0, #0
	adds r2, r3, #0
	cmp r1, #31
	ble .L_081870b4
	movs r1, #31
.L_081870b4:
	cmp r0, #31
	ble .L_081870ba
	movs r0, #31
.L_081870ba:
	lsls r3, r0, #10
	lsls r2, r2, #5
	movs r0, #1
	orrs r3, r2
	add r8, r0
	orrs r3, r1
	mov r1, r8
	strh r3, [r4]
	adds r4, #2
	cmp r1, #64
	bne .L_08187058
	ldr r4, .L_08187398
	movs r2, #0
	mov r8, r2
	movs r5, #64
	movs r0, #0
.L_081870da:
	mov r7, r8
	lsls r3, r7, #1
	adds r1, r3, #2
	adds r2, r0, #2
	cmp r1, #31
	ble .L_081870e8
	movs r1, #31
.L_081870e8:
	cmp r2, #31
	ble .L_081870ee
	movs r2, #31
.L_081870ee:
	lsls r3, r2, #10
	orrs r3, r5
	orrs r3, r1
	movs r1, #1
	add r8, r1
	mov r2, r8
	strh r3, [r4]
	adds r5, #32
	adds r0, #3
	adds r4, #2
	cmp r2, #16
	bne .L_081870da
	movs r2, #0
	ldr r1, .L_0818739c
	movs r0, #1
	bl Func_08118040
	movs r3, #0
	str r3, [sp, #76]
	ldr r3, .L_081873a0
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08187124
	bl .L_08187e52
.L_08187124:
	ldr r4, .L_081873a4
	ldr r5, .L_081873a8
	str r4, [sp, #16]
	str r5, [sp, #12]
.L_0818712c:
	ldr r7, [sp, #76]
	cmp r7, #0
	beq .L_08187134
	b .L_08187260
.L_08187134:
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r3, r0, r1
	str r7, [r3]
	ldr r5, [sp, #96]
	movs r2, #216
	movs r7, #160
	lsls r2, r2, #15
	movs r3, #240
	movs r0, #160
	lsls r7, r7, #3
	movs r4, #0
	str r2, [sp, #60]
	lsls r3, r3, #15
	lsls r0, r0, #19
	adds r7, #108
	movs r2, #128
	str r3, [sp, #64]
	lsls r2, r2, #1
	ldr r3, .L_081873ac
	str r4, [sp, #52]
	str r4, [sp, #56]
	adds r0, #192
	adds r1, r5, r7
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_081873b0
	movs r0, #0
	mov r8, r0
	movs r2, #1
.L_08187174:
	movs r1, #1
	add r8, r1
	mov r4, r8
	strb r2, [r3]
	strb r2, [r3, #1]
	adds r3, #2
	cmp r4, #31
	bne .L_08187174
	ldr r0, .L_081873b4
	ldr r2, [sp, #88]
	movs r5, #0
	movs r4, #128
	mov r8, r5
	lsls r4, r4, #13
	movs r1, #0
.L_08187192:
	ldrb r3, [r0]
	movs r7, #1
	lsls r3, r3, #16
	add r8, r7
	str r3, [r2]
	mov r3, r8
	str r4, [r2, #4]
	str r1, [r2, #12]
	str r1, [r2, #16]
	str r1, [r2, #24]
	adds r0, #1
	adds r2, #28
	cmp r3, #4
	bne .L_08187192
	ldr r5, [sp, #88]
	movs r4, #0
	mov r8, r4
	movs r7, #0
	movs r6, #0
	adds r5, #112
.L_081871ba:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #24
	str r3, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r6, [r5, #24]
	str r3, [r5, #4]
	str r7, [r5, #12]
	str r7, [r5, #16]
	adds r6, #3
	adds r5, #28
	cmp r1, #8
	bne .L_081871ba
	ldr r2, [sp, #88]
	movs r4, #192
	lsls r4, r4, #3
	adds r4, #228
	adds r3, r2, r4
	movs r2, #240
	lsls r2, r2, #14
	str r2, [r3]
	movs r2, #224
	movs r5, #0
	lsls r2, r2, #15
	str r5, [r3, #24]
	mov r8, r5
	str r2, [r3, #4]
	ldr r5, .L_081873b8
	movs r7, #0
	movs r6, #0
.L_08187206:
	adds r0, r6, #0
	bl Trig_Cos
	lsls r0, r0, #6
	negs r0, r0
	asrs r0, r0, #16
	strb r0, [r5]
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #3
	adds r3, r3, r0
	lsls r3, r3, #1
	negs r3, r3
	mov r0, r8
	asrs r3, r3, #16
	strb r3, [r5, #1]
	lsls r3, r0, #2
	subs r3, #64
	strb r3, [r5, #4]
	strb r7, [r5, #2]
	adds r0, r6, #0
	bl Trig_Cos
	lsls r0, r0, #6
	negs r0, r0
	asrs r0, r0, #16
	strb r0, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #3
	adds r3, r3, r0
	movs r2, #1
	lsrs r3, r3, #15
	movs r1, #248
	add r8, r2
	strb r3, [r5, #5]
	lsls r1, r1, #2
	mov r3, r8
	strb r7, [r5, #6]
	adds r6, r6, r1
	adds r5, #8
	cmp r3, #32
	bne .L_08187206
.L_08187260:
	ldr r4, [sp, #76]
	cmp r4, #31
	bhi .L_0818727c
	movs r1, #3
	lsls r0, r4, #1
	bl Math_Div
	ldr r2, [sp, #76]
	adds r1, r0, #0
	subs r1, #35
	subs r2, #35
	adds r0, r1, #0
	bl Func_08164b2c
.L_0818727c:
	ldr r3, [sp, #76]
	subs r3, #242
	cmp r3, #21
	bhi .L_08187298
	ldr r3, [sp, #76]
	movs r5, #1
	ands r3, r5
	cmp r3, #0
	beq .L_08187298
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_08164abc
.L_08187298:
	ldr r7, [sp, #76]
	movs r0, #22
	adds r0, #255
	cmp r7, r0
	bne .L_081872ae
	movs r2, #12
	negs r2, r2
	movs r0, #8
	adds r1, r2, #0
	bl Func_08164b2c
.L_081872ae:
	ldr r1, [sp, #76]
	movs r2, #104
	adds r2, #255
	cmp r1, r2
	ble .L_081872e6
	ldr r3, .L_081873bc
	adds r1, r1, r3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_081872c8
	ldr r4, [sp, #76]
	ldr r5, .L_081873c0
	adds r2, r4, r5
.L_081872c8:
	asrs r2, r2, #2
	movs r3, #8
	subs r0, r3, r2
	adds r2, r1, #0
	cmp r2, #0
	bge .L_081872da
	ldr r7, [sp, #76]
	ldr r1, .L_081873c4
	adds r2, r7, r1
.L_081872da:
	asrs r2, r2, #3
	movs r1, #12
	subs r2, #12
	negs r1, r1
	bl Func_08164b2c
.L_081872e6:
	ldr r2, [sp, #76]
	cmp r2, #48
	bne .L_081872f2
	movs r0, #136
	bl Audio_PlayCue
.L_081872f2:
	ldr r3, [sp, #76]
	cmp r3, #74
	bne .L_081872fe
	movs r0, #136
	bl Audio_PlayCue
.L_081872fe:
	ldr r4, [sp, #76]
	cmp r4, #100
	bne .L_0818730a
	movs r0, #136
	bl Audio_PlayCue
.L_0818730a:
	ldr r5, [sp, #76]
	cmp r5, #126
	bne .L_08187316
	movs r0, #136
	bl Audio_PlayCue
.L_08187316:
	ldr r7, [sp, #76]
	cmp r7, #140
	bne .L_08187322
	movs r0, #141
	bl Audio_PlayCue
.L_08187322:
	ldr r0, [sp, #76]
	movs r1, #133
	lsls r1, r1, #1
	cmp r0, r1
	ble .L_0818732e
	b .L_0818782e
.L_0818732e:
	movs r2, #192
	lsls r2, r2, #1
	adds r2, #255
	cmp r0, r2
	bgt .L_08187420
	add r3, sp, #176
	movs r4, #0
	str r4, [r3, #4]
	str r4, [r3, #12]
	ldr r7, [sp, #88]
	mov r8, r4
	mov r10, r3
.L_08187346:
	movs r3, #26
	mov r5, r8
	muls r5, r3
	ldr r0, [sp, #76]
	adds r3, r5, #0
	cmp r0, r3
	ble .L_08187414
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_081873cc
	ldr r3, [r7]
	mov r1, r10
	str r3, [r1]
	ldr r3, [r7, #4]
	movs r5, #1
	str r3, [r1, #8]
	movs r2, #240
	mov r3, r8
	ldr r4, [sp, #88]
	ands r3, r5
	lsls r2, r2, #7
	adds r2, #28
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r0, [r4, r3]
	ldr r2, .L_081873c8
	movs r3, #0
	bl Func_08020010
	movs r2, #128
	adds r0, r7, #0
	movs r1, #63
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	movs r0, #6
	ldrsh r3, [r7, r0]
	cmp r3, #119
	ble .L_08187414
	str r5, [r7, #24]
	b .L_08187414
.L_08187398:
	.4byte 0x050003c0
.L_0818739c:
	.4byte 0x0000005a
.L_081873a0:
	.4byte gInput
.L_081873a4:
	.4byte 0xffffe3d7
.L_081873a8:
	.4byte 0xfffffef5
.L_081873ac:
	.4byte IwramCopyWords
.L_081873b0:
	.4byte Data_02010fa0
.L_081873b4:
	.4byte Data_08199a3e
.L_081873b8:
	.4byte Data_020104f0
.L_081873bc:
	.4byte 0xfffffe98
.L_081873c0:
	.4byte 0xfffffe9b
.L_081873c4:
	.4byte 0xfffffe9f
.L_081873c8:
	.4byte Data_08199a44
.L_081873cc:
	cmp r3, #23
	bgt .L_08187414
	ldr r3, [r7]
	mov r1, r10
	str r3, [r1]
	ldr r3, [r7, #4]
	movs r2, #1
	str r3, [r1, #8]
	mov r5, r8
	movs r3, #240
	ands r5, r2
	ldr r4, [sp, #88]
	lsls r3, r3, #7
	adds r3, #12
	lsls r5, r5, #2
	adds r5, r5, r3
	ldr r0, [r7, #24]
	movs r1, #6
	ldr r6, [r4, r5]
	bl Math_Div
	movs r1, #3
	ands r1, r0
	adds r0, r6, #0
	bl Animation_ApplyChildArgumentFar
	ldr r1, [sp, #88]
	movs r3, #0
	ldr r0, [r1, r5]
	ldr r2, .L_08187660
	mov r1, r10
	bl Func_08020010
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_08187414:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #28
	cmp r3, #4
	bne .L_08187346
.L_08187420:
	ldr r4, [sp, #76]
	cmp r4, #46
	bgt .L_08187428
	b .L_0818760a
.L_08187428:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #36]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08187664
	ldr r3, [sp, #152]
	movs r1, #6
	ands r3, r2
	ldr r2, .L_08187668
	orrs r3, r1
	ldr r5, [sp, #76]
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	adds r7, r0, #0
	ldr r0, [sp, #88]
	orrs r3, r2
	movs r2, #142
	subs r5, #47
	lsls r2, r2, #7
	str r3, [sp, #152]
	mov r8, r5
	adds r3, r0, r2
	add r5, sp, #152
	str r3, [r5, #4]
	ldr r6, [sp, #76]
	subs r6, #111
	cmp r6, #0
	ble .L_0818746a
	movs r6, #0
.L_0818746a:
	ldr r3, .L_0818766c
	str r1, [r7]
	str r6, [r7, #20]
	str r5, [r7, #16]
	str r3, [r7, #8]
	ldr r3, [sp, #36]
	str r3, [r7, #12]
	bl Func_08014de4
	movs r1, #144
	lsls r1, r1, #14
	movs r2, #0
	movs r0, #0
	bl Func_08015160
	movs r0, #250
	lsls r0, r0, #4
	bl SceneTransform_ApplyPitch
	mov r4, r8
	negs r0, r4
	lsls r0, r0, #8
	bl Func_08015068
	movs r2, #234
	mov r1, r8
	lsls r2, r2, #8
	lsls r0, r1, #10
	adds r2, #96
	cmp r0, r2
	ble .L_081874ae
	movs r0, #234
	lsls r0, r0, #8
	adds r0, #96
.L_081874ae:
	bl Func_0801521c
	movs r2, #4
	ldr r1, [sp, #36]
	ldr r0, .L_08187670
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	movs r3, #5
	strb r3, [r5]
	strb r3, [r5, #1]
	ldr r4, [sp, #88]
	movs r0, #184
	lsls r0, r0, #5
	adds r3, r4, r0
	str r3, [r5, #4]
	movs r3, #9
	str r3, [r7]
	ldr r3, .L_08187674
	str r5, [r7, #16]
	str r3, [r7, #8]
	ldr r1, [sp, #36]
	str r6, [r7, #20]
	str r1, [r7, #12]
	bl Func_08014de4
	movs r1, #144
	movs r0, #0
	movs r2, #0
	lsls r1, r1, #14
	bl Func_08015160
	mov r2, r8
	lsls r3, r2, #1
	add r3, r8
	movs r4, #128
	movs r5, #151
	lsls r3, r3, #8
	lsls r4, r4, #3
	lsls r5, r5, #8
	adds r0, r3, r4
	adds r5, #112
	cmp r0, r5
	ble .L_08187510
	movs r0, #151
	lsls r0, r0, #8
	adds r0, #112
.L_08187510:
	bl Func_0801521c
	ldr r0, [sp, #76]
	cmp r0, #73
	ble .L_081875ba
	cmp r0, #74
	bne .L_08187524
	movs r1, #0
	str r1, [sp, #48]
	str r1, [sp, #44]
.L_08187524:
	ldr r0, [sp, #48]
	movs r1, #13
	bl __modsi3
	ldr r5, [sp, #48]
	adds r4, r0, #0
	adds r5, #1
	movs r1, #13
	adds r0, r5, #0
	str r4, [sp, #8]
	bl __modsi3
	ldr r2, [sp, #44]
	ldr r1, .L_08187678
	mov r9, r2
	mov r3, r9
	adds r3, #1
	str r3, [sp, #44]
	ldr r4, [sp, #8]
	ldr r2, [sp, #44]
	ldrb r3, [r1, r4]
	mov r12, r1
	cmp r2, r3
	bne .L_0818755a
	movs r3, #0
	str r3, [sp, #44]
	str r5, [sp, #48]
.L_0818755a:
	movs r5, #0
	lsls r1, r4, #5
	lsls r0, r0, #5
	mov r8, r5
	mov r11, r1
	mov r10, r0
	b .L_0818756c
.L_08187568:
	ldr r2, .L_08187678
	mov r12, r2
.L_0818756c:
	ldr r2, .L_0818767c
	ldr r5, .L_08187680
	mov r3, r8
	mov r1, r8
	lsls r6, r3, #3
	add r1, r11
	add r3, r10
	adds r6, r6, r5
	ldrb r3, [r2, r3]
	ldrb r5, [r2, r1]
	mov r2, r12
	subs r3, r3, r5
	mov r0, r9
	muls r0, r3
	ldrb r1, [r2, r4]
	str r4, [sp, #8]
	bl Math_Div
	movs r3, #1
	adds r5, r5, r0
	negs r5, r5
	add r8, r3
	strb r5, [r6, #1]
	mov r5, r8
	ldr r4, [sp, #8]
	cmp r5, #32
	bne .L_08187568
	ldr r2, .L_08187680
	movs r0, #0
	mov r8, r0
	subs r3, #65
.L_081875aa:
	movs r1, #1
	add r8, r1
	mov r4, r8
	strb r3, [r2]
	adds r2, #8
	adds r3, #4
	cmp r4, #32
	bne .L_081875aa
.L_081875ba:
	ldr r1, .L_08187684
	ldr r2, .L_08187688
	movs r5, #0
	movs r0, #1
	str r5, [r7, #4]
	str r0, [r7]
	str r1, [r7, #8]
	str r2, [r7, #12]
	ldr r3, [sp, #76]
	subs r3, #236
	cmp r3, #31
	bhi .L_081875ea
	ldr r3, [sp, #76]
	ldr r2, .L_0818768c
	subs r3, #235
	mov r8, r5
.L_081875da:
	movs r4, #1
	add r8, r4
	mov r5, r8
	strb r3, [r2]
	strb r3, [r2, #1]
	adds r2, #2
	cmp r5, #31
	bne .L_081875da
.L_081875ea:
	ldr r0, .L_0818768c
	ldr r1, .L_08187688
	movs r2, #64
	str r0, [r7, #20]
	ldr r0, .L_08187680
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	adds r0, r7, #0
	bl Sys_Free
	ldr r0, [sp, #36]
	bl Sys_Free
.L_0818760a:
	ldr r3, .L_08187690
	movs r1, #160
	ldr r4, [r3, #4]
	ldr r3, [r3]
	add r1, sp
	movs r2, #0
	str r3, [sp, #144]
	str r4, [sp, #148]
	str r2, [r1, #12]
	ldr r3, [sp, #76]
	mov r9, r1
	cmp r3, #149
	bgt .L_08187626
	b .L_0818782e
.L_08187626:
	ldr r5, [sp, #60]
	ldr r0, [sp, #64]
	ldr r4, [sp, #52]
	ldr r7, [sp, #56]
	adds r4, r4, r5
	adds r7, r7, r0
	str r4, [sp, #60]
	str r7, [sp, #64]
	cmp r3, #159
	bgt .L_08187644
	ldr r1, [sp, #56]
	ldr r2, .L_08187694
	adds r2, r1, r2
	str r2, [sp, #56]
	b .L_081876ba
.L_08187644:
	ldr r3, [sp, #76]
	subs r3, #210
	cmp r3, #13
	bhi .L_081876a0
	ldr r3, [sp, #52]
	ldr r5, [sp, #56]
	ldr r4, .L_08187698
	ldr r7, .L_0818769c
	adds r4, r3, r4
	adds r7, r5, r7
	str r4, [sp, #52]
	str r7, [sp, #56]
	b .L_081876ba
	.2byte 0x0000
.L_08187660:
	.4byte Data_08199a44
.L_08187664:
	.4byte 0xffffff00
.L_08187668:
	.4byte 0xffff00ff
.L_0818766c:
	.4byte Data_08199340
.L_08187670:
	.4byte Data_08199210
.L_08187674:
	.4byte Data_08199244
.L_08187678:
	.4byte Data_08199bec
.L_0818767c:
	.4byte Data_08199a4c
.L_08187680:
	.4byte Data_020104f0
.L_08187684:
	.4byte Data_02010ae8
.L_08187688:
	.4byte gMapCellBuffer
.L_0818768c:
	.4byte Data_02010fa0
.L_08187690:
	.4byte Data_08196f00
.L_08187694:
	.4byte 0xffffc000
.L_08187698:
	.4byte 0xfffff800
.L_0818769c:
	.4byte 0xfffff000
.L_081876a0:
	ldr r3, [sp, #76]
	subs r3, #236
	cmp r3, #3
	bhi .L_081876ba
	ldr r0, [sp, #52]
	ldr r2, [sp, #56]
	ldr r3, .L_08187a4c
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r0, r1
	adds r3, r2, r3
	str r1, [sp, #52]
	str r3, [sp, #56]
.L_081876ba:
	ldr r4, [sp, #52]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_081876c8
	adds r3, #63
.L_081876c8:
	ldr r5, [sp, #56]
	asrs r3, r3, #6
	str r3, [sp, #52]
	lsls r3, r5, #4
	subs r3, r3, r5
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_081876da
	adds r3, #63
.L_081876da:
	asrs r3, r3, #6
	str r3, [sp, #56]
	movs r7, #144
	movs r3, #128
	add r7, sp
	lsls r3, r3, #9
	str r3, [sp, #144]
	str r3, [r7, #4]
	ldr r1, [sp, #88]
	movs r2, #238
	lsls r2, r2, #7
	ldr r6, [sp, #64]
	movs r0, #0
	adds r2, #220
	mov r10, r7
	mov r8, r0
	mov r5, r9
	adds r7, r1, r2
.L_081876fe:
	ldr r3, [sp, #60]
	ldmia r7!, {r0}
	str r3, [r5]
	movs r3, #136
	lsls r3, r3, #17
	subs r3, r3, r6
	str r3, [r5, #4]
	movs r3, #128
	lsls r3, r3, #17
	str r3, [r5, #8]
	adds r1, r5, #0
	mov r2, r10
	movs r3, #0
	bl Func_08020010
	movs r0, #1
	movs r4, #128
	add r8, r0
	lsls r4, r4, #14
	mov r1, r8
	adds r6, r6, r4
	cmp r1, #2
	bne .L_081876fe
	ldr r3, [sp, #76]
	subs r3, #150
	cmp r3, #116
	bhi .L_081877d2
	str r0, [sp, #0]
	movs r2, #6
	movs r3, #3
	movs r0, #188
	movs r1, #6
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r6, [sp, #88]
	str r3, [sp, #84]
	movs r2, #0
	mov r8, r2
	adds r6, #112
.L_08187754:
	ldr r0, [r6, #24]
	cmp r0, #0
	bge .L_0818775c
	adds r0, #3
.L_0818775c:
	movs r1, #6
	asrs r0, r0, #2
	bl __modsi3
	ldr r2, .L_08187a50
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #88]
	ldr r2, [r6]
	adds r1, r3, r1
	ldr r3, .L_08187a54
	movs r4, #224
	ldrb r5, [r3, r0]
	lsls r4, r4, #3
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_08187a58
	adds r1, r1, r4
	ldrb r4, [r3, r0]
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	adds r3, r3, r4
	ldr r4, .L_08187a5c
	ldr r5, [sp, #88]
	ldrb r0, [r4, r0]
	movs r7, #207
	lsls r7, r7, #7
	str r0, [sp, #4]
	ldr r4, [sp, #84]
	adds r0, r5, r7
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #24
	bne .L_081877c0
	movs r5, #0
	str r5, [r6, #24]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #24
	str r3, [r6]
	bl Random16
	movs r3, #7
	ands r3, r0
	str r3, [r6, #4]
.L_081877c0:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r6, #28
	cmp r0, #8
	bne .L_08187754
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_081877d2:
	ldr r1, [sp, #76]
	movs r3, #141
	lsls r3, r3, #3
	ldr r2, .L_08187a60
	muls r3, r1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #11
	str r3, [sp, #144]
	cmp r3, r2
	ble .L_081877ea
	str r2, [sp, #144]
.L_081877ea:
	ldr r4, [sp, #76]
	movs r3, #141
	lsls r3, r3, #2
	muls r3, r4
	ldr r5, .L_08187a64
	movs r2, #128
	adds r3, r3, r5
	mov r7, r10
	lsls r2, r2, #10
	str r3, [r7, #4]
	cmp r3, r2
	ble .L_08187804
	str r2, [r7, #4]
.L_08187804:
	ldr r2, [sp, #64]
	movs r3, #240
	lsls r3, r3, #15
	mov r1, r9
	subs r3, r3, r2
	ldr r0, [sp, #60]
	str r3, [r1, #4]
	movs r3, #128
	lsls r3, r3, #16
	str r0, [r1]
	str r3, [r1, #8]
	ldr r4, [sp, #88]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #36
	adds r3, r4, r5
	ldr r0, [r3]
	mov r2, r10
	movs r3, #0
	bl Func_08020010
.L_0818782e:
	ldr r7, [sp, #76]
	movs r5, #12
	adds r5, #255
	cmp r7, r5
	bne .L_0818783e
	movs r0, #212
	bl Audio_PlayCue
.L_0818783e:
	ldr r0, [sp, #76]
	movs r1, #16
	adds r1, #255
	cmp r0, r1
	bne .L_0818784e
	movs r0, #144
	bl Audio_PlayCue
.L_0818784e:
	ldr r2, [sp, #12]
	cmp r2, #9
	bls .L_08187856
	b .L_081879d2
.L_08187856:
	ldr r3, [sp, #76]
	cmp r3, r5
	bne .L_081878c0
	movs r2, #160
	lsls r2, r2, #19
	movs r4, #1
	adds r2, #2
	mov r8, r4
.L_08187866:
	mov r3, r8
	cmp r3, #31
	ble .L_0818786e
	movs r3, #31
.L_0818786e:
	cmp r3, #0
	bge .L_08187874
	movs r3, #0
.L_08187874:
	movs r5, #1
	add r8, r5
	mov r7, r8
	strh r3, [r2]
	adds r2, #2
	cmp r7, #64
	bne .L_08187866
	ldr r2, [sp, #88]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08187a68
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_08187a6c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08187a70
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #88]
	movs r5, #239
	lsls r5, r5, #7
	adds r3, r4, r5
	movs r7, #1
	movs r0, #238
	str r7, [r3]
	lsls r0, r0, #7
	ldr r3, .L_08187a74
	adds r0, #132
	adds r2, r4, r0
	str r3, [r2]
.L_081878c0:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08187a78
	ldr r3, [sp, #136]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08187a7c
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #136]
	ldr r3, .L_08187a80
	add r2, sp, #136
	adds r7, r0, #0
	str r3, [r2, #4]
	movs r3, #0
	str r3, [r7, #20]
	ldr r3, .L_08187a84
	mov r4, r10
	str r1, [r7]
	str r2, [r7, #16]
	str r3, [r7, #8]
	str r4, [r7, #12]
	mov r8, r2
	bl Func_08014de4
	movs r0, #0
	ldr r1, .L_08187a88
	movs r2, #0
	bl Func_08015160
	ldr r5, [sp, #76]
	movs r1, #236
	movs r0, #134
	lsls r1, r1, #9
	lsls r0, r0, #1
	adds r1, #240
	cmp r5, r0
	ble .L_08187922
	ldr r2, .L_08187a8c
	lsls r3, r5, #12
	adds r1, r3, r2
.L_08187922:
	asrs r2, r1, #1
	adds r0, r2, #0
	bl Func_080151e4
	ldr r0, .L_08187a90
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	ldr r3, [sp, #12]
	cmp r3, #5
	bhi .L_081879c6
	movs r1, #216
	ldr r3, .L_08187a94
	ldr r0, .L_08187a98
	lsls r1, r1, #4
	mov lr, r3
	.2byte 0xf800
	ldr r5, [sp, #16]
	movs r6, #0
	lsls r4, r5, #7
	movs r5, #0
.L_08187954:
	ldr r2, .L_08187a98
	ldr r3, [sp, #88]
	adds r1, r5, r2
	adds r3, r3, r4
	movs r2, #224
	mov r12, r3
	lsls r2, r2, #3
	movs r0, #0
	add r2, r12
.L_08187966:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #48
	bne .L_08187966
	adds r6, #1
	adds r4, #48
	adds r5, #64
	cmp r6, #72
	bne .L_08187954
	ldr r4, [sp, #12]
	cmp r4, #5
	bhi .L_081879c6
	movs r3, #6
	mov r5, r8
	add r2, sp, #136
	strb r3, [r5]
	movs r3, #7
	strb r3, [r2, #1]
	ldr r0, .L_08187a98
	ldr r3, .L_08187a9c
	str r0, [r2, #4]
	str r3, [r7, #8]
	bl Func_08014de4
	movs r0, #0
	ldr r1, .L_08187a88
	movs r2, #0
	bl Func_08015160
	movs r1, #167
	lsls r1, r1, #10
	movs r2, #128
	ldr r0, .L_08187aa0
	adds r1, #64
	lsls r2, r2, #10
	bl Func_080151e4
	ldr r0, .L_08187a90
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081879c6:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
.L_081879d2:
	ldr r1, [sp, #76]
	movs r2, #138
	lsls r2, r2, #1
	cmp r1, r2
	bgt .L_081879de
	b .L_08187dfc
.L_081879de:
	movs r3, #22
	adds r3, #255
	cmp r1, r3
	beq .L_081879e8
	b .L_08187b54
.L_081879e8:
	ldr r4, [sp, #88]
	movs r5, #239
	movs r0, #238
	lsls r5, r5, #7
	lsls r0, r0, #7
	adds r3, r4, r5
	movs r7, #1
	adds r0, #132
	str r7, [r3]
	movs r1, #0
	adds r3, r4, r0
	movs r2, #1
	str r1, [r3]
	str r2, [sp, #40]
	adds r5, #92
	adds r3, r4, r5
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r0, #238
	ldr r7, [sp, #88]
	lsls r0, r0, #7
	adds r0, #224
	adds r3, r7, r0
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r2, #240
	lsls r2, r2, #7
	movs r1, #0
	adds r2, #12
	mov r8, r1
	adds r5, r7, r2
.L_08187a2a:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #4
	bne .L_08187a2a
	ldr r7, [sp, #88]
	movs r0, #240
	lsls r0, r0, #7
	movs r5, #0
	adds r0, #28
	mov r8, r5
	adds r5, r7, r0
	b .L_08187aa4
	.2byte 0x0000
.L_08187a4c:
	.4byte 0xffffc000
.L_08187a50:
	.4byte Data_0819747a
.L_08187a54:
	.4byte Data_08197467
.L_08187a58:
	.4byte Data_08197473
.L_08187a5c:
	.4byte Data_0819746d
.L_08187a60:
	.4byte 0xfffe6b10
.L_08187a64:
	.4byte 0xffffb588
.L_08187a68:
	.4byte 0x0000017a
.L_08187a6c:
	.4byte 0x00000150
.L_08187a70:
	.4byte IwramCopyWords
.L_08187a74:
	.4byte 0x01010101
.L_08187a78:
	.4byte 0xffffff00
.L_08187a7c:
	.4byte 0xffff00ff
.L_08187a80:
	.4byte Data_02014000
.L_08187a84:
	.4byte Data_08199364
.L_08187a88:
	.4byte 0xfffc0000
.L_08187a8c:
	.4byte 0xfff108f0
.L_08187a90:
	.4byte Data_081991e0
.L_08187a94:
	.4byte IwramClearWords
.L_08187a98:
	.4byte gMapCellBuffer
.L_08187a9c:
	.4byte Data_08199388
.L_08187aa0:
	.4byte 0x0001b1e0
.L_08187aa4:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #2
	bne .L_08187aa4
	ldr r4, [sp, #88]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #36
	adds r3, r4, r5
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r1, #202
	lsls r1, r1, #1
	movs r0, #11
	adds r1, #255
	movs r2, #2
	bl Func_08152404
	ldr r6, .L_08187d80
	ldr r5, [sp, #88]
	movs r7, #0
	mov r8, r7
	movs r7, #31
.L_08187adc:
	ldrb r2, [r6]
	ldrb r1, [r6, #1]
	lsrs r2, r2, #1
	lsls r3, r2, #16
	str r3, [r5]
	lsls r3, r1, #16
	str r3, [r5, #4]
	movs r3, #60
	subs r3, r3, r1
	subs r2, #120
	lsls r2, r2, #12
	lsls r3, r3, #10
	movs r0, #0
	str r2, [r5, #12]
	str r3, [r5, #16]
	str r0, [r5, #8]
	bl Random16
	ldr r3, [r5, #12]
	ands r0, r7
	subs r0, #16
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r5, #12]
	bl Random16
	ldr r3, [r5, #16]
	ands r0, r7
	subs r0, #16
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r5, #16]
	bl Random16
	movs r3, #127
	ands r0, r3
	lsls r3, r0, #2
	movs r1, #160
	adds r3, r3, r0
	lsls r1, r1, #2
	adds r3, r3, r1
	str r3, [r5, #20]
	bl Random16
	movs r2, #1
	ands r0, r2
	cmp r0, #0
	beq .L_08187b42
	ldr r3, [r5, #20]
	negs r3, r3
	str r3, [r5, #20]
.L_08187b42:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r6, #10
	adds r5, #28
	cmp r4, #31
	bne .L_08187adc
	movs r5, #0
	str r5, [sp, #52]
.L_08187b54:
	ldr r2, .L_08187d84
	movs r7, #255
	ldrh r3, [r2, #4]
	lsls r7, r7, #8
	adds r7, #244
	adds r3, r3, r7
	strh r3, [r2, #4]
	ldr r3, .L_08187d88
	add r6, sp, #160
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r0, #0
	str r3, [sp, #128]
	str r4, [sp, #132]
	str r0, [r6, #12]
	ldr r1, [sp, #76]
	lsls r5, r1, #9
	adds r0, r5, #0
	bl Trig_Sin
	movs r2, #216
	lsls r0, r0, #3
	lsls r2, r2, #15
	adds r2, r0, r2
	adds r0, r5, #0
	str r2, [sp, #60]
	bl Trig_Cos
	movs r3, #224
	lsls r3, r3, #14
	lsls r0, r0, #3
	subs r0, r3, r0
	ldr r3, [sp, #76]
	movs r4, #198
	lsls r4, r4, #1
	str r0, [sp, #64]
	cmp r3, r4
	ble .L_08187bc2
	ldr r5, .L_08187d8c
	ldr r7, [sp, #52]
	ldr r1, [sp, #56]
	adds r2, r3, r5
	ldr r0, [sp, #60]
	lsls r3, r2, #14
	lsls r2, r2, #13
	subs r3, r7, r3
	subs r2, r1, r2
	adds r0, r3, r0
	str r3, [sp, #52]
	str r2, [sp, #56]
	adds r2, r3, #0
	ldr r3, [sp, #64]
	str r0, [sp, #60]
	adds r2, r2, r3
	str r2, [sp, #64]
.L_08187bc2:
	ldr r0, [sp, #88]
	movs r1, #238
	adds r7, r6, #0
	lsls r1, r1, #7
	ldr r6, .L_08187d90
	movs r4, #0
	adds r1, #220
	mov r8, r4
	adds r5, r0, r1
	add r4, sp, #128
.L_08187bd6:
	ldrh r3, [r6]
	ldr r2, [sp, #60]
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r7]
	ldrh r3, [r6, #2]
	ldr r0, [sp, #64]
	lsls r3, r3, #16
	movs r2, #136
	adds r3, r3, r0
	lsls r2, r2, #17
	subs r2, r2, r3
	movs r3, #128
	lsls r3, r3, #17
	str r2, [r7, #4]
	str r3, [r7, #8]
	adds r2, r4, #0
	adds r1, r7, #0
	ldmia r5!, {r0}
	movs r3, #0
	str r4, [sp, #8]
	bl Func_08020010
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #4
	ldr r4, [sp, #8]
	cmp r2, #11
	bne .L_08187bd6
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #32]
	movs r0, #1
	bl Func_081969f8
	ldr r3, [sp, #88]
	movs r4, #224
	movs r5, #240
	ldr r7, [sp, #76]
	mov r10, r0
	lsls r4, r4, #3
	lsls r5, r5, #4
	movs r0, #22
	adds r4, r3, r4
	adds r5, r3, r5
	adds r0, #255
	str r4, [sp, #28]
	str r5, [sp, #24]
	cmp r7, r0
	bne .L_08187cb2
	ldr r0, [sp, #24]
	ldr r7, .L_08187d80
	movs r5, #0
	movs r1, #0
	movs r2, #2
	mov r12, r1
	mov r11, r2
	mov r9, r5
	mov lr, r5
.L_08187c50:
	movs r3, #0
	ldr r4, .L_08187d80
	mov r8, r3
	mov r3, lr
	add r3, r12
	lsls r3, r3, #1
	ldr r2, [sp, #28]
	adds r3, r3, r4
	ldrb r6, [r7, #1]
	ldrb r4, [r7]
	adds r1, r3, #2
	add r2, r9
.L_08187c68:
	ldrb r3, [r1]
	strb r5, [r2, #2]
	subs r3, r3, r4
	strb r3, [r2]
	ldrb r3, [r1, #1]
	adds r1, #2
	subs r3, r3, r6
	strb r3, [r2, #1]
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r2, #4
	cmp r3, #4
	bne .L_08187c68
	movs r4, #1
	strb r4, [r0, #1]
	movs r4, #1
	mov r1, r11
	movs r3, #3
	add r12, r4
	strb r1, [r0, #2]
	strb r1, [r0, #5]
	strb r3, [r0, #6]
	movs r2, #16
	movs r3, #4
	mov r1, r12
	strb r5, [r0]
	strb r5, [r0, #4]
	strb r5, [r0, #8]
	strb r5, [r0, #9]
	strb r5, [r0, #10]
	adds r7, #10
	adds r0, #12
	add r9, r2
	add lr, r3
	cmp r1, #31
	bne .L_08187c50
.L_08187cb2:
	ldr r3, [sp, #120]
	ldr r2, .L_08187d94
	mov r5, r10
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldr r2, .L_08187d98
	movs r4, #1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, .L_08187d9c
	str r3, [sp, #120]
	add r3, sp, #120
	str r2, [r3, #4]
	str r3, [r5, #16]
	str r4, [r5]
	ldr r7, [sp, #32]
	ldr r0, .L_08187da0
	ldr r3, .L_08187da0
	str r7, [r5, #12]
	str r0, [r5, #20]
	adds r2, r3, #0
	movs r1, #1
	adds r2, #31
.L_08187ce6:
	strb r1, [r3]
	adds r3, #1
	cmp r3, r2
	bne .L_08187ce6
	ldr r6, [sp, #24]
	ldr r5, [sp, #88]
	movs r1, #0
	mov r8, r1
.L_08187cf6:
	movs r4, #2
	ldrsh r3, [r5, r4]
	mov r2, r10
	adds r3, #16
	str r6, [r2, #8]
	cmp r3, #152
	bhi .L_08187dcc
	movs r7, #6
	ldrsh r3, [r5, r7]
	movs r0, #16
	negs r0, r0
	cmp r3, r0
	blt .L_08187dcc
	cmp r3, #136
	bgt .L_08187dcc
	bl Func_08014de4
	ldr r3, .L_08187da4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r0, r2, #0
	bl Func_080151e4
	movs r3, #3
	mov r1, r8
	ands r3, r1
	cmp r3, #1
	beq .L_08187d62
	cmp r3, #1
	bgt .L_08187d4a
	cmp r3, #0
	beq .L_08187d54
	b .L_08187db6
.L_08187d4a:
	cmp r3, #2
	beq .L_08187d72
	cmp r3, #3
	beq .L_08187da8
	b .L_08187db6
.L_08187d54:
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	bl Func_080150e4
	b .L_08187db6
.L_08187d62:
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	negs r0, r0
	bl Func_080150e4
	b .L_08187db6
.L_08187d72:
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	bl Func_08015068
	b .L_08187db6
.L_08187d80:
	.4byte Data_08199bf9
.L_08187d84:
	.4byte Data_03001120
.L_08187d88:
	.4byte Data_08196f08
.L_08187d8c:
	.4byte 0xfffffe73
.L_08187d90:
	.4byte Data_08199a12
.L_08187d94:
	.4byte 0xffffff00
.L_08187d98:
	.4byte 0xffff00ff
.L_08187d9c:
	.4byte gMapCellBuffer
.L_08187da0:
	.4byte Data_02010fa0
.L_08187da4:
	.4byte 0xffc00000
.L_08187da8:
	ldr r0, [r5, #8]
	bl Func_080150e4
	ldr r0, [r5, #8]
	negs r0, r0
	bl Func_08015068
.L_08187db6:
	ldr r3, [sp, #28]
	mov r2, r8
	lsls r0, r2, #4
	adds r0, r3, r0
	ldr r1, [sp, #32]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08187dcc:
	adds r0, r5, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r5]
	movs r4, #128
	movs r7, #1
	lsls r4, r4, #11
	add r8, r7
	adds r3, r3, r4
	mov r0, r8
	str r3, [r5]
	adds r6, #12
	adds r5, #28
	cmp r0, #31
	beq .L_08187df0
	b .L_08187cf6
.L_08187df0:
	mov r0, r10
	bl Sys_Free
	ldr r0, [sp, #32]
	bl Sys_Free
.L_08187dfc:
	bl Func_081434f8
	movs r2, #240
	ldr r1, [sp, #88]
	lsls r2, r2, #7
	adds r2, #232
	adds r3, r1, r2
	movs r2, #1
	str r2, [r3]
	ldr r3, [sp, #76]
	movs r4, #133
	lsls r4, r4, #1
	cmp r3, r4
	bgt .L_08187e22
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #236
	adds r3, r1, r5
	str r2, [r3]
.L_08187e22:
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #16]
	ldr r0, [sp, #12]
	ldr r1, [sp, #76]
	movs r2, #220
	adds r7, #27
	adds r0, #1
	adds r1, #1
	lsls r2, r2, #1
	str r7, [sp, #16]
	str r0, [sp, #12]
	str r1, [sp, #76]
	cmp r1, r2
	beq .L_08187e52
	ldr r3, .L_08187ec4
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08187e52
	bl .L_0818712c
.L_08187e52:
	ldr r3, [sp, #40]
	cmp r3, #0
	bne .L_08187ec8
	ldr r4, [sp, #88]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #220
	adds r3, r4, r5
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r0, #238
	ldr r7, [sp, #88]
	lsls r0, r0, #7
	adds r0, #224
	adds r3, r7, r0
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r2, #240
	lsls r2, r2, #7
	movs r1, #0
	adds r2, #12
	mov r8, r1
	adds r5, r7, r2
.L_08187e84:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #4
	bne .L_08187e84
	ldr r7, [sp, #88]
	movs r0, #240
	lsls r0, r0, #7
	movs r5, #0
	adds r0, #28
	mov r8, r5
	adds r5, r7, r0
.L_08187ea2:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #2
	bne .L_08187ea2
	ldr r4, [sp, #88]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #36
	adds r3, r4, r5
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	b .L_08187ee6
.L_08187ec4:
	.4byte gInput
.L_08187ec8:
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	movs r7, #0
	adds r1, #220
	mov r8, r7
	adds r5, r0, r1
.L_08187ed6:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #11
	bne .L_08187ed6
.L_08187ee6:
	ldr r4, [sp, #68]
	movs r3, #0
	str r3, [r4, #16]
	ldr r0, .L_08187ffc
	bl Scheduler_RemoveCallback
	bl Func_0814cca8
	ldr r2, .L_08188000
	movs r1, #202
	movs r3, #120
	lsls r1, r1, #1
	str r3, [r2, #16]
	adds r1, #255
	movs r2, #2
	movs r0, #11
	bl Func_08152404
	ldr r0, .L_08188004
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08188008
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r5, [sp, #88]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r5, r7
	ldr r0, .L_0818800c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #216
	ldr r3, .L_08188010
	ldr r0, .L_08188014
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	mov r8, r0
	mov r12, r0
	movs r7, #0
.L_08187f44:
	mov r2, r8
	mov r1, r12
	adds r3, r7, r2
	movs r6, #0
	lsls r5, r1, #7
	lsls r0, r3, #9
.L_08187f50:
	ldr r2, [sp, #88]
	ldr r3, .L_08188014
	adds r2, r2, r5
	mov lr, r2
	movs r2, #224
	lsls r2, r2, #3
	movs r4, #0
	adds r1, r0, r3
	add r2, lr
.L_08187f62:
	ldrb r3, [r2]
	adds r4, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r4, #48
	bne .L_08187f62
	adds r6, #1
	adds r5, #48
	adds r0, #64
	cmp r6, #72
	bne .L_08187f50
	movs r4, #1
	add r8, r4
	movs r3, #27
	mov r5, r8
	add r12, r3
	adds r7, #8
	cmp r5, #6
	bne .L_08187f44
	ldr r7, [sp, #88]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r7, r2
	ldr r0, .L_08188018
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0818801c
	ldr r1, [sp, #72]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r7, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r7, r4
	movs r3, #50
	str r3, [r2]
	ldr r3, .L_08187ff8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r5, #0
	movs r7, #104
	str r3, [sp, #80]
	str r5, [sp, #76]
	str r5, [sp, #20]
	add r7, sp
	mov r9, r7
.L_08187fe4:
	ldr r0, [sp, #76]
	cmp r0, #0
	bne .L_0818804a
	ldr r3, .L_08188020
	ldr r2, .L_08188024
	movs r4, #192
	movs r1, #135
	lsls r4, r4, #11
	lsls r1, r1, #17
	b .L_08188028
.L_08187ff8:
	.4byte 0x00001010
.L_08187ffc:
	.4byte Func_08143114
.L_08188000:
	.4byte gCameraSceneParameters
.L_08188004:
	.4byte 0x00000148
.L_08188008:
	.4byte IwramCopyWords
.L_0818800c:
	.4byte 0x0000017a
.L_08188010:
	.4byte IwramClearWords
.L_08188014:
	.4byte gMapCellBuffer
.L_08188018:
	.4byte 0x000000f4
.L_0818801c:
	.4byte 0x00000134
.L_08188020:
	.4byte 0xfff40000
.L_08188024:
	.4byte 0xffea0000
.L_08188028:
	str r3, [sp, #52]
	str r4, [sp, #56]
	ldr r3, .L_08188260
	str r1, [sp, #60]
	str r2, [sp, #64]
	movs r5, #0
	movs r1, #1
	movs r2, #128
	mov r8, r5
	negs r1, r1
	lsls r2, r2, #2
.L_0818803e:
	movs r7, #1
	add r8, r7
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0818803e
.L_0818804a:
	ldr r0, [sp, #76]
	cmp r0, #20
	bne .L_08188064
	ldr r1, .L_08188264
	ldr r2, .L_08188268
	movs r3, #192
	movs r4, #192
	lsls r3, r3, #12
	lsls r4, r4, #11
	str r1, [sp, #60]
	str r2, [sp, #64]
	str r3, [sp, #52]
	str r4, [sp, #56]
.L_08188064:
	ldr r5, [sp, #76]
	cmp r5, #40
	bne .L_0818807e
	ldr r1, .L_0818826c
	movs r7, #130
	movs r0, #128
	lsls r7, r7, #17
	lsls r0, r0, #15
	movs r2, #0
	str r7, [sp, #60]
	str r0, [sp, #64]
	str r1, [sp, #52]
	str r2, [sp, #56]
.L_0818807e:
	ldr r6, .L_08188270
	ldr r5, .L_08188274
	movs r3, #0
	mov r8, r3
.L_08188086:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_081880c8
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #72]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #92]
	ldr r4, [sp, #80]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_08188278
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_081880c8:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r8, r0
	bne .L_08188086
	ldr r1, [sp, #76]
	cmp r1, #39
	bgt .L_08188154
	lsls r3, r1, #1
	adds r3, r3, r1
	movs r2, #0
	mov r8, r2
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r4, [sp, #60]
	ldr r3, .L_08188274
	lsls r2, r2, #4
	adds r7, r2, r3
	lsrs r3, r4, #31
	adds r3, r3, r4
	asrs r3, r3, #1
	mov r10, r3
.L_081880f8:
	bl Random16
	movs r3, #63
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	add r3, r10
	str r3, [r7]
	bl Random16
	movs r3, #31
	ldr r5, [sp, #64]
	ands r3, r0
	lsls r3, r3, #16
	adds r3, r3, r5
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	movs r5, #127
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r0, #1
	asrs r3, r3, #6
	add r8, r0
	str r3, [r7, #16]
	mov r1, r8
	movs r3, #16
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #12
	bne .L_081880f8
.L_08188154:
	ldr r2, [sp, #76]
	cmp r2, #73
	ble .L_0818815c
	b .L_08188298
.L_0818815c:
	ldr r3, .L_0818827c
	add r1, sp, #160
	ldr r4, [r3, #4]
	ldr r3, [r3]
	subs r2, #20
	str r3, [sp, #112]
	str r4, [sp, #116]
	movs r3, #0
	str r3, [r1, #12]
	mov r10, r2
	cmp r2, #19
	bhi .L_081881ca
	ldr r3, .L_08188280
	adds r7, r1, #0
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	ldr r6, .L_08188284
	str r3, [sp, #112]
	adds r1, #220
	movs r3, #0
	mov r8, r3
	add r4, sp, #112
	adds r5, r0, r1
.L_0818818c:
	ldrh r3, [r6]
	ldr r2, [sp, #60]
	lsls r3, r3, #16
	subs r3, r2, r3
	str r3, [r7]
	ldrh r3, [r6, #2]
	ldr r0, [sp, #64]
	lsls r3, r3, #16
	movs r2, #136
	adds r3, r3, r0
	lsls r2, r2, #17
	subs r2, r2, r3
	movs r3, #128
	lsls r3, r3, #17
	str r2, [r7, #4]
	str r3, [r7, #8]
	adds r2, r4, #0
	adds r1, r7, #0
	ldmia r5!, {r0}
	movs r3, #0
	str r4, [sp, #8]
	bl Func_08020010
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #4
	ldr r4, [sp, #8]
	cmp r2, #11
	bne .L_0818818c
	b .L_0818821a
.L_081881ca:
	ldr r7, [sp, #88]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #220
	adds r6, r7, r0
	ldr r7, .L_08188284
	movs r3, #0
	mov r8, r3
	add r4, sp, #112
	adds r5, r1, #0
.L_081881de:
	ldrh r3, [r7]
	ldr r1, [sp, #60]
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r5]
	ldrh r3, [r7, #2]
	ldr r2, [sp, #64]
	lsls r3, r3, #16
	adds r3, r3, r2
	movs r2, #136
	lsls r2, r2, #17
	subs r2, r2, r3
	movs r3, #128
	lsls r3, r3, #17
	str r2, [r5, #4]
	str r3, [r5, #8]
	ldmia r6!, {r0}
	adds r2, r4, #0
	movs r3, #0
	adds r1, r5, #0
	str r4, [sp, #8]
	bl Func_08020010
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #4
	ldr r4, [sp, #8]
	cmp r0, #11
	bne .L_081881de
.L_0818821a:
	ldr r2, [sp, #60]
	ldr r4, [sp, #64]
	ldr r1, [sp, #52]
	ldr r3, [sp, #56]
	ldr r5, [sp, #76]
	adds r1, r1, r2
	adds r3, r3, r4
	str r1, [sp, #60]
	str r3, [sp, #64]
	cmp r5, #19
	bhi .L_08188244
	ldr r7, [sp, #52]
	ldr r1, [sp, #56]
	ldr r0, .L_08188278
	movs r2, #128
	lsls r2, r2, #8
	adds r0, r7, r0
	adds r2, r1, r2
	str r0, [sp, #52]
	str r2, [sp, #56]
	b .L_08188298
.L_08188244:
	mov r3, r10
	cmp r3, #19
	bhi .L_08188288
	ldr r4, [sp, #52]
	ldr r7, [sp, #56]
	movs r5, #128
	movs r0, #128
	lsls r5, r5, #8
	lsls r0, r0, #7
	adds r5, r4, r5
	adds r0, r7, r0
	str r5, [sp, #52]
	str r0, [sp, #56]
	b .L_08188298
.L_08188260:
	.4byte Data_02014018
.L_08188264:
	.4byte 0xfff60000
.L_08188268:
	.4byte 0xffea0000
.L_0818826c:
	.4byte 0xfff80000
.L_08188270:
	.4byte Data_08197410
.L_08188274:
	.4byte Data_02014000
.L_08188278:
	.4byte 0xffff8000
.L_0818827c:
	.4byte Data_08196f10
.L_08188280:
	.4byte 0xfffeffff
.L_08188284:
	.4byte Data_08199a12
.L_08188288:
	ldr r3, [sp, #76]
	subs r3, #40
	cmp r3, #19
	bhi .L_08188298
	ldr r1, [sp, #52]
	ldr r2, .L_08188524
	adds r2, r1, r2
	str r2, [sp, #52]
.L_08188298:
	ldr r3, [sp, #76]
	cmp r3, #10
	bne .L_081882a4
	movs r0, #212
	bl Audio_PlayCue
.L_081882a4:
	ldr r4, [sp, #76]
	cmp r4, #14
	bne .L_08188306
	movs r0, #144
	bl Audio_PlayCue
	ldr r7, [sp, #100]
	movs r5, #0
	ldr r3, [r7, #20]
	mov r8, r5
	cmp r3, #0
	beq .L_08188306
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	movs r6, #128
	adds r1, #168
	lsls r6, r6, #11
	movs r5, #36
	adds r7, r0, r1
.L_081882cc:
	ldr r2, [sp, #100]
	movs r1, #1
	ldrsh r0, [r5, r2]
	movs r3, #150
	str r3, [sp, #4]
	movs r3, #128
	adds r2, r6, #0
	lsls r3, r3, #12
	str r6, [sp, #0]
	bl Func_0815f000
	ldr r4, [sp, #100]
	movs r3, #16
	ldrsh r0, [r5, r4]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	bl Func_0814cd48
	movs r3, #8
	str r3, [r7]
	ldr r4, [sp, #100]
	movs r2, #1
	ldr r3, [r4, #20]
	add r8, r2
	adds r5, #2
	cmp r8, r3
	bne .L_081882cc
.L_08188306:
	ldr r5, [sp, #76]
	cmp r5, #30
	bne .L_08188312
	movs r0, #212
	bl Audio_PlayCue
.L_08188312:
	ldr r7, [sp, #76]
	cmp r7, #34
	bne .L_08188374
	movs r0, #144
	bl Audio_PlayCue
	ldr r1, [sp, #100]
	movs r0, #0
	ldr r3, [r1, #20]
	mov r8, r0
	cmp r3, #0
	beq .L_08188374
	ldr r2, [sp, #88]
	movs r3, #238
	lsls r3, r3, #7
	movs r6, #128
	adds r3, #168
	lsls r6, r6, #11
	movs r5, #36
	adds r7, r2, r3
.L_0818833a:
	ldr r4, [sp, #100]
	movs r3, #50
	ldrsh r0, [r5, r4]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #1
	adds r2, r6, #0
	str r6, [sp, #0]
	bl Func_0815f000
	ldr r2, [sp, #100]
	movs r1, #7
	ldrsh r0, [r5, r2]
	movs r3, #16
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl Func_0814cd48
	movs r3, #8
	str r3, [r7]
	ldr r0, [sp, #100]
	movs r4, #1
	ldr r3, [r0, #20]
	add r8, r4
	adds r5, #2
	cmp r8, r3
	bne .L_0818833a
.L_08188374:
	ldr r1, [sp, #76]
	cmp r1, #58
	bne .L_081883d6
	movs r0, #145
	bl Audio_PlayCue
	ldr r4, [sp, #100]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r8, r2
	cmp r3, #0
	beq .L_081883d6
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	movs r6, #128
	adds r1, #168
	lsls r6, r6, #11
	movs r5, #36
	adds r7, r0, r1
.L_0818839c:
	ldr r2, [sp, #100]
	movs r1, #1
	ldrsh r0, [r5, r2]
	movs r3, #200
	str r3, [sp, #4]
	movs r3, #128
	adds r2, r6, #0
	lsls r3, r3, #12
	str r6, [sp, #0]
	bl Func_0815f000
	ldr r4, [sp, #100]
	movs r3, #16
	ldrsh r0, [r5, r4]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	bl Func_0814cd48
	movs r3, #8
	str r3, [r7]
	ldr r4, [sp, #100]
	movs r2, #1
	ldr r3, [r4, #20]
	add r8, r2
	adds r5, #2
	cmp r8, r3
	bne .L_0818839c
.L_081883d6:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #1
	bl Func_081969f8
	movs r3, #8
	adds r6, r0, #0
	negs r3, r3
	str r3, [r6, #20]
	movs r3, #7
	str r3, [r6]
	add r5, sp, #104
	movs r3, #6
	strb r3, [r5]
	mov r2, r9
	movs r3, #5
	strb r3, [r2, #1]
	ldr r3, .L_08188528
	str r2, [r6, #16]
	str r3, [r6, #8]
	str r7, [r6, #12]
	ldr r0, [sp, #88]
	movs r1, #224
	lsls r1, r1, #3
	adds r3, r0, r1
	str r3, [r2, #4]
	ldr r3, [sp, #20]
	movs r2, #127
	ands r3, r2
	strb r3, [r6, #24]
	movs r2, #0
	mov r8, r2
.L_0818841a:
	ldr r3, .L_0818852c
	mov r4, r8
	ldrb r2, [r3, r4]
	ldr r5, [sp, #76]
	cmp r5, r2
	blt .L_081884b0
	adds r3, r2, #0
	adds r3, #16
	cmp r5, r3
	bge .L_081884b0
	subs r0, r5, r2
	lsls r0, r0, #11
	bl Trig_Sin
	cmp r0, #0
	bge .L_0818843c
	adds r0, #7
.L_0818843c:
	asrs r0, r0, #3
	movs r1, #4
	ldr r5, .L_08188530
	mov r11, r0
	negs r1, r1
	movs r0, #0
	mov r10, r0
	mov r9, r1
.L_0818844c:
	bl Func_08014de4
	mov r2, r8
	cmp r2, #0
	bne .L_0818846a
	mov r3, r9
	adds r0, r5, #0
	lsls r1, r3, #16
	movs r2, #0
	bl Func_08015160
	ldr r0, .L_08188534
	bl Func_080150e4
	b .L_08188482
.L_0818846a:
	ldr r4, .L_08188530
	mov r2, r9
	adds r0, r5, r4
	lsls r1, r2, #16
	movs r2, #0
	bl Func_08015160
	movs r0, #156
	lsls r0, r0, #6
	adds r0, #208
	bl Func_080150e4
.L_08188482:
	movs r0, #192
	movs r2, #128
	lsls r0, r0, #10
	mov r1, r11
	lsls r2, r2, #10
	bl Func_080151e4
	ldr r0, .L_08188538
	adds r1, r7, #0
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	movs r4, #1
	movs r3, #160
	add r10, r4
	lsls r3, r3, #13
	mov r0, r10
	adds r5, r5, r3
	cmp r0, #3
	bne .L_0818844c
.L_081884b0:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #2
	bne .L_0818841a
	movs r3, #6
	add r4, sp, #104
	strb r3, [r4]
	adds r1, r4, #0
	movs r3, #5
	strb r3, [r1, #1]
	ldr r3, .L_08188528
	mov r9, r1
	str r3, [r6, #8]
	movs r3, #7
	str r3, [r6]
	ldr r3, [sp, #76]
	subs r3, #56
	cmp r3, #31
	bhi .L_08188570
	ldr r5, [sp, #76]
	movs r3, #0
	cmp r5, #79
	ble .L_081884e6
	movs r3, #80
	subs r3, r3, r5
	lsls r3, r3, #3
.L_081884e6:
	str r3, [r6, #20]
	ldr r2, [sp, #20]
	movs r3, #127
	ands r2, r3
	strb r2, [r6, #24]
	ldr r0, [sp, #88]
	movs r2, #224
	lsls r2, r2, #3
	adds r3, r0, r2
	str r3, [r1, #4]
	ldr r3, [sp, #76]
	cmp r3, #71
	bgt .L_08188508
	ldr r4, .L_0818853c
	lsls r0, r3, #10
	adds r0, r0, r4
	b .L_08188514
.L_08188508:
	ldr r5, [sp, #76]
	cmp r5, #79
	ble .L_08188544
	ldr r1, .L_08188540
	lsls r0, r5, #10
	adds r0, r0, r1
.L_08188514:
	bl Trig_Sin
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r0, r3, #16
	b .L_08188546
	.2byte 0x0000
.L_08188524:
	.4byte 0xffff8000
.L_08188528:
	.4byte Data_081992f8
.L_0818852c:
	.4byte Data_08199d2f
.L_08188530:
	.4byte 0xfff00000
.L_08188534:
	.4byte 0xffffe000
.L_08188538:
	.4byte Data_081991e0
.L_0818853c:
	.4byte 0xffff2000
.L_08188540:
	.4byte 0xffff0000
.L_08188544:
	movs r0, #80
.L_08188546:
	ldr r3, .L_0818856c
	movs r1, #4
	subs r3, r3, r0
	strh r3, [r7, #2]
	strh r3, [r7, #10]
	adds r3, r0, #0
	movs r2, #123
	adds r3, #60
	strh r1, [r7]
	strh r2, [r7, #8]
	strh r1, [r7, #16]
	strh r3, [r7, #18]
	strh r2, [r7, #24]
	strh r3, [r7, #26]
	adds r0, r6, #0
	bl Func_08196a7c
	b .L_08188570
	.2byte 0x0000
.L_0818856c:
	.4byte 0x0000003c
.L_08188570:
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r7, #0
	bl Sys_Free
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #88]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #20]
	ldr r7, [sp, #76]
	adds r5, #8
	adds r7, #1
	str r5, [sp, #20]
	str r7, [sp, #76]
	cmp r7, #92
	beq .L_081885ae
	b .L_08187fe4
.L_081885ae:
	ldr r1, [sp, #88]
	movs r2, #238
	lsls r2, r2, #7
	movs r0, #0
	adds r2, #220
	mov r8, r0
	adds r5, r1, r2
.L_081885bc:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #11
	bne .L_081885bc
	ldr r0, .L_081885ec
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #192
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081885ec:
	.4byte Func_08143000
