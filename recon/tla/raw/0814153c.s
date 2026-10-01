.syntax unified
	.thumb
	.global Func_0814153c
	.thumb_func
Func_0814153c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #184
	str r0, [sp, #104]
	movs r2, #192
	lsls r2, r2, #18
	ldr r0, [r2, #96]
	ldr r3, .L_0814178c
	str r0, [sp, #100]
	movs r0, #0
	ldr r1, [r2, #92]
	movs r7, #239
	str r1, [sp, #96]
	lsls r7, r7, #7
	ldrh r3, [r3, #4]
	str r3, [sp, #80]
	adds r3, r2, #0
	adds r3, #176
	ldr r3, [r3]
	str r3, [sp, #76]
	ldr r3, [r2, #100]
	str r3, [sp, #72]
	ldr r2, [r2, #36]
	str r2, [sp, #68]
	bl BattleFx_BeginCanvasLayer
	bl Func_08179e6c
	ldr r4, [sp, #96]
	movs r3, #0
	adds r2, r4, r7
	movs r1, #200
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_08141790
	bl Scheduler_AddOrUpdateCallback
	movs r0, #80
	negs r0, r0
	movs r1, #0
	str r0, [sp, #64]
	str r1, [sp, #84]
.L_0814159a:
	ldr r2, [sp, #84]
	cmp r2, #27
	beq .L_081415a2
	b .L_081416ee
.L_081415a2:
	add r0, sp, #156
	movs r3, #255
	movs r1, #0
	strh r3, [r0]
	bl BattleActor_SpawnObjectsForListFar
	ldr r3, [sp, #68]
	movs r7, #238
	ldr r0, [r3, #84]
	bl Resource_ResetEntry
	bl Func_08020380
	ldr r2, .L_08141794
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	ldr r1, .L_08141798
	movs r2, #0
	bl Func_08118040
	ldr r4, [sp, #96]
	lsls r7, r7, #7
	movs r1, #238
	adds r7, #144
	lsls r1, r1, #7
	adds r3, r4, r7
	movs r0, #0
	adds r1, #148
	str r0, [r3]
	movs r2, #1
	adds r3, r4, r1
	str r2, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #152
	adds r2, r4, r3
	movs r3, #1
	negs r3, r3
	adds r7, #12
	str r3, [r2]
	movs r1, #144
	adds r3, r4, r7
	str r0, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0814179c
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #76]
	movs r0, #1
	str r0, [r1, #16]
	ldr r0, .L_081417a0
	bl Resource_GetTableEntry
	movs r2, #240
	adds r5, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #1
	ldr r3, .L_081417a4
	ldr r0, .L_081417a8
	mov lr, r3
	.2byte 0xf800
	movs r4, #240
	lsls r4, r4, #1
	adds r5, r5, r4
	adds r0, r5, #0
	ldr r1, .L_081417ac
	bl Resource_DecodeType01
	ldr r0, .L_081417b0
	ldr r1, .L_081417a4
	ldr r2, [sp, #96]
	movs r3, #238
	lsls r3, r3, #7
	movs r4, #13
	ldr r6, .L_081417ac
	movs r7, #0
	adds r3, #220
	negs r4, r4
	mov r8, r7
	mov r11, r0
	mov r9, r1
	adds r5, r2, r3
	mov r10, r4
.L_0814164a:
	movs r1, #32
	ldr r2, .L_081417b4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	mov r7, r10
	ands r3, r7
	movs r7, #8
	orrs r3, r7
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r11
	ldrh r0, [r3, #2]
	ldr r1, .L_081417b8
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #3
	adds r1, r6, #0
	mov lr, r9
	.2byte 0xf800
	movs r3, #1
	movs r2, #128
	add r8, r3
	lsls r2, r2, #3
	mov r4, r8
	adds r6, r6, r2
	cmp r4, #31
	bne .L_0814164a
	ldr r0, .L_081417bc
	bl Resource_GetTableEntry
	movs r2, #240
	adds r5, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #1
	ldr r3, .L_081417a4
	ldr r0, .L_081417a8
	mov lr, r3
	.2byte 0xf800
	movs r4, #240
	lsls r4, r4, #1
	adds r5, r5, r4
	ldr r1, .L_081417ac
	adds r0, r5, #0
	bl Resource_DecodeType01
	movs r1, #32
	ldr r2, .L_081417b4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	movs r2, #240
	ldr r1, [sp, #96]
	lsls r2, r2, #7
	adds r2, #88
	adds r3, r1, r2
	str r0, [r3]
	movs r4, #13
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	orrs r3, r7
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	ldr r2, .L_081417b0
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r0, [r3, #2]
	ldr r7, .L_081417b8
	movs r2, #128
	adds r0, r0, r7
	ldr r1, .L_081417ac
	lsls r2, r2, #3
	ldr r3, .L_081417a4
	mov lr, r3
	.2byte 0xf800
.L_081416ee:
	ldr r4, [sp, #64]
	movs r6, #128
	adds r4, #4
	lsls r6, r6, #19
	lsls r3, r4, #8
	adds r6, #40
	str r4, [sp, #64]
	str r3, [r6]
	ldr r7, [sp, #96]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r7, r0
	movs r1, #1
	str r1, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #84]
	adds r2, #1
	str r2, [sp, #84]
	cmp r2, #52
	beq .L_0814171e
	b .L_0814159a
.L_0814171e:
	movs r1, #240
	ldr r0, [sp, #100]
	ldr r5, .L_081417c0
	lsls r1, r1, #6
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	lsls r1, r1, #6
	movs r2, #0
	ldr r0, .L_081417c4
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_081417c8
	ldr r1, [sp, #72]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, .L_0814177c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08141780
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08141784
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08141788
	subs r2, #2
	strh r3, [r2]
	movs r3, #0
	str r3, [r6]
	movs r4, #238
	str r3, [sp, #60]
	str r3, [sp, #56]
	str r3, [sp, #52]
	str r3, [sp, #48]
	movs r3, #239
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r7, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	b .L_081417cc
.L_0814177c:
	.4byte 0x00007741
.L_08141780:
	.4byte 0x00000080
.L_08141784:
	.4byte 0x00001010
.L_08141788:
	.4byte 0x00003f44
.L_0814178c:
	.4byte Data_03001120
.L_08141790:
	.4byte Func_08143000
.L_08141794:
	.4byte gCameraSceneParameters
.L_08141798:
	.4byte 0x00000078
.L_0814179c:
	.4byte Func_0813baec
.L_081417a0:
	.4byte 0x000000a4
.L_081417a4:
	.4byte IwramCopyWords
.L_081417a8:
	.4byte 0x05000200
.L_081417ac:
	.4byte gMapCellBuffer
.L_081417b0:
	.4byte ResourceTableEntries
.L_081417b4:
	.4byte 0x80002000
.L_081417b8:
	.4byte 0x06010000
.L_081417bc:
	.4byte 0x000000a3
.L_081417c0:
	.4byte IwramFillWords
.L_081417c4:
	.4byte 0x06004000
.L_081417c8:
	.4byte 0x00000134
.L_081417cc:
	adds r2, r7, r4
	movs r3, #50
	str r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r0, .L_08141b20
	str r3, [sp, #88]
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08141b24
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r7, #0
	str r7, [sp, #84]
	ldr r3, .L_08141b28
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08141808
	b .L_08141f7a
.L_08141808:
	mov r0, sp
	adds r0, #140
	str r0, [sp, #20]
.L_0814180e:
	ldr r1, [sp, #84]
	cmp r1, #0
	bne .L_08141856
	ldr r0, [sp, #96]
	ldr r2, .L_08141b2c
	movs r1, #238
	ldr r7, .L_08141b30
	movs r3, #212
	lsls r1, r1, #7
	lsls r3, r3, #16
	adds r1, #180
	movs r4, #128
	str r2, [sp, #60]
	str r3, [sp, #56]
	adds r2, r0, r1
	lsls r4, r4, #11
	movs r3, #1
	str r4, [sp, #52]
	str r7, [sp, #48]
	str r3, [r2]
	movs r2, #238
	ldr r4, [sp, #84]
	lsls r2, r2, #7
	adds r2, #184
	movs r7, #236
	adds r3, r0, r2
	lsls r7, r7, #1
	str r4, [r3]
	adds r3, r0, r7
	str r4, [r3]
	ldr r0, .L_08141b34
	ldr r1, .L_08141b38
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_08141856:
	ldr r0, [sp, #84]
	cmp r0, #136
	bne .L_081418a6
	ldr r4, .L_08141b3c
	ldr r6, .L_08141b24
	ldr r5, .L_08141b40
	movs r1, #0
	movs r7, #128
	mov r8, r1
	lsls r7, r7, #3
.L_0814186a:
	ldr r3, .L_08141b44
	mov r2, r8
	ldrb r3, [r3, r2]
	movs r0, #238
	ldr r1, [sp, #96]
	lsls r0, r0, #7
	adds r0, #220
	lsls r3, r3, #2
	adds r3, r3, r0
	ldr r3, [r1, r3]
	ldr r2, .L_08141b48
	ldrb r3, [r3, #16]
	adds r1, r5, #0
	lsls r3, r3, #2
	adds r3, r3, r4
	ldrh r0, [r3, #2]
	str r4, [sp, #8]
	adds r0, r0, r2
	adds r2, r7, #0
	mov lr, r6
	.2byte 0xf800
	movs r0, #1
	movs r3, #128
	add r8, r0
	lsls r3, r3, #3
	mov r1, r8
	adds r5, r5, r3
	ldr r4, [sp, #8]
	cmp r1, #7
	bne .L_0814186a
.L_081418a6:
	ldr r2, [sp, #84]
	cmp r2, #48
	bne .L_081418b2
	movs r0, #139
	bl Audio_PlayCue
.L_081418b2:
	ldr r3, [sp, #84]
	cmp r3, #136
	bne .L_081418be
	movs r0, #140
	bl Audio_PlayCue
.L_081418be:
	ldr r4, [sp, #84]
	cmp r4, #172
	bne .L_081418ca
	movs r0, #144
	bl Audio_PlayCue
.L_081418ca:
	movs r7, #0
	mov r8, r7
	ldr r6, .L_08141b4c
	ldr r7, .L_08141b50
	ldr r5, .L_08141b54
.L_081418d4:
	ldrb r3, [r5]
	ldr r0, [sp, #84]
	adds r5, #1
	cmp r0, r3
	bne .L_081418ea
	movs r1, #240
	ldr r2, [r6]
	ldr r0, [sp, #100]
	lsls r1, r1, #6
	mov lr, r7
	.2byte 0xf800
.L_081418ea:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #4
	cmp r2, #17
	bne .L_081418d4
	ldr r3, .L_08141b58
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #132]
	str r4, [sp, #136]
	ldr r4, [sp, #20]
	movs r3, #0
	str r3, [r4, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r4, #4]
	ldr r7, [sp, #60]
	ldr r1, [sp, #56]
	ldr r0, [sp, #52]
	ldr r2, [sp, #48]
	ldr r3, [sp, #84]
	adds r7, r7, r0
	adds r1, r1, r2
	str r7, [sp, #60]
	str r1, [sp, #56]
	cmp r3, #171
	ble .L_0814195e
	cmp r3, #183
	ble .L_08141934
	ldr r4, .L_08141b5c
	movs r7, #128
	lsls r7, r7, #3
	adds r0, r0, r4
	adds r2, r2, r7
	str r0, [sp, #52]
	str r2, [sp, #48]
.L_08141934:
	ldr r0, [sp, #84]
	cmp r0, #205
	bgt .L_081419a4
	ldr r1, [sp, #52]
	movs r2, #54
	adds r3, r1, #0
	muls r3, r2
	cmp r3, #0
	bge .L_08141948
	adds r3, #63
.L_08141948:
	ldr r4, [sp, #48]
	asrs r3, r3, #6
	str r3, [sp, #52]
	adds r3, r4, #0
	muls r3, r2
	cmp r3, #0
	bge .L_08141958
	adds r3, #63
.L_08141958:
	asrs r3, r3, #6
	str r3, [sp, #48]
	b .L_081419a4
.L_0814195e:
	ldr r7, [sp, #48]
	cmp r7, #0
	bge .L_0814196e
	movs r1, #128
	adds r0, r7, #0
	lsls r1, r1, #7
	adds r0, r0, r1
	str r0, [sp, #48]
.L_0814196e:
	ldr r2, [sp, #52]
	cmp r2, #0
	ble .L_0814197a
	ldr r3, .L_08141b60
	adds r2, r2, r3
	str r2, [sp, #52]
.L_0814197a:
	ldr r4, [sp, #84]
	cmp r4, #31
	ble .L_081419a4
	adds r0, r4, #0
	subs r0, #32
	lsls r0, r0, #9
	bl Trig_Sin
	ldr r7, [sp, #56]
	lsls r0, r0, #2
	asrs r0, r0, #4
	adds r7, r7, r0
	ldr r0, [sp, #84]
	str r7, [sp, #56]
	cmp r0, #159
	bgt .L_081419a4
	ldr r1, [sp, #60]
	movs r2, #128
	lsls r2, r2, #7
	adds r1, r1, r2
	str r1, [sp, #60]
.L_081419a4:
	ldr r4, [sp, #96]
	movs r7, #238
	lsls r7, r7, #7
	ldr r5, [sp, #20]
	movs r3, #0
	adds r7, #220
	mov r8, r3
	adds r6, r4, r7
.L_081419b4:
	mov r0, r8
	cmp r0, #25
	bne .L_081419d4
	ldr r1, [sp, #84]
	cmp r1, #135
	ble .L_081419d4
	ldr r2, [sp, #60]
	movs r4, #128
	lsls r4, r4, #14
	adds r3, r2, r4
	str r3, [r5]
	ldr r7, [sp, #56]
	movs r0, #128
	lsls r0, r0, #15
	adds r3, r7, r0
	b .L_081419ec
.L_081419d4:
	ldr r3, .L_08141b64
	mov r1, r8
	ldrb r3, [r3, r1]
	ldr r2, [sp, #60]
	lsls r3, r3, #16
	adds r3, r2, r3
	str r3, [r5]
	ldr r3, .L_08141b68
	ldr r4, [sp, #56]
	ldrb r3, [r3, r1]
	lsls r3, r3, #16
	adds r3, r4, r3
.L_081419ec:
	str r3, [r5, #8]
	ldr r7, [sp, #20]
	ldr r0, .L_08141b6c
	ldr r3, [r7, #8]
	cmp r3, r0
	bgt .L_08141a04
	ldr r0, [r6]
	adds r1, r7, #0
	add r2, sp, #132
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08141a04:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #4
	cmp r2, #32
	bne .L_081419b4
	ldr r3, [sp, #84]
	cmp r3, #136
	bne .L_08141a30
	ldr r4, [sp, #96]
	movs r7, #238
	lsls r7, r7, #7
	movs r0, #238
	adds r7, #180
	lsls r0, r0, #7
	adds r2, r4, r7
	movs r3, #24
	adds r0, #184
	str r3, [r2]
	adds r2, r4, r0
	movs r3, #0
	str r3, [r2]
.L_08141a30:
	ldr r1, [sp, #60]
	ldr r2, [sp, #96]
	movs r4, #236
	lsls r4, r4, #1
	lsrs r3, r1, #31
	adds r5, r2, r4
	adds r3, r1, r3
	ldr r1, [r5]
	asrs r3, r3, #1
	lsls r2, r1, #13
	movs r7, #224
	subs r3, r3, r2
	lsls r7, r7, #13
	ldr r0, [sp, #56]
	adds r7, r3, r7
	ldr r3, [sp, #84]
	lsls r1, r1, #14
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #14
	adds r2, r1, r2
	subs r3, #136
	str r7, [sp, #44]
	str r2, [sp, #40]
	str r3, [sp, #36]
	cmp r3, #84
	bls .L_08141a68
	b .L_08141ba2
.L_08141a68:
	ldr r4, [sp, #84]
	cmp r4, #136
	bne .L_08141a98
	movs r3, #4
	str r3, [r5]
	ldr r7, [sp, #96]
	movs r0, #224
	lsls r0, r0, #1
	movs r1, #228
	adds r3, r7, r0
	movs r2, #0
	lsls r1, r1, #1
	str r2, [r3]
	adds r3, r7, r1
	str r2, [r3]
	movs r2, #224
	ldr r1, [r5]
	lsls r2, r2, #3
	adds r0, r7, r2
	movs r2, #128
	lsls r1, r1, #1
	lsls r2, r2, #9
	bl Func_0815b434
.L_08141a98:
	ldr r3, [sp, #84]
	cmp r3, #171
	ble .L_08141aea
	cmp r3, #172
	bne .L_08141ac2
	movs r3, #64
	str r3, [r5]
	ldr r4, [sp, #96]
	movs r7, #224
	lsls r7, r7, #3
	movs r2, #128
	adds r0, r4, r7
	movs r1, #128
	lsls r2, r2, #9
	bl Func_0815b434
	ldr r1, .L_08141b70
	movs r0, #192
	lsls r0, r0, #10
	str r0, [sp, #52]
	str r1, [sp, #48]
.L_08141ac2:
	ldr r2, [sp, #84]
	cmp r2, #203
	ble .L_08141b7c
	ldr r3, [r5]
	cmp r3, #1
	ble .L_08141b7c
	subs r3, #4
	str r3, [r5]
	cmp r3, #1
	bgt .L_08141ada
	movs r3, #2
	str r3, [r5]
.L_08141ada:
	ldr r1, [r5]
	ldr r3, [sp, #96]
	movs r4, #224
	lsls r4, r4, #3
	movs r2, #128
	lsls r1, r1, #1
	adds r0, r3, r4
	b .L_08141b18
.L_08141aea:
	ldr r7, [sp, #84]
	cmp r7, #135
	ble .L_08141b7c
	ldr r0, [sp, #52]
	ldr r2, [sp, #48]
	ldr r1, .L_08141b74
	movs r3, #128
	lsls r3, r3, #4
	adds r0, r0, r1
	adds r2, r2, r3
	str r0, [sp, #52]
	str r2, [sp, #48]
	ldr r3, [r5]
	adds r1, r3, #1
	str r1, [r5]
	cmp r1, #32
	bgt .L_08141b78
	ldr r4, [sp, #96]
	movs r7, #224
	lsls r7, r7, #3
	movs r2, #128
	lsls r1, r1, #1
	adds r0, r4, r7
.L_08141b18:
	lsls r2, r2, #9
	bl Func_0815b434
	b .L_08141b7c
.L_08141b20:
	.4byte 0x00000130
.L_08141b24:
	.4byte IwramCopyWords
.L_08141b28:
	.4byte gInput
.L_08141b2c:
	.4byte 0xffd00000
.L_08141b30:
	.4byte 0xfff60000
.L_08141b34:
	.4byte 0x000000b4
.L_08141b38:
	.4byte Data_02014000
.L_08141b3c:
	.4byte ResourceTableEntries
.L_08141b40:
	.4byte Data_02010400
.L_08141b44:
	.4byte Data_08197748
.L_08141b48:
	.4byte 0x06010000
.L_08141b4c:
	.4byte Data_08197760
.L_08141b50:
	.4byte IwramFillWords
.L_08141b54:
	.4byte Data_0819774f
.L_08141b58:
	.4byte Data_08196de0
.L_08141b5c:
	.4byte 0xfffffe00
.L_08141b60:
	.4byte 0xffffe000
.L_08141b64:
	.4byte Data_081977a4
.L_08141b68:
	.4byte Data_081977c4
.L_08141b6c:
	.4byte 0x00c7ffff
.L_08141b70:
	.4byte 0xfffd0000
.L_08141b74:
	.4byte 0xfffff800
.L_08141b78:
	mov r0, r8
	str r0, [r5]
.L_08141b7c:
	ldr r1, [sp, #96]
	movs r2, #236
	lsls r2, r2, #1
	ldr r4, [sp, #44]
	ldr r7, [sp, #40]
	adds r3, r1, r2
	ldr r3, [r3]
	asrs r4, r4, #16
	asrs r7, r7, #16
	movs r2, #224
	lsls r2, r2, #3
	mov r11, r4
	mov r9, r7
	adds r0, r1, r2
	lsls r3, r3, #1
	mov r1, r11
	mov r2, r9
	bl Func_0818caa8
.L_08141ba2:
	ldr r3, [sp, #84]
	cmp r3, #47
	bgt .L_08141baa
	b .L_08141dfe
.L_08141baa:
	cmp r3, #48
	bne .L_08141c16
	ldr r5, [sp, #96]
	movs r4, #0
	mov r8, r4
	movs r6, #0
.L_08141bb6:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	str r3, [r5, #4]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_08141be2
	ldr r3, [r5, #4]
	negs r3, r3
	str r3, [r5, #4]
.L_08141be2:
	movs r7, #1
	add r8, r7
	mov r0, r8
	str r6, [r5, #24]
	subs r6, #8
	adds r5, #28
	cmp r0, #16
	bne .L_08141bb6
	ldr r2, [sp, #96]
	movs r3, #156
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, .L_08141e64
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #96]
	movs r7, #142
	lsls r7, r7, #7
	ldr r0, .L_08141e68
	adds r1, r4, r7
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_08141c16:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	movs r0, #1
	bl Func_081969f8
	movs r2, #0
	adds r6, r0, #0
	str r2, [r6, #20]
	ldr r3, [sp, #124]
	ldr r2, .L_08141e6c
	ldr r0, [sp, #96]
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08141e70
	movs r1, #142
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	lsls r1, r1, #7
	str r3, [sp, #124]
	add r7, sp, #124
	adds r3, r0, r1
	str r3, [r7, #4]
	movs r3, #6
	str r3, [r6]
	ldr r3, .L_08141e74
	mov r2, r9
	str r3, [r6, #8]
	str r7, [r6, #16]
	str r2, [r6, #12]
	ldr r4, .L_08141e78
	movs r0, #128
	ldr r5, [sp, #96]
	movs r3, #0
	lsls r0, r0, #9
	mov r8, r3
	mov r11, r4
	mov r10, r0
.L_08141c6a:
	ldr r3, [r5, #24]
	ldr r2, [r5]
	ldr r1, [r5, #4]
	adds r3, #1
	adds r2, r2, r1
	str r3, [r5, #24]
	str r2, [r5]
	cmp r3, #47
	bhi .L_08141ce6
	bl Func_08014de4
	mov r2, r8
	movs r1, #3
	ldr r4, [sp, #44]
	ands r1, r2
	movs r3, #128
	ldr r2, [sp, #40]
	lsls r3, r3, #10
	lsls r0, r1, #17
	adds r1, #2
	adds r0, r0, r3
	lsls r1, r1, #16
	adds r0, r4, r0
	subs r1, r2, r1
	add r0, r11
	add r1, r11
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	mov r1, r10
	mov r2, r10
	lsls r0, r0, #8
	bl Func_080151e4
	ldr r0, [r5]
	bl Func_080150e4
	ldr r2, [r5, #24]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #170
	adds r0, r2, #0
	muls r0, r3
	bl Trig_Sin
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #128
	adds r0, r0, r3
	lsls r1, r1, #10
	mov r2, r10
	bl Func_080151e4
	ldr r0, .L_08141e7c
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_08141ce6:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r5, #28
	cmp r0, #8
	bne .L_08141c6a
	ldr r1, [sp, #36]
	cmp r1, #35
	bhi .L_08141d80
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #112
	adds r2, r1, #0
	muls r2, r3
	movs r3, #128
	lsls r3, r3, #11
	subs r5, r3, r2
	lsls r3, r1, #2
	adds r2, r3, #0
	movs r3, #16
	subs r2, #64
	negs r3, r3
	cmp r2, r3
	ble .L_08141d1a
	movs r2, #16
	negs r2, r2
.L_08141d1a:
	movs r3, #6
	str r2, [r6, #20]
	add r2, sp, #124
	strb r3, [r7]
	strb r3, [r2, #1]
	movs r3, #7
	ldr r4, .L_08141e80
	str r3, [r6]
	ldr r3, .L_08141e84
	mov r7, r9
	str r4, [r2, #4]
	str r2, [r6, #16]
	str r3, [r6, #8]
	str r7, [r6, #12]
	bl Func_08014de4
	ldr r4, .L_08141e88
	ldr r1, [sp, #44]
	ldr r3, [sp, #40]
	ldr r2, .L_08141e8c
	adds r0, r1, r2
	adds r1, r3, r4
	movs r2, #0
	bl Func_08015160
	adds r1, r5, #0
	adds r2, r5, #0
	asrs r0, r5, #1
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #6
	bl SceneTransform_ApplyPitch
	movs r0, #160
	lsls r0, r0, #6
	bl Func_080150e4
	ldr r7, [sp, #84]
	negs r0, r7
	lsls r0, r0, #10
	bl Func_08015068
	ldr r0, .L_08141e90
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_08141d80:
	ldr r3, [sp, #84]
	subs r3, #172
	cmp r3, #48
	bhi .L_08141df2
	bl Func_08014de4
	ldr r4, .L_08141e88
	ldr r1, [sp, #44]
	ldr r3, [sp, #40]
	ldr r2, .L_08141e8c
	movs r5, #128
	adds r0, r1, r2
	adds r1, r3, r4
	movs r2, #0
	bl Func_08015160
	lsls r5, r5, #9
	movs r0, #128
	lsls r0, r0, #8
	adds r1, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	movs r0, #160
	lsls r0, r0, #6
	bl Func_080150e4
	ldr r7, [sp, #84]
	cmp r7, #203
	ble .L_08141dd4
	adds r3, r7, #0
	subs r3, #204
	movs r0, #128
	lsls r3, r3, #15
	lsls r0, r0, #12
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_080151e4
	b .L_08141de2
.L_08141dd4:
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_080151e4
.L_08141de2:
	ldr r0, .L_08141e94
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_08141df2:
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r9
	bl Sys_Free
.L_08141dfe:
	ldr r3, [sp, #84]
	subs r3, #148
	cmp r3, #71
	bls .L_08141e08
	b .L_08141f48
.L_08141e08:
	ldr r1, [sp, #44]
	ldr r2, [sp, #40]
	movs r0, #0
	asrs r1, r1, #16
	asrs r2, r2, #16
	mov r10, r0
	mov r11, r1
	mov r9, r2
.L_08141e18:
	ldr r7, [sp, #84]
	ldr r4, [sp, #84]
	add r7, r10
	movs r3, #3
	ands r7, r3
	cmp r4, #171
	bgt .L_08141ea0
	ldr r0, [sp, #96]
	movs r1, #236
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r5, [r3]
	bl Random16
	lsls r5, r5, #1
	adds r1, r5, #0
	bl __umodsi3
	ldr r2, .L_08141e98
	asrs r6, r5, #1
	ldrb r3, [r2, r7]
	add r0, r11
	lsrs r3, r3, #1
	subs r0, r0, r6
	subs r0, r0, r3
	mov r8, r0
	bl Random16
	adds r1, r5, #0
	bl __umodsi3
	ldr r3, .L_08141e9c
	add r0, r9
	ldrb r3, [r3, r7]
	subs r0, r0, r6
	lsrs r3, r3, #1
	subs r5, r0, r3
	b .L_08141eea
.L_08141e64:
	.4byte 0x00000192
.L_08141e68:
	.4byte 0x000000c9
.L_08141e6c:
	.4byte 0xffffff00
.L_08141e70:
	.4byte 0xffff00ff
.L_08141e74:
	.4byte Data_08199220
.L_08141e78:
	.4byte 0xffc00000
.L_08141e7c:
	.4byte Data_081991b0
.L_08141e80:
	.4byte Data_02014000
.L_08141e84:
	.4byte Data_08199340
.L_08141e88:
	.4byte 0xffbe0000
.L_08141e8c:
	.4byte 0xffc20000
.L_08141e90:
	.4byte Data_08199210
.L_08141e94:
	.4byte Data_081991c0
.L_08141e98:
	.4byte Data_08197492
.L_08141e9c:
	.4byte Data_08197498
.L_08141ea0:
	bl Random16
	ldr r4, [sp, #96]
	adds r2, r0, #0
	movs r0, #236
	movs r3, #63
	lsls r0, r0, #1
	ands r2, r3
	adds r3, r4, r0
	ldr r6, [r3]
	str r2, [sp, #12]
	bl Random16
	ldr r4, .L_0814204c
	ldr r2, [sp, #12]
	ldrb r3, [r4, r7]
	lsrs r5, r2, #1
	mov r1, r11
	lsrs r3, r3, #1
	subs r5, r1, r5
	adds r1, r6, #0
	subs r5, r5, r3
	bl __umodsi3
	lsrs r3, r6, #31
	adds r6, r6, r3
	ldr r3, .L_08142050
	ldr r2, [sp, #12]
	ldrb r3, [r3, r7]
	adds r5, r5, r0
	asrs r6, r6, #1
	subs r5, r5, r6
	subs r5, #8
	add r2, r9
	lsrs r3, r3, #1
	mov r8, r5
	subs r5, r2, r3
.L_08141eea:
	bl Random16
	ldr r3, .L_08142054
	movs r1, #3
	ands r0, r1
	ldrb r2, [r3, r0]
	movs r3, #3
	orrs r3, r2
	movs r2, #1
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r2, .L_08142058
	lsls r3, r7, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #96]
	ldr r4, .L_0814204c
	movs r3, #156
	adds r1, r2, r1
	lsls r3, r3, #6
	adds r1, r1, r3
	ldrb r3, [r4, r7]
	ldr r0, [sp, #100]
	str r3, [sp, #0]
	ldr r3, .L_08142050
	mov r2, r8
	ldrb r3, [r3, r7]
	movs r7, #192
	lsls r7, r7, #18
	str r3, [sp, #4]
	adds r7, #188
	ldr r4, [r7]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #4
	beq .L_08141f48
	b .L_08141e18
.L_08141f48:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r4, #240
	ldr r3, [sp, #96]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #84]
	adds r7, #1
	str r7, [sp, #84]
	cmp r7, #220
	beq .L_08141f7a
	ldr r3, .L_0814205c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08141f7a
	b .L_0814180e
.L_08141f7a:
	ldr r0, .L_08142060
	bl Scheduler_RemoveCallback
	add r0, sp, #80
	ldr r3, .L_08142064
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r3, #4]
	ldr r1, [sp, #76]
	mov r8, r2
	str r2, [r1, #16]
	ldr r2, [sp, #96]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	adds r5, r2, r3
.L_08141f9a:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r4, #1
	add r8, r4
	mov r7, r8
	cmp r7, #32
	bne .L_08141f9a
	bl Func_08020388
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r1, [sp, #68]
	movs r4, #240
	str r0, [r1, #84]
	ldr r2, [sp, #96]
	lsls r4, r4, #7
	adds r4, #240
	adds r3, r2, r4
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r0, [sp, #104]
	movs r7, #0
	ldr r3, [r0, #20]
	mov r8, r7
	cmp r3, #0
	beq .L_08142008
	movs r6, #1
	negs r6, r6
	movs r5, #36
.L_08141fda:
	ldr r1, [sp, #104]
	adds r3, r6, #0
	ldrsh r0, [r5, r1]
	movs r1, #1
	adds r2, r6, #0
	str r7, [sp, #0]
	bl Func_0814cd48
	ldr r3, [sp, #104]
	movs r1, #0
	ldrsh r0, [r5, r3]
	adds r2, r6, #0
	adds r3, r6, #0
	str r7, [sp, #0]
	bl Func_0814cd48
	ldr r1, [sp, #104]
	movs r0, #1
	ldr r3, [r1, #20]
	add r8, r0
	adds r5, #2
	cmp r8, r3
	bne .L_08141fda
.L_08142008:
	bl Func_08014c4c
	ldr r3, .L_08142048
	ldr r2, .L_08142064
	movs r6, #128
	lsls r6, r6, #19
	strh r3, [r6]
	movs r3, #32
	strh r3, [r2, #6]
	ldr r2, [sp, #68]
	movs r4, #206
	lsls r4, r4, #3
	adds r3, r2, r4
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #24
	bl Func_08118040
	ldr r5, .L_08142068
	movs r1, #128
	ldr r0, [sp, #100]
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0814206c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	b .L_08142070
.L_08142048:
	.4byte 0x00000001
.L_0814204c:
	.4byte Data_08197492
.L_08142050:
	.4byte Data_08197498
.L_08142054:
	.4byte Data_08197744
.L_08142058:
	.4byte Data_08197486
.L_0814205c:
	.4byte gInput
.L_08142060:
	.4byte Func_0813baec
.L_08142064:
	.4byte Data_03001120
.L_08142068:
	.4byte IwramFillWords
.L_0814206c:
	.4byte 0x06004000
.L_08142070:
	movs r0, #1
	bl WaitFrames
	movs r7, #128
	movs r0, #128
	lsls r7, r7, #19
	lsls r0, r0, #5
	adds r7, #82
	adds r0, #16
	mov r8, r0
	mov r10, r7
	mov r1, r8
	mov r2, r10
	strh r1, [r2]
	ldr r3, .L_081420c4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_081420c8
	ldr r2, .L_081420cc
	strh r3, [r6]
	movs r3, #120
	str r3, [r2, #16]
	ldr r3, [sp, #96]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #3
	str r3, [r2]
	ldr r7, [sp, #96]
	movs r0, #238
	ldr r3, .L_081420d0
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	str r3, [r2]
	movs r1, #128
	ldr r0, [sp, #100]
	lsls r1, r1, #7
	ldr r2, .L_081420d4
	b .L_081420d8
.L_081420c4:
	.4byte 0x00003f46
.L_081420c8:
	.4byte 0x00007741
.L_081420cc:
	.4byte gCameraSceneParameters
.L_081420d0:
	.4byte Data_02020202
.L_081420d4:
	.4byte 0x3f3f3f3f
.L_081420d8:
	mov lr, r5
	.2byte 0xf800
	ldr r3, .L_08142118
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r1, [sp, #104]
	ldr r2, .L_0814211c
	ldr r3, [r1, #20]
	adds r4, r1, #0
	lsls r3, r3, #1
	adds r0, r4, #0
	adds r3, #36
	strh r2, [r4, r3]
	adds r0, #36
	movs r1, #0
	bl BattleActor_SpawnObjectsForListFar
	movs r2, #253
	lsls r2, r2, #6
	adds r1, r7, r2
	movs r3, #0
	ldr r0, .L_08142120
	movs r2, #0
	bl Resource_LoadAndDecompress
	mov r3, r8
	mov r4, r10
	strh r3, [r4]
	movs r7, #0
	b .L_08142124
.L_08142118:
	.4byte 0x00000784
.L_0814211c:
	.4byte 0x000000ff
.L_08142120:
	.4byte 0x000000ca
.L_08142124:
	movs r1, #224
	mov r8, r7
	lsls r1, r1, #3
.L_0814212a:
	mov r0, r8
	lsrs r3, r0, #31
	add r3, r8
	asrs r2, r3, #1
	cmp r2, #63
	ble .L_08142138
	movs r2, #63
.L_08142138:
	movs r3, #0
	mov r10, r3
	ldr r3, [sp, #96]
	add r3, r8
	adds r3, r3, r1
.L_08142142:
	movs r4, #1
	add r10, r4
	mov r7, r10
	strb r2, [r3]
	adds r3, #120
	cmp r7, #120
	bne .L_08142142
	add r8, r4
	mov r0, r8
	cmp r0, #120
	bne .L_0814212a
	movs r1, #0
	movs r2, #128
	str r1, [sp, #84]
	lsls r2, r2, #8
	mov r9, r1
	mov r10, r2
.L_08142164:
	ldr r3, [sp, #84]
	cmp r3, #39
	ble .L_08142182
	ldr r4, [sp, #96]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r4, r7
	movs r3, #4
	movs r0, #238
	str r3, [r2]
	lsls r0, r0, #7
	ldr r3, .L_081422f8
	adds r0, #132
	adds r2, r4, r0
	str r3, [r2]
.L_08142182:
	ldr r1, [sp, #84]
	cmp r1, #16
	bne .L_0814218e
	movs r0, #140
	bl Audio_PlayCue
.L_0814218e:
	ldr r2, [sp, #84]
	cmp r2, #18
	bne .L_081421e4
	ldr r3, [sp, #96]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #90
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
	movs r1, #128
	ldr r3, .L_081422fc
	ldr r0, [sp, #100]
	lsls r1, r1, #7
	ldr r2, .L_08142300
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #104]
	movs r7, #0
	ldr r3, [r0, #20]
	mov r8, r7
	cmp r3, #0
	beq .L_081421e4
	movs r6, #4
	movs r5, #36
.L_081421c6:
	ldr r1, [sp, #104]
	mov r3, r8
	ldrsh r0, [r5, r1]
	movs r1, #7
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r4, [sp, #104]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_081421c6
.L_081421e4:
	ldr r7, [sp, #84]
	cmp r7, #109
	bgt .L_081422e2
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08142304
	ldr r3, [sp, #116]
	adds r7, r0, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08142308
	ldr r0, [sp, #96]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	movs r1, #253
	orrs r3, r2
	lsls r1, r1, #6
	str r3, [sp, #116]
	add r2, sp, #116
	adds r3, r0, r1
	str r3, [r2, #4]
	ldr r3, .L_0814230c
	mov r4, r8
	str r3, [r7, #8]
	mov r3, r9
	str r3, [r7, #20]
	movs r3, #6
	str r2, [r7, #16]
	str r4, [r7, #12]
	str r3, [r7]
	ldr r0, [sp, #84]
	cmp r0, #15
	ble .L_081422d6
	ldr r1, [sp, #96]
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #228
	adds r6, r1, r2
	cmp r0, #16
	bne .L_0814224c
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, .L_08142310
	str r3, [r6, #4]
.L_0814224c:
	ldr r3, [sp, #84]
	cmp r3, #19
	bgt .L_08142264
	ldr r3, [r6]
	ldr r4, .L_08142314
	movs r0, #128
	adds r3, r3, r4
	str r3, [r6]
	ldr r3, [r6, #4]
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r6, #4]
.L_08142264:
	ldr r1, [sp, #84]
	cmp r1, #27
	bgt .L_0814228e
	ldr r2, [sp, #96]
	movs r3, #255
	lsls r3, r3, #6
	adds r5, r2, r3
	movs r2, #128
	adds r0, r5, #0
	movs r1, #96
	lsls r2, r2, #9
	bl Func_0815b434
	movs r0, #6
	ldrsh r2, [r6, r0]
	movs r4, #2
	ldrsh r1, [r6, r4]
	adds r0, r5, #0
	movs r3, #96
	bl Func_0818caa8
.L_0814228e:
	bl Func_08014de4
	ldr r0, [r6]
	ldr r1, .L_08142318
	ldr r2, .L_0814231c
	adds r0, r0, r1
	ldr r1, [r6, #4]
	adds r1, r1, r2
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	lsls r1, r1, #9
	mov r2, r10
	mov r0, r10
	bl Func_080151e4
	movs r0, #152
	lsls r0, r0, #8
	bl Func_080150e4
	movs r1, #167
	lsls r1, r1, #10
	ldr r0, .L_08142320
	adds r1, #64
	mov r2, r10
	bl Func_080151e4
	ldr r0, .L_08142324
	mov r1, r8
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081422d6:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
.L_081422e2:
	ldr r3, [sp, #84]
	movs r1, #8
	cmp r3, #130
	ble .L_081422ee
	movs r1, #2
	b .L_08142330
.L_081422ee:
	ldr r4, [sp, #84]
	cmp r4, #120
	ble .L_08142328
	movs r1, #4
	b .L_08142330
.L_081422f8:
	.4byte 0x04040404
.L_081422fc:
	.4byte IwramFillWords
.L_08142300:
	.4byte 0x3f3f3f3f
.L_08142304:
	.4byte 0xffffff00
.L_08142308:
	.4byte 0xffff00ff
.L_0814230c:
	.4byte Data_08199220
.L_08142310:
	.4byte 0xffd80000
.L_08142314:
	.4byte 0xffe00000
.L_08142318:
	.4byte 0xffc10000
.L_0814231c:
	.4byte 0xffc00000
.L_08142320:
	.4byte 0x000dc500
.L_08142324:
	.4byte Data_081991c0
.L_08142328:
	ldr r7, [sp, #84]
	cmp r7, #110
	ble .L_08142330
	movs r1, #6
.L_08142330:
	adds r0, r1, #0
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #96]
	lsls r1, r1, #7
	adds r1, #232
	adds r3, r0, r1
	movs r5, #1
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #84]
	adds r2, #1
	str r2, [sp, #84]
	cmp r2, #48
	beq .L_0814235a
	b .L_08142164
.L_0814235a:
	ldr r4, [sp, #76]
	movs r3, #1
	negs r3, r3
	str r3, [sp, #32]
	ldr r1, .L_08142710
	str r5, [r4, #16]
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	movs r1, #31
	movs r0, #31
	movs r2, #31
	bl Func_08164b2c
	ldr r2, .L_08142714
	movs r3, #0
	strh r3, [r2, #4]
	ldr r2, .L_08142718
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #96]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r7, r2
	ldr r0, .L_0814271c
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #176
	lsls r3, r3, #4
	adds r1, r7, r3
	ldr r0, .L_08142720
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r4, #253
	lsls r4, r4, #6
	adds r1, r7, r4
	movs r2, #0
	movs r3, #0
	ldr r0, .L_08142724
	bl Resource_LoadAndDecompress
	ldr r3, .L_08142728
	ldr r1, [sp, #32]
	movs r7, #0
	movs r2, #128
	mov r8, r7
	lsls r2, r2, #2
.L_081423c8:
	movs r0, #1
	add r8, r0
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_081423c8
	ldr r1, [sp, #96]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	str r3, [r2]
	ldr r2, .L_08142714
	movs r3, #32
	movs r7, #0
	strh r3, [r2, #6]
	str r7, [sp, #84]
.L_081423f6:
	ldr r3, .L_0814272c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08142410
	ldr r0, [sp, #32]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_08142410
	movs r2, #8
	str r2, [sp, #32]
.L_08142410:
	ldr r3, [sp, #84]
	cmp r3, #6
	bgt .L_0814244a
	ldr r4, [sp, #32]
	movs r7, #1
	negs r7, r7
	cmp r4, r7
	bne .L_0814244a
	lsls r2, r3, #3
	movs r3, #31
	subs r1, r3, r2
	adds r2, r1, #0
	subs r3, #41
	adds r0, r1, #0
	cmp r2, r3
	bge .L_08142434
	movs r0, #10
	negs r0, r0
.L_08142434:
	movs r4, #15
	negs r4, r4
	cmp r2, r4
	bge .L_08142440
	movs r1, #15
	negs r1, r1
.L_08142440:
	cmp r2, #3
	bgt .L_08142446
	movs r2, #4
.L_08142446:
	bl Func_08164b2c
.L_0814244a:
	ldr r7, [sp, #84]
	cmp r7, #17
	bgt .L_08142456
	ldr r0, [sp, #32]
	cmp r0, #0
	ble .L_0814246c
.L_08142456:
	movs r1, #4
	movs r0, #4
	movs r2, #4
	bl Func_08164abc
	ldr r1, [sp, #32]
	subs r1, #1
	str r1, [sp, #32]
	cmp r1, #0
	bne .L_0814246c
	b .L_08142864
.L_0814246c:
	ldr r3, [sp, #84]
	ldr r4, [sp, #84]
	movs r7, #15
	movs r2, #0
	lsls r3, r3, #4
	lsls r4, r4, #1
	mov r10, r7
	movs r7, #240
	mov r8, r2
	mov r11, r3
	mov r9, r4
	lsls r7, r7, #14
.L_08142484:
	movs r3, #128
	mov r2, r11
	lsls r3, r3, #1
	adds r3, #255
	add r2, r8
	ands r2, r3
	lsls r6, r2, #3
	ldr r0, .L_08142730
	subs r6, r6, r2
	lsls r6, r6, #2
	adds r6, r6, r0
	bl Random16
	adds r2, r0, #0
	str r2, [sp, #12]
	bl Random16
	ldr r2, [sp, #12]
	mov r1, r10
	adds r5, r0, #0
	adds r0, r2, #0
	ands r5, r1
	bl Trig_Sin
	add r5, r9
	adds r3, r5, #0
	muls r3, r0
	ldr r2, [sp, #12]
	adds r3, r3, r7
	adds r0, r2, #0
	str r3, [r6]
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	adds r3, r3, r7
	str r3, [r6, #4]
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #16]
	bl Random16
	mov r2, r10
	movs r3, #1
	ands r0, r2
	add r8, r3
	adds r0, #16
	mov r4, r8
	str r0, [r6, #24]
	cmp r4, #16
	bne .L_08142484
	ldr r6, .L_08142734
	ldr r5, .L_08142730
	movs r7, #0
	mov r8, r7
.L_081424f2:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08142534
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #72]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #100]
	ldr r4, [sp, #88]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_08142738
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08142534:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r8, r0
	bne .L_081424f2
	movs r0, #1
	bl Func_081969f8
	ldr r3, [sp, #84]
	mov r2, sp
	movs r1, #0
	adds r2, #108
	mov r10, r0
	str r1, [sp, #24]
	str r2, [sp, #16]
	cmp r3, #31
	bgt .L_081425e4
	movs r6, #128
	lsls r6, r6, #9
	cmp r3, #15
	ble .L_08142568
	lsls r3, r3, #12
	subs r3, r6, r3
	adds r6, r3, r6
.L_08142568:
	ldr r3, [sp, #108]
	ldr r2, .L_0814273c
	ldr r7, [sp, #96]
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08142740
	mov r4, r8
	ands r3, r2
	movs r0, #253
	mov r1, sp
	orrs r3, r4
	lsls r0, r0, #6
	str r3, [sp, #108]
	adds r1, #108
	adds r3, r7, r0
	str r1, [sp, #16]
	str r3, [r1, #4]
	ldr r3, .L_08142744
	mov r2, r10
	str r3, [r2, #8]
	ldr r3, [sp, #24]
	ldr r4, .L_08142748
	str r3, [r2, #20]
	movs r3, #6
	str r4, [r2, #12]
	str r3, [r2]
	str r1, [r2, #16]
	bl Func_08014de4
	movs r1, #128
	ldr r0, .L_0814274c
	lsls r1, r1, #13
	movs r2, #0
	movs r5, #128
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #152
	lsls r0, r0, #8
	bl Func_080150e4
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_08142750
	ldr r1, .L_08142748
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_081425e4:
	ldr r7, [sp, #84]
	cmp r7, #0
	bne .L_0814267c
	movs r0, #208
	bl Audio_PlayCue
	movs r3, #8
	ldr r0, .L_08142754
	movs r1, #16
	movs r2, #8
	str r3, [sp, #0]
	bl Func_0818de3c
	movs r0, #0
	str r0, [sp, #28]
	mov r9, r0
	mov r11, r0
.L_08142606:
	mov r0, r9
	bl Trig_Cos
	negs r0, r0
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #5
	asrs r3, r3, #16
	mov r0, r9
	mov r8, r3
	bl Trig_Sin
	ldr r6, .L_08142758
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r2, r3, #16
	movs r7, #0
	add r6, r11
.L_0814262c:
	lsls r5, r7, #12
	adds r0, r5, #0
	str r2, [sp, #12]
	bl Trig_Sin
	ldr r2, [sp, #12]
	mov r1, r8
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r6]
	strb r1, [r6, #1]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, [sp, #12]
	adds r7, #1
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r6, #2]
	adds r6, #4
	cmp r7, #16
	bne .L_0814262c
	ldr r4, [sp, #28]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #136
	movs r3, #64
	adds r4, #1
	add r9, r2
	add r11, r3
	str r4, [sp, #28]
	cmp r4, #8
	bne .L_08142606
	ldr r0, .L_0814275c
	ldr r1, .L_08142760
	movs r2, #32
	bl Func_081885f0
.L_0814267c:
	ldr r0, [sp, #16]
	add r6, sp, #108
	movs r7, #5
	strb r7, [r0]
	strb r7, [r6, #1]
	ldr r1, [sp, #96]
	movs r2, #224
	lsls r2, r2, #3
	adds r3, r1, r2
	ldr r0, .L_08142754
	ldr r1, .L_08142748
	mov r4, r10
	str r3, [r6, #4]
	movs r3, #6
	str r3, [r4]
	str r0, [r4, #8]
	str r1, [r4, #12]
	str r6, [r4, #16]
	ldr r2, [sp, #24]
	movs r3, #3
	str r3, [r4, #4]
	str r2, [r4, #20]
	ldr r4, [sp, #84]
	movs r0, #128
	lsls r3, r4, #9
	lsls r0, r0, #8
	movs r1, #128
	adds r5, r3, r0
	lsls r1, r1, #9
	cmp r5, r1
	ble .L_081426be
	movs r5, #128
	lsls r5, r5, #9
.L_081426be:
	bl Func_08014de4
	movs r1, #192
	lsls r1, r1, #12
	movs r2, #0
	ldr r0, .L_0814274c
	bl Func_08015160
	movs r0, #128
	lsls r0, r0, #6
	bl SceneTransform_ApplyPitch
	ldr r2, [sp, #84]
	negs r0, r2
	lsls r0, r0, #10
	bl Func_08015068
	adds r0, r5, #0
	bl Func_0801521c
	ldr r1, .L_08142748
	movs r2, #128
	ldr r0, .L_08142758
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	movs r2, #7
	strb r7, [r6, #1]
	strb r2, [r6]
	ldr r4, [sp, #96]
	ldr r1, .L_08142760
	movs r7, #176
	lsls r7, r7, #4
	mov r0, r10
	adds r3, r4, r7
	str r3, [r6, #4]
	str r6, [r0, #16]
	b .L_08142764
	.2byte 0x0000
.L_08142710:
	.4byte 0x00000075
.L_08142714:
	.4byte Data_03001120
.L_08142718:
	.4byte gCameraSceneParameters
.L_0814271c:
	.4byte 0x000000cd
.L_08142720:
	.4byte 0x000000c1
.L_08142724:
	.4byte 0x000000ca
.L_08142728:
	.4byte Data_02014018
.L_0814272c:
	.4byte gInput
.L_08142730:
	.4byte Data_02014000
.L_08142734:
	.4byte Data_08197410
.L_08142738:
	.4byte 0xffffc000
.L_0814273c:
	.4byte 0xffffff00
.L_08142740:
	.4byte 0xffff00ff
.L_08142744:
	.4byte Data_08199220
.L_08142748:
	.4byte gMapCellBuffer
.L_0814274c:
	.4byte 0xfffe0000
.L_08142750:
	.4byte Data_081991c0
.L_08142754:
	.4byte Data_02011000
.L_08142758:
	.4byte Data_02010800
.L_0814275c:
	.4byte Data_02012000
.L_08142760:
	.4byte Data_02012800
.L_08142764:
	str r2, [r0]
	str r1, [r0, #8]
	add r7, sp, #24
	ldr r2, [sp, #24]
	ldr r3, .L_0814290c
	ldrb r7, [r7]
	str r3, [r0, #12]
	str r2, [r0, #4]
	strb r2, [r0, #24]
	strb r7, [r0, #25]
	ldr r1, [sp, #84]
	movs r0, #0
	lsls r3, r1, #2
	adds r6, r1, #0
	adds r3, #96
	lsls r1, r1, #10
	mov r8, r0
	mov r11, r3
	adds r6, #24
	mov r9, r1
.L_0814278c:
	cmp r6, #63
	bhi .L_08142826
	ldr r3, .L_08142910
	mov r2, r8
	ldrb r3, [r3, r2]
	movs r4, #0
	muls r3, r6
	lsls r7, r3, #10
	movs r3, #128
	lsls r3, r3, #7
	adds r5, r7, r3
	str r4, [sp, #24]
	cmp r6, #15
	bgt .L_081427ae
	mov r0, r11
	subs r0, #64
	str r0, [sp, #24]
.L_081427ae:
	cmp r6, #40
	ble .L_081427ba
	movs r3, #40
	subs r3, r3, r6
	lsls r3, r3, #2
	str r3, [sp, #24]
.L_081427ba:
	ldr r1, [sp, #24]
	mov r2, r10
	str r1, [r2, #20]
	bl Func_08014de4
	movs r1, #128
	ldr r0, .L_08142914
	lsls r1, r1, #14
	movs r2, #0
	bl Func_08015160
	adds r0, r5, #0
	cmp r5, #0
	bge .L_081427de
	movs r3, #128
	lsls r3, r3, #7
	adds r3, #3
	adds r0, r7, r3
.L_081427de:
	asrs r2, r0, #2
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_080151e4
	movs r0, #156
	lsls r0, r0, #6
	adds r0, #16
	bl SceneTransform_ApplyPitch
	mov r0, r9
	bl Func_08015068
	movs r2, #128
	lsls r2, r2, #9
	movs r1, #192
	adds r0, r2, #0
	lsls r1, r1, #11
	bl Func_080151e4
	ldr r3, .L_08142918
	mov r4, r8
	ldrsb r1, [r3, r4]
	movs r0, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	ldr r0, .L_0814291c
	ldr r1, .L_0814290c
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08142826:
	movs r1, #1
	movs r7, #24
	movs r0, #128
	add r8, r1
	negs r7, r7
	lsls r0, r0, #6
	mov r2, r8
	add r11, r7
	subs r6, #6
	add r9, r0
	cmp r2, #4
	bne .L_0814278c
	mov r0, r10
	bl Sys_Free
	movs r4, #240
	ldr r3, [sp, #96]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #84]
	adds r7, #1
	str r7, [sp, #84]
	cmp r7, #26
	beq .L_08142864
	b .L_081423f6
.L_08142864:
	movs r1, #240
	ldr r5, .L_08142920
	lsls r1, r1, #6
	ldr r2, .L_08142924
	ldr r0, .L_08142928
	mov lr, r5
	.2byte 0xf800
	add r0, sp, #80
	ldr r3, .L_0814292c
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r3, #4]
	ldr r1, [sp, #76]
	movs r3, #120
	str r2, [r1, #16]
	ldr r2, .L_08142930
	movs r4, #206
	str r3, [r2, #16]
	ldr r2, [sp, #68]
	lsls r4, r4, #3
	adds r3, r2, r4
	ldrh r1, [r3]
	movs r2, #24
	movs r0, #1
	bl Func_08118040
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #96]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r7, r0
	movs r3, #3
	movs r1, #238
	str r3, [r2]
	lsls r1, r1, #7
	ldr r3, .L_08142934
	adds r1, #132
	adds r2, r7, r1
	str r3, [r2]
	bl Func_0815b410
	movs r1, #240
	ldr r2, .L_08142924
	ldr r0, [sp, #100]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	movs r0, #134
	bl Func_081180e8
	movs r2, #0
	movs r3, #240
	lsls r3, r3, #7
	str r2, [sp, #84]
	adds r3, #232
	adds r5, r7, r3
	movs r6, #1
.L_081428da:
	str r6, [r5]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #84]
	adds r4, #1
	str r4, [sp, #84]
	cmp r4, #32
	bne .L_081428da
	ldr r0, .L_08142938
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #184
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0814290c:
	.4byte gMapCellBuffer
.L_08142910:
	.4byte Data_081977e4
.L_08142914:
	.4byte 0xfffe0000
.L_08142918:
	.4byte Data_081977e8
.L_0814291c:
	.4byte Data_02012000
.L_08142920:
	.4byte IwramFillWords
.L_08142924:
	.4byte 0x3f3f3f3f
.L_08142928:
	.4byte 0x06004000
.L_0814292c:
	.4byte Data_03001120
.L_08142930:
	.4byte gCameraSceneParameters
.L_08142934:
	.4byte Data_02020202
.L_08142938:
	.4byte Func_08143000
	.4byte 0x00004770
	.4byte 0x00004770
