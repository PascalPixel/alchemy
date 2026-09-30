.syntax unified
	.thumb
	.global Func_08151040
	.thumb_func
Func_08151040:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	sub sp, #56
	mov r10, r0
	ldr r0, [r3, #92]
	str r1, [sp, #32]
	mov r4, r10
	ldr r2, [r3, #48]
	mov r11, r0
	str r2, [sp, #24]
	ldr r3, [r3, #100]
	str r3, [sp, #20]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08151076
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	b .L_0815107c
.L_08151076:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
.L_0815107c:
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_081510f0
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #20]
	movs r3, #0
	ldr r0, .L_081510f4
	bl Func_08157cf4
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #19
	movs r0, #188
	str r3, [sp, #36]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r5, sp
	adds r5, #36
	str r5, [sp, #12]
	movs r2, #128
	str r3, [r5, #4]
	ldr r3, .L_081510ec
	lsls r2, r2, #19
	adds r2, #82
	mov r1, r10
	strh r3, [r2]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r2, r10
	mov r8, r0
	ldr r0, [r2, #8]
	bl Func_08118070
	mov r4, r8
	ldr r3, [r4, #12]
	ldr r7, .L_081510f8
	adds r3, r3, r0
	str r3, [sp, #16]
	movs r5, #0
	mov r9, r5
	b .L_081510fc
	.2byte 0x0000
.L_081510ec:
	.4byte 0x00001010
.L_081510f0:
	.4byte 0x00000178
.L_081510f4:
	.4byte 0x00000134
.L_081510f8:
	.4byte gMapCellBuffer
.L_081510fc:
	bl Random16
	adds r6, r0, #0
	bl Random16
	adds r5, r0, #0
	movs r0, #127
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	bl Random16
	movs r1, #127
	ands r0, r1
	subs r0, #16
	lsls r0, r0, #16
	asrs r0, r0, #6
	str r0, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #20]
	mov r2, r8
	ldr r3, [r2, #8]
	movs r4, #1
	str r3, [r7]
	ldr r3, [sp, #16]
	add r9, r4
	str r3, [r7, #4]
	mov r5, r9
	ldr r3, [r2, #16]
	str r3, [r7, #8]
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
	adds r7, #28
	cmp r5, #64
	bne .L_081510fc
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #172
	movs r2, #0
	add r3, r11
	str r2, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #176
	movs r5, #200
	add r3, r11
	lsls r5, r5, #4
	str r2, [r3]
	adds r1, r5, #0
	ldr r0, .L_08151498
	bl Scheduler_AddOrUpdateCallback
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
	adds r1, r5, #0
	ldr r0, .L_0815149c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	str r0, [sp, #28]
	mov r1, r10
	ldr r2, .L_081514a0
	ldr r3, [r1, #24]
	ldrb r3, [r2, r3]
	movs r2, #132
	lsrs r3, r3, #1
	negs r2, r2
	cmp r3, r2
	bne .L_081511b2
	b .L_0815146e
.L_081511b2:
	ldr r3, [sp, #24]
	adds r3, #12
	str r3, [sp, #8]
.L_081511b8:
	ldr r3, [sp, #28]
	subs r3, #17
	cmp r3, #62
	bhi .L_081511ce
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #172
	movs r3, #128
	add r2, r11
	lsls r3, r3, #1
	b .L_081511d8
.L_081511ce:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #172
	add r2, r11
	movs r3, #0
.L_081511d8:
	str r3, [r2]
	mov r4, r10
	ldr r5, .L_081514a0
	ldr r3, [r4, #24]
	ldr r0, [sp, #28]
	ldrb r3, [r5, r3]
	lsrs r3, r3, #1
	adds r3, #108
	cmp r0, r3
	bne .L_081511f2
	movs r0, #133
	bl Func_081180e8
.L_081511f2:
	movs r2, #0
	movs r3, #100
	movs r0, #0
	movs r1, #0
	bl Func_08118028
	bl Func_08014de4
	ldr r1, [sp, #8]
	ldr r0, [sp, #24]
	bl Func_080156e8
	mov r3, r10
	ldr r2, [r3, #24]
	movs r1, #0
	ldrb r3, [r5, r2]
	mov r9, r1
	adds r1, r5, #0
	cmp r3, #0
	bne .L_0815121c
	b .L_081513c2
.L_0815121c:
	ldr r6, .L_081514a4
.L_0815121e:
	mov r4, r9
	lsrs r3, r4, #31
	add r3, r9
	asrs r3, r3, #1
	ldr r5, [sp, #28]
	mov r8, r3
	mov r7, r8
	adds r7, #48
	cmp r5, r8
	ble .L_081512ce
	ldr r3, [r6, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_081512ca
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #8]
	cmp r3, #159
	bgt .L_08151256
	movs r3, #160
	str r3, [r5, #8]
.L_08151256:
	movs r2, #136
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	ble .L_08151264
	str r2, [r5, #8]
	adds r3, r2, #0
.L_08151264:
	adds r2, r3, #0
	subs r2, #160
	cmp r2, #0
	bge .L_0815126e
	adds r2, #63
.L_0815126e:
	asrs r2, r2, #6
	movs r3, #10
	subs r4, r3, r2
	ldr r2, [sp, #28]
	mov r7, r8
	movs r1, #4
	adds r7, #48
	mov r12, r1
	cmp r2, r7
	blt .L_08151286
	movs r3, #0
	mov r12, r3
.L_08151286:
	ldr r2, .L_081514a8
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	lsrs r3, r4, #31
	adds r1, r2, r1
	ldr r2, [r5]
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #12]
	mov r5, r12
	subs r3, r3, r4
	ldr r4, [r5, r0]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6]
	ldr r2, [r6, #12]
	adds r3, r3, r2
	str r3, [r6]
	ldr r2, [r6, #16]
	ldr r3, [r6, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r2, [r6, #20]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_081512ce
.L_081512ca:
	mov r7, r8
	adds r7, #48
.L_081512ce:
	ldr r1, [sp, #28]
	cmp r1, r7
	ble .L_081513ae
	ldr r3, [r6, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_081513ae
	mov r3, r10
	ldr r1, [r3, #20]
	mov r0, r9
	bl __modsi3
	lsls r0, r0, #1
	mov r4, r10
	adds r0, #36
	ldrsh r0, [r4, r0]
	bl GetBattleObjectSlotFar
	ldr r1, [r0]
	ldr r2, [r6]
	ldr r3, [r1, #8]
	subs r3, r3, r2
	ldr r2, [r6, #12]
	asrs r3, r3, #9
	adds r0, r2, r3
	str r0, [r6, #12]
	ldr r2, [r6, #4]
	ldr r3, [r1, #12]
	subs r3, r3, r2
	ldr r2, [r6, #16]
	asrs r3, r3, #9
	adds r4, r2, r3
	str r4, [r6, #16]
	ldr r2, [r6, #8]
	ldr r3, [r1, #16]
	subs r3, r3, r2
	ldr r2, [r6, #20]
	asrs r3, r3, #9
	adds r1, r2, r3
	str r1, [r6, #20]
	ldr r2, [sp, #28]
	mov r3, r8
	adds r3, #85
	cmp r2, r3
	bge .L_0815135a
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08151336
	adds r2, #63
.L_08151336:
	asrs r3, r2, #6
	str r3, [r6, #12]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08151346
	adds r2, #63
.L_08151346:
	asrs r3, r2, #6
	str r3, [r6, #16]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08151356
	adds r2, #63
.L_08151356:
	asrs r3, r2, #6
	str r3, [r6, #20]
.L_0815135a:
	ldr r3, [r6, #4]
	cmp r3, #0
	bge .L_081513ae
	movs r3, #0
	str r3, [r6, #24]
	add r2, sp, #44
	ldr r3, [r2]
	mov r4, r10
	str r3, [r6]
	ldr r3, [r2, #4]
	str r3, [r6, #4]
	ldr r3, [r4, #24]
	cmp r3, #2
	bne .L_0815137e
	movs r0, #134
	bl Audio_PlayCue
	b .L_08151384
.L_0815137e:
	movs r0, #136
	bl Audio_PlayCue
.L_08151384:
	mov r5, r10
	ldr r1, [r5, #20]
	mov r0, r9
	bl __modsi3
	adds r3, r0, #0
	lsls r2, r3, #1
	adds r2, #36
	ldrsh r0, [r5, r2]
	movs r2, #4
	str r2, [sp, #0]
	movs r1, #10
	movs r2, #5
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #2
	str r3, [r2]
.L_081513ae:
	movs r2, #1
	mov r3, r10
	add r9, r2
	ldr r1, .L_081514a0
	ldr r2, [r3, #24]
	adds r6, #28
	ldrb r3, [r1, r2]
	cmp r9, r3
	beq .L_081513c2
	b .L_0815121e
.L_081513c2:
	ldrb r3, [r1, r2]
	movs r4, #0
	mov r9, r4
	cmp r3, #0
	beq .L_08151426
	ldr r6, .L_081514a4
.L_081513ce:
	ldr r3, [r6, #24]
	cmp r3, #11
	bhi .L_0815141a
	lsrs r4, r3, #31
	ldr r2, .L_081514ac
	adds r4, r3, r4
	asrs r4, r4, #1
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_081514b0
	movs r5, #224
	lsls r5, r5, #3
	add r1, r11
	adds r1, r1, r5
	ldrb r5, [r3, r4]
	ldr r2, [r6]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_081514b4
	ldrb r0, [r3, r4]
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_081514b8
	subs r3, #56
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #12]
	ldr r4, [r0, #4]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
	ldr r1, .L_081514a0
	adds r3, #1
	str r3, [r6, #24]
	mov r3, r10
	ldr r2, [r3, #24]
.L_0815141a:
	ldrb r3, [r1, r2]
	movs r4, #1
	add r9, r4
	adds r6, #28
	cmp r9, r3
	bne .L_081513ce
.L_08151426:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #176
	add r2, r11
	ldr r3, [r2]
	cmp r3, #0
	bne .L_08151438
	movs r3, #1
	str r3, [r2]
.L_08151438:
	movs r0, #8
	movs r1, #8
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
	ldr r5, [sp, #28]
	mov r0, r10
	adds r5, #1
	str r5, [sp, #28]
	ldr r2, .L_081514a0
	ldr r3, [r0, #24]
	ldrb r3, [r2, r3]
	lsrs r3, r3, #1
	adds r3, #132
	cmp r5, r3
	beq .L_0815146e
	b .L_081511b8
.L_0815146e:
	ldr r0, .L_0815149c
	bl Scheduler_RemoveCallback
	ldr r0, .L_08151498
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08151498:
	.4byte Func_0814c928
.L_0815149c:
	.4byte Func_08143000
.L_081514a0:
	.4byte Data_08198300
.L_081514a4:
	.4byte gMapCellBuffer
.L_081514a8:
	.4byte Data_08197410
.L_081514ac:
	.4byte Data_08198316
.L_081514b0:
	.4byte Data_08198303
.L_081514b4:
	.4byte Data_0819830f
.L_081514b8:
	.4byte Data_08198309
