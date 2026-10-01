.syntax unified
	.thumb
	.global Func_08179f18
	.thumb_func
Func_08179f18:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #292
	str r0, [sp, #180]
	movs r2, #192
	lsls r2, r2, #18
	ldr r0, [r2, #96]
	ldr r3, .L_0817a0e8
	str r0, [sp, #176]
	movs r0, #0
	ldr r1, [r2, #92]
	movs r5, #239
	str r1, [sp, #172]
	lsls r5, r5, #7
	ldrh r3, [r3, #4]
	movs r6, #80
	str r3, [sp, #156]
	adds r3, r2, #0
	adds r3, #176
	ldr r3, [r3]
	negs r6, r6
	str r3, [sp, #152]
	ldr r3, .L_0817a0ec
	ldr r2, [r2, #100]
	mov r9, r3
	str r2, [sp, #148]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	bl Func_08179e6c
	ldr r4, [sp, #172]
	movs r3, #0
	adds r2, r4, r5
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0817a0f0
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	str r6, [sp, #144]
	str r0, [sp, #160]
.L_08179f7a:
	ldr r1, [sp, #160]
	cmp r1, #27
	beq .L_08179f82
	b .L_0817a1a4
.L_08179f82:
	ldr r2, .L_0817a0f4
	movs r3, #240
	str r3, [r2, #16]
	ldr r2, [sp, #172]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #240
	adds r3, r2, r4
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	ldr r1, .L_0817a0f8
	movs r2, #0
	bl Func_08118040
	movs r6, #238
	ldr r5, [sp, #172]
	lsls r6, r6, #7
	movs r0, #238
	adds r6, #144
	lsls r0, r0, #7
	adds r3, r5, r6
	movs r1, #0
	adds r0, #148
	str r1, [r3]
	movs r2, #1
	adds r3, r5, r0
	str r2, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #152
	movs r4, #238
	adds r2, r5, r3
	lsls r4, r4, #7
	movs r3, #3
	adds r4, #156
	negs r3, r3
	str r3, [r2]
	adds r3, r5, r4
	str r1, [r3]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0817a0fc
	bl Scheduler_AddOrUpdateCallback
	ldr r6, [sp, #152]
	movs r5, #1
	str r5, [r6, #16]
	ldr r0, .L_0817a100
	bl Resource_GetTableEntry
	adds r6, r0, #0
	ldr r5, .L_0817a104
	adds r1, r6, #0
	movs r2, #32
	adds r6, #32
	ldr r0, .L_0817a108
	mov lr, r5
	.2byte 0xf800
	adds r0, r6, #0
	ldr r1, .L_0817a10c
	bl Func_0801587c
	ldr r1, .L_0817a110
	mov r8, r5
	movs r7, #240
	ldr r5, .L_0817a10c
	movs r0, #0
	lsls r7, r7, #7
	mov r10, r0
	mov r11, r1
	mov r6, r9
	adds r7, #12
.L_0817a016:
	movs r2, #128
	movs r3, #240
	movs r1, #16
	lsls r2, r2, #23
	lsls r3, r3, #8
	movs r0, #16
	bl Func_0815b290
	ldr r2, [sp, #172]
	ldrb r3, [r0, #9]
	movs r4, #13
	negs r4, r4
	str r0, [r7, r2]
	adds r2, r4, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	ldr r1, .L_0817a114
	lsls r3, r3, #2
	add r3, r11
	ldrh r0, [r3, #2]
	movs r2, #128
	adds r0, r0, r1
	adds r1, r5, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, [sp, #172]
	adds r5, #128
	ldr r3, [r7, r2]
	adds r7, #4
	ldrh r3, [r3, #8]
	lsls r3, r3, #22
	lsrs r3, r3, #22
	strh r3, [r6]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r6, #2
	cmp r4, #15
	bne .L_0817a016
	movs r0, #240
	lsls r0, r0, #7
	ldr r7, .L_0817a104
	movs r5, #0
	adds r0, #72
	mov r10, r5
	adds r6, r2, r0
.L_0817a078:
	movs r2, #128
	movs r3, #240
	lsls r3, r3, #8
	movs r1, #16
	lsls r2, r2, #23
	movs r0, #16
	bl Func_0815b3b0
	stmia r6!, {r0}
	ldr r1, [sp, #172]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #12
	adds r5, r1, r2
	ldr r1, [r5]
	movs r2, #24
	mov lr, r7
	.2byte 0xf800
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #17
	bne .L_0817a078
	movs r6, #0
	mov r10, r6
	ldr r7, .L_0817a0e0
	ldr r6, .L_0817a118
.L_0817a0ae:
	bl Random16
	movs r1, #15
	bl Math_ModU
	lsls r0, r0, #8
	strh r0, [r6]
	ldmia r5!, {r0}
	ldrh r3, [r6]
	mov r2, r9
	lsrs r3, r3, #8
	lsls r3, r3, #1
	ldrh r1, [r2, r3]
	ldrh r3, [r0, #8]
	ldr r2, .L_0817a0e4
	ands r1, r7
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r6, #2
	b .L_0817a11c
	.2byte 0x0000
.L_0817a0e0:
	.4byte 0x000003ff
.L_0817a0e4:
	.4byte 0xfffffc00
.L_0817a0e8:
	.4byte Data_03001120
.L_0817a0ec:
	.4byte Data_02014000
.L_0817a0f0:
	.4byte Func_08143000
.L_0817a0f4:
	.4byte gCameraSceneParameters
.L_0817a0f8:
	.4byte 0x00000045
.L_0817a0fc:
	.4byte Func_0813baec
.L_0817a100:
	.4byte 0x0000009a
.L_0817a104:
	.4byte IwramCopyWords
.L_0817a108:
	.4byte 0x050003e0
.L_0817a10c:
	.4byte gMapCellBuffer
.L_0817a110:
	.4byte ResourceTableEntries
.L_0817a114:
	.4byte 0x06010000
.L_0817a118:
	.4byte Data_0201401e
.L_0817a11c:
	cmp r4, #32
	bne .L_0817a0ae
	ldr r0, .L_0817a294
	bl Resource_GetTableEntry
	movs r5, #240
	adds r6, r0, #0
	lsls r5, r5, #1
	adds r6, r6, r5
	adds r0, r6, #0
	ldr r1, .L_0817a298
	bl Func_0801587c
	movs r6, #0
	ldr r0, .L_0817a29c
	ldr r1, .L_0817a2a0
	ldr r2, [sp, #172]
	movs r3, #238
	mov r10, r6
	lsls r3, r3, #7
	movs r4, #13
	ldr r6, .L_0817a298
	adds r3, #220
	negs r4, r4
	mov r11, r0
	mov r8, r1
	adds r5, r2, r3
	adds r7, r4, #0
.L_0817a154:
	movs r1, #32
	ldr r2, .L_0817a2a4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #8
	ands r3, r7
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r11
	ldrh r0, [r3, #2]
	ldr r1, .L_0817a2a8
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #3
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	movs r3, #1
	movs r2, #128
	add r10, r3
	lsls r2, r2, #3
	mov r4, r10
	adds r6, r6, r2
	cmp r4, #12
	bne .L_0817a154
	ldr r5, [sp, #172]
	movs r6, #224
	lsls r6, r6, #3
	ldr r0, .L_0817a2ac
	adds r1, r5, r6
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_0817a1a4:
	ldr r0, [sp, #144]
	movs r3, #128
	adds r0, #4
	lsls r3, r3, #19
	lsls r2, r0, #8
	adds r3, #40
	str r0, [sp, #144]
	str r2, [r3]
	ldr r1, [sp, #172]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	adds r3, r1, r2
	movs r4, #1
	str r4, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #160]
	adds r5, #1
	str r5, [sp, #160]
	cmp r5, #52
	beq .L_0817a1d4
	b .L_08179f7a
.L_0817a1d4:
	ldr r5, .L_0817a2b0
	ldr r1, .L_0817a298
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	adds r0, r5, #0
	ldr r1, [sp, #148]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0817a2b4
	movs r6, #0
	movs r1, #63
	mov r10, r6
	mov r12, r0
	movs r7, #4
	movs r5, #0
	movs r4, #0
	mov lr, r1
.L_0817a200:
	mov r2, r10
	ldr r6, [sp, #148]
	adds r3, r4, r2
	lsls r3, r3, #1
	movs r2, #128
	adds r3, r3, r6
	lsls r2, r2, #2
	mov r8, r3
	adds r2, #78
	movs r1, #0
	adds r0, r5, #0
	add r2, r8
.L_0817a218:
	mov r6, r12
	ldrh r3, [r6, r7]
	ldr r6, .L_0817a298
	adds r3, r3, r1
	adds r3, r3, r6
	ldrb r3, [r3]
	cmp r3, #0
	ble .L_0817a232
	mov r6, lr
	subs r3, r6, r0
	cmp r3, #0
	bgt .L_0817a232
	movs r3, #1
.L_0817a232:
	adds r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r1, #18
	bne .L_0817a218
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r5, #7
	adds r4, #8
	cmp r1, #10
	bne .L_0817a200
	ldr r3, .L_0817a284
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_0817a288
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0817a28c
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_0817a290
	subs r2, #2
	strh r3, [r2]
	subs r2, #40
	movs r3, #0
	str r3, [r2]
	movs r5, #128
	movs r2, #128
	movs r3, #44
	lsls r5, r5, #2
	str r2, [sp, #140]
	str r3, [sp, #136]
	movs r4, #0
	str r5, [sp, #128]
	mov r0, r9
	movs r5, #128
	mov r1, r9
	b .L_0817a2b8
	.2byte 0x0000
.L_0817a284:
	.4byte 0x00007741
.L_0817a288:
	.4byte 0x00000100
.L_0817a28c:
	.4byte 0x0000100e
.L_0817a290:
	.4byte 0x00003f44
.L_0817a294:
	.4byte 0x00000098
.L_0817a298:
	.4byte gMapCellBuffer
.L_0817a29c:
	.4byte ResourceTableEntries
.L_0817a2a0:
	.4byte IwramCopyWords
.L_0817a2a4:
	.4byte 0x80002000
.L_0817a2a8:
	.4byte 0x06010000
.L_0817a2ac:
	.4byte 0x000000c2
.L_0817a2b0:
	.4byte 0x00000134
.L_0817a2b4:
	.4byte Data_08197410
.L_0817a2b8:
	str r4, [sp, #132]
	str r4, [sp, #124]
	str r4, [sp, #120]
	mov r10, r4
	lsls r5, r5, #1
	movs r4, #22
	adds r0, #224
	adds r1, #96
.L_0817a2c8:
	mov r3, r10
	cmp r3, #0
	bge .L_0817a2d0
	adds r3, #7
.L_0817a2d0:
	asrs r3, r3, #3
	lsls r2, r3, #3
	mov r6, r10
	subs r2, r6, r2
	lsls r2, r2, #5
	muls r3, r4
	adds r2, r2, r5
	stmia r1!, {r2}
	movs r2, #1
	adds r3, #128
	add r10, r2
	stmia r0!, {r3}
	mov r3, r10
	cmp r3, #32
	bne .L_0817a2c8
	ldr r4, [sp, #172]
	movs r5, #239
	movs r6, #238
	lsls r5, r5, #7
	lsls r6, r6, #7
	adds r2, r4, r5
	movs r3, #2
	adds r6, #132
	str r3, [r2]
	adds r2, r4, r6
	movs r3, #50
	str r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r0, #0
	str r3, [sp, #164]
	str r0, [sp, #160]
	ldr r3, .L_0817a390
	movs r1, #138
	ldr r3, [r3, #12]
	lsls r1, r1, #1
	mov r2, sp
	mov r3, sp
	add r1, sp
	adds r2, #192
	adds r3, #184
	str r1, [sp, #48]
	str r2, [sp, #52]
	str r3, [sp, #56]
.L_0817a332:
	ldr r4, [sp, #160]
	cmp r4, #0
	bne .L_0817a3b2
	movs r1, #128
	ldr r5, .L_0817a394
	lsls r1, r1, #7
	ldr r0, .L_0817a398
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	ldr r0, [sp, #176]
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	movs r3, #152
	ldr r2, .L_0817a388
	movs r5, #0
	lsls r3, r3, #2
	mov r10, r5
	add r3, r9
.L_0817a35a:
	movs r6, #1
	add r10, r6
	mov r0, r10
	strh r2, [r3]
	adds r3, #2
	cmp r0, #240
	bne .L_0817a35a
	movs r2, #240
	ldr r3, .L_0817a39c
	ldr r1, .L_0817a3a0
	lsls r2, r2, #1
	ldr r0, .L_0817a3a4
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0817a38c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	bl Func_0815b410
	b .L_0817a3a8
	.2byte 0x0000
.L_0817a388:
	.4byte 0x00007fff
.L_0817a38c:
	.4byte 0x00000100
.L_0817a390:
	.4byte gInput
.L_0817a394:
	.4byte IwramClearWords
.L_0817a398:
	.4byte 0x06004000
.L_0817a39c:
	.4byte IwramCopyWords
.L_0817a3a0:
	.4byte Data_02014260
.L_0817a3a4:
	.4byte 0x05000200
.L_0817a3a8:
	movs r2, #128
	ldr r3, .L_0817a3d4
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
.L_0817a3b2:
	ldr r1, [sp, #160]
	movs r2, #30
	adds r2, #255
	cmp r1, r2
	ble .L_0817a3c4
	ldr r0, .L_0817a3d8
	bl Func_0815f0a0
	b .L_0817a3ec
.L_0817a3c4:
	ldr r3, [sp, #160]
	cmp r3, #199
	ble .L_0817a3e0
	ldr r0, .L_0817a3dc
	bl Func_0815f0a0
	b .L_0817a3ec
	.2byte 0x0000
.L_0817a3d4:
	.4byte 0xffffc400
.L_0817a3d8:
	.4byte 0x00000166
.L_0817a3dc:
	.4byte 0x00000167
.L_0817a3e0:
	ldr r4, [sp, #160]
	cmp r4, #180
	ble .L_0817a3ec
	ldr r0, .L_0817a434
	bl Func_0815f0a0
.L_0817a3ec:
	ldr r5, [sp, #160]
	cmp r5, #108
	bne .L_0817a44c
	ldr r3, .L_0817a430
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r3, #0
	adds r2, #8
	str r3, [r2]
	movs r5, #192
	movs r2, #128
	lsls r5, r5, #8
	ldr r0, .L_0817a438
	movs r1, #8
	lsls r2, r2, #9
	bl Func_0815b434
	ldr r0, .L_0817a43c
	movs r1, #16
	adds r2, r5, #0
	bl Func_0815b434
	ldr r0, .L_0817a440
	movs r1, #32
	adds r2, r5, #0
	bl Func_0815b434
	ldr r0, .L_0817a444
	movs r1, #48
	adds r2, r5, #0
	b .L_0817a448
	.2byte 0x0000
.L_0817a430:
	.4byte 0x00000080
.L_0817a434:
	.4byte 0x00000150
.L_0817a438:
	.4byte gMapCellBuffer
.L_0817a43c:
	.4byte Data_02010020
.L_0817a440:
	.4byte Data_020100a0
.L_0817a444:
	.4byte Data_020102a0
.L_0817a448:
	bl Func_0815b434
.L_0817a44c:
	ldr r3, [sp, #160]
	subs r3, #223
	cmp r3, #29
	bhi .L_0817a474
	bl Random16
	movs r3, #3
	ands r0, r3
	ldr r3, .L_0817a4a0
	ldr r2, .L_0817a4a4
	ldrb r3, [r3, r0]
	lsls r0, r0, #1
	ldrh r0, [r2, r0]
	ldr r6, .L_0817a4a8
	lsls r3, r3, #1
	adds r0, r0, r6
	movs r1, #44
	movs r2, #78
	bl Func_0818caa8
.L_0817a474:
	ldr r0, [sp, #160]
	subs r0, #112
	mov r8, r0
	cmp r0, #31
	bhi .L_0817a516
	ldr r0, .L_0817a4ac
	bl Resource_GetTableEntry
	ldr r2, .L_0817a49c
	movs r4, #152
	movs r1, #0
	movs r3, #31
	lsls r4, r4, #2
	adds r7, r0, #0
	mov r10, r1
	mov lr, r2
	mov r11, r3
	mov r12, r4
	b .L_0817a4b0
	.2byte 0x0000
.L_0817a49c:
	.4byte 0x0000001f
.L_0817a4a0:
	.4byte Data_08199421
.L_0817a4a4:
	.4byte Data_08199426
.L_0817a4a8:
	.4byte gMapCellBuffer
.L_0817a4ac:
	.4byte 0x00000098
.L_0817a4b0:
	mov r5, r12
	mov r6, r9
	ldrh r2, [r5, r6]
	mov r1, lr
	lsls r3, r2, #16
	lsrs r0, r3, #26
	lsrs r5, r3, #21
	ands r0, r1
	ands r5, r1
	ldrh r1, [r7]
	mov r6, r11
	lsls r3, r1, #16
	mov r4, lr
	ands r6, r2
	lsrs r2, r3, #26
	lsrs r3, r3, #21
	ands r2, r4
	ands r3, r4
	mov r4, r11
	ands r4, r1
	cmp r0, r2
	ble .L_0817a4de
	subs r0, #1
.L_0817a4de:
	cmp r5, r3
	ble .L_0817a4e4
	subs r5, #1
.L_0817a4e4:
	cmp r6, r4
	ble .L_0817a4ea
	subs r6, #1
.L_0817a4ea:
	lsls r2, r5, #5
	lsls r3, r0, #10
	movs r1, #1
	orrs r3, r2
	add r10, r1
	orrs r3, r6
	mov r5, r12
	mov r6, r9
	movs r0, #2
	mov r2, r10
	strh r3, [r5, r6]
	add r12, r0
	adds r7, #2
	cmp r2, #240
	bne .L_0817a4b0
	movs r2, #240
	ldr r3, .L_0817a844
	ldr r0, .L_0817a848
	ldr r1, .L_0817a84c
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
.L_0817a516:
	mov r3, r8
	cmp r3, #7
	bhi .L_0817a536
	lsls r3, r3, #8
	add r3, r8
	lsls r2, r3, #16
	adds r3, r3, r2
	ldr r2, .L_0817a850
	lsls r3, r3, #3
	movs r1, #128
	subs r2, r2, r3
	ldr r0, [sp, #176]
	ldr r3, .L_0817a854
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
.L_0817a536:
	ldr r3, .L_0817a858
	ldr r5, [sp, #48]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r7, #0
	str r3, [sp, #192]
	str r4, [sp, #196]
	movs r3, #255
	movs r4, #0
	lsls r3, r3, #16
	str r4, [r5, #12]
	str r3, [r5, #4]
	ldr r6, [sp, #160]
	mov r8, r4
	cmp r6, #119
	bgt .L_0817a582
	movs r5, #205
	lsls r5, r5, #8
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #152
	lsls r3, r3, #3
	lsls r0, r0, #16
	adds r0, r3, r0
	str r0, [sp, #140]
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r1, #176
	lsls r3, r3, #2
	lsls r1, r1, #14
	adds r1, r3, r1
	b .L_0817a674
.L_0817a582:
	ldr r2, [sp, #160]
	cmp r2, #179
	bgt .L_0817a5bc
	lsls r5, r2, #1
	ldr r3, .L_0817a85c
	adds r5, r5, r2
	lsls r5, r5, #8
	adds r5, r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r4, #152
	lsls r3, r3, #3
	lsls r4, r4, #16
	adds r4, r3, r4
	adds r0, r5, #0
	str r4, [sp, #140]
	bl Trig_Cos
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r5, #176
	lsls r3, r3, #2
	lsls r5, r5, #14
	adds r5, r3, r5
	str r5, [sp, #136]
	b .L_0817a676
.L_0817a5bc:
	ldr r6, [sp, #160]
	cmp r6, #221
	bgt .L_0817a5d6
	ldr r0, [sp, #140]
	ldr r2, [sp, #136]
	ldr r1, .L_0817a860
	movs r3, #128
	lsls r3, r3, #8
	adds r0, r0, r1
	adds r2, r2, r3
	str r0, [sp, #140]
	str r2, [sp, #136]
	b .L_0817a676
.L_0817a5d6:
	ldr r4, [sp, #160]
	cmp r4, #251
	bgt .L_0817a65e
	adds r0, r4, #0
	movs r1, #6
	bl Math_Mod
	cmp r0, #1
	bne .L_0817a5ea
	movs r7, #1
.L_0817a5ea:
	cmp r0, #2
	bne .L_0817a5f0
	movs r7, #1
.L_0817a5f0:
	cmp r0, #3
	bne .L_0817a5fc
	movs r5, #1
	negs r5, r5
	movs r7, #2
	mov r8, r5
.L_0817a5fc:
	cmp r0, #4
	bne .L_0817a608
	movs r6, #1
	negs r6, r6
	movs r7, #2
	mov r8, r6
.L_0817a608:
	cmp r0, #5
	bne .L_0817a614
	movs r1, #1
	negs r1, r1
	movs r7, #2
	mov r8, r1
.L_0817a614:
	cmp r0, #0
	bne .L_0817a61e
	movs r2, #0
	movs r7, #0
	mov r8, r2
.L_0817a61e:
	ldr r4, [sp, #160]
	movs r3, #3
	ands r3, r4
	cmp r3, #0
	bne .L_0817a62e
	ldr r2, .L_0817a864
	movs r3, #30
	strh r3, [r2, #6]
.L_0817a62e:
	ldr r3, [sp, #160]
	cmp r3, #0
	bge .L_0817a636
	adds r3, #3
.L_0817a636:
	ldr r5, [sp, #160]
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r1, r5, r3
	cmp r1, #1
	bne .L_0817a648
	ldr r2, .L_0817a864
	movs r3, #30
	strh r3, [r2, #6]
.L_0817a648:
	cmp r1, #2
	bne .L_0817a652
	ldr r2, .L_0817a864
	movs r3, #34
	strh r3, [r2, #6]
.L_0817a652:
	cmp r1, #3
	bne .L_0817a676
	ldr r2, .L_0817a864
	movs r3, #34
	strh r3, [r2, #6]
	b .L_0817a676
.L_0817a65e:
	ldr r2, .L_0817a864
	movs r3, #32
	strh r3, [r2, #6]
	ldr r6, [sp, #140]
	ldr r1, [sp, #136]
	ldr r2, .L_0817a868
	movs r0, #128
	lsls r0, r0, #12
	adds r6, r6, r0
	adds r1, r1, r2
	str r6, [sp, #140]
.L_0817a674:
	str r1, [sp, #136]
.L_0817a676:
	ldr r3, [sp, #160]
	cmp r3, #111
	ble .L_0817a6ca
	ldr r0, [sp, #172]
	movs r1, #238
	lsls r1, r1, #7
	ldr r5, [sp, #48]
	movs r4, #0
	adds r1, #220
	mov r10, r4
	adds r6, r0, r1
.L_0817a68c:
	mov r0, r10
	movs r1, #3
	bl Math_Mod
	ldr r2, [sp, #140]
	lsls r0, r0, #5
	adds r0, r0, r7
	lsls r0, r0, #16
	adds r0, r2, r0
	str r0, [r5]
	movs r1, #3
	mov r0, r10
	bl Math_Div
	ldr r3, [sp, #136]
	lsls r0, r0, #5
	add r0, r8
	lsls r0, r0, #16
	adds r0, r3, r0
	str r0, [r5, #8]
	adds r1, r5, #0
	ldmia r6!, {r0}
	ldr r2, [sp, #52]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r4, #1
	add r10, r4
	mov r0, r10
	cmp r0, #12
	bne .L_0817a68c
.L_0817a6ca:
	movs r1, #0
	movs r2, #128
	movs r4, #240
	movs r6, #176
	lsls r2, r2, #17
	mov r10, r1
	movs r3, #28
	lsls r4, r4, #1
	movs r5, #224
	lsls r6, r6, #1
	movs r0, #96
	str r1, [sp, #20]
	movs r1, #211
	mov r11, r2
	str r3, [sp, #40]
	movs r2, #224
	str r4, [sp, #36]
	str r5, [sp, #32]
	str r6, [sp, #28]
	str r0, [sp, #24]
	str r1, [sp, #16]
	mov r7, r9
	add r2, r9
	adds r7, #96
	mov r8, r2
.L_0817a6fc:
	mov r3, r10
	cmp r3, #0
	bne .L_0817a778
	ldr r4, [sp, #160]
	cmp r4, #0
	bne .L_0817a71a
	movs r3, #240
	movs r6, #192
	mov r5, r9
	lsls r3, r3, #16
	lsls r6, r6, #14
	movs r0, #224
	str r3, [r5, #96]
	str r6, [r5, r0]
	b .L_0817a76a
.L_0817a71a:
	ldr r1, [sp, #160]
	cmp r1, #47
	bgt .L_0817a73a
	mov r4, r9
	movs r3, #96
	ldr r2, [r4, r3]
	ldr r5, .L_0817a86c
	movs r6, #224
	adds r2, r2, r5
	str r2, [r4, r3]
	movs r0, #192
	ldr r3, [r4, r6]
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r4, r6]
	b .L_0817a76a
.L_0817a73a:
	ldr r1, [sp, #160]
	cmp r1, #79
	bgt .L_0817a756
	mov r4, r9
	movs r2, #96
	ldr r3, [r4, r2]
	ldr r1, .L_0817a870
	movs r5, #224
	adds r3, r3, r1
	str r3, [r4, r2]
	ldr r3, [r4, r5]
	adds r3, r3, r1
	str r3, [r4, r5]
	b .L_0817a76a
.L_0817a756:
	ldr r6, [sp, #160]
	cmp r6, #111
	bgt .L_0817a76a
	movs r3, #96
	mov r0, r9
	ldr r2, [r0, r3]
	movs r1, #192
	lsls r1, r1, #10
	adds r2, r2, r1
	str r2, [r0, r3]
.L_0817a76a:
	ldr r3, [sp, #160]
	cmp r3, #112
	beq .L_0817a772
	b .L_0817ac5a
.L_0817a772:
	mov r4, r11
	str r4, [r7]
	b .L_0817ac5a
.L_0817a778:
	mov r5, r10
	cmp r5, #1
	bne .L_0817a810
	ldr r6, [sp, #160]
	cmp r6, #27
	bgt .L_0817a792
	movs r2, #192
	mov r1, r9
	mov r0, r11
	lsls r2, r2, #14
	movs r3, #228
	str r0, [r1, #100]
	b .L_0817a800
.L_0817a792:
	ldr r4, [sp, #160]
	cmp r4, #28
	bne .L_0817a7aa
	movs r3, #128
	lsls r3, r3, #13
	mov r5, r9
	str r3, [r5, #100]
	movs r3, #128
	lsls r3, r3, #14
	movs r6, #228
	str r3, [r5, r6]
	b .L_0817a802
.L_0817a7aa:
	ldr r0, [sp, #160]
	cmp r0, #62
	bgt .L_0817a7c8
	mov r1, r9
	movs r3, #100
	ldr r2, [r1, r3]
	movs r4, #235
	lsls r4, r4, #9
	adds r4, #216
	adds r2, r2, r4
	str r2, [r1, r3]
	movs r5, #228
	ldr r3, [r1, r5]
	ldr r6, .L_0817a874
	b .L_0817a7e8
.L_0817a7c8:
	ldr r0, [sp, #160]
	cmp r0, #79
	bgt .L_0817a7ee
	mov r1, r9
	movs r3, #100
	ldr r2, [r1, r3]
	movs r4, #206
	lsls r4, r4, #9
	adds r4, #64
	adds r2, r2, r4
	str r2, [r1, r3]
	movs r5, #228
	ldr r3, [r1, r5]
	movs r6, #206
	lsls r6, r6, #7
	adds r6, #16
.L_0817a7e8:
	adds r3, r3, r6
	str r3, [r1, r5]
	b .L_0817a802
.L_0817a7ee:
	ldr r0, [sp, #160]
	cmp r0, #141
	bgt .L_0817a802
	movs r3, #100
	mov r1, r9
	ldr r2, [r1, r3]
	movs r4, #192
	lsls r4, r4, #10
	adds r2, r2, r4
.L_0817a800:
	str r2, [r1, r3]
.L_0817a802:
	ldr r6, [sp, #160]
	cmp r6, #112
	beq .L_0817a80a
	b .L_0817ac5a
.L_0817a80a:
	mov r0, r11
	str r0, [r7]
	b .L_0817ac5a
.L_0817a810:
	mov r1, r10
	cmp r1, #2
	bne .L_0817a8de
	ldr r2, [sp, #160]
	cmp r2, #49
	bgt .L_0817a82c
	movs r5, #192
	mov r4, r9
	mov r3, r11
	lsls r5, r5, #14
	movs r6, #232
	str r3, [r4, #104]
	str r5, [r4, r6]
	b .L_0817a8d0
.L_0817a82c:
	ldr r0, [sp, #160]
	cmp r0, #50
	bne .L_0817a878
	movs r3, #0
	mov r1, r9
	str r3, [r1, #104]
	movs r3, #248
	lsls r3, r3, #15
	movs r2, #232
	str r3, [r1, r2]
	b .L_0817a8d0
	.2byte 0x0000
.L_0817a844:
	.4byte IwramCopyWords
.L_0817a848:
	.4byte 0x05000200
.L_0817a84c:
	.4byte Data_02014260
.L_0817a850:
	.4byte 0x3f3f3f3f
.L_0817a854:
	.4byte IwramFillWords
.L_0817a858:
	.4byte Data_08196ec8
.L_0817a85c:
	.4byte 0xffff6800
.L_0817a860:
	.4byte 0xfffee000
.L_0817a864:
	.4byte Data_03001120
.L_0817a868:
	.4byte 0xfffa0000
.L_0817a86c:
	.4byte 0xfffd0000
.L_0817a870:
	.4byte 0xffff0000
.L_0817a874:
	.4byte 0x0001fde8
.L_0817a878:
	ldr r3, [sp, #160]
	cmp r3, #62
	bgt .L_0817a894
	mov r4, r9
	movs r3, #104
	ldr r2, [r4, r3]
	movs r5, #128
	lsls r5, r5, #11
	adds r2, r2, r5
	str r2, [r4, r3]
	movs r6, #232
	ldr r3, [r4, r6]
	ldr r0, .L_0817abdc
	b .L_0817a8cc
.L_0817a894:
	ldr r1, [sp, #160]
	cmp r1, #93
	bgt .L_0817a8b2
	mov r4, r9
	movs r3, #104
	ldr r2, [r4, r3]
	movs r5, #192
	lsls r5, r5, #10
	adds r2, r2, r5
	str r2, [r4, r3]
	movs r6, #232
	ldr r3, [r4, r6]
	movs r0, #192
	lsls r0, r0, #8
	b .L_0817a8cc
.L_0817a8b2:
	ldr r1, [sp, #160]
	cmp r1, #125
	bgt .L_0817a8d0
	mov r4, r9
	movs r3, #104
	ldr r2, [r4, r3]
	movs r5, #192
	lsls r5, r5, #10
	adds r2, r2, r5
	str r2, [r4, r3]
	movs r6, #232
	ldr r3, [r4, r6]
	ldr r0, .L_0817abe0
.L_0817a8cc:
	adds r3, r3, r0
	str r3, [r4, r6]
.L_0817a8d0:
	ldr r1, [sp, #160]
	cmp r1, #112
	beq .L_0817a8d8
	b .L_0817ac5a
.L_0817a8d8:
	mov r2, r11
	str r2, [r7]
	b .L_0817ac5a
.L_0817a8de:
	mov r3, r10
	cmp r3, #3
	bne .L_0817a94c
	ldr r4, [sp, #160]
	cmp r4, #49
	bgt .L_0817a8fa
	movs r0, #192
	mov r6, r9
	mov r5, r11
	movs r3, #236
	lsls r0, r0, #14
	str r5, [r6, #108]
	str r0, [r6, r3]
	b .L_0817a9b8
.L_0817a8fa:
	ldr r1, [sp, #160]
	cmp r1, #50
	bne .L_0817a910
	movs r3, #0
	mov r2, r9
	str r3, [r2, #108]
	movs r3, #216
	movs r2, #236
	lsls r3, r3, #15
	mov r4, r9
	b .L_0817a9a0
.L_0817a910:
	ldr r5, [sp, #160]
	cmp r5, #103
	bgt .L_0817a930
	mov r6, r9
	movs r2, #108
	ldr r3, [r6, r2]
	movs r0, #192
	lsls r0, r0, #10
	adds r3, r3, r0
	str r3, [r6, r2]
	movs r2, #236
	ldr r3, [r6, r2]
	ldr r1, .L_0817abdc
	adds r3, r3, r1
	str r3, [r6, r2]
	b .L_0817a9b8
.L_0817a930:
	ldr r2, [sp, #160]
	cmp r2, #135
	bgt .L_0817a9b8
	mov r4, r9
	movs r2, #108
	ldr r3, [r4, r2]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r4, r2]
	movs r2, #236
	ldr r3, [r4, r2]
	adds r3, r3, r1
	b .L_0817a9a0
.L_0817a94c:
	mov r0, r10
	cmp r0, #4
	bne .L_0817a9c6
	ldr r1, [sp, #160]
	cmp r1, #59
	bgt .L_0817a96a
	mov r3, r9
	mov r2, r11
	movs r5, #192
	str r2, [r3, #112]
	lsls r5, r5, #14
	movs r3, #240
	mov r4, r9
	str r5, [r4, r3]
	b .L_0817a9b8
.L_0817a96a:
	ldr r6, [sp, #160]
	cmp r6, #60
	bne .L_0817a982
	movs r3, #128
	lsls r3, r3, #13
	mov r0, r9
	str r3, [r0, #112]
	movs r3, #128
	movs r2, #240
	lsls r3, r3, #14
	str r3, [r0, r2]
	b .L_0817a9b8
.L_0817a982:
	ldr r1, [sp, #160]
	cmp r1, #82
	bgt .L_0817a9a4
	mov r4, r9
	movs r2, #112
	ldr r3, [r4, r2]
	movs r5, #128
	lsls r5, r5, #11
	adds r3, r3, r5
	str r3, [r4, r2]
	movs r2, #240
	ldr r3, [r4, r2]
	movs r6, #192
	lsls r6, r6, #10
	adds r3, r3, r6
.L_0817a9a0:
	str r3, [r4, r2]
	b .L_0817a9b8
.L_0817a9a4:
	ldr r0, [sp, #160]
	cmp r0, #135
	bgt .L_0817a9b8
	movs r2, #112
	mov r1, r9
	ldr r3, [r1, r2]
	movs r4, #128
	lsls r4, r4, #11
	adds r3, r3, r4
	str r3, [r1, r2]
.L_0817a9b8:
	ldr r5, [sp, #160]
	cmp r5, #112
	beq .L_0817a9c0
	b .L_0817ac5a
.L_0817a9c0:
	mov r6, r11
	str r6, [r7]
	b .L_0817ac5a
.L_0817a9c6:
	mov r0, r10
	cmp r0, #5
	bne .L_0817aa2a
	ldr r1, [sp, #160]
	cmp r1, #85
	bgt .L_0817a9e4
	mov r3, r9
	mov r2, r11
	movs r5, #192
	str r2, [r3, #116]
	lsls r5, r5, #14
	movs r3, #244
	mov r4, r9
	str r5, [r4, r3]
	b .L_0817aa1c
.L_0817a9e4:
	ldr r6, [sp, #160]
	cmp r6, #86
	bne .L_0817a9fc
	movs r3, #144
	lsls r3, r3, #16
	mov r0, r9
	str r3, [r0, #116]
	movs r3, #128
	movs r2, #244
	lsls r3, r3, #14
	str r3, [r0, r2]
	b .L_0817aa1c
.L_0817a9fc:
	ldr r1, [sp, #160]
	cmp r1, #135
	bgt .L_0817aa1c
	mov r4, r9
	movs r2, #116
	ldr r3, [r4, r2]
	movs r5, #128
	lsls r5, r5, #9
	adds r3, r3, r5
	str r3, [r4, r2]
	movs r2, #244
	ldr r3, [r4, r2]
	movs r6, #192
	lsls r6, r6, #10
	adds r3, r3, r6
	str r3, [r4, r2]
.L_0817aa1c:
	ldr r0, [sp, #160]
	cmp r0, #112
	beq .L_0817aa24
	b .L_0817ac5a
.L_0817aa24:
	mov r1, r11
	str r1, [r7]
	b .L_0817ac5a
.L_0817aa2a:
	mov r2, r10
	cmp r2, #15
	bgt .L_0817aafc
	ldr r3, [sp, #160]
	cmp r3, #0
	bne .L_0817aa44
	movs r3, #248
	lsls r3, r3, #16
	str r3, [r7]
	ldr r4, [sp, #160]
	mov r5, r8
	str r4, [r5]
	b .L_0817ac5a
.L_0817aa44:
	ldr r6, [sp, #160]
	ldr r0, [sp, #16]
	cmp r6, r0
	bne .L_0817aa98
	bl Random16
	movs r1, #7
	ands r0, r1
	movs r2, #184
	lsls r2, r2, #15
	lsls r0, r0, #16
	adds r0, r0, r2
	str r0, [r7]
	bl Random16
	movs r3, #7
	ands r0, r3
	movs r4, #208
	lsls r4, r4, #15
	lsls r0, r0, #16
	adds r0, r0, r4
	mov r5, r8
	str r0, [r5]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r0, r3
	lsls r3, r0, #1
	ldr r6, .L_0817abe4
	adds r3, r3, r0
	ldr r0, [sp, #28]
	lsls r3, r3, #2
	adds r3, r3, r6
	mov r1, r9
	str r3, [r0, r1]
	ldr r2, [sp, #36]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r2, r1]
	b .L_0817ac5a
.L_0817aa98:
	ldr r3, [sp, #160]
	ldr r4, [sp, #16]
	cmp r3, r4
	bge .L_0817aaa2
	b .L_0817ac5a
.L_0817aaa2:
	ldr r2, [r7]
	ldr r5, .L_0817abe8
	cmp r2, r5
	blt .L_0817aacc
	mov r6, r8
	ldr r3, [r6]
	movs r0, #168
	lsls r0, r0, #16
	cmp r3, r0
	bgt .L_0817aacc
	ldr r1, [sp, #28]
	mov r4, r9
	ldr r3, [r1, r4]
	adds r3, r2, r3
	str r3, [r7]
	ldr r5, [sp, #36]
	ldr r2, [r6]
	ldr r3, [r5, r4]
	adds r2, r2, r3
	str r2, [r6]
	b .L_0817ac5a
.L_0817aacc:
	ldr r6, [sp, #160]
	cmp r6, #251
	ble .L_0817aad4
	b .L_0817ac5a
.L_0817aad4:
	bl Random16
	movs r1, #7
	ands r0, r1
	movs r2, #184
	lsls r2, r2, #15
	lsls r0, r0, #16
	adds r0, r0, r2
	str r0, [r7]
	bl Random16
	movs r3, #7
	ands r0, r3
	movs r4, #208
	lsls r0, r0, #16
	lsls r4, r4, #15
	adds r0, r0, r4
	mov r5, r8
	str r0, [r5]
	b .L_0817ac5a
.L_0817aafc:
	mov r6, r10
	cmp r6, #31
	ble .L_0817ab04
	b .L_0817ac5a
.L_0817ab04:
	ldr r0, [sp, #160]
	cmp r0, #0
	bne .L_0817ab16
	movs r3, #248
	lsls r3, r3, #16
	mov r1, r8
	str r3, [r7]
	str r0, [r1]
	b .L_0817ac5a
.L_0817ab16:
	ldr r2, [sp, #160]
	cmp r2, #112
	bne .L_0817ab58
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	bl Random16
	movs r3, #255
	ands r3, r0
	movs r4, #128
	lsls r3, r3, #1
	lsls r4, r4, #1
	adds r6, r3, r4
	bl Random16
	movs r1, #7
	ands r0, r1
	movs r2, #168
	lsls r2, r2, #16
	lsls r0, r0, #16
	adds r0, r0, r2
	str r0, [r7]
	bl Random16
	movs r3, #7
	ands r0, r3
	movs r4, #184
	b .L_0817ab98
.L_0817ab58:
	ldr r5, [sp, #160]
	cmp r5, #252
	bne .L_0817abec
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	bl Random16
	movs r3, #255
	ands r3, r0
	movs r0, #128
	lsls r3, r3, #1
	lsls r0, r0, #1
	adds r6, r3, r0
	bl Random16
	movs r1, #7
	ands r0, r1
	movs r2, #184
	lsls r2, r2, #15
	lsls r0, r0, #16
	adds r0, r0, r2
	str r0, [r7]
	bl Random16
	movs r3, #7
	ands r0, r3
	movs r4, #208
.L_0817ab98:
	lsls r4, r4, #15
	lsls r0, r0, #16
	adds r0, r0, r4
	mov r1, r8
	str r0, [r1]
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	cmp r3, #0
	bge .L_0817abb2
	adds r3, #255
.L_0817abb2:
	ldr r4, [sp, #28]
	asrs r3, r3, #8
	mov r2, r9
	str r3, [r2, r4]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r5, [sp, #20]
	adds r3, r6, #0
	muls r3, r0
	movs r1, #240
	lsls r1, r1, #1
	adds r2, r5, r1
	cmp r3, #0
	bge .L_0817abd2
	adds r3, #255
.L_0817abd2:
	asrs r3, r3, #8
	mov r4, r9
	str r3, [r4, r2]
	b .L_0817ac5a
	.2byte 0x0000
.L_0817abdc:
	.4byte 0xfffec000
.L_0817abe0:
	.4byte 0xffff0000
.L_0817abe4:
	.4byte 0xfff40000
.L_0817abe8:
	.4byte 0xfff00000
.L_0817abec:
	ldr r5, [sp, #160]
	cmp r5, #111
	ble .L_0817ac5a
	ldr r6, [sp, #28]
	mov r0, r9
	ldr r3, [r6, r0]
	ldr r2, [r7]
	mov r1, r8
	adds r2, r2, r3
	str r2, [r7]
	ldr r4, [sp, #36]
	ldr r2, [r1]
	ldr r3, [r4, r0]
	adds r2, r2, r3
	str r2, [r1]
	ldr r2, [r6, r0]
	lsls r3, r2, #6
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0817ac16
	adds r3, #63
.L_0817ac16:
	ldr r6, [sp, #28]
	asrs r3, r3, #6
	mov r5, r9
	str r3, [r5, r6]
	ldr r0, [sp, #36]
	ldr r2, [r5, r0]
	lsls r3, r2, #6
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0817ac2c
	adds r3, #63
.L_0817ac2c:
	asrs r2, r3, #6
	ldr r3, [sp, #36]
	mov r1, r9
	str r2, [r1, r3]
	ldr r4, [sp, #28]
	movs r5, #255
	ldr r3, [r1, r4]
	lsls r5, r5, #8
	ldr r1, .L_0817ac78
	adds r5, #255
	adds r3, r3, r5
	cmp r3, r1
	bhi .L_0817ac5a
	adds r3, r2, r5
	cmp r3, r1
	bhi .L_0817ac5a
	ldr r1, [sp, #24]
	mov r6, r9
	mov r0, r11
	str r0, [r6, r1]
	ldr r2, [sp, #32]
	movs r3, #0
	str r3, [r6, r2]
.L_0817ac5a:
	mov r3, r10
	cmp r3, #5
	bgt .L_0817ac80
	ldr r5, [sp, #40]
	ldr r4, .L_0817ac7c
	movs r1, #240
	ldrh r0, [r4, r5]
	lsls r1, r1, #4
	adds r0, #32
	bl Math_Mod
	ldr r6, .L_0817ac7c
	adds r1, r5, #0
	strh r0, [r6, r1]
	b .L_0817ac96
.L_0817ac78:
	.4byte 0x0001fffe
.L_0817ac7c:
	.4byte Data_02014002
.L_0817ac80:
	ldr r3, [sp, #40]
	ldr r2, .L_0817acf4
	movs r1, #240
	ldrh r0, [r2, r3]
	lsls r1, r1, #4
	adds r0, #128
	bl Math_Mod
	ldr r4, .L_0817acf4
	ldr r5, [sp, #40]
	strh r0, [r4, r5]
.L_0817ac96:
	ldr r6, [sp, #20]
	movs r0, #240
	ldr r4, [sp, #40]
	ldr r1, [sp, #172]
	ldr r2, .L_0817acf4
	lsls r0, r0, #7
	adds r0, #12
	adds r3, r6, r0
	ldr r0, [r1, r3]
	ldrh r3, [r2, r4]
	mov r5, r9
	lsrs r3, r3, #8
	lsls r3, r3, #1
	ldrh r1, [r5, r3]
	ldr r3, .L_0817acec
	ldr r2, .L_0817acf0
	ands r1, r3
	ldrh r3, [r0, #8]
	adds r4, #2
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	ldr r6, [sp, #36]
	ldr r1, [sp, #32]
	adds r6, #4
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	ldr r5, [sp, #16]
	str r4, [sp, #40]
	ldr r4, [sp, #20]
	str r6, [sp, #36]
	movs r6, #1
	movs r0, #4
	add r10, r6
	add r8, r0
	adds r1, #4
	adds r2, #4
	adds r3, #4
	adds r4, #4
	adds r5, #2
	mov r0, r10
	str r1, [sp, #32]
	b .L_0817acf8
.L_0817acec:
	.4byte 0x000003ff
.L_0817acf0:
	.4byte 0xfffffc00
.L_0817acf4:
	.4byte Data_02014002
.L_0817acf8:
	str r2, [sp, #28]
	adds r7, #4
	str r3, [sp, #24]
	str r4, [sp, #20]
	str r5, [sp, #16]
	cmp r0, #32
	beq .L_0817ad08
	b .L_0817a6fc
.L_0817ad08:
	ldr r3, [sp, #172]
	movs r6, #240
	ldr r2, .L_0817b000
	lsls r6, r6, #7
	adds r6, #12
	ldr r5, [sp, #48]
	movs r1, #0
	adds r4, r3, r6
	mov r7, r9
	mov r6, r9
	mov r10, r1
	mov r8, r2
	adds r7, #224
	adds r6, #96
.L_0817ad24:
	ldmia r6!, {r3}
	ldmia r4!, {r0}
	add r3, r8
	str r3, [r5]
	ldmia r7!, {r3}
	adds r1, r5, #0
	add r3, r8
	str r3, [r5, #8]
	ldr r2, [sp, #52]
	movs r3, #0
	str r4, [sp, #12]
	bl Render_ApplyProjectedPlacementFar
	movs r0, #1
	add r10, r0
	mov r1, r10
	ldr r4, [sp, #12]
	cmp r1, #32
	bne .L_0817ad24
	ldr r2, [sp, #160]
	cmp r2, #114
	bne .L_0817ad56
	movs r0, #212
	bl Audio_PlayCue
.L_0817ad56:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0817b004
	ldr r3, [sp, #184]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_0817b008
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #184]
	ldr r3, [sp, #172]
	ldr r4, [sp, #56]
	adds r2, r3, r2
	ldr r3, .L_0817b00c
	adds r6, r0, #0
	mov r5, r8
	str r1, [r6]
	str r2, [r4, #4]
	movs r1, #128
	str r4, [r6, #16]
	str r3, [r6, #8]
	str r5, [r6, #12]
	movs r0, #0
	lsls r1, r1, #8
	mov r10, r0
	mov r11, r1
.L_0817ad9c:
	ldr r3, .L_0817b010
	mov r2, r10
	lsls r5, r2, #1
	ldrh r3, [r3, r5]
	ldr r4, [sp, #160]
	subs r1, r4, r3
	cmp r1, #0
	blt .L_0817ae1a
	lsls r7, r1, #13
	movs r3, #0
	cmp r1, #7
	ble .L_0817adbe
	movs r2, #8
	subs r2, r2, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #1
.L_0817adbe:
	movs r0, #64
	negs r0, r0
	str r3, [r6, #20]
	cmp r3, r0
	ble .L_0817ae1a
	bl Func_08014de4
	ldr r3, .L_0817b014
	mov r1, r10
	ldrsb r0, [r3, r1]
	ldr r3, .L_0817b018
	lsls r0, r0, #16
	ldrsb r1, [r3, r1]
	movs r2, #0
	lsls r1, r1, #16
	bl Func_08015160
	movs r1, #128
	mov r2, r11
	lsls r1, r1, #9
	mov r0, r11
	bl Func_080151e4
	ldr r3, .L_0817b01c
	ldrsh r0, [r3, r5]
	bl Func_080150e4
	ldr r3, .L_0817b020
	ldrsh r0, [r3, r5]
	bl SceneTransform_ApplyPitch
	adds r0, r7, #0
	bl Func_0801521c
	ldr r5, [sp, #160]
	lsls r0, r5, #11
	bl Func_08015068
	ldr r0, .L_0817b024
	mov r1, r8
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0817ae1a:
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #4
	bne .L_0817ad9c
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	bl Func_08014de4
	movs r0, #148
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	movs r0, #208
	lsls r0, r0, #7
	bl Func_08015068
	ldr r2, [sp, #160]
	cmp r2, #156
	bne .L_0817ae54
	movs r3, #128
	lsls r3, r3, #2
	movs r4, #0
	str r3, [sp, #128]
	str r4, [sp, #132]
.L_0817ae54:
	ldr r0, [sp, #132]
	bl Func_080150e4
	ldr r5, [sp, #160]
	cmp r5, #156
	bne .L_0817ae66
	movs r0, #142
	bl Audio_PlayCue
.L_0817ae66:
	ldr r6, [sp, #160]
	cmp r6, #223
	bne .L_0817ae72
	movs r0, #104
	bl Audio_PlayCue
.L_0817ae72:
	ldr r0, [sp, #160]
	cmp r0, #252
	bne .L_0817ae7e
	movs r0, #212
	bl Audio_PlayCue
.L_0817ae7e:
	ldr r2, [sp, #160]
	subs r2, #156
	cmp r2, #74
	bls .L_0817ae88
	b .L_0817afe2
.L_0817ae88:
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r1, [sp, #160]
	asrs r3, r3, #1
	movs r2, #80
	subs r2, r2, r3
	str r2, [sp, #116]
	cmp r1, #156
	bne .L_0817aea0
	movs r2, #0
	str r2, [sp, #124]
	str r2, [sp, #120]
.L_0817aea0:
	ldr r3, [sp, #160]
	cmp r3, #194
	bgt .L_0817aeb2
	ldr r4, [sp, #120]
	ldr r5, [sp, #124]
	adds r4, #32
	adds r5, #3
	str r4, [sp, #120]
	str r5, [sp, #124]
.L_0817aeb2:
	ldr r6, [sp, #128]
	ldr r0, [sp, #132]
	adds r6, #16
	adds r0, r0, r6
	str r0, [sp, #132]
	movs r1, #3
	movs r0, #188
	str r6, [sp, #128]
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	mov r2, sp
	str r3, [sp, #168]
	adds r2, #252
	movs r3, #132
	lsls r3, r3, #1
	str r2, [sp, #104]
	movs r1, #0
	add r3, sp
	mov r10, r1
	mov r11, r3
.L_0817aee2:
	mov r3, r10
	cmp r3, #0
	bge .L_0817aeea
	adds r3, #3
.L_0817aeea:
	ldr r0, [sp, #120]
	asrs r3, r3, #2
	lsls r4, r3, #13
	mov r5, r10
	lsls r3, r3, #2
	subs r3, r5, r3
	ldr r1, [sp, #124]
	adds r6, r0, #0
	muls r6, r3
	adds r0, r1, #0
	muls r0, r3
	movs r1, #30
	str r4, [sp, #112]
	str r6, [sp, #108]
	bl Math_Div
	ldr r2, [sp, #116]
	ldr r5, [sp, #160]
	adds r7, r2, r0
	ldr r0, .L_0817b028
	movs r3, #7
	movs r4, #3
	mov r8, r3
	mov r12, r3
	lsls r3, r5, #2
	mov lr, r4
	adds r1, r5, #0
	movs r6, #0
	adds r4, r3, r0
	movs r5, #208
.L_0817af26:
	ldr r2, [sp, #160]
	cmp r2, r5
	blt .L_0817af60
	mov r3, r10
	cmp r3, #0
	bge .L_0817af34
	adds r3, #3
.L_0817af34:
	asrs r3, r3, #2
	mov r0, r10
	lsls r3, r3, #2
	subs r3, r0, r3
	mov r0, lr
	subs r2, r0, r6
	cmp r3, r2
	bne .L_0817af60
	ldr r2, [sp, #116]
	adds r0, r1, #0
	subs r7, r2, r4
	subs r0, #208
	cmp r7, #0
	bge .L_0817af52
	movs r7, #0
.L_0817af52:
	mov r3, r12
	subs r3, r3, r0
	mov r8, r3
	cmp r3, #2
	bgt .L_0817af60
	movs r0, #3
	mov r8, r0
.L_0817af60:
	adds r6, #1
	subs r4, #16
	subs r1, #4
	adds r5, #4
	cmp r6, #4
	bne .L_0817af26
	ldr r1, [sp, #112]
	ldr r2, [sp, #108]
	movs r6, #0
	adds r5, r1, r2
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	ldr r4, [sp, #104]
	asrs r3, r3, #16
	str r3, [r4]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r5, [sp, #104]
	asrs r3, r3, #16
	str r3, [r5, #4]
	str r6, [r5, #8]
	ldr r0, [sp, #104]
	mov r1, r11
	bl Func_0815e1ec
	mov r6, r11
	ldr r2, [r6]
	ldr r0, .L_0817b02c
	mov r1, r8
	lsls r4, r1, #1
	asrs r2, r2, #1
	str r2, [r6]
	subs r1, r4, #2
	ldrh r1, [r0, r1]
	ldr r5, [sp, #148]
	ldr r3, [r6, #4]
	mov r6, r8
	lsrs r0, r6, #31
	adds r1, r5, r1
	add r0, r8
	movs r5, #1
	asrs r0, r0, #1
	adds r2, #48
	adds r3, #92
	add r10, r5
	subs r2, r2, r0
	subs r3, r3, r6
	str r6, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #176]
	ldr r4, [sp, #168]
	mov r6, r10
	mov lr, r4
	.2byte 0xf800
	cmp r6, #32
	bne .L_0817aee2
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_0817afe2:
	ldr r3, [sp, #160]
	subs r3, #186
	cmp r3, #79
	bls .L_0817afec
	b .L_0817b26c
.L_0817afec:
	ldr r0, [sp, #160]
	str r3, [sp, #100]
	cmp r0, #247
	ble .L_0817b030
	movs r3, #133
	lsls r2, r0, #1
	lsls r3, r3, #2
	subs r3, r3, r2
	str r3, [sp, #100]
	b .L_0817b03a
.L_0817b000:
	.4byte 0xfff80000
.L_0817b004:
	.4byte 0xffffff00
.L_0817b008:
	.4byte 0xffff00ff
.L_0817b00c:
	.4byte Data_08199364
.L_0817b010:
	.4byte Data_0819942e
.L_0817b014:
	.4byte Data_08199446
.L_0817b018:
	.4byte Data_0819944a
.L_0817b01c:
	.4byte Data_08199436
.L_0817b020:
	.4byte Data_0819943e
.L_0817b024:
	.4byte Data_08199210
.L_0817b028:
	.4byte 0xfffffcc0
.L_0817b02c:
	.4byte Data_08197410
.L_0817b030:
	ldr r1, [sp, #100]
	cmp r1, #36
	ble .L_0817b03a
	movs r2, #36
	str r2, [sp, #100]
.L_0817b03a:
	ldr r4, [sp, #100]
	mov r0, sp
	movs r3, #64
	adds r7, r4, #0
	adds r0, #248
	subs r3, r3, r4
	str r3, [sp, #96]
	str r0, [sp, #80]
	strb r7, [r0]
	strb r7, [r0, #1]
	ldr r1, [sp, #160]
	lsrs r3, r7, #31
	adds r3, r7, r3
	movs r2, #18
	asrs r3, r3, #1
	subs r6, r2, r3
	lsls r0, r1, #10
	movs r3, #36
	subs r5, r3, r7
	bl Func_080150e4
	ldr r2, [sp, #160]
	cmp r2, #221
	bgt .L_0817b070
	lsls r0, r5, #10
	bl SceneTransform_ApplyPitch
.L_0817b070:
	cmp r6, #0
	bge .L_0817b076
	movs r6, #0
.L_0817b076:
	cmp r5, #0
	bge .L_0817b07c
	movs r5, #0
.L_0817b07c:
	adds r6, #48
	str r6, [sp, #92]
	movs r3, #92
	ldr r6, [sp, #164]
	subs r3, r3, r5
	str r3, [sp, #88]
	movs r3, #88
	subs r3, r3, r5
	movs r4, #224
	movs r5, #236
	str r6, [sp, #76]
	str r3, [sp, #84]
	add r4, sp
	movs r3, #0
	add r5, sp
	mov r10, r3
	mov r11, r4
	mov r8, r5
.L_0817b0a0:
	mov r0, r10
	lsls r5, r0, #9
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, [sp, #96]
	mov r2, r11
	adds r3, r1, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [r2]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r4, [sp, #96]
	mov r5, r11
	adds r3, r4, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r1, r8
	mov r0, r11
	bl Func_0815e1ec
	mov r6, r8
	ldr r2, [r6]
	asrs r2, r2, #1
	str r2, [r6]
	ldr r0, [sp, #92]
	ldr r3, [sp, #88]
	adds r0, r2, r0
	str r0, [sp, #8]
	ldr r1, [r6, #4]
	adds r7, r1, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	cmp r3, #0
	bge .L_0817b0f2
	adds r3, #3
.L_0817b0f2:
	ldr r4, [sp, #92]
	asrs r3, r3, #2
	adds r6, r3, r4
	lsls r3, r1, #1
	adds r3, r3, r1
	cmp r3, #0
	bge .L_0817b102
	adds r3, #3
.L_0817b102:
	ldr r0, [sp, #84]
	ldr r2, [sp, #8]
	asrs r3, r3, #2
	adds r5, r3, r0
	subs r1, r7, #1
	movs r3, #1
	mov r12, r1
	str r3, [sp, #0]
	movs r4, #2
	add r3, sp, #248
	str r4, [sp, #4]
	adds r1, r3, #0
	ldr r4, [sp, #76]
	mov r3, r12
	subs r2, #1
	ldr r0, [sp, #176]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r4, sp, #248
	movs r1, #2
	subs r3, r5, #1
	str r0, [sp, #0]
	str r1, [sp, #4]
	subs r2, r6, #1
	adds r1, r4, #0
	ldr r0, [sp, #176]
	ldr r4, [sp, #76]
	mov lr, r4
	.2byte 0xf800
	movs r3, #31
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	bne .L_0817b168
	ldr r1, [sp, #100]
	ldr r0, [sp, #8]
	str r1, [sp, #0]
	adds r2, r6, #0
	adds r1, r7, #0
	adds r3, r5, #0
	bl Func_08143eb4
	ldr r2, [sp, #100]
	adds r1, r7, #1
	str r2, [sp, #0]
	adds r3, r5, #1
	ldr r0, [sp, #8]
	adds r2, r6, #0
	bl Func_08143eb4
.L_0817b168:
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #128
	bne .L_0817b0a0
	ldr r3, [sp, #160]
	subs r3, #228
	cmp r3, #24
	bhi .L_0817b26c
	ldr r5, [sp, #160]
	movs r3, #128
	lsls r3, r3, #1
	subs r7, r3, r5
	ldr r6, [sp, #80]
	ldr r3, [sp, #164]
	mov r1, sp
	subs r5, #204
	adds r1, #200
	str r5, [sp, #96]
	movs r2, #212
	strb r7, [r6]
	strb r7, [r6, #1]
	str r1, [sp, #72]
	str r6, [sp, #68]
	str r3, [sp, #64]
	movs r0, #0
	add r2, sp
	mov r10, r0
	mov r11, r2
.L_0817b1a2:
	mov r4, r10
	lsls r5, r4, #9
	adds r0, r5, #0
	bl Trig_Sin
	ldr r6, [sp, #96]
	adds r3, r6, #0
	muls r3, r0
	ldr r0, [sp, #72]
	asrs r3, r3, #16
	str r3, [r0]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	ldr r1, [sp, #72]
	asrs r3, r3, #16
	str r3, [r1, #4]
	movs r3, #0
	str r3, [r1, #8]
	ldr r0, [sp, #72]
	mov r1, r11
	bl Func_0815e1ec
	mov r3, r11
	ldr r2, [r3]
	ldr r1, [r3, #4]
	asrs r2, r2, #1
	str r2, [r3]
	ldr r4, [sp, #92]
	ldr r5, [sp, #88]
	lsls r3, r2, #1
	adds r4, r4, r2
	adds r5, r1, r5
	adds r3, r3, r2
	mov r8, r4
	str r5, [sp, #8]
	cmp r3, #0
	bge .L_0817b1f4
	adds r3, #3
.L_0817b1f4:
	ldr r0, [sp, #92]
	asrs r3, r3, #2
	adds r6, r3, r0
	lsls r3, r1, #1
	adds r3, r3, r1
	cmp r3, #0
	bge .L_0817b204
	adds r3, #3
.L_0817b204:
	ldr r1, [sp, #84]
	asrs r3, r3, #2
	adds r5, r3, r1
	ldr r3, [sp, #8]
	movs r4, #1
	movs r0, #2
	mov r2, r8
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r1, [sp, #68]
	ldr r4, [sp, #64]
	subs r2, #1
	subs r3, #1
	ldr r0, [sp, #176]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	movs r1, #2
	subs r3, r5, #1
	str r0, [sp, #0]
	str r1, [sp, #4]
	subs r2, r6, #1
	ldr r0, [sp, #176]
	ldr r1, [sp, #68]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	movs r3, #31
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	bne .L_0817b262
	mov r0, r8
	ldr r1, [sp, #8]
	adds r2, r6, #0
	adds r3, r5, #0
	str r7, [sp, #0]
	bl Func_08143eb4
	ldr r1, [sp, #8]
	adds r3, r5, #1
	adds r1, #1
	mov r0, r8
	adds r2, r6, #0
	str r7, [sp, #0]
	bl Func_08143eb4
.L_0817b262:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #128
	bne .L_0817b1a2
.L_0817b26c:
	ldr r3, [sp, #172]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #160]
	movs r6, #150
	adds r5, #1
	lsls r6, r6, #1
	str r5, [sp, #160]
	cmp r5, r6
	beq .L_0817b2a6
	ldr r3, .L_0817b378
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_0817b29e
	bl .L_0817a332
.L_0817b29e:
	cmp r5, #16
	bgt .L_0817b2a6
	bl .L_0817a332
.L_0817b2a6:
	ldr r0, .L_0817b37c
	bl Scheduler_RemoveCallback
	add r0, sp, #156
	ldr r3, .L_0817b380
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r3, #4]
	ldr r1, [sp, #152]
	str r2, [r1, #16]
	bl Func_0814cca8
	movs r4, #238
	ldr r3, [sp, #172]
	lsls r4, r4, #7
	movs r2, #0
	adds r4, #220
	mov r10, r2
	adds r5, r3, r4
.L_0817b2cc:
	movs r6, #1
	ldmia r5!, {r0}
	add r10, r6
	bl ResourceObject_ReleaseFar
	mov r0, r10
	cmp r0, #44
	bne .L_0817b2cc
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0817b384
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_0817b388
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	ldr r1, [sp, #172]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2]
	ldr r4, [sp, #172]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #132
	adds r1, r4, r5
	movs r3, #50
	str r3, [r1]
	movs r3, #1
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	movs r2, #0
	adds r3, #40
	str r2, [r1]
	str r2, [r3]
	adds r3, #40
	strh r2, [r3]
	ldr r3, .L_0817b36c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_0817b370
	adds r2, #70
	strh r3, [r2]
	ldr r3, .L_0817b374
	subs r2, #50
	strh r3, [r2]
	ldr r0, .L_0817b38c
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	ldr r3, .L_0817b390
	adds r1, r6, #0
	movs r2, #32
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #172]
	movs r2, #224
	adds r6, #32
	lsls r2, r2, #3
	adds r1, r0, r2
	adds r0, r6, #0
	b .L_0817b394
.L_0817b36c:
	.4byte 0x00000784
.L_0817b370:
	.4byte 0x00001010
.L_0817b374:
	.4byte 0x00000080
.L_0817b378:
	.4byte gInput
.L_0817b37c:
	.4byte Func_0813baec
.L_0817b380:
	.4byte Data_03001120
.L_0817b384:
	.4byte 0x05000200
.L_0817b388:
	.4byte 0x050001e8
.L_0817b38c:
	.4byte 0x0000009a
.L_0817b390:
	.4byte IwramCopyWords
.L_0817b394:
	bl Func_0801587c
	ldr r3, [sp, #172]
	movs r5, #0
	movs r4, #184
	lsls r4, r4, #5
	str r5, [sp, #60]
	str r5, [sp, #44]
	adds r3, r3, r4
	movs r6, #15
	mov r9, r3
	mov r10, r5
	mov r11, r6
.L_0817b3ae:
	ldr r1, [sp, #44]
	movs r0, #0
	mov lr, r0
	mov r8, r1
.L_0817b3b6:
	movs r2, #0
	mov r12, r2
.L_0817b3ba:
	mov r3, r8
	add r3, lr
	lsls r3, r3, #4
	add r3, r12
	movs r7, #0
	lsls r5, r3, #3
.L_0817b3c6:
	mov r3, r9
	adds r0, r5, r3
	ldr r3, [sp, #172]
	movs r1, #224
	add r3, r10
	lsls r1, r1, #3
	movs r6, #0
	adds r4, r3, r1
.L_0817b3d6:
	ldrb r1, [r4]
	movs r3, #15
	lsrs r2, r1, #4
	ands r2, r3
	mov r3, r11
	ands r3, r1
	adds r6, #1
	movs r1, #1
	strb r3, [r0]
	strb r2, [r0, #1]
	adds r4, #1
	add r10, r1
	adds r0, #2
	cmp r6, #4
	bne .L_0817b3d6
	adds r7, #1
	adds r5, #16
	cmp r7, #8
	bne .L_0817b3c6
	add r12, r1
	mov r2, r12
	cmp r2, #2
	bne .L_0817b3ba
	add lr, r1
	mov r3, lr
	cmp r3, #2
	bne .L_0817b3b6
	ldr r4, [sp, #44]
	ldr r5, [sp, #60]
	adds r4, #2
	adds r5, #1
	str r4, [sp, #44]
	str r5, [sp, #60]
	cmp r5, #15
	bne .L_0817b3ae
	movs r6, #0
	str r6, [sp, #160]
.L_0817b420:
	ldr r0, [sp, #160]
	cmp r0, #0
	bne .L_0817b4a2
	ldr r5, .L_0817b5c8
	movs r1, #0
	mov r10, r1
	movs r7, #63
	movs r6, #255
.L_0817b430:
	bl Random16
	ands r0, r7
	adds r0, #96
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r7
	negs r0, r0
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	adds r0, #128
	negs r0, r0
	lsls r0, r0, #7
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #128
	lsls r0, r0, #9
	str r0, [r5, #16]
	bl Random16
	movs r3, #31
	movs r2, #1
	ands r3, r0
	add r10, r2
	str r3, [r5, #24]
	mov r3, r10
	adds r5, #28
	cmp r3, #128
	bne .L_0817b430
	ldr r5, [sp, #180]
	movs r4, #0
	ldr r3, [r5, #20]
	mov r10, r4
	cmp r3, #0
	beq .L_0817b4a2
	ldr r5, [sp, #172]
	movs r6, #15
	adds r5, #24
.L_0817b48a:
	bl Random16
	ands r0, r6
	adds r0, #1
	str r0, [r5]
	ldr r1, [sp, #180]
	movs r0, #1
	ldr r3, [r1, #20]
	add r10, r0
	adds r5, #28
	cmp r10, r3
	bne .L_0817b48a
.L_0817b4a2:
	ldr r5, .L_0817b5c8
	movs r2, #0
	mov r10, r2
.L_0817b4a8:
	ldr r3, [sp, #160]
	cmp r3, r10
	ble .L_0817b4fc
	ldr r0, [r5, #24]
	cmp r0, #0
	bge .L_0817b4b6
	adds r0, #3
.L_0817b4b6:
	movs r1, #15
	asrs r0, r0, #2
	bl Math_Mod
	ldr r4, [sp, #172]
	adds r1, r0, #0
	movs r0, #2
	ldrsh r2, [r5, r0]
	lsls r1, r1, #8
	movs r6, #184
	adds r1, r4, r1
	movs r0, #16
	lsls r6, r6, #5
	movs r4, #6
	ldrsh r3, [r5, r4]
	adds r1, r1, r6
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #4
	ldr r0, [sp, #176]
	ldr r6, [sp, #164]
	mov lr, r6
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #12]
	ldr r0, .L_0817b5cc
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0817b4fc:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #28
	cmp r2, #64
	bne .L_0817b4a8
	ldr r3, [sp, #160]
	cmp r3, #56
	bne .L_0817b514
	movs r0, #134
	bl Func_081180e8
.L_0817b514:
	ldr r3, [sp, #160]
	subs r3, #57
	cmp r3, #50
	bhi .L_0817b57c
	ldr r5, [sp, #180]
	movs r4, #0
	ldr r3, [r5, #20]
	mov r10, r4
	cmp r3, #0
	beq .L_0817b57c
	ldr r5, [sp, #172]
	movs r6, #36
.L_0817b52c:
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
	cmp r3, #0
	bne .L_0817b56c
	movs r3, #15
	str r3, [r5, #24]
	ldr r0, [sp, #172]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #4
	str r3, [r2]
	movs r0, #133
	bl Audio_PlayCue
	ldr r2, [sp, #180]
	movs r1, #0
	ldrsh r0, [r6, r2]
	bl Func_08118088
	ldr r4, [sp, #180]
	movs r3, #8
	movs r2, #1
	ldrsh r0, [r6, r4]
	negs r2, r2
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r10
	bl Func_0814cd48
.L_0817b56c:
	ldr r4, [sp, #180]
	movs r2, #1
	ldr r3, [r4, #20]
	add r10, r2
	adds r6, #2
	adds r5, #28
	cmp r10, r3
	bne .L_0817b52c
.L_0817b57c:
	movs r0, #2
	movs r1, #2
	bl Func_08158ce0
	bl Func_081434f8
	movs r6, #240
	ldr r5, [sp, #172]
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r5, r6
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #160]
	adds r0, #1
	str r0, [sp, #160]
	cmp r0, #128
	beq .L_0817b5a8
	b .L_0817b420
.L_0817b5a8:
	ldr r0, .L_0817b5d0
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #292
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0817b5c8:
	.4byte gMapCellBuffer
.L_0817b5cc:
	.4byte 0xfffff000
.L_0817b5d0:
	.4byte Func_08143000
