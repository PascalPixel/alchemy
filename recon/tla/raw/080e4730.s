.syntax unified
	.thumb
	.global Func_080e4730
	.thumb_func
Func_080e4730:
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
	sub sp, #40
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	ldr r2, [r2, #108]
	adds r3, #224
	ldr r3, [r3]
	str r2, [sp, #36]
	mov r10, r0
	ldr r0, [r3, #16]
	mov r11, r3
	mov r8, r0
	bl Func_080dc954
	ldr r3, .L_080e4a18
	movs r1, #155
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #0
	strb r2, [r3]
	movs r1, #1
	mov r0, r8
	bl Func_080dc164
	ldr r0, .L_080e4a1c
	bl Func_08014644
	ldr r0, .L_080e4a20
	bl Resource_GetTableEntry
	mov r1, r10
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #4
	adds r1, r5, #0
	mov r2, r10
	str r0, [sp, #28]
	bl VramBlock_LoadCached
	movs r6, #144
	str r0, [sp, #24]
	lsls r6, r6, #5
	movs r7, #0
	add r6, r10
	movs r4, #0
	add r5, r10
.L_080e47ac:
	ldr r2, [sp, #36]
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #72
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r0, r5, #0
	str r3, [sp, #0]
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Func_080e45a8
	ldr r4, [sp, #4]
	adds r7, #1
	str r4, [r6, #24]
	adds r5, #40
	subs r4, #2
	adds r6, #28
	cmp r7, #63
	ble .L_080e47ac
	movs r2, #1
	negs r2, r2
	adds r0, r2, #0
	adds r1, r2, #0
	movs r3, #0
	bl Motion_CamBounds
	mov r2, r8
	adds r2, #85
	movs r3, #0
	str r2, [sp, #20]
	strb r3, [r2]
	mov r3, r8
	ldr r2, [r3, #12]
	movs r6, #160
	lsls r6, r6, #12
	adds r2, r2, r6
	ldr r1, [r3, #8]
	mov r0, r8
	ldr r3, [r3, #16]
	bl Object_SetPositionAndResetMotionFar
	ldr r6, .L_080e4a18
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r6, r0
	ldr r1, [r3]
	mov r0, r8
	bl Animation_SetIndexAndInitObjectsFar
	mov r2, r8
	ldr r1, [r2, #80]
	movs r5, #63
	ldrb r2, [r1, #5]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r1, #5]
	mov r3, r8
	ldr r2, [r3, #80]
	movs r0, #197
	ldrb r3, [r2, #7]
	lsls r0, r0, #1
	ands r5, r3
	movs r3, #128
	orrs r5, r3
	strb r5, [r2, #7]
	ldr r5, [sp, #36]
	adds r3, r5, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e484e
	mov r1, r8
	ldr r2, [r1, #80]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r2, #12]
.L_080e484e:
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #7
	strh r3, [r2, #6]
	mov r0, r8
	movs r1, #0
	bl Object_SetMode
	mov r3, r8
	ldr r0, [r3, #80]
	movs r1, #15
	bl Func_08020288
	mov r5, r8
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5, #80]
	movs r1, #0
	bl Func_08020288
	ldr r3, [r5, #80]
	movs r5, #128
	lsls r5, r5, #3
	ldr r0, [r3, #40]
	adds r2, r5, #0
	mov r1, r10
	bl Func_080e43a4
	mov r1, r8
	ldr r0, [r1, #80]
	movs r1, #15
	bl Func_08020288
	bl Resource_FindFreeEntry
	movs r2, #0
	adds r1, r5, #0
	str r0, [sp, #32]
	bl VramBlock_LoadCached
	adds r5, r0, #0
	movs r0, #200
	lsls r0, r0, #5
	adds r1, r5, #0
	add r0, r10
	bl Func_080e4510
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #3
	movs r2, #0
	add r3, r10
	lsls r5, r5, #5
	str r3, [sp, #16]
	str r2, [sp, #8]
	str r5, [sp, #12]
	mov r9, r2
.L_080e48c6:
	mov r5, r9
	cmp r5, #0
	bne .L_080e48d4
	movs r0, #136
	lsls r0, r0, #2
	bl Audio_PlayCue
.L_080e48d4:
	mov r6, r9
	cmp r6, #64
	bgt .L_080e490e
	ldr r0, [sp, #8]
	movs r2, #92
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r1, [sp, #16]
	mov r0, r10
	bl Func_080e446c
	ldr r6, .L_080e4a24
	ldr r5, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #84]
	ldr r0, [sp, #16]
	movs r1, #32
	movs r2, #32
	adds r3, r5, r6
	mov lr, r4
	.2byte 0xf800
	movs r0, #200
	lsls r0, r0, #5
	add r0, r10
	bl Func_080e45a0
.L_080e490e:
	mov r0, r9
	cmp r0, #66
	bne .L_080e49b2
	mov r0, r8
	movs r1, #1
	bl Object_SetMode
	mov r1, r8
	ldr r0, [r1, #80]
	movs r1, #0
	bl Func_08020288
	mov r3, r11
	movs r2, #24
	ldrsh r0, [r3, r2]
	bl Object_GetById
	mov r6, r11
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
	movs r1, #1
	movs r5, #24
	ldrsh r0, [r6, r5]
	bl Object_AttachWorkTargetToObject
	movs r0, #142
	lsls r0, r0, #1
	adds r0, #255
	bl Audio_PlayCue
	ldr r0, [sp, #36]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e496c
	ldr r2, .L_080e4a28
	movs r3, #1
	strb r3, [r2]
	ldr r5, [sp, #20]
	movs r3, #2
	strb r3, [r5]
	b .L_080e4978
.L_080e496c:
	ldr r6, .L_080e4a28
	movs r3, #0
	strb r3, [r6]
	ldr r0, [sp, #20]
	movs r3, #3
	strb r3, [r0]
.L_080e4978:
	ldr r1, [sp, #36]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #72
	adds r3, r1, r2
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	bne .L_080e49b2
	mov r1, r11
	movs r6, #30
	ldrsh r0, [r1, r6]
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #128
	mov r5, r11
	lsls r0, r0, #23
	movs r3, #30
	ldrsh r1, [r5, r3]
	adds r0, #5
	bl Func_080ce458
	movs r2, #1
	movs r6, #24
	ldrsh r1, [r5, r6]
	negs r2, r2
	bl Func_080ceafc
.L_080e49b2:
	movs r6, #144
	movs r5, #128
	lsls r6, r6, #5
	lsls r5, r5, #4
	movs r7, #0
	add r6, r10
	add r5, r10
.L_080e49c0:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r3, [sp, #24]
	adds r7, #1
	bl Func_080e46ac
	adds r5, #40
	adds r6, #28
	cmp r7, #29
	ble .L_080e49c0
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #8]
	movs r1, #1
	add r9, r1
	adds r0, #3
	mov r2, r9
	str r0, [sp, #8]
	cmp r2, #77
	bgt .L_080e49ee
	b .L_080e48c6
.L_080e49ee:
	movs r1, #0
	movs r2, #16
	mov r0, r8
	bl Func_080dc164
	ldr r0, [sp, #32]
	bl Func_08014274
	ldr r0, [sp, #28]
	bl Func_08014274
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e4a18:
	.4byte gPartyState
.L_080e4a1c:
	.4byte Func_080e42d4
.L_080e4a20:
	.4byte 0x000001e1
.L_080e4a24:
	.4byte 0x06010000
.L_080e4a28:
	.4byte Data_02000452
