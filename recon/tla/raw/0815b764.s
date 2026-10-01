.syntax unified
	.thumb
	.global Func_0815b764
	.thumb_func
Func_0815b764:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #152
	str r0, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	adds r2, r3, #0
	str r0, [sp, #96]
	adds r2, #176
	ldr r2, [r2]
	ldr r1, [r3, #92]
	str r2, [sp, #92]
	movs r2, #0
	ldr r3, [r3, #100]
	str r2, [sp, #80]
	str r3, [sp, #84]
	movs r3, #240
	str r2, [sp, #76]
	lsls r3, r3, #7
	mov r11, r1
	adds r3, #240
	add r3, r11
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r3, #0
	str r3, [sp, #88]
	ldr r5, .L_0815b7d8
	movs r6, #31
.L_0815b7aa:
	movs r0, #160
	lsls r0, r0, #19
	movs r4, #0
	adds r0, #192
	mov r9, r4
.L_0815b7b4:
	ldrh r3, [r0]
	adds r1, r6, #0
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	ands r2, r5
	lsrs r3, r3, #26
	ands r3, r5
	cmp r2, #0
	bne .L_0815b7dc
	cmp r3, #0
	ble .L_0815b7ce
	subs r3, #1
.L_0815b7ce:
	cmp r1, #0
	ble .L_0815b7dc
	subs r1, #1
	b .L_0815b7dc
	.2byte 0x0000
.L_0815b7d8:
	.4byte 0x0000001f
.L_0815b7dc:
	cmp r2, #0
	ble .L_0815b7e2
	subs r2, #1
.L_0815b7e2:
	lsls r3, r3, #10
	lsls r2, r2, #5
	movs r7, #1
	orrs r3, r2
	add r9, r7
	orrs r3, r1
	mov r1, r9
	strh r3, [r0]
	adds r0, #2
	cmp r1, #128
	bne .L_0815b7b4
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #88]
	adds r2, #1
	str r2, [sp, #88]
	cmp r2, #32
	bne .L_0815b7aa
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r1, .L_0815b8ec
	ldr r0, .L_0815b8f0
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0815b840
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #136
	adds r3, r3, r1
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0815b840:
	strh r4, [r0]
	movs r0, #1
	ldr r6, .L_0815b8f4
	bl WaitFrames
	movs r1, #128
	movs r2, #1
	ldr r5, .L_0815b8f8
	adds r0, r6, #0
	lsls r1, r1, #1
	negs r2, r2
	mov lr, r5
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #1
	adds r6, r6, r3
	adds r0, r6, #0
	movs r1, #128
	ldr r2, .L_0815b8fc
	mov lr, r5
	.2byte 0xf800
	movs r4, #128
	ldr r1, .L_0815b900
	lsls r4, r4, #10
	adds r6, #128
	movs r0, #0
	adds r4, #2
.L_0815b876:
	movs r3, #0
.L_0815b878:
	adds r3, #1
	stmia r6!, {r1}
	adds r1, r1, r4
	cmp r3, #8
	bne .L_0815b878
	ldr r2, .L_0815b8fc
	movs r3, #0
.L_0815b886:
	adds r3, #1
	stmia r6!, {r2}
	cmp r3, #8
	bne .L_0815b886
	adds r0, #1
	cmp r0, #16
	bne .L_0815b876
	movs r1, #200
	lsls r1, r1, #1
	adds r1, #255
	movs r0, #8
	movs r2, #3
	bl Func_08152404
	ldr r3, .L_0815b8d8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0815b8dc
	movs r0, #1
	strh r3, [r2]
	ldr r3, .L_0815b8e0
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_0815b8e4
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_0815b8e8
	subs r2, #70
	strh r3, [r2]
	bl WaitFrames
	mov r4, sp
	adds r4, #128
	adds r1, r4, #0
	movs r0, #0
	str r4, [sp, #72]
	bl Func_08144aac
	b .L_0815b904
.L_0815b8d8:
	.4byte 0x00000080
.L_0815b8dc:
	.4byte 0x00000100
.L_0815b8e0:
	.4byte 0x00001010
.L_0815b8e4:
	.4byte 0x00003f46
.L_0815b8e8:
	.4byte 0x00001f83
.L_0815b8ec:
	.4byte Data_020038e0
.L_0815b8f0:
	.4byte 0x04000208
.L_0815b8f4:
	.4byte 0x0600f800
.L_0815b8f8:
	.4byte IwramFillWords
.L_0815b8fc:
	.4byte 0x03ff03ff
.L_0815b900:
	.4byte Data_02010200
.L_0815b904:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	movs r5, #240
	adds r2, #132
	lsls r5, r5, #5
	add r2, r11
	movs r3, #0
	movs r1, #200
	adds r5, #129
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815ba0c
	add r5, r11
	bl Scheduler_AddOrUpdateCallback
	adds r1, r5, #0
	ldr r0, .L_0815ba10
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r7, #0
	movs r1, #140
	mov r9, r7
	lsls r1, r1, #6
.L_0815b940:
	ldrb r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0815b94c
	adds r3, #128
	strb r3, [r5]
.L_0815b94c:
	movs r0, #1
	add r9, r0
	adds r5, #1
	cmp r9, r1
	bne .L_0815b940
	movs r5, #240
	lsls r5, r5, #5
	movs r3, #80
	adds r5, #129
	str r3, [sp, #0]
	add r5, r11
	movs r3, #112
	str r3, [sp, #4]
	ldr r4, [sp, #128]
	ldr r0, [sp, #96]
	adds r1, r5, #0
	movs r2, #24
	movs r3, #8
	mov lr, r4
	.2byte 0xf800
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r11
	movs r6, #1
	str r6, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0815ba0c
	bl Scheduler_RemoveCallback
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0815ba14
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0815ba18
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_0815ba1c
	ldr r1, [sp, #84]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r4, [sp, #92]
	movs r1, #0
	movs r2, #200
	movs r3, #168
	str r1, [sp, #60]
	lsls r2, r2, #16
	str r6, [r4, #16]
	lsls r3, r3, #15
	movs r1, #240
	str r2, [sp, #64]
	str r3, [sp, #68]
	lsls r1, r1, #6
	ldr r3, .L_0815ba20
	ldr r0, [sp, #96]
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0815ba08
	movs r2, #128
	lsls r2, r2, #19
	movs r0, #1
	strh r3, [r2]
	bl WaitFrames
	movs r0, #184
	movs r1, #92
	movs r7, #0
	negs r0, r0
	negs r1, r1
	str r7, [sp, #88]
	str r0, [sp, #16]
	str r1, [sp, #12]
.L_0815b9f2:
	movs r2, #1
	str r2, [sp, #56]
	ldr r3, .L_0815ba24
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0815ba28
	movs r3, #1
	str r3, [sp, #76]
	b .L_0815ba28
.L_0815ba08:
	.4byte 0x00007141
.L_0815ba0c:
	.4byte Func_08143000
.L_0815ba10:
	.4byte 0x000000b1
.L_0815ba14:
	.4byte 0x00000178
.L_0815ba18:
	.4byte 0x000000b3
.L_0815ba1c:
	.4byte 0x00000137
.L_0815ba20:
	.4byte IwramClearWords
.L_0815ba24:
	.4byte gInput
.L_0815ba28:
	ldr r3, [sp, #88]
	ldr r4, [sp, #56]
	ands r3, r4
	cmp r3, #0
	beq .L_0815ba3c
	ldr r7, [sp, #76]
	cmp r7, #1
	bne .L_0815ba3c
	movs r0, #0
	str r0, [sp, #56]
.L_0815ba3c:
	ldr r1, [sp, #88]
	cmp r1, #0
	bne .L_0815bb1c
	ldr r2, [sp, #60]
	cmp r2, #0
	bne .L_0815ba76
	movs r1, #185
	lsls r1, r1, #4
	adds r1, #255
	ldr r0, .L_0815bac8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #255
	ldr r0, .L_0815bacc
	bl Scheduler_AddOrUpdateCallback
	movs r1, #184
	lsls r1, r1, #6
	adds r1, #129
	movs r3, #0
	ldr r0, .L_0815bad0
	add r1, r11
	movs r2, #1
	bl Func_08157cf4
	movs r3, #1
	str r3, [sp, #60]
.L_0815ba76:
	ldr r2, .L_0815bad4
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #144
	strh r3, [r2, #4]
	ldr r3, .L_0815bab8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0815babc
	subs r2, #82
	strh r3, [r2]
	ldr r3, .L_0815bac0
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_0815bac4
	subs r2, #2
	strh r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
	str r3, [r2]
	ldr r0, .L_0815bad8
	b .L_0815badc
	.2byte 0x0000
.L_0815bab8:
	.4byte 0x00001010
.L_0815babc:
	.4byte 0x00007741
.L_0815bac0:
	.4byte 0x00000784
.L_0815bac4:
	.4byte 0x00001f80
.L_0815bac8:
	.4byte Func_08143264
.L_0815bacc:
	.4byte Func_08143424
.L_0815bad0:
	.4byte 0x000000c7
.L_0815bad4:
	.4byte Data_03001120
.L_0815bad8:
	.4byte 0x00000150
.L_0815badc:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0815bc28
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r4, #0
	mov r9, r4
	movs r7, #15
	movs r6, #0
	mov r5, r11
.L_0815baf8:
	str r6, [r5, #24]
	bl Random16
	ands r0, r7
	adds r0, #60
	str r0, [r5]
	bl Random16
	ands r0, r7
	adds r0, #44
	str r0, [r5, #4]
	movs r0, #1
	add r9, r0
	mov r1, r9
	subs r6, #2
	adds r5, #28
	cmp r1, #9
	bne .L_0815baf8
.L_0815bb1c:
	ldr r2, [sp, #88]
	cmp r2, #75
	bgt .L_0815bb7c
	adds r0, r2, #0
	cmp r2, #0
	bge .L_0815bb2a
	adds r0, #3
.L_0815bb2a:
	movs r1, #18
	asrs r0, r0, #2
	bl Math_Mod
	ldr r4, [sp, #88]
	movs r3, #215
	lsls r3, r3, #2
	adds r6, r0, #0
	adds r0, r4, #0
	muls r0, r3
	bl Trig_Sin
	lsls r0, r0, #3
	asrs r0, r0, #16
	movs r2, #60
	subs r2, r2, r0
	ldr r0, .L_0815bc2c
	lsls r1, r6, #1
	ldrh r1, [r0, r1]
	ldr r0, .L_0815bc30
	ldr r3, [sp, #88]
	ldrb r5, [r0, r6]
	subs r3, #8
	lsrs r0, r5, #1
	subs r2, r2, r0
	ldr r0, .L_0815bc34
	movs r7, #240
	ldrb r4, [r0, r6]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #72]
	lsls r7, r7, #5
	add r1, r11
	adds r7, #129
	ldr r4, [r0, #4]
	adds r1, r1, r7
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
.L_0815bb7c:
	ldr r3, [sp, #88]
	subs r3, #76
	cmp r3, #31
	bhi .L_0815bbe4
	movs r1, #0
	mov r9, r1
	mov r7, r11
.L_0815bb8a:
	ldr r0, [r7, #24]
	cmp r0, #17
	bhi .L_0815bbd4
	movs r1, #3
	bl Math_Div
	mov r2, r9
	movs r5, #1
	ands r5, r2
	ldr r2, .L_0815bc38
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r11
	adds r1, r1, r3
	ldr r3, .L_0815bc3c
	ldr r2, [r7]
	ldrb r6, [r3, r0]
	lsls r5, r5, #2
	lsrs r3, r6, #1
	subs r2, r2, r3
	ldr r3, .L_0815bc40
	ldrb r4, [r3, r0]
	ldr r3, [r7, #4]
	str r6, [sp, #0]
	adds r3, r3, r4
	ldr r4, .L_0815bc44
	subs r3, #32
	ldrb r0, [r4, r0]
	str r0, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [r5, r0]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r7, #24]
.L_0815bbd4:
	movs r1, #1
	add r9, r1
	adds r3, r0, #1
	mov r2, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #4
	bne .L_0815bb8a
.L_0815bbe4:
	ldr r3, [sp, #88]
	cmp r3, #108
	bne .L_0815bbf4
	movs r2, #128
	ldr r3, .L_0815bc24
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0815bbf4:
	ldr r4, [sp, #88]
	subs r4, #108
	str r4, [sp, #52]
	cmp r4, #8
	bhi .L_0815bc12
	ldr r7, [sp, #88]
	ldr r0, .L_0815bc48
	ldr r1, .L_0815bc24
	movs r3, #128
	lsls r2, r7, #9
	lsls r3, r3, #19
	adds r2, r2, r0
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_0815bc12:
	ldr r1, [sp, #12]
	cmp r1, #39
	bls .L_0815bc1a
	b .L_0815bd42
.L_0815bc1a:
	ldr r2, [sp, #16]
	ldr r7, .L_0815bc4c
	adds r0, r2, r1
	b .L_0815bc50
	.2byte 0x0000
.L_0815bc24:
	.4byte 0x00000010
.L_0815bc28:
	.4byte IwramCopyWords
.L_0815bc2c:
	.4byte Data_08198686
.L_0815bc30:
	.4byte Data_08198662
.L_0815bc34:
	.4byte Data_08198674
.L_0815bc38:
	.4byte Data_0819747a
.L_0815bc3c:
	.4byte Data_08197467
.L_0815bc40:
	.4byte Data_08197473
.L_0815bc44:
	.4byte Data_0819746d
.L_0815bc48:
	.4byte 0xffff2800
.L_0815bc4c:
	.4byte 0x000107ff
.L_0815bc50:
	lsls r0, r0, #3
	adds r0, r0, r1
	lsls r0, r0, #5
	bl Trig_Cos
	movs r1, #3
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #9
	movs r4, #1
	negs r4, r4
	adds r6, r0, r3
	mov r8, r4
	cmp r6, r7
	bgt .L_0815bc74
	movs r6, #132
	lsls r6, r6, #9
.L_0815bc74:
	movs r0, #128
	adds r1, r6, #0
	lsls r0, r0, #17
	bl Math_Div
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #216
	add r3, r11
	ldrh r2, [r3]
	strh r0, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #218
	add r3, r11
	ldrh r2, [r3]
	strh r0, [r3]
	movs r0, #184
	adds r1, r6, #0
	lsls r0, r0, #16
	bl Math_Div
	movs r5, #238
	movs r7, #64
	lsls r5, r5, #7
	adds r5, #208
	subs r0, r7, r0
	add r5, r11
	lsls r0, r0, #8
	str r0, [r5]
	movs r0, #152
	lsls r0, r0, #15
	adds r1, r6, #0
	bl Math_Div
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #212
	subs r0, r7, r0
	lsls r0, r0, #8
	add r5, r11
	str r0, [r5]
	ldr r0, [sp, #12]
	cmp r0, #15
	bgt .L_0815bcd2
	mov r8, r0
	b .L_0815bce0
.L_0815bcd2:
	ldr r3, [sp, #88]
	subs r3, #116
	cmp r3, #8
	bhi .L_0815bce0
	ldr r1, [sp, #16]
	subs r1, r7, r1
	mov r8, r1
.L_0815bce0:
	movs r2, #1
	negs r2, r2
	cmp r8, r2
	beq .L_0815bd34
	mov r4, r8
	movs r3, #0
	ldr r1, .L_0815bd60
	mov r9, r3
	mov r7, r8
	lsls r3, r4, #3
	adds r6, r3, r4
	movs r0, #0
	movs r4, #0
	lsls r5, r7, #1
.L_0815bcfc:
	lsls r3, r6, #2
	cmp r3, #0
	bge .L_0815bd04
	adds r3, #15
.L_0815bd04:
	asrs r3, r3, #4
	mov r2, r9
	muls r2, r3
	adds r3, r2, #0
	cmp r3, #0
	bge .L_0815bd12
	adds r3, #63
.L_0815bd12:
	asrs r2, r3, #6
	adds r3, r4, r0
	cmp r3, #0
	bge .L_0815bd1c
	adds r3, #63
.L_0815bd1c:
	asrs r3, r3, #6
	lsls r3, r3, #10
	orrs r3, r2
	strh r3, [r1]
	movs r3, #1
	add r9, r3
	mov r7, r9
	adds r1, #2
	adds r4, r4, r5
	add r0, r8
	cmp r7, #32
	bne .L_0815bcfc
.L_0815bd34:
	ldr r0, [sp, #88]
	cmp r0, #116
	bne .L_0815bd42
	ldr r3, .L_0815bd58
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
.L_0815bd42:
	ldr r3, [sp, #88]
	subs r3, #124
	cmp r3, #7
	bhi .L_0815bda2
	ldr r4, .L_0815bd5c
	movs r0, #160
	movs r1, #0
	lsls r0, r0, #19
	mov r9, r1
	b .L_0815bd64
	.2byte 0x0000
.L_0815bd58:
	.4byte 0x00007341
.L_0815bd5c:
	.4byte 0x0000001f
.L_0815bd60:
	.4byte 0x05000100
.L_0815bd64:
	ldrh r3, [r0]
	movs r2, #31
	ands r2, r3
	lsls r3, r3, #16
	subs r1, r2, #3
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r4
	ands r3, r4
	subs r2, #3
	subs r3, #3
	cmp r1, #0
	bge .L_0815bd80
	movs r1, #0
.L_0815bd80:
	cmp r2, #0
	bge .L_0815bd86
	movs r2, #0
.L_0815bd86:
	cmp r3, #0
	bge .L_0815bd8c
	movs r3, #0
.L_0815bd8c:
	lsls r2, r2, #5
	lsls r3, r3, #10
	orrs r3, r2
	movs r2, #1
	orrs r3, r1
	add r9, r2
	strh r3, [r0]
	mov r3, r9
	adds r0, #2
	cmp r3, #64
	bne .L_0815bd64
.L_0815bda2:
	ldr r4, [sp, #88]
	cmp r4, #132
	bne .L_0815be50
	ldr r7, [sp, #60]
	cmp r7, #1
	bne .L_0815bddc
	ldr r0, .L_0815be28
	bl Scheduler_RemoveCallback
	ldr r0, .L_0815be2c
	bl Scheduler_RemoveCallback
	movs r1, #185
	lsls r1, r1, #4
	adds r1, #255
	ldr r0, .L_0815be30
	bl Scheduler_AddOrUpdateCallback
	movs r1, #148
	lsls r1, r1, #6
	adds r1, #151
	ldr r0, .L_0815be34
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r0, #0
	str r0, [sp, #60]
.L_0815bddc:
	ldr r3, .L_0815be14
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_0815be38
	adds r2, #40
	str r3, [r2]
	ldr r3, .L_0815be3c
	adds r2, #4
	str r3, [r2]
	ldr r3, .L_0815be18
	subs r2, #12
	strh r3, [r2]
	ldr r3, .L_0815be1c
	adds r2, #6
	strh r3, [r2]
	ldr r3, .L_0815be20
	subs r2, #26
	strh r3, [r2]
	ldr r3, .L_0815be24
	adds r2, #70
	strh r3, [r2]
	ldr r0, .L_0815be40
	bl Resource_GetTableEntry
	adds r1, r0, #0
	b .L_0815be44
	.2byte 0x0000
.L_0815be14:
	.4byte 0x00007141
.L_0815be18:
	.4byte 0x000000cc
.L_0815be1c:
	.4byte 0x00000100
.L_0815be20:
	.4byte 0x00000784
.L_0815be24:
	.4byte 0x00001010
.L_0815be28:
	.4byte Func_08143264
.L_0815be2c:
	.4byte Func_08143424
.L_0815be30:
	.4byte Func_08143000
.L_0815be34:
	.4byte 0x0000015c
.L_0815be38:
	.4byte 0xfffff800
.L_0815be3c:
	.4byte 0xfffff000
.L_0815be40:
	.4byte 0x0000013a
.L_0815be44:
	movs r0, #160
	ldr r3, .L_0815be90
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0815be50:
	ldr r1, [sp, #88]
	cmp r1, #133
	bne .L_0815bf08
	movs r1, #240
	movs r2, #0
	ldr r3, .L_0815be94
	ldr r0, [sp, #96]
	lsls r1, r1, #6
	mov lr, r3
	.2byte 0xf800
	movs r1, #240
	ldr r3, .L_0815be98
	lsls r1, r1, #6
	ldr r0, .L_0815be9c
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0815be8c
	movs r2, #128
	movs r4, #164
	lsls r2, r2, #19
	lsls r4, r4, #7
	ldr r1, .L_0815bea0
	strh r3, [r2]
	adds r4, #152
	movs r2, #0
	mov r9, r2
	add r4, r11
	movs r0, #0
	mov r2, r11
	b .L_0815bea4
.L_0815be8c:
	.4byte 0x00007541
.L_0815be90:
	.4byte IwramCopyWords
.L_0815be94:
	.4byte IwramFillWords
.L_0815be98:
	.4byte IwramClearWords
.L_0815be9c:
	.4byte 0x06004000
.L_0815bea0:
	.4byte Data_081986aa
.L_0815bea4:
	movs r3, #0
	ldrsb r3, [r1, r3]
	str r0, [r2, #24]
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	subs r0, #8
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #2
	ldrsb r3, [r1, r3]
	lsls r3, r3, #6
	str r3, [r2, #8]
	movs r3, #3
	ldrsb r3, [r1, r3]
	lsls r3, r3, #15
	str r3, [r2, #12]
	movs r3, #4
	ldrsb r3, [r1, r3]
	adds r1, #5
	lsls r3, r3, #15
	str r3, [r2, #16]
	movs r3, #0
	str r3, [r2, #20]
	movs r3, #1
	add r9, r3
	mov r7, r9
	adds r2, #28
	cmp r7, #6
	bne .L_0815bea4
	movs r0, #0
	movs r2, #1
	adds r3, r4, #0
	mov r9, r0
	negs r2, r2
	adds r3, #24
.L_0815beee:
	movs r1, #1
	add r9, r1
	mov r4, r9
	str r2, [r3]
	adds r3, #28
	cmp r4, #96
	bne .L_0815beee
	ldr r0, .L_0815c0dc
	ldr r1, .L_0815c0e0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_0815bf08:
	ldr r7, [sp, #88]
	cmp r7, #76
	bne .L_0815bf14
	movs r0, #138
	bl Audio_PlayCue
.L_0815bf14:
	ldr r0, [sp, #88]
	movs r1, #133
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_0815bf24
	movs r0, #138
	bl Audio_PlayCue
.L_0815bf24:
	ldr r3, [sp, #88]
	subs r3, #133
	cmp r3, #132
	bls .L_0815bf2e
	b .L_0815c32a
.L_0815bf2e:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #48]
	movs r0, #1
	bl Func_081969f8
	movs r2, #164
	lsls r2, r2, #7
	ldr r3, [sp, #88]
	adds r2, #152
	add r2, r11
	str r0, [sp, #44]
	movs r5, #0
	str r2, [sp, #40]
	cmp r3, #196
	ble .L_0815bf56
	ldr r0, .L_0815c0e4
	bl Func_0815f0a0
.L_0815bf56:
	ldr r2, .L_0815c0e8
	ldr r3, [sp, #120]
	ldr r4, [sp, #44]
	ands r3, r2
	movs r2, #6
	orrs r3, r2
	ldr r2, .L_0815c0ec
	mov r7, sp
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #120]
	movs r3, #7
	str r3, [r4]
	ldr r3, .L_0815c0f0
	adds r7, #120
	str r5, [r4, #20]
	str r7, [sp, #36]
	str r7, [r4, #16]
	str r3, [r4, #8]
	ldr r0, [sp, #48]
	mov r2, r11
	adds r2, #224
	str r0, [r4, #12]
	str r2, [sp, #20]
	movs r1, #0
	mov r9, r1
	mov r6, r11
.L_0815bf90:
	mov r3, r9
	lsls r3, r3, #3
	str r3, [sp, #32]
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #0
	bne .L_0815bfa6
	movs r0, #138
	bl Audio_PlayCue
.L_0815bfa6:
	ldr r5, [r6, #24]
	cmp r5, #60
	bne .L_0815bfb4
	movs r0, #212
	bl Audio_PlayCue
	ldr r5, [r6, #24]
.L_0815bfb4:
	cmp r5, #64
	bne .L_0815bfc0
	movs r0, #134
	bl Audio_PlayCue
	ldr r5, [r6, #24]
.L_0815bfc0:
	cmp r5, #35
	bhi .L_0815c00a
	adds r0, r5, #0
	movs r1, #6
	bl Math_Div
	movs r1, #6
	bl Math_Mod
	ldr r2, .L_0815c0f4
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_0815c0f8
	movs r7, #2
	ldrsh r2, [r6, r7]
	ldrb r5, [r3, r0]
	movs r4, #224
	lsls r4, r4, #3
	lsrs r3, r5, #1
	add r1, r11
	adds r1, r1, r4
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r6, r4]
	ldr r4, .L_0815c0fc
	ldrb r4, [r4, r0]
	str r5, [sp, #0]
	adds r3, r3, r4
	ldr r4, .L_0815c100
	subs r3, #36
	ldrb r0, [r4, r0]
	ldr r4, [sp, #128]
	str r0, [sp, #4]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [r6, #24]
.L_0815c00a:
	adds r0, r5, #0
	subs r0, #12
	cmp r0, #31
	bhi .L_0815c058
	cmp r0, #0
	bge .L_0815c018
	adds r0, #3
.L_0815c018:
	movs r1, #18
	asrs r0, r0, #2
	bl Math_Mod
	ldr r2, .L_0815c104
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	movs r3, #2
	ldrsh r2, [r6, r3]
	ldr r3, .L_0815c108
	movs r7, #240
	ldrb r5, [r3, r0]
	lsls r7, r7, #5
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r6, r4]
	ldr r4, .L_0815c10c
	adds r7, #129
	ldrb r4, [r4, r0]
	add r1, r11
	adds r1, r1, r7
	ldr r7, [sp, #72]
	lsrs r0, r4, #1
	str r5, [sp, #0]
	str r4, [sp, #4]
	subs r3, r3, r0
	ldr r4, [r7, #4]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [r6, #24]
.L_0815c058:
	adds r0, r5, #0
	subs r0, #44
	cmp r0, #21
	bls .L_0815c062
	b .L_0815c182
.L_0815c062:
	lsls r0, r0, #10
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r0, r3, #4
	cmp r0, #0
	bge .L_0815c074
	adds r0, #63
.L_0815c074:
	ldr r3, .L_0815c110
	ldr r2, [r6]
	asrs r0, r0, #6
	mov r10, r0
	adds r0, r2, r3
	mov r8, r0
	ldr r0, [r6, #4]
	subs r5, #59
	adds r7, r0, r3
	cmp r5, #0
	bge .L_0815c08c
	movs r5, #0
.L_0815c08c:
	ldr r3, [r6, #16]
	ldr r1, [r6, #12]
	adds r3, r0, r3
	str r3, [r6, #4]
	ldr r3, .L_0815c114
	adds r2, r2, r1
	str r2, [r6]
	cmp r2, r3
	bgt .L_0815c0a6
	movs r4, #128
	lsls r4, r4, #8
	adds r3, r1, r4
	str r3, [r6, #12]
.L_0815c0a6:
	ldr r3, [r6, #4]
	ldr r0, .L_0815c118
	cmp r3, r0
	bgt .L_0815c0b8
	ldr r3, [r6, #16]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r3, r1
	b .L_0815c0be
.L_0815c0b8:
	ldr r3, [r6, #16]
	ldr r2, .L_0815c11c
	adds r3, r3, r2
.L_0815c0be:
	str r3, [r6, #16]
	ldr r3, [r6, #8]
	cmp r3, #0
	blt .L_0815c120
	ldr r0, [r6, #20]
	bl Trig_Sin
	lsls r3, r0, #1
	ldr r2, [r6, #8]
	adds r3, r3, r0
	lsls r3, r3, #8
	asrs r3, r3, #16
	adds r2, r2, r3
	b .L_0815c132
	.2byte 0x0000
.L_0815c0dc:
	.4byte 0x000000c8
.L_0815c0e0:
	.4byte gMapCellBuffer
.L_0815c0e4:
	.4byte 0x00000150
.L_0815c0e8:
	.4byte 0xffffff00
.L_0815c0ec:
	.4byte 0xffff00ff
.L_0815c0f0:
	.4byte Data_08199340
.L_0815c0f4:
	.4byte Data_0819747a
.L_0815c0f8:
	.4byte Data_08197467
.L_0815c0fc:
	.4byte Data_08197473
.L_0815c100:
	.4byte Data_0819746d
.L_0815c104:
	.4byte Data_08198686
.L_0815c108:
	.4byte Data_08198662
.L_0815c10c:
	.4byte Data_08198674
.L_0815c110:
	.4byte 0xffc00000
.L_0815c114:
	.4byte 0x003fffff
.L_0815c118:
	.4byte 0x003bffff
.L_0815c11c:
	.4byte 0xffff8000
.L_0815c120:
	ldr r0, [r6, #20]
	bl Trig_Sin
	lsls r3, r0, #1
	ldr r2, [r6, #8]
	adds r3, r3, r0
	lsls r3, r3, #8
	asrs r3, r3, #16
	subs r2, r2, r3
.L_0815c132:
	str r2, [r6, #8]
	ldr r3, [r6, #20]
	movs r4, #128
	lsls r4, r4, #3
	adds r3, r3, r4
	str r3, [r6, #20]
	ldr r0, [sp, #56]
	cmp r0, #0
	beq .L_0815c180
	ldr r1, .L_0815c418
	ldr r2, [sp, #36]
	lsls r3, r5, #12
	adds r3, r3, r1
	str r3, [r2, #4]
	bl Func_08014de4
	adds r1, r7, #0
	movs r2, #0
	mov r0, r8
	bl Func_08015160
	movs r0, #192
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #8]
	bl Func_08015068
	mov r0, r10
	bl Func_0801521c
	ldr r0, .L_0815c41c
	ldr r1, [sp, #48]
	movs r2, #4
	bl Func_08196958
	ldr r0, [sp, #44]
	bl Func_08196a7c
.L_0815c180:
	ldr r5, [r6, #24]
.L_0815c182:
	cmp r5, #64
	bne .L_0815c26a
	movs r4, #2
	ldrsh r3, [r6, r4]
	ldr r7, [sp, #20]
	movs r1, #0
	str r3, [r7]
	mov r10, r1
	movs r0, #6
	ldrsh r3, [r6, r0]
	str r3, [r7, #4]
	mov r3, r9
	lsls r2, r3, #4
	ldr r4, [sp, #40]
	lsls r3, r3, #7
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r5, r3, r4
.L_0815c1a6:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r7, r0, #0
	ands r7, r3
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	mov r8, r3
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #8
	str r3, [r5, #24]
	adds r0, r7, #0
	ldr r3, [r6]
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	cmp r3, #0
	bge .L_0815c1e4
	adds r3, #3
.L_0815c1e4:
	asrs r3, r3, #2
	str r3, [r5, #12]
	adds r0, r7, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	cmp r3, #0
	bge .L_0815c1f8
	adds r3, #3
.L_0815c1f8:
	movs r7, #1
	add r10, r7
	asrs r3, r3, #2
	mov r0, r10
	str r3, [r5, #16]
	adds r5, #28
	cmp r0, #16
	bne .L_0815c1a6
	ldr r2, [sp, #100]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r10, r1
	cmp r3, #0
	beq .L_0815c26a
	movs r7, #36
.L_0815c216:
	bl Random16
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #9
	lsls r5, r5, #2
	adds r5, r5, r3
	bl Random16
	ldr r4, [sp, #32]
	movs r3, #31
	ldr r1, [sp, #100]
	ands r3, r0
	adds r3, r4, r3
	adds r3, #52
	ldrsh r0, [r7, r1]
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #128
	movs r1, #1
	lsls r2, r2, #10
	lsls r3, r3, #12
	str r5, [sp, #0]
	bl Func_0815f000
	ldr r3, [sp, #100]
	movs r2, #1
	ldrsh r0, [r7, r3]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r10
	negs r2, r2
	bl Func_0814cd48
	ldr r1, [sp, #100]
	movs r0, #1
	ldr r3, [r1, #20]
	add r10, r0
	adds r7, #2
	cmp r10, r3
	bne .L_0815c216
.L_0815c26a:
	ldr r2, [sp, #56]
	cmp r2, #0
	beq .L_0815c2b0
	ldr r3, [r6, #24]
	subs r3, #64
	cmp r3, #11
	bhi .L_0815c2b0
	lsrs r0, r3, #31
	adds r0, r3, r0
	movs r1, #6
	asrs r0, r0, #1
	bl Math_Mod
	lsls r1, r0, #4
	ldr r4, [sp, #20]
	subs r1, r1, r0
	movs r3, #148
	lsls r1, r1, #7
	lsls r3, r3, #6
	adds r3, #151
	add r1, r11
	ldr r2, [r4]
	movs r0, #40
	adds r1, r1, r3
	ldr r7, [sp, #72]
	ldr r3, [r4, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	subs r2, #20
	subs r3, #24
	ldr r4, [r7, #4]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
.L_0815c2b0:
	ldr r0, [sp, #20]
	movs r1, #1
	add r9, r1
	adds r0, #28
	mov r2, r9
	str r0, [sp, #20]
	adds r6, #28
	cmp r2, #6
	beq .L_0815c2c4
	b .L_0815bf90
.L_0815c2c4:
	ldr r6, .L_0815c420
	ldr r5, [sp, #40]
	movs r3, #0
	mov r9, r3
.L_0815c2cc:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0815c312
	ldr r7, [sp, #56]
	asrs r3, r3, #2
	adds r4, r3, #2
	cmp r7, #0
	beq .L_0815c300
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #84]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #72]
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r4, [r0, #4]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
.L_0815c300:
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0815c312:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #96
	bne .L_0815c2cc
	ldr r0, [sp, #44]
	bl Sys_Free
	ldr r0, [sp, #48]
	bl Sys_Free
.L_0815c32a:
	ldr r3, [sp, #56]
	cmp r3, #0
	bne .L_0815c332
	b .L_0815c4a6
.L_0815c332:
	ldr r4, [sp, #88]
	cmp r4, #131
	ble .L_0815c33a
	b .L_0815c4a6
.L_0815c33a:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #28]
	movs r0, #1
	bl Func_081969f8
	movs r2, #239
	adds r6, r0, #0
	movs r3, #0
	lsls r2, r2, #7
	ldr r7, [sp, #88]
	add r2, r11
	str r3, [r6, #20]
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	str r3, [r2]
	ldr r3, [sp, #112]
	ldr r2, .L_0815c424
	movs r1, #7
	ands r3, r2
	ldr r2, .L_0815c428
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #112]
	movs r3, #184
	lsls r3, r3, #6
	adds r3, #129
	add r2, sp, #112
	add r3, r11
	str r3, [r2, #4]
	ldr r3, .L_0815c42c
	str r2, [r6, #16]
	str r3, [r6, #8]
	str r1, [r6]
	ldr r1, [sp, #28]
	ldr r3, .L_0815c430
	movs r2, #0
	str r1, [r6, #12]
	str r2, [sp, #24]
	lsls r2, r7, #9
	adds r3, r3, r2
	lsls r0, r7, #1
	ldr r4, .L_0815c434
	adds r0, r0, r7
	mov r9, r3
	ldr r1, .L_0815c438
	lsls r3, r7, #11
	ldr r5, [sp, #88]
	movs r7, #152
	negs r2, r2
	lsls r7, r7, #8
	adds r7, r7, r2
	adds r4, r4, r3
	lsls r0, r0, #10
	mov r8, r7
	mov r10, r4
	adds r7, r0, r1
	subs r5, #76
.L_0815c3c0:
	cmp r5, #0
	blt .L_0815c47c
	bl Func_08014de4
	movs r0, #128
	movs r2, #0
	lsls r0, r0, #12
	movs r1, #0
	bl Func_08015160
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_0815c404
	ldr r3, .L_0815c43c
	adds r0, r7, r3
	cmp r5, #39
	bgt .L_0815c3f8
	movs r4, #192
	lsls r0, r5, #11
	lsls r4, r4, #8
	cmp r0, r4
	ble .L_0815c3f8
	movs r0, #192
	lsls r0, r0, #8
.L_0815c3f8:
	bl Func_0801521c
	mov r0, r8
	bl Func_08015068
	b .L_0815c46c
.L_0815c404:
	cmp r5, #23
	ble .L_0815c440
	movs r3, #224
	lsls r3, r3, #9
	mov r1, r10
	subs r0, r3, r1
	bl Func_0801521c
	b .L_0815c466
	.2byte 0x0000
.L_0815c418:
	.4byte gMapCellBuffer
.L_0815c41c:
	.4byte Data_08199210
.L_0815c420:
	.4byte Data_08197424
.L_0815c424:
	.4byte 0xffffff00
.L_0815c428:
	.4byte 0xffff00ff
.L_0815c42c:
	.4byte Data_08199364
.L_0815c430:
	.4byte 0xffff6800
.L_0815c434:
	.4byte 0xfffda000
.L_0815c438:
	.4byte 0xfffc7000
.L_0815c43c:
	.4byte 0xfffee000
.L_0815c440:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #170
	muls r3, r5
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #240
	subs r0, r0, r3
	bl SceneTransform_ApplyPitch
	movs r3, #169
	adds r0, r5, #0
	muls r0, r3
	movs r2, #240
	lsls r2, r2, #8
	adds r2, #40
	adds r0, r0, r2
	bl Func_080150e4
.L_0815c466:
	mov r0, r9
	bl Func_08015068
.L_0815c46c:
	ldr r0, .L_0815c570
	ldr r1, [sp, #28]
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0815c47c:
	ldr r2, [sp, #24]
	ldr r3, .L_0815c574
	ldr r4, .L_0815c578
	ldr r1, .L_0815c57c
	movs r0, #128
	lsls r0, r0, #6
	adds r2, #1
	add r9, r3
	add r10, r4
	add r8, r0
	adds r7, r7, r1
	subs r5, #16
	str r2, [sp, #24]
	cmp r2, #2
	bne .L_0815c3c0
	adds r0, r6, #0
	bl Sys_Free
	ldr r0, [sp, #28]
	bl Sys_Free
.L_0815c4a6:
	ldr r3, [sp, #52]
	cmp r3, #179
	bhi .L_0815c538
	ldr r3, .L_0815c580
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #104]
	str r4, [sp, #108]
	ldr r4, [sp, #88]
	cmp r4, #131
	ble .L_0815c4e2
	adds r5, r4, #0
	subs r5, #132
	lsls r0, r5, #11
	bl Trig_Sin
	movs r7, #200
	lsls r0, r0, #1
	lsls r7, r7, #16
	lsls r5, r5, #10
	adds r7, r0, r7
	adds r0, r5, #0
	str r7, [sp, #64]
	bl Trig_Sin
	movs r1, #168
	lsls r0, r0, #1
	lsls r1, r1, #15
	adds r1, r0, r1
	str r1, [sp, #68]
.L_0815c4e2:
	ldr r2, [sp, #56]
	cmp r2, #0
	beq .L_0815c538
	add r2, sp, #136
	movs r3, #0
	movs r5, #238
	lsls r5, r5, #7
	str r3, [r2, #12]
	str r3, [r2, #4]
	ldr r7, .L_0815c584
	adds r5, #220
	mov r9, r3
	add r4, sp, #104
	adds r6, r2, #0
	add r5, r11
.L_0815c500:
	ldr r3, .L_0815c588
	mov r0, r9
	ldrb r3, [r3, r0]
	ldr r1, [sp, #64]
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r3, r3, r7
	str r3, [r6]
	ldr r3, .L_0815c58c
	ldr r2, [sp, #68]
	ldrb r3, [r3, r0]
	adds r1, r6, #0
	lsls r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r7
	str r3, [r6, #8]
	ldmia r5!, {r0}
	adds r2, r4, #0
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r3, #1
	add r9, r3
	mov r0, r9
	ldr r4, [sp, #8]
	cmp r0, #7
	bne .L_0815c500
.L_0815c538:
	ldr r1, [sp, #88]
	movs r2, #10
	adds r2, #255
	cmp r1, r2
	ble .L_0815c62e
	movs r3, #133
	lsls r3, r3, #1
	cmp r1, r3
	bne .L_0815c5c8
	ldr r3, .L_0815c590
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
	ldr r2, .L_0815c56c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #32
	strh r2, [r3]
	adds r3, #6
	strh r2, [r3]
	movs r4, #0
	mov r9, r4
	mov r5, r11
	b .L_0815c594
	.2byte 0x0000
.L_0815c56c:
	.4byte 0x00000100
.L_0815c570:
	.4byte Data_08199210
.L_0815c574:
	.4byte 0xffffe000
.L_0815c578:
	.4byte 0xffff8000
.L_0815c57c:
	.4byte 0xffff4000
.L_0815c580:
	.4byte Data_08196e54
.L_0815c584:
	.4byte 0xffe00000
.L_0815c588:
	.4byte Data_08198654
.L_0815c58c:
	.4byte Data_0819865b
.L_0815c590:
	.4byte 0xffff9000
.L_0815c594:
	bl Random16
	movs r1, #48
	bl Math_ModU
	adds r0, #40
	str r0, [r5]
	bl Random16
	movs r1, #96
	bl Math_ModU
	mov r7, r9
	adds r0, #16
	negs r3, r7
	str r0, [r5, #4]
	lsrs r2, r3, #31
	movs r0, #1
	adds r3, r3, r2
	add r9, r0
	asrs r3, r3, #1
	mov r1, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #32
	bne .L_0815c594
.L_0815c5c8:
	movs r2, #0
	mov r9, r2
	mov r6, r11
.L_0815c5ce:
	ldr r3, [r6, #24]
	cmp r3, #23
	bhi .L_0815c61e
	ldr r4, [sp, #56]
	cmp r4, #0
	beq .L_0815c61e
	adds r0, r3, #0
	cmp r3, #0
	bge .L_0815c5e2
	adds r0, r3, #3
.L_0815c5e2:
	movs r1, #6
	asrs r0, r0, #2
	bl Math_Mod
	ldr r2, .L_0815c700
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_0815c704
	ldr r2, [r6]
	ldrb r5, [r3, r0]
	movs r7, #224
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_0815c708
	add r1, r11
	ldrb r4, [r3, r0]
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	adds r3, r3, r4
	ldr r4, .L_0815c70c
	lsls r7, r7, #3
	ldrb r0, [r4, r0]
	subs r3, #48
	str r0, [sp, #4]
	adds r1, r1, r7
	ldr r4, [sp, #128]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
.L_0815c61e:
	movs r0, #1
	add r9, r0
	adds r3, #1
	mov r1, r9
	str r3, [r6, #24]
	adds r6, #28
	cmp r1, #32
	bne .L_0815c5ce
.L_0815c62e:
	ldr r2, [sp, #88]
	movs r3, #144
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_0815c67e
	movs r1, #144
	ldr r0, .L_0815c710
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #228
	movs r3, #188
	add r2, r11
	lsls r3, r3, #16
	str r3, [r2]
	movs r2, #221
	lsls r2, r2, #3
	movs r3, #192
	add r2, r11
	lsls r3, r3, #15
	str r3, [r2]
	movs r3, #222
	lsls r3, r3, #3
	movs r2, #0
	add r3, r11
	str r2, [r3]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #244
	add r3, r11
	str r2, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #140
	add r3, r11
	movs r4, #1
	str r2, [r3]
	str r4, [sp, #80]
.L_0815c67e:
	ldr r7, [sp, #56]
	cmp r7, #0
	beq .L_0815c69a
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
.L_0815c69a:
	ldr r0, [sp, #16]
	ldr r1, [sp, #12]
	ldr r2, [sp, #88]
	movs r3, #163
	adds r0, #2
	adds r1, #1
	adds r2, #1
	lsls r3, r3, #1
	str r0, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #88]
	cmp r2, r3
	beq .L_0815c6b8
	bl .L_0815b9f2
.L_0815c6b8:
	movs r0, #134
	bl Func_081180e8
	ldr r3, .L_0815c6fc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r6, #192
	lsls r6, r6, #18
	ldr r0, [r6, #96]
	ldr r5, .L_0815c714
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0815c718
	movs r1, #240
	lsls r1, r1, #6
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0815c71c
	movs r1, #240
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	b .L_0815c720
.L_0815c6fc:
	.4byte 0x00001f83
.L_0815c700:
	.4byte Data_0819747a
.L_0815c704:
	.4byte Data_08197467
.L_0815c708:
	.4byte Data_08197473
.L_0815c70c:
	.4byte Data_0819746d
.L_0815c710:
	.4byte Func_0815b68c
.L_0815c714:
	.4byte IwramFillWords
.L_0815c718:
	.4byte 0x06004000
.L_0815c71c:
	.4byte 0x06008000
.L_0815c720:
	bl WaitFrames
	ldr r3, .L_0815c760
	movs r2, #128
	lsls r2, r2, #19
	movs r4, #128
	adds r2, #82
	lsls r4, r4, #19
	adds r4, #80
	strh r3, [r2]
	ldr r3, .L_0815c764
	mov r9, r4
	mov r7, r9
	movs r0, #128
	strh r3, [r7]
	lsls r0, r0, #19
	ldr r3, .L_0815c768
	mov r8, r0
	mov r1, r8
	strh r3, [r1]
	ldr r0, .L_0815c76c
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #92]
	b .L_0815c770
	.2byte 0x0000
.L_0815c760:
	.4byte 0x0000100e
.L_0815c764:
	.4byte 0x00003f46
.L_0815c768:
	.4byte 0x00007741
.L_0815c76c:
	.4byte Func_08143000
.L_0815c770:
	movs r7, #0
	movs r0, #195
	str r7, [r2, #16]
	lsls r0, r0, #1
	ldr r6, [r6, #36]
	bl Audio_PlayCue
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #160
	add r3, r11
	ldr r5, .L_0815c800
	ldr r3, [r3]
	ldr r2, .L_0815c804
	strh r3, [r5, #4]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #164
	add r3, r11
	ldr r3, [r3]
	movs r0, #160
	strh r3, [r5, #6]
	lsls r0, r0, #19
	movs r3, #120
	movs r1, #128
	str r3, [r2, #12]
	str r3, [r2, #16]
	mov r10, r6
	lsls r1, r1, #1
	ldr r6, .L_0815c808
	adds r0, #192
	mov lr, r6
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	movs r3, #206
	lsls r3, r3, #3
	add r3, r10
	movs r2, #1
	ldrh r1, [r3]
	negs r2, r2
	movs r0, #2
	bl Func_08118040
	ldr r3, .L_0815c7fc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	movs r1, #240
	strh r3, [r2]
	lsls r1, r1, #6
	ldr r0, .L_0815c80c
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_0815c810
	bl Scheduler_RemoveCallback
	ldr r1, .L_0815c814
	movs r3, #32
	strh r3, [r5, #6]
	ldr r0, .L_0815c818
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0815c83c
	b .L_0815c81c
	.2byte 0x0000
.L_0815c7fc:
	.4byte 0x00000787
.L_0815c800:
	.4byte Data_03001120
.L_0815c804:
	.4byte gCameraSceneParameters
.L_0815c808:
	.4byte IwramClearWords
.L_0815c80c:
	.4byte 0x06004000
.L_0815c810:
	.4byte Func_08143488
.L_0815c814:
	.4byte Data_020038e0
.L_0815c818:
	.4byte 0x04000208
.L_0815c81c:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	strh r2, [r1]
	lsls r3, r3, #2
	movs r2, #230
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	mov r1, r8
	stmia r3!, {r1}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0815c83c:
	strh r4, [r0]
	mov r2, r9
	strh r7, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #0
	ldr r4, .L_0815c864
	str r3, [sp, #88]
	mov r8, r4
.L_0815c850:
	movs r6, #160
	lsls r6, r6, #3
	movs r7, #160
	adds r6, #108
	lsls r7, r7, #19
	movs r0, #0
	add r6, r10
	adds r7, #192
	mov r9, r0
	b .L_0815c868
.L_0815c864:
	.4byte 0x0000001f
.L_0815c868:
	ldrh r3, [r6]
	movs r5, #31
	ands r5, r3
	lsls r3, r3, #16
	mov r2, r8
	lsrs r1, r3, #21
	lsrs r0, r3, #26
	ldr r4, [sp, #88]
	ands r1, r2
	ands r0, r2
	ldr r2, [sp, #88]
	adds r3, r5, r4
	adds r4, r3, #0
	adds r3, r1, r2
	adds r2, r3, #0
	ldr r3, [sp, #88]
	subs r4, #30
	adds r3, r3, r0
	subs r2, #40
	subs r3, #30
	cmp r4, #0
	bge .L_0815c898
	movs r4, #0
	b .L_0815c89e
.L_0815c898:
	cmp r4, r5
	ble .L_0815c89e
	adds r4, r5, #0
.L_0815c89e:
	cmp r2, #0
	bge .L_0815c8a6
	movs r2, #0
	b .L_0815c8ac
.L_0815c8a6:
	cmp r2, r1
	ble .L_0815c8ac
	adds r2, r1, #0
.L_0815c8ac:
	cmp r3, #0
	bge .L_0815c8b4
	movs r3, #0
	b .L_0815c8ba
.L_0815c8b4:
	cmp r3, r0
	ble .L_0815c8ba
	adds r3, r0, #0
.L_0815c8ba:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r4
	movs r4, #1
	add r9, r4
	mov r0, r9
	strh r3, [r7]
	adds r6, #2
	adds r7, #2
	cmp r0, #128
	bne .L_0815c868
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #88]
	adds r1, #1
	str r1, [sp, #88]
	cmp r1, #40
	bne .L_0815c850
	ldr r1, .L_0815c94c
	ldr r0, .L_0815c950
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0815c914
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #234
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
.L_0815c914:
	strh r4, [r0]
	ldr r2, [sp, #80]
	cmp r2, #1
	bne .L_0815c922
	ldr r0, .L_0815c954
	bl Scheduler_RemoveCallback
.L_0815c922:
	movs r5, #238
	lsls r5, r5, #7
	movs r3, #0
	adds r5, #220
	mov r9, r3
	add r5, r11
.L_0815c92e:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r4, #1
	add r9, r4
	mov r7, r9
	cmp r7, #8
	bne .L_0815c92e
	add sp, #152
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0815c94c:
	.4byte Data_020038e0
.L_0815c950:
	.4byte 0x04000208
.L_0815c954:
	.4byte Func_0815b68c
