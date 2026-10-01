.syntax unified
	.thumb
	.global Func_0814a814
	.thumb_func
Func_0814a814:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #120
	str r0, [sp, #72]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r6, #224
	ldr r3, [r3, #96]
	mov r11, r0
	lsls r6, r6, #3
	mov r10, r1
	movs r0, #0
	movs r1, #16
	add r6, r11
	str r3, [sp, #68]
	str r1, [sp, #52]
	bl BattleFx_BeginCanvasLayer
	movs r2, #1
	ldr r0, .L_0814aa34
	adds r1, r6, #0
	movs r3, #1
	bl Resource_LoadAndDecompress
	mov r2, r10
	cmp r2, #0
	bne .L_0814a85a
	ldr r0, .L_0814aa38
	b .L_0814a8ba
.L_0814a85a:
	mov r3, r10
	cmp r3, #1
	bne .L_0814a864
	ldr r0, .L_0814aa3c
	b .L_0814a8ba
.L_0814a864:
	mov r4, r10
	cmp r4, #2
	bne .L_0814a86e
	ldr r0, .L_0814aa40
	b .L_0814a8ba
.L_0814a86e:
	mov r0, r10
	cmp r0, #3
	bne .L_0814a878
	ldr r0, .L_0814aa44
	b .L_0814a8ba
.L_0814a878:
	mov r1, r10
	cmp r1, #4
	bne .L_0814a882
	ldr r0, .L_0814aa48
	b .L_0814a8ba
.L_0814a882:
	mov r2, r10
	cmp r2, #5
	bne .L_0814a88c
	ldr r5, .L_0814aa4c
	b .L_0814a8ac
.L_0814a88c:
	mov r3, r10
	cmp r3, #7
	bne .L_0814a8a6
	movs r4, #24
	adds r1, r6, #0
	ldr r0, .L_0814aa4c
	movs r2, #1
	movs r3, #0
	str r4, [sp, #52]
	bl Resource_LoadAndDecompress
	ldr r0, .L_0814aa3c
	b .L_0814a8ba
.L_0814a8a6:
	ldr r5, .L_0814aa50
	movs r0, #32
	str r0, [sp, #52]
.L_0814a8ac:
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	adds r0, r5, #0
.L_0814a8ba:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814aa54
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	mov r1, r10
	cmp r1, #4
	bne .L_0814a8e2
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0814aa58
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_0814a8e2:
	mov r2, r10
	cmp r2, #3
	bne .L_0814a8f8
	movs r1, #178
	lsls r1, r1, #6
	ldr r0, .L_0814aa5c
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_0814a8f8:
	ldr r5, .L_0814aa60
	movs r3, #0
	mov r9, r3
	movs r6, #255
.L_0814a900:
	mov r4, r10
	cmp r4, #1
	bls .L_0814a916
	cmp r4, #4
	beq .L_0814a916
	cmp r4, #5
	beq .L_0814a916
	cmp r4, #6
	beq .L_0814a916
	cmp r4, #7
	bne .L_0814a93a
.L_0814a916:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #14
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	b .L_0814a95c
.L_0814a93a:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #13
	str r0, [r5]
	bl Random16
	ands r0, r6
	subs r0, #255
	lsls r0, r0, #13
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #13
.L_0814a95c:
	str r0, [r5, #8]
	movs r0, #1
	movs r1, #128
	movs r3, #0
	add r9, r0
	lsls r1, r1, #2
	str r3, [r5, #24]
	adds r5, #28
	cmp r9, r1
	bne .L_0814a900
	mov r2, r10
	cmp r2, #1
	bls .L_0814a986
	cmp r2, #4
	beq .L_0814a986
	cmp r2, #5
	beq .L_0814a986
	cmp r2, #6
	beq .L_0814a986
	cmp r2, #7
	bne .L_0814a990
.L_0814a986:
	ldr r4, [sp, #72]
	ldr r3, [r4, #20]
	lsls r3, r3, #3
	adds r3, #64
	b .L_0814a998
.L_0814a990:
	ldr r0, [sp, #72]
	ldr r3, [r0, #20]
	lsls r3, r3, #3
	adds r3, #32
.L_0814a998:
	str r3, [sp, #60]
	mov r1, r10
	cmp r1, #1
	bls .L_0814a9ae
	cmp r1, #3
	beq .L_0814a9ae
	movs r1, #200
	ldr r0, .L_0814aa64
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_0814a9ae:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0814aa68
	bl Scheduler_AddOrUpdateCallback
	movs r0, #142
	bl Audio_PlayCue
	ldr r3, [sp, #60]
	movs r2, #0
	str r2, [sp, #64]
	cmp r3, #0
	bne .L_0814a9e0
	b .L_0814ae74
.L_0814a9e0:
	subs r3, #32
	str r3, [sp, #32]
.L_0814a9e4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	mov r4, r10
	str r3, [sp, #48]
	cmp r4, #7
	bne .L_0814aa6c
	ldr r3, [sp, #60]
	ldr r0, [sp, #64]
	subs r3, #46
	cmp r0, r3
	bne .L_0814aa0c
	ldr r1, [sp, #72]
	movs r3, #0
	ldr r0, [r1, #8]
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #16
	bl BattleMotion_ApproachTargetFar
.L_0814aa0c:
	ldr r3, [sp, #64]
	ldr r4, [sp, #32]
	cmp r3, r4
	bne .L_0814aa7a
	movs r0, #134
	bl Func_081180e8
	ldr r2, [sp, #72]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #4
	bl Func_08118088
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #8
	str r3, [r2]
	b .L_0814aa7a
.L_0814aa34:
	.4byte 0x00000161
.L_0814aa38:
	.4byte 0x00000163
.L_0814aa3c:
	.4byte 0x00000150
.L_0814aa40:
	.4byte 0x00000166
.L_0814aa44:
	.4byte 0x00000184
.L_0814aa48:
	.4byte 0x0000017f
.L_0814aa4c:
	.4byte 0x0000017b
.L_0814aa50:
	.4byte 0x00000191
.L_0814aa54:
	.4byte IwramCopyWords
.L_0814aa58:
	.4byte 0x0000016e
.L_0814aa5c:
	.4byte 0x00000192
.L_0814aa60:
	.4byte gMapCellBuffer
.L_0814aa64:
	.4byte Func_08152474
.L_0814aa68:
	.4byte Func_08143000
.L_0814aa6c:
	ldr r3, [sp, #64]
	ldr r4, [sp, #32]
	cmp r3, r4
	bne .L_0814aa7a
	movs r0, #133
	bl Func_081180e8
.L_0814aa7a:
	ldr r1, [sp, #64]
	movs r6, #225
	lsls r6, r6, #7
	movs r0, #0
	movs r7, #128
	add r6, r11
	mov r9, r0
	lsls r7, r7, #11
	lsls r5, r1, #12
.L_0814aa8c:
	adds r0, r5, #0
	bl Trig_Sin
	movs r3, #1
	lsls r0, r0, #2
	subs r0, r7, r0
	movs r2, #128
	add r9, r3
	asrs r0, r0, #10
	lsls r2, r2, #4
	mov r4, r9
	stmia r6!, {r0}
	adds r5, r5, r2
	cmp r4, #160
	bne .L_0814aa8c
	movs r0, #0
	str r0, [sp, #56]
	ldr r1, [sp, #72]
	ldr r3, [r1, #20]
	cmp r3, #0
	bne .L_0814aab8
	b .L_0814ae48
.L_0814aab8:
	ldr r3, [sp, #48]
	mov r2, sp
	mov r4, sp
	adds r2, #76
	adds r3, #12
	adds r4, #96
	movs r0, #36
	movs r1, #0
	str r2, [sp, #40]
	str r3, [sp, #24]
	str r4, [sp, #36]
	str r0, [sp, #20]
	str r1, [sp, #16]
.L_0814aad2:
	ldr r2, [sp, #20]
	ldr r4, [sp, #72]
	ldrsh r0, [r2, r4]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r1, r10
	str r0, [sp, #44]
	ldr r0, [sp, #56]
	lsls r0, r0, #3
	str r0, [sp, #28]
	cmp r1, #3
	bne .L_0814abcc
	ldr r2, [sp, #16]
	ldr r3, [sp, #64]
	str r2, [sp, #28]
	cmp r3, r2
	ble .L_0814abcc
	ldr r4, [sp, #64]
	adds r3, r2, #0
	adds r3, #32
	cmp r4, r3
	bge .L_0814abcc
	ldr r1, [sp, #20]
	ldr r3, [sp, #72]
	add r5, sp, #84
	ldrsh r0, [r1, r3]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r7, [sp, #64]
	movs r4, #0
	mov r0, r10
	mov r9, r4
	ands r7, r0
.L_0814ab18:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r0, #0
	ands r2, r3
	str r2, [sp, #12]
	bl Random16
	ldr r2, [sp, #12]
	movs r5, #31
	ands r5, r0
	adds r0, r2, #0
	bl Trig_Sin
	ldr r6, [sp, #84]
	adds r5, #4
	lsrs r3, r6, #31
	adds r6, r6, r3
	adds r3, r5, #0
	muls r3, r0
	ldr r1, .L_0814ae9c
	asrs r3, r3, #17
	asrs r6, r6, #1
	ldr r2, [sp, #12]
	adds r6, r6, r3
	ldrb r3, [r1, r7]
	adds r0, r2, #0
	lsrs r3, r3, #1
	mov r8, r1
	subs r6, r6, r3
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	ldr r4, .L_0814aea0
	ldr r5, [sp, #88]
	asrs r3, r3, #16
	subs r5, r5, r3
	ldrb r3, [r4, r7]
	str r4, [sp, #8]
	lsrs r3, r3, #1
	subs r5, r5, r3
	bl Random16
	ldr r3, .L_0814aea4
	movs r2, #3
	ands r0, r2
	ldrb r2, [r3, r0]
	movs r3, #3
	orrs r3, r2
	movs r2, #1
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r2, .L_0814aea8
	lsls r3, r7, #1
	ldrh r1, [r2, r3]
	movs r3, #178
	lsls r3, r3, #6
	mov r0, r8
	add r1, r11
	adds r1, r1, r3
	ldrb r3, [r0, r7]
	ldr r4, [sp, #8]
	str r3, [sp, #0]
	movs r2, #192
	ldrb r3, [r4, r7]
	lsls r2, r2, #18
	str r3, [sp, #4]
	adds r2, #188
	adds r5, #16
	ldr r4, [r2]
	adds r3, r5, #0
	ldr r0, [sp, #68]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	add r9, r3
	mov r4, r9
	cmp r4, #2
	bne .L_0814ab18
.L_0814abcc:
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #19
	movs r0, #188
	str r3, [sp, #76]
	bl Func_081963ec
	movs r0, #192
	lsls r0, r0, #18
	adds r0, #188
	ldr r3, [r0]
	ldr r1, [sp, #40]
	str r3, [r1, #4]
	bl Func_08014de4
	ldr r0, [sp, #48]
	ldr r1, [sp, #24]
	bl Graphics_PrepareTransferInIwramWork
	ldr r2, [sp, #44]
	ldr r4, [sp, #36]
	ldr r3, [r2, #8]
	str r3, [r4]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r4, #4]
	ldr r3, [r2, #16]
	str r3, [r4, #8]
	ldr r0, [sp, #36]
	bl SceneTransform_ApplyPosition
	ldr r0, [sp, #64]
	ldr r1, [sp, #16]
	cmp r0, r1
	bgt .L_0814ac1e
	b .L_0814adfa
.L_0814ac1e:
	lsls r5, r0, #9
	adds r0, r5, #0
	bl Func_08015068
	mov r2, r10
	cmp r2, #1
	bls .L_0814ac30
	cmp r2, #4
	bne .L_0814ac36
.L_0814ac30:
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
.L_0814ac36:
	ldr r4, [sp, #52]
	movs r3, #0
	mov r9, r3
	cmp r4, #0
	bne .L_0814ac42
	b .L_0814adfa
.L_0814ac42:
	ldr r0, [sp, #56]
	ldr r1, .L_0814aeac
	lsls r2, r0, #6
	lsls r3, r0, #9
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r7, r3, r1
.L_0814ac50:
	ldr r3, [sp, #28]
	ldr r2, [sp, #64]
	add r3, r9
	cmp r2, r3
	bgt .L_0814ac5c
	b .L_0814adec
.L_0814ac5c:
	ldr r3, [r7]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r7, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r7, #8]
	adds r0, r0, r2
	asrs r3, r3, #8
	adds r4, r3, #0
	muls r4, r3
	adds r3, r4, #0
	adds r0, r0, r3
	ldr r3, .L_0814aeb0
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #9
	mov r8, r0
	cmp r0, #0
	bne .L_0814ac8a
	b .L_0814adec
.L_0814ac8a:
	ldr r3, [r7, #24]
	cmp r3, #23
	ble .L_0814ac92
	b .L_0814adec
.L_0814ac92:
	adds r1, r3, #0
	cmp r1, #0
	bge .L_0814ac9a
	adds r1, #3
.L_0814ac9a:
	add r5, sp, #108
	asrs r6, r1, #2
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	mov r0, r10
	asrs r2, r3, #1
	str r2, [r5]
	cmp r0, #5
	beq .L_0814acb6
	cmp r0, #7
	bne .L_0814ace0
.L_0814acb6:
	lsls r1, r6, #1
	adds r1, r1, r6
	lsls r1, r1, #3
	adds r1, r1, r6
	lsls r1, r1, #6
	movs r3, #224
	movs r0, #40
	lsls r3, r3, #3
	add r1, r11
	adds r1, r1, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #40]
	subs r2, #20
	ldr r4, [r0, #4]
	subs r3, #20
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	b .L_0814ad58
.L_0814ace0:
	mov r1, r10
	cmp r1, #6
	bne .L_0814acfe
	movs r1, #12
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #24
	str r1, [sp, #4]
	ldr r0, [sp, #40]
	movs r1, #152
	lsls r1, r1, #5
	ldr r4, [r0, #4]
	subs r2, #6
	subs r3, #12
	b .L_0814ad1a
.L_0814acfe:
	mov r1, r10
	cmp r1, #4
	bne .L_0814ad24
	movs r1, #22
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #42
	str r1, [sp, #4]
	ldr r0, [sp, #40]
	movs r1, #224
	lsls r1, r1, #3
	ldr r4, [r0, #4]
	subs r2, #11
	subs r3, #21
.L_0814ad1a:
	ldr r0, [sp, #68]
	add r1, r11
	mov lr, r4
	.2byte 0xf800
	b .L_0814ad58
.L_0814ad24:
	mov r1, r9
	movs r3, #3
	ands r3, r1
	lsls r1, r6, #3
	negs r4, r3
	adds r1, r1, r6
	orrs r4, r3
	lsls r1, r1, #7
	movs r3, #224
	lsls r3, r3, #3
	movs r0, #24
	add r1, r11
	adds r1, r1, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r0, [sp, #40]
	lsrs r4, r4, #31
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	subs r2, #12
	subs r3, #24
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
.L_0814ad58:
	mov r1, r10
	cmp r1, #1
	bls .L_0814ad6a
	cmp r1, #4
	beq .L_0814ad6a
	cmp r1, #5
	beq .L_0814ad6a
	cmp r1, #6
	bne .L_0814ad96
.L_0814ad6a:
	ldr r5, [r7]
	mov r1, r8
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r7]
	ldr r5, [r7, #4]
	mov r1, r8
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r7, #4]
	ldr r5, [r7, #8]
	mov r1, r8
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r7, #8]
	b .L_0814ada0
.L_0814ad96:
	ldr r3, [r7, #4]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #4]
.L_0814ada0:
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
	cmp r3, #24
	bne .L_0814adec
	mov r3, r10
	cmp r3, #1
	bls .L_0814adbc
	cmp r3, #4
	beq .L_0814adbc
	cmp r3, #5
	beq .L_0814adbc
	cmp r3, #6
	bne .L_0814adc2
.L_0814adbc:
	movs r3, #0
	str r3, [r7, #24]
	b .L_0814adec
.L_0814adc2:
	bl Random16
	movs r4, #255
	ands r0, r4
	subs r0, #127
	lsls r0, r0, #13
	str r0, [r7]
	bl Random16
	movs r1, #255
	ands r0, r1
	subs r0, #255
	lsls r0, r0, #12
	str r0, [r7, #4]
	bl Random16
	movs r2, #255
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #13
	str r0, [r7, #8]
.L_0814adec:
	ldr r4, [sp, #52]
	movs r3, #1
	add r9, r3
	adds r7, #28
	cmp r9, r4
	beq .L_0814adfa
	b .L_0814ac50
.L_0814adfa:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #16]
	ldr r0, [sp, #64]
	adds r3, #16
	cmp r0, r3
	bne .L_0814ae2c
	ldr r1, [sp, #60]
	subs r3, r1, r0
	cmp r3, #31
	ble .L_0814ae1a
	movs r3, #31
.L_0814ae1a:
	ldr r2, [sp, #20]
	ldr r1, [sp, #72]
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #56]
	bl Func_0814cd48
.L_0814ae2c:
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	ldr r4, [sp, #56]
	adds r3, #8
	adds r2, #2
	adds r4, #1
	str r2, [sp, #20]
	str r3, [sp, #16]
	str r4, [sp, #56]
	ldr r0, [sp, #72]
	ldr r3, [r0, #20]
	cmp r4, r3
	beq .L_0814ae48
	b .L_0814aad2
.L_0814ae48:
	movs r1, #16
	movs r0, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #64]
	ldr r2, [sp, #60]
	adds r1, #1
	str r1, [sp, #64]
	cmp r1, r2
	beq .L_0814ae74
	b .L_0814a9e4
.L_0814ae74:
	ldr r0, .L_0814aeb4
	bl Scheduler_RemoveCallback
	mov r3, r10
	cmp r3, #1
	bls .L_0814ae8a
	cmp r3, #3
	beq .L_0814ae8a
	ldr r0, .L_0814aeb8
	bl Scheduler_RemoveCallback
.L_0814ae8a:
	bl Func_08143bb8
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0814ae9c:
	.4byte Data_08197492
.L_0814aea0:
	.4byte Data_08197498
.L_0814aea4:
	.4byte Data_08197a34
.L_0814aea8:
	.4byte Data_08197486
.L_0814aeac:
	.4byte gMapCellBuffer
.L_0814aeb0:
	.4byte IwramFillWords + 0x74
.L_0814aeb4:
	.4byte Func_08143000
.L_0814aeb8:
	.4byte Func_08152474
