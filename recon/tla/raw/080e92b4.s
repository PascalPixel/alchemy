.syntax unified
	.thumb
	.global Func_080e92b4
	.thumb_func
Func_080e92b4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #201
	lsls r1, r1, #5
	movs r0, #92
	sub sp, #88
	bl Runtime_AllocateHeapBlock
	movs r3, #192
	str r0, [sp, #84]
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #0
	str r3, [sp, #80]
	ldr r0, [r3, #20]
	ldr r5, [r3, #16]
	str r0, [sp, #76]
	movs r3, #224
	lsls r3, r3, #12
	ldr r1, [r0, #80]
	str r3, [sp, #32]
	str r2, [sp, #56]
	str r2, [sp, #52]
	str r2, [sp, #48]
	str r2, [sp, #44]
	str r2, [sp, #36]
	ldr r2, [sp, #80]
	mov r11, r1
	movs r1, #30
	ldrsh r0, [r2, r1]
	bl Func_080ce31c
	str r0, [sp, #28]
	ldr r2, [sp, #80]
	movs r0, #160
	lsls r0, r0, #23
	movs r3, #30
	ldrsh r1, [r2, r3]
	adds r0, #5
	ldr r2, [sp, #28]
	bl Func_080ce458
	ldr r3, [sp, #76]
	str r0, [sp, #24]
	cmp r3, #0
	beq .L_080e9334
	mov r0, r11
	cmp r0, #0
	beq .L_080e9356
	ldrb r3, [r0, #20]
	ldrb r2, [r0, #21]
	movs r1, #128
	muls r3, r2
	lsls r1, r1, #4
	cmp r3, r1
	ble .L_080e9332
	b .L_080e97a6
.L_080e9332:
	b .L_080e9356
.L_080e9334:
	ldr r2, [sp, #80]
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r2, #4]
	lsls r0, r0, #12
	ldr r3, [r5, #16]
	ldrh r1, [r2]
	adds r3, r3, r0
	str r3, [r2, #12]
	movs r0, #128
	ldr r3, [r5, #12]
	lsls r0, r0, #14
	str r3, [r2, #8]
	ldr r2, [sp, #80]
	adds r2, #4
	bl Vector_AddPolarOffset
.L_080e9356:
	movs r1, #0
	ldr r0, [sp, #76]
	str r1, [sp, #40]
	bl Func_08020330
	movs r2, #212
	lsls r2, r2, #1
	cmp r0, r2
	bne .L_080e936e
	movs r3, #1
	str r3, [sp, #40]
	str r3, [sp, #36]
.L_080e936e:
	ldr r0, [sp, #76]
	cmp r0, #0
	bne .L_080e9378
	movs r1, #2
	str r1, [sp, #36]
.L_080e9378:
	ldr r2, [sp, #36]
	cmp r2, #1
	bne .L_080e9384
	movs r3, #224
	lsls r3, r3, #12
	str r3, [sp, #32]
.L_080e9384:
	ldr r0, [sp, #36]
	cmp r0, #2
	bne .L_080e9390
	movs r1, #168
	lsls r1, r1, #13
	str r1, [sp, #32]
.L_080e9390:
	bl BattleEffect_InitializeSharedScene
	ldr r0, .L_080e9538
	bl Resource_GetTableEntry
	ldr r2, [sp, #84]
	adds r2, #32
	adds r1, r2, #0
	str r2, [sp, #20]
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	ldr r2, [sp, #20]
	str r0, [sp, #72]
	bl VramBlock_LoadCached
	ldr r3, [sp, #84]
	str r0, [sp, #68]
	movs r1, #242
	movs r0, #130
	lsls r0, r0, #4
	lsls r1, r1, #4
	movs r2, #13
	adds r6, r3, r0
	adds r5, r3, r1
	negs r2, r2
	movs r3, #63
	adds r7, r2, #0
	mov r9, r3
.L_080e93d0:
	ldr r0, [sp, #68]
	movs r3, #128
	str r0, [sp, #0]
	movs r1, #8
	adds r0, r5, #0
	movs r2, #8
	lsls r3, r3, #23
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	ands r3, r7
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r9, r3
	mov r1, r9
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r1, #0
	bge .L_080e93d0
	ldr r2, [sp, #76]
	cmp r2, #0
	beq .L_080e9432
	mov r3, r11
	cmp r3, #0
	beq .L_080e9432
	ldr r0, [sp, #40]
	cmp r0, #1
	bne .L_080e9432
	mov r0, r11
	movs r1, #0
	bl Animation_ApplyChildValueFar
	mov r1, r11
	ldrb r2, [r1, #20]
	ldrb r3, [r1, #21]
	ldr r0, [r1, #40]
	muls r2, r3
	ldr r1, [sp, #20]
	bl Func_080e43a4
.L_080e9432:
	movs r0, #2
	bl Func_080e89e4
	movs r0, #1
	bl WaitFrames
	movs r0, #238
	bl Audio_PlayCue
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #10
	str r2, [sp, #16]
	str r3, [sp, #12]
	str r2, [sp, #64]
	str r2, [sp, #60]
.L_080e9452:
	ldr r0, [sp, #48]
	cmp r0, #0
	beq .L_080e9480
	ldr r1, [sp, #36]
	cmp r1, #1
	bne .L_080e9470
	ldr r2, [sp, #32]
	movs r3, #160
	lsls r3, r3, #11
	cmp r2, r3
	ble .L_080e9480
	ldr r0, .L_080e953c
	adds r2, r2, r0
	str r2, [sp, #32]
	b .L_080e9480
.L_080e9470:
	ldr r1, [sp, #32]
	movs r2, #160
	lsls r2, r2, #11
	cmp r1, r2
	ble .L_080e9480
	ldr r3, .L_080e9540
	adds r1, r1, r3
	str r1, [sp, #32]
.L_080e9480:
	ldr r0, [sp, #48]
	cmp r0, #0
	beq .L_080e957e
	mov r9, r0
.L_080e9488:
	bl Random16
	ldr r1, [sp, #36]
	adds r7, r0, #0
	cmp r1, #2
	bne .L_080e94a4
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, .L_080e9544
	lsls r3, r3, #1
	lsrs r3, r3, #4
	adds r7, r3, r2
.L_080e94a4:
	ldr r0, [sp, #44]
	ldr r1, [sp, #84]
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	movs r2, #242
	adds r3, r1, r3
	lsls r2, r2, #4
	adds r5, r3, r2
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r3, r1, r3
	ldr r1, [sp, #76]
	movs r0, #130
	lsls r0, r0, #4
	adds r6, r3, r0
	cmp r1, #0
	beq .L_080e9502
	adds r0, r1, #0
	bl Func_080db9c0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #254
	lsls r0, r0, #2
	lsls r2, r2, #7
	orrs r3, r0
	adds r2, #255
	strb r3, [r5, #9]
	cmp r7, r2
	bgt .L_080e94f8
	ldr r0, [sp, #76]
	bl Func_080db9cc
	adds r0, #15
	b .L_080e9500
.L_080e94f8:
	ldr r0, [sp, #76]
	bl Func_080db9cc
	subs r0, #15
.L_080e9500:
	strh r0, [r5, #30]
.L_080e9502:
	movs r3, #0
	str r3, [r6, #24]
	ldr r0, [sp, #80]
	movs r2, #160
	ldr r3, [r0, #4]
	lsls r2, r2, #11
	str r3, [r6]
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r0, #12]
	str r3, [r6, #8]
	ldr r1, [sp, #32]
	cmp r1, r2
	ble .L_080e9548
	bl Random16
	adds r3, r0, #0
	ldr r2, [sp, #32]
	lsls r0, r3, #2
	adds r0, r0, r3
	adds r0, r2, r0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	b .L_080e955a
.L_080e9538:
	.4byte 0x000001f1
.L_080e953c:
	.4byte 0xfffff5c3
.L_080e9540:
	.4byte 0xffffeb86
.L_080e9544:
	.4byte 0xfffff000
.L_080e9548:
	bl Random16
	ldr r3, [sp, #32]
	lsls r0, r0, #2
	subs r0, r3, r0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Vector_AddPolarOffset
.L_080e955a:
	ldr r0, [sp, #44]
	adds r0, #1
	str r0, [sp, #44]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080e9568
	adds r3, #63
.L_080e9568:
	ldr r1, [sp, #44]
	movs r2, #1
	asrs r3, r3, #6
	negs r2, r2
	lsls r3, r3, #6
	add r9, r2
	subs r1, r1, r3
	mov r3, r9
	str r1, [sp, #44]
	cmp r3, #0
	bne .L_080e9488
.L_080e957e:
	ldr r2, [sp, #68]
	ldr r0, [sp, #84]
	ldr r3, .L_080e95b8
	movs r1, #130
	mov r8, r2
	lsls r1, r1, #4
	adds r6, r0, r1
	mov r0, r8
	ands r0, r3
	adds r4, r2, #0
	ldr r2, [sp, #84]
	mov r8, r0
	movs r0, #242
	lsls r0, r0, #4
	ldr r1, .L_080e95bc
	adds r5, r2, r0
	ldr r7, [sp, #68]
	ldr r2, [sp, #68]
	mov r10, r1
	adds r4, #8
	adds r2, #16
	adds r7, #24
	movs r1, #63
	ands r4, r3
	ands r2, r3
	ands r7, r3
	mov r9, r1
	b .L_080e95c0
	.2byte 0x0000
.L_080e95b8:
	.4byte 0x000003ff
.L_080e95bc:
	.4byte 0xfffffc00
.L_080e95c0:
	ldr r3, [r6, #24]
	cmp r3, #23
	bhi .L_080e961c
	cmp r3, #0
	bne .L_080e95d8
	ldrh r3, [r5, #8]
	mov r0, r10
	ands r3, r0
	mov r1, r8
	orrs r3, r1
	strh r3, [r5, #8]
	ldr r3, [r6, #24]
.L_080e95d8:
	cmp r3, #18
	bne .L_080e95e8
	ldrh r3, [r5, #8]
	mov r0, r10
	ands r3, r0
	orrs r3, r4
	strh r3, [r5, #8]
	ldr r3, [r6, #24]
.L_080e95e8:
	cmp r3, #20
	bne .L_080e95f8
	ldrh r3, [r5, #8]
	mov r1, r10
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	ldr r3, [r6, #24]
.L_080e95f8:
	cmp r3, #22
	bne .L_080e9606
	ldrh r3, [r5, #8]
	mov r0, r10
	ands r3, r0
	orrs r3, r7
	strh r3, [r5, #8]
.L_080e9606:
	adds r0, r5, #0
	adds r1, r6, #0
	str r2, [sp, #8]
	str r4, [sp, #4]
	bl Func_080eb298
	ldr r3, [r6, #24]
	ldr r4, [sp, #4]
	adds r3, #1
	str r3, [r6, #24]
	ldr r2, [sp, #8]
.L_080e961c:
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r3, r9
	adds r5, #40
	adds r6, #28
	cmp r3, #0
	bge .L_080e95c0
	ldr r0, [sp, #76]
	cmp r0, #0
	beq .L_080e9688
	mov r1, r11
	cmp r1, #0
	beq .L_080e9688
	ldr r2, [sp, #52]
	cmp r2, #0
	beq .L_080e9688
	ldrb r2, [r1, #20]
	ldrb r3, [r1, #21]
	adds r1, r3, #0
	muls r1, r2
	mov r3, r11
	ldrb r0, [r3, #16]
	movs r2, #0
	bl VramBlock_LoadCached
	ldr r1, .L_080e97bc
	lsls r0, r0, #5
	adds r5, r0, r1
	ldr r0, [sp, #56]
	movs r1, #3
	bl __modsi3
	cmp r0, #0
	bne .L_080e966e
	mov r2, r11
	ldrb r1, [r2, #20]
	ldr r0, [sp, #20]
	ldrb r2, [r2, #21]
	bl Func_080e9250
.L_080e966e:
	ldr r3, [sp, #56]
	mov r0, r11
	adds r3, #1
	str r3, [sp, #56]
	movs r3, #192
	lsls r3, r3, #18
	ldrb r1, [r0, #20]
	ldrb r2, [r0, #21]
	ldr r4, [r3, #84]
	ldr r0, [sp, #20]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_080e9688:
	ldr r1, [sp, #16]
	movs r0, #2
	adds r2, r1, #0
	adds r2, #1
	str r2, [sp, #16]
	ldr r2, [sp, #12]
	bl Func_080e8b44
	ldr r3, [sp, #64]
	cmp r3, #1
	beq .L_080e96e4
	cmp r3, #1
	bgt .L_080e96a8
	cmp r3, #0
	beq .L_080e96b4
	b .L_080e9776
.L_080e96a8:
	ldr r0, [sp, #64]
	cmp r0, #2
	beq .L_080e972c
	cmp r0, #3
	beq .L_080e9764
	b .L_080e9776
.L_080e96b4:
	ldr r1, [sp, #60]
	cmp r1, #40
	bne .L_080e96c4
	ldr r2, [sp, #36]
	cmp r2, #0
	beq .L_080e96c4
	movs r3, #1
	str r3, [sp, #48]
.L_080e96c4:
	ldr r0, [sp, #12]
	movs r1, #200
	lsls r1, r1, #5
	ldr r2, .L_080e97c0
	adds r1, #153
	adds r0, r0, r1
	str r0, [sp, #12]
	cmp r0, r2
	ble .L_080e9776
	ldr r3, [sp, #64]
	movs r0, #1
	adds r3, #1
	negs r0, r0
	str r3, [sp, #64]
	str r0, [sp, #60]
	b .L_080e9776
.L_080e96e4:
	ldr r1, [sp, #40]
	cmp r1, #0
	bne .L_080e970e
	ldr r2, [sp, #60]
	cmp r2, #50
	bne .L_080e96fa
	movs r0, #1
	movs r3, #2
	negs r0, r0
	str r3, [sp, #64]
	str r0, [sp, #60]
.L_080e96fa:
	ldr r1, [sp, #24]
	cmp r1, #0
	beq .L_080e9776
	ldr r3, [r1, #8]
	movs r0, #0
	ldr r1, [sp, #60]
	ldr r2, [sp, #28]
	mov lr, r3
	.2byte 0xf800
	b .L_080e9776
.L_080e970e:
	mov r3, r11
	ldrb r2, [r3, #21]
	ldr r0, [sp, #60]
	lsls r3, r2, #1
	adds r3, r3, r2
	cmp r0, r3
	bne .L_080e9726
	movs r2, #1
	movs r1, #2
	negs r2, r2
	str r1, [sp, #64]
	str r2, [sp, #60]
.L_080e9726:
	movs r3, #1
	str r3, [sp, #52]
	b .L_080e9776
.L_080e972c:
	ldr r0, [sp, #40]
	cmp r0, #1
	bne .L_080e9736
	movs r1, #0
	str r1, [sp, #52]
.L_080e9736:
	ldr r2, [sp, #60]
	cmp r2, #5
	bne .L_080e9746
	ldr r3, [sp, #36]
	cmp r3, #0
	beq .L_080e9746
	movs r0, #0
	str r0, [sp, #48]
.L_080e9746:
	ldr r1, [sp, #12]
	ldr r2, .L_080e97c4
	movs r3, #128
	adds r1, r1, r2
	lsls r3, r3, #10
	str r1, [sp, #12]
	cmp r1, r3
	bgt .L_080e9776
	ldr r0, [sp, #64]
	movs r1, #1
	adds r0, #1
	negs r1, r1
	str r0, [sp, #64]
	str r1, [sp, #60]
	b .L_080e9776
.L_080e9764:
	ldr r3, [sp, #60]
	movs r2, #0
	str r2, [sp, #12]
	cmp r3, #0
	bne .L_080e9776
	movs r0, #186
	lsls r0, r0, #2
	adds r0, #255
	str r0, [sp, #64]
.L_080e9776:
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #60]
	movs r3, #186
	ldr r2, [sp, #64]
	lsls r3, r3, #2
	adds r1, #1
	adds r3, #255
	str r1, [sp, #60]
	cmp r2, r3
	beq .L_080e9790
	b .L_080e9452
.L_080e9790:
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
	bl Func_080e8c9c
	bl BattleFx_PrepareBufferInterpolation
	ldr r0, [sp, #72]
	bl Resource_ResetEntry
.L_080e97a6:
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e97bc:
	.4byte 0x06010000
.L_080e97c0:
	.4byte 0x000bffff
.L_080e97c4:
	.4byte 0xffffd99a
