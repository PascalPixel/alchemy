.syntax unified
	.thumb
	.global Func_080d8740
	.thumb_func
Func_080d8740:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #52
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080d875e
	b .L_080d8960
.L_080d875e:
	bl Func_080d7a78
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r0, #129
	lsls r0, r0, #14
	adds r0, #132
	str r3, [sp, #4]
	bl Func_08108058
	movs r0, #30
	bl WaitFrames
	adds r2, r6, #0
	movs r3, #0
	adds r2, #91
	strb r3, [r2]
	movs r0, #173
	bl Audio_PlayCue
	movs r1, #1
	adds r0, r5, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #175
	bl Audio_PlayCue
	movs r1, #1
	adds r0, r5, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl WaitFrames
	movs r0, #152
	bl Audio_PlayCue
	movs r1, #3
	movs r2, #14
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #152
	bl Audio_PlayCue
	movs r1, #5
	movs r2, #16
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #152
	bl Audio_PlayCue
	movs r1, #7
	movs r2, #18
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #20
	bl WaitFrames
	ldr r3, [r6, #80]
	mov r8, r6
	ldr r3, [r3, #40]
	movs r7, #7
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	mov r9, r1
	add r1, sp, #20
	str r1, [sp, #0]
	mov r10, r3
	mov r11, r1
.L_080d87f4:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	mov r0, r9
	bl Func_080200c0
	ldr r3, [sp, #0]
	adds r5, r0, #0
	stmia r3!, {r5}
	adds r2, r3, #0
	str r2, [sp, #0]
	cmp r5, #0
	beq .L_080d885e
	movs r3, #240
	lsls r3, r3, #8
	adds r2, r5, #0
	str r3, [r5, #28]
	str r3, [r5, #24]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	subs r2, #50
	movs r3, #2
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #90
	ldrb r3, [r1]
	movs r2, #1
	orrs r3, r2
	strb r3, [r1]
	ldr r3, .L_080d8970
	movs r1, #9
	str r3, [r5, #108]
	ldrh r3, [r6, #6]
	strh r3, [r5, #6]
	bl Animation_ApplyChildValuesFar
	adds r0, r5, #0
	movs r1, #0
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	mov r1, r10
	ldr r0, [r5, #80]
	bl Func_080dc0d8
	mov r1, r8
	str r1, [r5, #104]
	mov r10, r0
	mov r8, r5
.L_080d885e:
	subs r7, #1
	cmp r7, #0
	bge .L_080d87f4
	mov r2, r10
	ldrb r2, [r2, #16]
	movs r0, #153
	mov r8, r2
	bl Audio_PlayCue
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r7, #14
.L_080d887a:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r0, #1
	subs r7, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_080d887a
	adds r0, r6, #0
	bl Func_080200c8
	mov r5, r11
	movs r7, #7
.L_080d889a:
	ldmia r5!, {r0}
	subs r7, #1
	bl Func_080200c8
	cmp r7, #0
	bge .L_080d889a
	mov r2, r8
	cmp r2, #96
	beq .L_080d88b2
	mov r0, r8
	bl Func_08014274
.L_080d88b2:
	movs r0, #10
	bl WaitFrames
	ldr r5, .L_080d8974
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	ldr r0, [r5]
	bl Func_080d3838
	movs r0, #20
	bl WaitFrames
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #28
	bl Object_SetMode
	movs r0, #20
	bl WaitFrames
	ldr r3, [r6, #8]
	add r5, sp, #8
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	movs r7, #23
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_080dc390
	ldr r6, [sp, #4]
	adds r6, #80
.L_080d88fe:
	movs r1, #46
	ldr r2, [r5]
	ldr r3, [r5, #8]
	adds r0, r6, #0
	adds r1, #255
	bl Func_080ebec8
	adds r0, r6, #0
	ldr r1, .L_080d8978
	bl Func_080ebeb4
	adds r0, r6, #0
	movs r1, #7
	bl Func_080ebea8
	ldr r0, [r6]
	movs r1, #9
	bl Animation_ApplyChildValuesToRecordFar
	subs r7, #1
	movs r0, #1
	bl WaitFrames
	adds r6, #72
	cmp r7, #0
	bge .L_080d88fe
	movs r0, #120
	bl WaitFrames
	ldr r2, [sp, #4]
	movs r1, #2
	adds r2, #144
	movs r7, #23
.L_080d8940:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080d894a
	strb r1, [r2]
.L_080d894a:
	subs r7, #1
	adds r2, #72
	cmp r7, #0
	bge .L_080d8940
	movs r0, #50
	bl WaitFrames
	bl Func_08108060
	bl Func_080d7ab4
.L_080d8960:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d8970:
	.4byte ObjectMotion_MoveHalfwayTowardTargetCopy
.L_080d8974:
	.4byte gPartyState
.L_080d8978:
	.4byte Func_080d85b8
