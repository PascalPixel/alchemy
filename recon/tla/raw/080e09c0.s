.syntax unified
	.thumb
	.global Func_080e09c0
	.thumb_func
Func_080e09c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #20
	movs r0, #92
	sub sp, #16
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	ldr r2, [r2, #108]
	adds r3, #224
	ldr r3, [r3]
	str r2, [sp, #12]
	mov r9, r3
	ldr r5, [r3, #16]
	ldr r3, [r3, #20]
	mov r10, r0
	cmp r3, #0
	beq .L_080e0a08
	mov r0, r9
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
.L_080e0a08:
	bl BattleEffect_InitializeSharedScene
	movs r0, #108
	adds r0, #255
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_Spawn
	mov r8, r0
	cmp r0, #0
	bne .L_080e0a22
	b .L_080e0c60
.L_080e0a22:
	ldrh r3, [r5, #6]
	ldr r2, .L_080e0a60
	mov r1, r8
	strh r3, [r1, #6]
	mov r3, r8
	adds r3, #85
	strb r2, [r3]
	adds r3, r5, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080e0a48
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #32
	orrs r3, r2
	strb r3, [r1]
.L_080e0a48:
	movs r1, #0
	mov r0, r8
	bl Object_SetMode
	movs r0, #138
	bl Audio_PlayCue
	ldr r0, .L_080e0a64
	bl Resource_GetTableEntry
	b .L_080e0a68
	.2byte 0x0000
.L_080e0a60:
	.4byte 0x00000000
.L_080e0a64:
	.4byte 0x000001ea
.L_080e0a68:
	mov r1, r10
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #1
	adds r1, r5, #0
	mov r2, r10
	str r0, [sp, #8]
	bl VramBlock_LoadCached
	movs r3, #132
	lsls r3, r3, #6
	mov r11, r0
	adds r3, #12
	add r3, r10
	mov r2, r11
	strh r2, [r3]
	ldr r0, .L_080e0c78
	bl Resource_GetTableEntry
	mov r1, r10
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	adds r1, r5, #0
	mov r2, r10
	str r0, [sp, #4]
	bl VramBlock_LoadCached
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #14
	add r3, r10
	movs r6, #208
	movs r5, #128
	strh r0, [r3]
	lsls r6, r6, #5
	lsls r5, r5, #5
	add r6, r10
	add r5, r10
	movs r7, #63
.L_080e0ac0:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r1, [r5, #9]
	movs r4, #13
	negs r4, r4
	movs r3, #250
	strh r3, [r5, #30]
	adds r3, r4, #0
	ands r1, r3
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r5, #5]
	movs r3, #15
	ands r1, r3
	strb r1, [r5, #9]
	mov r0, r8
	ldr r3, [r0, #8]
	subs r7, #1
	str r3, [r6]
	adds r5, #40
	ldr r3, [r0, #12]
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	str r3, [r6, #8]
	bl Random16
	adds r1, r0, #0
	movs r0, #192
	adds r2, r6, #0
	lsls r0, r0, #12
	bl Vector_AddPolarOffset
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
	adds r6, #28
	cmp r7, #0
	bge .L_080e0ac0
	mov r2, r9
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	movs r3, #128
	mov r4, r9
	lsls r3, r3, #14
	adds r2, r2, r3
	mov r0, r8
	ldr r3, [r4, #12]
	bl Func_080dbed0
	mov r0, r8
	movs r1, #1
	bl Object_SetMode
	movs r0, #10
	bl WaitFrames
	movs r7, #15
.L_080e0b40:
	mov r0, r8
	ldr r3, [r0, #12]
	ldr r1, .L_080e0c7c
	subs r7, #1
	adds r3, r3, r1
	str r3, [r0, #12]
	movs r0, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_080e0b40
	movs r0, #132
	bl Audio_PlayCue
	mov r3, r9
	adds r3, #32
	movs r6, #0
	ldrsb r6, [r3, r6]
	cmp r6, #0
	beq .L_080e0b84
	movs r7, #15
.L_080e0b6a:
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #12]
	subs r7, #1
	ldr r2, [r2, #16]
	bl Func_080dc044
	cmp r7, #0
	bge .L_080e0b6a
	movs r0, #1
	bl WaitFrames
	b .L_080e0c4e
.L_080e0b84:
	movs r0, #6
	bl WaitFrames
	movs r3, #132
	lsls r3, r3, #6
	add r3, r10
	mov r4, r8
	str r4, [r3]
	movs r5, #132
	movs r3, #132
	lsls r5, r5, #6
	lsls r3, r3, #6
	adds r5, #4
	adds r3, #6
	add r5, r10
	add r3, r10
	strh r6, [r5]
	strh r6, [r3]
	movs r3, #132
	lsls r3, r3, #6
	movs r2, #132
	adds r3, #10
	lsls r2, r2, #6
	add r3, r10
	adds r2, #8
	strh r6, [r3]
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #18
	add r3, r10
	movs r1, #144
	strh r6, [r3]
	ldr r0, .L_080e0c80
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #186
	movs r0, #0
	ldrsh r3, [r5, r0]
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	beq .L_080e0c48
	mov r9, r2
.L_080e0be2:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #16
	add r3, r10
	movs r1, #0
	ldrsh r5, [r3, r1]
	cmp r5, #1
	bne .L_080e0c32
	movs r0, #128
	lsls r0, r0, #2
	bl Audio_PlayCue
	movs r4, #208
	ldr r2, [sp, #12]
	lsls r4, r4, #4
	movs r1, #208
	adds r4, #49
	lsls r1, r1, #4
	adds r3, r2, r4
	adds r1, #50
	movs r0, #0
	ldrsb r0, [r3, r0]
	adds r4, #2
	adds r3, r2, r1
	movs r1, #0
	ldrsb r1, [r3, r1]
	adds r3, r2, r4
	movs r2, #0
	ldrsb r2, [r3, r2]
	bl Func_080d92d4
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #18
	add r3, r10
	movs r0, #140
	strh r5, [r3]
	adds r0, #255
	bl Audio_PlayCue
.L_080e0c32:
	movs r0, #1
	bl WaitFrames
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #4
	add r3, r10
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, r9
	bne .L_080e0be2
.L_080e0c48:
	ldr r0, .L_080e0c80
	bl Scheduler_RemoveCallback
.L_080e0c4e:
	ldr r0, [sp, #8]
	bl Resource_ResetEntry
	ldr r0, [sp, #4]
	bl Resource_ResetEntry
	mov r0, r8
	bl Func_080200c8
.L_080e0c60:
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e0c78:
	.4byte 0x000001eb
.L_080e0c7c:
	.4byte 0xfffe0000
.L_080e0c80:
	.4byte Func_080e0618
