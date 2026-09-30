.syntax unified
	.thumb
	.global Func_08151ff0
	.thumb_func
Func_08151ff0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #40]
	movs r3, #192
	lsls r3, r3, #18
	mov r10, r0
	ldr r0, [r3, #92]
	ldr r3, [r3, #96]
	mov r11, r0
	movs r0, #1
	str r3, [sp, #36]
	bl Func_081435e0
	ldr r3, .L_08152040
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #224
	adds r2, #82
	lsls r1, r1, #3
	strh r3, [r2]
	add r1, r11
	ldr r0, .L_08152044
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r1, [sp, #40]
	cmp r1, #1
	bne .L_0815206a
	movs r2, #0
	movs r0, #160
	mov r8, r2
	lsls r0, r0, #19
	b .L_08152048
.L_08152040:
	.4byte 0x00001010
.L_08152044:
	.4byte 0x00000184
.L_08152048:
	mov r4, r8
	lsrs r3, r4, #31
	add r3, r8
	asrs r3, r3, #1
	lsls r1, r3, #5
	lsls r2, r3, #10
	orrs r2, r1
	movs r1, #1
	orrs r2, r3
	add r8, r1
	strh r2, [r0]
	mov r2, r8
	adds r0, #2
	cmp r2, #64
	bne .L_08152048
	str r1, [sp, #24]
	b .L_08152084
.L_0815206a:
	ldr r0, .L_08152340
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08152344
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	mov r3, r10
	ldr r3, [r3, #24]
	str r3, [sp, #24]
.L_08152084:
	movs r4, #0
	mov r8, r4
	movs r7, #0
	movs r6, #63
	mov r5, r11
.L_0815208e:
	mov r0, r10
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_0815209c
	movs r3, #200
	lsls r3, r3, #14
	b .L_0815209e
.L_0815209c:
	ldr r3, .L_08152348
.L_0815209e:
	str r3, [r5]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #16
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	ands r0, r6
	subs r0, #32
	add r8, r1
	lsls r0, r0, #13
	mov r2, r8
	str r0, [r5, #20]
	str r7, [r5, #24]
	adds r5, #28
	cmp r2, #32
	bne .L_0815208e
	ldr r5, .L_0815234c
	movs r3, #0
	mov r8, r3
	movs r6, #0
	movs r7, #63
.L_081520e0:
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_081520ee
	movs r3, #200
	lsls r3, r3, #14
	b .L_081520f0
.L_081520ee:
	ldr r3, .L_08152348
.L_081520f0:
	str r3, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #8
	lsls r3, r3, #13
	str r3, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #20]
	movs r1, #128
	movs r0, #1
	add r8, r0
	lsls r1, r1, #3
	str r6, [r5, #24]
	adds r5, #28
	cmp r8, r1
	bne .L_081520e0
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r2, #239
	lsls r2, r2, #7
	str r3, [sp, #28]
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08152350
	bl Func_080145a8
	ldr r3, [sp, #24]
	ldr r4, [sp, #24]
	lsls r3, r3, #1
	movs r2, #0
	ldr r0, .L_08152354
	str r3, [sp, #12]
	mov r9, r2
	adds r2, r3, r4
	adds r1, r2, #2
	ldrb r3, [r0, r1]
	cmp r3, #0
	bne .L_08152176
	b .L_081523de
.L_08152176:
	adds r0, r2, #0
	adds r0, #1
	str r2, [sp, #16]
	str r0, [sp, #8]
	str r1, [sp, #20]
.L_08152180:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Func_080156e8
	mov r1, r9
	cmp r1, #2
	bne .L_081521a0
	movs r0, #144
	bl Audio_PlayCue
.L_081521a0:
	ldr r3, [sp, #24]
	ldr r2, [sp, #12]
	ldr r6, .L_08152354
	adds r5, r2, r3
	adds r3, r5, #2
	ldrb r3, [r6, r3]
	subs r3, #48
	cmp r9, r3
	bne .L_081521b8
	movs r0, #133
	bl Func_081180e8
.L_081521b8:
	ldrb r3, [r6, r5]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L_08152264
	ldr r6, .L_0815234c
.L_081521c4:
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L_08152254
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	mov r0, r10
	ldr r2, [r5]
	ldr r3, [r0, #4]
	asrs r2, r2, #1
	lsls r3, r3, #5
	adds r2, r2, r3
	subs r2, #16
	str r2, [r5]
	ldr r2, [r5, #8]
	cmp r2, #159
	bgt .L_081521f0
	movs r3, #160
	str r3, [r5, #8]
	movs r2, #160
.L_081521f0:
	movs r3, #136
	lsls r3, r3, #2
	adds r3, #255
	cmp r2, r3
	ble .L_081521fe
	str r3, [r5, #8]
	adds r2, r3, #0
.L_081521fe:
	adds r3, r2, #0
	subs r3, #160
	cmp r3, #0
	bge .L_08152208
	adds r3, #63
.L_08152208:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	ldr r2, .L_08152358
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	mov r3, r8
	movs r2, #1
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r1, r1, r3
	movs r2, #228
	lsls r2, r2, #6
	add r1, r11
	adds r1, r1, r2
	lsrs r3, r0, #31
	ldr r2, [r5]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #64
	ldr r2, .L_0815235c
	bl BattleFxKernels_IntegrateVector3
.L_08152254:
	ldr r1, .L_08152354
	ldr r2, [sp, #16]
	movs r0, #1
	ldrb r3, [r1, r2]
	add r8, r0
	adds r6, #28
	cmp r8, r3
	bne .L_081521c4
.L_08152264:
	mov r3, r9
	cmp r3, #2
	ble .L_081522f6
	ldr r2, .L_08152354
	ldr r0, [sp, #8]
	movs r4, #0
	ldrb r3, [r2, r0]
	mov r8, r4
	cmp r3, #0
	beq .L_081522f6
	mov r5, r11
.L_0815227a:
	cmp r8, r9
	bge .L_081522e8
	ldr r3, [r5, #4]
	cmp r3, #0
	blt .L_081522e8
	add r6, sp, #44
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_0815e1ec
	mov r1, r10
	ldr r2, [r6]
	ldr r3, [r1, #4]
	asrs r2, r2, #1
	lsls r3, r3, #5
	adds r2, r2, r3
	adds r7, r2, #0
	subs r7, #16
	str r7, [r6]
	ldr r0, [r5, #24]
	cmp r0, #20
	bhi .L_081522d4
	movs r1, #3
	bl Math_Div
	ldr r3, .L_08152360
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r3, .L_08152364
	movs r2, #224
	ldrh r4, [r3, r0]
	ldr r3, [r6, #4]
	lsrs r0, r4, #1
	lsls r2, r2, #3
	add r1, r11
	adds r1, r1, r2
	subs r3, r3, r0
	subs r2, r7, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_081522d4:
	cmp r0, #20
	bgt .L_081522dc
	adds r3, r0, #1
	str r3, [r5, #24]
.L_081522dc:
	ldr r2, .L_0815235c
	adds r0, r5, #0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector3
	ldr r2, .L_08152354
.L_081522e8:
	ldr r1, [sp, #8]
	movs r0, #1
	ldrb r3, [r2, r1]
	add r8, r0
	adds r5, #28
	cmp r8, r3
	bne .L_0815227a
.L_081522f6:
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_08152368
	mov r4, r10
	ldr r2, [r4, #20]
	movs r3, #0
	mov r8, r3
	cmp r2, #0
	beq .L_0815239e
	movs r5, #36
.L_0815230a:
	mov r3, r8
	adds r3, #6
	cmp r9, r3
	bne .L_08152332
	mov r1, r10
	movs r3, #10
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	mov r3, r8
	bl Func_0814cd48
	mov r3, r10
	ldrsh r0, [r5, r3]
	movs r1, #2
	bl Func_08118088
	mov r0, r10
	ldr r2, [r0, #20]
.L_08152332:
	movs r1, #1
	add r8, r1
	adds r5, #2
	cmp r8, r2
	bne .L_0815230a
	b .L_0815239e
	.2byte 0x0000
.L_08152340:
	.4byte 0x00000159
.L_08152344:
	.4byte IwramCopyWords
.L_08152348:
	.4byte 0xffce0000
.L_0815234c:
	.4byte gMapCellBuffer
.L_08152350:
	.4byte Func_08143000
.L_08152354:
	.4byte Data_081983a2
.L_08152358:
	.4byte Data_08197410
.L_0815235c:
	.4byte 0xffffe000
.L_08152360:
	.4byte Data_081983ac
.L_08152364:
	.4byte Data_081983ba
.L_08152368:
	movs r2, #0
	mov r3, r10
	mov r8, r2
	ldr r2, [r3, #20]
	cmp r2, #0
	beq .L_0815239e
	movs r5, #36
.L_08152376:
	mov r3, r8
	adds r3, #6
	cmp r9, r3
	bne .L_08152394
	mov r4, r10
	movs r3, #10
	ldrsh r0, [r5, r4]
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	movs r1, #7
	bl Func_0814cd48
	mov r3, r10
	ldr r2, [r3, #20]
.L_08152394:
	movs r4, #1
	add r8, r4
	adds r5, #2
	cmp r8, r2
	bne .L_08152376
.L_0815239e:
	mov r0, r9
	cmp r0, #2
	bne .L_081523b0
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #6
	str r3, [r2]
.L_081523b0:
	movs r1, #16
	movs r0, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r11
	movs r2, #1
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_081523fc
	ldr r4, [sp, #20]
	movs r1, #1
	ldrb r3, [r2, r4]
	add r9, r1
	cmp r9, r3
	beq .L_081523de
	b .L_08152180
.L_081523de:
	ldr r0, .L_08152400
	bl Func_08014644
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
.L_081523fc:
	.4byte Data_081983a2
.L_08152400:
	.4byte Func_08143000
