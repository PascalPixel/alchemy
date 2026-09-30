.syntax unified
	.thumb
	.global Func_0810b1b4
	.thumb_func
Func_0810b1b4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r1, #128
	lsls r1, r1, #3
	mov r8, r3
	adds r1, #220
	add r1, r8
	ldr r3, [r1]
	movs r2, #161
	ldrb r3, [r3, #5]
	lsls r2, r2, #3
	add r2, r8
	mov r11, r3
	movs r3, #255
	strb r3, [r2]
	movs r3, #13
	ldr r2, [r1]
	mov r10, r0
	strb r3, [r2, #5]
	movs r3, #129
	lsls r3, r3, #3
	adds r3, #255
	add r3, r8
	ldr r2, .L_0810b364
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #12
	ldrsb r0, [r2, r3]
	bl Audio_PlayCue
	ldr r0, .L_0810b368
	bl Func_081088d8
	mov r0, r10
	lsls r0, r0, #2
	mov r9, r0
	mov r3, r9
	adds r3, #248
	mov r1, r8
	ldr r0, [r1, r3]
	movs r1, #0
	bl Animation_ApplyChildValueFar
	movs r0, #20
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0810b36c
	bl Func_080145a8
	mov r3, r10
	movs r0, #140
	lsls r2, r3, #1
	lsls r0, r0, #1
	adds r3, r2, r0
	mov r1, r8
	ldrsh r3, [r1, r3]
	mov r6, sp
	lsls r3, r3, #16
	str r3, [r6]
	movs r1, #148
	lsls r1, r1, #1
	adds r3, r2, r1
	mov r2, r8
	ldrsh r3, [r2, r3]
	ldr r1, .L_0810b370
	lsls r3, r3, #16
	adds r3, r3, r1
	movs r5, #160
	lsls r5, r5, #3
	str r3, [r6, #8]
	adds r5, #12
	movs r7, #0
	add r5, r8
.L_0810b25e:
	movs r1, #168
	ldr r3, [r6, #8]
	ldr r2, [r6]
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_080c85b0
	adds r0, r5, #0
	ldr r1, .L_0810b374
	bl Func_080c85a8
	movs r1, #7
	adds r0, r5, #0
	bl Func_080c85a0
	bl Random16
	lsls r1, r0, #3
	subs r1, r1, r0
	lsrs r1, r1, #16
	ldr r0, [r5]
	bl Animation_ApplyChildValuesToRecordFar
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r5, #44]
	str r3, [r5, #40]
	movs r0, #3
	bl WaitFrames
	cmp r7, #5
	bne .L_0810b2aa
	movs r3, #161
	lsls r3, r3, #3
	add r3, r8
	mov r2, r10
	strb r2, [r3]
.L_0810b2aa:
	adds r7, #1
	adds r5, #72
	cmp r7, #17
	ble .L_0810b25e
	bl AudioCommand_WaitForStateByteClear
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #76
	movs r1, #2
	add r2, r8
	movs r7, #23
.L_0810b2c2:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_0810b2cc
	strb r1, [r2]
.L_0810b2cc:
	subs r7, #1
	adds r2, #72
	cmp r7, #0
	bge .L_0810b2c2
	movs r0, #20
	bl WaitFrames
	movs r0, #126
	bl Audio_PlayCue
	movs r2, #161
	lsls r2, r2, #3
	add r2, r8
	movs r3, #255
	strb r3, [r2]
	mov r3, r9
	adds r3, #248
	mov r1, r8
	ldr r0, [r1, r3]
	movs r1, #0
	bl Animation_ApplyChildValuesToRecordFar
	movs r0, #20
	bl WaitFrames
	movs r6, #160
	movs r5, #160
	lsls r6, r6, #3
	lsls r5, r5, #3
	adds r6, #81
	adds r5, #12
	add r6, r8
	add r5, r8
	movs r7, #23
.L_0810b310:
	ldrb r3, [r6]
	adds r6, #72
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_0810b320
	adds r0, r5, #0
	bl Func_080c85b8
.L_0810b320:
	subs r7, #1
	adds r5, #72
	cmp r7, #0
	bge .L_0810b310
	ldr r0, .L_0810b36c
	bl Func_08014644
	mov r3, r9
	adds r3, #248
	mov r2, r8
	ldr r0, [r2, r3]
	movs r1, #16
	bl Animation_ApplyChildValueFar
	bl Func_08108928
	movs r0, #30
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	add r3, r8
	ldr r3, [r3]
	mov r0, r11
	strb r0, [r3, #5]
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810b364:
	.4byte Data_0810cf28
.L_0810b368:
	.4byte 0x00202108
.L_0810b36c:
	.4byte Func_0810b168
.L_0810b370:
	.4byte 0xfff40000
.L_0810b374:
	.4byte Func_0810b0bc
