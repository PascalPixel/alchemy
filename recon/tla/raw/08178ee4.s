.syntax unified
	.thumb
	.global Func_08178ee4
	.thumb_func
Func_08178ee4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r0, [sp, #12]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	ldr r1, [r5, #96]
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #1
	mov r11, r1
	ldr r6, .L_08178f50
	bl Func_08143a88
	ldr r3, .L_08178f4c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r0, .L_08178f54
	movs r2, #1
	movs r3, #1
	adds r1, r6, #0
	bl Func_08157cf4
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #16]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	add r2, sp, #16
	str r3, [r2, #4]
	mov r10, r2
	movs r2, #239
	lsls r2, r2, #7
	add r2, r8
	b .L_08178f58
	.2byte 0x0000
.L_08178f4c:
	.4byte 0x00000100
.L_08178f50:
	.4byte gMapCellBuffer
.L_08178f54:
	.4byte 0x000000dc
.L_08178f58:
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #0
	add r2, r8
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08178fc4
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #12]
	mov r2, sp
	adds r2, #24
	movs r3, #36
	ldrsh r0, [r1, r3]
	adds r1, r2, #0
	str r2, [sp, #8]
	bl Func_0815e20c
	movs r3, #0
	mov r9, r3
.L_08178f88:
	mov r0, r9
	cmp r0, #0
	bne .L_08178fea
	ldr r1, [sp, #12]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08178fa4
	ldr r2, [sp, #8]
	mov r0, r8
	ldr r3, [r2]
	adds r3, #40
	lsls r3, r3, #16
	str r3, [r0]
	b .L_08178fb0
.L_08178fa4:
	ldr r1, [sp, #8]
	mov r2, r8
	ldr r3, [r1]
	subs r3, #168
	lsls r3, r3, #16
	str r3, [r2]
.L_08178fb0:
	ldr r3, .L_08178fc8
	mov r0, r8
	str r3, [r0, #4]
	ldr r1, [sp, #12]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08178fd0
	ldr r3, .L_08178fcc
	str r3, [r0, #12]
	b .L_08178fd8
.L_08178fc4:
	.4byte Func_08143000
.L_08178fc8:
	.4byte 0xff9c0000
.L_08178fcc:
	.4byte 0xfffa0000
.L_08178fd0:
	movs r3, #192
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #12]
.L_08178fd8:
	movs r3, #128
	lsls r3, r3, #12
	mov r0, r8
	str r3, [r0, #16]
	movs r1, #128
	ldr r3, .L_08179018
	lsls r1, r1, #19
	adds r1, #82
	strh r3, [r1]
.L_08178fea:
	mov r2, r9
	cmp r2, #31
	bgt .L_08178ffa
	mov r0, r8
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
.L_08178ffa:
	mov r3, r9
	cmp r3, #32
	bgt .L_08179028
	lsrs r3, r3, #31
	add r3, r9
	asrs r1, r3, #1
	ldr r2, .L_0817901c
	lsrs r3, r3, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r0, #128
	lsls r2, r2, #8
	b .L_08179020
	.2byte 0x0000
.L_08179018:
	.4byte 0x00001000
.L_0817901c:
	.4byte 0x00000010
.L_08179020:
	lsls r0, r0, #19
	orrs r2, r1
	adds r0, #82
	strh r2, [r0]
.L_08179028:
	mov r1, r9
	cmp r1, #91
	ble .L_08179060
	mov r2, r9
	movs r1, #108
	subs r1, r1, r2
	lsrs r2, r1, #31
	ldr r3, .L_08179058
	adds r2, r1, r2
	asrs r2, r2, #1
	subs r3, r3, r2
	movs r0, #128
	lsls r3, r3, #8
	lsls r0, r0, #19
	orrs r3, r1
	adds r0, #82
	strh r3, [r0]
	movs r1, #60
	mov r0, r8
	ldr r2, .L_0817905c
	bl BattleFxKernels_IntegrateVector2
	b .L_08179060
	.2byte 0x0000
.L_08179058:
	.4byte 0x00000010
.L_0817905c:
	.4byte 0xffff8000
.L_08179060:
	mov r1, r9
	lsls r6, r1, #8
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	movs r2, #2
	ldrsh r5, [r3, r2]
	lsls r0, r0, #3
	asrs r0, r0, #16
	adds r5, r5, r0
	adds r0, r6, #0
	bl Trig_Cos
	mov r2, r8
	movs r1, #6
	ldrsh r3, [r2, r1]
	lsls r0, r0, #3
	asrs r0, r0, #16
	adds r7, r3, r0
	movs r3, #128
	lsls r3, r3, #19
	negs r5, r5
	adds r3, #40
	lsls r5, r5, #8
	str r5, [r3]
	ldr r3, [sp, #12]
	ldr r1, [r3, #4]
	cmp r1, #0
	bne .L_081790fe
	movs r2, #96
	str r2, [sp, #0]
	movs r2, #83
	str r2, [sp, #4]
	mov r0, r10
	ldr r4, [r0]
	ldr r1, .L_0817935c
	adds r3, r7, #2
	mov r0, r11
	movs r2, #25
	mov lr, r4
	.2byte 0xf800
	mov r1, r9
	cmp r1, #47
	bhi .L_081790dc
	ldr r2, [sp, #12]
	mov r0, r10
	ldr r1, [r2, #4]
	movs r2, #48
	str r2, [sp, #0]
	movs r2, #51
	str r2, [sp, #4]
	lsls r1, r1, #2
	adds r3, r7, #0
	ldr r4, [r1, r0]
	adds r3, #22
	mov r0, r11
	ldr r1, .L_08179360
	movs r2, #31
	mov lr, r4
	.2byte 0xf800
	b .L_08179160
.L_081790dc:
	ldr r2, [sp, #12]
	mov r0, r10
	ldr r1, [r2, #4]
	movs r2, #42
	str r2, [sp, #0]
	movs r2, #35
	str r2, [sp, #4]
	lsls r1, r1, #2
	adds r3, r7, #0
	ldr r4, [r1, r0]
	adds r3, #16
	mov r0, r11
	ldr r1, .L_08179364
	movs r2, #64
	mov lr, r4
	.2byte 0xf800
	b .L_08179160
.L_081790fe:
	movs r2, #96
	str r2, [sp, #0]
	movs r2, #83
	str r2, [sp, #4]
	lsls r1, r1, #2
	mov r2, r10
	ldr r4, [r1, r2]
	adds r3, r7, #2
	mov r0, r11
	ldr r1, .L_0817935c
	movs r2, #7
	mov lr, r4
	.2byte 0xf800
	mov r3, r9
	cmp r3, #47
	bhi .L_08179140
	ldr r0, [sp, #12]
	movs r2, #48
	ldr r1, [r0, #4]
	str r2, [sp, #0]
	movs r2, #51
	str r2, [sp, #4]
	lsls r1, r1, #2
	mov r2, r10
	adds r3, r7, #0
	ldr r4, [r1, r2]
	adds r3, #22
	mov r0, r11
	ldr r1, .L_08179360
	movs r2, #49
	mov lr, r4
	.2byte 0xf800
	b .L_08179160
.L_08179140:
	ldr r3, [sp, #12]
	movs r2, #42
	ldr r1, [r3, #4]
	str r2, [sp, #0]
	movs r2, #35
	str r2, [sp, #4]
	lsls r1, r1, #2
	mov r0, r10
	adds r3, r7, #0
	ldr r4, [r1, r0]
	adds r3, #16
	mov r0, r11
	ldr r1, .L_08179364
	movs r2, #22
	mov lr, r4
	.2byte 0xf800
.L_08179160:
	mov r0, r9
	subs r0, #28
	cmp r0, #11
	bhi .L_081791d4
	movs r1, #3
	bl Math_Div
	ldr r1, [sp, #12]
	adds r6, r0, #0
	ldr r5, [r1, #4]
	cmp r5, #0
	bne .L_081791a6
	ldr r2, .L_08179368
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_0817936c
	ldr r2, .L_08179370
	ldrb r4, [r3, r6]
	adds r1, r1, r2
	lsrs r3, r4, #1
	movs r2, #46
	subs r2, r2, r3
	ldr r3, .L_08179374
	ldrb r0, [r3, r6]
	str r4, [sp, #0]
	str r0, [sp, #4]
	lsrs r3, r0, #1
	subs r3, r7, r3
	mov r0, r10
	ldr r4, [r0]
	adds r3, #61
	mov r0, r11
	mov lr, r4
	.2byte 0xf800
	b .L_081791d4
.L_081791a6:
	ldr r2, .L_08179368
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_0817936c
	ldr r2, .L_08179370
	ldrb r0, [r3, r6]
	ldr r3, .L_08179374
	adds r1, r1, r2
	ldrb r4, [r3, r6]
	str r0, [sp, #0]
	str r4, [sp, #4]
	lsrs r2, r0, #1
	lsrs r3, r4, #1
	subs r2, r2, r0
	lsls r5, r5, #2
	mov r0, r10
	subs r3, r7, r3
	ldr r4, [r5, r0]
	adds r2, #82
	adds r3, #61
	mov r0, r11
	mov lr, r4
	.2byte 0xf800
.L_081791d4:
	mov r1, r9
	cmp r1, #39
	ble .L_08179292
	cmp r1, #47
	bgt .L_08179220
	ldr r2, [sp, #12]
	ldr r1, [r2, #4]
	cmp r1, #0
	bne .L_08179202
	movs r2, #32
	str r2, [sp, #0]
	movs r2, #29
	str r2, [sp, #4]
	mov r0, r10
	adds r3, r7, #0
	ldr r4, [r0]
	adds r3, #46
	mov r0, r11
	ldr r1, .L_08179378
	movs r2, #30
	mov lr, r4
	.2byte 0xf800
	b .L_08179292
.L_08179202:
	movs r2, #32
	str r2, [sp, #0]
	movs r2, #29
	str r2, [sp, #4]
	lsls r1, r1, #2
	mov r2, r10
	adds r3, r7, #0
	ldr r4, [r1, r2]
	adds r3, #46
	mov r0, r11
	ldr r1, .L_08179378
	movs r2, #66
	mov lr, r4
	.2byte 0xf800
	b .L_08179292
.L_08179220:
	mov r0, r9
	subs r0, #56
	movs r1, #6
	bl Math_Div
	adds r4, r0, #0
	cmp r4, #0
	bge .L_08179232
	movs r4, #0
.L_08179232:
	cmp r4, #7
	bgt .L_08179292
	ldr r3, [sp, #12]
	ldr r0, [r3, #4]
	cmp r0, #0
	bne .L_08179268
	ldr r2, .L_0817937c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_08179380
	movs r2, #32
	ldrb r3, [r3, r4]
	str r2, [sp, #0]
	ldr r2, .L_08179384
	ldr r0, .L_08179378
	ldrb r2, [r2, r4]
	adds r3, r7, r3
	str r2, [sp, #4]
	mov r2, r10
	adds r1, r1, r0
	ldr r4, [r2]
	subs r3, #8
	mov r0, r11
	movs r2, #63
	mov lr, r4
	.2byte 0xf800
	b .L_08179292
.L_08179268:
	ldr r2, .L_0817937c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_08179378
	movs r2, #32
	adds r1, r1, r3
	ldr r3, .L_08179380
	lsls r0, r0, #2
	ldrb r3, [r3, r4]
	str r2, [sp, #0]
	ldr r2, .L_08179384
	adds r3, r7, r3
	ldrb r2, [r2, r4]
	subs r3, #8
	str r2, [sp, #4]
	mov r2, r10
	ldr r4, [r0, r2]
	mov r0, r11
	movs r2, #33
	mov lr, r4
	.2byte 0xf800
.L_08179292:
	mov r3, r9
	cmp r3, #80
	bne .L_081792be
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r8
	movs r3, #6
	str r3, [r2]
	movs r0, #134
	bl Func_081180e8
	ldr r2, [sp, #12]
	movs r3, #16
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_081792be:
	mov r3, r9
	cmp r3, #30
	bne .L_081792f8
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_08179388
	ldr r2, .L_0817938c
	mov r0, r11
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r8
	movs r3, #3
	str r3, [r2]
	movs r0, #212
	bl Audio_PlayCue
	ldr r2, [sp, #12]
	movs r3, #16
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_081792f8:
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r8
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #108
	beq .L_08179322
	b .L_08178f88
.L_08179322:
	ldr r0, .L_08179390
	bl Scheduler_RemoveCallback
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r1, #206
	lsls r1, r1, #3
	adds r3, r3, r1
	ldrh r1, [r3]
	movs r2, #24
	movs r0, #1
	bl Func_08118040
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817935c:
	.4byte gMapCellBuffer
.L_08179360:
	.4byte Data_020124de
.L_08179364:
	.4byte Data_02011f20
.L_08179368:
	.4byte Data_081993d8
.L_0817936c:
	.4byte Data_081993d0
.L_08179370:
	.4byte Data_02012e6e
.L_08179374:
	.4byte Data_081993d4
.L_08179378:
	.4byte Data_02014e9d
.L_0817937c:
	.4byte Data_081993f0
.L_08179380:
	.4byte Data_081993e8
.L_08179384:
	.4byte Data_081993e0
.L_08179388:
	.4byte IwramFillWords
.L_0817938c:
	.4byte 0x3f3f3f3f
.L_08179390:
	.4byte Func_08143000
