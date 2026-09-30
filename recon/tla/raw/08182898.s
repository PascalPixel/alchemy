.syntax unified
	.thumb
	.global Func_08182898
	.thumb_func
Func_08182898:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r9, r0
	ldr r0, [r5, #96]
	sub sp, #24
	str r0, [sp, #20]
	ldr r0, .L_08182a84
	ldr r1, [r5, #92]
	mov r11, r1
	movs r1, #200
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	str r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r0, .L_08182a88
	str r5, [sp, #12]
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08182a8c
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r1, #224
	lsls r1, r1, #3
	movs r2, #0
	ldr r0, .L_08182a90
	add r1, r11
	movs r3, #0
	bl Func_08157cf4
	movs r2, #0
	str r2, [sp, #8]
.L_0818290a:
	ldr r3, [sp, #8]
	cmp r3, #0
	bne .L_08182968
	ldr r5, .L_08182a94
	movs r4, #0
	mov r8, r4
	movs r7, #127
	movs r6, #31
.L_0818291a:
	bl Random16
	ands r0, r7
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r7
	adds r0, #120
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	movs r3, #8
	ands r0, r6
	subs r3, r3, r0
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #5
	movs r0, #1
	movs r1, #128
	str r3, [r5, #8]
	add r8, r0
	movs r3, #32
	lsls r1, r1, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r8, r1
	bne .L_0818291a
.L_08182968:
	ldr r3, .L_08182a98
	ldr r7, .L_08182a94
	movs r2, #0
	mov r8, r2
	mov r10, r3
.L_08182972:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_081829c8
	ldr r6, [r7, #8]
	ldr r2, .L_08182a9c
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_08182aa0
	movs r0, #2
	ldrsh r2, [r7, r0]
	ldrb r5, [r3, r6]
	movs r4, #224
	lsls r4, r4, #3
	lsrs r3, r5, #1
	mov r0, r10
	add r1, r11
	adds r1, r1, r4
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r7, r4]
	ldrb r4, [r0, r6]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #20]
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	ldr r2, .L_08182aa4
	movs r1, #63
	bl BattleFxKernels_IntegrateVector2
	mov r0, r10
	ldrb r3, [r0, r6]
	ldr r2, [r7, #4]
	lsls r3, r3, #16
	cmn r2, r3
	bge .L_081829c8
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_081829c8:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #1
	adds r7, #28
	cmp r8, r2
	bne .L_08182972
	mov r4, r9
	ldr r2, [r4, #20]
	movs r3, #0
	mov r8, r3
	cmp r2, #0
	beq .L_08182a42
	movs r6, #16
	movs r5, #36
.L_081829e6:
	mov r0, r8
	ldr r1, [sp, #8]
	lsls r3, r0, #3
	adds r3, #18
	cmp r1, r3
	bne .L_08182a38
	cmp r0, #0
	bne .L_08182a1c
	mov r3, r9
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r2, #1
	negs r2, r2
	movs r1, #7
	movs r3, #0
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r0, #126
	bl Func_081180e8
	movs r0, #126
	bl Audio_PlayCue
	mov r4, r9
	ldr r2, [r4, #20]
	b .L_08182a38
.L_08182a1c:
	movs r0, #126
	bl Audio_PlayCue
	mov r1, r9
	ldrsh r0, [r5, r1]
	movs r2, #1
	negs r2, r2
	mov r3, r8
	movs r1, #7
	str r6, [sp, #0]
	bl Func_0814cd48
	mov r3, r9
	ldr r2, [r3, #20]
.L_08182a38:
	movs r4, #1
	add r8, r4
	adds r5, #2
	cmp r8, r2
	bne .L_081829e6
.L_08182a42:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #8]
	adds r0, #1
	str r0, [sp, #8]
	cmp r0, #60
	beq .L_08182a64
	b .L_0818290a
.L_08182a64:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08182a84
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08182a84:
	.4byte Func_08143000
.L_08182a88:
	.4byte 0x00000190
.L_08182a8c:
	.4byte IwramCopyWords
.L_08182a90:
	.4byte 0x000000fd
.L_08182a94:
	.4byte Data_02015000
.L_08182a98:
	.4byte Data_0819967e
.L_08182a9c:
	.4byte Data_0819968e
.L_08182aa0:
	.4byte Data_0819966f
.L_08182aa4:
	.4byte 0xffffc000
