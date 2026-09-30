.syntax unified
	.thumb
	.global Func_081661dc
	.thumb_func
Func_081661dc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r9, r0
	ldr r0, [r3, #92]
	sub sp, #88
	str r0, [sp, #40]
	movs r0, #0
	ldr r1, [r3, #96]
	ldr r5, .L_0816657c
	str r1, [sp, #36]
	movs r7, #255
	ldr r2, [r3, #100]
	str r2, [sp, #28]
	ldr r3, [r3, #48]
	str r3, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, [sp, #40]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r0, .L_08166580
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	adds r1, r5, #0
	ldr r0, .L_08166584
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #28]
	ldr r0, .L_08166588
	movs r3, #0
	bl Func_08157cf4
	mov r3, r9
	movs r2, #36
	ldrsh r1, [r3, r2]
	ldr r0, [r3, #8]
	movs r2, #4
	movs r3, #0
	bl BattleMotion_ApproachTargetFar
	movs r0, #1
	bl WaitFrames
	mov r1, r9
	movs r3, #36
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	ldr r5, [sp, #40]
	movs r2, #0
	mov r10, r2
.L_0816625e:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r7
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ldr r3, [r5]
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	cmp r3, #0
	ble .L_08166298
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_08166298:
	mov r3, r10
	cmp r3, #0
	bge .L_081662a0
	adds r3, #3
.L_081662a0:
	asrs r3, r3, #2
	lsls r3, r3, #1
	adds r3, #16
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_0816625e
	mov r3, sp
	adds r3, #64
	mov r2, r9
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r3, #0
	str r3, [sp, #20]
	bl Func_0815e20c
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0816658c
	bl Scheduler_AddOrUpdateCallback
	ldr r0, [sp, #40]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r3, #75
	str r3, [r2]
	movs r0, #0
	str r0, [sp, #32]
	ldr r2, [sp, #24]
	ldr r0, .L_08166590
	ldr r3, [sp, #40]
	mov r1, sp
	adds r1, #76
	adds r2, #12
	adds r0, r3, r0
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r0, [sp, #8]
.L_08166300:
	ldr r1, [sp, #32]
	cmp r1, #8
	bne .L_0816630c
	movs r0, #134
	bl Func_081180e8
.L_0816630c:
	mov r2, r9
	ldr r3, [r2, #24]
	cmp r3, #0
	beq .L_08166320
	ldr r3, [sp, #32]
	cmp r3, #8
	bne .L_08166320
	movs r0, #212
	bl Audio_PlayCue
.L_08166320:
	mov r1, r9
	ldr r0, [r1, #8]
	ldr r1, [sp, #16]
	bl Func_0815e20c
	ldr r3, [sp, #32]
	subs r3, #6
	cmp r3, #5
	bhi .L_0816639e
	mov r2, r9
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08166344
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	b .L_0816634c
.L_08166344:
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
.L_0816634c:
	movs r3, #192
	lsls r3, r3, #18
	mov r0, r9
	ldr r4, [r3, #104]
	ldr r3, [r0, #4]
	str r4, [sp, #44]
	cmp r3, #0
	bne .L_0816637c
	ldr r2, [sp, #76]
	movs r1, #48
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [sp, #80]
	asrs r2, r2, #1
	str r1, [sp, #0]
	movs r1, #72
	str r1, [sp, #4]
	subs r2, #24
	subs r3, #24
	ldr r0, [sp, #36]
	ldr r1, [sp, #8]
	mov lr, r4
	.2byte 0xf800
	b .L_08166398
.L_0816637c:
	ldr r2, [sp, #76]
	movs r1, #48
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [sp, #80]
	str r1, [sp, #0]
	movs r1, #72
	str r1, [sp, #4]
	asrs r2, r2, #1
	subs r3, #24
	ldr r0, [sp, #36]
	ldr r1, [sp, #8]
	mov lr, r4
	.2byte 0xf800
.L_08166398:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_0816639e:
	ldr r2, [sp, #32]
	subs r2, #16
	cmp r2, #31
	bhi .L_08166412
	lsrs r3, r2, #31
	adds r3, r2, r3
	movs r0, #104
	movs r1, #19
	asrs r5, r3, #1
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	str r3, [sp, #44]
	cmp r5, #2
	ble .L_081663c2
	movs r5, #2
.L_081663c2:
	mov r1, r9
	ldr r3, [r1, #24]
	movs r2, #0
	cmp r3, #0
	beq .L_081663d0
	movs r2, #150
	lsls r2, r2, #6
.L_081663d0:
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #3
	adds r1, r1, r5
	lsls r1, r1, #7
	ldr r3, [sp, #20]
	adds r1, r2, r1
	ldr r2, .L_0816657c
	ldr r0, [sp, #20]
	adds r1, r1, r2
	ldr r2, [r3]
	ldr r4, [sp, #44]
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0, #4]
	movs r0, #40
	str r0, [sp, #0]
	asrs r2, r2, #1
	movs r0, #80
	str r0, [sp, #4]
	subs r2, #20
	subs r3, #48
	ldr r0, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	movs r0, #156
	lsls r0, r0, #6
	adds r0, #16
	bl BattleFx_RunNoEffectFrames
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_08166412:
	ldr r1, [sp, #32]
	cmp r1, #8
	bne .L_08166426
	movs r1, #128
	ldr r3, .L_08166594
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	ldr r2, .L_08166598
	mov lr, r3
	.2byte 0xf800
.L_08166426:
	bl Func_08014de4
	ldr r0, [sp, #24]
	ldr r1, [sp, #12]
	bl Func_080156e8
	ldr r2, [sp, #32]
	cmp r2, #3
	ble .L_081664d4
	mov r3, r9
	movs r1, #44
	ldr r0, [r3, #4]
	add r1, sp
	mov r11, r1
	bl Func_08144aac
	movs r3, #52
	ldr r7, [sp, #40]
	movs r2, #0
	add r3, sp
	mov r10, r2
	mov r8, r3
.L_08166452:
	ldr r5, [r7, #24]
	cmp r5, #0
	ble .L_081664bc
	mov r1, r8
	adds r0, r7, #0
	bl Func_0815e1ec
	mov r0, r8
	ldr r2, [r0]
	ldr r3, [r0, #4]
	asrs r2, r2, #1
	str r2, [r0]
	ldr r0, [sp, #20]
	asrs r5, r5, #3
	ldr r1, [r0, #4]
	mov r0, r10
	adds r3, r3, r1
	lsrs r4, r0, #31
	adds r5, #2
	ldr r0, .L_0816659c
	subs r3, #112
	mov r1, r8
	add r4, r10
	str r3, [r1, #4]
	lsls r6, r5, #1
	movs r1, #1
	asrs r4, r4, #1
	ands r4, r1
	subs r1, r6, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #28]
	str r5, [sp, #0]
	adds r1, r0, r1
	lsrs r0, r5, #31
	str r6, [sp, #4]
	adds r0, r5, r0
	asrs r0, r0, #1
	subs r2, r2, r0
	lsls r4, r4, #2
	mov r0, r11
	subs r3, r3, r5
	ldr r4, [r4, r0]
	ldr r0, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #60
	ldr r2, .L_081665a0
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_081664bc:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r7, #28
	cmp r2, #64
	bne .L_08166452
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_081664d4:
	ldr r3, [sp, #32]
	cmp r3, #8
	bne .L_081664f4
	mov r2, r9
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #4
	bl Func_08118088
	movs r0, #238
	ldr r3, [sp, #40]
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r3, r0
	movs r3, #4
	str r3, [r2]
.L_081664f4:
	ldr r1, [sp, #32]
	cmp r1, #6
	bne .L_08166510
	mov r3, r9
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #0
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #10
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_08166510:
	ldr r0, [sp, #32]
	cmp r0, #14
	bne .L_0816652c
	mov r2, r9
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r3, #0
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #10
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0816652c:
	movs r1, #16
	movs r0, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r0, #240
	ldr r3, [sp, #40]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r3, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #8]
	ldr r3, [sp, #32]
	movs r2, #216
	lsls r2, r2, #4
	adds r1, r1, r2
	adds r3, #1
	str r1, [sp, #8]
	str r3, [sp, #32]
	cmp r3, #64
	beq .L_08166562
	b .L_08166300
.L_08166562:
	ldr r0, .L_0816658c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816657c:
	.4byte gMapCellBuffer
.L_08166580:
	.4byte 0x00000159
.L_08166584:
	.4byte 0x00000125
.L_08166588:
	.4byte 0x00000134
.L_0816658c:
	.4byte Func_08143000
.L_08166590:
	.4byte 0xffffb600
.L_08166594:
	.4byte IwramFillWords
.L_08166598:
	.4byte 0x3f3f3f3f
.L_0816659c:
	.4byte Data_08197410
.L_081665a0:
	.4byte 0xfffffc00
