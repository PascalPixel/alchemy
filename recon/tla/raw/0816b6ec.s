.syntax unified
	.thumb
	.global Func_0816b6ec
	.thumb_func
Func_0816b6ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r0
	ldr r0, [r3, #92]
	sub sp, #84
	str r0, [sp, #48]
	mov r9, r1
	ldr r1, [r3, #96]
	movs r0, #0
	str r1, [sp, #44]
	ldr r2, [r3, #48]
	str r2, [sp, #32]
	ldr r5, [r3, #100]
	bl BattleFx_BeginCanvasLayer
	mov r3, r9
	cmp r3, #0
	bne .L_0816b748
	mov r4, r11
	ldr r1, [r4, #4]
	mov r0, sp
	adds r0, #60
	str r0, [sp, #12]
	movs r3, #3
	lsls r1, r1, #4
	orrs r1, r3
	ldr r2, [sp, #12]
	add r3, sp, #72
	mov r0, r11
	bl Func_0815585c
	movs r2, #128
	ldr r3, .L_0816b744
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	b .L_0816b762
.L_0816b744:
	.4byte 0x00000785
.L_0816b748:
	mov r2, r11
	ldr r1, [r2, #4]
	movs r3, #35
	lsls r1, r1, #4
	orrs r1, r3
	mov r3, sp
	adds r3, #60
	str r3, [sp, #12]
	mov r0, r11
	add r3, sp, #72
	ldr r2, [sp, #12]
	bl Func_0815585c
.L_0816b762:
	mov r4, r11
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0816b774
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_0816b77c
.L_0816b774:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_0816b77c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r2, #128
	str r3, [sp, #36]
	ldr r3, .L_0816b7c0
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r0, [sp, #48]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #50
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_0816b7c4
	bl Scheduler_AddOrUpdateCallback
	movs r4, #0
	movs r0, #36
	mov r1, r9
	str r4, [sp, #28]
	str r0, [sp, #24]
	cmp r1, #1
	bne .L_0816b7da
	b .L_0816b7c8
.L_0816b7c0:
	.4byte 0x00001010
.L_0816b7c4:
	.4byte Func_08143000
.L_0816b7c8:
	mov r3, r11
	ldr r2, [r3, #20]
	cmp r2, #4
	ble .L_0816b7da
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #12
	str r3, [sp, #24]
.L_0816b7da:
	ldr r4, [sp, #48]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r4, r2
	ldr r0, .L_0816bb30
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0816bb34
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r3, [sp, #48]
	movs r4, #156
	lsls r4, r4, #6
	adds r1, r3, r4
	ldr r0, .L_0816bb38
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_0816bb3c
	movs r1, #4
	movs r2, #32
	movs r3, #32
	bl Func_08178680
	ldr r1, [sp, #24]
	movs r0, #0
	mov r10, r0
	cmp r1, #0
	bne .L_0816b822
	b .L_0816bc34
.L_0816b822:
	ldr r2, [sp, #32]
	mov r3, sp
	adds r2, #12
	adds r3, #52
	str r2, [sp, #20]
	str r3, [sp, #16]
	str r0, [sp, #8]
.L_0816b830:
	mov r4, r10
	cmp r4, #0
	bne .L_0816b872
	movs r0, #0
	mov r1, r9
	str r0, [sp, #28]
	cmp r1, #0
	bne .L_0816b844
	movs r5, #80
	b .L_0816b846
.L_0816b844:
	movs r5, #30
.L_0816b846:
	movs r6, #32
	ldr r4, .L_0816bb40
	movs r1, #0
	movs r0, #0
.L_0816b84e:
	lsls r3, r1, #3
	movs r7, #0
	negs r2, r5
	adds r3, r3, r4
.L_0816b856:
	adds r7, #1
	strb r2, [r3, #1]
	strb r0, [r3]
	strb r6, [r3, #2]
	subs r2, #24
	adds r3, #4
	cmp r7, #2
	bne .L_0816b856
	adds r1, #1
	cmp r1, #33
	bne .L_0816b84e
	movs r0, #179
	bl Audio_PlayCue
.L_0816b872:
	mov r2, r10
	cmp r2, #19
	ble .L_0816b892
	mov r3, r9
	cmp r3, #0
	bne .L_0816b886
	ldr r0, .L_0816bb44
	bl Func_0815f0a0
	b .L_0816b892
.L_0816b886:
	mov r4, r9
	cmp r4, #1
	bne .L_0816b892
	ldr r0, .L_0816bb48
	bl Func_0815f0a0
.L_0816b892:
	mov r0, r10
	cmp r0, #79
	bgt .L_0816b8f0
	ldr r0, [sp, #8]
	bl Trig_Sin
	ldr r1, [sp, #12]
	lsls r0, r0, #3
	ldr r3, [r1]
	asrs r0, r0, #16
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r0, r0, r3
	adds r5, r0, #0
	ldr r0, [sp, #8]
	bl Trig_Cos
	ldr r2, [sp, #12]
	lsls r0, r0, #2
	ldr r3, [r2, #4]
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r4, r0, #0
	mov r3, r10
	subs r5, #10
	subs r4, #24
	cmp r3, #7
	ble .L_0816b8d4
	lsls r3, r3, #2
	subs r3, r4, r3
	adds r4, r3, #0
	adds r4, #32
.L_0816b8d4:
	movs r3, #20
	ldr r2, [sp, #48]
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	movs r3, #156
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, [sp, #44]
	adds r3, r4, #0
	adds r2, r5, #0
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
.L_0816b8f0:
	movs r3, #1
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	bne .L_0816b902
	cmp r0, #47
	bgt .L_0816b902
	cmp r0, #31
	bgt .L_0816b926
.L_0816b902:
	ldr r0, .L_0816bb4c
	movs r1, #32
.L_0816b906:
	lsls r3, r1, #3
	movs r7, #0
	adds r2, r3, r0
.L_0816b90c:
	ldrb r3, [r2]
	adds r7, #1
	strb r3, [r2, #8]
	ldrb r3, [r2, #1]
	strb r3, [r2, #9]
	ldrb r3, [r2, #2]
	strb r3, [r2, #10]
	adds r2, #4
	cmp r7, #2
	bne .L_0816b90c
	subs r1, #1
	cmp r1, #0
	bne .L_0816b906
.L_0816b926:
	mov r1, r10
	cmp r1, #23
	ble .L_0816b93c
	ldr r2, [sp, #28]
	lsls r3, r1, #4
	movs r4, #152
	subs r3, r2, r3
	lsls r4, r4, #4
	adds r4, r3, r4
	str r4, [sp, #28]
	b .L_0816b946
.L_0816b93c:
	ldr r0, [sp, #28]
	movs r1, #128
	lsls r1, r1, #4
	adds r0, r0, r1
	str r0, [sp, #28]
.L_0816b946:
	mov r2, r9
	cmp r2, #0
	bne .L_0816b96a
	mov r3, r10
	lsls r3, r3, #12
	mov r8, r3
	movs r3, #205
	lsls r3, r3, #2
	mov r0, r10
	muls r0, r3
	bl Trig_Sin
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	asrs r3, r3, #16
	movs r2, #80
	b .L_0816b984
.L_0816b96a:
	ldr r4, [sp, #28]
	movs r3, #205
	lsls r3, r3, #2
	mov r0, r10
	muls r0, r3
	mov r8, r4
	bl Trig_Sin
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	asrs r3, r3, #16
	movs r2, #30
.L_0816b984:
	subs r2, r2, r3
	ldr r5, .L_0816bb40
	movs r7, #0
	negs r6, r2
.L_0816b98c:
	mov r0, r8
	bl Trig_Sin
	lsrs r0, r0, #11
	strb r0, [r5]
	strb r6, [r5, #1]
	mov r0, r8
	bl Trig_Cos
	adds r7, #1
	lsrs r0, r0, #11
	strb r0, [r5, #2]
	subs r6, #24
	adds r5, #4
	cmp r7, #2
	bne .L_0816b98c
	mov r0, r9
	cmp r0, #0
	bne .L_0816b9e2
	mov r1, r10
	cmp r1, #28
	bne .L_0816ba2a
	ldr r3, [sp, #48]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #6
	str r3, [r2]
	movs r0, #133
	bl Func_081180e8
	mov r2, r11
	movs r3, #8
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	b .L_0816ba2a
.L_0816b9e2:
	mov r3, r10
	cmp r3, #12
	bne .L_0816b9f0
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
.L_0816b9f0:
	mov r4, r11
	ldr r3, [r4, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_0816ba2a
	movs r6, #36
	movs r5, #12
.L_0816b9fe:
	cmp r10, r5
	bne .L_0816ba20
	movs r0, #126
	bl Audio_PlayCue
	mov r1, r11
	ldrsh r0, [r6, r1]
	movs r3, #8
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	negs r2, r2
	bl Func_0814cd48
	mov r4, r11
	ldr r3, [r4, #20]
.L_0816ba20:
	adds r7, #1
	adds r6, #2
	adds r5, #4
	cmp r7, r3
	bne .L_0816b9fe
.L_0816ba2a:
	bl Func_08014de4
	ldr r0, [sp, #32]
	ldr r1, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	movs r0, #1
	bl Func_081969f8
	adds r5, r0, #0
	mov r0, r10
	ldr r6, .L_0816bb50
	movs r1, #0
	cmp r0, #15
	bgt .L_0816ba56
	lsls r3, r0, #2
	adds r1, r3, #0
	subs r1, #64
	cmp r1, #0
	ble .L_0816ba6e
	movs r1, #0
	b .L_0816ba6e
.L_0816ba56:
	mov r2, r10
	cmp r2, #27
	ble .L_0816ba6e
	movs r3, #28
	subs r3, r3, r2
	lsls r1, r3, #3
	movs r3, #64
	negs r3, r3
	cmp r1, r3
	bge .L_0816ba6e
	movs r1, #64
	negs r1, r1
.L_0816ba6e:
	ldr r3, [sp, #52]
	ldr r2, .L_0816bb54
	ldr r4, [sp, #48]
	ands r3, r2
	movs r2, #7
	orrs r3, r2
	ldr r2, .L_0816bb58
	movs r0, #224
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	ldr r2, [sp, #16]
	lsls r0, r0, #3
	str r3, [sp, #52]
	adds r3, r4, r0
	str r3, [r2, #4]
	movs r3, #6
	str r3, [r5]
	ldr r3, .L_0816bb3c
	str r2, [r5, #16]
	ldr r2, .L_0816bb5c
	str r3, [r5, #8]
	movs r3, #0
	strb r3, [r5, #24]
	strb r3, [r5, #25]
	movs r3, #104
	str r3, [r2, #16]
	ldr r3, .L_0816bb60
	mov r4, r9
	ldrb r3, [r3, r4]
	str r6, [r5, #12]
	str r1, [r5, #20]
	movs r7, #0
	cmp r3, #0
	bne .L_0816bab8
	b .L_0816bbf0
.L_0816bab8:
	bl Func_08014de4
	mov r0, r9
	cmp r0, #0
	bne .L_0816bb68
	ldr r1, [sp, #12]
	movs r2, #0
	ldr r0, [r1]
	movs r1, #160
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r1, r1, #14
	lsls r0, r0, #16
	bl Func_08015160
	lsls r0, r7, #11
	bl SceneTransform_ApplyPitch
	movs r3, #164
	lsls r3, r3, #7
	adds r3, #8
	adds r0, r7, #0
	muls r0, r3
	ldr r2, [sp, #28]
	adds r0, r0, r2
	bl Func_08015068
	mov r3, r10
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #4
	add r0, r10
	lsls r0, r0, #2
	bl Trig_Sin
	movs r3, #167
	lsls r3, r3, #9
	adds r3, #32
	mov r4, r10
	subs r3, r3, r0
	cmp r4, #27
	ble .L_0816bb18
	ldr r0, [sp, #8]
	ldr r1, .L_0816bb64
	adds r3, r3, r0
	adds r3, r3, r1
.L_0816bb18:
	lsls r2, r3, #1
	adds r2, r2, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	b .L_0816bbd2
	.2byte 0x0000
.L_0816bb30:
	.4byte 0x000000d3
.L_0816bb34:
	.4byte 0x00000134
.L_0816bb38:
	.4byte 0x00000153
.L_0816bb3c:
	.4byte Data_02010318
.L_0816bb40:
	.4byte gMapCellBuffer
.L_0816bb44:
	.4byte 0x00000166
.L_0816bb48:
	.4byte 0x00000167
.L_0816bb4c:
	.4byte Data_0200fff8
.L_0816bb50:
	.4byte Data_02010108
.L_0816bb54:
	.4byte 0xffffff00
.L_0816bb58:
	.4byte 0xffff00ff
.L_0816bb5c:
	.4byte gCameraSceneParameters
.L_0816bb60:
	.4byte Data_08198b24
.L_0816bb64:
	.4byte 0xffff2000
.L_0816bb68:
	ldr r2, [sp, #12]
	lsls r1, r7, #20
	ldr r0, [r2]
	movs r2, #0
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	movs r3, #128
	lsls r3, r3, #13
	subs r0, #64
	adds r1, r1, r3
	lsls r0, r0, #16
	bl Func_08015160
	ldr r4, .L_0816bc54
	lsls r0, r7, #12
	adds r0, r0, r4
	bl SceneTransform_ApplyPitch
	movs r3, #164
	lsls r3, r3, #7
	adds r3, #8
	adds r0, r7, #0
	muls r0, r3
	ldr r1, [sp, #28]
	adds r0, r0, r1
	bl Func_08015068
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #170
	mov r0, r10
	muls r0, r3
	bl Trig_Sin
	cmp r0, #0
	bge .L_0816bbb4
	adds r0, #3
.L_0816bbb4:
	movs r1, #128
	asrs r3, r0, #2
	lsls r1, r1, #9
	mov r2, r10
	subs r0, r1, r3
	cmp r2, #27
	ble .L_0816bbca
	ldr r4, [sp, #8]
	ldr r2, .L_0816bc58
	adds r3, r0, r4
	adds r0, r3, r2
.L_0816bbca:
	lsls r2, r0, #1
	adds r0, r2, #0
	bl Func_080151e4
.L_0816bbd2:
	ldr r0, .L_0816bc5c
	ldr r1, .L_0816bc60
	movs r2, #66
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	ldr r3, .L_0816bc64
	mov r4, r9
	ldrb r3, [r3, r4]
	adds r7, #1
	cmp r7, r3
	beq .L_0816bbf0
	b .L_0816bab8
.L_0816bbf0:
	ldr r2, .L_0816bc68
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #48]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #8]
	movs r3, #128
	ldr r0, [sp, #24]
	lsls r3, r3, #4
	movs r4, #1
	adds r2, r2, r3
	add r10, r4
	str r2, [sp, #8]
	cmp r10, r0
	beq .L_0816bc34
	b .L_0816b830
.L_0816bc34:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816bc6c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816bc54:
	.4byte 0xfffff800
.L_0816bc58:
	.4byte 0xffff2000
.L_0816bc5c:
	.4byte gMapCellBuffer
.L_0816bc60:
	.4byte Data_02010108
.L_0816bc64:
	.4byte Data_08198b24
.L_0816bc68:
	.4byte gCameraSceneParameters
.L_0816bc6c:
	.4byte Func_08143000
