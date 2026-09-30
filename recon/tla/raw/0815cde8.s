.syntax unified
	.thumb
	.global Func_0815cde8
	.thumb_func
Func_0815cde8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #120
	str r0, [sp, #84]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	str r0, [sp, #80]
	ldr r1, [r5, #92]
	str r1, [sp, #76]
	ldr r6, [r5, #100]
	bl Func_0813ba50
	movs r0, #3
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0815ce50
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r2, [r5, #104]
	adds r1, r6, #0
	str r2, [sp, #68]
	ldr r0, .L_0815ce54
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r6, #240
	ldr r4, [sp, #76]
	lsls r6, r6, #7
	adds r6, #240
	adds r3, r4, r6
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r7, [sp, #76]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	b .L_0815ce58
.L_0815ce50:
	.4byte 0x00003f44
.L_0815ce54:
	.4byte 0x00000134
.L_0815ce58:
	adds r2, r7, r0
	movs r3, #1
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r1, #192
	movs r3, #0
	lsls r1, r1, #4
	str r3, [r2]
	adds r1, #254
	ldr r0, .L_0815cffc
	bl Scheduler_AddOrUpdateCallback
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0815d000
	bl Scheduler_AddOrUpdateCallback
	movs r6, #152
	ldr r5, [r5, #36]
	movs r2, #224
	movs r3, #143
	movs r4, #240
	lsls r2, r2, #3
	lsls r3, r3, #4
	lsls r4, r4, #4
	lsls r6, r6, #5
	adds r6, r7, r6
	adds r2, r7, r2
	adds r3, r7, r3
	adds r4, r7, r4
	ldr r1, .L_0815d004
	str r6, [sp, #20]
	str r5, [sp, #60]
	str r2, [sp, #32]
	str r3, [sp, #28]
	str r4, [sp, #24]
	ldr r6, .L_0815d008
	movs r7, #0
	movs r0, #7
	mov r8, r7
	mov r9, r0
	mov r10, r1
.L_0815ceae:
	mov r5, r8
	mov r2, r9
	movs r7, #0
	ands r5, r2
.L_0815ceb6:
	mov r1, r8
	cmp r1, #0
	bge .L_0815cebe
	adds r1, #7
.L_0815cebe:
	asrs r1, r1, #3
	lsls r1, r1, #8
	ldr r3, .L_0815d00c
	adds r1, r1, r5
	lsls r1, r1, #3
	adds r1, r1, r7
	adds r0, r6, #0
	adds r1, r1, r3
	movs r2, #8
	mov lr, r10
	.2byte 0xf800
	movs r4, #224
	lsls r4, r4, #3
	adds r7, #64
	adds r4, #255
	adds r6, #8
	cmp r7, r4
	ble .L_0815ceb6
	movs r7, #1
	add r8, r7
	mov r0, r8
	cmp r0, #119
	ble .L_0815ceae
	ldr r1, [sp, #76]
	movs r2, #200
	lsls r2, r2, #6
	adds r2, #184
	adds r5, r1, r2
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	ldr r0, .L_0815d010
	bl Func_08157cf4
	movs r6, #208
	ldr r4, [sp, #76]
	lsls r6, r6, #6
	movs r3, #0
	movs r1, #144
	adds r6, #248
	mov r8, r3
	lsls r1, r1, #1
	adds r2, r4, r6
.L_0815cf14:
	ldrb r3, [r5]
	movs r7, #1
	add r8, r7
	strb r3, [r2]
	adds r5, #2
	adds r2, #1
	cmp r8, r1
	bne .L_0815cf14
	movs r0, #1
	bl WaitFrames
	movs r1, #163
	lsls r1, r1, #2
	movs r0, #12
	movs r2, #1
	bl Func_08152404
	movs r2, #238
	ldr r1, [sp, #76]
	lsls r2, r2, #7
	movs r3, #13
	ldr r7, .L_0815d004
	movs r0, #0
	adds r2, #220
	negs r3, r3
	mov r8, r0
	adds r5, r1, r2
	adds r6, r3, #0
.L_0815cf4c:
	movs r1, #32
	ldr r2, .L_0815d014
	movs r3, #0
	movs r0, #32
	bl Func_0815b3b0
	mov r4, r8
	str r0, [r5, #48]
	movs r3, #3
	movs r1, #238
	ands r3, r4
	ldr r2, [sp, #76]
	lsls r1, r1, #7
	adds r1, #240
	lsls r3, r3, #2
	adds r3, r3, r1
	ldr r1, [r2, r3]
	movs r2, #24
	mov lr, r7
	.2byte 0xf800
	ldmia r5!, {r1}
	movs r2, #4
	ldrb r3, [r1, #9]
	ands r3, r6
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #16
	bne .L_0815cf4c
	movs r6, #0
	str r6, [sp, #64]
.L_0815cf8e:
	ldr r3, .L_0815d018
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0815d024
	ldr r7, [sp, #64]
	cmp r7, #15
	ble .L_0815d024
	cmp r7, #185
	bgt .L_0815d024
	movs r1, #128
	ldr r3, .L_0815d01c
	ldr r0, [sp, #80]
	lsls r1, r1, #7
	ldr r2, .L_0815d020
	mov lr, r3
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	str r2, [sp, #64]
	ldr r3, .L_0815cff8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #76]
	movs r6, #238
	lsls r6, r6, #7
	movs r3, #0
	movs r1, #128
	adds r6, #220
	mov r8, r3
	lsls r1, r1, #7
	adds r2, r4, r6
.L_0815cfe6:
	ldmia r2!, {r3}
	movs r7, #1
	add r8, r7
	mov r0, r8
	strh r1, [r3, #18]
	cmp r0, #28
	bne .L_0815cfe6
	b .L_0815d024
	.2byte 0x0000
.L_0815cff8:
	.4byte 0x00001010
.L_0815cffc:
	.4byte Func_08164bb4
.L_0815d000:
	.4byte Func_08143000
.L_0815d004:
	.4byte IwramCopyWords
.L_0815d008:
	.4byte gMapCellBuffer
.L_0815d00c:
	.4byte 0x06008000
.L_0815d010:
	.4byte 0x00000190
.L_0815d014:
	.4byte 0x80002000
.L_0815d018:
	.4byte gInput
.L_0815d01c:
	.4byte IwramFillWords
.L_0815d020:
	.4byte 0x3f3f3f3f
.L_0815d024:
	ldr r1, [sp, #64]
	cmp r1, #8
	bgt .L_0815d058
	lsls r3, r1, #1
	adds r3, r3, r1
	movs r2, #24
	subs r2, r2, r3
	lsls r3, r2, #4
	adds r3, r3, r2
	lsls r3, r3, #4
	adds r3, r3, r2
	movs r2, #128
	lsls r3, r3, #2
	lsls r2, r2, #9
	subs r2, r2, r3
	movs r4, #160
	ldr r3, [sp, #60]
	lsls r4, r4, #3
	movs r1, #160
	adds r4, #108
	lsls r1, r1, #19
	adds r0, r3, r4
	adds r1, #192
	movs r3, #128
	bl ColorBuffer_ScaleFar
.L_0815d058:
	ldr r6, [sp, #64]
	cmp r6, #186
	bne .L_0815d074
	movs r0, #132
	ldr r2, .L_0815d0b4
	movs r7, #192
	lsls r0, r0, #15
	lsls r7, r7, #14
	adds r0, #3
	movs r1, #0
	str r7, [sp, #52]
	str r0, [sp, #56]
	str r1, [sp, #36]
	str r2, [sp, #40]
.L_0815d074:
	ldr r3, [sp, #64]
	cmp r3, #0
	beq .L_0815d07c
	b .L_0815d228
.L_0815d07c:
	ldr r3, .L_0815d0b0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #76]
	movs r6, #239
	lsls r6, r6, #7
	adds r2, r4, r6
	movs r3, #1
	str r3, [r2]
	movs r7, #238
	lsls r7, r7, #7
	ldr r0, [sp, #64]
	adds r7, #132
	adds r3, r4, r7
	str r0, [r3]
	movs r3, #238
	movs r1, #0
	lsls r3, r3, #7
	mov r8, r1
	adds r3, #220
	movs r1, #192
	lsls r1, r1, #8
	adds r2, r4, r3
	b .L_0815d0b8
.L_0815d0b0:
	.4byte 0x00000010
.L_0815d0b4:
	.4byte 0xfffa0000
.L_0815d0b8:
	ldmia r2!, {r3}
	movs r4, #1
	add r8, r4
	mov r6, r8
	strh r1, [r3, #18]
	cmp r6, #28
	bne .L_0815d0b8
	movs r1, #0
	ldr r0, .L_0815d24c
	str r1, [sp, #36]
	ldr r1, .L_0815d250
	movs r7, #240
	movs r2, #192
	lsls r7, r7, #15
	lsls r2, r2, #12
	str r7, [sp, #52]
	str r0, [sp, #56]
	str r2, [sp, #40]
	ldr r4, .L_0815d254
	ldrh r3, [r4]
	adds r0, r3, #0
	movs r6, #130
	ldr r7, .L_0815d254
	lsls r6, r6, #2
	strh r6, [r7]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0815d112
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
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
.L_0815d112:
	ldr r1, .L_0815d254
	strh r0, [r1]
	movs r7, #0
	ldr r4, [sp, #28]
	movs r2, #0
	ldr r3, .L_0815d258
	str r2, [sp, #16]
	ldr r5, [sp, #28]
	mov lr, r2
	adds r4, #12
	mov r11, r2
	mov r9, r3
	mov r10, r2
.L_0815d12c:
	mov r1, r10
	ldr r2, .L_0815d258
	adds r3, r1, r7
	movs r6, #0
	mov r0, r9
	lsls r3, r3, #1
	adds r3, r3, r2
	mov r8, r6
	ldrb r6, [r0, #1]
	mov r0, r10
	adds r1, r3, #2
	lsls r3, r0, #2
	ldr r0, [sp, #32]
	adds r2, r3, r0
	mov r3, r9
	ldrb r0, [r3]
.L_0815d14c:
	ldrb r3, [r1]
	subs r3, r3, r0
	strb r3, [r2]
	ldrb r3, [r1, #1]
	adds r1, #2
	subs r3, r3, r6
	strb r3, [r2, #1]
	mov r3, lr
	strb r3, [r2, #2]
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r2, #4
	cmp r3, #4
	bne .L_0815d14c
	movs r3, #1
	strb r3, [r5, #1]
	mov r6, lr
	movs r3, #2
	strb r3, [r5, #2]
	strb r6, [r5]
	strb r3, [r4, #1]
	movs r3, #3
	strb r3, [r4, #2]
	strb r6, [r4]
	strb r6, [r4, #12]
	strb r6, [r4, #13]
	strb r6, [r4, #14]
	mov r2, r11
	ldr r6, [sp, #28]
	movs r0, #0
	ldr r1, .L_0815d258
	adds r3, r2, r7
	mov r8, r0
	lsls r3, r3, #2
	ldr r0, [sp, #16]
	adds r2, r3, r6
	mov r12, r1
	adds r1, r2, #4
.L_0815d19a:
	ldrb r3, [r2]
	mov r6, r12
	adds r3, r0, r3
	lsls r3, r3, #1
	adds r3, #2
	ldrb r3, [r6, r3]
	strb r3, [r1]
	ldrb r3, [r2]
	adds r3, r0, r3
	lsls r3, r3, #1
	adds r3, #3
	ldrb r3, [r6, r3]
	strb r3, [r1, #1]
	ldrb r3, [r2, #12]
	adds r3, r0, r3
	lsls r3, r3, #1
	adds r3, #2
	ldrb r3, [r6, r3]
	strb r3, [r1, #12]
	ldrb r3, [r2, #12]
	adds r2, #1
	adds r3, r0, r3
	lsls r3, r3, #1
	adds r3, #3
	ldrb r3, [r6, r3]
	strb r3, [r1, #13]
	movs r3, #1
	add r8, r3
	mov r6, r8
	adds r1, #2
	cmp r6, #3
	bne .L_0815d19a
	ldr r0, [sp, #16]
	movs r1, #8
	adds r0, #5
	movs r2, #10
	movs r3, #4
	adds r7, #1
	adds r4, #36
	str r0, [sp, #16]
	add r11, r1
	adds r5, #36
	add r9, r2
	add r10, r3
	cmp r7, #31
	bne .L_0815d12c
	ldr r6, [sp, #84]
	movs r4, #0
	ldr r3, [r6, #20]
	mov r8, r4
	cmp r3, #0
	beq .L_0815d21e
	movs r6, #0
	movs r5, #36
.L_0815d206:
	ldr r7, [sp, #84]
	ldrsh r0, [r5, r7]
	bl GetBattleObjectSlotFar
	ldr r3, [r0]
	movs r2, #1
	str r6, [r3, #12]
	ldr r3, [r7, #20]
	add r8, r2
	adds r5, #2
	cmp r8, r3
	bne .L_0815d206
.L_0815d21e:
	movs r2, #128
	ldr r3, .L_0815d244
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
.L_0815d228:
	ldr r3, [sp, #64]
	subs r3, #10
	mov r11, r3
	cmp r3, #16
	bhi .L_0815d25c
	ldr r1, .L_0815d248
	movs r3, #128
	mov r4, r11
	lsls r3, r3, #19
	lsls r2, r4, #8
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	b .L_0815d25c
.L_0815d244:
	.4byte 0x00000787
.L_0815d248:
	.4byte 0x00000010
.L_0815d24c:
	.4byte 0xff880000
.L_0815d250:
	.4byte Data_020038e0
.L_0815d254:
	.4byte 0x04000208
.L_0815d258:
	.4byte Data_081986f2
.L_0815d25c:
	ldr r3, [sp, #64]
	subs r3, #16
	cmp r3, #63
	bhi .L_0815d334
	ldr r6, [sp, #64]
	cmp r6, #16
	bne .L_0815d2c8
	ldr r0, [sp, #76]
	movs r1, #217
	movs r7, #0
	lsls r1, r1, #2
	mov r8, r7
	movs r6, #31
	adds r5, r0, r1
.L_0815d278:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #120
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #63
	mov r2, r8
	lsls r0, r0, #12
	str r0, [r5, #16]
	lsls r0, r2, #9
	bl Trig_Cos
	movs r3, #1
	lsls r0, r0, #5
	asrs r0, r0, #16
	add r8, r3
	subs r0, #32
	mov r4, r8
	str r0, [r5, #24]
	adds r5, #28
	cmp r4, #27
	bne .L_0815d278
.L_0815d2c8:
	ldr r7, [sp, #76]
	ldr r1, [sp, #64]
	movs r0, #217
	movs r6, #0
	lsls r0, r0, #2
	mov r8, r6
	adds r5, r7, r0
	lsls r6, r1, #10
.L_0815d2d8:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0815d324
	movs r3, #208
	lsls r3, r3, #5
	adds r3, #22
	mov r0, r8
	muls r0, r3
	adds r0, r6, r0
	bl Trig_Sin
	ldr r2, [r5]
	movs r4, #6
	ldrsh r3, [r5, r4]
	ldr r7, [sp, #76]
	lsls r0, r0, #4
	movs r4, #208
	adds r2, r2, r0
	movs r1, #12
	lsls r4, r4, #6
	str r1, [sp, #0]
	adds r4, #248
	movs r1, #24
	asrs r2, r2, #16
	subs r3, #12
	str r1, [sp, #4]
	subs r2, #6
	adds r1, r7, r4
	ldr r0, [sp, #80]
	ldr r7, [sp, #68]
	mov lr, r7
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0815d378
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_0815d324:
	movs r0, #1
	add r8, r0
	adds r3, #1
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #27
	bne .L_0815d2d8
.L_0815d334:
	ldr r2, [sp, #64]
	cmp r2, #95
	ble .L_0815d342
	adds r3, r2, #0
	subs r3, #160
	cmp r3, #31
	bhi .L_0815d424
.L_0815d342:
	ldr r3, [sp, #76]
	movs r6, #236
	lsls r6, r6, #7
	adds r6, #64
	adds r4, r3, r6
	ldr r3, [sp, #64]
	cmp r3, #0
	bge .L_0815d354
	adds r3, #7
.L_0815d354:
	ldr r7, [sp, #64]
	asrs r3, r3, #3
	mov r10, r3
	cmp r7, #159
	ble .L_0815d37c
	adds r3, r7, #0
	subs r3, #160
	cmp r3, #0
	bge .L_0815d368
	adds r3, #3
.L_0815d368:
	asrs r3, r3, #2
	adds r3, #12
	mov r10, r3
	cmp r3, #20
	ble .L_0815d386
	movs r0, #20
	mov r10, r0
	b .L_0815d386
.L_0815d378:
	.4byte 0xffffc000
.L_0815d37c:
	mov r1, r10
	cmp r1, #12
	ble .L_0815d386
	movs r2, #12
	mov r10, r2
.L_0815d386:
	movs r3, #0
	mov r8, r3
.L_0815d38a:
	movs r6, #1
	add r8, r6
	mov r7, r8
	strh r3, [r4]
	adds r4, #2
	cmp r7, #15
	bne .L_0815d38a
.L_0815d398:
	mov r6, r8
	subs r6, #15
	movs r1, #15
	adds r0, r6, #0
	str r4, [sp, #8]
	bl Math_Div
	mov r1, r10
	negs r0, r0
	subs r0, r0, r1
	adds r7, r0, #0
	adds r7, #40
	adds r3, r6, #0
	ldr r4, [sp, #8]
	cmp r6, #0
	bge .L_0815d3ba
	mov r3, r8
.L_0815d3ba:
	asrs r3, r3, #4
	mov r2, r10
	negs r3, r3
	subs r3, r3, r2
	adds r0, r6, #0
	movs r1, #17
	adds r5, r3, #0
	str r4, [sp, #8]
	bl Math_Div
	mov r3, r10
	negs r0, r0
	subs r0, r0, r3
	adds r5, #32
	adds r0, #16
	ldr r4, [sp, #8]
	cmp r7, #0
	bge .L_0815d3e0
	movs r7, #0
.L_0815d3e0:
	cmp r7, #31
	ble .L_0815d3e6
	movs r7, #31
.L_0815d3e6:
	cmp r5, #0
	bge .L_0815d3ec
	movs r5, #0
.L_0815d3ec:
	cmp r5, #31
	ble .L_0815d3f2
	movs r5, #31
.L_0815d3f2:
	cmp r0, #0
	bge .L_0815d3f8
	movs r0, #0
.L_0815d3f8:
	cmp r0, #31
	ble .L_0815d3fe
	movs r0, #31
.L_0815d3fe:
	lsls r3, r7, #10
	lsls r2, r5, #5
	movs r6, #1
	orrs r3, r2
	add r8, r6
	orrs r3, r0
	mov r7, r8
	strh r3, [r4]
	adds r4, #2
	cmp r7, #135
	bne .L_0815d398
	ldr r3, .L_0815d448
.L_0815d416:
	movs r0, #1
	add r8, r0
	mov r1, r8
	strh r3, [r4]
	adds r4, #2
	cmp r1, #160
	bne .L_0815d416
.L_0815d424:
	ldr r2, [sp, #64]
	cmp r2, #0
	blt .L_0815d43a
	ldr r4, [sp, #52]
	ldr r7, [sp, #56]
	ldr r3, [sp, #36]
	ldr r6, [sp, #40]
	adds r3, r3, r4
	adds r6, r6, r7
	str r3, [sp, #52]
	str r6, [sp, #56]
.L_0815d43a:
	ldr r0, [sp, #64]
	cmp r0, #168
	bne .L_0815d4bc
	ldr r4, .L_0815d44c
	movs r3, #0
	b .L_0815d450
	.2byte 0x0000
.L_0815d448:
	.4byte 0x00000000
.L_0815d44c:
	.4byte 0xfffa0000
.L_0815d450:
	movs r1, #192
	ldr r6, [sp, #76]
	movs r7, #238
	lsls r1, r1, #14
	movs r2, #168
	lsls r7, r7, #7
	str r3, [sp, #36]
	str r4, [sp, #40]
	lsls r2, r2, #16
	str r1, [sp, #52]
	adds r7, #220
	movs r1, #128
	str r2, [sp, #56]
	mov r8, r3
	lsls r1, r1, #7
	adds r2, r6, r7
.L_0815d470:
	ldmia r2!, {r3}
	movs r0, #1
	add r8, r0
	strh r1, [r3, #18]
	mov r3, r8
	cmp r3, #28
	bne .L_0815d470
	ldr r6, [sp, #84]
	movs r4, #0
	ldr r3, [r6, #20]
	mov r8, r4
	cmp r3, #0
	beq .L_0815d4bc
	movs r6, #36
.L_0815d48c:
	ldr r7, [sp, #84]
	ldrsh r0, [r6, r7]
	bl GetBattleObjectSlotFar
	movs r3, #128
	lsls r3, r3, #11
	ldr r5, [r0]
	ldrsh r0, [r6, r7]
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r3, #0
	movs r2, #0
	bl Func_0815f000
	movs r3, #0
	str r3, [r5, #72]
	movs r3, #1
	add r8, r3
	ldr r3, [r7, #20]
	adds r6, #2
	cmp r8, r3
	bne .L_0815d48c
.L_0815d4bc:
	ldr r4, [sp, #64]
	cmp r4, #186
	bne .L_0815d4c6
	ldr r6, .L_0815d618
	str r6, [sp, #40]
.L_0815d4c6:
	ldr r3, [sp, #64]
	subs r3, #54
	cmp r3, #113
	bhi .L_0815d4d0
	b .L_0815d756
.L_0815d4d0:
	ldr r7, [sp, #64]
	cmp r7, #213
	ble .L_0815d4d8
	b .L_0815d750
.L_0815d4d8:
	add r5, sp, #104
	movs r3, #0
	str r3, [r5, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r5, #4]
	cmp r7, #167
	ble .L_0815d4f0
	ldr r0, [sp, #48]
	ldr r1, .L_0815d61c
	adds r1, r0, r1
	str r1, [sp, #48]
.L_0815d4f0:
	ldr r2, [sp, #64]
	cmp r2, #53
	ble .L_0815d4f8
	b .L_0815d638
.L_0815d4f8:
	ldr r4, [sp, #76]
	movs r7, #238
	lsls r7, r7, #7
	movs r3, #0
	adds r7, #220
	mov r8, r3
	adds r6, r4, r7
.L_0815d506:
	ldr r3, .L_0815d620
	mov r0, r8
	ldrb r2, [r3, r0]
	ldr r1, [sp, #52]
	movs r3, #40
	subs r3, r3, r2
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r5]
	ldr r3, .L_0815d624
	ldrb r3, [r3, r0]
	lsls r2, r3, #5
	subs r2, r2, r3
	cmp r2, #0
	bge .L_0815d526
	adds r2, #31
.L_0815d526:
	asrs r2, r2, #5
	movs r3, #128
	ldr r4, [sp, #56]
	subs r3, r3, r2
	lsls r3, r3, #16
	adds r2, r3, r4
	mov r7, r8
	str r2, [r5, #8]
	cmp r7, #8
	ble .L_0815d540
	ldr r0, .L_0815d628
	adds r3, r2, r0
	str r3, [r5, #8]
.L_0815d540:
	ldr r3, [r5, #8]
	ldr r2, .L_0815d62c
	cmp r3, r2
	bge .L_0815d54c
	str r2, [r5, #8]
	adds r3, r2, #0
.L_0815d54c:
	ldr r1, .L_0815d630
	cmp r3, r1
	ble .L_0815d558
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
.L_0815d558:
	ldr r2, .L_0815d634
	movs r3, #0
	ldmia r6!, {r0}
	adds r1, r5, #0
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #12
	bne .L_0815d506
	ldr r7, [sp, #76]
	movs r0, #240
	lsls r0, r0, #7
	movs r4, #0
	adds r0, #12
	mov r8, r4
	adds r6, r7, r0
.L_0815d57c:
	ldr r3, .L_0815d620
	mov r1, r8
	movs r4, #3
	ands r4, r1
	adds r1, r4, #5
	ldrb r2, [r3, r1]
	movs r3, #39
	subs r3, r3, r2
	ldr r2, [sp, #52]
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, .L_0815d624
	ldrb r2, [r3, r1]
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_0815d5a4
	adds r3, #31
.L_0815d5a4:
	asrs r3, r3, #5
	mov r2, r8
	negs r1, r3
	cmp r2, #0
	bge .L_0815d5b0
	adds r2, #3
.L_0815d5b0:
	asrs r2, r2, #2
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	ldr r7, [sp, #56]
	subs r3, r1, r3
	lsls r3, r3, #16
	movs r0, #132
	ldr r2, .L_0815d62c
	adds r3, r3, r7
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r5, #8]
	cmp r3, r2
	bge .L_0815d5d0
	str r2, [r5, #8]
.L_0815d5d0:
	ldr r3, [r5, #8]
	ldr r1, .L_0815d630
	cmp r3, r1
	ble .L_0815d5de
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
.L_0815d5de:
	movs r2, #238
	lsls r3, r4, #2
	lsls r2, r2, #7
	ldr r4, [sp, #76]
	adds r2, #240
	adds r3, r3, r2
	ldr r3, [r4, r3]
	ldmia r6!, {r0}
	ldrh r2, [r3, #8]
	ldr r1, .L_0815d614
	ldrh r3, [r0, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	movs r7, #1
	strh r3, [r0, #8]
	adds r1, r5, #0
	ldr r2, .L_0815d634
	movs r3, #0
	add r8, r7
	bl Render_ApplyProjectedPlacementFar
	mov r0, r8
	cmp r0, #16
	bne .L_0815d57c
	b .L_0815d850
.L_0815d614:
	.4byte 0xfffffc00
.L_0815d618:
	.4byte 0xfff00000
.L_0815d61c:
	.4byte 0xfffa0000
.L_0815d620:
	.4byte Data_08198830
.L_0815d624:
	.4byte Data_0819883c
.L_0815d628:
	.4byte 0xff100000
.L_0815d62c:
	.4byte 0xffff0000
.L_0815d630:
	.4byte 0x009fffff
.L_0815d634:
	.4byte Data_08198828
.L_0815d638:
	ldr r2, [sp, #76]
	movs r3, #238
	lsls r3, r3, #7
	movs r1, #0
	adds r3, #220
	mov r8, r1
	adds r7, r5, #0
	adds r4, r2, r3
.L_0815d648:
	ldr r6, .L_0815d73c
	mov r0, r8
	ldrb r3, [r6, r0]
	ldr r1, [sp, #52]
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r7]
	ldr r3, .L_0815d740
	ldr r2, [sp, #56]
	ldrb r3, [r3, r0]
	mov r10, r6
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r7, #8]
	cmp r0, #8
	ble .L_0815d670
	movs r6, #128
	lsls r6, r6, #17
	adds r3, r3, r6
	str r3, [r5, #8]
.L_0815d670:
	ldr r3, [r5, #8]
	ldr r6, .L_0815d744
	cmp r3, r6
	bge .L_0815d67c
	str r6, [r5, #8]
	adds r3, r6, #0
.L_0815d67c:
	ldr r0, .L_0815d748
	cmp r3, r0
	ble .L_0815d688
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
.L_0815d688:
	ldmia r4!, {r0}
	adds r1, r5, #0
	ldr r2, .L_0815d74c
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r1, #1
	add r8, r1
	mov r2, r8
	ldr r4, [sp, #8]
	cmp r2, #12
	bne .L_0815d648
	ldr r0, [sp, #76]
	movs r1, #240
	lsls r1, r1, #7
	movs r3, #0
	adds r1, #12
	adds r7, r6, #0
	mov r8, r3
	mov r4, r10
	adds r6, r0, r1
.L_0815d6b4:
	mov r2, r8
	movs r1, #3
	ands r1, r2
	adds r3, r1, #5
	ldrb r2, [r4, r3]
	ldr r0, [sp, #52]
	lsls r2, r2, #16
	adds r2, r2, r0
	adds r2, r2, r7
	str r2, [r5]
	ldr r2, .L_0815d740
	ldrb r2, [r2, r3]
	mov r3, r8
	cmp r3, #0
	bge .L_0815d6d4
	adds r3, #3
.L_0815d6d4:
	asrs r3, r3, #2
	lsls r3, r3, #6
	adds r3, r2, r3
	ldr r2, [sp, #56]
	lsls r3, r3, #16
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r5, #8]
	cmp r3, r7
	bge .L_0815d6ee
	str r7, [r5, #8]
.L_0815d6ee:
	ldr r3, [r5, #8]
	ldr r2, .L_0815d748
	cmp r3, r2
	ble .L_0815d6fc
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
.L_0815d6fc:
	lsls r3, r1, #2
	movs r1, #238
	ldr r2, [sp, #76]
	lsls r1, r1, #7
	adds r1, #240
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldmia r6!, {r0}
	ldrh r2, [r3, #8]
	ldr r1, .L_0815d738
	ldrh r3, [r0, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
	adds r1, r5, #0
	movs r3, #0
	ldr r2, .L_0815d74c
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r3, #1
	add r8, r3
	mov r0, r8
	ldr r4, [sp, #8]
	cmp r0, #16
	bne .L_0815d6b4
	b .L_0815d850
	.2byte 0x0000
.L_0815d738:
	.4byte 0xfffffc00
.L_0815d73c:
	.4byte Data_08198830
.L_0815d740:
	.4byte Data_0819883c
.L_0815d744:
	.4byte 0xffff0000
.L_0815d748:
	.4byte 0x009fffff
.L_0815d74c:
	.4byte Data_08198828
.L_0815d750:
	cmp r3, #113
	bls .L_0815d756
	b .L_0815d850
.L_0815d756:
	ldr r1, [sp, #64]
	cmp r1, #54
	bne .L_0815d78e
	ldr r3, [sp, #76]
	movs r4, #238
	lsls r4, r4, #7
	movs r2, #0
	adds r4, #220
	mov r8, r2
	movs r1, #0
	adds r2, r3, r4
.L_0815d76c:
	ldmia r2!, {r3}
	movs r6, #1
	add r8, r6
	mov r7, r8
	strh r1, [r3, #18]
	cmp r7, #28
	bne .L_0815d76c
	ldr r2, .L_0815d984
	movs r0, #128
	movs r1, #144
	lsls r0, r0, #17
	lsls r1, r1, #15
	movs r3, #0
	str r0, [sp, #52]
	str r1, [sp, #56]
	str r2, [sp, #36]
	str r3, [sp, #40]
.L_0815d78e:
	ldr r4, [sp, #64]
	cmp r4, #138
	bne .L_0815d79a
	movs r6, #128
	lsls r6, r6, #12
	str r6, [sp, #36]
.L_0815d79a:
	add r6, sp, #104
	movs r3, #0
	ldr r7, [sp, #36]
	str r3, [r6, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r6, #4]
	adds r0, r7, #0
	lsls r3, r7, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_0815d7b6
	adds r3, #63
.L_0815d7b6:
	ldr r1, [sp, #40]
	asrs r3, r3, #6
	str r3, [sp, #36]
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_0815d7c8
	adds r3, #63
.L_0815d7c8:
	ldr r2, [sp, #64]
	asrs r3, r3, #6
	str r3, [sp, #40]
	cmp r2, #137
	bgt .L_0815d7f6
	lsls r5, r2, #9
	adds r0, r5, #0
	bl Trig_Sin
	ldr r3, [sp, #52]
	lsls r0, r0, #4
	adds r0, r0, r3
	str r0, [sp, #44]
	mov r10, r0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r4, [sp, #56]
	lsls r0, r0, #3
	adds r0, r0, r4
	str r0, [sp, #48]
	adds r7, r0, #0
	b .L_0815d814
.L_0815d7f6:
	ldr r7, [sp, #64]
	lsls r5, r7, #9
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, [sp, #52]
	lsls r0, r0, #4
	adds r1, r1, r0
	adds r0, r5, #0
	mov r10, r1
	bl Trig_Cos
	ldr r2, [sp, #56]
	lsls r0, r0, #3
	adds r7, r0, r2
.L_0815d814:
	ldr r4, [sp, #76]
	movs r0, #238
	lsls r0, r0, #7
	movs r3, #0
	adds r0, #220
	adds r5, r6, #0
	mov r8, r3
	adds r6, r4, r0
.L_0815d824:
	ldr r3, .L_0815d988
	mov r1, r8
	ldrsb r3, [r3, r1]
	ldr r2, .L_0815d98c
	lsls r3, r3, #16
	add r3, r10
	str r3, [r5]
	ldr r3, .L_0815d990
	ldmia r6!, {r0}
	ldrsb r3, [r3, r1]
	adds r1, r5, #0
	lsls r3, r3, #16
	adds r3, r3, r7
	str r3, [r5, #8]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #8
	bne .L_0815d824
.L_0815d850:
	ldr r3, [sp, #64]
	subs r3, #138
	cmp r3, #29
	bhi .L_0815d86c
	ldr r4, [sp, #64]
	ldr r3, .L_0815d994
	cmp r4, #151
	ble .L_0815d866
	ldr r6, .L_0815d998
	lsls r3, r4, #13
	adds r3, r3, r6
.L_0815d866:
	ldr r7, [sp, #44]
	adds r3, r7, r3
	str r3, [sp, #44]
.L_0815d86c:
	ldr r0, [sp, #64]
	cmp r0, #78
	bne .L_0815d878
	movs r0, #140
	bl Audio_PlayCue
.L_0815d878:
	ldr r1, [sp, #64]
	cmp r1, #138
	bne .L_0815d884
	movs r0, #104
	bl Audio_PlayCue
.L_0815d884:
	ldr r2, [sp, #64]
	cmp r2, #152
	bne .L_0815d890
	movs r0, #163
	bl Audio_PlayCue
.L_0815d890:
	ldr r3, [sp, #64]
	cmp r3, #7
	bne .L_0815d89c
	movs r0, #212
	bl Audio_PlayCue
.L_0815d89c:
	ldr r4, [sp, #64]
	cmp r4, #10
	bne .L_0815d942
	movs r0, #144
	bl Audio_PlayCue
	movs r6, #0
	mov r8, r6
	ldr r5, [sp, #76]
	ldr r6, .L_0815d99c
.L_0815d8b0:
	ldrb r2, [r6]
	ldrb r1, [r6, #1]
	lsrs r2, r2, #1
	lsls r3, r2, #16
	str r3, [r5]
	subs r2, #60
	lsls r3, r1, #16
	subs r1, #60
	lsls r2, r2, #12
	lsls r1, r1, #10
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	str r2, [r5, #12]
	str r1, [r5, #16]
	bl Random16
	movs r3, #127
	ands r3, r0
	movs r7, #128
	lsls r3, r3, #2
	lsls r7, r7, #2
	adds r3, r3, r7
	str r3, [r5, #20]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_0815d8f2
	ldr r3, [r5, #20]
	negs r3, r3
	str r3, [r5, #20]
.L_0815d8f2:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #10
	adds r5, #28
	cmp r1, #31
	bne .L_0815d8b0
	ldr r1, .L_0815d9a0
	ldr r2, .L_0815d9a4
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	ldr r4, .L_0815d9a4
	lsls r3, r3, #2
	strh r3, [r4]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0815d938
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
.L_0815d938:
	ldr r6, .L_0815d9a4
	strh r0, [r6]
	ldr r7, [sp, #64]
	cmp r7, #10
	beq .L_0815d948
.L_0815d942:
	ldr r0, [sp, #64]
	cmp r0, #186
	bne .L_0815da26
.L_0815d948:
	ldr r2, [sp, #84]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r8, r1
	cmp r3, #0
	beq .L_0815da0a
	adds r2, #36
	mov r10, r2
.L_0815d958:
	mov r4, r10
	movs r3, #0
	ldrsh r0, [r4, r3]
	bl GetBattleObjectSlotFar
	adds r5, r0, #0
	ldr r6, [r5]
	ldr r7, [sp, #64]
	mov r9, r6
	cmp r7, #186
	bne .L_0815d978
	mov r2, r10
	movs r1, #0
	ldrsh r0, [r2, r1]
	bl ReleaseBattleObjectRecordsFar
.L_0815d978:
	ldr r3, [sp, #64]
	cmp r3, #10
	bne .L_0815d9fa
	movs r7, #0
	b .L_0815d9b2
	.2byte 0x0000
.L_0815d984:
	.4byte 0xfffa0000
.L_0815d988:
	.4byte Data_08198850
.L_0815d98c:
	.4byte Data_08198848
.L_0815d990:
	.4byte Data_08198858
.L_0815d994:
	.4byte 0xfffe0000
.L_0815d998:
	.4byte 0xffeb0000
.L_0815d99c:
	.4byte Data_081986f2
.L_0815d9a0:
	.4byte Data_020038e0
.L_0815d9a4:
	.4byte 0x04000208
.L_0815d9a8:
	ldr r1, [r5, #36]
	adds r0, r6, #0
	bl Func_08020060
	adds r7, #1
.L_0815d9b2:
	ldr r0, [r5]
	adds r1, r7, #0
	bl GetMotionRecordFar
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0815d9a8
	mov r7, r10
	movs r3, #8
	movs r4, #0
	ldrsh r0, [r7, r4]
	movs r1, #9
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl Func_0814cd48
	bl Random16
	movs r3, #0
	ldrsh r2, [r7, r3]
	movs r1, #128
	lsls r1, r1, #9
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r3, #100
	str r0, [sp, #0]
	str r3, [sp, #4]
	adds r0, r2, #0
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl Func_0815f000
	mov r4, r9
	str r6, [r4, #72]
.L_0815d9fa:
	ldr r0, [sp, #84]
	movs r7, #1
	ldr r3, [r0, #20]
	movs r6, #2
	add r8, r7
	add r10, r6
	cmp r8, r3
	bne .L_0815d958
.L_0815da0a:
	ldr r1, [sp, #64]
	cmp r1, #186
	bne .L_0815da26
	ldr r4, [sp, #84]
	ldr r2, .L_0815da3c
	lsls r3, r3, #1
	adds r3, #36
	adds r0, r4, #0
	strh r2, [r4, r3]
	adds r0, #36
	movs r1, #0
	movs r2, #0
	bl Func_08118148
.L_0815da26:
	ldr r6, [sp, #64]
	cmp r6, #9
	ble .L_0815da6a
	ldr r0, [sp, #84]
	movs r7, #0
	ldr r3, [r0, #20]
	mov r8, r7
	cmp r3, #0
	beq .L_0815da6a
	movs r5, #36
	b .L_0815da40
.L_0815da3c:
	.4byte 0x000000ff
.L_0815da40:
	ldr r1, [sp, #84]
	ldrsh r0, [r5, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r2, [r0, #40]
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_0815da58
	adds r3, #63
.L_0815da58:
	asrs r3, r3, #6
	str r3, [r0, #40]
	ldr r4, [sp, #84]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_0815da40
.L_0815da6a:
	mov r6, r11
	cmp r6, #43
	bls .L_0815da72
	b .L_0815db98
.L_0815da72:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0815db24
	ldr r3, [sp, #96]
	movs r1, #8
	ands r3, r2
	ldr r2, .L_0815db28
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	ldr r7, .L_0815db2c
	lsls r2, r2, #3
	orrs r3, r2
	adds r6, r0, #0
	str r3, [sp, #96]
	add r3, sp, #96
	str r7, [r3, #4]
	str r3, [r6, #16]
	ldr r3, .L_0815db30
	mov r0, r10
	ldrh r3, [r3, #4]
	str r1, [r6]
	str r0, [r6, #12]
	strb r3, [r6, #24]
	ldr r7, [sp, #28]
	ldr r5, [sp, #76]
	movs r1, #0
	mov r8, r1
.L_0815dab4:
	str r7, [r6, #8]
	bl Func_08014de4
	ldr r3, .L_0815db34
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	movs r3, #3
	mov r2, r8
	ands r3, r2
	cmp r3, #1
	beq .L_0815db04
	cmp r3, #1
	bgt .L_0815daec
	cmp r3, #0
	beq .L_0815daf6
	b .L_0815db46
.L_0815daec:
	cmp r3, #2
	beq .L_0815db14
	cmp r3, #3
	beq .L_0815db38
	b .L_0815db46
.L_0815daf6:
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	bl Func_080150e4
	b .L_0815db46
.L_0815db04:
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	negs r0, r0
	bl Func_080150e4
	b .L_0815db46
.L_0815db14:
	ldr r0, [r5, #8]
	bl SceneTransform_ApplyPitch
	ldr r0, [r5, #8]
	bl Func_08015068
	b .L_0815db46
	.2byte 0x0000
.L_0815db24:
	.4byte 0xffffff00
.L_0815db28:
	.4byte 0xffff00ff
.L_0815db2c:
	.4byte gMapCellBuffer
.L_0815db30:
	.4byte Data_03001120
.L_0815db34:
	.4byte 0xffc00000
.L_0815db38:
	ldr r0, [r5, #8]
	bl Func_080150e4
	ldr r0, [r5, #8]
	negs r0, r0
	bl Func_08015068
.L_0815db46:
	ldr r4, [sp, #32]
	mov r3, r8
	lsls r0, r3, #4
	adds r0, r4, r0
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r5, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	movs r3, #8
	mov r0, r8
	subs r3, r3, r0
	ldr r1, [sp, #64]
	lsls r3, r3, #1
	adds r3, #16
	cmp r1, r3
	blt .L_0815db78
	ldr r3, [r5, #16]
	ldr r2, .L_0815dd14
	adds r3, r3, r2
	str r3, [r5, #16]
.L_0815db78:
	adds r0, r6, #0
	bl Func_08196a7c
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #36
	adds r5, #28
	cmp r4, #31
	bne .L_0815dab4
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
.L_0815db98:
	ldr r6, [sp, #64]
	cmp r6, #78
	beq .L_0815dba0
	b .L_0815dcb6
.L_0815dba0:
	movs r3, #8
	ldr r0, [sp, #20]
	movs r1, #16
	movs r2, #16
	str r3, [sp, #0]
	movs r7, #0
	bl Func_0818de3c
	movs r0, #0
	mov r10, r7
	mov r9, r0
	mov r11, r0
.L_0815dbb8:
	mov r0, r9
	bl Trig_Cos
	negs r0, r0
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r3, r3, #16
	mov r0, r9
	mov r8, r3
	bl Trig_Sin
	lsls r3, r0, #1
	ldr r6, [sp, #24]
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r2, r3, #16
	movs r7, #0
	add r6, r11
.L_0815dbde:
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
	bne .L_0815dbde
	movs r2, #128
	movs r4, #1
	lsls r2, r2, #4
	add r10, r4
	adds r2, #136
	movs r3, #64
	mov r6, r10
	add r9, r2
	add r11, r3
	cmp r6, #16
	bne .L_0815dbb8
	ldr r0, .L_0815dd18
	ldr r1, .L_0815dd1c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r7, #0
	mov r8, r7
	movs r5, #0
.L_0815dc36:
	ldr r2, [sp, #76]
	movs r0, #0
	mov r1, r8
	lsls r3, r5, #4
	mov r10, r0
	lsls r4, r1, #11
	adds r0, r3, r2
.L_0815dc44:
	movs r3, #232
	ldr r6, .L_0815dd1c
	lsls r3, r3, #6
	adds r3, #8
	movs r7, #0
	adds r1, r0, r3
	adds r2, r4, r6
.L_0815dc52:
	ldrb r3, [r2]
	adds r7, #1
	strb r3, [r1]
	adds r2, #1
	subs r1, #16
	cmp r7, #64
	bne .L_0815dc52
	movs r7, #1
	add r10, r7
	mov r1, r10
	adds r4, #128
	adds r0, #1
	cmp r1, #16
	bne .L_0815dc44
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #64
	cmp r3, #6
	bne .L_0815dc36
	ldr r5, .L_0815dd1c
	ldr r0, .L_0815dd20
	adds r1, r5, #0
	movs r3, #0
	bl Func_08157cf4
	adds r0, r5, #0
	ldr r1, .L_0815dd24
	movs r2, #64
	movs r3, #64
	bl Func_0816ae40
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0815dd28
	adds r1, r5, #0
	bl Func_08157cf4
	ldr r4, [sp, #76]
	movs r6, #239
	movs r7, #238
	lsls r6, r6, #7
	lsls r7, r7, #7
	adds r2, r4, r6
	movs r3, #2
	adds r7, #132
	str r3, [r2]
	adds r2, r4, r7
	movs r3, #50
	str r3, [r2]
.L_0815dcb6:
	ldr r0, [sp, #64]
	cmp r0, #152
	bne .L_0815dcc8
	ldr r0, .L_0815dd2c
	ldr r1, .L_0815dd1c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_0815dcc8:
	ldr r1, [sp, #64]
	subs r1, #80
	mov r10, r1
	cmp r1, #107
	bls .L_0815dcd4
	b .L_0815df02
.L_0815dcd4:
	movs r3, #128
	ldr r2, .L_0815dd10
	lsls r3, r3, #19
	adds r3, #12
	strh r2, [r3]
	ldr r3, [sp, #64]
	subs r3, #168
	cmp r3, #17
	bhi .L_0815dd86
	movs r0, #188
	movs r1, #7
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r4, [sp, #56]
	movs r6, #16
	str r3, [sp, #72]
	mov r11, r6
	asrs r3, r4, #16
	ldr r6, [sp, #64]
	movs r2, #0
	adds r7, r3, #0
	movs r0, #64
	mov r8, r2
	subs r7, #32
	mov r9, r0
	b .L_0815dd30
.L_0815dd10:
	.4byte 0x00000784
.L_0815dd14:
	.4byte 0xffff8000
.L_0815dd18:
	.4byte 0x0000016a
.L_0815dd1c:
	.4byte Data_02014000
.L_0815dd20:
	.4byte 0x000000ec
.L_0815dd24:
	.4byte gMapCellBuffer
.L_0815dd28:
	.4byte 0x000000c2
.L_0815dd2c:
	.4byte 0x000000b4
.L_0815dd30:
	adds r0, r6, #0
	movs r1, #6
	bl __modsi3
	ldr r1, [sp, #76]
	adds r5, r0, #0
	movs r2, #216
	lsls r5, r5, #10
	lsls r2, r2, #6
	adds r5, r1, r5
	adds r2, #24
	adds r5, r5, r2
	mov r3, r11
	mov r4, r9
	str r3, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #80]
	ldr r4, [sp, #68]
	adds r1, r5, #0
	movs r2, #30
	adds r3, r7, #0
	mov lr, r4
	.2byte 0xf800
	mov r0, r11
	mov r1, r9
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #80]
	adds r1, r5, #0
	movs r2, #46
	adds r3, r7, #0
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #3
	cmp r1, #2
	bne .L_0815dd30
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_0815dd86:
	ldr r2, [sp, #64]
	cmp r2, #185
	ble .L_0815dd8e
	b .L_0815df08
.L_0815dd8e:
	movs r0, #1
	bl Func_081969f8
	mov r4, r10
	lsls r3, r4, #1
	add r3, r10
	movs r7, #128
	lsls r5, r3, #8
	lsls r7, r7, #9
	adds r6, r0, #0
	movs r1, #0
	cmp r5, r7
	ble .L_0815ddac
	movs r5, #128
	lsls r5, r5, #9
.L_0815ddac:
	ldr r3, [sp, #88]
	ldr r2, .L_0815e124
	movs r0, #7
	ands r3, r2
	ldr r2, .L_0815e128
	orrs r3, r0
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, .L_0815e12c
	add r7, sp, #88
	str r3, [sp, #88]
	str r0, [r6]
	str r2, [r7, #4]
	str r7, [r6, #16]
	ldr r3, [sp, #20]
	mov r8, r0
	str r3, [r6, #8]
	ldr r4, [sp, #32]
	str r1, [r6, #20]
	str r4, [r6, #12]
	strb r1, [r6, #24]
	strb r1, [r6, #25]
	bl Func_08014de4
	ldr r1, [sp, #44]
	ldr r3, [sp, #48]
	ldr r4, .L_0815e130
	lsrs r0, r1, #31
	ldr r2, .L_0815e134
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r1, r3, r4
	adds r0, r0, r2
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r0, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	movs r1, #0
	movs r2, #0
	ldr r0, .L_0815e138
	bl Func_08015160
	ldr r0, [sp, #64]
	lsls r5, r0, #10
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
	adds r0, r5, #0
	bl Func_08015068
	movs r2, #128
	ldr r0, [sp, #24]
	ldr r1, [sp, #32]
	lsls r2, r2, #1
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	ldr r1, [sp, #64]
	cmp r1, #137
	ble .L_0815de98
	lsls r3, r1, #2
	movs r2, #160
	adds r3, r3, r1
	lsls r2, r2, #3
	lsls r3, r3, #1
	adds r2, #164
	subs r1, r2, r3
	ldr r2, [sp, #64]
	ldr r4, .L_0815e13c
	lsls r3, r2, #13
	adds r5, r3, r4
	cmp r1, #0
	ble .L_0815de50
	movs r1, #0
.L_0815de50:
	movs r0, #64
	negs r0, r0
	cmp r1, r0
	ble .L_0815de98
	ldr r3, .L_0815e140
	mov r2, r8
	str r3, [r7, #4]
	ldr r3, .L_0815e144
	strb r2, [r7]
	str r3, [r6, #8]
	strb r2, [r7, #1]
	str r1, [r6, #20]
	bl Func_08014de4
	movs r0, #176
	movs r1, #0
	movs r2, #0
	lsls r0, r0, #14
	bl Func_08015160
	movs r0, #208
	lsls r0, r0, #6
	adds r0, #72
	bl Func_08015068
	adds r0, r5, #0
	bl Func_0801521c
	ldr r0, .L_0815e148
	ldr r1, [sp, #32]
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0815de98:
	ldr r3, [sp, #64]
	cmp r3, #151
	ble .L_0815defc
	ldr r4, [sp, #76]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r4, r0
	movs r3, #2
	str r3, [r2]
	ldr r2, [sp, #64]
	ldr r4, .L_0815e14c
	lsls r3, r2, #13
	adds r5, r3, r4
	add r2, sp, #88
	movs r3, #6
	strb r3, [r7]
	strb r3, [r2, #1]
	ldr r3, .L_0815e140
	movs r1, #0
	str r3, [r2, #4]
	ldr r3, .L_0815e150
	str r1, [r6, #20]
	str r3, [r6, #8]
	bl Func_08014de4
	ldr r0, .L_0815e130
	ldr r7, [sp, #48]
	movs r2, #0
	adds r1, r7, r0
	ldr r0, .L_0815e154
	bl Func_08015160
	asrs r2, r5, #1
	adds r0, r2, #0
	adds r1, r5, #0
	bl Func_080151e4
	ldr r1, [sp, #64]
	lsls r0, r1, #11
	bl Func_080150e4
	ldr r0, .L_0815e148
	ldr r1, [sp, #32]
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0815defc:
	adds r0, r6, #0
	bl Sys_Free
.L_0815df02:
	ldr r2, [sp, #64]
	cmp r2, #185
	ble .L_0815dfe4
.L_0815df08:
	ldr r3, [sp, #64]
	cmp r3, #186
	bne .L_0815df6e
	ldr r7, .L_0815e12c
	movs r4, #0
	mov r8, r4
.L_0815df14:
	mov r0, r8
	lsls r6, r0, #8
	bl Random16
	movs r3, #127
	adds r5, r0, #0
	adds r0, r6, #0
	ands r5, r3
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	ldr r2, [r7, #12]
	adds r1, r5, #0
	muls r1, r0
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r2, #192
	lsls r2, r2, #14
	lsls r3, r3, #1
	asrs r1, r1, #4
	adds r3, r3, r2
	str r3, [r7]
	lsls r3, r1, #2
	adds r3, r3, r1
	str r3, [r7, #4]
	str r1, [r7, #16]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r4, #128
	movs r3, #1
	add r8, r3
	lsls r4, r4, #1
	adds r7, #28
	cmp r8, r4
	bne .L_0815df14
.L_0815df6e:
	ldr r5, .L_0815e12c
	movs r6, #0
	mov r8, r6
.L_0815df74:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0815dfd2
	movs r7, #2
	ldrsh r2, [r5, r7]
	movs r0, #6
	ldrsh r3, [r5, r0]
	ldr r4, [sp, #76]
	movs r6, #208
	movs r1, #12
	lsls r6, r6, #6
	str r1, [sp, #0]
	adds r6, #248
	movs r1, #24
	subs r3, #12
	str r1, [sp, #4]
	subs r2, #6
	adds r1, r4, r6
	ldr r0, [sp, #80]
	ldr r7, [sp, #68]
	mov lr, r7
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #63
	lsls r2, r2, #9
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #4]
	movs r0, #192
	lsls r0, r0, #15
	cmp r3, r0
	ble .L_0815dfd0
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	lsls r3, r3, #2
	str r3, [r5, #12]
	ldr r3, [r5, #24]
	subs r3, #8
	str r3, [r5, #24]
	b .L_0815dfd2
.L_0815dfd0:
	ldr r3, [r5, #24]
.L_0815dfd2:
	movs r1, #1
	movs r2, #128
	subs r3, #1
	add r8, r1
	lsls r2, r2, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r8, r2
	bne .L_0815df74
.L_0815dfe4:
	ldr r3, [sp, #64]
	cmp r3, #186
	bne .L_0815e092
	ldr r4, [sp, #60]
	movs r6, #206
	lsls r6, r6, #3
	adds r3, r4, r6
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #24
	bl Func_08118040
	movs r1, #128
	ldr r3, .L_0815e158
	ldr r0, [sp, #80]
	lsls r1, r1, #7
	ldr r2, .L_0815e15c
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0815e160
	ldr r7, .L_0815e164
	ldrh r3, [r7]
	adds r0, r3, #0
	movs r2, #130
	ldr r3, .L_0815e164
	lsls r2, r2, #2
	strh r2, [r3]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0815e042
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
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
.L_0815e042:
	ldr r4, .L_0815e164
	strh r0, [r4]
	movs r6, #0
	ldr r7, [sp, #84]
	mov r8, r6
	ldr r3, [r7, #20]
	cmp r3, #0
	beq .L_0815e07e
	movs r5, #36
.L_0815e054:
	ldr r1, [sp, #84]
	movs r3, #16
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	movs r2, #5
	bl Func_0814cd48
	ldr r3, [sp, #84]
	movs r1, #4
	ldrsh r0, [r5, r3]
	bl Func_08118088
	ldr r7, [sp, #84]
	movs r6, #1
	ldr r3, [r7, #20]
	add r8, r6
	adds r5, #2
	cmp r8, r3
	bne .L_0815e054
.L_0815e07e:
	movs r0, #145
	bl Audio_PlayCue
	movs r1, #238
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #16
	str r3, [r2]
.L_0815e092:
	ldr r2, [sp, #64]
	cmp r2, #214
	bne .L_0815e09e
	movs r0, #134
	bl Func_081180e8
.L_0815e09e:
	ldr r3, [sp, #64]
	cmp r3, #185
	ble .L_0815e0ae
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	b .L_0815e0b6
.L_0815e0ae:
	movs r0, #2
	movs r1, #2
	bl Func_08158ce0
.L_0815e0b6:
	ldr r4, [sp, #64]
	cmp r4, #185
	ble .L_0815e0c0
	bl Func_081434f8
.L_0815e0c0:
	ldr r6, [sp, #76]
	movs r7, #240
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r6, r7
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #64]
	adds r0, #1
	str r0, [sp, #64]
	cmp r0, #228
	beq .L_0815e0e2
	bl .L_0815cf8e
.L_0815e0e2:
	ldr r0, .L_0815e168
	bl Scheduler_RemoveCallback
	ldr r0, .L_0815e16c
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r2, #238
	lsls r2, r2, #7
	movs r1, #0
	adds r2, #220
	mov r8, r1
	adds r5, r6, r2
.L_0815e100:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #28
	bne .L_0815e100
	bl Func_08143bb8
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0815e124:
	.4byte 0xffffff00
.L_0815e128:
	.4byte 0xffff00ff
.L_0815e12c:
	.4byte gMapCellBuffer
.L_0815e130:
	.4byte 0xffb00000
.L_0815e134:
	.4byte 0xffbc0000
.L_0815e138:
	.4byte 0xffd80000
.L_0815e13c:
	.4byte 0xffef0000
.L_0815e140:
	.4byte Data_02014000
.L_0815e144:
	.4byte Data_08199364
.L_0815e148:
	.4byte Data_081991e0
.L_0815e14c:
	.4byte 0xffed0000
.L_0815e150:
	.4byte Data_08199340
.L_0815e154:
	.4byte 0xfff00000
.L_0815e158:
	.4byte IwramFillWords
.L_0815e15c:
	.4byte 0x3f3f3f3f
.L_0815e160:
	.4byte Data_020038e0
.L_0815e164:
	.4byte 0x04000208
.L_0815e168:
	.4byte Func_08143000
.L_0815e16c:
	.4byte Func_08164bb4
