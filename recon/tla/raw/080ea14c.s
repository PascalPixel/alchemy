.syntax unified
	.thumb
	.global Func_080ea14c
	.thumb_func
Func_080ea14c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #235
	lsls r1, r1, #5
	adds r5, r0, #0
	movs r0, #92
	sub sp, #64
	bl Runtime_AllocateHeapBlock
	mov r9, r0
	bl EventRuntime_GetControlledOwner
	str r0, [sp, #48]
	bl ObjectTable_Get
	str r0, [sp, #44]
	adds r0, r5, #0
	bl ObjectTable_Get
	movs r1, #0
	str r0, [sp, #40]
	str r1, [sp, #8]
	str r1, [sp, #4]
	ldr r3, .L_080ea378
	movs r2, #7
	ldr r3, [r3]
	ldr r1, .L_080ea37c
	ands r3, r2
	lsls r3, r3, #2
	ldr r1, [r1, r3]
	movs r0, #8
	bl Func_080dc1b0
	ldr r2, [sp, #44]
	add r5, sp, #52
	ldr r3, [r2, #8]
	movs r1, #192
	str r3, [r5]
	lsls r1, r1, #13
	ldr r3, [r2, #12]
	adds r0, r5, #0
	adds r3, r3, r1
	str r3, [r5, #4]
	movs r7, #132
	ldr r3, [r2, #16]
	lsls r7, r7, #5
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r2, [r5]
	adds r0, r5, #0
	str r2, [sp, #24]
	movs r6, #240
	ldr r3, [r5, #8]
	add r7, r9
	str r3, [sp, #20]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #88
	add r3, r9
	str r2, [r3]
	movs r3, #232
	ldr r1, [sp, #20]
	lsls r3, r3, #5
	adds r3, #92
	add r3, r9
	str r1, [r3]
	ldr r2, [sp, #40]
	lsls r6, r6, #13
	ldr r3, [r2, #8]
	str r3, [r5]
	ldr r3, [r2, #12]
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r3, [r5]
	ldr r0, .L_080ea380
	str r3, [sp, #16]
	ldr r5, [r5, #8]
	str r5, [sp, #12]
	bl Resource_GetTableEntry
	mov r1, r9
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	mov r2, r9
	str r0, [sp, #32]
	bl VramBlock_LoadCached
	movs r5, #174
	movs r1, #0
	lsls r5, r5, #5
	mov r11, r0
	mov r8, r1
	add r5, r9
.L_080ea220:
	bl Random16
	lsls r0, r0, #4
	lsrs r0, r0, #16
	lsls r0, r0, #1
	add r0, r11
	str r0, [sp, #0]
	movs r1, #4
	movs r2, #4
	movs r3, #0
	adds r0, r5, #0
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	mov r10, r2
	mov r1, r10
	ands r3, r1
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	ldr r2, [sp, #16]
	mov r1, r8
	str r2, [r7]
	ldr r3, [sp, #12]
	str r3, [r7, #4]
	cmp r1, #0
	bge .L_080ea26c
	adds r1, #15
.L_080ea26c:
	asrs r1, r1, #4
	mov r3, r8
	lsls r2, r1, #4
	subs r2, r3, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #18
	adds r3, r3, r6
	str r3, [r7, #12]
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #19
	adds r3, r3, r6
	mov r1, r8
	movs r2, #1
	str r3, [r7, #16]
	add r8, r2
	negs r3, r1
	str r3, [r7, #24]
	mov r3, r8
	adds r5, #40
	adds r7, #28
	cmp r3, #47
	ble .L_080ea220
	ldr r0, .L_080ea384
	bl Resource_GetTableEntry
	mov r1, r9
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #4
	adds r1, r5, #0
	mov r2, r9
	str r0, [sp, #28]
	bl VramBlock_LoadCached
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #86
	adds r6, r0, #0
	add r3, r9
	mov r1, r9
	adds r7, r1, r5
	strh r6, [r3]
	movs r5, #184
	lsls r5, r5, #4
	movs r2, #31
	add r5, r9
	mov r8, r2
.L_080ea2d4:
	movs r3, #128
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	str r6, [sp, #0]
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	mov r1, r10
	ands r3, r1
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_080ea2d4
	movs r0, #1
	bl WaitFrames
	movs r0, #139
	bl Audio_PlayCue
	movs r1, #2
	ldr r0, [sp, #40]
	bl Object_SetMode
	movs r0, #60
	bl EventRuntime_Wait
	ldr r2, [sp, #40]
	movs r3, #0
	adds r2, #100
	strh r3, [r2]
	ldr r1, [sp, #40]
	ldr r3, .L_080ea388
	movs r0, #142
	str r3, [r1, #108]
	bl Audio_PlayCue
	movs r0, #80
	bl EventRuntime_Wait
	ldr r1, .L_080ea38c
	ldr r0, [sp, #40]
	bl ObjectDispatch_InitializeFar
	movs r3, #232
	ldr r5, .L_080ea374
	lsls r3, r3, #5
	adds r3, #85
	add r3, r9
	movs r1, #144
	strb r5, [r3]
	ldr r0, .L_080ea390
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	str r2, [sp, #36]
	mov r10, r2
.L_080ea364:
	ldr r3, [sp, #36]
	cmp r3, #7
	bls .L_080ea36c
	b .L_080ea6c4
.L_080ea36c:
	ldr r2, .L_080ea394
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	b .L_080ea398
.L_080ea374:
	.4byte 0x00000000
.L_080ea378:
	.4byte gFrameCount
.L_080ea37c:
	.4byte Data_080f3954
.L_080ea380:
	.4byte 0x000001f4
.L_080ea384:
	.4byte 0x000001f5
.L_080ea388:
	.4byte Func_080e9e8c
.L_080ea38c:
	.4byte Data_080f1040
.L_080ea390:
	.4byte Func_080e9f68
.L_080ea394:
	.4byte .L_080ea39c
.L_080ea398:
	mov pc, r3
	.2byte 0x0000
.L_080ea39c:
	.4byte .L_080ea3bc
	.4byte .L_080ea412
	.4byte .L_080ea4fa
	.4byte .L_080ea53c
	.4byte .L_080ea554
	.4byte .L_080ea60c
	.4byte .L_080ea65a
	.4byte .L_080ea68e
.L_080ea3bc:
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #84
	add r2, r9
	movs r3, #0
	mov r1, r10
	strb r3, [r2]
	cmp r1, #39
	bgt .L_080ea3e0
	movs r3, #7
	ands r3, r1
	cmp r3, #0
	bne .L_080ea3dc
	movs r0, #133
	bl Audio_PlayCue
.L_080ea3dc:
	movs r2, #1
	str r2, [sp, #8]
.L_080ea3e0:
	mov r3, r10
	cmp r3, #40
	bne .L_080ea3fc
	movs r0, #134
	bl Audio_PlayCue
	movs r1, #25
	ldr r0, [sp, #40]
	str r1, [sp, #8]
	bl Object_ResetMotion
	ldr r2, [sp, #40]
	movs r3, #0
	str r3, [r2, #24]
.L_080ea3fc:
	mov r3, r10
	cmp r3, #80
	beq .L_080ea404
	b .L_080ea6c4
.L_080ea404:
	ldr r1, [sp, #36]
	movs r2, #1
	adds r1, #1
	negs r2, r2
	str r1, [sp, #36]
	mov r10, r2
	b .L_080ea6c4
.L_080ea412:
	movs r1, #255
	lsls r1, r1, #8
	ldr r6, .L_080ea430
	ldr r5, .L_080ea434
	movs r3, #0
	adds r1, #224
	mov r8, r3
	mov r12, r1
	mov r7, r9
.L_080ea424:
	mov r2, r8
	cmp r2, #0
	bge .L_080ea438
	adds r2, #15
	b .L_080ea438
	.2byte 0x0000
.L_080ea430:
	.4byte 0x000003ff
.L_080ea434:
	.4byte 0xfffffc00
.L_080ea438:
	asrs r2, r2, #4
	lsls r3, r2, #4
	mov r1, r8
	subs r3, r1, r3
	adds r4, r3, r2
	mov r2, r10
	movs r3, #174
	lsls r3, r3, #5
	subs r4, r2, r4
	adds r0, r7, r3
	cmp r4, #0
	bne .L_080ea462
	ldrh r1, [r0, #8]
	adds r3, r5, #0
	lsls r2, r1, #22
	lsrs r2, r2, #22
	adds r2, #32
	ands r2, r6
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080ea462:
	cmp r4, #3
	bne .L_080ea478
	ldrh r1, [r0, #8]
	adds r3, r5, #0
	lsls r2, r1, #22
	lsrs r2, r2, #22
	add r2, r12
	ands r2, r6
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080ea478:
	cmp r4, #8
	bne .L_080ea48e
	ldrh r1, [r0, #8]
	adds r3, r5, #0
	lsls r2, r1, #22
	lsrs r2, r2, #22
	adds r2, #32
	ands r2, r6
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080ea48e:
	cmp r4, #10
	bne .L_080ea4a4
	ldrh r1, [r0, #8]
	adds r3, r5, #0
	lsls r2, r1, #22
	lsrs r2, r2, #22
	add r2, r12
	ands r2, r6
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080ea4a4:
	cmp r4, #16
	bne .L_080ea4ba
	ldrh r1, [r0, #8]
	adds r3, r5, #0
	lsls r2, r1, #22
	lsrs r2, r2, #22
	adds r2, #32
	ands r2, r6
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080ea4ba:
	cmp r4, #18
	bne .L_080ea4d0
	ldrh r1, [r0, #8]
	adds r3, r5, #0
	lsls r2, r1, #22
	lsrs r2, r2, #22
	add r2, r12
	ands r2, r6
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080ea4d0:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r7, #40
	cmp r2, #47
	ble .L_080ea424
	mov r3, r10
	cmp r3, #0
	bne .L_080ea4e8
	movs r0, #246
	bl Audio_PlayCue
.L_080ea4e8:
	mov r1, r10
	cmp r1, #56
	beq .L_080ea4f0
	b .L_080ea6c4
.L_080ea4f0:
	ldr r2, [sp, #36]
	movs r3, #1
	adds r2, #1
	negs r3, r3
	b .L_080ea688
.L_080ea4fa:
	mov r1, r10
	cmp r1, #0
	bne .L_080ea50e
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [sp, #48]
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_080ea6c4
.L_080ea50e:
	ldr r1, [sp, #44]
	movs r2, #4
	ldrsh r3, [r1, r2]
	ldr r2, [r1]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	cmp r3, #17
	bne .L_080ea52a
	ldr r2, [sp, #36]
	movs r3, #1
	adds r2, #1
	str r2, [sp, #36]
	negs r3, r3
	mov r10, r3
.L_080ea52a:
	mov r1, r10
	cmp r1, #60
	beq .L_080ea532
	b .L_080ea6c4
.L_080ea532:
	ldr r2, [sp, #36]
	movs r3, #1
	adds r2, #1
	negs r3, r3
	b .L_080ea688
.L_080ea53c:
	mov r1, r10
	cmp r1, #0
	bne .L_080ea54a
	ldr r0, [sp, #44]
	movs r1, #28
	bl Object_SetMode
.L_080ea54a:
	mov r2, r10
	cmp r2, #10
	beq .L_080ea552
	b .L_080ea6c4
.L_080ea552:
	b .L_080ea5fe
.L_080ea554:
	mov r2, r10
	cmp r2, #0
	bne .L_080ea5e0
	movs r7, #132
	lsls r7, r7, #5
	movs r1, #47
	add r7, r9
	movs r3, #0
	mov r8, r1
.L_080ea566:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r1, r8
	str r3, [r7, #24]
	subs r3, #3
	adds r7, #28
	cmp r1, #0
	bge .L_080ea566
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #76
	movs r3, #240
	add r2, r9
	lsls r3, r3, #14
	str r3, [r2]
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #72
	movs r3, #192
	add r2, r9
	lsls r3, r3, #8
	str r3, [r2]
	movs r3, #234
	lsls r3, r3, #5
	movs r2, #130
	add r3, r9
	lsls r2, r2, #16
	str r2, [r3]
	movs r3, #232
	lsls r3, r3, #5
	movs r2, #232
	adds r3, #68
	movs r1, #240
	lsls r2, r2, #5
	add r3, r9
	lsls r1, r1, #13
	adds r2, #85
	str r1, [r3]
	add r2, r9
	movs r3, #1
	strb r3, [r2]
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #80
	movs r3, #128
	add r2, r9
	lsls r3, r3, #11
	str r3, [r2]
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #84
	add r2, r9
	movs r3, #2
	strb r3, [r2]
	mov r2, r10
	cmp r2, #0
	bne .L_080ea5e0
	movs r0, #187
	bl Audio_PlayCue
.L_080ea5e0:
	mov r3, r10
	cmp r3, #48
	bne .L_080ea5ec
	movs r0, #187
	bl Audio_PlayCue
.L_080ea5ec:
	mov r1, r10
	cmp r1, #96
	bne .L_080ea5f8
	movs r0, #187
	bl Audio_PlayCue
.L_080ea5f8:
	mov r2, r10
	cmp r2, #100
	bne .L_080ea6c4
.L_080ea5fe:
	ldr r3, [sp, #36]
	movs r1, #1
	adds r3, #1
	negs r1, r1
	str r3, [sp, #36]
	mov r10, r1
	b .L_080ea6c4
.L_080ea60c:
	ldr r2, [sp, #24]
	ldr r1, .L_080ea7ac
	movs r5, #234
	adds r3, r2, r1
	mov r0, r10
	muls r0, r3
	movs r1, #30
	bl __divsi3
	movs r2, #130
	lsls r2, r2, #16
	lsls r5, r5, #5
	adds r0, r0, r2
	add r5, r9
	str r0, [r5]
	ldr r1, [sp, #20]
	ldr r2, .L_080ea7b0
	movs r5, #232
	adds r3, r1, r2
	mov r0, r10
	muls r0, r3
	movs r1, #30
	bl __divsi3
	lsls r5, r5, #5
	movs r3, #240
	adds r5, #68
	lsls r3, r3, #13
	add r5, r9
	adds r0, r0, r3
	mov r1, r10
	str r0, [r5]
	cmp r1, #30
	bne .L_080ea6c4
	ldr r2, [sp, #36]
	movs r3, #1
	adds r2, #1
	negs r3, r3
	b .L_080ea688
.L_080ea65a:
	mov r1, r10
	cmp r1, #0
	bne .L_080ea66a
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #80
	add r3, r9
	str r1, [r3]
.L_080ea66a:
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #76
	add r2, r9
	ldr r3, [r2]
	ldr r1, .L_080ea7b4
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_080ea6c4
	movs r3, #0
	str r3, [r2]
	ldr r2, [sp, #36]
	subs r3, #1
	adds r2, #1
.L_080ea688:
	str r2, [sp, #36]
	mov r10, r3
	b .L_080ea6c4
.L_080ea68e:
	mov r1, r10
	cmp r1, #0
	bne .L_080ea69c
	ldr r1, .L_080ea7b8
	ldr r0, [sp, #44]
	bl ObjectDispatch_InitializeFar
.L_080ea69c:
	movs r5, #3
	mov r3, r10
	ands r3, r5
	cmp r3, #0
	bne .L_080ea6ac
	movs r0, #167
	bl Audio_PlayCue
.L_080ea6ac:
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #84
	add r3, r9
	mov r2, r10
	strb r5, [r3]
	cmp r2, #60
	bne .L_080ea6c4
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	str r3, [sp, #36]
.L_080ea6c4:
	ldr r1, [sp, #8]
	cmp r1, #0
	beq .L_080ea750
	movs r2, #0
	mov r11, r2
	mov r8, r1
.L_080ea6d0:
	bl Random16
	ldr r3, [sp, #4]
	movs r1, #128
	lsls r5, r3, #3
	subs r5, r5, r3
	lsls r5, r5, #2
	add r5, r9
	lsls r1, r1, #4
	adds r7, r5, r1
	mov r2, r11
	str r2, [r7, #24]
	ldr r1, [sp, #40]
	movs r2, #192
	ldr r3, [r1, #8]
	lsls r2, r2, #12
	str r3, [r7]
	adds r6, r0, #0
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r1, #16]
	str r3, [r7, #8]
	bl Random16
	movs r3, #128
	lsls r3, r3, #10
	lsls r0, r0, #2
	adds r0, r0, r3
	adds r2, r7, #0
	adds r1, r6, #0
	bl Vector_AddPolarOffset
	movs r3, #144
	mov r1, r11
	lsls r3, r3, #11
	str r1, [r7, #12]
	str r3, [r7, #16]
	str r1, [r7, #20]
	bl Random16
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #12
	movs r2, #128
	adds r5, r5, r3
	lsls r2, r2, #10
	lsls r0, r0, #1
	adds r0, r0, r2
	adds r1, r6, #0
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r1, [sp, #4]
	movs r2, #1
	negs r2, r2
	movs r3, #31
	adds r1, #1
	add r8, r2
	ands r1, r3
	mov r3, r8
	str r1, [sp, #4]
	cmp r3, #0
	bne .L_080ea6d0
.L_080ea750:
	movs r1, #0
	movs r0, #1
	str r1, [sp, #8]
	bl WaitFrames
	movs r1, #186
	ldr r3, [sp, #36]
	lsls r1, r1, #2
	movs r2, #1
	adds r1, #255
	add r10, r2
	cmp r3, r1
	beq .L_080ea76c
	b .L_080ea364
.L_080ea76c:
	ldr r0, [sp, #44]
	bl Object_ResetMotion
	ldr r2, [sp, #44]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #10
	bl EventRuntime_Wait
	ldr r0, .L_080ea7bc
	bl Scheduler_RemoveCallback
	bl BattleFx_PrepareBufferInterpolation
	ldr r0, [sp, #32]
	bl Resource_ResetEntry
	ldr r0, [sp, #28]
	bl Resource_ResetEntry
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ea7ac:
	.4byte 0xff7e0000
.L_080ea7b0:
	.4byte 0xffe20000
.L_080ea7b4:
	.4byte 0xfffe0000
.L_080ea7b8:
	.4byte Data_080f3614
.L_080ea7bc:
	.4byte Func_080e9f68
