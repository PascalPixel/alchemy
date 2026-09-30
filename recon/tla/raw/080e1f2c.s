.syntax unified
	.thumb
	.global Func_080e1f2c
	.thumb_func
Func_080e1f2c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #52
	movs r0, #92
	sub sp, #32
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	ldr r2, [r2, #108]
	adds r6, r0, #0
	mov r11, r3
	str r2, [sp, #16]
	adds r7, r6, #4
	bl BattleEffect_InitializeSharedScene
	movs r1, #0
.L_080e1f62:
	negs r3, r1
	adds r1, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #31
	ble .L_080e1f62
	movs r5, #192
	lsls r5, r5, #5
	adds r1, r5, #0
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	str r0, [r6]
	ldr r0, .L_080e2254
	bl Resource_GetTableEntry
	ldr r1, [r6]
	bl Func_0801587c
	bl Resource_FindFreeEntry
	str r0, [sp, #8]
	adds r1, r5, #0
	ldr r2, [r6]
	bl VramBlock_LoadCached
	movs r1, #133
	lsls r1, r1, #3
	movs r2, #249
	adds r3, r6, r1
	lsls r2, r2, #2
	adds r7, r6, r2
	strh r0, [r3]
	movs r3, #128
	str r0, [sp, #0]
	lsls r3, r3, #24
	adds r0, r7, #0
	movs r1, #16
	movs r2, #16
	bl Func_080eaf98
	ldrb r3, [r7, #5]
	ldrb r2, [r7, #9]
	movs r1, #15
	movs r0, #32
	mov r8, r1
	orrs r3, r0
	strb r3, [r7, #5]
	mov r3, r8
	ands r3, r2
	mov r2, r11
	strb r3, [r7, #9]
	ldr r0, [r2, #16]
	bl Func_080db9c0
	movs r1, #13
	ldrb r2, [r7, #9]
	negs r1, r1
	mov r9, r1
	movs r3, #3
	ands r0, r3
	mov r3, r9
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	mov r2, r11
	strb r3, [r7, #9]
	ldr r0, [r2, #16]
	bl Func_080db9cc
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	adds r0, #1
	strh r0, [r7, #30]
	str r3, [r7, #20]
	str r3, [r7, #24]
	mov r0, r11
	ldrh r3, [r0, #2]
	ldr r2, [r0, #16]
	strh r3, [r7, #28]
	add r5, sp, #20
	ldr r3, [r2, #8]
	movs r1, #242
	str r3, [r5]
	lsls r1, r1, #2
	ldr r3, [r2, #16]
	movs r0, #128
	str r3, [r5, #8]
	mov r2, r11
	adds r7, r6, r1
	lsls r0, r0, #12
	ldrh r1, [r2, #2]
	adds r2, r5, #0
	bl Func_0801489c
	ldr r3, [r5]
	ldr r0, .L_080e2258
	str r3, [r7]
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r7, #4]
	mov r10, r3
	ldr r3, [r5, #8]
	str r3, [r7, #8]
	bl Resource_GetTableEntry
	ldr r1, [r6]
	bl Func_0801587c
	bl Resource_FindFreeEntry
	str r0, [sp, #12]
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, [r6]
	bl VramBlock_LoadCached
	movs r1, #232
	lsls r1, r1, #2
	adds r7, r6, r1
	movs r3, #128
	str r0, [sp, #0]
	lsls r3, r3, #23
	adds r0, r7, #0
	movs r1, #12
	movs r2, #8
	bl Func_080eaf98
	ldrb r3, [r7, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r7, #5]
	ldrb r3, [r7, #9]
	mov r0, r8
	ands r0, r3
	mov r1, r11
	strb r0, [r7, #9]
	mov r8, r0
	ldr r0, [r1, #16]
	bl Func_080db9c0
	ldrb r3, [r7, #9]
	movs r2, #3
	mov r1, r9
	ands r0, r2
	adds r2, r1, #0
	ands r2, r3
	lsls r0, r0, #2
	orrs r2, r0
	mov r1, r11
	strb r2, [r7, #9]
	ldr r0, [r1, #16]
	bl Func_080db9cc
	movs r2, #0
	adds r0, #2
	strh r0, [r7, #30]
	str r2, [r7, #20]
	str r2, [r7, #24]
	mov r0, r11
	ldrh r3, [r0, #2]
	ldr r2, [r0, #16]
	strh r3, [r7, #28]
	movs r1, #225
	ldr r3, [r2, #8]
	lsls r1, r1, #2
	str r3, [r5]
	movs r0, #144
	ldr r3, [r2, #16]
	mov r2, r11
	str r3, [r5, #8]
	adds r7, r6, r1
	lsls r0, r0, #13
	ldrh r1, [r2, #2]
	adds r2, r5, #0
	bl Func_0801489c
	ldr r3, [r5]
	movs r0, #96
	str r3, [r7]
	mov r3, r10
	str r3, [r7, #4]
	ldr r3, [r5, #8]
	str r3, [r7, #8]
	bl Runtime_ReleaseHeapBlock
	bl Func_080eb824
	movs r3, #192
	mov r1, r11
	lsls r3, r3, #18
	adds r3, #240
	ldr r0, [r1, #16]
	ldr r5, [r3]
	bl Func_080db9c0
	adds r3, r5, #0
	adds r3, #191
	mov r2, r11
	strb r0, [r3]
	ldr r0, [r2, #16]
	bl Func_080db9cc
	adds r3, r5, #0
	adds r0, #3
	adds r3, #190
	adds r2, r5, #0
	strb r0, [r3]
	adds r2, #192
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	mov r1, r11
	adds r3, #193
	movs r0, #0
	strb r0, [r3]
	ldrh r3, [r1, #2]
	subs r2, #8
	str r3, [r2]
	ldr r0, [r1, #16]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #12
	adds r2, r6, r3
	ldr r3, [r0, #8]
	movs r1, #130
	str r3, [r2]
	lsls r1, r1, #3
	ldr r3, [r0, #12]
	adds r2, r6, r1
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #20
	adds r2, r6, r3
	ldr r3, [r0, #16]
	adds r1, #8
	str r3, [r2]
	adds r2, r6, r1
	ldr r3, [r0, #8]
	movs r5, #248
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #28
	adds r1, r6, r3
	ldr r3, [r0, #12]
	lsls r5, r5, #7
	str r3, [r1]
	movs r3, #132
	lsls r3, r3, #3
	adds r1, r6, r3
	ldr r3, [r0, #16]
	movs r0, #160
	str r3, [r1]
	mov r3, r11
	lsls r0, r0, #15
	ldrh r1, [r3, #2]
	bl Func_0801489c
	ldr r4, .L_080e225c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #2
.L_080e2174:
	adds r2, r1, #0
	adds r2, #16
	adds r3, r0, #0
	orrs r3, r5
	orrs r3, r2
	strh r3, [r4]
	adds r4, #2
	adds r1, #1
	adds r0, #32
	cmp r1, #15
	ble .L_080e2174
	movs r0, #130
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #3
	movs r1, #128
	adds r0, #44
	lsls r1, r1, #3
	movs r2, #0
	adds r3, r6, r0
	adds r1, #45
	strb r2, [r3]
	adds r0, #6
	adds r3, r6, r1
	strb r2, [r3]
	adds r3, r6, r0
	strb r2, [r3]
	movs r3, #134
	lsls r3, r3, #3
	adds r1, r6, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	movs r1, #128
	lsls r1, r1, #3
	subs r0, #14
	adds r1, #38
	adds r3, r6, r1
	adds r5, r6, r0
	strh r2, [r5]
	ldr r0, .L_080e2260
	strh r2, [r3]
	adds r1, #90
	bl Func_080145a8
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	beq .L_080e21f6
	mov r8, r2
.L_080e21e8:
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, r8
	bne .L_080e21e8
.L_080e21f6:
	ldr r0, .L_080e2260
	bl Func_08014644
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #50
	adds r3, r6, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e222c
	movs r2, #134
	lsls r2, r2, #3
	adds r3, r6, r2
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	beq .L_080e222c
	ldr r0, [sp, #16]
	movs r1, #211
	lsls r1, r1, #4
	adds r3, r0, r1
	strb r2, [r3]
.L_080e222c:
	ldr r0, [sp, #12]
	bl Func_08014274
	ldr r0, [sp, #8]
	bl Func_08014274
	bl BattleFx_PrepareBufferInterpolation
	bl Func_080eb930
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e2254:
	.4byte 0x000001e7
.L_080e2258:
	.4byte 0x000001e6
.L_080e225c:
	.4byte 0x050003c0
.L_080e2260:
	.4byte Func_080e1678
