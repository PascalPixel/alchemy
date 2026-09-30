.syntax unified
	.thumb
	.global Func_08144368
	.thumb_func
Func_08144368:
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
	sub sp, #68
	str r1, [sp, #32]
	mov r9, r0
	ldr r2, [r5, #96]
	movs r0, #0
	str r2, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081443cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, [sp, #32]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r3, r2
	ldr r0, .L_081443d0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #31
	movs r0, #188
	str r3, [sp, #36]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r1, sp
	movs r5, #200
	adds r1, #36
	lsls r5, r5, #4
	b .L_081443d4
	.2byte 0x0000
.L_081443cc:
	.4byte 0x00000100
.L_081443d0:
	.4byte 0x00000173
.L_081443d4:
	str r1, [sp, #16]
	ldr r0, .L_081445e8
	str r3, [r1, #4]
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #32]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r3, r1
	movs r3, #2
	str r3, [r2]
	ldr r3, [sp, #32]
	adds r1, #4
	adds r2, r3, r1
	movs r3, #50
	str r3, [r2]
	ldr r0, .L_081445ec
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
	mov r2, r9
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08144418
	ldr r3, .L_081445f0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
	movs r3, #112
	negs r3, r3
	str r3, [sp, #20]
	b .L_0814441c
.L_08144418:
	movs r1, #0
	str r1, [sp, #20]
.L_0814441c:
	mov r1, r9
	ldr r3, [r1, #20]
	movs r2, #0
	mov r8, r2
	lsls r3, r3, #4
	subs r2, #48
	cmp r3, r2
	bne .L_0814442e
	b .L_081445be
.L_0814442e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	movs r1, #225
	mov r10, r3
	ldr r3, [sp, #32]
	lsls r1, r1, #7
	mov r2, r9
	adds r6, r3, r1
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08144470
	movs r3, #128
	mov r1, r8
	movs r7, #0
	lsls r3, r3, #12
	lsls r5, r1, #10
.L_08144450:
	adds r0, r5, #0
	str r3, [sp, #8]
	bl Trig_Sin
	ldr r3, [sp, #8]
	lsls r0, r0, #3
	subs r0, r3, r0
	movs r2, #128
	asrs r0, r0, #10
	lsls r2, r2, #3
	adds r7, #1
	stmia r6!, {r0}
	adds r5, r5, r2
	cmp r7, #160
	bne .L_08144450
	b .L_08144492
.L_08144470:
	mov r3, r8
	movs r7, #0
	lsls r5, r3, #10
.L_08144476:
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, .L_081445f4
	lsls r0, r0, #3
	asrs r0, r0, #10
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #3
	adds r7, #1
	stmia r6!, {r0}
	adds r5, r5, r2
	cmp r7, #160
	bne .L_08144476
.L_08144492:
	bl Func_08014de4
	mov r1, r10
	adds r1, #12
	mov r0, r10
	bl Graphics_PrepareTransferInIwramWork
	movs r3, #0
	str r3, [sp, #24]
	mov r1, r9
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_08144598
	movs r3, #36
	movs r2, #56
	str r3, [sp, #12]
	add r2, sp
	mov r10, r2
.L_081444b6:
	ldr r1, [sp, #12]
	mov r3, r9
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r1, [sp, #24]
	ldr r5, [r0]
	lsls r2, r1, #4
	cmp r8, r2
	ble .L_08144582
	adds r3, r2, #0
	adds r3, #60
	cmp r8, r3
	bge .L_08144582
	subs r3, #28
	cmp r8, r3
	bne .L_081444ec
	ldr r2, [sp, #12]
	mov r1, r9
	ldrsh r0, [r2, r1]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #5
	subs r3, #1
	bl Func_0814cd48
.L_081444ec:
	ldr r3, [r5, #8]
	mov r2, r10
	str r3, [r2]
	movs r3, #160
	lsls r3, r3, #14
	str r3, [r2, #4]
	mov r0, r10
	ldr r3, [r5, #16]
	add r5, sp, #44
	str r3, [r2, #8]
	adds r1, r5, #0
	bl Func_0815e1ec
	mov r3, r8
	mov r11, r5
	movs r7, #0
	lsls r5, r3, #9
.L_0814450e:
	adds r0, r5, #0
	bl Trig_Sin
	mov r1, r11
	ldr r3, [r1]
	ldr r2, [sp, #20]
	lsls r0, r0, #4
	asrs r0, r0, #16
	adds r3, r3, r0
	adds r0, r5, #0
	adds r6, r3, r2
	bl Trig_Cos
	mov r1, r11
	ldr r3, [r1, #4]
	lsls r0, r0, #4
	asrs r0, r0, #16
	adds r0, r3, r0
	mov r3, r8
	cmp r3, #0
	bge .L_0814453a
	adds r3, #15
.L_0814453a:
	asrs r2, r3, #4
	ldr r1, [sp, #16]
	movs r3, #1
	ands r3, r2
	lsls r3, r3, #2
	adds r4, r3, r1
	mov r1, r8
	cmp r1, #0
	bge .L_0814454e
	adds r1, #3
.L_0814454e:
	lsls r3, r2, #2
	asrs r1, r1, #2
	ldr r2, [sp, #32]
	subs r1, r1, r3
	lsls r1, r1, #10
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	adds r1, r1, r3
	adds r3, r0, #0
	movs r0, #32
	str r0, [sp, #0]
	str r0, [sp, #4]
	adds r2, r6, #0
	subs r2, #16
	subs r3, #16
	ldr r4, [r4]
	ldr r0, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	adds r7, #1
	adds r5, r5, r1
	cmp r7, #4
	bne .L_0814450e
.L_08144582:
	ldr r2, [sp, #12]
	ldr r3, [sp, #24]
	adds r2, #2
	adds r3, #1
	str r2, [sp, #12]
	str r3, [sp, #24]
	mov r1, r9
	ldr r3, [r1, #20]
	ldr r2, [sp, #24]
	cmp r2, r3
	bne .L_081444b6
.L_08144598:
	ldr r3, [sp, #32]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r3, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	mov r1, r9
	ldr r3, [r1, #20]
	movs r2, #1
	lsls r3, r3, #4
	add r8, r2
	adds r3, #48
	cmp r8, r3
	beq .L_081445be
	b .L_0814442e
.L_081445be:
	ldr r0, .L_081445ec
	bl Scheduler_RemoveCallback
	ldr r0, .L_081445e8
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
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
.L_081445e8:
	.4byte Func_08152474
.L_081445ec:
	.4byte Func_08143000
.L_081445f0:
	.4byte 0xffff9800
.L_081445f4:
	.4byte 0xffff9000
