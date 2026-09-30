.syntax unified
	.thumb
	.global Func_0818c7b8
	.thumb_func
Func_0818c7b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r0, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	ldr r3, [r3, #96]
	mov r11, r1
	movs r0, #0
	mov r8, r3
	mov r10, r2
	bl Func_08143a88
	mov r3, r11
	cmp r3, #0
	bne .L_0818c7f2
	ldr r2, [sp, #8]
	movs r4, #3
	ldr r3, [r2, #4]
	cmp r3, #0
	beq .L_0818c7fe
	movs r4, #7
	b .L_0818c7fe
.L_0818c7f2:
	ldr r2, [sp, #8]
	movs r4, #11
	ldr r3, [r2, #4]
	cmp r3, #0
	beq .L_0818c7fe
	movs r4, #15
.L_0818c7fe:
	movs r3, #1
	str r3, [sp, #0]
	adds r3, r4, #0
	movs r1, #7
	movs r2, #7
	movs r0, #104
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #104]
	ldr r0, .L_0818c870
	ldr r1, .L_0818c874
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	movs r3, #0
	ldr r0, .L_0818c878
	add r1, r10
	movs r2, #1
	bl Func_08157cf4
	mov r3, r11
	cmp r3, #0
	bne .L_0818c884
	ldr r0, .L_0818c87c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818c880
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0818c86c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
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
	b .L_0818c8a2
	.2byte 0x0000
.L_0818c86c:
	.4byte 0x00000810
.L_0818c870:
	.4byte 0x0000010b
.L_0818c874:
	.4byte gMapCellBuffer
.L_0818c878:
	.4byte 0x0000010c
.L_0818c87c:
	.4byte 0x00000150
.L_0818c880:
	.4byte IwramCopyWords
.L_0818c884:
	ldr r3, .L_0818c8c4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #75
.L_0818c8a2:
	str r3, [r2]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0818c8c8
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #8]
	movs r3, #36
	ldrsh r1, [r2, r3]
	movs r3, #128
	ldr r0, [r2, #8]
	lsls r3, r3, #11
	movs r2, #8
	bl BattleMotion_ApproachTargetFar
	b .L_0818c8cc
	.2byte 0x0000
.L_0818c8c4:
	.4byte 0x00001010
.L_0818c8c8:
	.4byte Func_08143000
.L_0818c8cc:
	movs r2, #0
	mov r9, r2
.L_0818c8d0:
	mov r3, r9
	cmp r3, #0
	bne .L_0818c8e2
	ldr r3, [sp, #8]
	add r1, sp, #12
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_0815e21c
.L_0818c8e2:
	mov r2, r9
	cmp r2, #8
	bne .L_0818c8ee
	movs r0, #212
	bl Audio_PlayCue
.L_0818c8ee:
	mov r3, r9
	cmp r3, #12
	bne .L_0818c92e
	mov r2, r11
	cmp r2, #1
	bne .L_0818c902
	movs r0, #144
	bl Func_081180e8
	b .L_0818c908
.L_0818c902:
	movs r0, #134
	bl Func_081180e8
.L_0818c908:
	ldr r2, [sp, #8]
	movs r1, #250
	movs r3, #36
	ldrsh r0, [r2, r3]
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #11
	lsls r3, r3, #13
	str r1, [sp, #4]
	movs r1, #1
	str r2, [sp, #0]
	bl Func_0815f000
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #4
	str r3, [r2]
.L_0818c92e:
	mov r3, r9
	cmp r3, #7
	bgt .L_0818c936
	b .L_0818ca42
.L_0818c936:
	mov r0, r9
	subs r0, #8
	movs r1, #3
	bl Math_Div
	mov r2, r11
	negs r3, r2
	orrs r3, r2
	ldr r2, [sp, #8]
	lsrs r5, r3, #31
	ldr r3, [r2, #4]
	lsls r5, r5, #4
	cmp r3, #0
	bne .L_0818c960
	ldr r3, [sp, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r3, #0
	subs r4, #48
	b .L_0818c96c
.L_0818c960:
	ldr r3, [sp, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r3, #0
	subs r4, #80
.L_0818c96c:
	mov r3, r11
	cmp r3, #0
	bne .L_0818c9b8
	cmp r0, #4
	bhi .L_0818ca42
	ldr r2, .L_0818ca8c
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0818c980:
	.4byte .L_0818c9e8
	.4byte .L_0818c9d8
	.4byte .L_0818c994
	.4byte .L_0818c9a2
	.4byte .L_0818c9b0
.L_0818c994:
	movs r3, #128
	str r3, [sp, #0]
	str r3, [sp, #4]
	subs r2, r4, #4
	mov r0, r8
	ldr r1, .L_0818ca90
	b .L_0818c9f8
.L_0818c9a2:
	movs r3, #128
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r1, .L_0818ca94
	subs r2, r4, #4
	mov r0, r8
	b .L_0818c9f8
.L_0818c9b0:
	movs r1, #253
	movs r3, #120
	lsls r1, r1, #6
	b .L_0818c9ee
.L_0818c9b8:
	cmp r0, #4
	bhi .L_0818ca42
	ldr r2, .L_0818ca98
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0818c9c4:
	.4byte .L_0818c9e8
	.4byte .L_0818c9d8
	.4byte .L_0818ca00
	.4byte .L_0818ca16
	.4byte .L_0818ca2c
.L_0818c9d8:
	movs r1, #128
	ldr r3, .L_0818ca9c
	mov r0, r8
	lsls r1, r1, #7
	ldr r2, .L_0818caa0
	mov lr, r3
	.2byte 0xf800
	b .L_0818ca42
.L_0818c9e8:
	movs r1, #224
	movs r3, #120
	lsls r1, r1, #3
.L_0818c9ee:
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r1, r10
	mov r0, r8
	adds r2, r4, #0
.L_0818c9f8:
	adds r3, r5, #0
	mov lr, r6
	.2byte 0xf800
	b .L_0818ca42
.L_0818ca00:
	movs r1, #128
	adds r3, r5, #0
	str r1, [sp, #0]
	str r1, [sp, #4]
	subs r2, r4, #4
	subs r3, #8
	mov r0, r8
	ldr r1, .L_0818ca90
	mov lr, r6
	.2byte 0xf800
	b .L_0818ca42
.L_0818ca16:
	movs r0, #128
	adds r3, r5, #0
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r1, .L_0818ca94
	subs r2, r4, #4
	subs r3, #8
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	b .L_0818ca42
.L_0818ca2c:
	movs r1, #253
	movs r3, #120
	lsls r1, r1, #6
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r1, r10
	mov r0, r8
	adds r2, r4, #0
	adds r3, r5, #0
	mov lr, r6
	.2byte 0xf800
.L_0818ca42:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #32
	beq .L_0818ca6c
	b .L_0818c8d0
.L_0818ca6c:
	ldr r0, .L_0818caa4
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0818ca8c:
	.4byte .L_0818c980
.L_0818ca90:
	.4byte gMapCellBuffer
.L_0818ca94:
	.4byte Data_02014000
.L_0818ca98:
	.4byte .L_0818c9c4
.L_0818ca9c:
	.4byte IwramFillWords
.L_0818caa0:
	.4byte 0x3f3f3f3f
.L_0818caa4:
	.4byte Func_08143000
