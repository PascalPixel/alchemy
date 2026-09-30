.syntax unified
	.thumb
	.global Func_0802b998
	.thumb_func
Func_0802b998:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #193
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #0
	sub sp, #36
	bl Func_08013eb4
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #120
	movs r0, #32
	bl Runtime_AllocateHeapBlock
	movs r3, #0
	adds r7, r0, #0
	add r0, sp, #32
	mov r9, r3
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r7, #0
	ldr r2, .L_0802bcc0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r3, r7, #0
	mov r1, r9
	adds r3, #228
	str r1, [r3]
	adds r3, #4
	str r1, [r3]
	adds r2, r7, #0
	movs r3, #128
	adds r2, #236
	lsls r3, r3, #14
	str r3, [r2]
	movs r3, #128
	adds r2, #4
	lsls r3, r3, #15
	str r3, [r2]
	ldr r2, .L_0802bcc4
	adds r3, r7, #0
	adds r3, #244
	str r2, [r3]
	adds r3, #4
	str r2, [r3]
	str r1, [r7, #16]
	ldr r0, .L_0802bcc8
	bl Resource_GetTableEntry
	movs r2, #138
	lsls r2, r2, #1
	adds r3, r7, r2
	str r0, [r3]
	movs r2, #252
	movs r3, #128
	lsls r2, r2, #6
	lsls r3, r3, #19
	adds r2, #158
	adds r3, #80
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #16
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	mov r1, r9
	strh r1, [r3]
	ldr r0, .L_0802bccc
	bl Resource_GetTableEntry
	ldr r1, .L_0802bcd0
	bl Func_0801587c
	movs r3, #248
	lsls r3, r3, #5
	strh r3, [r7, #20]
	movs r3, #128
	strb r3, [r7, #22]
	ldr r0, .L_0802bcd4
	bl Resource_GetTableEntry
	ldr r1, .L_0802bcd8
	bl Func_0801587c
	movs r2, #168
	movs r3, #128
	lsls r2, r2, #8
	lsls r3, r3, #19
	adds r2, #10
	adds r3, #14
	strh r2, [r3]
	movs r2, #170
	lsls r2, r2, #8
	adds r2, #14
	subs r3, #2
	strh r2, [r3]
	movs r2, #133
	lsls r2, r2, #8
	adds r2, #1
	subs r3, #2
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #1
	adds r3, #22
	strh r2, [r3]
	adds r3, #2
	mov r1, r9
	strh r1, [r3]
	adds r3, #2
	strh r1, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	mov r1, r9
	str r1, [r3]
	adds r3, #4
	str r1, [r3]
	adds r3, #4
	strh r2, [r3]
	adds r3, #2
	mov r1, r9
	strh r1, [r3]
	adds r3, #2
	strh r1, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	mov r2, r9
	str r2, [r3]
	adds r3, #4
	str r2, [r3]
	movs r0, #48
	movs r1, #76
	bl Runtime_AllocateBlock
	movs r1, #208
	lsls r1, r1, #6
	mov r10, r0
	adds r1, #132
	movs r0, #28
	bl Runtime_AllocateHeapBlock
	movs r3, #200
	movs r1, #144
	lsls r3, r3, #4
	lsls r1, r1, #4
	movs r2, #150
	adds r3, r0, r3
	adds r1, #92
	movs r5, #255
	lsls r2, r2, #4
	str r3, [sp, #12]
	lsls r5, r5, #17
	adds r3, r7, r1
	adds r2, r7, r2
	str r0, [sp, #16]
	adds r1, #12
	str r5, [r3]
	str r2, [sp, #8]
	str r5, [r2]
	movs r2, #128
	adds r3, r7, r1
	lsls r2, r2, #9
	adds r1, #4
	str r2, [r3]
	adds r3, r7, r1
	mov r2, r9
	strh r2, [r3]
	ldr r2, .L_0802bcdc
	mov r3, r9
	mov r1, r10
	str r3, [r1, #24]
	str r3, [r1, #28]
	movs r3, #120
	str r3, [r2, #12]
	movs r3, #96
	asrs r1, r5, #1
	str r3, [r2, #16]
	adds r0, r5, #0
	lsls r2, r5, #1
	mov r6, r10
	bl Camera_StoreSceneParameters
	adds r6, #12
	mov r2, r9
	str r2, [r6]
	str r2, [r6, #4]
	str r2, [r6, #8]
	bl Func_08014de4
	adds r0, r6, #0
	bl SceneTransform_ApplyPosition
	movs r3, #143
	lsls r3, r3, #1
	adds r3, r7, r3
	str r3, [sp, #4]
	ldrh r0, [r3]
	bl Func_08015068
	movs r1, #142
	lsls r1, r1, #1
	adds r1, r1, r7
	ldrh r0, [r1]
	mov r8, r1
	bl SceneTransform_ApplyPitch
	add r2, sp, #20
	mov r3, r9
	mov r11, r2
	str r3, [r2]
	str r3, [r2, #4]
	mov r1, r10
	str r5, [r2, #8]
	mov r0, r11
	ldr r2, .L_0802bce0
	mov lr, r2
	.2byte 0xf800
	bl Func_08014de4
	mov r0, r10
	adds r1, r6, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r5, .L_0802bce4
	movs r0, #104
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	movs r3, #128
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r1, r0, #0
	adds r3, #212
	ldr r0, .L_0802bce8
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r8
	ldrh r0, [r3]
	bl Trig_Cos
	mov r1, r8
	adds r5, r0, #0
	ldrh r0, [r1]
	bl Trig_Sin
	ldr r3, .L_0802bcec
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r2, [sp, #16]
	bl Func_0802dd70
	ldr r3, .L_0802bcf0
	movs r5, #0
	str r5, [r3]
	mov r1, r8
	ldrh r3, [r1]
	ldr r2, .L_0802bcf4
	movs r1, #192
	str r3, [r2]
	ldr r3, .L_0802bcf8
	lsls r1, r1, #18
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #12]
	lsls r3, r3, #10
	adds r3, r2, r3
	str r3, [sp, #0]
	ldr r2, [sp, #16]
	ldr r4, [r1, #104]
	mov r0, r10
	adds r1, r6, #0
	mov lr, r4
	.2byte 0xf800
	str r5, [r6]
	str r5, [r6, #4]
	str r5, [r6, #8]
	bl Func_08014de4
	movs r3, #224
	mov r1, r8
	lsls r3, r3, #8
	strh r3, [r1]
	ldr r2, [sp, #4]
	strh r5, [r2]
	bl Func_08014de4
	adds r0, r6, #0
	bl SceneTransform_ApplyPosition
	ldr r3, [sp, #4]
	ldrh r0, [r3]
	bl Func_08015068
	mov r1, r8
	ldrh r0, [r1]
	bl SceneTransform_ApplyPitch
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #118
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0802bc4e
	mov r3, r11
	str r5, [r3]
	str r5, [r3, #4]
	ldr r1, [sp, #8]
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #9
	adds r3, r3, r2
	mov r1, r11
	str r3, [r1, #8]
	mov r0, r11
	mov r1, r10
	ldr r2, .L_0802bce0
	mov lr, r2
	.2byte 0xf800
.L_0802bc4e:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #76
	mov r1, r9
	strh r1, [r3]
	movs r3, #66
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_0802bcfc
	ldr r0, .L_0802bd00
	strh r1, [r3, #4]
	strh r1, [r3, #8]
	strh r1, [r3, #12]
	movs r1, #130
	mov r2, r9
	lsls r1, r1, #1
	strh r2, [r3, #6]
	strh r2, [r3, #10]
	strh r2, [r3, #14]
	adds r3, r7, r1
	strh r2, [r3]
	movs r3, #131
	lsls r3, r3, #1
	movs r1, #128
	adds r2, r7, r3
	lsls r1, r1, #3
	movs r3, #159
	strh r3, [r2]
	adds r1, #133
	bl Scheduler_AddOrUpdateCallback
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0802bd04
	bl Scheduler_AddOrUpdateCallback
	movs r2, #192
	movs r1, #164
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r3, #0
	adds r2, #255
	adds r0, r7, r1
.L_0802bca6:
	strh r3, [r0]
	adds r3, #1
	adds r0, #2
	cmp r3, r2
	ble .L_0802bca6
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802bcc0:
	.4byte 0x8500025e
.L_0802bcc4:
	.4byte 0x3fe00000
.L_0802bcc8:
	.4byte 0x00000197
.L_0802bccc:
	.4byte 0x00000198
.L_0802bcd0:
	.4byte gMapCellBuffer
.L_0802bcd4:
	.4byte 0x00000199
.L_0802bcd8:
	.4byte Data_0202e000
.L_0802bcdc:
	.4byte gCameraSceneParameters
.L_0802bce0:
	.4byte IwramTransformVector
.L_0802bce4:
	.4byte 0x00000298
.L_0802bce8:
	.4byte Data_0802146c
.L_0802bcec:
	.4byte IwramRatioMulQ14
.L_0802bcf0:
	.4byte Data_03001244
.L_0802bcf4:
	.4byte Data_03001144
.L_0802bcf8:
	.4byte Data_0300122c
.L_0802bcfc:
	.4byte Data_03001120
.L_0802bd00:
	.4byte Func_0802c240
.L_0802bd04:
	.4byte Func_0802c088
