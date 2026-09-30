.syntax unified
	.thumb
	.global Func_081587e4
	.thumb_func
Func_081587e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #96
	str r0, [sp, #56]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	ldr r0, [r3, #92]
	str r1, [sp, #52]
	mov r9, r0
	ldr r3, [r3, #48]
	movs r0, #0
	str r3, [sp, #32]
	bl Func_081435e0
	ldr r2, [sp, #56]
	movs r3, #64
	ldr r1, [r2, #4]
	adds r0, r2, #0
	lsls r1, r1, #4
	orrs r1, r3
	mov r3, sp
	adds r3, #84
	str r3, [sp, #28]
	ldr r2, [sp, #28]
	add r3, sp, #72
	bl Func_0815585c
	ldr r3, .L_08158844
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #56]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08158848
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
	b .L_08158850
	.2byte 0x0000
.L_08158844:
	.4byte 0x00001010
.L_08158848:
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
.L_08158850:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #224
	lsls r1, r1, #3
	str r3, [sp, #36]
	ldr r0, .L_08158b3c
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #216
	lsls r1, r1, #7
	adds r1, #192
	ldr r0, .L_08158b40
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08158b44
	bl Func_080145a8
	ldr r5, [sp, #56]
	movs r7, #0
	ldr r0, [r5, #8]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	movs r1, #36
	ldrsh r0, [r5, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r2, #0
	str r0, [sp, #24]
	str r2, [sp, #48]
	mov r5, r9
.L_081588b8:
	ldr r3, [r6, #8]
	str r3, [r5]
	movs r3, #132
	lsls r3, r3, #15
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	asrs r3, r7, #5
	str r3, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #64
	lsls r3, r3, #16
	asrs r3, r3, #6
	str r3, [r5, #16]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #127
	lsls r3, r3, #16
	asrs r3, r3, #5
	str r3, [r5, #20]
	ldr r3, [r5]
	cmp r3, #0
	ble .L_081588f6
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_081588f6:
	movs r3, #1
	str r3, [r5, #24]
	ldr r4, [sp, #48]
	movs r3, #160
	lsls r3, r3, #15
	adds r4, #1
	adds r7, r7, r3
	adds r5, #28
	str r4, [sp, #48]
	cmp r4, #8
	bne .L_081588b8
	ldr r0, [sp, #32]
	movs r5, #0
	adds r0, #12
	str r5, [sp, #44]
	str r0, [sp, #16]
.L_08158916:
	ldr r1, [sp, #44]
	cmp r1, #16
	ble .L_08158922
	ldr r0, .L_08158b3c
	bl Func_0815f0a0
.L_08158922:
	ldr r2, [sp, #56]
	ldr r3, [r2, #28]
	cmp r3, #1
	bne .L_081589d0
	ldr r3, [sp, #44]
	lsls r5, r3, #11
	adds r0, r5, #0
	bl Trig_Sin
	ldr r4, [sp, #28]
	negs r0, r0
	ldr r3, [r4]
	lsls r0, r0, #2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r6, r0, #0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r5, [sp, #28]
	lsls r0, r0, #1
	ldr r3, [r5, #4]
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r5, r0, #0
	ldr r0, [sp, #44]
	subs r6, #10
	subs r5, #22
	cmp r0, #16
	ble .L_0815896c
	lsls r3, r0, #1
	subs r3, r5, r3
	adds r5, r3, #0
	adds r5, #32
.L_0815896c:
	ldr r1, [sp, #56]
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_0815897e
	movs r0, #188
	movs r1, #7
	bl Func_081963ec
	b .L_08158986
.L_0815897e:
	movs r0, #188
	movs r1, #3
	bl Func_081963ec
.L_08158986:
	ldr r2, [sp, #44]
	cmp r2, #3
	bgt .L_081589ae
	movs r3, #20
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	movs r2, #192
	movs r1, #216
	lsls r2, r2, #18
	lsls r1, r1, #7
	adds r2, #188
	adds r1, #192
	ldr r4, [r2]
	ldr r0, [sp, #52]
	add r1, r9
	adds r2, r6, #0
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_081589ae:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #216
	movs r3, #20
	lsls r1, r1, #7
	str r3, [sp, #0]
	adds r1, #192
	movs r3, #40
	str r3, [sp, #4]
	ldr r0, [sp, #52]
	add r1, r9
	adds r2, r6, #0
	adds r3, r5, #0
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
.L_081589d0:
	ldr r5, [sp, #44]
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	bne .L_08158a0c
	movs r0, #0
	movs r5, #224
	str r0, [sp, #48]
	ldr r6, .L_08158b48
	lsls r5, r5, #2
	add r5, r9
.L_081589e6:
	bl Random16
	movs r1, #6
	bl Math_ModU
	adds r0, #3
	str r0, [r5, #12]
	bl Random16
	movs r3, #3
	ands r3, r0
	ldrb r3, [r6, r3]
	str r3, [r5, #16]
	ldr r1, [sp, #48]
	adds r5, #28
	adds r1, #1
	str r1, [sp, #48]
	cmp r1, #32
	bne .L_081589e6
.L_08158a0c:
	bl Func_08014de4
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	bl Func_080156e8
	mov r12, r9
	movs r2, #0
	mov r3, r12
	str r2, [sp, #48]
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r6, r9
.L_08158a26:
	ldr r3, [r6, #24]
	cmp r3, #1
	beq .L_08158a2e
	b .L_08158c68
.L_08158a2e:
	ldr r0, [sp, #12]
	ldr r1, [sp, #44]
	str r0, [sp, #20]
	cmp r1, r0
	bgt .L_08158a3a
	b .L_08158bb8
.L_08158a3a:
	add r5, sp, #60
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	movs r1, #224
	asrs r3, r3, #1
	str r3, [r5]
	subs r3, #12
	mov r10, r3
	ldr r3, [r5, #4]
	lsls r1, r1, #3
	subs r3, #24
	mov r8, r3
	movs r3, #24
	str r3, [sp, #0]
	movs r3, #48
	str r3, [sp, #4]
	mov r2, r10
	mov r3, r8
	ldr r5, [sp, #36]
	ldr r0, [sp, #52]
	add r1, r9
	mov lr, r5
	.2byte 0xf800
	ldr r2, [sp, #44]
	movs r3, #3
	ands r3, r2
	cmp r3, #1
	bgt .L_08158aa4
	ldr r3, .L_08158b4c
	ldr r4, .L_08158b50
	ldrh r1, [r3, #2]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r9
	adds r1, r1, r3
	ldr r3, .L_08158b54
	ldrb r2, [r3, #1]
	ldrb r3, [r4, #1]
	ldr r4, .L_08158b58
	add r2, r10
	ldrb r0, [r4, #1]
	add r3, r8
	str r0, [sp, #0]
	ldr r0, .L_08158b5c
	ldrb r0, [r0, #1]
	str r0, [sp, #4]
	ldr r0, [sp, #52]
	mov lr, r5
	.2byte 0xf800
	b .L_08158ace
.L_08158aa4:
	ldr r3, .L_08158b4c
	ldr r4, .L_08158b50
	ldrh r1, [r3, #4]
	ldr r3, .L_08158b54
	movs r0, #224
	ldrb r2, [r3, #2]
	ldrb r3, [r4, #2]
	ldr r4, .L_08158b58
	lsls r0, r0, #3
	add r1, r9
	adds r1, r1, r0
	ldrb r0, [r4, #2]
	add r2, r10
	str r0, [sp, #0]
	ldr r0, .L_08158b5c
	add r3, r8
	ldrb r0, [r0, #2]
	str r0, [sp, #4]
	ldr r0, [sp, #52]
	mov lr, r5
	.2byte 0xf800
.L_08158ace:
	ldr r0, [sp, #8]
	movs r1, #224
	movs r5, #0
	lsls r1, r1, #2
	mov r11, r5
	adds r7, r0, r1
.L_08158ada:
	movs r2, #1
	ldr r3, [r7, #16]
	str r2, [sp, #0]
	movs r1, #7
	movs r0, #188
	movs r2, #7
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r1, [r7, #16]
	str r3, [sp, #40]
	movs r3, #4
	ands r3, r1
	cmp r3, #0
	beq .L_08158b12
	ldr r0, [r7, #12]
	ldr r4, .L_08158b58
	ldr r5, .L_08158b54
	ldrb r3, [r4, r0]
	mov r2, r10
	subs r3, r2, r3
	ldrb r2, [r5, r0]
	subs r3, r3, r2
	adds r3, #24
	b .L_08158b1c
.L_08158b12:
	ldr r0, [r7, #12]
	ldr r2, .L_08158b54
	ldr r4, .L_08158b58
	ldrb r3, [r2, r0]
	add r3, r10
.L_08158b1c:
	mov r12, r3
	movs r3, #8
	ands r3, r1
	cmp r3, #0
	beq .L_08158b60
	ldr r5, .L_08158b5c
	mov r1, r8
	ldrb r3, [r5, r0]
	ldr r5, .L_08158b50
	subs r3, r1, r3
	ldrb r2, [r5, r0]
	subs r3, r3, r2
	adds r5, r3, #0
	adds r5, #48
	b .L_08158b68
	.2byte 0x0000
.L_08158b3c:
	.4byte 0x0000016c
.L_08158b40:
	.4byte 0x00000157
.L_08158b44:
	.4byte Func_08143000
.L_08158b48:
	.4byte Data_08198524
.L_08158b4c:
	.4byte Data_0819853a
.L_08158b50:
	.4byte Data_08198555
.L_08158b54:
	.4byte Data_0819854c
.L_08158b58:
	.4byte Data_08198528
.L_08158b5c:
	.4byte Data_08198531
.L_08158b60:
	ldr r1, .L_08158ccc
	mov r2, r8
	ldrb r3, [r1, r0]
	adds r5, r2, r3
.L_08158b68:
	ldr r2, .L_08158cd0
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	movs r3, #224
	lsls r3, r3, #3
	add r1, r9
	adds r1, r1, r3
	ldrb r3, [r4, r0]
	ldr r4, .L_08158cd4
	str r3, [sp, #0]
	ldr r3, [r7, #12]
	ldr r0, [sp, #52]
	ldrb r3, [r4, r3]
	mov r2, r12
	str r3, [sp, #4]
	adds r3, r5, #0
	ldr r5, [sp, #40]
	mov lr, r5
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #4
	bne .L_08158ada
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
.L_08158bb8:
	ldr r3, [sp, #12]
	ldr r2, [sp, #44]
	adds r3, #16
	cmp r2, r3
	ble .L_08158c68
	ldr r4, [sp, #24]
	ldr r2, [r6]
	ldr r3, [r4, #8]
	subs r3, r3, r2
	ldr r2, [r6, #12]
	asrs r3, r3, #8
	adds r1, r2, r3
	ldr r2, [r6, #4]
	movs r3, #160
	lsls r3, r3, #13
	subs r3, r3, r2
	ldr r2, [r6, #16]
	asrs r3, r3, #8
	adds r0, r2, r3
	str r1, [r6, #12]
	str r0, [r6, #16]
	ldr r2, [r6, #8]
	ldr r3, [r4, #16]
	subs r3, r3, r2
	ldr r2, [r6, #20]
	asrs r3, r3, #8
	adds r4, r2, r3
	str r4, [r6, #20]
	ldr r3, [sp, #20]
	ldr r5, [sp, #44]
	adds r3, #85
	cmp r5, r3
	bge .L_08158c2a
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08158c06
	adds r2, #63
.L_08158c06:
	asrs r3, r2, #6
	str r3, [r6, #12]
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08158c16
	adds r2, #63
.L_08158c16:
	asrs r3, r2, #6
	str r3, [r6, #16]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08158c26
	adds r2, #63
.L_08158c26:
	asrs r3, r2, #6
	str r3, [r6, #20]
.L_08158c2a:
	ldr r3, [r6, #4]
	ldr r0, .L_08158cd8
	cmp r3, r0
	bgt .L_08158c68
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #8
	str r3, [r2]
	movs r3, #0
	str r3, [r6, #24]
	movs r0, #134
	bl Audio_PlayCue
	ldr r2, [sp, #56]
	movs r3, #4
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	movs r2, #5
	bl Func_0814cd48
	ldr r4, [sp, #56]
	movs r1, #4
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_08118088
.L_08158c68:
	ldr r5, [sp, #12]
	ldr r0, [sp, #8]
	ldr r1, [sp, #48]
	adds r5, #2
	adds r0, #112
	adds r1, #1
	str r5, [sp, #12]
	adds r6, #28
	str r0, [sp, #8]
	str r1, [sp, #48]
	cmp r1, #6
	beq .L_08158c82
	b .L_08158a26
.L_08158c82:
	movs r0, #16
	movs r1, #16
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
	ldr r2, [sp, #44]
	adds r2, #1
	str r2, [sp, #44]
	cmp r2, #96
	beq .L_08158cac
	b .L_08158916
.L_08158cac:
	ldr r0, .L_08158cdc
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08158ccc:
	.4byte Data_08198555
.L_08158cd0:
	.4byte Data_0819853a
.L_08158cd4:
	.4byte Data_08198531
.L_08158cd8:
	.4byte 0x0013ffff
.L_08158cdc:
	.4byte Func_08143000
