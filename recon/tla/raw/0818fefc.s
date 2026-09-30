.syntax unified
	.thumb
	.global Func_0818fefc
	.thumb_func
Func_0818fefc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #220
	str r0, [sp, #96]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	adds r3, r5, #0
	str r0, [sp, #92]
	adds r3, #176
	ldr r1, [r5, #96]
	movs r4, #0
	str r1, [sp, #88]
	ldr r0, [sp, #96]
	ldr r2, [r5, #100]
	str r2, [sp, #68]
	movs r2, #150
	ldr r3, [r3]
	str r3, [sp, #64]
	ldr r3, [r5, #48]
	str r4, [sp, #56]
	str r3, [sp, #60]
	movs r3, #160
	ldr r1, [r0, #8]
	lsls r3, r3, #11
	adds r0, r1, #0
	bl Func_08118078
	movs r0, #0
	bl Func_081435e0
	ldr r3, .L_0818ff84
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #68]
	ldr r0, .L_0818ff88
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r1, [sp, #92]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r4, #132
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818ff8c
	bl Func_080145a8
	movs r1, #19
	movs r0, #104
	b .L_0818ff90
	.2byte 0x0000
.L_0818ff84:
	.4byte 0x00001010
.L_0818ff88:
	.4byte 0x00000134
.L_0818ff8c:
	.4byte Func_08143000
.L_0818ff90:
	bl Func_081963ec
	movs r1, #204
	ldr r0, [sp, #92]
	movs r2, #212
	lsls r1, r1, #7
	lsls r2, r2, #7
	adds r1, #64
	adds r2, #64
	adds r1, r0, r1
	adds r2, r0, r2
	ldr r3, [sp, #96]
	ldr r5, [r5, #104]
	str r1, [sp, #52]
	str r2, [sp, #48]
	str r5, [sp, #72]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r4, #0
	str r0, [sp, #40]
	movs r0, #128
	lsls r0, r0, #9
	str r0, [sp, #28]
	movs r1, #8
	ldr r0, .L_08190178
	movs r2, #16
	movs r3, #32
	str r4, [sp, #36]
	str r4, [sp, #32]
	bl Func_08178680
	movs r1, #2
	ldr r0, [sp, #48]
	movs r2, #32
	movs r3, #15
	bl Func_08178680
	movs r1, #0
	mov r11, r1
	mov r10, r1
	mov r9, r1
.L_0818ffe6:
	mov r0, r10
	bl Trig_Cos
	negs r0, r0
	lsls r5, r0, #1
	adds r5, r5, r0
	mov r0, r10
	bl Trig_Sin
	lsls r5, r5, #4
	lsls r3, r0, #1
	ldr r6, [sp, #52]
	asrs r5, r5, #16
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r5, #24
	asrs r7, r3, #16
	movs r2, #0
	mov r8, r5
	add r6, r9
.L_0819000e:
	lsls r5, r2, #13
	adds r0, r5, #0
	str r2, [sp, #12]
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r6]
	mov r3, r8
	strb r3, [r6, #1]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r2, [sp, #12]
	asrs r3, r3, #16
	adds r2, #1
	strb r3, [r6, #2]
	adds r6, #4
	cmp r2, #2
	bne .L_0819000e
	movs r4, #128
	movs r1, #1
	lsls r4, r4, #3
	add r11, r1
	adds r4, #68
	movs r0, #8
	mov r2, r11
	add r10, r4
	add r9, r0
	cmp r2, #16
	bne .L_0818ffe6
	ldr r3, [sp, #40]
	ldr r0, [sp, #40]
	ldr r3, [r3, #72]
	movs r1, #0
	str r3, [sp, #44]
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r4, [sp, #40]
	mov r0, sp
	mov r1, sp
	movs r3, #0
	adds r0, #208
	adds r1, #100
	str r3, [r4, #36]
	str r3, [r4, #40]
	str r3, [r4, #44]
	str r3, [r4, #52]
	str r3, [r4, #72]
	str r3, [sp, #84]
	str r0, [sp, #20]
	str r1, [sp, #24]
.L_0819007c:
	ldr r2, [sp, #84]
	cmp r2, #4
	bne .L_08190088
	movs r0, #212
	bl Audio_PlayCue
.L_08190088:
	ldr r3, [sp, #84]
	cmp r3, #28
	bne .L_08190094
	movs r0, #140
	bl Audio_PlayCue
.L_08190094:
	ldr r4, [sp, #84]
	cmp r4, #84
	bne .L_081900a0
	movs r0, #104
	bl Audio_PlayCue
.L_081900a0:
	ldr r0, [sp, #84]
	cmp r0, #143
	bne .L_081900ac
	movs r0, #191
	bl Audio_PlayCue
.L_081900ac:
	ldr r1, [sp, #84]
	cmp r1, #171
	bne .L_081900b8
	movs r0, #145
	bl Audio_PlayCue
.L_081900b8:
	ldr r3, .L_0819017c
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081900d2
	ldr r2, [sp, #84]
	cmp r2, #3
	ble .L_081900d2
	cmp r2, #142
	bgt .L_081900d2
	movs r3, #143
	str r3, [sp, #84]
.L_081900d2:
	ldr r4, [sp, #84]
	cmp r4, #0
	beq .L_081900da
	b .L_081901d2
.L_081900da:
	movs r1, #240
	ldr r0, [sp, #88]
	lsls r1, r1, #6
	ldr r2, .L_08190180
	ldr r3, .L_08190184
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_08190188
	ldr r1, .L_0819018c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r4, [sp, #92]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r4, r2
	ldr r0, .L_08190190
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r3, [sp, #92]
	movs r4, #240
	lsls r4, r4, #4
	adds r1, r3, r4
	ldr r0, .L_08190194
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #92]
	movs r3, #184
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, .L_08190198
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r4, [sp, #92]
	movs r2, #172
	lsls r2, r2, #6
	adds r1, r4, r2
	ldr r0, .L_0819019c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	bl Func_0815b410
	movs r3, #128
	ldr r2, .L_08190174
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	ldr r4, [sp, #96]
	movs r0, #160
	ldr r3, [r4, #4]
	lsls r0, r0, #16
	str r0, [sp, #36]
	cmp r3, #0
	beq .L_0819015e
	movs r1, #160
	lsls r1, r1, #15
	str r1, [sp, #36]
.L_0819015e:
	ldr r2, .L_081901a0
	ldr r1, .L_081901a4
	str r2, [sp, #32]
	ldr r0, .L_081901a8
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_081901d0
	b .L_081901ac
.L_08190174:
	.4byte 0x00001010
.L_08190178:
	.4byte Data_02012000
.L_0819017c:
	.4byte gInput
.L_08190180:
	.4byte 0x3f3f3f3f
.L_08190184:
	.4byte IwramFillWords
.L_08190188:
	.4byte 0x000000c2
.L_0819018c:
	.4byte Data_02014000
.L_08190190:
	.4byte 0x000000e9
.L_08190194:
	.4byte 0x000000c9
.L_08190198:
	.4byte 0x00000103
.L_0819019c:
	.4byte 0x000000cd
.L_081901a0:
	.4byte 0xffa00000
.L_081901a4:
	.4byte Data_020038e0
.L_081901a8:
	.4byte 0x04000208
.L_081901ac:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #224
	adds r3, r3, r1
	lsls r2, r2, #3
	adds r3, #4
	adds r2, #132
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_081901d0:
	strh r4, [r0]
.L_081901d2:
	ldr r3, [sp, #84]
	cmp r3, #1
	bne .L_08190214
	ldr r4, [sp, #64]
	add r0, sp, #180
	str r3, [r4, #16]
	ldr r1, [sp, #96]
	ldr r3, [r1, #8]
	movs r1, #0
	strh r3, [r0]
	movs r3, #255
	strh r3, [r0, #2]
	bl Func_08118010
	movs r0, #1
	ldr r1, .L_08190388
	movs r2, #0
	bl Func_08118040
	bl Func_0817d6c4
	movs r0, #8
	movs r1, #2
	movs r2, #2
	bl Func_08164abc
	movs r1, #205
	lsls r1, r1, #1
	movs r0, #1
	adds r1, #255
	movs r2, #2
	bl Func_08152404
.L_08190214:
	ldr r2, [sp, #96]
	ldr r1, [sp, #20]
	ldr r0, [r2, #8]
	bl Func_0815e20c
	ldr r3, [sp, #84]
	cmp r3, #3
	bne .L_0819022c
	ldr r4, [sp, #60]
	movs r0, #54
	ldrsh r4, [r4, r0]
	str r4, [sp, #56]
.L_0819022c:
	ldr r3, [sp, #84]
	subs r3, #16
	cmp r3, #61
	bhi .L_08190258
	ldr r0, [sp, #96]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0819024a
	ldr r1, [sp, #60]
	ldr r2, .L_0819038c
	ldrh r3, [r1, #54]
	adds r4, r1, #0
	adds r3, r3, r2
	strh r3, [r4, #54]
	b .L_08190258
.L_0819024a:
	ldr r0, [sp, #60]
	movs r1, #128
	ldrh r3, [r0, #54]
	lsls r1, r1, #1
	adds r3, r3, r1
	adds r2, r0, #0
	strh r3, [r2, #54]
.L_08190258:
	movs r3, #0
	ldr r4, [sp, #84]
	str r3, [sp, #80]
	str r3, [sp, #16]
	mov r10, r4
	mov r11, r3
.L_08190264:
	ldr r5, .L_08190390
	ldr r7, [sp, #16]
	movs r0, #0
	mov r8, r0
	mov r9, r10
	add r5, r11
.L_08190270:
	mov r1, r8
	movs r6, #80
	cmp r1, #1
	beq .L_08190288
	mov r2, r9
	lsls r0, r2, #12
	bl Trig_Sin
	lsls r0, r0, #1
	asrs r0, r0, #16
	adds r6, r0, #0
	adds r6, #120
.L_08190288:
	adds r0, r7, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r5]
	adds r0, r7, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r4, #1
	asrs r3, r3, #16
	add r8, r4
	strb r3, [r5, #1]
	mov r0, r8
	movs r3, #0
	strb r3, [r5, #2]
	adds r5, #4
	cmp r0, #2
	bne .L_08190270
	ldr r1, [sp, #16]
	ldr r0, [sp, #80]
	movs r2, #128
	lsls r2, r2, #4
	adds r1, r1, r2
	movs r3, #4
	movs r4, #8
	adds r0, #1
	str r1, [sp, #16]
	add r10, r3
	add r11, r4
	str r0, [sp, #80]
	cmp r0, #33
	bne .L_08190264
	ldr r4, .L_08190394
	ldrh r1, [r4, #6]
	cmp r1, #104
	ble .L_081902dc
	ldr r2, .L_08190398
	adds r1, r1, r2
.L_081902dc:
	ldr r3, [sp, #84]
	cmp r3, #63
	bgt .L_081902ea
	movs r0, #16
	adds r5, r3, #0
	negs r0, r0
	b .L_08190342
.L_081902ea:
	ldr r3, [sp, #84]
	subs r3, #64
	cmp r3, #7
	bhi .L_081902fe
	ldr r0, [sp, #84]
	ldr r5, [sp, #84]
	lsls r2, r0, #1
	adds r0, r2, #0
	subs r0, #144
	b .L_08190342
.L_081902fe:
	ldr r3, [sp, #84]
	subs r3, #72
	cmp r3, #11
	bhi .L_0819030c
	ldr r5, [sp, #84]
	movs r0, #0
	b .L_08190342
.L_0819030c:
	ldr r2, [sp, #84]
	cmp r2, #154
	ble .L_0819032a
	adds r3, r2, #0
	subs r3, #155
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #16
	subs r0, r2, r3
	cmp r0, #0
	bge .L_08190340
	ldr r5, [sp, #84]
	movs r0, #0
	b .L_08190342
.L_0819032a:
	ldr r5, [sp, #84]
	subs r5, #84
	adds r0, r5, #0
	cmp r5, #16
	ble .L_08190336
	movs r0, #16
.L_08190336:
	ldr r2, .L_0819039c
	ldr r3, [r2, #16]
	subs r3, r3, r0
	str r3, [r2, #16]
	b .L_08190344
.L_08190340:
	ldr r5, [sp, #84]
.L_08190342:
	subs r5, #84
.L_08190344:
	movs r3, #16
	adds r1, r1, r0
	negs r3, r3
	cmp r1, r3
	bge .L_08190350
	adds r1, #120
.L_08190350:
	cmp r1, #104
	ble .L_08190356
	subs r1, #120
.L_08190356:
	strh r1, [r4, #6]
	ldr r3, [sp, #84]
	subs r3, #42
	cmp r3, #37
	bhi .L_08190370
	ldr r4, [sp, #32]
	movs r0, #128
	lsls r0, r0, #10
	adds r4, r4, r0
	ldr r0, .L_081903a0
	str r4, [sp, #32]
	bl Func_0815f0a0
.L_08190370:
	cmp r5, #58
	bhi .L_081903bc
	ldr r1, [sp, #96]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_081903a8
	ldr r2, [sp, #36]
	ldr r3, .L_081903a4
	adds r2, r2, r3
	str r2, [sp, #36]
	b .L_081903b2
	.2byte 0x0000
.L_08190388:
	.4byte 0x00000072
.L_0819038c:
	.4byte 0xffffff00
.L_08190390:
	.4byte gMapCellBuffer
.L_08190394:
	.4byte Data_03001120
.L_08190398:
	.4byte 0xffff0000
.L_0819039c:
	.4byte gCameraSceneParameters
.L_081903a0:
	.4byte 0x00000148
.L_081903a4:
	.4byte 0xfffe0000
.L_081903a8:
	ldr r4, [sp, #36]
	movs r0, #128
	lsls r0, r0, #10
	adds r4, r4, r0
	str r4, [sp, #36]
.L_081903b2:
	ldr r1, [sp, #32]
	movs r2, #128
	lsls r2, r2, #11
	adds r1, r1, r2
	str r1, [sp, #32]
.L_081903bc:
	ldr r3, [sp, #84]
	cmp r3, #143
	bne .L_0819043c
	movs r2, #2
	negs r2, r2
	movs r0, #12
	adds r1, r2, #0
	negs r0, r0
	bl Func_08164abc
	add r4, sp, #56
	ldr r0, [sp, #60]
	ldrh r4, [r4]
	movs r1, #192
	strh r4, [r0, #54]
	movs r0, #240
	lsls r0, r0, #15
	lsls r1, r1, #14
	str r0, [sp, #36]
	str r1, [sp, #32]
	ldr r2, [sp, #96]
	add r0, sp, #152
	ldrh r3, [r2, #36]
	movs r1, #0
	strh r3, [r0]
	movs r3, #255
	strh r3, [r0, #2]
	bl Func_08118010
	ldr r2, .L_0819077c
	movs r3, #139
	lsls r3, r3, #2
	str r3, [r2, #16]
	ldr r4, [sp, #96]
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl GetBattleObjectSlotFar
	ldr r3, [r0]
	movs r0, #0
	str r0, [r3, #8]
	str r0, [r3, #16]
	ldr r1, [sp, #92]
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
	ldr r0, .L_08190780
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08190784
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0819043c:
	ldr r0, [sp, #84]
	cmp r0, #154
	ble .L_0819045e
	adds r3, r0, #0
	subs r3, #155
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #16
	subs r1, r2, r3
	cmp r1, #0
	bge .L_08190456
	movs r1, #0
.L_08190456:
	ldr r2, .L_0819077c
	ldr r3, [r2, #16]
	subs r3, r3, r1
	str r3, [r2, #16]
.L_0819045e:
	ldr r1, [sp, #84]
	cmp r1, #84
	bne .L_081904c8
	ldr r1, .L_08190788
	ldr r0, .L_0819078c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08190498
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #224
	adds r3, r3, r1
	lsls r2, r2, #3
	adds r3, #4
	adds r2, #133
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08190498:
	strh r4, [r0]
	ldr r0, [sp, #40]
	movs r1, #128
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r1, #240
	lsls r1, r1, #6
	ldr r2, .L_08190790
	ldr r3, .L_08190794
	ldr r0, [sp, #88]
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #92]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r4, r0
	movs r3, #3
	movs r1, #238
	str r3, [r2]
	lsls r1, r1, #7
	ldr r3, .L_08190798
	adds r1, #132
	adds r2, r4, r1
	str r3, [r2]
.L_081904c8:
	ldr r2, [sp, #84]
	cmp r2, #171
	bne .L_08190528
	ldr r3, [sp, #92]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
	movs r1, #240
	ldr r2, .L_08190790
	ldr r0, [sp, #88]
	ldr r3, .L_08190794
	lsls r1, r1, #6
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #96]
	movs r4, #36
	ldrsh r0, [r1, r4]
	movs r1, #0
	bl Func_08118088
	ldr r3, [sp, #96]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #16
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r4, [sp, #92]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r4, r0
	movs r3, #1
	movs r1, #238
	str r3, [r2]
	lsls r1, r1, #7
	ldr r3, .L_0819079c
	adds r1, #132
	adds r2, r4, r1
	str r3, [r2]
.L_08190528:
	ldr r2, [sp, #84]
	cmp r2, #174
	ble .L_08190538
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
.L_08190538:
	ldr r3, [sp, #84]
	cmp r3, #1
	bgt .L_08190540
	b .L_081906e8
.L_08190540:
	cmp r3, #142
	ble .L_08190560
	movs r2, #128
	lsls r2, r2, #3
	subs r3, #143
	adds r2, #212
	muls r3, r2
	movs r4, #128
	lsls r4, r4, #7
	movs r0, #192
	adds r4, r3, r4
	lsls r0, r0, #8
	str r4, [sp, #28]
	cmp r4, r0
	ble .L_08190560
	str r0, [sp, #28]
.L_08190560:
	ldr r1, [sp, #28]
	add r2, sp, #116
	lsls r3, r1, #1
	str r3, [r2, #4]
	add r1, sp, #136
	str r3, [sp, #116]
	movs r3, #0
	str r3, [r1, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r1, #4]
	ldr r4, [sp, #36]
	str r4, [r1]
	ldr r0, [sp, #32]
	str r0, [r1, #8]
	ldr r4, [sp, #92]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #220
	adds r3, r4, r0
	ldr r0, [r3]
	movs r3, #32
	strb r3, [r0, #23]
	movs r3, #0
	strb r3, [r0, #22]
	movs r4, #208
	ldr r3, [r1, #8]
	lsls r4, r4, #14
	adds r3, r3, r4
	ldr r4, .L_081907a0
	cmp r3, r4
	bhi .L_081905a6
	movs r3, #0
	bl Func_08020010
.L_081905a6:
	ldr r0, [sp, #84]
	cmp r0, #1
	bgt .L_081905ae
	b .L_081906e8
.L_081905ae:
	movs r0, #1
	bl Func_081969f8
	ldr r1, .L_081907a4
	add r6, sp, #108
	adds r7, r0, #0
	movs r3, #6
	movs r2, #0
	str r3, [r7]
	strb r3, [r6]
	movs r3, #4
	str r1, [r7, #12]
	strb r3, [r6, #1]
	str r6, [r7, #16]
	str r2, [r7, #20]
	strb r2, [r7, #25]
	ldr r2, [sp, #84]
	movs r1, #5
	lsrs r0, r2, #31
	adds r0, r2, r0
	asrs r0, r0, #1
	bl __modsi3
	ldr r3, [sp, #92]
	lsls r0, r0, #10
	movs r4, #184
	adds r0, r3, r0
	lsls r4, r4, #5
	adds r0, r0, r4
	str r0, [r6, #4]
	ldr r0, .L_081907a8
	movs r5, #128
	str r0, [r7, #8]
	bl Func_08014de4
	lsls r5, r5, #8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_080151e4
	ldr r4, .L_081907ac
	ldr r1, [sp, #36]
	ldr r3, [sp, #32]
	ldr r2, .L_081907b0
	adds r0, r1, r2
	adds r1, r3, r4
	movs r2, #0
	bl Func_08015160
	movs r0, #181
	lsls r0, r0, #8
	adds r0, #200
	bl Func_0801521c
	ldr r0, [sp, #28]
	bl Func_0801521c
	ldr r1, .L_081907a4
	movs r2, #66
	ldr r0, .L_081907b4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	movs r3, #5
	strb r3, [r6]
	strb r3, [r6, #1]
	ldr r0, [sp, #92]
	movs r1, #172
	lsls r1, r1, #6
	adds r3, r0, r1
	str r3, [r6, #4]
	movs r3, #7
	str r3, [r7]
	ldr r2, [sp, #48]
	movs r3, #3
	str r2, [r7, #8]
	movs r2, #24
	str r3, [r7, #4]
	negs r2, r2
	movs r3, #0
	str r2, [r7, #20]
	strb r3, [r7, #25]
	ldr r4, [sp, #84]
	cmp r4, #142
	ble .L_081906e2
	adds r1, r4, #0
	subs r1, #143
	cmp r1, #32
	ble .L_0819066a
	movs r1, #32
.L_0819066a:
	ldr r3, [sp, #84]
	ldr r4, [sp, #84]
	lsls r2, r3, #1
	lsls r3, r1, #1
	adds r3, r3, r1
	ldr r6, .L_081907b8
	movs r0, #0
	lsls r3, r3, #7
	mov r8, r0
	mov r10, r3
	adds r5, r2, r4
.L_08190680:
	movs r3, #127
	bics r3, r5
	strb r3, [r7, #24]
	bl Func_08014de4
	ldr r0, .L_081907bc
	ldr r1, .L_081907c0
	movs r2, #0
	bl Func_08015160
	movs r0, #160
	lsls r0, r0, #9
	movs r2, #128
	ldr r1, .L_081907c4
	lsls r2, r2, #9
	adds r0, #80
	bl Func_080151e4
	ldr r0, [sp, #28]
	bl Func_0801521c
	movs r0, #128
	lsls r0, r0, #8
	bl Func_080150e4
	ldr r0, .L_081907c8
	adds r5, #20
	add r0, r10
	bl Func_08015024
	adds r0, r6, #0
	bl Func_08015068
	ldr r1, .L_081907a4
	ldr r0, [sp, #52]
	movs r2, #32
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	movs r1, #1
	movs r0, #192
	add r8, r1
	lsls r0, r0, #5
	mov r2, r8
	adds r6, r6, r0
	cmp r2, #10
	bne .L_08190680
.L_081906e2:
	adds r0, r7, #0
	bl Sys_Free
.L_081906e8:
	ldr r3, [sp, #84]
	cmp r3, #8
	bne .L_08190720
	movs r1, #240
	ldr r0, [sp, #88]
	lsls r1, r1, #6
	ldr r2, .L_08190790
	ldr r4, .L_08190794
	mov lr, r4
	.2byte 0xf800
	ldr r5, [sp, #92]
	movs r0, #0
	mov r8, r0
	movs r6, #127
.L_08190704:
	bl Random16
	ands r0, r6
	str r0, [r5]
	bl Random16
	movs r1, #1
	add r8, r1
	ands r0, r6
	mov r2, r8
	str r0, [r5, #4]
	adds r5, #28
	cmp r2, #64
	bne .L_08190704
.L_08190720:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #79
	bhi .L_081907d0
	ldr r4, [sp, #84]
	ldr r1, .L_081907cc
	lsls r3, r4, #1
	adds r6, r3, #0
	ldr r5, [sp, #92]
	subs r6, #16
	movs r3, #127
	movs r0, #0
	ands r6, r3
	mov r8, r0
	mov r10, r1
	movs r7, #2
.L_08190740:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0819076e
	ldr r3, [r5, #4]
	movs r0, #1
	subs r3, r3, r6
	ldr r2, [r5]
	cmp r3, #127
	ble .L_08190754
	subs r3, #128
.L_08190754:
	cmp r3, #0
	bge .L_0819075a
	adds r3, #128
.L_0819075a:
	mov r4, r10
	ldrh r1, [r4]
	ldr r4, [sp, #68]
	str r0, [sp, #0]
	adds r1, r4, r1
	str r7, [sp, #4]
	ldr r0, [sp, #88]
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
.L_0819076e:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_08190740
	b .L_081907d0
.L_0819077c:
	.4byte gCameraSceneParameters
.L_08190780:
	.4byte 0x00000148
.L_08190784:
	.4byte IwramCopyWords
.L_08190788:
	.4byte Data_020038e0
.L_0819078c:
	.4byte 0x04000208
.L_08190790:
	.4byte 0x3f3f3f3f
.L_08190794:
	.4byte IwramFillWords
.L_08190798:
	.4byte 0x06060606
.L_0819079c:
	.4byte 0x10101010
.L_081907a0:
	.4byte 0x00fbffff
.L_081907a4:
	.4byte Data_02011000
.L_081907a8:
	.4byte Data_02012000
.L_081907ac:
	.4byte 0xffb40000
.L_081907b0:
	.4byte 0xff820000
.L_081907b4:
	.4byte gMapCellBuffer
.L_081907b8:
	.4byte 0xffffc000
.L_081907bc:
	.4byte 0xfffe0000
.L_081907c0:
	.4byte 0xffe00000
.L_081907c4:
	.4byte 0x00035208
.L_081907c8:
	.4byte 0xffffd000
.L_081907cc:
	.4byte Data_08197410
.L_081907d0:
	ldr r2, [sp, #84]
	cmp r2, #142
	ble .L_0819086c
	adds r6, r2, #0
	subs r6, #143
	cmp r6, #32
	ble .L_081907e0
	movs r6, #32
.L_081907e0:
	ldr r3, [sp, #84]
	cmp r3, #143
	bne .L_08190818
	ldr r5, [sp, #92]
	movs r4, #0
	mov r8, r4
	movs r7, #127
.L_081907ee:
	bl Random16
	ands r0, r7
	str r0, [r5]
	bl Random16
	movs r1, #192
	bl Math_ModU
	subs r0, #64
	str r0, [r5, #4]
	bl Random16
	ands r0, r7
	str r0, [r5, #8]
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_081907ee
.L_08190818:
	bl Func_08014de4
	lsls r0, r6, #1
	ldr r2, .L_08190b2c
	adds r0, r0, r6
	lsls r0, r0, #7
	adds r0, r0, r2
	bl Func_08015024
	ldr r7, .L_08190b30
	ldr r5, [sp, #92]
	movs r3, #0
	movs r4, #1
	mov r8, r3
	mov r10, r4
	add r6, sp, #124
.L_08190838:
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_08015778
	ldrh r1, [r7]
	ldr r0, [sp, #68]
	mov r4, r10
	adds r1, r0, r1
	movs r0, #4
	ldr r3, [r6, #4]
	ldr r2, [r6]
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #72]
	ldr r0, [sp, #88]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #4]
	movs r0, #1
	add r8, r0
	subs r3, #2
	mov r1, r8
	str r3, [r5, #4]
	adds r5, #28
	cmp r1, #64
	bne .L_08190838
.L_0819086c:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08190b34
	ldr r3, [sp, #100]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08190b38
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #100]
	ldr r2, .L_08190b3c
	ldr r3, [sp, #24]
	adds r7, r0, #0
	str r2, [r3, #4]
	ldr r3, .L_08190b40
	str r1, [r7]
	str r3, [r7, #8]
	ldr r4, [sp, #24]
	mov r0, r11
	str r4, [r7, #16]
	str r0, [r7, #12]
	ldr r2, [sp, #20]
	movs r1, #0
	mov r8, r1
	mov r10, r2
	movs r6, #0
.L_081908b0:
	ldr r3, .L_08190b44
	ldrh r2, [r3, r6]
	ldr r3, [sp, #84]
	cmp r3, r2
	blt .L_08190942
	ldr r4, [sp, #84]
	adds r3, r2, #0
	adds r3, #20
	cmp r4, r3
	bge .L_08190942
	subs r1, r4, r2
	lsls r2, r1, #3
	movs r3, #64
	subs r4, r3, r2
	cmp r4, #0
	ble .L_081908d2
	movs r4, #0
.L_081908d2:
	movs r0, #64
	negs r0, r0
	cmp r4, r0
	ble .L_08190942
	lsls r3, r1, #14
	movs r1, #128
	lsls r1, r1, #7
	str r4, [r7, #20]
	adds r5, r3, r1
	bl Func_08014de4
	mov r2, r8
	cmp r2, #1
	bgt .L_0819090c
	mov r3, r10
	ldr r0, [r3]
	mov r4, r10
	lsrs r3, r0, #31
	ldr r1, [r4, #4]
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	b .L_08190918
.L_0819090c:
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #13
	movs r2, #0
	bl Func_08015160
.L_08190918:
	lsls r1, r5, #1
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	ldr r3, .L_08190b48
	ldrsh r0, [r3, r6]
	bl Func_08015024
	ldr r3, .L_08190b4c
	ldrsh r0, [r3, r6]
	bl Func_08015068
	ldr r0, .L_08190b50
	mov r1, r11
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08190942:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r6, #2
	cmp r4, #5
	bne .L_081908b0
	movs r3, #7
	str r3, [r7]
	ldr r0, [sp, #24]
	add r3, sp, #100
	strb r4, [r0]
	str r3, [sp, #24]
	ldr r1, [sp, #24]
	movs r3, #6
	strb r3, [r1, #1]
	ldr r2, [sp, #92]
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r4
	str r3, [r1, #4]
	ldr r3, .L_08190b54
	movs r0, #0
	str r3, [r7, #8]
	ldr r1, [sp, #20]
	movs r2, #16
	mov r8, r0
	mov r10, r1
	movs r6, #0
	mov r9, r2
.L_0819097c:
	ldr r3, [sp, #84]
	cmp r3, r9
	bge .L_08190984
	b .L_08190a80
.L_08190984:
	ldr r4, [sp, #84]
	adds r3, r6, #0
	adds r3, #80
	cmp r4, r3
	blt .L_08190990
	b .L_08190a80
.L_08190990:
	mov r0, r9
	subs r2, r4, r0
	lsls r3, r2, #3
	adds r1, r3, #0
	subs r1, #64
	lsls r5, r2, #11
	cmp r1, #0
	ble .L_081909a2
	movs r1, #0
.L_081909a2:
	movs r3, #128
	lsls r3, r3, #9
	cmp r5, r3
	ble .L_081909ae
	movs r5, #128
	lsls r5, r5, #9
.L_081909ae:
	cmp r2, #47
	ble .L_081909bc
	lsls r3, r2, #12
	movs r4, #192
	subs r3, r5, r3
	lsls r4, r4, #10
	adds r5, r3, r4
.L_081909bc:
	str r1, [r7, #20]
	bl Func_08014de4
	ldr r0, [sp, #96]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_081909f6
	mov r1, r10
	ldr r0, [r1]
	mov r2, r8
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, .L_08190b58
	asrs r0, r0, #1
	ldrsb r3, [r3, r2]
	mov r4, r10
	adds r0, r0, r3
	ldr r3, .L_08190b5c
	subs r0, #64
	ldrsb r1, [r3, r2]
	ldr r3, [r4, #4]
	lsls r0, r0, #16
	adds r1, r1, r3
	subs r1, #54
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	b .L_08190a20
.L_081909f6:
	mov r1, r10
	ldr r0, [r1]
	mov r2, r8
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, .L_08190b58
	asrs r0, r0, #1
	ldrsb r3, [r3, r2]
	mov r4, r10
	subs r0, r0, r3
	ldr r3, .L_08190b5c
	subs r0, #62
	ldrsb r1, [r3, r2]
	ldr r3, [r4, #4]
	lsls r0, r0, #16
	adds r1, r1, r3
	subs r1, #54
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
.L_08190a20:
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	ldr r0, [sp, #96]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_08190a40
	ldr r3, .L_08190b60
	ldrsh r0, [r3, r6]
	bl Func_080150e4
	b .L_08190a4a
.L_08190a40:
	ldr r3, .L_08190b60
	ldrsh r0, [r3, r6]
	negs r0, r0
	bl Func_080150e4
.L_08190a4a:
	ldr r3, .L_08190b64
	ldrsh r0, [r3, r6]
	bl Func_08015024
	ldr r3, .L_08190b68
	ldrsh r0, [r3, r6]
	bl Func_08015068
	adds r0, r5, #0
	cmp r5, #0
	bge .L_08190a62
	adds r0, r5, #3
.L_08190a62:
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r0, r0, #2
	lsls r1, r5, #2
	asrs r2, r2, #1
	bl Func_080151e4
	ldr r0, .L_08190b6c
	mov r1, r11
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08190a80:
	movs r3, #1
	add r8, r3
	movs r2, #2
	mov r4, r8
	adds r6, #2
	add r9, r2
	cmp r4, #8
	beq .L_08190a92
	b .L_0819097c
.L_08190a92:
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
	movs r1, #240
	ldr r0, [sp, #92]
	lsls r1, r1, #7
	adds r1, #228
	adds r2, r0, r1
.L_08190aa8:
	ldr r3, [r2]
	ldr r3, [r2]
	cmp r3, #1
	bls .L_08190aa8
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #92]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #84]
	adds r0, #1
	str r0, [sp, #84]
	cmp r0, #188
	beq .L_08190ad6
	bl .L_0819007c
.L_08190ad6:
	ldr r1, [sp, #92]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	adds r3, r1, r2
	ldr r0, [r3]
	bl Func_08020048
	ldr r3, [sp, #44]
	ldr r4, [sp, #40]
	movs r5, #0
	str r3, [r4, #72]
	bl BattleActor_CommitPlacementFar
	movs r3, #192
	ldr r0, [sp, #64]
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	str r5, [r0, #16]
	movs r1, #206
	lsls r1, r1, #3
	adds r3, r3, r1
	ldrh r1, [r3]
	movs r2, #24
	movs r0, #1
	bl Func_08118040
	ldr r0, .L_08190b70
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #220
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08190b2c:
	.4byte 0xffffd000
.L_08190b30:
	.4byte Data_08197410
.L_08190b34:
	.4byte 0xffffff00
.L_08190b38:
	.4byte 0xffff00ff
.L_08190b3c:
	.4byte Data_02014000
.L_08190b40:
	.4byte Data_08199364
.L_08190b44:
	.4byte Data_08199ee0
.L_08190b48:
	.4byte Data_08199ecc
.L_08190b4c:
	.4byte Data_08199ed6
.L_08190b50:
	.4byte Data_081991e0
.L_08190b54:
	.4byte Data_081992b0
.L_08190b58:
	.4byte Data_08199f1a
.L_08190b5c:
	.4byte Data_08199f22
.L_08190b60:
	.4byte Data_08199f0a
.L_08190b64:
	.4byte Data_08199eea
.L_08190b68:
	.4byte Data_08199efa
.L_08190b6c:
	.4byte Data_081991f0
.L_08190b70:
	.4byte Func_08143000
