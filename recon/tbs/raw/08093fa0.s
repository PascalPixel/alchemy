.syntax unified
	.thumb
	.global battle_owner_69
	.thumb_func
battle_owner_69:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_08094120
	ldr r0, .L_08094124
	mov r9, r0
	ldr r0, [r1]
	sub sp, #24
	bl Object_GetById
	adds r7, r0, #0
	movs r3, #10
	ldrsh r5, [r7, r3]
	movs r1, #18
	ldrsh r6, [r7, r1]
	ldr r3, .L_08094128
	movs r2, #1
	ands r5, r3
	ands r6, r3
	mov r11, r2
	movs r0, #8
	movs r2, #8
	adds r0, r0, r5
	adds r2, r2, r6
	mov r8, r0
	mov r10, r2
	bl Battle_Reset
	adds r3, r7, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_08093ff2
	ldr r3, [r7, #80]
	adds r3, #38
	ldrb r3, [r3]
	mov r11, r3
.L_08093ff2:
	movs r3, #249
	lsls r3, r3, #1
	add r9, r3
	mov r0, r9
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_080940b8
	mov r3, r8
	cmp r3, #0
	bge .L_0809400a
	adds r3, r5, #0
	adds r3, #23
.L_0809400a:
	asrs r2, r3, #4
	mov r3, r10
	cmp r3, #0
	bge .L_08094016
	adds r3, r6, #0
	adds r3, #23
.L_08094016:
	asrs r3, r3, #4
	lsls r3, r3, #7
	adds r3, r2, r3
	ldr r1, .L_0809412c
	ldr r0, .L_08094130
	lsls r3, r3, #2
	adds r2, r3, r1
	adds r3, r3, r0
	ldrb r2, [r2, #2]
	ldrb r3, [r3, #2]
	cmp r2, r3
	beq .L_08094030
	b .L_08094138
.L_08094030:
	ldr r3, [r7, #8]
	mov r0, sp
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl Func_08009220
	adds r5, r0, #0
	cmp r5, #0
	bne .L_08094138
	adds r6, r7, #0
	adds r6, #90
	ldr r1, .L_08094120
	strb r5, [r6]
	mov r2, r10
	ldr r0, [r1]
	mov r1, r8
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #6
	adds r0, r7, #0
	bl Func_08009080
	movs r0, #4
	bl WaitFrames
	movs r1, #7
	adds r0, r7, #0
	bl Func_08009080
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	movs r0, #4
	bl WaitFrames
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	mov r2, r11
	movs r3, #254
	ands r2, r3
	mov r11, r2
	adds r0, r7, #0
	mov r1, r11
	bl Func_080091e0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	adds r0, r7, #0
	movs r1, #12
	str r5, [r7, #40]
	bl Func_08009080
	movs r0, #4
	bl WaitFrames
	movs r3, #1
	mov r0, r9
	strb r3, [r0]
	strb r3, [r6]
	movs r0, #8
	bl WaitFrames
	b .L_08094112
.L_080940b8:
	adds r5, r7, #0
	adds r5, #85
	movs r6, #0
	strb r6, [r5]
	adds r0, r7, #0
	movs r1, #11
	bl Func_08009080
	mov r2, r8
	lsls r1, r2, #16
	movs r3, #128
	ldr r2, [r7, #12]
	lsls r3, r3, #12
	mov r0, r10
	adds r2, r2, r3
	lsls r3, r0, #16
	ldr r0, .L_08094134
	adds r3, r3, r0
	adds r0, r7, #0
	bl Object_SetMoveTargetFar
	ldr r1, .L_08094120
	ldr r0, [r1]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r5]
	ldr r5, .L_0809411c
	mov r2, r11
	ldr r3, [r7, #12]
	orrs r2, r5
	mov r11, r2
	str r3, [r7, #20]
	adds r0, r7, #0
	mov r1, r11
	bl Func_080091e0
	movs r0, #4
	bl Battle_WaitMode0
	mov r3, r9
	strb r6, [r3]
	adds r3, r7, #0
	adds r3, #90
	strb r5, [r3]
.L_08094112:
	bl BattleFx_FinishAction
	movs r0, #0
	b .L_08094140
	.2byte 0x0000
.L_0809411c:
	.4byte 0x00000001
.L_08094120:
	.4byte Data_02000434
.L_08094124:
	.4byte gCell
.L_08094128:
	.4byte 0x0000fff0
.L_0809412c:
	.4byte gMapCellBuffer
.L_08094130:
	.4byte Data_0200fe00
.L_08094134:
	.4byte 0xfff00000
.L_08094138:
	bl BattleFx_FinishAction
	movs r0, #1
	negs r0, r0
.L_08094140:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
