.syntax unified
	.thumb
	.global Func_08192c9c
	.thumb_func
Func_08192c9c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #196
	sub sp, #420
	lsls r0, r0, #1
	add r0, sp
	str r0, [sp, #176]
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #96]
	movs r2, #202
	lsls r2, r2, #1
	add r2, sp
	str r3, [r0]
	str r2, [sp, #64]
	adds r7, r2, #0
	movs r4, #206
	ldr r2, [r1, #92]
	lsls r4, r4, #1
	add r4, sp
	adds r3, r7, #0
	str r2, [r3]
	str r4, [sp, #180]
	ldr r3, .L_08192d50
	ldr r5, [r1, #100]
	movs r0, #208
	lsls r0, r0, #1
	add r0, sp
	str r3, [r4]
	str r5, [sp, #204]
	str r0, [sp, #184]
	ldr r3, .L_08192d54
	movs r4, #240
	str r3, [r0]
	ldr r3, [r1, #36]
	lsls r4, r4, #7
	str r3, [sp, #200]
	ldr r3, .L_08192d58
	adds r4, #240
	ldrh r3, [r3, #4]
	adds r2, r2, r4
	str r3, [sp, #196]
	movs r0, #128
	ldr r1, [r1, #48]
	lsls r0, r0, #6
	str r1, [sp, #192]
	str r6, [r2]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	ldr r2, .L_08192d48
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r5, [sp, #64]
	movs r0, #239
	ldr r2, [r5]
	lsls r0, r0, #7
	adds r1, r2, r0
	movs r3, #0
	str r3, [r1]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r2, r1
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08192d5c
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_08192d4c
	movs r2, #128
	lsls r2, r2, #19
	b .L_08192d60
	.2byte 0x0000
.L_08192d48:
	.4byte 0x00000000
.L_08192d4c:
	.4byte 0x00007741
.L_08192d50:
	.4byte gMapCellBuffer
.L_08192d54:
	.4byte Data_02011f40
.L_08192d58:
	.4byte Data_03001120
.L_08192d5c:
	.4byte Func_08143000
.L_08192d60:
	strh r3, [r2]
	ldr r3, .L_08192d9c
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_08192da0
	adds r2, #20
	strh r3, [r2]
	ldr r3, .L_08192da4
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08192da8
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	ldr r2, .L_08192dac
	movs r1, #4
	bl Func_08191958
	ldr r2, [sp, #176]
	movs r1, #128
	ldr r0, [r2]
	ldr r3, .L_08192db0
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #19
	b .L_08192db4
	.2byte 0x0000
.L_08192d9c:
	.4byte 0x00000784
.L_08192da0:
	.4byte 0x00000080
.L_08192da4:
	.4byte 0x00001010
.L_08192da8:
	.4byte 0x00003f44
.L_08192dac:
	.4byte Func_0819253c
.L_08192db0:
	.4byte IwramFillWords
.L_08192db4:
	movs r3, #128
	adds r2, #212
	lsls r3, r3, #24
.L_08192dba:
	ldr r5, [r2, #8]
	ands r5, r3
	cmp r5, #0
	bne .L_08192dba
	ldr r3, [r7]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r3, r3, r4
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_08192e10
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	str r5, [r3]
	ldr r3, .L_08192e14
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08192e18
	subs r2, #2
	strh r3, [r2]
	ldr r2, [r7]
	movs r5, #239
	movs r0, #238
	lsls r5, r5, #7
	lsls r0, r0, #7
	adds r1, r2, r5
	movs r3, #2
	adds r0, #132
	str r3, [r1]
	adds r2, r2, r0
	movs r3, #75
	str r3, [r2]
	ldr r1, [sp, #192]
	b .L_08192e1c
.L_08192e10:
	.4byte 0x00000080
.L_08192e14:
	.4byte 0x00001010
.L_08192e18:
	.4byte 0x00003f44
.L_08192e1c:
	movs r2, #54
	ldrsh r1, [r1, r2]
	str r1, [sp, #188]
	ldr r3, [r6, #4]
	cmp r3, #0
	bne .L_08192e32
	ldr r2, [sp, #192]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r2, #54]
	b .L_08192e3a
.L_08192e32:
	ldr r4, [sp, #192]
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r4, #54]
.L_08192e3a:
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r1, #3
	movs r0, #188
	bl Func_081963ec
	movs r6, #198
	lsls r6, r6, #1
	add r6, sp
	adds r5, r6, #0
	str r5, [sp, #172]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #104]
	adds r3, #188
	str r2, [r6]
	ldr r5, .L_08192ee4
	ldr r3, [r3]
	adds r0, r5, #0
	str r3, [r6, #4]
	ldr r1, .L_08192ee8
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	adds r0, r5, #0
	ldr r1, [sp, #204]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r1, #192
	ldr r2, [sp, #204]
	movs r0, #0
	lsls r1, r1, #2
	mov r10, r0
	adds r1, #2
.L_08192e88:
	ldrb r3, [r2]
	lsrs r3, r3, #1
	strb r3, [r2]
	movs r3, #1
	add r10, r3
	adds r2, #1
	cmp r10, r1
	bne .L_08192e88
	ldr r0, [r7]
	movs r4, #224
	lsls r4, r4, #3
	movs r2, #128
	adds r0, r0, r4
	lsls r2, r2, #9
	movs r1, #32
	bl Func_0815b434
	bl Func_0815b410
	movs r5, #204
	lsls r5, r5, #1
	add r5, sp
	movs r3, #0
	str r5, [sp, #168]
	movs r0, #188
	str r3, [r5]
	bl Runtime_ReleaseHeapBlock
	bl Func_08014de4
	ldr r6, [sp, #184]
	str r6, [sp, #164]
.L_08192ec8:
	ldr r3, .L_08192eec
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08192ed6
	b .L_08192ff4
.L_08192ed6:
	ldr r0, [sp, #168]
	ldr r3, [r0]
	cmp r3, #175
	bgt .L_08192ef0
	movs r3, #176
	str r3, [r0]
	b .L_08192ff4
.L_08192ee4:
	.4byte 0x00000134
.L_08192ee8:
	.4byte Data_02010578
.L_08192eec:
	.4byte gInput
.L_08192ef0:
	movs r1, #134
	subs r3, #177
	lsls r1, r1, #1
	cmp r3, r1
	bhi .L_08192fc2
	ldr r3, .L_08192f34
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r2, [sp, #176]
	movs r1, #128
	ldr r0, [r2]
	lsls r1, r1, #7
	ldr r2, .L_08192f38
	ldr r3, .L_08192f3c
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #64]
	movs r5, #240
	ldr r3, [r4]
	lsls r5, r5, #7
	adds r5, #232
	adds r3, r3, r5
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #168]
	movs r0, #193
	ldr r3, [r6]
	lsls r0, r0, #1
	b .L_08192f40
.L_08192f34:
	.4byte 0x00001010
.L_08192f38:
	.4byte 0x3f3f3f3f
.L_08192f3c:
	.4byte IwramFillWords
.L_08192f40:
	cmp r3, r0
	bgt .L_08192fb8
	movs r5, #238
	movs r1, #0
	lsls r5, r5, #7
	mov r10, r1
	adds r5, #220
.L_08192f4e:
	ldr r2, [sp, #64]
	ldr r3, [r2]
	ldr r0, [r3, r5]
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #4
	cmp r4, #16
	bne .L_08192f4e
	ldr r5, [sp, #64]
	movs r6, #240
	ldr r3, [r5]
	lsls r6, r6, #7
	adds r6, #240
	ldr r0, [r3, r6]
	bl Func_0814cc4c
	bl Func_08014c4c
	ldr r3, [r5]
	movs r0, #0
	ldr r3, [r3, r6]
	mov r10, r0
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_08192fb8
	ldr r5, [sp, #64]
	movs r7, #1
	adds r4, r6, #0
	negs r7, r7
	movs r6, #36
.L_08192f90:
	ldr r3, [r5]
	movs r2, #5
	ldr r3, [r3, r4]
	str r4, [sp, #16]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	adds r1, r7, #0
	adds r3, r7, #0
	bl Func_0814cd48
	ldr r3, [r5]
	ldr r4, [sp, #16]
	movs r2, #1
	ldr r3, [r3, r4]
	add r10, r2
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r10, r3
	bne .L_08192f90
.L_08192fb8:
	ldr r4, [sp, #168]
	movs r3, #223
	lsls r3, r3, #1
	str r3, [r4]
	b .L_08192ff4
.L_08192fc2:
	ldr r5, [sp, #176]
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_08192fec
	ldr r0, [r5]
	ldr r6, .L_08192ff0
	mov lr, r6
	.2byte 0xf800
	ldr r0, [sp, #64]
	movs r1, #240
	ldr r3, [r0]
	lsls r1, r1, #7
	adds r1, #232
	adds r3, r3, r1
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	bl .L_081956a0
.L_08192fec:
	.4byte 0x3f3f3f3f
.L_08192ff0:
	.4byte IwramFillWords
.L_08192ff4:
	ldr r2, [sp, #168]
	movs r4, #192
	ldr r3, [r2]
	lsls r4, r4, #2
	adds r4, #82
	cmp r3, r4
	bne .L_08193006
	bl .L_081956a0
.L_08193006:
	cmp r3, #0
	bne .L_08193010
	movs r0, #206
	bl Audio_PlayCue
.L_08193010:
	ldr r5, .L_08193300
	adds r6, r5, #0
	adds r6, #11
.L_08193016:
	ldr r0, [sp, #168]
	ldrb r2, [r5]
	ldr r3, [r0]
	adds r5, #1
	cmp r3, r2
	bne .L_08193028
	movs r0, #130
	bl Audio_PlayCue
.L_08193028:
	cmp r5, r6
	bne .L_08193016
	ldr r1, [sp, #168]
	ldr r3, [r1]
	cmp r3, #175
	bne .L_0819303a
	movs r0, #212
	bl Audio_PlayCue
.L_0819303a:
	ldr r5, .L_08193304
	movs r2, #0
	mov r10, r2
.L_08193040:
	ldr r4, [sp, #168]
	ldrh r2, [r5]
	ldr r3, [r4]
	adds r5, #2
	cmp r3, r2
	bne .L_08193052
	movs r0, #209
	bl Audio_PlayCue
.L_08193052:
	movs r6, #1
	add r10, r6
	mov r0, r10
	cmp r0, #5
	bne .L_08193040
	ldr r5, .L_08193308
	movs r1, #0
	mov r10, r1
.L_08193062:
	ldr r4, [sp, #168]
	ldrh r2, [r5]
	ldr r3, [r4]
	adds r5, #2
	cmp r3, r2
	bne .L_08193074
	movs r0, #212
	bl Audio_PlayCue
.L_08193074:
	movs r6, #1
	add r10, r6
	mov r0, r10
	cmp r0, #6
	bne .L_08193062
	ldr r1, [sp, #168]
	movs r2, #114
	ldr r3, [r1]
	adds r2, #255
	cmp r3, r2
	bne .L_08193090
	movs r0, #206
	bl Audio_PlayCue
.L_08193090:
	movs r3, #0
	movs r5, #161
	mov r10, r3
	lsls r5, r5, #2
.L_08193098:
	ldr r4, [sp, #168]
	ldr r3, [r4]
	cmp r3, r5
	bne .L_081930a6
	movs r0, #104
	bl Audio_PlayCue
.L_081930a6:
	movs r6, #1
	add r10, r6
	mov r0, r10
	adds r5, #12
	cmp r0, #6
	bne .L_08193098
	ldr r1, [sp, #168]
	movs r2, #250
	ldr r3, [r1]
	lsls r2, r2, #1
	adds r2, #255
	cmp r3, r2
	bne .L_081930c6
	movs r0, #212
	bl Audio_PlayCue
.L_081930c6:
	movs r5, #252
	movs r3, #0
	lsls r5, r5, #1
	mov r10, r3
	adds r5, #255
.L_081930d0:
	ldr r4, [sp, #168]
	ldr r3, [r4]
	cmp r3, r5
	bne .L_081930de
	movs r0, #145
	bl Audio_PlayCue
.L_081930de:
	movs r6, #1
	add r10, r6
	mov r0, r10
	adds r5, #9
	cmp r0, #6
	bne .L_081930d0
	ldr r1, [sp, #168]
	movs r2, #205
	ldr r3, [r1]
	lsls r2, r2, #2
	cmp r3, r2
	bne .L_08193100
	movs r0, #208
	bl Audio_PlayCue
	ldr r4, [sp, #168]
	ldr r3, [r4]
.L_08193100:
	cmp r3, #0
	bne .L_081931a4
	ldr r5, .L_0819330c
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5, #4]
	movs r3, #192
	ldr r0, .L_08193310
	ldr r1, .L_08193314
	lsls r3, r3, #12
	ldr r2, .L_08193318
	str r3, [r5, #16]
	ldr r7, .L_0819331c
	movs r6, #0
	movs r4, #150
	mov r12, r0
	mov lr, r1
	mov r10, r6
	lsls r4, r4, #16
	mov r8, r2
	movs r0, #0
	movs r1, #0
.L_08193132:
	movs r3, #200
	mov r2, r10
	muls r2, r3
	ldr r3, .L_08193320
	mov r6, r10
	mov r5, r12
	adds r2, r2, r3
	ldrsb r3, [r5, r6]
	mov r5, lr
	lsls r3, r3, #16
	str r3, [r2]
	ldrb r3, [r7, r6]
	mov r6, r8
	lsls r3, r3, #16
	str r3, [r2, #4]
	ldrh r3, [r1, r5]
	str r3, [r2, #8]
	ldrsh r3, [r1, r6]
	ldr r5, .L_08193324
	str r3, [r2, #12]
	movs r3, #0
	str r3, [r2, #16]
	mov r3, r10
	adds r2, r0, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r6, #1
	lsls r3, r3, #3
	mov r11, r6
	adds r3, r3, r5
.L_0819316e:
	movs r6, #1
	add r11, r6
	mov r2, r11
	str r4, [r3]
	str r4, [r3, #4]
	adds r3, #20
	cmp r2, #10
	bne .L_0819316e
	add r10, r6
	mov r3, r10
	adds r0, #4
	adds r1, #2
	cmp r3, #15
	bne .L_08193132
	ldr r4, [sp, #64]
	movs r5, #239
	ldr r2, [r4]
	movs r6, #238
	lsls r5, r5, #7
	lsls r6, r6, #7
	adds r1, r2, r5
	movs r3, #2
	adds r6, #132
	str r3, [r1]
	adds r2, r2, r6
	movs r3, #50
	str r3, [r2]
.L_081931a4:
	ldr r0, [sp, #168]
	ldr r3, [r0]
	cmp r3, #175
	ble .L_081931ae
	b .L_08193450
.L_081931ae:
	movs r1, #3
	movs r0, #188
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r1, [sp, #172]
	movs r2, #0
	str r3, [r1, #4]
	str r2, [sp, #24]
	mov r10, r2
.L_081931c8:
	ldr r4, [sp, #168]
	movs r2, #1
	ldr r3, [r4]
	ands r3, r2
	cmp r3, #0
	bne .L_081931fc
	ldr r2, [sp, #24]
	ldr r6, .L_08193328
	add r2, r10
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r5, #9
	lsls r3, r3, #3
	mov r11, r5
	adds r2, r3, r6
.L_081931e6:
	ldr r3, [r2]
	movs r0, #1
	str r3, [r2, #20]
	negs r0, r0
	ldr r3, [r2, #4]
	add r11, r0
	mov r1, r11
	str r3, [r2, #24]
	subs r2, #20
	cmp r1, #0
	bne .L_081931e6
.L_081931fc:
	ldr r4, [sp, #168]
	mov r2, r10
	lsls r3, r2, #3
	ldr r2, [r4]
	adds r3, #24
	cmp r2, r3
	blt .L_081932b8
	movs r3, #200
	mov r5, r10
	muls r5, r3
	ldr r6, .L_08193320
	adds r3, r5, #0
	adds r5, r3, r6
	movs r0, #6
	ldrsh r3, [r5, r0]
	cmp r3, #59
	ble .L_081932b8
	ldr r0, [r5, #8]
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [r5]
	asrs r3, r3, #1
	adds r2, r2, r3
	str r2, [r5]
	ldr r0, [r5, #8]
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [r5, #4]
	ldr r0, [r5, #12]
	asrs r3, r3, #1
	subs r2, r2, r3
	str r2, [r5, #4]
	movs r1, #3
	lsls r0, r0, #1
	bl Math_Div
	ldr r3, [r5, #8]
	movs r1, #3
	adds r3, r3, r0
	str r3, [r5, #8]
	mov r3, r10
	ands r3, r1
	adds r6, r3, #5
	movs r2, #0
	mov r3, r10
	mov r11, r2
	lsls r2, r3, #2
	add r2, r10
	ldr r5, .L_0819332c
	lsls r3, r2, #2
	ldr r0, .L_08193320
	adds r3, r3, r2
	asrs r4, r6, #1
	lsls r3, r3, #3
	mov r9, r5
	mov r8, r4
	lsls r7, r6, #1
	adds r5, r3, r0
.L_08193280:
	ldr r1, [sp, #176]
	subs r3, r7, #2
	mov r2, r9
	ldr r0, [r1]
	ldrh r1, [r2, r3]
	ldr r3, [sp, #204]
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r3, r1
	mov r3, r8
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r5, r4]
	str r6, [sp, #0]
	subs r3, r3, r6
	mov r12, r3
	str r7, [sp, #4]
	ldr r3, [sp, #172]
	adds r5, #20
	ldr r4, [r3, #4]
	mov r3, r12
	mov lr, r4
	.2byte 0xf800
	movs r4, #1
	add r11, r4
	mov r0, r11
	cmp r0, #10
	bne .L_08193280
.L_081932b8:
	ldr r1, [sp, #24]
	movs r2, #1
	add r10, r2
	adds r1, #4
	mov r3, r10
	str r1, [sp, #24]
	cmp r3, #15
	beq .L_081932ca
	b .L_081931c8
.L_081932ca:
	ldr r4, [sp, #168]
	ldr r2, [r4]
	cmp r2, #71
	bgt .L_08193366
	ldr r5, .L_0819330c
	ldr r6, .L_08193330
	ldr r3, [r5, #4]
	ldr r0, .L_08193334
	adds r3, r3, r6
	str r3, [r5, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r5, #16]
	mov r3, r10
	ands r3, r2
	cmp r3, #0
	bne .L_081933ce
	movs r4, #160
	ldr r5, .L_081932fc
	lsls r4, r4, #19
	movs r1, #0
	adds r4, #192
	mov r10, r1
	movs r6, #31
	b .L_08193338
.L_081932fc:
	.4byte 0x0000001f
.L_08193300:
	.4byte Data_08199fbc
.L_08193304:
	.4byte Data_08199fc8
.L_08193308:
	.4byte Data_08199fd2
.L_0819330c:
	.4byte Data_02012ee0
.L_08193310:
	.4byte Data_08199fde
.L_08193314:
	.4byte Data_08199ffc
.L_08193318:
	.4byte Data_0819a01a
.L_0819331c:
	.4byte Data_08199fed
.L_08193320:
	.4byte Data_02012ef4
.L_08193324:
	.4byte Data_02012f08
.L_08193328:
	.4byte Data_02012f94
.L_0819332c:
	.4byte Data_08197410
.L_08193330:
	.4byte 0xffff0000
.L_08193334:
	.4byte 0xfffff000
.L_08193338:
	ldrh r0, [r4]
	adds r1, r6, #0
	lsls r2, r0, #16
	lsrs r3, r2, #26
	lsrs r2, r2, #21
	ands r3, r5
	ands r2, r5
	adds r3, #1
	adds r2, #1
	lsls r2, r2, #5
	ands r1, r0
	lsls r3, r3, #10
	orrs r3, r2
	adds r1, #1
	movs r2, #1
	orrs r3, r1
	add r10, r2
	strh r3, [r4]
	mov r3, r10
	adds r4, #2
	cmp r3, #128
	bne .L_08193338
	b .L_081933ce
.L_08193366:
	ldr r4, .L_081933b4
	movs r5, #128
	ldr r3, [r4, #16]
	lsls r5, r5, #6
	adds r3, r3, r5
	str r3, [r4, #16]
	ldr r6, [sp, #168]
	movs r2, #7
	ldr r3, [r6]
	ands r3, r2
	cmp r3, #0
	bne .L_081933ce
	movs r5, #160
	ldr r6, .L_081933b0
	lsls r5, r5, #19
	movs r0, #0
	adds r5, #192
	mov r10, r0
.L_0819338a:
	ldrh r2, [r5]
	movs r0, #31
	lsls r3, r2, #16
	lsrs r4, r3, #26
	lsrs r1, r3, #21
	ands r0, r2
	ands r4, r6
	ands r1, r6
	cmp r0, #22
	bgt .L_081933a0
	adds r0, #1
.L_081933a0:
	cmp r1, #1
	ble .L_081933a6
	subs r1, #1
.L_081933a6:
	cmp r4, #14
	bgt .L_081933b8
	adds r4, #1
	b .L_081933b8
	.2byte 0x0000
.L_081933b0:
	.4byte 0x0000001f
.L_081933b4:
	.4byte Data_02012ee0
.L_081933b8:
	lsls r2, r1, #5
	lsls r3, r4, #10
	movs r1, #1
	orrs r3, r2
	add r10, r1
	orrs r3, r0
	mov r2, r10
	strh r3, [r5]
	adds r5, #2
	cmp r2, #128
	bne .L_0819338a
.L_081933ce:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r4, .L_08193440
	ldr r6, [sp, #64]
	movs r3, #18
	ldrsh r5, [r4, r3]
	ldr r0, [r6]
	movs r6, #224
	lsls r5, r5, #1
	lsls r6, r6, #3
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #9
	adds r0, r0, r6
	bl Func_0815b434
	ldr r1, [sp, #64]
	ldr r3, .L_08193440
	ldr r0, [r1]
	movs r2, #2
	ldrsh r1, [r3, r2]
	adds r0, r0, r6
	movs r4, #6
	ldrsh r2, [r3, r4]
	adds r3, r5, #0
	bl Func_0818caa8
	ldr r5, [sp, #168]
	ldr r3, [r5]
	cmp r3, #175
	bne .L_08193450
	ldr r2, .L_0819343c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #12
	strh r2, [r3]
	ldr r6, [sp, #176]
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	ldr r2, .L_08193444
	ldr r3, .L_08193448
	mov lr, r3
	.2byte 0xf800
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	adds r0, #192
	lsls r1, r1, #1
	ldr r2, .L_0819344c
	ldr r4, .L_08193448
	mov lr, r4
	.2byte 0xf800
	b .L_08193450
.L_0819343c:
	.4byte 0x00000785
.L_08193440:
	.4byte Data_02012ee0
.L_08193444:
	.4byte 0x3f3f3f3f
.L_08193448:
	.4byte IwramFillWords
.L_0819344c:
	.4byte 0x7fff7fff
.L_08193450:
	ldr r5, [sp, #168]
	ldr r3, [r5]
	cmp r3, #176
	beq .L_0819345a
	b .L_08193842
.L_0819345a:
	ldr r6, [sp, #176]
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_08193640
	ldr r0, [r6]
	ldr r3, .L_08193644
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #64]
	movs r5, #240
	ldr r3, [r4]
	lsls r5, r5, #7
	adds r5, #232
	adds r3, r3, r5
	movs r5, #1
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	movs r0, #237
	ldr r6, [sp, #200]
	lsls r0, r0, #3
	adds r0, #255
	adds r3, r6, r0
	strb r5, [r3]
	ldr r1, .L_08193648
	movs r2, #0
	movs r0, #1
	bl Func_08118040
	ldr r0, .L_0819364c
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r5, r0, #0
	adds r3, #212
	ldr r1, .L_08193650
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #2
	adds r5, r5, r1
	adds r0, r5, #0
	ldr r1, .L_08193654
	bl Func_0801587c
	movs r5, #238
	movs r3, #13
	ldr r4, .L_08193658
	ldr r6, .L_08193654
	movs r2, #0
	negs r3, r3
	lsls r5, r5, #7
	mov r10, r2
	adds r7, r3, #0
	adds r5, #220
.L_081934d2:
	movs r1, #32
	ldr r2, .L_0819365c
	movs r3, #0
	movs r0, #32
	str r4, [sp, #16]
	bl Func_0815b290
	ldr r1, [sp, #64]
	movs r2, #4
	ldr r3, [r1]
	ldr r4, [sp, #16]
	str r0, [r3, r5]
	ldrb r3, [r0, #9]
	ands r3, r7
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	ldr r2, .L_08193660
	lsls r3, r3, #2
	adds r3, r3, r4
	ldrh r1, [r3, #2]
	movs r3, #128
	lsls r3, r3, #19
	adds r1, r1, r2
	adds r3, #212
	adds r0, r6, #0
	ldr r2, .L_08193664
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	movs r3, #128
	add r10, r0
	lsls r3, r3, #3
	mov r1, r10
	adds r6, r6, r3
	adds r5, #4
	cmp r1, #16
	bne .L_081934d2
	ldr r0, .L_08193668
	bl Resource_GetTableEntry
	adds r5, r0, #0
	ldr r0, .L_0819366c
	movs r2, #1
	movs r1, #32
	negs r2, r2
	ldr r3, .L_08193644
	mov lr, r3
	.2byte 0xf800
	adds r5, #32
	ldr r4, [sp, #64]
	movs r6, #224
	ldr r1, [r4]
	lsls r6, r6, #3
	adds r1, r1, r6
	adds r0, r5, #0
	bl Func_0801587c
	ldr r6, .L_08193658
	movs r0, #0
	mov r10, r0
	movs r5, #72
.L_0819354e:
	movs r2, #128
	movs r3, #240
	movs r1, #16
	lsls r2, r2, #23
	lsls r3, r3, #8
	movs r0, #16
	bl Func_0815b290
	ldr r4, [sp, #180]
	mov r1, r10
	ldr r3, [r4]
	lsls r2, r1, #2
	str r0, [r2, r3]
	movs r1, #13
	ldrb r3, [r0, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r0, #9]
	ldr r3, [sp, #184]
	mov r4, r10
	ldr r2, [r3]
	ldrh r3, [r0, #8]
	lsls r3, r3, #22
	lsrs r3, r3, #22
	strh r3, [r2, r5]
	adds r2, r2, r5
	ldrb r3, [r0, #16]
	strh r4, [r2, #30]
	ldr r4, [sp, #64]
	lsls r3, r3, #2
	adds r3, r3, r6
	ldrh r1, [r3, #2]
	ldr r3, [r4]
	mov r2, r10
	lsls r0, r2, #7
	movs r2, #224
	adds r0, r0, r3
	lsls r2, r2, #3
	ldr r4, .L_08193660
	adds r0, r0, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r1, r4
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r5, #2
	cmp r1, #15
	bne .L_0819354e
	ldr r7, [sp, #180]
	movs r2, #130
	mov r8, r2
.L_081935c8:
	movs r2, #128
	movs r3, #240
	movs r1, #16
	lsls r2, r2, #23
	lsls r3, r3, #8
	movs r0, #16
	bl Func_0815b3b0
	ldr r2, [r7]
	mov r3, r10
	adds r1, r0, #0
	lsls r4, r3, #2
	str r1, [r4, r2]
	movs r3, #128
	ldr r0, [r2]
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r4, [sp, #16]
	bl Random16
	movs r1, #15
	ldr r5, [sp, #184]
	bl Math_ModU
	ldr r6, [r5]
	mov r1, r8
	adds r5, r6, #2
	strh r0, [r5, r1]
	ldr r3, [r7]
	ldr r4, [sp, #16]
	ldr r2, .L_08193638
	ldr r0, [r4, r3]
	ldrh r3, [r5, r1]
	lsls r3, r3, #1
	adds r3, #72
	ldrh r1, [r6, r3]
	ldr r3, .L_0819363c
	ands r1, r3
	ldrh r3, [r0, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	movs r3, #1
	add r10, r3
	movs r2, #2
	mov r4, r10
	add r8, r2
	cmp r4, #36
	bne .L_081935c8
	b .L_08193670
	.2byte 0x0000
.L_08193638:
	.4byte 0xfffffc00
.L_0819363c:
	.4byte 0x000003ff
.L_08193640:
	.4byte 0x3f3f3f3f
.L_08193644:
	.4byte IwramFillWords
.L_08193648:
	.4byte 0x00000076
.L_0819364c:
	.4byte 0x000000a5
.L_08193650:
	.4byte 0x05000200
.L_08193654:
	.4byte Data_02010578
.L_08193658:
	.4byte ResourceTableEntries
.L_0819365c:
	.4byte 0x80002000
.L_08193660:
	.4byte 0x06010000
.L_08193664:
	.4byte 0x84000100
.L_08193668:
	.4byte 0x0000009a
.L_0819366c:
	.4byte 0x050003e0
.L_08193670:
	ldr r7, [sp, #184]
	movs r5, #0
	movs r6, #63
	movs r0, #176
	mov r10, r5
	mov r9, r6
	mov r8, r0
.L_0819367e:
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r5, [r7]
	movs r1, #228
	lsls r1, r1, #14
	lsls r0, r0, #16
	mov r2, r8
	adds r0, r0, r1
	str r0, [r5, r2]
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r6, [r7]
	movs r3, #158
	lsls r3, r3, #15
	mov r5, r8
	lsls r0, r0, #16
	adds r0, r0, r3
	adds r5, #144
	str r0, [r6, r5]
	bl Random16
	mov r4, r9
	ldr r2, [r7]
	ands r0, r4
	movs r3, #144
	lsls r3, r3, #1
	subs r0, #32
	add r3, r8
	lsls r0, r0, #12
	str r0, [r2, r3]
	bl Random16
	mov r5, r9
	ldr r2, [r7]
	movs r3, #216
	ands r0, r5
	lsls r3, r3, #1
	subs r0, #32
	add r3, r8
	lsls r0, r0, #12
	str r0, [r2, r3]
	movs r0, #1
	add r10, r0
	movs r6, #4
	mov r1, r10
	add r8, r6
	cmp r1, #18
	bne .L_0819367e
	ldr r7, [sp, #184]
	movs r2, #63
	movs r3, #248
	mov r9, r2
	mov r8, r3
.L_081936f4:
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r5, [r7]
	movs r4, #169
	lsls r4, r4, #16
	lsls r0, r0, #16
	adds r0, r0, r4
	mov r6, r8
	str r0, [r5, r6]
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r6, [r7]
	movs r1, #158
	lsls r1, r1, #15
	mov r5, r8
	lsls r0, r0, #16
	adds r0, r0, r1
	adds r5, #144
	str r0, [r6, r5]
	bl Random16
	mov r4, r9
	ldr r2, [r7]
	ands r0, r4
	movs r3, #144
	lsls r3, r3, #1
	subs r0, #32
	add r3, r8
	lsls r0, r0, #12
	str r0, [r2, r3]
	bl Random16
	mov r5, r9
	ldr r2, [r7]
	movs r3, #216
	ands r0, r5
	lsls r3, r3, #1
	subs r0, #32
	add r3, r8
	lsls r0, r0, #12
	str r0, [r2, r3]
	movs r0, #1
	add r10, r0
	movs r6, #4
	mov r1, r10
	add r8, r6
	cmp r1, #36
	bne .L_081936f4
	ldr r2, [sp, #64]
	movs r3, #224
	ldr r1, [r2]
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r0, .L_081938a4
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r6, [sp, #64]
	movs r4, #0
	mov r10, r4
	movs r7, #4
	mov r8, r4
.L_0819377e:
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r5, [r6]
	movs r1, #132
	lsls r1, r1, #14
	lsls r0, r0, #16
	mov r2, r8
	adds r0, r0, r1
	str r0, [r5, r2]
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r5, [r6]
	movs r3, #228
	lsls r3, r3, #14
	lsls r0, r0, #16
	adds r0, r0, r3
	str r0, [r5, r7]
	bl Random16
	movs r4, #31
	ldr r2, [r6]
	ands r0, r4
	adds r3, r7, #0
	negs r0, r0
	lsls r0, r0, #12
	adds r3, #8
	str r0, [r2, r3]
	movs r0, #1
	add r10, r0
	movs r5, #28
	mov r1, r10
	adds r7, #28
	add r8, r5
	cmp r1, #6
	bne .L_0819377e
	ldr r6, [sp, #64]
	movs r2, #168
	movs r7, #172
	mov r8, r2
.L_081937d8:
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r5, [r6]
	movs r3, #162
	lsls r3, r3, #15
	lsls r0, r0, #16
	mov r4, r8
	adds r0, r0, r3
	str r0, [r5, r4]
	bl Random16
	movs r1, #15
	bl Math_ModU
	ldr r5, [r6]
	movs r1, #228
	lsls r1, r1, #14
	lsls r0, r0, #16
	adds r0, r0, r1
	str r0, [r5, r7]
	bl Random16
	ldr r2, [r6]
	movs r4, #31
	adds r3, r7, #0
	ands r0, r4
	lsls r0, r0, #12
	adds r3, #8
	str r0, [r2, r3]
	movs r0, #1
	add r10, r0
	movs r5, #28
	mov r1, r10
	adds r7, #28
	add r8, r5
	cmp r1, #12
	bne .L_081937d8
	ldr r3, [sp, #64]
	movs r4, #239
	ldr r2, [r3]
	movs r5, #238
	lsls r4, r4, #7
	lsls r5, r5, #7
	adds r1, r2, r4
	movs r3, #2
	adds r5, #132
	str r3, [r1]
	adds r2, r2, r5
	movs r3, #70
	str r3, [r2]
.L_08193842:
	ldr r6, [sp, #168]
	ldr r3, [r6]
	subs r3, #177
	cmp r3, #216
	bls .L_0819384e
	b .L_08193a6e
.L_0819384e:
	ldr r3, .L_081938a8
	movs r1, #175
	ldr r4, [r3, #4]
	ldr r3, [r3]
	lsls r1, r1, #1
	str r3, [sp, #232]
	str r4, [sp, #236]
	add r3, sp, #376
	adds r6, r3, #0
	movs r3, #0
	str r3, [r6, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r6, #4]
	ldr r0, [sp, #168]
	movs r7, #0
	ldr r3, [r0]
	cmp r3, r1
	ble .L_081938c6
	ldr r4, .L_081938ac
	ldr r5, .L_081938a0
	movs r2, #0
	mov r10, r2
.L_0819387c:
	ldrh r3, [r4]
	movs r1, #31
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r5
	ands r3, r5
	cmp r1, #31
	beq .L_08193892
	adds r1, #1
.L_08193892:
	cmp r2, #31
	beq .L_08193898
	adds r2, #1
.L_08193898:
	cmp r3, #31
	beq .L_081938b0
	adds r3, #1
	b .L_081938b0
.L_081938a0:
	.4byte 0x0000001f
.L_081938a4:
	.4byte 0x00000134
.L_081938a8:
	.4byte Data_08196f48
.L_081938ac:
	.4byte 0x05000200
.L_081938b0:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	strh r3, [r4]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r4, #2
	cmp r0, #255
	bne .L_0819387c
.L_081938c6:
	ldr r1, [sp, #168]
	movs r2, #130
	ldr r0, [r1]
	adds r2, #255
	cmp r0, r2
	bgt .L_08193980
	ldr r4, .L_081938e8
	adds r3, r0, r4
	cmp r3, #14
	bhi .L_081938f0
	ldr r5, .L_081938ec
	adds r0, r0, r5
	lsls r0, r0, #10
	bl Trig_Sin
	lsls r7, r0, #6
	b .L_0819390a
.L_081938e8:
	.4byte 0xfffffea1
.L_081938ec:
	.4byte 0xfffffea2
.L_081938f0:
	movs r1, #110
	adds r1, #255
	cmp r0, r1
	ble .L_0819390a
	movs r2, #183
	lsls r2, r2, #1
	subs r2, r2, r0
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r2, #128
	lsls r3, r3, #17
	lsls r2, r2, #15
	adds r7, r3, r2
.L_0819390a:
	ldr r3, [sp, #168]
	ldr r0, [r3]
	cmp r0, #229
	bgt .L_08193922
	ldr r4, .L_08193b58
	lsls r0, r0, #6
	adds r0, r0, r4
	bl Trig_Sin
	ldr r5, .L_08193b5c
	lsls r0, r0, #6
	adds r7, r0, r5
.L_08193922:
	mov r1, sp
	adds r1, #232
	movs r2, #224
	str r1, [sp, #36]
	movs r0, #0
	lsls r2, r2, #14
	mov r11, r0
	adds r4, r7, r2
	mov r9, r0
.L_08193934:
	movs r5, #238
	lsls r5, r5, #7
	movs r3, #0
	adds r5, #220
	movs r7, #144
	mov r8, r3
	mov r10, r4
	add r5, r9
	lsls r7, r7, #15
.L_08193946:
	mov r0, r10
	str r7, [r6]
	str r0, [r6, #8]
	ldr r1, [sp, #64]
	ldr r2, [sp, #36]
	ldr r3, [r1]
	adds r1, r6, #0
	ldr r0, [r3, r5]
	movs r3, #0
	str r4, [sp, #16]
	bl Func_08020010
	movs r3, #1
	movs r2, #128
	add r8, r3
	lsls r2, r2, #14
	mov r0, r8
	adds r5, #4
	adds r7, r7, r2
	ldr r4, [sp, #16]
	cmp r0, #4
	bne .L_08193946
	add r11, r3
	adds r4, r4, r2
	movs r1, #16
	mov r2, r11
	add r9, r1
	cmp r2, #4
	bne .L_08193934
.L_08193980:
	ldr r4, [sp, #168]
	movs r5, #193
	ldr r3, [r4]
	lsls r5, r5, #1
	cmp r3, r5
	bne .L_08193a02
	movs r5, #238
	movs r6, #0
	lsls r5, r5, #7
	mov r10, r6
	adds r5, #220
.L_08193996:
	ldr r0, [sp, #64]
	ldr r3, [r0]
	ldr r0, [r3, r5]
	bl ResourceObject_ReleaseFar
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #4
	cmp r2, #16
	bne .L_08193996
	ldr r4, [sp, #64]
	movs r6, #240
	ldr r3, [r4]
	lsls r6, r6, #7
	adds r6, #240
	ldr r0, [r3, r6]
	bl Func_0814cc4c
	bl Func_08014c4c
	ldr r0, [sp, #64]
	movs r5, #0
	ldr r3, [r0]
	mov r10, r5
	ldr r3, [r3, r6]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_08193a02
	movs r7, #1
	adds r4, r6, #0
	adds r5, r0, #0
	negs r7, r7
	movs r6, #36
.L_081939da:
	ldr r3, [r5]
	movs r2, #5
	ldr r3, [r3, r4]
	str r4, [sp, #16]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	adds r1, r7, #0
	adds r3, r7, #0
	bl Func_0814cd48
	ldr r3, [r5]
	ldr r4, [sp, #16]
	movs r2, #1
	ldr r3, [r3, r4]
	add r10, r2
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r10, r3
	bne .L_081939da
.L_08193a02:
	ldr r4, [sp, #168]
	ldr r3, [r4]
	cmp r3, #191
	bgt .L_08193a6e
	movs r5, #0
	mov r10, r5
.L_08193a0e:
	mov r3, r10
	ldr r6, [sp, #168]
	movs r2, #1
	ands r2, r3
	lsls r3, r2, #1
	ldr r1, [r6]
	adds r3, r3, r2
	lsls r3, r3, #4
	subs r3, r3, r2
	mov r0, r10
	adds r0, #62
	adds r1, r1, r3
	movs r3, #7
	ands r3, r0
	adds r1, #80
	asrs r0, r0, #3
	lsls r3, r3, #3
	lsls r1, r1, #24
	lsls r0, r0, #10
	adds r4, r3, r0
	lsrs r1, r1, #24
	movs r3, #0
.L_08193a3a:
	adds r5, r3, #0
	adds r5, #8
	adds r0, r3, #0
	cmp r3, r5
	beq .L_08193a5c
	ldr r6, [sp, #176]
	mov r12, r5
.L_08193a48:
	ldr r3, [r6]
	adds r2, r3, r4
	ldrb r3, [r2]
	cmp r3, r1
	bcs .L_08193a54
	strb r1, [r2]
.L_08193a54:
	adds r0, #1
	adds r4, #1
	cmp r0, r12
	bne .L_08193a48
.L_08193a5c:
	adds r3, r5, #0
	adds r4, #56
	cmp r3, #128
	bne .L_08193a3a
	movs r4, #1
	add r10, r4
	mov r5, r10
	cmp r5, #3
	bne .L_08193a0e
.L_08193a6e:
	ldr r6, [sp, #168]
	ldr r3, [r6]
	subs r3, #177
	cmp r3, #46
	bls .L_08193a7a
	b .L_08193b64
.L_08193a7a:
	movs r1, #224
	movs r2, #160
	movs r0, #0
	add r1, sp
	lsls r2, r2, #1
	mov r11, r6
	mov r10, r0
	mov r9, r1
	add r6, sp, #376
	movs r7, #100
	mov r8, r2
	movs r4, #176
.L_08193a92:
	mov r5, r11
	ldr r3, [r5]
	movs r2, #128
	subs r3, #177
	asrs r3, r3, #1
	lsls r3, r3, #11
	lsls r2, r2, #9
	subs r2, r2, r3
	movs r3, #255
	mov r0, r9
	movs r1, #0
	lsls r3, r3, #16
	str r2, [r0, #4]
	str r2, [sp, #224]
	str r1, [r6, #12]
	str r3, [r6, #4]
	ldr r3, [sp, #164]
	mov r1, r8
	ldr r2, [r3]
	mov r0, r10
	ldr r3, [r2, r4]
	lsls r5, r0, #2
	str r3, [r6]
	str r4, [sp, #16]
	ldr r3, [r2, r1]
	adds r1, r6, #0
	str r3, [r6, #8]
	ldr r2, [sp, #180]
	ldr r3, [r2]
	mov r2, r9
	ldr r0, [r5, r3]
	movs r3, #0
	bl Func_08020010
	ldr r3, [sp, #164]
	ldr r4, [sp, #16]
	ldr r1, [r3]
	movs r0, #232
	lsls r0, r0, #1
	adds r2, r5, r0
	ldr r2, [r1, r2]
	ldr r3, [r1, r4]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r1, r4]
	movs r3, #152
	lsls r3, r3, #2
	adds r2, r5, r3
	ldr r2, [r1, r2]
	ldr r3, [r1, r0]
	adds r3, r3, r2
	str r3, [r1, r0]
	mov r2, r11
	ldr r3, [r2]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_08193b0e
	adds r2, r1, #2
	ldrh r3, [r2, r7]
	adds r3, #1
	strh r3, [r2, r7]
.L_08193b0e:
	ldr r3, [sp, #184]
	ldr r1, [r3]
	adds r2, r1, #2
	ldrh r3, [r2, r7]
	cmp r3, #14
	bls .L_08193b1e
	ldr r0, .L_08193b4c
	strh r0, [r2, r7]
.L_08193b1e:
	ldr r2, [sp, #180]
	adds r4, #4
	ldr r3, [r2]
	ldr r2, .L_08193b50
	ldr r0, [r5, r3]
	adds r3, r1, #2
	ldrh r3, [r3, r7]
	movs r5, #1
	lsls r3, r3, #1
	adds r3, #72
	ldrh r1, [r1, r3]
	ldr r3, .L_08193b54
	add r10, r5
	ands r1, r3
	ldrh r3, [r0, #8]
	adds r7, #2
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	movs r3, #4
	mov r0, r10
	add r8, r3
	b .L_08193b60
.L_08193b4c:
	.4byte 0x00000000
.L_08193b50:
	.4byte 0xfffffc00
.L_08193b54:
	.4byte 0x000003ff
.L_08193b58:
	.4byte 0xffffd400
.L_08193b5c:
	.4byte 0xffeb8e00
.L_08193b60:
	cmp r0, #36
	bne .L_08193a92
.L_08193b64:
	ldr r1, [sp, #168]
	ldr r3, [r1]
	adds r2, r3, #0
	subs r2, #179
	cmp r2, #38
	bhi .L_08193c24
	adds r7, r3, #0
	movs r2, #0
	subs r7, #170
	mov r10, r2
.L_08193b78:
	mov r3, r10
	lsls r5, r3, #8
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, r7, #0
	muls r6, r0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, .L_08193c78
	ldr r5, [sp, #64]
	ldrh r2, [r2, #6]
	ldr r1, [r5]
	ldr r4, [sp, #176]
	adds r1, r1, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r3, r7, #0
	muls r3, r0
	adds r1, r1, r2
	movs r2, #4
	ldr r0, [r4]
	str r2, [sp, #0]
	movs r2, #8
	str r2, [sp, #4]
	ldr r5, [sp, #172]
	asrs r6, r6, #16
	lsls r3, r3, #1
	adds r6, #58
	asrs r3, r3, #16
	adds r2, r6, #0
	adds r3, #48
	ldr r4, [r5]
	mov lr, r4
	.2byte 0xf800
	movs r6, #1
	movs r0, #128
	add r10, r6
	lsls r0, r0, #1
	cmp r10, r0
	bne .L_08193b78
	ldr r6, [sp, #64]
	movs r1, #0
	mov r10, r1
	movs r7, #4
	movs r5, #0
.L_08193bd6:
	ldr r3, .L_08193c78
	ldr r2, [sp, #176]
	ldrh r1, [r3, #2]
	ldr r3, [r6]
	ldr r0, [r2]
	adds r1, r3, r1
	ldr r2, [r3, r5]
	ldr r3, [r3, r7]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r1, r4
	asrs r3, r3, #16
	movs r4, #2
	str r4, [sp, #0]
	subs r3, #2
	movs r4, #4
	mov r12, r3
	str r4, [sp, #4]
	ldr r3, [sp, #172]
	asrs r2, r2, #16
	ldr r4, [r3]
	subs r2, #2
	mov r3, r12
	mov lr, r4
	.2byte 0xf800
	ldr r1, [r6]
	adds r2, r7, #0
	adds r2, #8
	ldr r3, [r1, r5]
	ldr r2, [r1, r2]
	movs r4, #1
	add r10, r4
	adds r3, r3, r2
	mov r0, r10
	str r3, [r1, r5]
	adds r7, #28
	adds r5, #28
	cmp r0, #12
	bne .L_08193bd6
.L_08193c24:
	ldr r1, [sp, #168]
	ldr r0, [r1]
	adds r3, r0, #0
	subs r3, #179
	cmp r3, #31
	bhi .L_08193c42
	movs r2, #210
	ldr r1, .L_08193c6c
	movs r3, #128
	subs r2, r2, r0
	lsls r3, r3, #19
	asrs r2, r2, #1
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08193c42:
	cmp r0, #230
	beq .L_08193c48
	b .L_08193e66
.L_08193c48:
	ldr r3, .L_08193c70
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_08193c74
	subs r2, #70
	strh r3, [r2]
	ldr r3, [sp, #64]
	movs r5, #192
	movs r4, #224
	lsls r5, r5, #2
	movs r2, #1
	lsls r4, r4, #3
	adds r5, #2
	mov r10, r2
	b .L_08193c7c
	.2byte 0x0000
.L_08193c6c:
	.4byte 0x00001000
.L_08193c70:
	.4byte 0x00001010
.L_08193c74:
	.4byte 0x00000784
.L_08193c78:
	.4byte Data_08197410
.L_08193c7c:
	mov r9, r3
	mov r8, r4
	movs r6, #8
	mov lr, r5
.L_08193c84:
	movs r0, #0
	mov r11, r0
	mov r7, lr
.L_08193c8a:
	ldr r4, .L_08193d80
	mov r1, r11
	lsls r3, r1, #1
	mov r5, r9
	ldrh r2, [r4, r3]
	ldr r1, [r5]
	mov r4, r8
	adds r3, r1, r2
	adds r2, r2, r7
	adds r1, r1, r2
	mov r2, r11
	adds r2, #1
	adds r0, r3, r4
	lsls r3, r2, #1
	muls r3, r2
	add r1, r8
	movs r5, #0
	cmp r3, #0
	beq .L_08193cce
	adds r4, r6, #0
	mov r12, r3
.L_08193cb4:
	ldrb r3, [r0]
	subs r3, r3, r4
	lsls r3, r3, #24
	lsrs r3, r3, #24
	cmp r3, #63
	bls .L_08193cc2
	movs r3, #0
.L_08193cc2:
	adds r5, #1
	strb r3, [r1]
	adds r0, #1
	adds r1, #1
	cmp r5, r12
	bne .L_08193cb4
.L_08193cce:
	mov r11, r2
	cmp r2, #10
	bne .L_08193c8a
	movs r5, #192
	movs r0, #1
	lsls r5, r5, #2
	add r10, r0
	adds r5, #2
	mov r1, r10
	adds r6, #8
	add lr, r5
	cmp r1, #8
	bne .L_08193c84
	ldr r6, [sp, #64]
	movs r2, #0
	mov r10, r2
	movs r4, #0
	movs r7, #0
.L_08193cf2:
	ldr r0, [r6]
	movs r3, #136
	lsls r3, r3, #6
	mov r5, r10
	adds r0, r0, r7
	adds r3, #18
	adds r0, r0, r3
	lsls r3, r5, #13
	movs r5, #128
	lsls r5, r5, #9
	subs r5, r5, r3
	adds r2, r5, #0
	movs r1, #20
	str r4, [sp, #16]
	bl Func_0815b510
	ldr r0, [r6]
	ldr r4, [sp, #16]
	movs r1, #166
	lsls r1, r1, #7
	adds r0, r0, r4
	adds r1, #18
	adds r0, r0, r1
	adds r2, r5, #0
	movs r1, #12
	bl Func_0815b510
	movs r5, #1
	ldr r4, [sp, #16]
	movs r2, #144
	movs r3, #200
	add r10, r5
	lsls r2, r2, #1
	lsls r3, r3, #2
	mov r0, r10
	adds r4, r4, r2
	adds r7, r7, r3
	cmp r0, #8
	bne .L_08193cf2
	ldr r4, [sp, #184]
	ldr r2, .L_08193d7c
	ldr r3, [r4]
	movs r1, #0
	mov r10, r1
.L_08193d4a:
	movs r5, #1
	add r10, r5
	mov r6, r10
	strh r2, [r3]
	adds r3, #2
	cmp r6, #36
	bne .L_08193d4a
	ldr r0, .L_08193d84
	ldr r1, [sp, #204]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r5, .L_08193d88
	movs r0, #0
	movs r4, #254
	mov r10, r0
	lsls r4, r4, #1
.L_08193d6e:
	ldr r2, [sp, #204]
	movs r1, #0
	adds r3, r0, r2
	mov r11, r1
	adds r2, r5, #0
	adds r1, r3, r4
	b .L_08193d8c
.L_08193d7c:
	.4byte 0x00000000
.L_08193d80:
	.4byte Data_08197410
.L_08193d84:
	.4byte 0x00000134
.L_08193d88:
	.4byte Data_0819a038
.L_08193d8c:
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	movs r3, #1
	add r11, r3
	mov r6, r11
	adds r1, #1
	cmp r6, #8
	bne .L_08193d8c
	add r10, r3
	mov r1, r10
	adds r0, #8
	cmp r1, #32
	bne .L_08193d6e
	ldr r6, .L_08194064
	ldr r3, [sp, #64]
	ldr r5, [sp, #204]
	ldr r4, .L_08194068
	movs r2, #0
	mov lr, r6
	mov r10, r2
	mov r8, r3
	movs r7, #0
	mov r9, lr
.L_08193dbc:
	movs r0, #0
	mov r11, r0
	movs r6, #0
	mov r12, r7
.L_08193dc4:
	mov r2, lr
	ldr r1, [r7, r2]
	ldr r0, .L_08194068
	lsls r3, r1, #1
	subs r3, #2
	ldrh r2, [r0, r3]
	mov r0, r8
	ldr r3, [r0]
	movs r0, #224
	adds r3, r3, r2
	lsls r0, r0, #3
	adds r2, r3, r0
	adds r3, r1, #0
	muls r3, r1
	lsls r3, r3, #1
	movs r0, #0
	cmp r3, #0
	beq .L_08193e14
	mov r1, r12
	add r1, r9
	str r1, [sp, #12]
	adds r4, r6, #0
.L_08193df0:
	ldrb r3, [r2]
	subs r3, r3, r4
	cmp r3, #0
	bge .L_08193dfa
	movs r3, #0
.L_08193dfa:
	strb r3, [r5]
	ldr r1, [sp, #12]
	adds r0, #1
	ldr r3, [r1]
	adds r5, #1
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	lsls r3, r3, #1
	adds r2, #1
	cmp r0, r3
	bne .L_08193df0
	ldr r4, .L_08194068
.L_08193e14:
	movs r2, #1
	add r11, r2
	mov r3, r11
	adds r6, #16
	cmp r3, #3
	bne .L_08193dc4
	add r10, r2
	mov r6, r10
	adds r7, #4
	cmp r6, #2
	bne .L_08193dbc
	movs r0, #0
	mov r11, r0
	b .L_08193e32
.L_08193e30:
	ldr r4, .L_08194068
.L_08193e32:
	ldr r1, [sp, #64]
	ldrh r2, [r4, #2]
	ldr r3, [r1]
	movs r4, #224
	adds r3, r3, r2
	lsls r4, r4, #3
	mov r6, r11
	adds r2, r3, r4
	movs r0, #0
	lsls r1, r6, #3
.L_08193e46:
	ldrb r3, [r2]
	subs r3, r3, r1
	cmp r3, #0
	bge .L_08193e50
	movs r3, #0
.L_08193e50:
	adds r0, #1
	strb r3, [r5]
	adds r2, #1
	adds r5, #1
	cmp r0, #8
	bne .L_08193e46
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #8
	bne .L_08193e30
.L_08193e66:
	movs r0, #188
	movs r1, #3
	bl Func_081963ec
	ldr r3, [sp, #168]
	ldr r2, [r3]
	adds r3, r2, #0
	subs r3, #240
	cmp r3, #127
	bhi .L_08193e96
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_08193e96
	movs r3, #240
	subs r2, r3, r2
	cmp r2, #0
	bge .L_08193e8c
	adds r2, #15
.L_08193e8c:
	asrs r2, r2, #4
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_08164b2c
.L_08193e96:
	movs r4, #0
	str r4, [sp, #28]
	mov r11, r4
.L_08193e9c:
	ldr r3, .L_0819406c
	ldr r5, [sp, #28]
	ldr r6, [sp, #168]
	ldrh r1, [r3, r5]
	ldr r2, [r6]
	cmp r2, r1
	bgt .L_08193eac
	b .L_081943f2
.L_08193eac:
	ldr r0, .L_08194070
	ldrh r3, [r0, r5]
	cmp r2, r3
	blt .L_08193eb6
	b .L_081943ae
.L_08193eb6:
	ldr r3, .L_08194074
	mov r0, r11
	ldrb r3, [r3, r0]
	subs r2, r2, r1
	str r3, [sp, #160]
	ldr r3, .L_08194078
	mov r10, r2
	ldrb r3, [r3, r0]
	ldr r6, .L_0819407c
	mov r9, r3
	ldr r3, .L_08194080
	ldrb r3, [r3, r0]
	str r3, [sp, #156]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	ldr r5, [sp, #156]
	str r1, [sp, #152]
	ldr r2, [r3, #96]
	str r2, [sp, #148]
	ldr r4, [r3, #100]
	str r4, [sp, #136]
	ldr r3, [r3, #104]
	str r3, [sp, #140]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	str r3, [sp, #144]
	lsls r3, r5, #2
	ldr r3, [r3, r6]
	lsls r0, r3, #1
	cmp r10, r0
	blt .L_08193efc
	b .L_08194170
.L_08193efc:
	mov r1, r10
	subs r1, r0, r1
	mov r8, r1
	mov r0, r8
	movs r1, #6
	bl Math_Div
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #7
	adds r3, r3, r0
	lsls r3, r3, #1
	str r3, [sp, #132]
	ldr r3, [sp, #136]
	lsls r2, r0, #3
	adds r2, r3, r2
	str r2, [sp, #48]
	movs r7, #0
.L_08193f20:
	lsls r6, r7, #10
	adds r0, r6, #0
	bl Trig_Sin
	mov r5, r8
	muls r5, r0
	ldr r4, [sp, #160]
	lsrs r3, r5, #31
	adds r5, r5, r3
	adds r0, r6, #0
	asrs r5, r5, #17
	adds r5, r5, r4
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	movs r2, #2
	str r2, [sp, #0]
	ldr r6, [sp, #48]
	movs r2, #4
	str r2, [sp, #4]
	asrs r3, r3, #16
	movs r2, #222
	lsls r2, r2, #1
	add r3, r9
	adds r1, r6, r2
	subs r3, #2
	ldr r0, [sp, #148]
	adds r2, r5, #0
	ldr r4, [sp, #140]
	adds r7, #1
	mov lr, r4
	.2byte 0xf800
	cmp r7, #64
	bne .L_08193f20
	ldr r5, [sp, #156]
	cmp r5, #0
	bne .L_08194056
	mov r6, r10
	movs r1, #0
	cmp r6, #11
	bgt .L_08193f78
	movs r1, #196
	b .L_08193f80
.L_08193f78:
	mov r0, r10
	cmp r0, #23
	bgt .L_08193f80
	movs r1, #98
.L_08193f80:
	ldr r3, [sp, #136]
	ldr r4, [sp, #140]
	mov r2, r10
	lsls r2, r2, #2
	adds r1, r3, r1
	str r2, [sp, #52]
	str r1, [sp, #128]
	str r4, [sp, #124]
	movs r7, #0
.L_08193f92:
	ldr r3, [sp, #52]
	lsls r5, r7, #13
	add r3, r10
	lsls r3, r3, #9
	adds r5, r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	ldr r0, [sp, #160]
	asrs r3, r3, #16
	adds r6, r3, r0
	adds r0, r5, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	lsls r3, r3, #1
	asrs r3, r3, #16
	mov r1, r9
	subs r2, r6, #3
	adds r0, r3, r1
	movs r4, #7
	movs r6, #14
	str r4, [sp, #0]
	str r6, [sp, #4]
	ldr r1, [sp, #128]
	ldr r4, [sp, #124]
	subs r3, r0, #7
	ldr r0, [sp, #148]
	mov lr, r4
	.2byte 0xf800
	asrs r3, r5, #1
	adds r5, r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	mov r1, r8
	muls r1, r0
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r0, #160
	asrs r1, r1, #1
	ldr r6, .L_08194084
	lsls r0, r0, #10
	mov lr, r6
	.2byte 0xf800
	ldr r1, [sp, #160]
	asrs r0, r0, #16
	adds r6, r0, r1
	adds r0, r5, #0
	bl Trig_Cos
	mov r1, r8
	muls r1, r0
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r0, #160
	asrs r1, r1, #1
	ldr r2, .L_08194084
	lsls r0, r0, #11
	mov lr, r2
	.2byte 0xf800
	asrs r0, r0, #16
	add r0, r9
	movs r4, #7
	movs r5, #14
	subs r2, r6, #3
	subs r3, r0, #7
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #148]
	ldr r1, [sp, #128]
	ldr r6, [sp, #124]
	adds r7, #1
	mov lr, r6
	.2byte 0xf800
	cmp r7, #8
	bne .L_08193f92
	ldr r3, .L_08194068
	ldr r0, [sp, #132]
	ldrh r1, [r3, #16]
	ldr r2, [sp, #152]
	adds r1, r1, r0
	adds r1, r2, r1
	movs r3, #224
	ldr r2, [sp, #160]
	lsls r3, r3, #3
	movs r0, #9
	adds r1, r1, r3
	str r0, [sp, #0]
	mov r3, r9
	movs r0, #18
	str r0, [sp, #4]
	subs r2, #4
	subs r3, #9
	b .L_08194166
.L_08194056:
	movs r1, #147
	mov r5, r10
	lsls r1, r1, #1
	cmp r5, #11
	bgt .L_08194088
	adds r1, #100
	b .L_08194092
.L_08194064:
	.4byte Data_0819a040
.L_08194068:
	.4byte Data_08197410
.L_0819406c:
	.4byte Data_0819a048
.L_08194070:
	.4byte Data_0819a054
.L_08194074:
	.4byte Data_0819a060
.L_08194078:
	.4byte Data_0819a066
.L_0819407c:
	.4byte Data_08199f40
.L_08194080:
	.4byte Data_0819a06c
.L_08194084:
	.4byte IwramMulQ16
.L_08194088:
	mov r6, r10
	cmp r6, #23
	bgt .L_08194092
	movs r1, #172
	lsls r1, r1, #1
.L_08194092:
	ldr r2, [sp, #136]
	ldr r3, [sp, #140]
	mov r0, r10
	lsls r0, r0, #2
	adds r1, r2, r1
	str r0, [sp, #52]
	str r1, [sp, #120]
	str r3, [sp, #116]
	movs r7, #0
.L_081940a4:
	ldr r3, [sp, #52]
	lsls r5, r7, #13
	add r3, r10
	lsls r3, r3, #9
	adds r5, r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	ldr r4, [sp, #160]
	asrs r3, r3, #16
	adds r0, r5, #0
	adds r6, r3, r4
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	lsls r3, r3, #1
	asrs r3, r3, #16
	mov r1, r9
	subs r2, r6, #2
	adds r0, r3, r1
	movs r4, #5
	movs r6, #10
	str r4, [sp, #0]
	str r6, [sp, #4]
	ldr r1, [sp, #120]
	ldr r4, [sp, #116]
	subs r3, r0, #5
	ldr r0, [sp, #148]
	mov lr, r4
	.2byte 0xf800
	asrs r3, r5, #1
	adds r5, r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	mov r1, r8
	muls r1, r0
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r0, #160
	asrs r1, r1, #1
	ldr r6, .L_081942d8
	lsls r0, r0, #10
	mov lr, r6
	.2byte 0xf800
	ldr r1, [sp, #160]
	asrs r0, r0, #16
	adds r6, r0, r1
	adds r0, r5, #0
	bl Trig_Cos
	mov r1, r8
	muls r1, r0
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r0, #160
	asrs r1, r1, #1
	ldr r2, .L_081942d8
	lsls r0, r0, #11
	mov lr, r2
	.2byte 0xf800
	asrs r0, r0, #16
	add r0, r9
	movs r4, #5
	movs r5, #10
	subs r2, r6, #2
	subs r3, r0, #5
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #148]
	ldr r1, [sp, #120]
	ldr r6, [sp, #116]
	adds r7, #1
	mov lr, r6
	.2byte 0xf800
	cmp r7, #8
	bne .L_081940a4
	ldr r3, .L_081942dc
	ldr r0, [sp, #132]
	ldrh r1, [r3, #12]
	ldr r2, [sp, #152]
	adds r1, r1, r0
	adds r1, r2, r1
	movs r3, #224
	ldr r2, [sp, #160]
	lsls r3, r3, #3
	movs r0, #7
	adds r1, r1, r3
	str r0, [sp, #0]
	mov r3, r9
	movs r0, #14
	str r0, [sp, #4]
	subs r2, #3
	subs r3, #7
.L_08194166:
	ldr r0, [sp, #148]
	ldr r4, [sp, #140]
	mov lr, r4
	.2byte 0xf800
	b .L_0819430e
.L_08194170:
	adds r3, r0, r3
	cmp r10, r3
	bge .L_08194226
	mov r5, r10
	subs r5, r5, r0
	mov r8, r5
	mov r0, r8
	movs r1, #3
	bl Math_Div
	lsls r0, r0, #3
	cmp r0, #64
	bne .L_0819418c
	movs r0, #56
.L_0819418c:
	movs r6, #222
	ldr r1, [sp, #136]
	lsls r6, r6, #1
	adds r0, r0, r6
	adds r0, r1, r0
	str r0, [sp, #44]
	movs r7, #0
.L_0819419a:
	lsls r6, r7, #10
	adds r0, r6, #0
	bl Trig_Sin
	mov r5, r8
	muls r5, r0
	ldr r2, [sp, #160]
	asrs r5, r5, #16
	adds r0, r6, #0
	adds r5, r5, r2
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	lsls r3, r3, #1
	movs r2, #2
	asrs r3, r3, #16
	str r2, [sp, #0]
	add r3, r9
	movs r2, #4
	subs r5, #1
	str r2, [sp, #4]
	subs r3, #2
	ldr r0, [sp, #148]
	ldr r1, [sp, #44]
	adds r2, r5, #0
	ldr r4, [sp, #140]
	adds r7, #1
	mov lr, r4
	.2byte 0xf800
	cmp r7, #64
	bne .L_0819419a
	movs r7, #0
.L_081941dc:
	lsls r6, r7, #10
	adds r0, r6, #0
	bl Trig_Sin
	mov r5, r8
	muls r5, r0
	ldr r0, [sp, #160]
	lsls r5, r5, #1
	asrs r5, r5, #16
	adds r5, r5, r0
	adds r0, r6, #0
	bl Trig_Cos
	ldr r2, .L_081942e0
	ldr r1, [sp, #156]
	mov r3, r8
	muls r3, r0
	ldrb r2, [r2, r1]
	asrs r3, r3, #16
	lsls r2, r2, #1
	add r3, r9
	adds r3, r3, r2
	movs r2, #2
	str r2, [sp, #0]
	subs r5, #1
	movs r2, #4
	str r2, [sp, #4]
	subs r3, #2
	ldr r0, [sp, #148]
	ldr r1, [sp, #44]
	adds r2, r5, #0
	ldr r4, [sp, #140]
	adds r7, #1
	mov lr, r4
	.2byte 0xf800
	cmp r7, #64
	bne .L_081941dc
.L_08194226:
	ldr r5, [sp, #156]
	ldr r0, .L_081942e4
	lsls r6, r5, #2
	ldr r3, [r6, r0]
	mov r1, r10
	lsls r3, r3, #1
	movs r7, #0
	subs r3, r1, r3
	cmp r7, r3
	bge .L_0819427e
	mov r2, r9
	cmp r2, #112
	bgt .L_0819427e
	ldr r3, [sp, #160]
	ldr r4, [sp, #136]
	movs r5, #254
	lsls r5, r5, #1
	adds r5, r4, r5
	subs r3, #4
	str r5, [sp, #8]
	mov r8, r3
	mov r5, r9
.L_08194252:
	movs r3, #8
	str r3, [sp, #0]
	movs r3, #32
	str r3, [sp, #4]
	ldr r1, [sp, #8]
	adds r3, r5, #0
	ldr r0, [sp, #148]
	mov r2, r8
	ldr r4, [sp, #140]
	mov lr, r4
	.2byte 0xf800
	ldr r0, .L_081942e4
	mov r1, r10
	ldr r3, [r6, r0]
	adds r7, #1
	lsls r3, r3, #1
	subs r3, r1, r3
	adds r5, #32
	cmp r7, r3
	bge .L_0819427e
	cmp r5, #112
	ble .L_08194252
.L_0819427e:
	ldr r2, .L_081942e4
	ldr r3, [r6, r2]
	lsls r2, r3, #1
	adds r3, r2, #0
	adds r3, #8
	cmp r10, r3
	bge .L_081942ac
	mov r4, r10
	subs r3, r2, r4
	adds r3, #8
	ldr r0, .L_081942e8
	asrs r2, r3, #1
	movs r3, #1
	movs r1, #32
	ands r3, r4
	strh r1, [r0, #6]
	cmp r3, #0
	beq .L_081942a8
	adds r3, r2, #0
	adds r3, #32
	b .L_081942aa
.L_081942a8:
	subs r3, r1, r2
.L_081942aa:
	strh r3, [r0, #6]
.L_081942ac:
	ldr r5, [sp, #156]
	cmp r5, #0
	bne .L_081942ec
	ldr r6, [sp, #152]
	movs r0, #136
	lsls r0, r0, #6
	adds r0, #18
	ldr r2, [sp, #160]
	adds r1, r6, r0
	movs r0, #20
	mov r3, r9
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	subs r2, #10
	subs r3, #20
	ldr r0, [sp, #148]
	ldr r4, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	b .L_0819430e
	.2byte 0x0000
.L_081942d8:
	.4byte IwramMulQ16
.L_081942dc:
	.4byte Data_08197410
.L_081942e0:
	.4byte Data_08199f48
.L_081942e4:
	.4byte Data_08199f40
.L_081942e8:
	.4byte Data_03001120
.L_081942ec:
	ldr r5, [sp, #152]
	movs r6, #166
	ldr r2, [sp, #160]
	movs r0, #12
	lsls r6, r6, #7
	adds r6, #18
	mov r3, r9
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	adds r1, r5, r6
	subs r2, #6
	subs r3, #12
	ldr r0, [sp, #148]
	ldr r4, [sp, #144]
	mov lr, r4
	.2byte 0xf800
.L_0819430e:
	ldr r3, .L_0819437c
	ldr r5, [sp, #28]
	mov r6, r11
	ldrh r2, [r3, r5]
	ldr r3, .L_08194380
	ldr r0, [sp, #168]
	ldrb r3, [r3, r6]
	lsls r3, r3, #4
	subs r2, r2, r3
	ldr r3, [r0]
	adds r2, #48
	cmp r2, r3
	bne .L_081943f2
	ldr r6, [sp, #184]
	movs r1, #0
	mov r10, r1
	movs r7, #31
.L_08194330:
	add r2, sp, #420
	mov r9, r2
	bl Func_0819284c
	ldr r1, [r6]
	ldr r3, .L_08194378
	lsls r2, r0, #1
	strh r3, [r1, r2]
	ldr r3, .L_08194384
	mov r4, r11
	ldrb r3, [r3, r4]
	lsls r5, r0, #2
	adds r2, r5, #0
	adds r2, #176
	lsls r3, r3, #17
	str r3, [r1, r2]
	ldr r3, .L_08194388
	movs r0, #160
	ldrb r3, [r3, r4]
	movs r4, #128
	lsls r4, r4, #14
	lsls r0, r0, #1
	lsls r3, r3, #16
	adds r3, r3, r4
	adds r2, r5, r0
	str r3, [r1, r2]
	bl Random16
	ldr r2, [r6]
	movs r1, #232
	ands r0, r7
	lsls r1, r1, #1
	subs r0, #16
	adds r3, r5, r1
	lsls r0, r0, #12
	b .L_0819438c
.L_08194378:
	.4byte 0x00007900
.L_0819437c:
	.4byte Data_0819a048
.L_08194380:
	.4byte Data_0819a06c
.L_08194384:
	.4byte Data_0819a060
.L_08194388:
	.4byte Data_0819a066
.L_0819438c:
	str r0, [r2, r3]
	bl Random16
	movs r4, #152
	ldr r2, [r6]
	lsls r4, r4, #2
	ands r0, r7
	adds r3, r5, r4
	negs r0, r0
	movs r5, #1
	lsls r0, r0, #12
	add r10, r5
	str r0, [r2, r3]
	mov r0, r10
	cmp r0, #6
	bne .L_08194330
	b .L_081943f2
.L_081943ae:
	ldr r1, [sp, #28]
	ldr r3, .L_081944e0
	ldr r6, [sp, #168]
	mov r4, r11
	ldrh r2, [r0, r1]
	ldrb r3, [r3, r4]
	ldr r5, [r6]
	adds r3, r2, r3
	cmp r5, r3
	bge .L_081943f2
	ldr r3, .L_081944e4
	subs r5, r5, r2
	ldrb r1, [r3, r4]
	adds r0, r5, #0
	bl Math_Div
	ldr r3, .L_081944e8
	mov r2, r11
	ldrb r1, [r3, r2]
	ldr r3, .L_081944ec
	mov r4, r11
	ldrb r2, [r3, r2]
	ldr r3, .L_081944f0
	ldrb r3, [r3, r4]
	muls r3, r5
	lsrs r4, r3, #31
	adds r3, r3, r4
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, .L_081944f4
	mov r5, r11
	ldrb r3, [r3, r5]
	bl Func_08192648
.L_081943f2:
	ldr r6, [sp, #28]
	movs r0, #1
	add r11, r0
	adds r6, #2
	mov r1, r11
	str r6, [sp, #28]
	cmp r1, #6
	beq .L_08194404
	b .L_08193e9c
.L_08194404:
	ldr r3, [sp, #168]
	ldr r4, .L_081944f8
	ldr r2, [r3]
	adds r3, r2, r4
	cmp r3, #132
	bls .L_08194418
	movs r5, #223
	lsls r5, r5, #1
	cmp r2, r5
	ble .L_08194420
.L_08194418:
	add r6, sp, #420
	mov r9, r6
	bl Func_08192894
.L_08194420:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #168]
	movs r1, #185
	ldr r3, [r0]
	lsls r1, r1, #1
	cmp r3, r1
	bne .L_081944b8
	ldr r2, [sp, #64]
	movs r3, #160
	ldr r0, [r2]
	lsls r3, r3, #4
	adds r3, #232
	movs r2, #128
	adds r0, r0, r3
	movs r1, #16
	lsls r2, r2, #9
	bl Func_0815b434
	ldr r5, [sp, #64]
	movs r4, #0
	mov r10, r4
	movs r7, #0
	movs r6, #0
.L_08194452:
	ldr r1, [r5]
	movs r2, #160
	adds r4, r7, #0
	lsls r2, r2, #4
	adds r0, r1, r4
	adds r2, #232
	adds r1, r1, r6
	adds r0, r0, r2
	subs r2, #230
	adds r1, r1, r2
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [r5]
	movs r3, #0
	adds r4, r4, r2
	mov r11, r3
	movs r0, #160
	adds r3, r6, r2
	movs r2, #159
	lsls r0, r0, #4
	lsls r2, r2, #4
	adds r0, #10
	adds r2, #255
	adds r1, r3, r0
	adds r4, r4, r2
.L_08194490:
	ldrb r3, [r4]
	subs r4, #1
	strb r3, [r1]
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r1, #1
	cmp r0, #8
	bne .L_08194490
	add r10, r3
	mov r1, r10
	adds r7, #8
	adds r6, #16
	cmp r1, #16
	bne .L_08194452
	movs r2, #128
	ldr r3, .L_081944d8
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_081944b8:
	ldr r2, [sp, #168]
	ldr r4, .L_081944fc
	ldr r0, [r2]
	adds r3, r0, r4
	cmp r3, #7
	bhi .L_08194504
	ldr r5, .L_08194500
	ldr r2, .L_081944dc
	movs r1, #128
	adds r3, r0, r5
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	b .L_08194504
	.2byte 0x0000
.L_081944d8:
	.4byte 0x00001008
.L_081944dc:
	.4byte 0x00001000
.L_081944e0:
	.4byte Data_0819a078
.L_081944e4:
	.4byte Data_0819a07e
.L_081944e8:
	.4byte Data_0819a060
.L_081944ec:
	.4byte Data_0819a066
.L_081944f0:
	.4byte Data_0819a072
.L_081944f4:
	.4byte Data_0819a06c
.L_081944f8:
	.4byte 0xfffffee9
.L_081944fc:
	.4byte 0xfffffe62
.L_08194500:
	.4byte 0xfffffe6a
.L_08194504:
	movs r6, #130
	adds r6, #255
	cmp r0, r6
	ble .L_08194522
	lsls r2, r0, #1
	movs r3, #144
	ldr r1, .L_08194688
	adds r2, r2, r0
	lsls r3, r3, #4
	lsls r2, r2, #1
	adds r3, #252
	subs r3, r3, r2
	str r3, [r1, #16]
	ldr r1, [sp, #168]
	ldr r0, [r1]
.L_08194522:
	movs r2, #211
	lsls r2, r2, #1
	cmp r0, r2
	ble .L_0819452c
	b .L_0819465a
.L_0819452c:
	movs r3, #185
	lsls r3, r3, #1
	cmp r0, r3
	ble .L_08194548
	ldr r4, .L_0819468c
	adds r3, r0, r4
	lsls r2, r3, #3
	cmp r2, #127
	ble .L_08194540
	movs r2, #127
.L_08194540:
	movs r0, #3
	movs r1, #34
	bl Func_0819273c
.L_08194548:
	ldr r5, [sp, #168]
	movs r6, #120
	ldr r3, [r5]
	adds r6, #255
	cmp r3, r6
	ble .L_08194580
	ldr r2, .L_08194690
	add r0, sp, #356
	adds r3, r0, #0
	ldmia r2!, {r1, r4, r5}
	stmia r3!, {r1, r4, r5}
	ldmia r2!, {r1, r6}
	stmia r3!, {r1, r6}
	ldr r2, [sp, #168]
	ldr r4, .L_08194694
	ldr r3, [r2]
	adds r3, r3, r4
	lsls r2, r3, #3
	cmp r2, #127
	ble .L_08194572
	movs r2, #127
.L_08194572:
	asrs r1, r2, #5
	lsls r3, r1, #2
	lsls r1, r1, #1
	ldr r0, [r0, r3]
	adds r1, #94
	bl Func_0819273c
.L_08194580:
	ldr r5, [sp, #168]
	movs r6, #190
	ldr r3, [r5]
	lsls r6, r6, #1
	cmp r3, r6
	ble .L_081945c4
	ldr r3, .L_08194698
	add r6, sp, #328
	adds r2, r6, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r5}
	stmia r2!, {r0, r1, r5}
	ldr r3, [r3]
	ldr r4, .L_0819469c
	str r3, [r2]
	ldr r2, [sp, #168]
	ldr r3, [r2]
	adds r3, r3, r4
	lsls r5, r3, #2
	cmp r5, #127
	ble .L_081945ae
	movs r5, #127
.L_081945ae:
	movs r1, #19
	adds r0, r5, #0
	bl Math_Div
	adds r1, r0, #0
	lsls r3, r1, #2
	ldr r0, [r6, r3]
	adds r1, #12
	adds r2, r5, #0
	bl Func_0819273c
.L_081945c4:
	ldr r5, [sp, #168]
	movs r6, #191
	ldr r3, [r5]
	lsls r6, r6, #1
	cmp r3, r6
	ble .L_0819460a
	ldr r3, .L_081946a0
	add r6, sp, #328
	adds r2, r6, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r5}
	stmia r2!, {r0, r1, r5}
	ldr r3, [r3]
	ldr r4, .L_081946a4
	str r3, [r2]
	ldr r2, [sp, #168]
	ldr r3, [r2]
	adds r3, r3, r4
	lsls r5, r3, #3
	cmp r5, #127
	ble .L_081945f2
	movs r5, #127
.L_081945f2:
	movs r1, #21
	adds r0, r5, #0
	bl Math_Div
	adds r1, r0, #0
	lsls r3, r1, #2
	lsls r1, r1, #1
	ldr r0, [r6, r3]
	adds r1, #70
	adds r2, r5, #0
	bl Func_0819273c
.L_0819460a:
	ldr r5, [sp, #168]
	movs r6, #130
	ldr r3, [r5]
	adds r6, #255
	cmp r3, r6
	ble .L_0819462c
	ldr r0, .L_081946a8
	adds r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #127
	ble .L_08194622
	movs r2, #127
.L_08194622:
	asrs r1, r2, #3
	adds r1, #48
	movs r0, #14
	bl Func_0819273c
.L_0819462c:
	ldr r1, [sp, #168]
	movs r2, #211
	ldr r3, [r1]
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_0819465a
	ldr r4, [sp, #64]
	movs r6, #238
	ldr r3, [r4]
	movs r5, #239
	lsls r6, r6, #7
	lsls r5, r5, #7
	adds r6, #132
	adds r2, r3, r5
	movs r1, #0
	adds r3, r3, r6
	str r1, [r2]
	str r1, [r3]
	movs r2, #128
	ldr r3, .L_08194684
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0819465a:
	ldr r0, [sp, #168]
	ldr r1, .L_081946ac
	ldr r2, [r0]
	adds r3, r2, r1
	cmp r3, #19
	bhi .L_08194706
	ldr r4, .L_081946b0
	movs r7, #120
	adds r3, r2, r4
	lsls r3, r3, #3
	subs r3, r7, r3
	mov r10, r3
	adds r3, #40
	mov r7, r10
	cmp r10, r3
	beq .L_08194706
	mov r5, r10
	movs r6, #0
	cmp r5, #127
	bgt .L_08194706
	b .L_081946b4
.L_08194684:
	.4byte 0x00001010
.L_08194688:
	.4byte gCameraSceneParameters
.L_0819468c:
	.4byte 0xfffffe8e
.L_08194690:
	.4byte Data_08196f50
.L_08194694:
	.4byte 0xfffffe89
.L_08194698:
	.4byte Data_08196f64
.L_0819469c:
	.4byte 0xfffffe84
.L_081946a0:
	.4byte Data_08196f80
.L_081946a4:
	.4byte 0xfffffe82
.L_081946a8:
	.4byte 0xfffffe7f
.L_081946ac:
	.4byte 0xfffffe59
.L_081946b0:
	.4byte 0xfffffe5a
.L_081946b4:
	mov r12, r3
.L_081946b6:
	mov r0, r10
	cmp r0, #0
	blt .L_081946f2
	cmp r6, #63
	bls .L_081946c2
	movs r6, #63
.L_081946c2:
	mov r1, r10
	movs r2, #7
	ands r2, r1
	asrs r3, r1, #3
	ldr r4, [sp, #176]
	lsls r2, r2, #3
	lsls r3, r3, #10
	adds r2, r2, r3
	movs r5, #0
.L_081946d4:
	movs r0, #0
.L_081946d6:
	ldr r3, [r4]
	adds r1, r3, r2
	ldrb r3, [r1]
	cmp r3, r6
	bcs .L_081946e2
	strb r6, [r1]
.L_081946e2:
	adds r0, #1
	adds r2, #1
	cmp r0, #8
	bne .L_081946d6
	adds r5, #8
	adds r2, #56
	cmp r5, #128
	bne .L_081946d4
.L_081946f2:
	movs r2, #1
	add r10, r2
	cmp r10, r12
	beq .L_08194706
	mov r4, r10
	subs r3, r4, r7
	lsls r3, r3, #25
	lsrs r6, r3, #24
	cmp r4, #127
	ble .L_081946b6
.L_08194706:
	ldr r5, [sp, #168]
	ldr r6, .L_08194754
	ldr r2, [r5]
	adds r3, r2, r6
	cmp r3, #30
	bhi .L_08194772
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08194772
	ldr r0, .L_08194758
	bl Resource_GetTableEntry
	movs r0, #160
	ldr r4, .L_08194750
	lsls r0, r0, #19
	movs r1, #0
	adds r0, #192
	mov r10, r1
.L_0819472c:
	ldrh r3, [r0]
	movs r1, #31
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r4
	ands r3, r4
	cmp r1, #3
	ble .L_08194742
	subs r1, #2
.L_08194742:
	cmp r2, #3
	ble .L_08194748
	subs r2, #2
.L_08194748:
	cmp r3, #3
	ble .L_0819475c
	subs r3, #2
	b .L_0819475c
.L_08194750:
	.4byte 0x0000001f
.L_08194754:
	.4byte 0xfffffe6f
.L_08194758:
	.4byte 0x00000045
.L_0819475c:
	lsls r2, r2, #5
	lsls r3, r3, #10
	orrs r3, r2
	movs r2, #1
	orrs r3, r1
	add r10, r2
	strh r3, [r0]
	mov r3, r10
	adds r0, #2
	cmp r3, #128
	bne .L_0819472c
.L_08194772:
	ldr r4, [sp, #168]
	movs r5, #223
	ldr r3, [r4]
	lsls r5, r5, #1
	cmp r3, r5
	beq .L_08194780
	b .L_0819496c
.L_08194780:
	ldr r6, [sp, #176]
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_081947b8
	ldr r3, .L_081947bc
	ldr r0, [r6]
	mov lr, r3
	.2byte 0xf800
	bl Func_08014de4
	movs r0, #1
	ldr r1, .L_081947c0
	movs r2, #0
	bl Func_08118040
	movs r4, #0
	movs r7, #192
	movs r0, #160
	mov r10, r4
	ldr r4, .L_081947b4
	lsls r7, r7, #2
	lsls r0, r0, #19
	adds r7, #2
	adds r0, #192
	movs r5, #31
	b .L_081947c4
.L_081947b4:
	.4byte 0x0000001f
.L_081947b8:
	.4byte 0x3f3f3f3f
.L_081947bc:
	.4byte IwramFillWords
.L_081947c0:
	.4byte 0x00000077
.L_081947c4:
	ldrh r3, [r0]
	adds r1, r5, #0
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r3, r3, #26
	ands r2, r4
	ands r3, r4
	adds r2, #1
	adds r3, #1
	lsls r3, r3, #10
	lsls r2, r2, #5
	movs r6, #1
	adds r1, #1
	orrs r3, r2
	add r10, r6
	orrs r3, r1
	mov r1, r10
	strh r3, [r0]
	adds r0, #2
	cmp r1, #128
	bne .L_081947c4
	bl Func_0815b410
	movs r2, #0
	mov r10, r2
	movs r6, #28
.L_081947fa:
	ldr r3, [sp, #64]
	mov r5, r10
	ldr r0, [r3]
	movs r4, #224
	adds r5, #14
	adds r0, r0, r7
	lsls r4, r4, #3
	movs r2, #128
	adds r0, r0, r4
	adds r1, r5, #0
	lsls r2, r2, #9
	bl Func_0815b434
	adds r3, r6, #0
	muls r3, r5
	movs r5, #2
	add r10, r5
	mov r0, r10
	adds r7, r7, r3
	adds r6, #4
	cmp r0, #6
	bne .L_081947fa
	ldr r1, [sp, #64]
	movs r2, #248
	ldr r0, [r1]
	lsls r2, r2, #5
	adds r2, #16
	movs r1, #128
	adds r0, r0, r2
	lsls r1, r1, #5
	movs r2, #0
	ldr r3, .L_081948c8
	mov lr, r3
	.2byte 0xf800
	movs r4, #202
	lsls r4, r4, #1
	add r4, sp
	str r4, [sp, #64]
	movs r6, #156
	ldr r0, [r4]
	lsls r6, r6, #6
	movs r5, #128
	lsls r5, r5, #9
	adds r6, #16
	adds r3, r5, #0
	adds r0, r0, r6
	movs r1, #16
	movs r2, #64
	bl Func_08191c20
	ldr r1, [sp, #64]
	movs r2, #188
	ldr r0, [r1]
	lsls r2, r2, #6
	adds r2, #16
	adds r3, r5, #0
	adds r0, r0, r2
	movs r1, #16
	movs r2, #64
	bl Func_08191c20
	ldr r4, [sp, #64]
	movs r5, #0
	ldr r3, [r4]
	mov r10, r5
	adds r2, r3, r6
.L_0819487e:
	movs r6, #0
	mov r11, r6
.L_08194882:
	ldrb r3, [r2]
	cmp r3, r10
	ble .L_0819488c
	mov r0, r10
	strb r0, [r2]
.L_0819488c:
	movs r1, #1
	add r11, r1
	mov r3, r11
	adds r2, #1
	cmp r3, #32
	bne .L_08194882
	add r10, r1
	mov r4, r10
	cmp r4, #64
	bne .L_0819487e
	ldr r6, [sp, #184]
	ldr r2, .L_081948c4
	ldr r3, [r6]
	movs r5, #0
	mov r10, r5
.L_081948aa:
	movs r0, #1
	add r10, r0
	mov r1, r10
	strh r2, [r3]
	adds r3, #2
	cmp r1, #36
	bne .L_081948aa
	ldr r3, [sp, #404]
	movs r2, #0
	mov r10, r2
	adds r3, #24
	b .L_081948cc
	.2byte 0x0000
.L_081948c4:
	.4byte 0x00000000
.L_081948c8:
	.4byte IwramFillWords
.L_081948cc:
	movs r4, #1
	add r10, r4
	mov r5, r10
	str r2, [r3]
	adds r3, #28
	cmp r5, #32
	bne .L_081948cc
	movs r1, #192
	movs r0, #224
	lsls r1, r1, #2
	add r6, sp, #404
	lsls r0, r0, #3
	adds r1, #2
	mov r9, r6
	mov r10, r4
	mov r8, r0
	movs r6, #8
	mov lr, r1
.L_081948f0:
	movs r2, #0
	mov r11, r2
	mov r7, lr
.L_081948f6:
	ldr r5, .L_08194b0c
	mov r4, r11
	lsls r3, r4, #1
	mov r0, r9
	ldrh r2, [r5, r3]
	ldr r1, [r0]
	mov r4, r8
	adds r3, r1, r2
	adds r2, r2, r7
	adds r1, r1, r2
	mov r2, r11
	adds r2, #1
	adds r0, r3, r4
	lsls r3, r2, #1
	muls r3, r2
	add r1, r8
	movs r5, #0
	cmp r3, #0
	beq .L_0819493a
	adds r4, r6, #0
	mov r12, r3
.L_08194920:
	ldrb r3, [r0]
	subs r3, r3, r4
	lsls r3, r3, #24
	lsrs r3, r3, #24
	cmp r3, #63
	bls .L_0819492e
	movs r3, #0
.L_0819492e:
	adds r5, #1
	strb r3, [r1]
	adds r0, #1
	adds r1, #1
	cmp r5, r12
	bne .L_08194920
.L_0819493a:
	mov r11, r2
	cmp r2, #10
	bne .L_081948f6
	movs r5, #192
	movs r0, #1
	lsls r5, r5, #2
	add r10, r0
	adds r5, #2
	mov r1, r10
	adds r6, #8
	add lr, r5
	cmp r1, #8
	bne .L_081948f0
	ldr r2, [sp, #404]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r1, r2, r3
	adds r4, #132
	movs r3, #2
	str r3, [r1]
	adds r2, r2, r4
	movs r3, #70
	str r3, [r2]
.L_0819496c:
	ldr r5, [sp, #168]
	ldr r6, .L_08194b10
	ldr r2, [r5]
	adds r3, r2, r6
	cmp r3, #15
	bhi .L_081949e6
	ldr r0, .L_08194b14
	adds r3, r2, r0
	lsls r3, r3, #3
	movs r2, #136
	subs r2, r2, r3
	mov r10, r2
	mov r12, r10
	cmp r2, #0
	beq .L_081949e6
	movs r7, #0
.L_0819498c:
	mov r1, r10
	lsrs r4, r7, #24
	cmp r1, #127
	bgt .L_081949d4
	cmp r4, #63
	bls .L_0819499a
	movs r4, #63
.L_0819499a:
	mov r3, r12
	subs r3, #8
	cmp r3, r10
	bge .L_081949a4
	movs r4, #0
.L_081949a4:
	mov r3, r10
	movs r2, #7
	ands r2, r3
	ldr r5, [sp, #176]
	asrs r3, r3, #3
	lsls r2, r2, #3
	lsls r3, r3, #10
	adds r2, r2, r3
	movs r6, #0
.L_081949b6:
	movs r0, #0
.L_081949b8:
	ldr r3, [r5]
	adds r1, r3, r2
	ldrb r3, [r1]
	cmp r3, r4
	bcs .L_081949c4
	strb r4, [r1]
.L_081949c4:
	adds r0, #1
	adds r2, #1
	cmp r0, #8
	bne .L_081949b8
	adds r6, #8
	adds r2, #56
	cmp r6, #128
	bne .L_081949b6
.L_081949d4:
	movs r5, #1
	negs r5, r5
	movs r4, #128
	add r10, r5
	lsls r4, r4, #18
	mov r6, r10
	adds r7, r7, r4
	cmp r6, #0
	bne .L_0819498c
.L_081949e6:
	ldr r0, [sp, #168]
	movs r1, #223
	ldr r3, [r0]
	lsls r1, r1, #1
	cmp r3, r1
	ble .L_081949fe
	add r2, sp, #420
	mov r9, r2
	bl Func_08192a4c
	ldr r4, [sp, #168]
	ldr r3, [r4]
.L_081949fe:
	ldr r5, .L_08194b10
	adds r3, r3, r5
	cmp r3, #194
	bls .L_08194a08
	b .L_08194d7a
.L_08194a08:
	movs r0, #80
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #112]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08194b18
	ldr r3, [sp, #216]
	mov r11, r0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08194b1c
	mov r6, r11
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #216]
	bl Func_08014ee0
	movs r3, #4
	str r3, [r6]
	ldr r3, .L_08194b20
	movs r2, #154
	str r3, [r6, #8]
	ldr r0, [sp, #112]
	lsls r2, r2, #1
	str r0, [r6, #12]
	ldr r1, [sp, #168]
	adds r2, #255
	ldr r3, [r1]
	cmp r3, r2
	ble .L_08194a50
	b .L_08194d6e
.L_08194a50:
	movs r4, #250
	movs r2, #128
	lsls r4, r4, #1
	lsls r2, r2, #5
	cmp r3, r4
	ble .L_08194a70
	lsls r3, r3, #6
	movs r5, #250
	subs r3, r2, r3
	lsls r5, r5, #7
	adds r3, r3, r5
	lsls r3, r3, #16
	asrs r2, r3, #16
	cmp r2, #0
	bge .L_08194a70
	movs r2, #0
.L_08194a70:
	ldr r6, [sp, #64]
	movs r0, #248
	ldr r3, [r6]
	lsls r0, r0, #5
	adds r3, r3, r2
	adds r0, #16
	mov r1, sp
	adds r3, r3, r0
	adds r1, #216
	str r1, [sp, #108]
	str r3, [r1, #4]
	ldr r2, [sp, #168]
	ldr r3, [r2]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_08194b6a
	movs r3, #148
	lsls r3, r3, #1
	add r3, sp
	mov r10, r3
	ldr r3, .L_08194b24
	mov r2, r10
	ldmia r3!, {r4, r5, r6}
	stmia r2!, {r4, r5, r6}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	add r0, sp, #420
	ldmia r3!, {r5, r6}
	stmia r2!, {r5, r6}
	mov r9, r0
	bl Func_0819284c
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	beq .L_08194b6a
	bl Random16
	ldr r2, [sp, #164]
	adds r6, r0, #0
	movs r3, #127
	ldr r1, [r2]
	ands r6, r3
	ldr r3, .L_08194b08
	lsls r2, r5, #1
	strh r3, [r1, r2]
	bl Random16
	ldr r3, [sp, #164]
	lsls r7, r5, #2
	adds r5, r6, #1
	movs r4, #176
	asrs r5, r5, #5
	ldr r2, [r3]
	lsls r5, r5, #3
	adds r4, r4, r7
	adds r3, r5, #4
	mov r8, r4
	mov r4, r10
	ldr r1, [r4, r3]
	str r2, [sp, #20]
	bl Math_ModU
	mov r1, r10
	ldr r3, [r1, r5]
	ldr r2, [sp, #20]
	adds r3, r3, r0
	movs r5, #160
	movs r0, #128
	mov r4, r8
	lsls r3, r3, #16
	lsls r5, r5, #1
	lsls r0, r0, #13
	b .L_08194b28
.L_08194b08:
	.4byte 0xffff8400
.L_08194b0c:
	.4byte Data_08197410
.L_08194b10:
	.4byte 0xfffffe41
.L_08194b14:
	.4byte 0xfffffe42
.L_08194b18:
	.4byte 0xffffff00
.L_08194b1c:
	.4byte 0xffff00ff
.L_08194b20:
	.4byte Data_0819a0d4
.L_08194b24:
	.4byte Data_08196f9c
.L_08194b28:
	lsls r6, r6, #16
	str r3, [r2, r4]
	adds r6, r6, r0
	adds r3, r7, r5
	str r6, [r2, r3]
	bl Random16
	ldr r2, [sp, #164]
	movs r3, #232
	lsls r3, r3, #1
	ldr r1, [r2]
	adds r2, r7, r3
	movs r3, #15
	ands r0, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #11
	str r3, [r1, r2]
	bl Random16
	movs r2, #31
	ldr r5, [sp, #164]
	ands r0, r2
	lsls r2, r0, #1
	ldr r3, .L_08194c50
	ldr r4, [r5]
	movs r6, #152
	adds r2, r2, r0
	lsls r6, r6, #2
	lsls r2, r2, #11
	adds r1, r7, r6
	subs r3, r3, r2
	str r3, [r4, r1]
.L_08194b6a:
	ldr r3, [sp, #108]
	mov r4, r11
	str r3, [r4, #16]
	ldr r0, .L_08194c54
	ldr r1, [sp, #112]
	movs r2, #10
	bl Func_08196958
	mov r0, r11
	bl Func_08196a7c
	ldr r5, [sp, #168]
	movs r6, #154
	ldr r3, [r5]
	lsls r6, r6, #1
	adds r6, #255
	cmp r3, r6
	ble .L_08194b90
	b .L_08194d6e
.L_08194b90:
	movs r0, #235
	movs r2, #128
	lsls r0, r0, #1
	lsls r2, r2, #5
	cmp r3, r0
	ble .L_08194bb0
	lsls r3, r3, #6
	movs r1, #235
	subs r3, r2, r3
	lsls r1, r1, #7
	adds r3, r3, r1
	lsls r3, r3, #16
	asrs r2, r3, #16
	cmp r2, #0
	bge .L_08194bb0
	movs r2, #0
.L_08194bb0:
	ldr r4, [sp, #64]
	movs r5, #248
	ldr r3, [r4]
	lsls r5, r5, #5
	ldr r6, [sp, #108]
	adds r3, r3, r2
	adds r5, #16
	adds r3, r3, r5
	str r3, [r6, #4]
	ldr r0, [sp, #168]
	movs r3, #7
	ldr r2, [r0]
	ands r3, r2
	cmp r3, #0
	bne .L_08194c9e
	movs r1, #246
	adds r1, #255
	cmp r2, r1
	bgt .L_08194c9e
	ldr r3, .L_08194c58
	movs r2, #132
	lsls r2, r2, #1
	add r2, sp
	mov r10, r2
	ldmia r3!, {r4, r5, r6}
	stmia r2!, {r4, r5, r6}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	add r0, sp, #420
	ldmia r3!, {r5, r6}
	stmia r2!, {r5, r6}
	mov r9, r0
	bl Func_0819284c
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	beq .L_08194c9e
	bl Random16
	ldr r2, [sp, #164]
	adds r6, r0, #0
	movs r3, #127
	ldr r1, [r2]
	ands r6, r3
	ldr r3, .L_08194c4c
	lsls r2, r5, #1
	strh r3, [r1, r2]
	bl Random16
	ldr r3, [sp, #164]
	lsls r7, r5, #2
	adds r5, r6, #1
	movs r4, #176
	asrs r5, r5, #5
	ldr r2, [r3]
	lsls r5, r5, #3
	adds r4, r4, r7
	adds r3, r5, #4
	mov r8, r4
	mov r4, r10
	ldr r1, [r4, r3]
	str r2, [sp, #20]
	bl Math_ModU
	mov r1, r10
	ldr r3, [r1, r5]
	ldr r2, [sp, #20]
	adds r3, r3, r0
	movs r5, #160
	movs r0, #128
	mov r4, r8
	lsls r3, r3, #16
	lsls r5, r5, #1
	lsls r0, r0, #13
	b .L_08194c5c
	.2byte 0x0000
.L_08194c4c:
	.4byte 0xffff8400
.L_08194c50:
	.4byte 0xffff8000
.L_08194c54:
	.4byte Data_0819a084
.L_08194c58:
	.4byte Data_08196fbc
.L_08194c5c:
	lsls r6, r6, #16
	str r3, [r2, r4]
	adds r6, r6, r0
	adds r3, r7, r5
	str r6, [r2, r3]
	bl Random16
	ldr r2, [sp, #164]
	movs r3, #232
	lsls r3, r3, #1
	ldr r1, [r2]
	adds r2, r7, r3
	movs r3, #15
	ands r0, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #11
	str r3, [r1, r2]
	bl Random16
	movs r2, #31
	ldr r5, [sp, #164]
	ands r0, r2
	lsls r2, r0, #1
	ldr r3, .L_08194cf0
	ldr r4, [r5]
	movs r6, #152
	adds r2, r2, r0
	lsls r6, r6, #2
	lsls r2, r2, #11
	adds r1, r7, r6
	subs r3, r3, r2
	str r3, [r4, r1]
.L_08194c9e:
	ldr r4, [sp, #168]
	movs r2, #3
	ldr r3, [r4]
	ands r3, r2
	cmp r3, #0
	bne .L_08194d58
	add r5, sp, #420
	mov r9, r5
	bl Func_081929fc
	adds r6, r0, #0
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	beq .L_08194d58
	ldr r1, [sp, #64]
	lsls r7, r6, #3
	ldr r2, [r1]
	subs r3, r7, r6
	lsls r5, r3, #2
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, r5]
	ldr r2, [sp, #168]
	movs r4, #254
	ldr r3, [r2]
	adds r4, #255
	cmp r3, r4
	bgt .L_08194cf4
	bl Random16
	ldr r2, [sp, #64]
	ldr r1, [r2]
	movs r2, #63
	ldr r3, [r1, r5]
	ands r2, r0
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r1, r5]
	b .L_08194d08
	.2byte 0x0000
.L_08194cf0:
	.4byte 0xffff8000
.L_08194cf4:
	bl Random16
	ldr r3, [sp, #64]
	movs r4, #31
	ldr r2, [r3]
	ands r0, r4
	ldr r3, [r2, r5]
	lsls r0, r0, #16
	adds r3, r3, r0
	str r3, [r2, r5]
.L_08194d08:
	ldr r5, [sp, #64]
	subs r3, r7, r6
	ldr r1, [r5]
	lsls r5, r3, #2
	movs r3, #254
	adds r2, r5, #4
	lsls r3, r3, #15
	str r3, [r1, r2]
	bl Random16
	ldr r6, [sp, #64]
	movs r3, #15
	ldr r1, [r6]
	ands r3, r0
	adds r2, r5, #0
	adds r2, #12
	lsls r3, r3, #12
	str r3, [r1, r2]
	bl Random16
	movs r2, #31
	ands r0, r2
	lsls r2, r0, #3
	ldr r3, .L_08194dc4
	ldr r4, [r6]
	subs r2, r2, r0
	lsls r2, r2, #11
	adds r1, r5, #0
	subs r3, r3, r2
	adds r1, #16
	str r3, [r4, r1]
	bl Random16
	ldr r1, [r6]
	movs r3, #3
	adds r2, r5, #0
	ands r3, r0
	adds r2, #24
	adds r3, #1
	str r3, [r1, r2]
.L_08194d58:
	ldr r3, [sp, #108]
	mov r4, r11
	str r3, [r4, #16]
	ldr r0, .L_08194dc8
	ldr r1, [sp, #112]
	movs r2, #10
	bl Func_08196958
	mov r0, r11
	bl Func_08196a7c
.L_08194d6e:
	mov r0, r11
	bl Sys_Free
	ldr r0, [sp, #112]
	bl Sys_Free
.L_08194d7a:
	ldr r5, [sp, #168]
	ldr r6, .L_08194dcc
	ldr r2, [r5]
	adds r3, r2, r6
	cmp r3, #209
	bhi .L_08194da8
	ldr r3, .L_08194dbc
	movs r0, #252
	subs r3, r3, r2
	lsls r3, r3, #16
	lsls r0, r0, #14
	asrs r1, r3, #16
	cmp r3, r0
	bls .L_08194d98
	movs r1, #63
.L_08194d98:
	ldr r3, .L_08194dd0
	movs r0, #0
	adds r2, r2, r3
	lsls r3, r1, #24
	lsrs r3, r3, #24
	movs r1, #127
	bl Func_08192828
.L_08194da8:
	ldr r4, [sp, #168]
	ldr r5, .L_08194dd4
	ldr r0, [r4]
	adds r3, r0, r5
	cmp r3, #209
	bhi .L_08194e12
	ldr r3, .L_08194dc0
	movs r6, #252
	b .L_08194dd8
	.2byte 0x0000
.L_08194dbc:
	.4byte 0x000002d3
.L_08194dc0:
	.4byte 0x000002dc
.L_08194dc4:
	.4byte 0xffff8000
.L_08194dc8:
	.4byte Data_0819a0ac
.L_08194dcc:
	.4byte 0xfffffdff
.L_08194dd0:
	.4byte 0xfffffe00
.L_08194dd4:
	.4byte 0xfffffdf6
.L_08194dd8:
	subs r3, r3, r0
	lsls r3, r3, #16
	lsls r6, r6, #14
	asrs r5, r3, #16
	cmp r3, r6
	bls .L_08194de6
	movs r5, #63
.L_08194de6:
	ldr r1, .L_08194e5c
	lsls r0, r0, #16
	adds r0, r0, r1
	movs r1, #128
	ldr r3, .L_08194e60
	lsls r1, r1, #3
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #168]
	movs r2, #246
	adds r1, r0, #0
	lsls r2, r2, #15
	adds r1, r1, r2
	ldr r4, .L_08194e64
	ldr r2, [r3]
	lsls r3, r5, #24
	asrs r1, r1, #16
	adds r2, r2, r4
	lsrs r3, r3, #24
	movs r0, #0
	bl Func_08192828
.L_08194e12:
	ldr r5, [sp, #168]
	ldr r6, .L_08194e68
	ldr r0, [r5]
	adds r3, r0, r6
	cmp r3, #209
	bhi .L_08194e78
	ldr r3, .L_08194e58
	movs r1, #252
	subs r3, r3, r0
	movs r6, #128
	lsls r3, r3, #16
	lsls r1, r1, #14
	lsls r6, r6, #11
	asrs r5, r3, #16
	cmp r3, r1
	bls .L_08194e34
	movs r5, #63
.L_08194e34:
	ldr r2, .L_08194e6c
	lsls r0, r0, #16
	movs r1, #128
	adds r0, r0, r2
	ldr r3, .L_08194e60
	lsls r1, r1, #3
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #168]
	ldr r4, .L_08194e70
	ldr r2, [r3]
	subs r6, r6, r0
	lsls r3, r5, #24
	asrs r0, r6, #16
	adds r2, r2, r4
	lsrs r3, r3, #24
	movs r1, #127
	b .L_08194e74
.L_08194e58:
	.4byte 0x000002e5
.L_08194e5c:
	.4byte 0xfdf70000
.L_08194e60:
	.4byte IwramMulQ16
.L_08194e64:
	.4byte 0xfffffdf7
.L_08194e68:
	.4byte 0xfffffded
.L_08194e6c:
	.4byte 0xfdee0000
.L_08194e70:
	.4byte 0xfffffdee
.L_08194e74:
	bl Func_08192828
.L_08194e78:
	ldr r5, [sp, #168]
	ldr r6, .L_08194ec4
	ldr r0, [r5]
	adds r3, r0, r6
	cmp r3, #209
	bhi .L_08194eda
	ldr r3, .L_08194ec0
	movs r1, #252
	subs r3, r3, r0
	lsls r3, r3, #16
	lsls r1, r1, #14
	asrs r5, r3, #16
	cmp r3, r1
	bls .L_08194e96
	movs r5, #63
.L_08194e96:
	ldr r2, .L_08194ec8
	lsls r0, r0, #16
	movs r1, #192
	adds r0, r0, r2
	ldr r3, .L_08194ecc
	lsls r1, r1, #3
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #168]
	movs r3, #240
	ldr r2, [r4]
	ldr r6, .L_08194ed0
	adds r1, r0, #0
	lsls r3, r3, #15
	adds r1, r1, r3
	lsls r3, r5, #24
	asrs r1, r1, #16
	adds r2, r2, r6
	lsrs r3, r3, #24
	b .L_08194ed4
	.2byte 0x0000
.L_08194ec0:
	.4byte 0x000002e9
.L_08194ec4:
	.4byte 0xfffffde9
.L_08194ec8:
	.4byte 0xfdea0000
.L_08194ecc:
	.4byte IwramMulQ16
.L_08194ed0:
	.4byte 0xfffffdea
.L_08194ed4:
	movs r0, #0
	bl Func_08192828
.L_08194eda:
	ldr r1, [sp, #168]
	ldr r2, .L_08194f24
	ldr r0, [r1]
	adds r3, r0, r2
	cmp r3, #209
	bhi .L_08194f38
	ldr r3, .L_08194f20
	movs r4, #252
	subs r3, r3, r0
	movs r6, #192
	lsls r3, r3, #16
	lsls r4, r4, #14
	lsls r6, r6, #11
	asrs r5, r3, #16
	cmp r3, r4
	bls .L_08194efc
	movs r5, #63
.L_08194efc:
	ldr r1, .L_08194f28
	lsls r0, r0, #16
	adds r0, r0, r1
	movs r1, #192
	ldr r3, .L_08194f2c
	lsls r1, r1, #3
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #168]
	ldr r4, .L_08194f30
	ldr r2, [r3]
	subs r6, r6, r0
	lsls r3, r5, #24
	asrs r0, r6, #16
	adds r2, r2, r4
	lsrs r3, r3, #24
	movs r1, #127
	b .L_08194f34
.L_08194f20:
	.4byte 0x000002fc
.L_08194f24:
	.4byte 0xfffffdd6
.L_08194f28:
	.4byte 0xfdd70000
.L_08194f2c:
	.4byte IwramMulQ16
.L_08194f30:
	.4byte 0xfffffdd7
.L_08194f34:
	bl Func_08192828
.L_08194f38:
	ldr r5, [sp, #168]
	ldr r6, .L_08195178
	ldr r3, [r5]
	adds r3, r3, r6
	cmp r3, #110
	bls .L_08194f46
	b .L_08195048
.L_08194f46:
	movs r5, #240
	lsls r5, r5, #7
	adds r0, r5, #0
	bl Trig_Sin
	str r0, [sp, #104]
	adds r0, r5, #0
	bl Trig_Cos
	str r0, [sp, #100]
	movs r0, #0
	mov r11, r0
.L_08194f5e:
	ldr r1, [sp, #168]
	movs r3, #128
	lsls r3, r3, #2
	ldr r2, [r1]
	adds r3, #126
	add r3, r11
	cmp r3, r2
	bge .L_0819503e
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #158
	add r3, r11
	cmp r2, r3
	bge .L_0819503e
	ldr r4, .L_0819517c
	mov r3, r11
	subs r1, r2, r3
	adds r0, r1, r4
	asrs r5, r0, #1
	str r5, [sp, #96]
	ldr r3, .L_08195180
	adds r2, r0, #0
	ldrh r4, [r3]
	cmp r2, #0
	bge .L_08194f94
	ldr r6, .L_08195184
	adds r2, r1, r6
.L_08194f94:
	asrs r2, r2, #2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r4, r4, r3
	mov r3, r11
	asrs r0, r0, #2
	ldr r2, .L_08195188
	asrs r3, r3, #1
	str r4, [sp, #92]
	str r0, [sp, #40]
	str r3, [sp, #88]
	movs r1, #0
	mov r10, r1
	mov r9, r2
.L_08194fb6:
	mov r4, r10
	lsls r5, r4, #9
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, [sp, #40]
	adds r6, r1, #0
	muls r6, r0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, [sp, #96]
	mov r8, r6
	ldr r1, [sp, #100]
	adds r6, r2, #0
	muls r6, r0
	mov r0, r8
	mov lr, r9
	.2byte 0xf800
	lsls r6, r6, #1
	ldr r1, [sp, #104]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r9
	.2byte 0xf800
	subs r7, r5, r0
	ldr r1, [sp, #104]
	mov r0, r8
	mov lr, r9
	.2byte 0xf800
	ldr r1, [sp, #100]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r9
	.2byte 0xf800
	ldr r4, [sp, #88]
	ldr r2, [sp, #64]
	asrs r3, r7, #16
	adds r3, r3, r4
	ldr r1, [r2]
	adds r7, r3, #0
	ldr r3, [sp, #92]
	ldr r6, [sp, #176]
	adds r1, r1, r3
	movs r3, #1
	adds r5, r5, r0
	ldr r0, [r6]
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	ldr r6, [sp, #172]
	asrs r5, r5, #16
	subs r5, r5, r4
	movs r4, #224
	lsls r4, r4, #3
	adds r7, #64
	adds r5, #63
	adds r1, r1, r4
	adds r2, r7, #0
	ldr r4, [r6]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #128
	bne .L_08194fb6
.L_0819503e:
	movs r2, #8
	add r11, r2
	mov r3, r11
	cmp r3, #80
	bne .L_08194f5e
.L_08195048:
	ldr r4, [sp, #168]
	ldr r5, .L_0819518c
	ldr r2, [r4]
	adds r3, r2, r5
	cmp r3, #2
	bls .L_0819505e
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #13
	cmp r2, r6
	bne .L_0819506e
.L_0819505e:
	ldr r1, [sp, #176]
	ldr r2, .L_08195190
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_08195194
	mov lr, r3
	.2byte 0xf800
.L_0819506e:
	ldr r4, [sp, #168]
	movs r5, #128
	ldr r3, [r4]
	lsls r5, r5, #2
	adds r5, #242
	cmp r3, r5
	beq .L_0819507e
	b .L_081951b0
.L_0819507e:
	ldr r6, [sp, #64]
	movs r1, #190
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r5, #128
	lsls r5, r5, #9
	adds r1, #16
	adds r3, r5, #0
	adds r0, r0, r1
	movs r2, #32
	movs r1, #16
	bl Func_08191c20
	ldr r0, [r6]
	movs r2, #140
	lsls r2, r2, #6
	adds r2, #16
	adds r0, r0, r2
	movs r1, #32
	adds r2, r5, #0
	bl Func_0815b434
	ldr r4, [sp, #64]
	movs r3, #0
	mov r10, r3
	movs r5, #0
.L_081950b2:
	ldr r1, [r4]
	mov r6, r10
	movs r2, #140
	lsls r0, r6, #4
	lsls r2, r2, #6
	adds r0, r1, r0
	adds r2, #16
	movs r6, #248
	adds r0, r0, r2
	movs r3, #128
	lsls r6, r6, #5
	movs r2, #132
	adds r1, r1, r5
	adds r6, #16
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r1, r6
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r4]
	movs r2, #249
	adds r3, r5, r3
	lsls r2, r2, #5
	movs r0, #0
	adds r6, #15
	adds r1, r3, r2
	mov r11, r0
	adds r2, r3, r6
.L_081950ee:
	ldrb r3, [r2]
	movs r0, #1
	add r11, r0
	strb r3, [r1]
	mov r3, r11
	subs r2, #1
	adds r1, #1
	cmp r3, #16
	bne .L_081950ee
	add r10, r0
	mov r6, r10
	adds r5, #32
	cmp r6, #32
	bne .L_081950b2
	movs r6, #136
	movs r5, #140
	ldr r4, [sp, #64]
	movs r0, #0
	lsls r6, r6, #6
	lsls r5, r5, #6
	mov r10, r0
	adds r6, #240
	adds r5, #16
.L_0819511c:
	mov r1, r10
	lsls r3, r1, #5
	ldr r1, [r4]
	movs r2, #132
	subs r0, r1, r3
	adds r1, r1, r3
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r0, r6
	adds r1, r1, r5
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #32
	bne .L_0819511c
	movs r5, #248
	movs r0, #248
	ldr r6, [sp, #64]
	lsls r5, r5, #5
	movs r7, #128
	movs r4, #224
	lsls r0, r0, #24
	mov r10, r2
	adds r5, #16
	lsls r7, r7, #4
	lsls r4, r4, #22
	mov r12, r0
.L_0819515c:
	ldr r2, [r6]
	mov r1, r10
	lsls r3, r1, #11
	adds r0, r2, r5
	adds r2, r2, r3
	movs r3, #0
	adds r2, r2, r5
	mov r11, r3
	lsrs r1, r4, #24
.L_0819516e:
	ldrb r3, [r0]
	cmp r1, r3
	bcs .L_08195198
	strb r1, [r2]
	b .L_0819519a
.L_08195178:
	.4byte 0xfffffd81
.L_0819517c:
	.4byte 0xfffffd82
.L_08195180:
	.4byte Data_08197410
.L_08195184:
	.4byte 0xfffffd85
.L_08195188:
	.4byte IwramMulQ16
.L_0819518c:
	.4byte 0xfffffd0d
.L_08195190:
	.4byte 0x3f3f3f3f
.L_08195194:
	.4byte IwramFillWords
.L_08195198:
	strb r3, [r2]
.L_0819519a:
	movs r3, #1
	add r11, r3
	adds r2, #1
	adds r0, #1
	cmp r11, r7
	bne .L_0819516e
	add r10, r3
	mov r0, r10
	add r4, r12
	cmp r0, #8
	bne .L_0819515c
.L_081951b0:
	ldr r1, [sp, #168]
	ldr r2, .L_08195460
	ldr r3, [r1]
	adds r3, r3, r2
	cmp r3, #30
	bhi .L_08195282
	movs r5, #176
	lsls r5, r5, #7
	adds r0, r5, #0
	bl Trig_Sin
	str r0, [sp, #84]
	adds r0, r5, #0
	bl Trig_Cos
	str r0, [sp, #80]
	ldr r3, [sp, #168]
	ldr r4, .L_08195464
	ldr r1, [r3]
	ldr r3, .L_08195468
	adds r7, r1, r4
	ldrh r0, [r3, #10]
	adds r2, r7, #0
	cmp r7, #0
	bge .L_081951e6
	ldr r5, .L_0819546c
	adds r2, r1, r5
.L_081951e6:
	asrs r2, r2, #2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [sp, #76]
	ldr r1, .L_08195470
	lsls r0, r7, #1
	str r0, [sp, #32]
	movs r6, #0
	mov r10, r6
	mov r9, r1
	mov r11, r6
.L_08195204:
	mov r0, r11
	bl Trig_Sin
	ldr r2, [sp, #32]
	adds r5, r2, #0
	muls r5, r0
	mov r0, r11
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r1, [sp, #80]
	adds r0, r5, #0
	mov r8, r3
	mov lr, r9
	.2byte 0xf800
	ldr r1, [sp, #84]
	adds r6, r0, #0
	mov r0, r8
	mov lr, r9
	.2byte 0xf800
	ldr r1, [sp, #84]
	subs r6, r6, r0
	adds r0, r5, #0
	mov lr, r9
	.2byte 0xf800
	ldr r1, [sp, #80]
	adds r5, r0, #0
	mov r0, r8
	mov lr, r9
	.2byte 0xf800
	ldr r2, [sp, #64]
	ldr r3, [sp, #76]
	ldr r1, [r2]
	ldr r4, [sp, #176]
	adds r1, r1, r3
	movs r3, #6
	adds r5, r5, r0
	ldr r0, [r4]
	str r3, [sp, #0]
	movs r3, #12
	str r3, [sp, #4]
	ldr r2, [sp, #172]
	asrs r6, r6, #16
	asrs r5, r5, #16
	movs r4, #224
	lsls r4, r4, #3
	adds r5, #26
	adds r6, #101
	adds r3, r5, #0
	adds r1, r1, r4
	ldr r4, [r2]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r4, #1
	movs r3, #192
	add r10, r4
	lsls r3, r3, #1
	mov r5, r10
	add r11, r3
	cmp r5, #96
	bne .L_08195204
.L_08195282:
	ldr r6, [sp, #168]
	ldr r0, .L_08195474
	ldr r3, [r6]
	adds r3, r3, r0
	cmp r3, #46
	bhi .L_08195352
	movs r0, #232
	lsls r0, r0, #8
	bl Trig_Sin
	str r0, [sp, #72]
	movs r0, #224
	lsls r0, r0, #8
	bl Trig_Cos
	ldr r5, [r6]
	ldr r1, .L_08195478
	ldr r3, .L_08195468
	adds r5, r5, r1
	mov r11, r0
	movs r1, #6
	adds r0, r5, #0
	ldrh r6, [r3, #10]
	bl Math_Div
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #7
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r6, r6, r3
	asrs r3, r5, #1
	lsls r5, r5, #1
	str r6, [sp, #68]
	str r3, [sp, #56]
	str r5, [sp, #60]
	ldr r7, .L_08195470
	movs r2, #0
	mov r10, r2
	mov r9, r2
.L_081952d2:
	mov r0, r9
	bl Trig_Sin
	ldr r4, [sp, #56]
	adds r5, r4, #0
	muls r5, r0
	mov r0, r9
	bl Trig_Cos
	ldr r1, [sp, #60]
	adds r6, r1, #0
	muls r6, r0
	adds r0, r5, #0
	mov r1, r11
	mov lr, r7
	.2byte 0xf800
	mov r8, r6
	ldr r1, [sp, #72]
	adds r6, r0, #0
	mov r0, r8
	mov lr, r7
	.2byte 0xf800
	ldr r1, [sp, #72]
	subs r6, r6, r0
	adds r0, r5, #0
	mov lr, r7
	.2byte 0xf800
	mov r1, r11
	adds r5, r0, #0
	mov r0, r8
	mov lr, r7
	.2byte 0xf800
	ldr r3, [sp, #64]
	ldr r2, [sp, #176]
	ldr r1, [r3]
	movs r3, #6
	ldr r4, [sp, #68]
	adds r5, r5, r0
	ldr r0, [r2]
	str r3, [sp, #0]
	movs r3, #12
	str r3, [sp, #4]
	ldr r3, [sp, #172]
	asrs r6, r6, #16
	asrs r5, r5, #16
	movs r2, #224
	adds r1, r1, r4
	lsls r2, r2, #3
	adds r6, #93
	adds r5, #32
	ldr r4, [r3]
	adds r1, r1, r2
	adds r3, r5, #0
	adds r2, r6, #0
	movs r5, #1
	mov lr, r4
	.2byte 0xf800
	add r10, r5
	movs r4, #192
	lsls r4, r4, #1
	mov r6, r10
	add r9, r4
	cmp r6, #197
	bne .L_081952d2
.L_08195352:
	ldr r0, [sp, #168]
	movs r1, #128
	ldr r3, [r0]
	lsls r1, r1, #2
	adds r1, #246
	cmp r3, r1
	bgt .L_08195362
	b .L_081955e6
.L_08195362:
	movs r0, #64
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0819547c
	ldr r3, [sp, #208]
	movs r4, #190
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08195480
	lsls r4, r4, #7
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, [sp, #64]
	str r3, [sp, #208]
	ldr r3, [r2]
	adds r4, #16
	adds r3, r3, r4
	add r7, sp, #208
	str r3, [r7, #4]
	adds r5, r0, #0
	bl Func_08014ee0
	movs r3, #128
	add r0, sp, #252
	lsls r3, r3, #14
	str r3, [r0]
	ldr r3, .L_08195484
	movs r1, #0
	str r3, [r0, #4]
	str r1, [r0, #8]
	mov r8, r1
	bl SceneTransform_ApplyPosition
	movs r0, #152
	lsls r0, r0, #8
	bl Func_080150e4
	movs r2, #128
	add r0, sp, #240
	lsls r2, r2, #9
	str r2, [r0]
	ldr r4, [sp, #168]
	ldr r1, .L_08195464
	ldr r3, [r4]
	adds r3, r3, r1
	lsls r3, r3, #10
	str r3, [r0, #4]
	cmp r3, r2
	ble .L_081953d4
	str r2, [r0, #4]
.L_081953d4:
	mov r2, r8
	str r2, [r0, #8]
	bl Func_080151ac
	movs r3, #4
	str r3, [r5]
	ldr r3, .L_08195488
	adds r1, r6, #0
	str r3, [r5, #8]
	movs r2, #8
	str r7, [r5, #16]
	str r6, [r5, #12]
	ldr r0, .L_0819548c
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r6, #0
	bl Sys_Free
	add r3, sp, #420
	mov r9, r3
	bl Func_0819284c
	movs r4, #1
	negs r4, r4
	adds r5, r0, #0
	mov r10, r4
	cmp r5, r10
	beq .L_081954ac
	bl Random16
	ldr r6, [sp, #164]
	ldr r3, .L_0819545c
	ldr r1, [r6]
	ands r0, r3
	lsls r2, r5, #1
	lsls r0, r0, #15
	lsls r5, r5, #2
	strh r0, [r1, r2]
	movs r3, #239
	adds r2, r5, #0
	adds r2, #176
	lsls r3, r3, #16
	str r3, [r1, r2]
	bl Random16
	movs r3, #160
	lsls r3, r3, #1
	ldr r1, [r6]
	adds r2, r5, r3
	movs r3, #255
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r1, r2]
	bl Random16
	ldr r2, [r6]
	movs r6, #31
	movs r4, #232
	ands r0, r6
	lsls r4, r4, #1
	negs r0, r0
	b .L_08195490
.L_0819545c:
	.4byte 0x0000000f
.L_08195460:
	.4byte 0xfffffd09
.L_08195464:
	.4byte 0xfffffd0a
.L_08195468:
	.4byte Data_08197410
.L_0819546c:
	.4byte 0xfffffd0d
.L_08195470:
	.4byte IwramMulQ16
.L_08195474:
	.4byte 0xfffffcf1
.L_08195478:
	.4byte 0xfffffcf2
.L_0819547c:
	.4byte 0xffffff00
.L_08195480:
	.4byte 0xffff00ff
.L_08195484:
	.4byte 0xffe80000
.L_08195488:
	.4byte Data_0819a160
.L_0819548c:
	.4byte Data_0819a140
.L_08195490:
	adds r3, r5, r4
	lsls r0, r0, #12
	str r0, [r2, r3]
	bl Random16
	ldr r1, [sp, #164]
	movs r4, #152
	ldr r2, [r1]
	ands r0, r6
	lsls r4, r4, #2
	subs r0, #16
	adds r3, r5, r4
	lsls r0, r0, #12
	str r0, [r2, r3]
.L_081954ac:
	ldr r5, [sp, #168]
	movs r6, #3
	ldr r3, [r5]
	mov r8, r6
	ands r3, r6
	cmp r3, #0
	beq .L_081954bc
	b .L_081955e6
.L_081954bc:
	add r0, sp, #420
	mov r9, r0
	bl Func_081929fc
	adds r6, r0, #0
	cmp r6, r10
	beq .L_08195556
	bl Random16
	ldr r3, [sp, #64]
	mov r1, r8
	ldr r2, [r3]
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r7, r3, #2
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r2, r7]
	adds r5, r0, #0
	ands r5, r1
	bl Random16
	ldr r4, [sp, #64]
	movs r3, #7
	ldr r1, [r4]
	ands r3, r0
	movs r6, #144
	lsls r6, r6, #14
	lsls r3, r3, #16
	adds r3, r3, r6
	adds r2, r7, #4
	str r3, [r1, r2]
	ldr r0, [sp, #168]
	adds r2, #4
	ldr r3, [r0]
	movs r0, #152
	str r3, [r1, r2]
	lsls r0, r0, #8
	bl Trig_Sin
	adds r5, #2
	lsls r5, r5, #15
	adds r1, r0, #0
	ldr r6, .L_0819560c
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [sp, #64]
	adds r3, r7, #0
	ldr r2, [r1]
	adds r3, #12
	str r0, [r2, r3]
	movs r0, #152
	lsls r0, r0, #8
	bl Trig_Cos
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	ldr r3, [sp, #64]
	negs r0, r0
	ldr r2, [r3]
	adds r3, r7, #0
	adds r3, #16
	str r0, [r2, r3]
	bl Random16
	ldr r4, [sp, #64]
	mov r5, r8
	ldr r1, [r4]
	adds r2, r7, #0
	ands r0, r5
	movs r3, #16
	adds r2, #24
	orrs r3, r0
	str r3, [r1, r2]
.L_08195556:
	add r6, sp, #420
	mov r9, r6
	bl Func_081929fc
	adds r6, r0, #0
	cmp r6, r10
	beq .L_081955e6
	bl Random16
	lsls r3, r6, #3
	ldr r1, [sp, #64]
	subs r3, r3, r6
	lsls r6, r3, #2
	movs r3, #31
	ldr r2, [r1]
	ands r0, r3
	movs r4, #192
	lsls r4, r4, #15
	lsls r0, r0, #16
	adds r0, r0, r4
	str r0, [r2, r6]
	bl Random16
	ldr r5, [sp, #64]
	movs r2, #31
	ldr r1, [r5]
	ands r0, r2
	adds r3, r6, #4
	lsls r0, r0, #16
	str r0, [r1, r3]
	ldr r4, [sp, #168]
	adds r2, r6, #0
	ldr r3, [r4]
	adds r2, #8
	str r3, [r1, r2]
	bl Random16
	ldr r2, [r5]
	movs r5, #15
	ands r0, r5
	adds r3, r6, #0
	lsls r0, r0, #13
	adds r3, #12
	negs r0, r0
	str r0, [r2, r3]
	bl Random16
	ldr r2, [sp, #64]
	adds r3, r6, #0
	ldr r1, [r2]
	ands r0, r5
	adds r3, #16
	lsls r0, r0, #14
	str r0, [r1, r3]
	adds r2, r6, #0
	movs r3, #128
	adds r2, #20
	lsls r3, r3, #8
	str r3, [r1, r2]
	bl Random16
	ldr r3, [sp, #64]
	mov r4, r8
	ldr r1, [r3]
	ands r0, r4
	movs r3, #136
	adds r2, r6, #0
	adds r0, #1
	lsls r3, r3, #1
	adds r2, #24
	orrs r0, r3
	str r0, [r1, r2]
.L_081955e6:
	ldr r5, [sp, #168]
	movs r6, #192
	ldr r1, [r5]
	lsls r6, r6, #2
	adds r6, #14
	cmp r1, r6
	ble .L_08195622
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_08195614
	ldr r2, .L_08195610
	movs r3, #1
	ands r1, r3
	ldrh r3, [r2, #6]
	adds r3, r3, r1
	adds r3, #2
	b .L_08195620
	.2byte 0x0000
.L_0819560c:
	.4byte IwramMulQ16
.L_08195610:
	.4byte Data_03001120
.L_08195614:
	ldr r2, .L_08195664
	movs r3, #1
	ands r1, r3
	ldrh r3, [r2, #6]
	subs r3, r3, r1
	subs r3, #2
.L_08195620:
	strh r3, [r2, #6]
.L_08195622:
	ldr r0, [sp, #168]
	movs r1, #204
	ldr r3, [r0]
	lsls r1, r1, #2
	cmp r3, r1
	ble .L_0819567e
	movs r5, #160
	ldr r6, .L_08195660
	lsls r5, r5, #19
	movs r2, #0
	adds r5, #192
	mov r10, r2
.L_0819563a:
	ldrh r2, [r5]
	movs r0, #31
	lsls r3, r2, #16
	lsrs r4, r3, #26
	lsrs r1, r3, #21
	ands r0, r2
	ands r4, r6
	ands r1, r6
	cmp r0, #30
	bgt .L_08195650
	adds r0, #1
.L_08195650:
	cmp r1, #30
	bgt .L_08195656
	adds r1, #1
.L_08195656:
	cmp r4, #30
	bgt .L_08195668
	adds r4, #1
	b .L_08195668
	.2byte 0x0000
.L_08195660:
	.4byte 0x0000001f
.L_08195664:
	.4byte Data_03001120
.L_08195668:
	lsls r3, r4, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r5]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #2
	cmp r4, #128
	bne .L_0819563a
.L_0819567e:
	ldr r5, [sp, #168]
	movs r0, #240
	ldr r3, [r5]
	lsls r0, r0, #7
	adds r3, #1
	str r3, [r5]
	ldr r6, [sp, #64]
	adds r0, #232
	ldr r3, [r6]
	movs r2, #1
	adds r3, r3, r0
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	bl .L_08192ec8
.L_081956a0:
	add r1, sp, #188
	ldrh r1, [r1]
	ldr r2, [sp, #192]
	movs r0, #104
	strh r1, [r2, #54]
	bl Runtime_ReleaseHeapBlock
	bl Func_08191cc4
	ldr r2, [sp, #176]
	movs r1, #128
	ldr r0, [r2]
	ldr r3, .L_08195710
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #19
	movs r3, #128
	adds r2, #212
	lsls r3, r3, #24
.L_081956cc:
	ldr r5, [r2, #8]
	ands r5, r3
	cmp r5, #0
	bne .L_081956cc
	ldr r4, [sp, #64]
	movs r6, #240
	ldr r3, [r4]
	lsls r6, r6, #7
	adds r6, #232
	adds r3, r3, r6
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0819570c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #40
	add r0, sp, #196
	str r5, [r3]
	ldrh r0, [r0]
	ldr r3, .L_08195714
	movs r1, #0
	strh r0, [r3, #4]
	mov r10, r1
	b .L_08195718
	.2byte 0x0000
.L_0819570c:
	.4byte 0x00000080
.L_08195710:
	.4byte IwramFillWords
.L_08195714:
	.4byte Data_03001120
.L_08195718:
	ldr r4, [sp, #180]
	mov r2, r10
	lsls r3, r2, #2
	ldr r2, [r4]
	movs r5, #1
	add r10, r5
	ldr r0, [r3, r2]
	mov r6, r10
	bl ResourceObject_ReleaseFar
	cmp r6, #36
	bne .L_08195718
	ldr r0, .L_08195748
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #420
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08195748:
	.4byte Func_08143000
