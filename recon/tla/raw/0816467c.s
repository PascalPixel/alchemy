.syntax unified
	.thumb
	.global Func_0816467c
	.thumb_func
Func_0816467c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r0, [sp, #32]
	str r2, [sp, #28]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	mov r11, r1
	ldr r1, [r5, #96]
	mov r10, r0
	str r1, [sp, #24]
	mov r3, r11
	movs r0, #160
	adds r4, r3, #0
	lsls r0, r0, #14
	adds r4, r4, r0
	mov r11, r4
	ldr r2, [r5, #100]
	str r3, [sp, #8]
	lsrs r3, r4, #31
	add r3, r11
	asrs r3, r3, #1
	str r2, [sp, #12]
	mov r11, r3
	movs r2, #128
	ldr r3, .L_081646f8
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	adds r2, #8
	movs r3, #0
	str r3, [r2]
	ldr r3, .L_081646fc
	adds r2, #40
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r1, [r5, #104]
	movs r0, #188
	str r1, [sp, #16]
	movs r1, #3
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	ldr r1, [sp, #12]
	ldr r0, .L_08164700
	movs r2, #0
	movs r3, #0
	str r5, [sp, #20]
	bl Resource_LoadAndDecompress
	b .L_08164704
	.2byte 0x0000
.L_081646f8:
	.4byte 0x00000080
.L_081646fc:
	.4byte 0x00003f46
.L_08164700:
	.4byte 0x00000134
.L_08164704:
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08164a24
	add r1, r10
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #192
	lsls r1, r1, #7
	adds r1, #216
	ldr r0, .L_08164a28
	add r1, r10
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
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
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08164a2c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	mov r8, r2
	mov r7, r10
.L_0816474c:
	bl Random16
	movs r6, #255
	movs r3, #128
	lsls r3, r3, #1
	ands r6, r0
	adds r6, r6, r3
	bl Random16
	mov r4, r11
	str r4, [r7]
	movs r3, #255
	adds r5, r0, #0
	lsls r3, r3, #8
	ldr r0, [sp, #28]
	adds r3, #255
	ands r5, r3
	str r0, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	negs r3, r3
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	movs r1, #1
	ands r3, r0
	add r8, r1
	adds r3, #16
	mov r2, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #64
	bne .L_0816474c
	movs r5, #192
	lsls r5, r5, #3
	movs r3, #0
	adds r5, #172
	mov r8, r3
	movs r6, #0
	add r5, r10
.L_081647b4:
	mov r4, r11
	str r4, [r5]
	ldr r0, [sp, #28]
	str r0, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	lsls r0, r0, #5
	asrs r0, r0, #6
	str r0, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	movs r1, #170
	lsls r0, r0, #5
	movs r2, #1
	asrs r0, r0, #5
	lsls r1, r1, #7
	add r8, r2
	negs r0, r0
	adds r1, #85
	mov r3, r8
	str r0, [r5, #16]
	adds r6, r6, r1
	adds r5, #28
	cmp r3, #3
	bne .L_081647b4
	ldr r7, .L_08164a30
	movs r4, #0
	mov r8, r4
.L_081647f0:
	bl Random16
	movs r6, #255
	ands r6, r0
	bl Random16
	adds r5, r0, #0
	mov r0, r11
	str r0, [r7]
	movs r3, #255
	ldr r1, [sp, #28]
	lsls r3, r3, #8
	adds r3, #255
	ands r5, r3
	str r1, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #5
	negs r3, r3
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r2, #1
	adds r3, #20
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #64
	bne .L_081647f0
	movs r7, #0
.L_08164848:
	cmp r7, #4
	bne .L_08164852
	movs r0, #154
	bl Audio_PlayCue
.L_08164852:
	cmp r7, #32
	bne .L_0816485c
	movs r0, #212
	bl Audio_PlayCue
.L_0816485c:
	cmp r7, #47
	bgt .L_081648a4
	adds r0, r7, #0
	subs r0, #8
	movs r1, #5
	bl __divsi3
	adds r4, r0, #0
	cmp r4, #0
	bge .L_08164872
	movs r4, #0
.L_08164872:
	ldr r2, .L_08164a34
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	mov r3, r11
	asrs r2, r3, #16
	ldr r3, .L_08164a38
	movs r0, #224
	ldrb r5, [r3, r4]
	lsls r0, r0, #3
	add r1, r10
	adds r1, r1, r0
	ldr r0, [sp, #28]
	lsrs r3, r5, #1
	subs r2, r2, r3
	asrs r3, r0, #16
	ldr r0, .L_08164a3c
	ldrb r4, [r0, r4]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_081648a4:
	movs r0, #0
	mov r8, r0
	mov r6, r10
.L_081648aa:
	mov r1, r8
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	cmp r7, r3
	ble .L_08164900
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_08164900
	subs r3, #1
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_081648d2
	adds r4, #15
.L_081648d2:
	asrs r4, r4, #4
	adds r4, #3
	movs r3, #2
	ldrsh r2, [r6, r3]
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r0, .L_08164a40
	lsls r5, r4, #1
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #12]
	subs r3, r3, r4
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r2, r2, r0
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
.L_08164900:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #30
	bne .L_081648aa
	ldr r3, .L_08164a40
	ldr r6, .L_08164a30
	movs r2, #0
	mov r8, r2
	mov r9, r3
.L_08164916:
	cmp r7, #35
	ble .L_08164964
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_08164964
	subs r3, #1
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_08164936
	adds r4, #15
.L_08164936:
	asrs r4, r4, #4
	adds r4, #1
	lsls r5, r4, #1
	movs r0, #2
	ldrsh r2, [r6, r0]
	movs r1, #6
	ldrsh r3, [r6, r1]
	mov r0, r9
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #12]
	subs r3, r3, r4
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r2, r2, r0
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
.L_08164964:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #60
	bne .L_08164916
	movs r5, #192
	lsls r5, r5, #3
	movs r2, #0
	adds r6, r7, #0
	adds r5, #172
	mov r8, r2
	subs r6, #36
	add r5, r10
.L_08164980:
	cmp r6, #27
	bhi .L_081649c2
	movs r2, #0
	adds r0, r5, #0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector2
	movs r1, #7
	adds r0, r6, #0
	bl __divsi3
	lsls r1, r0, #3
	adds r1, r1, r0
	movs r0, #192
	lsls r1, r1, #5
	lsls r0, r0, #7
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r0, #216
	movs r4, #6
	ldrsh r3, [r5, r4]
	add r1, r10
	adds r1, r1, r0
	movs r0, #12
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	subs r2, #6
	subs r3, #12
	ldr r0, [sp, #24]
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_081649c2:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #3
	bne .L_08164980
	cmp r7, #35
	bgt .L_081649dc
	ldr r0, [sp, #32]
	ldr r1, [sp, #8]
	ldr r2, [sp, #28]
	bl Func_0816442c
.L_081649dc:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #72
	beq .L_081649f6
	b .L_08164848
.L_081649f6:
	ldr r0, .L_08164a2c
	bl Scheduler_RemoveCallback
	movs r1, #128
	ldr r3, .L_08164a44
	lsls r1, r1, #7
	ldr r0, .L_08164a48
	mov lr, r3
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08164a24:
	.4byte 0x00000120
.L_08164a28:
	.4byte 0x00000121
.L_08164a2c:
	.4byte Func_08143000
.L_08164a30:
	.4byte gMapCellBuffer
.L_08164a34:
	.4byte Data_081989f2
.L_08164a38:
	.4byte Data_081989e2
.L_08164a3c:
	.4byte Data_081989ea
.L_08164a40:
	.4byte Data_08197410
.L_08164a44:
	.4byte IwramClearWords
.L_08164a48:
	.4byte 0x06004000
