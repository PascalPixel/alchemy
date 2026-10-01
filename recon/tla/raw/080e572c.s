.syntax unified
	.thumb
	.global Func_080e572c
	.thumb_func
Func_080e572c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #16
	movs r0, #92
	sub sp, #40
	bl Runtime_AllocateHeapBlock
	movs r5, #192
	lsls r5, r5, #18
	adds r3, r5, #0
	adds r3, #224
	ldr r3, [r3]
	mov r9, r0
	str r3, [sp, #24]
	ldr r1, [sp, #24]
	ldr r3, [r3, #8]
	movs r0, #160
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r1, #8]
	bl BattleEffect_InitializeSharedScene
	bl Func_080eb824
	adds r5, #240
	ldr r5, [r5]
	ldr r2, [sp, #24]
	str r5, [sp, #20]
	add r5, sp, #28
	ldr r3, [r2, #4]
	adds r0, r5, #0
	str r3, [r5]
	ldr r3, [r2, #8]
	str r3, [r5, #4]
	ldr r3, [r2, #12]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r2, [sp, #20]
	movs r0, #2
	ldrsh r3, [r5, r0]
	adds r2, #168
	str r3, [r2]
	ldr r2, [sp, #20]
	movs r1, #10
	ldrsh r3, [r5, r1]
	adds r2, #172
	str r3, [r2]
	ldr r2, [sp, #24]
	movs r5, #0
	ldrh r1, [r2]
	ldr r0, [r2, #16]
	bl Func_080db9a8
	ldr r3, [sp, #20]
	adds r3, #191
	str r3, [sp, #8]
	strb r0, [r3]
	ldr r1, [sp, #24]
	ldr r0, [r1, #16]
	bl Func_080db9cc
	ldr r2, [sp, #20]
	adds r0, #2
	adds r2, #190
	str r2, [sp, #4]
	strb r0, [r2]
	ldr r2, [sp, #20]
	movs r3, #2
	adds r2, #192
	strb r3, [r2]
	ldr r3, [sp, #20]
	movs r1, #192
	adds r3, #176
	str r5, [r3]
	ldr r3, [sp, #20]
	lsls r1, r1, #8
	adds r3, #180
	str r5, [r3]
	ldr r0, [sp, #24]
	ldrh r3, [r0]
	cmp r3, r1
	bne .L_080e57e8
	ldr r2, [sp, #4]
	ldrb r3, [r2]
	adds r3, #252
	strb r3, [r2]
.L_080e57e8:
	bl Resource_FindFreeEntry
	movs r1, #64
	ldr r2, .L_080e5a50
	str r0, [sp, #16]
	bl VramBlock_LoadCached
	ldr r1, .L_080e5a54
	str r0, [sp, #12]
	movs r0, #128
	lsls r0, r0, #3
	movs r7, #240
	movs r3, #0
	add r0, r9
	lsls r7, r7, #3
	mov r10, r3
	mov r8, r0
	mov r11, r1
	add r7, r9
.L_080e580e:
	ldr r2, [sp, #12]
	adds r0, r7, #0
	str r2, [sp, #0]
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r3, [r7, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r7, #9]
	strb r3, [r7, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r7, #9]
	ldr r0, [sp, #8]
	movs r1, #3
	ldrb r2, [r0]
	movs r0, #13
	negs r0, r0
	ands r2, r1
	adds r1, r0, #0
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #9]
	ldr r1, [sp, #4]
	ldrb r3, [r1]
	adds r3, #1
	strh r3, [r7, #30]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #20]
	str r3, [r7, #24]
	bl Random16
	lsls r5, r0, #2
	adds r5, r5, r0
	movs r2, #160
	lsls r2, r2, #13
	lsls r5, r5, #2
	adds r5, r5, r2
	bl Random16
	adds r6, r0, #0
	bl Trig_Cos
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	mov r3, r8
	str r0, [r3]
	adds r0, r6, #0
	bl Trig_Sin
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	mov r1, r8
	str r0, [r1, #4]
	movs r1, #12
	adds r0, r5, #0
	bl __divsi3
	movs r2, #128
	lsls r2, r2, #8
	adds r6, r6, r2
	adds r5, r0, #0
	adds r0, r6, #0
	bl Trig_Cos
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	mov r3, r8
	str r0, [r3, #12]
	adds r0, r6, #0
	bl Trig_Sin
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	mov r2, r10
	negs r3, r2
	mov r1, r8
	lsls r3, r3, #1
	str r3, [r1, #24]
	movs r3, #1
	add r10, r3
	str r0, [r1, #16]
	movs r0, #28
	mov r1, r10
	adds r7, #40
	add r8, r0
	cmp r1, #31
	ble .L_080e580e
	ldr r0, .L_080e5a58
	bl Resource_GetTableEntry
	mov r1, r9
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	lsls r1, r1, #3
	mov r11, r0
	bl VramBlock_LoadCached
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #6
	movs r2, #140
	add r3, r9
	adds r6, r0, #0
	lsls r2, r2, #5
	movs r5, #200
	strh r6, [r3]
	add r2, r9
	lsls r5, r5, #4
	movs r3, #31
	mov r8, r2
	add r5, r9
	movs r7, #3
	mov r10, r3
.L_080e590c:
	movs r3, #128
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	str r6, [sp, #0]
	bl Func_080eaf98
	ldr r0, [sp, #4]
	ldrb r3, [r0]
	movs r0, #13
	adds r3, #1
	strh r3, [r5, #30]
	ldr r1, [sp, #8]
	negs r0, r0
	ldrb r2, [r1]
	ldrb r1, [r5, #9]
	adds r3, r0, #0
	ands r1, r3
	ands r2, r7
	ldrb r3, [r5, #5]
	lsls r2, r2, #2
	subs r0, #20
	orrs r1, r2
	adds r2, r0, #0
	ands r3, r2
	strb r3, [r5, #5]
	movs r3, #240
	orrs r1, r3
	movs r3, #1
	negs r3, r3
	strb r1, [r5, #9]
	add r10, r3
	mov r1, r8
	str r3, [r1, #24]
	movs r2, #28
	mov r3, r10
	adds r5, #40
	add r8, r2
	cmp r3, #0
	bge .L_080e590c
	ldr r0, .L_080e5a5c
	bl Resource_GetTableEntry
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	bl Resource_DecodeType01
	ldr r2, [sp, #20]
	movs r3, #2
	adds r2, #193
	strb r3, [r2]
	movs r0, #130
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	movs r3, #168
	movs r6, #168
	lsls r3, r3, #5
	movs r2, #168
	lsls r6, r6, #5
	adds r3, #2
	lsls r2, r2, #5
	movs r5, #0
	add r6, r9
	add r3, r9
	adds r2, #12
	strh r5, [r6]
	add r2, r9
	strh r5, [r3]
	movs r3, #1
	strb r3, [r2]
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #8
	add r3, r9
	strh r5, [r3]
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #10
	add r3, r9
	movs r1, #144
	strh r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_080e5a60
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #24]
	movs r0, #30
	ldrsh r5, [r1, r0]
	adds r0, r5, #0
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	lsls r0, r0, #23
	adds r1, r5, #0
	adds r0, #5
	bl Func_080ce458
	movs r2, #0
	ldrsh r3, [r6, r2]
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	adds r7, r0, #0
	movs r5, #0
	cmp r3, r2
	beq .L_080e5a22
	movs r6, #168
	lsls r6, r6, #5
	add r6, r9
	mov r8, r2
.L_080e59f4:
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	ldrsh r3, [r6, r0]
	cmp r3, r8
	bne .L_080e5a04
	mov r5, r8
.L_080e5a04:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #4
	add r3, r9
	movs r2, #0
	ldrsh r1, [r3, r2]
	adds r0, r7, #0
	adds r2, r5, #0
	bl Func_080ceafc
	movs r0, #0
	ldrsh r3, [r6, r0]
	adds r5, #1
	cmp r3, r8
	bne .L_080e59f4
.L_080e5a22:
	ldr r0, .L_080e5a60
	bl Scheduler_RemoveCallback
	ldr r0, [sp, #16]
	bl Resource_ResetEntry
	mov r0, r11
	bl Resource_ResetEntry
	bl BattleFx_PrepareBufferInterpolation
	bl Func_080eb930
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e5a50:
	.4byte Data_080f3984
.L_080e5a54:
	.4byte IwramMulQ16
.L_080e5a58:
	.4byte 0x000001e8
.L_080e5a5c:
	.4byte 0x000001d9
.L_080e5a60:
	.4byte Func_080e4d9c
