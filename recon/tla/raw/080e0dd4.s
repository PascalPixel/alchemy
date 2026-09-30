.syntax unified
	.thumb
	.global Func_080e0dd4
	.thumb_func
Func_080e0dd4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r6, [r3]
	sub sp, #12
	ldr r7, [r6, #16]
	bl Func_080d22a8
	movs r2, #1
	negs r2, r2
	movs r3, #0
	adds r0, r2, #0
	adds r1, r2, #0
	bl Motion_CamBounds
	bl BattleEffect_InitializeSharedScene
	movs r0, #10
	bl WaitFrames
	movs r1, #128
	movs r2, #0
	movs r3, #24
	ldrsh r0, [r6, r3]
	lsls r1, r1, #7
	bl Func_080d3838
	movs r0, #30
	bl WaitFrames
	ldr r3, .L_080e0e94
	movs r0, #131
	str r3, [r7, #108]
	bl Audio_PlayCue
	movs r1, #28
	adds r0, r7, #0
	bl Object_SetMode
	movs r0, #40
	bl WaitFrames
	movs r0, #220
	bl Audio_PlayCue
	adds r0, r7, #0
	movs r1, #0
	bl Animation_ApplyChildValuesFar
	movs r1, #3
	adds r0, r7, #0
	bl Object_SetMode
	ldr r3, .L_080e0e98
	adds r2, r7, #0
	adds r2, #100
	str r3, [r7, #108]
	movs r3, #0
	strh r3, [r2]
	movs r0, #70
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r5, .L_080e0e90
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	ldr r3, .L_080e0e9c
	mov r5, sp
	str r3, [r7, #108]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r6, #80
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	mov r10, r6
	str r3, [r5, #8]
	bl Func_080dc390
	movs r3, #0
	mov r8, r3
	b .L_080e0ea0
.L_080e0e90:
	.4byte 0x00000000
.L_080e0e94:
	.4byte Func_080db91c
.L_080e0e98:
	.4byte Func_080e0c84
.L_080e0e9c:
	.4byte Func_080e0cac
.L_080e0ea0:
	movs r1, #168
	ldr r2, [r5]
	ldr r3, [r5, #8]
	adds r0, r6, #0
	lsls r1, r1, #2
	bl Func_080ebec8
	adds r0, r6, #0
	ldr r1, .L_080e0f38
	bl Func_080ebeb4
	adds r0, r6, #0
	movs r1, #7
	bl Func_080ebea8
	bl Random16
	lsls r1, r0, #3
	subs r1, r1, r0
	lsrs r1, r1, #16
	ldr r0, [r6]
	bl Animation_ApplyChildValuesToRecordFar
	bl Random16
	ldr r3, .L_080e0f3c
	lsrs r0, r0, #1
	adds r0, r0, r3
	str r0, [r6, #44]
	str r0, [r6, #40]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r6, #72
	cmp r3, #23
	bls .L_080e0ea0
	movs r0, #70
	bl WaitFrames
	movs r3, #0
	mov r2, r10
	mov r8, r3
	movs r1, #2
	adds r2, #64
.L_080e0efe:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080e0f08
	strb r1, [r2]
.L_080e0f08:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r2, #72
	cmp r3, #23
	bls .L_080e0efe
	movs r0, #40
	bl WaitFrames
	bl BattleFx_PrepareBufferInterpolation
	movs r3, #0
	str r3, [r7, #24]
	ldr r3, [r7, #20]
	movs r0, #10
	str r3, [r7, #12]
	bl WaitFrames
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e0f38:
	.4byte Func_080e0cec
.L_080e0f3c:
	.4byte 0x00013333
