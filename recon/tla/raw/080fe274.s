.syntax unified
	.thumb
	.global Func_080fe274
	.thumb_func
Func_080fe274:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #236
	movs r0, #220
	sub sp, #12
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	movs r0, #64
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #160
	lsls r0, r0, #6
	bl Runtime_BumpAllocateAlternatePool
	movs r2, #1
	negs r2, r2
	str r2, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #24]
	ldr r5, [r3, #124]
	mov r11, r3
	movs r3, #1
	movs r1, #0
	adds r7, r0, #0
	strh r3, [r2, #4]
	movs r0, #0
	movs r2, #30
	movs r3, #20
	bl UiWindow_DrawFrameFar
	movs r0, #1
	bl WaitFrames
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #54
	adds r5, r5, r2
	ldrh r3, [r5]
	str r3, [sp, #4]
	movs r3, #17
	strh r3, [r5]
	bl Func_080f80c4
	movs r0, #0
	bl UiWindow_InitializeWork
	ldr r3, .L_080fe324
	movs r2, #135
	lsls r2, r2, #2
	adds r2, r2, r6
	strh r3, [r2]
	movs r3, #129
	lsls r3, r3, #2
	adds r0, r6, r3
	mov r9, r2
	bl Party_ListActiveOwnersFar
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r6, r2
	strb r0, [r3]
	movs r1, #3
	movs r0, #0
	movs r2, #0
	movs r3, #7
	bl Func_080fee04
	ldr r3, .L_080fe328
	movs r1, #160
	mov r10, r3
	mov r0, r8
	lsls r1, r1, #19
	movs r2, #64
	mov lr, r10
	.2byte 0xf800
	b .L_080fe32c
	.2byte 0x0000
.L_080fe324:
	.4byte 0x00000000
.L_080fe328:
	.4byte IwramCopyWords
.L_080fe32c:
	movs r2, #160
	ldr r1, .L_080fe3f8
	lsls r2, r2, #6
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	movs r0, #14
	bl Func_080f9108
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080fe3fc
	lsls r1, r1, #19
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080fe400
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080fe3fc
	adds r1, #4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080fe404
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl Func_080383c0
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #13
	movs r3, #5
	movs r1, #0
	movs r2, #17
	bl UiWindow_CreateFar
	adds r3, r6, #0
	adds r3, #240
	str r0, [r3]
	ldr r0, [sp, #8]
	bl Party_SumDjinnCountsFar
	cmp r0, #0
	beq .L_080fe3a8
	bl Func_081051a8
.L_080fe3a8:
	ldr r0, .L_080fe408
	bl Link_DrawShiftedTilePairFar
	bl Func_081053a8
	bl Func_080f9448
	ldr r2, .L_080fe3f4
	mov r3, r9
	strh r2, [r3]
	bl Func_080fe49c
	str r0, [sp, #8]
	bl Func_080f9464
	bl Func_08105468
	ldr r0, [r6, #40]
	bl RenderOutput_ClearListFar
	bl Func_0810526c
	bl Scheduler_DisableOverlayCallbacksWithFlags
	movs r3, #20
	movs r1, #0
	movs r2, #30
	movs r0, #0
	bl UiWindow_DrawFrameFar
	movs r0, #1
	bl WaitFrames
	bl Func_08038290
	movs r0, #0
	b .L_080fe40c
	.2byte 0x0000
.L_080fe3f4:
	.4byte 0x00000000
.L_080fe3f8:
	.4byte 0x06004000
.L_080fe3fc:
	.4byte 0x05000200
.L_080fe400:
	.4byte 0x050001c8
.L_080fe404:
	.4byte 0x050001e8
.L_080fe408:
	.4byte 0x06002500
.L_080fe40c:
	bl Func_080383c0
	movs r0, #160
	mov r1, r8
	movs r2, #64
	lsls r0, r0, #19
	mov lr, r10
	.2byte 0xf800
	movs r2, #160
	adds r1, r7, #0
	lsls r2, r2, #6
	ldr r0, .L_080fe478
	mov lr, r10
	.2byte 0xf800
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	mov r3, r11
	ldr r2, [r3, #60]
	ldr r3, .L_080fe470
	strb r3, [r2, #6]
	bl Func_080fa478
	movs r1, #0
	movs r2, #30
	movs r3, #20
	movs r0, #0
	bl UiWindow_DrawFrameFar
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	mov r2, r11
	ldr r3, [r2, #24]
	ldr r2, .L_080fe474
	movs r0, #1
	strh r2, [r3, #4]
	add r3, sp, #4
	ldrh r3, [r3]
	strh r3, [r5]
	bl WaitFrames
	movs r0, #0
	movs r1, #0
	movs r2, #30
	movs r3, #20
	b .L_080fe47c
.L_080fe470:
	.4byte 0x00000001
.L_080fe474:
	.4byte 0x00000000
.L_080fe478:
	.4byte 0x06004000
.L_080fe47c:
	bl Func_080383f0
	mov r3, r11
	ldr r2, [r3, #60]
	ldr r3, .L_080fe498
	strb r3, [r2, #6]
	ldr r0, [sp, #8]
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fe498:
	.4byte 0x00000000
