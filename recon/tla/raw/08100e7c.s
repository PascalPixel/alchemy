.syntax unified
	.thumb
	.global Func_08100e7c
	.thumb_func
Func_08100e7c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #236
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_08100f34
	movs r2, #139
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrb r2, [r3]
	movs r1, #0
	mov r9, r2
	movs r2, #2
	strb r2, [r3]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	movs r2, #1
	mov r10, r2
	mov r2, r10
	strh r2, [r3, #4]
	adds r7, r0, #0
	movs r2, #30
	movs r3, #20
	movs r0, #0
	bl UiWindow_DrawFrameFar
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	bl UiWindow_InitializeWork
	movs r0, #132
	lsls r0, r0, #6
	adds r0, #48
	bl Runtime_BumpAllocateAlternatePool
	movs r2, #192
	lsls r2, r2, #1
	adds r3, r7, r2
	str r0, [r3]
	movs r6, #132
	movs r3, #132
	lsls r3, r3, #6
	lsls r6, r6, #6
	mov r8, r0
	adds r3, #40
	adds r6, #44
	movs r5, #0
	add r3, r8
	add r6, r8
	movs r0, #183
	str r5, [r3]
	lsls r0, r0, #1
	str r5, [r6]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08100f3c
	movs r0, #112
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08100f22
	movs r0, #114
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08100f1e
	mov r3, r10
	b .L_08100f3a
.L_08100f1e:
	movs r3, #14
	b .L_08100f3a
.L_08100f22:
	movs r0, #114
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_08100f38
	movs r3, #27
	b .L_08100f3a
	.2byte 0x0000
.L_08100f34:
	.4byte gPartyState
.L_08100f38:
	movs r3, #28
.L_08100f3a:
	str r3, [r6]
.L_08100f3c:
	bl Func_080f80c4
	movs r0, #1
	bl Func_080383c0
	ldr r0, .L_08100fd4
	bl Link_DrawShiftedTilePairFar
	movs r2, #129
	lsls r2, r2, #2
	adds r0, r7, r2
	bl Party_ListActiveOwnersFar
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	strb r0, [r3]
	bl Func_08104a58
	movs r1, #3
	movs r2, #0
	movs r3, #7
	movs r0, #0
	bl Func_080fa368
	movs r0, #0
	bl Func_08100e34
	movs r0, #14
	bl Func_080f9108
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #17
	movs r3, #5
	movs r0, #13
	bl UiWindow_CreateFar
	adds r3, r7, #0
	adds r3, #240
	str r0, [r3]
	ldr r2, .L_08100fd0
	movs r3, #182
	lsls r3, r3, #1
	adds r1, r7, r3
	movs r3, #255
	strh r3, [r1]
	strb r2, [r7, #28]
	strb r2, [r7, #29]
	movs r2, #180
	lsls r2, r2, #1
	movs r5, #0
	adds r3, r7, r2
	adds r2, #2
	strh r5, [r3]
	adds r3, r7, r2
	strh r5, [r3]
	bl Func_081051a8
	bl Func_0810106c
	bl Func_0810526c
	bl Func_08104aa4
	movs r0, #1
	bl WaitFrames
	bl Func_080fa478
	movs r1, #0
	b .L_08100fd8
.L_08100fd0:
	.4byte 0x00000000
.L_08100fd4:
	.4byte 0x06002500
.L_08100fd8:
	movs r2, #30
	movs r0, #0
	movs r3, #20
	bl UiWindow_DrawFrameFar
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	strh r5, [r3, #4]
	bl Func_08038290
	movs r0, #0
	bl Func_080383c0
	mov r1, r8
	movs r2, #128
	ldr r5, .L_08101060
	adds r1, #168
	lsls r2, r2, #6
	ldr r0, .L_08101064
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #160
	adds r1, #168
	lsls r0, r0, #19
	add r1, r8
	movs r2, #128
	adds r0, #128
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	bl Scheduler_DisableOverlayCallbacksWithFlags
	movs r1, #0
	movs r0, #0
	movs r2, #30
	movs r3, #20
	bl UiWindow_EraseBorderRectFar
	movs r2, #192
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	bl Sys_Free
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	bl Event_ClearInvalidPackedValuesFar
	ldr r3, .L_08101068
	movs r2, #139
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r2, r9
	movs r0, #1
	strb r2, [r3]
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08101060:
	.4byte IwramCopyWords
.L_08101064:
	.4byte 0x06004000
.L_08101068:
	.4byte gPartyState
