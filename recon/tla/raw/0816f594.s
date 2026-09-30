.syntax unified
	.thumb
	.global Func_0816f594
	.thumb_func
Func_0816f594:
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
	sub sp, #48
	mov r11, r0
	movs r0, #0
	str r2, [sp, #24]
	mov r9, r1
	bl Func_081435e0
	ldr r3, .L_0816f5f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r1, #184
	lsls r1, r1, #5
	add r1, r9
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0816f5fc
	bl Func_08157cf4
	ldr r0, .L_0816f600
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816f604
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	b .L_0816f608
	.2byte 0x0000
.L_0816f5f8:
	.4byte 0x00001010
.L_0816f5fc:
	.4byte 0x000000c2
.L_0816f600:
	.4byte 0x00000148
.L_0816f604:
	.4byte IwramCopyWords
.L_0816f608:
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816f908
	bl Func_080145a8
	ldr r5, [r5, #48]
	mov r2, r11
	str r5, [sp, #16]
	movs r7, #0
	movs r4, #54
	ldrsh r3, [r5, r4]
	movs r6, #2
	str r3, [sp, #12]
	movs r5, #0
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl GetBattleObjectSlotFar
	movs r3, #224
	ldr r0, [r0]
	lsls r3, r3, #3
	add r3, r9
	str r0, [sp, #8]
	str r3, [sp, #4]
	movs r4, #0
	mov r8, r4
.L_0816f64c:
	ldr r1, .L_0816f90c
	movs r2, #167
	lsls r2, r2, #9
	adds r0, r7, r1
	adds r2, #32
	adds r1, r6, #0
	bl Func_0815b434
	adds r3, r5, #3
	muls r3, r6
	ldr r2, [sp, #4]
	adds r6, #2
	strh r7, [r5, r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r7, r7, r3
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #2
	cmp r4, #32
	bne .L_0816f64c
	movs r1, #0
	str r1, [sp, #20]
.L_0816f67e:
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_0816f750
	add r3, sp, #12
	ldr r4, [sp, #16]
	ldrh r3, [r3]
	mov r7, r9
	strh r3, [r4, #54]
	bl Func_08014de4
	ldr r0, [sp, #16]
	adds r1, r0, #0
	adds r1, #12
	bl Func_080156e8
	movs r1, #36
	movs r4, #0
	add r1, sp
	mov r8, r4
	mov r10, r1
.L_0816f6a6:
	bl Random16
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r5, #7
	mov r2, r8
	lsls r3, r2, #2
	ands r5, r0
	adds r0, r6, #0
	adds r5, r5, r3
	bl Trig_Sin
	ldr r4, [sp, #8]
	adds r2, r5, #0
	muls r2, r0
	ldr r3, [r4, #8]
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r6, #0
	bl Trig_Cos
	ldr r1, [sp, #8]
	adds r2, r5, #0
	muls r2, r0
	ldr r3, [r1, #12]
	adds r0, r7, #0
	adds r3, r3, r2
	movs r2, #192
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r1, #16]
	mov r1, r10
	str r3, [r7, #8]
	bl Func_0815e1ec
	mov r4, r10
	ldr r3, [r4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7]
	ldr r3, [r4, #4]
	str r3, [r7, #4]
	bl Random16
	mov r1, r8
	movs r3, #32
	subs r3, r3, r1
	str r3, [r7, #12]
	bl Random16
	str r0, [r7, #16]
	bl Random16
	mov r2, r8
	negs r3, r2
	lsls r3, r3, #2
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	str r0, [r7, #20]
	adds r7, #28
	cmp r4, #32
	bne .L_0816f6a6
	ldr r1, [sp, #20]
	cmp r1, #0
	bne .L_0816f750
	mov r2, r11
	movs r3, #36
	ldrsh r1, [r2, r3]
	ldr r0, [r2, #8]
	movs r3, #80
	movs r2, #8
	bl Func_08157530
.L_0816f750:
	ldr r4, [sp, #20]
	cmp r4, #8
	bne .L_0816f796
	movs r1, #240
	ldr r0, [sp, #24]
	lsls r1, r1, #6
	ldr r3, .L_0816f910
	ldr r2, .L_0816f914
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	movs r3, #16
	add r2, r9
	str r3, [r2]
	mov r2, r11
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #0
	bl Func_08118088
	ldr r1, [sp, #20]
	mov r4, r11
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r2, #5
	str r1, [sp, #0]
	movs r3, #0
	movs r1, #7
	bl Func_0814cd48
	movs r0, #145
	bl Func_081180e8
.L_0816f796:
	ldr r2, [sp, #20]
	cmp r2, #7
	bgt .L_0816f79e
	b .L_0816f8c0
.L_0816f79e:
	movs r3, #0
	mov r8, r3
	mov r6, r9
.L_0816f7a4:
	ldr r3, [r6, #24]
	adds r5, r3, #1
	str r5, [r6, #24]
	cmp r5, #8
	bne .L_0816f7dc
	movs r0, #144
	bl Audio_PlayCue
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r9
	str r5, [r3]
	mov r1, r11
	movs r4, #36
	ldrsh r0, [r1, r4]
	movs r1, #0
	bl Func_08118088
	mov r3, r11
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	str r5, [sp, #0]
	bl Func_0814cd48
.L_0816f7dc:
	ldr r0, [r6, #24]
	cmp r0, #31
	bhi .L_0816f806
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r3, [r6, #12]
	muls r3, r0
	asrs r3, r3, #16
	lsls r3, r3, #1
	subs r2, r3, #2
	cmp r2, #61
	bhi .L_0816f806
	ldr r4, [sp, #4]
	ldrsh r0, [r2, r4]
	ldr r2, .L_0816f90c
	ldr r1, [r6]
	adds r0, r0, r2
	ldr r2, [r6, #4]
	bl Func_0818caa8
.L_0816f806:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r6, #28
	cmp r4, #8
	bne .L_0816f7a4
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0816f918
	ldr r3, [sp, #28]
	adds r6, r0, #0
	ands r3, r2
	movs r2, #7
	orrs r3, r2
	ldr r2, .L_0816f91c
	mov r1, r10
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #28]
	movs r3, #184
	lsls r3, r3, #5
	add r2, sp, #28
	add r3, r9
	str r3, [r2, #4]
	movs r3, #6
	str r3, [r6]
	ldr r3, .L_0816f920
	str r1, [r6, #12]
	str r3, [r6, #8]
	str r2, [r6, #16]
	movs r2, #0
	mov r8, r2
	mov r5, r9
.L_0816f856:
	ldr r2, [r5, #24]
	cmp r2, #23
	bhi .L_0816f8a8
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #0
	lsls r7, r3, #11
	cmp r2, #15
	ble .L_0816f86e
	movs r3, #16
	subs r3, r3, r2
	lsls r1, r3, #3
.L_0816f86e:
	str r1, [r6, #20]
	bl Func_08014de4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	subs r0, #64
	subs r1, #64
	lsls r1, r1, #16
	movs r2, #0
	lsls r0, r0, #16
	bl Func_08015160
	ldr r0, [r5, #20]
	bl Func_080150e4
	ldr r0, [r5, #16]
	bl Func_08015024
	adds r0, r7, #0
	bl Func_0801521c
	ldr r0, .L_0816f924
	mov r1, r10
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0816f8a8:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #28
	cmp r4, #8
	bne .L_0816f856
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
.L_0816f8c0:
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #66
	beq .L_0816f8ea
	b .L_0816f67e
.L_0816f8ea:
	ldr r0, .L_0816f908
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816f908:
	.4byte Func_08143000
.L_0816f90c:
	.4byte gMapCellBuffer
.L_0816f910:
	.4byte IwramFillWords
.L_0816f914:
	.4byte 0x3f3f3f3f
.L_0816f918:
	.4byte 0xffffff00
.L_0816f91c:
	.4byte 0xffff00ff
.L_0816f920:
	.4byte Data_08199364
.L_0816f924:
	.4byte Data_081991e0
