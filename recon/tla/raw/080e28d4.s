.syntax unified
	.thumb
	.global Func_080e28d4
	.thumb_func
Func_080e28d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r2
	adds r6, r1, #0
	sub sp, #24
	adds r5, r0, #0
	bl Func_080cdf5c
	mov r9, r0
	bl Object_GetById
	mov r10, r0
	mov r0, r8
	lsls r0, r0, #16
	mov r8, r0
	lsls r6, r6, #16
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	mov r3, r8
	bl Func_080200c0
	adds r7, r0, #0
	cmp r7, #0
	bne .L_080e2912
	b .L_080e2a5a
.L_080e2912:
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r3, #178
	lsls r3, r3, #7
	adds r3, #153
	str r3, [r7, #72]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r7, #40]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r7, #24]
	str r3, [r7, #28]
	ldr r5, .L_080e2934
	b .L_080e2938
.L_080e2934:
	.4byte 0x00001000
.L_080e2938:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r7, #24]
	movs r3, #182
	lsls r3, r3, #2
	adds r3, #255
	adds r2, r2, r3
	ldrh r3, [r7, #6]
	movs r0, #255
	lsls r0, r0, #8
	adds r3, r3, r5
	adds r0, #255
	str r2, [r7, #24]
	str r2, [r7, #28]
	strh r3, [r7, #6]
	cmp r2, r0
	ble .L_080e2938
	movs r5, #128
	lsls r5, r5, #9
	mov r0, r10
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r1, #1
	bl Object_SetMode
	mov r1, r10
	adds r0, r7, #0
	bl FacingObject_TurnPairToFaceEachOther
	mov r3, r10
	ldrh r2, [r3, #6]
	adds r3, #100
	strh r2, [r3]
	movs r0, #10
	bl Battle_WaitMode0
	mov r0, r9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	mov r0, r9
	bl Func_080d489c
	movs r0, #32
	bl Battle_WaitMode0
	str r5, [r7, #72]
	movs r5, #192
	lsls r5, r5, #11
	movs r0, #152
	str r5, [r7, #40]
	bl Audio_PlayCue
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #152
	str r5, [r7, #40]
	bl Audio_PlayCue
	movs r0, #23
	bl Battle_WaitMode0
	movs r0, #146
	bl Audio_PlayCue
	ldr r3, [r7, #8]
	add r1, sp, #12
	str r3, [r1]
	ldr r3, [r7, #12]
	mov r0, r10
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	mov r2, sp
	str r3, [r1, #8]
	mov r9, r1
	ldr r3, [r0, #8]
	mov r10, r2
	str r3, [r2]
	ldr r3, [r0, #12]
	str r3, [r2, #4]
	ldr r3, [r0, #16]
	movs r0, #0
	str r3, [r2, #8]
	movs r3, #20
	mov r11, r3
	mov r8, r0
.L_080e29ec:
	mov r2, r10
	mov r0, r9
	ldr r3, [r2]
	ldr r5, [r0]
	movs r1, #20
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	bl __divsi3
	mov r2, r8
	adds r5, r5, r0
	movs r1, #20
	lsls r0, r2, #15
	str r5, [r7, #8]
	bl __divsi3
	bl Trig_Sin
	mov r2, r9
	adds r6, r0, #0
	mov r0, r10
	ldr r5, [r2, #4]
	ldr r3, [r0, #4]
	movs r1, #20
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	bl __divsi3
	lsls r3, r6, #2
	adds r3, r3, r6
	adds r5, r5, r0
	lsls r3, r3, #3
	adds r5, r5, r3
	str r5, [r7, #12]
	mov r2, r9
	mov r0, r10
	ldr r5, [r2, #8]
	ldr r3, [r0, #8]
	movs r1, #20
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r7, #16]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r8, r3
	cmp r8, r11
	blt .L_080e29ec
.L_080e2a5a:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
