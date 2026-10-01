.syntax unified
	.thumb
	.global Func_080e82cc
	.thumb_func
Func_080e82cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #164
	lsls r1, r1, #6
	adds r1, #140
	movs r0, #92
	sub sp, #64
	bl Runtime_AllocateBlock
	movs r2, #192
	str r0, [sp, #48]
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	str r3, [sp, #44]
	ldr r2, [r2, #108]
	str r2, [sp, #40]
	ldr r4, [sp, #40]
	ldr r0, [r3, #16]
	movs r2, #1
	str r0, [sp, #36]
	movs r0, #192
	str r1, [sp, #20]
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r1, [sp, #8]
	lsls r0, r0, #4
	adds r0, #172
	adds r3, r4, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e8320
	b .L_080e8768
.L_080e8320:
	bl BattleEffect_InitializeSharedScene
	add r1, sp, #16
	ldr r3, .L_080e83b0
	ldrb r1, [r1]
	strb r1, [r3]
	bl Func_080eb824
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #240
	ldr r3, [r3]
	add r2, sp, #16
	str r3, [sp, #32]
	adds r3, #192
	ldrb r2, [r2]
	movs r1, #128
	strb r2, [r3]
	mov r3, sp
	adds r3, #52
	str r3, [sp, #4]
	ldr r4, [sp, #44]
	ldr r0, [sp, #4]
	ldr r2, [r4, #16]
	lsls r1, r1, #13
	ldr r3, [r2, #8]
	str r3, [r0]
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r0, #4]
	ldr r3, [r2, #16]
	str r3, [r0, #8]
	ldr r0, [sp, #4]
	bl Camera_WorldToScreen
	ldr r4, [sp, #4]
	ldr r1, .L_080e83b4
	movs r2, #2
	ldrsh r3, [r4, r2]
	ldr r2, .L_080e83ac
	subs r3, r2, r3
	strh r3, [r1]
	movs r0, #10
	ldrsh r3, [r4, r0]
	subs r2, r2, r3
	strh r2, [r1, #2]
	ldr r6, .L_080e83b8
	movs r1, #128
	lsls r1, r1, #4
	movs r2, #0
	ldr r0, .L_080e83bc
	mov lr, r6
	.2byte 0xf800
	movs r0, #96
	movs r5, #128
	lsls r5, r5, #6
	bl Runtime_ReleaseHeapBlock
	adds r1, r5, #0
	movs r0, #96
	bl Runtime_AllocateBlock
	mov r9, r0
	adds r1, r5, #0
	movs r2, #0
	mov lr, r6
	.2byte 0xf800
	movs r0, #1
	b .L_080e83c0
	.2byte 0x0000
.L_080e83ac:
	.4byte 0x00000040
.L_080e83b0:
	.4byte Data_0300123c
.L_080e83b4:
	.4byte Data_03001120
.L_080e83b8:
	.4byte IwramFillWords
.L_080e83bc:
	.4byte 0x06002000
.L_080e83c0:
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	mov r0, r9
	lsls r1, r1, #19
	ldr r2, .L_080e84d4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #144
	lsls r2, r2, #3
	subs r3, #204
	strh r2, [r3]
	ldr r0, .L_080e84d8
	bl Resource_GetTableEntry
	ldr r1, [sp, #48]
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, [sp, #48]
	str r0, [sp, #28]
	bl VramBlock_LoadCached
	ldr r2, [sp, #48]
	movs r3, #208
	movs r4, #128
	str r0, [sp, #24]
	movs r1, #0
	lsls r3, r3, #5
	lsls r4, r4, #5
	mov r10, r1
	adds r6, r2, r3
	adds r5, r2, r4
.L_080e840e:
	ldr r0, [sp, #24]
	movs r1, #4
	str r0, [sp, #0]
	movs r2, #4
	movs r3, #0
	adds r0, r5, #0
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	mov r2, r10
	movs r3, #240
	strh r3, [r5, #30]
	negs r3, r2
	cmp r3, #0
	bge .L_080e8444
	adds r3, #3
.L_080e8444:
	asrs r3, r3, #2
	str r3, [r6, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #40
	adds r6, #28
	cmp r4, #63
	ble .L_080e840e
	ldr r0, [sp, #48]
	movs r1, #164
	lsls r1, r1, #6
	movs r2, #164
	adds r1, #132
	lsls r2, r2, #6
	adds r3, r0, r1
	adds r2, #133
	movs r1, #0
	strb r1, [r3]
	adds r3, r0, r2
	strb r1, [r3]
	movs r4, #164
	movs r3, #164
	lsls r3, r3, #6
	lsls r4, r4, #6
	adds r3, #134
	adds r4, #136
	adds r5, r0, r3
	movs r2, #1
	adds r3, r0, r4
	strb r2, [r5]
	strh r1, [r3]
	ldr r0, [sp, #40]
	movs r1, #192
	lsls r1, r1, #4
	movs r4, #192
	adds r1, #172
	lsls r4, r4, #4
	adds r3, r0, r1
	adds r4, #173
	strb r2, [r3]
	adds r3, r0, r4
	strb r2, [r3]
	movs r3, #186
	adds r1, #2
	lsls r3, r3, #2
	adds r2, r0, r1
	adds r3, #255
	ldr r6, .L_080e84d0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080e84dc
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	bl WaitFrames
	strb r6, [r5]
	movs r2, #0
	mov r11, r2
	mov r8, r2
.L_080e84c0:
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080e852e
	ldr r5, [sp, #20]
	movs r4, #7
	mov r10, r4
	b .L_080e84e0
	.2byte 0x0000
.L_080e84d0:
	.4byte 0x00000000
.L_080e84d4:
	.4byte 0x84000800
.L_080e84d8:
	.4byte 0x000001eb
.L_080e84dc:
	.4byte Func_080e807c
.L_080e84e0:
	movs r1, #208
	adds r2, r5, #0
	mov r0, r9
	lsls r1, r1, #14
	movs r3, #243
	bl Func_080e7f94
	movs r1, #212
	adds r2, r5, #0
	mov r0, r9
	lsls r1, r1, #14
	movs r3, #247
	bl Func_080e7f94
	movs r1, #216
	adds r2, r5, #0
	mov r0, r9
	lsls r1, r1, #14
	movs r3, #251
	bl Func_080e7f94
	movs r1, #220
	mov r0, r9
	lsls r1, r1, #14
	adds r2, r5, #0
	movs r3, #255
	bl Func_080e7f94
	movs r0, #1
	negs r0, r0
	add r10, r0
	mov r1, r10
	adds r5, #128
	cmp r1, #0
	bge .L_080e84e0
	ldr r2, [sp, #32]
	movs r3, #11
	adds r2, #193
	strb r3, [r2]
.L_080e852e:
	ldr r3, [sp, #48]
	movs r4, #208
	movs r0, #128
	movs r2, #0
	lsls r4, r4, #5
	lsls r0, r0, #5
	mov r10, r2
	adds r6, r3, r4
	adds r7, r3, r0
.L_080e8540:
	ldr r1, [r6, #24]
	cmp r1, #0
	bne .L_080e858a
	ldr r4, [sp, #36]
	ldr r3, [sp, #16]
	mov r2, r10
	movs r1, #1
	ands r1, r2
	lsls r1, r1, #15
	adds r1, r3, r1
	ldr r3, [r4, #8]
	movs r0, #128
	str r3, [r6]
	lsls r0, r0, #13
	ldr r3, [r4, #12]
	adds r2, r6, #0
	adds r3, r3, r0
	str r3, [r6, #4]
	movs r0, #220
	ldr r3, [r4, #16]
	lsls r0, r0, #14
	str r3, [r6, #8]
	bl Vector_AddPolarOffset
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	bl Random16
	lsls r5, r5, #1
	adds r1, r0, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	ldr r1, [r6, #24]
.L_080e858a:
	cmp r1, #15
	bhi .L_080e85c8
	ldr r2, [sp, #24]
	movs r3, #3
	ands r1, r3
	ldr r3, .L_080e85bc
	lsls r1, r1, #1
	adds r1, r2, r1
	ands r1, r3
	ldr r2, .L_080e85c0
	ldrh r3, [r7, #8]
	adds r0, r7, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	adds r1, r6, #0
	bl Func_080eb298
	ldr r3, [r6, #4]
	ldr r4, .L_080e85c4
	ldr r1, [r6, #24]
	adds r3, r3, r4
	str r3, [r6, #4]
	b .L_080e85c8
	.2byte 0x0000
.L_080e85bc:
	.4byte 0x000003ff
.L_080e85c0:
	.4byte 0xfffffc00
.L_080e85c4:
	.4byte 0xffff0000
.L_080e85c8:
	adds r1, #1
	str r1, [r6, #24]
	ldr r0, [sp, #12]
	cmp r0, #0
	beq .L_080e85da
	cmp r1, #16
	bne .L_080e85da
	movs r3, #0
	str r3, [r6, #24]
.L_080e85da:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r7, #40
	adds r6, #28
	cmp r2, #63
	ble .L_080e8540
	mov r3, r11
	cmp r3, #1
	beq .L_080e8618
	cmp r3, #1
	bgt .L_080e85f8
	cmp r3, #0
	beq .L_080e8600
	b .L_080e8646
.L_080e85f8:
	mov r4, r11
	cmp r4, #2
	beq .L_080e8634
	b .L_080e8646
.L_080e8600:
	mov r0, r8
	lsls r0, r0, #10
	mov r1, r8
	str r0, [sp, #16]
	cmp r1, #32
	bne .L_080e8646
	movs r3, #1
	movs r2, #1
	negs r3, r3
	mov r11, r2
	mov r8, r3
	b .L_080e8646
.L_080e8618:
	mov r0, r8
	lsls r3, r0, #10
	movs r4, #1
	str r4, [sp, #8]
	str r3, [sp, #20]
	str r3, [sp, #16]
	cmp r0, #32
	bne .L_080e8646
	movs r2, #1
	movs r1, #2
	negs r2, r2
	mov r11, r1
	mov r8, r2
	b .L_080e8646
.L_080e8634:
	movs r3, #0
	mov r4, r8
	str r3, [sp, #12]
	cmp r4, #16
	bne .L_080e8646
	movs r0, #186
	lsls r0, r0, #2
	adds r0, #255
	mov r11, r0
.L_080e8646:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	movs r1, #1
	adds r2, #255
	add r8, r1
	cmp r11, r2
	beq .L_080e865c
	b .L_080e84c0
.L_080e865c:
	ldr r0, .L_080e87b4
	bl Resource_GetTableEntry
	mov r1, r9
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	mov r0, r9
	lsls r1, r1, #19
	ldr r2, .L_080e87b8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_080e87bc
	bl Resource_GetTableEntry
	ldr r1, [sp, #48]
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #2
	ldr r2, [sp, #48]
	adds r5, r0, #0
	bl VramBlock_LoadCached
	ldr r4, [sp, #48]
	mov r9, r0
	movs r1, #164
	movs r0, #166
	lsls r0, r0, #6
	lsls r1, r1, #6
	adds r3, r4, r0
	adds r1, #130
	strh r5, [r3]
	mov r2, r9
	adds r3, r4, r1
	adds r0, #4
	strh r2, [r3]
	adds r1, #3
	adds r3, r4, r0
	movs r2, #1
	strb r2, [r3]
	adds r3, r4, r1
	strb r2, [r3]
	ldr r0, [sp, #4]
	movs r2, #0
	movs r3, #152
	ldr r1, [sp, #48]
	mov r10, r2
	lsls r3, r3, #6
	movs r2, #132
	adds r6, r4, r3
	lsls r2, r2, #6
	movs r4, #240
	mov r11, r4
	mov r8, r0
	adds r7, r1, r2
.L_080e86d6:
	bl Random16
	mov r3, r9
	str r3, [sp, #0]
	movs r3, #128
	adds r5, r0, #0
	movs r1, #8
	adds r0, r7, #0
	movs r2, #8
	lsls r3, r3, #23
	bl Func_080eaf98
	ldrb r1, [r7, #9]
	movs r0, #13
	negs r0, r0
	adds r3, r0, #0
	mov r4, r11
	strh r4, [r7, #30]
	ands r1, r3
	mov r0, r11
	ldrb r3, [r7, #5]
	movs r4, #33
	orrs r1, r0
	negs r4, r4
	strb r1, [r7, #9]
	adds r2, r4, #0
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #7
	strb r3, [r7, #5]
	adds r3, r5, r1
	strh r3, [r7, #28]
	ldr r3, [sp, #44]
	mov r4, r8
	ldr r2, [r3, #16]
	movs r0, #128
	ldr r3, [r2, #8]
	lsls r0, r0, #13
	str r3, [r4]
	adds r7, #40
	ldr r3, [r2, #12]
	adds r3, r3, r0
	str r3, [r4, #4]
	mov r0, r8
	ldr r3, [r2, #16]
	str r3, [r4, #8]
	bl Camera_WorldToScreen
	movs r0, #128
	adds r1, r5, #0
	mov r2, r8
	lsls r0, r0, #15
	bl Vector_AddPolarOffset
	mov r1, r8
	ldr r3, [r1]
	mov r2, r10
	str r3, [r6]
	ldr r3, [r1, #8]
	str r3, [r6, #4]
	negs r3, r2
	str r3, [r6, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r6, #28
	cmp r4, #31
	ble .L_080e86d6
	ldr r0, [sp, #28]
	bl Resource_ResetEntry
	bl BattleFx_PrepareBufferInterpolation
.L_080e8768:
	ldr r0, [sp, #48]
	movs r1, #164
	lsls r1, r1, #6
	adds r1, #134
	adds r3, r0, r1
	movs r2, #1
	strb r2, [r3]
	ldr r4, [sp, #40]
	movs r0, #192
	lsls r0, r0, #4
	movs r1, #192
	adds r0, #172
	lsls r1, r1, #4
	adds r3, r4, r0
	adds r1, #173
	strb r2, [r3]
	adds r3, r4, r1
	strb r2, [r3]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #174
	adds r2, r4, r3
	movs r3, #150
	lsls r3, r3, #2
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080e87c0
	bl Scheduler_AddOrUpdateCallback
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e87b4:
	.4byte 0x000001db
.L_080e87b8:
	.4byte 0x84000800
.L_080e87bc:
	.4byte 0x000001ed
.L_080e87c0:
	.4byte Func_080e807c
