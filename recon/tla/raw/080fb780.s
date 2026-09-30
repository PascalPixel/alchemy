.syntax unified
	.thumb
	.global Func_080fb780
	.thumb_func
Func_080fb780:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r5, #13
	mov r8, r3
	movs r3, #1
	mov r10, r3
	movs r3, #134
	lsls r3, r3, #2
	add r3, r8
	ldr r3, [r3]
	sub sp, #4
	strb r5, [r3, #5]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #30
	movs r3, #10
	mov r9, r0
	movs r0, #0
	bl UiWindow_CreateFar
	adds r7, r0, #0
	ldr r0, .L_080fb8a4
	bl Scheduler_RemoveCallback
	movs r3, #184
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	movs r6, #0
	strb r5, [r3, #5]
	bl Palette_CopyObjectBankToBackground14
	movs r0, #1
	bl WaitFrames
	b .L_080fb7fe
.L_080fb7d8:
	ldr r3, [r1, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080fb7e8
	movs r3, #1
	subs r6, #1
	mov r10, r3
.L_080fb7e8:
	ldr r3, [r1, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080fb7f8
	movs r3, #1
	adds r6, #1
	mov r10, r3
.L_080fb7f8:
	movs r0, #1
	bl WaitFrames
.L_080fb7fe:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fb840
	mov r3, r10
	cmp r3, #0
	beq .L_080fb826
	movs r3, #0
	adds r0, r6, #5
	movs r1, #5
	mov r10, r3
	bl Math_Mod
	mov r1, r9
	adds r6, r0, #0
	adds r0, r7, #0
	bl ItemMenu_DrawItemDetails
.L_080fb826:
	ldr r1, .L_080fb8a8
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_080fb840
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fb7d8
	movs r6, #1
	negs r6, r6
.L_080fb840:
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	mov r3, r8
	ldr r0, [r3, #16]
	bl RenderOutput_RedrawSavedRectFar
	mov r3, r8
	ldr r0, [r3, #16]
	movs r3, #3
	str r3, [sp, #0]
	movs r2, #3
	movs r3, #12
	movs r1, #0
	bl UiWindow_DrawDividerLineFar
	movs r0, #14
	bl Func_080f9108
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080fb8a4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #184
	lsls r3, r3, #1
	add r3, r8
	ldr r2, [r3]
	movs r3, #1
	strb r3, [r2, #5]
	movs r0, #13
	movs r1, #0
	movs r2, #17
	movs r3, #10
	bl UiWindow_DrawFrameFar
	adds r0, r6, #0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080fb8a4:
	.4byte Menu_UpdateEntryObjectTransforms
.L_080fb8a8:
	.4byte gInput
