.syntax unified
	.thumb
	.global Func_0816729c
	.thumb_func
Func_0816729c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #188
	str r0, [sp, #96]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	adds r3, r5, #0
	str r0, [sp, #92]
	adds r3, #176
	ldr r1, [r5, #92]
	movs r0, #128
	str r1, [sp, #88]
	lsls r0, r0, #6
	ldr r3, [r3]
	ldr r6, .L_08167314
	str r3, [sp, #84]
	ldr r2, [r5, #100]
	str r2, [sp, #68]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816730c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	bl Func_0813ba50
	ldr r2, .L_08167310
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [sp, #88]
	movs r4, #239
	lsls r4, r4, #7
	ldr r5, [r5, #104]
	adds r2, r3, r4
	movs r1, #200
	movs r3, #0
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08167318
	str r5, [sp, #76]
	b .L_0816731c
	.2byte 0x0000
.L_0816730c:
	.4byte 0x00000100
.L_08167310:
	.4byte 0x00000000
.L_08167314:
	.4byte gMapCellBuffer
.L_08167318:
	.4byte Func_08143000
.L_0816731c:
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #0
	bl Func_08163c2c
	movs r0, #240
	ldr r5, [sp, #88]
	lsls r0, r0, #7
	adds r0, #240
	adds r3, r5, r0
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r1, .L_08167424
	movs r2, #0
	movs r0, #1
	bl Func_08118040
	ldr r0, .L_08167428
	bl Resource_GetTableEntry
	movs r2, #128
	adds r7, r0, #0
	ldr r5, .L_0816742c
	adds r1, r7, #0
	lsls r2, r2, #2
	ldr r0, .L_08167430
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #2
	adds r7, r7, r1
	adds r0, r7, #0
	adds r1, r6, #0
	bl Resource_DecodeType01
	ldr r2, .L_08167434
	ldr r3, [sp, #88]
	movs r4, #238
	lsls r4, r4, #7
	movs r0, #13
	adds r4, #220
	negs r0, r0
	mov r10, r5
	movs r7, #0
	mov r9, r2
	adds r5, r3, r4
	mov r8, r0
.L_0816737e:
	movs r1, #32
	ldr r2, .L_08167438
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	mov r1, r8
	ands r3, r1
	movs r2, #8
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r9
	ldrh r0, [r3, #2]
	ldr r2, .L_0816743c
	adds r1, r6, #0
	adds r0, r0, r2
	movs r2, #128
	lsls r2, r2, #3
	mov lr, r10
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #3
	adds r7, #1
	adds r6, r6, r3
	cmp r7, #16
	bne .L_0816737e
	ldr r2, .L_08167440
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #84]
	ldr r2, .L_08167444
	movs r3, #1
	str r3, [r4, #16]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #0
	movs r1, #1
	bl Func_08163c2c
	ldr r3, .L_08167414
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08167418
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0816741c
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08167420
	subs r2, #2
	strh r3, [r2]
	ldr r2, [sp, #88]
	ldr r5, .L_08167448
	movs r3, #224
	lsls r3, r3, #3
	movs r6, #128
	adds r1, r2, r3
	lsls r6, r6, #12
	ldr r0, .L_0816744c
	movs r2, #1
	movs r3, #1
	str r5, [sp, #64]
	str r6, [sp, #60]
	bl Resource_LoadAndDecompress
	b .L_08167450
	.2byte 0x0000
.L_08167414:
	.4byte 0x00007741
.L_08167418:
	.4byte 0x00000080
.L_0816741c:
	.4byte 0x00001010
.L_08167420:
	.4byte 0x00003f44
.L_08167424:
	.4byte 0x00000045
.L_08167428:
	.4byte 0x000000a8
.L_0816742c:
	.4byte IwramCopyWords
.L_08167430:
	.4byte 0x05000200
.L_08167434:
	.4byte ResourceTableEntries
.L_08167438:
	.4byte 0x80002000
.L_0816743c:
	.4byte 0x06010000
.L_08167440:
	.4byte gCameraSceneParameters
.L_08167444:
	.4byte Data_03001120
.L_08167448:
	.4byte 0xffc00000
.L_0816744c:
	.4byte 0x0000017f
.L_08167450:
	ldr r4, [sp, #88]
	movs r5, #208
	lsls r5, r5, #4
	adds r1, r4, r5
	ldr r0, .L_081674d8
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_081674dc
	ldr r1, .L_081674e0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #68]
	movs r3, #0
	ldr r0, .L_081674e4
	bl Resource_LoadAndDecompress
	ldr r0, .L_081674e8
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	ldr r3, .L_081674ec
	adds r1, r7, #0
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r6, [sp, #88]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r6, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r6, r1
	movs r3, #50
	str r3, [r2]
	ldr r3, .L_081674d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	mov r3, sp
	movs r2, #0
	adds r3, #176
	movs r4, #148
	str r2, [sp, #72]
	str r3, [sp, #12]
	add r4, sp
	mov r11, r4
.L_081674c2:
	ldr r5, [sp, #72]
	cmp r5, #0
	bne .L_08167586
	movs r6, #255
	ldr r5, [sp, #88]
	lsls r6, r6, #8
	movs r7, #0
	adds r6, #255
	b .L_081674f0
.L_081674d4:
	.4byte 0x00000784
.L_081674d8:
	.4byte 0x00000192
.L_081674dc:
	.4byte 0x000000c2
.L_081674e0:
	.4byte Data_02014000
.L_081674e4:
	.4byte 0x00000134
.L_081674e8:
	.4byte 0x00000126
.L_081674ec:
	.4byte IwramCopyWords
.L_081674f0:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #72
	str r3, [r5]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	adds r7, #1
	ands r0, r6
	str r0, [r5, #16]
	adds r5, #28
	cmp r7, #64
	bne .L_081674f0
	ldr r0, .L_08167854
	bl Resource_GetTableEntry
	movs r6, #128
	adds r7, r0, #0
	lsls r6, r6, #2
	movs r2, #128
	ldr r5, .L_08167858
	adds r1, r7, #0
	lsls r2, r2, #2
	adds r7, r7, r6
	ldr r0, .L_0816785c
	mov lr, r5
	.2byte 0xf800
	adds r0, r7, #0
	ldr r1, .L_08167860
	bl Resource_DecodeType01
	ldr r0, .L_08167864
	ldr r1, [sp, #88]
	movs r2, #238
	adds r4, r5, #0
	lsls r2, r2, #7
	ldr r5, .L_08167860
	adds r2, #220
	movs r7, #0
	mov r8, r0
	adds r6, r1, r2
.L_08167554:
	ldmia r6!, {r3}
	movs r2, #128
	ldrb r3, [r3, #16]
	adds r1, r5, #0
	lsls r3, r3, #2
	add r3, r8
	ldrh r0, [r3, #2]
	ldr r3, .L_08167868
	str r4, [sp, #8]
	adds r0, r0, r3
	lsls r2, r2, #3
	mov lr, r4
	.2byte 0xf800
	movs r0, #128
	lsls r0, r0, #3
	adds r7, #1
	adds r5, r5, r0
	ldr r4, [sp, #8]
	cmp r7, #16
	bne .L_08167554
	ldr r1, .L_0816786c
	movs r2, #128
	lsls r2, r2, #12
	str r1, [sp, #64]
	str r2, [sp, #60]
.L_08167586:
	ldr r3, [sp, #72]
	cmp r3, #143
	bne .L_081675a0
	movs r1, #128
	ldr r3, .L_08167870
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	ldr r2, .L_08167874
	mov lr, r3
	.2byte 0xf800
	movs r0, #145
	bl Audio_PlayCue
.L_081675a0:
	ldr r4, [sp, #72]
	cmp r4, #80
	bne .L_081675ac
	movs r0, #142
	bl Audio_PlayCue
.L_081675ac:
	ldr r5, [sp, #72]
	cmp r5, #72
	bne .L_081675cc
	ldr r6, [sp, #88]
	movs r0, #238
	lsls r0, r0, #7
	movs r1, #238
	adds r0, #180
	lsls r1, r1, #7
	adds r2, r6, r0
	movs r3, #24
	adds r1, #184
	str r3, [r2]
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
.L_081675cc:
	ldr r2, [sp, #12]
	movs r3, #0
	str r3, [r2, #8]
	str r3, [r2, #4]
	ldr r6, [sp, #88]
	movs r7, #0
.L_081675d8:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_081675e0
	adds r3, r7, #3
.L_081675e0:
	ldr r4, [sp, #72]
	asrs r3, r3, #2
	adds r3, #80
	cmp r4, r3
	ble .L_0816767c
	ldr r3, [r6]
	cmp r3, #0
	ble .L_0816767c
	bl Func_08014de4
	ldr r0, [r6, #16]
	bl Func_080150e4
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r5, [sp, #72]
	ldr r0, [r6, #16]
	lsls r3, r5, #9
	adds r0, r0, r3
	bl Func_08015068
	ldr r3, [r6]
	ldr r0, [sp, #12]
	str r3, [r0]
	ldr r3, [r6]
	subs r3, #2
	str r3, [r6]
	cmp r3, #0
	bge .L_08167620
	movs r3, #0
	str r3, [r6]
.L_08167620:
	add r5, sp, #164
	ldr r0, [sp, #12]
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r0, [r5, #8]
	movs r3, #60
	negs r3, r3
	cmp r0, r3
	bge .L_08167638
	str r3, [r5, #8]
	adds r0, r3, #0
.L_08167638:
	cmp r0, #60
	ble .L_08167642
	movs r3, #60
	str r3, [r5, #8]
	movs r0, #60
.L_08167642:
	adds r0, #60
	str r0, [r5, #8]
	movs r1, #20
	bl Math_Div
	ldr r2, [r5]
	ldr r3, [r5, #4]
	adds r0, #2
	adds r2, #56
	adds r3, #96
	ldr r4, .L_08167878
	str r2, [r5]
	str r3, [r5, #4]
	lsls r5, r0, #1
	subs r1, r5, #2
	ldrh r1, [r4, r1]
	ldr r4, [sp, #68]
	subs r3, r3, r0
	adds r1, r4, r1
	lsrs r4, r0, #31
	adds r4, r0, r4
	asrs r4, r4, #1
	str r0, [sp, #0]
	str r5, [sp, #4]
	subs r2, r2, r4
	ldr r0, [sp, #92]
	ldr r5, [sp, #76]
	mov lr, r5
	.2byte 0xf800
.L_0816767c:
	adds r7, #1
	adds r6, #28
	cmp r7, #64
	bne .L_081675d8
	ldr r3, .L_0816787c
	mov r6, r11
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r1, #238
	str r3, [sp, #124]
	str r4, [sp, #128]
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #4]
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	adds r1, #220
	movs r7, #0
	adds r5, r0, r1
.L_081676a2:
	ldr r2, [sp, #72]
	cmp r2, #75
	ble .L_081676b4
	cmp r7, #8
	beq .L_081676e2
	cmp r7, #12
	beq .L_081676e2
	cmp r7, #15
	beq .L_081676e2
.L_081676b4:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_081676bc
	adds r3, r7, #3
.L_081676bc:
	asrs r3, r3, #2
	lsls r2, r3, #2
	subs r2, r7, r2
	movs r4, #152
	lsls r2, r2, #21
	lsls r4, r4, #15
	adds r2, r2, r4
	mov r6, r11
	str r2, [r6]
	ldr r0, [sp, #64]
	lsls r3, r3, #21
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r0, [r5]
	mov r1, r11
	add r2, sp, #124
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_081676e2:
	adds r7, #1
	adds r5, #4
	cmp r7, #16
	bne .L_081676a2
	ldr r1, [sp, #64]
	ldr r2, [sp, #60]
	ldr r3, [sp, #72]
	adds r1, r1, r2
	str r1, [sp, #64]
	cmp r3, #47
	bgt .L_081676fe
	ldr r4, .L_08167880
	adds r2, r2, r4
	str r2, [sp, #60]
.L_081676fe:
	ldr r5, [sp, #72]
	cmp r5, #32
	ble .L_08167716
	ldr r6, [sp, #60]
	lsls r3, r6, #4
	subs r3, r3, r6
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_08167712
	adds r3, #63
.L_08167712:
	asrs r3, r3, #6
	str r3, [sp, #60]
.L_08167716:
	ldr r0, [sp, #72]
	cmp r0, #144
	bne .L_08167720
	ldr r1, .L_08167884
	str r1, [sp, #60]
.L_08167720:
	ldr r2, [sp, #72]
	cmp r2, #146
	bne .L_0816772c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #60]
.L_0816772c:
	ldr r4, [sp, #72]
	cmp r4, #72
	bne .L_081677ae
	ldr r0, .L_08167888
	bl Resource_GetTableEntry
	movs r5, #128
	adds r7, r0, #0
	lsls r5, r5, #2
	movs r2, #128
	ldr r3, .L_08167858
	adds r1, r7, #0
	lsls r2, r2, #2
	adds r7, r7, r5
	ldr r0, .L_0816785c
	mov lr, r3
	.2byte 0xf800
	adds r0, r7, #0
	ldr r1, .L_08167860
	bl Resource_DecodeType01
	ldr r1, .L_08167864
	ldr r2, [sp, #88]
	movs r3, #238
	lsls r3, r3, #7
	ldr r6, .L_0816788c
	ldr r0, .L_08167890
	ldr r5, .L_08167860
	adds r3, #220
	mov r12, r1
	movs r7, #0
	adds r1, r2, r3
.L_0816776c:
	ldrh r3, [r0]
	mov lr, r3
	strh r0, [r0]
	ldrh r3, [r6]
	cmp r3, #31
	bgt .L_0816779c
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r2, r6, r2
	adds r2, #4
	adds r3, #1
	strh r3, [r6]
	stmia r2!, {r5}
	ldr r3, [r1]
	ldr r4, .L_08167868
	ldrb r3, [r3, #16]
	lsls r3, r3, #2
	add r3, r12
	ldrh r3, [r3, #2]
	adds r3, r3, r4
	stmia r2!, {r3}
	ldr r3, .L_08167894
	str r3, [r2]
.L_0816779c:
	mov r2, lr
	strh r2, [r0]
	movs r3, #128
	lsls r3, r3, #3
	adds r7, #1
	adds r5, r5, r3
	adds r1, #4
	cmp r7, #16
	bne .L_0816776c
.L_081677ae:
	ldr r4, [sp, #72]
	cmp r4, #76
	bne .L_08167836
	ldr r0, .L_08167898
	bl Resource_GetTableEntry
	movs r5, #128
	adds r7, r0, #0
	lsls r5, r5, #2
	movs r2, #128
	adds r1, r7, #0
	ldr r3, .L_08167858
	adds r7, r7, r5
	lsls r2, r2, #2
	ldr r0, .L_0816785c
	mov lr, r3
	.2byte 0xf800
	adds r0, r7, #0
	ldr r1, .L_08167860
	bl Resource_DecodeType01
	ldr r0, .L_08167864
	ldr r6, .L_0816788c
	ldr r1, .L_08167890
	ldr r5, .L_08167860
	ldr r4, .L_0816789c
	movs r7, #0
	mov r12, r0
.L_081677e6:
	ldrh r3, [r1]
	mov lr, r3
	strh r1, [r1]
	ldrh r3, [r6]
	cmp r3, #31
	bgt .L_08167824
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r6]
	lsls r2, r2, #2
	ldrb r3, [r4]
	adds r2, r6, r2
	movs r0, #238
	adds r2, #4
	lsls r0, r0, #7
	stmia r2!, {r5}
	adds r0, #220
	lsls r3, r3, #2
	adds r3, r3, r0
	ldr r0, [sp, #88]
	ldr r3, [r0, r3]
	ldr r0, .L_08167868
	ldrb r3, [r3, #16]
	lsls r3, r3, #2
	add r3, r12
	ldrh r3, [r3, #2]
	adds r3, r3, r0
	stmia r2!, {r3}
	ldr r3, .L_08167894
	str r3, [r2]
.L_08167824:
	mov r2, lr
	strh r2, [r1]
	movs r3, #128
	lsls r3, r3, #3
	adds r7, #1
	adds r5, r5, r3
	adds r4, #1
	cmp r7, #13
	bne .L_081677e6
.L_08167836:
	ldr r6, [sp, #72]
	subs r6, #116
	cmp r6, #27
	bls .L_08167840
	b .L_08167956
.L_08167840:
	ldr r4, [sp, #72]
	ldr r0, .L_081678a0
	lsls r3, r4, #1
	adds r3, r3, r4
	adds r5, r3, r0
	cmp r5, #80
	ble .L_081678a4
	movs r5, #80
	b .L_081678b8
	.2byte 0x0000
.L_08167854:
	.4byte 0x000000a8
.L_08167858:
	.4byte IwramCopyWords
.L_0816785c:
	.4byte 0x05000200
.L_08167860:
	.4byte gMapCellBuffer
.L_08167864:
	.4byte ResourceTableEntries
.L_08167868:
	.4byte 0x06010000
.L_0816786c:
	.4byte 0xffc00000
.L_08167870:
	.4byte IwramFillWords
.L_08167874:
	.4byte 0x2a2a2a2a
.L_08167878:
	.4byte Data_08197410
.L_0816787c:
	.4byte Data_08196e74
.L_08167880:
	.4byte 0xffffc000
.L_08167884:
	.4byte 0xfff80000
.L_08167888:
	.4byte 0x000000a9
.L_0816788c:
	.4byte gIoWriteQueue
.L_08167890:
	.4byte 0x04000208
.L_08167894:
	.4byte 0x84000100
.L_08167898:
	.4byte 0x000000aa
.L_0816789c:
	.4byte Data_08198ab4
.L_081678a0:
	.4byte 0xfffffea6
.L_081678a4:
	ldr r1, [sp, #88]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #86
	adds r0, r1, r2
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #9
	bl Func_0815b434
.L_081678b8:
	ldr r3, [sp, #88]
	movs r4, #200
	lsls r4, r4, #5
	adds r4, #86
	adds r0, r3, r4
	movs r1, #60
	movs r2, #80
	adds r3, r5, #0
	bl Func_0818caa8
	cmp r6, #27
	bhi .L_08167956
	adds r0, r6, #0
	movs r1, #3
	bl Math_Div
	adds r0, #16
	mov r8, r0
	cmp r0, #32
	ble .L_081678e4
	movs r5, #32
	mov r8, r5
.L_081678e4:
	ldr r6, .L_081679e8
	movs r7, #0
	mov r10, r6
.L_081678ea:
	movs r4, #3
	ands r4, r7
	str r4, [sp, #8]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	ldr r4, [sp, #8]
	mov r6, r8
	muls r6, r0
	mov r0, r10
	ldrb r3, [r0, r4]
	asrs r6, r6, #16
	lsrs r3, r3, #1
	adds r0, r5, #0
	subs r6, r6, r3
	bl Trig_Cos
	ldr r2, .L_081679ec
	ldr r4, [sp, #8]
	mov r3, r8
	muls r3, r0
	ldrb r0, [r2, r4]
	ldr r1, .L_081679f0
	lsrs r2, r0, #1
	asrs r3, r3, #16
	subs r3, r3, r2
	lsls r2, r4, #1
	ldrh r1, [r1, r2]
	ldr r2, [sp, #88]
	movs r5, #224
	adds r1, r2, r1
	lsls r5, r5, #3
	adds r1, r1, r5
	mov r5, r10
	ldrb r2, [r5, r4]
	adds r6, #60
	str r2, [sp, #0]
	str r0, [sp, #4]
	adds r2, r6, #0
	adds r3, #80
	ldr r0, [sp, #92]
	ldr r6, [sp, #76]
	adds r7, #1
	mov lr, r6
	.2byte 0xf800
	cmp r7, #6
	bne .L_081678ea
.L_08167956:
	ldr r2, [sp, #72]
	movs r3, #0
	subs r2, #108
	cmp r2, #34
	bhi .L_0816797e
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r2, r3, #1
	adds r1, r2, #0
	cmp r2, #0
	bge .L_0816796e
	adds r1, r2, #3
.L_0816796e:
	asrs r1, r1, #2
	lsrs r3, r3, #31
	adds r3, r2, r3
	str r1, [sp, #56]
	str r2, [sp, #48]
	asrs r3, r3, #1
	str r3, [sp, #52]
	movs r3, #1
.L_0816797e:
	ldr r0, [sp, #72]
	cmp r0, #143
	bne .L_0816798e
	movs r1, #31
	str r1, [sp, #56]
	str r1, [sp, #52]
	str r1, [sp, #48]
	movs r3, #1
.L_0816798e:
	ldr r2, [sp, #72]
	subs r2, #144
	mov r9, r2
	cmp r2, #8
	bhi .L_081679aa
	ldr r3, [sp, #72]
	lsls r2, r3, #1
	movs r3, #152
	lsls r3, r3, #1
	subs r2, r3, r2
	str r2, [sp, #52]
	str r2, [sp, #56]
	str r2, [sp, #48]
	movs r3, #1
.L_081679aa:
	cmp r3, #1
	bne .L_08167a2c
	ldr r0, .L_081679f4
	bl Resource_GetTableEntry
	ldr r5, .L_081679e4
	ldr r4, .L_081679f8
	movs r7, #0
.L_081679ba:
	ldrh r3, [r0]
	ldr r6, [sp, #56]
	movs r2, #31
	ands r2, r3
	adds r1, r2, r6
	lsls r3, r3, #16
	ldr r6, [sp, #52]
	lsrs r2, r3, #21
	ands r2, r5
	adds r2, r2, r6
	ldr r6, [sp, #48]
	lsrs r3, r3, #26
	ands r3, r5
	adds r3, r3, r6
	cmp r1, #31
	ble .L_081679dc
	movs r1, #31
.L_081679dc:
	cmp r2, #31
	ble .L_081679fc
	movs r2, #31
	b .L_081679fc
.L_081679e4:
	.4byte 0x0000001f
.L_081679e8:
	.4byte Data_08198ace
.L_081679ec:
	.4byte Data_08198ad4
.L_081679f0:
	.4byte Data_08198ac2
.L_081679f4:
	.4byte 0x000000aa
.L_081679f8:
	.4byte 0x05000200
.L_081679fc:
	cmp r3, #31
	ble .L_08167a02
	movs r3, #31
.L_08167a02:
	cmp r1, #0
	bge .L_08167a08
	movs r1, #0
.L_08167a08:
	cmp r2, #0
	bge .L_08167a0e
	movs r2, #0
.L_08167a0e:
	cmp r3, #0
	bge .L_08167a14
	movs r3, #0
.L_08167a14:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	movs r1, #128
	adds r7, #1
	lsls r1, r1, #1
	strh r3, [r4]
	adds r0, #2
	adds r4, #2
	cmp r7, r1
	bne .L_081679ba
.L_08167a2c:
	ldr r2, [sp, #72]
	cmp r2, #143
	ble .L_08167a94
	ldr r3, .L_08167c38
	lsls r2, r2, #4
	mov r8, r2
	movs r1, #3
	movs r0, #188
	add r8, r3
	bl Func_081963ec
	movs r6, #24
	movs r5, #64
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r2, [sp, #88]
	movs r4, #192
	lsls r4, r4, #18
	movs r3, #224
	adds r4, #188
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, [sp, #92]
	movs r2, #36
	mov r3, r8
	mov r10, r4
	ldr r4, [r4]
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #7
	movs r0, #188
	bl Func_081963ec
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r6, [sp, #88]
	movs r2, #224
	lsls r2, r2, #3
	mov r5, r10
	adds r1, r6, r2
	ldr r4, [r5]
	ldr r0, [sp, #92]
	movs r2, #60
	mov r3, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_08167a94:
	mov r3, r9
	cmp r3, #15
	bhi .L_08167b26
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r4, [sp, #72]
	ldr r1, .L_08167c3c
	lsls r3, r4, #13
	adds r7, r3, r1
	ldr r2, .L_08167c40
	ldr r3, [sp, #116]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08167c44
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, .L_08167c48
	adds r5, r0, #0
	str r3, [sp, #116]
	add r3, sp, #116
	str r2, [r3, #4]
	str r3, [r5, #16]
	ldr r3, .L_08167c4c
	movs r0, #0
	str r1, [r5]
	str r3, [r5, #8]
	str r6, [r5, #12]
	cmp r4, #151
	ble .L_08167ae8
	lsls r3, r4, #3
	movs r2, #152
	negs r3, r3
	lsls r2, r2, #3
	adds r0, r3, r2
.L_08167ae8:
	str r0, [r5, #20]
	bl Func_08014de4
	movs r1, #192
	lsls r1, r1, #13
	movs r2, #0
	ldr r0, .L_08167c50
	bl Func_08015160
	adds r0, r7, #0
	bl Func_0801521c
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #184
	bl SceneTransform_ApplyPitch
	adds r1, r6, #0
	movs r2, #4
	ldr r0, .L_08167c54
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r6, #0
	bl Sys_Free
.L_08167b26:
	ldr r3, [sp, #88]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #72]
	adds r5, #1
	str r5, [sp, #72]
	cmp r5, #160
	beq .L_08167b58
	cmp r5, #4
	bgt .L_08167b4a
	b .L_081674c2
.L_08167b4a:
	ldr r3, .L_08167c58
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08167b58
	b .L_081674c2
.L_08167b58:
	movs r1, #128
	ldr r3, .L_08167c5c
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	movs r0, #238
	ldr r6, [sp, #88]
	lsls r0, r0, #7
	adds r0, #220
	movs r7, #0
	adds r5, r6, r0
.L_08167b72:
	ldmia r5!, {r0}
	adds r7, #1
	bl ResourceObject_ReleaseFar
	cmp r7, #16
	bne .L_08167b72
	movs r0, #1
	bl WaitFrames
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08167c60
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08167c64
	movs r3, #160
	ldrh r2, [r2]
	lsls r3, r3, #19
	adds r3, #188
	movs r6, #238
	movs r1, #13
	strh r2, [r3]
	lsls r6, r6, #7
	negs r1, r1
	movs r7, #0
	adds r6, #220
	mov r8, r1
.L_08167bba:
	movs r0, #199
	lsls r0, r0, #1
	adds r0, #255
	bl GetBattleEffectObject
	ldr r2, [sp, #88]
	adds r5, r0, #0
	str r5, [r6, r2]
	cmp r5, #0
	beq .L_08167bee
	movs r3, #0
	strb r3, [r5, #26]
	movs r1, #3
	adds r0, r7, #0
	bl Math_Mod
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgumentFar
	ldr r3, [sp, #88]
	mov r4, r8
	ldr r2, [r6, r3]
	ldrb r3, [r2, #9]
	ands r3, r4
	strb r3, [r2, #9]
.L_08167bee:
	adds r7, #1
	adds r6, #4
	cmp r7, #16
	bne .L_08167bba
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r5, [sp, #88]
	movs r6, #142
	lsls r6, r6, #7
	str r3, [sp, #76]
	ldr r0, .L_08167c68
	adds r1, r5, r6
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r0, #192
	ldr r1, [sp, #68]
	lsls r0, r0, #2
	movs r7, #0
	movs r4, #1
	adds r0, #2
.L_08167c2a:
	ldrb r3, [r1]
	adds r2, r3, #0
	cmp r2, #32
	bls .L_08167c6c
	adds r3, #224
	strb r3, [r1]
	b .L_08167c72
.L_08167c38:
	.4byte 0xfffff720
.L_08167c3c:
	.4byte 0xffee2000
.L_08167c40:
	.4byte 0xffffff00
.L_08167c44:
	.4byte 0xffff00ff
.L_08167c48:
	.4byte Data_02014000
.L_08167c4c:
	.4byte Data_08199364
.L_08167c50:
	.4byte 0xfffc0000
.L_08167c54:
	.4byte Data_08199210
.L_08167c58:
	.4byte gInput
.L_08167c5c:
	.4byte IwramFillWords
.L_08167c60:
	.4byte 0x05000200
.L_08167c64:
	.4byte 0x050001e8
.L_08167c68:
	.4byte 0x00000126
.L_08167c6c:
	cmp r2, #0
	beq .L_08167c72
	strb r4, [r1]
.L_08167c72:
	adds r7, #1
	adds r1, #1
	cmp r7, r0
	bne .L_08167c2a
	ldr r0, [sp, #88]
	movs r1, #238
	lsls r1, r1, #7
	movs r4, #238
	adds r1, #180
	lsls r4, r4, #7
	adds r3, r0, r1
	movs r2, #0
	adds r4, #184
	str r2, [r3]
	adds r3, r0, r4
	str r2, [r3]
	ldr r1, .L_08167ce0
	movs r0, #1
	bl Func_08118040
	ldr r3, .L_08167cd8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08167ce4
	adds r2, #8
	str r3, [r2]
	ldr r3, .L_08167cdc
	subs r2, #28
	strh r3, [r2]
	ldr r5, .L_08167ce8
	movs r1, #128
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	movs r6, #60
	ldr r4, [sp, #88]
	movs r1, #6
	str r6, [sp, #44]
	str r1, [sp, #36]
	movs r6, #239
	movs r1, #238
	movs r0, #44
	movs r3, #2
	lsls r6, r6, #7
	lsls r1, r1, #7
	b .L_08167cec
	.2byte 0x0000
.L_08167cd8:
	.4byte 0x00000100
.L_08167cdc:
	.4byte 0x00000784
.L_08167ce0:
	.4byte 0x00000075
.L_08167ce4:
	.4byte 0xffffc400
.L_08167ce8:
	.4byte IwramFillWords
.L_08167cec:
	str r0, [sp, #40]
	str r3, [sp, #28]
	movs r2, #0
	adds r3, r4, r6
	movs r0, #2
	adds r1, #132
	str r2, [sp, #32]
	str r2, [sp, #24]
	str r0, [r3]
	adds r2, r4, r1
	movs r3, #75
	str r3, [r2]
	movs r2, #224
	lsls r2, r2, #3
	movs r1, #225
	adds r0, r4, r2
	lsls r1, r1, #6
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [sp, #72]
.L_08167d18:
	ldr r4, [sp, #72]
	cmp r4, #66
	bne .L_08167d24
	movs r0, #208
	bl Audio_PlayCue
.L_08167d24:
	ldr r5, [sp, #72]
	cmp r5, #88
	bne .L_08167d30
	movs r0, #230
	bl Audio_PlayCue
.L_08167d30:
	ldr r6, [sp, #72]
	cmp r6, #155
	bne .L_08167d3c
	movs r0, #162
	bl Audio_PlayCue
.L_08167d3c:
	ldr r0, [sp, #72]
	cmp r0, #217
	bne .L_08167d48
	movs r0, #156
	bl Audio_PlayCue
.L_08167d48:
	ldr r1, [sp, #72]
	movs r2, #140
	lsls r2, r2, #1
	cmp r1, r2
	bne .L_08167d58
	movs r0, #157
	bl Audio_PlayCue
.L_08167d58:
	ldr r3, [sp, #72]
	movs r4, #150
	lsls r4, r4, #1
	cmp r3, r4
	bne .L_08167d98
	ldr r5, [sp, #96]
	movs r7, #0
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_08167d92
	movs r5, #36
.L_08167d6e:
	ldr r6, [sp, #96]
	movs r3, #8
	ldrsh r0, [r5, r6]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	bl Func_0814cd48
	ldrsh r0, [r5, r6]
	movs r1, #1
	bl Func_08118088
	ldr r3, [r6, #20]
	adds r7, #1
	adds r5, #2
	cmp r7, r3
	bne .L_08167d6e
.L_08167d92:
	movs r0, #145
	bl Func_081180e8
.L_08167d98:
	ldr r3, .L_08167e40
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08167e5e
	ldr r3, [sp, #72]
	subs r3, #5
	cmp r3, #144
	bhi .L_08167e32
	ldr r1, .L_08167e44
	movs r3, #150
	str r3, [sp, #72]
	ldr r4, .L_08167e48
	ldrh r3, [r4]
	adds r0, r3, #0
	movs r5, #130
	ldr r6, .L_08167e48
	lsls r5, r5, #2
	strh r5, [r6]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08167de4
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
.L_08167de4:
	ldr r2, .L_08167e48
	strh r0, [r2]
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	ldr r4, .L_08167e48
	lsls r3, r3, #2
	strh r3, [r4]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08167e1a
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	movs r5, #0
	stmia r3!, {r5}
	strh r2, [r1]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_08167e1a:
	ldr r6, .L_08167e48
	strh r0, [r6]
	ldr r0, .L_08167e4c
	movs r3, #142
	ldr r2, [sp, #88]
	lsls r3, r3, #7
	adds r1, r2, r3
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_08167e5e
.L_08167e32:
	ldr r3, [sp, #72]
	subs r3, #155
	cmp r3, #58
	bhi .L_08167e50
	movs r4, #214
	str r4, [sp, #72]
	b .L_08167e5e
.L_08167e40:
	.4byte gInput
.L_08167e44:
	.4byte gIoWriteQueue
.L_08167e48:
	.4byte 0x04000208
.L_08167e4c:
	.4byte 0x00000131
.L_08167e50:
	ldr r3, [sp, #72]
	subs r3, #219
	cmp r3, #60
	bhi .L_08167e5e
	movs r5, #140
	lsls r5, r5, #1
	str r5, [sp, #72]
.L_08167e5e:
	ldr r6, [sp, #72]
	movs r0, #159
	lsls r0, r0, #1
	cmp r6, r0
	bne .L_08167e76
	movs r1, #128
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	ldr r2, .L_08167f7c
	ldr r3, .L_08167f80
	mov lr, r3
	.2byte 0xf800
.L_08167e76:
	ldr r4, [sp, #72]
	movs r5, #64
	adds r5, #255
	cmp r4, r5
	bne .L_08167e9c
	bl Func_0815b410
	ldr r6, [sp, #88]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r6, r0
	movs r3, #3
	movs r1, #238
	str r3, [r2]
	lsls r1, r1, #7
	ldr r3, .L_08167f84
	adds r1, #132
	adds r2, r6, r1
	str r3, [r2]
.L_08167e9c:
	ldr r2, [sp, #72]
	cmp r2, #64
	bne .L_08167f42
	ldr r1, .L_08167f88
	ldr r4, .L_08167f8c
	ldrh r3, [r4]
	adds r0, r3, #0
	movs r5, #130
	ldr r6, .L_08167f8c
	lsls r5, r5, #2
	strh r5, [r6]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08167ed6
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
.L_08167ed6:
	ldr r2, .L_08167f8c
	strh r0, [r2]
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	ldr r4, .L_08167f8c
	lsls r3, r3, #2
	strh r3, [r4]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08167f0c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	movs r5, #0
	stmia r3!, {r5}
	strh r2, [r1]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_08167f0c:
	ldr r6, .L_08167f8c
	strh r0, [r6]
	ldr r0, [sp, #92]
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	ldr r3, .L_08167f80
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_08167f90
	movs r5, #142
	ldr r4, [sp, #88]
	lsls r5, r5, #7
	adds r1, r4, r5
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r6, [sp, #88]
	movs r2, #206
	lsls r2, r2, #7
	adds r1, r6, r2
	ldr r0, .L_08167f94
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_08167f42:
	ldr r3, [sp, #72]
	cmp r3, #66
	bne .L_08167f60
	movs r2, #160
	ldr r1, .L_08167f78
	lsls r2, r2, #19
	adds r2, #192
	movs r7, #0
.L_08167f52:
	ldrh r3, [r2]
	adds r7, #1
	eors r3, r1
	strh r3, [r2]
	adds r2, #2
	cmp r7, #128
	bne .L_08167f52
.L_08167f60:
	ldr r4, [sp, #72]
	cmp r4, #69
	bne .L_08167f98
	movs r1, #128
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	ldr r2, .L_08167f7c
	ldr r5, .L_08167f80
	mov lr, r5
	.2byte 0xf800
	b .L_08167f98
	.2byte 0x0000
.L_08167f78:
	.4byte 0x00007fff
.L_08167f7c:
	.4byte 0x3f3f3f3f
.L_08167f80:
	.4byte IwramFillWords
.L_08167f84:
	.4byte 0x04040404
.L_08167f88:
	.4byte gIoWriteQueue
.L_08167f8c:
	.4byte 0x04000208
.L_08167f90:
	.4byte 0x00000131
.L_08167f94:
	.4byte 0x00000127
.L_08167f98:
	ldr r6, [sp, #72]
	cmp r6, #70
	bne .L_08167fa8
	movs r0, #1
	ldr r1, .L_081680b0
	movs r2, #7
	bl Func_08118038
.L_08167fa8:
	ldr r0, [sp, #72]
	cmp r0, #150
	bne .L_08167fee
	movs r1, #112
	movs r2, #32
	movs r3, #0
	movs r4, #4
	movs r5, #8
	str r1, [sp, #44]
	str r2, [sp, #40]
	str r3, [sp, #32]
	str r4, [sp, #28]
	str r5, [sp, #36]
	ldr r6, [sp, #88]
	movs r1, #224
	lsls r1, r1, #3
	adds r0, r6, r1
	movs r1, #225
	lsls r1, r1, #6
	movs r2, #0
	ldr r3, .L_081680b4
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #92]
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_081680b8
	ldr r4, .L_081680b4
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	ldr r1, .L_081680bc
	movs r2, #0
	bl Func_08118040
.L_08167fee:
	ldr r5, [sp, #72]
	cmp r5, #214
	bne .L_08168066
	ldr r6, [sp, #88]
	movs r1, #224
	lsls r1, r1, #3
	adds r0, r6, r1
	movs r1, #225
	lsls r1, r1, #6
	movs r2, #0
	ldr r3, .L_081680b4
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #92]
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_081680b8
	ldr r4, .L_081680b4
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	ldr r1, .L_081680c0
	movs r2, #0
	bl Func_08118040
	ldr r5, [sp, #88]
	movs r7, #0
.L_08168024:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #96
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	ldr r2, [r5]
	adds r3, #16
	lsls r3, r3, #15
	str r3, [r5, #16]
	movs r3, #128
	asrs r2, r2, #7
	lsls r3, r3, #8
	subs r3, r3, r2
	str r3, [r5, #8]
	adds r7, #1
	movs r3, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #16
	bne .L_08168024
.L_08168066:
	ldr r5, [sp, #72]
	movs r6, #140
	lsls r6, r6, #1
	cmp r5, r6
	bne .L_0816811c
	bl Func_0814cca8
	ldr r1, [sp, #88]
	movs r2, #224
	lsls r2, r2, #3
	adds r0, r1, r2
	movs r1, #225
	lsls r1, r1, #6
	movs r2, #0
	ldr r3, .L_081680b4
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #92]
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_081680c4
	ldr r4, .L_081680b4
	mov lr, r4
	.2byte 0xf800
	movs r5, #0
	ldr r6, [sp, #84]
	movs r2, #128
	ldr r3, .L_081680ac
	lsls r2, r2, #19
	adds r2, #82
	str r5, [r6, #16]
	strh r3, [r2]
	ldr r5, [sp, #88]
	movs r7, #0
	b .L_081680c8
.L_081680ac:
	.4byte 0x00001010
.L_081680b0:
	.4byte 0x00000075
.L_081680b4:
	.4byte IwramFillWords
.L_081680b8:
	.4byte 0x3f3f3f3f
.L_081680bc:
	.4byte 0x00000040
.L_081680c0:
	.4byte 0x00000044
.L_081680c4:
	.4byte 0x01010101
.L_081680c8:
	bl Random16
	movs r3, #63
	ands r3, r0
	movs r0, #128
	lsls r0, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #96
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	lsls r3, r3, #15
	movs r1, #0
	adds r7, #1
	str r3, [r5, #16]
	str r1, [r5, #8]
	adds r5, #28
	cmp r7, #16
	bne .L_081680c8
	ldr r4, [sp, #88]
	movs r5, #239
	movs r6, #238
	movs r2, #136
	lsls r5, r5, #7
	lsls r6, r6, #7
	adds r3, r4, r5
	lsls r2, r2, #1
	adds r6, #132
	str r2, [sp, #24]
	str r1, [r3]
	adds r3, r4, r6
	str r1, [r3]
.L_0816811c:
	ldr r0, [sp, #72]
	ldr r1, .L_08168158
	adds r3, r0, r1
	cmp r3, #38
	bhi .L_08168174
	movs r5, #160
	ldr r6, .L_08168154
	lsls r5, r5, #19
	adds r5, #2
	movs r7, #0
.L_08168130:
	ldrh r2, [r5]
	movs r4, #31
	lsls r3, r2, #16
	lsrs r0, r3, #26
	ands r0, r6
	lsrs r1, r3, #21
	ands r1, r6
	ands r4, r2
	adds r0, #1
	adds r1, #1
	adds r4, #1
	cmp r0, #31
	ble .L_0816814c
	movs r0, #31
.L_0816814c:
	cmp r1, #31
	ble .L_0816815c
	movs r1, #31
	b .L_0816815c
.L_08168154:
	.4byte 0x0000001f
.L_08168158:
	.4byte 0xfffffee8
.L_0816815c:
	cmp r4, #31
	ble .L_08168162
	movs r4, #31
.L_08168162:
	lsls r3, r0, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r4
	adds r7, #1
	strh r3, [r5]
	adds r5, #2
	cmp r7, #63
	bne .L_08168130
.L_08168174:
	ldr r2, [sp, #72]
	cmp r2, #182
	bne .L_08168188
	movs r1, #128
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	ldr r2, .L_081684ac
	ldr r3, .L_081684b0
	mov lr, r3
	.2byte 0xf800
.L_08168188:
	ldr r4, [sp, #72]
	cmp r4, #63
	bgt .L_0816824e
	ldr r5, [sp, #72]
	ldr r6, [sp, #88]
	movs r3, #7
	movs r0, #142
	subs r4, #4
	lsls r0, r0, #7
	ands r3, r5
	mov r8, r4
	adds r7, r6, r0
	cmp r3, #3
	ble .L_081681ae
	ldr r1, [sp, #88]
	movs r2, #146
	lsls r2, r2, #7
	adds r2, #64
	adds r7, r1, r2
.L_081681ae:
	movs r1, #19
	movs r0, #188
	bl Func_081963ec
	movs r5, #24
	str r5, [sp, #0]
	str r5, [sp, #4]
	movs r3, #192
	mov r6, r8
	lsls r3, r3, #18
	subs r6, #24
	adds r3, #188
	ldr r0, [sp, #92]
	ldr r4, [r3]
	movs r2, #36
	adds r3, r6, #0
	adds r1, r7, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	movs r0, #192
	str r5, [sp, #0]
	str r5, [sp, #4]
	lsls r0, r0, #18
	adds r0, #188
	ldr r4, [r0]
	adds r3, r6, #0
	ldr r0, [sp, #92]
	movs r2, #59
	adds r1, r7, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #27
	movs r0, #188
	bl Func_081963ec
	movs r1, #192
	str r5, [sp, #0]
	str r5, [sp, #4]
	lsls r1, r1, #18
	adds r6, #23
	adds r1, #188
	ldr r4, [r1]
	ldr r0, [sp, #92]
	movs r2, #36
	adds r3, r6, #0
	adds r1, r7, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #31
	movs r0, #188
	bl Func_081963ec
	movs r2, #192
	str r5, [sp, #0]
	str r5, [sp, #4]
	lsls r2, r2, #18
	adds r2, #188
	ldr r4, [r2]
	ldr r0, [sp, #92]
	adds r1, r7, #0
	movs r2, #59
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_0816824e:
	ldr r3, [sp, #72]
	subs r3, #64
	cmp r3, #1
	bhi .L_08168272
	movs r3, #16
	str r3, [sp, #0]
	movs r3, #17
	str r3, [sp, #4]
	ldr r3, [sp, #88]
	movs r4, #206
	lsls r4, r4, #7
	adds r1, r3, r4
	ldr r0, [sp, #92]
	movs r2, #52
	movs r3, #51
	ldr r5, [sp, #76]
	mov lr, r5
	.2byte 0xf800
.L_08168272:
	ldr r3, [sp, #72]
	subs r3, #66
	cmp r3, #1
	bhi .L_08168298
	ldr r6, [sp, #88]
	movs r2, #208
	movs r3, #24
	lsls r2, r2, #7
	adds r2, #16
	str r3, [sp, #0]
	movs r3, #41
	str r3, [sp, #4]
	adds r1, r6, r2
	ldr r0, [sp, #92]
	movs r2, #48
	movs r3, #40
	ldr r4, [sp, #76]
	mov lr, r4
	.2byte 0xf800
.L_08168298:
	ldr r3, [sp, #72]
	subs r3, #68
	cmp r3, #7
	bhi .L_08168320
	ldr r5, [sp, #72]
	movs r6, #76
	subs r6, r6, r5
	ldr r0, [sp, #88]
	lsrs r5, r6, #31
	movs r2, #38
	movs r1, #214
	mov r10, r2
	adds r5, r6, r5
	ldr r2, [sp, #76]
	lsls r1, r1, #7
	adds r1, #232
	asrs r5, r5, #1
	movs r4, #49
	adds r7, r0, r1
	subs r4, r4, r5
	movs r1, #44
	mov r3, r10
	movs r0, #22
	subs r3, r3, r6
	str r0, [sp, #0]
	mov r8, r1
	str r1, [sp, #4]
	mov r11, r2
	str r4, [sp, #8]
	ldr r0, [sp, #92]
	adds r2, r4, #0
	adds r1, r7, #0
	mov r10, r3
	mov lr, r11
	.2byte 0xf800
	ldr r4, [sp, #8]
	adds r6, #38
	movs r3, #22
	mov r0, r8
	adds r2, r4, #0
	str r3, [sp, #0]
	str r0, [sp, #4]
	adds r1, r7, #0
	ldr r0, [sp, #92]
	adds r3, r6, #0
	mov lr, r11
	.2byte 0xf800
	adds r5, #49
	movs r1, #22
	mov r2, r8
	str r1, [sp, #0]
	str r2, [sp, #4]
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r10
	ldr r0, [sp, #92]
	mov lr, r11
	.2byte 0xf800
	movs r3, #22
	mov r4, r8
	str r3, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #92]
	adds r1, r7, #0
	adds r2, r5, #0
	adds r3, r6, #0
	mov lr, r11
	.2byte 0xf800
.L_08168320:
	ldr r3, [sp, #72]
	subs r3, #78
	cmp r3, #1
	bhi .L_08168344
	ldr r5, [sp, #88]
	movs r3, #16
	movs r6, #206
	str r3, [sp, #0]
	lsls r6, r6, #7
	movs r3, #17
	str r3, [sp, #4]
	ldr r0, [sp, #92]
	adds r1, r5, r6
	movs r2, #52
	movs r3, #51
	ldr r4, [sp, #76]
	mov lr, r4
	.2byte 0xf800
.L_08168344:
	ldr r3, [sp, #72]
	subs r3, #80
	cmp r3, #1
	bhi .L_0816836a
	ldr r5, [sp, #88]
	movs r6, #208
	movs r3, #24
	lsls r6, r6, #7
	str r3, [sp, #0]
	adds r6, #16
	movs r3, #41
	str r3, [sp, #4]
	ldr r0, [sp, #92]
	adds r1, r5, r6
	movs r2, #48
	movs r3, #40
	ldr r4, [sp, #76]
	mov lr, r4
	.2byte 0xf800
.L_0816836a:
	ldr r3, [sp, #72]
	subs r3, #82
	cmp r3, #3
	bhi .L_081683fa
	ldr r5, [sp, #72]
	ldr r6, [sp, #88]
	movs r0, #214
	lsls r7, r5, #1
	lsls r0, r0, #7
	adds r2, r7, #0
	adds r0, #232
	subs r2, #164
	adds r6, r6, r0
	mov r9, r6
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08168390
	adds r3, r7, #0
	subs r3, #161
.L_08168390:
	movs r4, #38
	subs r4, r4, r2
	ldr r2, [sp, #76]
	asrs r3, r3, #2
	movs r5, #49
	subs r5, r5, r3
	movs r6, #44
	movs r1, #22
	str r3, [sp, #20]
	mov r8, r1
	str r1, [sp, #0]
	mov r10, r2
	adds r3, r4, #0
	str r4, [sp, #8]
	str r6, [sp, #4]
	ldr r0, [sp, #92]
	adds r2, r5, #0
	mov r1, r9
	mov lr, r10
	.2byte 0xf800
	subs r7, #126
	mov r11, r7
	mov r3, r8
	str r3, [sp, #0]
	adds r2, r5, #0
	str r6, [sp, #4]
	ldr r0, [sp, #92]
	mov r1, r9
	mov r3, r11
	mov lr, r10
	.2byte 0xf800
	ldr r5, [sp, #20]
	ldr r4, [sp, #8]
	adds r5, #49
	mov r0, r8
	str r0, [sp, #0]
	mov r1, r9
	adds r2, r5, #0
	adds r3, r4, #0
	str r5, [sp, #16]
	str r6, [sp, #4]
	ldr r0, [sp, #92]
	mov lr, r10
	.2byte 0xf800
	mov r1, r8
	str r1, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #92]
	mov r1, r9
	ldr r2, [sp, #16]
	mov r3, r11
	mov lr, r10
	.2byte 0xf800
.L_081683fa:
	ldr r3, [sp, #72]
	subs r3, #72
	cmp r3, #15
	bhi .L_08168490
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, [sp, #72]
	ldr r4, .L_081684b4
	lsls r3, r2, #13
	adds r7, r3, r4
	ldr r2, .L_081684b8
	ldr r3, [sp, #108]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_081684bc
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #108]
	ldr r3, .L_081684c0
	add r2, sp, #108
	str r3, [r2, #4]
	ldr r3, .L_081684c4
	adds r5, r0, #0
	str r1, [r5]
	str r2, [r5, #16]
	str r3, [r5, #8]
	str r6, [r5, #12]
	ldr r1, [sp, #72]
	movs r0, #0
	cmp r1, #79
	ble .L_08168452
	lsls r3, r1, #3
	movs r2, #160
	negs r3, r3
	lsls r2, r2, #2
	adds r0, r3, r2
.L_08168452:
	str r0, [r5, #20]
	bl Func_08014de4
	movs r1, #128
	lsls r1, r1, #12
	movs r2, #0
	ldr r0, .L_081684c8
	bl Func_08015160
	adds r0, r7, #0
	bl Func_0801521c
	movs r0, #152
	lsls r0, r0, #5
	adds r0, #136
	bl SceneTransform_ApplyPitch
	adds r1, r6, #0
	movs r2, #4
	ldr r0, .L_081684cc
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r6, #0
	bl Sys_Free
.L_08168490:
	ldr r3, [sp, #72]
	cmp r3, #85
	bgt .L_08168498
	b .L_081686a0
.L_08168498:
	cmp r3, #213
	ble .L_0816849e
	b .L_081686a0
.L_0816849e:
	ldr r4, [sp, #28]
	movs r7, #0
	cmp r4, #0
	bne .L_081684a8
	b .L_081686a0
.L_081684a8:
	b .L_081684d0
	.2byte 0x0000
.L_081684ac:
	.4byte 0x3f3f3f3f
.L_081684b0:
	.4byte IwramFillWords
.L_081684b4:
	.4byte 0xfff70800
.L_081684b8:
	.4byte 0xffffff00
.L_081684bc:
	.4byte 0xffff00ff
.L_081684c0:
	.4byte Data_02014000
.L_081684c4:
	.4byte Data_08199364
.L_081684c8:
	.4byte 0xfffc0000
.L_081684cc:
	.4byte Data_08199210
.L_081684d0:
	ldr r5, [sp, #32]
	movs r6, #0
	mov lr, r5
	mov r10, r6
	mov r8, r5
	cmp r5, #0
	bge .L_081684e0
	b .L_08168690
.L_081684e0:
	movs r0, #224
	lsls r0, r0, #3
	mov r9, r0
.L_081684e6:
	mov r1, lr
	lsrs r3, r1, #31
	add r3, lr
	asrs r3, r3, #1
	mov r11, r3
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081684f8
	adds r3, #7
.L_081684f8:
	ldr r4, [sp, #44]
	ldr r5, [sp, #44]
	ldr r6, [sp, #36]
	asrs r3, r3, #3
	adds r0, r4, r3
	subs r4, r5, r3
	mov r3, r10
	muls r3, r6
	cmp r3, #0
	bge .L_0816850e
	adds r3, #7
.L_0816850e:
	ldr r1, [sp, #40]
	asrs r3, r3, #3
	mov r5, r10
	adds r2, r1, r3
	lsls r3, r5, #3
	cmp r3, #0
	bge .L_0816851e
	adds r3, #7
.L_0816851e:
	ldr r6, [sp, #40]
	asrs r3, r3, #3
	subs r1, r6, r3
	cmp r1, #0
	bge .L_0816852a
	movs r1, #0
.L_0816852a:
	cmp r2, #119
	ble .L_08168530
	movs r2, #119
.L_08168530:
	cmp r4, #0
	bge .L_08168536
	movs r4, #0
.L_08168536:
	cmp r0, #119
	ble .L_0816853c
	movs r0, #119
.L_0816853c:
	lsls r3, r2, #4
	subs r3, r3, r2
	ldr r6, [sp, #88]
	lsls r5, r3, #3
	adds r3, r5, r0
	movs r2, #20
	add r3, r9
	strb r2, [r6, r3]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r1, r3, #3
	adds r3, r1, r0
	add r3, r9
	strb r2, [r6, r3]
	adds r3, r5, r4
	add r3, r9
	strb r2, [r6, r3]
	adds r3, r1, r4
	add r3, r9
	strb r2, [r6, r3]
	ldr r3, [sp, #44]
	ldr r2, [sp, #44]
	add r3, r11
	mov r4, r11
	adds r0, r3, #1
	subs r3, r2, r4
	adds r4, r3, #1
	cmp r4, #0
	bge .L_08168578
	movs r4, #0
.L_08168578:
	cmp r0, #119
	ble .L_0816857e
	movs r0, #119
.L_0816857e:
	movs r6, #224
	adds r3, r5, r0
	lsls r6, r6, #3
	adds r3, r3, r6
	ldr r6, [sp, #88]
	movs r2, #20
	strb r2, [r6, r3]
	adds r3, r1, r0
	movs r0, #224
	lsls r0, r0, #3
	adds r3, r3, r0
	strb r2, [r6, r3]
	adds r3, r5, r4
	adds r3, r3, r0
	strb r2, [r6, r3]
	adds r3, r1, r4
	adds r3, r3, r0
	mov r1, r10
	strb r2, [r6, r3]
	lsrs r3, r1, #31
	add r3, r10
	asrs r3, r3, #1
	mov r12, r3
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_081685b4
	adds r3, #7
.L_081685b4:
	ldr r4, [sp, #44]
	ldr r5, [sp, #44]
	ldr r6, [sp, #36]
	asrs r3, r3, #3
	adds r0, r4, r3
	subs r4, r5, r3
	mov r3, lr
	muls r3, r6
	cmp r3, #0
	bge .L_081685ca
	adds r3, #7
.L_081685ca:
	ldr r1, [sp, #40]
	asrs r3, r3, #3
	mov r5, lr
	adds r2, r1, r3
	lsls r3, r5, #3
	cmp r3, #0
	bge .L_081685da
	adds r3, #7
.L_081685da:
	ldr r6, [sp, #40]
	asrs r3, r3, #3
	subs r1, r6, r3
	cmp r4, #0
	bge .L_081685e6
	movs r4, #0
.L_081685e6:
	cmp r0, #119
	ble .L_081685ec
	movs r0, #119
.L_081685ec:
	cmp r1, #0
	bge .L_081685f2
	movs r1, #0
.L_081685f2:
	cmp r2, #119
	ble .L_081685f8
	movs r2, #119
.L_081685f8:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r6, r3, #3
	movs r2, #224
	adds r3, r6, r0
	lsls r2, r2, #3
	adds r3, r3, r2
	ldr r2, [sp, #88]
	movs r5, #20
	strb r5, [r2, r3]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #3
	ldr r1, [sp, #88]
	adds r3, r2, r0
	movs r0, #224
	lsls r0, r0, #3
	adds r3, r3, r0
	strb r5, [r1, r3]
	adds r3, r6, r4
	adds r3, r3, r0
	strb r5, [r1, r3]
	adds r3, r2, r4
	adds r3, r3, r0
	strb r5, [r1, r3]
	ldr r3, [sp, #44]
	ldr r4, [sp, #44]
	add r3, r12
	mov r1, r12
	adds r0, r3, #1
	subs r3, r4, r1
	adds r4, r3, #1
	cmp r4, #0
	bge .L_0816863e
	movs r4, #0
.L_0816863e:
	cmp r0, #119
	ble .L_08168644
	movs r0, #119
.L_08168644:
	movs r1, #224
	adds r3, r6, r0
	lsls r1, r1, #3
	adds r3, r3, r1
	ldr r1, [sp, #88]
	strb r5, [r1, r3]
	adds r3, r2, r0
	movs r0, #224
	lsls r0, r0, #3
	adds r3, r3, r0
	strb r5, [r1, r3]
	adds r3, r6, r4
	adds r3, r3, r0
	strb r5, [r1, r3]
	adds r3, r2, r4
	adds r3, r3, r0
	strb r5, [r1, r3]
	mov r1, r10
	mov r2, r8
	lsls r3, r1, #1
	subs r3, r2, r3
	subs r3, #1
	mov r8, r3
	cmp r3, #0
	bge .L_08168686
	mov r4, lr
	lsls r3, r4, #1
	add r3, r8
	movs r5, #1
	subs r3, #2
	negs r5, r5
	mov r8, r3
	add lr, r5
.L_08168686:
	movs r6, #1
	add r10, r6
	cmp lr, r10
	blt .L_08168690
	b .L_081684e6
.L_08168690:
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	adds r0, #1
	adds r7, #1
	str r0, [sp, #32]
	cmp r7, r1
	beq .L_081686a0
	b .L_081684d0
.L_081686a0:
	ldr r2, [sp, #72]
	subs r2, #86
	mov r8, r2
	cmp r2, #63
	bhi .L_08168720
	ldr r3, [sp, #72]
	ldr r4, [sp, #72]
	subs r3, #70
	mov r9, r3
	lsrs r3, r4, #31
	adds r3, r4, r3
	ldr r6, .L_08168a28
	asrs r3, r3, #1
	movs r7, #0
	mov r10, r3
.L_081686be:
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	adds r0, r6, #0
	asrs r5, r3, #16
	bl Trig_Cos
	mov r3, r9
	muls r3, r0
	mov r1, r10
	adds r2, r1, r7
	asrs r0, r3, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_081686e2
	adds r3, r2, #3
.L_081686e2:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	ldr r2, [sp, #88]
	lsls r1, r1, #2
	subs r1, r1, r3
	lsls r1, r1, #6
	movs r3, #142
	adds r1, r2, r1
	lsls r3, r3, #7
	adds r1, r1, r3
	adds r3, r0, #0
	movs r0, #32
	adds r2, r5, #0
	str r0, [sp, #0]
	movs r5, #128
	movs r0, #54
	str r0, [sp, #4]
	adds r2, #44
	adds r3, #17
	ldr r0, [sp, #92]
	ldr r4, [sp, #76]
	lsls r5, r5, #5
	adds r7, #1
	mov lr, r4
	.2byte 0xf800
	adds r6, r6, r5
	cmp r7, #9
	bne .L_081686be
.L_08168720:
	movs r0, #188
	movs r1, #3
	bl Func_081963ec
	movs r6, #192
	lsls r6, r6, #18
	adds r6, #188
	ldr r4, [r6]
	ldr r0, [sp, #72]
	str r4, [sp, #80]
	cmp r0, #85
	ble .L_08168750
	movs r3, #120
	ldr r2, [sp, #88]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, [sp, #92]
	movs r2, #0
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
.L_08168750:
	ldr r4, [sp, #72]
	movs r5, #24
	adds r5, #255
	cmp r4, r5
	ble .L_08168760
	ldr r6, [sp, #24]
	subs r6, #8
	str r6, [sp, #24]
.L_08168760:
	ldr r0, [sp, #72]
	cmp r0, #238
	bne .L_08168774
	movs r1, #128
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	ldr r2, .L_08168a2c
	ldr r3, .L_08168a30
	mov lr, r3
	.2byte 0xf800
.L_08168774:
	ldr r3, [sp, #72]
	subs r3, #214
	cmp r3, #65
	bhi .L_08168784
	ldr r2, .L_08168a34
	ldrh r3, [r2, #4]
	adds r3, #8
	strh r3, [r2, #4]
.L_08168784:
	ldr r3, [sp, #72]
	subs r3, #246
	cmp r3, #33
	bhi .L_081687aa
	ldr r4, [sp, #24]
	ldr r5, [sp, #88]
	movs r6, #239
	movs r1, #238
	lsls r6, r6, #7
	lsls r1, r1, #7
	adds r3, r5, r6
	adds r4, #8
	movs r0, #0
	adds r1, #132
	str r4, [sp, #24]
	adds r2, r5, r1
	str r0, [r3]
	movs r3, #75
	str r3, [r2]
.L_081687aa:
	ldr r2, [sp, #72]
	cmp r2, #213
	bgt .L_081687b2
	b .L_081689ae
.L_081687b2:
	add r3, sp, #132
	movs r4, #0
	str r4, [r3, #12]
	str r4, [r3, #4]
	ldr r0, [sp, #88]
	movs r1, #238
	movs r5, #100
	lsls r1, r1, #7
	add r5, sp
	adds r1, #220
	mov r8, r5
	movs r7, #0
	adds r4, r3, #0
	adds r6, r0, r1
	adds r5, r0, #0
.L_081687d0:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_0816887e
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	asrs r2, r2, #7
	adds r3, r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #255
	str r3, [sp, #100]
	cmp r3, r2
	bgt .L_081687f0
	movs r3, #128
	lsls r3, r3, #4
	str r3, [sp, #100]
.L_081687f0:
	mov r0, r8
	str r3, [r0, #4]
	movs r2, #255
	ldr r3, [r5]
	lsls r2, r2, #16
	str r2, [r4, #4]
	str r3, [r4]
	adds r1, r4, #0
	ldr r3, [r5, #4]
	ldr r0, [r6]
	adds r3, r3, r2
	str r3, [r4, #8]
	mov r2, r8
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	ldr r2, [r5, #4]
	ldr r3, [r5, #16]
	ldr r1, .L_08168a38
	subs r2, r2, r3
	str r2, [r5, #4]
	ldr r4, [sp, #8]
	cmp r2, r1
	bgt .L_0816887e
	ldr r2, [sp, #72]
	movs r3, #24
	adds r3, #255
	cmp r2, r3
	bgt .L_08168842
	bl Random16
	movs r3, #63
	ands r3, r0
	ldr r0, [sp, #24]
	ldr r4, [sp, #8]
	adds r3, r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	b .L_08168858
.L_08168842:
	str r4, [sp, #8]
	bl Random16
	ldr r1, [sp, #24]
	movs r3, #63
	ands r3, r0
	adds r3, r3, r1
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	ldr r4, [sp, #8]
.L_08168858:
	ldr r2, [sp, #72]
	movs r3, #46
	adds r3, #255
	cmp r2, r3
	ble .L_08168868
	movs r0, #0
	str r0, [r5, #24]
	b .L_0816887e
.L_08168868:
	ldr r1, [sp, #72]
	cmp r1, #245
	ble .L_08168874
	movs r3, #192
	lsls r3, r3, #15
	b .L_0816887c
.L_08168874:
	ldr r2, [r5]
	movs r3, #192
	lsls r3, r3, #16
	subs r3, r3, r2
.L_0816887c:
	str r3, [r5, #4]
.L_0816887e:
	adds r7, #1
	adds r6, #4
	adds r5, #28
	cmp r7, #16
	bne .L_081687d0
	ldr r2, [sp, #72]
	movs r3, #24
	adds r3, #255
	cmp r2, r3
	bgt .L_08168920
	ldr r5, [sp, #24]
	asrs r2, r2, #31
	lsrs r3, r5, #31
	adds r3, r5, r3
	movs r4, #15
	asrs r3, r3, #1
	movs r7, #0
	mov r8, r2
	mov r10, r4
	mov r9, r3
.L_081688a6:
	bl Random16
	movs r2, #1
	ands r2, r7
	lsls r3, r2, #2
	lsrs r5, r7, #31
	adds r3, r3, r2
	mov r6, r10
	adds r5, r7, r5
	ands r0, r6
	asrs r5, r5, #1
	lsls r3, r3, #2
	lsls r2, r5, #2
	subs r3, r3, r0
	adds r3, r3, r2
	add r3, r9
	adds r6, r3, #0
	bl Random16
	ldr r4, [sp, #72]
	mov r2, r8
	lsrs r3, r2, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	mov r1, r10
	lsls r5, r5, #5
	ands r0, r1
	adds r2, r3, r7
	subs r6, #16
	subs r5, r5, r0
	adds r3, r2, #0
	cmp r2, #0
	bge .L_081688ea
	adds r3, r2, #3
.L_081688ea:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	ldr r0, [sp, #88]
	lsls r1, r1, #2
	subs r1, r1, r3
	lsls r1, r1, #6
	movs r3, #32
	movs r2, #142
	adds r1, r0, r1
	lsls r2, r2, #7
	str r3, [sp, #0]
	movs r3, #54
	adds r1, r1, r2
	str r3, [sp, #4]
	ldr r0, [sp, #92]
	adds r2, r6, #0
	adds r3, r5, #0
	ldr r4, [sp, #80]
	adds r7, #1
	mov lr, r4
	.2byte 0xf800
	cmp r7, #8
	bne .L_081688a6
	b .L_081689ae
.L_08168920:
	ldr r0, [sp, #24]
	ldr r5, [sp, #72]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r5, r5, #31
	movs r6, #15
	asrs r3, r3, #1
	movs r7, #0
	mov r8, r5
	mov r10, r6
	mov r9, r3
.L_08168936:
	bl Random16
	movs r2, #1
	ands r2, r7
	lsls r3, r2, #2
	lsrs r5, r7, #31
	adds r3, r3, r2
	mov r1, r10
	adds r5, r7, r5
	ands r0, r1
	asrs r5, r5, #1
	lsls r3, r3, #2
	lsls r2, r5, #2
	adds r3, r3, r0
	subs r3, r3, r2
	add r3, r9
	adds r6, r3, #0
	bl Random16
	mov r2, r10
	ands r0, r2
	lsls r5, r5, #5
	subs r5, r5, r0
	ldr r0, [sp, #72]
	mov r4, r8
	lsrs r3, r4, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r2, r3, r7
	subs r6, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0816897a
	adds r3, r2, #3
.L_0816897a:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	ldr r2, [sp, #88]
	lsls r1, r1, #2
	subs r1, r1, r3
	lsls r1, r1, #6
	movs r3, #142
	adds r1, r2, r1
	lsls r3, r3, #7
	adds r1, r1, r3
	movs r3, #32
	str r3, [sp, #0]
	movs r3, #54
	str r3, [sp, #4]
	ldr r0, [sp, #92]
	adds r2, r6, #0
	adds r3, r5, #0
	ldr r4, [sp, #80]
	adds r7, #1
	mov lr, r4
	.2byte 0xf800
	cmp r7, #8
	bne .L_08168936
.L_081689ae:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #72]
	cmp r5, #63
	ble .L_081689c8
	bl Random16
	movs r3, #3
	ldr r2, .L_08168a34
	ands r3, r0
	adds r3, #30
	strh r3, [r2, #6]
.L_081689c8:
	bl Func_081434f8
	movs r0, #240
	ldr r6, [sp, #88]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r6, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #72]
	movs r2, #168
	adds r1, #1
	lsls r2, r2, #1
	str r1, [sp, #72]
	cmp r1, r2
	beq .L_081689f2
	bl .L_08167d18
.L_081689f2:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	movs r7, #0
	adds r5, r6, r3
.L_081689fc:
	ldmia r5!, {r0}
	adds r7, #1
	bl ResourceObject_ReleaseFar
	cmp r7, #16
	bne .L_081689fc
	ldr r0, .L_08168a3c
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #188
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08168a28:
	.4byte 0xffffc000
.L_08168a2c:
	.4byte 0x3f3f3f3f
.L_08168a30:
	.4byte IwramFillWords
.L_08168a34:
	.4byte Data_03001120
.L_08168a38:
	.4byte 0x000fffff
.L_08168a3c:
	.4byte Func_08143000
