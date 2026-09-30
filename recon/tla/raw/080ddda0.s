.syntax unified
	.thumb
	.global Func_080ddda0
	.thumb_func
Func_080ddda0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r6, [r3]
	sub sp, #20
	ldr r7, [r6, #20]
	ldr r5, [r6, #16]
	cmp r7, #0
	beq .L_080dde92
	bl BattleEffect_InitializeSharedScene
	adds r0, r5, #0
	str r7, [r5, #104]
	ldr r1, .L_080dde9c
	bl Object_SetCallback
	ldr r0, [r6, #4]
	add r5, sp, #8
	str r0, [r5]
	movs r2, #128
	ldr r1, [r6, #8]
	lsls r2, r2, #13
	adds r1, r1, r2
	str r1, [r5, #4]
	movs r3, #128
	ldr r2, [r6, #12]
	lsls r3, r3, #14
	adds r0, r0, r3
	movs r3, #128
	str r2, [r5, #8]
	lsls r3, r3, #8
	bl Func_080ddfd4
	ldr r2, .L_080ddea0
	str r0, [sp, #0]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r2
	movs r3, #0
	ldr r2, [r5, #8]
	bl Func_080ddfd4
	str r0, [sp, #4]
	movs r0, #15
	mov r8, sp
	bl WaitFrames
	mov r6, r8
	movs r5, #1
.L_080dde08:
	ldmia r6!, {r0}
	cmp r0, #0
	beq .L_080dde18
	movs r1, #224
	ldrh r2, [r0, #6]
	lsls r1, r1, #12
	bl Motion_SetTargetPositionFromMagnitudeAngle
.L_080dde18:
	subs r5, #1
	cmp r5, #0
	bge .L_080dde08
	ldr r0, [sp, #0]
	bl Object_CommitPosition
	ldr r3, .L_080ddea4
	movs r0, #130
	str r3, [r7, #108]
	bl Audio_PlayCue
	adds r2, r7, #0
	ldr r0, [sp, #0]
	adds r2, #85
	movs r3, #4
	strb r3, [r2]
	ldr r5, [r7, #12]
	cmp r0, #0
	beq .L_080dde82
	mov r2, r8
	ldr r3, [r2, #4]
	cmp r3, #0
	beq .L_080dde82
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r5, r2
	cmp r5, r3
	bgt .L_080dde82
	b .L_080dde54
.L_080dde52:
	ldr r0, [sp, #0]
.L_080dde54:
	ldr r3, [r0, #12]
	movs r1, #128
	lsls r1, r1, #7
	adds r3, r3, r1
	str r3, [r0, #12]
	mov r3, r8
	ldr r2, [r3, #4]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r1
	str r3, [r7, #12]
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r5, r3
	ldr r3, [r7, #12]
	cmp r3, r2
	ble .L_080dde52
	ldr r0, [sp, #0]
.L_080dde82:
	bl UpdateRisingParticleBurst
	mov r2, r8
	ldr r0, [r2, #4]
	bl UpdateRisingParticleBurst
	bl BattleFx_PrepareBufferInterpolation
.L_080dde92:
	add sp, #20
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dde9c:
	.4byte Data_080f0e60
.L_080ddea0:
	.4byte 0xffe00000
.L_080ddea4:
	.4byte ObjectGroup_ApplyRandomChildValues
