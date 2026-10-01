.syntax unified
	.thumb
	.global Func_080e74d8
	.thumb_func
Func_080e74d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #240
	lsls r1, r1, #5
	adds r1, #144
	movs r0, #92
	sub sp, #24
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #20]
	bl Func_080cdf5c
	mov r9, r0
	bl Func_080cdf5c
	bl ObjectTable_Get
	adds r7, r0, #0
	movs r0, #20
	bl WaitFrames
	bl BattleEffect_InitializeSharedScene
	ldr r0, .L_080e75d4
	bl Resource_GetTableEntry
	ldr r1, [sp, #20]
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #3
	adds r1, r5, #0
	ldr r2, [sp, #20]
	str r0, [sp, #12]
	bl VramBlock_LoadCached
	ldr r2, [sp, #20]
	movs r3, #239
	movs r1, #0
	lsls r3, r3, #4
	mov r8, r0
	mov r10, r1
	adds r5, r2, r5
	movs r4, #15
	adds r6, r2, r3
.L_080e7540:
	mov r3, r10
	ands r3, r4
	lsls r3, r3, #1
	add r3, r8
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	str r4, [sp, #4]
	bl Func_080eaf98
	ldrb r3, [r6, #5]
	movs r2, #32
	ldr r4, [sp, #4]
	orrs r3, r2
	ldrb r2, [r6, #9]
	movs r1, #13
	strb r3, [r6, #5]
	negs r1, r1
	adds r3, r4, #0
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r6, #9]
	movs r2, #1
	movs r3, #240
	strh r3, [r6, #30]
	add r10, r2
	subs r3, #241
	str r3, [r5, #24]
	mov r3, r10
	adds r6, #40
	adds r5, #28
	cmp r3, #99
	ble .L_080e7540
	movs r0, #10
	bl WaitFrames
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	mov r0, r9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl WaitFrames
	movs r1, #28
	adds r0, r7, #0
	bl Object_SetMode
	ldr r3, .L_080e75d8
	movs r0, #131
	str r3, [r7, #108]
	bl Audio_PlayCue
	movs r0, #30
	bl WaitFrames
	movs r0, #220
	bl Audio_PlayCue
	ldr r6, .L_080e75d0
	movs r1, #3
	adds r0, r7, #0
	bl Object_SetMode
	movs r1, #49
	movs r5, #0
	mov r9, r1
	b .L_080e75dc
.L_080e75d0:
	.4byte 0x00001000
.L_080e75d4:
	.4byte 0x000001ef
.L_080e75d8:
	.4byte ObjectGroup_ApplyRandomChildValues
.L_080e75dc:
	ldrh r3, [r7, #6]
	movs r0, #1
	adds r3, r3, r5
	adds r3, r3, r6
	strh r3, [r7, #6]
	bl WaitFrames
	movs r2, #1
	negs r2, r2
	add r9, r2
	mov r3, r9
	adds r5, #60
	cmp r3, #0
	bge .L_080e75dc
	movs r2, #0
	movs r1, #2
	str r1, [sp, #8]
	str r2, [sp, #16]
	mov r11, r2
	mov r9, r2
.L_080e7604:
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080e76bc
	mov r10, r3
.L_080e760c:
	bl Random16
	mov r1, r11
	ldr r2, [sp, #20]
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r3, r3, #2
	adds r6, r2, r3
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r6, r3
	movs r1, #0
	str r1, [r5, #24]
	ldr r3, [r7, #8]
	mov r8, r0
	str r3, [r5]
	bl Random16
	ldr r2, [r7, #12]
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16
	adds r2, r5, #0
	lsls r0, r0, #3
	mov r1, r8
	bl Vector_AddPolarOffset
	movs r2, #1
	mov r3, r11
	ands r2, r3
	cmp r2, #0
	beq .L_080e767e
	ldr r3, .L_080e77fc
	movs r1, #0
	str r1, [r5, #12]
	str r3, [r5, #16]
	str r1, [r5, #20]
	bl Random16
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #3
	lsls r2, r2, #11
	lsls r0, r0, #2
	adds r3, #12
	adds r0, r0, r2
	mov r1, r8
	adds r2, r6, r3
	bl Vector_AddPolarOffset
	b .L_080e76a2
.L_080e767e:
	movs r3, #160
	lsls r3, r3, #11
	str r2, [r5, #12]
	str r3, [r5, #16]
	str r2, [r5, #20]
	bl Random16
	movs r3, #128
	movs r1, #128
	lsls r3, r3, #3
	lsls r1, r1, #11
	lsls r0, r0, #2
	adds r3, #12
	adds r0, r0, r1
	adds r2, r6, r3
	mov r1, r8
	bl Vector_AddPolarOffset
.L_080e76a2:
	movs r1, #1
	add r11, r1
	mov r0, r11
	movs r1, #100
	bl Math_Mod
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	mov r11, r0
	cmp r3, #0
	bne .L_080e760c
.L_080e76bc:
	movs r1, #0
	ldr r2, [sp, #20]
	mov r10, r1
	movs r3, #128
	movs r1, #239
	lsls r3, r3, #3
	lsls r1, r1, #4
	adds r5, r2, r3
	adds r6, r2, r1
.L_080e76ce:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080e770e
	cmp r3, #19
	bhi .L_080e7700
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080eb298
	movs r3, #1
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080e76f4
	movs r2, #128
	adds r0, r5, #0
	movs r1, #63
	lsls r2, r2, #8
	b .L_080e76fa
.L_080e76f4:
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_080e7800
.L_080e76fa:
	bl BattleFx_IntegrateVector3
	ldr r3, [r5, #24]
.L_080e7700:
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #20
	bne .L_080e770e
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080e770e:
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r6, #40
	adds r5, #28
	cmp r1, #99
	ble .L_080e76ce
	ldr r2, [sp, #16]
	cmp r2, #1
	beq .L_080e7774
	cmp r2, #1
	bgt .L_080e772c
	cmp r2, #0
	beq .L_080e7734
	b .L_080e77c8
.L_080e772c:
	ldr r3, [sp, #16]
	cmp r3, #2
	beq .L_080e77b6
	b .L_080e77c8
.L_080e7734:
	mov r1, r9
	cmp r1, #10
	bne .L_080e7740
	movs r0, #198
	bl Audio_PlayCue
.L_080e7740:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	mov r3, r9
	cmp r3, #40
	bne .L_080e7754
	movs r1, #3
	str r1, [sp, #8]
.L_080e7754:
	mov r2, r9
	cmp r2, #50
	bne .L_080e775e
	movs r3, #4
	str r3, [sp, #8]
.L_080e775e:
	mov r1, r9
	cmp r1, #60
	bne .L_080e77c8
	movs r1, #1
	movs r2, #5
	movs r3, #1
	negs r1, r1
	str r2, [sp, #8]
	str r3, [sp, #16]
	mov r9, r1
	b .L_080e77c8
.L_080e7774:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	mov r3, r9
	lsls r0, r3, #16
	movs r1, #30
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #9
	subs r3, r3, r0
	movs r5, #0
	str r3, [r7, #24]
	cmp r3, #0
	bge .L_080e7798
	str r5, [r7, #24]
.L_080e7798:
	ldr r3, [r7, #24]
	movs r1, #0
	str r3, [r7, #28]
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26Far
	mov r1, r9
	cmp r1, #30
	bne .L_080e77c8
	movs r3, #1
	movs r2, #2
	negs r3, r3
	str r2, [sp, #16]
	mov r9, r3
	b .L_080e77c8
.L_080e77b6:
	movs r1, #0
	mov r2, r9
	str r1, [sp, #8]
	cmp r2, #20
	bne .L_080e77c8
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	str r3, [sp, #16]
.L_080e77c8:
	movs r0, #1
	bl WaitFrames
	movs r3, #186
	ldr r2, [sp, #16]
	lsls r3, r3, #2
	movs r1, #1
	adds r3, #255
	add r9, r1
	cmp r2, r3
	beq .L_080e77e0
	b .L_080e7604
.L_080e77e0:
	ldr r0, [sp, #12]
	bl Resource_ResetEntry
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e77fc:
	.4byte 0xfffc0000
.L_080e7800:
	.4byte 0xffff6000
