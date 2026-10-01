.syntax unified
	.thumb
	.global Func_0816aa08
	.thumb_func
Func_0816aa08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	sub sp, #56
	mov r11, r0
	ldr r0, [r3, #92]
	str r1, [sp, #32]
	mov r9, r0
	ldr r3, [r3, #100]
	movs r0, #0
	str r3, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816aa70
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	add r1, sp, #36
	strh r3, [r2]
	movs r0, #0
	bl Func_08144aac
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
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816aa74
	bl Scheduler_AddOrUpdateCallback
	mov r3, r11
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlotFar
	b .L_0816aa78
	.2byte 0x0000
.L_0816aa70:
	.4byte 0x00001010
.L_0816aa74:
	.4byte Func_08143000
.L_0816aa78:
	movs r1, #156
	ldr r0, [r0]
	lsls r1, r1, #7
	adds r1, #16
	str r0, [sp, #20]
	movs r4, #0
	add r1, r9
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0816ae04
	str r4, [sp, #16]
	bl Resource_LoadAndDecompress
	ldr r0, .L_0816ae08
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816ae0c
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0816ae10
	ldr r1, [sp, #24]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0816ae14
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #156
	lsls r1, r1, #6
	ldr r0, .L_0816ae18
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, .L_0816ae1c
	movs r0, #0
	movs r1, #1
	movs r2, #128
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #3
.L_0816aae0:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0816aae0
	movs r2, #224
	mov r1, sp
	lsls r2, r2, #3
	movs r0, #0
	adds r1, #44
	add r2, r9
	str r0, [sp, #28]
	str r1, [sp, #8]
	str r2, [sp, #12]
.L_0816aafe:
	mov r4, r11
	movs r3, #36
	ldrsh r0, [r4, r3]
	ldr r1, [sp, #8]
	bl Func_0815e21c
	ldr r0, [sp, #28]
	cmp r0, #0
	bne .L_0816abb0
	ldr r2, [sp, #8]
	movs r1, #0
	mov r8, r1
	mov r10, r2
	mov r7, r9
.L_0816ab1a:
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	ldr r3, .L_0816ae20
	lsls r5, r5, #7
	adds r5, #255
	mov r4, r10
	ands r5, r0
	adds r5, r5, r3
	ldr r3, [r4]
	adds r0, r5, #0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	adds r6, #32
	ldr r3, [r4, #4]
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	movs r1, #1
	asrs r3, r3, #8
	mov r0, r8
	add r8, r1
	str r3, [r7, #16]
	mov r2, r8
	negs r3, r0
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #64
	bne .L_0816ab1a
	ldr r4, [sp, #20]
	movs r3, #0
	str r3, [r4, #72]
	str r3, [r4, #12]
	ldr r0, [sp, #8]
	mov r2, r11
	ldr r0, [r0, #4]
	str r0, [sp, #16]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #1
	negs r1, r1
	movs r2, #5
	str r1, [sp, #0]
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #4
	str r3, [r2]
	movs r0, #144
	bl Audio_PlayCue
.L_0816abb0:
	ldr r3, [sp, #28]
	cmp r3, #32
	bne .L_0816ac62
	ldr r0, [sp, #8]
	ldr r7, .L_0816ae24
	movs r4, #0
	mov r8, r4
	mov r10, r0
.L_0816abc0:
	bl Random16
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #255
	ands r5, r0
	bl Random16
	mov r1, r10
	ldr r3, [r1]
	adds r6, r0, #0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	adds r5, #32
	ldr r3, [r1, #4]
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #48
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r8, r3
	bne .L_0816abc0
	mov r1, r11
	movs r4, #36
	ldrsh r0, [r1, r4]
	movs r1, #4
	bl Func_08118088
	mov r3, r11
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #8
	str r3, [r2]
	movs r0, #145
	bl Func_081180e8
	movs r1, #128
	ldr r3, .L_0816ae28
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_0816ae2c
	mov lr, r3
	.2byte 0xf800
.L_0816ac62:
	ldr r4, [sp, #28]
	cmp r4, #23
	bhi .L_0816ac74
	ldr r0, [sp, #20]
	movs r1, #128
	ldr r3, [r0, #12]
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r0, #12]
.L_0816ac74:
	ldr r2, [sp, #28]
	cmp r2, #18
	bne .L_0816ac8a
	mov r3, r11
	ldr r0, [r3, #8]
	movs r4, #36
	ldrsh r1, [r3, r4]
	movs r2, #8
	movs r3, #80
	bl Func_08157530
.L_0816ac8a:
	ldr r0, [sp, #28]
	cmp r0, #21
	bne .L_0816aca2
	mov r1, r11
	movs r3, #128
	ldr r0, [r1, #8]
	lsls r3, r3, #12
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #12
	bl BattleMotion_ApproachTargetFar
.L_0816aca2:
	ldr r4, .L_0816ae30
	ldr r7, .L_0816ae34
	movs r3, #0
	mov r8, r3
	mov r10, r4
.L_0816acac:
	ldr r6, [r7, #24]
	cmp r6, #0
	blt .L_0816acfc
	adds r0, r6, #0
	movs r1, #6
	bl Math_Div
	adds r5, r0, #0
	cmp r5, #1
	bgt .L_0816acc2
	movs r5, #2
.L_0816acc2:
	subs r3, r6, #1
	movs r2, #128
	str r3, [r7, #24]
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	lsls r0, r5, #1
	subs r3, r0, #2
	mov r2, r10
	ldrh r1, [r2, r3]
	ldr r3, [sp, #24]
	movs r4, #2
	ldrsh r2, [r7, r4]
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r7, r4]
	str r0, [sp, #4]
	subs r3, r3, r5
	str r5, [sp, #0]
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_0816acfc:
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #2
	adds r7, #28
	cmp r8, r1
	bne .L_0816acac
	movs r2, #0
	mov r8, r2
	mov r6, r9
.L_0816ad10:
	ldr r3, [r6, #24]
	adds r0, r3, #1
	str r0, [r6, #24]
	cmp r0, #17
	bhi .L_0816ad54
	movs r1, #3
	bl Math_Div
	movs r1, #60
	adds r5, r0, #0
	ldr r2, .L_0816ae38
	adds r0, r6, #0
	bl BattleFxKernels_IntegrateVector2
	lsls r5, r5, #11
	movs r3, #156
	lsls r3, r3, #6
	add r5, r9
	movs r4, #2
	ldrsh r2, [r6, r4]
	adds r5, r5, r3
	movs r0, #6
	ldrsh r3, [r6, r0]
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	subs r2, #16
	subs r3, #32
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_0816ad54:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #28
	cmp r2, #32
	bne .L_0816ad10
	ldr r3, [sp, #28]
	cmp r3, #31
	bhi .L_0816adbc
	movs r4, #104
	lsls r0, r3, #3
	movs r1, #104
	mov r8, r4
	bl __modsi3
	mov r10, r0
	ldr r0, [sp, #8]
	ldr r1, [sp, #16]
	ldr r2, [r0]
	movs r6, #34
	lsrs r3, r2, #31
	adds r2, r2, r3
	mov r3, r10
	subs r5, r1, r3
	adds r3, r5, #0
	mov r4, r8
	asrs r2, r2, #1
	subs r2, #17
	subs r3, #96
	str r4, [sp, #4]
	ldr r1, [sp, #12]
	ldr r4, [sp, #36]
	str r6, [sp, #0]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [sp, #8]
	mov r1, r10
	ldr r2, [r0]
	adds r5, #8
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r1, [sp, #4]
	subs r2, #17
	str r6, [sp, #0]
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	ldr r1, [sp, #12]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_0816adbc:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	adds r2, #1
	str r2, [sp, #28]
	cmp r2, #80
	beq .L_0816adde
	b .L_0816aafe
.L_0816adde:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816ae3c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816ae04:
	.4byte 0x000000da
.L_0816ae08:
	.4byte 0x00000155
.L_0816ae0c:
	.4byte IwramCopyWords
.L_0816ae10:
	.4byte 0x00000134
.L_0816ae14:
	.4byte 0x00000146
.L_0816ae18:
	.4byte 0x0000013e
.L_0816ae1c:
	.4byte Data_02010018
.L_0816ae20:
	.4byte 0xffffc000
.L_0816ae24:
	.4byte Data_02011c00
.L_0816ae28:
	.4byte IwramFillWords
.L_0816ae2c:
	.4byte 0x3f3f3f3f
.L_0816ae30:
	.4byte Data_08197410
.L_0816ae34:
	.4byte gMapCellBuffer
.L_0816ae38:
	.4byte 0xffffe000
.L_0816ae3c:
	.4byte Func_08143000
