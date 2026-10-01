.syntax unified
	.thumb
	.global Func_0818cb74
	.thumb_func
Func_0818cb74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	str r0, [sp, #56]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	movs r7, #240
	str r0, [sp, #52]
	movs r0, #0
	ldr r1, [r5, #96]
	lsls r7, r7, #7
	str r1, [sp, #48]
	adds r7, #240
	ldr r2, [r5, #100]
	movs r6, #2
	str r2, [sp, #36]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0818cbdc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #52]
	adds r3, r4, r7
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #4
	movs r2, #1
	bl Func_08152404
	ldr r3, .L_0818cbe0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_0818cbe4
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_0818cbe8
	subs r2, #6
	b .L_0818cbec
	.2byte 0x0000
.L_0818cbdc:
	.4byte 0x00001010
.L_0818cbe0:
	.4byte 0x00002737
.L_0818cbe4:
	.4byte 0x00003f21
.L_0818cbe8:
	.4byte 0x0000107c
.L_0818cbec:
	strh r3, [r2]
	ldr r3, .L_0818cc2c
	adds r2, #2
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r0, [sp, #52]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	ldr r5, [r5, #104]
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818cc30
	str r5, [sp, #40]
	bl Scheduler_AddOrUpdateCallback
	movs r7, #166
	ldr r4, [sp, #52]
	lsls r7, r7, #7
	movs r2, #224
	b .L_0818cc34
.L_0818cc2c:
	.4byte 0x00001088
.L_0818cc30:
	.4byte Func_08143000
.L_0818cc34:
	lsls r2, r2, #3
	adds r7, #86
	adds r7, r4, r7
	adds r1, r4, r2
	ldr r0, .L_0818cf60
	movs r2, #1
	movs r3, #0
	str r7, [sp, #28]
	bl Resource_LoadAndDecompress
	movs r4, #152
	ldr r3, [sp, #52]
	lsls r4, r4, #5
	adds r4, #86
	adds r1, r3, r4
	ldr r0, .L_0818cf64
	movs r2, #0
	movs r3, #0
	ldr r7, .L_0818cf68
	bl Resource_LoadAndDecompress
	movs r2, #160
	lsls r2, r2, #7
	adds r1, r7, r2
	movs r3, #0
	ldr r0, .L_0818cf6c
	movs r2, #1
	bl Resource_LoadAndDecompress
	movs r7, #144
	movs r3, #0
	lsls r7, r7, #6
	mov r9, r3
	movs r5, #0
.L_0818cc78:
	ldr r4, .L_0818cf68
	movs r2, #128
	adds r0, r7, r4
	adds r1, r6, #0
	lsls r2, r2, #9
	bl Func_0815b434
	adds r3, r5, #3
	muls r3, r6
	ldr r0, [sp, #28]
	lsrs r2, r3, #31
	movs r1, #1
	adds r3, r3, r2
	add r9, r1
	asrs r3, r3, #1
	mov r2, r9
	strh r7, [r5, r0]
	adds r6, #2
	adds r7, r7, r3
	adds r5, #2
	cmp r2, #12
	bne .L_0818cc78
	ldr r0, .L_0818cf70
	ldr r1, .L_0818cf68
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0818cf74
	ldr r1, .L_0818cf78
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0818cf7c
	ldr r1, .L_0818cf80
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #36]
	movs r3, #0
	ldr r0, .L_0818cf84
	bl Resource_LoadAndDecompress
	ldr r0, .L_0818cf88
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818cf8c
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	ldr r7, [sp, #52]
	movs r3, #0
	mov r9, r3
.L_0818ccee:
	mov r4, r9
	lsls r6, r4, #1
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r7]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	mov r0, r9
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r0, #31
	add r3, r9
	movs r1, #1
	asrs r3, r3, #1
	add r9, r1
	adds r3, #25
	mov r2, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #32
	bne .L_0818ccee
	add r5, sp, #92
	adds r0, r5, #0
	bl Func_0815e22c
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #32]
	movs r3, #0
	mov r11, r3
.L_0818cd48:
	mov r4, r11
	cmp r4, #0
	bne .L_0818cd62
	ldr r0, .L_0818cf88
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818cf8c
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0818cd62:
	mov r7, r11
	cmp r7, #76
	bgt .L_0818cd7c
	mov r0, r11
	cmp r0, #64
	ble .L_0818cd76
	ldr r0, .L_0818cf90
	bl Func_0815f0a0
	b .L_0818cd90
.L_0818cd76:
	mov r1, r11
	cmp r1, #23
	ble .L_0818cd84
.L_0818cd7c:
	ldr r0, .L_0818cf88
	bl Func_0815f0a0
	b .L_0818cd90
.L_0818cd84:
	mov r2, r11
	cmp r2, #8
	ble .L_0818cd90
	ldr r0, .L_0818cf90
	bl Func_0815f0a0
.L_0818cd90:
	mov r3, r11
	cmp r3, #8
	bne .L_0818cdd2
	ldr r4, [sp, #52]
	movs r7, #238
	lsls r7, r7, #7
	adds r7, #168
	adds r3, r4, r7
	mov r0, r11
	str r0, [r3]
	ldr r2, [sp, #56]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r9, r1
	cmp r3, #0
	beq .L_0818cdd2
	movs r6, #16
	movs r5, #36
.L_0818cdb4:
	ldr r3, [sp, #56]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r2, #5
	mov r3, r9
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r0, [sp, #56]
	movs r7, #1
	ldr r3, [r0, #20]
	add r9, r7
	adds r5, #2
	cmp r9, r3
	bne .L_0818cdb4
.L_0818cdd2:
	mov r1, r11
	cmp r1, #48
	bne .L_0818cde6
	ldr r3, [sp, #52]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
.L_0818cde6:
	mov r7, r11
	cmp r7, #64
	bne .L_0818ce36
	ldr r0, [sp, #52]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r0, r1
	movs r2, #16
	str r2, [r3]
	movs r0, #144
	bl Func_081180e8
	ldr r4, [sp, #56]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r9, r2
	cmp r3, #0
	beq .L_0818ce36
	movs r6, #128
	lsls r6, r6, #12
	movs r5, #36
	movs r7, #120
.L_0818ce14:
	ldr r1, [sp, #56]
	adds r3, r6, #0
	ldrsh r0, [r5, r1]
	movs r2, #128
	movs r1, #1
	lsls r2, r2, #10
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl Func_0815f000
	ldr r4, [sp, #56]
	movs r3, #1
	add r9, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r9, r3
	bne .L_0818ce14
.L_0818ce36:
	mov r7, r11
	cmp r7, #0
	bne .L_0818ce42
	movs r0, #212
	bl Audio_PlayCue
.L_0818ce42:
	mov r0, r11
	cmp r0, #4
	bne .L_0818ce4e
	movs r0, #144
	bl Audio_PlayCue
.L_0818ce4e:
	mov r1, r11
	cmp r1, #16
	bne .L_0818ce5a
	movs r0, #164
	bl Audio_PlayCue
.L_0818ce5a:
	mov r2, r11
	cmp r2, #64
	bne .L_0818ce66
	movs r0, #145
	bl Audio_PlayCue
.L_0818ce66:
	mov r3, r11
	cmp r3, #63
	bgt .L_0818ceee
	ldr r0, [sp, #32]
	movs r2, #128
	subs r0, #8
	cmp r3, #9
	bgt .L_0818ce7e
	lsls r3, r3, #4
	adds r1, r3, #0
	subs r1, #144
	b .L_0818ce80
.L_0818ce7e:
	movs r1, #0
.L_0818ce80:
	adds r3, r1, #0
	adds r3, #128
	cmp r3, #108
	ble .L_0818ce8e
	subs r3, r2, r1
	adds r2, r3, #0
	subs r2, #20
.L_0818ce8e:
	cmp r2, #0
	ble .L_0818ceee
	ldr r3, .L_0818cf94
	add r2, sp, #76
	ldr r4, [r3, #4]
	ldr r3, [r3]
	lsls r0, r0, #17
	str r3, [sp, #68]
	str r4, [sp, #72]
	movs r3, #255
	movs r4, #0
	lsls r3, r3, #16
	str r4, [r2, #12]
	str r3, [r2, #4]
	mov r8, r0
	adds r7, r2, #0
	ldr r0, [sp, #52]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	adds r6, r0, r2
	movs r0, #160
	lsls r3, r1, #16
	lsls r0, r0, #14
	mov r9, r4
	adds r5, r3, r0
	add r4, sp, #68
.L_0818cec4:
	movs r3, #128
	lsls r3, r3, #13
	add r3, r8
	adds r2, r4, #0
	str r3, [r7]
	str r5, [r7, #8]
	adds r1, r7, #0
	movs r3, #0
	ldmia r6!, {r0}
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	movs r1, #128
	add r9, r2
	lsls r1, r1, #14
	mov r3, r9
	adds r5, r5, r1
	ldr r4, [sp, #8]
	cmp r3, #4
	bne .L_0818cec4
.L_0818ceee:
	mov r3, r11
	subs r3, #12
	cmp r3, #52
	bls .L_0818cef8
	b .L_0818d078
.L_0818cef8:
	mov r4, r11
	cmp r4, #12
	bne .L_0818cf18
	ldr r3, [sp, #52]
	movs r7, #0
	mov r9, r7
	adds r3, #24
	movs r2, #0
.L_0818cf08:
	movs r0, #1
	add r9, r0
	mov r1, r9
	str r2, [r3]
	adds r3, #28
	subs r2, #2
	cmp r1, #32
	bne .L_0818cf08
.L_0818cf18:
	mov r4, r11
	lsls r3, r4, #2
	ldr r7, [sp, #52]
	movs r2, #0
	subs r3, #156
	mov r9, r2
	mov r10, r3
.L_0818cf26:
	ldr r3, [r7, #24]
	mov r0, r9
	adds r3, #1
	str r3, [r7, #24]
	cmp r0, #0
	bne .L_0818cf98
	mov r1, r11
	cmp r1, #39
	bgt .L_0818cf3a
	b .L_0818d06a
.L_0818cf3a:
	ldr r2, [sp, #52]
	movs r3, #166
	lsls r3, r3, #7
	adds r3, #134
	adds r5, r2, r3
	movs r2, #128
	adds r0, r5, #0
	mov r1, r10
	lsls r2, r2, #9
	bl Func_0815b434
	adds r0, r5, #0
	ldr r1, [sp, #32]
	movs r2, #100
	mov r3, r10
	bl Func_0818caa8
	b .L_0818d06a
	.2byte 0x0000
.L_0818cf60:
	.4byte 0x00000192
.L_0818cf64:
	.4byte 0x000000f8
.L_0818cf68:
	.4byte gMapCellBuffer
.L_0818cf6c:
	.4byte 0x0000013e
.L_0818cf70:
	.4byte 0x000000c1
.L_0818cf74:
	.4byte 0x000000fa
.L_0818cf78:
	.4byte Data_02011000
.L_0818cf7c:
	.4byte 0x000000da
.L_0818cf80:
	.4byte Data_02011400
.L_0818cf84:
	.4byte 0x00000134
.L_0818cf88:
	.4byte 0x00000149
.L_0818cf8c:
	.4byte IwramCopyWords
.L_0818cf90:
	.4byte 0x00000148
.L_0818cf94:
	.4byte Data_08196f18
.L_0818cf98:
	cmp r3, #0
	bne .L_0818d020
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r6, r0, #0
	movs r4, #128
	lsls r4, r4, #7
	ands r6, r3
	adds r2, r6, r4
	str r2, [sp, #16]
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	ldr r2, [sp, #16]
	adds r5, #255
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #2
	adds r5, r5, r0
	adds r0, r2, #0
	bl Trig_Sin
	ldr r1, [sp, #32]
	adds r3, r5, #0
	muls r3, r0
	ldr r2, [sp, #16]
	lsls r1, r1, #16
	mov r8, r1
	asrs r3, r3, #4
	add r3, r8
	str r3, [r7]
	adds r0, r2, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #208
	lsls r2, r2, #15
	asrs r3, r3, #3
	adds r3, r3, r2
	str r3, [r7, #4]
	movs r3, #192
	lsls r3, r3, #8
	adds r6, r6, r3
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #16]
	bl Random16
	movs r4, #7
	ands r0, r4
	adds r0, #4
	str r0, [r7, #20]
.L_0818d020:
	ldr r0, [r7, #24]
	cmp r0, #31
	bhi .L_0818d06a
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r3, [r7, #20]
	muls r3, r0
	asrs r3, r3, #16
	lsls r4, r3, #1
	subs r5, r4, #2
	cmp r5, #61
	bhi .L_0818d06a
	movs r1, #6
	ldrsh r3, [r7, r1]
	movs r0, #2
	ldrsh r6, [r7, r0]
	movs r1, #64
	adds r0, r7, #0
	movs r2, #0
	str r3, [sp, #12]
	str r4, [sp, #8]
	bl BattleFxKernels_IntegrateVector2
	ldr r2, [sp, #28]
	asrs r5, r5, #1
	lsls r5, r5, #1
	ldrsh r0, [r5, r2]
	ldr r3, [sp, #12]
	ldr r2, .L_0818d2e8
	ldr r4, [sp, #8]
	adds r0, r0, r2
	adds r1, r6, #0
	adds r2, r3, #0
	adds r3, r4, #0
	bl Func_0818caa8
.L_0818d06a:
	movs r3, #1
	add r9, r3
	mov r4, r9
	adds r7, #28
	cmp r4, #14
	beq .L_0818d078
	b .L_0818cf26
.L_0818d078:
	mov r7, r11
	cmp r7, #7
	bgt .L_0818d080
	b .L_0818d27c
.L_0818d080:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #24]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0818d2ec
	ldr r3, [sp, #60]
	adds r7, r0, #0
	ands r3, r2
	ldr r2, .L_0818d2f0
	movs r0, #7
	orrs r3, r0
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #60]
	movs r3, #0
	str r3, [r7, #4]
	ldr r2, .L_0818d2e8
	ldr r3, .L_0818d2f4
	add r1, sp, #60
	str r2, [r1, #4]
	str r0, [r7]
	str r1, [r7, #16]
	str r3, [r7, #8]
	ldr r4, [sp, #24]
	movs r0, #0
	str r4, [r7, #12]
	strb r0, [r7, #24]
	strb r0, [r7, #25]
	ldr r2, [sp, #32]
	mov r8, r1
	subs r2, #64
	movs r1, #0
	mov r9, r1
	mov r6, r11
	mov r10, r2
.L_0818d0d2:
	ldr r3, .L_0818d2f8
	mov r4, r9
	ldrb r1, [r3, r4]
	cmp r11, r1
	ble .L_0818d14e
	ldr r3, .L_0818d2fc
	mov r0, r11
	ldrb r3, [r3, r4]
	subs r2, r0, r1
	muls r2, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	muls r3, r2
	movs r2, #131
	lsls r2, r2, #7
	adds r5, r3, r2
	subs r3, r1, r0
	lsls r3, r3, #3
	adds r3, #56
	cmp r3, #0
	ble .L_0818d100
	movs r3, #0
.L_0818d100:
	movs r4, #64
	negs r4, r4
	cmp r3, r4
	ble .L_0818d14e
	str r3, [r7, #20]
	bl Func_08014de4
	ldr r3, .L_0818d300
	mov r0, r9
	ldrsb r1, [r3, r0]
	mov r2, r10
	lsls r0, r2, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r4, #7
	adds r3, r6, #0
	ands r3, r4
	movs r0, #176
	lsls r3, r3, #4
	lsls r0, r0, #4
	strb r3, [r7, #24]
	adds r0, #184
	bl SceneTransform_ApplyPitch
	ldr r0, .L_0818d304
	ldr r1, [sp, #24]
	movs r2, #32
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818d14e:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r6, #5
	cmp r1, #8
	bne .L_0818d0d2
	mov r2, r11
	cmp r2, #71
	ble .L_0818d162
	b .L_0818d270
.L_0818d162:
	movs r3, #5
	mov r4, r8
	strb r3, [r4]
	ldr r3, .L_0818d308
	mov r0, sp
	adds r0, #60
	movs r1, #7
	movs r2, #7
	str r0, [sp, #20]
	strb r1, [r0, #1]
	str r0, [r7, #16]
	str r2, [r7]
	str r3, [r7, #8]
	ldr r3, [sp, #24]
	movs r4, #0
	mov r0, r11
	str r3, [r7, #12]
	strb r4, [r7, #24]
	strb r4, [r7, #25]
	cmp r0, #8
	bne .L_0818d1bc
	ldr r2, [sp, #52]
	movs r3, #224
	ldr r6, .L_0818d30c
	movs r1, #0
	lsls r3, r3, #2
	mov r9, r1
	adds r5, r2, r3
.L_0818d19a:
	ldr r3, [r6]
	mov r0, r9
	str r3, [r5, #8]
	movs r1, #6
	ldr r3, [r6, #4]
	adds r6, #8
	str r3, [r5, #20]
	bl __modsi3
	movs r4, #1
	lsls r0, r0, #2
	add r9, r4
	str r0, [r5, #24]
	mov r0, r9
	adds r5, #28
	cmp r0, #8
	bne .L_0818d19a
.L_0818d1bc:
	ldr r3, [sp, #52]
	movs r4, #224
	movs r1, #0
	movs r2, #18
	lsls r4, r4, #2
	mov r9, r1
	mov r10, r2
	adds r6, r3, r4
.L_0818d1cc:
	mov r0, r9
	lsls r5, r0, #2
	cmp r11, r10
	blt .L_0818d260
	adds r3, r5, #0
	adds r3, #42
	cmp r11, r3
	bge .L_0818d260
	ldr r0, [r6, #24]
	cmp r0, #0
	bge .L_0818d1e4
	adds r0, #7
.L_0818d1e4:
	asrs r0, r0, #3
	movs r1, #3
	bl __modsi3
	mov r8, r0
	movs r3, #16
	negs r3, r3
	str r3, [r7, #20]
	ldr r2, [sp, #52]
	mov r1, r8
	movs r4, #152
	ldr r0, [sp, #20]
	lsls r3, r1, #12
	lsls r4, r4, #5
	adds r4, #86
	adds r3, r2, r3
	adds r3, r3, r4
	str r3, [r0, #4]
	bl Func_08014de4
	ldr r0, [sp, #32]
	movs r1, #128
	subs r0, #64
	lsls r1, r1, #14
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	ldr r0, [r6, #8]
	bl Func_080150e4
	movs r3, #1
	mov r1, r9
	ands r3, r1
	cmp r3, #0
	beq .L_0818d234
	movs r0, #128
	lsls r0, r0, #8
	bl Func_08015068
.L_0818d234:
	ldr r3, [r6, #8]
	ldr r2, [r6, #20]
	movs r0, #128
	adds r3, r3, r2
	movs r2, #128
	str r3, [r6, #8]
	lsls r0, r0, #10
	ldr r1, .L_0818d310
	lsls r2, r2, #9
	bl Func_080151e4
	ldr r3, .L_0818d314
	mov r2, r8
	lsls r0, r2, #4
	adds r0, r0, r3
	ldr r1, [sp, #24]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818d260:
	movs r4, #1
	add r9, r4
	movs r3, #4
	mov r0, r9
	add r10, r3
	adds r6, #28
	cmp r0, #7
	bne .L_0818d1cc
.L_0818d270:
	adds r0, r7, #0
	bl Sys_Free
	ldr r0, [sp, #24]
	bl Sys_Free
.L_0818d27c:
	mov r1, r11
	cmp r1, #63
	bgt .L_0818d284
	b .L_0818d458
.L_0818d284:
	cmp r1, #64
	beq .L_0818d28a
	b .L_0818d3ac
.L_0818d28a:
	ldr r3, [sp, #32]
	ldr r7, [sp, #52]
	movs r2, #0
	lsls r3, r3, #16
	mov r9, r2
	mov r8, r3
.L_0818d296:
	bl Random16
	adds r6, r0, #0
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	movs r4, #128
	lsls r4, r4, #2
	ands r5, r0
	adds r0, r6, #0
	adds r5, r5, r4
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	mov r0, r9
	asrs r2, r3, #7
	str r2, [r7, #16]
	cmp r0, #15
	ble .L_0818d318
	ldr r3, [r7, #12]
	movs r1, #200
	lsls r3, r3, #2
	add r3, r8
	str r3, [r7]
	lsls r1, r1, #15
	lsls r3, r2, #2
	adds r3, r3, r1
	str r3, [r7, #4]
	movs r3, #16
	subs r3, r3, r0
	b .L_0818d32e
.L_0818d2e8:
	.4byte gMapCellBuffer
.L_0818d2ec:
	.4byte 0xffffff00
.L_0818d2f0:
	.4byte 0xffff00ff
.L_0818d2f4:
	.4byte Data_08198ec4
.L_0818d2f8:
	.4byte Data_08199dcb
.L_0818d2fc:
	.4byte Data_08199dd3
.L_0818d300:
	.4byte Data_08199ddb
.L_0818d304:
	.4byte Data_08198cac
.L_0818d308:
	.4byte Data_08199268
.L_0818d30c:
	.4byte Data_08199de4
.L_0818d310:
	.4byte 0x00012710
.L_0818d314:
	.4byte Data_08199e24
.L_0818d318:
	mov r2, r8
	str r2, [r7]
	bl Random16
	movs r1, #104
	bl Math_ModU
	mov r4, r9
	lsls r0, r0, #16
	negs r3, r4
	str r0, [r7, #4]
.L_0818d32e:
	cmp r3, #0
	bge .L_0818d334
	adds r3, #3
.L_0818d334:
	asrs r3, r3, #2
	str r3, [r7, #24]
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r7, #28
	cmp r1, #32
	bne .L_0818d296
	ldr r7, .L_0818d56c
	movs r2, #0
	mov r9, r2
.L_0818d34a:
	bl Random16
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	movs r3, #128
	lsls r3, r3, #7
	ands r6, r0
	adds r6, r6, r3
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	movs r3, #200
	lsls r3, r3, #15
	ands r5, r0
	movs r4, #128
	mov r0, r8
	lsls r4, r4, #2
	str r0, [r7]
	str r3, [r7, #4]
	adds r0, r6, #0
	adds r5, r5, r4
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	movs r1, #1
	ands r3, r0
	add r9, r1
	adds r3, #16
	mov r2, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #128
	bne .L_0818d34a
.L_0818d3ac:
	ldr r5, [sp, #52]
	movs r3, #0
	mov r9, r3
.L_0818d3b2:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_0818d3f0
	adds r1, r3, #0
	cmp r3, #0
	bge .L_0818d3c0
	adds r1, r3, #3
.L_0818d3c0:
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r7, #2
	ldrsh r2, [r5, r7]
	ldr r4, .L_0818d570
	movs r0, #32
	asrs r1, r1, #2
	str r0, [sp, #0]
	lsls r1, r1, #11
	movs r0, #64
	adds r1, r1, r4
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #48]
	ldr r4, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_0818d574
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_0818d3f0:
	movs r7, #1
	add r9, r7
	adds r3, #1
	mov r0, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #32
	bne .L_0818d3b2
	ldr r6, .L_0818d578
	ldr r5, .L_0818d56c
	movs r1, #0
	mov r9, r1
.L_0818d408:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0818d44c
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #36]
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
	ldr r0, [sp, #48]
	ldr r4, [sp, #40]
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
.L_0818d44c:
	movs r7, #1
	add r9, r7
	mov r0, r9
	adds r5, #28
	cmp r0, #128
	bne .L_0818d408
.L_0818d458:
	mov r3, r11
	subs r3, #24
	cmp r3, #39
	bhi .L_0818d504
	movs r1, #0
	mov r9, r1
	movs r7, #3
.L_0818d466:
	mov r4, r9
	ands r4, r7
	str r4, [sp, #8]
	bl Random16
	adds r6, r0, #0
	bl Trig_Sin
	ldr r3, .L_0818d57c
	ldr r4, [sp, #8]
	ldr r2, [sp, #32]
	adds r5, r0, #0
	mov r10, r3
	ldrb r3, [r3, r4]
	lsls r5, r5, #3
	asrs r5, r5, #16
	adds r5, r2, r5
	lsrs r3, r3, #1
	adds r0, r6, #0
	subs r5, r5, r3
	bl Trig_Cos
	ldr r2, .L_0818d580
	ldr r4, [sp, #8]
	mov r1, r9
	ldrb r3, [r2, r4]
	lsls r0, r0, #4
	lsls r6, r1, #5
	asrs r0, r0, #16
	lsrs r3, r3, #1
	adds r6, r6, r0
	mov r8, r2
	subs r6, r6, r3
	bl Random16
	ldr r3, .L_0818d584
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
	ldr r4, [sp, #8]
	ldr r2, .L_0818d588
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #52]
	mov r2, r10
	adds r1, r3, r1
	ldrb r3, [r2, r4]
	movs r0, #224
	str r3, [sp, #0]
	lsls r0, r0, #3
	adds r1, r1, r0
	mov r0, r8
	ldrb r3, [r0, r4]
	movs r2, #192
	str r3, [sp, #4]
	lsls r2, r2, #18
	adds r2, #188
	ldr r4, [r2]
	adds r3, r6, #0
	ldr r0, [sp, #48]
	adds r2, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	add r9, r3
	mov r4, r9
	cmp r4, #4
	bne .L_0818d466
.L_0818d504:
	movs r1, #16
	movs r0, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r0, #240
	ldr r7, [sp, #52]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r7, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r11, r1
	mov r2, r11
	cmp r2, #104
	beq .L_0818d532
	bl .L_0818cd48
.L_0818d532:
	ldr r0, .L_0818d58c
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r4, #238
	lsls r4, r4, #7
	movs r3, #0
	adds r4, #220
	mov r9, r3
	adds r5, r7, r4
.L_0818d54a:
	movs r7, #1
	ldmia r5!, {r0}
	add r9, r7
	bl ResourceObject_ReleaseFar
	mov r0, r9
	cmp r0, #4
	bne .L_0818d54a
	bl Func_08143bb8
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0818d56c:
	.4byte Data_02012964
.L_0818d570:
	.4byte Data_02015000
.L_0818d574:
	.4byte 0xffffc000
.L_0818d578:
	.4byte Data_08197410
.L_0818d57c:
	.4byte Data_08197492
.L_0818d580:
	.4byte Data_08197498
.L_0818d584:
	.4byte Data_08199e54
.L_0818d588:
	.4byte Data_08197486
.L_0818d58c:
	.4byte Func_08143000
