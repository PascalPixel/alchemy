.syntax unified
	.thumb
	.global Func_08044dc8
	.thumb_func
Func_08044dc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	sub sp, #52
	lsls r3, r3, #18
	movs r2, #0
	ldr r5, [r3, #60]
	adds r7, r0, #0
	str r2, [sp, #0]
	movs r0, #4
	movs r1, #1
	movs r2, #22
	movs r3, #5
	bl UiWindow_Create
	movs r6, #0
	mov r9, r0
	cmp r0, #0
	bne .L_08044dfa
	b .L_08044f28
.L_08044dfa:
	movs r2, #152
	lsls r2, r2, #5
	movs r3, #1
	adds r2, #132
	strb r3, [r5, #3]
	adds r3, r5, r2
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	strh r6, [r3]
	ldr r2, .L_08044f38
	movs r3, #31
	ands r3, r7
	lsls r3, r3, #1
	ldrh r0, [r2, r3]
	movs r1, #4
	bl Func_0803ccd0
	ldr r0, .L_08044f3c
	bl Func_0803cf60
	mov r10, r0
	mov r1, r10
	movs r2, #0
	movs r3, #2
	mov r0, r9
	str r6, [sp, #0]
	bl Func_0803954c
	movs r0, #81
	bl Audio_PlayCue
	movs r5, #129
	ldr r6, .L_08044f40
	lsls r5, r5, #2
	adds r5, #255
.L_08044e42:
	movs r0, #1
	bl WaitFrames
	bl Audio_Check
	cmp r0, #0
	beq .L_08044e58
	ldr r3, [r6, #4]
	ands r3, r5
	cmp r3, #0
	beq .L_08044e42
.L_08044e58:
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	movs r3, #0
	str r3, [sp, #0]
	movs r0, #3
	movs r1, #1
	movs r2, #24
	movs r3, #7
	bl UiWindow_Create
	mov r9, r0
	cmp r0, #0
	beq .L_08044f28
	ldr r2, .L_08044f38
	movs r3, #31
	ands r3, r7
	lsls r3, r3, #1
	ldrh r0, [r2, r3]
	movs r1, #4
	bl Func_0803ccd0
	ldr r0, .L_08044f44
	bl Func_0803cf60
	mov r10, r0
	adds r0, r7, #0
	bl SummonDefinition_Get
	add r7, sp, #8
	adds r2, r7, #0
	adds r0, #4
	movs r1, #3
.L_08044ea2:
	ldrb r3, [r0]
	subs r1, #1
	adds r0, #1
	stmia r2!, {r3}
	cmp r1, #0
	bge .L_08044ea2
	movs r3, #0
	mov r8, r3
	mov r11, r7
.L_08044eb4:
	movs r5, #0
	movs r0, #0
	movs r1, #0
	mov r2, r11
.L_08044ebc:
	ldmia r2!, {r3}
	cmp r0, r3
	bge .L_08044ec6
	adds r5, r1, #0
	adds r0, r3, #0
.L_08044ec6:
	adds r1, #1
	cmp r1, #3
	ble .L_08044ebc
	movs r1, #5
	bl Func_0803ccd0
	ldr r6, .L_08044f48
	adds r0, r5, r6
	bl Func_0803cf6c
	movs r2, #1
	lsls r3, r5, #2
	add r8, r2
	movs r5, #0
	str r5, [r7, r3]
	mov r3, r8
	cmp r3, #1
	ble .L_08044eb4
	adds r0, r6, #4
	bl Func_0803cf6c
	mov r0, r9
	mov r1, r10
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl Func_0803954c
	movs r5, #129
	ldr r6, .L_08044f40
	lsls r5, r5, #2
	adds r5, #255
.L_08044f06:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #4]
	ands r3, r5
	cmp r3, #0
	beq .L_08044f06
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #4]
	bl Func_08014274
.L_08044f28:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08044f38:
	.4byte Data_0805f676
.L_08044f3c:
	.4byte 0x00000032
.L_08044f40:
	.4byte gInput
.L_08044f44:
	.4byte 0x00000033
.L_08044f48:
	.4byte 0x00000034
