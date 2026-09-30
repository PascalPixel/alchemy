.syntax unified
	.thumb
	.global Func_08153ebc
	.thumb_func
Func_08153ebc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #128
	movs r5, #192
	str r0, [sp, #64]
	lsls r5, r5, #18
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	ldr r6, .L_08153f3c
	str r3, [sp, #60]
	ldr r3, .L_08153f40
	ldr r0, [r5, #96]
	str r0, [sp, #56]
	movs r0, #128
	ldr r2, [r5, #100]
	ldr r1, [r5, #92]
	str r2, [sp, #40]
	lsls r0, r0, #6
	ldrh r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #36]
	bl Func_081435e0
	ldr r3, .L_08153f34
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	bl Func_0813ba50
	ldr r2, .L_08153f38
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08153f44
	bl Func_080145a8
	movs r1, #0
	movs r0, #0
	bl Func_08163c2c
	movs r3, #240
	lsls r3, r3, #7
	b .L_08153f48
	.2byte 0x0000
.L_08153f34:
	.4byte 0x00000100
.L_08153f38:
	.4byte 0x00000000
.L_08153f3c:
	.4byte gMapCellBuffer
.L_08153f40:
	.4byte Data_03001120
.L_08153f44:
	.4byte Func_08143000
.L_08153f48:
	adds r3, #240
	add r3, r9
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r3, [r5, #36]
	movs r0, #237
	lsls r0, r0, #3
	adds r0, #255
	adds r3, r3, r0
	movs r2, #1
	strb r2, [r3]
	ldr r0, .L_081540d4
	bl Resource_GetTableEntry
	movs r2, #128
	adds r7, r0, #0
	ldr r5, .L_081540d8
	adds r1, r7, #0
	lsls r2, r2, #2
	ldr r0, .L_081540dc
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #2
	adds r7, r7, r1
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0801587c
	movs r7, #238
	movs r2, #0
	ldr r3, .L_081540e0
	lsls r7, r7, #7
	movs r0, #13
	str r2, [sp, #48]
	adds r7, #220
	negs r0, r0
	mov r8, r5
	mov r10, r3
	add r7, r9
	adds r5, r0, #0
.L_08153f9c:
	movs r1, #32
	ldr r2, .L_081540e4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #4
	ands r3, r5
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r7!, {r0}
	lsls r3, r3, #2
	add r3, r10
	ldrh r0, [r3, #2]
	ldr r1, .L_081540e8
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #3
	adds r1, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r3, [sp, #48]
	movs r2, #128
	lsls r2, r2, #3
	adds r3, #1
	adds r6, r6, r2
	str r3, [sp, #48]
	cmp r3, #11
	bne .L_08153f9c
	movs r1, #224
	lsls r1, r1, #3
	add r1, r9
	movs r2, #1
	movs r3, #1
	ldr r0, .L_081540ec
	bl Func_08157cf4
	ldr r0, .L_081540f0
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	adds r1, r7, #0
	ldr r3, .L_081540d8
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	ldr r1, [sp, #40]
	movs r3, #0
	ldr r0, .L_081540f4
	bl Func_08157cf4
	ldr r0, .L_081540f8
	bl Resource_GetTableEntry
	movs r1, #19
	str r0, [sp, #52]
	movs r0, #104
	bl Func_081963ec
	movs r1, #3
	movs r0, #188
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #104]
	adds r3, #188
	str r2, [sp, #68]
	mov r0, sp
	ldr r3, [r3]
	ldr r2, .L_081540fc
	adds r0, #68
	str r0, [sp, #24]
	str r3, [r0, #4]
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_08154100
	movs r0, #1
	movs r2, #0
	bl Func_08118028 + 0x18
	movs r3, #238
	lsls r3, r3, #7
	movs r2, #238
	adds r3, #144
	lsls r2, r2, #7
	movs r1, #0
	add r3, r9
	adds r2, #148
	str r1, [r3]
	add r2, r9
	movs r3, #4
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	add r2, r9
	subs r3, #5
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #156
	add r3, r9
	str r1, [r3]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08154104
	bl Func_080145a8
	ldr r1, [sp, #60]
	movs r3, #1
	str r3, [r1, #16]
	movs r0, #0
	movs r1, #1
	bl Func_08163c2c
	ldr r3, .L_081540c4
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_081540c8
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_081540cc
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_081540d0
	subs r2, #2
	strh r3, [r2]
	movs r2, #0
	movs r3, #255
	lsls r3, r3, #8
	str r2, [sp, #32]
	str r2, [sp, #28]
	str r2, [sp, #48]
	adds r3, #255
	mov r8, r3
	mov r7, r9
	b .L_08154108
	.2byte 0x0000
.L_081540c4:
	.4byte 0x00007741
.L_081540c8:
	.4byte 0x00000080
.L_081540cc:
	.4byte 0x00001010
.L_081540d0:
	.4byte 0x00003f44
.L_081540d4:
	.4byte 0x000000a6
.L_081540d8:
	.4byte IwramCopyWords
.L_081540dc:
	.4byte 0x05000200
.L_081540e0:
	.4byte ResourceTableEntries
.L_081540e4:
	.4byte 0x80002000
.L_081540e8:
	.4byte 0x06010000
.L_081540ec:
	.4byte 0x0000012b
.L_081540f0:
	.4byte 0x00000163
.L_081540f4:
	.4byte 0x00000134
.L_081540f8:
	.4byte 0x00000195
.L_081540fc:
	.4byte gCameraSceneParameters
.L_08154100:
	.4byte 0x00000045
.L_08154104:
	.4byte Func_0813baec
.L_08154108:
	bl Random16
	movs r1, #96
	bl Math_ModU
	adds r0, #12
	lsls r0, r0, #16
	str r0, [r7]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r7, #4]
	movs r3, #0
	str r3, [r7, #12]
	str r3, [r7, #16]
	str r3, [r7, #24]
	ldr r0, [sp, #48]
	mov r11, r3
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r3, r2, #1
	ldr r1, .L_081541dc
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r6, r3, r1
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_081541e0
	lsls r3, r3, #5
	adds r5, r3, r2
.L_0815414a:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #48
	str r3, [r5]
	bl Func_08014de4
	bl Random16
	mov r3, r8
	ands r0, r3
	bl Func_080150e4
	bl Random16
	mov r1, r8
	ands r0, r1
	bl Func_08015024
	bl Random16
	mov r2, r8
	ands r0, r2
	bl Func_08015068
	adds r0, r6, #0
	bl Func_08014e74
	movs r3, #1
	add r11, r3
	mov r0, r11
	adds r5, #28
	adds r6, #48
	cmp r0, #24
	bne .L_0815414a
	ldr r1, [sp, #48]
	adds r7, #28
	adds r1, #1
	str r1, [sp, #48]
	cmp r1, #16
	bne .L_08154108
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #50
	str r3, [r2]
	ldr r3, .L_081541d8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	movs r2, #0
	str r2, [sp, #44]
	ldr r3, .L_081541e4
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081541d0
	b .L_081545ae
.L_081541d0:
	mov r3, sp
	adds r3, #116
	str r3, [sp, #20]
	b .L_081541e8
.L_081541d8:
	.4byte 0x00000784
.L_081541dc:
	.4byte Data_02013800
.L_081541e0:
	.4byte gMapCellBuffer
.L_081541e4:
	.4byte gInput
.L_081541e8:
	ldr r0, [sp, #44]
	cmp r0, #209
	bgt .L_081542b8
	cmp r0, #0
	bne .L_08154212
	ldr r1, [sp, #52]
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r2, [r1, #1]
	lsls r3, r3, #8
	adds r3, r3, r2
	str r3, [sp, #32]
	movs r3, #2
	ldrsb r3, [r1, r3]
	ldrb r2, [r1, #3]
	lsls r3, r3, #8
	adds r3, r3, r2
	adds r1, #4
	str r3, [sp, #28]
	str r1, [sp, #52]
	b .L_0815422c
.L_08154212:
	ldr r2, [sp, #52]
	ldr r0, [sp, #32]
	movs r3, #0
	ldrsb r3, [r2, r3]
	ldr r1, [sp, #28]
	adds r0, r0, r3
	str r0, [sp, #32]
	movs r3, #1
	ldrsb r3, [r2, r3]
	adds r2, #2
	adds r1, r1, r3
	str r1, [sp, #28]
	str r2, [sp, #52]
.L_0815422c:
	add r7, sp, #88
	movs r3, #0
	str r3, [r7, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r7, #4]
	ldr r3, [sp, #28]
	ldr r0, [sp, #32]
	movs r2, #0
	mov r10, r2
	str r2, [sp, #12]
	lsls r0, r0, #16
	lsls r2, r3, #16
	movs r3, #128
	lsls r3, r3, #15
	str r0, [sp, #16]
	subs r2, r3, r2
	mov r8, r2
.L_08154250:
	ldr r1, [sp, #16]
	movs r2, #160
	ldr r0, [sp, #12]
	lsls r2, r2, #15
	adds r5, r1, r2
	movs r1, #238
	lsls r3, r0, #2
	lsls r1, r1, #7
	add r3, r9
	adds r1, #220
	movs r4, #0
	mov r11, r8
	adds r6, r3, r1
.L_0815426a:
	mov r2, r10
	cmp r2, #3
	bne .L_08154274
	cmp r4, #2
	beq .L_08154298
.L_08154274:
	mov r3, r11
	mov r0, r10
	str r5, [sp, #88]
	str r3, [sp, #96]
	cmp r0, #3
	bne .L_08154288
	movs r1, #128
	lsls r1, r1, #14
	adds r3, r5, r1
	str r3, [r7]
.L_08154288:
	ldr r0, [r6]
	adds r1, r7, #0
	ldr r2, .L_081544ec
	movs r3, #0
	str r4, [sp, #8]
	bl Func_08020010
	ldr r4, [sp, #8]
.L_08154298:
	movs r2, #128
	lsls r2, r2, #14
	adds r4, #1
	adds r5, r5, r2
	adds r6, #4
	cmp r4, #3
	bne .L_0815426a
	ldr r3, [sp, #12]
	movs r0, #1
	add r10, r0
	adds r3, #3
	mov r1, r10
	str r3, [sp, #12]
	add r8, r2
	cmp r1, #4
	bne .L_08154250
.L_081542b8:
	ldr r2, [sp, #20]
	movs r1, #0
	str r1, [r2, #4]
	str r1, [r2, #8]
	ldr r3, [sp, #44]
	cmp r3, #48
	bne .L_081542dc
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #180
	add r3, r9
	movs r2, #24
	str r2, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #184
	add r3, r9
	str r1, [r3]
.L_081542dc:
	movs r0, #0
	str r0, [sp, #48]
.L_081542e0:
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
	lsls r6, r1, #3
	adds r7, r6, #0
	adds r7, #64
	cmp r2, r7
	bge .L_081542f0
	b .L_08154578
.L_081542f0:
	subs r3, r6, r1
	lsls r3, r3, #2
	mov r0, r9
	adds r5, r0, r3
	movs r2, #2
	ldrsh r1, [r5, r2]
	movs r0, #6
	ldrsh r3, [r5, r0]
	mov r8, r1
	ldr r1, [sp, #44]
	mov r10, r3
	adds r3, r6, #0
	adds r3, #84
	cmp r1, r3
	bne .L_08154314
	movs r0, #212
	bl Audio_PlayCue
.L_08154314:
	ldr r2, [sp, #44]
	adds r3, r6, #0
	adds r3, #85
	cmp r2, r3
	blt .L_08154394
	ldr r1, [r5, #12]
	ldr r3, [r5]
	ldr r2, [r5, #16]
	adds r3, r3, r1
	str r3, [r5]
	ldr r3, [r5, #4]
	movs r0, #128
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, .L_081544f0
	lsls r0, r0, #10
	adds r1, r1, r3
	str r1, [r5, #12]
	movs r1, #232
	adds r2, r2, r0
	lsls r1, r1, #5
	str r2, [r5, #16]
	movs r0, #16
	movs r5, #21
	adds r1, #172
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	ldr r4, [sp, #68]
	add r1, r9
	adds r2, #4
	subs r3, #40
	str r5, [sp, #4]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #240
	movs r0, #29
	lsls r1, r1, #5
	str r0, [sp, #0]
	adds r1, #252
	movs r0, #35
	mov r2, r8
	mov r3, r10
	add r1, r9
	subs r2, #16
	subs r3, #19
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #136
	lsls r1, r1, #6
	movs r0, #24
	adds r1, #243
	mov r2, r8
	mov r3, r10
	str r0, [sp, #4]
	add r1, r9
	subs r2, #20
	adds r3, #16
	str r5, [sp, #0]
	b .L_081544e0
.L_08154394:
	ldr r1, [sp, #44]
	adds r3, r6, #0
	adds r3, #80
	cmp r1, r3
	bge .L_081543a0
	b .L_081544f8
.L_081543a0:
	subs r3, r1, r7
	subs r3, #16
	cmp r3, #4
	bls .L_081543aa
	b .L_08154578
.L_081543aa:
	ldr r2, .L_081544f4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_081543b4:
	.4byte .L_081543c8
	.4byte .L_081543e0
	.4byte .L_081543fa
	.4byte .L_08154434
	.4byte .L_0815448c
.L_081543c8:
	movs r0, #14
	movs r1, #224
	lsls r1, r1, #3
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #28
	str r0, [sp, #4]
	add r1, r9
	subs r2, #7
	subs r3, #14
	b .L_081544e0
.L_081543e0:
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #4
	adds r1, #136
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #44
	str r0, [sp, #4]
	add r1, r9
	subs r2, #11
	subs r3, #22
	b .L_081544e0
.L_081543fa:
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #4
	str r0, [sp, #0]
	adds r1, #124
	movs r0, #30
	mov r2, r8
	mov r3, r10
	add r1, r9
	subs r2, #4
	subs r3, #31
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #224
	movs r0, #22
	lsls r1, r1, #4
	adds r1, #212
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #33
	str r0, [sp, #4]
	add r1, r9
	subs r2, #16
	subs r3, #1
	b .L_081544e0
.L_08154434:
	movs r1, #136
	movs r0, #18
	lsls r1, r1, #5
	str r0, [sp, #0]
	adds r1, #170
	movs r0, #27
	mov r2, r8
	mov r3, r10
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	add r1, r9
	adds r2, #1
	subs r3, #38
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #152
	lsls r1, r1, #5
	movs r0, #22
	adds r1, #144
	mov r2, r8
	mov r3, r10
	add r1, r9
	subs r2, #11
	subs r3, #11
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #168
	movs r0, #19
	lsls r1, r1, #5
	adds r1, #116
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #28
	str r0, [sp, #4]
	add r1, r9
	subs r2, #19
	adds r3, #11
	b .L_081544e0
.L_0815448c:
	movs r1, #184
	lsls r1, r1, #5
	movs r5, #23
	movs r0, #16
	adds r1, #136
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #68]
	add r1, r9
	adds r2, #4
	subs r3, #40
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #192
	lsls r1, r1, #5
	str r5, [sp, #0]
	adds r1, #248
	movs r5, #28
	mov r2, r8
	mov r3, r10
	add r1, r9
	subs r2, #10
	subs r3, #17
	ldr r4, [sp, #68]
	str r5, [sp, #4]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #216
	lsls r1, r1, #5
	movs r0, #20
	adds r1, #124
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	add r1, r9
	subs r2, #20
	adds r3, #11
	str r5, [sp, #4]
.L_081544e0:
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	b .L_08154578
	.2byte 0x0000
.L_081544ec:
	.4byte Data_081983cc
.L_081544f0:
	.4byte 0xffff0000
.L_081544f4:
	.4byte .L_081543b4
.L_081544f8:
	ldr r3, [sp, #48]
	movs r2, #0
	mov r11, r2
	lsls r2, r3, #1
	adds r3, r2, r3
	lsls r2, r3, #1
	ldr r0, .L_0815463c
	adds r2, r2, r3
	lsls r2, r2, #7
	adds r7, r2, r0
	ldr r1, .L_08154640
	lsls r2, r3, #3
	subs r2, r2, r3
	lsls r2, r2, #5
	add r6, sp, #104
	adds r5, r2, r1
.L_08154518:
	ldr r3, [r5]
	cmp r3, #0
	ble .L_0815456a
	adds r0, r7, #0
	bl Func_08014e90
	ldr r3, [r5]
	ldr r2, [sp, #20]
	adds r1, r6, #0
	str r3, [r2]
	ldr r0, [sp, #20]
	bl Func_0815e1ec
	ldr r3, [r6]
	movs r0, #5
	asrs r3, r3, #1
	add r3, r8
	str r3, [r6]
	ldr r3, [r6, #4]
	add r3, r10
	adds r3, #16
	str r3, [r6, #4]
	ldr r3, [r5]
	subs r3, #4
	str r3, [r5]
	ldr r3, .L_08154644
	ldr r2, [r6]
	ldrh r1, [r3, #8]
	ldr r3, [sp, #40]
	subs r2, #2
	adds r1, r3, r1
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #10
	str r0, [sp, #4]
	ldr r0, [sp, #24]
	subs r3, #5
	ldr r4, [r0, #4]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
.L_0815456a:
	movs r1, #1
	add r11, r1
	mov r2, r11
	adds r7, #48
	adds r5, #28
	cmp r2, #24
	bne .L_08154518
.L_08154578:
	ldr r3, [sp, #48]
	adds r3, #1
	str r3, [sp, #48]
	cmp r3, #16
	beq .L_08154584
	b .L_081542e0
.L_08154584:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #44]
	adds r0, #1
	str r0, [sp, #44]
	cmp r0, #220
	beq .L_081545ae
	ldr r3, .L_08154648
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_081545ae
	b .L_081541e8
.L_081545ae:
	ldr r0, .L_0815464c
	bl Func_08014644
	ldr r1, [sp, #60]
	add r2, sp, #36
	movs r3, #0
	str r3, [r1, #16]
	ldrh r2, [r2]
	ldr r3, .L_08154650
	strh r2, [r3, #4]
	bl Func_0814cca8
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08154654
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08154658
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	movs r5, #238
	movs r3, #0
	lsls r5, r5, #7
	str r3, [sp, #48]
	adds r5, #220
	add r5, r9
.L_081545fa:
	ldmia r5!, {r0}
	bl Func_08020040 + 0x8
	ldr r0, [sp, #48]
	adds r0, #1
	str r0, [sp, #48]
	cmp r0, #11
	bne .L_081545fa
	ldr r3, .L_08154634
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08154638
	subs r2, #32
	strh r3, [r2]
	ldr r1, .L_08154640
	ldr r0, .L_0815465c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #0
	str r1, [sp, #48]
	add r6, sp, #76
	mov r5, r9
	movs r7, #31
	b .L_08154660
	.2byte 0x0000
.L_08154634:
	.4byte 0x00000080
.L_08154638:
	.4byte 0x00007741
.L_0815463c:
	.4byte Data_02013800
.L_08154640:
	.4byte gMapCellBuffer
.L_08154644:
	.4byte Data_08197410
.L_08154648:
	.4byte gInput
.L_0815464c:
	.4byte Func_0813baec
.L_08154650:
	.4byte Data_03001120
.L_08154654:
	.4byte 0x05000200
.L_08154658:
	.4byte 0x050001e8
.L_0815465c:
	.4byte 0x00000178
.L_08154660:
	ldr r0, [sp, #48]
	movs r1, #6
	bl Math_Mod
	ldr r2, [sp, #64]
	ldr r3, [r2, #20]
	cmp r0, r3
	bge .L_081546a0
	lsls r3, r0, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	adds r1, r6, #0
	bl Func_0815e20c
	bl Random16
	ands r0, r7
	adds r0, #40
	negs r0, r0
	str r0, [r5, #4]
	ldr r1, [r6]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #80
	subs r3, r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r1, #1
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [r5]
	b .L_081546b8
.L_081546a0:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #80
	str r3, [r5]
	bl Random16
	ands r0, r7
	adds r0, #40
	negs r0, r0
	str r0, [r5, #4]
.L_081546b8:
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
	ldr r2, [sp, #48]
	adds r5, #28
	adds r2, #1
	str r2, [sp, #48]
	cmp r2, #32
	bne .L_08154660
	movs r3, #0
	str r3, [sp, #44]
.L_081546ce:
	movs r0, #0
	str r0, [sp, #48]
	mov r7, r9
.L_081546d4:
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
	lsls r3, r1, #1
	cmp r2, r3
	bge .L_081546e4
	cmp r2, #40
	bgt .L_081546e4
	b .L_08154844
.L_081546e4:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_08154780
	cmp r3, #23
	bgt .L_0815477a
	adds r6, r3, #0
	cmp r3, #0
	bge .L_081546f6
	adds r6, r3, #3
.L_081546f6:
	ldr r3, [sp, #48]
	ldr r2, .L_081548a0
	asrs r6, r6, #2
	movs r4, #1
	ands r4, r3
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_081548a4
	ldr r2, [r7]
	ldrb r5, [r3, r6]
	ldr r0, .L_081548a8
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_081548ac
	adds r1, r1, r0
	ldrb r0, [r3, r6]
	ldr r3, [r7, #4]
	str r5, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_081548b0
	lsls r4, r4, #2
	ldrb r0, [r0, r6]
	subs r3, #40
	str r0, [sp, #4]
	ldr r0, [sp, #24]
	subs r2, #8
	ldr r4, [r4, r0]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #24]
	cmp r3, #11
	bgt .L_0815477a
	movs r1, #16
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #21
	str r1, [sp, #4]
	movs r1, #232
	lsls r1, r1, #5
	adds r1, #172
	ldr r4, [sp, #68]
	adds r2, #4
	subs r3, #40
	ldr r0, [sp, #56]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
	movs r1, #29
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #35
	str r1, [sp, #4]
	movs r1, #240
	lsls r1, r1, #5
	adds r1, #252
	subs r3, #19
	subs r2, #16
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #24]
.L_0815477a:
	adds r3, #1
	str r3, [r7, #24]
	b .L_08154844
.L_08154780:
	ldr r1, [r7, #4]
	movs r5, #24
	cmp r1, #56
	ble .L_0815478e
	subs r3, r5, r1
	adds r5, r3, #0
	adds r5, #56
.L_0815478e:
	adds r3, r1, #0
	movs r1, #16
	ldr r2, [r7]
	str r1, [sp, #0]
	movs r1, #232
	lsls r1, r1, #5
	movs r6, #21
	adds r1, #172
	adds r2, #4
	subs r3, #40
	ldr r4, [sp, #68]
	add r1, r9
	str r6, [sp, #4]
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r1, #29
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #35
	str r1, [sp, #4]
	movs r1, #240
	lsls r1, r1, #5
	adds r1, #252
	subs r2, #16
	subs r3, #19
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
	cmp r5, #0
	ble .L_081547ee
	ldr r2, [r7]
	ldr r3, [r7, #4]
	movs r1, #136
	lsls r1, r1, #6
	adds r1, #243
	subs r2, #20
	adds r3, #16
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #56]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
.L_081547ee:
	ldr r3, [r7]
	subs r3, #6
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #12
	str r3, [r7, #4]
	cmp r3, #79
	ble .L_08154844
	movs r2, #238
	lsls r2, r2, #7
	movs r3, #0
	adds r2, #168
	str r3, [r7, #24]
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r0, #134
	bl Audio_PlayCue
	movs r1, #6
	ldr r0, [sp, #48]
	bl Math_Mod
	ldr r1, [sp, #64]
	adds r4, r0, #0
	ldr r3, [r1, #20]
	cmp r4, r3
	bge .L_08154844
	lsls r5, r4, #1
	adds r5, #36
	movs r3, #8
	ldrsh r0, [r1, r5]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r4, #0
	bl Func_0814cd48
	ldr r3, [sp, #64]
	ldrsh r0, [r3, r5]
	movs r1, #1
	bl Func_08118088
.L_08154844:
	ldr r2, [sp, #48]
	adds r7, #28
	adds r2, #1
	str r2, [sp, #48]
	cmp r2, #24
	beq .L_08154852
	b .L_081546d4
.L_08154852:
	movs r0, #4
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #44]
	adds r3, #1
	str r3, [sp, #44]
	cmp r3, #88
	beq .L_0815487c
	b .L_081546ce
.L_0815487c:
	ldr r0, .L_081548b4
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #128
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081548a0:
	.4byte Data_0819747a
.L_081548a4:
	.4byte Data_08197467
.L_081548a8:
	.4byte gMapCellBuffer
.L_081548ac:
	.4byte Data_08197473
.L_081548b0:
	.4byte Data_0819746d
.L_081548b4:
	.4byte Func_08143000
