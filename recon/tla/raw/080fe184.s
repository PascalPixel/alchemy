.syntax unified
	.thumb
	.global Func_080fe184
	.thumb_func
Func_080fe184:
	push {r5, r6, r7, lr}
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #236
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlock
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #24]
	movs r3, #1
	adds r7, r0, #0
	movs r1, #0
	strh r3, [r2, #4]
	movs r0, #0
	movs r3, #20
	movs r2, #30
	bl UiWindow_DrawFrameFar
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	bl UiWindow_InitializeWork
	movs r2, #129
	lsls r2, r2, #2
	adds r0, r7, r2
	bl Party_ListActiveOwnersFar
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	strb r0, [r3]
	movs r1, #3
	movs r0, #0
	movs r2, #0
	movs r3, #7
	bl Func_080fee04
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
	ldr r1, .L_080fe1fc
	movs r0, #151
	lsls r0, r0, #1
	movs r2, #3
	adds r3, r7, r0
	b .L_080fe200
	.2byte 0x0000
.L_080fe1fc:
	.4byte 0x0000001a
.L_080fe200:
	subs r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #0
	bge .L_080fe200
	movs r3, #135
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #3
	strh r3, [r2]
	bl Func_080fe240
	adds r6, r0, #0
	ldr r0, [r7, #40]
	bl RenderOutput_ClearListFar
	bl Func_080fa478
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	movs r5, #0
	strh r5, [r3, #4]
	movs r0, #1
	bl WaitFrames
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	adds r0, r6, #0
	add sp, #4
	pop {r5, r6, r7, pc}
