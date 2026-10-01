.syntax unified
	.thumb
	.global Func_0816d3d8
	.thumb_func
Func_0816d3d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	ldr r1, [r5, #92]
	ldr r2, [r5, #96]
	sub sp, #68
	mov r10, r0
	movs r0, #1
	str r2, [sp, #40]
	mov r11, r1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816d438
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0816d43c
	movs r1, #224
	adds r2, #48
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0816d440
	movs r2, #1
	movs r3, #1
	add r1, r11
	bl Func_08157cf4
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #1
	str r5, [sp, #36]
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	b .L_0816d444
.L_0816d438:
	.4byte 0x00000100
.L_0816d43c:
	.4byte 0x00000000
.L_0816d440:
	.4byte 0x0000014d
.L_0816d444:
	adds r2, #132
	add r2, r11
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816d710
	bl Scheduler_AddOrUpdateCallback
	movs r3, #0
	str r3, [sp, #32]
	str r3, [sp, #28]
	str r3, [sp, #24]
	mov r4, r10
	ldr r0, [r4, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r1, #48
	str r0, [sp, #20]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r1, sp
	mov r2, sp
	adds r1, #56
	adds r2, #44
	str r1, [sp, #8]
	str r2, [sp, #12]
	movs r5, #0
	mov r9, r5
.L_0816d480:
	mov r3, r10
	ldr r0, [r3, #8]
	ldr r1, [sp, #8]
	bl Func_0815e20c
	mov r5, r10
	ldr r1, [sp, #12]
	movs r4, #36
	ldrsh r0, [r5, r4]
	bl Func_0815e20c
	mov r1, r9
	cmp r1, #0
	bne .L_0816d4a2
	movs r0, #157
	bl Audio_PlayCue
.L_0816d4a2:
	mov r2, r9
	cmp r2, #64
	bne .L_0816d4ae
	movs r0, #104
	bl Audio_PlayCue
.L_0816d4ae:
	mov r3, r9
	cmp r3, #0
	bne .L_0816d524
	movs r4, #192
	lsls r4, r4, #15
	movs r7, #0
	movs r6, #0
	mov r8, r4
	mov r5, r11
.L_0816d4c0:
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816d4da
	bl Random16
	movs r1, #40
	bl Math_ModU
	adds r0, #8
	lsls r0, r0, #16
	str r0, [r5]
	b .L_0816d4ec
.L_0816d4da:
	bl Random16
	movs r1, #40
	bl Math_ModU
	movs r3, #120
	subs r3, r3, r0
	lsls r3, r3, #16
	str r3, [r5]
.L_0816d4ec:
	bl Random16
	mov r2, r8
	str r2, [r5, #4]
	str r6, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #32
	negs r3, r3
	lsls r3, r3, #11
	str r3, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #10
	adds r7, #1
	str r6, [r5, #20]
	str r6, [r5, #24]
	add r8, r3
	adds r5, #28
	cmp r7, #21
	bne .L_0816d4c0
.L_0816d524:
	mov r4, r9
	cmp r4, #52
	bne .L_0816d532
	ldr r0, [sp, #20]
	movs r1, #2
	bl Object_SetMode
.L_0816d532:
	mov r5, r9
	cmp r5, #64
	ble .L_0816d56a
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816d54a
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	adds r2, r2, r3
	str r2, [sp, #32]
	b .L_0816d552
.L_0816d54a:
	ldr r4, [sp, #32]
	ldr r5, [sp, #28]
	subs r4, r4, r5
	str r4, [sp, #32]
.L_0816d552:
	ldr r1, [sp, #28]
	movs r2, #128
	lsls r2, r2, #8
	adds r1, r1, r2
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_0816d566
	adds r3, #63
.L_0816d566:
	asrs r3, r3, #6
	str r3, [sp, #28]
.L_0816d56a:
	ldr r3, [sp, #32]
	ldr r4, [sp, #8]
	asrs r3, r3, #16
	str r3, [sp, #16]
	movs r1, #128
	ldr r2, [r4]
	lsls r1, r1, #19
	subs r2, r2, r3
	movs r3, #64
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	movs r5, #0
	mov r8, r5
	movs r7, #0
	mov r6, r11
.L_0816d58c:
	movs r1, #5
	adds r0, r7, #0
	bl Math_Mod
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r6, #20]
	bl Math_Div
	movs r1, #3
	bl Math_Mod
	ldr r3, .L_0816d714
	adds r5, r5, r0
	ldrb r4, [r3, r5]
	movs r1, #2
	ldrsh r2, [r6, r1]
	lsrs r3, r4, #1
	subs r2, r2, r3
	ldr r3, [sp, #16]
	adds r2, r2, r3
	movs r1, #6
	ldrsh r3, [r6, r1]
	ldr r1, .L_0816d718
	ldrb r0, [r1, r5]
	lsls r5, r5, #1
	lsrs r1, r0, #1
	subs r3, r3, r1
	ldr r1, .L_0816d71c
	ldrh r1, [r1, r5]
	movs r5, #240
	lsls r5, r5, #4
	add r1, r11
	adds r5, #60
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r1, r1, r5
	ldr r0, [sp, #40]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
	cmp r3, #0
	bne .L_0816d61e
	mov r5, r10
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0816d604
	ldr r4, [sp, #8]
	movs r1, #2
	ldrsh r3, [r6, r1]
	ldr r2, [r4]
	ldr r5, [sp, #12]
	adds r3, r3, r2
	ldr r2, [r5]
	subs r3, #64
	cmp r2, r3
	ble .L_0816d61e
	b .L_0816d618
.L_0816d604:
	ldr r4, [sp, #8]
	movs r1, #2
	ldrsh r3, [r6, r1]
	ldr r2, [r4]
	ldr r5, [sp, #12]
	adds r3, r3, r2
	ldr r2, [r5]
	subs r3, #64
	cmp r2, r3
	bge .L_0816d61e
.L_0816d618:
	movs r3, #1
	str r3, [r6, #24]
	mov r8, r3
.L_0816d61e:
	movs r2, #0
	adds r0, r6, #0
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #20]
	ldr r2, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #20]
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	adds r3, #64
	cmp r9, r3
	ble .L_0816d660
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816d64c
	ldr r3, [r6, #12]
	ldr r2, .L_0816d720
	adds r3, r3, r2
	b .L_0816d654
.L_0816d64c:
	ldr r3, [r6, #12]
	movs r4, #128
	lsls r4, r4, #8
	adds r3, r3, r4
.L_0816d654:
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	cmp r3, #1
	ble .L_0816d660
	subs r3, #1
	str r3, [r6, #8]
.L_0816d660:
	adds r7, #1
	adds r6, #28
	cmp r7, #21
	bne .L_0816d58c
	ldr r5, [sp, #24]
	cmp r5, #0
	ble .L_0816d672
	subs r5, #1
	str r5, [sp, #24]
.L_0816d672:
	mov r1, r8
	cmp r1, #1
	bne .L_0816d6c0
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_0816d688
	movs r3, #8
	movs r0, #134
	str r3, [sp, #24]
	bl Audio_PlayCue
.L_0816d688:
	mov r5, r10
	movs r3, #150
	movs r4, #36
	ldrsh r0, [r5, r4]
	movs r2, #128
	str r3, [sp, #4]
	movs r3, #128
	lsls r2, r2, #10
	movs r1, #1
	lsls r3, r3, #11
	str r2, [sp, #0]
	bl Func_0815f000
	movs r3, #4
	movs r1, #36
	ldrsh r0, [r5, r1]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #2
	str r3, [r2]
.L_0816d6c0:
	movs r0, #2
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #107
	beq .L_0816d6ea
	b .L_0816d480
.L_0816d6ea:
	ldr r0, [sp, #20]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r0, .L_0816d710
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816d710:
	.4byte Func_08143000
.L_0816d714:
	.4byte Data_0819749e
.L_0816d718:
	.4byte Data_081974ad
.L_0816d71c:
	.4byte Data_081974bc
.L_0816d720:
	.4byte 0xffff8000
