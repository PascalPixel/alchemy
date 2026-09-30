.syntax unified
	.thumb
	.global Func_0817b9b8
	.thumb_func
Func_0817b9b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #32]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	ldr r1, [r5, #96]
	mov r9, r0
	movs r0, #0
	str r1, [sp, #28]
	bl Func_081435e0
	ldr r3, .L_0817ba1c
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #139
	adds r2, #82
	lsls r1, r1, #7
	strh r3, [r2]
	ldr r0, .L_0817ba20
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	movs r2, #1
	movs r3, #1
	ldr r0, .L_0817ba24
	add r1, r9
	bl Func_08157cf4
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r2, [r5, #104]
	movs r3, #2
	str r2, [sp, #16]
	movs r2, #239
	lsls r2, r2, #7
	b .L_0817ba28
	.2byte 0x0000
.L_0817ba1c:
	.4byte 0x00001010
.L_0817ba20:
	.4byte 0x0000012e
.L_0817ba24:
	.4byte 0x00000161
.L_0817ba28:
	add r2, r9
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #50
	add r2, r9
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0817bd78
	bl Func_080145a8
	ldr r4, [sp, #32]
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r1, [sp, #32]
	mov r11, r0
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r3, #128
	str r0, [sp, #12]
	lsls r3, r3, #4
	ldr r5, [r5, #36]
	adds r3, #102
	adds r2, r5, r3
	movs r4, #0
	movs r3, #1
	str r5, [sp, #8]
	strb r3, [r2]
	str r4, [sp, #24]
.L_0817ba70:
	ldr r1, [sp, #32]
	ldr r0, [r1, #8]
	movs r1, #3
	bl Func_0817b970
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_0817baec
	ldr r3, [sp, #32]
	add r5, sp, #48
	ldr r0, [r3, #8]
	adds r1, r5, #0
	bl Func_0815e21c
	movs r4, #0
	mov r8, r4
	mov r10, r5
	mov r7, r9
.L_0817ba94:
	movs r3, #0
	str r3, [r7, #24]
	mov r0, r10
	ldr r3, [r0]
	movs r5, #255
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r0, #4]
	subs r3, #8
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r1, #1
	add r8, r1
	asrs r3, r3, #5
	mov r2, r8
	str r3, [r7, #16]
	adds r7, #28
	cmp r2, #32
	bne .L_0817ba94
	movs r0, #136
	bl Audio_PlayCue
.L_0817baec:
	ldr r3, [sp, #24]
	cmp r3, #0
	blt .L_0817bb84
	movs r4, #0
	mov r8, r4
	mov r5, r9
.L_0817baf8:
	mov r3, r8
	cmp r3, #0
	bge .L_0817bb00
	adds r3, #7
.L_0817bb00:
	ldr r0, [sp, #24]
	asrs r3, r3, #3
	cmp r0, r3
	blt .L_0817bb50
	ldr r3, [r5, #24]
	cmp r3, #23
	bgt .L_0817bb50
	cmp r3, #0
	bge .L_0817bb14
	adds r3, #3
.L_0817bb14:
	asrs r3, r3, #2
	lsls r1, r3, #3
	adds r1, r1, r3
	lsls r1, r1, #7
	movs r2, #224
	lsls r2, r2, #3
	add r1, r9
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	subs r3, #24
	str r0, [sp, #4]
	subs r2, #12
	ldr r0, [sp, #28]
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #48
	ldr r2, .L_0817bd7c
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0817bb50:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #24
	bne .L_0817baf8
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_0817bb84
	mov r4, r11
	ldr r3, [r4, #16]
	ldr r0, [sp, #12]
	movs r1, #100
	str r3, [r0, #16]
	ldr r3, [r4, #8]
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #5
	bl Math_Div
	ldr r1, [sp, #12]
	mov r2, r11
	str r0, [r1, #8]
	ldr r4, [sp, #12]
	ldrh r3, [r2, #6]
	strh r3, [r4, #6]
.L_0817bb84:
	ldr r0, [sp, #24]
	cmp r0, #16
	bne .L_0817bba2
	ldr r1, [sp, #32]
	movs r3, #120
	ldr r0, [r1, #8]
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #16
	bl Func_08157530
	ldr r4, [sp, #12]
	movs r3, #224
	lsls r3, r3, #11
	str r3, [r4, #40]
.L_0817bba2:
	ldr r0, [sp, #24]
	cmp r0, #32
	beq .L_0817bbaa
	b .L_0817bcb6
.L_0817bbaa:
	ldr r0, .L_0817bd80
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817bd84
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	mov r1, r11
	ldr r2, [sp, #12]
	ldr r3, [r1, #16]
	movs r7, #224
	str r3, [r2, #16]
	lsls r7, r7, #2
	ldr r3, [r1, #8]
	movs r1, #100
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #3
	bl Math_Div
	ldr r3, [sp, #12]
	mov r4, r11
	str r0, [r3, #8]
	movs r1, #100
	ldr r3, [r4, #8]
	add r7, r9
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #4
	bl Math_Div
	mov r1, r11
	str r0, [r1, #8]
	ldrh r3, [r1, #6]
	ldr r2, [sp, #12]
	add r5, sp, #36
	strh r3, [r2, #6]
	ldr r4, [sp, #32]
	adds r1, r5, #0
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_0815e20c
	movs r0, #0
	mov r8, r0
	mov r10, r5
.L_0817bc0c:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	mov r1, r10
	ldr r3, [r1]
	movs r6, #254
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	lsls r6, r6, #7
	ldr r3, [r1, #4]
	adds r6, #255
	subs r3, #8
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	movs r2, #128
	lsls r2, r2, #7
	ands r6, r0
	adds r6, r6, r2
	bl Random16
	movs r5, #255
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #28
	cmp r4, #16
	bne .L_0817bc0c
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #4
	str r3, [r2]
	movs r0, #134
	bl Func_081180e8
	ldr r2, [sp, #32]
	mov r3, r8
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r1, [sp, #32]
	movs r3, #192
	lsls r3, r3, #11
	movs r4, #36
	ldrsh r0, [r1, r4]
	str r3, [sp, #0]
	movs r3, #10
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #160
	movs r1, #1
	lsls r2, r2, #12
	lsls r3, r3, #13
	bl Func_0815f000
.L_0817bcb6:
	ldr r3, [sp, #24]
	subs r3, #32
	cmp r3, #23
	bhi .L_0817bd22
	movs r5, #224
	movs r2, #0
	lsls r5, r5, #2
	mov r8, r2
	add r5, r9
.L_0817bcc8:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0817bd16
	ldr r0, [sp, #24]
	add r0, r8
	cmp r0, #0
	bge .L_0817bcd8
	adds r0, #3
.L_0817bcd8:
	movs r1, #6
	asrs r0, r0, #2
	bl __modsi3
	adds r1, r0, #0
	lsls r1, r1, #8
	movs r3, #139
	lsls r3, r3, #7
	add r1, r9
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #16
	subs r3, #8
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #8
	ldr r0, [sp, #28]
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #56
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0817bd16:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #16
	bne .L_0817bcc8
.L_0817bd22:
	movs r0, #4
	movs r1, #4
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
	ldr r2, [sp, #24]
	adds r2, #1
	str r2, [sp, #24]
	cmp r2, #60
	beq .L_0817bd4c
	b .L_0817ba70
.L_0817bd4c:
	ldr r4, [sp, #8]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #102
	adds r2, r4, r0
	movs r3, #0
	strb r3, [r2]
	ldr r0, .L_0817bd78
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817bd78:
	.4byte Func_08143000
.L_0817bd7c:
	.4byte 0xffffe000
.L_0817bd80:
	.4byte 0x0000012e
.L_0817bd84:
	.4byte IwramCopyWords
