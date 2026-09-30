.syntax unified
	.thumb
	.global Func_080f8ce8
	.thumb_func
Func_080f8ce8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r9, r2
	ldr r2, [r3, #20]
	mov r8, r3
	movs r3, #13
	strb r3, [r2, #5]
	movs r2, #1
	negs r2, r2
	sub sp, #24
	adds r7, r0, #0
	mov r10, r1
	cmp r9, r2
	beq .L_080f8d54
	add r0, sp, #8
	add r1, sp, #20
	add r2, sp, #16
	add r3, sp, #12
	str r0, [sp, #0]
	adds r0, r7, #0
	bl Func_08038108
	ldr r2, [sp, #8]
	mov r5, r8
	str r2, [sp, #0]
	movs r2, #129
	lsls r2, r2, #1
	adds r5, #64
	str r2, [sp, #4]
	ldr r3, [sp, #12]
	adds r0, r5, #0
	mov r1, r10
	mov r2, r9
	bl UiWindow_UpdateOrCreate
	cmp r0, #0
	bne .L_080f8d50
	ldr r2, [sp, #8]
	ldr r0, [r5]
	ldr r3, [sp, #12]
	str r2, [sp, #0]
	mov r1, r10
	mov r2, r9
	bl UiWindow_SetBounds
.L_080f8d50:
	ldr r6, [r5]
	b .L_080f8d58
.L_080f8d54:
	mov r3, r8
	ldr r6, [r3, #48]
.L_080f8d58:
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r0, r6, #0
	bl RenderOutput_ClearListFar
	movs r2, #1
	negs r2, r2
	cmp r9, r2
	bne .L_080f8d7a
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080f8d86
.L_080f8d7a:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResourceFar
.L_080f8d86:
	movs r3, #1
	negs r3, r3
	cmp r10, r3
	beq .L_080f8dc8
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_080f8dc4
	movs r7, #1
.L_080f8d98:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #4]
	ands r3, r7
	cmp r3, #0
	bne .L_080f8dba
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_080f8dba
	ldr r3, [r5, #4]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080f8d98
.L_080f8dba:
	adds r0, r6, #0
	bl RenderOutput_ClearListFar
	b .L_080f8dd0
	.2byte 0x0000
.L_080f8dc4:
	.4byte gInput
.L_080f8dc8:
	movs r0, #82
	adds r0, #255
	bl GameFlag_SetBit
.L_080f8dd0:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #30
	add r2, r8
	movs r3, #1
	strh r3, [r2]
	mov r2, r8
	ldr r3, [r2, #20]
	ldr r1, .L_080f8df8
	strb r1, [r3, #5]
	movs r3, #1
	negs r3, r3
	cmp r9, r3
	beq .L_080f8dfc
	mov r0, r8
	adds r0, #64
	movs r1, #1
	bl UiWindow_CloseIfOpen
	b .L_080f8dfc
.L_080f8df8:
	.4byte 0x00000001
.L_080f8dfc:
	add sp, #24
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
