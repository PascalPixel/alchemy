.syntax unified
	.thumb
	.global Func_080d7524
	.thumb_func
Func_080d7524:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #52
	mov r9, r0
	bl Object_GetById
	ldr r3, .L_080d7770
	adds r7, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	str r0, [sp, #16]
	movs r1, #128
	ldrh r3, [r0, #6]
	lsls r1, r1, #6
	adds r1, r3, r1
	movs r3, #192
	lsls r3, r3, #8
	ands r1, r3
	str r1, [sp, #8]
	bl Func_080d22a8
	movs r0, #10
	bl WaitFrames
	movs r0, #173
	bl Audio_PlayCue
	movs r1, #1
	mov r0, r9
	bl Motion_SetVarCbAndRefresh
	movs r0, #175
	bl Audio_PlayCue
	movs r1, #1
	mov r0, r9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl WaitFrames
	ldr r2, [sp, #8]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r2, r3
	adds r1, r5, #0
	movs r2, #0
	mov r0, r9
	bl Func_080d3838
	movs r0, #10
	bl WaitFrames
	movs r1, #4
	adds r1, #255
	movs r2, #50
	strh r5, [r7, #6]
	mov r0, r9
	bl Func_080d47b4
	mov r0, r9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	adds r0, r7, #0
	adds r0, #85
	movs r3, #2
	str r0, [sp, #4]
	strb r3, [r0]
	ldr r3, .L_080d7774
	movs r0, #152
	str r3, [r7, #108]
	bl Audio_PlayCue
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #72]
	movs r3, #144
	lsls r3, r3, #11
	str r3, [r7, #40]
	movs r0, #33
	bl WaitFrames
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #9]
	movs r0, #152
	bl Audio_PlayCue
	ldr r3, .L_080d7778
	movs r0, #35
	str r3, [r7, #40]
	bl WaitFrames
	movs r0, #152
	bl Audio_PlayCue
	ldr r3, .L_080d777c
	movs r0, #38
	str r3, [r7, #40]
	bl WaitFrames
	ldr r3, [r7, #80]
	movs r6, #0
	ldr r3, [r3, #40]
	str r6, [r7, #108]
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #9
	strh r5, [r7, #6]
	mov r8, r1
	mov r11, r3
	cmp r1, #243
	bne .L_080d762c
	movs r0, #2
	mov r11, r0
.L_080d762c:
	mov r1, r8
	cmp r1, #245
	bne .L_080d7636
	movs r2, #10
	mov r11, r2
.L_080d7636:
	mov r3, r8
	cmp r3, #244
	bne .L_080d7640
	movs r0, #9
	mov r11, r0
.L_080d7640:
	mov r2, sp
	movs r1, #0
	adds r2, #20
	str r1, [sp, #12]
	str r2, [sp, #0]
	mov r10, r7
	movs r6, #0
.L_080d764e:
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	mov r0, r8
	bl Func_080200c0
	adds r5, r0, #0
	ldr r0, [sp, #0]
	lsls r3, r6, #2
	str r5, [r0, r3]
	cmp r5, #0
	beq .L_080d76c6
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
	ldr r3, .L_080d7780
	ldr r1, [r5, #80]
	str r3, [r5, #108]
	ldrh r3, [r7, #6]
	movs r0, #13
	strh r3, [r5, #6]
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	mov r1, r11
	bl Animation_ApplyChildValuesFar
	adds r0, r5, #0
	movs r1, #0
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r0, [r5, #80]
	ldr r1, [sp, #12]
	bl Func_080dc0d8
	mov r1, r10
	str r0, [sp, #12]
	str r1, [r5, #104]
	mov r10, r5
.L_080d76c6:
	adds r6, #1
	cmp r6, #7
	ble .L_080d764e
	movs r3, #152
	lsls r3, r3, #7
	adds r3, #204
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, [sp, #8]
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #128
	adds r2, r3, r0
	lsls r1, r1, #13
	adds r0, r7, #0
	bl Func_080db974
	movs r0, #153
	bl Audio_PlayCue
	mov r0, r9
	movs r1, #6
	movs r2, #5
	bl ObjectMotion_Launch
	movs r0, #24
	bl WaitFrames
	movs r6, #0
	b .L_080d770a
.L_080d7702:
	movs r0, #1
	bl WaitFrames
	adds r6, #1
.L_080d770a:
	cmp r6, #119
	bgt .L_080d772a
	ldr r2, [sp, #16]
	ldr r1, [r7, #12]
	ldr r3, [r2, #12]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_080d7722
	ldr r3, .L_080d7784
	cmp r2, r3
	ble .L_080d772a
	b .L_080d7702
.L_080d7722:
	ldr r0, .L_080d7784
	subs r3, r3, r1
	cmp r3, r0
	bgt .L_080d7702
.L_080d772a:
	ldr r1, [sp, #4]
	movs r3, #0
	strb r3, [r1]
	str r3, [r7, #36]
	str r3, [r7, #44]
	str r3, [r7, #40]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	str r3, [r7, #60]
	adds r0, r7, #0
	movs r1, #0
	bl Object_SetMode
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	bl Func_080d2350
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d7770:
	.4byte gPartyState
.L_080d7774:
	.4byte Func_080d74e4
.L_080d7778:
	.4byte 0x0004cccc
.L_080d777c:
	.4byte 0x0004e666
.L_080d7780:
	.4byte ObjectMotion_MoveHalfwayTowardTarget
.L_080d7784:
	.4byte 0x0013ffff
