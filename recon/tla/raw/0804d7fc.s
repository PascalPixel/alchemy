.syntax unified
	.thumb
	.global Func_0804d7fc
	.thumb_func
Func_0804d7fc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	movs r3, #1
	mov r2, r8
	ands r2, r3
	mov r8, r2
	sub sp, #4
	bl AffineEffect_InitializeWork
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r7, [r3]
	mov r3, r8
	cmp r3, #0
	bne .L_0804d830
	movs r0, #44
	bl Menu_AppendResourceEntry
	movs r0, #45
	bl Menu_AppendResourceEntry
	b .L_0804d842
.L_0804d830:
	movs r0, #46
	bl Menu_AppendResourceEntry
	movs r0, #47
	bl Menu_AppendResourceEntry
	movs r0, #48
	bl Menu_AppendResourceEntry
.L_0804d842:
	movs r2, #0
	movs r0, #17
	movs r1, #7
	bl Menu_CenterResourceEntries
	mov r2, r8
	cmp r2, #0
	beq .L_0804d8be
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #118
	ldr r0, .L_0804d958
	bl Scheduler_AddOrUpdateCallback
	movs r3, #255
	adds r2, r7, #0
	lsls r3, r3, #8
	adds r2, #150
	adds r3, #255
	strh r3, [r2]
	movs r3, #2
	str r3, [sp, #0]
	mov r10, r3
	movs r1, #0
	movs r2, #14
	movs r3, #4
	movs r0, #8
	bl UiWindow_Create
	ldr r5, .L_0804d95c
	adds r6, r7, #0
	adds r1, r0, #0
	adds r6, #128
	adds r0, r5, #0
	str r1, [r6]
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
	mov r2, r10
	str r2, [sp, #0]
	movs r1, #4
	movs r2, #22
	movs r3, #12
	movs r0, #4
	bl UiWindow_Create
	adds r1, r0, #0
	str r1, [r7, #124]
	adds r0, r5, #1
	movs r2, #0
	movs r3, #4
	adds r5, #2
	bl UiText_DrawResource
	ldr r1, [r7, #124]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawResource
	b .L_0804d908
.L_0804d8be:
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #118
	ldr r0, .L_0804d960
	bl Scheduler_AddOrUpdateCallback
	movs r3, #255
	adds r2, r7, #0
	lsls r3, r3, #8
	adds r2, #150
	adds r3, #255
	strh r3, [r2]
	movs r5, #2
	movs r1, #0
	movs r2, #18
	movs r3, #4
	movs r0, #6
	str r5, [sp, #0]
	bl UiWindow_Create
	adds r6, r7, #0
	adds r1, r0, #0
	adds r6, #128
	str r1, [r6]
	ldr r0, .L_0804d964
	movs r2, #12
	movs r3, #4
	bl UiText_DrawResource
	movs r0, #5
	movs r1, #5
	movs r2, #21
	movs r3, #7
	str r5, [sp, #0]
	bl UiWindow_Create
	str r0, [r7, #124]
.L_0804d908:
	movs r0, #0
	bl Menu_RunResourceSelectionLoop
	mov r3, r8
	adds r5, r0, #0
	cmp r3, #0
	beq .L_0804d91e
	ldr r0, .L_0804d958
	bl Scheduler_RemoveCallback
	b .L_0804d924
.L_0804d91e:
	ldr r0, .L_0804d960
	bl Scheduler_RemoveCallback
.L_0804d924:
	ldr r0, [r6]
	bl RenderOutput_PrepareForRedraw
	ldr r0, [r7, #124]
	bl RenderOutput_PrepareForRedraw
	ldr r0, [r6]
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [r7, #124]
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	bl Menu_EndResourceSelection
	adds r0, r5, #0
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804d958:
	.4byte Menu_DrawModeLabel
.L_0804d95c:
	.4byte 0x00001179
.L_0804d960:
	.4byte Menu_DrawModeIndicator
.L_0804d964:
	.4byte 0x00001178
