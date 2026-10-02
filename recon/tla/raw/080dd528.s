.syntax unified
	.thumb
	.global BattleFx_StartItemBreak
	.thumb_func
BattleFx_StartItemBreak:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldrh r3, [r0, #6]
	mov r8, r0
	mov r2, r8
	movs r0, #128
	lsls r0, r0, #6
	ldr r1, [r2, #8]
	ldr r2, [r2, #12]
	adds r5, r3, r0
	movs r6, #128
	movs r3, #192
	lsls r3, r3, #8
	mov r4, r8
	movs r0, #139
	lsls r6, r6, #13
	ands r5, r3
	lsls r0, r0, #1
	adds r2, r2, r6
	ldr r3, [r4, #16]
	bl Object_Spawn
	mov r10, r0
	cmp r0, #0
	bne .L_080dd564
	movs r0, #0
	b .L_080dd628
.L_080dd564:
	movs r3, #128
	mov r0, r10
	lsls r3, r3, #7
	str r3, [r0, #28]
	str r3, [r0, #24]
	ldr r3, .L_080dd634
	mov r2, r10
	str r3, [r0, #108]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #48]
	str r3, [r0, #52]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #3
	bl Object_SetMode
	adds r2, r5, #0
	mov r0, r10
	adds r1, r6, #0
	bl Motion_SetTargetPositionFromMagnitudeAngle
	movs r2, #7
	mov r9, r2
.L_080dd596:
	mov r3, r8
	ldr r2, [r3, #12]
	movs r4, #128
	movs r0, #209
	lsls r4, r4, #13
	lsls r0, r0, #1
	ldr r1, [r3, #8]
	adds r2, r2, r4
	ldr r3, [r3, #16]
	adds r0, #255
	bl Object_Spawn
	adds r7, r0, #0
	cmp r7, #0
	beq .L_080dd614
	ldr r1, .L_080dd638
	bl ObjectDispatch_InitializeFar
	bl Random16
	movs r3, #128
	lsls r3, r3, #9
	adds r2, r7, #0
	adds r2, #85
	adds r0, r0, r3
	str r3, [r7, #52]
	movs r3, #2
	str r0, [r7, #48]
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #30
	str r3, [r7, #72]
	bl Random16
	adds r5, r0, #0
	bl Random16
	subs r5, r5, r0
	str r5, [r7, #40]
	bl Random16
	lsls r6, r0, #1
	adds r6, r6, r0
	movs r0, #128
	lsls r0, r0, #12
	lsls r6, r6, #3
	adds r6, r6, r0
	bl Random16
	adds r5, r0, #0
	bl Random16
	mov r2, r8
	ldrh r3, [r2, #6]
	subs r5, r5, r0
	lsrs r5, r5, #3
	adds r5, r5, r3
	adds r0, r7, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl Motion_SetTargetPositionFromMagnitudeAngle
.L_080dd614:
	movs r3, #1
	negs r3, r3
	add r9, r3
	mov r4, r9
	cmp r4, #0
	bge .L_080dd596
	movs r0, #138
	bl Audio_PlayCue
	mov r0, r10
.L_080dd628:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dd634:
	.4byte BattleFx_UpdateItemBreakFragment
.L_080dd638:
	.4byte Data_080f0e78
