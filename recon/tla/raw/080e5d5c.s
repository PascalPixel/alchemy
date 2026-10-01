.syntax unified
	.thumb
	.global Func_080e5d5c
	.thumb_func
Func_080e5d5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #5
	adds r1, #180
	movs r0, #92
	sub sp, #44
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #28]
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	ldr r2, [r2, #108]
	adds r3, #224
	ldr r3, [r3]
	str r2, [sp, #24]
	movs r0, #128
	ldr r5, [r3, #20]
	lsls r0, r0, #14
	ldr r7, [r3, #16]
	mov r9, r3
	str r0, [sp, #4]
	cmp r5, #0
	bne .L_080e5db4
	ldr r3, [r7, #8]
	mov r1, r9
	str r3, [r1, #4]
	ldr r3, [r7, #12]
	mov r2, r9
	str r3, [r1, #8]
	ldr r3, [r7, #16]
	adds r2, #4
	str r3, [r1, #12]
	ldr r0, [sp, #4]
	ldrh r1, [r1]
	bl Vector_AddPolarOffset
	b .L_080e5dd6
.L_080e5db4:
	adds r3, r5, #0
	adds r3, #85
	ldrb r2, [r3]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080e5dd6
	ldr r3, [r5, #20]
	mov r4, r9
	str r3, [r4, #8]
	movs r0, #128
	ldr r3, [r5, #12]
	ldr r2, [r5, #20]
	lsls r0, r0, #14
	subs r3, r3, r2
	adds r0, r3, r0
	str r0, [sp, #4]
.L_080e5dd6:
	mov r3, r9
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e5e48
	mov r6, r9
	mov r2, r9
	adds r6, #4
	movs r1, #30
	ldrsh r0, [r2, r1]
	adds r1, r7, #0
	adds r2, r6, #0
	bl Func_080cda84
	mvns r0, r0
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	str r3, [sp, #8]
	cmp r3, #1
	bne .L_080e5e5e
	ldr r5, [sp, #24]
	movs r3, #0
	mov r10, r3
	adds r5, #20
.L_080e5e0c:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_080e5e3a
	ldr r3, [r1]
	cmp r3, #0
	beq .L_080e5e3a
	adds r3, r1, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080e5e3a
	ldrh r3, [r1, #32]
	adds r2, r1, #0
	adds r2, #8
	subs r3, #2
	adds r0, r6, #0
	movs r1, #4
	bl Func_080dbe80
	cmp r0, #0
	bge .L_080e5e5a
.L_080e5e3a:
	movs r4, #1
	add r10, r4
	mov r0, r10
	adds r5, #4
	cmp r0, #79
	ble .L_080e5e0c
	b .L_080e5e5e
.L_080e5e48:
	movs r1, #1
	str r1, [sp, #8]
	cmp r5, #0
	beq .L_080e5e5e
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080e5e5e
.L_080e5e5a:
	movs r2, #0
	str r2, [sp, #8]
.L_080e5e5e:
	bl BattleEffect_InitializeSharedScene
	movs r0, #138
	bl Audio_PlayCue
	movs r0, #176
	movs r1, #0
	movs r3, #0
	lsls r0, r0, #1
	movs r2, #0
	bl Object_Spawn
	mov r11, r0
	ldr r4, [sp, #28]
	movs r0, #192
	lsls r0, r0, #5
	adds r0, #168
	adds r3, r4, r0
	mov r1, r11
	str r1, [r3]
	cmp r1, #0
	bne .L_080e5e8c
	b .L_080e62d2
.L_080e5e8c:
	mov r2, r11
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r2, [sp, #24]
	movs r4, #208
	lsls r4, r4, #4
	adds r4, #76
	adds r3, r2, r4
	ldrh r2, [r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080e5ed6
	mov r3, r11
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_080e5ede
	mov r0, r11
	ldr r1, [r0, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	mov r1, r11
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	b .L_080e5ede
.L_080e5ed6:
	mov r2, r11
	adds r2, #35
	movs r3, #1
	strb r3, [r2]
.L_080e5ede:
	ldrh r3, [r7, #6]
	mov r2, r11
	movs r1, #0
	strh r3, [r2, #6]
	mov r0, r11
	mov r10, r1
	movs r1, #2
	bl Object_SetMode
	movs r0, #208
	ldr r4, [sp, #24]
	lsls r0, r0, #4
	adds r0, #76
	adds r3, r4, r0
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080e5f08
	bl Func_080dba5c
.L_080e5f08:
	bl Func_080eb824
	movs r1, #192
	lsls r1, r1, #18
	mov r8, r1
	mov r2, r9
	mov r3, r8
	adds r3, #240
	ldr r0, [r2, #16]
	ldr r7, [r3]
	bl Func_080db9c0
	adds r3, r7, #0
	adds r3, #191
	strb r0, [r3]
	ldr r3, .L_080e5f64
	adds r2, r7, #0
	adds r2, #190
	strb r3, [r2]
	adds r3, r7, #0
	movs r6, #2
	adds r3, #192
	strb r6, [r3]
	mov r4, r10
	subs r3, #16
	str r4, [r3]
	adds r3, #4
	str r4, [r3]
	mov r0, r9
	ldr r3, [r0, #4]
	add r5, sp, #32
	str r3, [r5]
	ldr r3, [r0, #8]
	str r3, [r5, #4]
	ldr r3, [r0, #12]
	adds r0, r5, #0
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	movs r1, #2
	ldrsh r3, [r5, r1]
	adds r2, r7, #0
	adds r2, #168
	str r3, [r2]
	adds r2, #4
	b .L_080e5f68
.L_080e5f64:
	.4byte 0x00000001
.L_080e5f68:
	movs r4, #10
	ldrsh r3, [r5, r4]
	ldr r0, .L_080e6194
	str r3, [r2]
	bl Resource_GetTableEntry
	mov r2, r8
	ldr r1, [r2, #96]
	bl Resource_DecodeType01
	adds r3, r7, #0
	adds r3, #193
	strb r6, [r3]
	ldr r3, .L_080e6198
	movs r1, #128
	ldr r0, [sp, #28]
	lsls r1, r1, #4
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	bl Resource_FindFreeEntry
	movs r1, #128
	ldr r2, [sp, #28]
	lsls r1, r1, #4
	str r0, [sp, #12]
	bl VramBlock_LoadCached
	ldr r3, [sp, #28]
	movs r4, #196
	lsls r4, r4, #5
	adds r6, r3, r4
	movs r3, #192
	str r0, [sp, #0]
	lsls r3, r3, #24
	adds r0, r6, #0
	movs r1, #32
	movs r2, #32
	bl Func_080eaf98
	ldrb r3, [r6, #9]
	movs r2, #13
	ldrb r1, [r6, #5]
	negs r2, r2
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	movs r3, #204
	lsls r3, r3, #8
	mov r0, r10
	adds r3, #204
	strb r2, [r6, #9]
	strh r0, [r6, #30]
	str r3, [r6, #20]
	str r3, [r6, #24]
	mov r1, r9
	ldr r3, [r1, #4]
	adds r0, r5, #0
	str r3, [r5]
	ldr r3, [r1, #8]
	str r3, [r5, #4]
	ldr r3, [r1, #12]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r3, [r5]
	ldr r0, .L_080e619c
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	bl Resource_GetTableEntry
	ldr r1, [sp, #28]
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, [sp, #28]
	str r0, [sp, #20]
	bl VramBlock_LoadCached
	ldr r3, [sp, #28]
	str r0, [sp, #16]
	movs r4, #128
	ldr r0, [sp, #28]
	lsls r4, r4, #5
	movs r1, #156
	movs r2, #0
	adds r3, r3, r4
	lsls r1, r1, #5
	mov r10, r2
	mov r8, r3
	adds r7, r0, r1
.L_080e602e:
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #3
	adds r0, r0, r3
	movs r1, #10
	lsls r0, r0, #1
	bl __udivsi3
	movs r2, #160
	lsls r2, r2, #10
	adds r6, r0, #0
	adds r6, r6, r2
	bl Random16
	movs r3, #168
	lsls r3, r3, #5
	mov r4, r9
	adds r3, #85
	mov r5, r10
	muls r5, r3
	ldr r3, [r4, #4]
	lsrs r0, r0, #5
	str r3, [sp, #32]
	adds r5, r5, r0
	ldr r3, [r4, #8]
	movs r0, #128
	str r3, [sp, #36]
	adds r1, r5, #0
	ldr r3, [r4, #12]
	lsls r0, r0, #11
	str r3, [sp, #40]
	add r3, sp, #32
	adds r2, r3, #0
	bl Vector_AddPolarOffset
	add r4, sp, #32
	adds r0, r4, #0
	bl Camera_WorldToScreen
	ldr r3, [sp, #32]
	mov r0, r8
	str r3, [r0]
	ldr r3, [sp, #40]
	str r3, [r0, #4]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, .L_080e61a0
	adds r1, r6, #0
	mov lr, r2
	.2byte 0xf800
	mov r3, r8
	str r0, [r3, #12]
	adds r0, r5, #0
	bl Trig_Sin
	ldr r4, .L_080e61a0
	adds r1, r6, #0
	mov lr, r4
	.2byte 0xf800
	mov r2, r10
	mov r1, r8
	negs r3, r2
	str r0, [r1, #16]
	str r3, [r1, #24]
	ldr r3, [sp, #16]
	adds r0, r7, #0
	str r3, [sp, #0]
	movs r3, #128
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_080eaf98
	ldrb r3, [r7, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r7, #9]
	movs r4, #13
	strb r3, [r7, #5]
	negs r4, r4
	movs r3, #15
	ands r3, r2
	adds r2, r4, #0
	ands r3, r2
	strb r3, [r7, #9]
	movs r3, #240
	strh r3, [r7, #30]
	movs r0, #1
	movs r3, #204
	lsls r3, r3, #8
	add r10, r0
	adds r3, #204
	movs r1, #28
	mov r2, r10
	str r3, [r7, #20]
	str r3, [r7, #24]
	add r8, r1
	adds r7, #40
	cmp r2, #31
	ble .L_080e602e
	bl WaitFrames
	mov r3, r9
	ldr r4, [sp, #4]
	ldr r2, [r3, #8]
	ldr r1, [r3, #4]
	adds r2, r2, r4
	ldr r3, [r3, #12]
	mov r0, r11
	bl Func_080dbed0
	mov r0, r11
	movs r1, #1
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r5, #24
.L_080e6120:
	mov r0, r11
	ldr r3, [r0, #12]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r3, r1
	str r3, [r0, #12]
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_080e6120
	movs r0, #15
	bl WaitFrames
	movs r3, #192
	ldr r2, [sp, #28]
	lsls r3, r3, #5
	adds r3, #172
	adds r6, r2, r3
	movs r2, #0
	strh r2, [r6]
	ldr r4, [sp, #28]
	movs r0, #192
	lsls r0, r0, #5
	adds r0, #174
	adds r3, r4, r0
	strh r2, [r3]
	movs r2, #192
	ldr r1, .L_080e6190
	lsls r2, r2, #5
	adds r2, #176
	adds r3, r4, r2
	strb r1, [r3]
	ldr r3, [sp, #8]
	cmp r3, #0
	bne .L_080e61e6
	movs r1, #144
	ldr r0, .L_080e61a4
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #186
	movs r4, #0
	ldrsh r3, [r6, r4]
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	beq .L_080e61b6
	ldr r0, [sp, #28]
	movs r1, #192
	lsls r1, r1, #5
	adds r1, #172
	adds r5, r0, r1
	mov r8, r2
	b .L_080e61a8
.L_080e6190:
	.4byte 0x00000000
.L_080e6194:
	.4byte 0x000001d9
.L_080e6198:
	.4byte IwramFillWords
.L_080e619c:
	.4byte 0x000001e4
.L_080e61a0:
	.4byte IwramMulQ16
.L_080e61a4:
	.4byte Func_080e5cf0
.L_080e61a8:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, r8
	bne .L_080e61a8
.L_080e61b6:
	ldr r0, .L_080e62ec
	bl Scheduler_RemoveCallback
	movs r3, #15
	mov r10, r3
.L_080e61c0:
	mov r4, r11
	ldr r1, [r4, #12]
	ldr r0, [r4, #8]
	ldr r2, [r4, #16]
	bl Func_080dc044
	movs r0, #1
	negs r0, r0
	add r10, r0
	mov r1, r10
	cmp r1, #0
	bge .L_080e61c0
	movs r0, #114
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	b .L_080e62bc
.L_080e61e6:
	mov r3, r9
	movs r2, #30
	ldrsh r0, [r3, r2]
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	mov r3, r9
	lsls r0, r0, #23
	movs r4, #30
	ldrsh r1, [r3, r4]
	adds r0, #5
	bl Func_080ce458
	movs r1, #144
	mov r8, r0
	lsls r1, r1, #3
	ldr r0, .L_080e62f0
	bl Scheduler_AddOrUpdateCallback
	movs r2, #186
	movs r4, #0
	ldrsh r3, [r6, r4]
	lsls r2, r2, #2
	adds r2, #255
	movs r5, #0
	cmp r3, r2
	beq .L_080e629a
	ldr r0, [sp, #28]
	movs r1, #192
	lsls r1, r1, #5
	adds r1, #177
	adds r7, r0, r1
	mov r10, r2
.L_080e622a:
	movs r0, #1
	bl WaitFrames
	movs r3, #224
	movs r2, #0
	ldrsb r2, [r7, r2]
	lsls r3, r3, #3
	adds r3, #196
	add r3, r9
	strh r2, [r3]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #198
	add r3, r9
	strh r5, [r3]
	ldr r2, [sp, #24]
	movs r4, #197
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080e627a
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #76
	adds r3, r2, r0
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080e627a
	ldr r1, [sp, #28]
	movs r2, #196
	lsls r2, r2, #5
	adds r6, r1, r2
	adds r0, r6, #0
	bl Func_080eb01c
.L_080e627a:
	movs r1, #0
	ldrsb r1, [r7, r1]
	adds r2, r5, #0
	mov r0, r8
	bl Func_080ceafc
	movs r0, #192
	ldr r4, [sp, #28]
	lsls r0, r0, #5
	adds r0, #172
	adds r3, r4, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r5, #1
	cmp r3, r10
	bne .L_080e622a
.L_080e629a:
	ldr r0, .L_080e62f0
	bl Scheduler_RemoveCallback
	movs r0, #10
	bl WaitFrames
	movs r0, #114
	bl Audio_PlayCue
	mov r2, r11
	mov r4, r11
	ldr r1, [r2, #8]
	ldr r3, [r4, #16]
	ldr r2, [r2, #12]
	mov r0, r11
	bl Func_080dbf94
.L_080e62bc:
	mov r0, r11
	bl Object_Destroy
	ldr r0, [sp, #20]
	bl Resource_ResetEntry
	ldr r0, [sp, #12]
	bl Resource_ResetEntry
	bl Func_080eb930
.L_080e62d2:
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e62ec:
	.4byte Func_080e5cf0
.L_080e62f0:
	.4byte Func_080e5a64
