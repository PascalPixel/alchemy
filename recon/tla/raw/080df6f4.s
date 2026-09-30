.syntax unified
	.thumb
	.global Func_080df6f4
	.thumb_func
Func_080df6f4:
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
	ldr r3, [r3]
	sub sp, #20
	mov r10, r3
	bl BattleEffect_InitializeSharedScene
	mov r3, r10
	ldr r0, [r3, #4]
	add r5, sp, #8
	str r0, [r5]
	ldr r1, [r3, #8]
	movs r3, #128
	lsls r3, r3, #13
	adds r1, r1, r3
	str r1, [r5, #4]
	mov r3, r10
	ldr r2, [r3, #12]
	movs r3, #128
	lsls r3, r3, #14
	adds r0, r0, r3
	movs r3, #128
	str r2, [r5, #8]
	lsls r3, r3, #8
	bl Func_080df820
	ldr r3, .L_080df818
	str r0, [sp, #0]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	ldr r2, [r5, #8]
	movs r3, #0
	bl Func_080df820
	str r0, [sp, #4]
	movs r0, #15
	mov r11, sp
	bl WaitFrames
	movs r0, #1
	mov r7, r11
	mov r8, r0
.L_080df75a:
	ldmia r7!, {r6}
	cmp r6, #0
	beq .L_080df76c
	movs r1, #192
	ldrh r2, [r6, #6]
	adds r0, r6, #0
	lsls r1, r1, #13
	bl Motion_SetTargetPositionFromMagnitudeAngle
.L_080df76c:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r0, r8
	cmp r0, #0
	bge .L_080df75a
	ldr r0, [sp, #0]
	bl Object_CommitPosition
	movs r0, #134
	bl Audio_PlayCue
	movs r0, #128
	movs r3, #23
	lsls r0, r0, #10
	adds r7, r5, #0
	mov r8, r3
	mov r9, r0
.L_080df790:
	mov r3, r10
	ldr r1, [r3, #4]
	movs r0, #128
	str r1, [r7]
	lsls r0, r0, #13
	ldr r2, [r3, #8]
	adds r2, r2, r0
	str r2, [r7, #4]
	movs r0, #209
	ldr r3, [r3, #12]
	lsls r0, r0, #1
	str r3, [r7, #8]
	adds r0, #255
	bl Object_Spawn
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080df7ec
	ldr r1, .L_080df81c
	bl Object_SetCallback
	bl Random16
	mov r3, r9
	adds r2, r6, #0
	adds r2, #85
	str r3, [r6, #52]
	add r0, r9
	movs r3, #0
	str r0, [r6, #48]
	strb r3, [r2]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #12
	lsls r5, r5, #3
	adds r5, r5, r0
	bl Random16
	adds r1, r5, #0
	adds r2, r0, #0
	adds r0, r6, #0
	bl Motion_SetTargetPositionFromMagnitudeAngle
.L_080df7ec:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r0, r8
	cmp r0, #0
	bge .L_080df790
	ldr r0, [sp, #0]
	bl Func_080200c8
	mov r3, r11
	ldr r0, [r3, #4]
	bl Func_080200c8
	bl BattleFx_PrepareBufferInterpolation
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080df818:
	.4byte 0xffe00000
.L_080df81c:
	.4byte Data_080f0e78
