.syntax unified
	.thumb
	.global Func_08044a58
	.thumb_func
Func_08044a58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r2, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	ldr r2, .L_08044b70
	adds r6, r1, #0
	movs r3, #3
	ands r3, r6
	movs r1, #0
	lsls r3, r3, #1
	mov r9, r1
	mov r10, r1
	ldrsh r7, [r2, r3]
	add r2, sp, #20
	mov r3, r9
	str r3, [sp, #0]
	mov r11, r0
	mov r8, r2
	movs r0, #2
	movs r1, #1
	movs r2, #26
	movs r3, #5
	bl UiWindow_Create
	mov r9, r0
	cmp r0, #0
	beq .L_08044b60
	movs r3, #4
	movs r1, #4
	movs r2, #0
	str r3, [sp, #0]
	bl UiWindow_DrawDividerLine
	movs r3, #1
	strb r3, [r5, #3]
	adds r0, r7, #0
	bl Func_0803d2f0
	movs r1, #14
	add r2, sp, #16
	add r3, sp, #12
	str r1, [sp, #0]
	mov r1, r10
	str r1, [sp, #4]
	bl Func_0803dab0
	mov r2, r10
	mov r3, r8
	str r2, [r3]
	ldr r3, .L_08044b74
	movs r2, #224
	str r3, [sp, #24]
	ldr r3, [sp, #12]
	movs r1, #152
	lsls r2, r2, #8
	lsls r1, r1, #5
	orrs r3, r2
	adds r1, #132
	str r3, [sp, #28]
	mov r2, r10
	adds r3, r5, r1
	adds r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	strh r2, [r3]
	mov r0, r11
	movs r1, #1
	bl Func_0803ccd0
	lsls r0, r6, #2
	ldr r3, [sp, #8]
	adds r0, r0, r6
	lsls r0, r0, #2
	movs r1, #150
	adds r0, r0, r3
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #4
	bl Func_0803ccd0
	ldr r0, .L_08044b78
	movs r5, #129
	adds r0, r6, r0
	bl Func_0803cf60
	mov r2, r10
	adds r1, r0, #0
	str r2, [sp, #0]
	movs r3, #2
	movs r2, #36
	mov r0, r9
	bl Func_0803954c
	movs r0, #81
	bl Audio_PlayCue
	ldr r6, .L_08044b7c
	lsls r5, r5, #2
	adds r5, #255
.L_08044b2e:
	movs r1, #250
	mov r0, r8
	bl Func_08014128
	movs r0, #1
	bl WaitFrames
	bl Audio_Check
	cmp r0, #0
	beq .L_08044b4c
	ldr r3, [r6, #4]
	ands r3, r5
	cmp r3, #0
	beq .L_08044b2e
.L_08044b4c:
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #16]
	bl Func_08014274
.L_08044b60:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08044b70:
	.4byte Data_0805f65e
.L_08044b74:
	.4byte 0x8014000c
.L_08044b78:
	.4byte 0x0000002e
.L_08044b7c:
	.4byte gInput
