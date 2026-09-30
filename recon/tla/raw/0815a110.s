.syntax unified
	.thumb
	.global BattlePres_RunBurstScene
	.thumb_func
BattlePres_RunBurstScene:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #152
	str r0, [sp, #68]
	str r1, [sp, #64]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #60]
	movs r0, #0
	ldr r1, [r3, #96]
	str r1, [sp, #56]
	ldr r2, [r3, #100]
	str r2, [sp, #48]
	ldr r3, [r3, #48]
	str r3, [sp, #44]
	bl Func_081435e0
	ldr r3, .L_0815a178
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r5, [sp, #68]
	ldr r3, [r5, #28]
	cmp r3, #1
	bne .L_0815a162
	ldr r1, [r5, #4]
	movs r3, #71
	lsls r1, r1, #4
	orrs r1, r3
	add r2, sp, #116
	add r3, sp, #104
	adds r0, r5, #0
	bl Func_0815585c
.L_0815a162:
	ldr r1, [sp, #48]
	ldr r0, .L_0815a17c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #60]
	movs r5, #224
	lsls r5, r5, #3
	b .L_0815a180
	.2byte 0x0000
.L_0815a178:
	.4byte 0x00001010
.L_0815a17c:
	.4byte 0x00000134
.L_0815a180:
	adds r1, r2, r5
	ldr r0, .L_0815a4f8
	movs r3, #0
	movs r2, #1
	bl Func_08157cf4
	movs r0, #184
	lsls r0, r0, #6
	movs r1, #144
	movs r3, #0
	adds r0, #136
	lsls r1, r1, #1
	mov r12, r3
	mov lr, r0
	mov r8, r5
	mov r10, r1
	movs r7, #0
	movs r6, #0
.L_0815a1a4:
	ldr r5, [sp, #60]
	mov r2, r8
	adds r3, r7, r2
	movs r0, #0
	adds r4, r6, #0
	adds r1, r3, r5
.L_0815a1b0:
	lsrs r3, r0, #31
	adds r3, r0, r3
	ldr r2, .L_0815a4fc
	asrs r3, r3, #1
	adds r3, r4, r3
	adds r3, r3, r2
	ldrb r2, [r1]
	mov r5, lr
	adds r0, #1
	adds r1, #1
	strb r2, [r3, r5]
	cmp r0, #40
	bne .L_0815a1b0
	movs r0, #1
	add r12, r0
	adds r7, #40
	adds r6, #20
	cmp r12, r10
	bne .L_0815a1a4
	ldr r2, [sp, #60]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815a500
	movs r3, #0
	movs r2, #1
	bl Func_08157cf4
	ldr r5, [sp, #64]
	ldr r3, .L_0815a504
	lsls r5, r5, #3
	str r5, [sp, #40]
	ldrb r3, [r3, r5]
	cmp r3, #0
	bne .L_0815a20a
	ldr r2, [sp, #60]
	movs r3, #156
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, .L_0815a508
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	b .L_0815a21c
.L_0815a20a:
	ldr r5, [sp, #60]
	movs r2, #156
	lsls r2, r2, #5
	adds r1, r5, r2
	ldr r0, .L_0815a50c
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_0815a21c:
	ldr r0, .L_0815a510
	ldr r1, .L_0815a4fc
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0815a514
	ldr r1, .L_0815a518
	bl Func_08157cf4
	ldr r3, [sp, #40]
	ldr r2, .L_0815a504
	adds r3, #1
	ldrb r3, [r2, r3]
	cmp r3, #1
	beq .L_0815a254
	cmp r3, #1
	bgt .L_0815a24a
	cmp r3, #0
	beq .L_0815a250
	b .L_0815a25c
.L_0815a24a:
	cmp r3, #2
	beq .L_0815a258
	b .L_0815a25c
.L_0815a250:
	ldr r0, .L_0815a51c
	b .L_0815a25e
.L_0815a254:
	ldr r0, .L_0815a520
	b .L_0815a25e
.L_0815a258:
	ldr r0, .L_0815a50c
	b .L_0815a25e
.L_0815a25c:
	ldr r0, .L_0815a524
.L_0815a25e:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0815a528
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #64]
	cmp r3, #12
	bne .L_0815a290
	ldr r5, [sp, #68]
	movs r3, #192
	ldr r0, [r5, #8]
	movs r2, #36
	ldrsh r1, [r5, r2]
	lsls r3, r3, #11
	movs r2, #8
	bl Func_08118078
	movs r0, #7
	bl WaitFrames
	b .L_0815a2a0
.L_0815a290:
	ldr r3, [sp, #68]
	movs r2, #4
	ldr r0, [r3, #8]
	movs r5, #36
	ldrsh r1, [r3, r5]
	movs r3, #0
	bl Func_08118078
.L_0815a2a0:
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #68]
	mov r3, sp
	adds r3, #128
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r3, #0
	str r3, [sp, #36]
	bl Func_0815e20c
	ldr r5, [sp, #60]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r5, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r5, r1
	movs r5, #200
	movs r3, #75
	lsls r5, r5, #4
	str r3, [r2]
	ldr r0, .L_0815a52c
	adds r1, r5, #0
	bl Func_080145a8
	ldr r3, [sp, #40]
	ldr r2, .L_0815a504
	adds r3, #7
	ldrb r3, [r2, r3]
	cmp r3, #2
	bne .L_0815a2f0
	ldr r0, .L_0815a530
	adds r1, r5, #0
	bl Func_080145a8
.L_0815a2f0:
	ldr r3, [sp, #68]
	movs r5, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlotFar
	mov r10, r5
	ldr r6, [r0]
	ldr r5, .L_0815a534
	movs r7, #255
.L_0815a304:
	ldr r3, [r6, #8]
	movs r0, #200
	str r3, [r5]
	lsls r0, r0, #13
	ldr r3, [r6, #12]
	adds r3, r3, r0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r7
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ldr r3, [r5]
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	cmp r3, #0
	ble .L_0815a344
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_0815a344:
	movs r3, #1
	movs r1, #1
	movs r2, #128
	negs r3, r3
	add r10, r1
	lsls r2, r2, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r2
	bne .L_0815a304
	ldr r5, [sp, #60]
	movs r3, #0
	mov r10, r3
	movs r7, #0
	movs r6, #255
.L_0815a362:
	ldr r0, [sp, #36]
	ldr r3, [r0]
	str r7, [r5, #8]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #9
	str r0, [r5, #12]
	bl Random16
	mov r1, r10
	ands r0, r6
	movs r2, #1
	negs r3, r1
	subs r0, #128
	add r10, r2
	str r3, [r5, #24]
	lsls r0, r0, #9
	mov r3, r10
	str r0, [r5, #16]
	str r7, [r5, #20]
	adds r5, #28
	cmp r3, #64
	bne .L_0815a362
	movs r5, #0
	ldr r3, [sp, #40]
	str r5, [sp, #52]
	ldr r2, .L_0815a504
	adds r3, #5
	ldrb r3, [r2, r3]
	cmp r3, #0
	bne .L_0815a3b6
	b .L_0815a97e
.L_0815a3b6:
	ldr r2, [sp, #44]
	mov r0, sp
	mov r1, sp
	adds r0, #140
	adds r1, #72
	adds r2, #12
	str r0, [sp, #24]
	str r1, [sp, #28]
	str r2, [sp, #20]
.L_0815a3c8:
	ldr r3, [sp, #60]
	ldr r1, [sp, #52]
	movs r5, #225
	lsls r5, r5, #7
	movs r0, #0
	movs r7, #192
	adds r6, r3, r5
	mov r10, r0
	lsls r7, r7, #11
	lsls r5, r1, #11
.L_0815a3dc:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	subs r3, r7, r3
	asrs r3, r3, #10
	stmia r6!, {r3}
	movs r3, #1
	movs r2, #128
	add r10, r3
	lsls r2, r2, #4
	mov r0, r10
	adds r5, r5, r2
	cmp r0, #160
	bne .L_0815a3dc
	ldr r1, [sp, #64]
	cmp r1, #10
	bne .L_0815a410
	ldr r2, [sp, #52]
	cmp r2, #8
	bne .L_0815a410
	movs r0, #221
	bl Audio_PlayCue
.L_0815a410:
	ldr r3, [sp, #40]
	ldr r2, .L_0815a504
	adds r3, #2
	ldrb r2, [r2, r3]
	ldr r3, [sp, #68]
	str r2, [sp, #32]
	ldr r1, [sp, #24]
	ldr r0, [r3, #8]
	bl Func_0815e20c
	ldr r5, [sp, #24]
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
	ldr r0, [sp, #68]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0815a44a
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #27
	bl Func_081963ec
	b .L_0815a45a
.L_0815a44a:
	movs r1, #23
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #31
	bl Func_081963ec
.L_0815a45a:
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #104]
	movs r2, #192
	str r3, [sp, #72]
	lsls r2, r2, #18
	adds r2, #188
	ldr r3, [r2]
	ldr r5, [sp, #28]
	movs r0, #0
	str r3, [r5, #4]
	ldr r1, [sp, #32]
	mov r10, r0
	cmp r1, #0
	bne .L_0815a47a
	b .L_0815a710
.L_0815a47a:
	ldr r2, [sp, #36]
	str r0, [sp, #16]
	mov r11, r2
.L_0815a480:
	ldr r3, [sp, #40]
	ldr r5, .L_0815a504
	adds r3, #4
	ldrb r3, [r5, r3]
	ldr r1, [sp, #52]
	mov r0, r10
	muls r0, r3
	mov r8, r0
	cmp r1, r8
	bge .L_0815a496
	b .L_0815a5e6
.L_0815a496:
	mov r3, r8
	adds r3, #6
	cmp r1, r3
	blt .L_0815a4a0
	b .L_0815a5e6
.L_0815a4a0:
	movs r3, #3
	mov r5, r10
	ands r3, r5
	subs r2, r1, r0
	cmp r3, #1
	ble .L_0815a4b6
	ldr r0, .L_0815a504
	ldr r1, [sp, #40]
	ldrb r3, [r0, r1]
	cmp r3, #1
	bne .L_0815a570
.L_0815a4b6:
	ldr r5, [sp, #68]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0815a538
	lsls r1, r2, #3
	subs r1, r1, r2
	lsls r1, r1, #2
	subs r1, r1, r2
	ldr r2, [sp, #60]
	mov r5, r11
	lsls r1, r1, #7
	adds r1, r2, r1
	ldr r2, [r5]
	movs r3, #156
	movs r0, #1
	lsls r3, r3, #5
	mov r4, r10
	ands r4, r0
	adds r1, r1, r3
	movs r0, #48
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	ldr r5, [sp, #28]
	lsls r4, r4, #2
	asrs r2, r2, #1
	subs r2, #16
	subs r3, #40
	b .L_0815a5a6
	.2byte 0x0000
.L_0815a4f8:
	.4byte 0x0000015c
.L_0815a4fc:
	.4byte Data_02012400
.L_0815a500:
	.4byte 0x00000192
.L_0815a504:
	.4byte Data_081985b3
.L_0815a508:
	.4byte 0x00000179
.L_0815a50c:
	.4byte 0x0000017a
.L_0815a510:
	.4byte 0x0000012c
.L_0815a514:
	.4byte 0x00000161
.L_0815a518:
	.4byte Data_02013788
.L_0815a51c:
	.4byte 0x00000150
.L_0815a520:
	.4byte 0x00000163
.L_0815a524:
	.4byte 0x00000178
.L_0815a528:
	.4byte IwramCopyWords
.L_0815a52c:
	.4byte Func_08143000
.L_0815a530:
	.4byte Func_08152474
.L_0815a534:
	.4byte gMapCellBuffer
.L_0815a538:
	lsls r1, r2, #3
	subs r1, r1, r2
	lsls r1, r1, #2
	subs r1, r1, r2
	ldr r2, [sp, #60]
	mov r5, r11
	lsls r1, r1, #7
	adds r1, r2, r1
	ldr r2, [r5]
	movs r3, #156
	movs r0, #1
	lsls r3, r3, #5
	mov r4, r10
	ands r4, r0
	adds r1, r1, r3
	movs r0, #48
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	ldr r5, [sp, #28]
	lsls r4, r4, #2
	asrs r2, r2, #1
	subs r2, #32
	subs r3, #40
	b .L_0815a5a6
.L_0815a570:
	ldr r0, [sp, #68]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0815a5b0
	movs r1, #1
	mov r4, r10
	ands r4, r1
	lsls r1, r2, #1
	adds r1, r1, r2
	ldr r3, [sp, #36]
	ldr r2, .L_0815a64c
	lsls r1, r1, #8
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r5, [sp, #24]
	lsrs r3, r2, #31
	movs r0, #48
	adds r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #16
	str r0, [sp, #4]
	ldr r5, [sp, #28]
	lsls r4, r4, #2
	asrs r2, r2, #1
	subs r2, #16
	subs r3, #8
.L_0815a5a6:
	ldr r4, [r4, r5]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	b .L_0815a5e6
.L_0815a5b0:
	lsls r1, r2, #1
	adds r1, r1, r2
	ldr r3, [sp, #36]
	ldr r2, .L_0815a64c
	lsls r1, r1, #8
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r5, [sp, #24]
	movs r0, #1
	mov r4, r10
	ands r4, r0
	lsrs r3, r2, #31
	movs r0, #48
	adds r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #16
	str r0, [sp, #4]
	ldr r5, [sp, #28]
	lsls r4, r4, #2
	asrs r2, r2, #1
	subs r2, #32
	subs r3, #8
	ldr r4, [r4, r5]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
.L_0815a5e6:
	movs r0, #2
	ldr r1, [sp, #52]
	add r0, r8
	mov r9, r0
	cmp r1, r9
	bne .L_0815a6b6
	ldr r3, [sp, #40]
	ldr r2, .L_0815a650
	adds r3, #6
	ldrb r3, [r2, r3]
	cmp r3, #1
	bne .L_0815a60c
	movs r1, #128
	ldr r3, .L_0815a654
	ldr r0, [sp, #56]
	lsls r1, r1, #7
	ldr r2, .L_0815a658
	mov lr, r3
	.2byte 0xf800
.L_0815a60c:
	ldr r5, [sp, #68]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r5, r3]
	movs r2, #5
	movs r3, #0
	movs r5, #4
	str r5, [sp, #0]
	bl Func_0814cd48
	ldr r3, [sp, #32]
	subs r3, #1
	cmp r10, r3
	bne .L_0815a65c
	ldr r2, [sp, #68]
	movs r5, #238
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #4
	bl Func_08118088
	ldr r3, [sp, #60]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	str r3, [r2]
	movs r0, #134
	bl Func_081180e8
	b .L_0815a684
	.2byte 0x0000
.L_0815a64c:
	.4byte Data_02012400
.L_0815a650:
	.4byte Data_081985b3
.L_0815a654:
	.4byte IwramFillWords
.L_0815a658:
	.4byte 0x2f2f2f2f
.L_0815a65c:
	mov r3, r10
	movs r0, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0815a672
	ldr r2, [sp, #68]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #7
	bl Func_08118088
.L_0815a672:
	ldr r0, [sp, #60]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r0, r1
	str r5, [r3]
	movs r0, #134
	bl Audio_PlayCue
.L_0815a684:
	ldr r3, [sp, #40]
	ldr r2, .L_0815a9a8
	adds r3, #3
	ldrb r3, [r2, r3]
	movs r6, #0
	cmp r3, #0
	beq .L_0815a6b6
	ldr r3, [sp, #16]
	ldr r0, .L_0815a9ac
	movs r7, #7
	adds r5, r3, r0
.L_0815a69a:
	str r2, [sp, #12]
	bl Random16
	ands r0, r7
	adds r0, #15
	str r0, [r5]
	ldr r3, [sp, #40]
	ldr r2, [sp, #12]
	adds r3, #3
	ldrb r3, [r2, r3]
	adds r6, #1
	adds r5, #28
	cmp r6, r3
	bne .L_0815a69a
.L_0815a6b6:
	ldr r1, [sp, #52]
	cmp r1, r9
	blt .L_0815a6fa
	mov r3, r8
	adds r3, #14
	cmp r1, r3
	bge .L_0815a6fa
	mov r2, r8
	subs r3, r1, r2
	subs r3, #2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r1, r3, #4
	mov r5, r11
	ldr r2, [r5]
	subs r1, r1, r3
	ldr r3, .L_0815a9b0
	lsls r1, r1, #6
	adds r1, r1, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5, #4]
	movs r0, #20
	asrs r2, r2, #1
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	subs r2, #10
	subs r3, #24
	ldr r4, [sp, #72]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
.L_0815a6fa:
	ldr r1, [sp, #16]
	movs r2, #224
	ldr r5, [sp, #32]
	lsls r2, r2, #2
	movs r3, #1
	adds r1, r1, r2
	add r10, r3
	str r1, [sp, #16]
	cmp r10, r5
	beq .L_0815a710
	b .L_0815a480
.L_0815a710:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08014de4
	ldr r0, [sp, #44]
	ldr r1, [sp, #20]
	bl Func_080156e8
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r1, #19
	movs r0, #188
	bl Func_081963ec
	movs r0, #192
	lsls r0, r0, #18
	ldr r3, [r0, #104]
	movs r1, #192
	str r3, [sp, #72]
	lsls r1, r1, #18
	adds r1, #188
	ldr r3, [r1]
	ldr r2, [sp, #28]
	ldr r6, .L_0815a9b4
	str r3, [r2, #4]
	movs r3, #0
	mov r10, r3
	add r7, sp, #92
.L_0815a754:
	ldr r5, [r6, #24]
	cmp r5, #0
	ble .L_0815a7c8
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r2, [r7]
	mov r1, r10
	lsrs r3, r2, #31
	lsrs r0, r1, #31
	asrs r5, r5, #3
	adds r5, #1
	adds r2, r2, r3
	ldr r1, .L_0815a9b8
	add r0, r10
	lsls r4, r5, #1
	asrs r2, r2, #1
	movs r3, #1
	asrs r0, r0, #1
	str r2, [r7]
	ands r0, r3
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #48]
	lsls r0, r0, #2
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r7, #4]
	str r5, [sp, #0]
	subs r3, r3, r5
	str r4, [sp, #4]
	ldr r5, [sp, #28]
	ldr r4, [r0, r5]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	ldr r2, .L_0815a9bc
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r6, #4]
	ldr r0, .L_0815a9c0
	cmp r3, r0
	bgt .L_0815a7c2
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
.L_0815a7c2:
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_0815a7c8:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #1
	adds r6, #28
	cmp r10, r2
	bne .L_0815a754
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #52]
	cmp r3, #3
	bgt .L_0815a7ea
	b .L_0815a94a
.L_0815a7ea:
	ldr r3, [sp, #40]
	ldr r2, .L_0815a9a8
	adds r3, #7
	ldrb r3, [r2, r3]
	cmp r3, #1
	bne .L_0815a8d2
	ldr r5, [sp, #52]
	cmp r5, #31
	bgt .L_0815a8d2
	ldr r2, [sp, #68]
	add r5, sp, #80
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r7, [sp, #52]
	movs r0, #3
	movs r3, #0
	mov r10, r3
	mov r9, r0
	ands r7, r0
	mov r11, r5
.L_0815a818:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r0, #0
	ands r2, r3
	str r2, [sp, #12]
	bl Random16
	ldr r2, [sp, #12]
	movs r5, #31
	ands r5, r0
	adds r0, r2, #0
	bl Trig_Sin
	mov r1, r11
	ldr r6, [r1]
	adds r5, #4
	lsrs r3, r6, #31
	adds r6, r6, r3
	adds r3, r5, #0
	muls r3, r0
	asrs r6, r6, #1
	asrs r3, r3, #17
	adds r6, r6, r3
	ldr r3, .L_0815a9c4
	ldr r2, [sp, #12]
	mov r8, r3
	ldrb r3, [r3, r7]
	adds r0, r2, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	mov r0, r11
	ldr r4, .L_0815a9c8
	ldr r5, [r0, #4]
	asrs r3, r3, #17
	subs r5, r5, r3
	ldrb r3, [r4, r7]
	str r4, [sp, #8]
	lsrs r3, r3, #1
	subs r5, r5, r3
	bl Random16
	ldr r3, .L_0815a9cc
	mov r1, r9
	ands r0, r1
	ldrb r2, [r3, r0]
	mov r3, r9
	orrs r3, r2
	movs r2, #0
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r2, .L_0815a9d0
	lsls r3, r7, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #60]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	mov r0, r8
	adds r1, r1, r3
	ldrb r3, [r0, r7]
	ldr r4, [sp, #8]
	str r3, [sp, #0]
	movs r2, #192
	ldrb r3, [r4, r7]
	lsls r2, r2, #18
	str r3, [sp, #4]
	adds r2, #188
	adds r3, r5, #0
	ldr r4, [r2]
	ldr r0, [sp, #56]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	add r10, r3
	mov r5, r10
	cmp r5, #3
	bne .L_0815a818
	ldr r2, .L_0815a9a8
.L_0815a8d2:
	ldr r3, [sp, #40]
	adds r3, #7
	ldrb r3, [r2, r3]
	cmp r3, #2
	bne .L_0815a94a
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r0, #192
	lsls r0, r0, #18
	ldr r3, [r0, #104]
	ldr r5, [sp, #60]
	str r3, [sp, #72]
	movs r1, #0
	mov r10, r1
.L_0815a8f2:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_0815a934
	adds r2, r3, #0
	cmp r3, #0
	bge .L_0815a900
	adds r2, r3, #3
.L_0815a900:
	asrs r3, r2, #2
	lsls r1, r3, #3
	ldr r2, .L_0815a9d4
	adds r1, r1, r3
	lsls r1, r1, #7
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	subs r3, #24
	str r0, [sp, #4]
	subs r2, #12
	ldr r4, [sp, #72]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_0815a9d8
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_0815a934:
	movs r1, #1
	add r10, r1
	adds r3, #1
	mov r2, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #16
	bne .L_0815a8f2
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_0815a94a:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r5, #240
	ldr r3, [sp, #60]
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #52]
	ldr r3, [sp, #40]
	adds r0, #1
	str r0, [sp, #52]
	ldr r2, .L_0815a9a8
	adds r3, #5
	ldrb r3, [r2, r3]
	cmp r0, r3
	beq .L_0815a97e
	b .L_0815a3c8
.L_0815a97e:
	ldr r3, [sp, #40]
	adds r3, #7
	ldrb r3, [r2, r3]
	cmp r3, #2
	bne .L_0815a98e
	ldr r0, .L_0815a9dc
	bl Func_08014644
.L_0815a98e:
	ldr r0, .L_0815a9e0
	bl Func_08014644
	bl Func_08143bb8
	add sp, #152
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0815a9a8:
	.4byte Data_081985b3
.L_0815a9ac:
	.4byte Data_02010018
.L_0815a9b0:
	.4byte Data_02015288
.L_0815a9b4:
	.4byte gMapCellBuffer
.L_0815a9b8:
	.4byte Data_08197410
.L_0815a9bc:
	.4byte 0xfffffc00
.L_0815a9c0:
	.4byte 0x0007ffff
.L_0815a9c4:
	.4byte Data_08197492
.L_0815a9c8:
	.4byte Data_08197498
.L_0815a9cc:
	.4byte Data_0819861b
.L_0815a9d0:
	.4byte Data_08197486
.L_0815a9d4:
	.4byte Data_02013788
.L_0815a9d8:
	.4byte 0xfffff000
.L_0815a9dc:
	.4byte Func_08152474
.L_0815a9e0:
	.4byte Func_08143000
