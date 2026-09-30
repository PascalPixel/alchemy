.syntax unified
	.thumb
	.global Func_0814bee0
	.thumb_func
Func_0814bee0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	str r0, [sp, #56]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	movs r5, #0
	str r0, [sp, #52]
	ldr r1, [r3, #96]
	str r1, [sp, #48]
	ldr r2, [r3, #92]
	ldr r6, [r3, #100]
	adds r3, #176
	ldr r3, [r3]
	mov r10, r2
	str r3, [sp, #44]
	ldr r3, .L_0814c0d4
	mov r11, r6
	ldrh r3, [r3, #4]
	str r3, [sp, #40]
	bl Func_0813ba50
	movs r0, #0
	bl Func_081435e0
	bl Func_08179e6c
	movs r3, #239
	lsls r3, r3, #7
	add r3, r10
	movs r1, #200
	lsls r1, r1, #4
	str r5, [r3]
	ldr r0, .L_0814c0d8
	bl Func_080145a8
	movs r0, #80
	negs r0, r0
	movs r1, #0
	mov r8, r0
	mov r9, r1
.L_0814bf3e:
	mov r2, r9
	cmp r2, #27
	bne .L_0814c014
	ldr r2, .L_0814c0dc
	movs r3, #240
	str r3, [r2, #16]
	lsls r3, r3, #7
	adds r3, #240
	add r3, r10
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	movs r3, #8
	add r2, r10
	negs r3, r3
	str r3, [r2]
	ldr r6, [sp, #44]
	movs r0, #160
	movs r3, #1
	lsls r0, r0, #19
	movs r1, #128
	str r3, [r6, #16]
	lsls r1, r1, #1
	ldr r3, .L_0814c0e0
	ldr r2, .L_0814c0e4
	adds r0, #192
	mov lr, r3
	.2byte 0xf800
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0814c0e8
	add r1, r10
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0814c0ec
	movs r1, #6
	mov lr, r0
	movs r5, #0
	mov r12, r1
	movs r4, #0
	movs r0, #0
.L_0814bf9a:
	str r0, [sp, #8]
	lsls r2, r4, #1
	movs r7, #0
	add r2, r11
.L_0814bfa2:
	mov r6, lr
	mov r1, r12
	ldrh r3, [r6, r1]
	movs r6, #224
	adds r3, r3, r7
	lsls r6, r6, #3
	adds r3, r3, r6
	mov r1, r10
	ldrb r3, [r1, r3]
	cmp r3, #0
	beq .L_0814bfc2
	ldr r6, [sp, #8]
	subs r3, r3, r6
	cmp r3, #0
	bgt .L_0814bfc2
	movs r3, #1
.L_0814bfc2:
	adds r7, #1
	strb r3, [r2]
	adds r2, #1
	cmp r7, #32
	bne .L_0814bfa2
	adds r5, #1
	adds r4, #16
	adds r0, #7
	cmp r5, #10
	bne .L_0814bf9a
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0814c0f0
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #200
	movs r2, #200
	movs r0, #202
	lsls r1, r1, #7
	lsls r2, r2, #7
	lsls r0, r0, #7
	adds r1, #224
	adds r2, #192
	movs r7, #0
	add r0, r10
	add r1, r10
	add r2, r10
.L_0814bffe:
	adds r3, r7, #0
	adds r3, #16
	adds r7, #1
	strb r3, [r2]
	strb r3, [r1]
	adds r2, #1
	strb r3, [r0]
	adds r1, #1
	adds r0, #1
	cmp r7, #32
	bne .L_0814bffe
.L_0814c014:
	movs r0, #4
	add r8, r0
	movs r5, #128
	movs r2, #240
	mov r1, r8
	lsls r5, r5, #19
	lsls r2, r2, #7
	lsls r3, r1, #8
	adds r5, #40
	adds r2, #232
	str r3, [r5]
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #28
	beq .L_0814c042
	b .L_0814bf3e
.L_0814c042:
	mov r6, sp
	adds r6, #76
	adds r1, r6, #0
	movs r0, #0
	str r6, [sp, #36]
	bl Func_08144aac
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	movs r0, #0
	str r3, [r2]
	str r0, [sp, #16]
	ldr r0, .L_0814c0f4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_0814c0f8
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	add r3, r10
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #146
	movs r2, #1
	movs r0, #12
	bl Func_08152404
	movs r2, #128
	ldr r1, [sp, #16]
	ldr r3, .L_0814c0d0
	lsls r2, r2, #19
	adds r2, #32
	str r1, [r5]
	strh r3, [r2]
	movs r2, #0
	mov r9, r2
.L_0814c0b2:
	ldr r3, .L_0814c0fc
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0814c0c2
	movs r3, #1
	str r3, [sp, #16]
.L_0814c0c2:
	mov r6, r9
	cmp r6, #0
	bne .L_0814c100
	movs r0, #212
	bl Audio_PlayCue
	b .L_0814c100
.L_0814c0d0:
	.4byte 0x00000080
.L_0814c0d4:
	.4byte Data_03001120
.L_0814c0d8:
	.4byte Func_08143000
.L_0814c0dc:
	.4byte gCameraSceneParameters
.L_0814c0e0:
	.4byte IwramFillWords
.L_0814c0e4:
	.4byte 0x7fff7fff
.L_0814c0e8:
	.4byte 0x00000134
.L_0814c0ec:
	.4byte Data_08197410
.L_0814c0f0:
	.4byte 0x000000c5
.L_0814c0f4:
	.4byte 0x00000148
.L_0814c0f8:
	.4byte IwramCopyWords
.L_0814c0fc:
	.4byte gInput
.L_0814c100:
	mov r0, r9
	cmp r0, #32
	bne .L_0814c10c
	movs r0, #191
	bl Audio_PlayCue
.L_0814c10c:
	mov r2, r9
	subs r2, #78
	cmp r2, #31
	bhi .L_0814c12a
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r2, .L_0814c13c
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, .L_0814c140
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	orrs r2, r3
	strh r2, [r1]
.L_0814c12a:
	mov r1, r9
	cmp r1, #140
	bne .L_0814c148
	movs r2, #128
	ldr r3, .L_0814c144
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	b .L_0814c148
.L_0814c13c:
	.4byte 0x00000010
.L_0814c140:
	.4byte 0x00001000
.L_0814c144:
	.4byte 0x00001010
.L_0814c148:
	mov r2, r9
	cmp r2, #157
	bne .L_0814c156
	movs r0, #145
	bl Func_08118088 + 0x60
	b .L_0814c164
.L_0814c156:
	mov r3, r9
	cmp r3, #161
	beq .L_0814c164
	cmp r3, #165
	beq .L_0814c164
	cmp r3, #169
	bne .L_0814c19a
.L_0814c164:
	ldr r6, [sp, #56]
	movs r7, #0
	ldr r3, [r6, #20]
	cmp r3, #0
	beq .L_0814c19a
	movs r5, #36
.L_0814c170:
	ldr r1, [sp, #56]
	ldrsh r0, [r5, r1]
	movs r1, #4
	bl Func_08118088
	ldr r3, [sp, #56]
	movs r2, #1
	ldrsh r0, [r5, r3]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	negs r2, r2
	bl Func_0814cd48
	ldr r0, [sp, #56]
	adds r7, #1
	ldr r3, [r0, #20]
	adds r5, #2
	cmp r7, r3
	bne .L_0814c170
.L_0814c19a:
	mov r1, r9
	cmp r1, #0
	bne .L_0814c26e
	ldr r3, [sp, #44]
	ldr r1, .L_0814c218
	movs r2, #1
	movs r6, #240
	movs r0, #128
	str r2, [r3, #16]
	lsls r6, r6, #16
	lsls r0, r0, #15
	movs r2, #0
	movs r5, #238
	lsls r5, r5, #7
	str r6, [sp, #28]
	str r0, [sp, #32]
	str r1, [sp, #20]
	str r2, [sp, #24]
	adds r5, #220
	movs r7, #0
	add r5, r10
.L_0814c1c4:
	adds r1, r7, #0
	ldmia r5!, {r0}
	adds r1, #12
	adds r7, #1
	bl Animation_ApplyChildArgumentFar
	cmp r7, #12
	bne .L_0814c1c4
	ldr r3, .L_0814c214
	movs r2, #128
	lsls r2, r2, #19
	movs r0, #160
	adds r2, #82
	lsls r0, r0, #19
	movs r1, #128
	strh r3, [r2]
	lsls r1, r1, #1
	ldr r3, .L_0814c21c
	ldr r2, .L_0814c220
	adds r0, #192
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	movs r3, #8
	add r2, r10
	negs r3, r3
	str r3, [r2]
	ldr r6, .L_0814c224
	ldr r3, .L_0814c228
	movs r1, #141
	str r3, [r6, #12]
	movs r3, #120
	str r3, [r6, #16]
	ldr r0, [sp, #40]
	ldr r2, .L_0814c22c
	lsls r1, r1, #3
	b .L_0814c230
	.2byte 0x0000
.L_0814c214:
	.4byte 0x00001010
.L_0814c218:
	.4byte 0xffec0000
.L_0814c21c:
	.4byte IwramFillWords
.L_0814c220:
	.4byte 0x7fff7fff
.L_0814c224:
	.4byte gCameraSceneParameters
.L_0814c228:
	.4byte 0xfffffc38
.L_0814c22c:
	.4byte Data_03001120
.L_0814c230:
	adds r3, r0, r1
	movs r1, #224
	lsls r1, r1, #3
	strh r3, [r2, #4]
	add r1, r10
	movs r2, #1
	movs r3, #1
	ldr r0, .L_0814c2c8
	bl Func_08157cf4
	ldr r0, .L_0814c2cc
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814c2d0
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	str r3, [r2]
.L_0814c26e:
	movs r1, #238
	ldr r6, .L_0814c2d4
	lsls r1, r1, #7
	adds r1, #152
	add r1, r10
	ldr r2, [r1]
	ldr r3, [r6, #12]
	ldr r0, .L_0814c2d8
	subs r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r0, #4]
	ldr r2, [r1]
	adds r1, r0, #0
	adds r3, r3, r2
	strh r3, [r1, #4]
	movs r2, #1
	mov r3, r9
	ands r3, r2
	cmp r3, #0
	bne .L_0814c348
	mov r3, r9
	subs r3, #32
	cmp r3, #111
	bhi .L_0814c348
	ldr r6, [sp, #52]
	movs r0, #206
	lsls r0, r0, #3
	adds r3, r6, r0
	ldrh r0, [r3]
	bl Resource_GetTableEntry
	movs r1, #160
	ldr r2, .L_0814c2c4
	lsls r1, r1, #19
	adds r1, #192
	movs r3, #31
	mov r12, r0
	mov r8, r1
	movs r7, #0
	mov lr, r2
	mov r11, r3
	b .L_0814c2dc
	.2byte 0x0000
.L_0814c2c4:
	.4byte 0x0000001f
.L_0814c2c8:
	.4byte 0x000000c5
.L_0814c2cc:
	.4byte 0x00000148
.L_0814c2d0:
	.4byte IwramCopyWords
.L_0814c2d4:
	.4byte gCameraSceneParameters
.L_0814c2d8:
	.4byte Data_03001120
.L_0814c2dc:
	mov r6, r8
	ldrh r2, [r6]
	mov r5, r11
	lsls r3, r2, #16
	mov r1, lr
	lsrs r0, r3, #26
	lsrs r4, r3, #21
	ands r5, r2
	mov r2, r12
	ands r0, r1
	ands r4, r1
	ldrh r1, [r2]
	mov r6, lr
	lsls r2, r1, #16
	lsrs r3, r2, #26
	ands r3, r6
	subs r6, r3, #2
	lsrs r2, r2, #21
	mov r3, lr
	ands r2, r3
	mov r3, r11
	ands r3, r1
	subs r2, #10
	subs r3, #6
	cmp r6, #0
	bge .L_0814c312
	movs r6, #0
.L_0814c312:
	cmp r2, #0
	bge .L_0814c318
	movs r2, #0
.L_0814c318:
	cmp r3, #0
	bge .L_0814c31e
	movs r3, #0
.L_0814c31e:
	cmp r0, r6
	ble .L_0814c324
	subs r0, #1
.L_0814c324:
	cmp r4, r2
	ble .L_0814c32a
	subs r4, #1
.L_0814c32a:
	cmp r5, r3
	ble .L_0814c330
	subs r5, #1
.L_0814c330:
	lsls r3, r0, #10
	lsls r2, r4, #5
	orrs r3, r2
	mov r6, r8
	movs r0, #2
	orrs r3, r5
	adds r7, #1
	strh r3, [r6]
	add r12, r0
	add r8, r0
	cmp r7, #128
	bne .L_0814c2dc
.L_0814c348:
	mov r3, r9
	subs r3, #64
	cmp r3, #35
	bhi .L_0814c362
	ldr r1, [sp, #20]
	ldr r3, [sp, #24]
	ldr r6, .L_0814c658
	movs r2, #128
	lsls r2, r2, #8
	adds r2, r1, r2
	adds r6, r3, r6
	str r2, [sp, #20]
	str r6, [sp, #24]
.L_0814c362:
	mov r0, r9
	cmp r0, #177
	ble .L_0814c372
	ldr r1, [sp, #20]
	ldr r3, [sp, #24]
	ldr r2, .L_0814c65c
	ldr r6, .L_0814c660
	b .L_0814c394
.L_0814c372:
	mov r0, r9
	cmp r0, #139
	ble .L_0814c384
	ldr r1, [sp, #20]
	ldr r3, [sp, #24]
	ldr r2, .L_0814c664
	movs r6, #128
	lsls r6, r6, #6
	b .L_0814c394
.L_0814c384:
	mov r0, r9
	cmp r0, #99
	ble .L_0814c39c
	ldr r1, [sp, #20]
	ldr r3, [sp, #24]
	ldr r2, .L_0814c668
	movs r6, #128
	lsls r6, r6, #4
.L_0814c394:
	adds r2, r1, r2
	adds r6, r3, r6
	str r2, [sp, #20]
	str r6, [sp, #24]
.L_0814c39c:
	mov r0, r9
	cmp r0, #156
	bne .L_0814c3b0
	movs r1, #240
	ldr r3, .L_0814c66c
	ldr r0, [sp, #48]
	lsls r1, r1, #6
	ldr r2, .L_0814c670
	mov lr, r3
	.2byte 0xf800
.L_0814c3b0:
	mov r1, r9
	cmp r1, #157
	bne .L_0814c402
	movs r1, #224
	lsls r1, r1, #3
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0814c674
	add r1, r10
	bl Func_08157cf4
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #16
	movs r5, #238
	lsls r5, r5, #7
	str r3, [r2]
	adds r5, #220
	movs r7, #0
	add r5, r10
.L_0814c3dc:
	adds r1, r7, #0
	ldmia r5!, {r0}
	adds r1, #24
	adds r7, #1
	bl Animation_ApplyChildArgumentFar
	cmp r7, #11
	bne .L_0814c3dc
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	str r3, [r2]
.L_0814c402:
	mov r3, r9
	subs r3, #156
	cmp r3, #7
	bhi .L_0814c424
	movs r3, #58
	movs r1, #224
	str r3, [sp, #0]
	lsls r1, r1, #3
	movs r3, #120
	str r3, [sp, #4]
	ldr r4, [sp, #76]
	ldr r0, [sp, #48]
	add r1, r10
	movs r2, #12
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
.L_0814c424:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	movs r7, #0
	add r2, r10
	movs r1, #114
.L_0814c430:
	cmp r9, r1
	bne .L_0814c43a
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0814c43a:
	adds r7, #1
	adds r1, #6
	cmp r7, #8
	bne .L_0814c430
	mov r2, r9
	cmp r2, #163
	bne .L_0814c458
	ldr r6, .L_0814c678
	movs r0, #255
	ldrh r3, [r6, #4]
	lsls r0, r0, #8
	adds r0, #236
	adds r3, r3, r0
	adds r1, r6, #0
	strh r3, [r1, #4]
.L_0814c458:
	mov r2, r9
	cmp r2, #167
	bne .L_0814c46e
	ldr r6, .L_0814c678
	movs r0, #255
	ldrh r3, [r6, #4]
	lsls r0, r0, #8
	adds r0, #246
	adds r3, r3, r0
	adds r1, r6, #0
	strh r3, [r1, #4]
.L_0814c46e:
	mov r2, r9
	cmp r2, #171
	bne .L_0814c484
	ldr r6, .L_0814c678
	movs r0, #255
	ldrh r3, [r6, #4]
	lsls r0, r0, #8
	adds r0, #246
	adds r3, r3, r0
	adds r1, r6, #0
	strh r3, [r1, #4]
.L_0814c484:
	mov r2, r9
	cmp r2, #0
	bne .L_0814c4c4
	movs r7, #0
	movs r6, #3
	mov r5, r10
.L_0814c490:
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #160
	str r3, [r5]
	str r6, [r5, #4]
	bl Random16
	movs r3, #1
	ands r0, r3
	str r0, [r5, #24]
	bl Random16
	ldr r3, [r5, #24]
	movs r2, #3
	ands r2, r0
	lsls r3, r3, #3
	adds r2, r2, r3
	adds r2, #8
	adds r7, #1
	str r2, [r5, #12]
	adds r6, #6
	adds r5, #28
	cmp r7, #20
	bne .L_0814c490
.L_0814c4c4:
	mov r6, r9
	cmp r6, #119
	bhi .L_0814c514
	movs r0, #1
	movs r7, #0
	ands r6, r0
	mov r5, r10
.L_0814c4d2:
	cmp r6, #0
	beq .L_0814c4dc
	ldr r1, [sp, #16]
	cmp r1, #0
	bne .L_0814c4fa
.L_0814c4dc:
	movs r1, #32
	ldr r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	ldr r4, [sp, #76]
	ldr r1, [r5, #24]
	ldr r0, [sp, #48]
	adds r1, #1
	str r1, [sp, #4]
	movs r1, #200
	lsls r1, r1, #7
	adds r1, #192
	add r1, r10
	mov lr, r4
	.2byte 0xf800
.L_0814c4fa:
	ldr r2, [r5]
	ldr r3, [r5, #12]
	adds r2, r2, r3
	str r2, [r5]
	cmp r2, #128
	ble .L_0814c50c
	adds r3, r2, #0
	subs r3, #160
	str r3, [r5]
.L_0814c50c:
	adds r7, #1
	adds r5, #28
	cmp r7, #20
	bne .L_0814c4d2
.L_0814c514:
	mov r2, r9
	cmp r2, #195
	bls .L_0814c51c
	b .L_0814c828
.L_0814c51c:
	ldr r3, .L_0814c67c
	add r1, sp, #84
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r6, #1
	str r3, [sp, #68]
	str r4, [sp, #72]
	movs r3, #0
	str r3, [r1, #12]
	str r3, [r1, #4]
	mov r3, r9
	ands r3, r6
	cmp r3, #0
	beq .L_0814c53e
	ldr r0, [sp, #16]
	cmp r0, #0
	bne .L_0814c5d4
.L_0814c53e:
	mov r2, r9
	cmp r2, #156
	ble .L_0814c590
	movs r5, #238
	movs r3, #68
	lsls r5, r5, #7
	ldr r6, .L_0814c680
	add r3, sp
	adds r5, #220
	movs r7, #0
	mov r8, r3
	adds r4, r1, #0
	add r5, r10
.L_0814c558:
	ldrh r3, [r6]
	ldr r0, [sp, #28]
	ldr r1, .L_0814c684
	lsls r3, r3, #16
	adds r3, r3, r0
	adds r3, r3, r1
	str r3, [r4]
	ldrh r3, [r6, #2]
	ldr r2, [sp, #32]
	lsls r3, r3, #16
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r4, #8]
	adds r1, r4, #0
	ldmia r5!, {r0}
	mov r2, r8
	movs r3, #0
	str r4, [sp, #12]
	bl Func_08020010
	adds r7, #1
	adds r6, #4
	ldr r4, [sp, #12]
	cmp r7, #11
	bne .L_0814c558
	b .L_0814c5d4
.L_0814c590:
	movs r6, #238
	movs r2, #68
	lsls r6, r6, #7
	ldr r4, .L_0814c688
	add r2, sp
	adds r6, #220
	movs r7, #0
	mov r8, r2
	adds r5, r1, #0
	add r6, r10
.L_0814c5a4:
	ldrh r3, [r4]
	ldr r0, [sp, #28]
	lsls r3, r3, #16
	adds r3, r3, r0
	str r3, [r5]
	ldrh r3, [r4, #2]
	ldr r1, [sp, #32]
	ldr r2, .L_0814c68c
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	ldmia r6!, {r0}
	adds r1, r5, #0
	mov r2, r8
	movs r3, #0
	str r4, [sp, #12]
	bl Func_08020010
	ldr r4, [sp, #12]
	adds r7, #1
	adds r4, #4
	cmp r7, #12
	bne .L_0814c5a4
.L_0814c5d4:
	mov r3, r9
	cmp r3, #177
	ble .L_0814c602
	ldr r0, [sp, #28]
	ldr r6, [sp, #20]
	ldr r2, [sp, #32]
	adds r6, r6, r0
	str r6, [sp, #28]
	ldr r6, [sp, #20]
	ldr r1, [sp, #24]
	lsls r3, r6, #6
	adds r1, r1, r2
	subs r3, r3, r6
	str r1, [sp, #32]
	cmp r3, #0
	bge .L_0814c5f6
	adds r3, #63
.L_0814c5f6:
	ldr r0, [sp, #24]
	asrs r3, r3, #6
	str r3, [sp, #20]
	lsls r3, r0, #6
	subs r3, r3, r0
	b .L_0814c6be
.L_0814c602:
	mov r1, r9
	cmp r1, #149
	ble .L_0814c628
	ldr r2, [sp, #32]
	ldr r3, .L_0814c690
	cmp r2, r3
	ble .L_0814c616
	movs r6, #0
	str r6, [sp, #20]
	str r6, [sp, #24]
.L_0814c616:
	ldr r1, [sp, #28]
	ldr r3, [sp, #32]
	ldr r0, [sp, #20]
	ldr r2, [sp, #24]
	adds r0, r0, r1
	adds r2, r2, r3
	str r0, [sp, #28]
	str r2, [sp, #32]
	b .L_0814c6c8
.L_0814c628:
	mov r6, r9
	cmp r6, #59
	bgt .L_0814c694
	ldr r3, [sp, #32]
	ldr r2, [sp, #24]
	ldr r6, [sp, #20]
	ldr r1, [sp, #28]
	adds r2, r2, r3
	ldr r0, [sp, #20]
	movs r3, #58
	muls r3, r6
	adds r0, r0, r1
	str r0, [sp, #28]
	str r2, [sp, #32]
	cmp r3, #0
	bge .L_0814c64a
	adds r3, #63
.L_0814c64a:
	ldr r0, [sp, #24]
	asrs r3, r3, #6
	str r3, [sp, #20]
	lsls r3, r0, #5
	subs r3, r3, r0
	lsls r3, r3, #1
	b .L_0814c6be
.L_0814c658:
	.4byte 0xfffff000
.L_0814c65c:
	.4byte 0xfffe0000
.L_0814c660:
	.4byte 0xffff0000
.L_0814c664:
	.4byte 0xffff63c0
.L_0814c668:
	.4byte 0xffffe000
.L_0814c66c:
	.4byte IwramFillWords
.L_0814c670:
	.4byte 0x3f3f3f3f
.L_0814c674:
	.4byte 0x000000c6
.L_0814c678:
	.4byte Data_03001120
.L_0814c67c:
	.4byte Data_08196e0c
.L_0814c680:
	.4byte Data_08197a38
.L_0814c684:
	.4byte 0xffe00000
.L_0814c688:
	.4byte Data_08197a64
.L_0814c68c:
	.4byte 0xfff00000
.L_0814c690:
	.4byte 0x0037ffff
.L_0814c694:
	ldr r6, [sp, #32]
	ldr r3, [sp, #24]
	ldr r0, [sp, #20]
	adds r3, r3, r6
	ldr r2, [sp, #28]
	ldr r1, [sp, #20]
	str r3, [sp, #32]
	movs r3, #58
	muls r3, r0
	adds r1, r1, r2
	str r1, [sp, #28]
	cmp r3, #0
	bge .L_0814c6b0
	adds r3, #63
.L_0814c6b0:
	ldr r1, [sp, #24]
	asrs r3, r3, #6
	str r3, [sp, #20]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	adds r3, r3, r1
.L_0814c6be:
	cmp r3, #0
	bge .L_0814c6c4
	adds r3, #63
.L_0814c6c4:
	asrs r3, r3, #6
	str r3, [sp, #24]
.L_0814c6c8:
	mov r3, r9
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0814c6d8
	ldr r3, [sp, #16]
	cmp r3, #0
	bne .L_0814c71e
.L_0814c6d8:
	mov r6, r9
	cmp r6, #119
	bgt .L_0814c71e
	movs r1, #3
	mov r0, r9
	bl __divsi3
	movs r3, #184
	movs r5, #3
	lsls r3, r3, #5
	ands r5, r0
	adds r3, #112
	adds r1, r5, #0
	muls r1, r3
	movs r0, #224
	ldr r3, [sp, #28]
	lsls r0, r0, #3
	add r1, r10
	adds r1, r1, r0
	movs r0, #60
	str r0, [sp, #0]
	ldr r6, [sp, #32]
	movs r0, #100
	lsrs r2, r3, #31
	str r0, [sp, #4]
	ldr r0, [sp, #36]
	adds r2, r2, r3
	asrs r2, r2, #17
	asrs r3, r6, #16
	ldr r4, [r0, #4]
	subs r2, #12
	subs r3, #58
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_0814c71e:
	mov r1, r9
	cmp r1, #157
	bne .L_0814c730
	ldr r0, .L_0814c8f4
	ldr r1, .L_0814c8f8
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_0814c730:
	mov r3, r9
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0814c740
	ldr r3, [sp, #16]
	cmp r3, #0
	bne .L_0814c828
.L_0814c740:
	mov r6, r9
	cmp r6, #156
	bgt .L_0814c748
	b .L_0814c882
.L_0814c748:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0814c8fc
	ldr r3, [sp, #60]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_0814c900
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	adds r6, r0, #0
	lsls r2, r2, #3
	ldr r0, .L_0814c8f8
	orrs r3, r2
	str r3, [sp, #60]
	add r3, sp, #60
	str r0, [r3, #4]
	str r3, [r6, #16]
	ldr r3, .L_0814c904
	str r1, [r6]
	str r3, [r6, #8]
	mov r1, r11
	movs r3, #0
	str r1, [r6, #12]
	strb r3, [r6, #24]
	strb r3, [r6, #25]
	movs r7, #0
	mov r8, r9
.L_0814c78c:
	ldr r3, .L_0814c908
	ldrb r3, [r3, r7]
	adds r1, r3, #0
	adds r1, #157
	cmp r9, r1
	ble .L_0814c812
	mov r3, r9
	subs r2, r3, r1
	ldr r3, .L_0814c90c
	movs r0, #128
	ldrb r3, [r3, r7]
	lsls r0, r0, #8
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	lsls r3, r3, #4
	mov r2, r9
	adds r5, r3, r0
	subs r3, r1, r2
	lsls r3, r3, #3
	adds r3, #56
	cmp r3, #0
	ble .L_0814c7c0
	movs r3, #0
.L_0814c7c0:
	movs r0, #64
	negs r0, r0
	cmp r3, r0
	ble .L_0814c812
	str r3, [r6, #20]
	bl Func_08014de4
	ldr r3, .L_0814c910
	movs r2, #0
	ldrsb r1, [r3, r7]
	ldr r0, .L_0814c914
	lsls r1, r1, #16
	bl Func_08015160
	movs r1, #3
	lsls r0, r5, #2
	bl __divsi3
	adds r2, r5, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #184
	bl Func_08015024
	mov r1, r8
	movs r3, #7
	ands r3, r1
	lsls r3, r3, #4
	ldr r0, .L_0814c918
	strb r3, [r6, #24]
	mov r1, r11
	movs r2, #32
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0814c812:
	movs r2, #5
	adds r7, #1
	add r8, r2
	cmp r7, #4
	bne .L_0814c78c
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
.L_0814c828:
	mov r3, r9
	cmp r3, #156
	ble .L_0814c882
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #168
	add r6, r10
	ldr r3, [r6]
	cmp r3, #0
	ble .L_0814c86c
	bl Random16
	movs r5, #3
	ands r5, r0
	bl Random16
	movs r3, #15
	ldr r1, .L_0814c91c
	ands r0, r3
	adds r2, r0, #0
	adds r0, #24
	strh r0, [r1, #6]
	ldr r0, .L_0814c920
	movs r3, #120
	subs r5, #8
	subs r2, #8
	subs r5, r3, r5
	subs r3, r3, r2
	str r5, [r0, #12]
	str r3, [r0, #16]
	ldr r3, [r6]
	subs r3, #1
	str r3, [r6]
	b .L_0814c882
.L_0814c86c:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #164
	add r3, r10
	ldr r3, [r3]
	ldr r1, .L_0814c91c
	ldr r2, .L_0814c920
	strh r3, [r1, #6]
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
.L_0814c882:
	bl Func_081434f8
	movs r2, #1
	mov r3, r9
	ands r3, r2
	cmp r3, #0
	beq .L_0814c896
	ldr r3, [sp, #16]
	cmp r3, #0
	bne .L_0814c8a6
.L_0814c896:
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r10
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
.L_0814c8a6:
	movs r6, #1
	add r9, r6
	mov r0, r9
	cmp r0, #200
	beq .L_0814c8b4
	bl .L_0814c0b2
.L_0814c8b4:
	ldr r0, .L_0814c924
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #44]
	movs r3, #0
	movs r5, #238
	lsls r5, r5, #7
	str r3, [r1, #16]
	adds r5, #220
	movs r7, #0
	add r5, r10
.L_0814c8d6:
	ldmia r5!, {r0}
	adds r7, #1
	bl Func_08020040 + 0x8
	cmp r7, #12
	bne .L_0814c8d6
	bl Func_08143bb8
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0814c8f4:
	.4byte 0x000000c1
.L_0814c8f8:
	.4byte Data_02014000
.L_0814c8fc:
	.4byte 0xffffff00
.L_0814c900:
	.4byte 0xffff00ff
.L_0814c904:
	.4byte Data_08198ec4
.L_0814c908:
	.4byte Data_08197a94
.L_0814c90c:
	.4byte Data_08197a98
.L_0814c910:
	.4byte Data_08197a9c
.L_0814c914:
	.4byte 0xfff00000
.L_0814c918:
	.4byte Data_08198cac
.L_0814c91c:
	.4byte Data_03001120
.L_0814c920:
	.4byte gCameraSceneParameters
.L_0814c924:
	.4byte Func_08143000
