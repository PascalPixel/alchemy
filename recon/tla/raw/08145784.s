.syntax unified
	.thumb
	.global Func_08145784
	.thumb_func
Func_08145784:
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
	sub sp, #72
	str r1, [sp, #36]
	mov r11, r0
	ldr r2, [r5, #96]
	movs r0, #128
	movs r3, #0
	lsls r0, r0, #6
	str r2, [sp, #32]
	mov r8, r3
	bl BattleFx_BeginTiledCanvas
	ldr r3, .L_081457ec
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_081457f0
	adds r2, #50
	strh r3, [r2]
	ldr r4, [sp, #36]
	ldr r6, .L_081457f4
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r4, r2
	ldr r0, .L_081457f8
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #1
	adds r1, r6, #0
	movs r3, #0
	ldr r0, .L_081457fc
	bl Func_08157cf4
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	b .L_08145800
	.2byte 0x0000
.L_081457ec:
	.4byte 0x00000100
.L_081457f0:
	.4byte 0x00001010
.L_081457f4:
	.4byte gMapCellBuffer
.L_081457f8:
	.4byte 0x0000016f
.L_081457fc:
	.4byte 0x00000170
.L_08145800:
	movs r1, #39
	movs r0, #188
	str r3, [sp, #40]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r4, sp
	movs r5, #200
	adds r4, #40
	lsls r5, r5, #4
	str r4, [sp, #16]
	adds r1, r5, #0
	str r3, [r4, #4]
	ldr r0, .L_081458ec
	bl Scheduler_AddOrUpdateCallback
	ldr r6, [sp, #36]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r6, r1
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	mov r4, r8
	str r4, [r3]
	adds r1, r5, #0
	ldr r0, .L_081458f0
	bl Scheduler_AddOrUpdateCallback
	movs r5, #1
	str r5, [sp, #20]
	mov r6, r11
	ldr r3, [r6, #4]
	cmp r3, #1
	bne .L_08145854
	ldr r1, .L_081458f4
	str r1, [sp, #24]
	b .L_0814585a
.L_08145854:
	movs r2, #224
	lsls r2, r2, #15
	str r2, [sp, #24]
.L_0814585a:
	ldr r3, .L_081458f8
	movs r4, #0
	str r3, [sp, #28]
	mov r8, r4
.L_08145862:
	mov r6, r8
	lsls r5, r6, #9
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, [sp, #24]
	lsls r0, r0, #4
	asrs r3, r1, #16
	asrs r0, r0, #16
	adds r3, r3, r0
	adds r0, r5, #0
	adds r7, r3, #0
	bl Trig_Cos
	ldr r2, [sp, #28]
	lsls r0, r0, #2
	asrs r3, r2, #16
	asrs r0, r0, #16
	adds r3, r3, r0
	adds r3, #16
	adds r7, #48
	mov r9, r3
	cmp r6, #24
	bne .L_081458b0
	mov r4, r11
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_081458a0
	ldr r5, .L_081458f8
	str r5, [sp, #24]
	b .L_081458a6
.L_081458a0:
	movs r6, #144
	lsls r6, r6, #15
	str r6, [sp, #24]
.L_081458a6:
	movs r1, #192
	lsls r1, r1, #13
	movs r2, #0
	str r1, [sp, #28]
	str r2, [sp, #20]
.L_081458b0:
	mov r3, r8
	cmp r3, #25
	bne .L_081458c4
	ldr r3, .L_081458e8
	movs r4, #128
	lsls r4, r4, #19
	adds r4, #82
	movs r5, #1
	strh r3, [r4]
	str r5, [sp, #20]
.L_081458c4:
	mov r6, r8
	cmp r6, #48
	bne .L_0814590a
	mov r2, r11
	add r5, sp, #48
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e20c
	mov r4, r11
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_081458fc
	ldr r3, [r5]
	subs r3, #128
	b .L_08145900
	.2byte 0x0000
.L_081458e8:
	.4byte 0x00001010
.L_081458ec:
	.4byte Func_08152474
.L_081458f0:
	.4byte Func_08143000
.L_081458f4:
	.4byte 0xffb00000
.L_081458f8:
	.4byte 0xffe00000
.L_081458fc:
	ldr r3, [r5]
	subs r3, #64
.L_08145900:
	lsls r3, r3, #16
	str r3, [sp, #24]
	movs r5, #0
	str r5, [sp, #28]
	str r5, [sp, #20]
.L_0814590a:
	mov r6, r8
	cmp r6, #49
	bne .L_0814591e
	ldr r3, .L_08145944
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	movs r2, #1
	strh r3, [r1]
	str r2, [sp, #20]
.L_0814591e:
	ldr r3, [sp, #36]
	movs r4, #225
	lsls r4, r4, #7
	mov r5, r8
	adds r1, r3, r4
	movs r4, #0
	cmp r5, #23
	bgt .L_0814595a
	cmp r5, #15
	ble .L_0814597a
	lsls r3, r5, #1
	adds r4, r3, #0
	ldr r3, .L_08145948
	ldr r2, .L_0814594c
	movs r6, #128
	subs r3, r3, r5
	lsls r6, r6, #19
	b .L_08145950
	.2byte 0x0000
.L_08145944:
	.4byte 0x00001010
.L_08145948:
	.4byte 0x0000001f
.L_0814594c:
	.4byte 0x00001000
.L_08145950:
	orrs r3, r2
	adds r6, #82
	subs r4, #32
	strh r3, [r6]
	b .L_0814597a
.L_0814595a:
	mov r2, r8
	cmp r2, #47
	bgt .L_0814597a
	cmp r2, #31
	ble .L_0814597a
	lsls r3, r2, #1
	adds r4, r3, #0
	ldr r3, .L_08145990
	movs r5, #128
	subs r3, r3, r2
	ldr r2, .L_08145994
	lsls r5, r5, #19
	orrs r3, r2
	adds r5, #82
	strh r3, [r5]
	subs r4, #64
.L_0814597a:
	cmp r4, #0
	bge .L_08145980
	movs r4, #0
.L_08145980:
	movs r3, #6
	subs r3, r3, r7
	mov r2, r8
	movs r6, #0
	lsls r7, r3, #8
	lsls r5, r2, #11
	b .L_08145998
	.2byte 0x0000
.L_08145990:
	.4byte 0x0000002f
.L_08145994:
	.4byte 0x00001000
.L_08145998:
	adds r0, r5, #0
	str r1, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Sin
	ldr r4, [sp, #8]
	ldr r1, [sp, #12]
	adds r3, r4, #0
	muls r3, r0
	asrs r3, r3, #10
	subs r3, r7, r3
	stmia r1!, {r3}
	movs r3, #128
	lsls r3, r3, #4
	adds r6, #1
	adds r5, r5, r3
	cmp r6, #160
	bne .L_08145998
	ldr r4, [sp, #20]
	cmp r4, #0
	bne .L_081459c4
	b .L_08145ba2
.L_081459c4:
	mov r5, r11
	ldr r0, [r5, #4]
	cmp r0, #0
	bne .L_081459d4
	movs r6, #0
	movs r7, #0
	mov r10, r6
	b .L_081459da
.L_081459d4:
	movs r1, #0
	movs r7, #1
	mov r10, r1
.L_081459da:
	mov r2, r8
	cmp r2, #71
	bgt .L_08145a12
	ldr r2, .L_08145c40
	lsls r3, r7, #3
	mov r4, r10
	subs r3, r3, r7
	ldr r1, .L_08145c44
	ldrb r2, [r2, r3]
	lsls r3, r4, #3
	subs r3, r3, r4
	ldrb r3, [r1, r3]
	movs r1, #57
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	ldr r5, [sp, #16]
	lsls r0, r0, #2
	ldr r6, [sp, #36]
	ldr r4, [r0, r5]
	movs r5, #224
	lsls r5, r5, #3
	add r3, r9
	ldr r0, [sp, #32]
	adds r1, r6, r5
	mov lr, r4
	.2byte 0xf800
	b .L_08145ba2
.L_08145a12:
	mov r6, r8
	cmp r6, #75
	bgt .L_08145a56
	ldr r2, .L_08145c40
	lsls r6, r7, #3
	subs r3, r6, r7
	ldrb r2, [r2, r3]
	ldr r1, .L_08145c44
	mov r12, r2
	mov r2, r10
	lsls r5, r2, #3
	subs r3, r5, r2
	ldrb r3, [r1, r3]
	movs r1, #57
	str r1, [sp, #0]
	add r3, r9
	movs r1, #98
	mov lr, r3
	str r1, [sp, #4]
	ldr r3, [sp, #16]
	ldr r2, [sp, #36]
	lsls r0, r0, #2
	ldr r4, [r0, r3]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, [sp, #32]
	mov r2, r12
	mov r3, lr
	mov lr, r4
	.2byte 0xf800
	mov r4, r11
	ldr r0, [r4, #4]
	b .L_08145a5c
.L_08145a56:
	mov r1, r10
	lsls r6, r7, #3
	lsls r5, r1, #3
.L_08145a5c:
	ldr r2, .L_08145c40
	subs r6, r6, r7
	adds r3, r6, #1
	ldrb r3, [r2, r3]
	ldr r7, .L_08145c44
	mov r12, r3
	mov r3, r10
	subs r5, r5, r3
	adds r3, r5, #1
	ldrb r3, [r7, r3]
	movs r1, #99
	str r1, [sp, #0]
	add r3, r9
	movs r1, #69
	mov lr, r3
	str r1, [sp, #4]
	ldr r2, [sp, #36]
	ldr r1, [sp, #16]
	movs r3, #224
	lsls r3, r3, #5
	lsls r0, r0, #2
	adds r3, #210
	ldr r4, [r0, r1]
	adds r1, r2, r3
	ldr r0, [sp, #32]
	mov r3, lr
	mov r2, r12
	mov lr, r4
	.2byte 0xf800
	mov r3, r8
	subs r3, #72
	cmp r3, #1
	bhi .L_08145aac
	movs r1, #128
	ldr r3, .L_08145c48
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_08145c4c
	mov lr, r3
	.2byte 0xf800
.L_08145aac:
	mov r3, r8
	subs r3, #74
	cmp r3, #1
	bhi .L_08145aea
	ldr r1, .L_08145c40
	adds r3, r6, #2
	ldrb r3, [r1, r3]
	mov r4, r11
	mov r12, r3
	movs r1, #128
	adds r3, r5, #2
	ldr r0, [r4, #4]
	ldrb r3, [r7, r3]
	str r1, [sp, #0]
	movs r1, #91
	str r1, [sp, #4]
	ldr r2, [sp, #16]
	lsls r0, r0, #2
	add r3, r9
	mov lr, r3
	ldr r4, [r0, r2]
	ldr r3, [sp, #36]
	movs r2, #220
	lsls r2, r2, #6
	adds r2, #129
	adds r1, r3, r2
	ldr r0, [sp, #32]
	mov r2, r12
	mov r3, lr
	mov lr, r4
	.2byte 0xf800
.L_08145aea:
	mov r3, r8
	subs r3, #76
	cmp r3, #1
	bhi .L_08145b18
	ldr r4, .L_08145c40
	mov r3, r11
	ldr r0, [r3, #4]
	adds r3, r6, #3
	ldrb r2, [r4, r3]
	movs r1, #128
	adds r3, r5, #3
	ldrb r3, [r7, r3]
	str r1, [sp, #0]
	movs r1, #91
	str r1, [sp, #4]
	ldr r1, [sp, #16]
	lsls r0, r0, #2
	ldr r4, [r0, r1]
	add r3, r9
	ldr r0, [sp, #32]
	ldr r1, .L_08145c50
	mov lr, r4
	.2byte 0xf800
.L_08145b18:
	mov r3, r8
	subs r3, #78
	cmp r3, #1
	bhi .L_08145b46
	ldr r4, .L_08145c40
	mov r2, r11
	adds r3, r6, #4
	ldr r0, [r2, #4]
	movs r1, #128
	ldrb r2, [r4, r3]
	adds r3, r5, #4
	ldrb r3, [r7, r3]
	str r1, [sp, #0]
	movs r1, #59
	str r1, [sp, #4]
	ldr r1, [sp, #16]
	lsls r0, r0, #2
	ldr r4, [r0, r1]
	add r3, r9
	ldr r0, [sp, #32]
	ldr r1, .L_08145c54
	mov lr, r4
	.2byte 0xf800
.L_08145b46:
	mov r3, r8
	subs r3, #80
	cmp r3, #1
	bhi .L_08145b74
	ldr r4, .L_08145c40
	mov r2, r11
	adds r3, r6, #5
	ldr r0, [r2, #4]
	movs r1, #122
	ldrb r2, [r4, r3]
	adds r3, r5, #5
	ldrb r3, [r7, r3]
	str r1, [sp, #0]
	movs r1, #29
	str r1, [sp, #4]
	ldr r1, [sp, #16]
	lsls r0, r0, #2
	ldr r4, [r0, r1]
	add r3, r9
	ldr r0, [sp, #32]
	ldr r1, .L_08145c58
	mov lr, r4
	.2byte 0xf800
.L_08145b74:
	mov r3, r8
	subs r3, #82
	cmp r3, #1
	bhi .L_08145ba2
	ldr r4, .L_08145c40
	mov r2, r11
	adds r3, r6, #6
	ldr r0, [r2, #4]
	movs r1, #76
	ldrb r2, [r4, r3]
	adds r3, r5, #6
	ldrb r3, [r7, r3]
	str r1, [sp, #0]
	movs r1, #25
	str r1, [sp, #4]
	ldr r5, [sp, #16]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	add r3, r9
	ldr r0, [sp, #32]
	ldr r1, .L_08145c5c
	mov lr, r4
	.2byte 0xf800
.L_08145ba2:
	mov r6, r8
	cmp r6, #68
	bne .L_08145bae
	movs r0, #212
	bl Audio_PlayCue
.L_08145bae:
	mov r1, r8
	cmp r1, #72
	bne .L_08145bd4
	mov r3, r11
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r1, #0
	bl Func_08118088
	movs r5, #238
	ldr r4, [sp, #36]
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r4, r5
	movs r3, #8
	str r3, [r2]
	movs r0, #144
	bl Func_081180e8
.L_08145bd4:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r6, [sp, #36]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r6, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #104
	beq .L_08145c00
	b .L_08145862
.L_08145c00:
	ldr r0, .L_08145c60
	bl Scheduler_RemoveCallback
	ldr r0, .L_08145c64
	bl Scheduler_RemoveCallback
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r4, #206
	lsls r4, r4, #3
	adds r3, r3, r4
	ldrh r1, [r3]
	movs r2, #24
	movs r0, #1
	bl Func_08118040
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
.L_08145c40:
	.4byte Data_081978f0
.L_08145c44:
	.4byte Data_081978fe
.L_08145c48:
	.4byte IwramFillWords
.L_08145c4c:
	.4byte 0x3f3f3f3f
.L_08145c50:
	.4byte gMapCellBuffer
.L_08145c54:
	.4byte Data_02012d80
.L_08145c58:
	.4byte Data_02014b00
.L_08145c5c:
	.4byte Data_020158d2
.L_08145c60:
	.4byte Func_08143000
.L_08145c64:
	.4byte Func_08152474
