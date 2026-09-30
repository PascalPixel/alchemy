.syntax unified
	.thumb
	.global Func_0815806c
	.thumb_func
Func_0815806c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r0, [sp, #24]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	ldr r0, [r3, #92]
	str r1, [sp, #20]
	mov r11, r0
	ldr r3, [r3, #100]
	movs r0, #0
	str r3, [sp, #16]
	bl BattleFx_BeginCanvasLayer
	mov r2, sp
	adds r2, #28
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #12]
	bl Func_08144aac
	ldr r0, .L_081583f4
	ldr r1, [sp, #16]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_081583f8
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #162
	lsls r1, r1, #4
	ldr r0, .L_081583fc
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #75
	add r2, r11
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08158400
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #0
	str r3, [sp, #8]
	movs r7, #0
	mov r6, r11
.L_081580f6:
	lsls r5, r7, #11
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r6]
	adds r0, r5, #0
	bl Trig_Cos
	lsls r0, r0, #2
	asrs r0, r0, #16
	movs r3, #1
	adds r0, #52
	ands r3, r7
	str r0, [r6, #4]
	cmp r3, #0
	beq .L_08158126
	ldr r2, [r6]
	movs r3, #32
	subs r3, r3, r2
	b .L_0815812a
.L_08158126:
	ldr r3, [r6]
	adds r3, #32
.L_0815812a:
	str r3, [r6]
	lsls r3, r7, #1
	negs r3, r3
	str r3, [r6, #24]
	movs r1, #255
	ldr r2, [sp, #8]
	ldr r3, .L_08158404
	lsls r1, r1, #8
	movs r4, #0
	movs r0, #127
	adds r1, #255
	mov r8, r4
	mov r9, r0
	mov r10, r1
	adds r5, r2, r3
.L_08158148:
	bl Random16
	ldr r2, [r6]
	movs r3, #15
	ands r3, r0
	adds r3, r3, r2
	subs r3, #8
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #96
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	mov r4, r9
	ands r0, r4
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #64
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	mov r2, r10
	ands r0, r2
	str r0, [r5, #8]
	bl Random16
	mov r3, r10
	movs r4, #1
	ands r0, r3
	add r8, r4
	str r0, [r5, #20]
	mov r0, r8
	adds r5, #28
	cmp r0, #16
	bne .L_08158148
	ldr r1, [sp, #8]
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r1, r2
	adds r7, #1
	adds r6, #28
	str r1, [sp, #8]
	cmp r7, #9
	bne .L_081580f6
	movs r0, #136
	bl Audio_PlayCue
	movs r4, #172
	movs r3, #0
	negs r4, r4
	mov r8, r3
	mov r10, r4
.L_081581c6:
	mov r0, r8
	cmp r0, #56
	bne .L_081581d2
	movs r0, #133
	bl Func_081180e8
.L_081581d2:
	mov r1, r8
	cmp r1, #23
	bgt .L_08158206
	mov r3, r8
	cmp r1, #0
	bge .L_081581e0
	adds r3, #3
.L_081581e0:
	asrs r3, r3, #2
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	adds r1, r1, r3
	lsls r1, r1, #6
	movs r2, #162
	movs r3, #40
	lsls r2, r2, #4
	add r1, r11
	adds r1, r1, r2
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #28]
	ldr r0, [sp, #20]
	movs r2, #40
	movs r3, #20
	mov lr, r4
	.2byte 0xf800
.L_08158206:
	mov r3, r8
	cmp r3, #20
	bne .L_08158220
	ldr r0, .L_081583f8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08158408
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08158220:
	mov r3, r8
	subs r3, #20
	cmp r3, #11
	bhi .L_0815826a
	mov r4, r8
	cmp r4, #23
	ble .L_08158250
	lsls r3, r4, #2
	movs r2, #146
	subs r2, r2, r3
	movs r3, #20
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	ldr r0, [sp, #12]
	movs r1, #224
	lsls r1, r1, #3
	ldr r4, [r0, #4]
	add r1, r11
	ldr r0, [sp, #20]
	mov r3, r10
	mov lr, r4
	.2byte 0xf800
	b .L_0815826a
.L_08158250:
	movs r3, #20
	movs r1, #224
	str r3, [sp, #0]
	lsls r1, r1, #3
	movs r3, #40
	str r3, [sp, #4]
	ldr r4, [sp, #28]
	ldr r0, [sp, #20]
	add r1, r11
	movs r2, #50
	movs r3, #20
	mov lr, r4
	.2byte 0xf800
.L_0815826a:
	mov r1, r8
	cmp r1, #32
	bne .L_08158292
	movs r0, #145
	bl Audio_PlayCue
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	movs r1, #224
	add r2, r11
	movs r3, #8
	lsls r1, r1, #3
	str r3, [r2]
	ldr r0, .L_0815840c
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_08158292:
	mov r2, r8
	cmp r2, #31
	ble .L_081582ea
	movs r7, #0
	mov r6, r11
.L_0815829c:
	ldr r3, [r6, #24]
	cmp r3, #47
	bhi .L_081582de
	adds r5, r3, #0
	cmp r3, #0
	bge .L_081582aa
	adds r5, r3, #7
.L_081582aa:
	ldr r2, .L_08158410
	asrs r5, r5, #3
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r11
	adds r1, r1, r3
	ldr r3, .L_08158414
	ldr r2, [r6]
	ldrb r4, [r3, r5]
	lsrs r3, r4, #1
	subs r2, r2, r3
	ldr r3, .L_08158418
	ldrb r0, [r3, r5]
	ldr r3, [r6, #4]
	str r4, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_0815841c
	ldr r4, [sp, #28]
	ldrb r0, [r0, r5]
	str r0, [sp, #4]
	ldr r0, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
.L_081582de:
	adds r3, #1
	adds r7, #1
	str r3, [r6, #24]
	adds r6, #28
	cmp r7, #9
	bne .L_0815829c
.L_081582ea:
	ldr r6, .L_08158404
	movs r7, #0
.L_081582ee:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_081582f6
	adds r3, #15
.L_081582f6:
	asrs r3, r3, #4
	lsls r3, r3, #1
	adds r3, #40
	cmp r8, r3
	blt .L_0815835e
	ldr r0, [r6, #8]
	bl Trig_Sin
	movs r5, #1
	movs r4, #2
	ldrsh r2, [r6, r4]
	ands r5, r7
	lsls r0, r0, #2
	adds r5, #3
	asrs r0, r0, #16
	ldr r1, .L_08158420
	adds r2, r2, r0
	lsls r0, r5, #1
	subs r3, r0, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #16]
	adds r1, r3, r1
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r6, r4]
	str r0, [sp, #4]
	str r5, [sp, #0]
	ldr r0, [sp, #12]
	subs r3, r3, r5
	ldr r4, [r0, #4]
	ldr r0, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	movs r1, #64
	ldr r2, .L_08158424
	adds r0, r6, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r2, [r6, #8]
	movs r1, #128
	movs r4, #255
	lsls r1, r1, #4
	lsls r4, r4, #8
	adds r3, r2, r1
	adds r4, #255
	str r3, [r6, #8]
	cmp r3, r4
	ble .L_0815835e
	ldr r0, .L_08158428
	adds r3, r2, r0
	str r3, [r6, #8]
.L_0815835e:
	adds r7, #1
	adds r6, #28
	cmp r7, #144
	bne .L_081582ee
	mov r1, r8
	cmp r1, #38
	bne .L_081583a0
	ldr r2, [sp, #24]
	movs r7, #0
	ldr r3, [r2, #20]
	cmp r3, #0
	beq .L_081583a0
	movs r5, #36
.L_08158378:
	ldr r3, [sp, #24]
	movs r2, #5
	ldrsh r0, [r5, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	bl Func_0814cd48
	ldr r1, [sp, #24]
	adds r7, #1
	ldrsh r0, [r5, r1]
	movs r1, #6
	bl Func_08118088
	ldr r4, [sp, #24]
	adds r5, #2
	ldr r3, [r4, #20]
	cmp r7, r3
	bne .L_08158378
.L_081583a0:
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r8, r1
	movs r0, #8
	mov r2, r8
	add r10, r0
	cmp r2, #112
	beq .L_081583ce
	b .L_081581c6
.L_081583ce:
	ldr r0, .L_08158400
	bl Scheduler_RemoveCallback
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
	.2byte 0x0000
.L_081583f4:
	.4byte 0x00000134
.L_081583f8:
	.4byte 0x00000151
.L_081583fc:
	.4byte 0x0000017b
.L_08158400:
	.4byte Func_08143000
.L_08158404:
	.4byte gMapCellBuffer
.L_08158408:
	.4byte IwramCopyWords
.L_0815840c:
	.4byte 0x00000178
.L_08158410:
	.4byte Data_0819747a
.L_08158414:
	.4byte Data_08197467
.L_08158418:
	.4byte Data_08197473
.L_0815841c:
	.4byte Data_0819746d
.L_08158420:
	.4byte Data_08197410
.L_08158424:
	.4byte 0xffffe000
.L_08158428:
	.4byte 0xffff0801
