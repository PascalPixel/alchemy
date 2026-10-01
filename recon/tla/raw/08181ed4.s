.syntax unified
	.thumb
	.global Func_08181ed4
	.thumb_func
Func_08181ed4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r0, [sp, #36]
	str r1, [sp, #32]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #28]
	movs r0, #1
	ldr r1, [r3, #96]
	str r1, [sp, #24]
	ldr r3, [r3, #100]
	str r3, [sp, #20]
	bl BattleFx_BeginCanvasLayer
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_08181f10
	movs r2, #128
	ldr r3, .L_08181f0c
	b .L_08181f14
	.2byte 0x0000
.L_08181f0c:
	.4byte 0x00000c10
.L_08181f10:
	movs r2, #128
	ldr r3, .L_08181f50
.L_08181f14:
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r3, sp
	adds r3, #48
	adds r1, r3, #0
	movs r0, #0
	str r3, [sp, #16]
	bl Func_08144aac
	ldr r5, [sp, #28]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r5, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r5, r0
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08181f54
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	str r1, [sp, #12]
	b .L_08181f58
.L_08181f50:
	.4byte 0x00001010
.L_08181f54:
	.4byte Func_08143000
.L_08181f58:
	movs r2, #0
	ldr r0, .L_0818229c
	ldr r1, [sp, #20]
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_08181f90
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r5, r3
	movs r2, #1
	movs r3, #1
	ldr r0, .L_081822a0
	bl Resource_LoadAndDecompress
	ldr r0, .L_081822a4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081822a8
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_08181fb6
.L_08181f90:
	ldr r5, [sp, #28]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r5, r7
	movs r2, #0
	movs r3, #0
	ldr r0, .L_081822ac
	bl Resource_LoadAndDecompress
	ldr r0, .L_081822b0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081822a8
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08181fb6:
	ldr r1, [sp, #32]
	movs r0, #0
	subs r1, #1
	str r1, [sp, #8]
	mov r10, r0
.L_08181fc0:
	mov r2, r10
	cmp r2, #19
	bgt .L_08181fd0
	add r3, sp, #56
	mov r9, r3
	mov r0, r9
	bl Func_0815e22c
.L_08181fd0:
	mov r5, r10
	cmp r5, #0
	bne .L_08181fea
	movs r0, #144
	bl Audio_PlayCue
	ldr r7, [sp, #28]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r7, r0
	movs r3, #16
	str r3, [r2]
.L_08181fea:
	mov r1, r10
	cmp r1, #20
	bne .L_08182010
	ldr r2, [sp, #28]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r3, r2, r5
	movs r2, #8
	movs r0, #134
	str r2, [r3]
	bl Func_081180e8
	ldr r1, [sp, #36]
	movs r7, #36
	ldrsh r0, [r1, r7]
	movs r1, #1
	bl Func_08118088
.L_08182010:
	mov r2, r10
	cmp r2, #0
	bne .L_08182032
	movs r3, #0
	str r3, [sp, #12]
	mov r8, r3
	ldr r3, .L_081822b4
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #2
.L_08182026:
	movs r5, #1
	add r8, r5
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08182026
.L_08182032:
	ldr r7, [sp, #8]
	cmp r7, #1
	bhi .L_0818206c
	movs r0, #0
	mov r8, r0
.L_0818203c:
	mov r1, r10
	cmp r1, #7
	ble .L_08182062
	cmp r1, #19
	bgt .L_0818204e
	ldr r0, .L_081822b8
	bl Func_0815f0a0
	b .L_08182062
.L_0818204e:
	mov r2, r10
	cmp r2, #31
	bgt .L_0818205c
	ldr r0, .L_081822bc
	bl Func_0815f0a0
	b .L_08182062
.L_0818205c:
	ldr r0, .L_081822c0
	bl Func_0815f0a0
.L_08182062:
	movs r3, #1
	add r8, r3
	mov r5, r8
	cmp r5, #3
	bne .L_0818203c
.L_0818206c:
	ldr r0, [sp, #32]
	movs r7, #0
	mov r11, r7
	cmp r0, #2
	bne .L_0818208e
	mov r1, r10
	cmp r1, #0
	bne .L_08182080
	movs r2, #100
	mov r11, r2
.L_08182080:
	mov r3, r10
	subs r3, #1
	cmp r3, #22
	bhi .L_081820aa
	movs r3, #2
	mov r11, r3
	b .L_081820aa
.L_0818208e:
	mov r5, r10
	cmp r5, #0
	bne .L_08182098
	movs r7, #200
	mov r11, r7
.L_08182098:
	ldr r0, [sp, #8]
	cmp r0, #1
	bhi .L_081820aa
	mov r3, r10
	subs r3, #1
	cmp r3, #22
	bhi .L_081820aa
	movs r1, #16
	mov r11, r1
.L_081820aa:
	movs r2, #0
	mov r3, r11
	mov r8, r2
	cmp r3, #0
	beq .L_0818214e
	add r5, sp, #56
	mov r9, r5
.L_081820b8:
	ldr r7, [sp, #12]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r7
	lsls r3, r2, #3
	ldr r0, .L_081822c4
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r6, r3, r0
	bl Random16
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r0
	adds r7, r3, #0
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	ldr r1, .L_081822c8
	adds r3, #255
	ands r3, r0
	mov r2, r9
	adds r5, r3, r1
	ldr r3, [r2]
	adds r7, #32
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r6]
	movs r3, #216
	lsls r3, r3, #15
	str r3, [r6, #4]
	ldr r3, [sp, #32]
	cmp r3, #2
	bne .L_08182114
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #8
	b .L_08182120
.L_08182114:
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #6
.L_08182120:
	str r3, [r6, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r6, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #48
	str r3, [r6, #24]
	ldr r7, [sp, #12]
	movs r5, #1
	adds r7, #1
	add r8, r5
	str r7, [sp, #12]
	cmp r8, r11
	bne .L_081820b8
.L_0818214e:
	ldr r6, .L_081822c4
	movs r0, #0
	mov r8, r0
.L_08182154:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_081821de
	subs r3, #1
	str r3, [r6, #24]
	ldr r1, [sp, #32]
	ldr r2, .L_081822cc
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	adds r0, r6, #0
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #4]
	asrs r2, r3, #16
	mov r12, r2
	cmp r2, #120
	ble .L_08182186
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_081821de
.L_08182186:
	ldr r2, [r6]
	cmp r2, #0
	blt .L_081821de
	asrs r7, r2, #16
	cmp r7, #126
	bgt .L_081821de
	cmp r3, #0
	blt .L_081821de
	ldr r3, [r6, #24]
	adds r2, r3, #0
	subs r2, #16
	cmp r2, #0
	bge .L_081821a2
	adds r2, #7
.L_081821a2:
	ldr r3, .L_081822d0
	ldr r0, [sp, #32]
	asrs r5, r2, #3
	ldrsb r3, [r3, r0]
	cmp r5, r3
	bge .L_081821b0
	adds r5, r3, #0
.L_081821b0:
	ldr r2, .L_081822d4
	lsls r4, r5, #1
	subs r3, r4, #2
	mov r1, r8
	movs r0, #1
	ands r0, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	str r5, [sp, #0]
	adds r1, r2, r1
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	subs r3, r7, r5
	str r4, [sp, #4]
	ldr r5, [sp, #16]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #24]
	mov lr, r4
	.2byte 0xf800
.L_081821de:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #2
	adds r6, #28
	cmp r8, r0
	bne .L_08182154
	mov r1, r10
	cmp r1, #39
	ble .L_081821f4
	b .L_08182332
.L_081821f4:
	mov r0, r8
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_081822d8
	ldr r3, [sp, #40]
	movs r7, #224
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_081822dc
	lsls r7, r7, #3
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, [sp, #28]
	str r3, [sp, #40]
	adds r5, r0, #0
	adds r3, r2, r7
	mov r0, r10
	add r7, sp, #40
	movs r2, #31
	str r3, [r7, #4]
	lsls r3, r0, #2
	ands r3, r2
	strb r3, [r5, #24]
	lsls r3, r0, #1
	ands r3, r2
	strb r3, [r5, #25]
	bl Func_08014de4
	ldr r0, [sp, #56]
	ldr r1, .L_081822e0
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	movs r0, #250
	lsls r0, r0, #3
	bl SceneTransform_ApplyPitch
	ldr r1, [sp, #32]
	cmp r1, #0
	bne .L_08182286
	mov r2, r10
	cmp r2, #15
	bgt .L_0818226a
	lsls r0, r2, #10
	bl Trig_Sin
	b .L_08182280
.L_0818226a:
	movs r0, #128
	mov r3, r10
	lsls r0, r0, #9
	cmp r3, #23
	ble .L_08182280
	ldr r2, .L_081822e4
	mov r1, r10
	lsls r0, r1, #10
	adds r0, r0, r2
	bl Trig_Sin
.L_08182280:
	movs r2, #32
	negs r2, r2
	b .L_081822fc
.L_08182286:
	ldr r3, [sp, #32]
	cmp r3, #1
	bne .L_081822e8
	mov r1, r10
	movs r2, #32
	lsls r0, r1, #12
	negs r2, r2
	cmp r1, #31
	ble .L_081822fc
	lsls r2, r1, #3
	b .L_081822f8
.L_0818229c:
	.4byte 0x00000134
.L_081822a0:
	.4byte 0x000000c0
.L_081822a4:
	.4byte 0x00000148
.L_081822a8:
	.4byte IwramCopyWords
.L_081822ac:
	.4byte 0x000000d2
.L_081822b0:
	.4byte 0x00000184
.L_081822b4:
	.4byte Data_02010018
.L_081822b8:
	.4byte 0x00000154
.L_081822bc:
	.4byte 0x00000150
.L_081822c0:
	.4byte 0x00000152
.L_081822c4:
	.4byte gMapCellBuffer
.L_081822c8:
	.4byte 0xffffc000
.L_081822cc:
	.4byte Data_08199660
.L_081822d0:
	.4byte Data_0819966c
.L_081822d4:
	.4byte Data_08197410
.L_081822d8:
	.4byte 0xffffff00
.L_081822dc:
	.4byte 0xffff00ff
.L_081822e0:
	.4byte 0xfff00000
.L_081822e4:
	.4byte 0xffffe000
.L_081822e8:
	movs r0, #128
	movs r2, #32
	mov r3, r10
	lsls r0, r0, #7
	negs r2, r2
	cmp r3, #31
	ble .L_081822fc
	lsls r2, r3, #3
.L_081822f8:
	movs r3, #224
	subs r2, r3, r2
.L_081822fc:
	movs r3, #7
	str r3, [r5]
	ldr r3, .L_08182384
	str r2, [r5, #20]
	movs r1, #224
	lsls r2, r0, #1
	str r3, [r5, #8]
	adds r0, r2, #0
	lsls r1, r1, #11
	str r7, [r5, #16]
	str r6, [r5, #12]
	bl Func_080151e4
	adds r1, r6, #0
	movs r2, #16
	ldr r0, .L_08182388
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r6, #0
	bl Sys_Free
.L_08182332:
	movs r1, #16
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r7, #240
	ldr r5, [sp, #28]
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r5, r7
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #54
	beq .L_0818235e
	b .L_08181fc0
.L_0818235e:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0818238c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08182384:
	.4byte Data_08198df8
.L_08182388:
	.4byte Data_08198c6c
.L_0818238c:
	.4byte Func_08143000
