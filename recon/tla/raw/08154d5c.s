.syntax unified
	.thumb
	.global Func_08154d5c
	.thumb_func
Func_08154d5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #92]
	ldr r2, [r0, #96]
	sub sp, #72
	mov r8, r0
	movs r0, #0
	str r2, [sp, #12]
	mov r10, r1
	bl BattleFx_BeginCanvasLayer
	mov r3, r9
	ldr r1, [r3, #4]
	movs r3, #132
	lsls r1, r1, #4
	add r2, sp, #60
	orrs r1, r3
	mov r0, r9
	add r3, sp, #48
	bl Func_0815585c
	ldr r3, .L_08154dd0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_08154dd4
	adds r2, #20
	strh r3, [r2]
	ldr r3, .L_08154dd8
	movs r1, #224
	adds r2, #48
	lsls r1, r1, #3
	strh r3, [r2]
	add r1, r10
	movs r3, #1
	ldr r0, .L_08154ddc
	movs r2, #1
	bl Resource_LoadAndDecompress
	mov r2, r9
	add r6, sp, #36
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r6, #0
	bl Func_0815e20c
	mov r0, r9
	ldr r3, [r0, #20]
	b .L_08154de0
.L_08154dd0:
	.4byte 0x00000785
.L_08154dd4:
	.4byte 0x00000100
.L_08154dd8:
	.4byte 0x00000000
.L_08154ddc:
	.4byte 0x0000016b
.L_08154de0:
	add r5, sp, #24
	lsls r3, r3, #1
	adds r3, #34
	ldrsh r0, [r0, r3]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r1, [r6]
	ldr r3, [r5]
	movs r0, #104
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	movs r2, #128
	movs r3, #64
	subs r3, r3, r1
	lsls r2, r2, #19
	adds r2, #40
	lsls r3, r3, #8
	str r1, [r6]
	str r3, [r2]
	movs r1, #35
	bl Func_081963ec
	mov r2, r8
	ldr r3, [r2, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #16]
	bl Func_081963ec
	movs r3, #188
	add r8, r3
	mov r0, r8
	ldr r3, [r0]
	add r1, sp, #16
	mov r11, r1
	str r3, [r1, #4]
	ldr r1, .L_08154f7c
	movs r6, #0
	mov r2, r10
.L_08154e36:
	ldrb r3, [r1]
	adds r6, #1
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r3, #64
	str r3, [r2]
	adds r1, #1
	adds r2, #28
	cmp r6, #16
	bne .L_08154e36
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08154f80
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r7, #0
.L_08154e6c:
	cmp r7, #32
	bne .L_08154ea4
	movs r0, #143
	bl Audio_PlayCue
	mov r2, r9
	ldr r3, [r2, #20]
	movs r6, #0
	cmp r3, #0
	beq .L_08154ea4
	movs r4, #16
	movs r5, #36
.L_08154e84:
	mov r3, r9
	ldrsh r0, [r5, r3]
	movs r2, #5
	adds r3, r6, #0
	movs r1, #7
	str r4, [sp, #0]
	str r4, [sp, #8]
	bl Func_0814cd48
	mov r2, r9
	ldr r3, [r2, #20]
	adds r6, #1
	adds r5, #2
	ldr r4, [sp, #8]
	cmp r6, r3
	bne .L_08154e84
.L_08154ea4:
	movs r6, #0
	mov r8, r10
.L_08154ea8:
	lsls r3, r6, #2
	adds r3, #5
	cmp r7, r3
	bne .L_08154ebc
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #2
	str r3, [r2]
.L_08154ebc:
	lsls r5, r6, #1
	adds r3, r5, #4
	cmp r7, r3
	ble .L_08154f28
	adds r0, r7, #0
	cmp r7, #0
	bge .L_08154ecc
	adds r0, r7, #3
.L_08154ecc:
	asrs r0, r0, #2
	adds r0, r0, r6
	movs r1, #5
	bl Math_Mod
	adds r3, r5, #0
	adds r3, #32
	cmp r7, r3
	bge .L_08154eee
	subs r3, r7, r5
	lsls r3, r3, #2
	adds r5, r3, #0
	subs r5, #16
	cmp r5, #32
	ble .L_08154ef6
	movs r5, #32
	b .L_08154ef6
.L_08154eee:
	subs r3, r7, r5
	lsls r3, r3, #2
	movs r2, #160
	subs r5, r2, r3
.L_08154ef6:
	cmp r5, #0
	ble .L_08154f28
	lsls r1, r0, #10
	movs r3, #224
	mov r0, r8
	ldr r2, [r0]
	lsls r3, r3, #3
	movs r0, #32
	add r1, r10
	str r0, [sp, #0]
	movs r4, #1
	adds r1, r1, r3
	str r5, [sp, #4]
	movs r3, #7
	ands r4, r6
	ands r3, r6
	mov r0, r11
	lsls r4, r4, #2
	subs r3, r3, r5
	ldr r4, [r4, r0]
	subs r2, #16
	adds r3, #104
	ldr r0, [sp, #12]
	mov lr, r4
	.2byte 0xf800
.L_08154f28:
	movs r1, #28
	adds r6, #1
	add r8, r1
	cmp r6, #16
	bne .L_08154ea8
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #70
	bne .L_08154e6c
	ldr r0, .L_08154f80
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
	.2byte 0x0000
.L_08154f7c:
	.4byte Data_08198422
.L_08154f80:
	.4byte Func_08143000
