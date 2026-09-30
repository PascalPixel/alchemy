.syntax unified
	.thumb
	.global Func_080e03cc
	.thumb_func
Func_080e03cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	sub sp, #40
	ldr r1, [r5, #20]
	ldr r7, [r5, #16]
	str r1, [sp, #0]
	ldr r3, [r7, #8]
	add r2, sp, #16
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	mov r11, r2
	str r3, [r2, #8]
	adds r3, r5, #0
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e042c
	ldr r3, [r7, #8]
	add r2, sp, #4
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r0, #128
	lsls r0, r0, #14
	adds r3, r3, r0
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	mov r9, r2
	str r3, [r2, #8]
	ldrh r1, [r5]
	bl Func_0801489c
	b .L_080e0444
.L_080e042c:
	add r3, sp, #4
	mov r9, r3
	ldr r3, [r5, #4]
	mov r1, r9
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #8]
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r3, [r5, #12]
	str r3, [r1, #8]
.L_080e0444:
	ldr r1, [r5, #4]
	add r0, sp, #28
	str r1, [r0]
	movs r3, #128
	ldr r2, [r5, #8]
	lsls r3, r3, #14
	adds r2, r2, r3
	str r2, [r0, #4]
	ldr r3, [r5, #12]
	str r3, [r0, #8]
	movs r0, #139
	lsls r0, r0, #1
	bl Func_080dc10c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080e0468
	b .L_080e05fe
.L_080e0468:
	bl BattleEffect_InitializeSharedScene
	movs r0, #138
	bl Audio_PlayCue
	ldrh r3, [r7, #6]
	ldr r2, .L_080e04a0
	strh r3, [r6, #6]
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r6, #48]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r0, r6, #0
	movs r1, #5
	bl Object_SetMode
	adds r0, r6, #0
	movs r1, #1
	bl Animation_ApplyChildValuesFar
	movs r7, #0
	mov r10, r11
	mov r8, r9
	b .L_080e04a4
	.2byte 0x0000
.L_080e04a0:
	.4byte 0x00000000
.L_080e04a4:
	mov r2, r10
	mov r1, r8
	ldr r5, [r2]
	ldr r3, [r1]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6, #8]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6, #12]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	movs r3, #192
	lsls r3, r3, #8
	adds r5, r5, r0
	movs r1, #10
	adds r0, r7, #0
	muls r0, r3
	str r5, [r6, #16]
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #7
	adds r0, r0, r3
	str r0, [r6, #24]
	str r0, [r6, #28]
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, #11
	blt .L_080e04a4
	movs r0, #10
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #6
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r5, #9
.L_080e0526:
	ldr r3, [r6, #12]
	ldr r1, .L_080e060c
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_080e0526
	adds r0, r6, #0
	movs r1, #5
	bl Object_SetMode
	movs r0, #132
	bl Audio_PlayCue
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_080e055a
	ldr r3, .L_080e0610
	ldr r2, [r2, #12]
	ldr r0, [sp, #0]
	adds r1, r3, #0
	bl Object_SetPositionAndResetMotionFar
.L_080e055a:
	movs r0, #20
	bl WaitFrames
	movs r5, #12
.L_080e0562:
	ldr r3, [r6, #12]
	movs r1, #192
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_080e0562
	movs r0, #10
	bl WaitFrames
	movs r0, #114
	bl Audio_PlayCue
	movs r7, #0
	mov r10, r9
	mov r8, r11
.L_080e058a:
	mov r2, r8
	mov r1, r10
	ldr r3, [r2]
	ldr r5, [r1]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6, #8]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2, #4]
	ldr r5, [r1, #4]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6, #12]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2, #8]
	ldr r5, [r1, #8]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	ldr r3, .L_080e0614
	adds r5, r5, r0
	movs r1, #10
	adds r0, r7, #0
	muls r0, r3
	str r5, [r6, #16]
	bl __divsi3
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	str r0, [r6, #24]
	str r0, [r6, #28]
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, #11
	blt .L_080e058a
	adds r0, r6, #0
	bl Func_080200c8
	bl BattleFx_PrepareBufferInterpolation
.L_080e05fe:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e060c:
	.4byte 0xfffe0000
.L_080e0610:
	.4byte 0xfff70000
.L_080e0614:
	.4byte 0xffff4000
