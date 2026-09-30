.syntax unified
	.thumb
	.global Func_0815a9e4
	.thumb_func
Func_0815a9e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #80
	str r0, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #48]
	movs r0, #0
	ldr r1, [r3, #96]
	str r1, [sp, #44]
	ldr r3, [r3, #100]
	str r3, [sp, #28]
	bl Func_081435e0
	ldr r2, [sp, #52]
	ldr r0, [r2, #24]
	cmp r0, #0
	bne .L_0815aa20
	movs r2, #128
	ldr r3, .L_0815aa1c
	b .L_0815aa28
	.2byte 0x0000
.L_0815aa1c:
	.4byte 0x000000cc
.L_0815aa20:
	cmp r0, #1
	bne .L_0815aa34
	movs r2, #128
	ldr r3, .L_0815aa30
.L_0815aa28:
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	b .L_0815aa34
.L_0815aa30:
	.4byte 0x000000aa
.L_0815aa34:
	ldr r3, [sp, #52]
	movs r7, #16
	ldr r4, [r3, #4]
	movs r1, #12
	negs r7, r7
	negs r1, r1
	str r7, [sp, #24]
	str r1, [sp, #20]
	cmp r4, #1
	bne .L_0815aa60
	movs r2, #8
	movs r3, #40
	str r2, [sp, #24]
	str r3, [sp, #20]
	cmp r0, #0
	beq .L_0815aa60
	movs r7, #36
	str r7, [sp, #20]
	cmp r0, #1
	beq .L_0815aa60
	movs r1, #40
	str r1, [sp, #20]
.L_0815aa60:
	lsls r3, r4, #1
	ldr r2, .L_0815aac8
	adds r3, r3, r4
	adds r3, r0, r3
	ldrsb r3, [r2, r3]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	movs r2, #1
	ldr r1, .L_0815aacc
	movs r3, #0
	ldr r0, .L_0815aad0
	bl Func_08157cf4
	ldr r0, .L_0815aad4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0815aad8
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r5, #160
	ldr r7, .L_0815aac4
	lsls r5, r5, #19
	adds r5, #2
	movs r6, #0
.L_0815aa9e:
	ldrh r2, [r5]
	movs r4, #31
	lsls r3, r2, #16
	lsrs r0, r3, #26
	ands r0, r7
	lsrs r1, r3, #21
	ands r1, r7
	ands r4, r2
	subs r0, #8
	subs r1, #8
	subs r4, #8
	cmp r0, #0
	bge .L_0815aaba
	movs r0, #0
.L_0815aaba:
	cmp r1, #0
	bge .L_0815aadc
	movs r1, #0
	b .L_0815aadc
	.2byte 0x0000
.L_0815aac4:
	.4byte 0x0000001f
.L_0815aac8:
	.4byte Data_08198648
.L_0815aacc:
	.4byte gMapCellBuffer
.L_0815aad0:
	.4byte 0x0000013c
.L_0815aad4:
	.4byte 0x0000013d
.L_0815aad8:
	.4byte IwramCopyWords
.L_0815aadc:
	cmp r4, #0
	bge .L_0815aae2
	movs r4, #0
.L_0815aae2:
	lsls r3, r0, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r4
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #63
	bne .L_0815aa9e
	ldr r2, .L_0815ac8c
	movs r3, #224
	ldr r5, .L_0815ac90
	mov lr, r2
	lsls r3, r3, #3
	movs r4, #0
	movs r7, #0
	mov r9, lr
	mov r10, r3
.L_0815ab06:
	mov r0, lr
	ldrb r0, [r0]
	movs r6, #0
	mov r12, r0
.L_0815ab0e:
	movs r1, #0
	mov r3, r12
	mov r8, r1
	cmp r3, #0
	beq .L_0815ab38
	ldr r1, [sp, #48]
	ldrb r0, [r2, r7]
	adds r3, r4, r1
	mov r2, r10
	adds r1, r3, r2
	adds r2, r5, #0
.L_0815ab24:
	ldrb r3, [r2]
	adds r4, #1
	strb r3, [r1]
	movs r3, #1
	add r8, r3
	adds r2, #1
	adds r1, #1
	cmp r8, r0
	bne .L_0815ab24
	mov r2, r9
.L_0815ab38:
	adds r6, #1
	cmp r6, #32
	bne .L_0815ab0e
	mov r0, lr
	ldrb r3, [r0]
	movs r1, #1
	adds r7, #1
	adds r5, r5, r3
	add lr, r1
	cmp r7, #9
	bne .L_0815ab06
	movs r2, #224
	lsls r2, r2, #3
	movs r7, #0
	mov r12, r2
.L_0815ab56:
	adds r0, r5, #0
	movs r6, #0
	adds r0, #48
.L_0815ab5c:
	ldr r1, [sp, #48]
	mov r2, r12
	adds r3, r4, r1
	adds r1, r3, r2
	adds r2, r5, #0
.L_0815ab66:
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	adds r4, #1
	adds r1, #1
	cmp r2, r0
	bne .L_0815ab66
	adds r6, #1
	cmp r6, #3
	bne .L_0815ab5c
	adds r7, #1
	adds r5, #48
	cmp r7, #32
	bne .L_0815ab56
	ldr r7, [sp, #48]
	movs r0, #224
	movs r1, #252
	adds r3, r4, r7
	lsls r0, r0, #3
	movs r6, #0
	lsls r1, r1, #2
	adds r2, r3, r0
.L_0815ab92:
	ldrb r3, [r5]
	adds r6, #1
	strb r3, [r2]
	adds r5, #1
	adds r2, #1
	adds r4, #1
	cmp r6, r1
	bne .L_0815ab92
	ldr r1, .L_0815ac94
	ldr r0, .L_0815ac98
	movs r6, #224
	adds r7, r1, #3
	lsls r6, r6, #3
.L_0815abac:
	movs r2, #0
	mov r8, r2
	ldrb r3, [r0]
	ldrb r2, [r1]
	muls r3, r2
	cmp r3, #0
	beq .L_0815abd4
	ldr r2, [sp, #48]
	mov r12, r3
	adds r3, r4, r2
	adds r2, r3, r6
.L_0815abc2:
	ldrb r3, [r5]
	adds r4, #1
	strb r3, [r2]
	movs r3, #1
	add r8, r3
	adds r2, #1
	adds r5, #1
	cmp r8, r12
	bne .L_0815abc2
.L_0815abd4:
	adds r1, #1
	adds r0, #1
	cmp r1, r7
	bne .L_0815abac
	ldr r0, .L_0815ac9c
	ldr r1, [sp, #28]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r5, #238
	movs r4, #13
	lsls r5, r5, #7
	negs r4, r4
	movs r6, #0
	adds r5, #220
	adds r7, r4, #0
.L_0815abf6:
	movs r0, #199
	lsls r0, r0, #1
	adds r0, #255
	bl Func_08020040
	ldr r1, [sp, #48]
	str r0, [r5, r1]
	cmp r0, #0
	beq .L_0815ac28
	movs r3, #0
	strb r3, [r0, #26]
	adds r1, r6, #0
	cmp r6, #0
	bge .L_0815ac14
	adds r1, r6, #3
.L_0815ac14:
	asrs r1, r1, #2
	bl Animation_ApplyChildArgumentFar
	ldr r2, [sp, #48]
	ldr r1, [r5, r2]
	movs r2, #4
	ldrb r3, [r1, #9]
	ands r3, r7
	orrs r3, r2
	strb r3, [r1, #9]
.L_0815ac28:
	adds r6, #1
	adds r5, #4
	cmp r6, #11
	bne .L_0815abf6
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #23
	movs r0, #188
	str r3, [sp, #32]
	bl Func_081963ec
	ldr r3, .L_0815ac84
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0815ac88
	adds r2, #2
	strh r3, [r2]
	ldr r4, [sp, #48]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	adds r5, #188
	lsls r0, r0, #7
	adds r2, r4, r7
	ldr r5, [r5]
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r4, r0
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815aca0
	str r5, [sp, #36]
	bl Func_080145a8
	ldr r1, [sp, #48]
	b .L_0815aca4
.L_0815ac84:
	.4byte 0x00003f46
.L_0815ac88:
	.4byte 0x00001010
.L_0815ac8c:
	.4byte Data_08198632
.L_0815ac90:
	.4byte gMapCellBuffer
.L_0815ac94:
	.4byte Data_08198642
.L_0815ac98:
	.4byte Data_08198645
.L_0815ac9c:
	.4byte 0x00000134
.L_0815aca0:
	.4byte Func_08143000
.L_0815aca4:
	movs r2, #140
	lsls r2, r2, #1
	movs r6, #0
	movs r7, #15
	adds r5, r1, r2
.L_0815acae:
	bl Random16
	ands r0, r7
	movs r3, #128
	adds r0, #88
	str r3, [r5, #4]
	str r0, [r5]
	bl Random16
	movs r3, #1
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #20]
	lsls r2, r6, #2
	ands r0, r7
	movs r3, #44
	str r6, [r5, #12]
	adds r0, #2
	subs r3, r3, r2
	adds r6, #1
	str r0, [r5, #8]
	str r3, [r5, #24]
	adds r5, #28
	cmp r6, #11
	bne .L_0815acae
	ldr r3, [sp, #48]
	movs r4, #224
	ldr r1, .L_0815ad78
	lsls r4, r4, #1
	movs r6, #0
	adds r2, r3, r4
.L_0815acee:
	ldrb r3, [r1]
	adds r6, #1
	str r3, [r2]
	adds r1, #1
	adds r2, #28
	cmp r6, #6
	bne .L_0815acee
	ldr r5, .L_0815ad7c
	movs r6, #0
	movs r7, #63
.L_0815ad02:
	bl Random16
	ldr r1, [sp, #24]
	ands r0, r7
	adds r0, r0, r1
	adds r0, #32
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #96
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r7
	adds r0, #32
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	movs r3, #31
	movs r2, #128
	ands r3, r0
	adds r6, #1
	lsls r2, r2, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r6, r2
	bne .L_0815ad02
	ldr r3, .L_0815ad74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, [sp, #48]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #250
	str r3, [r2]
	ldr r0, [sp, #24]
	movs r7, #0
	adds r0, #64
	str r7, [sp, #40]
	str r0, [sp, #16]
.L_0815ad64:
	ldr r1, [sp, #40]
	cmp r1, #0
	bne .L_0815ad80
	movs r0, #212
	bl Audio_PlayCue
	b .L_0815ad80
	.2byte 0x0000
.L_0815ad74:
	.4byte 0x00000785
.L_0815ad78:
	.4byte Data_0819864e
.L_0815ad7c:
	.4byte gMapCellBuffer
.L_0815ad80:
	ldr r2, [sp, #40]
	cmp r2, #40
	bne .L_0815ad8c
	movs r0, #141
	bl Audio_PlayCue
.L_0815ad8c:
	ldr r3, [sp, #40]
	cmp r3, #96
	bne .L_0815ad98
	movs r0, #145
	bl Audio_PlayCue
.L_0815ad98:
	ldr r4, [sp, #40]
	cmp r4, #120
	bne .L_0815ada4
	movs r0, #134
	bl Func_081180e8
.L_0815ada4:
	ldr r7, [sp, #40]
	cmp r7, #81
	bgt .L_0815ae18
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0815adb2
	adds r3, #3
.L_0815adb2:
	asrs r7, r3, #2
	cmp r7, #2
	ble .L_0815adbe
	movs r3, #1
	ands r3, r7
	adds r7, r3, #1
.L_0815adbe:
	ldr r0, .L_0815b12c
	ldr r4, .L_0815b130
	lsls r1, r7, #1
	ldr r2, [sp, #48]
	ldr r6, .L_0815b134
	mov r11, r0
	mov r10, r1
	mov r9, r4
	ldrh r1, [r0, r1]
	ldrb r4, [r4, r7]
	ldr r0, [sp, #24]
	adds r1, r2, r1
	movs r3, #224
	subs r2, r0, r4
	ldrb r0, [r6, r7]
	lsls r3, r3, #3
	mov r8, r3
	movs r5, #116
	subs r3, r5, r0
	add r1, r8
	adds r2, #64
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #32]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	mov r0, r11
	mov r2, r10
	ldrh r1, [r0, r2]
	ldr r3, [sp, #48]
	ldrb r2, [r6, r7]
	mov r4, r9
	adds r1, r3, r1
	ldrb r3, [r4, r7]
	subs r5, r5, r2
	str r3, [sp, #0]
	str r2, [sp, #4]
	add r1, r8
	ldr r0, [sp, #44]
	ldr r2, [sp, #16]
	adds r3, r5, #0
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
.L_0815ae18:
	ldr r3, [sp, #40]
	subs r3, #12
	cmp r3, #75
	bhi .L_0815aeaa
	ldr r0, [sp, #40]
	movs r1, #3
	subs r0, #64
	bl Math_Div
	adds r5, r0, #0
	cmp r5, #0
	bge .L_0815ae32
	movs r5, #0
.L_0815ae32:
	cmp r5, #7
	ble .L_0815ae38
	movs r5, #7
.L_0815ae38:
	ldr r0, [sp, #24]
	lsls r1, r5, #1
	ldr r3, .L_0815b138
	movs r2, #224
	str r1, [sp, #8]
	movs r4, #32
	adds r0, #64
	lsls r2, r2, #3
	mov r10, r4
	movs r6, #0
	mov r8, r0
	mov r11, r2
	mov r9, r3
	subs r4, #44
.L_0815ae54:
	ldr r0, [sp, #8]
	ldr r7, .L_0815b13c
	ldr r2, [sp, #48]
	ldrh r1, [r7, r0]
	mov r7, r9
	ldrb r3, [r7, r5]
	ldr r0, [sp, #24]
	str r3, [sp, #0]
	adds r1, r2, r1
	subs r2, r0, r3
	mov r3, r10
	str r3, [sp, #4]
	str r4, [sp, #12]
	adds r3, r4, #0
	ldr r7, [sp, #32]
	add r1, r11
	adds r2, #64
	ldr r0, [sp, #44]
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_0815b13c
	ldr r2, [sp, #8]
	ldr r3, [sp, #48]
	ldrh r1, [r0, r2]
	mov r7, r9
	ldr r4, [sp, #12]
	adds r1, r3, r1
	ldrb r3, [r7, r5]
	mov r0, r10
	str r3, [sp, #0]
	str r0, [sp, #4]
	adds r3, r4, #0
	add r1, r11
	ldr r0, [sp, #44]
	mov r2, r8
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
	ldr r4, [sp, #12]
	adds r6, #1
	adds r4, #32
	cmp r6, #4
	bne .L_0815ae54
.L_0815aeaa:
	ldr r0, [sp, #40]
	subs r0, #160
	cmp r0, #23
	bhi .L_0815af3a
	movs r1, #3
	bl Math_Div
	movs r3, #7
	subs r5, r3, r0
	cmp r5, #0
	bge .L_0815aec2
	movs r5, #0
.L_0815aec2:
	cmp r5, #7
	ble .L_0815aec8
	movs r5, #7
.L_0815aec8:
	ldr r0, [sp, #24]
	lsls r1, r5, #1
	ldr r3, .L_0815b138
	movs r2, #224
	str r1, [sp, #8]
	movs r4, #32
	adds r0, #64
	lsls r2, r2, #3
	mov r10, r4
	movs r6, #0
	mov r8, r0
	mov r11, r2
	mov r9, r3
	subs r4, #44
.L_0815aee4:
	ldr r0, [sp, #8]
	ldr r7, .L_0815b13c
	ldr r2, [sp, #48]
	ldrh r1, [r7, r0]
	mov r7, r9
	ldrb r3, [r7, r5]
	ldr r0, [sp, #24]
	str r3, [sp, #0]
	adds r1, r2, r1
	subs r2, r0, r3
	mov r3, r10
	str r3, [sp, #4]
	str r4, [sp, #12]
	adds r3, r4, #0
	ldr r7, [sp, #32]
	add r1, r11
	adds r2, #64
	ldr r0, [sp, #44]
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_0815b13c
	ldr r2, [sp, #8]
	ldr r3, [sp, #48]
	ldrh r1, [r0, r2]
	mov r7, r9
	ldr r4, [sp, #12]
	adds r1, r3, r1
	ldrb r3, [r7, r5]
	mov r0, r10
	str r3, [sp, #0]
	str r0, [sp, #4]
	adds r3, r4, #0
	add r1, r11
	ldr r0, [sp, #44]
	mov r2, r8
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
	ldr r4, [sp, #12]
	adds r6, #1
	adds r4, #32
	cmp r6, #4
	bne .L_0815aee4
.L_0815af3a:
	ldr r3, [sp, #40]
	subs r3, #88
	cmp r3, #71
	bhi .L_0815afac
	ldr r0, [sp, #48]
	ldr r3, [sp, #32]
	ldr r2, [sp, #24]
	movs r1, #214
	lsls r1, r1, #5
	adds r4, r0, r1
	movs r6, #48
	movs r5, #96
	str r5, [sp, #4]
	str r4, [sp, #12]
	adds r2, #16
	str r6, [sp, #0]
	mov r10, r3
	ldr r0, [sp, #44]
	adds r1, r4, #0
	movs r3, #0
	mov r9, r2
	mov lr, r10
	.2byte 0xf800
	ldr r4, [sp, #12]
	ldr r7, [sp, #36]
	str r5, [sp, #4]
	adds r1, r4, #0
	ldr r2, [sp, #16]
	str r6, [sp, #0]
	ldr r0, [sp, #44]
	movs r3, #0
	mov r8, r7
	mov lr, r8
	.2byte 0xf800
	ldr r0, [sp, #48]
	movs r1, #179
	lsls r1, r1, #6
	adds r4, r0, r1
	movs r5, #21
	adds r1, r4, #0
	mov r2, r9
	movs r3, #96
	str r4, [sp, #12]
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #44]
	mov lr, r10
	.2byte 0xf800
	ldr r4, [sp, #12]
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #44]
	adds r1, r4, #0
	ldr r2, [sp, #16]
	movs r3, #96
	mov lr, r8
	.2byte 0xf800
.L_0815afac:
	ldr r2, [sp, #40]
	cmp r2, #87
	ble .L_0815b012
	ldr r3, .L_0815b140
	ldr r5, .L_0815b144
	movs r6, #0
	mov r8, r3
.L_0815afba:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_0815b006
	movs r0, #3
	ands r0, r6
	adds r0, #5
	lsls r4, r0, #1
	subs r3, r4, #2
	mov r7, r8
	ldrh r1, [r7, r3]
	ldr r2, [sp, #28]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r5, #4]
	ldr r3, [r5, #16]
	subs r2, r2, r3
	str r2, [r5, #4]
	cmp r2, #0
	bge .L_0815b00a
	ldr r7, [sp, #40]
	cmp r7, #159
	bgt .L_0815b00a
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r5, #4]
	b .L_0815b00a
.L_0815b006:
	subs r3, #1
	str r3, [r5, #24]
.L_0815b00a:
	adds r6, #1
	adds r5, #28
	cmp r6, #64
	bne .L_0815afba
.L_0815b012:
	ldr r0, [sp, #40]
	cmp r0, #4
	bgt .L_0815b01a
	b .L_0815b172
.L_0815b01a:
	movs r1, #6
	mov r10, r1
	cmp r0, #71
	ble .L_0815b026
	movs r2, #11
	mov r10, r2
.L_0815b026:
	movs r3, #0
	mov r4, r10
	mov r8, r3
	cmp r4, #0
	bne .L_0815b032
	b .L_0815b172
.L_0815b032:
	ldr r7, [sp, #48]
	movs r0, #140
	lsls r0, r0, #1
	adds r5, r7, r0
.L_0815b03a:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_0815b042
	b .L_0815b162
.L_0815b042:
	ldr r3, .L_0815b148
	ldr r1, [sp, #40]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #56]
	str r4, [sp, #60]
	cmp r1, #71
	ble .L_0815b066
	ldr r7, [sp, #52]
	mov r3, r8
	lsls r2, r3, #12
	ldr r3, [r7, #24]
	movs r4, #128
	lsls r4, r4, #8
	adds r2, r2, r4
	lsls r3, r3, #14
	adds r3, r3, r2
	b .L_0815b06a
.L_0815b066:
	movs r3, #128
	lsls r3, r3, #8
.L_0815b06a:
	str r3, [sp, #56]
	ldr r3, [sp, #56]
	add r7, sp, #56
	str r3, [r7, #4]
	add r6, sp, #64
	movs r3, #0
	str r3, [r6, #12]
	ldr r0, [sp, #20]
	ldr r3, [r5]
	lsls r2, r0, #1
	adds r3, r3, r2
	lsls r3, r3, #16
	str r3, [r6]
	movs r2, #128
	ldr r3, [r5, #4]
	lsls r2, r2, #18
	lsls r3, r3, #16
	subs r3, r2, r3
	str r3, [r6, #4]
	str r2, [r6, #8]
	ldr r1, [sp, #40]
	lsrs r0, r1, #31
	adds r0, r1, r0
	asrs r0, r0, #1
	add r0, r8
	movs r1, #11
	bl __modsi3
	movs r4, #1
	negs r4, r4
	cmp r0, r4
	beq .L_0815b0c6
	movs r2, #238
	ldr r1, [sp, #48]
	lsls r2, r2, #7
	lsls r3, r0, #2
	adds r2, #220
	adds r3, r3, r2
	ldr r0, [r1, r3]
	adds r2, r7, #0
	adds r1, r6, #0
	movs r3, #0
	str r4, [sp, #12]
	bl Func_08020010
	ldr r4, [sp, #12]
.L_0815b0c6:
	ldr r3, [r5, #4]
	ldr r2, [r5, #8]
	subs r3, r3, r2
	str r3, [r5, #4]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r2, r2, r3
	str r2, [r5, #12]
	cmp r2, #12
	ble .L_0815b0e0
	adds r3, r2, #0
	subs r3, #12
	str r3, [r5, #12]
.L_0815b0e0:
	ldr r3, [r5, #4]
	cmp r3, #0
	bge .L_0815b166
	ldr r2, [sp, #40]
	cmp r2, #159
	ble .L_0815b0f0
	str r4, [r5, #24]
	b .L_0815b166
.L_0815b0f0:
	ldr r3, [sp, #40]
	cmp r3, #87
	ble .L_0815b15a
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #8
	str r3, [r5, #8]
	ldr r4, [sp, #52]
	ldr r3, [r4, #24]
	cmp r3, #0
	bne .L_0815b118
	bl Random16
	movs r1, #96
	bl Math_ModU
	adds r0, #42
	b .L_0815b158
.L_0815b118:
	cmp r3, #1
	bne .L_0815b14c
	bl Random16
	movs r1, #112
	bl Math_ModU
	adds r0, #34
	b .L_0815b158
	.2byte 0x0000
.L_0815b12c:
	.4byte Data_0819863c
.L_0815b130:
	.4byte Data_08198642
.L_0815b134:
	.4byte Data_08198645
.L_0815b138:
	.4byte Data_08198632
.L_0815b13c:
	.4byte Data_08198620
.L_0815b140:
	.4byte Data_08197410
.L_0815b144:
	.4byte gMapCellBuffer
.L_0815b148:
	.4byte Data_08196e44
.L_0815b14c:
	bl Random16
	movs r1, #160
	bl Math_ModU
	adds r0, #10
.L_0815b158:
	str r0, [r5]
.L_0815b15a:
	movs r3, #128
	str r3, [r5, #4]
	movs r3, #8
	b .L_0815b164
.L_0815b162:
	subs r3, #1
.L_0815b164:
	str r3, [r5, #24]
.L_0815b166:
	movs r7, #1
	add r8, r7
	adds r5, #28
	cmp r8, r10
	beq .L_0815b172
	b .L_0815b03a
.L_0815b172:
	ldr r0, [sp, #40]
	cmp r0, #158
	bgt .L_0815b1cc
	ldr r1, [sp, #52]
	movs r6, #0
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_0815b1cc
	movs r7, #3
	ands r7, r0
	movs r5, #36
.L_0815b188:
	ldr r2, [sp, #40]
	cmp r2, #85
	ble .L_0815b1c4
	adds r0, r2, #0
	movs r1, #12
	bl __modsi3
	cmp r0, #0
	bne .L_0815b1ac
	ldr r3, [sp, #52]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r3, #6
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r6, #0
	bl Func_0814cd48
.L_0815b1ac:
	cmp r7, #0
	bne .L_0815b1c0
	ldr r1, [sp, #52]
	ldrsh r0, [r5, r1]
	movs r1, #5
	bl Func_08118088
	ldr r4, [sp, #52]
	ldr r3, [r4, #20]
	b .L_0815b1c4
.L_0815b1c0:
	ldr r0, [sp, #52]
	ldr r3, [r0, #20]
.L_0815b1c4:
	adds r6, #1
	adds r5, #2
	cmp r6, r3
	bne .L_0815b188
.L_0815b1cc:
	ldr r3, [sp, #40]
	subs r3, #90
	cmp r3, #70
	bls .L_0815b1de
	movs r0, #2
	movs r1, #2
	bl Func_08158ce0
	b .L_0815b1e6
.L_0815b1de:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
.L_0815b1e6:
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #48]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #40]
	adds r4, #1
	str r4, [sp, #40]
	cmp r4, #192
	beq .L_0815b20a
	b .L_0815ad64
.L_0815b20a:
	ldr r0, .L_0815b248
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #238
	ldr r7, [sp, #48]
	lsls r0, r0, #7
	adds r0, #220
	movs r6, #0
	adds r5, r7, r0
.L_0815b228:
	ldmia r5!, {r0}
	adds r6, #1
	bl Func_08020048
	cmp r6, #11
	bne .L_0815b228
	bl Func_08143bb8
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0815b248:
	.4byte Func_08143000
