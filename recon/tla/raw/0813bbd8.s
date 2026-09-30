.syntax unified
	.thumb
	.global Func_0813bbd8
	.thumb_func
Func_0813bbd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #96]
	sub sp, #36
	ldr r1, [r3, #92]
	str r2, [sp, #24]
	mov r10, r0
	ldr r3, [r3, #100]
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #1
	str r3, [sp, #16]
	mov r9, r1
	bl Func_081435e0
	ldr r3, .L_0813bc40
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #224
	adds r2, #32
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0813bc4c
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0813bc50
	ldr r1, [sp, #16]
	bl Func_08157cf4
	bl Func_0813ba50
	ldr r3, .L_0813bc44
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0813bc48
	subs r2, #8
	b .L_0813bc54
	.2byte 0x0000
.L_0813bc40:
	.4byte 0x00000100
.L_0813bc44:
	.4byte 0x00003f44
.L_0813bc48:
	.4byte 0x00003337
.L_0813bc4c:
	.4byte 0x00000177
.L_0813bc50:
	.4byte 0x0000017e
.L_0813bc54:
	movs r7, #128
	negs r7, r7
	strh r3, [r2]
	movs r3, #0
	mov r11, r7
	mov r8, r3
	adds r7, #112
	mov r6, r9
.L_0813bc64:
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r2, #63
	movs r3, #7
	ands r3, r0
	ands r2, r5
	mov r1, r10
	adds r2, r2, r3
	ldr r3, [r1, #4]
	adds r2, #24
	cmp r3, #1
	bne .L_0813bc8a
	adds r3, r2, r7
	adds r2, r3, #0
	adds r2, #24
	b .L_0813bc90
.L_0813bc8a:
	subs r3, r2, r7
	adds r2, r3, #0
	adds r2, #80
.L_0813bc90:
	lsls r3, r2, #3
	mov r2, r11
	str r2, [r6, #4]
	movs r1, #64
	movs r2, #1
	str r3, [r6]
	negs r1, r1
	movs r3, #1
	add r8, r2
	negs r3, r3
	add r11, r1
	mov r1, r8
	str r3, [r6, #24]
	subs r7, #8
	adds r6, #28
	cmp r1, #32
	bne .L_0813bc64
	movs r2, #0
	mov r8, r2
	adds r2, r3, #0
	movs r3, #230
	lsls r3, r3, #2
	add r3, r9
.L_0813bcbe:
	movs r7, #1
	add r8, r7
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #32
	bne .L_0813bcbe
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0813bce6
	movs r1, #18
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #2
	bl Func_081963ec
	b .L_0813bcf6
.L_0813bce6:
	movs r1, #22
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #6
	bl Func_081963ec
.L_0813bcf6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #104]
	adds r3, #188
	str r2, [sp, #28]
	movs r7, #28
	ldr r3, [r3]
	add r7, sp
	mov r1, r10
	str r3, [r7, #4]
	ldr r3, [r1, #4]
	mov r11, r7
	cmp r3, #0
	bne .L_0813bd66
	movs r2, #0
	mov r8, r2
	ldr r6, .L_0813bd40
	ldr r2, .L_0813bd4c
	ldr r5, .L_0813bd44
	ldr r4, .L_0813bd48
	ldr r0, .L_0813bd50
	movs r1, #224
	lsls r1, r1, #7
.L_0813bd24:
	mov r3, r8
	subs r3, #8
	cmp r3, #95
	bhi .L_0813bd36
	mov r7, r8
	subs r3, r6, r7
	orrs r3, r1
	strh r3, [r2]
	b .L_0813bd56
.L_0813bd36:
	mov r3, r8
	cmp r3, #135
	bgt .L_0813bd54
	strh r5, [r2]
	b .L_0813bd56
.L_0813bd40:
	.4byte 0x000000f0
.L_0813bd44:
	.4byte 0x00000888
.L_0813bd48:
	.4byte 0x00000100
.L_0813bd4c:
	.4byte gMapCellBuffer
.L_0813bd50:
	.4byte 0xffffff00
.L_0813bd54:
	strh r4, [r2]
.L_0813bd56:
	movs r7, #1
	add r8, r7
	mov r3, r8
	adds r2, #2
	adds r1, r1, r0
	cmp r3, #160
	bne .L_0813bd24
	b .L_0813bdb0
.L_0813bd66:
	ldr r5, .L_0813bd94
	ldr r4, .L_0813bd98
	ldr r2, .L_0813bd9c
	movs r7, #0
	movs r1, #192
	movs r0, #128
	mov r8, r7
	lsls r1, r1, #5
	lsls r0, r0, #1
.L_0813bd78:
	mov r3, r8
	subs r3, #8
	cmp r3, #87
	bhi .L_0813bd88
	adds r3, #160
	orrs r3, r1
	strh r3, [r2]
	b .L_0813bda2
.L_0813bd88:
	mov r3, r8
	cmp r3, #135
	bgt .L_0813bda0
	strh r5, [r2]
	b .L_0813bda2
	.2byte 0x0000
.L_0813bd94:
	.4byte 0x000078f8
.L_0813bd98:
	.4byte 0x00000100
.L_0813bd9c:
	.4byte gMapCellBuffer
.L_0813bda0:
	strh r4, [r2]
.L_0813bda2:
	movs r7, #1
	add r8, r7
	mov r3, r8
	adds r2, #2
	adds r1, r1, r0
	cmp r3, #160
	bne .L_0813bd78
.L_0813bdb0:
	movs r1, #200
	ldr r0, .L_0813c0a4
	lsls r1, r1, #4
	bl Func_080145a8
	movs r2, #239
	lsls r2, r2, #7
	movs r3, #2
	add r2, r9
	str r3, [r2]
	mov r7, r10
	ldr r3, [r7, #24]
	cmp r3, #1
	bne .L_0813bdd8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #75
	b .L_0813bde2
.L_0813bdd8:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #50
.L_0813bde2:
	str r3, [r2]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0813c0a8
	bl Func_080145a8
	movs r1, #0
	str r1, [sp, #20]
	mov r2, r10
	ldr r4, [r2, #24]
	ldr r5, .L_0813c0ac
	adds r2, r4, #0
	lsls r3, r2, #1
	adds r3, #1
	ldrb r3, [r5, r3]
	cmp r3, #0
	bne .L_0813be06
	b .L_0813c074
.L_0813be06:
	lsls r3, r2, #1
	adds r3, #1
	ldrb r3, [r5, r3]
	ldr r7, [sp, #20]
	subs r3, #16
	cmp r7, r3
	bne .L_0813be1e
	movs r0, #133
	bl Func_081180e8
	mov r1, r10
	ldr r4, [r1, #24]
.L_0813be1e:
	lsls r3, r4, #1
	ldrb r3, [r5, r3]
	movs r2, #0
	mov r8, r2
	cmp r3, #0
	bne .L_0813be2c
	b .L_0813bfb8
.L_0813be2c:
	mov r7, r9
.L_0813be2e:
	ldr r0, [r7, #24]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0813beec
	ldr r2, [r7]
	cmp r2, #0
	bge .L_0813be40
	adds r2, #7
.L_0813be40:
	ldr r3, [r7, #4]
	asrs r2, r2, #3
	cmp r3, #0
	bge .L_0813be4a
	adds r3, #7
.L_0813be4a:
	asrs r5, r3, #3
	movs r1, #4
	cmp r4, #2
	beq .L_0813be54
	movs r1, #0
.L_0813be54:
	movs r3, #32
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r3, r11
	ldr r4, [r1, r3]
	movs r1, #224
	lsls r1, r1, #3
	add r1, r9
	adds r3, r5, #0
	ldr r0, [sp, #24]
	mov lr, r4
	.2byte 0xf800
	movs r1, #192
	ldr r3, [r7, #4]
	lsls r1, r1, #1
	adds r1, #255
	cmp r3, r1
	bgt .L_0813be94
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0813be86
	ldr r3, [r7]
	subs r3, #64
	b .L_0813be8a
.L_0813be86:
	ldr r3, [r7]
	adds r3, #64
.L_0813be8a:
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #64
	str r3, [r7, #4]
	b .L_0813bee2
.L_0813be94:
	movs r3, #3
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	bne .L_0813bea4
	movs r0, #115
	bl Audio_PlayCue
.L_0813bea4:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r3, #0
	str r3, [r7, #24]
	mov r2, r10
	ldr r3, [r2, #20]
	movs r5, #0
	cmp r3, #0
	beq .L_0813bee2
	movs r4, #8
	movs r6, #36
.L_0813bec2:
	mov r3, r10
	ldrsh r0, [r6, r3]
	movs r2, #5
	adds r3, r5, #0
	movs r1, #9
	str r4, [sp, #0]
	str r4, [sp, #8]
	bl Func_0814cd48
	mov r2, r10
	ldr r3, [r2, #20]
	adds r5, #1
	adds r6, #2
	ldr r4, [sp, #8]
	cmp r5, r3
	bne .L_0813bec2
.L_0813bee2:
	ldr r0, [r7, #24]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_0813bfa2
.L_0813beec:
	ldr r2, [r7]
	cmp r2, #0
	bge .L_0813bef4
	adds r2, #7
.L_0813bef4:
	ldr r3, [r7, #4]
	asrs r2, r2, #3
	cmp r3, #0
	bge .L_0813befe
	adds r3, #7
.L_0813befe:
	asrs r6, r3, #3
	subs r3, r0, #1
	cmp r3, #13
	bhi .L_0813bf3c
	mov r1, r10
	ldr r3, [r1, #24]
	movs r5, #4
	cmp r3, #2
	beq .L_0813bf12
	movs r5, #0
.L_0813bf12:
	movs r1, #3
	str r2, [sp, #12]
	bl Math_Div
	adds r1, r0, #0
	lsls r1, r1, #10
	movs r3, #176
	lsls r3, r3, #4
	add r1, r9
	adds r1, r1, r3
	movs r3, #32
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r5, r11
	ldr r4, [r5]
	ldr r0, [sp, #24]
	ldr r2, [sp, #12]
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r7, #24]
.L_0813bf3c:
	adds r3, r0, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_0813bf9a
	movs r6, #224
	lsls r6, r6, #2
	movs r5, #0
	add r6, r9
.L_0813bf4c:
	ldr r3, [r6, #24]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_0813bf92
	movs r3, #18
	str r3, [r6, #24]
	bl Random16
	movs r3, #31
	ands r0, r3
	ldr r3, [r7]
	cmp r3, #0
	bge .L_0813bf6a
	adds r3, #7
.L_0813bf6a:
	asrs r3, r3, #3
	adds r3, r0, r3
	lsls r3, r3, #3
	adds r3, #8
	str r3, [r6]
	bl Random16
	movs r3, #15
	ands r0, r3
	ldr r3, [r7, #4]
	cmp r3, #0
	bge .L_0813bf84
	adds r3, #7
.L_0813bf84:
	asrs r3, r3, #3
	adds r3, r0, r3
	subs r3, #15
	lsls r3, r3, #3
	str r3, [r6, #4]
	ldr r0, [r7, #24]
	b .L_0813bf9a
.L_0813bf92:
	adds r5, #1
	adds r6, #28
	cmp r5, #32
	bne .L_0813bf4c
.L_0813bf9a:
	cmp r0, #14
	bgt .L_0813bfa2
	adds r3, r0, #1
	str r3, [r7, #24]
.L_0813bfa2:
	mov r1, r10
	ldr r4, [r1, #24]
	ldr r3, .L_0813c0ac
	movs r2, #1
	add r8, r2
	lsls r2, r4, #1
	ldrb r3, [r3, r2]
	adds r7, #28
	cmp r8, r3
	beq .L_0813bfb8
	b .L_0813be2e
.L_0813bfb8:
	movs r5, #224
	movs r2, #0
	lsls r5, r5, #2
	mov r8, r2
	add r5, r9
.L_0813bfc2:
	ldr r2, [r5, #24]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	beq .L_0813c02e
	cmp r2, #17
	bgt .L_0813c022
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r2, [r5]
	asrs r0, r3, #1
	cmp r2, #0
	bge .L_0813bfde
	adds r2, #7
.L_0813bfde:
	ldr r6, .L_0813c0b0
	asrs r2, r2, #3
	ldrb r3, [r6, r0]
	lsrs r1, r3, #1
	ldr r3, [r5, #4]
	subs r2, r2, r1
	mov r12, r2
	cmp r3, #0
	bge .L_0813bff2
	adds r3, #7
.L_0813bff2:
	asrs r3, r3, #3
	subs r7, r3, r1
	mov r1, r10
	ldr r3, [r1, #24]
	movs r4, #4
	cmp r3, #2
	beq .L_0813c002
	movs r4, #0
.L_0813c002:
	ldr r2, .L_0813c0b4
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldrb r3, [r6, r0]
	ldr r2, [sp, #16]
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r3, r11
	adds r1, r2, r1
	ldr r4, [r4, r3]
	mov r2, r12
	ldr r0, [sp, #24]
	adds r3, r7, #0
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r5, #24]
.L_0813c022:
	movs r7, #1
	negs r7, r7
	cmp r2, r7
	ble .L_0813c02e
	subs r3, r2, #1
	str r3, [r5, #24]
.L_0813c02e:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #32
	bne .L_0813bfc2
	bl Func_081434f8
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #20]
	mov r7, r10
	adds r3, #1
	str r3, [sp, #20]
	ldr r2, [r7, #24]
	ldr r5, .L_0813c0ac
	lsls r3, r2, #1
	adds r3, #1
	ldrb r3, [r5, r3]
	ldr r1, [sp, #20]
	adds r4, r2, #0
	cmp r1, r3
	beq .L_0813c074
	b .L_0813be06
.L_0813c074:
	ldr r0, .L_0813c0a8
	bl Func_08014644
	ldr r0, .L_0813c0a4
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	bl Func_0813ba50
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813c0a4:
	.4byte Func_0813bba0
.L_0813c0a8:
	.4byte Func_08143000
.L_0813c0ac:
	.4byte Data_08197521
.L_0813c0b0:
	.4byte Data_0819745e
.L_0813c0b4:
	.4byte Data_0819744c
