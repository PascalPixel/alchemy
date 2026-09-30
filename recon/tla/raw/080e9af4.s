.syntax unified
	.thumb
	.global Func_080e9af4
	.thumb_func
Func_080e9af4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #56
	movs r0, #92
	sub sp, #24
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	ldr r2, [r2, #108]
	mov r8, r0
	movs r1, #30
	ldrsh r0, [r3, r1]
	ldr r7, [r3, #16]
	mov r10, r3
	mov r11, r2
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	mov r4, r10
	lsls r0, r0, #23
	movs r3, #30
	ldrsh r1, [r4, r3]
	adds r0, #5
	bl Func_080ce458
	str r0, [sp, #8]
	ldr r0, .L_080e9da8
	bl Resource_GetTableEntry
	mov r1, r8
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	mov r2, r8
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #44
	add r3, r8
	strh r5, [r3]
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #46
	adds r3, #40
	movs r6, #0
	add r2, r8
	add r3, r8
	strh r0, [r2]
	str r6, [r3]
	movs r5, #128
	movs r0, #0
	ldrsh r2, [r2, r0]
	lsls r5, r5, #5
	add r5, r8
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	lsls r3, r3, #24
	movs r1, #15
	movs r2, #15
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r5, #9]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotionFar
	adds r0, r7, #0
	movs r1, #0
	bl Func_080e1420
	movs r1, #5
	adds r0, r7, #0
	bl Object_SetMode
	movs r0, #130
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #48
	add r3, r8
	strh r6, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #50
	add r3, r8
	strh r6, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #52
	add r3, r8
	movs r1, #144
	strh r6, [r3]
	lsls r1, r1, #3
	ldr r0, .L_080e9dac
	bl Scheduler_AddOrUpdateCallback
	adds r3, r7, #0
	movs r1, #0
	adds r3, #34
	mov r9, r1
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	ldrb r2, [r3]
	bl Func_080dbcc0
	mov r6, r11
	movs r4, #0
	add r5, sp, #12
	adds r6, #20
	cmp r0, #0
	bne .L_080e9c20
	movs r2, #1
	mov r9, r2
	b .L_080e9c6e
.L_080e9c1a:
	movs r3, #1
	mov r9, r3
	b .L_080e9c6e
.L_080e9c20:
	ldmia r6!, {r0}
	cmp r0, #0
	beq .L_080e9c68
	ldr r3, [r0]
	cmp r3, #0
	beq .L_080e9c68
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080e9c68
	mov r1, r10
	ldr r3, [r1, #16]
	cmp r0, r3
	beq .L_080e9c68
	ldr r3, [r0, #8]
	adds r2, r0, #0
	str r3, [r5]
	adds r2, #8
	ldr r3, [r0, #20]
	str r4, [sp, #4]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldrh r3, [r0, #32]
	adds r0, r7, #0
	ldrh r1, [r7, #32]
	subs r3, #2
	adds r0, #8
	bl Func_080dbe80
	ldr r4, [sp, #4]
	cmp r0, #0
	bge .L_080e9c1a
.L_080e9c68:
	adds r4, #1
	cmp r4, #79
	ble .L_080e9c20
.L_080e9c6e:
	mov r2, r9
	cmp r2, #0
	beq .L_080e9cf4
	movs r0, #5
	bl WaitFrames
	movs r0, #246
	bl Audio_PlayCue
	movs r6, #128
	lsls r6, r6, #5
	adds r6, #40
	movs r5, #0
	add r6, r8
.L_080e9c8a:
	lsls r3, r5, #11
	str r3, [r6]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #31
	ble .L_080e9c8a
	movs r0, #10
	bl WaitFrames
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080e9cb6
	mov r0, r10
	movs r4, #24
	ldrsh r1, [r0, r4]
	movs r3, #26
	ldrsh r2, [r0, r3]
	ldr r0, [sp, #8]
	bl Func_080ceafc
.L_080e9cb6:
	movs r6, #128
	lsls r6, r6, #5
	adds r6, #40
	movs r5, #31
	add r6, r8
.L_080e9cc0:
	lsls r3, r5, #11
	str r3, [r6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bne .L_080e9cc0
	movs r0, #5
	bl WaitFrames
	ldr r0, .L_080e9dac
	bl Scheduler_RemoveCallback
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #44
	add r3, r8
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Resource_ResetEntry
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	b .L_080e9d9a
.L_080e9cf4:
	movs r0, #85
	adds r0, r0, r7
	mov r1, r9
	strb r1, [r0]
	ldr r3, [r7, #20]
	mov r2, r9
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #40
	add r3, r8
	str r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	movs r2, #208
	adds r3, #70
	lsls r2, r2, #4
	add r3, r11
	mov r4, r9
	adds r2, #68
	strh r4, [r3]
	add r2, r11
	movs r3, #100
	movs r6, #128
	lsls r6, r6, #5
	strh r3, [r2]
	adds r6, #40
	mov r10, r0
	movs r5, #0
	add r6, r8
.L_080e9d30:
	cmp r5, #10
	bne .L_080e9d3a
	movs r0, #246
	bl Audio_PlayCue
.L_080e9d3a:
	ldr r3, [r7, #12]
	movs r0, #128
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r7, #12]
	lsls r3, r5, #11
	str r3, [r6]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #31
	ble .L_080e9d30
	movs r3, #4
	mov r1, r10
	strb r3, [r1]
	ldr r3, [r7, #20]
	movs r2, #146
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #72]
	movs r4, #128
	ldr r3, .L_080e9db0
	lsls r4, r4, #2
	adds r4, #18
	adds r3, r3, r4
	movs r2, #10
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #52
	movs r2, #1
	add r3, r8
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r11
	strh r2, [r3]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #68
	add r2, r11
	movs r3, #112
	strh r3, [r2]
.L_080e9d9a:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e9da8:
	.4byte 0x000001da
.L_080e9dac:
	.4byte Func_080e9940
.L_080e9db0:
	.4byte gPartyState
