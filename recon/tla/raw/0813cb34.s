.syntax unified
	.thumb
	.global Func_0813cb34
	.thumb_func
Func_0813cb34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r10, r1
	ldr r1, [r3, #92]
	sub sp, #68
	str r1, [sp, #40]
	mov r1, r10
	ldr r2, [r3, #96]
	mov r9, r0
	str r2, [sp, #36]
	ldr r4, [r3, #48]
	str r4, [sp, #24]
	ldr r3, [r3, #100]
	str r3, [sp, #20]
	cmp r1, #0
	bne .L_0813cb6a
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	b .L_0813cb70
.L_0813cb6a:
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
.L_0813cb70:
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r2, [r5, #104]
	movs r1, #27
	movs r0, #188
	str r2, [sp, #28]
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	movs r3, #0
	ldr r0, .L_0813ce84
	ldr r1, [sp, #20]
	movs r2, #0
	str r5, [sp, #32]
	bl Func_08157cf4
	mov r3, r10
	cmp r3, #0
	bne .L_0813cba4
	ldr r0, .L_0813ce88
	b .L_0813cbb0
.L_0813cba4:
	mov r4, r10
	cmp r4, #1
	bne .L_0813cbae
	ldr r0, .L_0813ce8c
	b .L_0813cbb0
.L_0813cbae:
	ldr r0, .L_0813ce90
.L_0813cbb0:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0813ce94
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #40]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0813ce98
	bl Scheduler_AddOrUpdateCallback
	mov r1, r9
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r6, .L_0813ce9c
	str r0, [sp, #12]
	movs r2, #0
	mov r8, r2
.L_0813cbf6:
	mov r3, r10
	cmp r3, #2
	bne .L_0813cc4e
	mov r4, r9
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0813cc18
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #8
	adds r5, r3, r1
	b .L_0813cc26
.L_0813cc18:
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r5, r0, #0
	adds r3, #255
	ands r5, r3
.L_0813cc26:
	bl Random16
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	movs r2, #128
	ands r3, r0
	lsls r2, r2, #4
	adds r7, r3, r2
	bl Random16
	ldr r4, [sp, #12]
	lsls r0, r0, #4
	ldr r3, [r4, #12]
	movs r1, #160
	adds r3, r3, r0
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r6, #4]
	b .L_0813cc70
.L_0813cc4e:
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r3, #192
	ldr r2, [sp, #12]
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r0
	adds r7, r3, #0
	ldr r3, [r2, #12]
	movs r4, #160
	lsls r4, r4, #11
	adds r3, r3, r4
	str r3, [r6, #4]
	adds r7, #32
.L_0813cc70:
	ldr r1, [sp, #12]
	adds r0, r5, #0
	ldr r3, [r1, #8]
	str r3, [r6]
	ldr r3, [r1, #16]
	str r3, [r6, #8]
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	mov r2, r10
	asrs r3, r3, #8
	str r3, [r6, #12]
	cmp r2, #2
	bne .L_0813ccac
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	lsls r3, r3, #10
	str r3, [r6, #16]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #9
	b .L_0813ccca
.L_0813ccac:
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #9
	str r3, [r6, #16]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #8
.L_0813ccca:
	str r3, [r6, #20]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #48
	str r3, [r6, #24]
	mov r3, r10
	cmp r3, #0
	bne .L_0813ccf2
	ldr r3, [r6, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #20]
.L_0813ccf2:
	movs r4, #1
	movs r1, #128
	add r8, r4
	lsls r1, r1, #1
	adds r6, #28
	cmp r8, r1
	beq .L_0813cd02
	b .L_0813cbf6
.L_0813cd02:
	movs r2, #96
	mov r3, r10
	str r2, [sp, #16]
	cmp r3, #2
	beq .L_0813cd10
	movs r4, #128
	str r4, [sp, #16]
.L_0813cd10:
	ldr r2, [sp, #16]
	movs r1, #0
	mov r11, r1
	cmp r2, #0
	bne .L_0813cd1c
	b .L_0813cf20
.L_0813cd1c:
	ldr r3, [sp, #24]
	adds r3, #12
	str r3, [sp, #8]
.L_0813cd22:
	bl Func_08014de4
	ldr r0, [sp, #24]
	ldr r1, [sp, #8]
	bl Graphics_PrepareTransferInIwramWork
	mov r4, r10
	cmp r4, #2
	bne .L_0813cd40
	mov r1, r11
	cmp r1, #0
	bne .L_0813cd40
	movs r0, #103
	bl Audio_PlayCue
.L_0813cd40:
	ldr r6, .L_0813ce9c
	movs r2, #0
	mov r8, r2
	add r7, sp, #56
.L_0813cd48:
	mov r0, r8
	movs r1, #32
	bl Math_Div
	lsls r0, r0, #3
	cmp r11, r0
	blt .L_0813ce30
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_0813ce30
	mov r4, r8
	lsls r0, r4, #2
	adds r0, r0, r3
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r3, [r6]
	lsls r0, r0, #4
	adds r3, r3, r0
	str r3, [r7]
	add r5, sp, #44
	ldr r3, [r6, #4]
	adds r1, r5, #0
	str r3, [r7, #4]
	adds r0, r7, #0
	ldr r3, [r6, #8]
	str r3, [r7, #8]
	bl Func_0815e1ec
	ldr r3, [r5]
	movs r1, #58
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #8]
	adds r1, #255
	cmp r3, r1
	bgt .L_0813cd98
	movs r3, #157
	lsls r3, r3, #1
	str r3, [r5, #8]
.L_0813cd98:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #122
	cmp r3, r2
	ble .L_0813cda6
	str r2, [r5, #8]
	adds r3, r2, #0
.L_0813cda6:
	ldr r4, .L_0813cea0
	adds r2, r3, r4
	cmp r2, #0
	bge .L_0813cdb2
	adds r2, r3, #0
	subs r2, #251
.L_0813cdb2:
	asrs r3, r2, #6
	movs r0, #6
	subs r0, r0, r3
	ldr r2, .L_0813cea4
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
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
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	mov r1, r10
	cmp r1, #2
	bne .L_0813cdfe
	ldr r2, .L_0813cea8
	adds r0, r6, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r6, #4]
	ldr r2, .L_0813ceac
	cmp r3, r2
	bgt .L_0813ce0a
	ldr r3, [r6, #16]
	negs r3, r3
	str r3, [r6, #16]
	b .L_0813ce0a
.L_0813cdfe:
	movs r2, #128
	adds r0, r6, #0
	movs r1, #62
	lsls r2, r2, #3
	bl BattleFxKernels_IntegrateVector3
.L_0813ce0a:
	mov r3, r10
	cmp r3, #1
	bne .L_0813ce2a
	ldr r4, [sp, #12]
	ldr r3, [r4, #8]
	cmp r3, #0
	bge .L_0813ce22
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #6
	adds r3, r3, r1
	b .L_0813ce28
.L_0813ce22:
	ldr r3, [r6, #12]
	ldr r2, .L_0813cea8
	adds r3, r3, r2
.L_0813ce28:
	str r3, [r6, #12]
.L_0813ce2a:
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_0813ce30:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r6, #28
	cmp r4, #128
	bne .L_0813cd48
	mov r1, r10
	cmp r1, #1
	bne .L_0813ceb0
	mov r4, r9
	ldr r3, [r4, #20]
	movs r2, #0
	mov r8, r2
	cmp r3, #0
	beq .L_0813cefc
	movs r6, #36
	movs r5, #48
.L_0813ce52:
	cmp r11, r5
	bne .L_0813ce74
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
	mov r1, r9
	movs r3, #8
	ldrsh r0, [r6, r1]
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	movs r2, #5
	bl Func_0814cd48
	mov r4, r9
	ldr r3, [r4, #20]
.L_0813ce74:
	movs r1, #1
	add r8, r1
	adds r6, #2
	adds r5, #8
	cmp r8, r3
	bne .L_0813ce52
	b .L_0813cefc
	.2byte 0x0000
.L_0813ce84:
	.4byte 0x00000134
.L_0813ce88:
	.4byte 0x0000013d
.L_0813ce8c:
	.4byte 0x0000013c
.L_0813ce90:
	.4byte 0x00000138
.L_0813ce94:
	.4byte IwramCopyWords
.L_0813ce98:
	.4byte Func_08143000
.L_0813ce9c:
	.4byte gMapCellBuffer
.L_0813cea0:
	.4byte 0xfffffec6
.L_0813cea4:
	.4byte Data_08197410
.L_0813cea8:
	.4byte 0xffffe000
.L_0813ceac:
	.4byte 0x0004ffff
.L_0813ceb0:
	mov r2, r10
	cmp r2, #0
	bne .L_0813cefc
	movs r3, #0
	mov r4, r9
	mov r8, r3
	ldr r3, [r4, #20]
	cmp r3, #0
	beq .L_0813cefc
	movs r7, #1
	negs r7, r7
	movs r6, #36
	movs r5, #48
.L_0813ceca:
	cmp r11, r5
	bne .L_0813cef0
	movs r0, #126
	bl Audio_PlayCue
	adds r0, r7, #0
	bl Func_081180e8
	mov r1, r9
	movs r3, #8
	ldrsh r0, [r6, r1]
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	adds r2, r7, #0
	bl Func_0814cd48
	mov r4, r9
	ldr r3, [r4, #20]
.L_0813cef0:
	movs r1, #1
	add r8, r1
	adds r6, #2
	adds r5, #8
	cmp r8, r3
	bne .L_0813ceca
.L_0813cefc:
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #40]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #16]
	movs r1, #1
	add r11, r1
	cmp r11, r2
	beq .L_0813cf20
	b .L_0813cd22
.L_0813cf20:
	ldr r0, .L_0813cf70
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	mov r3, r10
	cmp r3, #2
	bne .L_0813cf5c
	movs r1, #240
	ldr r5, .L_0813cf74
	lsls r1, r1, #6
	ldr r0, .L_0813cf78
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #36]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0813cf7c
	bl Scheduler_RemoveCallback
	mov r0, r9
	bl Func_081504c0
	b .L_0813cf60
.L_0813cf5c:
	bl Func_08143bb8
.L_0813cf60:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813cf70:
	.4byte Func_08143000
.L_0813cf74:
	.4byte IwramClearWords
.L_0813cf78:
	.4byte 0x06004000
.L_0813cf7c:
	.4byte Func_08143488
