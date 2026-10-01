.syntax unified
	.thumb
	.global Func_0815585c
	.thumb_func
Func_0815585c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #120
	str r3, [sp, #48]
	str r0, [sp, #56]
	str r2, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	lsrs r6, r1, #4
	str r0, [sp, #44]
	movs r5, #7
	ldr r2, [r3, #96]
	ands r5, r1
	str r2, [sp, #40]
	lsrs r1, r1, #5
	ldr r3, [r3, #100]
	str r3, [sp, #28]
	movs r3, #1
	ands r6, r3
	movs r3, #15
	ands r1, r3
	movs r3, #40
	str r1, [sp, #20]
	str r3, [sp, #24]
	cmp r1, #6
	beq .L_081558ec
	ldr r4, [sp, #20]
	cmp r4, #8
	bne .L_081558c0
	ldr r7, [sp, #44]
	movs r0, #238
	lsls r0, r0, #7
	movs r1, #238
	adds r0, #180
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #24
	adds r1, #184
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #0
	str r3, [r2]
	movs r2, #54
	b .L_081558ea
.L_081558c0:
	ldr r3, [sp, #44]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #180
	adds r2, r3, r4
	movs r3, #24
	str r3, [r2]
	ldr r7, [sp, #44]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #184
	adds r2, r7, r0
	movs r3, #0
	str r3, [r2]
	cmp r5, #3
	ble .L_081558e8
	movs r1, #84
	subs r5, #4
	str r1, [sp, #24]
	b .L_081558ec
.L_081558e8:
	movs r2, #55
.L_081558ea:
	str r2, [sp, #24]
.L_081558ec:
	cmp r5, #1
	beq .L_08155904
	cmp r5, #1
	bgt .L_081558fa
	cmp r5, #0
	beq .L_08155900
	b .L_0815590c
.L_081558fa:
	cmp r5, #2
	beq .L_08155908
	b .L_0815590c
.L_08155900:
	ldr r0, .L_08155b48
	b .L_0815590e
.L_08155904:
	ldr r0, .L_08155b4c
	b .L_0815590e
.L_08155908:
	ldr r0, .L_08155b50
	b .L_0815590e
.L_0815590c:
	ldr r0, .L_08155b54
.L_0815590e:
	bl Resource_GetTableEntry
	adds r2, r0, #0
	movs r0, #160
	adds r1, r2, #0
	ldr r3, .L_08155b58
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	cmp r5, #1
	beq .L_0815593a
	cmp r5, #1
	bgt .L_08155930
	cmp r5, #0
	beq .L_08155936
	b .L_08155942
.L_08155930:
	cmp r5, #2
	beq .L_0815593e
	b .L_08155942
.L_08155936:
	ldr r0, .L_08155b5c
	b .L_08155944
.L_0815593a:
	ldr r0, .L_08155b4c
	b .L_08155944
.L_0815593e:
	ldr r0, .L_08155b50
	b .L_08155944
.L_08155942:
	ldr r0, .L_08155b54
.L_08155944:
	bl Resource_GetTableEntry
	adds r2, r0, #0
	ldr r3, [sp, #44]
	movs r4, #224
	adds r2, #128
	lsls r4, r4, #3
	adds r1, r3, r4
	adds r0, r2, #0
	bl Resource_DecodeType01
	ldr r0, .L_08155b60
	ldr r1, [sp, #28]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	cmp r6, #1
	bne .L_0815597c
	movs r1, #7
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #23
	bl Func_081963ec
	b .L_0815598c
.L_0815597c:
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #19
	bl Func_081963ec
.L_0815598c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [sp, #56]
	ldr r5, [r3, #104]
	adds r3, #188
	ldr r3, [r3]
	ldr r0, [r7, #8]
	str r5, [sp, #32]
	str r3, [sp, #36]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r9, r0
	movs r1, #36
	ldrsh r0, [r7, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r7, [sp, #44]
	movs r2, #0
	mov r11, r0
	mov r8, r2
	mov r10, r2
.L_081559ba:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	mov r3, r10
	str r3, [r7]
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #20
	lsls r3, r3, #16
	mov r4, r10
	str r3, [r7, #4]
	str r4, [r7, #8]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	mov r0, r10
	asrs r3, r3, #5
	str r3, [r7, #12]
	str r0, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #1
	asrs r3, r3, #5
	add r8, r2
	str r3, [r7, #20]
	mov r1, r10
	mov r3, r8
	str r1, [r7, #24]
	adds r7, #28
	cmp r3, #64
	bne .L_081559ba
	ldr r4, [sp, #44]
	movs r5, #239
	movs r7, #238
	lsls r5, r5, #7
	lsls r7, r7, #7
	adds r2, r4, r5
	movs r3, #2
	adds r7, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r4, r7
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08155b64
	bl Scheduler_AddOrUpdateCallback
	mov r0, sp
	adds r0, #108
	str r0, [sp, #16]
	mov r1, r9
	ldr r3, [r1, #8]
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	ldr r3, [r1, #16]
	str r3, [r0, #8]
	ldr r2, [sp, #20]
	cmp r2, #10
	bls .L_08155a56
	b .L_08155b6c
.L_08155a56:
	lsls r3, r2, #2
	ldr r2, .L_08155b68
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08155a60:
	.4byte .L_08155a8c
	.4byte .L_08155aa8
	.4byte .L_08155ace
	.4byte .L_08155ae4
	.4byte .L_08155af2
	.4byte .L_08155b1a
	.4byte .L_08155af2
	.4byte .L_08155b06
	.4byte .L_08155aa4
	.4byte .L_08155ab6
	.4byte .L_08155b30
.L_08155a8c:
	add r3, sp, #96
	mov r4, r11
	mov r10, r3
	ldr r3, [r4, #8]
	mov r5, r10
	str r3, [r5]
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r5, #4]
	ldr r3, [r4, #16]
	str r3, [r5, #8]
	b .L_08155b70
.L_08155aa4:
	mov r0, r11
	b .L_08155ad0
.L_08155aa8:
	mov r2, r11
	ldr r3, [r2, #8]
	add r1, sp, #96
	str r3, [r1]
	movs r3, #240
	lsls r3, r3, #14
	b .L_08155b3c
.L_08155ab6:
	add r3, sp, #96
	mov r4, r11
	mov r10, r3
	ldr r3, [r4, #8]
	mov r5, r10
	str r3, [r5]
	movs r3, #240
	lsls r3, r3, #15
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	b .L_08155b70
.L_08155ace:
	mov r0, r9
.L_08155ad0:
	ldr r3, [r0, #8]
	add r7, sp, #96
	str r3, [r7]
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r7, #4]
	mov r10, r7
	ldr r3, [r0, #16]
	str r3, [r7, #8]
	b .L_08155b70
.L_08155ae4:
	mov r2, r9
	ldr r3, [r2, #8]
	add r1, sp, #96
	str r3, [r1]
	movs r3, #240
	lsls r3, r3, #14
	b .L_08155b3c
.L_08155af2:
	add r3, sp, #96
	movs r2, #0
	str r2, [r3]
	mov r10, r3
	movs r3, #240
	mov r4, r10
	lsls r3, r3, #14
	str r3, [r4, #4]
	str r2, [r4, #8]
	b .L_08155b70
.L_08155b06:
	movs r5, #96
	movs r3, #160
	add r5, sp
	movs r2, #0
	lsls r3, r3, #13
	mov r10, r5
	str r2, [r5]
	str r3, [r5, #4]
	str r2, [r5, #8]
	b .L_08155b70
.L_08155b1a:
	mov r0, r9
	ldr r3, [r0, #8]
	add r7, sp, #96
	str r3, [r7]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r7, #4]
	movs r3, #0
	mov r10, r7
	str r3, [r7, #8]
	b .L_08155b70
.L_08155b30:
	mov r2, r9
	ldr r3, [r2, #8]
	add r1, sp, #96
	str r3, [r1]
	movs r3, #240
	lsls r3, r3, #13
.L_08155b3c:
	str r3, [r1, #4]
	movs r3, #0
	mov r10, r1
	str r3, [r1, #8]
	b .L_08155b70
	.2byte 0x0000
.L_08155b48:
	.4byte 0x00000184
.L_08155b4c:
	.4byte 0x00000155
.L_08155b50:
	.4byte 0x00000151
.L_08155b54:
	.4byte 0x00000153
.L_08155b58:
	.4byte IwramCopyWords
.L_08155b5c:
	.4byte 0x00000157
.L_08155b60:
	.4byte 0x00000134
.L_08155b64:
	.4byte Func_08143000
.L_08155b68:
	.4byte .L_08155a60
.L_08155b6c:
	add r3, sp, #96
	mov r10, r3
.L_08155b70:
	mov r4, sp
	adds r4, #84
	ldr r7, [sp, #16]
	str r4, [sp, #12]
	mov r5, r10
	ldr r3, [r7]
	ldr r0, [r5]
	movs r1, #40
	subs r0, r0, r3
	bl Math_Div
	ldr r1, [sp, #12]
	str r0, [r1]
	ldr r3, [r7, #4]
	ldr r0, [r5, #4]
	movs r1, #40
	subs r0, r0, r3
	bl Math_Div
	ldr r2, [sp, #12]
	movs r1, #40
	str r0, [r2, #4]
	ldr r3, [r7, #8]
	ldr r0, [r5, #8]
	subs r0, r0, r3
	bl Math_Div
	ldr r3, [sp, #12]
	movs r4, #0
	str r0, [r3, #8]
	ldr r5, [sp, #24]
	mov r9, r4
	cmp r5, #0
	bne .L_08155bb6
	b .L_08155f3c
.L_08155bb6:
	movs r3, #192
	lsls r3, r3, #18
	mov r7, r9
	ldr r5, [r3, #48]
	cmp r7, #75
	ble .L_08155bd4
	ldr r3, .L_08155bec
	lsls r2, r7, #1
	subs r3, r3, r2
	ldr r2, .L_08155bf0
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
.L_08155bd4:
	ldr r0, [sp, #20]
	cmp r0, #6
	bne .L_08155bf4
	mov r1, r9
	cmp r1, #23
	ble .L_08155bf4
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164abc
	b .L_08155bf4
.L_08155bec:
	.4byte 0x000000a8
.L_08155bf0:
	.4byte 0x00001000
.L_08155bf4:
	mov r2, r9
	cmp r2, #8
	bne .L_08155c00
	movs r0, #212
	bl Audio_PlayCue
.L_08155c00:
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	mov r3, r9
	subs r3, #6
	cmp r3, #39
	bhi .L_08155c32
	ldr r4, [sp, #16]
	ldr r5, [sp, #12]
	ldr r3, [r4]
	ldr r2, [r5]
	adds r3, r3, r2
	str r3, [r4]
	ldr r3, [r4, #4]
	ldr r2, [r5, #4]
	adds r3, r3, r2
	str r3, [r4, #4]
	ldr r3, [r4, #8]
	ldr r2, [r5, #8]
	adds r3, r3, r2
	str r3, [r4, #8]
.L_08155c32:
	ldr r0, [sp, #16]
	mov r7, r9
	bl SceneTransform_ApplyPosition
	cmp r7, #0
	bne .L_08155c52
	ldr r1, [sp, #56]
	movs r2, #1
	movs r3, #1
	ldr r0, [r1, #8]
	negs r2, r2
	movs r1, #7
	negs r3, r3
	str r7, [sp, #0]
	bl Func_0814cd48
.L_08155c52:
	mov r2, r9
	cmp r2, #24
	bne .L_08155c6c
	ldr r3, [sp, #56]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_08155c6c:
	mov r4, r9
	cmp r4, #63
	ble .L_08155c74
	b .L_08155dee
.L_08155c74:
	negs r4, r4
	mov r0, r9
	str r4, [sp, #8]
	ldr r6, [sp, #44]
	movs r5, #0
	lsls r0, r0, #8
	mov r8, r5
	lsls r7, r4, #8
	mov r11, r0
.L_08155c86:
	mov r3, r8
	cmp r3, #0
	bge .L_08155c8e
	adds r3, #7
.L_08155c8e:
	asrs r3, r3, #3
	cmp r9, r3
	bge .L_08155c96
	b .L_08155dd4
.L_08155c96:
	ldr r3, [r6, #24]
	cmp r3, #0
	beq .L_08155c9e
	b .L_08155dd4
.L_08155c9e:
	bl Func_08014e38
	movs r3, #3
	mov r1, r8
	ands r3, r1
	cmp r3, #1
	beq .L_08155cc8
	cmp r3, #1
	bgt .L_08155cb6
	cmp r3, #0
	beq .L_08155cc0
	b .L_08155ce4
.L_08155cb6:
	cmp r3, #2
	beq .L_08155cd0
	cmp r3, #3
	beq .L_08155cd8
	b .L_08155ce4
.L_08155cc0:
	mov r0, r11
	bl Func_08015068
	b .L_08155ce4
.L_08155cc8:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	b .L_08155ce4
.L_08155cd0:
	adds r0, r7, #0
	bl Func_080150e4
	b .L_08155ce4
.L_08155cd8:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	adds r0, r7, #0
	bl Func_080150e4
.L_08155ce4:
	add r5, sp, #60
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	bl Func_08014ea8
	ldr r2, [r5, #8]
	cmp r2, #249
	bgt .L_08155d04
	movs r3, #250
	str r3, [r5, #8]
	movs r2, #250
.L_08155d04:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #122
	cmp r2, r3
	ble .L_08155d12
	str r3, [r5, #8]
	adds r2, r3, #0
.L_08155d12:
	adds r3, r2, #0
	subs r3, #250
	cmp r3, #0
	bge .L_08155d1c
	adds r3, #63
.L_08155d1c:
	asrs r3, r3, #6
	movs r0, #8
	subs r0, r0, r3
	ldr r2, .L_08155f80
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	lsrs r3, r0, #31
	adds r1, r2, r1
	ldr r2, [r5]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #40]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	mov r3, r8
	cmp r3, #0
	bge .L_08155d5a
	adds r3, #7
.L_08155d5a:
	asrs r3, r3, #3
	adds r3, #24
	cmp r9, r3
	blt .L_08155dd4
	ldr r3, [r6]
	ldr r2, [r6, #4]
	negs r3, r3
	asrs r5, r3, #7
	ldr r3, [r6, #8]
	negs r2, r2
	negs r3, r3
	asrs r4, r3, #7
	ldr r3, [r6, #16]
	ldr r1, [r6, #12]
	asrs r2, r2, #7
	adds r2, r3, r2
	ldr r3, [r6, #20]
	adds r1, r1, r5
	adds r0, r3, r4
	lsls r3, r1, #5
	subs r3, r3, r1
	lsls r3, r3, #1
	str r1, [r6, #12]
	str r2, [r6, #16]
	str r0, [r6, #20]
	cmp r3, #0
	bge .L_08155d92
	adds r3, #63
.L_08155d92:
	asrs r3, r3, #6
	str r3, [r6, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_08155da2
	adds r3, #63
.L_08155da2:
	asrs r3, r3, #6
	str r3, [r6, #16]
	lsls r3, r0, #5
	subs r3, r3, r0
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_08155db2
	adds r3, #63
.L_08155db2:
	movs r0, #224
	lsls r0, r0, #3
	movs r2, #240
	asrs r3, r3, #6
	adds r0, #255
	lsls r2, r2, #4
	str r3, [r6, #20]
	adds r2, #254
	adds r3, r5, r0
	cmp r3, r2
	bhi .L_08155dd4
	adds r3, r4, r0
	cmp r3, r2
	bhi .L_08155dd4
	movs r1, #1
	negs r1, r1
	str r1, [r6, #24]
.L_08155dd4:
	ldr r2, [sp, #8]
	movs r5, #1
	lsls r3, r2, #5
	mov r4, r9
	add r8, r5
	adds r7, r7, r3
	mov r0, r8
	lsls r3, r4, #5
	add r11, r3
	adds r6, #28
	cmp r0, #32
	beq .L_08155dee
	b .L_08155c86
.L_08155dee:
	mov r3, r9
	subs r3, #54
	cmp r3, #15
	bhi .L_08155e5a
	mov r1, r9
	lsls r5, r1, #10
	adds r0, r5, #0
	bl Trig_Sin
	movs r3, #0
	add r6, sp, #72
	lsls r0, r0, #2
	str r0, [r6]
	str r3, [r6, #4]
	str r3, [r6, #8]
	adds r0, r5, #0
	bl Trig_Sin
	mov r2, r10
	ldr r3, [r2]
	ldr r4, [sp, #48]
	lsls r0, r0, #2
	adds r3, r3, r0
	str r3, [r4]
	add r5, sp, #60
	ldr r3, [r2, #4]
	adds r1, r5, #0
	str r3, [r4, #4]
	adds r0, r6, #0
	ldr r3, [r2, #8]
	str r3, [r4, #8]
	bl Func_0815e1ec
	ldr r2, [r5]
	ldr r7, [sp, #52]
	movs r1, #20
	str r2, [r7]
	asrs r2, r2, #1
	ldr r3, [r5, #4]
	str r3, [r7, #4]
	str r2, [r5]
	ldr r4, [sp, #44]
	movs r5, #224
	str r1, [sp, #0]
	lsls r5, r5, #3
	movs r1, #40
	str r1, [sp, #4]
	subs r2, #10
	subs r3, #20
	ldr r0, [sp, #40]
	adds r1, r4, r5
	ldr r7, [sp, #32]
	mov lr, r7
	.2byte 0xf800
.L_08155e5a:
	mov r0, r9
	cmp r0, #64
	bne .L_08155ec0
	ldr r7, [sp, #44]
	movs r1, #0
	mov r8, r1
.L_08155e66:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	ldr r2, [sp, #52]
	movs r5, #255
	ldr r3, [r2]
	ands r5, r0
	lsls r3, r3, #15
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r2, #4]
	adds r5, #128
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #28
	cmp r4, #64
	bne .L_08155e66
.L_08155ec0:
	mov r5, r9
	cmp r5, #63
	ble .L_08155f1c
	ldr r6, .L_08155f80
	ldr r5, [sp, #44]
	movs r7, #0
	mov r8, r7
.L_08155ece:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08155f10
	asrs r0, r0, #3
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #28]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08155f10:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #64
	bne .L_08155ece
.L_08155f1c:
	ldr r1, [sp, #44]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #24]
	movs r4, #1
	add r9, r4
	cmp r9, r5
	beq .L_08155f3c
	b .L_08155bb6
.L_08155f3c:
	ldr r0, .L_08155f84
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #128
	ldr r5, .L_08155f88
	lsls r1, r1, #7
	ldr r0, .L_08155f8c
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	ldr r0, [sp, #40]
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	ldr r3, .L_08155f7c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08155f7c:
	.4byte 0x00001010
.L_08155f80:
	.4byte Data_08197410
.L_08155f84:
	.4byte Func_08143000
.L_08155f88:
	.4byte IwramClearWords
.L_08155f8c:
	.4byte 0x06004000
