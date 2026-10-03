.syntax unified
	.thumb
	.global Func_080df1fc
	.thumb_func
Func_080df1fc:
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
	sub sp, #128
	str r3, [sp, #32]
	ldr r0, [r3, #16]
	str r0, [sp, #28]
	ldr r7, [r3, #20]
	cmp r7, #0
	bne .L_080df222
	b .L_080df6de
.L_080df222:
	bl BattleEffect_InitializeSharedScene
	ldr r1, [sp, #28]
	str r7, [r1, #104]
	ldr r0, [sp, #28]
	ldr r1, .L_080df534
	bl ObjectDispatch_InitializeFar
	ldr r3, [sp, #32]
	movs r2, #36
	ldr r0, [r3, #4]
	add r2, sp
	str r0, [r2]
	mov r10, r2
	ldr r1, [r3, #8]
	movs r2, #128
	lsls r2, r2, #13
	adds r1, r1, r2
	mov r3, r10
	str r1, [r3, #4]
	ldr r3, [sp, #32]
	ldr r2, [r3, #12]
	mov r3, r10
	str r2, [r3, #8]
	movs r3, #128
	lsls r3, r3, #14
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #8
	bl Func_080df820
	mov r1, r10
	adds r6, r0, #0
	ldr r2, .L_080df538
	ldr r0, [r1]
	mov r3, r10
	adds r0, r0, r2
	ldr r1, [r1, #4]
	ldr r2, [r3, #8]
	movs r3, #0
	str r6, [sp, #20]
	bl Func_080df820
	adds r5, r0, #0
	ldr r0, [sp, #20]
	str r5, [sp, #24]
	cmp r0, #0
	beq .L_080df286
	cmp r5, #0
	bne .L_080df28c
.L_080df286:
	bl BattleFx_PrepareBufferInterpolation
	b .L_080df6de
.L_080df28c:
	movs r0, #15
	bl WaitFrames
	ldr r1, [r7, #8]
	mov r2, r10
	str r1, [r2]
	ldr r2, [r7, #12]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	mov r0, r10
	str r2, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #13
	adds r1, r1, r0
	adds r0, r6, #0
	bl Object_SetPosition
	mov r2, r10
	ldr r1, [r2]
	ldr r3, .L_080df53c
	mov r0, r10
	adds r1, r1, r3
	ldr r2, [r2, #4]
	ldr r3, [r0, #8]
	adds r0, r5, #0
	bl Object_SetPosition
	adds r0, r6, #0
	bl Object_CommitPosition
	adds r0, r5, #0
	bl Object_CommitPosition
	mov r1, r10
	ldr r3, [r1]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	movs r2, #0
	str r2, [r6, #36]
	str r3, [r6, #8]
	ldr r0, .L_080df53c
	ldr r3, [r1]
	movs r1, #144
	adds r3, r3, r0
	str r3, [r5, #8]
	ldr r3, .L_080df540
	str r2, [r5, #36]
	str r3, [r7, #108]
	lsls r1, r1, #3
	ldr r0, .L_080df544
	bl Scheduler_AddOrUpdateCallback
	movs r0, #130
	bl Audio_PlayCue
	adds r1, r7, #0
	adds r1, #85
	movs r3, #4
	str r1, [sp, #16]
	adds r0, r7, #0
	strb r3, [r1]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_080df35c
	ldr r3, [sp, #24]
	cmp r3, #0
	beq .L_080df35c
	ldr r3, [r7, #12]
	ldr r2, [r7, #20]
	movs r0, #192
	subs r3, r3, r2
	lsls r0, r0, #13
	cmp r3, r0
	bgt .L_080df35c
	movs r1, #192
	lsls r1, r1, #7
.L_080df332:
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	str r1, [sp, #0]
	ldr r3, [r5, #12]
	adds r3, r3, r1
	str r3, [r5, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r1
	str r3, [r7, #12]
	bl WaitFrames
	ldr r2, [r7, #20]
	ldr r3, [r7, #12]
	ldr r1, [sp, #0]
	subs r3, r3, r2
	movs r2, #192
	lsls r2, r2, #13
	cmp r3, r2
	ble .L_080df332
.L_080df35c:
	ldr r3, [sp, #20]
	ldr r0, [sp, #24]
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #8
	str r1, [r3, #48]
	str r2, [r3, #52]
	movs r3, #204
	str r2, [r0, #52]
	lsls r3, r3, #7
	movs r2, #204
	str r1, [r0, #48]
	adds r3, #102
	lsls r2, r2, #6
	adds r1, r7, #0
	str r3, [r7, #48]
	adds r1, #90
	movs r3, #0
	adds r2, #51
	str r2, [r7, #52]
	strb r3, [r1]
	adds r3, r7, #0
	adds r3, #34
	str r3, [sp, #12]
	ldr r1, [sp, #12]
	movs r3, #2
	strb r3, [r1]
	ldr r3, .L_080df548
	movs r1, #128
	lsls r1, r1, #13
	mov r11, r3
	mov r5, r10
	mov r9, r1
	str r0, [sp, #8]
	b .L_080df550
.L_080df3a4:
	mov r2, r11
	ldr r0, [r2]
	bl BattleFx_GetCycledTableWord
	movs r3, #255
	lsls r0, r0, #16
	lsls r3, r3, #8
	lsrs r6, r0, #16
	adds r3, #255
	cmp r6, r3
	bne .L_080df3f2
	ldr r1, [r7, #8]
	ldr r0, [sp, #20]
	str r1, [r5]
	ldr r2, [r7, #12]
	add r1, r9
	add r2, r9
	str r2, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Object_SetPosition
	ldr r1, [r5]
	ldr r0, .L_080df53c
	ldr r2, [r5, #4]
	adds r1, r1, r0
	ldr r3, [r5, #8]
	ldr r0, [sp, #24]
	bl Object_SetPosition
	ldr r0, [sp, #20]
	movs r1, #1
	bl Object_SetMode
	ldr r0, [sp, #24]
	movs r1, #1
	bl Object_SetMode
	b .L_080df550
.L_080df3f2:
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r0, r0, #10
	add r3, r9
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r6, #0
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r1, [r5]
	ldr r0, [sp, #20]
	add r1, r9
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetPosition
	ldr r1, [r5]
	ldr r2, .L_080df53c
	ldr r0, [sp, #24]
	adds r1, r1, r2
	ldr r3, [r5, #8]
	ldr r2, [r5, #4]
	bl Object_SetPosition
	ldr r0, [sp, #20]
	bl Object_CommitPosition
	ldr r0, [sp, #24]
	bl Object_CommitPosition
	ldr r3, [r7, #8]
	mov r0, r9
	str r3, [r5]
	ldr r3, [r7, #20]
	adds r1, r6, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_08020298
	mov r8, r0
	cmp r0, #0
	bne .L_080df478
	ldr r3, [r7, #20]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r7, #20]
	mov r1, r10
	adds r0, r7, #0
	bl Func_08020210
	ldr r3, [r7, #20]
	ldr r1, .L_080df53c
	adds r3, r3, r1
	str r3, [r7, #20]
	cmp r0, #0
	ble .L_080df49c
.L_080df478:
	ldr r0, [sp, #20]
	movs r1, #4
	bl Object_SetMode
	ldr r0, [sp, #24]
	movs r1, #4
	bl Object_SetMode
	ldr r3, .L_080df54c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_080df550
	movs r0, #114
	bl Audio_PlayCue
	b .L_080df550
.L_080df49c:
	movs r0, #175
	bl Audio_PlayCue
	ldr r0, [sp, #20]
	mov r2, r10
	movs r1, #4
	ldr r5, [r2]
	ldr r6, [r2, #8]
	bl Object_SetMode
	ldr r0, [sp, #24]
	movs r1, #4
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r1, #204
	adds r3, r7, #0
	lsls r1, r1, #6
	adds r1, #51
	adds r3, #91
	mov r0, r8
	strb r0, [r3]
	str r1, [r7, #48]
	str r1, [r7, #52]
	mov r2, r10
	mov r0, r10
	ldr r1, [r2]
	ldr r3, [r0, #8]
	ldr r2, [r2, #4]
	adds r0, r7, #0
	bl Object_SetPosition
	ldr r3, [sp, #20]
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	str r1, [r3, #48]
	str r1, [r3, #52]
	ldr r2, [sp, #8]
	mov r3, r10
	str r1, [r2, #48]
	str r1, [r2, #52]
	movs r0, #128
	ldr r1, [r3]
	lsls r0, r0, #13
	adds r1, r1, r0
	ldr r2, [r3, #4]
	ldr r0, [sp, #20]
	ldr r3, [r3, #8]
	bl Object_SetPosition
	mov r2, r10
	ldr r1, [r2]
	ldr r3, .L_080df53c
	mov r0, r10
	adds r1, r1, r3
	ldr r2, [r2, #4]
	ldr r3, [r0, #8]
	ldr r0, [sp, #8]
	bl Object_SetPosition
	adds r0, r7, #0
	bl Object_CommitPosition
	mov r1, r8
	str r5, [r7, #8]
	str r6, [r7, #16]
	str r1, [r7, #36]
	str r1, [r7, #44]
	movs r0, #10
	bl WaitFrames
	b .L_080df568
	.2byte 0x0000
.L_080df534:
	.4byte Data_080f0e60
.L_080df538:
	.4byte 0xffe00000
.L_080df53c:
	.4byte 0xfff00000
.L_080df540:
	.4byte ObjectGroup_ApplyRandomChildValues
.L_080df544:
	.4byte Func_080df174
.L_080df548:
	.4byte gInput
.L_080df54c:
	.4byte gFrameCount
.L_080df550:
	movs r0, #1
	bl WaitFrames
	mov r3, r11
	ldr r2, [r3, #4]
	movs r3, #129
	lsls r3, r3, #2
	adds r3, #255
	ands r2, r3
	cmp r2, #0
	bne .L_080df568
	b .L_080df3a4
.L_080df568:
	ldr r0, [sp, #20]
	movs r1, #4
	bl Object_SetMode
	ldr r0, [sp, #24]
	movs r1, #4
	bl Object_SetMode
	ldr r0, .L_080df6ec
	bl Scheduler_RemoveCallback
	movs r0, #135
	bl Audio_PlayCue
	movs r0, #15
	bl WaitFrames
	movs r0, #135
	bl Audio_PlayCue
	movs r0, #15
	bl WaitFrames
	ldr r3, [r7, #8]
	mov r0, r10
	str r3, [r0]
	ldr r3, [r7, #12]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	mov r2, sp
	adds r2, #48
	str r3, [r0, #8]
	movs r3, #128
	str r2, [sp, #4]
	lsls r3, r3, #10
	movs r0, #19
	mov r9, r3
	mov r8, r0
.L_080df5ba:
	mov r0, r10
	ldr r3, [r0, #8]
	movs r0, #209
	mov r2, r10
	lsls r0, r0, #1
	ldr r1, [r2]
	adds r0, #255
	ldr r2, [r2, #4]
	bl Object_Spawn
	ldr r2, [sp, #4]
	adds r6, r0, #0
	stmia r2!, {r6}
	adds r1, r2, #0
	str r1, [sp, #4]
	cmp r6, #0
	beq .L_080df614
	ldr r1, .L_080df6f0
	bl ObjectDispatch_InitializeFar
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
.L_080df614:
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	cmp r2, #0
	bge .L_080df5ba
	movs r0, #131
	bl Audio_PlayCue
	ldr r0, [sp, #20]
	bl Object_Destroy
	ldr r0, [sp, #24]
	bl Object_Destroy
	ldr r3, [sp, #32]
	adds r0, r7, #0
	adds r3, #64
	ldrb r1, [r3]
	bl Animation_ApplyChildValuesFar
	ldr r3, [sp, #32]
	adds r0, r7, #0
	ldr r1, [r3, #60]
	bl ObjectDispatch_InitializeFar
	ldr r0, [sp, #32]
	movs r2, #0
	ldr r3, [r0, #56]
	str r3, [r7, #108]
	ldr r1, [sp, #16]
	movs r3, #3
	strb r3, [r1]
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r7, #40]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #68]
	ldr r3, [sp, #12]
	movs r1, #0
	strb r2, [r3]
	ldr r0, [sp, #28]
	str r2, [r0, #108]
	ldr r0, [sp, #28]
	bl Animation_ApplyChildValuesFar
	ldr r3, [sp, #32]
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080df6da
	ldr r3, [r7, #40]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	blt .L_080df6a2
.L_080df68c:
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #89
	bgt .L_080df6a2
	ldr r3, [r7, #40]
	cmp r3, #0
	bge .L_080df68c
.L_080df6a2:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #40]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	bge .L_080df6c8
.L_080df6b2:
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #89
	bgt .L_080df6c8
	ldr r3, [r7, #40]
	cmp r3, #0
	blt .L_080df6b2
.L_080df6c8:
	adds r0, r7, #0
	bl Func_080dfb0c
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #30
	bl WaitFrames
	b .L_080df6de
.L_080df6da:
	bl BattleFx_PrepareBufferInterpolation
.L_080df6de:
	add sp, #128
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080df6ec:
	.4byte Func_080df174
.L_080df6f0:
	.4byte Data_080f0e78
