.syntax unified
	.thumb
	.global Func_080e4a34
	.thumb_func
Func_080e4a34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #44
	movs r0, #92
	sub sp, #48
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	mov r9, r0
	str r3, [sp, #44]
	movs r6, #128
	ldr r2, [r2, #108]
	lsls r6, r6, #6
	str r2, [sp, #40]
	adds r6, #32
	ldr r0, [r3, #16]
	ldr r1, [r0, #12]
	mov r8, r0
	mov r2, r8
	adds r2, #85
	str r1, [sp, #24]
	str r2, [sp, #16]
	mov r5, r8
	ldrb r3, [r2]
	str r3, [sp, #20]
	ldr r3, [r5, #80]
	ldrh r3, [r3, #20]
	cmp r3, r6
	beq .L_080e4a86
	b .L_080e4d60
.L_080e4a86:
	bl Func_080dc954
	mov r0, r8
	movs r1, #1
	movs r2, #0
	bl Func_080dc164
	mov r3, r8
	adds r3, #34
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	ldrb r2, [r3]
	bl Func_080dbcd8
	cmp r0, #0
	bne .L_080e4ab4
	ldr r0, [sp, #40]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #72
	adds r2, r0, r1
	movs r3, #1
	b .L_080e4ac0
.L_080e4ab4:
	ldr r3, [sp, #40]
	movs r5, #208
	lsls r5, r5, #4
	adds r5, #72
	adds r2, r3, r5
	movs r3, #0
.L_080e4ac0:
	strh r3, [r2]
	bl BattleEffect_InitializeSharedScene
	ldr r0, .L_080e4d8c
	bl Resource_GetTableEntry
	mov r1, r9
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #4
	adds r1, r5, #0
	mov r2, r9
	str r0, [sp, #32]
	bl VramBlock_LoadCached
	movs r6, #144
	str r0, [sp, #28]
	lsls r6, r6, #5
	movs r4, #12
	movs r7, #0
	add r6, r9
	negs r4, r4
	add r5, r9
.L_080e4af4:
	ldr r0, [sp, #40]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #72
	adds r3, r0, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r0, r5, #0
	str r3, [sp, #0]
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r3, [sp, #28]
	str r4, [sp, #4]
	bl Func_080e45a8
	ldr r4, [sp, #4]
	adds r7, #1
	str r4, [r6, #24]
	adds r5, #40
	subs r4, #2
	adds r6, #28
	cmp r7, #63
	ble .L_080e4af4
	movs r2, #1
	negs r2, r2
	adds r0, r2, #0
	adds r1, r2, #0
	movs r3, #0
	bl Motion_CamBounds
	ldr r3, [sp, #44]
	mov r5, r8
	ldr r0, [r3, #16]
	movs r1, #0
	bl Func_080e1420
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	mov r0, r8
	bl Object_SetPositionAndResetMotionFar
	ldr r6, [sp, #16]
	movs r3, #0
	movs r0, #2
	strb r3, [r6]
	bl WaitFrames
	movs r0, #0
	mov r10, r0
.L_080e4b58:
	mov r1, r10
	cmp r1, #0
	ble .L_080e4b6c
	mov r2, r8
	ldr r3, [r2, #12]
	movs r5, #228
	lsls r5, r5, #6
	adds r5, #153
	adds r3, r3, r5
	str r3, [r2, #12]
.L_080e4b6c:
	movs r3, #31
	mov r6, r10
	ands r3, r6
	cmp r3, #0
	beq .L_080e4b80
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #54
	bl Audio_PlayCue
.L_080e4b80:
	mov r0, r8
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #6
	adds r3, r3, r1
	mov r2, r8
	strh r3, [r2, #6]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r10, r3
	mov r5, r10
	cmp r5, #39
	ble .L_080e4b58
	movs r3, #128
	lsls r3, r3, #7
	mov r6, r8
	strh r3, [r6, #6]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #80]
	movs r5, #128
	lsls r5, r5, #3
	ldr r0, [r3, #40]
	adds r2, r5, #0
	mov r1, r9
	bl Func_080e43a4
	bl Resource_FindFreeEntry
	adds r1, r5, #0
	movs r2, #0
	str r0, [sp, #36]
	bl VramBlock_LoadCached
	adds r5, r0, #0
	movs r0, #200
	lsls r0, r0, #5
	adds r1, r5, #0
	add r0, r9
	bl Func_080e4510
	movs r1, #128
	lsls r1, r1, #3
	lsls r5, r5, #5
	add r1, r9
	str r5, [sp, #8]
	str r1, [sp, #12]
	movs r0, #15
	movs r2, #45
	mov r10, r0
	mov r11, r2
.L_080e4bec:
	mov r3, r11
	lsrs r2, r3, #31
	add r2, r11
	ldr r1, [sp, #12]
	asrs r2, r2, #1
	mov r0, r9
	bl Func_080e446c
	ldr r6, .L_080e4d90
	ldr r5, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #84]
	ldr r0, [sp, #12]
	adds r3, r5, r6
	movs r1, #32
	movs r2, #32
	mov lr, r4
	.2byte 0xf800
	movs r0, #200
	lsls r0, r0, #5
	add r0, r9
	bl Func_080e45a0
	movs r6, #144
	movs r5, #128
	lsls r6, r6, #5
	lsls r5, r5, #4
	movs r7, #0
	add r6, r9
	add r5, r9
.L_080e4c2a:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r3, [sp, #28]
	adds r7, #1
	bl Func_080e46ac
	adds r5, #40
	adds r6, #28
	cmp r7, #32
	ble .L_080e4c2a
	mov r0, r10
	cmp r0, #16
	bne .L_080e4c50
	mov r1, r8
	ldr r0, [r1, #80]
	movs r1, #15
	bl Animation_ApplyChildValuesToRecordFar + 0x8
.L_080e4c50:
	mov r2, r10
	cmp r2, #45
	bne .L_080e4c5c
	movs r0, #136
	bl Audio_PlayCue
.L_080e4c5c:
	mov r3, r10
	cmp r3, #60
	bne .L_080e4cb4
	mov r5, r8
	mov r0, r8
	movs r1, #251
	bl Animation_SetIndexAndInitObjectsFar
	ldr r0, [r5, #80]
	movs r2, #63
	ldrb r1, [r0, #5]
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r0, #5]
	mov r0, r8
	ldr r1, [r5, #80]
	ldrb r3, [r1, #7]
	ands r2, r3
	movs r3, #64
	orrs r2, r3
	strb r2, [r1, #7]
	movs r1, #1
	bl Object_SetMode
	ldr r1, [sp, #44]
	movs r6, #24
	ldrsh r0, [r1, r6]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r3, [sp, #44]
	movs r1, #1
	movs r2, #24
	ldrsh r0, [r3, r2]
	bl Object_AttachWorkTargetToObject
	ldr r6, [sp, #24]
	add r0, sp, #20
	str r6, [r5, #12]
	ldrb r0, [r0]
	ldr r1, [sp, #16]
	strb r0, [r1]
.L_080e4cb4:
	mov r1, r10
	cmp r1, #61
	bne .L_080e4cc4
	mov r2, r8
	ldr r0, [r2, #80]
	movs r1, #0
	bl Animation_ApplyChildValuesToRecordFar + 0x8
.L_080e4cc4:
	movs r5, #1
	movs r0, #1
	add r10, r5
	bl WaitFrames
	mov r6, r10
	movs r3, #3
	add r11, r3
	cmp r6, #92
	ble .L_080e4bec
	ldr r0, [sp, #40]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e4cf8
	ldr r3, .L_080e4d94
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	movs r2, #9
	b .L_080e4d04
.L_080e4cf8:
	ldr r3, .L_080e4d94
	movs r5, #128
	lsls r5, r5, #2
	adds r5, #18
	adds r3, r3, r5
	movs r2, #5
.L_080e4d04:
	strb r2, [r3]
	mov r3, r8
	movs r2, #0
	adds r3, #100
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r6, [sp, #40]
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #74
	adds r2, r6, r0
	movs r3, #3
	strh r3, [r2]
	ldr r3, .L_080e4d94
	movs r1, #155
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	strb r2, [r3]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #72
	adds r3, r6, r2
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	bne .L_080e4d46
	movs r1, #144
	ldr r0, .L_080e4d98
	lsls r1, r1, #3
	bl Func_080145a8
.L_080e4d46:
	movs r1, #0
	movs r2, #16
	mov r0, r8
	bl Func_080dc164
	ldr r0, [sp, #36]
	bl Func_08014274
	ldr r0, [sp, #32]
	bl Func_08014274
	bl BattleFx_PrepareBufferInterpolation
.L_080e4d60:
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	movs r0, #208
	ldr r6, [sp, #40]
	lsls r0, r0, #4
	adds r0, #72
	adds r3, r6, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080e4d7c
	bl Func_080e4730
.L_080e4d7c:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e4d8c:
	.4byte 0x000001e1
.L_080e4d90:
	.4byte 0x06010000
.L_080e4d94:
	.4byte gPartyState
.L_080e4d98:
	.4byte Func_080e42d4
