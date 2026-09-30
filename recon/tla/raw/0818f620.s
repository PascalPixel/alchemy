.syntax unified
	.thumb
	.global Func_0818f620
	.thumb_func
Func_0818f620:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r1, [sp, #56]
	str r0, [sp, #60]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r2, [sp, #56]
	str r0, [sp, #52]
	ldr r1, [r3, #96]
	str r1, [sp, #48]
	ldr r3, [r3, #100]
	str r3, [sp, #32]
	cmp r2, #1
	bhi .L_0818f652
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	b .L_0818f658
.L_0818f652:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
.L_0818f658:
	ldr r2, .L_0818f688
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r4, #224
	str r3, [sp, #36]
	ldr r3, [sp, #52]
	ldr r6, [sp, #56]
	lsls r4, r4, #3
	adds r4, r3, r4
	movs r5, #78
	str r4, [sp, #28]
	str r5, [sp, #24]
	cmp r6, #0
	beq .L_0818f69a
	b .L_0818f68c
.L_0818f688:
	.4byte 0x00001010
.L_0818f68c:
	ldr r0, [sp, #56]
	movs r7, #58
	str r7, [sp, #24]
	cmp r0, #3
	beq .L_0818f69a
	movs r1, #66
	str r1, [sp, #24]
.L_0818f69a:
	ldr r3, [sp, #52]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r5, [sp, #52]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #132
	adds r2, r5, r6
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818f940
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_0818f944
	ldr r1, [sp, #32]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r0, #0
	movs r7, #0
	mov r8, r0
	movs r6, #2
	movs r5, #0
.L_0818f6d4:
	ldr r1, .L_0818f948
	movs r2, #167
	lsls r2, r2, #9
	adds r0, r7, r1
	adds r2, #32
	adds r1, r6, #0
	bl Func_0815b434
	adds r3, r5, #3
	muls r3, r6
	ldr r2, [sp, #28]
	adds r6, #2
	strh r7, [r5, r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r7, r7, r3
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #2
	cmp r4, #32
	bne .L_0818f6d4
	ldr r5, [sp, #56]
	cmp r5, #3
	bne .L_0818f71e
	ldr r0, .L_0818f94c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818f950
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0818f732
.L_0818f71e:
	ldr r0, .L_0818f954
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818f950
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0818f732:
	ldr r6, [sp, #52]
	movs r7, #240
	lsls r7, r7, #4
	ldr r0, .L_0818f958
	adds r1, r6, r7
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r1, [sp, #24]
	movs r0, #0
	str r0, [sp, #44]
	cmp r1, #0
	bne .L_0818f750
	b .L_0818fda0
.L_0818f750:
	movs r4, #192
	lsls r4, r4, #3
	mov r2, sp
	mov r3, sp
	adds r4, #228
	adds r2, #76
	adds r3, #64
	adds r4, r6, r4
	str r2, [sp, #12]
	str r3, [sp, #16]
	str r4, [sp, #20]
.L_0818f766:
	ldr r5, [sp, #60]
	ldr r1, [sp, #12]
	ldr r0, [r5, #8]
	bl Func_0815e21c
	ldr r6, [sp, #56]
	cmp r6, #3
	bne .L_0818f77e
	ldr r0, [sp, #16]
	bl Func_0815e22c
	b .L_0818f78a
.L_0818f77e:
	ldr r1, [sp, #60]
	movs r7, #36
	ldrsh r0, [r1, r7]
	ldr r1, [sp, #16]
	bl Func_0815e21c
.L_0818f78a:
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_0818f79c
	ldr r3, [sp, #44]
	cmp r3, #0
	bne .L_0818f814
	movs r0, #140
	bl Audio_PlayCue
.L_0818f79c:
	ldr r4, [sp, #44]
	cmp r4, #0
	bne .L_0818f814
	ldr r5, [sp, #56]
	cmp r5, #0
	bne .L_0818f7ce
	ldr r5, [sp, #52]
	movs r6, #0
	mov r8, r6
	movs r6, #31
.L_0818f7b0:
	mov r7, r8
	lsls r3, r7, #12
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	adds r0, #16
	str r0, [r5, #24]
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #16
	bne .L_0818f7b0
	b .L_0818f814
.L_0818f7ce:
	ldr r2, [sp, #12]
	ldr r0, [r2]
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [sp, #20]
	asrs r0, r0, #1
	lsls r0, r0, #16
	str r0, [r3]
	ldr r4, [r2, #4]
	subs r4, #36
	lsls r4, r4, #16
	str r4, [r3, #4]
	ldr r5, [sp, #16]
	ldr r3, [r2]
	ldr r1, [r5]
	ldr r6, [sp, #20]
	subs r1, r1, r3
	lsls r5, r1, #10
	str r5, [r6, #12]
	ldr r7, [sp, #16]
	ldr r6, [sp, #12]
	ldr r2, [r7, #4]
	ldr r3, [r6, #4]
	ldr r7, [sp, #20]
	subs r2, r2, r3
	lsls r3, r2, #10
	lsls r1, r1, #13
	lsls r2, r2, #13
	subs r1, r1, r5
	subs r2, r2, r3
	adds r0, r0, r1
	adds r4, r4, r2
	str r3, [r7, #16]
	str r0, [r7]
	str r4, [r7, #4]
.L_0818f814:
	ldr r0, [sp, #56]
	cmp r0, #0
	bne .L_0818f918
	ldr r3, [sp, #44]
	subs r3, #18
	cmp r3, #36
	bls .L_0818f824
	b .L_0818fa44
.L_0818f824:
	ldr r1, [sp, #44]
	lsls r3, r1, #1
	adds r7, r3, #0
	subs r7, #32
	cmp r7, #64
	ble .L_0818f832
	movs r7, #64
.L_0818f832:
	ldr r2, [sp, #16]
	movs r4, #32
	ldr r6, [r2]
	negs r4, r4
	lsrs r3, r6, #31
	adds r6, r6, r3
	ldr r3, [r2, #4]
	movs r1, #19
	mov r10, r3
	movs r0, #188
	add r10, r4
	bl Func_081963ec
	subs r5, r7, #2
	ldr r0, [sp, #28]
	asrs r5, r5, #1
	lsls r5, r5, #1
	adds r5, r5, r0
	movs r2, #0
	ldrsh r1, [r5, r2]
	ldr r3, .L_0818f948
	asrs r4, r7, #1
	mov r8, r4
	mov r2, r8
	str r2, [sp, #0]
	str r7, [sp, #4]
	adds r1, r1, r3
	asrs r6, r6, #1
	mov r0, r10
	movs r3, #192
	subs r4, r6, r4
	subs r0, r0, r7
	lsls r3, r3, #18
	mov r11, r4
	mov r9, r0
	adds r3, #188
	ldr r0, [sp, #48]
	ldr r4, [r3]
	mov r2, r11
	mov r3, r9
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	mov r2, r8
	movs r4, #0
	ldrsh r1, [r5, r4]
	ldr r0, .L_0818f948
	str r2, [sp, #0]
	str r7, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	adds r1, r1, r0
	adds r2, r6, #0
	ldr r0, [sp, #48]
	mov r3, r9
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #27
	movs r0, #188
	bl Func_081963ec
	mov r2, r8
	movs r4, #0
	ldrsh r1, [r5, r4]
	ldr r0, .L_0818f948
	str r2, [sp, #0]
	str r7, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	adds r1, r1, r0
	mov r2, r11
	ldr r0, [sp, #48]
	mov r3, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #31
	movs r0, #188
	bl Func_081963ec
	mov r0, r8
	movs r4, #0
	ldrsh r1, [r5, r4]
	movs r2, #192
	str r0, [sp, #0]
	str r7, [sp, #4]
	ldr r5, .L_0818f948
	lsls r2, r2, #18
	adds r2, #188
	ldr r4, [r2]
	adds r1, r1, r5
	ldr r0, [sp, #48]
	adds r2, r6, #0
	mov r3, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	b .L_0818fa44
.L_0818f918:
	ldr r3, [sp, #44]
	subs r3, #2
	cmp r3, #30
	bls .L_0818f922
	b .L_0818fa44
.L_0818f922:
	ldr r3, [sp, #44]
	cmp r3, #0
	bge .L_0818f92a
	adds r3, #3
.L_0818f92a:
	asrs r3, r3, #2
	lsls r3, r3, #1
	adds r7, r3, #0
	ldr r3, [sp, #56]
	adds r7, #16
	cmp r3, #1
	bne .L_0818f95c
	cmp r7, #24
	ble .L_0818f962
	movs r7, #24
	b .L_0818f962
.L_0818f940:
	.4byte Func_08143000
.L_0818f944:
	.4byte 0x00000134
.L_0818f948:
	.4byte gMapCellBuffer
.L_0818f94c:
	.4byte 0x00000130
.L_0818f950:
	.4byte IwramCopyWords
.L_0818f954:
	.4byte 0x00000148
.L_0818f958:
	.4byte 0x0000013e
.L_0818f95c:
	cmp r7, #40
	ble .L_0818f962
	movs r7, #40
.L_0818f962:
	ldr r4, [sp, #20]
	subs r5, r7, #2
	ldr r2, [r4]
	ldr r3, [r4, #12]
	ldr r1, [r4, #4]
	asrs r6, r2, #16
	adds r2, r2, r3
	ldr r3, [r4, #16]
	asrs r0, r1, #16
	adds r1, r1, r3
	mov r10, r0
	str r2, [r4]
	str r1, [r4, #4]
	movs r0, #188
	movs r1, #19
	bl Func_081963ec
	ldr r1, [sp, #28]
	asrs r5, r5, #1
	lsls r5, r5, #1
	adds r5, r5, r1
	mov r9, r6
	asrs r6, r7, #1
	movs r2, #0
	ldrsh r1, [r5, r2]
	ldr r3, .L_0818fcf8
	str r6, [sp, #0]
	str r7, [sp, #4]
	mov r4, r9
	mov r0, r10
	movs r2, #192
	subs r4, r4, r6
	subs r0, r0, r7
	lsls r2, r2, #18
	mov r11, r4
	mov r8, r0
	adds r2, #188
	ldr r0, [sp, #48]
	ldr r4, [r2]
	adds r1, r1, r3
	mov r2, r11
	mov r3, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r4, .L_0818fcf8
	str r6, [sp, #0]
	str r7, [sp, #4]
	movs r0, #192
	lsls r0, r0, #18
	adds r0, #188
	mov r2, r9
	adds r1, r1, r4
	mov r3, r8
	ldr r4, [r0]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #27
	movs r0, #188
	bl Func_081963ec
	movs r2, #0
	ldrsh r1, [r5, r2]
	ldr r3, .L_0818fcf8
	str r6, [sp, #0]
	str r7, [sp, #4]
	movs r0, #192
	lsls r0, r0, #18
	adds r0, #188
	ldr r4, [r0]
	mov r2, r11
	adds r1, r1, r3
	ldr r0, [sp, #48]
	mov r3, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #31
	movs r0, #188
	bl Func_081963ec
	movs r2, #0
	ldrsh r1, [r5, r2]
	ldr r3, .L_0818fcf8
	str r6, [sp, #0]
	str r7, [sp, #4]
	movs r5, #192
	lsls r5, r5, #18
	adds r5, #188
	adds r1, r1, r3
	ldr r4, [r5]
	ldr r0, [sp, #48]
	mov r2, r9
	mov r3, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_0818fa44:
	ldr r6, [sp, #56]
	cmp r6, #0
	bne .L_0818fae8
	ldr r7, [sp, #44]
	cmp r7, #53
	bgt .L_0818fae0
	ldr r7, [sp, #52]
	movs r0, #0
	mov r8, r0
.L_0818fa56:
	ldr r2, [r7, #24]
	subs r3, r2, #1
	adds r2, #31
	str r3, [r7, #24]
	cmp r2, #47
	bhi .L_0818fad4
	ldr r1, [sp, #16]
	movs r2, #8
	movs r3, #2
	movs r4, #0
	mov r10, r1
	mov r9, r2
	mov r11, r3
.L_0818fa70:
	ldr r3, [r7, #24]
	lsls r3, r3, #1
	adds r3, r3, r4
	lsls r6, r3, #1
	cmp r6, #0
	blt .L_0818face
	ldr r0, [r7, #8]
	str r4, [sp, #8]
	bl Trig_Sin
	mov r1, r10
	ldr r5, [r1]
	lsrs r3, r5, #31
	adds r5, r5, r3
	adds r3, r6, #0
	muls r3, r0
	asrs r5, r5, #1
	asrs r3, r3, #16
	ldr r0, [r7, #8]
	adds r5, r5, r3
	bl Trig_Cos
	adds r2, r6, #0
	muls r2, r0
	mov r6, r10
	ldr r3, [r6, #4]
	asrs r2, r2, #16
	ldr r1, .L_0818fcfc
	adds r3, r3, r2
	mov r2, r9
	subs r2, #2
	ldrh r1, [r1, r2]
	ldr r0, [sp, #32]
	mov r2, r11
	adds r1, r0, r1
	subs r5, r5, r2
	mov r0, r9
	movs r6, #4
	str r0, [sp, #4]
	adds r2, r5, #0
	subs r3, #36
	str r6, [sp, #0]
	ldr r0, [sp, #48]
	ldr r5, [sp, #36]
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #8]
.L_0818face:
	adds r4, #1
	cmp r4, #16
	bne .L_0818fa70
.L_0818fad4:
	movs r6, #1
	add r8, r6
	mov r0, r8
	adds r7, #28
	cmp r0, #16
	bne .L_0818fa56
.L_0818fae0:
	ldr r1, [sp, #56]
	movs r3, #54
	cmp r1, #0
	beq .L_0818faf2
.L_0818fae8:
	ldr r2, [sp, #56]
	movs r3, #24
	cmp r2, #3
	beq .L_0818faf2
	movs r3, #32
.L_0818faf2:
	ldr r4, [sp, #44]
	cmp r4, r3
	bge .L_0818fafa
	b .L_0818fd72
.L_0818fafa:
	cmp r4, r3
	beq .L_0818fb00
	b .L_0818fc90
.L_0818fb00:
	ldr r5, [sp, #56]
	cmp r5, #0
	beq .L_0818fb1a
	cmp r5, #2
	beq .L_0818fb1a
	cmp r5, #3
	beq .L_0818fb1a
	movs r0, #133
	movs r6, #0
	bl Func_081180e8
	mov r8, r6
	b .L_0818fb5c
.L_0818fb1a:
	movs r0, #145
	bl Func_081180e8
	ldr r0, [sp, #60]
	movs r7, #0
	ldr r3, [r0, #20]
	mov r8, r7
	cmp r3, #0
	beq .L_0818fb64
	movs r5, #36
.L_0818fb2e:
	ldr r1, [sp, #60]
	ldrsh r0, [r5, r1]
	movs r1, #4
	bl Func_08118088
	ldr r4, [sp, #60]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_0818fb2e
	b .L_0818fb64
.L_0818fb48:
	mov r5, r8
	ldr r6, [sp, #60]
	lsls r3, r5, #1
	adds r3, #36
	ldrsh r0, [r6, r3]
	movs r1, #0
	bl Func_08118088
	movs r0, #1
	add r8, r0
.L_0818fb5c:
	ldr r1, [sp, #60]
	ldr r3, [r1, #20]
	cmp r8, r3
	bne .L_0818fb48
.L_0818fb64:
	movs r2, #0
	mov r8, r2
	cmp r3, #0
	beq .L_0818fb8e
	movs r6, #8
	movs r5, #36
.L_0818fb70:
	ldr r3, [sp, #60]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r2, #5
	mov r3, r8
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r0, [sp, #60]
	movs r7, #1
	ldr r3, [r0, #20]
	add r8, r7
	adds r5, #2
	cmp r8, r3
	bne .L_0818fb70
.L_0818fb8e:
	ldr r1, [sp, #52]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #8
	str r3, [r2]
	ldr r7, [sp, #16]
	ldr r6, [sp, #52]
	movs r4, #0
	movs r5, #4
	mov r8, r4
	mov r10, r5
.L_0818fba8:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #255
	ands r5, r3
	ldr r3, [r7]
	adds r0, r5, #0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [r7, #4]
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r6, #4]
	bl Trig_Sin
	mov r3, r10
	muls r3, r0
	asrs r3, r3, #3
	str r3, [r6, #12]
	adds r0, r5, #0
	bl Trig_Cos
	mov r3, r10
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #3
	str r3, [r6, #16]
	mov r0, r8
	movs r1, #3
	bl Math_Div
	movs r1, #1
	adds r0, #17
	add r8, r1
	str r0, [r6, #24]
	mov r2, r8
	movs r0, #4
	add r10, r0
	adds r6, #28
	cmp r2, #16
	bne .L_0818fba8
	ldr r4, [sp, #16]
	ldr r5, [sp, #52]
	movs r6, #224
	movs r3, #0
	lsls r6, r6, #1
	mov r8, r3
	mov r10, r4
	adds r7, r5, r6
.L_0818fc14:
	bl Random16
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r6, r0, #0
	adds r3, #255
	mov r0, r10
	ands r6, r3
	ldr r3, [r0]
	adds r5, #128
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r0, #4]
	adds r0, r6, #0
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	negs r1, r3
	asrs r0, r1, #5
	str r0, [r7, #16]
	ldr r2, [sp, #56]
	cmp r2, #1
	bne .L_0818fc78
	ldr r3, [r7, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #12]
	lsrs r3, r1, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	str r3, [r7, #16]
.L_0818fc78:
	mov r0, r8
	movs r1, #3
	bl Math_Div
	movs r3, #1
	add r8, r3
	adds r0, #17
	mov r4, r8
	str r0, [r7, #24]
	adds r7, #28
	cmp r4, #47
	bne .L_0818fc14
.L_0818fc90:
	ldr r5, [sp, #56]
	cmp r5, #0
	beq .L_0818fc9e
	cmp r5, #2
	beq .L_0818fc9e
	cmp r5, #3
	bne .L_0818fd16
.L_0818fc9e:
	ldr r7, .L_0818fd00
	ldr r5, [sp, #52]
	movs r6, #0
	mov r8, r6
	mov r10, r7
.L_0818fca8:
	movs r0, #2
	ldrsh r6, [r5, r0]
	movs r1, #6
	ldrsh r7, [r5, r1]
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_0818fcf0
	movs r1, #3
	bl Math_Div
	mov r2, r10
	ldrb r1, [r2, r0]
	ldr r3, [sp, #52]
	movs r0, #32
	lsls r1, r1, #11
	movs r4, #240
	adds r1, r3, r1
	lsls r4, r4, #4
	adds r2, r6, #0
	str r0, [sp, #0]
	adds r3, r7, #0
	movs r0, #64
	str r0, [sp, #4]
	adds r1, r1, r4
	subs r2, #16
	subs r3, #32
	ldr r0, [sp, #48]
	ldr r6, [sp, #36]
	mov lr, r6
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_0818fcf0:
	cmp r0, #0
	ble .L_0818fd04
	subs r3, r0, #1
	b .L_0818fd08
.L_0818fcf8:
	.4byte gMapCellBuffer
.L_0818fcfc:
	.4byte Data_08197410
.L_0818fd00:
	.4byte Data_08199ec5
.L_0818fd04:
	movs r3, #1
	negs r3, r3
.L_0818fd08:
	str r3, [r5, #24]
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #16
	bne .L_0818fca8
.L_0818fd16:
	ldr r2, [sp, #52]
	movs r3, #224
	ldr r6, .L_0818fde4
	movs r1, #0
	lsls r3, r3, #1
	mov r8, r1
	adds r5, r2, r3
.L_0818fd24:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0818fd66
	asrs r0, r0, #2
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #32]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0818fd66:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #47
	bne .L_0818fd24
.L_0818fd72:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #52]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #44]
	ldr r5, [sp, #24]
	adds r4, #1
	str r4, [sp, #44]
	cmp r4, r5
	beq .L_0818fda0
	b .L_0818f766
.L_0818fda0:
	ldr r0, .L_0818fde8
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r6, [sp, #56]
	cmp r6, #0
	bne .L_0818fdd6
	movs r1, #240
	ldr r5, .L_0818fdec
	lsls r1, r1, #6
	ldr r0, .L_0818fdf0
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #48]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0818fdf4
	bl Scheduler_RemoveCallback
	ldr r0, [sp, #60]
	bl Func_081504c0
	b .L_0818feee
.L_0818fdd6:
	ldr r7, [sp, #56]
	cmp r7, #3
	bne .L_0818fdf8
	bl Func_08143bb8
	b .L_0818feee
	.2byte 0x0000
.L_0818fde4:
	.4byte Data_08197410
.L_0818fde8:
	.4byte Func_08143000
.L_0818fdec:
	.4byte IwramClearWords
.L_0818fdf0:
	.4byte 0x06004000
.L_0818fdf4:
	.4byte Func_08143488
.L_0818fdf8:
	movs r3, #192
	movs r0, #195
	lsls r3, r3, #18
	lsls r0, r0, #1
	ldr r7, [r3, #36]
	bl Audio_PlayCue
	movs r1, #238
	ldr r0, [sp, #52]
	lsls r1, r1, #7
	adds r1, #160
	adds r3, r0, r1
	ldr r2, .L_0818fe54
	ldr r3, [r3]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #164
	strh r3, [r2, #4]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r1, #128
	strh r3, [r2, #6]
	ldr r3, .L_0818fe50
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	lsls r1, r1, #7
	ldr r3, .L_0818fe58
	ldr r0, .L_0818fe5c
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0818fe60
	bl Scheduler_RemoveCallback
	ldr r6, .L_0818fe64
	ldr r5, .L_0818fe68
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0818fe8e
	b .L_0818fe6c
.L_0818fe50:
	.4byte 0x00000787
.L_0818fe54:
	.4byte Data_03001120
.L_0818fe58:
	.4byte IwramClearWords
.L_0818fe5c:
	.4byte 0x06004000
.L_0818fe60:
	.4byte Func_08143488
.L_0818fe64:
	.4byte Data_020038e0
.L_0818fe68:
	.4byte 0x04000208
.L_0818fe6c:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #230
	adds r3, r3, r6
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0818fe8e:
	strh r1, [r5]
	movs r2, #128
	ldr r3, .L_0818fec0
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #206
	lsls r0, r0, #3
	adds r3, r7, r0
	ldrh r1, [r3]
	movs r0, #2
	movs r2, #0
	bl Func_08118038
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0818fee6
	b .L_0818fec4
	.2byte 0x0000
.L_0818fec0:
	.4byte 0x00000000
.L_0818fec4:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #234
	adds r3, r3, r6
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0818fee6:
	strh r1, [r5]
	movs r0, #1
	bl WaitFrames
.L_0818feee:
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
