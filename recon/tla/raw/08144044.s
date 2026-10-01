.syntax unified
	.thumb
	.global Func_08144044
	.thumb_func
Func_08144044:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #76
	str r0, [sp, #40]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	movs r0, #0
	str r1, [sp, #36]
	movs r5, #200
	ldr r3, [r3, #96]
	lsls r5, r5, #4
	str r3, [sp, #32]
	bl BattleFx_BeginCanvasLayer
	ldr r2, [sp, #36]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08144348
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_0814434c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08144350
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	add r1, sp, #44
	bl Func_08144aac
	adds r1, r5, #0
	ldr r0, .L_08144354
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #36]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #3
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	ldr r3, .L_08144358
	adds r1, r5, #0
	str r3, [r2]
	ldr r0, .L_0814435c
	ldr r5, .L_08144360
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	mov r10, r1
	movs r6, #255
.L_081440ca:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	movs r2, #1
	movs r3, #128
	lsls r0, r0, #15
	add r10, r2
	lsls r3, r3, #2
	str r0, [r5, #8]
	adds r5, #28
	cmp r10, r3
	bne .L_081440ca
	movs r0, #142
	bl Audio_PlayCue
	movs r1, #0
	str r1, [sp, #28]
	ldr r2, [sp, #40]
	subs r1, #64
	ldr r3, [r2, #20]
	lsls r3, r3, #5
	cmp r3, r1
	bne .L_08144114
	b .L_0814431e
.L_08144114:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	ldr r2, [sp, #28]
	str r3, [sp, #24]
	cmp r2, #92
	bne .L_08144128
	movs r0, #0
	bl Func_081180e8
.L_08144128:
	ldr r3, [sp, #36]
	ldr r2, [sp, #40]
	movs r1, #225
	lsls r1, r1, #7
	adds r6, r3, r1
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08144168
	ldr r1, [sp, #28]
	movs r3, #0
	mov r10, r3
	lsls r5, r1, #11
.L_08144140:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r2, r0, #1
	adds r2, r2, r0
	movs r3, #192
	lsls r2, r2, #1
	lsls r3, r3, #11
	subs r3, r3, r2
	asrs r3, r3, #10
	stmia r6!, {r3}
	movs r3, #1
	movs r2, #128
	add r10, r3
	lsls r2, r2, #4
	mov r1, r10
	adds r5, r5, r2
	cmp r1, #160
	bne .L_08144140
	b .L_08144190
.L_08144168:
	ldr r3, [sp, #28]
	movs r2, #0
	mov r10, r2
	lsls r5, r3, #11
.L_08144170:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	movs r2, #1
	asrs r3, r3, #10
	movs r1, #128
	add r10, r2
	stmia r6!, {r3}
	lsls r1, r1, #4
	mov r3, r10
	adds r5, r5, r1
	cmp r3, #160
	bne .L_08144170
.L_08144190:
	ldr r2, [sp, #40]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r11, r1
	cmp r3, #0
	bne .L_0814419e
	b .L_081442f2
.L_0814419e:
	ldr r3, [sp, #24]
	movs r2, #36
	adds r3, #12
	str r3, [sp, #16]
	movs r3, #0
	movs r1, #52
	str r2, [sp, #12]
	str r3, [sp, #8]
	add r1, sp
	mov r9, r1
.L_081441b2:
	ldr r1, [sp, #12]
	ldr r3, [sp, #40]
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	bl Func_08014de4
	ldr r0, [sp, #24]
	ldr r1, [sp, #16]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	mov r1, r9
	str r3, [r1]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r1, #4]
	mov r0, r9
	ldr r3, [r5, #16]
	str r3, [r1, #8]
	bl SceneTransform_ApplyPosition
	ldr r3, [sp, #28]
	mov r2, r11
	lsls r5, r2, #5
	cmp r3, r5
	ble .L_081442d4
	lsls r0, r3, #9
	bl SceneTransform_ApplyPitch
	ldr r1, [sp, #28]
	adds r3, r5, #0
	adds r3, #32
	cmp r1, r3
	bne .L_0814420e
	ldr r2, [sp, #12]
	ldr r1, [sp, #40]
	ldrsh r0, [r2, r1]
	movs r3, #32
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r11
	bl Func_0814cd48
.L_0814420e:
	mov r3, r11
	movs r2, #0
	lsls r3, r3, #3
	mov r10, r2
	str r3, [sp, #20]
	ldr r2, [sp, #8]
	ldr r3, .L_08144360
	add r1, sp, #64
	mov r8, r1
	adds r6, r2, r3
.L_08144222:
	ldr r3, [sp, #20]
	ldr r1, [sp, #28]
	add r3, r10
	lsls r3, r3, #2
	cmp r1, r3
	ble .L_081442c8
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	adds r0, r0, r2
	asrs r3, r3, #8
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_08144364
	mov lr, r3
	.2byte 0xf800
	asrs r7, r0, #8
	cmp r7, #0
	beq .L_081442c8
	mov r1, r8
	adds r0, r6, #0
	bl Func_0815e1ec
	mov r2, r8
	ldr r5, [r2]
	movs r1, #3
	asrs r5, r5, #1
	str r5, [r2]
	mov r0, r10
	bl __modsi3
	ldr r3, [sp, #36]
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #6
	movs r2, #224
	adds r1, r3, r1
	lsls r2, r2, #3
	adds r1, r1, r2
	mov r2, r8
	ldr r3, [r2, #4]
	subs r5, #12
	movs r2, #24
	subs r3, #12
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r4, [sp, #44]
	adds r2, r5, #0
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [r6]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	ldr r3, [r6, #24]
	subs r5, r5, r0
	adds r3, #1
	str r5, [r6, #8]
	str r3, [r6, #24]
.L_081442c8:
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r6, #28
	cmp r1, #8
	bne .L_08144222
.L_081442d4:
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	movs r1, #224
	lsls r1, r1, #3
	adds r3, r3, r1
	adds r2, #2
	str r2, [sp, #12]
	str r3, [sp, #8]
	ldr r1, [sp, #40]
	movs r2, #1
	ldr r3, [r1, #20]
	add r11, r2
	cmp r11, r3
	beq .L_081442f2
	b .L_081441b2
.L_081442f2:
	bl Func_081434f8
	movs r1, #240
	ldr r3, [sp, #36]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r3, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	ldr r1, [sp, #40]
	adds r2, #1
	str r2, [sp, #28]
	ldr r3, [r1, #20]
	lsls r3, r3, #5
	adds r3, #64
	cmp r2, r3
	beq .L_0814431e
	b .L_08144114
.L_0814431e:
	ldr r0, .L_0814435c
	bl Scheduler_RemoveCallback
	ldr r0, .L_08144354
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08144348:
	.4byte 0x0000016d
.L_0814434c:
	.4byte 0x0000017f
.L_08144350:
	.4byte IwramCopyWords
.L_08144354:
	.4byte Func_08152474
.L_08144358:
	.4byte 0x04040404
.L_0814435c:
	.4byte Func_08143000
.L_08144360:
	.4byte gMapCellBuffer
.L_08144364:
	.4byte IwramFillWords + 0x74
