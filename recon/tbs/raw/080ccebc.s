.syntax unified
	.thumb
	.global Unnamed_080ccebc
	.thumb_func
Unnamed_080ccebc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_080ccf2c
	mov r8, r1
	mov r3, r8
	subs r3, #4
	ldr r7, [r3]
	ldr r3, .L_080ccf30
	ldr r2, [r1]
	adds r5, r7, r3
	str r0, [r5]
	movs r0, #2
	sub sp, #32
	mov r11, r2
	bl BattleFx_BeginCanvasLayer
	ldr r1, .L_080ccf34
	ldr r2, .L_080ccf38
	ldr r3, .L_080ccf24
	mov r10, r1
	strh r3, [r2]
	ldr r3, .L_080ccf28
	mov r2, r10
	strh r3, [r2]
	ldr r3, [r5]
	add r6, sp, #20
	movs r1, #36
	ldrsh r0, [r3, r1]
	adds r1, r6, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r2, [r5]
	ldr r3, [r2, #20]
	lsls r3, r3, #1
	add r5, sp, #8
	adds r3, #34
	ldrsh r0, [r2, r3]
	adds r1, r5, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r1, [r6]
	ldr r3, [r5]
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	b .L_080ccf3c
.L_080ccf24:
	.4byte 0x00000100
.L_080ccf28:
	.4byte 0x00001000
.L_080ccf2c:
	.4byte Data_03001ef0
.L_080ccf30:
	.4byte 0x00007828
.L_080ccf34:
	.4byte 0x04000052
.L_080ccf38:
	.4byte 0x04000020
.L_080ccf3c:
	adds r1, r1, r3
	movs r3, #64
	ldr r2, .L_080ccfb4
	subs r3, r3, r1
	lsls r3, r3, #8
	str r1, [r6]
	ldr r0, .L_080ccfb8
	str r3, [r2]
	adds r1, r7, #0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #239
	lsls r3, r3, #7
	ldr r1, .L_080ccfbc
	adds r2, r7, r3
	movs r3, #1
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #0
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080ccfc0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #143
	bl Func_080f9010
	movs r2, #0
	movs r3, #1
	movs r1, #32
	mov r9, r2
	mov r10, r3
	mov r8, r1
.L_080ccf84:
	mov r2, r9
	cmp r2, #8
	bgt .L_080ccf94
	lsls r3, r2, #1
	ldr r2, .L_080ccfac
	ldr r1, .L_080ccfc4
	orrs r3, r2
	strh r3, [r1]
.L_080ccf94:
	mov r2, r9
	cmp r2, #53
	ble .L_080ccfc8
	lsls r3, r2, #1
	ldr r2, .L_080ccfb0
	subs r2, r2, r3
	ldr r3, .L_080ccfac
	orrs r2, r3
	ldr r3, .L_080ccfc4
	strh r2, [r3]
	b .L_080ccfc8
	.2byte 0x0000
.L_080ccfac:
	.4byte 0x00001000
.L_080ccfb0:
	.4byte 0x0000007c
.L_080ccfb4:
	.4byte 0x04000028
.L_080ccfb8:
	.4byte 0x00000059
.L_080ccfbc:
	.4byte 0x00007784
.L_080ccfc0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080ccfc4:
	.4byte 0x04000052
.L_080ccfc8:
	mov r1, r10
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, .L_080cd0f4
	adds r1, r7, #0
	ldr r4, [r3]
	movs r2, #33
	movs r3, #41
	mov r0, r11
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	mov r1, r10
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #7
	movs r0, #46
	bl Unnamed_080ed408
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, .L_080cd0f4
	adds r1, r7, #0
	ldr r4, [r3]
	movs r2, #64
	movs r3, #41
	mov r0, r11
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	mov r1, r10
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #11
	movs r0, #46
	bl Unnamed_080ed408
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, .L_080cd0f4
	adds r1, r7, #0
	ldr r4, [r3]
	movs r2, #33
	movs r3, #72
	mov r0, r11
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	mov r1, r10
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #15
	movs r0, #46
	bl Unnamed_080ed408
	mov r2, r8
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, .L_080cd0f4
	adds r1, r7, #0
	ldr r4, [r3]
	movs r2, #64
	movs r3, #72
	mov r0, r11
	bl _call_via_r4
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	mov r1, r9
	cmp r1, #32
	bne .L_080cd084
	movs r0, #143
	bl BattleEventRuntime_BeginPhaseFar
.L_080cd084:
	ldr r2, .L_080cd0f8
	ldr r3, [r7, r2]
	ldr r3, [r3, #20]
	movs r5, #0
	cmp r3, #0
	beq .L_080cd0ba
	movs r6, #36
.L_080cd092:
	mov r3, r9
	cmp r3, #10
	bne .L_080cd0ac
	ldr r3, [r7, r2]
	movs r2, #1
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	adds r3, r5, #0
	bl ObjectGroup_UpdateMembers
.L_080cd0ac:
	ldr r2, .L_080cd0f8
	ldr r3, [r7, r2]
	ldr r3, [r3, #20]
	adds r5, #1
	adds r6, #2
	cmp r5, r3
	bne .L_080cd092
.L_080cd0ba:
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cd0fc
	mov r1, r10
	adds r3, r7, r2
	str r1, [r3]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #63
	beq .L_080cd0d8
	b .L_080ccf84
.L_080cd0d8:
	ldr r0, .L_080cd100
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080cd0f4:
	.4byte Data_03001f08
.L_080cd0f8:
	.4byte 0x00007828
.L_080cd0fc:
	.4byte 0x00007824
.L_080cd100:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
