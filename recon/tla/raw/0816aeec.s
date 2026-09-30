.syntax unified
	.thumb
	.global Func_0816aeec
	.thumb_func
Func_0816aeec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r1, [sp, #92]
	str r0, [sp, #96]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #88]
	ldr r3, [r3, #96]
	str r3, [sp, #72]
	bl Func_0813ba50
	movs r0, #1
	bl Func_081435e0
	ldr r1, [sp, #92]
	cmp r1, #0
	bne .L_0816af28
	movs r2, #128
	ldr r3, .L_0816af24
	b .L_0816af2c
	.2byte 0x0000
.L_0816af24:
	.4byte 0x00000c10
.L_0816af28:
	movs r2, #128
	ldr r3, .L_0816af68
.L_0816af2c:
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #88]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r7, [sp, #88]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816af6c
	bl Func_080145a8
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #104]
	ldr r3, [r3, #48]
	b .L_0816af70
.L_0816af68:
	.4byte 0x00001010
.L_0816af6c:
	.4byte Func_08143000
.L_0816af70:
	str r1, [sp, #76]
	str r3, [sp, #68]
	movs r1, #224
	movs r5, #54
	ldrsh r2, [r3, r5]
	lsls r1, r1, #3
	adds r5, r7, r1
	str r2, [sp, #64]
	adds r1, r5, #0
	ldr r0, .L_0816b284
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #176
	lsls r2, r2, #4
	adds r1, r7, r2
	adds r0, r5, #0
	movs r2, #32
	movs r3, #32
	bl Func_0816ae40
	movs r3, #156
	lsls r3, r3, #6
	adds r1, r7, r3
	ldr r0, .L_0816b288
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r5, [sp, #92]
	cmp r5, #1
	bne .L_0816b000
	movs r1, #220
	lsls r1, r1, #6
	adds r5, r7, r1
	ldr r0, .L_0816b28c
	adds r1, r5, #0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #216
	movs r4, #0
	movs r0, #63
	lsls r1, r1, #5
.L_0816afcc:
	ldrb r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0816afe4
	adds r3, #8
	movs r2, #252
	strb r3, [r5]
	lsls r2, r2, #22
	lsls r3, r3, #24
	cmp r3, r2
	bls .L_0816afe4
	strb r0, [r5]
.L_0816afe4:
	adds r4, #1
	adds r5, #1
	cmp r4, r1
	bne .L_0816afcc
	ldr r0, .L_0816b290
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816b294
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0816b000:
	movs r7, #84
	movs r3, #0
	movs r5, #15
	negs r7, r7
	str r3, [sp, #32]
	str r5, [sp, #28]
	str r7, [sp, #24]
	mov r11, r3
	mov r9, r3
.L_0816b012:
	ldr r7, [sp, #32]
	movs r3, #60
	movs r0, #0
	negs r3, r3
	ldr r2, [sp, #28]
	mov r8, r0
	mov r10, r3
	ldr r0, .L_0816b298
	lsls r3, r7, #1
	mov r4, r9
	ldr r1, [sp, #24]
	adds r3, r3, r7
	ldr r7, .L_0816b29c
	lsls r3, r3, #3
	adds r5, r4, #0
	adds r5, #17
	mov r12, r2
	adds r6, r4, #0
	adds r2, r3, r0
	lsls r3, r4, #2
	str r1, [sp, #56]
	str r5, [sp, #8]
	adds r6, #16
	adds r5, r4, #1
	adds r1, r3, r7
.L_0816b044:
	mov r0, r9
	add r0, r8
	mov r3, r8
	mov lr, r0
	lsls r0, r3, #3
	movs r3, #11
	mov r7, r8
	muls r7, r3
	adds r3, r7, #0
	add r7, sp, #56
	subs r3, #82
	ldrb r7, [r7]
	strb r3, [r1]
	movs r3, #60
	strb r3, [r1, #1]
	mov r3, r11
	strb r7, [r1, #2]
	cmp r3, #6
	bgt .L_0816b0a2
	mov r7, r8
	cmp r7, #14
	bgt .L_0816b0a2
	mov r3, r10
	adds r3, #67
	strb r3, [r2, #4]
	strb r3, [r2, #6]
	strb r3, [r2, #18]
	add r3, sp, #8
	ldrb r3, [r3]
	mov r7, r12
	strb r7, [r2, #7]
	strb r0, [r2, #8]
	strb r7, [r2, #9]
	strb r0, [r2, #16]
	strb r0, [r2, #20]
	mov r7, lr
	mov r0, r12
	strb r4, [r2, #5]
	strb r4, [r2, #17]
	strb r4, [r2, #19]
	strb r0, [r2, #21]
	strb r5, [r2]
	strb r3, [r2, #1]
	strb r6, [r2, #2]
	strb r7, [r2, #12]
	strb r5, [r2, #13]
	strb r6, [r2, #14]
.L_0816b0a2:
	ldr r3, [sp, #8]
	movs r7, #1
	movs r0, #8
	add r8, r7
	add r10, r0
	adds r3, #1
	mov r0, r8
	adds r6, #1
	str r3, [sp, #8]
	adds r5, #1
	adds r2, #24
	adds r1, #4
	cmp r0, #16
	bne .L_0816b044
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	add r11, r7
	adds r1, #15
	adds r2, #16
	adds r3, #24
	movs r5, #16
	mov r0, r11
	str r1, [sp, #32]
	str r2, [sp, #28]
	str r3, [sp, #24]
	add r9, r5
	cmp r0, #8
	bne .L_0816b012
	ldr r2, .L_0816b2a0
	movs r3, #0
	strb r3, [r2]
	strb r3, [r2, #1]
	strb r3, [r2, #2]
	ldr r2, [sp, #92]
	movs r1, #48
	str r1, [sp, #60]
	cmp r2, #0
	beq .L_0816b0f4
	movs r3, #81
	str r3, [sp, #60]
.L_0816b0f4:
	ldr r7, [sp, #60]
	movs r5, #0
	str r5, [sp, #84]
	cmp r7, #0
	bne .L_0816b100
	b .L_0816b686
.L_0816b100:
	mov r0, sp
	adds r0, #100
	str r0, [sp, #40]
	str r5, [sp, #36]
.L_0816b108:
	ldr r1, [sp, #84]
	cmp r1, #0
	bne .L_0816b12a
	add r2, sp, #64
	ldrh r2, [r2]
	ldr r3, [sp, #68]
	movs r4, #0
	strh r2, [r3, #54]
	ldr r3, [sp, #88]
	movs r2, #1
	negs r2, r2
	adds r3, #24
.L_0816b120:
	adds r4, #1
	str r2, [r3]
	adds r3, #28
	cmp r4, #64
	bne .L_0816b120
.L_0816b12a:
	ldr r5, [sp, #96]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0816b14e
	ldr r7, [sp, #68]
	movs r1, #184
	movs r0, #54
	ldrsh r3, [r7, r0]
	lsls r1, r1, #6
	adds r1, #223
	ldrh r2, [r7, #54]
	cmp r3, r1
	bgt .L_0816b164
	adds r3, r2, #0
	adds r3, #128
	adds r2, r7, #0
	strh r3, [r2, #54]
	b .L_0816b164
.L_0816b14e:
	ldr r3, [sp, #68]
	ldr r7, .L_0816b2a4
	ldrh r2, [r3, #54]
	movs r5, #54
	ldrsh r3, [r3, r5]
	cmp r3, r7
	blt .L_0816b164
	ldr r0, [sp, #68]
	adds r3, r2, #0
	subs r3, #128
	strh r3, [r0, #54]
.L_0816b164:
	ldr r1, [sp, #92]
	cmp r1, #0
	bne .L_0816b1ae
	ldr r2, [sp, #84]
	cmp r2, #17
	bne .L_0816b260
	movs r0, #144
	bl Func_08118088 + 0x60
	ldr r5, [sp, #96]
	movs r1, #1
	movs r3, #36
	ldrsh r0, [r5, r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r2, #0
	movs r3, #0
	bl Func_0815f000
	movs r3, #16
	movs r7, #36
	ldrsh r0, [r5, r7]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r1, #238
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	b .L_0816b25c
.L_0816b1ae:
	ldr r2, [sp, #84]
	cmp r2, #17
	bne .L_0816b1fe
	movs r0, #145
	bl Audio_PlayCue
	ldr r5, [sp, #96]
	movs r4, #0
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_0816b1f0
	movs r5, #36
.L_0816b1c6:
	ldr r7, [sp, #96]
	str r4, [sp, #16]
	ldrsh r0, [r5, r7]
	movs r1, #0
	bl Func_08118088
	ldr r4, [sp, #16]
	movs r3, #16
	ldrsh r0, [r5, r7]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r4, #0
	bl Func_0814cd48
	ldr r4, [sp, #16]
	ldr r3, [r7, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_0816b1c6
.L_0816b1f0:
	ldr r3, [sp, #88]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r3, r5
	movs r3, #8
	str r3, [r2]
.L_0816b1fe:
	ldr r7, [sp, #84]
	cmp r7, #46
	bne .L_0816b260
	movs r0, #134
	bl Func_08118088 + 0x60
	ldr r0, [sp, #96]
	movs r4, #0
	ldr r3, [r0, #20]
	cmp r3, #0
	beq .L_0816b252
	movs r5, #36
.L_0816b216:
	ldr r1, [sp, #96]
	movs r3, #128
	lsls r3, r3, #12
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	str r4, [sp, #16]
	bl Func_0815f000
	ldr r3, [sp, #96]
	ldr r4, [sp, #16]
	ldrsh r0, [r5, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
	bl Func_0814cd48
	ldr r0, [sp, #96]
	ldr r4, [sp, #16]
	ldr r3, [r0, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_0816b216
.L_0816b252:
	ldr r1, [sp, #88]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
.L_0816b25c:
	movs r3, #8
	str r3, [r2]
.L_0816b260:
	ldr r5, [sp, #92]
	cmp r5, #0
	bne .L_0816b2a8
	ldr r3, [sp, #84]
	subs r3, #33
	cmp r3, #14
	bhi .L_0816b2cc
	ldr r7, [sp, #84]
	ldr r2, .L_0816b27c
	ldr r1, .L_0816b280
	movs r3, #128
	lsls r3, r3, #19
	subs r2, r2, r7
	b .L_0816b2bc
.L_0816b27c:
	.4byte 0x00000030
.L_0816b280:
	.4byte 0x00001000
.L_0816b284:
	.4byte 0x000000bf
.L_0816b288:
	.4byte 0x000000b4
.L_0816b28c:
	.4byte 0x00000161
.L_0816b290:
	.4byte 0x00000193
.L_0816b294:
	.4byte IwramCopyWords
.L_0816b298:
	.4byte Data_02014000
.L_0816b29c:
	.4byte gMapCellBuffer
.L_0816b2a0:
	.4byte Data_020149d8
.L_0816b2a4:
	.4byte 0xffffd121
.L_0816b2a8:
	ldr r3, [sp, #84]
	subs r3, #65
	cmp r3, #14
	bhi .L_0816b2cc
	ldr r0, [sp, #84]
	ldr r2, .L_0816b2c4
	ldr r1, .L_0816b2c8
	movs r3, #128
	lsls r3, r3, #19
	subs r2, r2, r0
.L_0816b2bc:
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	b .L_0816b2cc
.L_0816b2c4:
	.4byte 0x00000050
.L_0816b2c8:
	.4byte 0x00001000
.L_0816b2cc:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #52]
	movs r0, #1
	bl Func_081969f8
	ldr r2, [sp, #92]
	movs r1, #0
	mov r9, r0
	str r1, [sp, #48]
	cmp r2, #0
	bne .L_0816b30e
	ldr r3, [sp, #84]
	cmp r3, #31
	bgt .L_0816b2fc
	lsls r6, r3, #2
	cmp r6, #64
	ble .L_0816b338
	lsls r3, r3, #1
	movs r6, #64
	mov r10, r3
	b .L_0816b34e
.L_0816b2fc:
	ldr r5, [sp, #36]
	movs r3, #160
	lsls r3, r3, #1
	subs r6, r3, r5
	cmp r6, #0
	bge .L_0816b340
	ldr r7, [sp, #84]
	movs r6, #0
	b .L_0816b34a
.L_0816b30e:
	ldr r0, [sp, #84]
	cmp r0, #63
	bgt .L_0816b322
	lsls r0, r0, #1
	mov r10, r0
	mov r6, r10
	cmp r0, #64
	ble .L_0816b34e
	movs r6, #64
	b .L_0816b34e
.L_0816b322:
	ldr r1, [sp, #36]
	movs r3, #144
	lsls r3, r3, #2
	subs r6, r3, r1
	cmp r6, #0
	bge .L_0816b348
	ldr r2, [sp, #84]
	movs r6, #0
	lsls r2, r2, #1
	mov r10, r2
	b .L_0816b34e
.L_0816b338:
	ldr r3, [sp, #84]
	lsls r3, r3, #1
	mov r10, r3
	b .L_0816b34e
.L_0816b340:
	ldr r5, [sp, #84]
	lsls r5, r5, #1
	mov r10, r5
	b .L_0816b34e
.L_0816b348:
	ldr r7, [sp, #84]
.L_0816b34a:
	lsls r7, r7, #1
	mov r10, r7
.L_0816b34e:
	ldr r1, [sp, #84]
	movs r0, #1
	lsls r1, r1, #11
	str r1, [sp, #44]
	mov r11, r0
	movs r1, #64
.L_0816b35a:
	ldr r3, .L_0816b6a4
	movs r2, #0
	mov r8, r2
	adds r7, r1, r3
.L_0816b362:
	mov r5, r8
	movs r3, #16
	subs r3, r3, r5
	adds r5, r6, #0
	muls r5, r3
	cmp r5, #0
	bge .L_0816b372
	adds r5, #31
.L_0816b372:
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #225
	mov r0, r11
	muls r0, r2
	str r1, [sp, #20]
	bl Trig_Sin
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #136
	adds r2, r0, #0
	mov r0, r8
	muls r0, r3
	ldr r3, [sp, #44]
	asrs r5, r5, #5
	adds r0, r3, r0
	adds r0, r0, r2
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	subs r3, r3, r5
	movs r5, #1
	add r8, r5
	adds r3, #64
	mov r0, r8
	strb r3, [r7]
	ldr r1, [sp, #20]
	adds r7, #4
	cmp r0, #16
	bne .L_0816b362
	add r11, r5
	mov r2, r11
	adds r1, #64
	cmp r2, #7
	bne .L_0816b35a
	lsrs r3, r6, #31
	ldr r5, [sp, #88]
	adds r3, r6, r3
	movs r7, #222
	movs r4, #0
	asrs r6, r3, #1
	lsls r7, r7, #4
.L_0816b3cc:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_0816b3f2
	adds r0, r7, #0
	str r4, [sp, #16]
	bl Trig_Sin
	ldr r3, [sp, #44]
	adds r0, r3, r0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #16
	subs r3, r3, r6
	adds r3, #64
	lsls r3, r3, #16
	str r3, [r5, #4]
	ldr r4, [sp, #16]
.L_0816b3f2:
	movs r0, #222
	lsls r0, r0, #2
	adds r4, #1
	adds r7, r7, r0
	adds r5, #28
	cmp r4, #20
	bne .L_0816b3cc
	ldr r3, [sp, #100]
	ldr r2, .L_0816b6a8
	ldr r1, [sp, #88]
	ands r3, r2
	movs r2, #6
	orrs r3, r2
	ldr r2, .L_0816b6ac
	ldr r5, [sp, #40]
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	movs r2, #176
	lsls r2, r2, #4
	str r3, [sp, #100]
	adds r3, r1, r2
	str r3, [r5, #4]
	bl Func_08014de4
	ldr r7, [sp, #92]
	cmp r7, #0
	beq .L_0816b434
	movs r0, #250
	lsls r0, r0, #4
	bl Func_08015024
.L_0816b434:
	ldr r0, [sp, #96]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0816b448
	ldr r2, [sp, #68]
	movs r1, #54
	ldrsh r0, [r2, r1]
	bl Func_08015068
	b .L_0816b458
.L_0816b448:
	ldr r5, [sp, #68]
	movs r7, #128
	movs r3, #54
	ldrsh r0, [r5, r3]
	lsls r7, r7, #8
	adds r0, r0, r7
	bl Func_08015068
.L_0816b458:
	ldr r1, [sp, #36]
	movs r3, #128
	movs r2, #64
	subs r0, r3, r1
	negs r2, r2
	cmp r0, r2
	bge .L_0816b46a
	movs r0, #64
	negs r0, r0
.L_0816b46a:
	lsls r0, r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_08015160
	mov r5, r9
	movs r3, #7
	str r3, [r5]
	ldr r7, [sp, #40]
	ldr r0, .L_0816b6b0
	str r7, [r5, #16]
	str r0, [r5, #8]
	ldr r1, [sp, #52]
	ldr r0, .L_0816b6b4
	str r1, [r5, #12]
	ldr r2, [sp, #48]
	str r2, [r5, #20]
	ldr r3, [sp, #84]
	movs r2, #127
	add r3, r10
	ands r3, r2
	strb r3, [r5, #24]
	ldr r3, [sp, #84]
	bics r2, r3
	strb r2, [r5, #25]
	ldr r1, [sp, #52]
	movs r2, #128
	bl Func_08196958
	mov r0, r9
	bl Func_08196a7c
	ldr r5, [sp, #84]
	cmp r5, #0
	bne .L_0816b4e2
	ldr r7, .L_0816b6b8
	ldr r5, [sp, #88]
	movs r4, #0
	movs r6, #0
.L_0816b4b8:
	str r4, [sp, #16]
	bl Random16
	movs r1, #6
	bl Math_ModU
	ldr r4, [sp, #16]
	ldr r3, .L_0816b6b8
	str r0, [r5, #24]
	movs r0, #192
	lsls r0, r0, #11
	adds r4, #1
	str r7, [r5, #8]
	str r3, [r5]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	adds r7, r7, r0
	adds r5, #28
	cmp r4, #20
	bne .L_0816b4b8
.L_0816b4e2:
	ldr r1, [sp, #92]
	cmp r1, #1
	bne .L_0816b56c
	ldr r7, [sp, #88]
	movs r4, #0
.L_0816b4ec:
	ldr r6, [r7, #24]
	mov r8, r7
	cmp r6, #0
	blt .L_0816b562
	add r5, sp, #120
	adds r0, r7, #0
	adds r1, r5, #0
	str r4, [sp, #16]
	bl Func_0815e1ec
	ldr r4, [sp, #16]
	cmp r0, #0
	bne .L_0816b552
	movs r2, #10
	ldrsh r3, [r5, r2]
	cmp r3, #10
	ble .L_0816b552
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r3, r2, #0
	adds r3, #64
	str r3, [r5]
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #220
	adds r1, r3, #0
	adds r1, #64
	str r1, [r5, #4]
	ldr r5, [sp, #88]
	lsls r1, r6, #3
	adds r1, r1, r6
	lsls r1, r1, #7
	adds r1, r5, r1
	lsls r0, r0, #6
	adds r1, r1, r0
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	adds r2, #52
	adds r3, #40
	ldr r0, [sp, #72]
	ldr r5, [sp, #76]
	mov lr, r5
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #62
	ldr r2, .L_0816b6bc
	bl BattleFxKernels_IntegrateVector3
	ldr r4, [sp, #16]
.L_0816b552:
	mov r0, r8
	ldr r3, [r0, #24]
	adds r3, #1
	str r3, [r0, #24]
	cmp r3, #6
	bne .L_0816b562
	movs r3, #0
	str r3, [r0, #24]
.L_0816b562:
	adds r4, #1
	adds r7, #28
	cmp r4, #20
	bne .L_0816b4ec
	b .L_0816b646
.L_0816b56c:
	ldr r2, [sp, #96]
	add r5, sp, #108
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r7, [sp, #40]
	add r0, sp, #100
	movs r3, #6
	strb r3, [r7]
	str r0, [sp, #40]
	strb r3, [r0, #1]
	ldr r3, .L_0816b6c0
	mov r1, r9
	str r3, [r1, #8]
	ldr r3, [sp, #52]
	movs r2, #0
	str r3, [r1, #12]
	strb r2, [r1, #24]
	strb r2, [r1, #25]
	mov r10, r5
	ldr r5, [sp, #84]
	mov r8, r0
	lsls r3, r5, #1
	ldr r0, .L_0816b6c4
	adds r3, r3, r5
	adds r6, r5, #0
	lsls r3, r3, #12
	movs r5, #128
	movs r4, #0
	adds r7, r3, r0
	subs r6, #16
	lsls r5, r5, #8
.L_0816b5b0:
	cmp r6, #0
	blt .L_0816b63a
	ldr r1, [sp, #88]
	movs r2, #156
	lsls r2, r2, #6
	adds r3, r1, r2
	mov r0, r8
	str r3, [r0, #4]
	cmp r6, #7
	ble .L_0816b5cc
	movs r3, #8
	subs r3, r3, r6
	lsls r3, r3, #2
	str r3, [sp, #48]
.L_0816b5cc:
	ldr r1, [sp, #48]
	movs r3, #64
	mov r2, r9
	negs r3, r3
	str r1, [r2, #20]
	cmp r1, r3
	ble .L_0816b63a
	str r4, [sp, #16]
	bl Func_08014de4
	mov r1, r10
	ldr r0, [r1]
	ldr r1, [r1, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	lsls r1, r1, #9
	adds r0, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r4, [sp, #16]
	movs r2, #128
	lsls r0, r4, #14
	lsls r2, r2, #6
	adds r0, r0, r2
	bl Func_080150e4
	movs r0, #128
	lsls r0, r0, #5
	bl Func_08015024
	ldr r0, [sp, #44]
	bl Func_08015068
	adds r0, r7, #0
	bl Func_0801521c
	ldr r0, .L_0816b6c8
	ldr r1, [sp, #52]
	movs r2, #4
	bl Func_08196958
	mov r0, r9
	bl Func_08196a7c
	ldr r4, [sp, #16]
.L_0816b63a:
	ldr r3, .L_0816b6cc
	adds r4, #1
	adds r7, r7, r3
	subs r6, #4
	cmp r4, #2
	bne .L_0816b5b0
.L_0816b646:
	mov r0, r9
	bl Sys_Free
	ldr r0, [sp, #52]
	bl Sys_Free
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r7, #240
	ldr r5, [sp, #88]
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r5, r7
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #36]
	ldr r1, [sp, #84]
	ldr r2, [sp, #60]
	adds r0, #8
	adds r1, #1
	str r0, [sp, #36]
	str r1, [sp, #84]
	cmp r1, r2
	beq .L_0816b686
	b .L_0816b108
.L_0816b686:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816b6d0
	bl Func_08014644
	bl Func_08143bb8
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816b6a4:
	.4byte Data_02010001
.L_0816b6a8:
	.4byte 0xffffff00
.L_0816b6ac:
	.4byte 0xffff00ff
.L_0816b6b0:
	.4byte Data_02014000
.L_0816b6b4:
	.4byte gMapCellBuffer
.L_0816b6b8:
	.4byte 0xffc40000
.L_0816b6bc:
	.4byte 0xffffe000
.L_0816b6c0:
	.4byte Data_08199340
.L_0816b6c4:
	.4byte 0xfffd0000
.L_0816b6c8:
	.4byte Data_08199210
.L_0816b6cc:
	.4byte 0xffff4000
.L_0816b6d0:
	.4byte Func_08143000
