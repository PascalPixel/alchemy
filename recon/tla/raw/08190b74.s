.syntax unified
	.thumb
	.global Func_08190b74
	.thumb_func
Func_08190b74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #136
	str r0, [sp, #60]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r7, #216
	str r0, [sp, #56]
	movs r0, #0
	ldr r1, [r3, #96]
	lsls r7, r7, #6
	str r1, [sp, #52]
	adds r7, #16
	ldr r3, [r3, #100]
	str r3, [sp, #48]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08190be0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #48]
	ldr r0, .L_08190be4
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #56]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08190be8
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r6, [sp, #56]
	ldr r0, .L_08190bec
	adds r1, r6, r7
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	b .L_08190bf0
.L_08190be0:
	.4byte 0x00001010
.L_08190be4:
	.4byte 0x00000134
.L_08190be8:
	.4byte 0x00000102
.L_08190bec:
	.4byte 0x000000cd
.L_08190bf0:
	lsls r1, r1, #7
	adds r2, r6, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r6, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08190c1c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #60]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08190c20
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_08190c28
.L_08190c1c:
	.4byte Func_08143000
.L_08190c20:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_08190c28:
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	adds r5, #176
	str r3, [sp, #76]
	ldr r3, [sp, #60]
	movs r6, #0
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r3, .L_08190ce0
	str r0, [sp, #44]
	mov r7, sp
	ldr r5, [r5]
	mov r0, sp
	str r5, [sp, #40]
	mov r1, sp
	ldrh r3, [r3, #4]
	adds r7, #112
	adds r0, #124
	adds r1, #68
	str r3, [sp, #36]
	str r7, [sp, #32]
	str r0, [sp, #28]
	str r1, [sp, #24]
	mov r11, r6
.L_08190c5e:
	ldr r2, [sp, #60]
	ldr r1, [sp, #32]
	ldr r0, [r2, #8]
	bl Func_0815e20c
	ldr r6, [sp, #60]
	ldr r1, [sp, #28]
	movs r3, #36
	ldrsh r0, [r6, r3]
	mov r7, r11
	bl Func_0815e20c
	cmp r7, #0
	bne .L_08190cc8
	movs r1, #240
	ldr r0, [sp, #52]
	ldr r3, .L_08190ce4
	lsls r1, r1, #6
	ldr r2, .L_08190ce8
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_08190cec
	ldr r1, .L_08190cf0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_08190cf4
	movs r2, #0
	movs r3, #0
	ldr r1, .L_08190cf8
	bl Resource_LoadAndDecompress
	bl Func_0815b410
	ldr r3, .L_08190cd8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #56]
	movs r0, #0
	movs r2, #1
	mov r8, r0
	negs r2, r2
	adds r3, #24
.L_08190cba:
	movs r1, #1
	add r8, r1
	mov r6, r8
	str r2, [r3]
	adds r3, #28
	cmp r6, #64
	bne .L_08190cba
.L_08190cc8:
	mov r7, r11
	cmp r7, #2
	bne .L_08190d58
	ldr r1, [sp, #60]
	add r0, sp, #84
	ldr r3, [r1, #8]
	ldr r5, .L_08190cdc
	b .L_08190cfc
.L_08190cd8:
	.4byte 0x00001010
.L_08190cdc:
	.4byte 0x0000001f
.L_08190ce0:
	.4byte Data_03001120
.L_08190ce4:
	.4byte IwramFillWords
.L_08190ce8:
	.4byte 0x3f3f3f3f
.L_08190cec:
	.4byte 0x000000c2
.L_08190cf0:
	.4byte gMapCellBuffer
.L_08190cf4:
	.4byte 0x000000e9
.L_08190cf8:
	.4byte Data_02014000
.L_08190cfc:
	strh r3, [r0]
	ldrh r3, [r1, #36]
	movs r1, #0
	strh r3, [r0, #2]
	movs r3, #255
	strh r3, [r0, #4]
	bl BattleActor_SpawnObjectsForListFar
	movs r2, #0
	movs r0, #1
	ldr r1, .L_08191040
	bl Func_08118040
	bl Func_0817d6c4
	movs r4, #160
	lsls r4, r4, #19
	movs r2, #0
	adds r4, #192
	mov r8, r2
.L_08190d24:
	ldrh r3, [r4]
	movs r0, #31
	ands r0, r3
	lsls r3, r3, #16
	lsrs r1, r3, #26
	ands r1, r5
	cmp r0, #31
	ble .L_08190d36
	movs r0, #31
.L_08190d36:
	cmp r1, #31
	ble .L_08190d3c
	movs r1, #31
.L_08190d3c:
	lsrs r3, r0, #1
	lsls r3, r3, #5
	lsls r2, r0, #10
	orrs r2, r3
	movs r3, #1
	add r8, r3
	orrs r2, r1
	mov r6, r8
	strh r2, [r4]
	adds r4, #2
	cmp r6, #128
	bne .L_08190d24
	ldr r7, [sp, #40]
	str r3, [r7, #16]
.L_08190d58:
	mov r0, r11
	cmp r0, #1
	ble .L_08190d66
	ldr r2, .L_08191044
	ldrh r3, [r2, #4]
	adds r3, #8
	strh r3, [r2, #4]
.L_08190d66:
	mov r1, r11
	cmp r1, #8
	bne .L_08190da6
	movs r1, #240
	ldr r2, .L_08191048
	ldr r3, .L_0819104c
	ldr r0, [sp, #52]
	lsls r1, r1, #6
	mov lr, r3
	.2byte 0xf800
	ldr r5, [sp, #56]
	movs r2, #0
	mov r8, r2
.L_08190d80:
	bl Random16
	movs r1, #216
	bl Math_ModU
	subs r0, #96
	str r0, [r5]
	bl Random16
	movs r1, #216
	bl Math_ModU
	movs r3, #1
	add r8, r3
	mov r6, r8
	str r0, [r5, #4]
	adds r5, #28
	cmp r6, #64
	bne .L_08190d80
.L_08190da6:
	mov r7, r11
	cmp r7, #86
	bne .L_08190dbe
	ldr r2, [sp, #56]
	movs r3, #156
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, .L_08191050
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_08190dbe:
	mov r6, r11
	cmp r6, #84
	bne .L_08190dea
	movs r0, #212
	bl Audio_PlayCue
	movs r1, #2
	ldr r0, [sp, #44]
	bl Object_SetMode
	movs r1, #48
	ldr r0, [sp, #44]
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r7, [sp, #60]
	movs r3, #80
	movs r2, #36
	ldrsh r1, [r7, r2]
	ldr r0, [r7, #8]
	movs r2, #7
	bl Func_08157530
.L_08190dea:
	mov r3, r11
	cmp r3, #90
	bne .L_08190eca
	movs r1, #240
	ldr r3, .L_0819104c
	ldr r0, [sp, #52]
	lsls r1, r1, #6
	ldr r2, .L_08191048
	mov lr, r3
	.2byte 0xf800
	movs r0, #144
	bl Audio_PlayCue
	ldr r7, [sp, #60]
	movs r3, #16
	movs r6, #36
	ldrsh r0, [r7, r6]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r1, #238
	ldr r0, [sp, #56]
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #8
	str r3, [r2]
	ldr r3, [sp, #28]
	ldr r6, .L_08191054
	movs r2, #0
	mov r8, r2
	mov r10, r3
.L_08190e30:
	bl Random16
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r0
	adds r7, r3, #0
	bl Random16
	movs r3, #254
	adds r5, r0, #0
	ldr r0, [sp, #60]
	lsls r3, r3, #7
	adds r3, #255
	ands r5, r3
	ldr r3, [r0, #4]
	adds r7, #32
	cmp r3, #1
	bne .L_08190e5c
	movs r1, #128
	lsls r1, r1, #8
	adds r5, r5, r1
.L_08190e5c:
	mov r2, r10
	ldr r3, [r2]
	mov r0, r10
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [r0, #4]
	adds r0, r5, #0
	lsls r3, r3, #16
	str r3, [r6, #4]
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r6, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #5
	str r3, [r6, #16]
	bl Random16
	str r0, [r6, #8]
	bl Random16
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #3
	adds r3, r3, r1
	str r3, [r6, #20]
	bl Random16
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	movs r2, #128
	ands r3, r0
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r6, #24]
	movs r3, #1
	add r8, r3
	mov r7, r8
	adds r6, #28
	cmp r7, #128
	bne .L_08190e30
.L_08190eca:
	mov r0, r11
	cmp r0, #85
	ble .L_08190f62
	ldr r1, [sp, #60]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08190ef0
	movs r3, #176
	lsls r3, r3, #4
	lsls r2, r0, #5
	adds r3, #56
	subs r5, r3, r2
	movs r2, #38
	negs r2, r2
	cmp r5, r2
	bge .L_08190efe
	movs r5, #38
	negs r5, r5
	b .L_08190efe
.L_08190ef0:
	ldr r7, .L_08191058
	mov r6, r11
	lsls r3, r6, #5
	adds r5, r3, r7
	cmp r5, #0
	ble .L_08190efe
	movs r5, #0
.L_08190efe:
	movs r3, #158
	ldr r2, [sp, #56]
	str r3, [sp, #0]
	movs r3, #56
	str r3, [sp, #4]
	movs r3, #156
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r4, [sp, #76]
	movs r3, #8
	ldr r0, [sp, #52]
	adds r2, r5, #0
	mov lr, r4
	.2byte 0xf800
	ldr r6, [sp, #60]
	ldr r3, [r6, #4]
	cmp r3, #0
	bne .L_08190f2c
	movs r0, #188
	movs r1, #27
	bl Func_081963ec
	b .L_08190f34
.L_08190f2c:
	movs r0, #188
	movs r1, #31
	bl Func_081963ec
.L_08190f34:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	add r0, sp, #76
	str r3, [r0, #4]
	movs r3, #158
	str r3, [sp, #0]
	movs r3, #56
	str r3, [sp, #4]
	ldr r7, [sp, #56]
	movs r2, #156
	lsls r2, r2, #5
	ldr r4, [r0, #4]
	adds r1, r7, r2
	ldr r0, [sp, #52]
	adds r2, r5, #0
	movs r3, #64
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_08190f62:
	mov r3, r11
	cmp r3, #89
	bgt .L_08190fbe
	movs r6, #0
	mov r8, r6
	ldr r5, [sp, #56]
	ldr r6, .L_0819105c
.L_08190f70:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08190fb2
	asrs r0, r0, #2
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #48]
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
	ldr r0, [sp, #52]
	ldr r4, [sp, #76]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08190fb2:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_08190f70
.L_08190fbe:
	mov r6, r11
	lsls r3, r6, #2
	add r3, r11
	lsls r3, r3, #1
	movs r2, #0
	subs r3, #240
	movs r7, #24
	str r3, [sp, #16]
	str r7, [sp, #12]
	str r2, [sp, #8]
	mov r8, r2
.L_08190fd4:
	mov r0, r8
	lsls r5, r0, #4
	adds r3, r5, #0
	adds r3, #28
	cmp r11, r3
	bne .L_08190fe6
	movs r0, #212
	bl Audio_PlayCue
.L_08190fe6:
	adds r3, r5, #0
	adds r3, #32
	cmp r11, r3
	bne .L_081910de
	ldr r6, [sp, #8]
	ldr r2, [sp, #28]
	lsls r3, r6, #3
	ldr r7, [sp, #56]
	subs r3, r3, r6
	movs r1, #0
	lsls r3, r3, #2
	mov r9, r1
	mov r10, r2
	adds r6, r3, r7
.L_08191002:
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r7, r3, #0
	bl Random16
	movs r3, #252
	adds r5, r0, #0
	ldr r0, [sp, #60]
	lsls r3, r3, #6
	adds r3, #255
	ands r5, r3
	ldr r3, [r0, #4]
	adds r7, #128
	cmp r3, #0
	bne .L_08191032
	mov r1, r8
	cmp r1, #0
	beq .L_0819106a
	movs r2, #128
	lsls r2, r2, #7
	adds r5, r5, r2
	b .L_0819106a
.L_08191032:
	mov r3, r8
	cmp r3, #0
	bne .L_08191064
	ldr r0, .L_08191060
	adds r5, r5, r0
	b .L_0819106a
	.2byte 0x0000
.L_08191040:
	.4byte 0x00000072
.L_08191044:
	.4byte Data_03001120
.L_08191048:
	.4byte 0x3f3f3f3f
.L_0819104c:
	.4byte IwramFillWords
.L_08191050:
	.4byte 0x00000101
.L_08191054:
	.4byte Data_02016000
.L_08191058:
	.4byte 0xfffff4a2
.L_0819105c:
	.4byte Data_08197410
.L_08191060:
	.4byte 0xffffc000
.L_08191064:
	movs r1, #128
	lsls r1, r1, #8
	adds r5, r5, r1
.L_0819106a:
	mov r2, r10
	ldr r3, [r2]
	mov r0, r10
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [r0, #4]
	adds r0, r5, #0
	lsls r3, r3, #16
	str r3, [r6, #4]
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r6, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #5
	str r3, [r6, #16]
	bl Random16
	movs r3, #7
	movs r1, #1
	ands r3, r0
	add r9, r1
	adds r3, #17
	mov r2, r9
	str r3, [r6, #24]
	adds r6, #28
	cmp r2, #16
	bne .L_08191002
	movs r0, #133
	bl Audio_PlayCue
	ldr r6, [sp, #60]
	movs r2, #5
	movs r3, #36
	ldrsh r0, [r6, r3]
	movs r3, #64
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	movs r0, #238
	ldr r7, [sp, #56]
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r7, r0
	movs r3, #4
	str r3, [r2]
.L_081910de:
	ldr r1, [sp, #12]
	cmp r11, r1
	blt .L_081911b2
	mov r2, r11
	cmp r2, #89
	bgt .L_081911b2
	subs r1, r2, r1
	cmp r1, #8
	bgt .L_08191138
	ldr r3, [sp, #60]
	ldr r4, [r3, #4]
	cmp r4, #0
	bne .L_0819110c
	ldr r6, [sp, #28]
	ldr r7, [sp, #16]
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, r3, r7
	adds r6, r3, #0
	adds r6, #80
	b .L_08191122
.L_0819110c:
	ldr r0, [sp, #28]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	lsls r2, r1, #2
	adds r2, r2, r1
	asrs r3, r3, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r6, r3, #0
	subs r6, #80
.L_08191122:
	mov r2, r8
	cmp r2, #0
	bne .L_08191130
	lsls r3, r1, #4
	adds r5, r3, #0
	subs r5, #120
	b .L_0819115a
.L_08191130:
	lsls r2, r1, #4
	movs r3, #168
	subs r5, r3, r2
	b .L_0819115a
.L_08191138:
	ldr r3, [sp, #60]
	ldr r4, [r3, #4]
	cmp r4, #0
	bne .L_08191146
	ldr r6, [sp, #28]
	ldr r3, [r6]
	b .L_0819114a
.L_08191146:
	ldr r7, [sp, #28]
	ldr r3, [r7]
.L_0819114a:
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r6, r3, #1
	mov r0, r8
	movs r5, #8
	cmp r0, #0
	beq .L_0819115a
	movs r5, #40
.L_0819115a:
	mov r1, r8
	add r0, sp, #76
	cmp r1, #1
	bne .L_08191184
	cmp r4, #0
	bne .L_08191170
	movs r0, #188
	movs r1, #27
	bl Func_081963ec
	b .L_08191178
.L_08191170:
	movs r0, #188
	movs r1, #31
	bl Func_081963ec
.L_08191178:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	add r0, sp, #76
	str r3, [r0, #4]
.L_08191184:
	movs r3, #40
	str r3, [sp, #0]
	movs r3, #80
	str r3, [sp, #4]
	mov r2, r8
	ldr r3, [sp, #56]
	lsls r1, r2, #2
	adds r2, r6, #0
	movs r6, #224
	lsls r6, r6, #3
	ldr r4, [r1, r0]
	subs r2, #20
	adds r1, r3, r6
	ldr r0, [sp, #52]
	adds r3, r5, #0
	mov r7, r8
	mov lr, r4
	.2byte 0xf800
	cmp r7, #1
	bne .L_081911b2
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_081911b2:
	ldr r0, [sp, #16]
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	movs r3, #1
	add r8, r3
	subs r0, #160
	adds r1, #16
	adds r2, #32
	mov r6, r8
	str r0, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #8]
	cmp r6, #2
	beq .L_081911d0
	b .L_08190fd4
.L_081911d0:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_081914f8
	ldr r3, [sp, #68]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_081914fc
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #68]
	ldr r3, .L_08191500
	adds r7, r0, #0
	ldr r2, [sp, #24]
	ldr r0, .L_08191504
	movs r6, #0
	str r3, [r7, #8]
	mov r3, r9
	str r0, [r2, #4]
	str r1, [r7]
	str r2, [r7, #16]
	str r3, [r7, #12]
	str r6, [sp, #20]
	mov r8, r6
	mov r10, r6
.L_08191210:
	ldr r6, [sp, #20]
	adds r6, #2
	cmp r11, r6
	bne .L_0819121e
	movs r0, #212
	bl Audio_PlayCue
.L_0819121e:
	cmp r11, r6
	blt .L_08191292
	ldr r3, [sp, #20]
	adds r3, #18
	cmp r11, r3
	bge .L_08191292
	mov r0, r11
	subs r5, r0, r6
	lsls r2, r5, #3
	movs r3, #64
	subs r3, r3, r2
	cmp r3, #0
	ble .L_0819123a
	movs r3, #0
.L_0819123a:
	movs r1, #64
	negs r1, r1
	cmp r3, r1
	ble .L_08191292
	movs r2, #128
	lsls r2, r2, #7
	str r3, [r7, #20]
	lsls r5, r5, #14
	adds r5, r5, r2
	bl Func_08014de4
	ldr r3, [sp, #32]
	movs r1, #0
	ldr r0, [r3]
	movs r2, #0
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r0, r0, #16
	bl Func_08015160
	lsls r1, r5, #1
	adds r0, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r2, .L_08191508
	mov r6, r10
	ldrsh r0, [r6, r2]
	bl SceneTransform_ApplyPitch
	ldr r3, .L_0819150c
	ldrsh r0, [r3, r6]
	bl Func_08015068
	ldr r0, .L_08191510
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08191292:
	mov r2, r8
	lsls r5, r2, #3
	adds r3, r5, #0
	adds r6, r5, #0
	adds r3, #68
	adds r6, #64
	cmp r11, r3
	bne .L_081912a8
	movs r0, #212
	bl Audio_PlayCue
.L_081912a8:
	adds r3, r5, #0
	adds r3, #72
	cmp r11, r3
	bne .L_081912c6
	ldr r3, [sp, #60]
	movs r1, #0
	movs r2, #1
	ldr r0, [r3, #8]
	negs r2, r2
	str r1, [sp, #0]
	movs r3, #0
	movs r1, #7
	bl Func_0814cd48
	b .L_081912e0
.L_081912c6:
	adds r3, r5, #0
	adds r3, #76
	cmp r11, r3
	bne .L_081912e0
	ldr r2, [sp, #60]
	movs r3, #0
	ldr r0, [r2, #8]
	movs r2, #1
	movs r1, #0
	negs r2, r2
	str r3, [sp, #0]
	bl Func_0814cd48
.L_081912e0:
	cmp r11, r6
	blt .L_08191344
	adds r3, r6, #0
	adds r3, #11
	cmp r11, r3
	bge .L_08191344
	mov r0, r11
	subs r3, r0, r6
	movs r1, #0
	movs r5, #192
	lsls r3, r3, #14
	str r1, [r7, #20]
	lsls r5, r5, #10
	subs r5, r5, r3
	bl Func_08014de4
	ldr r2, [sp, #32]
	movs r1, #0
	ldr r0, [r2]
	movs r2, #0
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r0, r0, #16
	bl Func_08015160
	adds r2, r5, #0
	lsls r1, r5, #1
	adds r0, r5, #0
	bl Func_080151e4
	ldr r1, .L_08191508
	mov r3, r10
	ldrsh r0, [r3, r1]
	bl SceneTransform_ApplyPitch
	ldr r3, .L_0819150c
	mov r2, r10
	ldrsh r0, [r3, r2]
	bl Func_08015068
	ldr r0, .L_08191510
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08191344:
	mov r0, r8
	lsls r3, r0, #4
	adds r6, r3, #0
	adds r6, #32
	cmp r11, r6
	blt .L_081913c0
	adds r3, #48
	cmp r11, r3
	bge .L_081913c0
	mov r1, r11
	subs r5, r1, r6
	lsls r2, r5, #3
	movs r3, #64
	subs r3, r3, r2
	cmp r3, #0
	ble .L_08191366
	movs r3, #0
.L_08191366:
	movs r2, #64
	negs r2, r2
	cmp r3, r2
	ble .L_081913c0
	str r3, [r7, #20]
	bl Func_08014de4
	ldr r1, [sp, #28]
	movs r6, #128
	ldr r0, [r1]
	lsls r6, r6, #7
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	lsls r5, r5, #14
	subs r0, #64
	adds r5, r5, r6
	lsls r0, r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r0, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r6, .L_08191508
	mov r2, r10
	ldrsh r0, [r2, r6]
	bl SceneTransform_ApplyPitch
	ldr r3, .L_0819150c
	mov r1, r10
	ldrsh r0, [r3, r1]
	bl Func_08015068
	ldr r0, .L_08191510
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_081913c0:
	ldr r6, [sp, #20]
	movs r0, #1
	add r8, r0
	movs r3, #2
	adds r6, #6
	mov r1, r8
	add r10, r3
	str r6, [sp, #20]
	cmp r1, #2
	beq .L_081913d6
	b .L_08191210
.L_081913d6:
	mov r3, r11
	movs r1, #1
	movs r2, #3
	cmp r3, #89
	ble .L_0819147a
	ldr r3, .L_08191514
	mov r6, r11
	str r3, [r7, #8]
	str r2, [r7]
	cmp r6, #99
	ble .L_0819140c
	ldr r0, [sp, #56]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r0, r2
	str r1, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	ldr r3, .L_08191518
	movs r0, #2
	str r3, [r2]
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
.L_0819140c:
	movs r6, #0
	ldr r5, .L_0819151c
	mov r8, r6
	mov r6, sp
	adds r6, #67
.L_08191416:
	movs r2, #7
	mov r3, r8
	ands r3, r2
	adds r3, #32
	strb r3, [r6]
	mov r3, sp
	adds r3, #67
	adds r6, r3, #0
	str r6, [r7, #20]
	bl Func_08014de4
	ldr r3, .L_08191520
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	ldr r2, [r5, #24]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r1, r0, #1
	bl Func_080151e4
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	bl Func_08015068
	mov r1, r9
	movs r2, #3
	ldr r0, .L_08191524
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	adds r0, r5, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #128
	bne .L_08191416
.L_0819147a:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r9
	bl Sys_Free
	movs r0, #8
	bl Func_08158d68
	bl Func_081434f8
	movs r6, #240
	ldr r3, [sp, #56]
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r3, r6
	movs r7, #1
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	add r11, r7
	bl WaitFrames
	mov r0, r11
	cmp r0, #114
	beq .L_081914b2
	bl .L_08190c5e
.L_081914b2:
	bl BattleActor_CommitPlacementFar
	add r2, sp, #36
	ldr r3, .L_08191528
	ldrh r2, [r2]
	ldr r1, [sp, #40]
	strh r2, [r3, #4]
	movs r3, #192
	lsls r3, r3, #18
	movs r5, #0
	ldr r3, [r3, #36]
	str r5, [r1, #16]
	movs r6, #206
	lsls r6, r6, #3
	adds r3, r3, r6
	ldrh r1, [r3]
	movs r2, #24
	movs r0, #1
	bl Func_08118040
	ldr r0, .L_0819152c
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #136
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081914f8:
	.4byte 0xffffff00
.L_081914fc:
	.4byte 0xffff00ff
.L_08191500:
	.4byte Data_08199364
.L_08191504:
	.4byte gMapCellBuffer
.L_08191508:
	.4byte Data_08199f2a
.L_0819150c:
	.4byte Data_08199f2e
.L_08191510:
	.4byte Data_081991e0
.L_08191514:
	.4byte Data_0819919c
.L_08191518:
	.4byte Data_02020202
.L_0819151c:
	.4byte Data_02016000
.L_08191520:
	.4byte 0xffc00000
.L_08191524:
	.4byte Data_081991a4
.L_08191528:
	.4byte Data_03001120
.L_0819152c:
	.4byte Func_08143000
