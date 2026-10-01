.syntax unified
	.thumb
	.global Func_081638d0
	.thumb_func
Func_081638d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r3, [r3, #96]
	mov r10, r0
	movs r0, #128
	lsls r0, r0, #6
	str r3, [sp, #16]
	bl BattleFx_BeginCanvasLayer
	ldr r2, [sp, #20]
	movs r3, #70
	ldr r1, [r2, #4]
	ldr r0, [sp, #20]
	lsls r1, r1, #4
	add r2, sp, #44
	orrs r1, r3
	add r3, sp, #32
	bl Func_0815585c
	ldr r3, .L_08163944
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_08163948
	adds r2, #70
	strh r3, [r2]
	ldr r3, .L_0816394c
	subs r2, #50
	strh r3, [r2]
	ldr r3, [sp, #20]
	add r1, sp, #24
	ldr r0, [r3, #4]
	bl Func_08144aac
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
	movs r1, #200
	lsls r1, r1, #4
	b .L_08163950
.L_08163944:
	.4byte 0x00002784
.L_08163948:
	.4byte 0x00001000
.L_0816394c:
	.4byte 0x000000aa
.L_08163950:
	str r3, [r2]
	ldr r0, .L_08163aa4
	bl Scheduler_AddOrUpdateCallback
	movs r5, #127
	movs r1, #0
	mov r9, r1
	mov r11, r5
.L_08163960:
	movs r2, #208
	movs r0, #0
	mov r3, r9
	lsls r2, r2, #3
	movs r5, #224
	add r3, r10
	adds r2, #255
	lsls r5, r5, #3
	mov r8, r0
	mov r0, r11
	adds r4, r3, r2
	adds r7, r3, r5
	subs r3, r0, r1
	movs r0, #224
	lsls r3, r3, #7
	lsls r0, r0, #3
	adds r2, r3, r2
	mov r5, r10
	adds r3, r3, r0
	adds r6, r2, r5
	adds r5, r3, r5
.L_0816398a:
	adds r2, r1, #0
	cmp r1, #0
	bge .L_08163992
	adds r2, r1, #7
.L_08163992:
	asrs r2, r2, #3
	adds r2, #64
	mov r3, r8
	subs r2, r1, r2
	subs r3, #64
	adds r0, r3, #0
	muls r0, r3
	adds r3, r2, #0
	muls r3, r2
	str r1, [sp, #12]
	adds r0, r0, r3
	str r4, [sp, #8]
	ldr r3, .L_08163aa8
	mov lr, r3
	.2byte 0xf800
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r0, r3, #1
	ldr r1, [sp, #12]
	ldr r4, [sp, #8]
	cmp r0, #0
	bne .L_081639c0
	movs r0, #1
.L_081639c0:
	cmp r0, #63
	ble .L_081639c6
	movs r0, #63
.L_081639c6:
	movs r2, #1
	add r8, r2
	mov r3, r8
	strb r0, [r7]
	strb r0, [r4]
	adds r7, #1
	strb r0, [r5]
	subs r4, #1
	strb r0, [r6]
	adds r5, #1
	subs r6, #1
	cmp r3, #64
	bne .L_0816398a
	movs r5, #128
	adds r1, #1
	add r9, r5
	cmp r1, #64
	bne .L_08163960
	ldr r4, .L_08163aac
	movs r7, #1
.L_081639ee:
	cmp r7, #31
	ble .L_081639f8
	movs r3, #64
	subs r2, r3, r7
	b .L_081639fa
.L_081639f8:
	adds r2, r7, #0
.L_081639fa:
	lsls r3, r2, #3
	adds r0, r3, r2
	subs r3, r3, r2
	adds r1, r3, #0
	adds r2, r3, #0
	subs r1, #42
	subs r2, #56
	cmp r0, #0
	bge .L_08163a0e
	movs r0, #0
.L_08163a0e:
	cmp r1, #0
	bge .L_08163a14
	movs r1, #0
.L_08163a14:
	cmp r2, #0
	bge .L_08163a1a
	movs r2, #0
.L_08163a1a:
	cmp r0, #255
	ble .L_08163a20
	movs r0, #255
.L_08163a20:
	cmp r1, #255
	ble .L_08163a26
	movs r1, #255
.L_08163a26:
	cmp r2, #250
	ble .L_08163a2c
	movs r2, #250
.L_08163a2c:
	asrs r1, r1, #3
	asrs r2, r2, #3
	movs r5, #160
	lsls r2, r2, #10
	lsls r1, r1, #5
	lsls r3, r7, #1
	asrs r0, r0, #3
	lsls r5, r5, #19
	orrs r2, r1
	orrs r2, r0
	adds r3, r3, r5
	adds r7, #1
	strh r2, [r3]
	strh r2, [r4]
	adds r4, #2
	cmp r7, #64
	bne .L_081639ee
	movs r1, #224
	movs r3, #128
	lsls r1, r1, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r1, r10
	ldr r4, [sp, #24]
	ldr r0, [sp, #16]
	movs r2, #0
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08163ab0
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_08163ab4
	movs r0, #0
	mov r8, r0
	mov r11, r2
	mov r9, r0
.L_08163a86:
	mov r3, r8
	cmp r3, #8
	bgt .L_08163ab8
	ldr r2, .L_08163aa0
	movs r0, #128
	mov r3, r9
	lsls r0, r0, #19
	orrs r3, r2
	adds r0, #82
	mov r5, r9
	strh r3, [r0]
	b .L_08163abc
	.2byte 0x0000
.L_08163aa0:
	.4byte 0x00001000
.L_08163aa4:
	.4byte Func_08143000
.L_08163aa8:
	.4byte IwramFillWords + 0x74
.L_08163aac:
	.4byte Data_02010002
.L_08163ab0:
	.4byte Func_08152474
.L_08163ab4:
	.4byte 0x04000208
.L_08163ab8:
	mov r2, r8
	lsls r5, r2, #1
.L_08163abc:
	mov r3, r8
	cmp r3, #72
	ble .L_08163ad4
	ldr r3, .L_08163afc
	ldr r2, .L_08163b00
	mov r0, r9
	subs r3, r3, r0
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_08163ad4:
	mov r3, r8
	cmp r3, #48
	bne .L_08163b04
	movs r0, #134
	bl Func_081180e8
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r10
	movs r2, #8
	str r2, [r3]
	ldr r3, [sp, #20]
	movs r1, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118088
	b .L_08163b04
	.2byte 0x0000
.L_08163afc:
	.4byte 0x000000a0
.L_08163b00:
	.4byte 0x00001000
.L_08163b04:
	movs r6, #225
	lsls r6, r6, #7
	lsls r3, r5, #9
	add r6, r10
	movs r7, #0
	negs r5, r3
.L_08163b10:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r7, #18
	lsls r0, r0, #7
	subs r3, r3, r0
	movs r0, #128
	lsls r0, r0, #11
	adds r3, r3, r0
	movs r2, #128
	asrs r3, r3, #10
	lsls r2, r2, #2
	adds r7, #1
	stmia r6!, {r3}
	adds r5, r5, r2
	cmp r7, #160
	bne .L_08163b10
	mov r3, r8
	cmp r3, #127
	ble .L_08163b46
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	b .L_08163b98
.L_08163b46:
	ldr r5, .L_08163be4
	ldr r0, .L_08163be8
	ldrh r3, [r5]
	ldr r1, .L_08163be4
	strh r3, [r0, #2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, #124
	ldr r2, .L_08163bec
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_08163bf0
	mov r2, r11
	ldrh r3, [r2]
	adds r0, r3, #0
	mov r3, r11
	mov r5, r11
	strh r3, [r5]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08163b94
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	ldr r2, .L_08163bf4
	adds r3, r3, r1
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #2
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #63
	str r2, [r3]
.L_08163b94:
	mov r2, r11
	strh r0, [r2]
.L_08163b98:
	movs r0, #8
	movs r1, #8
	movs r5, #1
	bl Func_08158ce0
	add r8, r5
	bl Func_081434f8
	movs r0, #1
	bl WaitFrames
	movs r3, #2
	mov r0, r8
	add r9, r3
	cmp r0, #80
	beq .L_08163bba
	b .L_08163a86
.L_08163bba:
	ldr r0, .L_08163bf8
	bl Scheduler_RemoveCallback
	ldr r0, .L_08163bfc
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08163be4:
	.4byte Data_0201007e
.L_08163be8:
	.4byte gMapCellBuffer
.L_08163bec:
	.4byte 0x80a0003e
.L_08163bf0:
	.4byte gIoWriteQueue
.L_08163bf4:
	.4byte Data_02010002
.L_08163bf8:
	.4byte Func_08152474
.L_08163bfc:
	.4byte Func_08143000
