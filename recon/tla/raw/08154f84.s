.syntax unified
	.thumb
	.global Func_08154f84
	.thumb_func
Func_08154f84:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #52]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	ldr r1, [r5, #96]
	mov r9, r0
	movs r0, #1
	str r1, [sp, #48]
	bl Func_081435e0
	ldr r3, .L_08154fe0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08154fe4
	adds r2, #48
	strh r3, [r2]
	ldr r3, .L_08154fe8
	movs r1, #224
	adds r2, #2
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_08154fec
	movs r2, #1
	movs r3, #1
	add r1, r9
	bl Func_08157cf4
	ldr r2, [sp, #52]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08154ff8
	movs r2, #128
	ldr r3, .L_08154ff0
	lsls r2, r2, #19
	b .L_08154ff4
	.2byte 0x0000
.L_08154fe0:
	.4byte 0x00000100
.L_08154fe4:
	.4byte 0x00000000
.L_08154fe8:
	.4byte 0x00001010
.L_08154fec:
	.4byte 0x0000013f
.L_08154ff0:
	.4byte 0xffff9000
.L_08154ff4:
	adds r2, #40
	str r3, [r2]
.L_08154ff8:
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #36]
	bl Func_081963ec
	adds r3, r5, #0
	adds r3, #188
	ldr r3, [r3]
	movs r2, #239
	lsls r2, r2, #7
	str r3, [sp, #40]
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_081550cc
	bl Func_080145a8
	ldr r4, [sp, #52]
	ldr r2, .L_081550d0
	ldr r3, [r4, #24]
	movs r6, #0
	ldrb r3, [r2, r3]
	movs r1, #1
	lsls r3, r3, #3
	adds r3, #56
	str r3, [sp, #32]
	ldr r3, .L_081550d4
	movs r2, #128
	mov r11, r6
	negs r1, r1
	lsls r2, r2, #3
.L_08155050:
	movs r0, #1
	add r11, r0
	str r1, [r3]
	adds r3, #28
	cmp r11, r2
	bne .L_08155050
	ldr r2, [sp, #32]
	movs r1, #0
	str r1, [sp, #44]
	cmp r2, #0
	bne .L_08155068
	b .L_0815533c
.L_08155068:
	ldr r3, [sp, #32]
	subs r2, #64
	subs r3, #16
	str r2, [sp, #28]
	str r3, [sp, #24]
.L_08155072:
	ldr r4, [sp, #44]
	ldr r6, [sp, #28]
	cmp r4, r6
	bne .L_08155080
	movs r0, #133
	bl Func_081180e8
.L_08155080:
	ldr r0, [sp, #44]
	ldr r1, [sp, #24]
	cmp r0, r1
	blt .L_081550a4
	ldr r3, .L_081550c4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r2, [sp, #32]
	movs r1, #128
	subs r3, r2, r0
	ldr r2, .L_081550c8
	lsls r1, r1, #19
	subs r3, #1
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
.L_081550a4:
	ldr r4, [sp, #52]
	movs r3, #0
	mov r11, r3
	ldr r2, [r4, #24]
	ldr r3, .L_081550d0
	ldrb r3, [r3, r2]
	cmp r3, #0
	bne .L_081550b6
	b .L_08155276
.L_081550b6:
	ldr r0, .L_081550d8
	ldr r1, [sp, #44]
	movs r6, #12
	movs r2, #8
	str r6, [sp, #20]
	b .L_081550dc
	.2byte 0x0000
.L_081550c4:
	.4byte 0x00003f44
.L_081550c8:
	.4byte 0x00001000
.L_081550cc:
	.4byte Func_08143000
.L_081550d0:
	.4byte Data_08198442
.L_081550d4:
	.4byte Data_02010018
.L_081550d8:
	.4byte Data_0819843a
.L_081550dc:
	str r0, [sp, #16]
	str r2, [sp, #12]
	subs r1, #8
	mov r10, r1
.L_081550e4:
	ldr r3, [sp, #44]
	ldr r4, [sp, #12]
	cmp r3, r4
	bgt .L_081550ee
	b .L_08155212
.L_081550ee:
	ldr r3, .L_08155360
	mov r6, r11
	ldrb r2, [r3, r6]
	adds r3, r2, #0
	cmp r3, #1
	bhi .L_08155148
	mov r1, r10
	lsls r3, r1, #1
	lsls r0, r1, #4
	add r3, r10
	lsls r1, r3, #1
	cmp r0, #80
	ble .L_0815510a
	movs r0, #80
.L_0815510a:
	cmp r1, #30
	ble .L_08155110
	movs r1, #30
.L_08155110:
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08155126
	ldr r3, [sp, #16]
	movs r4, #108
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r6, [sp, #40]
	subs r2, r2, r1
	b .L_08155132
.L_08155126:
	ldr r3, [sp, #16]
	movs r4, #108
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r6, [sp, #36]
	adds r2, r2, r1
.L_08155132:
	movs r1, #48
	str r1, [sp, #0]
	movs r1, #224
	lsls r1, r1, #3
	subs r3, r4, r0
	str r0, [sp, #4]
	add r1, r9
	ldr r0, [sp, #48]
	mov lr, r6
	.2byte 0xf800
	b .L_081551a4
.L_08155148:
	mov r1, r10
	lsls r0, r1, #3
	cmp r0, #64
	ble .L_08155152
	movs r0, #64
.L_08155152:
	mov r3, r10
	cmp r3, #8
	ble .L_0815515a
	movs r1, #8
.L_0815515a:
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08155184
	ldr r4, [sp, #16]
	movs r6, #108
	movs r2, #0
	ldrsb r2, [r4, r2]
	subs r3, r6, r0
	subs r2, r2, r1
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #176
	lsls r1, r1, #5
	str r0, [sp, #4]
	add r1, r9
	ldr r0, [sp, #48]
	ldr r4, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	b .L_081551a4
.L_08155184:
	ldr r6, [sp, #16]
	ldr r4, [sp, #36]
	movs r2, #0
	ldrsb r2, [r6, r2]
	str r0, [sp, #4]
	adds r2, r2, r1
	movs r1, #108
	subs r3, r1, r0
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #176
	lsls r1, r1, #5
	ldr r0, [sp, #48]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
.L_081551a4:
	ldr r3, [sp, #12]
	ldr r6, [sp, #44]
	adds r3, #1
	cmp r6, r3
	bne .L_081551ba
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #3
	str r3, [r2]
.L_081551ba:
	ldr r3, [sp, #12]
	ldr r0, [sp, #44]
	adds r3, #3
	cmp r0, r3
	bge .L_08155212
	bl Random16
	movs r3, #31
	ands r3, r0
	ldr r7, .L_08155364
	adds r1, r3, #0
	adds r1, #72
	movs r6, #0
	b .L_081551dc
.L_081551d6:
	adds r7, r5, #0
	adds r7, #28
	adds r6, #1
.L_081551dc:
	cmp r6, #64
	beq .L_08155212
	adds r5, r7, #0
	ldr r3, [r5, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_081551d6
	str r1, [sp, #8]
	bl Random16
	ldr r3, [sp, #16]
	ldr r1, [sp, #8]
	movs r2, #0
	ldrsb r2, [r3, r2]
	movs r3, #31
	ands r3, r0
	adds r2, r2, r3
	adds r2, #32
	str r2, [r5]
	cmp r2, #96
	ble .L_0815520c
	movs r3, #96
	str r3, [r5]
.L_0815520c:
	movs r3, #0
	str r1, [r7, #4]
	str r3, [r7, #24]
.L_08155212:
	ldr r4, [sp, #52]
	movs r6, #0
	ldr r3, [r4, #20]
	cmp r3, #0
	beq .L_0815524c
	ldr r0, [sp, #20]
	movs r5, #36
	mov r8, r0
.L_08155222:
	ldr r1, [sp, #44]
	cmp r1, r8
	bne .L_08155244
	movs r0, #133
	bl Audio_PlayCue
	ldr r2, [sp, #52]
	movs r1, #7
	ldrsh r0, [r5, r2]
	movs r3, #3
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r6, #0
	bl Func_0814cd48
	ldr r4, [sp, #52]
	ldr r3, [r4, #20]
.L_08155244:
	adds r6, #1
	adds r5, #2
	cmp r6, r3
	bne .L_08155222
.L_0815524c:
	ldr r6, [sp, #20]
	ldr r0, [sp, #16]
	ldr r2, [sp, #12]
	adds r6, #8
	adds r2, #8
	adds r0, #1
	str r6, [sp, #20]
	str r0, [sp, #16]
	str r2, [sp, #12]
	ldr r4, [sp, #52]
	movs r3, #1
	add r11, r3
	ldr r2, [r4, #24]
	ldr r3, .L_08155368
	movs r1, #8
	ldrb r3, [r3, r2]
	negs r1, r1
	add r10, r1
	cmp r11, r3
	beq .L_08155276
	b .L_081550e4
.L_08155276:
	ldr r0, .L_0815536c
	ldr r7, .L_08155364
	movs r6, #0
	mov r11, r6
	mov r10, r0
.L_08155280:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_08155304
	lsrs r5, r3, #31
	ldr r2, .L_08155370
	adds r5, r3, r5
	asrs r5, r5, #1
	lsls r1, r5, #1
	mov r8, r1
	ldrh r1, [r2, r1]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r9
	adds r1, r1, r3
	mov r3, r10
	ldrb r0, [r3, r5]
	ldr r4, .L_08155374
	lsls r0, r0, #24
	ldrsb r6, [r4, r5]
	ldr r2, [r7]
	asrs r4, r0, #24
	ldr r3, [r7, #4]
	lsrs r0, r0, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r2, r2, r6
	subs r3, r3, r0
	str r6, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r6, .L_08155370
	mov r0, r8
	mov r3, r10
	ldrh r1, [r6, r0]
	ldrb r0, [r3, r5]
	ldr r6, .L_08155374
	lsls r0, r0, #24
	asrs r4, r0, #24
	ldr r3, [r7, #4]
	lsrs r0, r0, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r3, r3, r0
	ldrsb r0, [r6, r5]
	movs r2, #224
	lsls r2, r2, #3
	add r1, r9
	adds r1, r1, r2
	ldr r2, [r7]
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
	cmp r3, #14
	bne .L_08155304
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_08155304:
	movs r6, #1
	add r11, r6
	mov r0, r11
	adds r7, #28
	cmp r0, #64
	bne .L_08155280
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #44]
	ldr r2, [sp, #32]
	adds r1, #1
	str r1, [sp, #44]
	cmp r1, r2
	beq .L_0815533c
	b .L_08155072
.L_0815533c:
	ldr r0, .L_08155378
	bl Func_08014644
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
.L_08155360:
	.4byte Data_08198432
.L_08155364:
	.4byte gMapCellBuffer
.L_08155368:
	.4byte Data_08198442
.L_0815536c:
	.4byte Data_0819844c
.L_08155370:
	.4byte Data_08198454
.L_08155374:
	.4byte Data_08198445
.L_08155378:
	.4byte Func_08143000
