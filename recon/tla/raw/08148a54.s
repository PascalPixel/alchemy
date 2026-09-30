.syntax unified
	.thumb
	.global Func_08148a54
	.thumb_func
Func_08148a54:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r8, r0
	ldr r0, [r5, #96]
	sub sp, #32
	ldr r7, [r5, #92]
	str r0, [sp, #16]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r7, r2
	movs r3, #1
	movs r2, #1
	ldr r0, .L_08148ad4
	bl Func_08157cf4
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #31
	movs r0, #188
	str r3, [sp, #8]
	bl Func_081963ec
	adds r5, #188
	ldr r3, .L_08148ad0
	movs r2, #128
	ldr r5, [r5]
	lsls r2, r2, #19
	adds r2, #82
	mov r4, r8
	strh r3, [r2]
	ldr r0, [r4, #8]
	str r5, [sp, #12]
	bl GetBattleObjectSlotFar
	mov r1, r8
	ldr r6, [r0]
	ldr r0, [r1, #8]
	bl Func_08118070
	ldr r3, [r6, #12]
	movs r2, #0
	adds r3, r3, r0
	mov r11, r3
	movs r3, #255
	mov r10, r2
	mov r9, r3
	adds r5, r7, #0
	b .L_08148ad8
	.2byte 0x0000
.L_08148ad0:
	.4byte 0x00000f0f
.L_08148ad4:
	.4byte 0x00000191
.L_08148ad8:
	ldr r3, [r6, #8]
	mov r4, r11
	str r4, [r5, #4]
	str r3, [r5]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #127
	lsls r0, r0, #16
	asrs r0, r0, #5
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	asrs r3, r3, #6
	str r3, [r5, #16]
	bl Random16
	mov r2, r9
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #16
	asrs r0, r0, #5
	movs r3, #1
	ldr r4, .L_08148e8c
	movs r1, #1
	str r0, [r5, #20]
	negs r3, r3
	mov r0, r10
	add r10, r1
	str r3, [r5, #24]
	mov r2, r10
	movs r3, #0
	strb r3, [r4, r0]
	adds r5, #28
	cmp r2, #30
	bne .L_08148ad8
	movs r3, #0
	mov r4, r8
	mov r10, r3
	ldr r3, [r4, #20]
	cmp r3, #0
	beq .L_08148b68
	movs r0, #224
	lsls r0, r0, #2
	movs r6, #36
	adds r5, r7, r0
.L_08148b42:
	mov r1, r8
	ldrsh r0, [r6, r1]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	mov r4, r8
	ldr r3, [r2, #8]
	adds r6, #2
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	movs r3, #1
	add r10, r3
	ldr r3, [r4, #20]
	adds r5, #28
	cmp r10, r3
	bne .L_08148b42
.L_08148b68:
	movs r0, #238
	lsls r0, r0, #7
	movs r1, #238
	adds r0, #172
	lsls r1, r1, #7
	movs r2, #0
	adds r3, r7, r0
	adds r1, #176
	movs r5, #200
	str r2, [r3]
	lsls r5, r5, #4
	adds r3, r7, r1
	str r2, [r3]
	adds r1, r5, #0
	ldr r0, .L_08148e90
	bl Scheduler_AddOrUpdateCallback
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r7, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r7, r4
	movs r3, #75
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_08148e94
	bl Scheduler_AddOrUpdateCallback
	movs r0, #164
	bl Audio_PlayCue
	mov r1, r8
	ldr r3, [r1, #24]
	ldr r2, .L_08148e98
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r2, r3]
	movs r0, #0
	mov r11, r0
	cmp r3, #0
	bne .L_08148bc4
	b .L_08148e60
.L_08148bc4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	mov r3, r11
	subs r3, #17
	cmp r3, #46
	bhi .L_08148be0
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #172
	adds r2, r7, r3
	movs r3, #192
	lsls r3, r3, #1
	b .L_08148bea
.L_08148be0:
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #172
	adds r2, r7, r4
	movs r3, #0
.L_08148bea:
	str r3, [r2]
	mov r0, r8
	ldr r3, [r0, #24]
	ldr r6, .L_08148e98
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r6, r3]
	subs r3, #16
	cmp r11, r3
	bne .L_08148c04
	movs r0, #133
	bl Func_081180e8
.L_08148c04:
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Func_080156e8
	movs r1, #0
	mov r2, r8
	mov r10, r1
	ldr r1, [r2, #24]
	lsls r3, r1, #1
	ldrb r3, [r6, r3]
	cmp r3, #0
	bne .L_08148c24
	b .L_08148e18
.L_08148c24:
	movs r3, #0
	mov r9, r3
	adds r6, r7, #0
.L_08148c2a:
	cmp r11, r9
	ble .L_08148c94
	ldr r4, .L_08148e8c
	mov r0, r10
	ldrsb r3, [r4, r0]
	cmp r3, #0
	bne .L_08148c94
	add r5, sp, #20
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #8]
	cmp r3, #159
	bgt .L_08148c52
	movs r3, #160
	str r3, [r5, #8]
.L_08148c52:
	movs r2, #136
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	ble .L_08148c5e
	str r2, [r5, #8]
.L_08148c5e:
	ldr r2, [r5]
	ldr r3, [r5, #4]
	movs r1, #12
	movs r4, #152
	str r1, [sp, #0]
	lsls r4, r4, #5
	movs r1, #24
	subs r2, #6
	subs r3, #12
	str r1, [sp, #4]
	ldr r0, [sp, #16]
	adds r1, r7, r4
	ldr r4, [sp, #8]
	mov lr, r4
	.2byte 0xf800
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
.L_08148c94:
	mov r3, r9
	adds r3, #48
	cmp r11, r3
	ble .L_08148d92
	ldr r0, .L_08148e8c
	mov r1, r10
	ldrsb r3, [r0, r1]
	cmp r3, #0
	bne .L_08148d92
	mov r2, r8
	ldr r1, [r2, #20]
	mov r0, r10
	bl __modsi3
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	movs r3, #224
	lsls r3, r3, #2
	adds r1, r7, r1
	adds r1, r1, r3
	ldr r3, [r1]
	ldr r2, [r6]
	subs r3, r3, r2
	ldr r2, [r6, #12]
	asrs r3, r3, #9
	adds r0, r2, r3
	str r0, [r6, #12]
	ldr r2, [r6, #4]
	ldr r3, [r1, #4]
	subs r3, r3, r2
	ldr r2, [r6, #16]
	asrs r3, r3, #9
	adds r4, r2, r3
	str r4, [r6, #16]
	ldr r2, [r6, #8]
	ldr r3, [r1, #8]
	subs r3, r3, r2
	ldr r2, [r6, #20]
	asrs r3, r3, #9
	adds r1, r2, r3
	mov r3, r9
	adds r3, #85
	str r1, [r6, #20]
	cmp r11, r3
	bge .L_08148d20
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08148cfc
	adds r2, #63
.L_08148cfc:
	asrs r3, r2, #6
	str r3, [r6, #12]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08148d0c
	adds r2, #63
.L_08148d0c:
	asrs r3, r2, #6
	str r3, [r6, #16]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_08148d1c
	adds r2, #63
.L_08148d1c:
	asrs r3, r2, #6
	str r3, [r6, #20]
.L_08148d20:
	ldr r3, [r6, #4]
	cmp r3, #0
	bge .L_08148d92
	ldr r4, .L_08148e8c
	movs r3, #1
	mov r0, r10
	strb r3, [r4, r0]
	movs r3, #0
	str r3, [r6, #24]
	add r5, sp, #20
	ldr r3, [r5]
	str r3, [r6]
	bl Random16
	ldr r3, [r5, #4]
	movs r2, #31
	ands r2, r0
	adds r3, r3, r2
	subs r3, #16
	str r3, [r6, #4]
	mov r2, r8
	ldr r1, [r2, #20]
	mov r0, r10
	bl __modsi3
	adds r3, r0, #0
	lsls r2, r3, #1
	mov r4, r8
	adds r2, #36
	ldrsh r0, [r4, r2]
	movs r2, #4
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #5
	bl Func_0814cd48
	mov r3, r8
	ldr r1, [r3, #20]
	mov r0, r10
	bl __modsi3
	lsls r0, r0, #1
	mov r4, r8
	adds r0, #36
	ldrsh r0, [r4, r0]
	movs r1, #0
	bl Func_08118088
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	adds r3, r7, r2
	movs r4, #4
	str r4, [r3]
	movs r0, #132
	bl Audio_PlayCue
.L_08148d92:
	ldr r3, [r6, #24]
	cmp r3, #15
	bhi .L_08148dfe
	lsrs r0, r3, #31
	adds r0, r3, r0
	movs r1, #3
	asrs r0, r0, #1
	bl __modsi3
	adds r1, r0, #0
	ldr r2, [r6]
	ldr r3, [r6, #4]
	lsls r1, r1, #10
	movs r0, #224
	lsls r0, r0, #3
	adds r1, r7, r1
	adds r1, r1, r0
	movs r4, #16
	movs r0, #64
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #8]
	subs r2, #16
	ldr r0, [sp, #16]
	subs r3, #56
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r6, #24]
	movs r1, #3
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl __modsi3
	adds r1, r0, #0
	ldr r3, [r6, #4]
	lsls r1, r1, #10
	movs r0, #224
	lsls r0, r0, #3
	adds r1, r7, r1
	adds r1, r1, r0
	movs r4, #16
	movs r0, #64
	ldr r2, [r6]
	subs r3, #56
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #12]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
.L_08148dfe:
	movs r1, #1
	mov r2, r8
	add r10, r1
	ldr r1, [r2, #24]
	ldr r3, .L_08148e98
	lsls r2, r1, #1
	ldrb r3, [r3, r2]
	movs r0, #2
	add r9, r0
	adds r6, #28
	cmp r10, r3
	beq .L_08148e18
	b .L_08148c2a
.L_08148e18:
	lsls r1, r1, #1
	adds r1, #2
	adds r0, r1, #0
	bl Func_08158ce0
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #176
	adds r2, r7, r3
	ldr r3, [r2]
	cmp r3, #0
	bne .L_08148e34
	movs r3, #1
	str r3, [r2]
.L_08148e34:
	bl Func_081434f8
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r7, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	mov r1, r8
	ldr r3, [r1, #24]
	ldr r2, .L_08148e98
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r2, r3]
	movs r0, #1
	add r11, r0
	cmp r11, r3
	beq .L_08148e60
	b .L_08148bc4
.L_08148e60:
	ldr r0, .L_08148e90
	bl Scheduler_RemoveCallback
	ldr r0, .L_08148e94
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08148e8c:
	.4byte gMapCellBuffer
.L_08148e90:
	.4byte Func_0814c928
.L_08148e94:
	.4byte Func_08143000
.L_08148e98:
	.4byte Data_081979a8
