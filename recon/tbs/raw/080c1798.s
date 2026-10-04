@ Uncredited owned-ROM disassembly; ordinary draft: ../en/main/080c1798.c.
.syntax unified
	.text
	.balign 4, 0
	.thumb
	.global BattleFx_PlayUnitElementEffect
	.type BattleFx_PlayUnitElementEffect, %function
	.thumb_func
BattleFx_PlayUnitElementEffect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #520]
	adds r6, r3, #0
	adds r3, r7, #0
	subs r3, #12
	ldr r3, [r3]
	sub sp, #240
	mov r10, r0
	movs r0, #1
	str r3, [sp, #4]
	mov r11, r1
	adds r5, r2, #0
	bl WaitFrames
	movs r1, #201
	ldr r0, [sp, #4]
	lsls r1, r1, #3
	adds r3, r0, r1
	movs r2, #0
	ldrh r1, [r3]
	movs r0, #1
	bl BattlePresentation_ConfigurePaletteFade
	movs r1, #128
	ldr r3, [pc, #476]
	lsls r1, r1, #7
	ldr r0, [pc, #476]
	bl _call_via_r3
	movs r0, #128
	lsls r0, r0, #19
	ldr r1, [pc, #472]
	bl QueueIoWriteDelay2
	ldr r0, [pc, #468]
	ldr r1, [pc, #472]
	bl QueueIoWriteDelay2
	ldr r1, [pc, #468]
	ldr r0, [pc, #472]
	bl QueueIoWriteDelay2
	movs r0, #1
	bl WaitFrames
	movs r2, #240
	ldr r3, [pc, #460]
	strh r2, [r3]
	ldr r2, [pc, #460]
	adds r3, #4
	strh r2, [r3]
	movs r2, #63
	adds r3, #4
	strh r2, [r3]
	movs r2, #17
	adds r3, #2
	strh r2, [r3]
	cmp r5, #0
	bne .L0
	ldr r0, [pc, #444]
	ldr r1, [pc, #444]
	bl QueueIoWriteDelay2
	mov r0, r11
	bl BattleFx_InitializeStarField
	ldr r4, [pc, #436]
	ldr r3, [sp, #4]
	adds r4, r3, r4
	movs r2, #0
	str r4, [sp, #0]
	ldr r7, [pc, #432]
	mov r8, r2
	add r6, sp, #188
	mov r9, r2
.L3:
	ldr r3, [pc, #428]
	mov r0, r8
	adds r3, #156
	ldr r5, [r3]
	cmp r0, #24
	bgt .L1
	movs r2, #128
	mov r1, r9
	ldr r3, [sp, #0]
	lsls r2, r2, #9
	subs r2, r2, r1
	str r2, [r3]
	ldr r1, [pc, #408]
	ldr r4, [sp, #4]
	movs r3, #128
	adds r0, r4, r1
	ldr r1, [pc, #404]
	bl Graphics_ScaleRgb555Clamped
.L1:
	adds r1, r6, #0
	mov r0, r10
	bl BattleMotion_ProjectScaledPosition
	ldr r3, [r6]
	movs r4, #64
	ldr r2, [pc, #388]
	subs r3, r4, r3
	adds r0, r5, r2
	lsls r3, r3, #8
	str r3, [r0]
	ldr r1, [pc, #384]
	ldr r3, [r6, #4]
	adds r2, r5, r1
	subs r3, r4, r3
	ldr r1, [pc, #380]
	lsls r3, r3, #8
	str r3, [r2]
	ldrh r3, [r7]
	adds r4, r3, #0
	strh r7, [r7]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	stmia r3!, {r0}
	strh r2, [r1]
	ldr r2, [pc, #348]
	stmia r3!, {r2}
	ldr r2, [pc, #348]
	str r2, [r3]
.L2:
	strh r4, [r7]
	ldr r3, [pc, #348]
	adds r2, r5, r3
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	ldr r4, [pc, #336]
	add r8, r0
	mov r1, r8
	add r9, r4
	cmp r1, #44
	ble .L3
	mov r0, r11
	bl Graphics_ResetVramBlockAndReleaseHeapBlocks
	b .L4
.L0:
	cmp r5, #1
	bne .L5
	mov r0, r11
	bl BattlePresentation_PrepareSceneFar
	ldr r2, [pc, #288]
	movs r3, #39
	movs r4, #64
	ldr r6, [pc, #260]
	add r7, sp, #176
	mov r9, r2
	mov r8, r3
	mov r11, r4
.L7:
	ldr r0, [pc, #292]
	adds r1, r7, #0
	ldr r5, [r0]
	mov r0, r10
	bl BattleMotion_ProjectScaledPosition
	ldr r3, [r7]
	mov r4, r11
	ldr r2, [pc, #248]
	subs r3, r4, r3
	adds r1, r5, r2
	lsls r3, r3, #8
	str r3, [r1]
	ldr r3, [r7, #4]
	ldr r0, [pc, #240]
	subs r3, r4, r3
	adds r2, r5, r0
	lsls r3, r3, #8
	str r3, [r2]
	ldrh r3, [r6]
	adds r0, r3, #0
	strh r6, [r6]
	mov r3, r9
	ldrh r2, [r3]
	cmp r2, #31
	bgt .L6
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	add r3, r9
	adds r3, #4
	adds r2, #1
	mov r4, r9
	stmia r3!, {r1}
	strh r2, [r4]
	ldr r2, [pc, #208]
	stmia r3!, {r2}
	ldr r2, [pc, #208]
	str r2, [r3]
.L6:
	strh r0, [r6]
	ldr r0, [pc, #204]
	movs r3, #1
	adds r2, r5, r0
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	cmp r2, #0
	bge .L7
	bl BattleFx_ScheduleCallbacksAndReleaseBlocksFar
	b .L4
.L5:
	cmp r5, #2
	bne .L8
	add r0, sp, #92
	movs r3, #0
	str r3, [r0, #28]
	mov r3, r11
	mov r4, r10
	str r3, [r0]
	mov r1, r10
	movs r3, #1
	str r6, [r0, #24]
	str r4, [r0, #8]
	strh r1, [r0, #36]
	str r4, [r0, #12]
	str r3, [r0, #20]
	str r3, [r0, #16]
	bl Func_080c9020
	b .L4
.L8:
	add r0, sp, #8
	movs r3, #0
	str r3, [r0, #28]
	str r3, [r0, #24]
	mov r3, r10
	mov r2, r11
	str r3, [r0, #8]
	mov r4, r10
	str r3, [r0, #12]
	movs r3, #1
	str r2, [r0]
	strh r4, [r0, #36]
	str r3, [r0, #20]
	str r3, [r0, #16]
	bl Func_080c9030
.L4:
	add sp, #240
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte gCameraWork
	.4byte IwramClearWords
	.4byte 0x06004000
	.4byte 0x00003741
	.4byte 0x0400000c
	.4byte 0x00000784
	.4byte 0x00003f44
	.4byte 0x04000050
	.4byte 0x04000040
	.4byte 0x00001088
	.4byte 0x04000052
	.4byte 0x0000100e
	.4byte 0x00000644
	.4byte 0x04000208
	.4byte gWorkSlot
	.4byte 0x00000544
	.4byte 0x050000c0
	.4byte 0x000013c4
	.4byte 0x000013c8
	.4byte gIoWriteQueue
	.4byte 0x04000028
	.4byte 0x84000002
	.4byte 0x000013cc
	.4byte 0x00000444
	.4byte gBattleFxWork
	.size BattleFx_PlayUnitElementEffect, .-BattleFx_PlayUnitElementEffect
