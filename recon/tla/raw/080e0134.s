.syntax unified
	.thumb
	.global Func_080e0134
	.thumb_func
Func_080e0134:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	ldr r5, [r3, #16]
	mov r9, r3
	ldr r3, [r5, #12]
	mov r0, r9
	str r3, [r0, #8]
	movs r0, #54
	adds r0, #255
	movs r2, #0
	movs r3, #0
	sub sp, #36
	bl Func_080dc10c
	movs r1, #0
	adds r6, r0, #0
	movs r7, #0
	bl Object_SetMode
	cmp r6, #0
	bne .L_080e0170
	b .L_080e02dc
.L_080e0170:
	bl BattleEffect_InitializeSharedScene
	ldr r3, [r5, #8]
	add r1, sp, #12
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r1, #4]
	mov r0, r9
	ldr r3, [r5, #16]
	mov r2, sp
	str r3, [r1, #8]
	mov r10, r1
	ldr r3, [r0, #4]
	mov r8, r2
	str r3, [r2]
	ldr r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r2, #4]
	mov r0, r9
	ldr r3, [r0, #12]
	str r3, [r2, #8]
.L_080e01a4:
	mov r2, r8
	mov r0, r10
	ldr r3, [r2]
	ldr r5, [r0]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6, #8]
	mov r2, r8
	mov r0, r10
	ldr r3, [r2, #4]
	ldr r5, [r0, #4]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl __divsi3
	adds r5, r5, r0
	str r5, [r6, #12]
	mov r2, r8
	mov r0, r10
	ldr r3, [r2, #8]
	ldr r5, [r0, #8]
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
	movs r2, #128
	lsls r2, r2, #7
	adds r0, r0, r2
	str r0, [r6, #24]
	str r0, [r6, #28]
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, #11
	blt .L_080e01a4
	movs r0, #5
	bl WaitFrames
	movs r1, #1
	adds r0, r6, #0
	bl Object_SetMode
	movs r0, #108
	bl Audio_PlayCue
	movs r0, #10
	bl WaitFrames
	movs r0, #108
	bl Audio_PlayCue
	movs r0, #10
	bl WaitFrames
	movs r0, #108
	bl Audio_PlayCue
	movs r0, #10
	bl WaitFrames
	movs r0, #109
	bl Audio_PlayCue
	add r3, sp, #24
	mov r5, r9
	mov r8, r3
	movs r0, #15
	adds r5, #80
	mov r7, r8
	mov r10, r0
.L_080e0256:
	ldr r3, [r6, #8]
	movs r2, #128
	str r3, [r7]
	lsls r2, r2, #12
	ldr r3, [r6, #12]
	adds r0, r7, #0
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	bl Func_080dc390
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #11
	adds r2, r7, #0
	bl Func_0801489c
	movs r1, #209
	lsls r1, r1, #1
	ldr r3, [r7, #8]
	ldr r2, [r7]
	adds r0, r5, #0
	adds r1, #255
	bl Func_080ebec8
	adds r0, r5, #0
	ldr r1, .L_080e02e8
	bl Func_080ebeb4
	ldr r0, [r5]
	movs r1, #7
	bl Animation_ApplyChildValuesToRecordFar
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r0, r10
	adds r5, #72
	cmp r0, #0
	bge .L_080e0256
	ldr r3, [r6, #8]
	mov r2, r8
	str r3, [r2]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r2, #4]
	movs r0, #8
	ldr r3, [r6, #16]
	str r3, [r2, #8]
	bl WaitFrames
	adds r0, r6, #0
	bl Func_080200c8
	movs r0, #4
	bl WaitFrames
	movs r0, #30
	bl WaitFrames
	bl BattleFx_PrepareBufferInterpolation
.L_080e02dc:
	add sp, #36
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080e02e8:
	.4byte Func_080e0020
