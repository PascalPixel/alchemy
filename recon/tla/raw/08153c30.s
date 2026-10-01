.syntax unified
	.thumb
	.global Func_08153c30
	.thumb_func
Func_08153c30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #72
	str r0, [sp, #44]
	movs r5, #192
	lsls r5, r5, #18
	ldr r1, [r5, #92]
	ldr r2, [r5, #96]
	movs r0, #0
	str r2, [sp, #40]
	mov r9, r1
	bl BattleFx_BeginCanvasLayer
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08153ea8
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #136
	lsls r1, r1, #6
	add r1, r9
	movs r2, #0
	movs r3, #0
	ldr r0, .L_08153eac
	bl Resource_LoadAndDecompress
	ldr r0, .L_08153eb0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08153eb4
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r0, #188
	movs r1, #19
	str r3, [sp, #28]
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	movs r4, #0
	str r5, [sp, #32]
	mov r10, r4
	mov r8, r4
	mov r5, r9
.L_08153caa:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r7, r0, #0
	ands r7, r3
	bl Random16
	mov r1, r8
	adds r6, r0, #0
	movs r3, #255
	str r1, [r5]
	str r1, [r5, #4]
	str r1, [r5, #8]
	mov r0, r10
	movs r1, #6
	ands r6, r3
	bl Math_Mod
	cmp r0, #5
	bne .L_08153cde
	mov r2, r8
	str r2, [r5, #12]
	str r2, [r5, #16]
	b .L_08153cfa
.L_08153cde:
	adds r0, r7, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r5, #12]
	adds r0, r7, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r5, #16]
.L_08153cfa:
	movs r4, #1
	add r10, r4
	mov r3, r8
	mov r1, r10
	str r3, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #64
	bne .L_08153caa
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
	ldr r0, .L_08153eb8
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	str r2, [sp, #36]
.L_08153d30:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	ldr r4, [sp, #44]
	str r3, [sp, #20]
	movs r3, #0
	str r3, [sp, #24]
	ldr r3, [r4, #20]
	cmp r3, #0
	bne .L_08153d46
	b .L_08153e66
.L_08153d46:
	ldr r1, [sp, #20]
	movs r3, #0
	adds r1, #12
	movs r4, #36
	movs r2, #48
	str r1, [sp, #16]
	str r3, [sp, #12]
	str r4, [sp, #8]
	add r2, sp
	mov r8, r2
.L_08153d5a:
	ldr r1, [sp, #8]
	ldr r3, [sp, #44]
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	bl Func_08014de4
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	mov r4, r8
	str r3, [r4]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r4, #4]
	mov r0, r8
	ldr r3, [r5, #16]
	str r3, [r4, #8]
	bl SceneTransform_ApplyPosition
	ldr r1, [sp, #24]
	ldr r2, [sp, #36]
	lsls r3, r1, #3
	cmp r2, r3
	blt .L_08153e4a
	adds r3, #40
	cmp r2, r3
	bge .L_08153e4a
	ldr r4, [sp, #12]
	movs r3, #0
	mov r10, r3
	lsls r3, r4, #3
	subs r3, r3, r4
	add r7, sp, #60
	lsls r3, r3, #2
	mov r1, r9
	mov r11, r7
	adds r6, r3, r1
.L_08153dac:
	ldr r0, [r6, #24]
	movs r1, #6
	bl Math_Div
	adds r5, r0, #0
	cmp r5, #5
	ble .L_08153dbc
	movs r5, #5
.L_08153dbc:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0815e1ec
	mov r2, r11
	ldr r3, [r2]
	asrs r3, r3, #1
	str r3, [r2]
	mov r3, r10
	cmp r3, #5
	bne .L_08153e06
	movs r2, #128
	adds r0, r6, #0
	movs r1, #62
	lsls r2, r2, #4
	bl BattleFxKernels_IntegrateVector3
	lsls r1, r5, #3
	ldr r2, [r7]
	ldr r3, [r7, #4]
	adds r1, r1, r5
	movs r0, #24
	lsls r1, r1, #7
	movs r4, #136
	lsls r4, r4, #6
	add r1, r9
	str r0, [sp, #0]
	movs r0, #48
	adds r1, r1, r4
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #36
	ldr r0, [sp, #40]
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	b .L_08153e38
.L_08153e06:
	movs r2, #128
	adds r0, r6, #0
	movs r1, #60
	lsls r2, r2, #2
	bl BattleFxKernels_IntegrateVector3
	lsls r1, r5, #3
	adds r1, r1, r5
	lsls r1, r1, #7
	movs r2, #224
	lsls r2, r2, #3
	add r1, r9
	ldr r3, [r7, #4]
	adds r1, r1, r2
	ldr r2, [r7]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #36
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_08153e38:
	ldr r3, [r6, #24]
	movs r1, #1
	add r10, r1
	adds r3, #1
	mov r2, r10
	str r3, [r6, #24]
	adds r6, #28
	cmp r2, #6
	bne .L_08153dac
.L_08153e4a:
	ldr r3, [sp, #12]
	ldr r4, [sp, #8]
	ldr r1, [sp, #24]
	adds r3, #6
	adds r4, #2
	adds r1, #1
	str r3, [sp, #12]
	str r4, [sp, #8]
	str r1, [sp, #24]
	ldr r2, [sp, #44]
	ldr r3, [r2, #20]
	cmp r1, r3
	beq .L_08153e66
	b .L_08153d5a
.L_08153e66:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #36]
	adds r3, #1
	str r3, [sp, #36]
	cmp r3, #96
	beq .L_08153e84
	b .L_08153d30
.L_08153e84:
	ldr r0, .L_08153eb8
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08153ea8:
	.4byte 0x00000161
.L_08153eac:
	.4byte 0x0000012d
.L_08153eb0:
	.4byte 0x0000017f
.L_08153eb4:
	.4byte IwramCopyWords
.L_08153eb8:
	.4byte Func_08143000
